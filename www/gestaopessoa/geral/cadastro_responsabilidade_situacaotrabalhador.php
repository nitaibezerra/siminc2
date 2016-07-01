<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header('Content-Type: text/html; charset=iso-8859-1');

include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once '../_funcoes.php';
include_once '../_constantes.php';

$db     = new cls_banco();
$perfil = arrayPerfil();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$gravar = $_REQUEST['gravar'];

if ( !in_array(PERFIL_SUPER_USER, $perfil) && !in_array(PERFIL_GESTOR, $perfil) ){	
	
	$sql = "SELECT
				ur.fstid 
			FROM
				gestaopessoa.ftsituacaotrabalhador od
				INNER JOIN gestaopessoa.usuarioresponsabilidade ur ON ur.fstid = od.fstid
			WHERE
				ur.rpustatus = 'A' AND
				ur.pflcod = ".PERFIL_ADMINISTRADOR." AND
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
	$fstid = $_REQUEST["uniresp"];
	atribuiOrigemDemanda($usucpf, $pflcod, $fstid);
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
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<!-- Lista de origens demandas -->
		<div id="tabela" style="overflow:auto; width:496px; height:335px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Área(s) da(s) demanda(s)</strong></td>		
						</tr>
					</thead>
					<?php listaOrigemDemanda(); ?>
				</table>
		</div>
		<!-- Origens demandas Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaOrigemCadastrada($usucpf, $pflcod);
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
	
	var elemento = document.formulario.fstid;
	
	if (!document.formulario.fstid.length){
		elemento 	= document.formulario.fstid;
		elementoDsc = document.formulario.fstdescricao;
	}else{
		elemento 	= document.formulario.fstid[objeto];
		elementoDsc = document.formulario.fstdescricao[objeto];
	}
	
	if (elemento.checked){
		if (vinculado && !confirm('Esta área, já está vinculada a um usuário.\nDeseja sobrescrevê-lo?')){
			elemento.checked = false;
			return;
		}
	}
	
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (elemento.checked == true){
		campoSelect.options[tamanho] = new Option(elementoDsc.value, elemento.value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (elemento.value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Origem da demanda.', '', false, false);}
			sortSelect(campoSelect);
	}
}

/*
function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function filtroFunid (id) {

	var d 	   = document;
	var orgid  = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
	var estuf  = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
	var muncod = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

	if (!orgid){
		alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
		return false;
	}
	
	selectAllOptions(campoSelect);
	d.formulario.submit();
	//window.location.href = '?pflcod=<?=$_GET['pflcod']; ?>&usucpf=<?=$_GET['usucpf']; ?>&funid='+funid+'&estuf='+estuf+'&muncod='+muncod;
}

function limpaMuncod(){
	if (document.getElementsByName('muncod')[0]) {
		document.getElementsByName('muncod')[0].value='';
	}
}
*/
</script>
	</body>
</html>
<?php
function buscaOrigemCadastrada($usucpf, $pflcod){	
	global $db;
	
	$sql = "SELECT
				od.fstid AS codigo,
				od.fstdescricao AS descricao
			FROM
				gestaopessoa.ftsituacaotrabalhador od
				INNER JOIN gestaopessoa.usuarioresponsabilidade ur ON ur.fstid = od.fstid 
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
 * @param string $fstid
 */
function atribuiOrigemDemanda($usucpf, $pflcod, $fstid){	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE 
					gestaopessoa.usuarioresponsabilidade 
				   SET 
					rpustatus = 'I' 
				   WHERE 
					usucpf = '{$usucpf}' AND 
					pflcod = '{$pflcod}' AND 
					fstid IS NOT NULL");

	if (PERFIL_ADMINISTRADOR == $pflcod && $fstid[0]){
		$db->executar("UPDATE 
						gestaopessoa.usuarioresponsabilidade 
					   SET 
						rpustatus = 'I' 
					   WHERE  
						fstid IN (" . implode(',',$fstid) . ") AND
						pflcod = '{$pflcod}'"); 
	}	
	
	if (is_array($fstid) && !empty($fstid[0])){
		$count = count($fstid);
		
		// Insere a nova origem demanda
		$sql_insert = "INSERT INTO gestaopessoa.usuarioresponsabilidade (
							fstid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
						
			$arrSql[] = "(
							'{$fstid[$i]}',
							'{$usucpf}', 
							'A', 
							'{$data}', 
							'{$pflcod}'
						 )";

			
		}
	
		$sql_insert = (string) $sql_insert.implode(",",$arrSql);

		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");
	
}

/**
 * Função que lista as origens das demandas
 *
 */
function listaOrigemDemanda(){
	global $db,$usucpf,$pflcod,$perfil;
	
	$select = array(); 
	$from	= array();
	
	if (PERFIL_ADMINISTRADOR == $pflcod){
		$select[] = 'ur.rpuid';
		$from[]   = "LEFT JOIN gestaopessoa.usuarioresponsabilidade ur ON od.fstid = ur.fstid AND
																 	  ur.pflcod = ".PERFIL_ADMINISTRADOR." AND
																 	  ur.rpustatus = 'A' AND
																      ur.usucpf != '{$usucpf}'";
	}
	
	if ( !in_array(PERFIL_SUPER_USER,$perfil) ){
		$from[] = "INNER JOIN gestaopessoa.usuarioresponsabilidade ur2 ON od.fstid = ur2.fstid AND
																 	  ur2.pflcod IN (".implode(',', $perfil).") AND
																 	  ur2.rpustatus = 'A' AND
																      ur2.usucpf = '{$_SESSION['usucpf']}'";
	}
	
	$sql = "SELECT
				DISTINCT
				od.fstid,
				fstdescricao
				".($select ? ','.implode(',',$select) : '')."	
			FROM
				gestaopessoa.ftsituacaotrabalhador od
				".implode('',$from)."
			WHERE
				fststatus= 'A'
			ORDER BY 2;";
	
	$origemDemanda = $db->carregar($sql);
	
	$count = count($origemDemanda);

	// Monta as TR e TD com as Origens demandas
	for ($i = 0; $i < $count; $i++){
		$codigo    = $origemDemanda[$i]["fstid"];
		$descricao = $origemDemanda[$i]["fstdescricao"];
		$rpuid 	   = $origemDemanda[$i]["rpuid"];
		
		
		$cor = fmod($i,2) == 0 ? '#f4f4f4' : '#e0e0e0';
		
		echo "<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"fstid\" id=\"".$codigo."\" value=\"$codigo\" onclick=\"retorna('".$i."','".$rpuid."');\">
					<input type=\"hidden\" name=\"fstdescricao\" value=\"". $codigo .' - '.$descricao ."\">
				</td>
				<td>
					".$descricao."
				</td>
			 </tr>";
	}

}


?>