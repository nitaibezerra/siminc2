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

if ($_REQUEST["tipoexecucaoresp"]){
	$tipoexecucaoresp = $_REQUEST["tipoexecucaoresp"];
	atribuiTipoExecucao($usucpf, $pflcod, $tipoexecucaoresp);
}

/**
 * Fun��o que lista os hospitais
 *
 */
function listaTipoExecucao(){
	$db = new cls_banco();
	
	// SQL para buscar estados existentes
	$tipoexecucao = $db->carregar("SELECT tpeid, tpedsc
								   FROM fabrica.tipoexecucao
							       WHERE tpestatus = 'A' 
								   ORDER BY tpedsc");
	
	$count = count($tipoexecucao);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $tipoexecucao[$i]["tpeid"];
		$descricao = $tipoexecucao[$i]["tpedsc"];
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"estuf\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"estdescricao\" value=\"".$codigo." - ".$descricao."\">
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

function atribuiTipoExecucao($usucpf, $pflcod, $tipoexecucao) {
	
	$db = new cls_banco();
	$data = date("Y-m-d H:i:s");
	$db->executar("UPDATE fabrica.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND tpeid IS NOT NULL");
	if ($tipoexecucao[0]){
		foreach($tipoexecucao as $tipo) {
			$dadosur = $db->carregar("SELECT * FROM fabrica.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND tpeid = '". $tipo ."'");
			if($dadosur[0]) {
				// Se existir registro atualizar para ativo
				$db->carregar("UPDATE fabrica.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW()
 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND tpeid = '". $tipo ."'");
			} else {
				// Se n�o existir, inserir novo
				$db->executar("INSERT INTO fabrica.usuarioresponsabilidade (
            				   pflcod, usucpf, tpeid, rpustatus, rpudata_inc)
    						   VALUES ('". $pflcod ."', '". $usucpf ."', '". $tipo ."', 'A', NOW());");
			}
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Opera��o realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

function buscaTipoExecucaoAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				tpe.tpeid AS codigo, 
				tpe.tpedsc AS descricao
			FROM fabrica.usuarioresponsabilidade ur 
			INNER JOIN fabrica.tipoexecucao tpe ON ur.tpeid = tpe.tpeid 
			WHERE ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod";
	
	$RS = @$db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$codigo\">$codigo - $descricao</option>";		
			}
		}
	} else {
		print '<option value="">Clique no estado selecionar.</option>';
	}
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Tipo de execu��o</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione o tipo de ensino</strong></td>		
						</tr>
					</thead>
					<?php listaTipoExecucao(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_tipoexecucao.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="tipoexecucaoresp[]" id="estresp" style="width:500px;" class="CampoEstilo">
				<?php 
					buscaTipoExecucaoAtribuido($usucpf, $pflcod);
				?>
			</select>
		</form>
		
		<!-- Submit do Formul�rio -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="JavaScript">

var campoSelect = document.getElementById("estresp");


if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
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
	if (document.formulario.estuf[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.estdescricao[objeto].value, document.formulario.estuf[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.estuf[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Estado.', '', false, false);}
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