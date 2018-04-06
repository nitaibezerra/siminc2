<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

include_once APPRAIZ . 'includes/classes/conteudoFlutuante.class.inc';

$db = new cls_banco();
$areaFlutuante = new ConteudoFlutuante();

if($_REQUEST['salvaParametros']){
	echo $areaFlutuante->salvar();
}

?>