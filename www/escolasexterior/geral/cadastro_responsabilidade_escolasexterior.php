<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   M�dulo:seleciona_unid_perfilresp.php
  
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

if ($gravar == "1"){
	$eexid = $_REQUEST["estresp"];
	atribuiPais( $usucpf, $pflcod, $eexid );
}

function recuperaEscola ($eexid = null){
	global $db;	
	
	$sql = "SELECT
				eexnomeestabelecimento
			FROM
				escolasexterior.escolasexterior
			WHERE
				eexid = '{$eexid}'";
	
	return $dsc = $db->pegaUm($sql);
}
/**
 * Fun��o que lista as uf's
 *
 */
function listaEscolas(){
	
	global $usucpf, $pflcod, $db;	
		
	// SQL para buscar estados existentes
	$escolasExistentes = $db->carregar(
								"SELECT
									eexid, eexnomeestabelecimento
								 FROM 
								 	escolasexterior.escolasexterior
								ORDER BY 
									eexnomeestabelecimento");
	
	$count = count($escolasExistentes);

	//$orgdsc = recuperaOrgao($orgid);	
	
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		
		$codigo    = $escolasExistentes[$i]["eexid"];
		$descricao = $escolasExistentes[$i]["eexnomeestabelecimento"];
		
		$sql = "SELECT 
					rpuid 
				FROM 
					escolasexterior.usuarioresponsabilidade 
				WHERE 
					eexid = '{$codigo}' AND 
					pflcod = {$pflcod} AND 
					usucpf = '{$usucpf}' AND 
					rpustatus = 'A'";
	
		$checked = $db->pegaUm( $sql );
		$checado = $checked ? "checked" : "";
		
		$cor = ( fmod($i,2) == 0 ) ? '#f4f4f4' : '#e0e0e0'; 
				
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"eexid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\" {$checado}>
					<input type=\"hidden\" name=\"eexnomeestabelecimento\" value=\"$codigo - $descricao\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">
					".$codigo."
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}
			
}

/**
 * Fun��o que atribui a responsabilidade de uma uf ao usu�rio
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $eexid
 */
function atribuiPais( $usucpf, $pflcod, $eexid ){	
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	// Altera o status para I
	$sql_limpa = $db->executar("UPDATE 
								 	escolasexterior.usuarioresponsabilidade 
								SET 
									rpustatus = 'I' 
								WHERE 
									usucpf = '{$usucpf}' AND 
									pflcod = '{$pflcod}' AND 
									eexid IS NOT NULL");
	
	
	if (is_array($eexid) && $eexid[0]){
	
		$count = count($eexid);
		
		for ($i = 0; $i < $count; $i++){
			
			$sql_insere = $db->carregar("INSERT INTO escolasexterior.usuarioresponsabilidade (
										  eexid,
										  usucpf, 
										  rpustatus, 
										  rpudata_inc, 
										  pflcod
									    )VALUES(
									      '{$eexid[$i]}',
									      '{$usucpf}', 
									      'A', 
									      '{$data}', 
									      '{$pflcod}' 
									    )");
					
		}
	
	}

	$db->commit();
	
	echo "
		<script>
			alert('Opera��o realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		</script>";
	
}

function buscaEscolaAtribuido($usucpf, $pflcod){
	
	global $db;

	$sql = "SELECT DISTINCT 
				u.eexid AS codigo, 
				u.eexid||' - '||u.eexnomeestabelecimento AS descricao
			FROM
				escolasexterior.usuarioresponsabilidade ur  	  
			INNER JOIN 
				escolasexterior.escolasexterior u ON ur.eexid = u.eexid
			WHERE 
				ur.rpustatus = 'A' AND
				ur.usucpf = '$usucpf' AND 
				ur.pflcod = $pflcod;";

	$RS = $db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k => $v) ${$k} = $v;
				$option .= " <option value=\"$codigo\">$descricao</option>";
	    				
			}
		}
	}
	
	return $option;
	
}

?>


<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">

		<title>Escolas</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?flush();?>
	<form name="formulario" method="post">
		
		<!-- Lista de Escolas -->
		<div id="tabela" style="overflow:auto; width:496px; height:300px; border:2px solid #ececec; background-color: #ffffff;">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o Escola</strong></td>		
						</tr>
					</thead>
					<?php listaEscolas(); ?>
				</table>
		</div>		
		<!-- Escolas Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		    <select multiple size="8" name="estresp[]" id="estresp" style="width:500px;" class="CampoEstilo" onchange="//moveto(this);">		
					<?=buscaEscolaAtribuido($usucpf, $pflcod);?>
			</select>		
		<!-- Submit do Formul�rio -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="hidden" name="gravar" id="gravar" value="">
					<input type="Button" name="ok" value="OK" onclick="document.getElementById('gravar').value=1; enviaForm();" id="ok">
				</td>
			</tr>
		</table>
	</form>		
	</body>
</html>

<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

var campoSelect = document.getElementById("estresp");

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
	
	if (document.formulario.eexid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.eexnomeestabelecimento[objeto].value, document.formulario.eexid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.eexid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			//if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Escola.', '', false, false);}
			//sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function enviaForm(){
	selectAllOptions(campoSelect);
	document.formulario.submit();
}

</script>