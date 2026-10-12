import { readFileSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';
import * as k from './kit/iso-kit.mjs';
import * as G from './kit/lathe.mjs';
import * as A from './kit/audit.mjs';
import { kitScript } from './scripts/inline-kit.mjs';

const HERE = dirname(fileURLToPath(import.meta.url));
const data = JSON.parse(readFileSync(join(HERE, 'implantation.json'), 'utf8'));
const theme = process.argv.includes('--dark') ? 'dark' : 'light';
const scale = 3;
const at = (x, y, z=0) => [(x-140)*scale, (y-95)*scale, z*scale];
const plan = (x,y,w,d,r=0.5) => ({x:(x-140)*scale,y:(y-95)*scale,w:w*scale,d:d*scale,r:r*scale});
const bench = plan(64,34,130,120,3);
const board = plan(100,70,90,80,0.3);
const P = k.fitProjection([...k.boxCorners(bench,-14,50),...k.boxCorners(board,0,18)],1000,660,{pad:60,azimuth:38,elevation:42});
const solids=[];
const R=A.recorder(P);
let parts=0;
let layer=0;
const put=(point,svg,shape=null,ref='')=>{const owner=ref||(layer===0?'stand':'pcb');if(shape){shape.name=owner+'.part'+parts;shape.owner=owner;R.put({at:point,svg,piece:owner,shapes:[R.solid(shape)]});}else solids.push({point,svg,layer});parts++;};
const slab=(ref,pl,z,h,tone='mid',inner='')=>put([pl.x+pl.w/2,pl.y+pl.d/2,(z+h/2)*scale],k.solidSvg(k.slabOf(pl,z*scale,h*scale,P,6,.4),{tone,inner,className:ref?`part part-${ref}${h>.8?' package':''}`:''}),A.slab(pl,z*scale,h*scale,{},6),ref);
const cyl=(ref,x,y,z,r,h,tone='mid')=>put(at(x,y,z+h/2),k.solidSvg(k.cylinder(...at(x,y,0).slice(0,2),r*scale,z*scale,h*scale,P,28,.2),{tone,className:ref?`part part-${ref}`:''}),A.cylinder(...at(x,y,0).slice(0,2),r*scale,z*scale,h*scale),ref);
const wire=(ref,a,b,z,w=.45)=>{
 const dx=b.x-a.x,dy=b.y-a.y,len=Math.hypot(dx,dy),ux=dx/len,uy=dy/len;
 const ring=[[a.x-uy*w/2,a.y+ux*w/2],[b.x-uy*w/2,b.y+ux*w/2],[b.x+uy*w/2,b.y-ux*w/2],[a.x+uy*w/2,a.y-ux*w/2]].map(([x,y])=>at(x,y).slice(0,2));
 put(at((a.x+b.x)/2,(a.y+b.y)/2,z+.1),k.solidSvg(k.extrude(ring,z*scale,.2*scale,P),{tone:'lo',className:`part part-${ref}`}),A.extruded(ring,z*scale,.2*scale),ref);
};
const info={
 U1:['Traitement','PIC16F1788, 5 V. Analyse du courant et décision. L’entrée RA0 ne rejoint pas encore le net AN0 du capteur. Oscillateur interne et configuration numérique du LCD à prévoir.'],
 U3:['Mesure de courant','Capteur nominal ACS756 ±100 A : le courant doit traverser IP+ et IP−. Ces broches sont ouvertes et le composant est hors PCB. Le conducteur primaire reste isolé de VCC/GND/VIOUT.'],
 TRIAC1:['Commande optique','Optotriac : une LED transmet la commande à travers une barrière optique. La sortie déclenche le triac de puissance, sans porter le courant moteur. Commande non raccordée, composant hors carte et modèle nominal FODM3011 à revoir pour 230 V.'],
 DS1:['Affichage','LCD Displaytech 162B 16 × 2 en interface 4 bits. Numérotation spécifique du module contrôlée. Le corps du LCD dépasse le bord ; ses broches restent sur la carte.'],
 J1:['Alimentation','Entrée 5 V externe et GND. Ce bornier ne représente pas l’entrée secteur 230 V.'],
 J2:['Communication','UART TTL : GND broche 1, RX du PIC broche 4, TX du PIC broche 5. Ne pas confondre TTL et RS-232.'],
 J4:['Programmation','PICkit : MCLR/VPP, +5 V, GND, ICSPDAT, ICSPCLK. Le strap rouge MCLR est conservé.'],
 SW4:['Reset','Bouton de remise à zéro du PIC, relié à MCLR. Ce n’est pas encore un acquittement logiciel indépendant.'],
 C1:['Réserve d’énergie','47 µF sur le rail +5 V. Tension de service et référence fabricant à préciser.'],
 C2:['Découplage','100 nF pour la logique. Le condensateur est à environ 33,49 mm de VDD du PIC : implantation à améliorer lors d’une prochaine correction.'],
 C5:['Découplage LCD','100 nF entre +5 V et GND près de l’afficheur.'],
 R3:['Commande de gâchette','Résistance présente sur les deux fichiers, mais valeur et nets du PCB encore issus de l’ancienne carte : 10 kΩ sur PCB, R sans valeur dans le schéma.'],
 R4:['Branche moteur','Rôle et valeur à définir dans le schéma de puissance. Sur PCB, ancienne résistance 10 kΩ : les deux fichiers ne sont pas synchronisés.'],
 R7:['Courant LED optotriac','Résistance de commande à dimensionner. Le schéma la nomme R et le PCB garde 270 Ω avec d’anciens nets.'],
 R9:['Alerte sonore','270 Ω vers le buzzer au schéma. Buzzer absent du PCB ; son + est relié à GND et la polarité doit être revue.'],
 R10:['Alerte visuelle','270 Ω pour la LED D4. Courant approximatif 11 mA si une chute LED de 2 V est supposée ; modèle réel à préciser.'],
 R11:['Contraste LCD','56 kΩ, résistance haute du pont avec R13.'],
 R12:['Reset','10 kΩ de rappel de MCLR vers +5 V.'],
 R13:['Contraste LCD','1,5 kΩ à la masse. Avec R11 et 5 V, VO vaut environ 0,1304 V à vide.'],
 R14:['Rétroéclairage','270 Ω. Avec une LED nominale à 3,2 V, environ 6,7 mA : luminosité à vérifier.'],
 D4:['Indication locale','LED rouge d’alerte pilotée par le PIC via R10. Le modèle exact n’est pas renseigné.']
};

for(const [x,y] of [[70,40],[188,40],[70,146],[188,146]])cyl('',x,y,-5,3,2,'lo');
slab('',bench,-3,3,'lo');
layer=1;
slab('',board,0,1.6,'mid');
const screws=[[103,73],[187,73],[103,147],[187,147]];
for(const [x,y] of screws){cyl('',x,y,1.6,1.2,.25,'lo');}
layer=2;

for(const fp of data.components){
 const ref=fp.ref;
 const off=fp.x<100;
 const base=off?3.6:1.6;
 if(off)slab(ref,ref==='U3'?plan(69.5,84,20,26,1):plan(90.5,80,9,14,1),0,3.6,'lo');
 const unique=[...new Map(fp.pads.map(p=>[p.number,p])).values()];
 for(const p of unique){
  const r=Math.max(.45,Math.min(.9,Math.min(p.w,p.h)/2));
  cyl(ref,p.x,p.y,base,r,.12,'lo');
 }
 if(ref==='U1'){
  const x=139.57,y=114.69;
  for(const p of unique){cyl(ref,p.x,p.y,base+.12,.32,2.3,'mid');wire(ref,p,{x:p.x,y:p.y<y?111.7:117.7},base+2.4,.45);}
  slab(ref,plan(123,111.7,33,6,1),base+2.5,3.4,'hi');
  const engraving=k.faceTextSvg(k.topMatrix(at(x,y).slice(0,2),(base+5.9)*scale,P),'PIC16F1788',{size:6,tone:'mid',anchor:'middle'});
  put(at(x,y,base+5.95),engraving);
 }else if(ref==='DS1'){
  for(const p of unique)cyl(ref,p.x,p.y,base+.12,.35,2.4,'mid');
  slab(ref,plan(104,39.1,84,43.9,1),base+2.5,3.5,'mid');
  slab(ref,plan(108,48,76,24,.6),base+6,1.2,'hi');
  slab(ref,plan(113,51,65,18,.3),base+7.2,.12,'lo');
  for(let row=0;row<2;row++)for(let col=0;col<16;col++){
   slab(ref,plan(115+col*3.6,53+row*7.5,2.5,5.1,.1),base+7.33,.06,'faint');
  }
 }else if(ref==='C1'){
  for(const p of unique)cyl(ref,p.x,p.y,base+.12,.25,1.1,'mid');
  cyl(ref,fp.x,fp.y,base+1.2,3.1,10.5,'mid');
  put(at(fp.x,fp.y,base+11.7),k.lineSvg(k.ring(...at(fp.x,fp.y).slice(0,2),2.3*scale,(base+11.7)*scale,P),{tone:'lo'}));
 }else if(ref.startsWith('R')){
  const [a,b]=unique;
  const dx=b.x-a.x,dy=b.y-a.y,l=Math.hypot(dx,dy),ux=dx/l,uy=dy/l;
  const mid={x:(a.x+b.x)/2,y:(a.y+b.y)/2};
  for(const [p,sign] of [[a,-1],[b,1]]){
   cyl(ref,p.x,p.y,base+.12,.23,1.5,'mid');
   wire(ref,p,{x:mid.x+sign*ux*3.2,y:mid.y+sign*uy*3.2},base+1.6,.32);
  }
  const F=G.frameAlong(at(mid.x-ux*3.2,mid.y-uy*3.2,base+1.6),[ux,uy,0]);
  const profile=[[0,0],[0,3.2],[19.2,3.2],[19.2,0]];
  put(at(mid.x,mid.y,base+1.6),k.solidSvg(G.lathe(profile,F,P,{steps:32}),{tone:'mid',className:`part part-${ref}`}),A.body(F,profile),ref);
 }else if(['C2','C5'].includes(ref)){
  const [a,b]=unique,mid={x:(a.x+b.x)/2,y:(a.y+b.y)/2};
  const dx=b.x-a.x,dy=b.y-a.y,len=Math.hypot(dx,dy),ux=dx/len,uy=dy/len;
  for(const [p,sign] of [[a,-1],[b,1]]){cyl(ref,p.x,p.y,base+.12,.22,3.5,'mid');wire(ref,p,{x:mid.x+sign*ux*2.36,y:mid.y+sign*uy*2.36},base+3.6,.32);}
  const F=G.frameAlong(at(mid.x-uy*1.25,mid.y+ux*1.25,base+4.0),[uy,-ux,0]);
  const profile=[[0,0],[0,7.05],[7.5,7.05],[7.5,0]];
  put(at(mid.x,mid.y,base+4),k.solidSvg(G.lathe(profile,F,P,{steps:32}),{tone:'mid',className:`part part-${ref}`}),A.body(F,profile),ref);
 }else if(ref==='J1'){
  slab(ref,plan(181,87.5,8,10,1),base+.12,7,'mid');
  for(const p of unique){cyl(ref,p.x,p.y,base+7.12,1.25,.4,'hi');put(at(p.x,p.y,base+7.52),k.lineSvg(k.lineOnTop(at(p.x-.75,p.y).slice(0,2),at(p.x+.75,p.y).slice(0,2),(base+7.52)*scale,P),{tone:'lo'}));}
 }else if(ref==='J2'||ref==='J4'){
  const xs=unique.map(p=>p.x),ys=unique.map(p=>p.y);slab(ref,plan(Math.min(...xs)-1.2,Math.min(...ys)-1.2,Math.max(...xs)-Math.min(...xs)+2.4,Math.max(...ys)-Math.min(...ys)+2.4,.3),base+.12,2.2,'hi');
  for(const p of unique)slab(ref,plan(p.x-.32,p.y-.32,.64,.64,.08),base+2.32,3.4,'mid');
 }else if(ref==='SW4'){
  for(const p of unique)cyl(ref,p.x,p.y,base+.12,.3,1.2,'mid');
  const cx=unique.reduce((a,p)=>a+p.x,0)/unique.length,cy=unique.reduce((a,p)=>a+p.y,0)/unique.length;
  slab(ref,plan(cx-3,cy-3,6,6,.4),base+1.3,2.5,'mid');cyl(ref,cx,cy,base+3.8,1.5,1.8,'hi');
 }else if(ref==='D4'){
  for(const p of unique)cyl(ref,p.x,p.y,base+.12,.23,2.1,'mid');
  const x=unique.reduce((a,p)=>a+p.x,0)/unique.length,y=unique.reduce((a,p)=>a+p.y,0)/unique.length;
  cyl(ref,x,y,base+2.2,2.5,6,'hi');
 }else if(ref==='U3'){
  slab(ref,plan(70,87,18.8,15,1),base+.12,6,'mid');
  for(const p of unique)if(Number(p.number)<=3){cyl(ref,p.x,p.y,base+.12,.3,2,'mid');wire(ref,p,{x:p.x,y:102.02},base+2.12,.45);}
 }else if(ref==='TRIAC1'){
  for(const p of unique){wire(ref,p,{x:p.x<fp.x?fp.x-2.8:fp.x+2.8,y:p.y},base+.12,.6);}
  slab(ref,plan(fp.x-2.8,fp.y-2.1,5.6,4.2,.35),base+.32,2.5,'hi');
 }
}

A.settle(R,P);
R.items.sort((a,b)=>a.key-b.key);
const art=R.items.map(x=>x.svg).join('')+solids.map(x=>x.svg).join('');
const svg=k.figureSvg({width:1000,height:660,label:'Implantation simplifiée du PCB G206 et composants encore hors carte',body:art});
const buttons=['U1','U3','TRIAC1','DS1','J4','J2','SW4'].map(ref=>`<button type="button" data-ref="${ref}">${ref} · ${info[ref][0]}</button>`).join('');
const card=k.plateHtml({fig:'Fig 1',title:'Carte de surveillance du vide-cave',hint:'Cliquer un composant · flèches pour parcourir',readout:'U1 · traitement',keys:[{mark:'raised',label:'21 empreintes conservées dans KiCad'},{mark:'lit',label:'Composant sélectionné'}],caption:'Positions X/Y extraites du PCB nettoyé. Les boîtiers, hauteurs et supports sont simplifiés pour expliquer le projet ; cette vue n’est pas un modèle de fabrication. U3 et TRIAC1 restent hors du contour de la carte.',body:`<div class="iso-stage" tabindex="0" role="group" aria-label="Carte interactive">${svg}</div>`});
const details=`<section class="details" aria-live="polite"><div class="eyebrow">Explorer la carte</div><nav>${buttons}</nav><h2 data-name>U1 · Traitement</h2><p data-description>${info.U1[1]}</p><p class="status">Le schéma reste incomplet : mesure de phase absente, liaisons capteur/PIC et commande/opto ouvertes, triac de puissance et buzzer à définir.</p></section>`;
const copper=`<section class="chain"><h2>Le cuivre réel dans KiCad</h2><img class="copper" alt="PCB nettoyé : cuivre bleu B.Cu et deux straps rouges F.Cu ; capteur et optotriac hors contour" src="data:image/png;base64,${readFileSync(join(HERE,'pcb-nettoye.png')).toString('base64')}"><p>Export du fichier PCB après nettoyage. Cette vue sert à comparer l’illustration aux pistes réellement enregistrées. Elle ne représente pas un routage terminé de la puissance.</p></section>`;
const body=`<header><p class="eyebrow">Projet GEII · version en cours</p><h1>Comprendre notre carte vide-cave</h1><p>Le PIC lit le courant, prend la décision et commande l’arrêt de la pompe à travers une isolation optique. La mesure du déphasage demande aussi une référence de la tension secteur.</p></header>${card}${details}<section class="chain"><h2>Commande et puissance</h2><p><strong>Commande :</strong> sortie RB2 du PIC → résistance R7 → LED de l’optotriac. <strong>Isolation :</strong> la lumière transmet la commande ; les masses ne traversent pas la barrière. <strong>Puissance :</strong> sortie de l’optotriac → gâchette du triac → circuit moteur, avec capteur de courant dans le conducteur primaire.</p><p>Cette chaîne décrit le principe à réaliser. Dans le schéma actuel, RB2 et « commande octo coupleur » ne sont pas le même net ; AN0 et « capteur de courant » non plus. IP+ et IP− ne sont pas raccordés.</p><p><strong>Routage :</strong> face bleue B.Cu, avec seulement deux straps rouges F.Cu conservés : GND 14 mm et MCLR 10 mm. Le triac de puissance n’a pas encore d’empreinte définie ; aucune pièce de remplacement n’est inventée dans cette illustration.</p></section><footer><a href="../05_Notes/Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md">Rapport technique</a> · <a href="../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv">Composants CSV</a> · <a href="../05_Notes/Obsidian/00%20Accueil.md">Notes Obsidian</a><p>Illustration réalisée avec <a href="https://github.com/wheresryan22/anatomy">Anatomy</a> · KiCad est la source de vérité du PCB.</p></footer>`;
const live=`
const infos=${JSON.stringify(info)};
const refs=${JSON.stringify(data.components.map(x=>x.ref).sort())};
const stage=document.querySelector('.iso-stage'), readout=document.querySelector('[data-readout]');
let selected='U1', visible=true;
const still=matchMedia('(prefers-reduced-motion: reduce)');
function select(ref){if(!infos[ref])return;selected=ref;document.querySelectorAll('.part').forEach(e=>e.classList.toggle('selected',e.classList.contains('part-'+ref)));document.querySelector('[data-name]').textContent=ref+' · '+infos[ref][0];document.querySelector('[data-description]').textContent=infos[ref][1];readout.textContent=ref+' · '+infos[ref][0];document.querySelectorAll('[data-ref]').forEach(e=>e.setAttribute('aria-pressed',String(e.dataset.ref===ref)));}
document.querySelectorAll('[data-ref]').forEach(e=>e.addEventListener('click',()=>select(e.dataset.ref)));
document.querySelectorAll('.part').forEach(e=>{const ref=[...e.classList].find(c=>c.startsWith('part-'))?.slice(5);if(ref){e.addEventListener('pointerenter',()=>select(ref));e.addEventListener('click',()=>select(ref));}});
stage.addEventListener('keydown',e=>{if(!['ArrowLeft','ArrowRight','Home','End'].includes(e.key))return;e.preventDefault();let n=refs.indexOf(selected);n=e.key==='Home'?0:e.key==='End'?refs.length-1:(n+(e.key==='ArrowRight'?1:-1)+refs.length)%refs.length;select(refs[n]);});
new IntersectionObserver(es=>{visible=es.at(-1).isIntersecting;stage.classList.toggle('paused',!visible)}).observe(stage);
select('U1');
`;
const css=`<style>header{margin-bottom:28px}h1{font-size:clamp(25px,4vw,40px);line-height:1.12;letter-spacing:-1px}h2{font-size:21px;line-height:1.3}.eyebrow{text-transform:uppercase;font-size:11px;letter-spacing:1.5px;font-weight:700;opacity:.6}.details,.chain{border:1px solid var(--anatomy-outline);border-radius:14px;padding:24px;margin:22px 0}nav{display:flex;gap:8px;flex-wrap:wrap}button{border:1px solid #aeb5be;border-radius:8px;background:transparent;color:inherit;padding:8px 11px;cursor:pointer;font:inherit;font-size:12px}button[aria-pressed=true]{background:#182331;color:white}.status{border-left:3px solid #b48b43;padding-left:12px;font-size:13px}.part{cursor:pointer;transition:opacity .25s,filter .25s}.part.selected{filter:drop-shadow(0px 0px 2px #728499)}footer{font-size:12px;opacity:.75}a{color:inherit;text-underline-offset:3px}.iso-stage{outline-offset:4px}.iso-plate{overflow:hidden}@media(max-width:550px){.details,.chain{padding:16px}.iso-plate-corner{font-size:9px}nav{gap:5px}button{font-size:11px}}@media(prefers-reduced-motion:reduce){.part{transition:none}}</style>`;
const materials=`<style>.package{--anatomy-paper:#6b7480;--anatomy-top:#59636e;--anatomy-shade-0:#343e49;--anatomy-shade-1:#414c58;--anatomy-shade-2:#53606d;--anatomy-shade-3:#697684}.package>.iso-edge{stroke:#25303c}.package.selected{--anatomy-top:#8595a5}.copper{display:block;width:100%;height:auto;border-radius:8px}body[data-theme="dark"] .package{--anatomy-top:#8291a0;--anatomy-shade-0:#3d4a57;--anatomy-shade-1:#4e5f70;--anatomy-shade-2:#687a8b;--anatomy-shade-3:#92a3b3}body[data-theme="dark"] .package>.iso-edge{stroke:#d6dfe7}</style>`;
const html=k.pageHtml({title:'Carte vide-cave — visualisation Anatomy',theme,width:1040,body:body.replace('</footer>', '</footer>'+copper).replace('Obsidian/00%20Accueil.md','Obsidian/00%20-%20Accueil.md'),script:kitScript()+live}).replace('</head>',css+materials+'</head>').replace('<html lang="en">','<html lang="fr">');
writeFileSync(join(HERE,theme==='light'?'index.html':'index-dark.html'),html);
writeFileSync(join(HERE,'carte.svg'),k.figureSvg({width:1000,height:660,label:'Carte vide-cave G206, implantation simplifiée',standalone:'light',body:art}));
writeFileSync(join(HERE,'parts.json'),JSON.stringify({physical_parts:parts,schematic_components:24,pcb_footprints:21,geometry:'simplified packages; X/Y from KiCad; not fabrication geometry'},null,2));
console.log(`Built ${theme}: ${data.components.length} footprints, ${parts} physical illustration parts.`);
A.auditOrExit(R,P);
