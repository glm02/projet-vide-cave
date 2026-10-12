from pathlib import Path
import json,hashlib
ROOT=Path(__file__).resolve().parents[2];J=Path(__file__).resolve().parent
moves=json.loads((J/'deplacements.json').read_text(encoding='utf-8-sig'))
before=json.loads((J/'inventaire_avant.json').read_text(encoding='utf-8-sig'))
missing=[];changed=[]
for item in before:
    old=item['chemin'];new=old
    for move in moves:
        if old==move['ancien'] or old.startswith(move['ancien']+'\\'):
            new=move['nouveau']+old[len(move['ancien']):];break
    path=ROOT/new
    if not path.is_file():missing.append(new)
    elif hashlib.sha256(path.read_bytes()).hexdigest().upper()!=item['sha256']:changed.append(new)
assert not missing,missing
allowed=['04_Maquette_3D\\lire_kicad.py','04_Maquette_3D\\LISEZ_MOI.md','04_Maquette_3D\\implantation_kicad.json']
assert all(x in allowed or '__pycache__' in x for x in changed),changed
for name in ['lire_kicad.py','LISEZ_MOI.md','implantation_kicad.json']:
    ref=next(x for x in before if x['chemin'].replace('\\','/')=='maquette_3d/'+name)
    assert hashlib.sha256((J/'Versions_avant_adaptation'/name).read_bytes()).hexdigest().upper()==ref['sha256']
report={'fichiers_manquants':missing,'fichiers_adaptes':changed,'copies_originales_verifiees':True,'total_original':len(before)}
(J/'verification_finale.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf8')
print(json.dumps(report,ensure_ascii=False))
