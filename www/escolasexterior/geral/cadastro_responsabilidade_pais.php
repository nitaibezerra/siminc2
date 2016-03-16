<?
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

if ($gravar == "1"){
	$paiid = $_REQUEST["estresp"];
	atribuiPais( $usucpf, $pflcod, $paiid );
}

function recuperaPais ($paiid = null){
	global $db;	
	
	$sql = "SELECT
				paidescricao
			FROM
				territorios.pais
			WHERE
				paiid = '{$paiid}'";
	
	return $dsc = $db->pegaUm($sql);
}
/**
 * Função que lista as uf's
 *
 */
function listaPaises(){
	
	global $usucpf, $pflcod, $db;	
		
	// SQL para buscar estados existentes
	$paisesExistentes = $db->carregar(
								"SELECT
									paiid, paidescricao
								 FROM 
								 	territorios.pais
								ORDER BY 
									paidescricao");
	
	$count = count($paisesExistentes);

	//$orgdsc = recuperaOrgao($orgid);	
	
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		
		$codigo    = $paisesExistentes[$i]["paiid"];
		$descricao = $paisesExistentes[$i]["paidescricao"];
		
		$sql = "SELECT 
					rpuid 
				FROM 
					escolasexterior.usuarioresponsabilidade 
				WHERE 
					paiid = '{$codigo}' AND 
					pflcod = {$pflcod} AND 
					usucpf = '{$usucpf}' AND 
					rpustatus = 'A'";
	
		$checked = $db->pegaUm( $sql );
		$checado = $checked ? "checked" : "";
		
		$cor = ( fmod($i,2) == 0 ) ? '#f4f4f4' : '#e0e0e0'; 
				
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"paiid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\" {$checado}>
					<input type=\"hidden\" name=\"paidescricao\" value=\"$codigo - $descricao\">
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
 * Função que atribui a responsabilidade de uma uf ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $paiid
 */
function atribuiPais( $usucpf, $pflcod, $paiid ){	
	
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
									paiid IS NOT NULL");
	
	
	if (is_array($paiid) && $paiid[0]){
	
		$count = count($paiid);
		
		for ($i = 0; $i < $count; $i++){
			
			$sql_insere = $db->carregar("INSERT INTO escolasexterior.usuarioresponsabilidade (
										  paiid,
										  usucpf, 
										  rpustatus, 
										  rpudata_inc, 
										  pflcod
									    )VALUES(
									      '{$paiid[$i]}',
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
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		</script>";
	
}

function buscaPaisAtribuido($usucpf, $pflcod){
	
	global $db;

	$sql = "SELECT DISTINCT 
				u.paiid AS codigo, 
				u.paiid||' - '||u.paidescricao AS descricao
			FROM
				escolasexterior.usuarioresponsabilidade ur  	  
			INNER JOIN 
				territorios.pais u ON ur.paiid = u.paiid
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

		<title>Países</title>
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
		
		<!-- Lista de Países -->
		<div id="tabela" style="overflow:auto; width:496px; height:300px; border:2px solid #ececec; background-color: #ffffff;">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o País</strong></td>		
						</tr>
					</thead>
					<?php listaPaises(); ?>
				</table>
		</div>		
		<!-- Países Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		    <select multiple size="8" name="estresp[]" id="estresp" style="width:500px;" class="CampoEstilo" onchange="//moveto(this);">		
					<?=buscaPaisAtribuido($usucpf, $pflcod);?>
			</select>		
		<!-- Submit do Formulário -->
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
	
	if (document.formulario.paiid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.paidescricao[objeto].value, document.formulario.paiid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.paiid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			//if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no País.', '', false, false);}
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