<?php
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
/*
 * Validando a variavel de sessão do pregão
 */
if(!$_SESSION['rehuf_var']['preid']) {
	echo "<script>
			alert('Não existe pregão selecionado');
			window.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
	exit;
}


$titulo_modulo = "Preenchimento do Hospital";
monta_titulo( $titulo_modulo, '&nbsp;' );
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid'],$_SESSION['rehuf_var']['preid']);

montaSubTitulo('Preenchimento de Itens');
?>
<html>
<head>
<meta http-equiv="Pragma" content="no-cache">
<title>Imprimir Extrato</title>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 100px;}
</style>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<?php
	global $db;
	
	$sql = "SELECT
				itecatmat,
                                il.itlabrev,
                                i.iteitem,
				'<b>' || itedescricao ||
				' </b>' as dados,
                                iteapresentacao,
				ippquantidadeanual,
				replace(trim(to_char(trunc(ippprecounitario), '999,999,999,999,999,999,999')),',','.')||','||substr(trim(to_char(ippprecounitario, '999999999.0000')),position('.' in trim(to_char(ippprecounitario, '999999999.0000')))+1),
				to_Char(to_number(ippcnpjfornecedor,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as ippcnpjfornecedor,
				ippnomefornecedor,
				ippdescricaomarca,
				to_char(ippdatavencimentocontrato, 'DD/MM/YYYY') as ippdatavencimentocontrato,
				gi.grunome
			FROM rehuf.huparticipante hp
			INNER JOIN rehuf.itemgrupopregao igp ON igp.preid = hp.preid AND igp.igpstatus = '" . ATIVO . "'
			INNER JOIN rehuf.itenspregaopreenchido ipp ON ipp.igpid = igp.igpid AND ipp.hupid = hp.hupid AND (ipp.ippstatus = '" . ATIVO . "' OR ipp.ippstatus = '".PREENCHIMENTO_PREGAO."')												 
			INNER JOIN rehuf.itemgrupo ig ON ig.itgid = igp.itgid AND ig.itgstatus = '" . ATIVO . "'
			INNER JOIN rehuf.item i ON i.iteid = ig.iteid AND i.itestatus = '" . ATIVO . "'
                        LEFT JOIN rehuf.itemlote il ON il.itlid = i.itlid 
			INNER JOIN rehuf.grupoitens gi ON gi.gruid = ig.gruid AND gi.grustatus = '" . ATIVO . "'
			WHERE hp.preid = {$_SESSION['rehuf_var']['preid']} AND 
				  hp.entid = {$_SESSION['rehuf_var']['entid']} AND 
				  hp.hupstatus = '" . ATIVO . "' ORDER BY il.itlabrev, i.iteitem, i.itecatmat, i.itlid";
				  //ver($sql, d);
	$dados = $db->carregar($sql);
	?>
	<table class="tabela" cellspacing="1" cellpadding="2" border="0" align="center" width="95%" style="background: #F5F5F5">
		<tr>
			<td class="title" style="background: #DCDCDC;" width="44%"><center><b>Informações para este pregão</b></center></td>
			<td class="title" width="56%"><center><b>Informações da última compra</b></center></td>
		</tr>
	</table><?

	$cabecalho = array("CAT/MAT", "Lote", "Item", "Descrição", "Apresentação", "Quantidade", "Preço Unitário", "CNPJ do Fornecedor", "Nome do Fornecedor", "Fabricante/Marca", "Data Vigência");	
	$db->monta_lista_grupo($sql, $cabecalho, 500, 5, 'N','Center','','formListaItensPreenchidos', 'grunome');
	?>
	<table cellspacing="1" cellpadding="2" border="0" align="center" width="95%">
		<tr>
			<td>&nbsp;</td>
		</tr>
		<tr class="div_rolagem">
			<td align="center"><input type="button" name="imprimir" value="Imprimir Extrato" onclick="imprimirExtrato();">&nbsp;
							   <input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();"></td>
		</tr>
	</table>
<script>
// carregando a lista de preenchimento de itens
function imprimirExtrato(){
	window.print();
}
</script>
</body>
