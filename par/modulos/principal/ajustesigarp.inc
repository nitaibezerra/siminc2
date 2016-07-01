<?php

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

global $db;

//echo montarAbasArray(criaAbaPar(), "par.php?modulo=principal/listaEstados&acao=A");
$titulo_modulo = "Ajustes SIGARP x SIMEC";
monta_titulo($titulo_modulo, ''); 

?>
<form method="post" name="formulario" id="formulario">
<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php 
			$municipio = $_POST['municipio'];
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td colspan="3">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input class="botao" type="button" name="pesquisar" id= "pesquisar" value="Pesquisar" onclick="submeteFormulario();">
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/ajustesigarp&acao=A';" />
		</td>
	</tr>
</table>
</form>
<?php
//$cabecalho = array('ITRID', 'INUID', 'Local', 'Data', 'Adesão', 'Item', 'Quantidade', 'Valor', 'Subação', 'Ano', 'Processo');
$cabecalho = array('Ação', 'ITRID', 'INUID', 'Local', 'Data', 'Adesão', 'Item', 'Quantidade', 'Valor');

$where = "";
if($_REQUEST['requisicao'] == 'pesquisar'){
	if( $municipio ){
		$municipioTemp = removeAcentos($_POST['municipio']);
		$where .= " AND public.removeacento(mun.mundescricao) ilike '%".$municipioTemp."%'";
	}
	if( $estuf ){
		$where .= " AND (iu.estuf = '".$estuf."' OR iu.mun_estuf = '".$estuf."')";
	}
}

$sql = "SELECT DISTINCT
			'<img src=\"/imagens/alterar.gif\" title=\"Selecionar\" onclick=\"javascript: alteradados(' || id || ')\">' as acao,
			iu.itrid,
			iu.inuid,
			CASE WHEN iu.muncod IS NOT NULL THEN mun.mundescricao || ' - ' || mun.estuf ELSE iu.estuf END as local,
			data,
			adesao,
			item,
			quantidade,
			REPLACE(valor, ',', '.')::numeric(20,2) as valor --,
			--array_to_string(array(SELECT ppc.sbdid FROM par.processoparcomposicao ppc WHERE ppc.prpid = prp.prpid), ',')|| '&nbsp;' as subacao,
			--'' as ano,
			--ane.processo
		FROM
			carga.adesoesnaoencontradas6 ane
		INNER JOIN par.instrumentounidade iu ON iu.inucnpj = ane.cnpj
		LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
		INNER JOIN par.processopar prp ON prp.inuid = iu.inuid AND prpstatus = 'A'
	--	INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
		WHERE
			obra = '' AND
			epiid IS NULL
			{$where}
		ORDER BY
			itrid, local";

$db->monta_lista($sql, $cabecalho, 100, 70, 'N', '95%', 'N', '');

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function alteradados(id){
		url = 'par.php?modulo=principal/popupAjusteSigarp&acao=A&id='+id;
		window.open(url, 'popupSigarp', "height=600,width=800,scrollbars=yes,top=50,left=200" );
	}
	
	function submeteFormulario(){
		jQuery('#requisicao').val('pesquisar');
		jQuery('#formulario').submit();
	}
</script>