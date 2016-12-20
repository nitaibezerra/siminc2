<?php

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
include "../_constantes.php";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

/**
 * INICIO DA VERIFICAÇÃO
 **/
if(is_array($_POST['ugsresp']) && @count($_POST['ugsresp']) > 0) {

	$txtUGSCoordenador = "";
	$UGSTrue = false;
	$acoesConfirmadas = $_REQUEST["acoesConfirmadas"];

	$sqlAcumul = "SELECT pflsncumulativo FROM seguranca.perfil WHERE pflcod = " . $pflcod;
	$dados = $db->pegaLinha($sqlAcumul);
	
	$pflsncumulativo = $dados['pflsncumulativo'];
	
	$sqlSelUGS = "
		Select 	ur.rpuid, 
				ur.usucpf, 
				ur.rpustatus,
				ur.pflcod,
				ung.ungcod,
				ung.ungdsc,
				usu.usunome
		From ted.usuarioresponsabilidade ur
		Join seguranca.usuario usu on usu.usucpf = ur.usucpf
		Join public.unidadegestora ung ON ur.ungcod = ung.ungcod 
		Join public.unidade u ON u.unicod = ung.unicod
		Where ur.rpustatus = 'A' AND ur.usucpf <> '".$usucpf."' AND ur.pflcod = ".$pflcod." and ur.ungcod = '%s'
	";

	$i = 0; 
	$x = 0;
	
	#Verificar quais itens possuem outro coordenador ativo
	if($pflsncumulativo == 'f' && $_POST['ugsresp'][0] != "") {

		foreach ($_POST['ugsresp'] as $ugsCod){
			$sql = '';
			$sql = vsprintf($sqlSelUGS, $ugsCod);
			$respUGS = $db->carregar($sql);

			if ($respUGS[$i] != '') {				
				foreach ($respUGS as $respUnd) {
					$UGSTrue = true;
					$respUnd["ungdsc"] = str_replace( array( "\n", "\r" ), " ", $respUnd["ungdsc"] );
					$txtUGSCoordenador .= $respUnd["ungcod"] . " - " . $respUnd["ungdsc"] . " - Nome: ".$respUnd['usunome']." - CPF: " . $respUnd["usucpf"] . '\\n';
				}
			}
			$i = $i + 1;
		}
	}
	
	#Caso nao existam outros coordenadores, registrar os itens selecionados.
	if(!$UGSTrue || $acoesConfirmadas == 1){
		if($pflsncumulativo == 'f'){
			foreach ($_POST['ugsresp'] as $ugsCod){
				$sql = '';
				$sql = vsprintf($sqlSelUGS, $ugsCod);
				$respUGS = $db->carregar($sql);
				
				if ($respUGS[$x] != '') {
					foreach ($respUGS as $respUnd) {
						$db->executar("Update ted.usuarioresponsabilidade SET rpustatus = 'I' Where usucpf = '".$respUnd['usucpf']."' and pflcod = '".$respUnd['pflcod']."' and ungcod = '".$respUnd['ungcod']."'");
					}
				}
				$x = $x + 1;
				$db->commit();
			}
		}
		
		$ugsresp = $_REQUEST["ugsresp"];		
		atribuiUgs($usucpf, $pflcod, $ugsresp);

		#Exibir a tela de aviso dos itens que já possuem coordenador e confirmar. A substituição pelo usuario que está sendo liberado e/ou alterado
		}else{
			$msg = 'Existem usuários ativos com o perfil Secretário selecionado para estas Entidades:\\n\\n';
			$msg .= $txtUGSCoordenador;
			$msg .= '\\nDeseja sobrescrevê-los?\\n\\n';
			$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<body> 
			<form name="formassocia" style="margin:0px;" method="POST">
				<input type="hidden" name="usucpf" value="<?=$usucpf?>">
				<input type="hidden" name="pflcod" value="<?=$pflcod?>">
				<input type="hidden" name="acoesConfirmadas" value="1"> 
					
		<?php foreach ($_POST['ugsresp'] as $UGScod) { ?>
				<input type="hidden" name="ugsresp[]" value="<?=$UGScod?>">
		<?php } ?>
			
 			</form>
 			 
 			<script> 
				if(confirm("<?=$msg?>")){
					document.formassocia.submit();
				}else{
					self.close();			
				}
			</script>
		</body>
	<?php
		exit(0);
	}
	?>
		<script type="text/javascript">
			alert("Operação realizada com sucesso!");
			opener.location.reload();
			self.close();
		</script>
	<?php 
	exit(0);
}
/**
 * FIM DA VERIFICAÇÃO.
 **/

/**
 * Função que lista os hospitais
 *
 */
function listaUgs(){
	$db = new cls_banco();
	
	// SQL para buscar estados existentes
	$sql = "SELECT ungcod, ungabrev || ' / ' || ungdsc as ungdsc  
			FROM public.unidadegestora 
			WHERE
                unicod = '42101'
                AND ungstatus='A'
			ORDER BY ungcod, ungcod";
	$ugs = $db->carregar($sql);
	
	$count = count($ugs);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $ugs[$i]["ungcod"];
		$descricao = $ugs[$i]["ungdsc"];
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"ungcod\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"ungdsc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
				<td>".$descricao."</td>
			</tr>";
	}
			
}

function atribuiUgs($usucpf, $pflcod, $ungcods)
{
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	if($pflcod == PERFIL_SECRETARIA){
		//$db->executar("UPDATE elabrev.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."'");
		$db->executar("DELETE FROM ted.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."'");
	}else{
		$db->executar("UPDATE ted.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod IS NOT NULL");
	}
	
	if ($ungcods[0]){
		foreach($ungcods as $ungcod) {
			$dadosur = $db->carregar("SELECT * FROM ted.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod = '". $ungcod ."'");
			if($dadosur) {
				// Se existir registro atualizar para ativo
				$db->executar("UPDATE ted.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW(), unicod='26101' 
 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND ungcod = '". $ungcod ."'");
			} else {
				// Se não existir, inserir novo
				$db->executar("INSERT INTO ted.usuarioresponsabilidade(
            				   rpuid, pflcod, usucpf, unicod, ungcod, rpustatus, rpudata_inc, prsano)
    						   VALUES ((select max(rpuid) from ted.usuarioresponsabilidade), '". $pflcod ."', '". $usucpf ."', '26101', '". $ungcod ."', 'A', NOW(), '{$_SESSION['exercicio']}');");
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
				ung.ungcod AS codigo, 
				ung.ungdsc AS descricao
			FROM ted.usuarioresponsabilidade ur
			INNER JOIN public.unidadegestora ung ON ur.ungcod = ung.ungcod 
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
					<?php listaUgs(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_unidadegestoradireta.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="ugsresp[]" id="ugsresp" style="width:500px;" class="CampoEstilo">
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

var campoSelect = document.getElementById("ugsresp");


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
	if (document.formulario.ungcod[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.ungdsc[objeto].value, document.formulario.ungcod[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.ungcod[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na UG.', '', false, false);}
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