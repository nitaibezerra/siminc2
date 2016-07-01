<?php

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo: seleciona_unid_perfilresp.php
   */

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if ($_REQUEST["coordresp"]){
	$coordresp = $_REQUEST["coordresp"];
	atribuiCoordenacoes($usucpf, $pflcod, $coordresp);
}

/**
 * Função que lista os hospitais
 *
 */
function listaCoordenacoes(){
	$db = new cls_banco();
	
	// SQL para buscar estados existentes
	$sql = "SELECT d.cooid, ungabrev || ' - ' || coodsc as coodsc
			FROM public.unidadegestora ug
			INNER JOIN elabrev.coordenacao d ON d.ungcodconcedente = ug.ungcod
			WHERE ungstatus='A' and coostatus = 'A'
			ORDER BY 2";
	$coordenacoes = $db->carregar($sql);
	
	$count = count($coordenacoes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $coordenacoes[$i]["cooid"];
		$descricao = $coordenacoes[$i]["coodsc"];
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"cooid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"coodsc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
				<td>".$descricao."</td>
			</tr>";
	}
			
}

function atribuiCoordenacoes($usucpf, $pflcod, $cooids){
	$db = new cls_banco();
	//ver($pflcod, $cooids);die;
	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE elabrev.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND cooid IS NOT NULL");

	if ($cooids[0]){
		foreach($cooids as $cooid) {
			$dadosur = $db->carregar("SELECT * FROM elabrev.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND prsano = '".$_SESSION['exercicio']."' AND cooid = ". $cooid );
			if($dadosur) {
	
				// Se existir registro atualizar para ativo
				$db->executar("UPDATE elabrev.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW()
 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND cooid = ". $cooid );
			} else {

				$ungcod = $db->pegaUm("SELECT ungcodconcedente FROM elabrev.coordenacao WHERE cooid = ". $cooid);
				$uo = $db->pegaUm("SELECT unicod FROM public.unidadegestora WHERE ungcod = '". $ungcod ."'");
				// Se não existir, inserir novo
				$db->executar("INSERT INTO elabrev.usuarioresponsabilidade(
            				   pflcod, usucpf, cooid, rpustatus, rpudata_inc, unicod, ungcod, prsano )
    						   VALUES ('". $pflcod ."', '". $usucpf ."', ".$cooid.", 'A', NOW(), '".$uo."', '".$ungcod."', '".$_SESSION['exercicio']."');");
			}
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Operação realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

function buscaUgsAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				d.cooid AS codigo, 
				ungabrev || ' / ' || coodsc AS descricao
			FROM elabrev.usuarioresponsabilidade ur 
			inner join elabrev.coordenacao d ON d.cooid = ur.cooid
			inner join public.unidadegestora ug ON ug.ungcod = d.ungcodconcedente
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
		<title>Unidades Gestoras</title>
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
					<?php listaCoordenacoes(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_coordenacao.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="coordresp[]" id="coordresp" style="width:500px;" class="CampoEstilo">
				<?php 
					buscaUgsAtribuido($usucpf, $pflcod);
				?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
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

var campoSelect = document.getElementById("coordresp");


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
	//if (document.formulario.coordresp[objeto].checked == true){
	if (document.formulario.cooid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.coodsc[objeto].value, document.formulario.cooid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.cooid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Coordenação.', '', false, false);}
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
<?php die; ?>