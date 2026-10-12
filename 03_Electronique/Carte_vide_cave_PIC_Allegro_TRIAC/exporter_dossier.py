"""Exports KiCad natifs, cartouches d'impression et rendu de contrôle PDF.
Exécuter avec un Python disposant de reportlab et pypdf (runtime Codex).
"""
from pathlib import Path
import subprocess,json,io,zipfile
from pypdf import PdfReader,PdfWriter
from pypdf.generic import ContentStream,FloatObject
from reportlab.pdfgen import canvas
from reportlab.lib.units import mm
ROOT=Path(__file__).resolve().parent
BASE=ROOT/'Vide_cave_5A';OUT=ROOT/'exports';T=OUT/'typons';G=OUT/'gerbers'
CLI=Path(r'C:\Users\rafae\AppData\Local\Programs\KiCad\8.0\bin\kicad-cli.exe')
POPP=Path(r'C:\Users\rafae\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\poppler\Library\bin\pdftoppm.exe')
for d in [OUT,T,G,OUT/'controle']:d.mkdir(exist_ok=True,parents=True)
def run(args):
    r=subprocess.run([str(CLI)]+[str(x) for x in args],capture_output=True,text=True)
    if r.returncode:raise RuntimeError(r.stdout+r.stderr)
    return r.stdout
def pdf_board(path,layers,mirror=False,bw=False):
    run(['pcb','export','pdf',BASE.with_suffix('.kicad_pcb'),'-o',path,'-l',layers,'--drill-shape-opt','2']+(['--mirror'] if mirror else [])+(['--black-and-white'] if bw else []))
def stamp(path,title,info,recolor=False):
    r=PdfReader(path);writer=PdfWriter();writer.add_page(r.pages[0]);page=writer.pages[0];w=float(page.mediabox.width);h=float(page.mediabox.height)
    if recolor:
        content=ContentStream(page.get_contents(),writer)
        for args,op in content.operations:
            if op in [b'rg',b'RG'] and len(args)==3:
                red,green,blue=map(float,args)
                if red>.6 and green>.6 and blue<.7:args[:]=list(map(FloatObject,[.22,.23,.18]))
                elif .8<red<.99 and .8<green<.99 and .8<blue<.99:args[:]=list(map(FloatObject,[.38,.38,.38]))
        page.replace_contents(content)
    stream=io.BytesIO();c=canvas.Canvas(stream,pagesize=(w,h))
    c.setFont('Helvetica-Bold',12);c.drawString(25*mm,h-13*mm,title)
    c.setFont('Helvetica',9);c.drawString(25*mm,h-19*mm,info)
    c.setFont('Helvetica',8);c.drawString(25*mm,h-166*mm,'Prototype 230 V / pompe ~300 W : faire valider avant fabrication et mise sous tension.')
    c.setLineWidth(.3);c.line(25*mm,h-176*mm,45*mm,h-176*mm)
    for x in [25,45]:c.line(x*mm,h-174.5*mm,x*mm,h-177.5*mm)
    c.drawString(50*mm,h-177*mm,'20 mm - imprimer a 100 %, taille reelle, sans ajustement.')
    c.save();page.merge_page(PdfReader(stream).pages[0])
    with path.open('wb') as f:writer.write(f)

run(['sch','export','pdf',BASE.with_suffix('.kicad_sch'),'-o',OUT/'Schema.pdf','--exclude-drawing-sheet','--exclude-pdf-property-popups','--no-background-color'])
pdf_board(OUT/'Implantation.pdf','F.Cu,B.Cu,F.Silkscreen,Edge.Cuts')
stamp(OUT/'Implantation.pdf','Implantation - vide-cave 230 V / environ 300 W','Vue du dessus : rouge = F.Cu ; bleu = B.Cu ; composants en traversant.',True)
pdf_board(T/'F_Cu_contact_miroir.pdf','F.Cu,Edge.Cuts',True,True)
stamp(T/'F_Cu_contact_miroir.pdf','Typon face composants F.Cu - miroir','Cuivre noir positif. Film : face imprimee au contact du cuivre avant insolation.')
pdf_board(T/'B_Cu_contact.pdf','B.Cu,Edge.Cuts',False,True)
stamp(T/'B_Cu_contact.pdf','Typon face soudures B.Cu - contact','Geometrie B.Cu vue depuis le dessus. Face imprimee au contact du cuivre, selon procede.')
for name,layer,mir in [('F_Cu_contact_miroir','F.Cu',True),('B_Cu_contact','B.Cu',False)]:
    run(['pcb','export','svg',BASE.with_suffix('.kicad_pcb'),'-o',T/(name+'.svg'),'-l',layer+',Edge.Cuts','--black-and-white','--exclude-drawing-sheet','--page-size-mode','2','--drill-shape-opt','2']+(['--mirror'] if mir else []))
run(['pcb','export','gerbers',BASE.with_suffix('.kicad_pcb'),'-o',str(G)+'/', '-l','F.Cu,B.Cu,F.Mask,B.Mask,F.Silkscreen,Edge.Cuts','--subtract-soldermask'])
run(['pcb','export','drill',BASE.with_suffix('.kicad_pcb'),'-o',str(G)+'/', '--format','excellon','--excellon-units','mm','--excellon-oval-format','route','--excellon-separate-th'])
for file in [OUT/'Schema.pdf',OUT/'Implantation.pdf',T/'F_Cu_contact_miroir.pdf',T/'B_Cu_contact.pdf']:
    subprocess.run([str(POPP),'-singlefile','-r','180','-png',str(file),str(file.with_suffix(''))],capture_output=True,check=True)
summary={}
for name in ['ERC','DRC']:
    d=json.loads((OUT/'controle'/(name+'.json')).read_text(encoding='utf8'))
    if name=='ERC':summary[name]=sum(len(s['violations']) for s in d['sheets'])
    else:summary.update({key:len(d[key]) for key in ['violations','unconnected_items','schematic_parity']})
(OUT/'controle/Resume.json').write_text(json.dumps(summary,indent=2),encoding='utf8')
with zipfile.ZipFile(OUT/'Gerbers_prototype.zip','w',zipfile.ZIP_DEFLATED) as z:
    for file in G.iterdir():z.write(file,file.name)
print('Exports terminés',summary)
