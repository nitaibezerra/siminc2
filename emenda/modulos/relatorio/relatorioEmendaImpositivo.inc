<?php
require_once APPRAIZ . 'includes/cabecalho.inc';

monta_titulo( "Relatórios Emenda Impositivo", '<b>Filtros de Pesquisa</b>');
?>
<form action="emenda.php?modulo=relatorio/resultRelatorioEmendaImpositivo&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" width="20%">Unidade Orçamentária:</td>
		<td width="30%">
			<?php 
				
				$unicod = $_REQUEST["unicod"];
				$sql = array();
				$sql = "SELECT DISTINCT 
							u.unicod as codigo,
							u.unicod||' - '||u.unidsc as descricao
						FROM
							emenda.emenda e
							INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							INNER JOIN public.unidade u ON u.unicod = f.unicod
						ORDER BY descricao";
				
				$db->monta_combo("unicod",$sql,'S',"Todas",'','','','500','N','unicod', '', '', '', 'class="sel_chosen"');
			?>
		</td>
		<td class="subtitulodireita" width="20%">Origem Emenda:</td>
		<td width="30%">
			<?php 
				
				$etoid = $_REQUEST["etoid"];
				
				$sql = "select 
							o.etoid as codigo,
							o.etodescricao as descricao
						 from emenda.emendatipoorigem o
						 where o.etostatus = 'A'
						 order by o.etodescricao";
				
				$db->monta_combo("etoid",$sql,'S',"Todas",'','','','300','N','etoid');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Ação Orçamentária:</td>
		<td>
			<?php 
				
				$acacod = $_REQUEST["acacod"];
				
				$sql = array();
				$sql ="SELECT DISTINCT 
							f.acacod as codigo,
							f.acacod || ' - ' || f.acadsc as descricao
						FROM
							emenda.emenda e
							INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
						ORDER BY f.acacod";
				
				$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '500', 'N','acacod', '', '', '', 'class="sel_chosen"');
				
			?>
		</td>
		<td class="subtitulodireita">Ano:</td>
		<td>
			<?php 
			$emeano = $_SESSION['exercicio'];
			echo campo_texto('emeano', 'N', 'S', '', 27, 20, '[#]', '', '', '', 0, '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Autor do Recurso:</td>
		<td>
			<?php
				$sql ="SELECT DISTINCT 
						ea.autid as codigo,
						ea.autnome as descricao
					FROM
						emenda.autor ea
						INNER JOIN emenda.emenda ee ON ea.autid = ee.autid and ea.autstatus = 'A'
					ORDER BY
						ea.autnome";				
				$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '500', 'N','autid', '', '', '', 'class="sel_chosen"');				
			?>
		</td>
		<td class="subtitulodireita">Emenda Relator:</td>
		<td>
			<input type="radio" value="S" id="emenda_relator" name="emenda_relator" <? if($_REQUEST["emenda_relator"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="emenda_relator" name="emenda_relator" <? if($_REQUEST["emenda_relator"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="emenda_relator" name="emenda_relator"<? if($_REQUEST["emenda_relator"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Descentralização:</td>
		<td>
			<input type="radio" value="S" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="descentralizacao" name="descentralizacao"  <? if($_REQUEST["descentralizacao"] == "") { echo "checked"; } ?> /> Todos
		</td>
		<td class="subtitulodireita">Emenda Impositivo:</td>
		<td colspan="3">
			<input type="radio" value="S" id="emdimpositiva" name="emdimpositiva" <? if($_REQUEST["emdimpositiva"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="emdimpositiva" name="emdimpositiva" <? if($_REQUEST["emdimpositiva"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="emdimpositiva" name="emdimpositiva"  <? if($_REQUEST["emdimpositiva"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Impedimento:</td>
		<td>
			<input type="radio" value="NH" id="ediimpositivo" name="ediimpositivo" <? if($_REQUEST["ediimpositivo"] == "NH") { echo "checked"; } ?> /> Não há
			<input type="radio" value="PA" id="ediimpositivo" name="ediimpositivo" <? if($_REQUEST["ediimpositivo"] == "PA") { echo "checked"; } ?> /> Parcial
			<input type="radio" value="TO" id="ediimpositivo" name="ediimpositivo"  <? if($_REQUEST["ediimpositivo"] == "TO") { echo "checked"; } ?> /> Total
			<input type="radio" value="" id="ediimpositivo" name="ediimpositivo"  <? if($_REQUEST["ediimpositivo"] == "") { echo "checked"; } ?> /> Todos
		</td>
		<td class="subtitulodireita">Com Plano/Proposta de Trabalho:</td>
		<td colspan="3">
			<input type="radio" value="S" id="pta" name="pta" <? if($_REQUEST["pta"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="pta" name="pta" <? if($_REQUEST["pta"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="pta" name="pta"  <? if($_REQUEST["pta"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Com Análise de Mérito:</td>
		<td>
			<input type="radio" value="S" id="analise_merito" name="analise_merito" <? if($_REQUEST["analise_merito"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="analise_merito" name="analise_merito" <? if($_REQUEST["analise_merito"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="analise_merito" name="analise_merito"  <? if($_REQUEST["analise_merito"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="4" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio('');" style="cursor: pointer;"/>
			<input type="button" name="excel" value="Gerar Excel" onclick="geraPopRelatorio('excel');">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
	jq('.sel_chosen').chosen({allow_single_deselect:true});
	
	function geraPopRelatorio( tipo ){
		var form = document.getElementById('formulario');

		form.target = 'page';
		form.action = 'emenda.php?modulo=relatorio/resultRelatorioEmendaImpositivo&acao=A&tipo='+tipo;
		if(tipo != 'excel'){
			var janela = window.open('emenda.php?modulo=relatorio/resultRelatorioEmendaImpositivo&acao=A&tipo='+tipo,'page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
			janela.focus();
		}
		
		form.submit();
	}
</script>