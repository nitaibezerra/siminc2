<?php

if( $_POST['requisicao'] == 'salvar' ){
	
	if( is_array($_POST['fluxo']) ){
		$db->executar("DELETE FROM emenda.fluxoreformulacao WHERE trefid = {$_POST['trefid']} and esdid = {$_POST['esdid']}");
		foreach ($_POST['fluxo'] as $fluxo) {
			$sql = "INSERT INTO emenda.fluxoreformulacao(trefid, esdid, aedid) 
					VALUES ({$_POST['trefid']}, {$_POST['esdid']}, $fluxo)";
			
			$db->executar($sql);
		}
		$db->commit();
	}
	$db->sucesso( 'sistema/tabelasapoio/fluxoReformulacao', '' );
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( 'Gerenciar Fluxo de Reformulação', '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<style>
.button{
	width: 120px; 
	height: 25px;
}
</style>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
		<tr>
			<td class="subtitulodireita" width="25%"><b>Reformulação:</b></td>
			<td><?
			$sql = "select 
						trefid as codigo,
						trefnome as descricao
					from emenda.tiporeformulacao 
					where trefstatus = 'A'
					order by trefnome";
			
			$trefid = $_REQUEST['trefid'];
			echo $db->monta_combo('trefid', $sql, 'S', 'Selecione...', 'carregarFluxo', '', '', 350, '', 'trefid', false ); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="25%"><b>Fluxo:</b></td>
			<td><?
			$sql = "select 
						e.esdid as codigo,
					    e.esddsc as descricao
					from 
						workflow.estadodocumento e
					where 
						e.tpdid = 8
					    and e.esdstatus = 'A'
					order by
						e.esdordem";
			
			$esdid = $_REQUEST['esdid'];
			echo $db->monta_combo('esdid', $sql, 'S', 'Selecione...', 'carregarFluxo', '', '', 350, '', 'esdid', false ); ?></td>
		</tr>
	<?if( $esdid && $trefid ){ ?>
		<tr>
			<td class="subtitulodireita" width="25%"><b>Fluxo de Workflow:</b></td>
			<td>
			<!--  <input type="checkbox" name="menu[]" id="menu" value=""> -->
			<?
			print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';			
			print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Selecione os fluxos que não deverão aparecer</b></td></tr></table>';
			
			$sql = "select distinct
						case when (select count(*) from emenda.fluxoreformulacao where esdid = $esdid and aedid = e.aedid and trefid = $trefid) > 0 then
					    	'<center><input type=\"checkbox\" checked=\"checked\" name=\"fluxo[]\" id=\"fluxo\" value=\"'||e.aedid||'\"></center>'
					    else 
					    	'<center><input type=\"checkbox\" name=\"fluxo[]\" id=\"fluxo\" value=\"'||e.aedid||'\"></center>'
					    end  as acoes,
						e.aedid as codigo,
					    e.aeddscrealizar as descricao
					from 
						workflow.acaoestadodoc e
					where 
						e.esdidorigem = $esdid
					    and e.aedstatus = 'A'
					    and e.aedvisivel = 't'";
			
			$cabecalho = array( 'Ação', "Codigo", "Descrição" );
			$db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%', '', '', '', '', true);
	 	?></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="3" align="center">
				<input type="button" value="Salvar" class="button" name="btnSalvar" id="btnSalvar" onclick="salvaAbas();" style="cursor: pointer;">
			</td>
		</tr>
	<?} ?>
	</table>
</form>
<script type="text/javascript">

function carregarFluxo(valor){
	if( $('[name="trefid"]').val() != '' && $('[name="esdid"]').val() != '' ){
		$('#formulario').submit();
	}
}

function salvaAbas(){

	if($('[name="trefid"]').val() == ''){
		alert('Informe a Reformulação');
		$('[name="trefid"]').focus();
		return false;
	}
	if($('[name="esdid"]').val() == ''){
		alert('Informe o Fluxo');
		$('[name="esdid"]').focus();
		return false;
	}
	if( $('[name="fluxo[]"]:checked').length == '0'){
		alert('Selecione um Fluxo.');
		return false;
	}
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}
</script>