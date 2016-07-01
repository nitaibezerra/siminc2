<?php

if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
	die;
}
switch($_REQUEST['alterabd']) {
	case 'I':
		$sql = "INSERT INTO sig.item(
        	    tpiid, itmdsc, itmobs, itmpermiteobs, itmnotatecnica, itmglobal)
		    	VALUES ('". $_POST['tpiid'] ."', '". $_POST['itmdsc'] ."', '". $_POST['itmobs'] ."', '". $_POST['itmpermiteobs'] ."', '". $_POST['itmnotatecnica'] ."', '". $_POST['itmglobal'] ."') RETURNING itmid;";
		
		$itemid = $db->pegaUm($sql);
		if($_POST['tpeid']) {
			foreach($_POST['tpeid'] as $tpeid) {
				$sql = "SELECT MAX(teiordem) AS ordem FROM sig.tipoensinoitem tpi
				 		LEFT JOIN sig.item itm ON tpi.itmid = itm.itmid 
						WHERE tpi.tpeid = '". $tpeid ."' AND itm.itmglobal='". $_POST['itmglobal'] ."'";
				$ordem = $db->pegaUm($sql);				
				$sql = "INSERT INTO sig.tipoensinoitem(itmid, tpeid, teiordem) VALUES ('". $itemid ."', '". $tpeid ."', '". (($ordem)?$ordem+1:'1') ."')";			
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
		$sql = "UPDATE sig.item 
				SET 
				tpiid = '". $_POST['tpiid'] ."', 
				itmdsc = '". $_POST['itmdsc'] ."', 
				itmobs = '". $_POST['itmobs'] ."', 
				itmpermiteobs = '". $_POST['itmpermiteobs'] ."', 
				itmnotatecnica = '". $_POST['itmnotatecnica'] ."', 
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
		$sql = "DELETE FROM sig.tipoensinoitem WHERE tpeid = '". $_REQUEST['tpeid'] ."' AND itmid = '". $_REQUEST['itmid']."'";
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('O item foi excluído com sucesso.');
				window.location = '?modulo=principal/cadastraritens&acao=A&tpeid=".$_REQUEST['tpeid']."';
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
<script language="JavaScript" src="./js/sig.js"></script>
<?
switch($_REQUEST['acao']) {
	case 'E':
		$sql = "SELECT * FROM sig.item WHERE itmid = '". $_REQUEST['itmid'] ."'";
		$item = $db->carregar($sql);
		if($item) {
			$item = current($item);
			$tpiid = $item['tpiid'];
			$itmdsc = $item['itmdsc'];
			$itmobs = $item['itmobs'];
			$itmpermiteobs = $item['itmpermiteobs'];						
			$itmnotatecnica = $item['itmnotatecnica'];
			$itmglobal = $item['itmglobal'];
		}
		$sql = "SELECT tpe.tpeid as codigo, tpe.tpedsc as descricao FROM sig.tipoensinoitem tei
				LEFT JOIN sig.tipoensino tpe ON tpe.tpeid = tei.tpeid 
				WHERE tei.itmid = '". $_REQUEST['itmid'] ."'";
		$tpeid = $db->carregar($sql);
	case 'I':
	$titulo_modulo = "Sistema de Informações Gerenciais";
	monta_titulo( $titulo_modulo, 'Gerenciar itens' );
		
?>
<form id="formulario" name="formulario" method="post" action="?modulo=principal/cadastraritens&acao=<? echo $_REQUEST['acao']; ?>" onSubmit="selectAllOptions( document.getElementById('tpeid'));return validarFormularioCadastrarItens(this);">
<? echo (($_REQUEST['acao']== 'E')?"<input type='hidden' name='itmid' value='". $_REQUEST['itmid'] ."'>":""); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Tipo de ensino :</td>
		<td>
		<?
		$sqlComboSituacao = "SELECT	tpeid as codigo, tpedsc as descricao
						 	FROM sig.tipoensino";
		combo_popup('tpeid', $sqlComboSituacao, 'Orgão', "400x400", 0, array(), "", 'N', false, false, 5, 400 );
		?>
		</td>
	</tr>
	<tr>
			<td class="SubTituloDireita">Descrição do item :</td>
			<td>
			<? echo campo_texto('itmdsc', "N", "N", "Descrição do item", 67, 150, "", "", '', '', 0, '' ); ?>
			</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de item :</td>
		<td>
		<? 
		$sql = "SELECT tpiid AS codigo, tpidsc AS descricao FROM sig.tipoitem";
		$db->monta_combo('tpiid', $sql, 'N', "Selecione...", '', '', '', '100', 'N', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Permite observação ?</td>
		<td><input type="radio" name="itmpermiteobs" value="0" <? echo (($itmpermiteobs == "f" || $itmpermiteobs == "")?'checked':''); ?> disabled="disabled"> Não <input type="radio" name="itmpermiteobs" value="1" <? echo (($itmpermiteobs == "t")?'checked':''); ?> disabled="disabled"> Sim</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Faz parte da nota técnica ?</td>
		<td><input type="radio" name="itmnotatecnica" value="0" <? echo (($itmnotatecnica == "f" || $itmnotatecnica == "")?'checked':''); ?> disabled="disabled"> Não <input type="radio" name="itmnotatecnica" value="1" <? echo (($itmnotatecnica == "t")?'checked':''); ?> disabled="disabled"> Sim</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Item global ?</td>
		<td><input type="radio" name="itmglobal" value="0" <? echo (($itmglobal == "f" || $itmglobal == "")?'checked':''); ?> disabled="disabled"> Não <input type="radio" name="itmglobal" value="1" <? echo (($itmglobal == "t")?'checked':''); ?> disabled="disabled"> Sim</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Observação :</td>
		<td>
		<? echo campo_textarea( 'itmobs', 'N', 'N', '', '70', '4', '1000');	?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td>
			<div style="float: left;">
				<input type="hidden" name="alterabd" value="<? echo $_REQUEST['acao']; ?>" />
				<input type="submit" value="Salvar" style="cursor: pointer" disabled="disabled"> 
				<input type="button" value="Voltar" style="cursor: pointer" onclick="window.location='?modulo=principal/cadastraritens&acao=A'; this.disabled=true;">
			</div>
		</td>
	</tr>
</table>
</form>
<?
	break;
	case 'A':
		// 	Monta menu contendo os tipos de ensino
		$sql = "SELECT tpeid AS id, tpedsc AS descricao, '/sig/sig.php?modulo=principal/cadastraritens&acao=A&tpeid=' || tpeid AS link  FROM sig.tipoensino";
		$menutipoensino = $db->carregar($sql);
		if ($menutipoensino){
   			echo montarAbasArray($menutipoensino, (($_REQUEST['tpeid'])?false:'/sig/sig.php?modulo=principal/cadastraritens&acao=A&tpeid='.TIPOENSINO_DEFAULT));
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
		<input type="button" value="Cadastrar item" disabled="disabled">
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
ajaxatualizar("tpeid=<? echo (($_REQUEST['tpeid'])?$_REQUEST['tpeid']:TIPOENSINO_DEFAULT); ?>&exec_function=listaitens","listaitens");
function ordenaritens(itematual, itemir) {
	ajaxatualizar("tpeid=<? echo (($_REQUEST['tpeid'])?$_REQUEST['tpeid']:TIPOENSINO_DEFAULT); ?>&itematual="+itematual+"&itemir="+itemir+"&exec_function=ordenaritens");
	ajaxatualizar("tpeid=<? echo (($_REQUEST['tpeid'])?$_REQUEST['tpeid']:TIPOENSINO_DEFAULT); ?>&exec_function=listaitens","listaitens");
}
</script>