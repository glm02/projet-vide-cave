"""Maquette Blender du vide-cave GEII. Exécution : blender -b --python creer_maquette.py.
Unités : mètres. Les dimensions mécaniques sont des propositions de présentation.
L'implantation et les pistes de la scène KiCad sont issues du fichier source.
"""
import bpy
import math
import json
import pathlib
from mathutils import Vector

HERE = pathlib.Path(__file__).resolve().parent
PI = math.pi
KICAD = json.loads((HERE/'implantation_kicad.json').read_text(encoding='utf-8'))
bpy.ops.wm.read_factory_settings(use_empty=True)
M = {}

def material(name, color, metal=0, rough=.4, emission=0, alpha=1):
    m = bpy.data.materials.new(name)
    m.diffuse_color = (*color, alpha)
    m.use_nodes = True
    p = next(n for n in m.node_tree.nodes if n.type=='BSDF_PRINCIPLED')
    p.inputs['Base Color'].default_value = (*color,alpha)
    p.inputs['Metallic'].default_value = metal
    p.inputs['Roughness'].default_value = rough
    p.inputs['Alpha'].default_value = alpha
    if emission:
        p.inputs['Emission Color'].default_value = (*color,1)
        p.inputs['Emission Strength'].default_value = emission
    if alpha < 1:
        m.surface_render_method = 'DITHERED'
    M[name] = m
    return m

material('Graphite',(.018,.029,.042),.15,.3)
material('Coque bleue',(.015,.19,.29),.35,.26)
material('Orange',(.95,.22,.035),.1,.3)
material('Inox',(.48,.57,.63),.8,.25)
material('Noir',(.009,.013,.017),.05,.34)
material('ABS clair',(.72,.78,.8),.08,.32)
material('Blanc',(.85,.93,.95),.1,.45)
material('Béton',(.25,.3,.33),0,.88)
material('Béton clair',(.44,.49,.49),0,.85)
material('Eau',(.018,.42,.57),.12,.13,0,.22)
material('Eau tranche',(.025,.35,.5),.05,.2,0,.15)
material('PCB',(.02,.17,.095),.25,.42)
material('Cuivre',(.72,.43,.16),.8,.25)
material('Piste',(.045,.28,.16),.2,.5)
material('Résistance',(.64,.48,.3),0,.4)
material('Rouge',(.82,.025,.025),.1,.3, .3)
material('Vert',(.035,.82,.3),.1,.22,2)
material('LCD',(.012,.07,.08),0,.28)
material('Texte LCD',(.25,.93,.67),0,.3,1.4)
material('Bleu pâle',(.12,.55,.7),0,.35, .5)
material('Laiton',(.65,.48,.22),.7,.3)
material('Sol',(.025,.04,.059),.05,.65)
material('Cartouche blanc',(.85,.93,.95),0,.5,1)
material('Cartouche bleu',(.12,.55,.7),0,.5,1)

scene = None
collection = None

def group(name):
    global collection
    collection = bpy.data.collections.new(name)
    scene.collection.children.link(collection)
    return collection

def assign(obj,name,mat):
    obj.name = name
    for c in list(obj.users_collection):
        c.objects.unlink(obj)
    collection.objects.link(obj)
    if mat:
        obj.data.materials.append(M[mat])
    return obj

def bevel(obj,width=.002,segments=3):
    mod = obj.modifiers.new('Arêtes arrondies','BEVEL')
    mod.width = width
    mod.segments = segments
    mod = obj.modifiers.new('Normales','WEIGHTED_NORMAL')
    return obj

def box(name,loc,size,mat='Graphite',radius=.002):
    bpy.ops.mesh.primitive_cube_add(size=1,location=loc)
    o=assign(bpy.context.object,name,mat)
    o.dimensions=size
    bpy.ops.object.transform_apply(location=False,rotation=False,scale=True)
    if radius:
        bevel(o,radius)
    return o

def cut_box(obj,loc,size):
    cutter=box('Découpe temporaire',loc,size,None,0)
    bpy.context.view_layer.objects.active=obj
    mod=obj.modifiers.new('Ouverture','BOOLEAN')
    mod.operation='DIFFERENCE'
    mod.solver='EXACT'
    mod.object=cutter
    bpy.ops.object.modifier_apply(modifier=mod.name)
    bpy.data.objects.remove(cutter,do_unlink=True)
    return obj

