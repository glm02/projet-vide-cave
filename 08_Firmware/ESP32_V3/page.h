#pragma once

// Jetons de couleur et style des boutons et tags repris de kargulstudio/sales-crm (MIT), voir THIRD_PARTY.md.
static const char PAGE[] PROGMEM = R"html(<!doctype html>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Vide-cave</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Geist:wght@400;500;600&family=Geist+Mono:wght@400;500&display=swap">
<style>
:root{color-scheme:dark;--fond:#161616;--texte:#f9fbff;--carte:#1b1d20;--second:#1e1e1e;--muted:#2a2a2a;--gris:#7f7f7f;--subtle:#676767;--soft:#a4a4a4;--chip:#cfcfcf;--bord:#232323;--bord-fort:#393939;--ring:#676767;--accent:#4124fb;--succes:#22c55e;--alerte:#fbbf24;--danger:#f97373;--barre:#171717;
--vert-bg:#1f3a2d;--vert-bd:#275137;--vert-tx:#b1ebc5;--rouge-bg:#3e1d1e;--rouge-bd:#4c2324;--rouge-tx:#febfc6;--ambre-bg:#31221b;--ambre-bd:#6c4830;--ambre-tx:#fed7aa;--bleu-bg:#1d2b3e;--bleu-bd:#23354c;--bleu-tx:#bfdbfe;--neutre-bg:#2a2a2a;--neutre-bd:#363636;--neutre-tx:#cfcfcf;
--ease:cubic-bezier(.25,1,.5,1);--sans:Geist,system-ui,-apple-system,"Segoe UI",sans-serif;--mono:"Geist Mono",ui-monospace,Consolas,monospace}
*{box-sizing:border-box}
html,body{margin:0;height:100%}
body{background:var(--fond);color:var(--texte);font:14px/1.15 var(--sans);-webkit-font-smoothing:antialiased}
button{font:inherit;color:inherit}
:focus-visible{outline:2px solid rgba(103,103,103,.6);outline-offset:2px}
.app{display:flex;min-height:100%}
aside{width:254px;flex:none;background:var(--barre);border-right:1px solid var(--bord);padding:14px 10px;display:flex;flex-direction:column;gap:18px}
.marque{display:flex;align-items:center;gap:10px;padding:4px 8px;font-weight:500;font-size:16px}
.logo{width:22px;height:22px;border-radius:6px;background:var(--accent);box-shadow:inset 0 4px 6px rgba(255,255,255,.2),inset 0 0 0 1px rgba(255,255,255,.15)}
.surtitre{font-size:12px;text-transform:uppercase;letter-spacing:1px;color:var(--subtle);padding:0 8px 6px}
nav{display:flex;flex-direction:column;gap:2px}
.nav{display:flex;align-items:center;justify-content:space-between;width:100%;padding:8px;border:0;background:none;border-radius:8px;color:var(--gris);cursor:pointer;text-align:left;transition:background-color .15s var(--ease),color .15s var(--ease)}
.nav:hover{color:var(--texte)}
.nav[data-actif=true]{background:var(--muted);color:var(--texte);box-shadow:0 0 0 1px rgba(0,0,0,.4),inset 0 1px 0 rgba(255,255,255,.1),inset 0 0 0 1px rgba(255,255,255,.06)}
.compteur{font:12px var(--mono);color:var(--subtle)}
.pied{margin-top:auto;padding:8px;font-size:12px;color:var(--subtle);line-height:1.5}
.principal{flex:1;min-width:0;display:flex;flex-direction:column}
.entete{height:52px;display:flex;align-items:center;justify-content:space-between;gap:12px;padding:0 20px;border-bottom:1px solid var(--bord)}
.entete h1{font-size:16px;font-weight:500;margin:0;display:flex;gap:8px;align-items:center}
.entete h1 span{color:var(--gris);font-weight:400}
.actions{display:flex;gap:8px;align-items:center}
.contenu{padding:20px;max-width:1100px;width:100%}
.vue{display:none}.vue.active{display:block}
.btn{display:inline-flex;align-items:center;gap:6px;border:0;border-radius:999px;padding:9px;font-size:12px;font-weight:500;line-height:1;cursor:pointer;white-space:nowrap;transition:background-color .15s var(--ease)}
.btn.secondaire{background:var(--second);box-shadow:0 0 0 1px rgba(0,0,0,.4),inset 0 1px 0 rgba(255,255,255,.1),inset 0 0 0 1px rgba(255,255,255,.06)}
.btn.secondaire:hover{background:var(--muted)}
.btn.primaire{background:var(--accent);padding:9px 12px;box-shadow:0 4px 4px rgba(42,42,42,.32),0 0 0 1px #0e0e0e,inset 0 4px 6px rgba(255,255,255,.2),inset 0 0 0 1px rgba(255,255,255,.15),inset 0 -8px 14px rgba(0,0,0,.15)}
.btn.primaire:hover{background:#4b30ff}
.btn:disabled{opacity:.5;pointer-events:none}
.tag{display:inline-flex;align-items:center;gap:6px;height:22px;padding:0 8px;border-radius:999px;border:1px solid var(--neutre-bd);background:var(--neutre-bg);color:var(--neutre-tx);font-size:14px;white-space:nowrap}
.tag::before{content:"";width:6px;height:6px;border-radius:50%;background:currentColor}
.tag.vert{border-color:var(--vert-bd);background:var(--vert-bg);color:var(--vert-tx)}
.tag.rouge{border-color:var(--rouge-bd);background:var(--rouge-bg);color:var(--rouge-tx)}
.tag.ambre{border-color:var(--ambre-bd);background:var(--ambre-bg);color:var(--ambre-tx)}
.tag.bleu{border-color:var(--bleu-bd);background:var(--bleu-bg);color:var(--bleu-tx)}
.bandeau{display:none;align-items:baseline;flex-wrap:wrap;gap:4px 12px;padding:12px 14px;margin-bottom:16px;border:1px solid var(--rouge-bd);background:var(--rouge-bg);color:var(--rouge-tx);border-radius:8px}
.bandeau.visible{display:flex}
.bandeau b{font-weight:600}
.grille{display:grid;grid-template-columns:minmax(0,360px) minmax(0,1fr);gap:16px;align-items:start}
.panneau{background:var(--carte);border:1px solid var(--bord);border-radius:8px}
.panneau h2{font-size:12px;text-transform:uppercase;letter-spacing:1px;color:var(--subtle);font-weight:500;margin:0;padding:14px 14px 6px}
dl{margin:0;padding:0 14px 6px}
dl div{display:flex;justify-content:space-between;gap:12px;padding:10px 0;border-top:1px solid var(--bord)}
dl div:first-child{border-top:0}
dt{color:var(--gris)}
dd{margin:0;text-align:right;font-variant-numeric:tabular-nums}
.mono{font-family:var(--mono);font-size:13px}
.graphe{padding:4px 14px 14px}
canvas{width:100%;display:block}
.legende{display:flex;gap:14px;flex-wrap:wrap;font-size:12px;color:var(--gris);padding:0 14px 14px}
.legende i{display:inline-block;width:10px;height:2px;margin-right:6px;vertical-align:middle;border-radius:2px}
.outils{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:12px;flex-wrap:wrap}
.segments{display:inline-flex;gap:2px;padding:2px;background:var(--second);border:1px solid var(--bord);border-radius:999px}
.segments button{border:0;background:none;color:var(--gris);padding:6px 12px;border-radius:999px;font-size:12px;cursor:pointer;display:inline-flex;gap:6px;transition:background-color .15s var(--ease),color .15s var(--ease)}
.segments button[data-actif=true]{background:var(--muted);color:var(--texte)}
.segments .compteur{font-size:11px}
table{width:100%;border-collapse:collapse}
th{font-weight:400;color:var(--gris);text-align:left;font-size:12px;padding:10px 14px;border-bottom:1px solid var(--bord)}
td{padding:10px 14px;border-bottom:1px solid var(--bord);vertical-align:middle}
tbody tr:hover{background:rgba(255,255,255,.03)}
td.date{width:130px;color:var(--soft)}
td.type{width:130px}
tfoot td{color:var(--gris);font-size:12px;border-bottom:0}
.vide td{color:var(--gris);text-align:center;padding:28px}
.onglets-mobile{display:none}
@media (max-width:760px){
 aside{display:none}
 .entete{padding:0 16px}
 .contenu{padding:16px}
 .grille{grid-template-columns:1fr}
 .onglets-mobile{display:flex;gap:2px;padding:8px 16px;border-bottom:1px solid var(--bord);overflow-x:auto}
 .onglets-mobile .nav{width:auto;padding:8px 10px;gap:6px}
 td.date{width:auto}
 tbody tr{display:grid;grid-template-columns:1fr auto;border-bottom:1px solid var(--bord)}
 tbody td{border:0;padding:4px 14px}
 tbody td.date{grid-row:1;padding-top:10px;font-size:12px}
 tbody td.type{grid-row:1;grid-column:2;padding-top:10px;width:auto}
 tbody td.texte{grid-column:1/3;padding-bottom:10px}
 thead{display:none}
}
</style>
</head>
<body>
<div class="app">
<aside>
 <div class="marque"><span class="logo"></span>Vide-cave</div>
 <div><div class="surtitre">Supervision</div>
 <nav>
  <button class="nav" data-vue="etat">État <span class="compteur" id="navEtat"></span></button>
  <button class="nav" data-vue="historique">Historique <span class="compteur" id="navHisto"></span></button>
  <button class="nav" data-vue="evenements">Événements <span class="compteur" id="navEvts"></span></button>
 </nav></div>
 <div class="pied" id="reseau">Connexion…</div>
</aside>
<div class="principal">
 <header class="entete"><h1 id="titre">État</h1><div class="actions"><span class="tag" id="tagEtat">…</span></div></header>
 <div class="onglets-mobile">
  <button class="nav" data-vue="etat">État</button>
  <button class="nav" data-vue="historique">Historique</button>
  <button class="nav" data-vue="evenements">Événements <span class="compteur" id="navEvtsM"></span></button>
 </div>
 <main class="contenu">
  <section class="vue" id="vue-etat">
   <div class="bandeau" id="bandeau"><b>Moteur bloqué</b><span id="bandeauTexte"></span></div>
   <div class="grille">
    <div class="panneau"><h2>Pompe</h2><dl>
     <div><dt>État</dt><dd id="etat">—</dd></div>
     <div><dt>Courant (ADC)</dt><dd class="mono" id="adc">—</dd></div>
     <div><dt>Dernière trame du PIC</dt><dd id="age">—</dd></div>
     <div><dt>Température</dt><dd class="mono" id="temp">—</dd></div>
     <div><dt>Humidité</dt><dd class="mono" id="hum">—</dd></div>
     <div><dt>Alertes Telegram</dt><dd id="tg">—</dd></div>
     <div><dt>Heure de l'ESP32</dt><dd class="mono" id="heure">—</dd></div>
    </dl></div>
    <div class="panneau"><h2>Dernière heure</h2><div class="graphe"><canvas id="apercu" height="210"></canvas></div>
     <div class="legende"><span><i style="background:var(--chip)"></i>Courant ADC</span><span><i style="background:var(--succes)"></i>Marche</span><span><i style="background:var(--danger)"></i>Défaut</span><span><i style="background:var(--subtle)"></i>Arrêt</span></div></div>
   </div>
  </section>
  <section class="vue" id="vue-historique">
   <div class="outils"><div class="segments" id="plages"><button data-plage="1h" data-actif="true">1 h</button><button data-plage="24h">24 h</button></div></div>
   <div class="panneau" style="margin-bottom:16px"><h2>Climat de la cave</h2><div class="graphe"><canvas id="climat" height="220"></canvas></div>
    <div class="legende"><span><i style="background:var(--accent)"></i>Température (°C), axe gauche</span><span><i style="background:var(--soft)"></i>Humidité (%), axe droit</span></div></div>
   <div class="panneau"><h2>Courant et état de la pompe</h2><div class="graphe"><canvas id="courant" height="180"></canvas></div>
    <div class="legende"><span><i style="background:var(--chip)"></i>Courant ADC</span><span><i style="background:var(--succes)"></i>Marche</span><span><i style="background:var(--danger)"></i>Défaut</span><span><i style="background:var(--subtle)"></i>Arrêt</span></div></div>
  </section>
  <section class="vue" id="vue-evenements">
   <div class="outils"><div class="segments" id="filtres">
    <button data-filtre="tous" data-actif="true">Tous <span class="compteur" id="nTous">0</span></button>
    <button data-filtre="alertes">Alertes <span class="compteur" id="nAlertes">0</span></button>
    <button data-filtre="autres">Autres <span class="compteur" id="nAutres">0</span></button>
   </div><button class="btn primaire" id="test">Envoyer une alerte de test</button></div>
   <div class="panneau"><table><thead><tr><th>Date</th><th>Type</th><th>Message</th></tr></thead><tbody id="lignes"></tbody><tfoot><tr><td colspan="3" id="total"></td></tr></tfoot></table></div>
  </section>
 </main>
</div>
</div>
<script>
const $=id=>document.getElementById(id);
const V=n=>getComputedStyle(document.documentElement).getPropertyValue(n).trim();
const ETATS={ARRET:['À l’arrêt','neutre'],MARCHE:['En marche','vert'],DEFAUT:['Moteur bloqué','rouge'],INCONNU:['Pas de nouvelles du PIC','ambre']};
const TYPES={defaut:['Défaut','rouge'],rappel:['Rappel','rouge'],liaison:['Liaison','ambre'],retour:['Rétabli','vert'],etat:['État',''],test:['Test','bleu'],demarrage:['Démarrage','']};
const ALERTES=['defaut','rappel','liaison'];
const TITRES={etat:'État',historique:'Historique',evenements:'Événements'};
let vue='etat',plage='1h',points=[],apercu=[],evts=[],filtre='tous';
const fmt=(v,u,d=1)=>v==null?'—':v.toFixed(d).replace('.',',')+u;
const hm=t=>new Date(t*1000).toLocaleTimeString('fr-FR',{hour:'2-digit',minute:'2-digit'});
const date=t=>new Date(t*1000).toLocaleString('fr-FR',{day:'2-digit',month:'2-digit',hour:'2-digit',minute:'2-digit'});
async function lire(u){const r=await fetch(u,{cache:'no-store'});if(!r.ok)throw r.status;return r.json()}
function tag(el,texte,ton){el.className='tag'+(ton?' '+ton:'');el.textContent=texte}

function montrer(v){
 vue=TITRES[v]?v:'etat';
 document.querySelectorAll('.vue').forEach(s=>s.classList.toggle('active',s.id==='vue-'+vue));
 document.querySelectorAll('.nav').forEach(b=>b.dataset.actif=b.dataset.vue===vue);
 $('titre').textContent=TITRES[vue];
 if(location.hash!=='#'+vue)history.replaceState(null,'','#'+vue);
 dessiner();
}

async function etat(){
 try{
  const e=await lire('/api/etat'),[lib,ton]=ETATS[e.pompe];
  tag($('tagEtat'),lib,ton);
  $('etat').replaceChildren();const t=document.createElement('span');tag(t,lib,ton);$('etat').append(t);
  $('adc').textContent=e.adc==null?'—':e.adc+' / 4095';
  $('age').textContent=e.pic_age_s==null?'aucune':'il y a '+e.pic_age_s+' s';
  $('temp').textContent=fmt(e.temperature_c,' °C')+(e.climat==='simulation'?' (sim.)':'');
  $('hum').textContent=fmt(e.humidite_pct,' %',0);
  $('tg').textContent=e.telegram;
  $('heure').textContent=e.heure;
  $('bandeau').classList.toggle('visible',e.pompe==='DEFAUT');
  $('bandeauTexte').textContent=e.bloque_depuis?'depuis le '+date(e.bloque_depuis)+' · réarmer avec le bouton reset du PIC':'réarmer avec le bouton reset du PIC';
  $('navEtat').textContent=e.pompe==='DEFAUT'?'!':'';
  $('reseau').textContent=e.ip+' · '+e.wifi_dbm+' dBm\nallumé depuis '+Math.floor(e.uptime_s/60)+' min';
  $('reseau').style.whiteSpace='pre-line';
 }catch(_){tag($('tagEtat'),'ESP32 injoignable','ambre')}
}
async function historique(){
 try{
  apercu=(await lire('/api/historique?plage=1h')).points;
  points=plage==='1h'?apercu:(await lire('/api/historique?plage=24h')).points;
  $('navHisto').textContent=points.length||'';dessiner();
 }catch(_){}
}
async function evenements(){try{evts=(await lire('/api/evenements')).evenements;tableau()}catch(_){}}

function tableau(){
 const al=evts.filter(e=>ALERTES.includes(e[1]));
 $('nTous').textContent=evts.length;$('nAlertes').textContent=al.length;$('nAutres').textContent=evts.length-al.length;
 $('navEvts').textContent=$('navEvtsM').textContent=al.length?al.length:'';
 const l=evts.filter(e=>filtre==='tous'||(filtre==='alertes')===ALERTES.includes(e[1]));
 const tb=$('lignes');tb.replaceChildren();
 if(!l.length){tb.innerHTML='<tr class="vide"><td colspan="3">Aucun événement</td></tr>'}
 for(const [t,type,texte] of l){
  const tr=document.createElement('tr'),d=document.createElement('td'),y=document.createElement('td'),x=document.createElement('td');
  d.className='date mono';d.textContent=t?date(t):'—';
  y.className='type';const s=document.createElement('span');const [n,ton]=TYPES[type]||[type,''];tag(s,n,ton);y.append(s);
  x.className='texte';x.textContent=texte;tr.append(d,y,x);tb.append(tr);
 }
 $('total').textContent=l.length+' événement'+(l.length>1?'s':'')+' · 40 derniers conservés';
}

function toile(c){
 const r=devicePixelRatio||1,w=c.clientWidth,h=+(c.dataset.h||(c.dataset.h=c.getAttribute('height')));
 if(!w)return null;c.width=w*r;c.height=h*r;c.style.height=h+'px';
 const g=c.getContext('2d');g.setTransform(r,0,0,r,0,0);g.font='11px '+V('--mono');return [g,w,h];
}
function bornes(pts,duree){const now=Date.now()/1000,t1=pts.length?Math.max(pts[pts.length-1][0],now-5):now;return [t1-duree,t1]}
function axeX(g,w,h,x0,t0,t1){g.fillStyle=V('--subtle');g.textAlign='center';for(let i=0;i<=4;i++)g.fillText(hm(t0+(t1-t0)*i/4),x0+(w-x0-40)*i/4,h-4)}
function echelle(vals,marge){const v=vals.filter(x=>x!=null);if(!v.length)return null;let a=Math.min(...v),b=Math.max(...v);if(b-a<marge){const m=(a+b)/2;a=m-marge/2;b=m+marge/2}const p=(b-a)*.1;return [a-p,b+p]}
function grille(g,w,x0,haut,bas){g.strokeStyle=V('--bord');g.lineWidth=1;for(let i=0;i<=3;i++){const y=Math.round(haut+(bas-haut)*i/3)+.5;g.beginPath();g.moveTo(x0,y);g.lineTo(w-40,y);g.stroke()}}
function graduer(g,x,haut,bas,e,cote,col,dec){g.fillStyle=col;g.textAlign=cote;for(let i=0;i<=3;i++){const y=haut+(bas-haut)*i/3,v=e[1]-(e[1]-e[0])*i/3;g.fillText(v.toFixed(dec),x,y+4)}}
function courbe(g,pts,x,y,col,pas){g.strokeStyle=col;g.lineWidth=1.6;g.lineJoin='round';g.beginPath();let on=false,prev=0;
 for(const [t,v] of pts){if(v==null||t-prev>3*pas){on=false}if(v==null){prev=t;continue}const X=x(t),Y=y(v);on?g.lineTo(X,Y):g.moveTo(X,Y);on=true;prev=t}g.stroke()}
function vide(g,w,h){g.fillStyle=V('--gris');g.textAlign='center';g.font='13px '+V('--sans');g.fillText('Pas encore de mesures',w/2,h/2)}

function pompe(c,pts,duree,pas){
 const o=toile(c);if(!o)return;const [g,w,h]=o,x0=44,bande=8,bas=h-22-bande-8,haut=8,[t0,t1]=bornes(pts,duree);
 const x=t=>x0+(t-t0)/(t1-t0)*(w-x0-40);
 grille(g,w,x0,haut,bas);graduer(g,x0-8,haut,bas,[0,4095],'right',V('--subtle'),0);
 if(!pts.length)vide(g,w,h);
 const cols=[V('--subtle'),V('--succes'),V('--danger'),V('--bord')];
 for(const p of pts){g.fillStyle=cols[p[4]];const a=Math.max(x0,x(p[0]-pas));g.fillRect(a,h-22-bande,Math.max(1,x(p[0])-a+.6),bande)}
 courbe(g,pts.map(p=>[p[0],p[4]===3?null:p[3]]),x,v=>bas-v/4095*(bas-haut),V('--chip'),pas);
 axeX(g,w,h,x0,t0,t1);
}
function climat(c,pts,duree,pas){
 const o=toile(c);if(!o)return;const [g,w,h]=o,x0=44,haut=8,bas=h-22,[t0,t1]=bornes(pts,duree);
 const x=t=>x0+(t-t0)/(t1-t0)*(w-x0-40);grille(g,w,x0,haut,bas);
 const eT=echelle(pts.map(p=>p[1]),2),eH=echelle(pts.map(p=>p[2]),10);
 if(!eT&&!eH)vide(g,w,h);
 if(eT){graduer(g,x0-8,haut,bas,eT,'right',V('--soft'),1);courbe(g,pts.map(p=>[p[0],p[1]]),x,v=>bas-(v-eT[0])/(eT[1]-eT[0])*(bas-haut),V('--accent'),pas)}
 if(eH){graduer(g,w-34,haut,bas,eH,'left',V('--soft'),0);courbe(g,pts.map(p=>[p[0],p[2]]),x,v=>bas-(v-eH[0])/(eH[1]-eH[0])*(bas-haut),V('--soft'),pas)}
 axeX(g,w,h,x0,t0,t1);
}
function dessiner(){
 if(vue==='etat')pompe($('apercu'),apercu,3600,10);
 if(vue==='historique'){const d=plage==='1h'?3600:86400,p=plage==='1h'?10:60;climat($('climat'),points,d,p);pompe($('courant'),points,d,p)}
}

document.querySelectorAll('.nav').forEach(b=>b.onclick=()=>montrer(b.dataset.vue));
$('plages').onclick=e=>{const b=e.target.closest('button');if(!b)return;plage=b.dataset.plage;[...$('plages').children].forEach(o=>o.dataset.actif=o===b);historique()};
$('filtres').onclick=e=>{const b=e.target.closest('button');if(!b)return;filtre=b.dataset.filtre;[...$('filtres').children].forEach(o=>o.dataset.actif=o===b);tableau()};
$('test').onclick=async()=>{const b=$('test');b.disabled=true;
 try{const j=await(await fetch('/api/test-alerte',{method:'POST'})).json();b.textContent=j.ok?'Alerte envoyée':j.erreur}catch(_){b.textContent='Échec de l’envoi'}
 setTimeout(()=>{b.disabled=false;b.textContent='Envoyer une alerte de test';evenements();etat()},4000)};
addEventListener('resize',dessiner);
addEventListener('hashchange',()=>montrer(location.hash.slice(1)));
montrer(location.hash.slice(1));
etat();historique();evenements();
setInterval(etat,2000);setInterval(historique,10000);setInterval(evenements,5000);
</script>
</body>
</html>)html";
