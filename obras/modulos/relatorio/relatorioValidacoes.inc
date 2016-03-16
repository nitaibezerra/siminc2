<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Monitoramento de Obras', 'Relatório Validações' );
?>
<form action="obras.php?modulo=relatorio/popupRelatorioValidacoes&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<!-- <tr>
	<td class="SubTituloDireita">Tipo de Ensino</td>
	<td>
		<?php						
			$orgaos = $db->carregar("SELECT orgid, orgdesc FROM obras.orgao where orgid in (1, 2, 3)");					
			$count = count($orgaos);
			for($i = 0; $i < $count; $i++){						
				echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';						
			}
		?>
	</td>
</tr>-->
	<?
	// Programa
	$stSql = "SELECT
				prfid AS codigo,
				prfdesc AS descricao
			 FROM 
				obras.programafonte
			 ORDER BY
				prfdesc ";
	mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)' );
	
	// Fonte
	$stSql = "SELECT
				tooid AS codigo,
				toodescricao AS descricao
			 FROM 
				obras.tipoorigemobra
			WHERE
				toostatus = 'A'
			 ORDER BY
				toodescricao ";
	
	$sql_carregados = "	SELECT
							tooid AS codigo,
							toodescricao AS descricao
						 FROM 
							obras.tipoorigemobra
						WHERE
							toostatus = 'A'
						AND
							tooid = 1
						 ORDER BY
							toodescricao ";
	
	mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
?>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
	<td>
		<?php 			
			$sql = "SELECT 
						ee.entid as codigo, 
						upper(ee.entnome) as descricao 
					FROM
						entidade.entidade ee
					INNER JOIN 
						obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
					WHERE
						orgid = 3 AND
						obsstatus = 'A'
					GROUP BY 
						ee.entnome, 
						ee.entid 
					ORDER BY 
						ee.entnome";
		
			$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "550", "N", "entidunidade" );
			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
	<td>
		<?php 
			$sql = "SELECT 
						stoid as codigo, 
						stodesc as descricao 
					FROM 
						obras.situacaoobra 
					WHERE
						stostatus = 'A'
					ORDER BY 
						stodesc";
		
			$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">UF:</td>
	<td>
		<?php 
			$sql = "SELECT
						estuf as codigo,
						estdescricao as descricao
					FROM
						territorios.estado
					ORDER BY
						estdescricao";
		
			$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">ID Obras</td>
	<td>
		<?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">ID Pré-Obra:</td>
	<td>
		<?php echo campo_texto( 'preid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Homologação Validada:</td>
	<td>
		<input type="radio" value="S" id="homologacao" name="homologacao" <? if($_REQUEST["homologacao"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="homologacao" name="homologacao" <? if($_REQUEST["homologacao"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="homologacao" name="homologacao"  <? if($_REQUEST["homologacao"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="homologacao" name="homologacao"  <? if($_REQUEST["homologacao"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Execução 25% Validada:</td>
	<td>
		<input type="radio" value="S" id="execucao25" name="execucao25" <? if($_REQUEST["execucao25"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="execucao25" name="execucao25" <? if($_REQUEST["execucao25"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="execucao25" name="execucao25"  <? if($_REQUEST["execucao25"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="execucao25" name="execucao25"  <? if($_REQUEST["execucao25"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Execução 50% Validada:</td>
	<td>
		<input type="radio" value="S" id="execucao50" name="execucao50" <? if($_REQUEST["execucao50"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="execucao50" name="execucao50" <? if($_REQUEST["execucao50"] == "N") { echo "checked"; } ?> /> Não Validados
		<input type="radio" value="P" id="execucao50" name="execucao50"  <? if($_REQUEST["execucao50"] == "P") { echo "checked"; } ?> /> Não Analisados
		<input type="radio" value="" id="execucao50" name="execucao50"  <? if($_REQUEST["execucao50"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Homologação com arquivo(s) anexado(s):</td>
	<td>
		<input type="radio" value="S" id="arqanexosim" name="arqanexo" <? if($_REQUEST["arqanexo"] == "S") { echo "checked"; } ?> /> Sim
		<input type="radio" value="N" id="arqanexonao" name="arqanexo" <? if($_REQUEST["arqanexo"] == "N") { echo "checked"; } ?> /> Não Validados		
		<input type="radio" value="" id="arqanexotodos" name="arqanexo"  <? if($_REQUEST["arqanexo"] == "") { echo "checked"; } ?> /> Todos
	</td>
</tr>
<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
		<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
	</td>
</tr>
</table>
<script type="text/javascript">
var formulario = document.getElementById('filtro');

function obras_exibeRelatorioGeral(tipo){
			 
	/*if ( !document.getElementsByName('orgid[]').item(0).checked &&
		 !document.getElementsByName('orgid[]').item(1).checked &&
		 !document.getElementsByName('orgid[]').item(2).checked ){
		alert( 'Favor selecionar ao menos um tipo de ensino!' );
		return false;
	}*/
	
	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'prfid' ) );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'tooid' ) );
	}
	
	formulario.target = 'RelatorioValidacoes';
	var janela = window.open( 'obras.php?modulo=relatorio/popupRelatorioValidacoes&acao=A', 'RelatorioValidacoes', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
	janela.focus();
	
	// Tipo de relatorio
	document.getElementById('pesquisa').value= tipo ;
	formulario.submit();		
}

/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
</form>