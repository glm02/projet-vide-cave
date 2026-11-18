#include <Arduino.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <time.h>

#include "page.h"
#include "telegram_ca.h"

#if __has_include("wifi.local.h")
#include "wifi.local.h"
#else
constexpr char WIFI_SSID[] = "";
constexpr char WIFI_PASSWORD[] = "";
constexpr char TELEGRAM_TOKEN[] = "";
constexpr char TELEGRAM_CHAT_ID[] = "";
#endif

constexpr int SDA_PIN = 25;
constexpr int SCL_PIN = 26;
constexpr int PIC_RX_PIN = 16;
constexpr int PIC_TX_PIN = 17;
constexpr uint8_t SENSOR_ADDRESS = 0x44;
constexpr uint32_t SENSOR_PERIOD_MS = 2000;
constexpr uint32_t SENSOR_READY_MS = 20;
constexpr uint32_t PIC_TIMEOUT_MS = 5000;
constexpr uint32_t PIC_LOST_ALERT_MS = 60000;
constexpr uint32_t SIMULATION_HOLD_MS = 60000;
constexpr uint32_t SAMPLE_PERIOD_MS = 10000;
constexpr uint32_t SAVE_PERIOD_MS = 600000;
constexpr uint32_t BLOCKED_REMINDER_MS = 1800000;
constexpr size_t RECENT_SIZE = 360;
constexpr size_t DAY_SIZE = 1440;
constexpr size_t EVENT_SIZE = 40;
constexpr uint32_t FILE_MAGIC = 0x56430003;
constexpr char TIMEZONE[] = "CET-1CEST,M3.5.0,M10.5.0/3";
constexpr char HOSTNAME[] = "vide-cave";

enum PumpState : uint8_t { ARRET, MARCHE, DEFAUT, INCONNU };
const char *const STATE_NAMES[] = {"ARRET", "MARCHE", "DEFAUT", "INCONNU"};
constexpr int16_t NO_VALUE = INT16_MIN;

struct Point {
  uint32_t time;
  int16_t temperature;
  int16_t humidity;
  uint16_t adc;
  uint8_t state;
  uint8_t reserved;
};

struct Event {
  uint32_t time;
  uint32_t uptimeMs;
  char kind[12];
  char text[64];
};

template <typename T, size_t N>
struct Ring {
  T items[N];
  uint16_t head = 0;
  uint16_t count = 0;
  void push(const T &item) {
    items[head] = item;
    head = (head + 1) % N;
    if (count < N) ++count;
  }
  const T &oldest(size_t i) const { return items[(head + N - count + i) % N]; }
  T &newest(size_t i) { return items[(head + N - 1 - i) % N]; }
};

struct LineReader {
  char line[48];
  size_t length = 0;
  bool overflow = false;
};

HardwareSerial pic(2);
WebServer http(80);
QueueHandle_t telegramQueue;

float temperature = NAN;
float humidity = NAN;
uint32_t lastMeasurement = 0;
bool measurementPending = false;
uint32_t simulatedUntil = 0;

PumpState reportedState = INCONNU;
PumpState shownState = INCONNU;
unsigned int currentAdc = 0;
uint32_t lastPicReport = 0;
bool picSeen = false;
bool picLostAlerted = false;
uint32_t blockedSinceMs = 0;
uint32_t blockedSinceEpoch = 0;
uint32_t lastBlockedReminder = 0;

LineReader fromPic;
LineReader fromUsb;

Ring<Point, RECENT_SIZE> recent;
Ring<Point, DAY_SIZE> day;
Ring<Event, EVENT_SIZE> events;
uint32_t lastSample = 0;
uint32_t lastSave = 0;
uint8_t samplesInMinute = 0;
float sumTemperature = 0, sumHumidity = 0;
uint8_t climateSamples = 0;
uint16_t maxAdc = 0;
uint8_t worstState = INCONNU;
bool clockAnnounced = false;
bool wifiAnnounced = false;
uint16_t eventsThisBoot = 0;

volatile int telegramStatus = 0;

bool clockSynced() { return time(nullptr) > 1700000000; }

uint32_t epochNow() { return clockSynced() ? uint32_t(time(nullptr)) : 0; }

void formatTime(uint32_t epoch, char *out, size_t size, const char *format = "%d/%m %H:%M") {
  if (!epoch) {
    snprintf(out, size, "heure inconnue");
    return;
  }
  time_t t = epoch;
  struct tm local;
  localtime_r(&t, &local);
  strftime(out, size, format, &local);
}

bool telegramConfigured() { return TELEGRAM_TOKEN[0] && TELEGRAM_CHAT_ID[0]; }

