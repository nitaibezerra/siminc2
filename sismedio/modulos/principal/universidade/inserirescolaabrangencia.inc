<?
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>

<script>

function carregarFiltrosEscolas(estuf) {
	ajaxatualizar('requisicao=carregarFiltrosEscolas&ecuid=<?=$_REQUEST['ecuid'] ?>&estuf='+estuf,'');
}

function cadastrarEscolaAbrangencia() {
	
	selectAllOptions( document.getElementById( 'lemcodigoinep_abrangencia' ) );
	
	document.getElementById('formulario').submit();
}

function habilitarUF(rede) {
	if(rede=='') {
		document.getElementById('estuf_abrangencia').value = '';
		carregarFiltrosMunicipios(document.getElementById('estuf_abrangencia').value);
		document.getElementById('estuf_abrangencia').disabled = true;
	} else {
		document.getElementById('estuf_abrangencia').disabled = false;
	}
}

</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="cadastrarEscolaAbrangencia">
<input type="hidden" name="ecuid" value="<?=$_REQUEST['ecuid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir municípios</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sismedio/sismedio.php?modulo=principal/universidade/inserirescolaabrangencia&acao=A"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_abrangencia', $sql, 'S', 'Selecione', 'carregarFiltrosEscolas', '', '', '', 'S', 'estuf_abrangencia', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Escolas</td>
	<td>
	<?
	$sql = "SELECT lemcodigoinep as codigo, lemcodigoinep as descricao FROM sismedio.abrangencia WHERE 1=2";
	combo_popup( "lemcodigoinep_abrangencia", $sql, "Escolas", "600x800", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="cadastrarabrangencia" value="Cadastrar Escolas" onclick="cadastrarEscolaAbrangencia();"></td>
</tr>
</table>
</form>
