#pragma once

// Jetons de couleur et style des boutons et tags repris de kargulstudio/sales-crm (MIT), voir THIRD_PARTY.md.
static const char PAGE[] PROGMEM = R"html(<!doctype html>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#161616">
<title>Vide-cave</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Geist:wght@400;500;600&family=Geist+Mono:wght@400;500&display=swap">
<style>
:root{color-scheme:dark;--fond:#161616;--texte:#f9fbff;--carte:#1b1d20;--carte2:#1f2124;--second:#1e1e1e;--muted:#2a2a2a;--gris:#7f7f7f;--subtle:#676767;--faint:#454545;--soft:#a4a4a4;--chip:#cfcfcf;--bord:#232323;--bord-fort:#393939;--accent:#4124fb;--accent-clair:#7b68ff;--succes:#22c55e;--alerte:#fbbf24;--danger:#f97373;--hum:#38bdf8;--barre:#171717;
--vert-bg:#1f3a2d;--vert-bd:#275137;--vert-tx:#b1ebc5;--rouge-bg:#3e1d1e;--rouge-bd:#4c2324;--rouge-tx:#febfc6;--ambre-bg:#31221b;--ambre-bd:#6c4830;--ambre-tx:#fed7aa;--bleu-bg:#1d2b3e;--bleu-bd:#23354c;--bleu-tx:#bfdbfe;--neutre-bg:#2a2a2a;--neutre-bd:#363636;--neutre-tx:#cfcfcf;
--ease:cubic-bezier(.25,1,.5,1);--sans:Geist,system-ui,-apple-system,"Segoe UI",sans-serif;--mono:"Geist Mono",ui-monospace,Consolas,monospace;--relief:0 0 0 1px rgba(0,0,0,.4),inset 0 1px 0 rgba(255,255,255,.1),inset 0 0 0 1px rgba(255,255,255,.06)}
*{box-sizing:border-box}
html,body{margin:0;height:100%}
body{background:var(--fond);color:var(--texte);font:14px/1.15 var(--sans);-webkit-font-smoothing:antialiased}
button{font:inherit;color:inherit}
svg.i{width:16px;height:16px;flex:none;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}
:focus-visible{outline:2px solid rgba(103,103,103,.6);outline-offset:2px}
.app{display:flex;min-height:100%}
aside{width:254px;flex:none;background:var(--barre);border-right:1px solid var(--bord);padding:14px 10px;display:flex;flex-direction:column;gap:20px;position:sticky;top:0;height:100vh}
.marque{display:flex;align-items:center;gap:10px;padding:4px 6px}
.logo{width:30px;height:30px;border-radius:8px;background:var(--accent);display:grid;place-items:center;box-shadow:0 0 0 1px #0e0e0e,inset 0 4px 6px rgba(255,255,255,.2),inset 0 0 0 1px rgba(255,255,255,.15)}
.logo svg{width:17px;height:17px;stroke:#fff}
.marque b{display:block;font-weight:500;font-size:15px}
.marque small{color:var(--gris);font-size:12px}
.surtitre{font-size:12px;text-transform:uppercase;letter-spacing:1px;color:var(--subtle);padding:0 8px 8px}
nav{display:flex;flex-direction:column;gap:2px}
.nav{display:flex;align-items:center;gap:10px;width:100%;padding:8px;border:0;background:none;border-radius:8px;color:var(--gris);cursor:pointer;text-align:left;transition:background-color .15s var(--ease),color .15s var(--ease)}
.nav:hover{color:var(--texte)}
.nav[data-actif=true]{background:var(--muted);color:var(--texte);box-shadow:var(--relief)}
.nav .compteur{margin-left:auto}
.compteur{font:12px var(--mono);color:var(--subtle)}
.compteur.rouge{color:var(--rouge-tx);background:var(--rouge-bg);border:1px solid var(--rouge-bd);border-radius:999px;padding:1px 6px;font-size:11px}
.systeme{margin-top:auto;border:1px solid var(--bord);border-radius:8px;padding:4px 10px;background:var(--carte)}
.systeme div{display:flex;align-items:center;gap:8px;padding:8px 0;font-size:12px;color:var(--gris);border-top:1px solid var(--bord)}
.systeme div:first-child{border-top:0}
.systeme span{margin-left:auto;color:var(--chip);font-family:var(--mono)}
.point{width:6px;height:6px;border-radius:50%;background:var(--subtle);display:inline-block}
.point.vert{background:var(--succes)}.point.rouge{background:var(--danger)}.point.ambre{background:var(--alerte)}
.principal{flex:1;min-width:0;display:flex;flex-direction:column}
.entete{height:56px;display:flex;align-items:center;justify-content:space-between;gap:12px;padding:0 24px;border-bottom:1px solid var(--bord);position:sticky;top:0;background:rgba(22,22,22,.85);backdrop-filter:blur(8px);z-index:2}
.fil{display:flex;gap:8px;align-items:center;font-size:14px;color:var(--gris)}
.fil h1{font-size:16px;font-weight:500;margin:0;color:var(--texte)}
.direct{display:flex;align-items:center;gap:8px;font:12px var(--mono);color:var(--gris)}
.direct .point{animation:respire 2s infinite}
@keyframes respire{50%{opacity:.3}}
.contenu{padding:24px;max-width:1240px;width:100%}
.vue{display:none}.vue.active{display:block;animation:entree .25s var(--ease)}
@keyframes entree{from{opacity:0;transform:translateY(4px)}}
.panneau{background:var(--carte);border:1px solid var(--bord);border-radius:10px}
.panneau-tete{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:14px 16px 4px}
.panneau-tete h2{font-size:12px;text-transform:uppercase;letter-spacing:1px;color:var(--subtle);font-weight:500;margin:0}
.lien{border:0;background:none;color:var(--gris);font-size:12px;cursor:pointer;padding:4px 0}
.lien:hover{color:var(--texte)}
.hero{display:flex;align-items:center;gap:18px;padding:20px;margin-bottom:16px;position:relative;overflow:hidden}
.hero::after{content:"";position:absolute;inset:0;background:radial-gradient(600px 180px at 0% 0%,var(--teinte,transparent),transparent 70%);pointer-events:none;opacity:.55}
.hero.vert{--teinte:rgba(34,197,94,.18)}.hero.rouge{--teinte:rgba(249,115,115,.28);border-color:var(--rouge-bd)}.hero.ambre{--teinte:rgba(251,191,36,.18)}.hero.neutre{--teinte:rgba(160,160,160,.1)}
.pastille{width:52px;height:52px;border-radius:14px;display:grid;place-items:center;flex:none;background:var(--neutre-bg);border:1px solid var(--neutre-bd);color:var(--neutre-tx);position:relative;z-index:1}
.pastille svg{width:24px;height:24px}
.hero.vert .pastille{background:var(--vert-bg);border-color:var(--vert-bd);color:var(--succes)}
.hero.rouge .pastille{background:var(--rouge-bg);border-color:var(--rouge-bd);color:var(--danger);animation:alarme 1.4s infinite}
.hero.ambre .pastille{background:var(--ambre-bg);border-color:var(--ambre-bd);color:var(--alerte)}
@keyframes alarme{50%{box-shadow:0 0 0 8px rgba(249,115,115,.12)}}
.hero-texte{flex:1;min-width:0;position:relative;z-index:1}
.hero-texte .surtitre{padding:0 0 6px}
.hero-titre{font-size:22px;font-weight:600;letter-spacing:-.01em}
.hero-sous{color:var(--gris);margin-top:6px;line-height:1.4}
.hero-droite{position:relative;z-index:1;text-align:right;display:flex;flex-direction:column;gap:6px;align-items:flex-end}
.hero-droite b{font:500 22px var(--mono)}
.hero-droite small{color:var(--gris);font-size:12px}
.consigne{display:none;margin-top:10px;font-size:13px;color:var(--rouge-tx);align-items:center;gap:6px}
.hero.rouge .consigne{display:flex}
.tuiles{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:12px;margin-bottom:16px}
.tuile{padding:14px 16px 0;display:flex;flex-direction:column;gap:8px;overflow:hidden}
.tuile-tete{display:flex;align-items:center;gap:8px;color:var(--gris);font-size:13px}
.tuile-tete .etiquette{margin-left:auto;font-size:11px;color:var(--subtle);font-family:var(--mono)}
.valeur{display:flex;align-items:baseline;gap:6px}
.valeur b{font:500 28px var(--mono);letter-spacing:-.02em}
.valeur span{color:var(--gris)}
.ecart{font-size:12px;color:var(--gris);font-family:var(--mono)}
.jauge{height:6px;border-radius:99px;background:var(--muted);overflow:hidden;margin:4px 0 2px}
.jauge i{display:block;height:100%;width:0;border-radius:99px;background:var(--chip);transition:width .4s var(--ease)}
.spark{width:calc(100% + 32px);margin:0 -16px;display:block}
.deux{display:grid;grid-template-columns:minmax(0,1fr) 340px;gap:16px}
.graphe{padding:6px 16px 4px;position:relative}
canvas{width:100%;display:block;touch-action:pan-y}
.legende{display:flex;gap:16px;flex-wrap:wrap;font-size:12px;color:var(--gris);padding:6px 16px 14px}
.legende i{display:inline-block;width:8px;height:8px;border-radius:2px;margin-right:6px;vertical-align:-1px}
.flux{list-style:none;margin:0;padding:4px 16px 8px}
.flux li{display:grid;grid-template-columns:auto 1fr;gap:4px 10px;padding:10px 0;border-top:1px solid var(--bord);align-items:center}
.flux li:first-child{border-top:0}
.flux p{margin:0;grid-column:1/3;line-height:1.35;color:var(--chip)}
.flux time{justify-self:end;font:12px var(--mono);color:var(--subtle)}
.btn{display:inline-flex;align-items:center;gap:6px;border:0;border-radius:999px;padding:9px 12px;font-size:12px;font-weight:500;line-height:1;cursor:pointer;white-space:nowrap;transition:background-color .15s var(--ease)}
.btn.primaire{background:var(--accent);box-shadow:0 4px 4px rgba(42,42,42,.32),0 0 0 1px #0e0e0e,inset 0 4px 6px rgba(255,255,255,.2),inset 0 0 0 1px rgba(255,255,255,.15),inset 0 -8px 14px rgba(0,0,0,.15)}
.btn.primaire:hover{background:#4b30ff}
.btn:disabled{opacity:.5;pointer-events:none}
.btn svg.i{width:14px;height:14px}
.tag{display:inline-flex;align-items:center;gap:6px;height:22px;padding:0 8px;border-radius:999px;border:1px solid var(--neutre-bd);background:var(--neutre-bg);color:var(--neutre-tx);font-size:13px;white-space:nowrap}
.tag::before{content:"";width:6px;height:6px;border-radius:50%;background:currentColor}
.tag.vert{border-color:var(--vert-bd);background:var(--vert-bg);color:var(--vert-tx)}
.tag.rouge{border-color:var(--rouge-bd);background:var(--rouge-bg);color:var(--rouge-tx)}
.tag.ambre{border-color:var(--ambre-bd);background:var(--ambre-bg);color:var(--ambre-tx)}
.tag.bleu{border-color:var(--bleu-bd);background:var(--bleu-bg);color:var(--bleu-tx)}
.outils{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:14px;flex-wrap:wrap}
.segments{display:inline-flex;gap:2px;padding:2px;background:var(--second);border:1px solid var(--bord);border-radius:999px}
.segments button{border:0;background:none;color:var(--gris);padding:7px 14px;border-radius:999px;font-size:12px;cursor:pointer;display:inline-flex;gap:6px;transition:background-color .15s var(--ease),color .15s var(--ease)}
.segments button[data-actif=true]{background:var(--muted);color:var(--texte);box-shadow:var(--relief)}
.stats{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));border:1px solid var(--bord);border-radius:10px;background:var(--carte);margin-bottom:16px}
.stats div{padding:14px 16px;border-left:1px solid var(--bord)}
.stats div:first-child{border-left:0}
.stats span{display:block;color:var(--gris);font-size:12px;margin-bottom:8px}
.stats b{font:500 18px var(--mono)}
.stats small{display:block;color:var(--subtle);font:12px var(--mono);margin-top:6px}
.pile{display:flex;flex-direction:column;gap:16px}
table{width:100%;border-collapse:collapse}
th{font-weight:400;color:var(--gris);text-align:left;font-size:12px;padding:12px 16px;border-bottom:1px solid var(--bord)}
td{padding:11px 16px;border-bottom:1px solid var(--bord);vertical-align:middle}
tbody tr{transition:background-color .15s var(--ease)}
tbody tr:hover{background:rgba(255,255,255,.025)}
td.date{width:140px;color:var(--soft);font:13px var(--mono)}
td.type{width:140px}
tfoot td{color:var(--gris);font-size:12px;border-bottom:0}
.vide{color:var(--gris);text-align:center;padding:36px 16px}
.onglets-mobile{display:none}
@media (max-width:1000px){.deux{grid-template-columns:1fr}}
@media (max-width:760px){
 aside{display:none}
 .entete{padding:0 16px}.contenu{padding:16px}
 .direct span{display:none}
 .onglets-mobile{display:flex;gap:4px;padding:8px 12px;border-bottom:1px solid var(--bord);overflow-x:auto;position:sticky;top:56px;background:var(--fond);z-index:2}
 .onglets-mobile .nav{width:auto;padding:8px 10px;white-space:nowrap}
 .onglets-mobile .nav svg{display:none}
 .hero{flex-wrap:wrap;padding:16px}
 .hero-droite{width:100%;flex-direction:row;justify-content:space-between;align-items:baseline;text-align:left;border-top:1px solid var(--bord);padding-top:12px}
 .hero-titre{font-size:19px}
 .tuiles{grid-template-columns:1fr}
 .stats{grid-template-columns:repeat(2,minmax(0,1fr))}
 .stats div:nth-child(3){border-left:0}
 .stats div:nth-child(n+3){border-top:1px solid var(--bord)}
 thead{display:none}
 tbody tr{display:grid;grid-template-columns:1fr auto}
 tbody td{border:0;padding:4px 16px}
 tbody td.date{padding-top:12px;width:auto;font-size:12px}
 tbody td.type{grid-column:2;grid-row:1;padding-top:10px;width:auto}
 tbody td.texte{grid-column:1/3;padding-bottom:12px;border-bottom:1px solid var(--bord)}
}
</style>
</head>
<body>
<svg width="0" height="0" style="position:absolute">
 <symbol id="i-grille" viewBox="0 0 24 24"><rect x="3" y="3" width="7" height="7" rx="1.5"/><rect x="14" y="3" width="7" height="7" rx="1.5"/><rect x="3" y="14" width="7" height="7" rx="1.5"/><rect x="14" y="14" width="7" height="7" rx="1.5"/></symbol>
 <symbol id="i-courbe" viewBox="0 0 24 24"><path d="M3 3v18h18"/><path d="M7 15l4-4 3 3 5-6"/></symbol>
 <symbol id="i-cloche" viewBox="0 0 24 24"><path d="M6 8a6 6 0 0 1 12 0c0 7 3 9 3 9H3s3-2 3-9"/><path d="M10.3 21a1.94 1.94 0 0 0 3.4 0"/></symbol>
 <symbol id="i-thermo" viewBox="0 0 24 24"><path d="M14 4v10.54a4 4 0 1 1-4 0V4a2 2 0 0 1 4 0Z"/></symbol>
 <symbol id="i-goutte" viewBox="0 0 24 24"><path d="M12 2.7l5.7 5.6a8 8 0 1 1-11.4 0z"/></symbol>
 <symbol id="i-eclair" viewBox="0 0 24 24"><path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z"/></symbol>
 <symbol id="i-ok" viewBox="0 0 24 24"><path d="M20 6L9 17l-5-5"/></symbol>
 <symbol id="i-alerte" viewBox="0 0 24 24"><path d="M10.3 3.9L1.8 18a2 2 0 0 0 1.7 3h17a2 2 0 0 0 1.7-3L13.7 3.9a2 2 0 0 0-3.4 0z"/><path d="M12 9v4"/><path d="M12 17h.01"/></symbol>
 <symbol id="i-pause" viewBox="0 0 24 24"><rect x="6" y="4" width="4" height="16" rx="1"/><rect x="14" y="4" width="4" height="16" rx="1"/></symbol>
 <symbol id="i-question" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10"/><path d="M9.1 9a3 3 0 0 1 5.8 1c0 2-3 3-3 3"/><path d="M12 17h.01"/></symbol>
 <symbol id="i-wifi" viewBox="0 0 24 24"><path d="M5 12.55a11 11 0 0 1 14 0"/><path d="M8.5 16.1a6 6 0 0 1 7 0"/><path d="M12 20h.01"/></symbol>
 <symbol id="i-envoi" viewBox="0 0 24 24"><path d="M22 2L11 13"/><path d="M22 2l-7 20-4-9-9-4 20-7z"/></symbol>
 <symbol id="i-horloge" viewBox="0 0 24 24"><circle cx="12" cy="12" r="10"/><path d="M12 6v6l4 2"/></symbol>
 <symbol id="i-lien" viewBox="0 0 24 24"><path d="M9 17H7A5 5 0 0 1 7 7h2"/><path d="M15 7h2a5 5 0 1 1 0 10h-2"/><path d="M8 12h8"/></symbol>
 <symbol id="i-cave" viewBox="0 0 24 24"><path d="M3 21V10l9-7 9 7v11"/><path d="M8 21v-6h8v6"/><path d="M12 12v.01"/></symbol>
</svg>
<div class="app">
<aside>
 <div class="marque"><span class="logo"><svg class="i"><use href="#i-cave"/></svg></span><div><b>Vide-cave</b><small>Supervision ESP32</small></div></div>
 <div><div class="surtitre">Supervision</div>
 <nav>
  <button class="nav" data-vue="etat"><svg class="i"><use href="#i-grille"/></svg>Vue d’ensemble</button>
  <button class="nav" data-vue="historique"><svg class="i"><use href="#i-courbe"/></svg>Historique</button>
  <button class="nav" data-vue="evenements"><svg class="i"><use href="#i-cloche"/></svg>Événements<span class="compteur" id="navEvts"></span></button>
 </nav></div>
 <div class="systeme">
  <div><svg class="i"><use href="#i-wifi"/></svg>Wi-Fi<span id="sWifi">—</span></div>
  <div><svg class="i"><use href="#i-lien"/></svg>Liaison PIC<span id="sPic">—</span></div>
  <div><svg class="i"><use href="#i-envoi"/></svg>Telegram<span id="sTg">—</span></div>
  <div><svg class="i"><use href="#i-horloge"/></svg>Allumé depuis<span id="sUp">—</span></div>
 </div>
</aside>
<div class="principal">
 <header class="entete">
  <div class="fil"><span>Supervision</span><span>/</span><h1 id="titre">Vue d’ensemble</h1></div>
  <div class="direct"><i class="point vert" id="pointDirect"></i><span id="direct">Connexion…</span></div>
 </header>
 <div class="onglets-mobile">
  <button class="nav" data-vue="etat"><svg class="i"><use href="#i-grille"/></svg>Vue d’ensemble</button>
  <button class="nav" data-vue="historique"><svg class="i"><use href="#i-courbe"/></svg>Historique</button>
  <button class="nav" data-vue="evenements"><svg class="i"><use href="#i-cloche"/></svg>Événements<span class="compteur" id="navEvtsM"></span></button>
 </div>
 <main class="contenu">
  <section class="vue" id="vue-etat">
   <div class="panneau hero neutre" id="hero">
    <div class="pastille"><svg class="i"><use id="heroIcone" href="#i-question"/></svg></div>
    <div class="hero-texte"><div class="surtitre">Pompe</div><div class="hero-titre" id="heroTitre">Connexion…</div>
     <div class="hero-sous" id="heroSous">En attente des données de l’ESP32</div>
     <div class="consigne"><svg class="i"><use href="#i-alerte"/></svg>Réarmer avec le bouton reset du PIC après intervention</div></div>
    <div class="hero-droite"><small>Courant mesuré</small><b id="heroAdc">—</b><small id="heroAge">—</small></div>
   </div>
   <div class="tuiles">
    <div class="panneau tuile"><div class="tuile-tete"><svg class="i"><use href="#i-thermo"/></svg>Température<span class="etiquette" id="srcClimat"></span></div>
     <div class="valeur"><b id="vTemp">—</b><span>°C</span></div><div class="ecart" id="dTemp">—</div><canvas class="spark" id="sparkTemp" height="46"></canvas></div>
    <div class="panneau tuile"><div class="tuile-tete"><svg class="i"><use href="#i-goutte"/></svg>Humidité</div>
     <div class="valeur"><b id="vHum">—</b><span>%HR</span></div><div class="ecart" id="dHum">—</div><canvas class="spark" id="sparkHum" height="46"></canvas></div>
    <div class="panneau tuile"><div class="tuile-tete"><svg class="i"><use href="#i-eclair"/></svg>Courant moteur<span class="etiquette">ADC 12 bits</span></div>
     <div class="valeur"><b id="vAdc">—</b><span>/ 4095</span></div><div class="jauge"><i id="jauge"></i></div><div class="ecart" id="dAdc">—</div><canvas class="spark" id="sparkAdc" height="30"></canvas></div>
   </div>
   <div class="deux">
    <div class="panneau"><div class="panneau-tete"><h2>Courant · dernière heure</h2><button class="lien" data-aller="historique">Historique →</button></div>
     <div class="graphe"><canvas id="apercu" height="262"></canvas></div>
     <div class="legende"><span><i style="background:var(--chip)"></i>Courant</span><span><i style="background:var(--succes)"></i>Marche</span><span><i style="background:var(--danger)"></i>Défaut</span><span><i style="background:var(--faint)"></i>Arrêt</span></div></div>
    <div class="panneau"><div class="panneau-tete"><h2>Derniers événements</h2><button class="lien" data-aller="evenements">Tout voir →</button></div><ul class="flux" id="flux"></ul></div>
   </div>
  </section>

  <section class="vue" id="vue-historique">
   <div class="outils"><div class="segments" id="plages"><button data-plage="1h" data-actif="true">1 heure</button><button data-plage="24h">24 heures</button></div><span class="compteur" id="nbPoints"></span></div>
   <div class="stats">
    <div><span>Température min · max</span><b id="stT">—</b><small id="stTm"></small></div>
    <div><span>Humidité min · max</span><b id="stH">—</b><small id="stHm"></small></div>
    <div><span>Pompe en marche</span><b id="stM">—</b><small id="stMd"></small></div>
    <div><span>Blocages</span><b id="stD">—</b><small id="stDd"></small></div>
   </div>
   <div class="pile">
    <div class="panneau"><div class="panneau-tete"><h2>Climat de la cave</h2></div><div class="graphe"><canvas id="climat" height="250"></canvas></div>
     <div class="legende"><span><i style="background:var(--accent-clair)"></i>Température °C · axe gauche</span><span><i style="background:var(--hum)"></i>Humidité %HR · axe droit</span></div></div>
    <div class="panneau"><div class="panneau-tete"><h2>Courant et état de la pompe</h2></div><div class="graphe"><canvas id="courant" height="210"></canvas></div>
     <div class="legende"><span><i style="background:var(--chip)"></i>Courant</span><span><i style="background:var(--succes)"></i>Marche</span><span><i style="background:var(--danger)"></i>Défaut</span><span><i style="background:var(--faint)"></i>Arrêt</span></div></div>
   </div>
  </section>

  <section class="vue" id="vue-evenements">
   <div class="outils"><div class="segments" id="filtres">
    <button data-filtre="tous" data-actif="true">Tous <span class="compteur" id="nTous">0</span></button>
    <button data-filtre="alertes">Alertes <span class="compteur" id="nAlertes">0</span></button>
    <button data-filtre="autres">Autres <span class="compteur" id="nAutres">0</span></button>
   </div><button class="btn primaire" id="test"><svg class="i"><use href="#i-envoi"/></svg><span>Alerte de test</span></button></div>
   <div class="panneau"><table><thead><tr><th>Date</th><th>Type</th><th>Message</th></tr></thead><tbody id="lignes"></tbody><tfoot><tr><td colspan="3" id="total"></td></tr></tfoot></table></div>
  </section>
 </main>
</div>
</div>
<script>
const $=id=>document.getElementById(id);
const V=n=>getComputedStyle(document.documentElement).getPropertyValue(n).trim();
const rgba=(hex,a)=>{const n=parseInt(hex.slice(1),16);return 'rgba('+(n>>16)+','+(n>>8&255)+','+(n&255)+','+a+')'};
const ETATS={ARRET:['À l’arrêt','neutre','i-pause'],MARCHE:['En marche','vert','i-ok'],DEFAUT:['Moteur bloqué','rouge','i-alerte'],INCONNU:['Pas de nouvelles du PIC','ambre','i-question']};
const TYPES={defaut:['Défaut','rouge'],rappel:['Rappel','rouge'],liaison:['Liaison','ambre'],retour:['Rétabli','vert'],etat:['État',''],test:['Test','bleu'],demarrage:['Démarrage','']};
const ALERTES=['defaut','rappel','liaison'];
const TITRES={etat:'Vue d’ensemble',historique:'Historique',evenements:'Événements'};
const NOMS_ETATS=['Arrêt','Marche','Défaut','Inconnu'];
let vue='etat',plage='1h',points=[],apercu=[],evts=[],filtre='tous',dernier=null;
const virgule=(v,d)=>v.toFixed(d).replace('.',',').replace('-','−');
const fmt=(v,d=1)=>v==null?'—':virgule(v,d);
const hm=t=>new Date(t*1000).toLocaleTimeString('fr-FR',{hour:'2-digit',minute:'2-digit'});
const date=t=>new Date(t*1000).toLocaleString('fr-FR',{day:'2-digit',month:'2-digit',hour:'2-digit',minute:'2-digit'});
function duree(s){if(s<60)return Math.max(0,Math.round(s))+' s';const m=Math.floor(s/60);if(m<60)return m+' min';const h=Math.floor(m/60);if(h<48)return h+' h '+String(m%60).padStart(2,'0');return Math.floor(h/24)+' j '+h%24+' h'}
async function lire(u){const r=await fetch(u,{cache:'no-store'});if(!r.ok)throw r.status;return r.json()}
function tag(texte,ton){const s=document.createElement('span');s.className='tag'+(ton?' '+ton:'');s.textContent=texte;return s}

function montrer(v){
 vue=TITRES[v]?v:'etat';
 document.querySelectorAll('.vue').forEach(s=>s.classList.toggle('active',s.id==='vue-'+vue));
 document.querySelectorAll('.nav').forEach(b=>b.dataset.actif=b.dataset.vue===vue);
 $('titre').textContent=TITRES[vue];
 if(location.hash!=='#'+vue)history.replaceState(null,'','#'+vue);
 dessiner();
}

async function etat(){
 try{
  const e=await lire('/api/etat');dernier=e;
  const [lib,ton,ico]=ETATS[e.pompe],now=Date.now()/1000;
  $('hero').className='panneau hero '+ton;$('heroIcone').setAttribute('href','#'+ico);$('heroTitre').textContent=lib;
  let sous=e.pompe==='INCONNU'?(e.pic_age_s==null?'Aucune trame reçue du PIC depuis le démarrage':'Dernière trame il y a '+duree(e.pic_age_s)):'';
  const depuis=e.pompe==='DEFAUT'&&e.bloque_depuis?e.bloque_depuis:e.depuis;
  if(e.pompe!=='INCONNU')sous=depuis?'Depuis '+duree(now-depuis)+' · le '+date(depuis):'Depuis le démarrage';
  $('heroSous').textContent=sous;
  $('heroAdc').textContent=e.adc==null?'—':e.adc;
  $('heroAge').textContent=e.pic_age_s==null?'pas de trame':'trame il y a '+e.pic_age_s+' s';
  $('vTemp').textContent=fmt(e.temperature_c);$('vHum').textContent=fmt(e.humidite_pct,0);
  $('srcClimat').textContent=e.climat==='simulation'?'simulation':e.climat==='absent'?'capteur absent':'SHT45';
  $('vAdc').textContent=e.adc==null?'—':e.adc;$('jauge').style.width=(e.adc==null?0:e.adc/40.95)+'%';
  $('jauge').style.background=e.pompe==='DEFAUT'?V('--danger'):V('--chip');
  $('direct').textContent=e.heure.slice(-8);$('pointDirect').className='point vert';
  const rs=e.wifi_dbm;$('sWifi').textContent=rs+' dBm';
  $('sPic').textContent=e.pic_age_s==null?'aucune':e.pompe==='INCONNU'?'perdue':'ok';
  $('sTg').textContent=e.telegram;$('sUp').textContent=duree(e.uptime_s);
 }catch(_){$('direct').textContent='hors ligne';$('pointDirect').className='point ambre'}
}

async function historique(){
 try{
  apercu=(await lire('/api/historique?plage=1h')).points;
  points=plage==='1h'?apercu:(await lire('/api/historique?plage=24h')).points;
  tuiles();stats();dessiner();
 }catch(_){}
}

function ecart(i,d,u){
 const v=apercu.map(p=>p[i]).filter(x=>x!=null);if(v.length<2)return 'pas assez de mesures';
 const x=v[v.length-1]-v[0];return (x>=0?'+':'')+virgule(x,d)+u+' sur 1 h';
}
function tuiles(){
 $('dTemp').textContent=ecart(1,1,' °C');$('dHum').textContent=ecart(2,0,' %');
 const a=apercu.filter(p=>p[4]!==3).map(p=>p[3]);
 $('dAdc').textContent=a.length?'max '+Math.max(...a)+' sur 1 h':'pas de trame sur 1 h';
 if(vue==='etat'){spark('sparkTemp',apercu.map(p=>p[1]),V('--accent-clair'));spark('sparkHum',apercu.map(p=>p[2]),V('--hum'));spark('sparkAdc',apercu.map(p=>p[4]===3?null:p[3]),V('--chip'),[0,4095])}
}
function stats(){
 const T=points.map(p=>p[1]).filter(x=>x!=null),H=points.map(p=>p[2]).filter(x=>x!=null),k=points.filter(p=>p[4]!==3);
 $('stT').textContent=T.length?virgule(Math.min(...T),1)+' · '+virgule(Math.max(...T),1):'—';$('stTm').textContent=T.length?'moy. '+virgule(T.reduce((a,b)=>a+b)/T.length,1)+' °C':'';
 $('stH').textContent=H.length?virgule(Math.min(...H),0)+' · '+virgule(Math.max(...H),0):'—';$('stHm').textContent=H.length?'moy. '+virgule(H.reduce((a,b)=>a+b)/H.length,0)+' %':'';
 $('stM').textContent=k.length?Math.round(100*k.filter(p=>p[4]===1).length/k.length)+' %':'—';
 const pas=plage==='1h'?10:60;$('stMd').textContent=k.length?duree(k.filter(p=>p[4]===1).length*pas):'';
 let n=0;for(let i=0;i<points.length;i++)if(points[i][4]===2&&(i===0||points[i-1][4]!==2))n++;
 $('stD').textContent=n;$('stDd').textContent=n?'dont '+duree(points.filter(p=>p[4]===2).length*pas)+' bloqué':'aucun';
 $('nbPoints').textContent=points.length+' points';
}

async function evenements(){try{evts=(await lire('/api/evenements')).evenements;tableau();flux()}catch(_){}}
function flux(){
 const ul=$('flux');ul.replaceChildren();
 if(!evts.length){const li=document.createElement('li');li.className='vide';li.textContent='Aucun événement';ul.append(li);return}
 for(const [t,type,texte] of evts.slice(0,5)){
  const li=document.createElement('li'),[n,ton]=TYPES[type]||[type,''],tm=document.createElement('time'),p=document.createElement('p');
  tm.textContent=t?date(t):'—';p.textContent=texte;li.append(tag(n,ton),tm,p);ul.append(li);
 }
}
function tableau(){
 const al=evts.filter(e=>ALERTES.includes(e[1]));
 $('nTous').textContent=evts.length;$('nAlertes').textContent=al.length;$('nAutres').textContent=evts.length-al.length;
 for(const id of ['navEvts','navEvtsM']){$(id).textContent=al.length||'';$(id).className='compteur'+(al.length?' rouge':'')}
 const l=evts.filter(e=>filtre==='tous'||(filtre==='alertes')===ALERTES.includes(e[1]));
 const tb=$('lignes');tb.replaceChildren();
 if(!l.length){const tr=document.createElement('tr'),td=document.createElement('td');td.colSpan=3;td.className='vide';td.textContent='Aucun événement dans cette catégorie';tr.append(td);tb.append(tr)}
 for(const [t,type,texte] of l){
  const tr=document.createElement('tr'),d=document.createElement('td'),y=document.createElement('td'),x=document.createElement('td'),[n,ton]=TYPES[type]||[type,''];
  d.className='date';d.textContent=t?date(t):'—';y.className='type';y.append(tag(n,ton));x.className='texte';x.textContent=texte;tr.append(d,y,x);tb.append(tr);
 }
 $('total').textContent=l.length+' événement'+(l.length>1?'s':'')+' · les 40 derniers sont conservés';
}

function toile(c){
 const r=devicePixelRatio||1,w=c.clientWidth,h=+(c.dataset.h||(c.dataset.h=c.getAttribute('height')));
 if(!w)return null;c.width=w*r;c.height=h*r;c.style.height=h+'px';
 const g=c.getContext('2d');g.setTransform(r,0,0,r,0,0);g.font='11px '+V('--mono');return [g,w,h];
}
function bornes(pts,d){const now=Date.now()/1000,t1=pts.length?Math.max(pts[pts.length-1][0],now-5):now;return [t1-d,t1]}
function echelle(vals,marge){const v=vals.filter(x=>x!=null);if(!v.length)return null;let a=Math.min(...v),b=Math.max(...v);if(b-a<marge){const m=(a+b)/2;a=m-marge/2;b=m+marge/2}const p=(b-a)*.12;return [a-p,b+p]}
function trace(g,pts,x,y,pas,fermer){
 const segs=[];let s=[],prev=-1e12;
 for(const [t,v] of pts){if(v==null||t-prev>3*pas){if(s.length)segs.push(s);s=[]}if(v!=null)s.push([x(t),y(v)]);prev=t}
 if(s.length)segs.push(s);return segs;
}
function ligne(g,segs,col,base){
 if(base!=null)for(const s of segs){if(s.length<2)continue;const gr=g.createLinearGradient(0,Math.min(...s.map(p=>p[1])),0,base);gr.addColorStop(0,rgba(col,.22));gr.addColorStop(1,rgba(col,0));
  g.fillStyle=gr;g.beginPath();g.moveTo(s[0][0],base);for(const p of s)g.lineTo(p[0],p[1]);g.lineTo(s[s.length-1][0],base);g.closePath();g.fill()}
 g.strokeStyle=col;g.lineWidth=1.7;g.lineJoin='round';g.lineCap='round';
 for(const s of segs){g.beginPath();s.forEach((p,i)=>i?g.lineTo(p[0],p[1]):g.moveTo(p[0],p[1]));g.stroke()}
}
function spark(id,vals,col,fixe){
 const o=toile($(id));if(!o)return;const [g,w,h]=o,e=fixe||echelle(vals,1);if(!e)return;
 const n=vals.length,x=i=>i/(Math.max(1,n-1))*w,y=v=>h-2-(v-e[0])/(e[1]-e[0])*(h-6);
 ligne(g,trace(g,vals.map((v,i)=>[i,v]),x,y,1),col,h);
}

const graphes={};
function graphe(id,cfg){graphes[id]=Object.assign(graphes[id]||{},cfg);tracer(id)}
function tracer(id){
 const cfg=graphes[id],c=$(id),o=toile(c);if(!o)return;const [g,w,h]=o;
 const {pts,d,pas,series,bande}=cfg,G=46,D=cfg.droite?46:14,haut=12,bH=bande?10:0,bas=h-26-(bande?bH+10:0);
 const [t0,t1]=bornes(pts,d),x=t=>G+(t-t0)/(t1-t0)*(w-G-D);
 g.strokeStyle=V('--bord');g.lineWidth=1;g.setLineDash([2,4]);
 for(let i=0;i<=4;i++){const yy=Math.round(haut+(bas-haut)*i/4)+.5;g.beginPath();g.moveTo(G,yy);g.lineTo(w-D,yy);g.stroke()}
 g.setLineDash([]);
 const actives=[];
 for(const s of series){
  const e=s.fixe||echelle(pts.map(p=>p[s.i]),s.marge);if(!e)continue;
  const y=v=>bas-(v-e[0])/(e[1]-e[0])*(bas-haut);actives.push({...s,y});
  g.fillStyle=s.axe==='d'?rgba(s.col,.8):V('--subtle');g.textAlign=s.axe==='d'?'left':'right';
  if(s.axe)for(let i=0;i<=4;i++){const v=e[1]-(e[1]-e[0])*i/4;g.fillText(virgule(v,s.dec),s.axe==='d'?w-D+8:G-8,haut+(bas-haut)*i/4+4)}
  ligne(g,trace(g,pts.map(p=>[p[0],s.masque&&s.masque(p)?null:p[s.i]]),x,y,pas),s.col,s.aire?bas:null);
 }
 if(!pts.some(p=>series.some(s=>p[s.i]!=null&&!(s.masque&&s.masque(p))))){g.fillStyle=V('--gris');g.textAlign='center';g.font='13px '+V('--sans');g.fillText('Pas encore de mesures sur cette période',(G+w-D)/2,(haut+bas)/2);g.font='11px '+V('--mono')}
 if(bande){const cols=[V('--faint'),V('--succes'),V('--danger'),'transparent'],yb=h-26-bH;
  for(const p of pts){const a=Math.max(G,x(p[0]-pas));g.fillStyle=cols[p[4]];g.fillRect(a,yb,Math.max(1,x(p[0])-a+.6),bH)}}
 g.fillStyle=V('--subtle');g.textAlign='center';
 for(let i=0;i<=4;i++){g.textAlign=i?i<4?'center':'right':'left';g.fillText(hm(t0+(t1-t0)*i/4),G+(w-G-D)*i/4,h-6)}
 const k=cfg.survol;if(k==null||!pts[k])return;
 const p=pts[k],X=x(p[0]);
 g.strokeStyle=V('--bord-fort');g.beginPath();g.moveTo(X+.5,haut);g.lineTo(X+.5,bas);g.stroke();
 const lignes=[[date(p[0]),V('--gris')]];
 for(const s of actives){const v=s.masque&&s.masque(p)?null:p[s.i];if(v==null)continue;
  g.fillStyle=s.col;g.beginPath();g.arc(X,s.y(v),3.5,0,7);g.fill();g.strokeStyle=V('--carte');g.lineWidth=2;g.stroke();g.lineWidth=1;
  lignes.push([s.nom+'  '+virgule(v,s.dec)+s.unite,s.col])}
 if(bande)lignes.push(['Pompe  '+NOMS_ETATS[p[4]],[V('--soft'),V('--succes'),V('--danger'),V('--alerte')][p[4]]]);
 g.font='12px '+V('--mono');const lw=Math.max(...lignes.map(l=>g.measureText(l[0]).width))+20,lh=lignes.length*18+12;
 let bx=X+12;if(bx+lw>w-4)bx=X-12-lw;const by=haut+4;
 g.fillStyle=rgba('#161616',.95);g.strokeStyle=V('--bord-fort');g.beginPath();g.roundRect?g.roundRect(bx,by,lw,lh,8):g.rect(bx,by,lw,lh);g.fill();g.stroke();
 g.textAlign='left';lignes.forEach((l,i)=>{g.fillStyle=l[1];g.fillText(l[0],bx+10,by+20+i*18)});
}
function survol(id){
 const c=$(id);
 c.addEventListener('pointermove',e=>{const cfg=graphes[id];if(!cfg||!cfg.pts.length)return;const r=c.getBoundingClientRect(),G=46,D=cfg.droite?46:14;
  const [t0,t1]=bornes(cfg.pts,cfg.d),t=t0+(e.clientX-r.left-G)/(r.width-G-D)*(t1-t0);
  let k=0,b=1e12;cfg.pts.forEach((p,i)=>{const dd=Math.abs(p[0]-t);if(dd<b){b=dd;k=i}});if(cfg.survol!==k){cfg.survol=k;tracer(id)}});
 c.addEventListener('pointerleave',()=>{if(graphes[id]){graphes[id].survol=null;tracer(id)}});
}
const COURANT=()=>({i:3,nom:'Courant',unite:'',dec:0,col:V('--chip'),fixe:[0,4095],axe:'g',aire:true,masque:p=>p[4]===3});
function dessiner(){
 if(vue==='etat'){graphe('apercu',{pts:apercu,d:3600,pas:10,bande:true,series:[COURANT()]});tuiles()}
 if(vue==='historique'){const d=plage==='1h'?3600:86400,pas=plage==='1h'?10:60;
  graphe('climat',{pts:points,d,pas,droite:true,series:[{i:1,nom:'Temp.',unite:' °C',dec:1,col:V('--accent-clair'),marge:2,axe:'g',aire:true},{i:2,nom:'Hum.',unite:' %',dec:0,col:V('--hum'),marge:10,axe:'d'}]});
  graphe('courant',{pts:points,d,pas,bande:true,series:[COURANT()]})}
}

document.querySelectorAll('.nav').forEach(b=>b.onclick=()=>montrer(b.dataset.vue));
document.querySelectorAll('[data-aller]').forEach(b=>b.onclick=()=>montrer(b.dataset.aller));
$('plages').onclick=e=>{const b=e.target.closest('button');if(!b)return;plage=b.dataset.plage;[...$('plages').children].forEach(o=>o.dataset.actif=o===b);historique()};
$('filtres').onclick=e=>{const b=e.target.closest('button');if(!b)return;filtre=b.dataset.filtre;[...$('filtres').children].forEach(o=>o.dataset.actif=o===b);tableau()};
$('test').onclick=async()=>{const b=$('test'),s=b.querySelector('span');b.disabled=true;
 try{const j=await(await fetch('/api/test-alerte',{method:'POST'})).json();s.textContent=j.ok?'Alerte envoyée':j.erreur}catch(_){s.textContent='Échec de l’envoi'}
 setTimeout(()=>{b.disabled=false;s.textContent='Alerte de test';evenements();etat()},4000)};
['apercu','climat','courant'].forEach(survol);
let attente;addEventListener('resize',()=>{clearTimeout(attente);attente=setTimeout(dessiner,100)});
addEventListener('hashchange',()=>montrer(location.hash.slice(1)));
montrer(location.hash.slice(1));
etat();historique();evenements();
setInterval(etat,2000);setInterval(historique,10000);setInterval(evenements,5000);
</script>
</body>
</html>)html";
