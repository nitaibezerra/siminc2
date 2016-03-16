<?php

if( $_POST['alterabd'] == 'I' ){
	$_REQUEST['orgid'] = $_POST['orgid'][0];
}

if( $_REQUEST['orgid'] ){
	$_SESSION['academico']['orgid'] = ($_REQUEST['orgid'] != $_SESSION['academico']['orgid']) ? $_REQUEST['orgid'] : $_SESSION['academico']['orgid'];
}

if($_REQUEST['exec_function']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['exec_function']($_REQUEST);
	die;
}

$_POST['itmtcu'] = $_SESSION['academico']['orgid'] == 2 ? $_POST['itmtcu'] : 'false';

switch($_REQUEST['alterabd']) {
	case 'I':
		$sql = "INSERT INTO academico.item(
        	    tpiid, itmdsc, itmobs, itmpermiteobs, itmnotatecnica, itmglobal, itmtcu)
		    	VALUES ('". $_POST['tpiid'] ."', '". $_POST['itmdsc'] ."', '". $_POST['itmobs'] ."', '". $_POST['itmpermiteobs'] ."', '". $_POST['itmnotatecnica'] ."', '". $_POST['itmglobal'] ."', ".$_POST['itmtcu'].") RETURNING itmid;";
		
		$itemid = $db->pegaUm($sql);
		if($_POST['orgid']) {
			foreach($_POST['orgid'] as $orgid) {
				$sql = "SELECT MAX(teiordem) AS ordem FROM academico.orgaoitem tpi
				 		LEFT JOIN academico.item itm ON tpi.itmid = itm.itmid 
						WHERE tpi.orgid = '". $orgid ."' AND itm.itmglobal='". $_POST['itmglobal'] ."'";
				$ordem = $db->pegaUm($sql);				
				$sql = "INSERT INTO academico.orgaoitem(itmid, orgid, teiordem) VALUES ('". $itemid ."', '". $orgid ."', '". (($ordem)?$ordem+1:'1') ."')";			
				$db->executar($sql);
			}
		}
		$db->commit();
		echo "<script>
				alert('Os dados do item foram cadastrados com sucesso.');
				window.location = '?modulo=principal/cadastraritens&acao=A';
			  </script>";
		exit;
		break;
	case 'E':
		$sql = "UPDATE academico.item 
				SET 
				tpiid = '". $_POST['tpiid'] ."', 
				itmdsc = '". $_POST['itmdsc'] ."', 
				itmobs = '". $_POST['itmobs'] ."', 
				itmpermiteobs = '". $_POST['itmpermiteobs'] ."', 
				itmnotatecnica = '". $_POST['itmnotatecnica'] ."', 
				itmtcu = ".$_POST['itmtcu'].",
				itmglobal = '". $_POST['itmglobal'] ."' 
				WHERE itmid = '". $_POST['itmid']."'
				";
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('Os dados do item foram atualizados com sucesso.');
				window.location = '?modulo=principal/cadastraritens&acao=A';
			  </script>";
		exit;		
		break;
	case 'R':
		$sql = "DELETE FROM academico.orgaoitem WHERE orgid = '". $_REQUEST['orgid'] ."' AND itmid = '". $_REQUEST['itmid']."'";
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('O item foi excluído com sucesso.');
				window.location = '?modulo=principal/cadastraritens&acao=A&orgid=".$_REQUEST['orgid']."';
			  </script>";
		
		break;
}

session_start();

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";


