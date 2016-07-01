<?php 
/*
 * Sistema Simec
* Setor responsvel: MEC
* Desenvolvedor: Equipe Simec
* Programador Raphaell Resende (raphaelresende@mec.gov.br)
* Mуdulo: inicio.inc
* Finalidae: Definiзгo das regras da pбgina inicial
* Data: 30/03/2012
*/

if(checkPerfil(array(PERFIL_EQUIPE_MUNICIPAL, PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PERFIL_EQUIPE_ESTADUAL, PERFIL_EQUIPE_ESTADUAL_APROVACAO))){
	//header("Location: snf.php?modulo=principal/listaEscolas&acao=A");
	$arrEstMun = recuperaEstadoMunicipioPerfil();
	if($arrEstMun['estuf'] || $arrEstMun['muncod']){
		$regra = regrasGrupoWorkflow();
	}
	include APPRAIZ."snf/modulos/principal/apresentacao.inc";
} elseif (checkPerfil(PERFIL_DIRIGENTE)){
	$termo = verificaAdesaoIES();
	if($termo != ''){
		include APPRAIZ."snf/modulos/principal/aderirTermoAdesao.inc";
		
	}else{
		include APPRAIZ."snf/modulos/principal/termoAdesao.inc";	
	}
} elseif (checkperfil(PERFIL_ADMINISTRADOR)){
	include APPRAIZ."snf/modulos/principal/listaAdesoes.inc";
} else{
	include APPRAIZ."snf/modulos/principal/termoAdesao.inc";
}

?>