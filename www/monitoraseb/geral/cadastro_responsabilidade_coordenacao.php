<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
?>
<script language='javascript' type='text/javascript' src='../js/monitoraseb.js'></script>
<?php 
$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST['acao'];

if ($_REQUEST['coonid']){
	$coonid = $_REQUEST['coonid'];
	atribuiCoordenacoes($usucpf, $pflcod, $coonid);
}

/**
 * Função que lista as coordenações
 *
 */
function listaCoordenacoes(){
	$db = new cls_banco();
	
	// SQL para buscar coordenacoes existentes
	$coordenacoesExistentes = $db->carregar("SELECT	coonid as codigo, coosigla || ' - ' || coodsc as descricao
								 			 FROM monitoraseb.coordenacao
											 ORDER BY coonid");
	$count = count($coordenacoesExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $coordenacoesExistentes[$i]["codigo"];
		$descricao = $coordenacoesExistentes[$i]["descricao"];
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		$vinculoExistente = "";
		if($db->eof("select * from seguranca.perfil where pflcod =".$_REQUEST['pflcod']." AND pflnivel <> 2")){
		
			$sql = "select uresp.* from monitoraseb.usuarioresponsabilidade uresp 
						join seguranca.perfil perfil on (uresp.pflcod = perfil.pflcod)
						where perfil.pflnivel = 2 and usucpf <> '". $_REQUEST['usucpf'] ."' AND rpustatus like 'A'  AND coonid = '". $codigo ."'";
				
				$vinculoExistente = ($db->eof($sql))? "": "disabled";
		}
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"estuf\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\" ".$vinculoExistente."/>
					<input type=\"hidden\" name=\"estdescricao\" value=\"".$descricao."\">
				</td>				
				<td>
					".$descricao."
				</td>
			</tr>";
		
	}
			
}

function atribuiCoordenacoes($usucpf, $pflcod, $coonid){
	$vinculoExistente = true;
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE monitoraseb.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."'");
	if ($coonid[0]){
		foreach($coonid as $coid) {
			if($db->eof("select * from seguranca.perfil where pflcod =".$_REQUEST['pflcod']." AND pflnivel <> 2")){
				$sql = "select uresp.* from monitoraseb.usuarioresponsabilidade uresp 
						join seguranca.perfil perfil on (uresp.pflcod = perfil.pflcod)
						where perfil.pflnivel = 2 and usucpf <> '". $usucpf ."' AND rpustatus like 'A'  AND coonid = '". $coid ."'";
				$vinculoExistente = $db->eof($sql);
			}
			
			if($vinculoExistente){
				$dadosur = $db->carregar("SELECT * FROM monitoraseb.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND coonid = '". $coid ."'");
				if($dadosur) {
					// Se existir registro atualizar para ativo
					$db->carregar("UPDATE monitoraseb.usuarioresponsabilidade
	   							   SET rpustatus = 'A', rpudata_inc= NOW()
	 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND coonid = '". $coid ."'");
				} else {
					// Se não existir, inserir novo
					$db->executar("INSERT INTO monitoraseb.usuarioresponsabilidade(
	            				   pflcod, usucpf, coonid, rpustatus, rpudata_inc)
	    						   VALUES ('". $pflcod ."', '". $usucpf ."', '". $coid ."', 'A', NOW());");
				}
			}
		}
	}
	$db->commit();
	
	if($vinculoExistente){
	echo '
		<script>
			alert(\'Operação realizada com sucesso!\');
			executarScriptPai("carregaAssociacao('.$_SESSION['sisid'].',\''.$usucpf.'\')");
			self.close();
		  </script>';
	}else{
		echo '<script>
				alert(\'A Coordenação selecionada já possui um Coordenador Geral!\');
			  </script>';
	}
	
	
}

function buscaCoordenacoesAtribuido($usucpf, $pflcod){
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				cor.coonid AS codigo, 
				cor.coosigla || ' - ' || cor.coodsc AS descricao 
			FROM monitoraseb.usuarioresponsabilidade ur 
			INNER JOIN monitoraseb.coordenacao cor ON ur.coonid = cor.coonid 
			WHERE ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod";
	
	$RS = @$db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$codigo\">$descricao</option>";		
			}
		}
	} else {
		print '<option value="">Selecione a(s) Coordenação(ões).</option>';
	}
}

?>


<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Coordenações</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?flush();?>
		
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione Coordenação</strong></td>		
						</tr>
					</thead>
					<?php listaCoordenacoes(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action=" " method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="coonid[]" id="estresp" style="width:500px;" class="CampoEstilo">
				<?php 
					buscaCoordenacoesAtribuido($usucpf, $pflcod);
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
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


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

	var tamanho = campoSelect.options.length;
	var campo = campoSelect.options[0];
	if (campo.value=='') {tamanho--;}
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