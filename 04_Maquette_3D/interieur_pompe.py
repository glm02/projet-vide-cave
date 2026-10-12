"""Intérieur générique d'un moteur asynchrone et d'une pompe centrifuge."""
import bpy
import math
from mathutils import Vector

def ajouter_interieur(api):
    pi=math.pi
    group=api['group']; box=api['box']; cyl=api['cylinder']; torus=api['torus']
    tube=api['tube']; text=api['text']; assign=api['assign']; bevel=api['bevel']
    for name,color,metal in [('Cuivre bobinage',(.72,.22,.065),.72),('Acier feuilleté',(.15,.21,.24),.75),('Aluminium rotor',(.48,.55,.58),.82),('Coupe claire',(.57,.69,.72),.55)]:
        if name not in api['M']:api['material'](name,color,metal,.28)

    def ring(name,x,y,z,r0,r1,h,mat,start=30,end=210):
        n=max(30,int((end-start)/3))
        verts=[]
        for zz in [z-h/2,z+h/2]:
            for rr in [r0,r1]:
                verts.extend((x+rr*math.cos(math.radians(start+(end-start)*i/n)),y+rr*math.sin(math.radians(start+(end-start)*i/n)),zz) for i in range(n+1))
        k=n+1
        faces=[]
        for i in range(n):
            faces.extend([(i,i+1,k+i+1,k+i),(2*k+i,3*k+i,3*k+i+1,2*k+i+1),(i,2*k+i,2*k+i+1,i+1),(k+i,k+i+1,3*k+i+1,3*k+i)])
        if end-start<359:
            faces.extend([(0,k,3*k,2*k),(n,2*k+n,3*k+n,k+n)])
        mesh=bpy.data.meshes.new(name)
        mesh.from_pydata(verts,[],faces); mesh.update()
        obj=bpy.data.objects.new(name,mesh)
        api['collection'].objects.link(obj)
        mesh.materials.append(api['M'][mat])
        bevel(obj,.00035,2)
        obj['géométrie']='Coupe pédagogique générique, sans plan fabricant'
        return obj

    def blade(x,y,index):
        n=16; verts=[]
        for zz in [.122,.134]:
            for sign in [-1,1]:
                for j in range(n+1):
                    t=j/n; r=.016+.047*t; a=index*2*pi/6+.7*t
                    verts.append((x+r*math.cos(a)+sign*.0012*math.cos(a+pi/2),y+r*math.sin(a)+sign*.0012*math.sin(a+pi/2),zz))
        k=n+1; faces=[]
        for j in range(n):
            faces.extend([(j,j+1,k+j+1,k+j),(2*k+j,3*k+j,3*k+j+1,2*k+j+1),(j,2*k+j,2*k+j+1,j+1),(k+j,k+j+1,3*k+j+1,3*k+j)])
        faces.extend([(0,k,3*k,2*k),(n,2*k+n,3*k+n,k+n)])
        mesh=bpy.data.meshes.new('Aube courbe');mesh.from_pydata(verts,[],faces);mesh.update()
        obj=bpy.data.objects.new('Roue / aube courbe '+str(index+1),mesh)
        api['collection'].objects.link(obj);mesh.materials.append(api['M']['Coque bleue']);bevel(obj,.0004,2)

    def assembly(x,y,detailed=False):
        group('21 Pompe / carter en coupe')
        ring('Moteur / enveloppe inox ouverte',x,y,.238,.067,.071,.160,'Inox')
        ring('Pompe / volute ouverte',x,y,.145,.078,.087,.032,'Coque bleue')
        ring('Pompe / crépine ouverte',x,y,.096,.080,.09,.037,'Graphite')
        ring('Pompe / couronne ouverte',x,y,.077,.083,.095,.015,'Noir')
        group('22 Moteur / stator et bobinages')
        ring('Stator / tôles magnétiques en coupe',x,y,.237,.039,.060,.090,'Acier feuilleté')
        # Fines strates sur la face de coupe évoquant l'empilage de tôles.
        for i in range(24):
            for deg in [30,210]:
                a=math.radians(deg)
                tube('Stator / tranche de tôle',[(x+.040*math.cos(a),y+.040*math.sin(a),.194+i*.0036),(x+.059*math.cos(a),y+.059*math.sin(a),.194+i*.0036)],.00018,'Inox',False)
        for i in range(7):
            a=math.radians(30+180*i/6)
            for zz in [.188,.285]:
                for turn in range(5):
                    pts=[]
                    for j in range(33):
                        t=2*pi*j/32
                        rr=.050+(.006+turn*.0007)*math.cos(t)
                        tt=(.007+turn*.0007)*math.sin(t)
                        pts.append((x+rr*math.cos(a)-tt*math.sin(a),y+rr*math.sin(a)+tt*math.cos(a),zz+turn*.0009))
                    tube('Bobinage / tête de spire',pts,.00065,'Cuivre bobinage',False)
            for j in range(4):
                aa=a+(j-1.5)*.028
                tube('Bobinage / conducteurs dans encoche',[(x+.042*math.cos(aa),y+.042*math.sin(aa),.192),(x+.042*math.cos(aa),y+.042*math.sin(aa),.282)],.00065,'Cuivre bobinage',False)
        group('23 Moteur / rotor et arbre')
        cyl('Rotor / noyau feuilleté',(x,y,.237),.031,.078,'Acier feuilleté')
        for i in range(18):
            a=2*pi*i/18
            tube('Rotor / barre de cage',[(x+.0315*math.cos(a),y+.0315*math.sin(a),.198),(x+.0315*math.cos(a+.14),y+.0315*math.sin(a+.14),.276)],.0010,'Aluminium rotor',False)
        for z in [.198,.276]:ring('Rotor / anneau de court-circuit',x,y,z,.024,.033,.005,'Aluminium rotor',0,360)
        cyl('Arbre moteur et pompe',(x,y,.215),.006,.190,'Inox')
        for z in [.169,.304]:
            ring('Roulement / bague extérieure',x,y,z,.011,.019,.009,'Inox',0,360)
            ring('Roulement / bague intérieure',x,y,z,.0063,.0085,.009,'Inox',0,360)
            for i in range(9):
                a=2*pi*i/9
                api['sphere']('Roulement / bille',(x+.0097*math.cos(a),y+.0097*math.sin(a),z),(.002,.002,.002),'Inox')
        group('24 Étanchéité / séparation moteur et eau')
        ring('Flasque moteur / coupe',x,y,.176,.019,.066,.009,'Coupe claire')
        ring('Chambre de séparation / coupe',x,y,.153,.024,.069,.031,'Graphite')
        ring('Garniture mécanique / bague fixe',x,y,.156,.0063,.013,.005,'Blanc',0,360)
        ring('Garniture mécanique / bague tournante',x,y,.150,.0063,.012,.005,'Noir',0,360)
        for i in range(5):torus('Garniture / ressort',(x,y,.14+i*.0016),.0075,.00045,'Inox')
        group('25 Pompe / roue centrifuge')
        ring('Roue / plateau inférieur',x,y,.120,.012,.068,.003,'Coque bleue',0,360)
        ring('Roue / plateau supérieur en coupe',x,y,.136,.016,.067,.003,'Coque bleue')
        for i in range(6):blade(x,y,i)
        cyl('Roue / moyeu',(x,y,.128),.013,.016,'Laiton')
        ring('Aspiration / œil de la roue',x,y,.107,.013,.024,.019,'Coupe claire',30,210)
        if detailed:
            group('26 Enveloppe extérieure / repères')
            ring('Capot / coupe',x,y,.327,.015,.074,.023,'Coque bleue')
            ring('Joint de capot / coupe',x,y,.315,.066,.071,.003,'Noir')
            for sx in [-1,1]:box('Poignée / montant',(x+sx*.055,y,.363),(.017,.019,.066),'Orange',.006)
            box('Poignée / traverse',(x,y,.395),(.125,.019,.022),'Orange',.007)
            for i in range(3):
                a=2*pi*i/3
                box('Pied de pompe',(x+.067*math.cos(a),y+.067*math.sin(a),.068),(.027,.028,.022),'Noir',.003)
            o=ring('Sortie de refoulement / tube',0,0,0,.017,.023,.090,'Coque bleue',0,360)
            o.location=(x+.11,y,.145);o.rotation_euler[1]=pi/2
            for o in api['collection'].objects:o['statut']='Éléments génériques sans flotteur'

    # Installation existante : remplacement des volumes opaques par des carters ouverts.
    s1=bpy.data.scenes['01 Installation du vide-cave']
    bpy.context.window.scene=s1;api['scene']=s1
    remove=['Socle / crépine','Couronne inférieure','Volute','Moteur / corps inox','Plaque moteur','Plaque pompe','Tension pompe']
    for o in list(s1.objects):
        if o.name in remove or o.name in ['Flotteur de niveau','Câble flotteur']:bpy.data.objects.remove(o,do_unlink=True)
        elif o.name.startswith('Ouverture crépine'):
            deg=math.degrees(math.atan2(o.location.y+.055,o.location.x+.28))%360
            if not 30<=deg<=210:bpy.data.objects.remove(o,do_unlink=True)
    for o in s1.objects:
        if o.type=='FONT' and 'flotteur' in o.data.body.casefold():o.data.body=o.data.body.replace('Pompe, flotteur, refoulement et coffret','Pompe en coupe, refoulement et coffret')
        if 'statut' in o and 'flotteur' in str(o['statut']):o['statut']='Pompe générique sans flotteur, dimensions proposées'
    assembly(-.28,-.055)

    s4=api['new_scene']('04 Intérieur moteur et pompe')
    assembly(0,0,True)
    group('99 Sol studio')
    box('Socle de coupe',(0,0,.051),(.245,.205,.012),'Graphite',.009)
    box('Sol studio',(0,0,.039),(200,200,.01),'Sol',0)
    cam=api['setup_camera']((.60,-1.05,.67),(.005,0,.243),.68)
    api['overlay'](cam,'MOTEUR & POMPE','VUE INTÉRIEURE EN COUPE  /  MOTEUR ASYNCHRONE MONOPHASÉ 230 V','Coupe pédagogique générique  •  Géométrie interne à adapter au modèle réel  •  Sans flotteur',.68)
    group('80 Légendes de la coupe')
    # Légendes dans le plan de la caméra, avec traits de rappel projetés.
    def label(title,desc,x,y,point,left=False):
        for body,dy,size,mat in [(title,0,.0061,'Cartouche blanc'),(desc,-.008,.0042,'Cartouche bleu')]:
            obj=text('Légende '+title,body,(0,0,0),size,mat,(0,0,0));obj.parent=cam;obj.location=(x,y+dy,-.015)
            obj.visible_shadow=False;obj.visible_diffuse=False;obj.visible_glossy=False
        co=cam.rotation_euler.to_quaternion().inverted()@(Vector(point)-cam.location)
        endpoint=x+.115 if left else x-.008
        line=tube('Repère '+title,[(endpoint,y-.001,0),(co.x+(-.012 if left else .012),y-.001,0),(co.x,co.y,0)],.00035,'Cartouche bleu',False)
        line.parent=cam;line.location=(0,0,-.016)
        line.visible_shadow=False;line.visible_diffuse=False;line.visible_glossy=False
    label('01  STATOR + BOBINAGES','Tôles fixes et enroulements cuivre',.155,.076,(.037,-.015,.284))
    label('02  ROTOR À CAGE','Partie tournante du moteur',.155,.022,(.022,-.021,.244))
    label('03  ARBRE MOTEUR','Transmission vers la roue',.155,-.035,(0,-.006,.186))
    label('04  ROUE À AUBES','Pompage centrifuge',.155,-.094,(.034,-.041,.128))
    label('05  ROULEMENT','Guidage de l’arbre',-.292,.042,(-.014,-.004,.304),True)
    label('06  ÉTANCHÉITÉ','Garniture mécanique',-.292,-.026,(-.012,-.004,.153),True)
    label('07  ASPIRATION','Entrée de l’eau par le dessous',-.292,-.093,(0,-.015,.106),True)
    s4['hypothèses']='Coupe générique : moteur asynchrone à cage, stator bobiné, arbre vertical, deux roulements, garniture mécanique et roue centrifuge à six aubes. Aucun plan mécanique du fabricant fourni.'
    s1['intérieur']='Carters ouverts et intérieur pédagogique ajoutés. Flotteur et câble retirés.'
    return s4
