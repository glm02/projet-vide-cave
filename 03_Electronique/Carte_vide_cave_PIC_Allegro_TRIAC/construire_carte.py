"""Projet KiCad indépendant. Exécuter avec le Python fourni avec KiCad 8.
Les connexions et l'implantation sont définies une seule fois dans C.
"""
from pathlib import Path
import sys, json, uuid, math, copy, csv, subprocess, xml.etree.ElementTree as ET
import pcbnew as p
ROOT=Path(__file__).resolve().parent
BASE=ROOT/'Vide_cave_5A'
K=Path(r'C:\Users\rafae\AppData\Local\Programs\KiCad\8.0\share\kicad')
sys.path.insert(0,str(ROOT/'sources'))
from sexpr import parse,field
def uid(s): return str(uuid.uuid5(uuid.NAMESPACE_URL,'vide-cave-5A/'+s))
SHEET=uid('sheet')
def q(s): return json.dumps(str(s),ensure_ascii=False)
def dump(n):
    if isinstance(n,list):
        return '('+' '.join(str(x) if i==0 else q(x) if (n[0] in ['symbol','extends','name','number','text'] and i==1) or (n[0]=='property' and i in [1,2]) else dump(x) for i,x in enumerate(n))+')'
    # Parser atoms are strings; retain numbers and known syntax atoms.
    if n in ATOMS or __import__('re').fullmatch(r'-?\d+(\.\d+)?',str(n)): return str(n)
    return q(n)
ATOMS=set('symbol extends property at effects font size thickness justify left right top bottom hide yes no pin_names offset pin_numbers exclude_from_sim in_bom on_board rectangle start end stroke width type default fill none background outline polyline pts xy circle center radius arc length pin name number passive input output bidirectional tri_state power_in power_out open_collector no_connect free line clock inverted inverted_clock input_low clock_low output_low edge_clock_high non_logic italic bold'.split())
def sym(lib,name):
    src=parse((K/'symbols'/(lib+'.kicad_sym')).read_text(encoding='utf8'))
    symbols={n[1]:n for n in src if isinstance(n,list) and n[0]=='symbol'}
    def resolve(name):
        n=copy.deepcopy(symbols[name]); parent=field(n,'extends')
        if parent:
            old=resolve(parent[0]); n=[n[0],n[1]]+[x for x in n[2:] if x[0]!='extends']+[x for x in old[2:] if x[0]!='property' and x[0] not in {z[0] for z in n[2:] if z[0]!='symbol'}]
            for x in n:
                if isinstance(x,list) and x[0]=='symbol': x[1]=name+x[1][x[1].rfind('_',0,x[1].rfind('_')):]
        return n
    return resolve(name)
pic=copy.deepcopy(next(x for x in parse((ROOT/'sources/PIC16F1788.kicad_sym').read_text(encoding='utf8')) if isinstance(x,list) and x[0]=='symbol'))
pic[1]='PIC16F1788';
for x in pic:
    if isinstance(x,list) and x[0]=='symbol': x[1]='PIC16F1788'+x[1][x[1].rfind('_',0,x[1].rfind('_')):]
LIB={'PIC16F1788':pic}
C=[]
def add(ref,val,lib,name,fp,nets,xy,sch,rot=0,desc=''):
    if name not in LIB: LIB[name]=sym(lib,name)
    C.append(dict(ref=ref,val=val,symbol=name,fp=fp,nets={str(k):v for k,v in nets.items()},xy=xy,sch=sch,rot=rot,desc=desc,uuid=uid(ref)))
RFP='Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal'
CFP='Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm'
def r(ref,val,a,b,xy,sch,fp=RFP): add(ref,val,'Device','R',fp,{1:a,2:b},xy,sch)
def cap(ref,val,a,b,xy,sch,fp=CFP): add(ref,val,'Device','C',fp,{1:a,2:b},xy,sch)
def conn(ref,val,n,nets,xy,sch,fp=None):
    add(ref,val,'Connector_Generic',f'Conn_01x{n:02d}',fp or f'Connector_PinHeader_2.54mm:PinHeader_1x{n:02d}_P2.54mm_Vertical',dict(enumerate(nets,1)),xy,sch)
