import {} from "piu/MC";
import Button from "pebble/button";

const BM=["","Bahá","Jalál","Jamál","'Aẓamat","Núr","Raḥmat","Kalimát","Kamál","Asmá","'Izzat","Mashíyyat","'Ilm","Qudrat","Qawl","Masá'il","Sharaf","Sulṭán","Mulk","'Alá"];
const BE=["","Splendour","Glory","Beauty","Grandeur","Light","Mercy","Words","Perfection","Names","Might","Will","Knowledge","Power","Speech","Questions","Honour","Sovereignty","Dominion","Loftiness"];

function ly(y){return(y%4===0&&y%100!==0)||(y%400===0);}
function getBadi(){const d=new Date(),y=d.getFullYear(),l=ly(y),md=[0,31,59,90,120,151,181,212,243,273,304,334];let doy=md[d.getMonth()]+d.getDate();if(d.getMonth()>1&&l)doy++;const n=l?80:79;let by=y-1843,ds=doy>=n?doy-n:doy+(ly(y-1)?366:365)-n;if(doy<n)by=y-1844;const a=l?5:4;let m,day,ia=false;if(ds<342){m=Math.floor(ds/19)+1;day=(ds%19)+1;}else if(ds<342+a){ia=true;day=ds-342+1;}else{m=19;day=ds-342-a+1;}return{str:ia?("Ayyám-i-Há "+day):(day+" "+BM[m]+" ("+BE[m]+")"),year:by};}

const OBLIG_T="Short Obligatory";
const GENER_T="O Thou Whose face...\x01O Thou kind Lord!\x01Prayer for a child\x01O God, my Master\x01Prayer for the Dead\x01Suffer me, O my God\x01Glory to Thee\x01O God, the Goal\x01Glory be unto Thee\x01Glory be to Thee, O King\x01Glorified art Thou (I)\x01O God, my God!\x01Glorified art Thou (II)\x01He is God! Thou seest us\x01O God my God! I beg\x01My God, Whom I worship\x01Thy unity is inscrutable\x01He is the Bestower\x01O my God and my Master\x01Glory be to Thee\x01Unto Thee be praise\x01All praise, O my God\x01Lauded be Thy name\x01Praise be to Thee\x01Magnified be Thy name\x01O God of all Names\x01From sweet-scented streams\x01I magnify Thy Name\x01Magnified be Thy name (II)\x01O Thou incomparable God\x01O Thou Whose tests heal\x01Praise be to Thee (II)\x01Thou seest, O my God\x01O my God! Unite hearts\x01O Thou Whose face (II)\x01O Thou kind Lord! (II)";
const OCCAS_T="Naw-Rúz\x01Magnified art Thou, O Lord\x01Intercalary Days\x01We are servants\x01Tablet of Ahmad";

const CATS=[{t:"Obligatory",i:"O",n:1,T:OBLIG_T},{t:"General",i:"G",n:35,T:GENER_T},{t:"Occasional",i:"C",n:5,T:OCCAS_T}];

let pData=null;
function getP(c,i){
    if(!pData){
        try{const R=require("Resource");pData=JSON.parse((new R("PRAYERS")).read(String));}
        catch(e){
            try{const f=device.files.openFile({path:"PRAYERS"});const s=f.status().size;const b=f.read(s,0);f.close();pData=JSON.parse(String.fromArrayBuffer(b));}
            catch(e2){return{title:"Error",by:"",text:"No prayer data."};}
        }
    }
    let g=0;for(let j=0;j<c;j++)g+=CATS[j].n;g+=i;const p=pData[g];
    return p?{title:p.title,by:p.by,text:p.text}:{title:"?",by:"",text:""};
}

const bgS=new Skin({fill:"#1a1a2e"}),hdS=new Skin({fill:"black"}),itS=new Skin({fill:["#1a1a2e","#0f3460"]});
const hSt=new Style({font:"bold 18px Gothic",color:"#e8e8e8",horizontal:"center",top:4,bottom:4});
const rSt=new Style({font:"bold 18px Gothic",color:"#e94560",horizontal:"center",top:2,bottom:2});
const iSt=new Style({font:"bold 18px Gothic",color:["#e8e8e8","white"],horizontal:"left",left:10});
const sSt=new Style({font:"14px Gothic",color:["#b0b0b0","white"],horizontal:"left",left:10});
const bSt=new Style({font:"14px Gothic",color:"#e8e8e8",horizontal:"left",leading:4,left:10,right:10,top:5,bottom:5});
const gSt=new Style({font:"14px Gothic",color:"#555",horizontal:"center",top:2,bottom:2});

let nav={screen:"main",mi:0,si:0,ci:0,so:0,count:0};
const MENU=[{t:"Obligatory",i:"O",type:"data",ci:0},{t:"General",i:"G",type:"data",ci:1},{t:"Occasional",i:"C",type:"data",ci:2},{t:"Alláh-u-Abhá",i:"9",type:"counter"},{t:"Badí Date",i:"B",type:"badi"}];

function hl(a,i){const c=a.first;if(!c)return;let ch=c.first,j=-1;while(ch){if(j>=0)ch.state=(j===i)?1:0;ch=ch.next;j++;}}

