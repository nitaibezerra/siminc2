<?php

/*
 * Verifica se o usuário possui o perfil 'Gestor da Unidade de Planejamento' 
 * e possui a unidade do FNDE(26298) associada.
 */
$GESTOR_FNDE = false;

$sql = "SELECT
			trim(ur.unicod) as unicod
		FROM
			monitora.usuarioresponsabilidade ur
		WHERE
			ur.usucpf = '".$_SESSION["usucpf"]."' AND
			ur.pflcod = 112 AND
			ur.rpustatus = 'A'";

$unidades = $db->carregar($sql);
if($unidades) {
	for($i=0; $i<count($unidades); $i++) {
		if($unidades[$i]["unicod"] == "26298") {
			$GESTOR_FNDE = true;
		}
	}
}


/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número
 */
define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);

/*
 * Constantes das unidades que utilizam regras especificas para o gerador
 * Inclusão de Unidade Gestora e Unidade Responsável na geração do número 
 */


// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";

/*
 * SCRIPT que controle as regras de combobox
 */
include APPRAIZ."/includes/controlecombobox.inc";
if(is_array($_REQUEST['dados'])) {
	$parametroconstrutor = array_merge($_REQUEST['dados'], array());
}
$combo = new ControleComboBox($parametroconstrutor);

/*
 * Constantes das opções da combobox
 */
define('COMBO_REGRAGERAL', 5);
define('COMBO_REGRAPDEEPPA', 3);
/*
 * Constantes das opções da combobox
 */


/*
 * FIM SCRIPT que controle as regras de combobox
 */

// carrega as funções de integração
include "planotrabalho/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";
include "planotrabalho/_componentes.php";

// definindo variaveis
$atiid  = $_POST['atiid'] ? $_POST['atiid'] : $_GET['atiid']; 
$plicod = $_POST['plicod'] ? $_POST['plicod'] : $_GET['plicod']; 

// verifica se ocorre algum evento
if(isset($_REQUEST['evento']) && ($_REQUEST['evento'] != '') ){
	switch($_REQUEST['evento']) {
		
		// inserir plano interno
		case 'I':
			
		$sql = "INSERT INTO monitora.subacao(
            		sbatitulo, sbasigla, sbacod, exeid, gstid, eqdid, sbadsc, sbastatus, usucpf, sbadata, sbaobra)
		    	 VALUES ('".$_POST['sbatitulo']."', NULL, UPPER('".$_REQUEST['sbacod']."'), NULL, NULL, NULL, '".$_POST['sbadsc']."', 'A', '".$_SESSION['usucpf']."', NOW(), NULL) 
		    	 RETURNING sbaid;
		       ";
		$sbaid = $db->pegaUm($sql);
	
		if($_POST['entid']) {
			$sql = "INSERT INTO monitora.subacaounidade(sbaid, entid)
    				VALUES ('".$sbaid."', '".$_POST['entid']."');";
			$db->executar($sql);
		}
		
		$db->commit();
		if($_REQUEST['atiid']) {
			echo "<script>alert('Dados salvos com sucesso.');window.location='monitora.php?modulo=principal/planotrabalho/listarsubacoesatividade&acao=A&atiid=".$_REQUEST['atiid']."';</script>";
		} else {
			echo "<script>alert('Dados salvos com sucesso.');window.close();</script>";
		}
		exit;
		
		
		case 'E':
		$pliid = $db->pegaUm("SELECT pliid FROM monitora.planointerno WHERE plistatus='A' and sbaid='".$_REQUEST['sbaid']."'");
		if($pliid) {
			
			echo "<script>alert('Não é possível excluir! \nExistem um ou mais planos internos associados a esta subação.');window.location = '?modulo=principal/planotrabalho/pesqSubacao&acao=A';</script>";
			exit;
			
		} else {
		
			$sbacod = $db->pegaUm("SELECT sbacod FROM monitora.subacao WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM public.geradorsequencialpi WHERE gspidentificador='".substr($sbacod,0,2)."' AND gspseq='00".substr($sbacod,2,2)."'");
			$db->executar("DELETE FROM monitora.subacaounidade WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM monitora.subacaodotacao WHERE sbaid='".$_REQUEST['sbaid']."'");
			$db->executar("DELETE FROM monitora.subacaodados WHERE sbaid=".$_REQUEST['sbaid']."");
			$sql_D = "DELETE FROM monitora.subacao where sbaid = '".$_REQUEST['sbaid']."'";
			$db->executar($sql_D);
			$db->commit();
			echo "<script>alert('SubAção removido com sucesso.');window.location = '?modulo=principal/planotrabalho/pesqSubacao&acao=A';</script>";
			exit;
			
			
		}
	}
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="<? echo (($_REQUEST['sbaid'])?"G":"I"); ?>">
<input type="hidden" name="atiid" id="atiid" value="<? echo $_REQUEST['atiid']; ?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td><? echo campo_texto('sbatitulo','S','S','',30,25,'','',null,null,null,'',"limitaTitulo();"); ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Código:</td>
    <td><? echo campo_texto('sbacod','S','S','',5,4,'','',null,null,null,''); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Unidades Orçamentárias / Gestoras:</td>
	<td>
	<?
	$unidade = $db->pegaLinha("SELECT * FROM entidade.entidade WHERE entid='".$_SESSION['monitora_var']['entid']."'");
	echo "<input type='hidden' name='entid' value='".$unidade['entid']."'>";
	echo $unidade['entnome'];
	?>
	</td>
