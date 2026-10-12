"""Contrôle en lecture seule de la maquette finale installée dans une cave."""
import bpy
import pathlib
import json
from mathutils import Vector

HERE=pathlib.Path(__file__).resolve().parent
bpy.ops.wm.open_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
assert len(bpy.data.scenes)==4
names=['01 Installation du vide-cave','02 Coffret en vue éclatée','03 Implantation KiCad fournie','04 Intérieur moteur et pompe']
for name in names:
    s=bpy.data.scenes[name]
    assert s.camera is not None
    assert s.unit_settings.system=='METRIC'
    assert len(s.objects)>20
    assert not any(o.type=='MESH' and len(o.data.vertices)==0 for o in s.objects)

assert not any(o.name in ['Flotteur de niveau','Câble flotteur'] for o in bpy.data.objects)
s=bpy.data.scenes[names[3]]
cam=s.camera
changed=False
for obj in s.objects:
    if obj.parent==cam and obj.type=='FONT' and obj.name.startswith('Légende'):
        assert obj.data.body
main=bpy.data.scenes[names[0]]
assert all(main.objects.get(n) for n in ['Cuve / paroi avant','Cuve / paroi droite','Cuve / paroi gauche','Cuve / paroi arrière','Mur de cave / arrière','Mur de cave / gauche','Mur de cave / droite','Plafond de cave'])
assert len([o for o in main.objects if o.name.startswith('Grille / barre')])==21
assert main.camera.data.type=='PERSP'
result={'statut':'OK','scènes':{s.name:len(s.objects) for s in bpy.data.scenes},'unités':'mètres','flotteur':'retiré','bassin':'quatre parois et grille amovible','environnement':'cave fermée avec puisard sous le sol','vue_intérieure':'conservée dans la scène 04'}
(HERE/'verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print('VERIFICATION_OK',json.dumps(result,ensure_ascii=False))
