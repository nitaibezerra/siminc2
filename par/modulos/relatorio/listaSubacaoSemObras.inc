<?php

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

global $db;

//echo montarAbasArray(criaAbaPar(), "par.php?modulo=principal/listaEstados&acao=A");
$titulo_modulo = "Subações sem Obras Vinculadas";
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
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=relatorio/listaSubacaoSemObras&acao=A';" />
		</td>
	</tr>
</table>
</form>
<?php

$cabecalho = array('Entidade', 'UF', 'Subação', 'Descrição', 'Ano');

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

$sql = "SELECT
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao || ' - ' || mun.estuf END as entidade,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
			par.retornacodigosubacao(s.sbaid) as subacao,
			s.sbadsc as descricao,
			sd.sbdano
		FROM
			par.subacao s
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
		INNER JOIN par.acao a ON a.aciid = s.aciid
		INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		LEFT JOIN  territorios.municipio mun ON mun.muncod = iu.muncod
		WHERE
			s.frmid = ".ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA." ".$where." AND
			s.sbastatus = 'A' AND
			(s.sbaid, sd.sbdano) NOT IN ( SELECT sbaid, sovano FROM par.subacaoobravinculacao )
		ORDER BY
			itrid, mun.estuf, mun.mundescricao, subacao, sd.sbdano";

$db->monta_lista($sql, $cabecalho, 100, 70, 'N', '95%', 'N', '');

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function submeteFormulario(){
		jQuery('#requisicao').val('pesquisar');
		jQuery('#formulario').submit();
	}
</script>