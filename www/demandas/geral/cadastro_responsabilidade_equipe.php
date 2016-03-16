<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header('Content-Type: text/html; charset=iso-8859-1');

include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once '../_constantes.php';
include_once '../_funcoes.php';

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$gravar = $_REQUEST['gravar'];
$perfil = arrayPerfil();

if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) && !in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) && !in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){	
	$sql = "SELECT
				ur.ordid 
			FROM
				demandas.origemdemanda od
				INNER JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = od.ordid
			WHERE
				ur.rpustatus = 'A' AND
				ur.ordid = ".ORIGEM_DEMANDA_SISTEMA_INFORMACAO." AND
				ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR." AND
				ur.usucpf = '".$_SESSION['usucpf']."'
			LIMIT 1;";
	
	if (!$db->pegaUm($sql)){
		die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.close();
			 </script>');	
	}
}

if ($_POST && $gravar == 1){
	$sidid = $_REQUEST["uniresp"];
	atribuiSistema($usucpf, $pflcod, $sidid);
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Sistema</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?flush();?>
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<table class="tabela" style="width:100%; border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="SubTituloDireita" valign="top">
					<b>Célula</b>
				</td>
				<td align="left" style="width:0px;">
				<?php 
				$celid = $_POST['celid'];
				$sql = "SELECT
							DISTINCT
							c.celid AS codigo,
							c.celnome AS descricao
						FROM
							demandas.celula c
							INNER JOIN demandas.sistemacelula sc ON sc.celid = c.celid
						WHERE
							celstatus = 'A'";
				$db->monta_combo( "celid", $sql, 'S', 'Selecione...', '', '' );
				?>
				</td>
				<td align="left" style="padding-left: 5px;">
					<input type="button" name="btFiltro" value="Filtrar" onclick="document.getElementsByName('gravar')[0].value=2; document.formulario.submit();">
				</td>
			</tr>		
		</table>
		<!-- Lista de origens demandas -->
		<div id="tabela" style="overflow:auto; width:496px; height:310px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >
								<input type="Checkbox" name="mark"  onclick="allMark(this);">
							</td>		
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
								<strong>Selecione o(s) Sistema(s)</strong>
							</td>		
						</tr>
					</thead>
					<?php listaSistema(); ?>
				</table>
		</div>
		<!-- Origens demandas Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaSistema($usucpf, $pflcod);
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
<script type="text/JavaScript">

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
//document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = 'block';

var campoSelect = document.getElementById("uniresp");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		var id = campoSelect.options[i].value;
		
		if (document.getElementById(id)){
			document.getElementById(id).checked = true;
		}
	}
}

function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}



function retorna(objeto, vinculado) {
	var d 			 = document;
	var sidid 		 = d.getElementsByName('sidid');
	var orddescricao = d.getElementsByName('orddescricao');
	
	if (sidid[objeto].checked){
		if (vinculado && !confirm('Este sistema já está vinculado a um usuário.\nDeseja sobrescrevê-lo?')){
			sidid[objeto].checked = false;
			return;
		}
	}
	
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (sidid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(orddescricao[objeto].value, sidid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (sidid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no sistema desejado.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function allMark(obj){
	var check = obj.checked;
	var d = document;
	var element = d.getElementsByName('sidid');

	
	for (i=0; i < element.length; i++){
		if (check == true && element[i].checked == false){
			element[i].click();
		}else if (check == false && element[i].checked == true){
			element[i].click();
		}
	}	
}

</script>
	</body>
</html>
<?php
function buscaSistema($usucpf, $pflcod){	
	global $db;
	
	$sql = "SELECT
				sd.sidid AS codigo,
				upper(sd.sidabrev) || ' - ' || sd.siddescricao AS descricao
			FROM
				demandas.sistemadetalhe sd
				INNER JOIN demandas.usuarioresponsabilidade ur ON ur.sidid = sd.sidid 
			WHERE
				ur.usucpf = '{$usucpf}' AND
				ur.pflcod = {$pflcod} AND
				ur.rpustatus = 'A'
			ORDER BY
				descricao ASC;";

	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v){ 
					${$k}=$v;
				}
				
	    		print " <option value=\"{$codigo}\">{$codigo} - {$descricao}</option>";		
			}
		}
	} else{
		
		print '<option value="">Clique faça o filtro para selecionar.</option>';
		
	}
}

