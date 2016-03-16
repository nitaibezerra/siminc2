<?php

include_once "_funcoes.php";
include_once "_funcoes_universidade.php";
include_once "_funcoes_coordenadoradjuntoies.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if ( isset( $_REQUEST['buscar'] ) ) {

	?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisindigena.js"></script>

<script>

jQuery(document).ready(function() {
	jQuery("[id^='img2_']").each(function() {
		jQuery(this).click();
	});

	jQuery("[id^='img_']").each(function() {
		jQuery(this).click();
	});
});

</script>
<body>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro">Visualização do Plano de Trabalho</td>
	</tr>
	<tr>
		<td>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2">Orçamento</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
				<td>
				<? carregarListaCustos(array("consulta"=>true,"uncid"=>$_REQUEST['uncid'])); ?>
				</td>
			</tr>
		</table>
		
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2">Núcleos</td>
			</tr>
			<tr>
				<td colspan="2">
				<? carregarNucleosUniversidades(array('uncid'=>$_REQUEST['uncid'],'consulta' => true)); ?>
				</td>
			</tr>
		</table>
		
		
		
		</td>
	</tr>
</table>
</body>	
	<?
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório dos Planos de Trabalho das Universidades", "" );


?>
<script type="text/javascript">
function exibirRelatorio() {
	if(document.getElementById('uncid').value!='') {
		var formulario = document.formulario;
		// submete formulario
		formulario.target = 'planotrabalho';
		var janela = window.open( '', 'planotrabalho', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		formulario.submit();
		janela.focus();
	} else {
		alert("Selecione uma universidade");
		return false;
	}
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td>Filtros</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top">Selecione a Universidade</td>
			<td>
			<?

			$perfis = pegaPerfilGeral();
				
			
			if(in_array(PFL_COORDENADORIES,$perfis) || in_array(PFL_SUPERVISORIES,$perfis) || in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
			
				$wh = "WHERE u.uncid IN(SELECT uncid FROM sisindigena.identificacaousuario WHERE iuscpf='".$_SESSION['usucpf']."')";
			
			}
				
			$sql = "SELECT u.uncid as codigo, uu.unisigla||' - '||uu.uninome as descricao FROM sisindigena.universidadecadastro u
					INNER JOIN sisindigena.universidade uu ON uu.uniid = u.uniid 
					{$wh}";
				
			
			$db->monta_combo('uncid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'uncid');
			?>
			</td>
		</tr>

	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
		</tr>
</table>