<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header('Content-Type: text/html; charset=iso-8859-1');

include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();

$pflcod  = $_GET['pflcod'];
$usucpf  = $_GET['usucpf'];

$entnome = $_REQUEST['entnome'];
$estuf   = $_REQUEST['estuf'];
$muncod  = $_REQUEST['muncod'];
$gravar  = $_REQUEST['gravar'];
$arEntid = $_REQUEST["uniresp"];

if ($_POST && $gravar == 1){
	atribuiEmpresa($usucpf, $pflcod, $arEntid);
}
?>
<html>
<head>
	<meta http-equiv="Pragma" content="no-cache">
	<title>Empresa</title>
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	<script type="text/javascript" src="../../includes/JQuery/jquery-1.7.2.min.js"></script>
	<script type="text/javascript">
	var campoSelect = '';
	$(document).ready(function (){
		document.getElementById('aguarde').style.visibility = "hidden";
		document.getElementById('aguarde').style.display  = "none";
		document.getElementById('tabela').style.display  = 'block';
		campoSelect = document.getElementById("uniresp");
		
		if (campoSelect.options[0].value != ''){
			for(var i=0; i<campoSelect.options.length; i++){
				var id = campoSelect.options[i].value;
//				var id = campoSelect.options[i].value.split('|');
				if (document.getElementById('entid_' + id)){
					document.getElementById('entid_' + id).checked = true;
				}
			}
		}
		
	});
	
	function filtroFunid (id) {
		var d 	   = document;
		var estuf  = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value  : '';
		var muncod = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';
			
		selectAllOptions(campoSelect);
		d.formulario.submit();
	}
	
	function limpaMuncod(){
		if (document.getElementsByName('muncod')[0]) {
			document.getElementsByName('muncod')[0].value='';
		}
	}
	
	function retorna(objeto){
		tamanho = campoSelect.options.length;
		if (campoSelect.options[0].value=='') {tamanho--;}
		if(document.formulario.entid[objeto]){
			if (document.formulario.entid[objeto].checked == true){
				campoSelect.options[tamanho] = new Option(document.formulario.entnome[objeto].value, document.formulario.entid[objeto].value, false, false);
				sortSelect(campoSelect);
			}else {
				for(var i=0; i<=campoSelect.length-1; i++){
					if (document.formulario.entid[objeto].value == campoSelect.options[i].value){
						campoSelect.options[i] = null;
					}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na empresa.', '', false, false);}
				sortSelect(campoSelect);
			}
		} else {
			// qunado possui apenas 1 registro
			if (document.formulario.entid.checked == true){
				campoSelect.options[tamanho] = new Option(document.formulario.entnome.value, document.formulario.entid.value, false, false);
				sortSelect(campoSelect);
			}else{
				campoSelect.options[0] = null;
				if (!campoSelect.options[0]){
					campoSelect.options[0] = new Option('Clique na empresa para selecionar.', '', false, false);
				}
			}
		}
	}
	
	</script>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
	<font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>

<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
<table style="width:100%; display:none;" id="filtro" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<!-- 
	<tr>
		<td class="subtitulodireita">Nome da empresa:</td>
		<td>
		<?//=campo_texto('entnome','N','S','',40,100,'','', '', '', '', 'id="entnome"');?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Unidade Federativa:</td>
		<td>
		<?php
//		$sql = "SELECT
//					 estuf AS codigo,
//					 estuf || ' - ' || estdescricao AS descricao
//				FROM
//				 	territorios.estado
//				ORDER BY
//				 	estuf";
//		
//		$db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'limpaMuncod(); filtroFunid','');
		?>
		</td>
	</tr>
	<? if ($estuf): ?>			
	<tr>
		<td class="subtitulodireita">Município:</td>
		<td>
		<?php
//		$sql = "SELECT
//				 	muncod AS codigo,
//				 	mundescricao AS descricao
//				FROM
//				 	territorios.municipio
//				WHERE
//				 	estuf = '{$estuf}'
//				ORDER BY
//				 	mundescricao";
//		
//		$db->monta_combo('muncod',$sql,'S',"-- Selecione para filtrar --",'filtroFunid','');
		?>
		</td>
	</tr>		
	<? endif; ?>
-->	
</table>		
<!-- Lista de Unidades -->
<div id="tabela" style="overflow:auto; width:496px; height:365px; border:2px solid #ececec; background-color: #ffffff; display: none;">	
	<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead>
			<tr>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) empresa(s)</strong></td>		
			</tr>
		</thead>
		<?php listaEmpresa(); ?>
	</table>