def cylinder(name,loc,r,depth,mat='Inox',axis='Z',vertices=48):
    bpy.ops.mesh.primitive_cylinder_add(vertices=vertices,radius=r,depth=depth,location=loc)
    o=assign(bpy.context.object,name,mat)
    if axis=='Y': o.rotation_euler[0]=PI/2
    if axis=='X': o.rotation_euler[1]=PI/2
    bevel(o,min(.0015,r*.1),2)
    for p in o.data.polygons: p.use_smooth=True
    return o

def sphere(name,loc,size,mat):
    bpy.ops.mesh.primitive_uv_sphere_add(segments=32,ring_count=16,location=loc)
    o=assign(bpy.context.object,name,mat)
    o.scale=size
    for p in o.data.polygons: p.use_smooth=True
    return o

def torus(name,loc,major,minor,mat='Inox',axis='Z'):
    bpy.ops.mesh.primitive_torus_add(major_segments=48,minor_segments=10,location=loc,major_radius=major,minor_radius=minor)
    o=assign(bpy.context.object,name,mat)
    if axis=='Y':o.rotation_euler[0]=PI/2
    if axis=='X':o.rotation_euler[1]=PI/2
    for p in o.data.polygons:p.use_smooth=True
    return o

def tube(name,points,r=.004,mat='Noir',smooth=True):
    curve=bpy.data.curves.new(name,'CURVE')
    curve.dimensions='3D'
    curve.resolution_u=16
    curve.bevel_depth=r
    curve.bevel_resolution=3
    if smooth:
        s=curve.splines.new('BEZIER')
        s.bezier_points.add(len(points)-1)
        for p,co in zip(s.bezier_points,points):
            p.co=co
            p.handle_left_type=p.handle_right_type='AUTO'
    else:
        s=curve.splines.new('POLY')
        s.points.add(len(points)-1)
        for p,co in zip(s.points,points):p.co=(*co,1)
    o=bpy.data.objects.new(name,curve)
    collection.objects.link(o)
    curve.materials.append(M[mat])
    return o

def text(name,body,loc,size=.016,mat='Blanc',rot=(PI/2,0,0),align='LEFT'):
    d=bpy.data.curves.new(name,'FONT')
    d.body=body
    d.size=size
    d.align_x=align
    d.extrude=.000025
    d.space_character=1.1
    o=bpy.data.objects.new(name,d)
    collection.objects.link(o)
    o.location=loc
    o.rotation_euler=rot
    d.materials.append(M[mat])
    return o

def bolt(name,loc,r=.0023,axis='Y'):
    cylinder(name,loc,r,.0016,'Inox',axis,24)
    x,y,z=loc
    if axis=='Y':box(name+' empreinte',(x,y-.001,z),(r*1.1,.0005,.0004),'Noir',0)
    else:box(name+' empreinte',(x,y,z+.001),(r*1.1,.0004,.0005),'Noir',0)

def new_scene(name):
    global scene
    scene=bpy.data.scenes.new(name)
    bpy.context.window.scene=scene
    scene.unit_settings.system='METRIC'
    scene.unit_settings.length_unit='METERS'
    scene.render.engine='CYCLES'
    scene.cycles.samples=40
    scene.cycles.use_denoising=True
    scene.render.resolution_x=1600
    scene.render.resolution_y=1200
    scene.render.resolution_percentage=100
    scene.render.image_settings.file_format='PNG'
    scene.world=bpy.data.worlds.new(name+' monde')
    scene.world.use_nodes=True
    bg=next(n for n in scene.world.node_tree.nodes if n.type=='BACKGROUND')
    bg.inputs[0].default_value=(.09,.13,.18,1)
    bg.inputs[1].default_value=.45
    scene.view_settings.view_transform='AgX'
    scene.render.film_transparent=False
    return scene

