<?
include_once "_funcoes.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>
<script>

function inserirCusto() {

	if(jQuery('#gdeid').val()=='') {
		alert('Grupo de despesa em branco');
		return false;
	}
	
	jQuery('#orcvlrunitario').val(mascaraglobal('###.###.###,##',jQuery('#orcvlrunitario').val()));
	
	if(jQuery('#orcvlrunitario').val()=='') {
		alert('Valor Unitário (R$) em branco');
		return false;
	}

	var vlrloa2014=0;
	if(jQuery('#orcvlrloa2014').val()!='') {orcvlrloa2014=parseFloat(replaceAll(replaceAll(jQuery('#orcvlrloa2014').val(),'.',''),',','.'));}
	var vlrloa2015=0;
	if(jQuery('#orcvlrloa2015').val()!='') {orcvlrloa2015=parseFloat(replaceAll(replaceAll(jQuery('#orcvlrloa2015').val(),'.',''),',','.'));}
	var vlrloa2016=0;
	if(jQuery('#orcvlrloa2016').val()!='') {orcvlrloa2016=parseFloat(replaceAll(replaceAll(jQuery('#orcvlrloa2016').val(),'.',''),',','.'));}
	var orcvlrunitario=0;
	if(jQuery('#orcvlrunitario').val()!='') {orcvlrunitario=parseFloat(replaceAll(replaceAll(jQuery('#orcvlrunitario').val(),'.',''),',','.'));}

	var total = orcvlrloa2014+orcvlrloa2015+orcvlrloa2016;

	if(total != orcvlrunitario) {
		alert('Somatório da LOA 2014 + 2015 + 2016 deve ser igual ao Valor Total (R$)');
		return false;
	}
	
	
	if(jQuery('#orcdescricao').val()=='') {
		alert('Detalhamento em branco');
		return false;
	}

	document.getElementById('formulario').submit();
}


</script>
<?
if($_REQUEST['orcid']) {
	$orc = carregarOrcamento(array("orcid"=>$_REQUEST['orcid']));
	$requisicao = "atualizarCusto";
	extract($orc);
} else {
	$requisicao = "inserirCusto";
}

?>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
<input type="hidden" name="orcid" value="<?=$orcid ?>">
<input type="hidden" name="sifid" value="<?=$_SESSION['sisfor']['sifid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Custos</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Grupo de despesa</td>
	<td><?
	$sql = "SELECT 
                gdeid as codigo,
                gdedesc as descricao
			FROM  
                sisfor.grupodespesa 
            WHERE 
            	gdestatus='A'";

	$db->monta_combo('gdeid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'gdeid', '', $gdeid); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Unidade de Medida</td>
	<td>Verba</td>
</tr>
<tr>
	<td class="SubTituloDireita">Valor da LOA 2014 (R$)</td>
	<td><?=campo_texto('orcvlrloa2014', "S", "S", "Valor da LOA 2014 (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="orcvlrloa2014"', '', mascaraglobal($orcvlrloa2014,"###.###.###,##") ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Valor da LOA 2015 (R$)</td>
	<td><?=campo_texto('orcvlrloa2015', "S", "S", "Valor da LOA 2015 (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="orcvlrloa2015"', '', mascaraglobal($orcvlrloa2015,"###.###.###,##") ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Valor da LOA 2016 (R$)</td>
	<td><?=campo_texto('orcvlrloa2016', "S", "S", "Valor da LOA 2016 (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="orcvlrloa2016"', '', mascaraglobal($orcvlrloa2016,"###.###.###,##") ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Valor Total (R$)</td>
	<td><?=campo_texto('orcvlrunitario', "S", "S", "Valor Unitário (R$)", 15, 14, "###.###.###,##", "", '', '', 0, 'id="orcvlrunitario"', '', mascaraglobal($orcvlrunitario,"###.###.###,##") ); ?></td>
</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Memória de cálculo</td>
		<td>
		<? echo campo_textarea( 'orcdescricao', 'S', 'S', '', '70', '4', '500'); ?>
		</td>
	</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" onclick="inserirCusto();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
</table>
</form>
