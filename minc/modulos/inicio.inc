<?php

  /*
   * Sistema Simec
   * Setor responsvel: MEC
   * Desenvolvedor: Equipe Consultores Simec
   * Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   * Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   * Mdulo:inicio.inc
   * Finalidade: permitir abrir a página inicial
   */

unset($_SESSION['minc']);

$sql = "select 
			muncod,
			estuf,
			entid 
		from 
			minc.usuarioresponsabilidade 
		where 
			usucpf = '{$_SESSION['usucpf']}'
		and
			rpustatus = 'A'";

$rs = $db->carregar($sql);
$rs = $rs ? $rs : array();

foreach($rs as $dados){
	
	if($dados['estuf'])
		$arEstuf[] = $dados['estuf'];

	if($dados['muncod'])
		$arMuncod[] = $dados['muncod'];
		
	if($dados['entid']){
		$arEntid[] = $dados['entid'];
		$mceid = $db->pegaUm("select max(mceid) from minc.mcemaiscultura where entid = {$dados['entid']} and mcestatus= 'A'");
		if($mceid){
			$arMceid[] = $mceid;
		}
		unset($mceid);
	}
	
}

if($arMceid)
	$_SESSION['minc']['mceid'] = count($arMceid) == 1 ? $arMceid[0] : $arMceid; 

if($arMuncod)
	$_SESSION['minc']['muncod'] = count($arMuncod) == 1 ? $arMuncod[0] : $arMuncod;

if($arEstuf)
	$_SESSION['minc']['estuf'] = count($arEstuf) == 1 ? $arEstuf[0] : $arEstuf;

if($arEntid)	
	$_SESSION['minc']['entid'] = count($arEntid) == 1 ? $arEntid[0] : $arEntid;
	

if(checkPerfil( array(PERFIL_MINC_ADMINISTRADOR, PERFIL_MINC_SEC_ESTADUAL, PERFIL_MINC_SEC_MUNICIPAL, PERFIL_MINC_MEC) )){
	header("Location: minc.php?modulo=principal/lista&acao=A");
}else
if( checkPerfil( array(PERFIL_MINC_CADASTRADOR) ) ){
	header("Location: minc.php?modulo=principal/informacoes&acao=A");	
}

?>