echo "<br/>";

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<?
switch($_REQUEST['acao']) {
	case 'E':
		$sql = "SELECT * FROM academico.item WHERE itmid = '". $_REQUEST['itmid'] ."'";
		$item = $db->carregar($sql);
		if($item) {
			$item = current($item);
			$tpiid = $item['tpiid'];
			$itmdsc = $item['itmdsc'];
			$itmobs = $item['itmobs'];
			$itmpermiteobs = $item['itmpermiteobs'];						
			$itmnotatecnica = $item['itmnotatecnica'];
			$itmglobal = $item['itmglobal'];
			$itmtcu = $item['itmtcu'];
		}
		$sql = "SELECT tpe.orgid as codigo, tpe.orgdesc as descricao FROM academico.orgaoitem tei
				LEFT JOIN academico.orgao tpe ON tpe.orgid = tei.orgid 
				WHERE tei.itmid = '". $_REQUEST['itmid'] ."'";
		$orgid = $db->carregar($sql);
	case 'I':
	$titulo_modulo = "Sistema de Informações Gerenciais";
	monta_titulo( $titulo_modulo, 'Gerenciar itens' );
		
?>
<form id="formulario" name="formulario" method="post" action="?modulo=principal/cadastraritens&acao=<? echo $_REQUEST['acao']; ?>&orgid=<? echo $_SESSION['academico']['orgid']; ?>" onSubmit="selectAllOptions( document.getElementById('orgid'));return validarFormularioCadastrarItens(this);">
<? echo (($_REQUEST['acao']== 'E')?"<input type='hidden' name='itmid' value='". $_REQUEST['itmid'] ."'>":""); ?>
	<input type="hidden" name="orgid" value="<?=$_SESSION['academico']['orgid'] ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Tipo de ensino :</td>
		<td>
		<?
		$sqlComboSituacao = "SELECT	orgid as codigo, orgdesc as descricao
						 	FROM academico.orgao";
		combo_popup('orgid', $sqlComboSituacao, 'Orgão', "400x400", 0, array(), "", (($orgid[0])?"N":"S"), false, false, 5, 400 );
		?>
		<img src="../imagens/obrig.gif" border="0">
		</td>
	</tr>
	<tr>
			<td class="SubTituloDireita">Descrição do item :</td>
			<td>
			<? echo campo_texto('itmdsc', "S", "S", "Descrição do item", 67, 150, "", "", '', '', 0, '' ); ?>
			</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de item :</td>
		<td>
		<? 
		$sql = "SELECT tpiid AS codigo, tpidsc AS descricao FROM academico.tipoitem";
		$db->monta_combo('tpiid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Permite observação ?</td>
		<td><input type="radio" name="itmpermiteobs" value="0" <? echo (($itmpermiteobs == "f" || $itmpermiteobs == "")?'checked':''); ?>> Não <input type="radio" name="itmpermiteobs" value="1" <? echo (($itmpermiteobs == "t")?'checked':''); ?>> Sim</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Faz parte da nota técnica ?</td>
		<td><input type="radio" name="itmnotatecnica" value="0" <? echo (($itmnotatecnica == "f" || $itmnotatecnica == "")?'checked':''); ?>> Não <input type="radio" name="itmnotatecnica" value="1" <? echo (($itmnotatecnica == "t")?'checked':''); ?>> Sim</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Item global ?</td>
		<td><input type="radio" name="itmglobal" value="0" <? echo (($itmglobal == "f" || $itmglobal == "")?'checked':''); ?>> Não <input type="radio" name="itmglobal" value="1" <? echo (($itmglobal == "t")?'checked':''); ?>> Sim</td>
	</tr>
	<?php 
	if( $_SESSION['academico']['orgid'] == 2 || $_REQUEST['acao'] == 'I' ){
	?>
	<tr>
		<td class="SubTituloDireita">Faz parte do Indicador TCU ?</td>
		<td>
			<input type="radio" name="itmtcu" value="false" <? echo (($itmtcu == "f" || $itmtcu == "")?'checked':''); ?>> Não 
			<input type="radio" name="itmtcu" value="true" <? echo (($itmtcu == "t")?'checked':''); ?>> Sim
		</td>
	</tr>
	<?php 
	}
	?>
	<tr>
		<td class="SubTituloDireita">Observação :</td>
		<td>
		<? echo campo_textarea( 'itmobs', 'N', 'S', '', '70', '4', '1000');	?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td>
			<div style="float: left;">
				<input type="hidden" name="alterabd" value="<? echo $_REQUEST['acao']; ?>" />
				<input type="submit" value="Salvar" onclick="this.disabled=true;" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
				<input type="button" value="Voltar" style="cursor: pointer" onclick="window.location = '?modulo=principal/cadastraritens&acao=A'; this.disabled=true;">
			</div>
		</td>
	</tr>
</table>
</form>
<?
	break;
	case 'A':
		// 	Monta menu contendo os tipos de ensino
		$sql = "SELECT orgid AS id, orgdesc AS descricao, '/academico/academico.php?modulo=principal/cadastraritens&acao=A&orgid=' || orgid AS link  FROM academico.orgao";
		$menutipoensino = $db->carregar($sql);
		if ($menutipoensino){
   			echo montarAbasArray($menutipoensino, (($_REQUEST['orgid'])?false:'/academico/academico.php?modulo=principal/cadastraritens&acao=A&orgid=' . TIPOENSINO_DEFAULT));
		}
		
		$titulo_modulo = "Sistema de Informações Gerenciais";
		monta_titulo( $titulo_modulo, 'Gerenciar itens' );
		?>
<div id="listaitens">

</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: left;">
		<input type="button" value="Cadastrar item" onclick="window.location='?modulo=principal/cadastraritens&acao=I';">
		</div>
	</td>
</tr>
</table>
<?
}
?>
<div id="test">

</div>
<script>
academico_ajaxatualizar("orgid=<? echo (($_REQUEST['orgid'])?$_REQUEST['orgid']:TIPOENSINO_DEFAULT); ?>&exec_function=academico_listaitens","listaitens");

function ordenaritens(itematual, itemir) {
	academico_ajaxatualizar("orgid=<? echo (($_REQUEST['orgid'])?$_REQUEST['orgid']:TIPOENSINO_DEFAULT); ?>&itematual="+itematual+"&itemir="+itemir+"&exec_function=academico_ordenaritens");
	academico_ajaxatualizar("orgid=<? echo (($_REQUEST['orgid'])?$_REQUEST['orgid']:TIPOENSINO_DEFAULT); ?>&exec_function=academico_listaitens","listaitens");
}
</script>