add('U1','PIC16F1788-I/SP','','PIC16F1788','Package_DIP:DIP-28_W7.62mm_LongPads',
    {1:'MCLR',2:'ADC_I',3:'LCD_RS',4:'LCD_E',5:'LCD_D4',6:'LCD_D5',7:'LCD_D6',8:'GND',9:None,10:'LCD_D7',11:None,12:None,13:None,14:None,15:None,16:None,17:'UART_TX',18:'UART_RX',19:'GND',20:'+5V',21:'LED_POMPE',22:'BOUTON_START',23:'POMPE_CMD',24:'BOUTON_STOP',25:None,26:None,27:'ICSP_CLK',28:'ICSP_DAT'},(60,58),(85,65),desc='Microcontrôleur 5 V; oscillateur interne; sortie RB2 active à 1')
add('U2','ACS756SCB-100B-PFF-T','Sensor_Current','ACS756xCB-100B-PFF','VideCave:ACS756_PFF',{1:'+5V',2:'GND',3:'I_BRUT',4:'L_FUSIBLE',5:'MT2'},(118,53),(225,58),desc='Capteur repris du projet; 20 mV/A; offset 2,5 V; boîtier PFF')
add('U3','MOC3083TVM','Relay_SolidState','MOC3083M','VideCave:MOC3083TVM_slot',{1:'OPTO_A',2:'OPTO_K',3:None,4:'GATE',5:None,6:'OPTO_MT2'},(118,108),(255,190),desc='Optotriac 800 V; passage par zéro; IFT max 5 mA; version TVM entre rangées 10.16 mm')
add('Q1','BTA16-800CRG','Triac_Thyristor','BTA16-800C','Package_TO_SOT_THT:TO-220-3_Vertical',{1:'L_POMPE',2:'MT2',3:'GATE'},(180,110),(350,190),desc='Triac standard 4 quadrants 800 V / 16 A; gâchette 25 mA en QI/QIII; dissipateur externe')
add('Q2','2N3904','Transistor_BJT','2N3904','Package_TO_SOT_THT:TO-92_Inline_Wide',{1:'GND',2:'BASE_NPN',3:'OPTO_K'},(107,107),(190,185),desc='Commande de la LED optotriac; défaut matériel = arrêt')
conn('J1','SECTEUR L N',2,['L_ENTREE','N'],(185,40),(330,45),'VideCave:RND_205_00298_P10_drill1_4')
conn('J2','POMPE L N',2,['L_POMPE','N'],(185,140),(380,190),'VideCave:RND_205_00298_P10_drill1_4')
add('J8','TERRE EXTERNE','Connector_Generic','Conn_01x03',None,{1:'PE',2:'PE',3:'PE'},None,(375,100),desc='Bornier PE extérieur au PCB: secteur, pompe, coffret conducteur si présent')
add('F1','T3.15A HBC 250VAC','Device','Fuse','Fuse:Fuseholder_Littelfuse_445_030_series_5x20mm',{1:'L_ENTREE',2:'L_FUSIBLE'},(175,57),(280,45),rot=180,desc='Littelfuse 02153.15MXP temporisé céramique; candidat pompe 300 W; calibre à valider')
conn('J3','5V ISOLEE',2,['+5V','GND'],(45,120),(45,135),'TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal')
conn('J4','ICSP PICkit',6,['MCLR','+5V','GND','ICSP_DAT','ICSP_CLK',None],(84,52),(135,55))
conn('J5','LCD 16x2 4 BITS',16,['GND','+5V','LCD_VO','LCD_RS','GND','LCD_E',None,None,None,None,'LCD_D4','LCD_D5','LCD_D6','LCD_D7','LCD_A','GND'],(40,45),(35,42))
conn('J6','BOUTONS IHM',4,['GND','BOUTON_START','BOUTON_STOP','+5V'],(89,87),(135,100))
conn('J7','UART TTL 5V',4,['GND','UART_TX','UART_RX','+5V'],(80,126),(135,140))
r('R1','10k','+5V','MCLR',(52,40),(75,132))
cap('C1','100nF','+5V','GND',(74,78),(100,132))
add('C2','47uF 16V','Device','C_Polarized','Capacitor_THT:CP_Radial_D6.3mm_P2.50mm',{1:'+5V',2:'GND'},(52,108),(45,172))
cap('C3','100nF','+5V','GND',(110,52),(195,58))
r('R2','1k','I_BRUT','ADC_I',(95,72),(225,100))
cap('C4','10nF','ADC_I','GND',(74,60),(260,100))
r('R3','4.7k','POMPE_CMD','BASE_NPN',(88,102),(150,185))
r('R4','100k','BASE_NPN','GND',(97,117),(190,222))
r('R5','330R','+5V','OPTO_A',(105,95),(225,150))
r('R6','91R 0.5W','MT2','R_G_SERIE',(135,93),(295,148),'Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal')
r('R7','91R 0.5W','R_G_SERIE','OPTO_MT2',(151,93),(330,148),'Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal')
r('R8','330R 0.5W','GATE','L_POMPE',(162,121),(330,230),'Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal')
r('R9','39R 1W','MT2','SNUBBER',(150,102),(290,260),'Resistor_THT:R_Axial_DIN0411_L9.9mm_D3.6mm_P15.24mm_Horizontal')
cap('C5','10nF X2 305VAC','SNUBBER','L_POMPE',(150,133),(330,260),'Capacitor_THT:C_Rect_L19.0mm_W5.0mm_P15.00mm_MKS4')
r('R10','1k','LED_POMPE','LED_A',(61,119),(75,215))
add('D1','LED POMPE','Device','LED','LED_THT:LED_D3.0mm',{1:'GND',2:'LED_A'},(69,129),(100,215))
r('R11','220R','+5V','LCD_A',(48,87),(45,220))
add('RV1','10k CONTRASTE','Device','R_Potentiometer','Potentiometer_THT:Potentiometer_Bourns_3296W_Vertical',{1:'GND',2:'LCD_VO',3:'+5V'},(55,96),(45,260))
# Power flags are virtual schematic-only components; J3 must be an isolated supply.
add('#FLG01','PWR_FLAG','power','PWR_FLAG',None,{1:'+5V'},None,(78,265))
add('#FLG02','PWR_FLAG','power','PWR_FLAG',None,{1:'GND'},None,(110,265))
for i,(x,y) in enumerate([(30,30),(210,30),(30,150),(210,150)],1):
    add('H'+str(i),'M3','Mechanical','MountingHole','MountingHole:MountingHole_3.2mm_M3',{},(x,y),(150+(i-1)*25,265))