def setup_camera(loc,target,scale):
    group('90 Présentation / éclairage')
    d=bpy.data.cameras.new('Caméra')
    cam=bpy.data.objects.new('Caméra',d)
    collection.objects.link(cam)
    cam.location=loc
    cam.rotation_euler=(Vector(target)-cam.location).to_track_quat('-Z','Y').to_euler()
    d.type='ORTHO'
    d.ortho_scale=scale
    d.clip_start=.001
    scene.camera=cam
    def area(name,loc,energy,size,color):
        light=bpy.data.lights.new(name,'AREA')
        light.energy=energy
        light.shape='DISK'
        light.size=size
        light.color=color
        o=bpy.data.objects.new(name,light)
        collection.objects.link(o)
        o.location=loc
        o.rotation_euler=(Vector(target)-o.location).to_track_quat('-Z','Y').to_euler()
    unit=scale/1.8
    area('Softbox principal',(-1.2*unit,-1.8*unit,2.6*unit),230*unit*unit,1.9*unit,(.79,.9,1))
    area('Softbox chaud',(1.5*unit,.7*unit,1.9*unit),180*unit*unit,1.4*unit,(1,.83,.65))
    area('Lumière de face',(1.1*unit,-1.4*unit,1.1*unit),70*unit*unit,1.1*unit,(.66,.85,1))
    return cam

def overlay(cam,title,subtitle,footnote,scale):
    # Texte réellement éditable dans Blender, fixé dans le plan de la caméra.
    w=scale
    h=scale*scene.render.resolution_y/scene.render.resolution_x
    for body,x,y,size,mat in [(title,-.44*w,.40*h,.038*w,'Cartouche blanc'),(subtitle,-.44*w,.335*h,.011*w,'Cartouche bleu'),(footnote,-.44*w,-.435*h,.009*w,'Cartouche blanc')]:
        o=text('Cartouche '+body[:20],body,(0,0,0),size,mat,(0,0,0))
        o.parent=cam
        o.location=(x,y,-.02)
        o.visible_shadow=False
        o.visible_diffuse=False
        o.visible_glossy=False

def screen(c,y,z,w=.084):
    x=c
    box('LCD 16 × 2 / module',(x,y+.004,z),(w+.015,.012,.043),'PCB',.001)
    box('Cadre LCD',(x,y-.003,z),(w,.008,.034),'Noir',.0015)
    box('Surface LCD',(x,y-.008,z),(w-.009,.001,.024),'LCD',.001)
    text('LCD ligne 1','POMPAGE  : OK',(x-w*.42,y-.0088,z+.003),.0055,'Texte LCD')
    text('LCD ligne 2','AUTO TEST : 3 J',(x-w*.42,y-.0088,z-.006),.0048,'Texte LCD')