/**
 * Função que atribui a responsabilidade de uma origem da demanda ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $sidid
 */
function atribuiSistema($usucpf, $pflcod, $sidid){	
	global $db;

	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE 
					demandas.usuarioresponsabilidade 
				   SET 
					rpustatus = 'I' 
				   WHERE 
					usucpf = '{$usucpf}' AND 
					pflcod = '{$pflcod}' AND 
					sidid IS NOT NULL");

	if (DEMANDA_PERFIL_GERENTE_PROJETO == $pflcod && $sidid[0]){
		$db->executar("UPDATE 
						demandas.usuarioresponsabilidade 
					   SET 
						rpustatus = 'I' 
					   WHERE  
						sidid IN (" . implode(',',$sidid) . ") AND
						pflcod = '{$pflcod}'"); 
	}	
	
	if (is_array($sidid) && !empty($sidid[0])){
		$count = count($sidid);
		
		// Insere a nova origem demanda
		$sql_insert = "INSERT INTO demandas.usuarioresponsabilidade (
							sidid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++):
						
			$arrSql[] = "(
							'{$sidid[$i]}',
							'{$usucpf}', 
							'A', 
							'{$data}', 
							'{$pflcod}'
						 )";

			
		endfor;
	
		$sql_insert = (string) $sql_insert.implode(",",$arrSql);

		$db->executar($sql_insert);
	}
	$db->commit();
	die('<script>
			alert("Operação realizada com sucesso!");
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>');
	
}

/**
 * Função que lista as origens das demandas
 *
 */
function listaSistema(){
	global $db,$usucpf,$pflcod;
	
	$select = array(); 
	$from	= array();
	
	if (DEMANDA_PERFIL_GERENTE_PROJETO == $pflcod){
		$select[] = 'rpuid';
		$from[]   = "LEFT JOIN demandas.usuarioresponsabilidade ur ON sd.sidid = ur.sidid AND
																 	  ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND
																 	  ur.rpustatus = 'A' AND
																      ur.usucpf != '{$usucpf}'";
	}
	
	if ($_POST['celid']){
		$from[] = " INNER JOIN demandas.sistemacelula sc ON sc.sidid = sd.sidid AND sc.celid = " . $_POST['celid']; 
	}
	
	$sql = "SELECT
				sd.sidid,
				upper(sd.sidabrev) || ' - ' || sd.siddescricao as descricao
				".($select ? ','.implode(',',$select) : '')."	
			FROM
				demandas.sistemadetalhe sd
				".implode('',$from)."
			WHERE
				sidstatus= 'A'";
	//echo $sql;
	$sistema = $db->carregar($sql);
	
	$count = count($sistema);

	if ($sistema){
		// Monta as TR e TD com as Origens demandas
		for ($i = 0; $i < $count; $i++){
			$codigo    = $sistema[$i]["sidid"];
			$descricao = $sistema[$i]["descricao"];
			$rpuid 	   = $sistema[$i]["rpuid"];
			
			
			$cor = fmod($i,2) == 0 ? '#f4f4f4' : '#e0e0e0';
			
			echo "<tr bgcolor=\"".$cor."\">
					<td align=\"right\" width=\"10%\">
						<input type=\"Checkbox\" name=\"sidid\" id=\"".$codigo."\" value=\"$codigo\" onclick=\"retorna('".$i."','".$rpuid."');\">
						<input type=\"hidden\" name=\"orddescricao\" value=\"". $codigo .' - '.$descricao ."\">
					</td>
					<td>
						".$descricao."
					</td>
				 </tr>";
		}
	}else{
			echo "<tr>
					<td align=\"center\" style=\"color:red;\">
						A busca não retornou registros.
					</td>
				 </tr>";
	}
}


?>