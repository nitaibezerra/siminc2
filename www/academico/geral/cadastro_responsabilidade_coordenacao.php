<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
$gravar = $_REQUEST['gravar'];
$uniresp = $_REQUEST["uniresp"];

if ($_POST && $gravar == 1){
	atribui($usucpf, $pflcod, $uniresp);
}

/**
 * Função que lista as unidades
 *
 */
function listaCoordenacao(){
	global $db;
	$sql = "SELECT 
					  cooid,
					  coodsc
					FROM 
					  academico.coordenacao
					WHERE
						coostatus = 'A'
					ORDER BY
						coodsc";
	$arrCoor = $db->carregar($sql);
	
	if (!$arrCoor){
		echo "<tr>
				<td style='color:red;'>Busca não retornou registros...</td>
			  </tr>";
		return;
	}
	$count = count($arrCoor);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $arrCoor[$i]["cooid"];
		$descricao = $arrCoor[$i]["coodsc"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"unicod\" id=\"".$codigo."\" value=\"$codigo\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"unidsc\" value=\"".($descricao)."\">
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}

}

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribui($usucpf, $pflcod, $uniresp){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql_zera = $db->executar("UPDATE 
								academico.usuarioresponsabilidade 
							   SET 
								rpustatus = 'I' 
							   WHERE 
								usucpf = '{$usucpf}' AND 
								pflcod = '{$pflcod}' AND 
								-- prsano = '{$_SESSION["exercicio"]}' AND 
								conid IS NOT NULL");
	
	if (is_array($uniresp) && !empty($uniresp[0])){
		$count = count($uniresp);
				
		// Insere a nova unidade
		$sql_insert = "INSERT INTO academico.usuarioresponsabilidade (
							cooid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod -- , 
							-- prsano
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){			
			$arrSql[] = "(
							'{$uniresp[$i]}',
							'{$usucpf}', 
							'A', 
							'{$data}', 
							'{$pflcod}' -- , 
						--	'{$_SESSION["exercicio"]}'
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

function buscaDadosCadastrado($usucpf, $pflcod){
	
	global $db;
	
	/*if (!$_POST['gravar'] && $_REQUEST["uniresp"][0]){
		foreach ($_REQUEST["uniresp"] as $v){
			list(,$entid[]) = explode('|', $v );
		}
		$where = " e.entid IN (".implode(',',$entid).")";
	}else{*/
		$where = " and ur.usucpf = '{$usucpf}' AND 
			 	    ur.pflcod = {$pflcod}";	
	//}

	$sql = "SELECT 
			  c.cooid as codigo,
			  c.coodsc as descricao
			FROM 
			  academico.coordenacao c
              inner join academico.usuarioresponsabilidade ur on ur.cooid = c.cooid
			WHERE 
				c.coostatus = 'A'
				and ur.rpustatus='A'
			 ".$where;
	
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k=>$v){ 
					${$k}=$v;
				}  
				
	    		print " <option value=\"$codigo\">$codigo - $descricao</option>";
	    		
			}
		}
	} else{
		print '<option value="">Clique faça o filtro para selecionar.</option>';
		
	}
	
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades</title>
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
		<!-- Lista de Unidades -->
		<div id="tabela" style="overflow:auto; width:496px; height:370px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						//document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>		
						</tr>
					</thead>
					<?php listaCoordenacao(); ?>
				</table>
		</div>
		<script language="JavaScript">
			//document.getElementById('filtro').style.display = 'block';
		</script>
		<!-- Unidades Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaDadosCadastrado($usucpf, $pflcod);
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
		var id = campoSelect.options[i].value; //.split('|');
		
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



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.unicod[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.unicod[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}
</script>
	</body>
</html>
