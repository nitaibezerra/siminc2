<?php
ini_set("memory_limit", "100M");

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Monitoramento de Obras', 'Nível de Execução' );
?>
<form action="obras.php?modulo=relatorio/popupRelatorioCarimbo&acao=A" method="post" name="filtro" id="filtro">
	<input type="hidden" name="tipoRelatorio" id="tipoRelatorio" value=""  />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
	<td>
		<?php 
		$sql = "SELECT 
					tpoid AS codigo, 
					tpodsc AS descricao
  				FROM 
  					obras.tipologiaobra
				WHERE
  					tpostatus = 'A'
				ORDER BY
					tpodsc";
					
		$db->monta_combo( "tpoid", $sql, "S", "Todas", "", "", "", "", "N", "tpoid" );?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Visualização:</td>
	<td>
		<input type="radio" name="carimbo" value="E" /> Externo
		<input type="radio" name="carimbo" value="I" checked="checked" /> Interno
		<!--  <input type="radio" name="carimbo" id="" value="" /> Todas-->
	</td>
</tr>
<?
	// Programa
	$stSql = "SELECT
				prfid AS codigo,
				prfdesc AS descricao
			 FROM 
				obras.programafonte
			 ORDER BY
				prfdesc ";
	$stSqlCarregados1 = "SELECT
							prfid AS codigo,
							prfdesc AS descricao
						 FROM 
							obras.programafonte
						where
							prfid = 41";
	$_REQUEST['prfid'][0] = 41; 
	mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlCarregados1, 'Selecione o(s) Programa(s)' );
	
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
	$stSqlCarregados2 = "SELECT
							tooid AS codigo,
							toodescricao AS descricao
						 FROM 
							obras.tipoorigemobra
						WHERE
							tooid = 1";
	$_REQUEST['tooid'][0] = 1; 
	mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $stSqlCarregados2, 'Selecione a(s) Fonte(s)' );
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
		
			$db->monta_combo("entidunidade", $sql, "S", "Todos", "", "", "", "550", "N", "entidunidade" );
			
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
	<td class="SubTituloDireita" style="width: 190px;">ID Obras/ID Pré-Obra:</td>
	<td>
		<?php echo campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Situação Atual:</td>
	<td><input type="checkbox" name="nivel[]" id="nivel_std" value="SD" onclick="validaCheck(this);"/> Adequado
		<input type="checkbox" name="nivel[]" id="nivel_sta" value="SA" onclick="validaCheck(this);"/> Atenção
		<input type="checkbox" name="nivel[]" id="nivel_stp" value="SP" onclick="validaCheck(this);"/> Preocupante
		<input type="checkbox" name="nivel[]" id="nivel_st" value="ST" onclick="validaCheck(this);"/> Todas</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">Nível:</td>
	<td>
		<table class="tabela" style="width: 550px;" align="left" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
		<tr>
			<td class="SubTituloDireita">Homologação:</td>
			<td><input type="checkbox" name="nivel[]" id="nivel_hod" value="HD" onclick="validaCheck(this);"/> Adequado
				<input type="checkbox" name="nivel[]" id="nivel_hoa" value="HA" onclick="validaCheck(this);"/> Atenção
				<input type="checkbox" name="nivel[]" id="nivel_hop" value="HP" onclick="validaCheck(this);"/> Preocupante
				<input type="checkbox" name="nivel[]" id="nivel_ho" value="HT" onclick="validaCheck(this);"/> Todas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">25% de Execução:</td>
			<td><input type="checkbox" name="nivel[]" id="nivel_25d" value="25D" onclick="validaCheck(this);"/> Adequado
				<input type="checkbox" name="nivel[]" id="nivel_25a" value="25A" onclick="validaCheck(this);"/> Atenção
				<input type="checkbox" name="nivel[]" id="nivel_25p" value="25P" onclick="validaCheck(this);"/> Preocupante
				<input type="checkbox" name="nivel[]" id="nivel_25" value="25T" onclick="validaCheck(this);"/> Todas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">50% de Execução:</td>
			<td><input type="checkbox" name="nivel[]" id="nivel_50d" value="50D" onclick="validaCheck(this);"/> Adequado
				<input type="checkbox" name="nivel[]" id="nivel_50a" value="50A" onclick="validaCheck(this);"/> Atenção
				<input type="checkbox" name="nivel[]" id="nivel_50p" value="50P" onclick="validaCheck(this);"/> Preocupante
				<input type="checkbox" name="nivel[]" id="nivel_50" value="50T"  onclick="validaCheck(this);"/> Todas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">75% de Execução:</td>
			<td><input type="checkbox" name="nivel[]" id="nivel_75d" value="75D" onclick="validaCheck(this);"/> Adequado
				<input type="checkbox" name="nivel[]" id="nivel_75a" value="75A" onclick="validaCheck(this);"/> Atenção
				<input type="checkbox" name="nivel[]" id="nivel_75p" value="75P" onclick="validaCheck(this);"/> Preocupante
				<input type="checkbox" name="nivel[]" id="nivel_75" value="75T" onclick="validaCheck(this);" /> Todas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">100% de Execução:</td>
			<td><input type="checkbox" name="nivel[]" id="nivel_100d" value="100D" onclick="validaCheck(this);" /> Adequado
				<input type="checkbox" name="nivel[]" id="nivel_100a" value="100A" onclick="validaCheck(this);" /> Atenção
				<input type="checkbox" name="nivel[]" id="nivel_100p" value="100P" onclick="validaCheck(this);" /> Preocupante
				<input type="checkbox" name="nivel[]" id="nivel_100" value="100T" onclick="validaCheck(this);" /> Todas</td>
		</tr>
		</table>
	</td>
</tr>
<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="obras_exibeRelatorioCarimbo();" style="cursor: pointer;"/>
		<input type="button" value="Exportar Planilha" onclick="obras_exportarPlanilhaCarimbo()" style="cursor: pointer;"/>
		<input type="button" value="Limpar" onclick="javascript: window.location.href=window.location" style="cursor: pointer;"/>
	</td>
</tr>
</table>
<script type="text/javascript">
var formulario = document.getElementById('filtro');

function obras_exibeRelatorioCarimbo(){
	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( formulario.prfid );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( formulario.tooid );
	}
	document.getElementById('tipoRelatorio').value = "";
	formulario.target = 'RelatorioCarimbo';
	var janela = window.open( 'obras.php?modulo=relatorio/popupRelatorioCarimbo&acao=A', 'RelatorioCarimbo', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
	janela.focus();
	formulario.submit();		
}

function obras_exportarPlanilhaCarimbo()
{
	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( formulario.prfid );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( formulario.tooid );
	}
	document.getElementById('tipoRelatorio').value = "excel";
	formulario.submit();
}

function validaCheck(valor){
	var nivel = valor.id.split("_");
	var nivelSelect = nivel[1].substring(0, 2 );
	
	for(i=0; i<formulario.length; i++){
		if( formulario.elements[i].type == 'checkbox' ){
			if(formulario.elements[i].id.indexOf('nivel_') != -1  && formulario.elements[i].id.indexOf(nivelSelect) != -1 ){
				if( valor.value == 'TD' && formulario.elements[i].id != valor.id && valor.checked == true ){
					//formulario.elements[i].checked = true;
				} else if(valor.value == 'TD' && formulario.elements[i].id != valor.id && valor.checked == false) {
					formulario.elements[i].checked = false;
				} else if( formulario.elements[i].value == 'TD' && valor.value != 'TD' ){
					formulario.elements[i].checked = false;
				}
			}
		}
	}
}
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco ){
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
function onOffCampo( campo ){
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