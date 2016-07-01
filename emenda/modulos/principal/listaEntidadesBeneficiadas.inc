<?php
set_time_limit(0);

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
include  APPRAIZ."emenda/classes/Habilita.class.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

$obEntBenef = new EntidadeBeneficiada();

if ($_REQUEST['pesquisaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obEntBenef->listaEntidadeBeneficiada($_POST);
	exit;
}

if ($_REQUEST['excluirAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obEntBenef->excluirEntidadeBeneficiadaAjax($_POST['enbid']);
	exit;
}
if ($_REQUEST['consultarSituacaoEntidade']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obEntBenef->consultarSituacaoEntidade($_POST['enbcnpj'], $_POST['enbid']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

//echo montarAbasArray(carregaAbasEmendas('listaPrograma'), "/emenda/emenda.php?modulo=principal/listaPrograma&acao=A");
monta_titulo( 'Listar Entidade(s) Beneficiada(s)', "Filtro de Pesquisa" );
?>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/entidadeBeneficiada.js"></script>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">CNPJ:</td>
		<td><?=campo_texto( 'enbcnpj', 'N', 'S', '', 40, 20, '', '','','','','id="enbcnpj"', "this.value=mascaraglobal('##.###.###/####-##',this.value);"); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da Entidade:</td>
		<td><?=campo_texto( 'enbnome', 'N', 'S', '', 40, 100, '', '','','','','id="enbnome"'); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisar();"></th>
	</tr>
</table>
</form>
<div id="lista"><? $obEntBenef->listaEntidadeBeneficiada() ?></div>
<div id="erro"></div>	
</body>
</html>