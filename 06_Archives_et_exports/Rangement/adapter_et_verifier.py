"""Adaptation des chemins de la maquette après rangement; aucune modification du .blend."""
from pathlib import Path
import json,hashlib,re,shutil,importlib.util
ROOT=Path(__file__).resolve().parents[2]
J=Path(__file__).resolve().parent
BACK=J/'Versions_avant_adaptation';BACK.mkdir(exist_ok=True)
M=ROOT/'04_Maquette_3D'
before=json.loads((J/'inventaire_avant.json').read_text(encoding='utf-8-sig'))
oldhash={x['chemin'].replace('\\','/'):x['sha256'].lower() for x in before}
def sha(b):return hashlib.sha256(b).hexdigest()
# Reconstruct the exact prior reader from the two path substitutions already applied.
reader=M/'lire_kicad.py';data=reader.read_bytes()
original=data.replace(b"ROOT / '03_Electronique' / 'G206A_CAMEL_TP_CAO'",b"ROOT / 'G206A_CAMEL_TP_CAO'")
original=original.replace(b"pathlib.Path(__file__).resolve().parent/'implantation_kicad.json'",b"ROOT/'maquette_3d'/'implantation_kicad.json'")
expected=oldhash['maquette_3d/lire_kicad.py']
variants=[original,original.replace(b'\r\n',b'\n'),original.replace(b'\r\n',b'\n').replace(b'\n',b'\r\n')]
original=next((x for x in variants if sha(x)==expected),None)
if original is None:raise RuntimeError('Impossible de retrouver la version originale du lecteur')
(BACK/'lire_kicad.py').write_bytes(original)
for name in ['LISEZ_MOI.md','implantation_kicad.json']:
    if not (BACK/name).exists():shutil.copy2(M/name,BACK/name)
readme=M/'LISEZ_MOI.md'
text=readme.read_text(encoding='utf8')
text=text.replace('`Rapport_pre-etude_vide-cave.docx`','`01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx`')
text=text.replace('`projet_vide_cave_geii.pptx` et les trois synoptiques `.drawio`','`01_Documents/Presentation/projet_vide_cave_geii.pptx` et les trois synoptiques `.drawio` de `02_Synoptiques`')
text=text.replace('`G206A_CAMEL_TP_CAO/','`03_Electronique/G206A_CAMEL_TP_CAO/')
text=text.replace('.\\maquette_3d\\creer_maquette.py','.\\04_Maquette_3D\\creer_maquette.py')
text=text.replace('dans `maquette_3d`','dans `04_Maquette_3D`')
readme.write_text(text,encoding='utf8')
spec=importlib.util.spec_from_file_location('lecture_kicad_range',reader)
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
assert module.SOURCE.is_file();module.run()
for f in M.glob('*.py'):compile(f.read_text(encoding='utf8'),str(f),'exec')
# Local KiCad library tables keep libraries relative to each project's new folder.
libraries=[]
for folder in [ROOT/'03_Electronique/G206A_CAMEL_TP_CAO',ROOT/'03_Electronique/Carte_vide_cave_PIC_Allegro_TRIAC']:
    for name in ['sym-lib-table','fp-lib-table']:
        for uri in re.findall(r'\(uri "([^\"]+)"\)',(folder/name).read_text(encoding='utf8')):
            if uri.startswith('${KIPRJMOD}/'):
                target=folder/uri[len('${KIPRJMOD}/'):]
                assert target.exists(),target
                libraries.append(str(target.relative_to(ROOT)))
for f in (ROOT/'03_Electronique').rglob('*.kicad_pro'):json.loads(f.read_text(encoding='utf8'))
assert sha((M/'Vide_cave_GEII.blend').read_bytes())==oldhash['maquette_3d/Vide_cave_GEII.blend']
assert (ROOT/'maquette_3d/Vide_cave_GEII.blend').samefile(M/'Vide_cave_GEII.blend')
for filename in ['01_installation.png','02_coffret_eclate.png','03_carte_kicad.png','04_interieur_moteur_pompe.png']:
    assert (M/filename).is_file()
assert (ROOT/'05_Notes/PROJET VIDE CAVE/.obsidian').is_dir()
report={'chemins_sources_valides':True,'scripts_maquette_syntaxe_valide':True,'bibliotheques_kicad_locales':libraries,'blend_et_rendus_presents':True,'alias_blender_valide':True,'coffre_obsidian_conserve':True,'fichiers_adaptes':['04_Maquette_3D/lire_kicad.py','04_Maquette_3D/LISEZ_MOI.md','04_Maquette_3D/implantation_kicad.json']}
(J/'verification_chemins.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf8')
print(json.dumps(report,ensure_ascii=False))