def housing(origin,exploded=False):
    x,y,z=origin
    w,h,d=.19,.245,.075
    group('30 Coffret de surveillance / proposition')
    # Coque creuse, ouverture en face avant.
    shell=box('Coffret / coque creuse',(x,y,z),(w,d,h),'ABS clair',0)
    cut_box(shell,(x,y-.006,z),(w-.012,d,h-.012))
    bevel(shell,.002)
    for sx in [-1,1]:
        for sz in [-1,1]:
            box('Patte de fixation',(x+sx*.085,y+.04,z+sz*.127),(.02,.005,.018),'Graphite',.002)
            bolt('Vis fixation',(x+sx*.085,y+.036,z+sz*.127),.002)
    group('31 Électronique / volumes fonctionnels')
    board_y=y+.013
    box('Carte de commande / volume simplifié',(x,board_y,z+.006),(.12,.0016,.105),'PCB',.001)
    for sx in [-1,1]:
        for sz in [-1,1]:
            cylinder('Entretoise M3',(x+sx*.052,board_y+.008,z+.006+sz*.044),.003,.014,'Laiton','Y')
            bolt('Vis carte',(x+sx*.052,board_y-.002,z+.006+sz*.044),.002)
    box('PIC16F1788 / boîtier DIP',(x+.009,board_y-.005,z+.004),(.036,.008,.01),'Noir',.001)
    for i in range(14):
        for sz in [-1,1]:
            box('PIC / patte',(x-.007+i*.00254,board_y-.003,z+.004+sz*.006),(.0007,.003,.003),'Inox',.00015)
    text('PIC référence','PIC16F1788',(x-.006,board_y-.01,z+.003),.0024)
    box('ACS756 / capteur de courant',(x-.036,board_y-.008,z+.032),(.023,.012,.011),'Noir',.001)
    text('Capteur référence','ACS756',(x-.046,board_y-.015,z+.032),.003)
    box('FODM3011 / optotriac',(x+.041,board_y-.005,z+.032),(.011,.007,.008),'Noir',.0006)
    box('Triac / commande moteur',(x+.038,board_y-.01,z-.032),(.015,.013,.02),'Noir',.001)
    box('Dissipateur / volume indicatif',(x+.047,board_y+.0,z-.032),(.012,.02,.035),'Inox',.001)
    for i in range(5):box('Ailette dissipateur',(x+.042+i*.0025,board_y-.01,z-.032),(.0008,.02,.035),'Inox',.0001)
    for i in range(3):
        cylinder('Condensateur',(x-.036+i*.014,board_y-.007,z-.026),.003,.012,'Graphite','Y')
    for j in range(2):
        box('Bornier '+str(j),(x-.045+j*.085,board_y-.008,z-.06),(.025,.015,.012),'Coque bleue',.001)
        for i in range(2):bolt('Bornier vis',(x-.051+j*.085+i*.012,board_y-.016,z-.06),.002)
    box('Alimentation basse tension / volume proposé',(x,y+.008,z-.084),(.071,.032,.025),'Graphite',.002)
    text('Alimentation référence','ALIM. BT',(x-.025,y-.009,z-.084),.004)
    tube('Faisceau LCD',[(x-.012,board_y,z+.04),(x-.05,y-.02,z+.07),(x-.05,y-.055,z+.07)],.002,'Orange')
    group('32 Face avant / LCD / alertes')
    face_y=y-d/2-(.115 if exploded else 0)
    front=[]
    before=set(collection.objects)
    plate=box('Face avant / fenêtre LCD',(x,face_y,z),(w,.008,h),'Graphite',0)
    cut_box(plate,(x,face_y,z+.064),(.086,.04,.025))
    bevel(plate,.0015)
    text('Nom face avant','VIDE-CAVE',(x-.076,face_y-.0048,z+.103),.011)
    screen(x,face_y-.004,z+.064,.092)
    text('Mention face avant','SURVEILLANCE MOTEUR',(x-.071,face_y-.0048,z+.024),.005)
    for dx,mat,label in [(-.052,'Vert','MARCHE'),(0,'Orange','TEST'),(.052,'Rouge','DÉFAUT')]:
        cylinder('Voyant / embase',(x+dx,face_y-.006,z-.004),.0075,.004,'Inox','Y')
        sphere('Voyant '+label,(x+dx,face_y-.009,z-.004),(.0048,.003,.0048),mat)
        text('Voyant / légende',label,(x+dx,face_y-.005,z-.021),.0036,align='CENTER')
    cylinder('Bouton test / collerette',(x-.045,face_y-.006,z-.048),.01,.005,'Inox','Y')
    cylinder('Bouton test',(x-.045,face_y-.01,z-.048),.008,.008,'Orange','Y')
    cylinder('Bouton acquittement / collerette',(x+.016,face_y-.006,z-.048),.01,.005,'Inox','Y')
    cylinder('Bouton acquittement',(x+.016,face_y-.01,z-.048),.008,.008,'Noir','Y')
    text('Bouton test / texte','TEST',(x-.045,face_y-.005,z-.068),.004,align='CENTER')
    text('Acquittement / texte','ACQUIT.',(x+.016,face_y-.005,z-.068),.004,align='CENTER')
    cylinder('Buzzer / grille',(x+.065,face_y-.005,z-.05),.013,.004,'Noir','Y')
    for i in range(5):box('Fente buzzer',(x+.065,face_y-.008,z-.05+(i-2)*.004),(.018,.001,.0012),'Inox',.0003)
    text('Auto-test / spécification','AUTO-TEST : 3 J / 1 MIN MAX',(x-.072,face_y-.005,z-.101),.004)
    for sx in [-1,1]:
        for sz in [-1,1]:bolt('Vis face avant',(x+sx*.081,face_y-.006,z+sz*.108))
    front=list(set(collection.objects)-before)
    for o in front:o['hypothèse']='Face avant proposée, dimensions non validées'
    group('33 Raccordements / presse-étoupes')
    for dx,label in [(-.053,'Secteur'),(.053,'Pompe')]:
        cylinder('Presse-étoupe '+label,(x+dx,y,z-h/2-.01),.011,.02,'Graphite')
        cylinder('Écrou '+label,(x+dx,y,z-h/2-.001),.013,.007,'Noir',vertices=6)
        torus('Joint '+label,(x+dx,y,z-h/2-.019),.006,.002,'Noir')
    if exploded:
        for sx in [-1,1]:
            for sz in [-1,1]:tube('Axe de remontage',[(x+sx*.081,y-d/2,z+sz*.108),(x+sx*.081,face_y+.004,z+sz*.108)],.00035,'Bleu pâle',False)
    return front

