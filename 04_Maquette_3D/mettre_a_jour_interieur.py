"""Met à jour le .blend existant en conservant ses trois scènes."""
import ast
import bpy
import pathlib
import math
import json
import sys
from mathutils import Vector

HERE=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(HERE))
from interieur_pompe import ajouter_interieur
bpy.ops.wm.open_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
# Réutiliser les fonctions de construction sans relancer le générateur ni réinitialiser Blender.
tree=ast.parse((HERE/'creer_maquette.py').read_text(encoding='utf-8'))
api={'bpy':bpy,'math':math,'json':json,'pathlib':pathlib,'Vector':Vector,'PI':math.pi,'HERE':HERE,'M':{m.name:m for m in bpy.data.materials}}
exec(compile(ast.Module(body=[n for n in tree.body if isinstance(n,ast.FunctionDef)],type_ignores=[]),'<fonctions maquette>','exec'),api)
if bpy.data.scenes.get('04 Intérieur moteur et pompe'):
    bpy.data.scenes.remove(bpy.data.scenes['04 Intérieur moteur et pompe'])
coupe=ajouter_interieur(api)
notice=bpy.data.texts.get('LISEZ-MOI / sources et hypothèses')
if notice:notice.clear();notice.write((HERE/'LISEZ_MOI.md').read_text(encoding='utf-8'))
bpy.context.preferences.filepaths.save_version=0
prefs=bpy.context.preferences.filepaths
if hasattr(prefs,'file_preview_type') and 'NONE' in [v.identifier for v in prefs.bl_rna.properties['file_preview_type'].enum_items]:prefs.file_preview_type='NONE'
for s,filename in [(coupe,'04_interieur_moteur_pompe.png'),(bpy.data.scenes['01 Installation du vide-cave'],'01_installation.png')]:
    bpy.context.window.scene=s
    s.render.filepath=str(HERE/filename)
    bpy.ops.render.render(write_still=True)
bpy.context.window.scene=coupe
for screen in bpy.data.screens:
    for area in screen.areas:
        if area.type=='VIEW_3D':area.spaces.active.region_3d.view_perspective='CAMERA'
assert len(bpy.data.scenes)==4
assert not any('flotteur' in o.name.casefold() for s in bpy.data.scenes for o in s.objects)
bpy.ops.wm.save_as_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
result={'statut':'OK','scènes':{s.name:len(s.objects) for s in bpy.data.scenes},'flotteur':'retiré','vue_intérieure':'coupe pédagogique générique','unités':'mètres'}
(HERE/'verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print('INTERIEUR_TERMINE',json.dumps(result,ensure_ascii=False))