void queueTelegram(const char *text) {
  if (!telegramConfigured()) return;
  char message[200];
  strlcpy(message, text, sizeof(message));
  if (xQueueSend(telegramQueue, message, 0) != pdTRUE) Serial.println("Telegram : file pleine, message perdu");
}

void appendJsonString(String &out, const char *text) {
  out += '"';
  for (const char *c = text; *c; ++c) {
    if (*c == '"' || *c == '\\') out += '\\';
    if (*c == '\n') {
      out += "\\n";
      continue;
    }
    if (uint8_t(*c) < 0x20) continue;
    out += *c;
  }
  out += '"';
}

bool sendTelegram(const char *text) {
  WiFiClientSecure client;
  client.setCACert(TELEGRAM_CA);
  client.setTimeout(10);
  HTTPClient request;
  request.setTimeout(10000);
  String url = String("https://api.telegram.org/bot") + TELEGRAM_TOKEN + "/sendMessage";
  if (!request.begin(client, url)) {
    telegramStatus = -1;
    return false;
  }
  request.addHeader("Content-Type", "application/json");
  String body = "{\"chat_id\":";
  appendJsonString(body, TELEGRAM_CHAT_ID);
  body += ",\"text\":";
  appendJsonString(body, text);
  body += "}";
  int code = request.POST(body);
  request.end();
  telegramStatus = code;
  Serial.printf("Telegram : HTTP %d\n", code);
  return code == 200;
}

void telegramTask(void *) {
  char message[200];
  for (;;) {
    if (xQueueReceive(telegramQueue, message, portMAX_DELAY) != pdTRUE) continue;
    for (int attempt = 0; attempt < 5; ++attempt) {
      if (WiFi.status() == WL_CONNECTED && sendTelegram(message)) break;
      vTaskDelay(pdMS_TO_TICKS(15000));
    }
  }
}

void saveEvents() {
  File file = LittleFS.open("/evenements.bin", "w");
  if (!file) return;
  file.write(reinterpret_cast<const uint8_t *>(&FILE_MAGIC), sizeof(FILE_MAGIC));
  file.write(reinterpret_cast<const uint8_t *>(&events), sizeof(events));
  file.close();
}

void saveHistory() {
  File file = LittleFS.open("/historique.bin", "w");
  if (!file) return;
  file.write(reinterpret_cast<const uint8_t *>(&FILE_MAGIC), sizeof(FILE_MAGIC));
  file.write(reinterpret_cast<const uint8_t *>(&day), sizeof(day));
  file.close();
}

template <typename T>
void loadFile(const char *path, T &target) {
  File file = LittleFS.open(path, "r");
  if (!file) return;
  uint32_t magic = 0;
  bool valid = file.read(reinterpret_cast<uint8_t *>(&magic), sizeof(magic)) == sizeof(magic) &&
               magic == FILE_MAGIC && file.size() == sizeof(magic) + sizeof(T);
  if (valid) file.read(reinterpret_cast<uint8_t *>(&target), sizeof(T));
  file.close();
}

void addEvent(const char *kind, const char *text, bool notify) {
  Event event{};
  event.time = epochNow();
  event.uptimeMs = millis();
  if (eventsThisBoot < EVENT_SIZE) ++eventsThisBoot;
  strlcpy(event.kind, kind, sizeof(event.kind));
  strlcpy(event.text, text, sizeof(event.text));
  events.push(event);
  saveEvents();
  Serial.printf("Événement [%s] %s\n", kind, text);
  if (!notify) return;

  char when[20];
  formatTime(event.time, when, sizeof(when), "%d/%m à %H:%M");
  char message[200];
  snprintf(message, sizeof(message), "Vide-cave — %s\n%s", text, when);
  queueTelegram(message);
}

uint8_t crcSht31(const uint8_t *bytes) {
  uint8_t crc = 0xff;
  for (int i = 0; i < 2; ++i) {
    crc ^= bytes[i];
    for (int bit = 0; bit < 8; ++bit) crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : crc << 1;
  }
  return crc;
}

void reportClimate() {
  if (isnan(temperature)) {
    pic.println("TH,ERR");
    return;
  }
  char line[48];
  snprintf(line, sizeof(line), "TH,%.2f,%.2f", temperature, humidity);
  pic.println(line);
}

