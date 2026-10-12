"""Routage reproductible du prototype: puissance manuelle, logique A* deux faces."""
import heapq,math,time,json
import numpy as np
import pcbnew as p
from construire_carte import ROOT,BASE,vec,HV
b=p.LoadBoard(str(BASE.with_suffix('.kicad_pcb')))
fps={f.GetReference():f for f in b.GetFootprints()}
netinfo={n.GetNetname().removeprefix('/'):n for n in b.GetNetInfo().NetsByNetcode().values() if n.GetNetCode()}
# Always run construire_carte.py first, so the native board starts unrouted.
if len(list(b.GetTracks())):raise RuntimeError('Relancer construire_carte.py avant router_carte.py')
F,B=p.F_Cu,p.B_Cu
def track(net,pts,width=.35,layer=F):
    for a,z in zip(pts,pts[1:]):
        if a==z:continue
        t=p.PCB_TRACK(b);t.SetStart(vec(*a));t.SetEnd(vec(*z));t.SetWidth(p.FromMM(width));t.SetLayer(layer);t.SetNet(netinfo[net]);b.Add(t)
def pad(ref,n):
    z=next(z for z in fps[ref].Pads() if z.GetNumber()==str(n));return (p.ToMM(z.GetPosition().x),p.ToMM(z.GetPosition().y))
# Main 5 A circuit. Short necks are confined to TO-220 pad escape.
track('N',[pad('J1',2),pad('J2',2)],4)
track('L_ENTREE',[pad('J1',1),(185,57),(175,57),(169.8,57)],4)
track('L_FUSIBLE',[(159.7,57),(154.5,57),(154.5,46.09),pad('U2',4)],4)
track('MT2',[pad('U2',5),(139.4,77),(182.54,77),(182.54,103)],4)
track('MT2',[(182.54,103),pad('Q1',2)],1.1)
track('L_POMPE',[pad('Q1',1),(180,117)],1.1)
track('L_POMPE',[(180,117),(185,128),pad('J2',1)],4)
# HV trigger, gate reference and snubber.
track('MT2',[pad('R6',1),(135,77),(139.4,77)],.6,F)
track('R_G_SERIE',[pad('R6',2),pad('R7',1)],.6,B)
track('OPTO_MT2',[pad('R7',2),(163,93),(163,99),(130,99),(130,108),pad('U3',6)],.6,B)
track('GATE',[pad('U3',4),(134,113.08),(134,124),(162,124),pad('R8',1)],.6,B)
track('GATE',[pad('R8',1),(162,125),(190,125),(190,110),pad('Q1',3)],.6,B)
track('L_POMPE',[pad('R8',2),(177,121),pad('Q1',1)],.6,B)
track('MT2',[pad('R9',1),(147,100),(147,85),(150,77)],.6,F)
track('SNUBBER',[pad('R9',2),(165.24,106),(146,106),(146,133),pad('C5',1)],.6,F)
track('L_POMPE',[pad('C5',2),(175,133),(175,128),(180,117)],.6,F)

STEP=.25; X0=26.;Y0=26.;NX=int((120-X0)/STEP)+1;NY=int((154-Y0)/STEP)+1
def grid(pt):return (round((pt[0]-X0)/STEP),round((pt[1]-Y0)/STEP))
def mm(g):return (X0+g[0]*STEP,Y0+g[1]*STEP)
PAD=[]
for f in b.GetFootprints():
    for z in f.Pads():
        pos=(p.ToMM(z.GetPosition().x),p.ToMM(z.GetPosition().y));size=(p.ToMM(z.GetSize().x),p.ToMM(z.GetSize().y));ang=math.radians(z.GetOrientationDegrees())
        PAD.append((z.GetNetname().removeprefix('/'),pos,size,ang,z.GetShape(),z))
def raster(mask,box,fn):
    x1,y1,x2,y2=box
    ix1=max(0,math.floor((x1-X0)/STEP));ix2=min(NX-1,math.ceil((x2-X0)/STEP));iy1=max(0,math.floor((y1-Y0)/STEP));iy2=min(NY-1,math.ceil((y2-Y0)/STEP))
    if ix2<ix1 or iy2<iy1:return
    xx=X0+np.arange(ix1,ix2+1)[None,:]*STEP;yy=Y0+np.arange(iy1,iy2+1)[:,None]*STEP
    mask[iy1:iy2+1,ix1:ix2+1] |=fn(xx,yy)
def capsule(mask,a,z,r):
    x,y=a;dx=z[0]-x;dy=z[1]-y;l=dx*dx+dy*dy
    def fn(xx,yy):
        u=np.clip(((xx-x)*dx+(yy-y)*dy)/l,0,1) if l else 0
        return (xx-x-u*dx)**2+(yy-y-u*dy)**2<=r*r
    raster(mask,(min(x,z[0])-r,min(y,z[1])-r,max(x,z[0])+r,max(y,z[1])+r),fn)
