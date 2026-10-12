"""Exécute les trois bancs LTspice, et conserve les résultats de mesure portables."""
import os, subprocess, json, re, sys
from pathlib import Path
ROOT=Path(__file__).resolve().parent
exe=os.environ.get('LTSPICE_PATH') or str(Path(os.environ.get('LOCALAPPDATA',''))/'Programs/ADI/LTspice/LTspice.exe')
if not Path(exe).is_file():
    sys.exit('Configurer LTSPICE_PATH vers LTspice.exe.')
results={}
logs={}
for name in ['led_moc3041','filtre_capteur','capteur_gain']:
    p=subprocess.run([exe,'-b',str(ROOT/(name+'.cir'))],capture_output=True,timeout=30)
    if p.returncode:
        sys.exit(f'{name}: LTspice retourne {p.returncode}')
    raw=(ROOT/(name+'.log')).read_bytes()
    log=raw.decode('utf-16' if raw.startswith((b'\xff\xfe',b'\xfe\xff')) else 'utf-8',errors='replace')
    log=log.replace('\r','')
    if re.search(r'(fatal error|measurement.*fail|unknown parameter|syntax error)',log,re.I):
        sys.exit(log)
    # Les .raw/.db sont locaux ; seuls les journaux textuels et ce résumé sont partageables.
    (ROOT/(name+'_resultats.txt')).write_text(log,encoding='utf-8')
    logs[name]=log.replace('\r','')
    results[name]={'status':'completed','measurement_log':name+'_resultats.txt','model':'ideal / behavioural; see circuit comments'}
    print(f'{name}: simulation exécutée')
def table(name,measurement):
    block=logs[name].split('Measurement: '+measurement+'\n',1)[1].split('Measurement:',1)[0]
    return [float(m.group(1)) for m in re.finditer(r'^\s*\d+\s+([-+0-9.eE]+)\s',block,re.M)]
led=table('led_moc3041','i_led')
expected=[(4.75-1.5-.2)/(r*1.05) for r in [150,180,220,270]]+[(5-1.25-.2)/r for r in [150,180,220,270]]+[(5.25-1.1-.1)/(r*.95) for r in [150,180,220,270]]
assert len(led)==12 and all(abs(a-b)/b<.001 for a,b in zip(led,expected)), 'Courants LED incohérents'
fc=float(re.search(r'fc:.* AT ([0-9.eE+-]+)',logs['filtre_capteur']).group(1))
assert abs(fc-15915.4943)/15915.4943<.001, 'RC incohérent'
rms=table('capteur_gain','vac_rms')
assert len(rms)==9 and all(abs(a-b)/b<.01 for a,b in zip(rms[:7],[.04,.2,.3,.2,1,1.5,.4])), 'Gain/RMS incohérent'
low=table('capteur_gain','vout_min');high=table('capteur_gain','vout_max')
assert all(low[i]<.3 and high[i]>4.7 for i in [7,8]), 'Écrêtage non constaté'
results['checks']={'status':'PASS','led_corners':12,'filter_fc_hz':fc,'gain_cases':9,'rms_uncut_measured_V':rms[:7],'clipping_cases':['10 A gain 10','15 A gain 10'],'scope':'ideal models only, not full board validation'}
print('PASS : 12 coins LED, fréquence RC, 7 gains non écrêtés et 2 cas écrêtés.')
(ROOT/'resultats_spice.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
