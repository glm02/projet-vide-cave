"""Installation en cave : bassin fermé, grille et décor intérieur."""
import math
import bpy
from mathutils import Vector

def installer_cave(api):
    scene=bpy.data.scenes['01 Installation du vide-cave']
    bpy.context.window.scene=scene;api['scene']=scene
    group=api['group'];box=api['box'];tube=api['tube'];cyl=api['cylinder'];torus=api['torus']
    # Nettoyage du studio, des repères et des raccordements remplacés.
    remove={'Socle de présentation','Support du coffret','Pied support','Sol studio','Refoulement / conduite','Câble alimentation pompe','Arrivée secteur','Repère cuve','Repère coffret','Mention coupe'}
    for obj in list(scene.objects):
        if obj.name in remove or obj.type in {'LIGHT','CAMERA'} or obj.name.startswith('Cartouche') or obj.name.startswith('Collier conduite'):
            bpy.data.objects.remove(obj,do_unlink=True)
    # Le coffret se fixe sur le mur de la cave.
    shift=Vector((0,.50,.456))
    for coll in scene.collection.children:
        if coll.name.startswith(('30 ','31 ','32 ','33 ')):
            for obj in coll.objects:obj.location+=shift
    group('35 Coffret / fixations au mur')
    for sx in [-1,1]:
        for sz in [-1,1]:cyl('Entretoise / coffret mural',(.52+sx*.085,.766,1.16+sz*.127),.006,.068,'Inox','Y')

    def stone_material(name,base,brick=False,side=False):
        mat=api['material'](name,base,0,.91)
        nodes=mat.node_tree.nodes;links=mat.node_tree.links
        p=next(n for n in nodes if n.type=='BSDF_PRINCIPLED')
        geom=nodes.new('ShaderNodeNewGeometry')
        noise=nodes.new('ShaderNodeTexNoise');noise.inputs['Scale'].default_value=8;noise.inputs['Detail'].default_value=4
        links.new(geom.outputs['Position'],noise.inputs['Vector'])
        ramp=nodes.new('ShaderNodeValToRGB')
        ramp.color_ramp.elements[0].color=(*[v*.55 for v in base],1)
        ramp.color_ramp.elements[1].color=(*[min(v*1.4,1) for v in base],1)
        links.new(noise.outputs['Fac'],ramp.inputs[0])
        bump=nodes.new('ShaderNodeBump');bump.inputs['Strength'].default_value=.3;bump.inputs['Distance'].default_value=.009
        if brick:
            sep=nodes.new('ShaderNodeSeparateXYZ');comb=nodes.new('ShaderNodeCombineXYZ')
            links.new(geom.outputs['Position'],sep.inputs[0])
            links.new(sep.outputs['Y' if side else 'X'],comb.inputs['X'])
            links.new(sep.outputs['Z'],comb.inputs['Y'])
            tex=nodes.new('ShaderNodeTexBrick')
            tex.offset=.5;tex.offset_frequency=2
            tex.inputs['Scale'].default_value=1
            tex.inputs['Brick Width'].default_value=.26
            tex.inputs['Row Height'].default_value=.09
            tex.inputs['Mortar Size'].default_value=.006
            tex.inputs['Mortar Smooth'].default_value=.03
            tex.inputs['Color1'].default_value=(.23,.12,.072,1)
            tex.inputs['Color2'].default_value=(.38,.22,.13,1)
            tex.inputs['Mortar'].default_value=(.12,.13,.12,1)
            links.new(comb.outputs[0],tex.inputs['Vector'])
            mix=nodes.new('ShaderNodeMixRGB');mix.blend_type='MULTIPLY';mix.inputs[0].default_value=.27
            links.new(tex.outputs['Color'],mix.inputs[1]);links.new(ramp.outputs['Color'],mix.inputs[2])
            links.new(mix.outputs[0],p.inputs['Base Color'])
            links.new(tex.outputs['Fac'],bump.inputs['Height']);bump.invert=True
        else:
            links.new(ramp.outputs['Color'],p.inputs['Base Color'])
            links.new(noise.outputs['Fac'],bump.inputs['Height'])
        links.new(bump.outputs[0],p.inputs['Normal'])
        return mat

    stone_material('Sol cave / béton patiné',(.23,.24,.22))
    stone_material('Mur cave / briques arrière',(.33,.24,.17),True)
    stone_material('Mur cave / briques latérales',(.33,.24,.17),True,True)
    stone_material('Bassin / béton brut',(.38,.4,.37))
    api['material']('Bois cave',(.15,.072,.028),0,.72)
    api['material']('Carton',(.30,.20,.10),0,.92)
    api['material']('Verre bouteille',(.018,.07,.032),.05,.18)
    api['material']('Lampe cave',(.95,.68,.31),0,.2,8)

    group('12 Bassin fermé / quatre parois')
    box('Cuve / paroi avant',(-.29,-.332,.244),(.78,.026,.38),'Bassin / béton brut',.002)
    box('Cuve / paroi droite',(.088,0,.244),(.025,.69,.38),'Bassin / béton brut',.002)
    box('Cuve / arête avant',(-.29,-.332,.44),(.80,.04,.018),'Béton clair',.002)
    box('Cuve / arête droite',(.088,0,.44),(.04,.71,.018),'Béton clair',.002)
    for obj in scene.objects:
        if obj.type=='MESH' and obj.name in ['Cuve / fond','Cuve / paroi arrière','Cuve / paroi gauche']:
            obj.data.materials.clear();obj.data.materials.append(api['M']['Bassin / béton brut'])
    # Le niveau du sol coïncide avec le haut du bassin : puisard enterré.
    group('50 Cave / sol autour du puisard')
    for name,loc,size in [
        ('Sol / gauche',(-.9675,-.30,.397),(.565,2.20,.09)),
        ('Sol / droite',(.6775,-.30,.397),(1.145,2.20,.09)),
        ('Sol / devant',(-.29,-.88,.397),(.790,1.04,.09)),
        ('Sol / derrière',(-.29,.58,.397),(.790,.44,.09)),
    ]:box(name,loc,size,'Sol cave / béton patiné',0)
    group('13 Grille amovible du bassin')
    for x in [-.697,.117]:box('Grille / cadre latéral',(x,0,.461),(.025,.735,.016),'Graphite',.002)
    for y in [-.367,.367]:box('Grille / cadre transversal',(-.29,y,.461),(.84,.025,.016),'Graphite',.002)
    # Mailles régulières, avec un passage dédié au tuyau de refoulement.
    for i in range(11):
        x=-.659+i*.074
        bar=box('Grille / barre longitudinale',(x,0,.462),(.008,.706,.011),'Inox',.001)
        if abs(x+.045)<.040:api['cut_box'](bar,(-.045,-.055,.462),(.078,.09,.04))
    for i in range(10):
        y=-.324+i*.072
        bar=box('Grille / barre transversale',(-.29,y,.468),(.790,.008,.009),'Inox',.001)
        if abs(y+.055)<.045:api['cut_box'](bar,(-.045,-.055,.468),(.078,.09,.04))
    for x,y in [(-.64,-.32),(.06,.32)]:
        torus('Grille / anneau de levage',(x,y,.475),.013,.002,'Graphite','X')
    group('51 Cave / murs et plafond')
    box('Mur de cave / arrière',(0,.85,1.585),(2.65,.10,2.40),'Mur cave / briques arrière',0)
    box('Mur de cave / gauche',(-1.30,-.30,1.585),(.10,2.40,2.40),'Mur cave / briques latérales',0)
    box('Mur de cave / droite',(1.30,-.30,1.585),(.10,2.40,2.40),'Mur cave / briques latérales',0)
    box('Mur de cave / entrée',(0,-1.45,1.585),(2.65,.10,2.40),'Mur cave / briques arrière',0)
    box('Plafond de cave',(0,-.30,2.81),(2.65,2.40,.08),'Sol cave / béton patiné',0)
    # Encadrement d'une porte donnant accès à la pièce.
    door=scene.objects.get('Mur de cave / entrée')
    api['cut_box'](door,(.57,-1.45,1.44),(.90,.30,2.00))
    for x in [.10,1.04]:box('Porte / encadrement',(x,-1.39,1.43),(.06,.06,1.98),'Bois cave',.003)
    box('Porte / linteau',(.57,-1.39,2.43),(.99,.06,.08),'Bois cave',.003)

    group('52 Cave / refoulement et alimentation')
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
    tube('Refoulement / dans la cave',pts,.018,'Graphite',False)
    for x in [.28,.90]:
        torus('Collier / mur',(x,.66,1.03),.019,.003,'Inox','X')
        box('Collier / fixation mur',(x,.735,1.03),(.028,.13,.032),'Inox',.002)
    tube('Câble pompe / sans flotteur',[(-.24,-.055,.336),(-.15,.10,.40),(.05,.36,.50),(.35,.63,.65),(.573,.69,.96),(.573,.69,1.027)],.0045,'Noir')
    tube('Câble secteur / prise murale',[(.467,.69,1.027),(.467,.66,.92),(.67,.66,.85),(.82,.675,.92)],.0045,'Noir')
    box('Prise de cave / boîtier',(.82,.762,.94),(.075,.066,.090),'ABS clair',.008)
    box('Prise de cave / couvercle',(.82,.725,.965),(.062,.008,.038),'Graphite',.003)
    cyl('Prise de cave / fiche',(.82,.710,.94),.019,.034,'Noir','Y')
    tube('Conduit électrique mural',[(.82,.778,.985),(.82,.778,2.58),(.74,.778,2.65),(-.20,.778,2.65),(-.20,.30,2.65)],.009,'ABS clair')
    for z in [1.35,1.95,2.48]:box('Conduit / attache',(.82,.771,z),(.038,.018,.018),'Graphite',.002)

    group('53 Cave / rangement')
    # Étagère modeste et quelques objets qui donnent l'échelle de la cave.
    for z in [.85,1.28,1.71]:
        box('Étagère / tablette',(-.99,.49,z),(.43,.54,.030),'Bois cave',.004)
    for x in [-1.19,-.79]:
        for y in [.24,.74]:box('Étagère / montant',(x,y,1.07),(.035,.035,1.26),'Graphite',.002)
    box('Carton / stockage',(-.99,.51,.992),(.29,.30,.25),'Carton',.005)
    box('Carton / ruban',(-.99,.51,1.120),(.036,.30,.003),'Résistance',.0001)
    for x,y in [(-1.08,.47),(-.95,.48),(-1.04,.62)]:
        cyl('Bouteille / corps',(x,y,1.39),.025,.17,'Verre bouteille')
        cyl('Bouteille / goulot',(x,y,1.50),.010,.060,'Verre bouteille')
        cyl('Bouteille / bouchon',(x,y,1.533),.011,.012,'Laiton')
        cyl('Bouteille / étiquette',(x,y,1.383),.0255,.044,'Résistance')

    group('54 Cave / luminaire')
    cyl('Lampe / fixation plafond',(-.20,.30,2.757),.065,.024,'Graphite')
    tube('Lampe / suspension',[(-.20,.30,2.75),(-.20,.30,2.51)],.007,'Noir',False)
    cyl('Lampe / abat-jour',(-.20,.30,2.49),.12,.04,'Graphite')
    api['sphere']('Lampe / diffuseur',(-.20,.30,2.465),(.086,.086,.025),'Lampe cave')
    for i in range(8):
        a=2*math.pi*i/8
        tube('Lampe / grille',[( -.20+.11*math.cos(a),.30+.11*math.sin(a),2.49),(-.20+.095*math.cos(a),.30+.095*math.sin(a),2.43)],.002,'Graphite',False)
    torus('Lampe / cercle inférieur',(-.20,.30,2.43),.095,.003,'Graphite')

    group('90 Cave / caméra et éclairage')
    data=bpy.data.cameras.new('Caméra dans la cave')
    cam=bpy.data.objects.new('Caméra dans la cave',data);api['collection'].objects.link(cam)
    cam.location=(1.10,-1.29,1.30)
    cam.rotation_euler=(Vector((-.18,.18,.82))-cam.location).to_track_quat('-Z','Y').to_euler()
    data.type='PERSP';data.lens=22;data.clip_start=.02;scene.camera=cam
    def area(name,loc,target,power,size,color):
        d=bpy.data.lights.new(name,'AREA');d.energy=power;d.shape='DISK';d.size=size;d.color=color
        o=bpy.data.objects.new(name,d);api['collection'].objects.link(o);o.location=loc
        o.rotation_euler=(Vector(target)-o.location).to_track_quat('-Z','Y').to_euler()
    area('Éclairage / plafonnier',(-.20,.30,2.44),(-.1,.1,.45),150,.24,(1,.76,.48))
    area('Éclairage / entrée',( .7,-1.20,1.72),(-.20,.20,.58),75,1.0,(.70,.82,1))
    area('Éclairage / coffret',(.9,.40,1.78),(.52,.72,1.10),15,.6,(1,.85,.65))
    bg=next(n for n in scene.world.node_tree.nodes if n.type=='BACKGROUND')
    bg.inputs[0].default_value=(.07,.09,.12,1);bg.inputs[1].default_value=.08
    scene.cycles.samples=64
    scene.render.resolution_x=1600;scene.render.resolution_y=1200
    scene['installation']='Bassin fermé sur quatre côtés, grille amovible, puisard sous le sol d’une cave en briques. Coffret mural. Flotteur retiré.'
    scene['dimensions_cave']='Cave proposée : 2,50 × 2,20 m au sol, hauteur environ 2,40 m. Bassin et installation conservés à titre de maquette.'
    return scene
