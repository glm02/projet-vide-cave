"""Lecture seule du fichier déplacé dans Blender; aucune sauvegarde."""
import bpy,json,pathlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
assert len(bpy.data.scenes)==4
missing=[]
for img in bpy.data.images:
    if img.source=='FILE' and not img.packed_file and img.filepath:
        if not pathlib.Path(bpy.path.abspath(img.filepath)).exists():missing.append(img.filepath)
for lib in bpy.data.libraries:
    if not pathlib.Path(bpy.path.abspath(lib.filepath)).exists():missing.append(lib.filepath)
assert not missing,missing
result={'ouverture_blender':'OK','scenes':list(bpy.data.scenes.keys()),'ressources_manquantes':missing,'aucune_modification_du_modele':True}
(ROOT/'06_Archives_et_exports/Rangement/verification_blender.json').write_text(json.dumps(result,indent=2,ensure_ascii=False),encoding='utf8')
print(json.dumps(result,ensure_ascii=False))
