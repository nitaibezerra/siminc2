<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', montaTituloEstMun());

$us = pegarUsuarioProJovem();

$us['ajustado'] = true;

if($_SESSION['projovemurbano']['estuf']) {
	termoEstado($us);
} elseif($_SESSION['projovemurbano']['muncod']) {
// 	termoEstado($us);
	termoMunicipio($us);
}

$perfis = pegaPerfilGeral();

if(!in_array(PFL_EQUIPE_MEC,    $perfis)&& !in_array(PFL_ADMINISTRADOR, $perfis)&& !$db->testa_superuser()){
	$bloqueioHorario = bloqueioadesao2014();
}
$adesaotermoajustada = $db->pegaUm("SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid=".$_SESSION['projovemurbano']['pjuid']);

$condPerfil = ( in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_EQUIPE_MEC,    $perfis) ||
		in_array(PFL_SECRETARIO_ESTADUAL , $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) ||
		$db->testa_superuser() );
if(($adesaotermoajustada == 'f' || !$adesaotermoajustada) && $condPerfil /*&& !$bloqueioHorario*/) :
?>
<style>
.tabela p{font-size: 11pt !important;  }
</style>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
    <td class="SubTituloCentro">  
        <input type="button" name="aceitar" value="Aceito" onclick="window.location='projovemurbano.php?modulo=principal/termoAdesao&acao=A&requisicao=aceitarTermoAjustado';"> 
        <input type="button" name="naoaceitar" value="Não aceito" onclick="window.location='projovemurbano.php?modulo=principal/termoAdesao&acao=A&requisicao=naoAceitarTermoAjustado';">
    </td>
</tr>
</table>
<? 
else :
?>
<style>
.tabela p{font-size: 11pt !important;  }
</style>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro"><input type="button" name="imprimir" value="Imprimir" onclick="window.print();"></td>
</tr>
</table>
<?
endif;

registarUltimoAcesso(); 
?>