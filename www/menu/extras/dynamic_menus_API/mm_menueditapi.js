/*
   Milonic DHTML Menu Editing API Module  mm_menueditapi.js version 2.9 - April 10 2006
   This module is only compatible with the Milonic DHTML Menu version 5.742 or higher

   Copyright 2005 (c) Milonic Solutions Limited. All Rights Reserved.
   This is a commercial software product, please visit http://www.milonic.com/ for more information.

Syntax:


     mm_createNewMenus ()                                          -- mm_createNewMenus()
            mm_addMenu ()                                          -- mm_addMenu()
   mm_returnChildMenus (menuRef)                                   -- mm_returnChildMenus("sub Menu")
   mm_deleteChildMenus (menuRef)                                   -- mm_deleteChildMenus(3)
         mm_deleteMenu (menuRef)                                   -- mm_deleteMenu("Sub Menu To Delete")
         mm_insertItem (menuRef,itemNumber,'aI Text',rebuildMenus) -- mm_insertItem('Main Menu',2,'text=New Item;url=test.html;offcolor=yellow;offbgcolor=darkblue;') // Inserts a new item at position 2 in Main Menu
   mm_addItemByItemRef (itemRef,'aI Text')                         -- mm_addItemByItemRef(22,'text=New Item;url=test.html;')
           mm_editItem (menuRef,itemNumber,'aI Text',rebuildmenus) -- mm_editItem('MainMenu',4,'text=Item;url=index.php')  // Modifies the details of menu item 4 in Main Menu
  mm_editItemByItemRef (itemRef,'aI Text')                         -- mm_editItemByItemRef (22,'text=New Text;fontweight=bold;')
         mm_deleteItem (menuRef,itemNumber)                        -- mm_deleteItem('Main Menu',3);  Removes third item in Main Menu
mm_deleteItemByItemRef (itemRef)                                   -- mm_deleteItemByItemRef(24)
          mm_shiftItem (itemRef,numberOfShifts)                    -- mm_shiftItem(12,-1)
         mmm$Pos (itemRef)                                   -- mmm$Pos(22)

*/

function mmm$Pos(i){
	var M=_m[_mi[i][0]][0]
	for(var a=0;a<M.length;a++)if(M[a]==i)return a;
	return -1
}

function _gM3nu(m){
	if(isNaN(m))_mN=$h(m); else _mN=m
	return _mN
}

function _rbMenus(m){
	for(var r=0;r<_m.length;r++){
		_m[r][23]=0;
		_gm=$F("menu"+r)
		if(_gm.style.visibility==$6){
			_gm.innerHTML=o$(r);
			p$(r);
			_mcnt--;
		}
		else{
			_gm.innerHTML="";
			$z(m);
		}
	}
}

function _excMD(d) { 
	_it=d.split(":"); 
	return _it[1].replace(/;/g,"") 
} 

function mm_deleteItemByItemRef(r){
	$m=_mi[r][0]
	_mis=_m[$m][0]
	for(var b=0;b<_mis.length;b++)if(_mis[b]==r)mm_deleteItem($m, b+1)
}

function mm_addItemByItemRef(a,i){
	var $m=_mi[a][0]
	_mis=_m[$m][0]
	
	for(var b=0;b<_mis.length;b++){
		if(_mis[b]==a){
			mm_insertItem($m, (b+1), i);
			return
		}
		
	}
}

function mm_editItemByItemRef(i,t){
	$m=_mi[i][0]
	_mis=_m[$m][0]
	for(var b=0;b<_mis.length;b++)if(_mis[b]==i)mm_editItem($m, b, t)
}

function mm_deleteItem(_mN, _iN){
	_mnO=_gM3nu(_mN)
	_ii=_m[_mnO][0][_iN-1];
	_tA=[];
	for(_r=0;_r<_mi.length;_r++){
		if(_r!=_ii)_tA[_tA.length]=_mi[_r];
	}
	_mi=_tA;
	_tA=[];
	_cnt=0;
	for(_r=0;_r<_m[_mnO][0].length;_r++){
		if(_m[_mnO][0][_r]!=_ii){
			_tA[_tA.length]=_m[_mnO][0][_cnt];
			_cnt++;
		}
	}
	_m[_mnO][0]=_tA;
	for(_r=_mnO+1;_r<_m.length;_r++){
		for(_p=0;_p<_m[_r][0].length;_p++){
			_m[_r][0][_p]--;
		}
	}
	_rbMenus(_mnO);
}

function _zeroShift(m,i){
	var b=_m[m][0].length-1
	_blB=_bl
	for(var a=0;a<b;a++){
		if(i==0)mm_shiftItem(_blB,-1,1)
		_blB--
	}
}

_mmiI_TO=_n
function mm_insertItem()
{
	var r=arguments
	_mN=r[0];
	_iN=r[1];
	_aI=r[2];
	_mn=_gM3nu(_mN)
	_ii=_m[_mn][0][_iN-1];
	if(_ii+" "==$u)_ii=_m[_mn][0][_m[_mn][0].length-1];
	_tA=[];
	for(_r=0;_r<_mi.length;_r++){
		if(_r==_ii){
			_tA[_tA.length]=_mi[_r];
			_c=_mn;
			_x=_m[_c];
			_bl=_ii;
			f_(_aI);
			//udtb("in "+ _bl + " - " +_m[_mn][0] + " - " + _iN)
		}
		_tA[_tA.length]=_mi[_r];
	}
	_mi=_tA;
	for(_r=0;_r<_m.length;_r++){
		_tA=[];
		for(_p=0;_p<_mi.length;_p++){
			if(_mi[_p][0]==_r)_tA[_tA.length]=_p;
		}
		_m[_r][0]=_tA;
	}
	
	if(_startM){
		_mmiI_TO=$P(_mmiI_TO)	
		_mmiI_TO=_StO("_zeroShift("+_mn+","+_iN+");_rbMenus("+_mn+")",150);
		return
	}
	_zeroShift(_mn,_iN)
	if(!r[3])_rbMenus(_mn)
}