def pump():
    group('20 Pompe vide-cave / modèle de présentation')
    x,y=-.28,-.055
    cylinder('Socle / crépine',(x,y,.11),.09,.076,'Graphite')
    cylinder('Couronne inférieure',(x,y,.077),.095,.015,'Noir')
    cylinder('Volute',(x,y,.154),.087,.021,'Coque bleue')
    cylinder('Moteur / corps inox',(x,y,.238),.071,.16,'Inox')
    cylinder('Capot moteur',(x,y,.327),.074,.023,'Coque bleue')
    torus('Joint capot',(x,y,.315),.072,.003,'Noir')
    for i in range(18):
        a=2*PI*i/18
        o=box('Ouverture crépine',(x+.091*math.cos(a),y+.091*math.sin(a),.111),(.007,.011,.039),'Noir',.0015)
        o.rotation_euler[2]=a
    for i in range(3):
        a=2*PI*i/3
        box('Pied',(x+.067*math.cos(a),y+.067*math.sin(a),.072),(.027,.028,.022),'Noir',.003)
    # Poignée en U, section rectangulaire.
    for sx in [-1,1]:box('Poignée / montant',(x+sx*.055,y,.363),(.017,.019,.066),'Orange',.006)
    box('Poignée / traverse',(x,y,.395),(.125,.019,.022),'Orange',.007)
    for sx in [-1,1]:bolt('Poignée vis',(x+sx*.055,y-.011,.342),.003)
    box('Plaque moteur',(x,y-.071,.258),(.065,.002,.044),'Graphite',.002)
    text('Plaque pompe','POMPE',(x-.028,y-.073,.264),.008)
    text('Tension pompe','230 V ~',(x-.028,y-.073,.249),.007)
    cylinder('Sortie pompe',(x+.095,y,.168),.023,.079,'Coque bleue','X')
    cylinder('Raccord laiton',(x+.138,y,.168),.025,.022,'Laiton','X',6)
    tube('Refoulement / conduite',[(x+.153,y,.168),(x+.205,y,.168),(x+.235,y,.205),(x+.235,y,.7),(x+.26,y,.78),(x+.36,y,.79),(x+.57,y,.79)],.018,'Graphite')
    for z in [.26,.57]:
        torus('Collier conduite',(x+.235,y,z),.019,.003,'Inox')
    tube('Câble alimentation pompe',[(x+.04,y,.336),(x+.115,y+.045,.34),(.05,.11,.42),(.21,.2,.50),(.43,.18,.47),(.573,.19,.53),(.573,.19,.563)],.0045)
    for obj in collection.objects:obj['statut']='Pompe générique sans flotteur, dimensions proposées'

# SCÈNE 1 : installation dans une cuve en coupe.
s1=new_scene('01 Installation du vide-cave')
group('10 Cuve en coupe / eau')
box('Socle de présentation',(0,.01,.011),(1.5,.98,.04),'Graphite',.035)
box('Cuve / fond',(-.29,.00,.043),(.78,.69,.036),'Béton',.005)
box('Cuve / paroi arrière',(-.29,.332,.244),(.78,.026,.38),'Béton',.003)
box('Cuve / paroi gauche',(-.668,.00,.244),(.025,.69,.38),'Béton',.003)
box('Cuve / arête arrière',(-.29,.332,.44),(.80,.04,.018),'Béton clair',.003)
box('Cuve / arête gauche',(-.668,.00,.44),(.04,.71,.018),'Béton clair',.003)
box('Eau / surface',(-.29,-.008,.204),(.72,.64,.001),'Eau',0)
box('Eau / coupe avant',(-.29,-.329,.134),(.72,.001,.14),'Eau tranche',0)
box('Eau / coupe droite',(.07,-.008,.134),(.001,.64,.14),'Eau tranche',0)
for i in range(4):torus('Ondulation eau',(-.28,-.055,.205+i*.0002),.116+i*.033,.0005,'Bleu pâle')
for i in range(5):
    box('Graduation de cuve',(-.64,-.337,.10+i*.065),(.018,.001,.001),'Blanc',0)
