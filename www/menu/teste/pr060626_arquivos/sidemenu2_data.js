_scrollAmount=3      // Used for Netscape 4 scrolling
_scrollDelay=20	     // Used for Netscape 4 scrolling

_menuCloseDelay=500  // The delay for menus to remain visible on mouse off
_menuOpenDelay=200   // The delay for opening menus on mouse over
//_followSpeed=0       // Follow Scrolling speed (higher number makes the scrolling smoother but slower)
//_followRate=0        // Follow Scrolling Rate (use a minimum of 40 or you may experience problems)
_subOffsetTop=5;     // Sub menu offset Top position
_subOffsetLeft=-10;  // Sub menu offset Left position


with(SideStyle=new mm_style()){
     onbgcolor = "#D3355A";
       oncolor = "#dddddd";
    offbgcolor = "#C2113A";
      offcolor = "#ffffff";
   bordercolor = "";
   borderstyle = "";
separatorcolor = "";
 separatorsize = 0;
       padding = 2;
      onborder = "";
      fontsize = 11;
     fontstyle = "normal";
    fontweight = "normal";
    fontfamily = "arial, helvetica, verdana";
   high3dcolor = null; // Not sure if this will be included in final release
    low3dcolor = null; // Not sure if this will be included in final release
     pagecolor = "#ffffff";
   pagebgcolor = "#C2113A";
   topbarimage = "";
topbarimageloc = "left;middle";
      subimage = "";
   subimageloc = "left;middle"
  //ondecoration = "underline"
  //onbold = true;
  //onitalic = true;
}




with(milonic=new menuname("mainmenu2")){
top = 145;
left = 562;
itemwidth = 188;
 menuwidth = 188;
borderwidth = 0;
//screenposition = "center;middle";
//alignment="center";
style = SideStyle;
alwaysvisible = 1;
//followscroll = "1,50,2"
//orientation="horizontal"
//filter = null;
//followscroll = null;
//keepalive = 1;
//overallwidth = null;
//righttoleft = null;
//itemheight=200;
//openonclick = null;
//bgimage="winxp_back.gif";
//position="relative"
//separatorcolor="green";
aI("text=&nbsp;&nbsp;&nbsp;&nbsp;Press Home &raquo;;url=/press/");
aI("text=<img src=\"http://www.usaid.gov/images/leftpoparrow.gif\" align=bottom width=10 border=0 height=10>&nbsp;Press Releases;url=/press/releases/;showmenu=pr");
aI("text=<img src=\"http://www.usaid.gov/images/leftpoparrow.gif\" align=bottom width=10 border=0 height=10>&nbsp;Fact Sheets;showmenu=fs;url=/press/factsheets/");
aI("text=<img src=\"http://www.usaid.gov/images/leftpoparrow.gif\" align=bottom width=10 border=0 height=10>&nbsp;Media Advisories;showmenu=ma;url=/press/mediaadvisories/");
aI("text=<img src=\"http://www.usaid.gov/images/leftpoparrow.gif\" align=bottom width=10 border=0 height=10>&nbsp;Speeches &amp; Testimony;showmenu=st;url=/press/speeches/");
aI("text=&nbsp;&nbsp;&nbsp;&nbsp;FrontLines &raquo;url=/press/frontlines/");
}

	with(milonic=new menuname("pr")){
	borderwidth = 0;
	style = SideStyle;
	itemwidth = 100;
	overfilter="Fade(duration=0.2);Shadow(color='#777777', Direction=135, Strength=5)"
	aI("text=&nbsp;&nbsp;2006;url=/press/releases/");
	aI("text=&nbsp;&nbsp;2005;url=/press/releases/2005/");
	aI("text=&nbsp;&nbsp;2004;url=/press/releases/2004/");
	aI("text=&nbsp;&nbsp;2003;url=/press/releases/2003/");
	aI("text=&nbsp;&nbsp;2002;url=/press/releases/2002/");
	aI("text=&nbsp;&nbsp;2001;url=/press/releases/2001/");
	aI("text=&nbsp;&nbsp;2000;url=/press/releases/2000/");
	aI("text=&nbsp;&nbsp;1999;url=/press/releases/99press.html");
	aI("text=&nbsp;&nbsp;1998;url=/press/releases/98press.html");
	aI("text=&nbsp;&nbsp;1997;url=/press/releases/97press.html");
	aI("text=&nbsp;&nbsp;1996;url=/press/releases/96press.html");
	}
	
			with(milonic=new menuname("fs")){
	borderwidth = 0;
	style = SideStyle;
	itemwidth = 100;
	overfilter="Fade(duration=0.2);Shadow(color='#777777', Direction=135, Strength=5)"
	aI("text=&nbsp;&nbsp;2006;url=/press/factsheets/");
	aI("text=&nbsp;&nbsp;2005;url=/press/factsheets/2005/");
	aI("text=&nbsp;&nbsp;2004;url=/press/factsheets/2004/");
	aI("text=&nbsp;&nbsp;2003;url=/press/factsheets/2003/");
	aI("text=&nbsp;&nbsp;2002;url=/press/releases/2002/fsindex.html");
	aI("text=&nbsp;&nbsp;2001;url=/press/releases/2001/fsindex.html");
	aI("text=&nbsp;&nbsp;2000;url=/press/releases/2000/fsindex.html");
	aI("text=&nbsp;&nbsp;1999;url=/press/releases/99fsindex.html");
	aI("text=&nbsp;&nbsp;1998;url=/press/releases/98fsindex.html");
	aI("text=&nbsp;&nbsp;1997;url=/press/releases/97press.html");
	aI("text=&nbsp;&nbsp;1996;url=/press/releases/96press.html");
	}
	
			with(milonic=new menuname("ma")){
	borderwidth = 0;
	style = SideStyle;
	itemwidth = 100;
	overfilter="Fade(duration=0.2);Shadow(color='#777777', Direction=135, Strength=5)"
	aI("text=&nbsp;&nbsp;2006;url=/press/mediaadvisories/");
	aI("text=&nbsp;&nbsp;2005;url=/press/mediaadvisories/2005");
	aI("text=&nbsp;&nbsp;2004;url=/press/mediaadvisories/2004/");
	aI("text=&nbsp;&nbsp;2003;url=/press/mediaadvisories/2003/");
	aI("text=&nbsp;&nbsp;2002;url=/press/releases/2002/maindex.html");
	}	
	
				with(milonic=new menuname("st")){
	borderwidth = 0;
	style = SideStyle;
	itemwidth = 100;
	overfilter="Fade(duration=0.2);Shadow(color='#777777', Direction=135, Strength=5)"
	aI("text=&nbsp;&nbsp;2006;url=/press/speeches/");
	aI("text=&nbsp;&nbsp;2005;url=/press/speeches/2005/");
	aI("text=&nbsp;&nbsp;2004;url=/press/speeches/2004/");
	aI("text=&nbsp;&nbsp;2003;url=/press/speeches/2003/");
	aI("text=&nbsp;&nbsp;2002;url=/press/spe_test/index_2002.html");
	aI("text=&nbsp;&nbsp;2001;url=/press/spe_test/index_2001.html");
	aI("text=&nbsp;&nbsp;2000;url=/press/spe_test/index_2000.html");
	aI("text=&nbsp;&nbsp;1999;url=/press/spe_test/index_1999.html");
	aI("text=&nbsp;&nbsp;1998;url=/press/spe_test/index_1998.html");
	aI("text=&nbsp;&nbsp;1997;url=/press/spe_test/index_1997.html");
	}
	
drawMenus();