void readClimate() {
  float t = NAN, h = NAN;
  uint8_t data[6];
  if (Wire.requestFrom(SENSOR_ADDRESS, uint8_t{6}) == 6) {
    for (auto &byte : data) byte = Wire.read();
    if (crcSht31(data) == data[2] && crcSht31(data + 3) == data[5]) {
      uint16_t rawT = (uint16_t(data[0]) << 8) | data[1];
      uint16_t rawH = (uint16_t(data[3]) << 8) | data[4];
      t = -45.0f + 175.0f * rawT / 65535.0f;
      h = 100.0f * rawH / 65535.0f;
    }
  }
  bool simulating = int32_t(simulatedUntil - millis()) > 0;
  if (!isnan(t) || !simulating) {
    temperature = t;
    humidity = h;
  }
}

void updateClimate(uint32_t now) {
  if (measurementPending && now - lastMeasurement >= SENSOR_READY_MS) {
    measurementPending = false;
    readClimate();
    reportClimate();
  }
  if (now - lastMeasurement < SENSOR_PERIOD_MS) return;

  lastMeasurement = now;
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.write(0x24);
  Wire.write(0x00);
  measurementPending = Wire.endTransmission() == 0;
  if (!measurementPending) {
    readClimate();
    reportClimate();
  }
}

bool parseState(const char *name, PumpState &state) {
  for (uint8_t i = 0; i < 3; ++i) {
    if (!strcmp(name, STATE_NAMES[i])) {
      state = PumpState(i);
      return true;
    }
  }
  return false;
}

void parseLine(const char *line, bool fromUsbPort, uint32_t now) {
  char name[10];
  unsigned int adc;
  char trailing;
  PumpState state;
  if (sscanf(line, "PUMP,%9[A-Z],%u%c", name, &adc, &trailing) == 2 && adc <= 4095 && parseState(name, state)) {
    reportedState = state;
    currentAdc = adc;
    lastPicReport = now;
    picSeen = true;
    return;
  }
  float t, h;
  if (fromUsbPort && sscanf(line, "TH,%f,%f%c", &t, &h, &trailing) == 2 && h >= 0 && h <= 100) {
    temperature = t;
    humidity = h;
    simulatedUntil = now + SIMULATION_HOLD_MS;
    return;
  }
  if (fromUsbPort && !strcmp(line, "TEST")) {
    addEvent("test", "Alerte de test demandée depuis l'USB", true);
    return;
  }
  if (fromUsbPort && line[0]) Serial.printf("Trame ignorée : %s\n", line);
}

void receiveFrom(Stream &port, LineReader &reader, bool fromUsbPort, uint32_t now) {
  for (int i = 0; i < 128 && port.available(); ++i) {
    char byte = char(port.read());
    if (byte == '\n') {
      if (!reader.overflow) {
        reader.line[reader.length] = '\0';
        parseLine(reader.line, fromUsbPort, now);
      }
      reader.length = 0;
      reader.overflow = false;
    } else if (byte != '\r') {
      if (reader.length < sizeof(reader.line) - 1) reader.line[reader.length++] = byte;
      else reader.overflow = true;
    }
  }
}

void updatePump(uint32_t now) {
  bool picAlive = picSeen && now - lastPicReport <= PIC_TIMEOUT_MS;
  PumpState state = picAlive ? reportedState : INCONNU;

  if (picSeen && !picAlive && !picLostAlerted && now - lastPicReport > PIC_LOST_ALERT_MS) {
    picLostAlerted = true;
    addEvent("liaison", "Plus aucune trame du PIC depuis 1 minute", true);
  }
  if (picAlive) picLostAlerted = false;

  if (state == shownState) {
    if (state == DEFAUT && now - lastBlockedReminder >= BLOCKED_REMINDER_MS) {
      lastBlockedReminder = now;
      char text[64];
      snprintf(text, sizeof(text), "Moteur toujours bloqué (depuis %lu min)", (now - blockedSinceMs) / 60000);
      addEvent("rappel", text, true);
    }
    return;
  }

  PumpState previous = shownState;
  shownState = state;
  char text[64];
  if (state == DEFAUT) {
    blockedSinceMs = lastBlockedReminder = now;
    blockedSinceEpoch = epochNow();
    snprintf(text, sizeof(text), "MOTEUR BLOQUÉ : pompe en défaut (ADC %u)", currentAdc);
    addEvent("defaut", text, true);
  } else if (previous == DEFAUT && state != INCONNU) {
    blockedSinceEpoch = 0;
    snprintf(text, sizeof(text), "Défaut acquitté, pompe en %s", STATE_NAMES[state]);
    addEvent("retour", text, true);
  } else if (state != INCONNU) {
    snprintf(text, sizeof(text), "Pompe en %s", STATE_NAMES[state]);
    addEvent("etat", text, false);
  } else if (previous != INCONNU) {
    addEvent("liaison", "Trames du PIC interrompues", false);
  }
}