</tr>

<tr>
    <td align='right' class="SubTituloDireita" valign=top>Descrição / Finalidade:</td>
    <td><?= campo_textarea( 'sbadsc', 'S', 'S', '', 80, 5, 250 );?></td>
</tr>

<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td><input type="button" class="botao" name="btg" value="Gravar" onclick="submeter('<?=$plicod?>');"> 
	  	  <?
			if($_REQUEST['atiid']) {
				echo "<input type=\"button\" class=\"botao\" name=\"btg1\" value=\"Voltar\" onclick=\"window.location='monitora.php?modulo=principal/planotrabalho/listarsubacoesatividade&acao=A&atiid=".$_REQUEST['atiid']."';\">";
			}
	  	  ?>
	  	  </td>
	  	  
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter(pliid) {
	if(validar()) {
		document.formulario.submit();
	}
}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function validar(){	
	var msg = "";

	if(document.formulario.sbatitulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}
	if(document.formulario.sbacod.value == '') {
		msg+="O preenchimento do campo Código é obrigatório.\n";
	}

	if(msg != ""){
		alert(msg);
		return false;
	}else return true;	
}


function abrir_lista() {
	var janela = window.open( '?modulo=principal/planotrabalho/listarProgramaSubacao&acao=A', 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
	   	alert(novo);
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }
}


function verificaDisponivel(campo, ptrid, vlold) {
	var linha = document.getElementById('ptrid_'+ptrid);
	valordisp = parseFloat(replaceAll(replaceAll(linha.cells[4].innerHTML,".",""),",","."));
	valoratual = parseFloat(replaceAll(replaceAll(campo.value,".",""),",","."));
	if(valoratual>valordisp) {
		alert('Valor não pode ser maior do que o Disponível');
		campo.value = vlold;
		calculovalorPI();
	}
}
function validabotao(bot) {
	form = document.formulario;
	if(bot=='N'){
		window.location='?modulo=principal/planotrabalho/programainstitucional&acao=A';
	}
	if(bot=='V'){
		window.location='?modulo=principal/planotrabalho/pesqSubacao&acao=A';
	}
}
function limitaTitulo() {
	tit = document.formulario.sbatitulo.value;
	tot_tit = tit.length;
	if(tot_tit>24){
		alert('O campo Título possui o limite de 25 caracteres.');
		document.formulario.btg.focus();
	}
}
</script>