def write_schematic():
    # Portable, fully resolved symbol library (no dependency on original project).
    libtext=['(kicad_symbol_lib (version 20231120) (generator kicad_symbol_editor)']
    for name,node in LIB.items(): libtext.append(dump(node))
    libtext.append(')'); (ROOT/'VideCave.kicad_sym').write_text('\n'.join(libtext),encoding='utf8')
    (ROOT/'sym-lib-table').write_text('(sym_lib_table (version 7) (lib (name "VideCave") (type "KiCad") (uri "${KIPRJMOD}/VideCave.kicad_sym") (options "") (descr "Projet vide cave 5 A")))',encoding='utf8')
    out=[f'(kicad_sch (version 20231120) (generator eeschema) (uuid {q(SHEET)}) (paper "A3") (title_block (title "Vide-cave / PIC + ACS756 + TRIAC") (date "2026-10-07") (rev "A - PROTOTYPE 300W") (comment 1 "230 VAC / environ 300 W; dimensionnement pistes 5 A a valider")) (lib_symbols']
    for name,node in LIB.items():
        n=copy.deepcopy(node);n[1]='VideCave:'+name;out.append(dump(n))
    out.append(')')
    def text(txt,x,y,size=1.5): out.append(f'(text {q(txt)} (at {x} {y} 0) (effects (font (size {size} {size})) (justify left)) (uuid {q(uid(txt))}))')
    for txt,x,y in [('COMMANDE / 5 V ISOLE',15,15),('MESURE DU COURANT',183,15),('SECTEUR 230 VAC - POMPE ~300 W',275,15),('ALIMENTATION ET PROGRAMMATION',15,120),('COMMANDE TOUT OU RIEN / PASSAGE PAR ZERO',145,125),('RC ANTICOMMUTATION : VALEURS A VALIDER SUR POMPE',265,246)]:text(txt,x,y)
    text('J1/J2 : 1=L  2=N. Terre pompe : liaison externe J8.',265,285,1.1)
    text('Dissipateur Q1 : cible <= 8 C/W; integration mecanique a valider.',265,280,1.1)
    text('Prototype a valider electriquement et thermiquement avant fabrication.',145,290,1.1)
    for c in C:
        x,y=[round(z/1.27)*1.27 for z in c['sch']]; node=LIB[c['symbol']]; virtual=c['fp'] is None; no_bom=virtual or c['ref'].startswith('H')
        offsets={'U1':(-20,-36),'U2':(-8,-20),'U3':(-5,-14)};ox,oy=offsets.get(c['ref'],(5,-7))
        props=[f'(property "Reference" {q(c["ref"])} (at {x+ox} {y+oy} 0) (effects (font (size 1.27 1.27)) (justify left)))',f'(property "Value" {q(c["val"])} (at {x+ox} {y+oy+3} 0) (effects (font (size 1.1 1.1)) (justify left)))',f'(property "Footprint" {q(c["fp"] or "")} (at {x} {y} 0) (effects (font (size 1.27 1.27)) (hide yes)))']
        out.append(f'(symbol (lib_id {q("VideCave:"+c["symbol"])}) (at {x} {y} 0) (unit 1) (exclude_from_sim no) (in_bom {"no" if no_bom else "yes"}) (on_board {"no" if virtual else "yes"}) (dnp no) (uuid {q(c["uuid"])}) '+''.join(props))
        pins=[]
        for n in node:
            if isinstance(n,list) and n[0]=='symbol':
                pins.extend(z for z in n if isinstance(z,list) and z[0]=='pin')
        for pin in pins:
            num=field(pin,'number')[0];out.append(f'(pin {q(num)} (uuid {q(uid(c["ref"]+"pin"+num))}))')
        out.append(f'(instances (project "Vide_cave_5A" (path {q("/"+SHEET)} (reference {q(c["ref"])}) (unit 1)))) )')
        for pin in pins:
            num=field(pin,'number')[0]; px,py,ang=map(float,field(pin,'at')); px+=x;py=y-py
            net=c['nets'].get(num);key=c['ref']+'wire'+num
            if net is None:
                if pin[1]!='no_connect': out.append(f'(no_connect (at {px:g} {py:g}) (uuid {q(uid(key))}))')
                continue
            # Extend each pin outwards and use named, electrically connected local labels.
            dx,dy={0:(-1,0),180:(1,0),90:(0,1),270:(0,-1)}[int(ang)]
            ex=px+dx*5.08;ey=py+dy*5.08
            out.append(f'(wire (pts (xy {px:g} {py:g}) (xy {ex:g} {ey:g})) (stroke (width 0) (type default)) (uuid {q(uid(key))}))')
            # Vertical pin labels horizontal, avoiding touching neighbors.
            la=0 if dx>=0 else 180
            if c['ref']=='U1' and num=='19':
                out.append(f'(wire (pts (xy {ex:g} {ey:g}) (xy {x:g} {ey:g})) (stroke (width 0) (type default)) (uuid {q(uid(key+"bridge"))}))')
            else:out.append(f'(label {q(net)} (at {ex:g} {ey:g} {la}) (effects (font (size 1.0 1.0)) (justify left bottom)) (uuid {q(uid(key+"label"))}))')
    out.append(')'); BASE.with_suffix('.kicad_sch').write_text('\n'.join(out),encoding='utf8')

