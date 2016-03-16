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

$gravar  = $_REQUEST['gravar'];
$arEmiid = $_REQUEST["uniresp"];

if ($_POST && $gravar == 1){
	atribuiEmpresa($usucpf, $pflcod, $arEmiid);
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
				for(var i=0; i < campoSelect.options.length; i++){
					var id = campoSelect.options[i].value;
					if (document.getElementById('emiid_' + id)){
						document.getElementById('emiid_' + id).checked = true;
					}
				}
			}
			
		});
		
		function retorna(objeto){
			tamanho = campoSelect.options.length;
			if (campoSelect.options[0].value=='') {tamanho--;}
			if(document.formulario.emiid[objeto]){
				if (document.formulario.emiid[objeto].checked == true){
					campoSelect.options[tamanho] = new Option(document.formulario.emidsc[objeto].value, document.formulario.emiid[objeto].value, false, false);
					sortSelect(campoSelect);
				}else {
					for(var i=0; i<=campoSelect.length-1; i++){
						if (document.formulario.emiid[objeto].value == campoSelect.options[i].value){
							campoSelect.options[i] = null;
						}
					}
					if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na empresa.', '', false, false);}
					sortSelect(campoSelect);
				}
			} else {
				// qunado possui apenas 1 registro
				if (document.formulario.emiid.checked == true){
					campoSelect.options[tamanho] = new Option(document.formulario.emidsc.value, document.formulario.emiid.value, false, false);
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
function atribuiEmpresa($usucpf, $pflcod, $arEmiid){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE obras2.usuarioresponsabilidade SET 
				rpustatus = 'I' 
		   	WHERE 
				usucpf = '{$usucpf}' AND 
				pflcod = '{$pflcod}' AND 
				emiid IS NOT NULL";
	$sql_zera = $db->executar($sql);
	
	if (is_array($arEmiid) && !empty($arEmiid[0])){
		$count = count($arEmiid);
		
		// Insere a nova unidade
		$sql_insert = "INSERT INTO obras2.usuarioresponsabilidade (
							emiid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			if ( $arEmiid[$i] != $entidade_antiga ){
				$arrSql[] = "(
								'{$arEmiid[$i]}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}'
							 )";
			}
			
			$entidade_antiga = $arEmiid[$i];
			
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
	
	$sql = "SELECT DISTINCT
				e.emiid as codigo,
				e.emidsc as descricao
			FROM
				obras2.usuarioresponsabilidade ur
			INNER JOIN obras2.empresami e ON e.emiid = ur.emiid AND e.emistatus = 'A'
			WHERE
				ur.rpustatus = 'A' AND
				ur.usucpf = '{$usucpf}' AND
				ur.pflcod = {$pflcod}";
	
	$arDados = $db->carregar( $sql );

	if ( $arDados ){
		foreach ( $arDados as $dados ){
			extract($dados);
			
			print '<option value="' . $codigo . '">' . $descricao . '</option>';
		}	
	}else{
		print '<option value="">Clique na empresa para selecionar.</option>';
	}
}

function listaEmpresa(){
	global $db;
	
	$sql = "SELECT DISTINCT
				emiid as codigo,
				emidsc as descricao
			FROM 
				obras2.empresami 
			WHERE
				emistatus = 'A'";	
	
	$arDados = $db->carregar( $sql );
	$arDados = ($arDados ? $arDados : array());
	$i = 0;
	foreach ( $arDados as $dados ){
		
		extract($dados);
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		$html.= "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"emiid\" id=\"emiid_" . $codigo . "\" value=\"$codigo\" onclick=\"retorna('" . $i . "');\">
					<input type=\"hidden\" name=\"emidsc\" id=\"emidsc_" . $codigo . "\" value=\"" . $descricao . "\">
				</td>
				<td>
					<label for=\"emiid_" . $codigo . "\">
					" . $descricao . "
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