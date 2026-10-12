// Proposition pédagogique, indépendante des fichiers KiCad.
// Charge R-L équivalente : ce modèle ne décrit pas le démarrage mécanique d'une pompe.
export function dimensionner({rled=150, gain=1, irms=2, phi=35, igtd=10, rg=360, rgk=330}={}) {
  const omega=2*Math.PI*50, sens=.020, lsb=5/4096;
  const imin=(4.75-1.5-.2)/(rled*1.05);
  const imax=(5.25-1.1-.1)/(rled*.95);
  const inom=(5-1.25-.2)/rled;
  const r=230/irms*Math.cos(phi*Math.PI/180);
  const l=230/irms*Math.sin(phi*Math.PI/180)/omega;
  const vgate=3+1.3+rg*(igtd/1000+1.3/rgk);
  return {rled,gain,irms,phi,igtd,rg,rgk,imin,imax,inom,pmax:imax*imax*rled*.95,
    r,l,lsb,ampsPerCode:lsb/(sens*gain),sensorPeak:sens*irms*Math.SQRT2,
    headroomRms:2.3/(sens*gain*Math.SQRT2),vgate,
    fc:1/(2*Math.PI*1000*10e-9),filterPhase:Math.atan(omega*1000*10e-9)*180/Math.PI};
}
export function simuler(parameters={}) {
  const d=dimensionner(parameters), dt=10e-6, n=8000, samples=[];
  let current=0, conducting=false, armed=false, onTime=null, offTime=null;
  for(let k=0;k<=n;k++) {
    const t=k*dt, v=230*Math.SQRT2*Math.sin(2*Math.PI*50*t);
    const command=t>=.005&&t<.035;
    // Fenêtre de 20 V : plafond nominal de la fiche, PAS seuil garanti de chaque pièce.
    // Première occasion d'amorçage : ensuite commande maintenue IDEALE.
    // Le réamorçage réel MOC/triac sur charge inductive exige un modèle fabricant.
    if(!command)armed=false;
    if(command&&d.imin>=.015&&Math.abs(v)<=20&&Math.abs(v)>=d.vgate)armed=true;
    const gate=command&&armed;
    if(!conducting&&gate){conducting=true;if(onTime===null)onTime=t;}
    if(conducting){
      const next=d.l>1e-9?current+dt*(v-d.r*current)/d.l:v/d.r;
      if(current*next<0){current=0;conducting=false;if(!command)offTime=t;}
      else current=next;
      if(d.l<=1e-9&&!command&&Math.abs(v)<1){current=0;conducting=false;offTime=t;}
    }
    const sensor=2.5+.020*current, ideal=2.5+d.gain*(sensor-2.5);
    const adc=Math.min(4.8,Math.max(.2,ideal));
    if(k%8===0)samples.push({t,v,current,command,conducting,sensor,adc,clipped:ideal<.2||ideal>4.8});
  }
  return {d,samples,onTime,offTime,clipped:samples.some(p=>p.clipped)};
}
