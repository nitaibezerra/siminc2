<?php
	include "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	$sql = sprintf(
		"select
			usucpf_delegante as usucpf from seguranca.delegacao_competencia
			where sisid=%d and usucpf_delegado='%s' and dcpstatus='A' and dcpdata_ini <='%s' and dcpdata_fim >='%s'",
		$_SESSION['sisid'],
		$_SESSION['usucpf'],
		date('Y-m-d'),
		date('Y-m-d')
	);
	$cpfs = array();
	$lista = @$db->carregar( $sql );
	if ( !empty( $lista ) ) {
		foreach ( $lista as $registro ) {
			array_push( $cpfs, $registro['usucpf'] );
		}
	}
	array_push( $cpfs, $_SESSION['usucpf'] );
	$cpfs = "'". implode( "','", $cpfs ) ."'";
?>
fixMozillaZIndex=true; //Fixes Z-Index problem  with Mozilla browsers but causes odd scrolling problem, toggle to see if it helps
_menuCloseDelay=500;
_menuOpenDelay=150;
_subOffsetTop=2;
_subOffsetLeft=-2;

with(menuStyle=new mm_style()){
bordercolor="#999999";
borderstyle="solid";
borderwidth=1;
fontfamily="Verdana, Tahoma, Arial";
fontsize="75%";
fontstyle="normal";
headerbgcolor="#ffffff";
headercolor="#000000";
offbgcolor="#eeeeee";
offcolor="#000000";
onbgcolor="#ddffdd";
oncolor="#000099";
outfilter="randomdissolve(duration=0.3)";
overfilter="Fade(duration=0.2);Alpha(opacity=90);Shadow(color=#777777', Direction=135, Strength=3)";
padding=4;
pagebgcolor="#82B6D7";
pagecolor="black";
separatorcolor="#999999";
separatorsize=1;
subimage="/imagens/arrow.gif";
subimagepadding=2;
}

with(milonic=new menuname("Main Menu")){
	alwaysvisible=1;
	followscroll=1;
	left=10;
	orientation="horizontal";
	style=menuStyle;
	top=60;
	<?php
		$sql = sprintf(
			"select
				distinct mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod
				from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
				where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod and pfu.usucpf in (%s) and mnu.mnutipo=1 and mnu.mnushow='t' and mnu.mnustatus='A' and mnu.sisid=%d
				order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod",
			$cpfs,
			$_SESSION['sisid']
		);
		$lista = @$db->carregar( $sql );
		if ( !empty( $lista ) ) {
			foreach( $lista as $registro )
			{
				if ( $registro['mnusnsubmenu'] == 'f' ) {
					printf( "aI(\"text=%s;url=%s;\");", $registro['mnudsc'], $registro['mnulink'] );
				} else if ( $registro['mnulink'] == '' ) {
					printf( "aI(\"showmenu=%s;text=%s;\");", $registro['mnudsc'], $registro['mnudsc'] );
				}
			}
		}
	?>
	aI("text=<img src=../imagens/sair.gif width=12 height=12 align=top border=0>&nbsp;Sair;url=http://www.google.com.br;");
}

<?php

	// menus verticais
	$sql = sprintf(
		"select
			distinct mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod, mnu2.mnudsc as mnudscpai
			from seguranca.menu mnu, seguranca.menu mnu2, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu
			where mnu2.mnuid=mnu.mnuidpai and mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod and pfu.usucpf in (%s) and mnu.mnuidpai is not null and mnu.mnushow= 't' and mnu.mnustatus='A' and mnu.sisid=%d
			order by mnu.mnuidpai, mnu.mnuid,mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod, mnu2.mnudsc ",
		$cpfs,
		$_SESSION['sisid']
	);
	$lista = @$db->carregar( $sql );
	
	if ( !empty( $lista ) ) {
		// agrupar por menu
		$arvore = array();
		for ( $i = 0; $i < count( $lista ); $i++ ) {
			$arvore[$lista[$i]['mnudscpai']][] = $lista[$i];
		}
		// montar os menus
		foreach ( $arvore as $menu ) {
			printf( "with(milonic=new menuname(\"%s\")){ style=menuStyle;", $menu[0]['mnudscpai'] );
			for ( $i = 0; $i < count( $menu ); $i++ ) {
				if ( $menu[$i]['mnusnsubmenu'] == 'f' ) {
					printf( "\naI(\"text=%s;url=%s;\");", $menu[$i]['mnudsc'], $menu[$i]['mnulink'] );
				} else if ( $menu[$i]['mnulink'] == '' ) {
					printf( "\naI(\"showmenu=%s;text=%s;\");", $menu[$i]['mnudsc'], $menu[$i]['mnudsc'] );
				}
			}
			printf( "}" );
		}
	}

?>

drawMenus();
