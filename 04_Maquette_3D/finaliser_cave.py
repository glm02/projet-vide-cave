"""Rectifie le tube rigide et vérifie les quatre scènes du fichier enregistré."""
import bpy,math,pathlib,ast,json
from mathutils import Vector
HERE=pathlib.Path(__file__).resolve().parent
bpy.ops.wm.open_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
scene=bpy.data.scenes['01 Installation du vide-cave'];bpy.context.window.scene=scene
tree=ast.parse((HERE/'creer_maquette.py').read_text(encoding='utf-8'))
api={'bpy':bpy,'math':math,'json':json,'pathlib':pathlib,'Vector':Vector,'PI':math.pi,'HERE':HERE,'M':{m.name:m for m in bpy.data.materials},'scene':scene}
exec(compile(ast.Module(body=[n for n in tree.body if isinstance(n,ast.FunctionDef)],type_ignores=[]),'<fonctions maquette>','exec'),api)
obj=scene.objects['Refoulement / dans la cave'];bpy.data.objects.remove(obj,do_unlink=True)
api['group']('35 Coffret / fixations au mur')
for sx in [-1,1]:
    for sz in [-1,1]:api['cylinder']('Entretoise / coffret mural',(.52+sx*.085,.766,1.16+sz*.127),.006,.068,'Inox','Y')
pts=[(-.127,-.055,.168),(-.070,-.055,.168)]
for i in range(1,25):
    a=math.pi/2*i/24;pts.append((-.070+.025*math.sin(a),-.055,.193-.025*math.cos(a)))
pts.append((-.045,-.055,.91))
for i in range(1,25):
    a=math.pi/2*i/24;pts.append((-.045,.065-.12*math.cos(a),.91+.12*math.sin(a)))
pts.append((-.045,.54,1.03))
for i in range(1,25):
    a=math.pi/2*i/24;pts.append((.075-.12*math.cos(a),.54+.12*math.sin(a),1.03))
pts.append((1.28,.66,1.03))
api['group']('52 Refoulement / tube rigide')
api['tube']('Refoulement / dans la cave',pts,.018,'Graphite',False)
for obj in scene.objects:
    if obj.name.startswith('Collier / mur'):obj.location.z=1.03
    if obj.name.startswith('Collier / fixation mur'):obj.location.y=.735;obj.location.z=1.03;obj.dimensions.y=.13
prefs=bpy.context.preferences.filepaths;prefs.save_version=0
if hasattr(prefs,'file_preview_type') and 'NONE' in [v.identifier for v in prefs.bl_rna.properties['file_preview_type'].enum_items]:prefs.file_preview_type='NONE'
scene.render.filepath=str(HERE/'01_installation.png')
bpy.ops.render.render(write_still=True)
bpy.ops.wm.save_as_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
exec(compile((HERE/'verifier_maquette.py').read_text(encoding='utf-8'),str(HERE/'verifier_maquette.py'),'exec'))