class AppB extends Behavior{
    onButtonPush(a,d,t){
        if(!d)return;
        if(nav.screen==="main"){const n=MENU.length;if(t==="up"){nav.mi=(nav.mi-1+n)%n;hl(a,nav.mi);}else if(t==="down"){nav.mi=(nav.mi+1)%n;hl(a,nav.mi);}else if(t==="select"){const it=MENU[nav.mi];if(it.type==="data"){nav.ci=it.ci;nav.si=0;nav.screen="sub";a.remove(a.first);a.add(new SubCol(CATS[it.ci],{}));}else if(it.type==="counter"){nav.count=0;nav.screen="counter";a.remove(a.first);a.add(new CountCol({}));}else if(it.type==="badi"){nav.screen="badi";a.remove(a.first);a.add(new BadiCol({}));}}}
        else if(nav.screen==="sub"){const n=CATS[nav.ci].n;if(t==="up"){nav.si=(nav.si-1+n)%n;hl(a,nav.si);}else if(t==="down"){nav.si=(nav.si+1)%n;hl(a,nav.si);}else if(t==="select"){nav.so=0;nav.screen="reader";const p=getP(nav.ci,nav.si);a.remove(a.first);a.add(new ReadCol(p,{}));}else if(t==="back"){nav.screen="main";a.remove(a.first);a.add(new MainCol(MENU,{}));}}
        else if(nav.screen==="reader"){if(t==="up"){nav.so=Math.max(0,nav.so-25);a.first.delegate("onScroll",nav.so);}else if(t==="down"){nav.so+=25;a.first.delegate("onScroll",nav.so);}else if(t==="back"){nav.screen="sub";a.remove(a.first);a.add(new SubCol(CATS[nav.ci],{}));}}
        else if(nav.screen==="counter"){if(t==="up"||t==="select"){if(nav.count<95){nav.count++;a.first.delegate("onCount",nav.count);}}else if(t==="down"){if(nav.count>0){nav.count--;a.first.delegate("onCount",nav.count);}}else if(t==="back"){nav.screen="main";a.remove(a.first);a.add(new MainCol(MENU,{}));}}
        else if(nav.screen==="badi"){if(t==="back"||t==="select"){nav.screen="main";a.remove(a.first);a.add(new MainCol(MENU,{}));}}
    }
    onMenuTap(a,i){nav.mi=i;this.onButtonPush(a,false,"select");}
    onSubTap(a,i){nav.si=i;this.onButtonPush(a,false,"select");}
}

class ReadB extends Behavior{onDisplaying(c){this.sc=c.last;}onScroll(c,o){if(this.sc)this.sc.scrollTo(0,o);}}
class CountB extends Behavior{onDisplaying(c){this.lbl=c.first.next;}onCount(c,n){if(this.lbl)this.lbl.string=String(n)+" / 95";}}

const MainItem=Row.template($=>({top:0,height:34,left:0,right:0,skin:itS,active:true,contents:[Label(null,{width:28,left:4,style:rSt,string:$.i}),Label(null,{left:0,right:0,style:iSt,string:$.t})],Behavior:class extends Behavior{onTouchEnded(r){r.bubble("onMenuTap",$.idx);}}}));
const SubItem=Label.template($=>({top:0,height:32,left:0,right:0,skin:itS,style:sSt,active:true,string:$.title,Behavior:class extends Behavior{onTouchEnded(l){l.bubble("onSubTap",$.idx);}}}));
const MainCol=Column.template($=>({top:0,bottom:0,left:0,right:0,skin:bgS,contents:[Label(null,{top:0,height:30,left:0,right:0,skin:hdS,style:hSt,string:"Prayer Book"}),...$.map((c,i)=>new MainItem({t:c.t,i:c.i,idx:i}))]}));
const SubCol=Column.template($=>({top:0,bottom:0,left:0,right:0,skin:bgS,contents:[Label(null,{top:0,height:30,left:0,right:0,skin:hdS,style:hSt,string:$.t}),...rng($.n).map(i=>{const t=$.T.split("\x01");return new SubItem({title:t[i],idx:i});})]}));
const ReadCol=Column.template($=>({top:0,bottom:0,left:0,right:0,skin:bgS,contents:[Label(null,{top:0,height:26,left:0,right:0,skin:hdS,style:rSt,string:$.title}),new Scroller(null,{left:0,right:0,top:0,bottom:0,active:true,backgroundTouch:true,contents:[new Text(null,{left:5,right:5,top:5,style:bSt,string:$.text+"\n\n— "+$.by})]})],Behavior:ReadB}));
const CountCol=Column.template($=>({top:0,bottom:0,left:0,right:0,skin:bgS,active:true,contents:[Label(null,{top:0,height:30,left:0,right:0,skin:hdS,style:hSt,string:"Alláh-u-Abhá"}),Label(null,{top:15,height:40,left:0,right:0,style:rSt,string:"0 / 95"}),Label(null,{top:10,height:30,left:0,right:0,style:gSt,string:"Up/Select: count\nDown: undo\nBack: exit"})],Behavior:CountB}));
const BadiCol=Column.template($=>{const b=getBadi();return{top:0,bottom:0,left:0,right:0,skin:bgS,contents:[Label(null,{top:0,height:30,left:0,right:0,skin:hdS,style:hSt,string:"Badí Calendar"}),Label(null,{top:15,height:30,left:10,right:10,style:rSt,string:b.str}),Label(null,{top:5,height:24,left:10,right:10,style:gSt,string:b.year+" B.E."}),Label(null,{top:10,height:20,left:0,right:0,style:gSt,string:"Back: menu"})]};});

function rng(n){const a=[];for(let i=0;i<n;i++)a.push(i);return a;}

const App=Application.template($=>({skin:bgS,style:hSt,contents:[new MainCol(MENU,{})],Behavior:AppB}));
const application=new App(null,{displayListLength:4096,touchCount:0,pixels:screen.width*4});
new Button({types:["select","up","down","back"],onPush(d,t){application.delegate("onButtonPush",d,t);}});
export default application;