pump()
group('11 Support coffret')
box('Support du coffret',(.52,.255,.479),(.29,.035,.84),'Béton',.006)
box('Pied support',(.52,.23,.06),(.36,.21,.05),'Béton clair',.008)
housing((.52,.19,.704))
group('34 Cheminement électrique')
tube('Arrivée secteur',[(.467,.19,.563),(.46,.20,.40),(.62,.24,.27),(.67,.26,.09)],.0045)
group('80 Repères')
text('Repère cuve','01  POMPE 230 V',(-.61,-.355,.047),.022,'Blanc')
text('Repère coffret','02  SURVEILLANCE',(.31,-.22,.055),.019,'Blanc')
text('Mention coupe','CUVE EN COUPE',(-.58,.315,.447),.014,'Blanc',rot=(0,0,0))
group('99 Sol studio')
box('Sol studio',(0,0,-.027),(200,200,.02),'Sol',0)
cam=setup_camera((1.5,-2.8,1.8),(-.01,.03,.47),1.82)
overlay(cam,'VIDE-CAVE','MAQUETTE DU SYSTÈME DE SURVEILLANCE  /  PROJET GEII','Pompe en coupe, refoulement et coffret  •  Dimensions mécaniques proposées',1.82)
s1['hypothèses']='Pompe générique Ø 190 mm, hauteur 330 mm ; cuve 780 × 690 mm ; coffret 190 × 245 × 75 mm. Aucune cote mécanique fournie.'
s1['sources']='Rapport_pre-etude_vide-cave.docx ; synoptiques drawio ; projet_vide_cave_geii.pptx ; schéma KiCad G206_CAMEL_TP_CAO.'

# SCÈNE 2 : vue de la coque creuse et de son électronique.
s2=new_scene('02 Coffret en vue éclatée')
housing((0,0,.145),True)
group('99 Sol studio')
box('Socle coffret',(0,-.05,.005),(.31,.31,.01),'Sol',.004)
box('Sol studio',(0,0,-.008),(200,200,.01),'Sol',0)
cam=setup_camera((.53,-.80,.54),(0,-.043,.191),.63)
overlay(cam,'LE COFFRET','LCD 16 × 2  /  PIC16F1788  /  ACS756  /  COMMANDE TRIAC','Vue éclatée  •  Coffret 190 × 245 × 75 mm proposé  •  Implantation intérieure simplifiée',.63)
s2['hypothèses']='Implantation fonctionnelle simplifiée, hors plan de fabrication. Dimensions des composants et du coffret à valider.'

# SCÈNE 3 : restitution fidèle des positions, du contour et des pistes KiCad.
s3=new_scene('03 Implantation KiCad fournie')
group('40 Carte KiCad / contour source')
ox,oy,ex,ey=KICAD['outline']
cx,cy=(ox+ex)/2,(oy+ey)/2
def xy(p):return ((p[0]-cx)/1000,(cy-p[1])/1000)
box('PCB / contour 90 × 80 mm',(0,0,.004),((ex-ox)/1000,(ey-oy)/1000,.0016),'PCB',.0003)
group('41 Cuivre / pistes de la face arrière')
for i,seg in enumerate(KICAD['segments']):
    if seg['layer']!='B.Cu':continue
    a,b=xy(seg['start']),xy(seg['end'])
    tube('Piste KiCad '+str(i),[(*a,.00310),(*b,.00310)],max(seg['width']/2000,.00008),'Piste',False)