int16_t scaled(float value) { return isnan(value) ? NO_VALUE : int16_t(lroundf(value * 100)); }

uint8_t statePriority(uint8_t state) {
  static const uint8_t priority[] = {1, 2, 3, 0};
  return priority[state];
}

void sampleHistory(uint32_t now) {
  if (now - lastSample < SAMPLE_PERIOD_MS) return;
  lastSample = now;
  uint32_t epoch = epochNow();
  if (!epoch) return;

  Point point{epoch, scaled(temperature), scaled(humidity), uint16_t(shownState == INCONNU ? 0 : currentAdc),
              shownState, 0};
  recent.push(point);

  if (!isnan(temperature)) {
    sumTemperature += temperature;
    sumHumidity += humidity;
    ++climateSamples;
  }
  maxAdc = max<uint16_t>(maxAdc, point.adc);
  if (statePriority(shownState) > statePriority(worstState) || samplesInMinute == 0) worstState = shownState;
  if (++samplesInMinute < 6) return;

  Point minute{epoch, climateSamples ? scaled(sumTemperature / climateSamples) : NO_VALUE,
               climateSamples ? scaled(sumHumidity / climateSamples) : NO_VALUE, maxAdc, worstState, 0};
  day.push(minute);
  samplesInMinute = climateSamples = 0;
  sumTemperature = sumHumidity = 0;
  maxAdc = 0;

  if (now - lastSave >= SAVE_PERIOD_MS) {
    lastSave = now;
    saveHistory();
  }
}

void appendNumber(String &out, int16_t value, bool hundredths) {
  if (value == NO_VALUE) {
    out += "null";
    return;
  }
  if (!hundredths) {
    out += value;
    return;
  }
  char text[12];
  snprintf(text, sizeof(text), "%.2f", value / 100.0f);
  out += text;
}

template <typename R>
void streamPoints(const R &ring, int step) {
  http.setContentLength(CONTENT_LENGTH_UNKNOWN);
  http.send(200, "application/json; charset=utf-8", "");
  String chunk = "{\"pas\":";
  chunk += step;
  chunk += ",\"points\":[";
  for (size_t i = 0; i < ring.count; ++i) {
    const Point &p = ring.oldest(i);
    if (i) chunk += ',';
    chunk += '[';
    chunk += p.time;
    chunk += ',';
    appendNumber(chunk, p.temperature, true);
    chunk += ',';
    appendNumber(chunk, p.humidity, true);
    chunk += ',';
    chunk += p.adc;
    chunk += ',';
    chunk += p.state;
    chunk += ']';
    if (chunk.length() > 1200) {
      http.sendContent(chunk);
      chunk = "";
    }
  }
  chunk += "]}";
  http.sendContent(chunk);
  http.sendContent("");
}

void handleHistory() {
  if (http.arg("plage") == "24h") streamPoints(day, 60);
  else streamPoints(recent, 10);
}

void handleEvents() {
  String out = "{\"evenements\":[";
  for (size_t i = 0; i < events.count; ++i) {
    const Event &e = events.oldest(events.count - 1 - i);
    if (i) out += ',';
    out += '[';
    out += e.time;
    out += ',';
    appendJsonString(out, e.kind);
    out += ',';
    appendJsonString(out, e.text);
    out += ']';
  }
  out += "]}";
  http.send(200, "application/json; charset=utf-8", out);
}

const char *telegramText() {
  if (!telegramConfigured()) return "non configuré";
  static char text[24];
  if (telegramStatus == 0) return "prêt";
  if (telegramStatus == 200) return "ok";
  snprintf(text, sizeof(text), "erreur %d", telegramStatus);
  return text;
}

void handleState() {
  uint32_t now = millis();
  char clock[24];
  formatTime(epochNow(), clock, sizeof(clock), "%d/%m/%Y %H:%M:%S");
  bool simulating = int32_t(simulatedUntil - now) > 0;

  String out = "{\"heure\":";
  appendJsonString(out, clock);
  out += ",\"pompe\":\"";
  out += STATE_NAMES[shownState];
  out += "\",\"adc\":";
  out += shownState == INCONNU ? String("null") : String(currentAdc);
  out += ",\"temperature_c\":";
  appendNumber(out, scaled(temperature), true);
  out += ",\"humidite_pct\":";
  appendNumber(out, scaled(humidity), true);
  out += ",\"climat\":\"";
  out += isnan(temperature) ? "absent" : simulating ? "simulation" : "capteur";
  out += "\",\"pic_age_s\":";
  out += picSeen ? String((now - lastPicReport) / 1000) : String("null");
  out += ",\"bloque_depuis\":";
  out += shownState == DEFAUT && blockedSinceEpoch ? String(blockedSinceEpoch) : String("null");
  out += ",\"wifi_dbm\":";
  out += WiFi.RSSI();
  out += ",\"ip\":\"";
  out += WiFi.localIP().toString();
  out += "\",\"uptime_s\":";
  out += now / 1000;
  out += ",\"telegram\":";
  appendJsonString(out, telegramText());
  out += "}";
  http.send(200, "application/json; charset=utf-8", out);
}

