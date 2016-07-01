<?
$codicao = "";
if($_REQUEST['coidsc'] && $_POST['formulario'] == 2){
	$coidsc = $_REQUEST['coidsc'];
	$coidscbusca =  str_replace(" ","%",$coidsc);
	$codicao = " AND i.coicodsiasg || i.coidsc ilike '%".$coidscbusca."%' ";
}
if($_POST['formulario'] == 1 && is_array($_POST['itensSelecionados'])){
	$copid = $_SESSION['copid'];
	
	$arCoiidTemp = array();
	
	$sql = "select distinct ip.coiid from evento.codemandaitem di
				inner join evento.coitemprocesso ip on di.cotid = ip.cotid 
			where ip.copid = $copid";
	$arCoiidTemp = $db->carregar($sql);
	$arCoiid = array();
	if(is_array($arCoiidTemp)){
		foreach($arCoiidTemp as $coiid ){
			$arCoiid[] = $coiid['coiid'];
		}
	}
	
	if(count($arCoiid)){
		$sql = "DELETE FROM evento.coitemprocesso WHERE copid = {$copid} and coiid not in (". implode(",", $arCoiid).")";		
	} else {
		$sql = "DELETE FROM evento.coitemprocesso WHERE copid = {$copid}";	
	}
	$db->executar($sql);
	
	if($_POST['itensSelecionados'][0] > 0){
		foreach($_POST['itensSelecionados'] as $coiid){
			if(!in_array($coiid,$arCoiid)){
				$sql = "insert into evento.coitemprocesso (copid, coiid)
						   				       values ($copid, $coiid)";
				$db->executar($sql);
			}
		}
	}
	$db->commit();
	echo "<script>window.parent.opener.location.reload();self.close();</script>";
	die;
}

$sql = "SELECT
 			'<input type=\"checkbox\" value=\"'|| i.coiid ||'\" name=\"coiid_'|| i.coiid ||'\" id=\"coiid_'|| i.coiid ||'\"  onclick=\"retorna(this);\" >
 			<input type=\"hidden\" name=\"coidsc_'|| i.coiid ||'\" id=\"coidsc_'|| i.coiid ||'\" 
 			value=\"'|| 
			CASE WHEN i.coicodsiasg is null 
			  THEN '' 
			  ELSE i.coicodsiasg || ' - ' 
			END || i.coidsc ||'\">' as acao,
			i.coicodsiasg, i.coidsc,
			um.umedescricao, 
			coalesce( i.coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
			coalesce( i.coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax
	    FROM evento.coitem i
			LEFT JOIN evento.unidademedida um on i.umeid = um.umeid
			WHERE i.coistatus = 'A' 
			$codicao 
			and coiiditempai is null";
?>
<script src="../includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<form action="<?php echo $_SERVER['REQUEST_URI']?>" method="POST" name="formularioItem" id="formularioItem">
<input type="hidden" id="formulario" name="formulario" value="1" />
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
				<div style="float: left;">
					<table border="0" cellpadding="3" cellspacing="0">
						<tr>
							<td valign="bottom">
								Descrição do Item:
								<br/> 
								<?= campo_texto( 'coidsc', 'N', 'S', '', 50, 200, '', '', '', '', '', 'id="coidsc"' ); ?>
							</td>
						</tr>
					</table>
					<div style="float: left;">
					<br>
						<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
					</div>	
				</div>	
			</td>
		</tr>
	</tbody>
</table>
<div >
<select style="display: none;" multiple size="8" name="itensSelecionados[]" id="itensSelecionados" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?php
$sqlVinculados = "SELECT
DISTINCT
 			i.coiid,
			i.coicodsiasg, 
			i.coidsc
	    FROM evento.coitem i
			 
			INNER JOIN evento.coitemprocesso ip  on i.coiid = ip.coiid
			WHERE i.coistatus = 'A' and ip.copid = {$_SESSION['copid']} 
			and coiiditempai is null";

$arItensVinculados = @$db->carregar($sqlVinculados);
if(is_array($arItensVinculados)) {
	foreach ($arItensVinculados as $itensVinculados){
		echo " <option value=\"{$itensVinculados['coiid']}\">{$itensVinculados['coicodsiasg']} - {$itensVinculados['coidsc']}</option>";
	}	
} else {?>
<option value="">Clique nos Itens</option>
<?
}
?>
</select>
 
</div>
</form>
<?php
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Cód. SIASG", "Descrição", "Unidade de Medida", "Valor Máx.", "Valor Min." ); 
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', 'formulario');
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formularioItem.submit();" id="ok">
		</td>
	</tr>
</table>
<script type="text/javascript"><!--
function validaForm()
{ 
  	if(document.getElementById('coidsc').value == ''){
  		alert('Digite algum critério para pesquisar.');
  		document.getElementById('coidsc').focus();
  		return false;
  	}
	document.getElementById('formulario').value = 2;	
  	document.formularioItem.submit(); 
}

var campoSelect = document.getElementById("itensSelecionados");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		var coiid = campoSelect.options[i].value;
		if( document.getElementById('coiid_'+coiid)){
			document.getElementById('coiid_'+coiid).checked = true;
		}	
	} 
}

function retorna(objeto)
{
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	
	var descricao = document.getElementById('coidsc_'+objeto.value).value;
	
	if (objeto.checked == true){
		campoSelect.options[tamanho] = new Option(descricao, objeto.value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (objeto.value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Item.', '', false, false);}
			sortSelect(campoSelect);
	}
}
</script>