group('42 Composants / positions source')
for fp in KICAD['footprints']:
    x,y=xy(fp['at'])
    angle=math.radians(fp['at'][2] if len(fp['at'])>2 else 0)
    name=fp['name'].split(':')[-1]
    ref=fp['ref']
    # KiCad exprime l'angle du composant dans le sens opposé aux axes écran Y.
    def position(px,py,pz):
        return (x+px*math.cos(angle)-py*math.sin(angle),y+px*math.sin(angle)+py*math.cos(angle),pz)
    padcoords=[]
    for pad in fp['pads']:
        p=pad['at']
        co=position(p[0]/1000,-p[1]/1000,.0049)
        padcoords.append(co)
        po=box(ref+' / pastille '+pad['number'],co,(pad['size'][0]/1000,pad['size'][1]/1000,.00015),'Cuivre',.00015)
        po.rotation_euler[2]=angle
    if not padcoords:continue
    mid=Vector((sum(c[0] for c in padcoords)/len(padcoords),sum(c[1] for c in padcoords)/len(padcoords),.005))
    o=None
    if 'R_Axial' in name:
        a,b=Vector(padcoords[0]),Vector(padcoords[-1])
        tube(ref+' / fils',[(a.x,a.y,.005),(a.x,a.y,.0075),(b.x,b.y,.0075),(b.x,b.y,.005)],.00028,'Inox',False)
        o=cylinder(ref+' / résistance',(mid.x,mid.y,.0075),.0011,.0063,'Résistance')
        o.rotation_euler=(b-a).to_track_quat('Z','Y').to_euler()
        for i in [-1,0,1]:
            ring=torus(ref+' / bague',Vector((mid.x,mid.y,.0075))+(b-a).normalized()*(i*.001),.0011,.00018,'Noir')
            ring.rotation_euler=o.rotation_euler
    elif 'DIP-28' in name:
        o=box(ref+' / PIC16F1788',(mid.x,mid.y,.0085),(.009,.035,.006),'Noir',.0005)
        o.rotation_euler[2]=angle
        label=text('Référence PIC','PIC16F1788',(mid.x-.008,mid.y,.012),.0015,'Blanc',(0,0,0))
    elif 'SMDIP' in name:
        o=box(ref+' / optotriac',(mid.x,mid.y,.0078),(.007,.005,.005),'Noir',.0005)
        o.rotation_euler[2]=angle
    elif name=='162B':
        # Corps positionné selon l'empreinte fournie, qui place la rangée de broches en bordure.
        center=position(0,-.018,.011)
        o=box(ref+' / LCD / corps approximatif',center,(.08,.036,.013),'Noir',.001)
        o.rotation_euler[2]=angle
        box('DS1 / afficheur',(center[0],center[1],.018),(.064,.025,.001),'LCD',.0005)
        text('DS1 / exemple','VIDE-CAVE',(center[0]-.028,center[1]-.003,.019),.004,'Texte LCD',(0,0,0))
    elif 'LED' in name:
        color='Rouge' if 'R' in fp['value'] else 'Vert'
        cylinder(ref+' / LED corps',(mid.x,mid.y,.007),.0025,.004,color)
        o=sphere(ref+' / LED tête',(mid.x,mid.y,.009),(.0025,.0025,.0025),color)
    elif 'CP_Radial' in name:
        o=cylinder(ref+' / électrolytique',(mid.x,mid.y,.010),.00315,.010,'Graphite')
        cylinder(ref+' / dessus',(mid.x,mid.y,.0151),.0028,.0003,'Inox')
    elif 'C_Disc' in name:
        o=sphere(ref+' / condensateur',(mid.x,mid.y,.008),(.0025,.0012,.003),'Orange')
        o.rotation_euler[2]=angle
    elif 'SW_TH' in name:
        o=box(ref+' / bouton',(mid.x,mid.y,.0065),(.006,.006,.003),'Inox',.0004)
        cylinder(ref+' / poussoir',(mid.x,mid.y,.009),.0015,.003,'Noir')
    elif 'Bornier' in name:
        o=box(ref+' / bornier',(mid.x,mid.y,.009),(.01,.0075,.008),'Coque bleue',.0005)
        o.rotation_euler[2]=angle
        for p in padcoords:bolt(ref+' / vis',(p[0],p[1],.0135),.0015,'Z')
    elif 'Allegro' in name:
        o=box(ref+' / ACS756 / hors contour',(mid.x,mid.y,.011),(.018,.014,.012),'Noir',.0006)
    elif 'TestPoint' in name:
        o=torus(ref+' / boucle',(mid.x,mid.y,.007),.0012,.00025,'Inox','Y')
    elif 'Potentiometer' in name:
        o=box(ref+' / potentiomètre',(mid.x,mid.y,.008),(.006,.006,.006),'Coque bleue',.0004)
        bolt(ref+' / réglage',(mid.x,mid.y,.0115),.0015,'Z')
    else:
        spanx=max(c[0] for c in padcoords)-min(c[0] for c in padcoords)+.004
        spany=max(c[1] for c in padcoords)-min(c[1] for c in padcoords)+.004
        o=box(ref+' / connecteur',(mid.x,mid.y,.007),(spanx,spany,.004),'Noir',.0003)
        for p in padcoords:cylinder(ref+' / broche',(p[0],p[1],.010),.0004,.007,'Laiton',vertices=12)
    if o:
        o['référence']=ref
        o['valeur']=fp['value']
        o['source']=KICAD['source']
        o['géométrie']='Corps approché ; position issue de KiCad'
    text(ref+' / sérigraphie',ref,(x+.001,y+.003,.00495),.0016,'Blanc',(0,0,0))