def masks(net,via=False):
    m=np.zeros((2,NY,NX),bool);radius=.4 if via else .175
    for name,(x,y),(sx,sy),ang,shape,z in PAD:
        if name==net:continue
        extra=(8 if name in HV else .25)+radius+.055
        ca,sa=math.cos(ang),math.sin(ang)
        bx=abs(ca)*sx/2+abs(sa)*sy/2+extra;by=abs(sa)*sx/2+abs(ca)*sy/2+extra
        def fn(xx,yy):
            u=abs((xx-x)*ca+(yy-y)*sa)-sx/2;v=abs(-(xx-x)*sa+(yy-y)*ca)-sy/2
            return np.maximum(u,0)**2+np.maximum(v,0)**2<=extra*extra
        for layer in range(2):raster(m[layer],(x-bx,y-by,x+bx,y+by),fn)
    for t in b.GetTracks():
        name=t.GetNetname().removeprefix('/')
        if name==net:continue
        a=(p.ToMM(t.GetStart().x),p.ToMM(t.GetStart().y));z=(p.ToMM(t.GetEnd().x),p.ToMM(t.GetEnd().y));cl=8 if name in HV else .25
        rad=p.ToMM(t.GetWidth())/2+cl+radius+.065
        if isinstance(t,p.PCB_VIA):
            for layer in range(2):capsule(m[layer],a,a,rad)
        else:capsule(m[0 if t.GetLayer()==F else 1],a,z,rad)
    # Keep logic away from power zone, board edge and mounting holes.
    m[:,:,int((119-X0)/STEP):]=True
    for x,y in [(30,30),(30,150)]:
        for layer in range(2):capsule(m[layer],(x,y),(x,y),1.6+.5+radius)
    return m

MOV=[(1,0,10),(-1,0,10),(0,1,10),(0,-1,10),(1,1,14),(-1,1,14),(1,-1,14),(-1,-1,14)]
def astar(start,goal,m,vm):
    sx,sy=start;gx,gy=goal
    def h(x,y):
        dx=abs(gx-x);dy=abs(gy-y);return 10*max(dx,dy)+4*min(dx,dy)
    heap=[];best={};parent={}
    for l in [0,1]:
        if not m[l,sy,sx]:
            state=(sx,sy,l);best[state]=0;heapq.heappush(heap,(h(sx,sy),0,state))
    end=None;count=0
    while heap:
        _,g,s=heapq.heappop(heap)
        if best.get(s)!=g:continue
        x,y,l=s
        if (x,y)==goal:end=s;break
        count+=1
        if count>850000:break
        for dx,dy,cost in MOV:
            xx,yy=x+dx,y+dy
            if not(0<=xx<NX and 0<=yy<NY) or m[l,yy,xx]:continue
            if dx and dy and (m[l,y,xx] or m[l,yy,x]):continue
            ns=(xx,yy,l);gg=g+cost
            if gg<best.get(ns,10**12):best[ns]=gg;parent[ns]=s;heapq.heappush(heap,(gg+h(xx,yy),gg,ns))
        if not vm[0,y,x] and not vm[1,y,x]:
            ns=(x,y,1-l);gg=g+100
            if gg<best.get(ns,10**12):best[ns]=gg;parent[ns]=s;heapq.heappush(heap,(gg+h(x,y),gg,ns))
    if end is None:return None
    path=[end]
    while path[-1] in parent:path.append(parent[path[-1]])
    path.reverse();return path

def emit(net,path,a,z):
    current=[a,mm(path[0][:2])];layer=path[0][2]
    for i in range(1,len(path)):
        s=path[i]
        if s[2]!=layer:
            current.append(mm(s[:2]));track(net,current,.35,F if layer==0 else B)
            v=p.PCB_VIA(b);v.SetPosition(vec(*mm(s[:2])));v.SetWidth(p.FromMM(.8));v.SetDrill(p.FromMM(.4));v.SetViaType(p.VIATYPE_THROUGH);v.SetLayerPair(F,B);v.SetNet(netinfo[net])
            if not any(isinstance(t,p.PCB_VIA) and t.GetPosition()==v.GetPosition() and t.GetNetCode()==v.GetNetCode() for t in b.GetTracks()):b.Add(v)
            layer=s[2];current=[mm(s[:2])]
        elif i==len(path)-1 or path[i-1][2]!=s[2] or (i+1<len(path) and ((s[0]-path[i-1][0],s[1]-path[i-1][1])!=(path[i+1][0]-s[0],path[i+1][1]-s[1]) or path[i+1][2]!=s[2])):
            current.append(mm(s[:2]))
    current.append(z);track(net,current,.35,F if layer==0 else B)

def route():
    # Small nets first; large supply trees last. Complete pads rather than airwires.
    groups={}
    for name,xy,sz,an,sh,z in PAD:
        if name and name not in HV and not name.startswith('unconnected-'):groups.setdefault(name,[]).append(xy)
    for net,pts in sorted(groups.items(),key=lambda kv:(len(kv[1]),kv[0])):
        connected=[pts[0]];remaining=pts[1:];n=0
        while remaining:
            pairs=sorted((math.dist(a,z),a,z) for a in connected for z in remaining)
            m=masks(net);vm=masks(net,True);done=False
            for _,a,z in pairs:
                path=astar(grid(a),grid(z),m,vm)
                if path:
                    emit(net,path,a,z);connected.append(z);remaining.remove(z);n+=1;done=True;break
            if not done:raise RuntimeError('Impossible de router '+net+' '+str(remaining))
        print(net,n,'branches',flush=True)
    p.SaveBoard(str(BASE.with_suffix('.kicad_pcb')),b)
    print('Routage terminé',len(list(b.GetTracks())),'segments/vias',flush=True)
if __name__=='__main__':route()
