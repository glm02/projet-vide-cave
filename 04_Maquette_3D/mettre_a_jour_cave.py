"""Met à jour l'installation dans le fichier Blender existant."""
import ast
import bpy
import pathlib
import math
import json
import sys
from mathutils import Vector
HERE=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(HERE))
from installer_dans_cave import installer_cave
bpy.ops.wm.open_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
tree=ast.parse((HERE/'creer_maquette.py').read_text(encoding='utf-8'))
api={'bpy':bpy,'math':math,'json':json,'pathlib':pathlib,'Vector':Vector,'PI':math.pi,'HERE':HERE,'M':{m.name:m for m in bpy.data.materials}}
exec(compile(ast.Module(body=[n for n in tree.body if isinstance(n,ast.FunctionDef)],type_ignores=[]),'<fonctions maquette>','exec'),api)
scene=installer_cave(api)
notice=bpy.data.texts.get('LISEZ-MOI / sources et hypothèses')
if notice:notice.clear();notice.write((HERE/'LISEZ_MOI.md').read_text(encoding='utf-8'))
prefs=bpy.context.preferences.filepaths;prefs.save_version=0
if hasattr(prefs,'file_preview_type') and 'NONE' in [v.identifier for v in prefs.bl_rna.properties['file_preview_type'].enum_items]:prefs.file_preview_type='NONE'
scene.render.filepath=str(HERE/'01_installation.png')
bpy.ops.render.render(write_still=True)
for scr in bpy.data.screens:
    for area in scr.areas:
        if area.type=='VIEW_3D':area.spaces.active.region_3d.view_perspective='CAMERA'
assert len(bpy.data.scenes)==4
assert all(scene.objects.get(name) for name in ['Cuve / paroi avant','Cuve / paroi droite','Cuve / paroi gauche','Cuve / paroi arrière'])
assert not any(o.name in ['Flotteur de niveau','Câble flotteur'] for o in bpy.data.objects)
bpy.ops.wm.save_as_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
result={'statut':'OK','scènes':{s.name:len(s.objects) for s in bpy.data.scenes},'bassin':'quatre parois et grille amovible','environnement':'cave fermée, puisard sous le sol','flotteur':'retiré','vue_intérieure':'conservée dans la scène 04'}
(HERE/'verification.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print('CAVE_TERMINEE',json.dumps(result,ensure_ascii=False))