group('80 Avertissement implantation')
text('Contour carte / cote','CONTOUR SOURCE : 90 × 80 mm',(-.063,-.060,.003),.0035,'Blanc',(0,0,0))
text('Hors contour / note','U3 ET TRIAC1 HORS CONTOUR DANS LE FICHIER FOURNI',(-.063,-.067,.003),.0023,'Orange',(0,0,0))
group('99 Sol studio')
box('Sol studio',(0,0,-.001),(200,200,.001),'Sol',0)
cam=setup_camera((.105,-.18,.24),(-.01,-.006,.010),.24)
overlay(cam,'LA CARTE KiCad','POSITIONS ET PASTILLES DU FICHIER FOURNI  /  CORPS APPROCHÉS','Implantation source conservée  •  Composants hors contour visibles  •  Carte à finaliser',.24)
s3['source']=KICAD['source']
s3['limite']='La géométrie des corps est approchée. U3 et TRIAC1 ont leur origine hors du contour KiCad ; implantation à vérifier avant fabrication.'

import sys
sys.path.insert(0,str(HERE))
from interieur_pompe import ajouter_interieur
s4=ajouter_interieur(globals())
from installer_dans_cave import installer_cave
installer_cave(globals())

# Notice intégrée accessible depuis l'espace Scripting de Blender.
notice=(HERE/'LISEZ_MOI.md').read_text(encoding='utf-8') if (HERE/'LISEZ_MOI.md').exists() else 'Voir LISEZ_MOI.md.'
bpy.data.texts.new('LISEZ-MOI / sources et hypothèses').write(notice)
for s in [s1,s2,s3,s4]:
    bpy.context.window.scene=s
    s.render.fps=24
    for o in s.objects:
        if o.type in {'LIGHT','CAMERA'}:o.hide_set(True)
    # Les lumières et la caméra restent actives au rendu.
    for screen_data in bpy.data.screens:
        for a in screen_data.areas:
            if a.type=='VIEW_3D':
                a.spaces.active.region_3d.view_perspective='CAMERA'
                a.spaces.active.shading.type='MATERIAL'
                a.spaces.active.overlay.show_overlays=False

bpy.context.window.scene=s1
empty=bpy.data.scenes.get('Scene')
if empty and empty != s1:bpy.data.scenes.remove(empty)
prefs=bpy.context.preferences.filepaths
if hasattr(prefs,'file_preview_type'):
    if 'NONE' in [v.identifier for v in prefs.bl_rna.properties['file_preview_type'].enum_items]:
        prefs.file_preview_type='NONE'
bpy.context.preferences.filepaths.save_version=0
bpy.ops.wm.save_as_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
for s,filename in [(s1,'01_installation.png'),(s2,'02_coffret_eclate.png'),(s3,'03_carte_kicad.png'),(s4,'04_interieur_moteur_pompe.png')]:
    bpy.context.window.scene=s
    s.render.filepath=str(HERE/filename)
    bpy.ops.render.render(write_still=True)
bpy.context.window.scene=s1
bpy.ops.wm.save_as_mainfile(filepath=str(HERE/'Vide_cave_GEII.blend'))
print('MAQUETTE_TERMINEE')