void handleTestAlert() {
  if (!telegramConfigured()) {
    http.send(409, "application/json; charset=utf-8", "{\"ok\":false,\"erreur\":\"Telegram non configuré\"}");
    return;
  }
  addEvent("test", "Alerte de test envoyée depuis le site", true);
  http.send(200, "application/json; charset=utf-8", "{\"ok\":true}");
}

void handleTelemetry() {
  bool alive = shownState != INCONNU;
  String out = "{\"temperature_c\":";
  appendNumber(out, scaled(temperature), true);
  out += ",\"humidite_pct\":";
  appendNumber(out, scaled(humidity), true);
  out += ",\"pompe\":\"";
  out += STATE_NAMES[shownState];
  out += "\",\"adc\":";
  out += alive ? String(currentAdc) : String("null");
  out += "}";
  http.send(200, "application/json; charset=utf-8", out);
}

void handlePage() {
  http.sendHeader("Cache-Control", "no-cache");
  http.send_P(200, "text/html; charset=utf-8", PAGE);
}

void updateNetwork() {
  bool connected = WiFi.status() == WL_CONNECTED;
  if (connected && !wifiAnnounced) {
    wifiAnnounced = true;
    Serial.printf("Wi-Fi connecté : http://%s/ ou http://%s.local/\n", WiFi.localIP().toString().c_str(), HOSTNAME);
    MDNS.begin(HOSTNAME);
    MDNS.addService("http", "tcp", 80);
  } else if (!connected && wifiAnnounced) {
    wifiAnnounced = false;
    Serial.println("Wi-Fi perdu, reconnexion automatique");
  }
  if (!clockAnnounced && clockSynced()) {
    clockAnnounced = true;
    uint32_t epoch = epochNow(), uptime = millis();
    for (uint16_t i = 0; i < min<uint16_t>(eventsThisBoot, events.count); ++i) {
      Event &e = events.newest(i);
      if (!e.time) e.time = epoch - (uptime - e.uptimeMs) / 1000;
    }
    addEvent("demarrage", "ESP32 démarré, horloge à l'heure", false);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN, 100000);
  Wire.setTimeOut(30);
  pic.setRxBufferSize(1024);
  pic.begin(19200, SERIAL_8N1, PIC_RX_PIN, PIC_TX_PIN);
  lastMeasurement = millis() - SENSOR_PERIOD_MS;

  if (LittleFS.begin(true)) {
    loadFile("/historique.bin", day);
    loadFile("/evenements.bin", events);
  } else {
    Serial.println("LittleFS indisponible : historique non conservé");
  }

  telegramQueue = xQueueCreate(8, 200);
  xTaskCreate(telegramTask, "telegram", 8192, nullptr, 1, nullptr);

  configTzTime(TIMEZONE, "pool.ntp.org", "time.google.com");
  if (WIFI_SSID[0]) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  } else {
    Serial.println("Pas de wifi.local.h : site et alertes désactivés");
  }

  http.on("/", HTTP_GET, handlePage);
  http.on("/api/etat", HTTP_GET, handleState);
  http.on("/api/historique", HTTP_GET, handleHistory);
  http.on("/api/evenements", HTTP_GET, handleEvents);
  http.on("/api/test-alerte", HTTP_POST, handleTestAlert);
  http.on("/telemetrie", HTTP_GET, handleTelemetry);
  http.onNotFound([] { http.send(404, "text/plain; charset=utf-8", "Introuvable"); });
  http.begin();
  Serial.printf("Vide-cave ESP32 prêt (Telegram %s)\n", telegramConfigured() ? "configuré" : "non configuré");
}

void loop() {
  uint32_t now = millis();
  receiveFrom(pic, fromPic, false, now);
  receiveFrom(Serial, fromUsb, true, now);
  updateClimate(now);
  updatePump(now);
  sampleHistory(now);
  updateNetwork();
  http.handleClient();
  delay(2);
}