def vec(x,y): return p.VECTOR2I(p.FromMM(x),p.FromMM(y))
def custom_footprint():
    folder=ROOT/'VideCave.pretty';folder.mkdir(exist_ok=True)
    # ACS756 PFF blade holes: 4 mm blade width, 1.5 mm thickness;
    # oval 4.8 x 1.9 mm drills, 8 x 5.5 mm copper lands, 10 mm centres.
    # Pin positions per Allegro package drawing. No spurious duplicate via ring.
    o=['(footprint "ACS756_PFF" (version 20240108) (generator pcbnew) (layer "F.Cu") (descr "ACS756 CB PFF - Allegro package drawing; prototype fit check required") (attr through_hole)',
       '(property "Reference" "REF**" (at 5 -9 0) (layer "F.SilkS") (effects (font (size 1 1) (thickness 0.15))))',
       '(property "Value" "ACS756_PFF" (at 5 9 0) (layer "F.Fab") (effects (font (size 1 1) (thickness 0.15))))',
       '(fp_rect (start 7.2 -5.2) (end 18.1 9.0) (stroke (width 0.12) (type solid)) (fill none) (layer "F.SilkS"))',
       '(fp_rect (start -1 -10.4) (end 24.5 10.4) (stroke (width 0.05) (type solid)) (fill none) (layer "F.CrtYd"))']
    for i in range(3):o.append(f'(pad "{i+1}" thru_hole {"rect" if i==0 else "circle"} (at 0 {1.91*i}) (size 1.5 1.5) (drill 0.8) (layers "*.Cu" "*.Mask"))')
    for num,y in [(4,-6.91),(5,3.09)]:o.append(f'(pad "{num}" thru_hole oval (at 21.4 {y}) (size 5.5 8) (drill oval 1.9 4.8) (layers "*.Cu" "*.Mask"))')
    o.append(')');(folder/'ACS756_PFF.kicad_mod').write_text('\n'.join(o),encoding='utf8')
    opto=p.FootprintLoad(str(K/'footprints/Package_DIP.pretty'),'DIP-6_W10.16mm')
    for item in list(opto.GraphicalItems()):
        if item.GetLayer()==p.F_SilkS:item.SetLayer(p.F_Fab)
    opto.SetFPID(p.LIB_ID('VideCave','MOC3083TVM_slot'))
    opto.SetValue('MOC3083TVM');opto.SetLibDescription('DIP6 option TVM 10.16 mm; silk removed at isolation slot')
    p.PCB_IO_KICAD_SEXPR().FootprintSave(str(folder),opto)
    terminal=p.FootprintLoad(str(K/'footprints/TerminalBlock_RND.pretty'),'TerminalBlock_RND_205-00298_1x02_P10.00mm_Horizontal')
    for pad in terminal.Pads():pad.SetDrillSize(vec(1.4,1.4))
    terminal.SetFPID(p.LIB_ID('VideCave','RND_205_00298_P10_drill1_4'))
    terminal.SetLibDescription('RND 205-00298, 10 A / 300 V, pas 10 mm; trou 1.4 mm selon fabricant')
    p.PCB_IO_KICAD_SEXPR().FootprintSave(str(folder),terminal)
    (ROOT/'fp-lib-table').write_text('(fp_lib_table (version 7) (lib (name "VideCave") (type "KiCad") (uri "${KIPRJMOD}/VideCave.pretty") (options "") (descr "Empreinte capteur PFF")))',encoding='utf8')

