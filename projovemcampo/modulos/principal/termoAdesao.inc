<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['projovemcampo']['apcid']){
    die("
        <script>
            alert('Problemas de navegação. Inicie novamente.');
            window.location='projovemcampo.php?modulo=inicio&acao=C';
        </script>"
    );
}

$adesaotermo = $db->pegaUm("SELECT apctermoaceito FROM projovemcampo.adesaoprojovemcampo WHERE apcid=".$_SESSION['projovemcampo']['apcid']);

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProjovemCampo(), $_SERVER['REQUEST_URI']);

monta_titulo('Projovem Campo', montaTituloEstMun());

$us = pegarUsuarioProJovemCampo();

if($_SESSION['projovemcampo']['estuf']) {
	
	termoProjovemCampoEstado($us);
	
} elseif($_SESSION['projovemcampo']['muncod']) {
	
	termoProjovemCampoMunicipio($us);
}
$perfis = pegaPerfilGeral();
if(!in_array(PFL_EQUIPE_MEC,    $perfis)&& !in_array(PFL_ADMINISTRADOR, $perfis)&& !$db->testa_superuser()){
	$bloqueioHorario = bloqueioadesao();
}

$cond = ( !$adesaotermo || $adesaotermo == 'f' );

$condPerfil = ( (in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_EQUIPE_MEC,    $perfis) || 
		    	in_array(PFL_SECRETARIO_ESTADUAL , $perfis) || in_array(PFL_ADMINISTRADOR, $perfis) || 
		    	$db->testa_superuser())&&!$bloqueioHorario );

if( $cond ) :
?>
<style>
.tabela p{font-size: 11pt !important;  }
</style>
<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloCentro">
			<?php if( $condPerfil ){?>
			<input type="button" name="aceitar" value="Aceito" onclick="window.location='projovemcampo.php?modulo=principal/termoAdesao&acao=A&requisicao=aceitarTermo';"> 
			<input type="button" name="naoaceitar" value="Não aceito" onclick="window.location='projovemcampo.php?modulo=principal/termoAdesao&acao=A&requisicao=naoAceitarTermo';">
			<?php }?>
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