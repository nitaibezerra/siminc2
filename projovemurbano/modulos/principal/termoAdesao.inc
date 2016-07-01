<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['projovemurbano']['pjuid']){
    die("
        <script>
            alert('Problemas de navegação. Inicie novamente.');
            window.location='projovemurbano.php?modulo=inicio&acao=C';
        </script>"
    );
}

$adesaotermo = $db->pegaUm("SELECT adesaotermo FROM projovemurbano.projovemurbano WHERE pjuid=".$_SESSION['projovemurbano']['pjuid']);
//ver($db,0);

// if(date("Y-m-d")!="2011-12-01" && !$adesaotermo) {
// 	die("<script>alert('O prazo para adesão ao Projovem Urbano encerrou em 01/12/2011, às 23h59 (horário de Brasília).');window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';</script>");
// }

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Urbano', montaTituloEstMun());

$us = pegarUsuarioProJovem();

if($_SESSION['projovemurbano']['estuf']) {
	termoEstado($us);
} elseif($_SESSION['projovemurbano']['muncod']) {
	termoMunicipio($us);
// 	termoEstado($us);
}
$perfis = pegaPerfilGeral();

if(!in_array(PFL_EQUIPE_MEC,    $perfis)&& !in_array(PFL_ADMINISTRADOR, $perfis)&& !$db->testa_superuser()){
	$bloqueioHorario = bloqueioadesao2014();
}

$cond = ( !$adesaotermo || $adesaotermo == 'f' );

$condPerfil = ( in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_EQUIPE_MEC,    $perfis) || 
		    	in_array(PFL_SECRETARIO_ESTADUAL , $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) || 
		    	$db->testa_superuser() );

if( $cond ) :
?>
<style>
.tabela p{font-size: 11pt !important; }
</style>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro">
			<?php if( $condPerfil && !$bloqueioHorario ){?>
			<input type="button" name="aceitar" value="Aceito" onclick="window.location='projovemurbano.php?modulo=principal/termoAdesao&acao=A&requisicao=aceitarTermo';"> 
			<input type="button" name="naoaceitar" value="Não aceito" onclick="window.location='projovemurbano.php?modulo=principal/termoAdesao&acao=A&requisicao=naoAceitarTermo';">
			<?php }?>
		</td>
	</tr>
</table>
<? 
else :
?>
<style>
.tabela p{font-size: 11pt !important; }
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