HV={'L_ENTREE','L_FUSIBLE','MT2','L_POMPE','N','PE','GATE','R_G_SERIE','OPTO_MT2','SNUBBER'}
def make_board():
    custom_footprint();b=p.BOARD();b.SetFileName(str(BASE.with_suffix('.kicad_pcb')))
    subprocess.run([str(K.parents[1]/'bin/kicad-cli.exe'),'sch','export','netlist',str(BASE.with_suffix('.kicad_sch')),'--format','kicadxml','--output',str(ROOT/'sources/netlist.xml')],check=True)
    nets={};pinmap={}
    for net in ET.parse(ROOT/'sources/netlist.xml').findall('./nets/net'):
        name=net.attrib['name']; ni=p.NETINFO_ITEM(b,name,int(net.attrib['code']));b.Add(ni);nets[name.removeprefix('/')]=ni
        for z in net.findall('node'):pinmap[(z.attrib['ref'],z.attrib['pin'])]=ni
    fps={}
    for c in C:
        if not c['fp']:continue
        lib,name=c['fp'].split(':'); directory=ROOT/'VideCave.pretty' if lib=='VideCave' else K/'footprints'/(lib+'.pretty')
        fp=p.FootprintLoad(str(directory),name)
        if not fp:raise RuntimeError(c['fp'])
        b.Add(fp);fp.SetReference(c['ref']);fp.SetValue(c['val']);fp.SetFPID(p.LIB_ID(lib,name));fp.SetPath(p.KIID_PATH('/'+SHEET+'/'+c['uuid']))
        fp.SetPosition(vec(*c['xy']));fp.SetOrientationDegrees(c['rot'])
        fp.Value().SetVisible(False);fp.Reference().SetTextSize(vec(1,1));fp.Reference().SetTextThickness(p.FromMM(.15))
        for pad in fp.Pads():
            ni=pinmap.get((c['ref'],pad.GetNumber()))
            if ni:pad.SetNet(ni)
        fps[c['ref']]=fp
        if c['ref'].startswith('H'):fp.Reference().SetVisible(False)
    def line(a,z,layer=p.Edge_Cuts,width=.1):
        sh=p.PCB_SHAPE(b);sh.SetShape(p.SHAPE_T_SEGMENT);sh.SetStart(vec(*a));sh.SetEnd(vec(*z));sh.SetWidth(p.FromMM(width));sh.SetLayer(layer);b.Add(sh)
    for a,z in [((25,25),(215,25)),((215,25),(215,155)),((215,155),(25,155)),((25,155),(25,25))]:line(a,z)
    # Routed isolation slot under the optocoupler; 0.8 x 9 mm, square ends.
    for a,z in [((121.4,106.2),(122.2,106.2)),((122.2,106.2),(122.2,115.2)),((122.2,115.2),(121.4,115.2)),((121.4,115.2),(121.4,106.2))]:line(a,z)
    fps['U3'].Reference().SetPosition(vec(113,114));fps['U3'].Reference().SetTextAngle(p.EDA_ANGLE(0,p.DEGREES_T))
    # Slot intersects standard DIP silk; retain body drawing on F.Fab only.
    for item in list(fps['U3'].GraphicalItems()):
        if item.GetLayer()==p.F_SilkS:item.SetLayer(p.F_Fab)
    def txt(t,x,y,size=1.2):
        tt=p.PCB_TEXT(b);tt.SetText(t);tt.SetPosition(vec(x,y));tt.SetTextSize(vec(size,size));tt.SetTextThickness(p.FromMM(.15));tt.SetLayer(p.F_SilkS);b.Add(tt)
    txt('VIDE CAVE - REV A / PROTOTYPE',73,30,1.4)
    txt('PIC16F1788 + ACS756',73,33,1.1)
    txt('5V ISOLE',47,130);txt('ICSP',85,48);txt('LCD 16x2',40,39);txt('TTL 5V',80,140)
    txt('230VAC / POMPE ~300W',170,28,1.3);txt('L   N',190,31)
    txt('POMPE : L N',188,150);txt('DISSIPATEUR Q1 REQUIS',169,116,1)
    txt('PE : LIAISON EXTERNE',170,146,1)
    txt('DANGER SECTEUR',198,95,1.3);txt('T3.15A - A VALIDER',165,69,1)
    # Barrier on both silk layers, interrupted at crossing components.
    for ya,yz in [(29,42),(67,103),(119,150)]:line((122,ya),(122,yz),p.F_SilkS,.15)
    ds=b.GetDesignSettings();ds.m_MinClearance=p.FromMM(.25);ds.m_CopperEdgeClearance=p.FromMM(.5);ds.m_HoleClearance=p.FromMM(.25)
    ds.m_SilkClearance=p.FromMM(.15)
    p.SaveBoard(str(BASE.with_suffix('.kicad_pcb')),b)
    (ROOT/'connexions.json').write_text(json.dumps(C,indent=2,ensure_ascii=False),encoding='utf8')
    return b,fps,nets