</div>
<script language="JavaScript">
	document.getElementById('filtro').style.display = 'block';
</script>
<!-- Unidades Selecionadas -->
	<input type="hidden" name="usucpf" value="<?=$usucpf?>">
	<input type="hidden" name="pflcod" value="<?=$pflcod?>">
	<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
		<?php 
			buscaEmpresaCadastrada($usucpf, $pflcod);
		?>
	</select>
<!-- Submit do Formulário -->
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.getElementsByName('gravar')[0].value=1; document.formulario.submit();" id="ok">
			<input type="hidden" name="gravar" value="">
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?php
/**
 * Função que atribui a responsabilidade de uma empresa ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiEmpresa($usucpf, $pflcod, $arEntid){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE obras2.usuarioresponsabilidade SET 
				rpustatus = 'I' 
		   	WHERE 
				usucpf = '{$usucpf}' AND 
				pflcod = '{$pflcod}' AND 
				entid IS NOT NULL";
	$sql_zera = $db->executar($sql);
	
	if (is_array($arEntid) && !empty($arEntid[0])){
		$count = count($arEntid);
		
		// Insere a nova unidade
		$sql_insert = "INSERT INTO obras2.usuarioresponsabilidade (
							entid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			if ( $arEntid[$i] != $entidade_antiga ){
				$arrSql[] = "(
								'{$arEntid[$i]}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}'
							 )";
			}
			
			$entidade_antiga = $arEntid[$i];
			
		}

		$sql_insert = (string) $sql_insert . implode(",",$arrSql);
		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");
	
}

function buscaEmpresaCadastrada(){
	global $db;
	
	$pflcod  = $_GET['pflcod'];
	$usucpf  = $_GET['usucpf'];
	
	$sql = "SELECT
				e.entid,
				e.entnome	
			FROM
				obras2.usuarioresponsabilidade ur
			JOIN entidade.entidade e ON e.entid = ur.entid AND
										e.entstatus = 'A'
			WHERE
				ur.rpustatus = 'A' AND
				ur.usucpf = '{$usucpf}' AND
				ur.pflcod = {$pflcod}";
	
	$arDados = $db->carregar( $sql );
//	$arDados = ( $arDados ? $arDados : array() );
	if ( $arDados ){
		foreach ( $arDados as $dados ){
			$entid   = $dados['entid'];
			$entnome = $dados['entnome'];
			
			print '<option value="' . $entid . '">' . $entnome . '</option>';
		}	
	}else{
		print '<option value="">Clique na empresa para selecionar.</option>';
	}
}

function listaEmpresa(){
	global $db;
	
	$entnome = $_REQUEST['entnome'];
	$estuf   = $_REQUEST['estuf'];
	$muncod  = $_REQUEST['muncod'];
	
	$where = array();
	
//	if ( $entnome ){
//		$where[] = ' ILIKE() =  ';
//	}
	
	$sql = "SELECT
				DISTINCT
				e.entid,
				e.entnome
			FROM
				obras2.supervisao_grupo_empresa se
			JOIN entidade.entidade e ON e.entid = se.entid AND
						    			e.entstatus = 'A'
			JOIN entidade.endereco ed ON ed.entid = e.entid AND
										 ed.endstatus = 'A'
			WHERE
				se.sgestatus = 'A'";	
	
	$arDados = $db->carregar( $sql );
	$arDados = ($arDados ? $arDados : array());
	$i = 0;
	foreach ( $arDados as $dados ){
		$entid   = $dados['entid'];
		$entnome = $dados['entnome'];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		$html.= "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"entid\" id=\"entid_" . $entid . "\" value=\"$entid\" onclick=\"retorna('" . $i . "');\">
					<input type=\"hidden\" name=\"entnome\" id=\"entnome_" . $entid . "\" value=\"" . $entnome . "\">
				</td>
				<td>
					<label for=\"entid_" . $entid . "\">
					" . $entnome . "
					</label>
				</td>
			</tr>";
		
		$i++;
	}
	$html.= '<thead>
				<tr>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Total de Registros: ' . count($arDados) . '</strong></td>		
				</tr>
			</thead>';
	echo $html;
	
}
?>