function mm_editItem()
{
	var r=arguments
	_mN=r[0];
	_iN=r[1];
	i=r[2];
	_mnO=_gM3nu(_mN)
	_ii=_m[_mnO][0][_iN];
	i=i.split(";");
	_sc=""
	for(var a=0;a<i.length;a++){		
		var p=i[a].indexOf("`");
		if(p!=-1){
			_sc=";"
			_tI=i[a]
			if(p==i[a].lastIndexOf("`")){
				for(var b=a;b<i.length;b++){
					if(i[b+1]){
						_tI+=";"+i[b+1];
						a++;
						if(i[b+1].indexOf("`")!=-1)b=i.length;
					}
				}
			}
			i[a]=_tI.replace(/`/g,"")
		}

		p=i[a].indexOf("=");	
		if(p==-1){
			if(i[a])_si=_si+";"+i[a]+_sc
		}
		else{
			_si=i[a].slice(p+1);
			_w=i[a].slice(0,p);	
			if(_w=="showmenu")_si=$tL(_si)
		}
		_mi[_ii][_$S[_w]]=_si;
	}
	if(!r[3])_rbMenus(_mnO);

}


function mm_shiftItem(i,s)
{
	var g=arguments, s=g[1],i=g[0], rB=g[2]
	$m=_mi[i][0]
	_mni=_m[$m][0]
	_sWi=(i+s)
	if(_sWi>=_mni[0]&&_sWi<=_mni[_mni.length-1]){
		var t=[]
		for(var r=0;r<_mi[i].length;r++){
			t[r]=_mi[i][r]
			_mi[i][r]=_mi[_sWi][r]
			_mi[_sWi][r]=t[r]
		}
		if(!rB){
			_rbMenus($m)
			selectedItem=_sWi
			h$(selectedItem)
		}
		
	}
}

function mm_addMenu()
{	
	var a=arguments
	var n=_m.length;
	with(milonic=new menuname("newmenu"+n)){
		style=a[0];
		alwaysvisible=true;
		if(a[1])eval(a[1])
		_itemProps=""
		if(a[2])_itemProps=a[2]
		_bl=_mi.length
		aI("text=New Item;"+_itemProps);
	}
	mm_createNewMenus();
	popup("newmenu"+n,1)
	return n;
}


function mm_createNewMenus() { 
	$r();
	_startM=0
	for(var y=_mcnt;y<_m.length;y++) { 
		var M=_m[y]
		var o=_d.createElement("div") 
		o.id="menu"+y 
		o.onmouseout=new Function("$I()"); 
		o.onmouseover=new Function("$J("+y+")"); 
		o.onselectstart=new Function("return _f"); 
		if(_dB.appendChild){
			_dB.appendChild(o) 
			o$(y,0) 
			o.className=_cls 
			var n=o.style 
			if($1B)n.padding=_excMD($1B) 
			if(M[17])n.width=M[17]+"px"
			if(M[24])n.height=M[24]+"px"
			if(_ofb)n.background=_excMD(_ofb) 
			if(p_)n.border=_excMD(p_) 
			if(_wid)n.width=_excMD(_wid) 
			o.style.zindex=999 
			o.style.visibility=_visi 
			if(n_)n.position=_excMD(n_) 
			if($k)n.top=_excMD($k) 
			if($l)n.left=_excMD($l) 
			if(_bgimg)n.backgroundImage="url("+_excMD(_bgimg)+")"; 
			if(_mbgc)n.background=_excMD(_mbgc) 
			M[23]=0 
		}
	} 
} 

function mm_deleteMenu(m)
{
	m=_gM3nu(m)
	if(_m[m]){
		_mLth=_m[m][0].length
		for(var r=1;r<_mLth+1;r++)mm_deleteItem(m, 1)
	}
	var t=[]
	for(var r=0;r<m;r++)t[r]=_m[r]
	_gmo=$F("menu"+m)
	if(!_gmo)return
	_gmo.id="mm_deleted"
	_dB.removeChild(_gmo)
	for(var r=m+1;r<_m.length;r++){
		for(var q=0;q<_m[r][0].length;q++)_mi[_m[r][0][q]][0]=_mi[_m[r][0][q]][0]-1
		t[r-1]=_m[r]
		$F("menu"+(r)).id="menu"+(r-1)
		if($F("mmlink"+(r)))$F("mmlink"+(r)).id="mmlink"+(r-1)
	}
	_trueItemRef=-1
	_m=t
	_itemRef=-1
	_mcnt--
	_mn=_mcnt-1
}

rCI=_n
function mm_returnChildMenus(m){
	m=_gM3nu(m)
	if(m+" "==$u)return
	if(rCI.length==0)rCI[rCI.length]=_m[m][1]
	for(var a=0; a<_m[m][0].length;a++){
		var i=_m[m][0][a]
		var I=_mi[i]
		if(I[3]){
			rCI[rCI.length]=I[3]
			mm_returnChildMenus(I[3])
		}
	}
}

function mm_deleteChildMenus(m){
	rCI=[];
	mm_returnChildMenus(m)
	for(var a=0;a<rCI.length;a++){
		mm_deleteMenu(rCI[a])
	}
	
}