def project():
    config={'meta':{'filename':'Vide_cave_5A.kicad_pro','version':1},'board':{'design_settings':{'rules':{'min_clearance':.25,'min_track_width':.25,'min_via_diameter':.8,'min_through_hole_diameter':.3,'min_hole_clearance':.25,'min_copper_edge_clearance':.5,'min_silk_clearance':.15,'min_silk_text_height':.8,'min_silk_text_thickness':.12}}},'net_settings':{'meta':{'version':3},'classes':[{'name':'Default','clearance':.25,'track_width':.35,'via_diameter':.8,'via_drill':.4,'microvia_diameter':.3,'microvia_drill':.1,'bus_width':12,'wire_width':6,'pcb_color':'rgba(0, 0, 0, 0.000)','schematic_color':'rgba(0, 0, 0, 0.000)'}],'netclass_assignments':{},'netclass_patterns':[]},'schematic':{'drawing':{'default_line_thickness':6.0,'default_text_size':50.0}}}
    BASE.with_suffix('.kicad_pro').write_text(json.dumps(config,indent=2),encoding='utf8')
    lv={v for c in C for v in c['nets'].values() if v and v not in HV}
    condition="("+' || '.join("B.NetName == '/"+n+"'" for n in sorted(HV))+") && ("+' || '.join("A.NetName == '/"+n+"'" for n in sorted(lv))+")"
    BASE.with_suffix('.kicad_dru').write_text('''(version 1)
(rule "Isolation secteur logique 8 mm minimum"
 (condition "'''+condition+'''")
 (constraint clearance (min 8mm)))
''',encoding='utf8')
    with (ROOT/'Nomenclature.csv').open('w',newline='',encoding='utf-8-sig') as f:
        w=csv.writer(f,delimiter=';');w.writerow(['Repère','Valeur / référence','Empreinte','Fonction / spécification'])
        for c in C:
            if c['fp']:w.writerow([c['ref'],c['val'],c['fp'],c['desc']])

if __name__=='__main__':
    write_schematic();project();b,fps,nets=make_board()
    print('Projet généré :',BASE,'; composants',len(fps),'nets',len(nets))
    for ref in ['F1','U2','U3','Q1']:print(ref,[(z.GetNumber(),p.ToMM(z.GetPosition().x),p.ToMM(z.GetPosition().y),z.GetNetname()) for z in fps[ref].Pads()])
