<?php

if( $_REQUEST['submetido'] )
{
	if( $_REQUEST['tpoid'] )
	{
		$tpoid = $_REQUEST['tpoid'];
		$sql = "UPDATE obras.tipologiaobra SET tpodsc = '".pg_escape_string($_REQUEST['tpodsc'])."', tpodetalhe = '".pg_escape_string($_REQUEST['tpodetalhe'])."', orgid = ".$_REQUEST['orgid']." WHERE tpoid = ".$tpoid;
		$db->executar($sql);
	}
	else
	{
		$sql = "INSERT INTO obras.tipologiaobra(tpodsc, tpodetalhe, tpostatus, orgid) VALUES('".pg_escape_string($_REQUEST['tpodsc'])."', '".pg_escape_string($_REQUEST['tpodetalhe'])."', 'A', ".$_REQUEST['orgid'].") RETURNING tpoid";
		$tpoid = $db->pegaUm($sql);
	}
	
	$db->executar("DELETE FROM obras.tipologiamodalidade WHERE tpoid = ".$tpoid);
	for($i=0; $i<count($_REQUEST["moeid"]); $i++)
	{
		$sql = "INSERT INTO obras.tipologiamodalidade(tpoid,moeid) VALUES(".$tpoid.", ".$_REQUEST["moeid"][$i].")";
		$db->executar($sql);
	}
	
	$db->executar("DELETE FROM obras.tipologiatipoobra WHERE tpoid = ".$tpoid);
	for($i=0; $i<count($_REQUEST["tobaid"]); $i++)
	{
		$sql = "INSERT INTO obras.tipologiatipoobra(tpoid,tobaid) VALUES(".$tpoid.", ".$_REQUEST["tobaid"][$i].")";
		$db->executar($sql);
	}
	
	$db->executar("DELETE FROM obras.tipologiaorigem WHERE tpoid = ".$tpoid);
	for($i=0; $i<count($_REQUEST["tooid"]); $i++)
	{
		$sql = "INSERT INTO obras.tipologiaorigem(tpoid,tooid) VALUES(".$tpoid.", ".$_REQUEST["tooid"][$i].")";
		$db->executar($sql);
	}
	
	$db->executar("DELETE FROM obras.tipologiaprograma WHERE tpoid = ".$tpoid);
	for($i=0; $i<count($_REQUEST["prfid"]); $i++)
	{
		$sql = "INSERT INTO obras.tipologiaprograma(tpoid,prfid) VALUES(".$tpoid.", ".$_REQUEST["prfid"][$i].")";
		$db->executar($sql);
	}
	
	$db->commit();
	echo '<script>
			alert("Dados gravados com sucesso");
			window.location.href = "obras.php?modulo=sistema/geral/listaTipologiaObra&acao=A";
		  </script>';
	die;
}

header('Content-type: text/html; charset="iso-8859-1"',true);
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Cadastro de Tipologia da Obra' );

extract( $_REQUEST );

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	onOffCampo("tooid");
	onOffCampo("prfid");
	onOffCampo("moeid");
	onOffCampo("tobaid");
	
	$('#btSalvar').click(function()
	{
		if( $('#tpodsc').val() == '' )
		{
			alert("O campo 'Descrição' deve ser preenchido");
			$('#tpodsc').focus();
			return;
		}
		if( $('#orgid').val() == '' )
		{
			alert("O campo 'Tipo de Ensino' deve ser preenchido");
			$('#orgid').focus();
			return;
		}
		if( $('#tpodetalhe').val() == '' )
		{
			alert("O campo 'Detalhe' deve ser preenchido");
			$('#tpodetalhe').focus();
			return;
		}
		
		if( document.getElementById( 'prfid' ).length == 0 || (document.getElementById( 'prfid' ).length == 1 && document.getElementById( 'prfid' )[0].value == '') )
		{
			alert("O campo 'Programa' deve ser preenchido");
			$('#prfid').focus();
			return;
		}
		if( document.getElementById( 'moeid' ).length == 0 || (document.getElementById( 'moeid' ).length == 1 && document.getElementById( 'moeid' )[0].value == '') )
		{
			alert("O campo 'Modalidade de Ensino' deve ser preenchido");
			$('#moeid').focus();
			return;
		}
		if( document.getElementById( 'tobaid' ).length == 0 || (document.getElementById( 'tobaid' ).length == 1 && document.getElementById( 'tobaid' )[0].value == '') )
		{
			alert("O campo 'Tipo de Obra' deve ser preenchido");
			$('#tobaid').focus();
			return;
		}
		if( document.getElementById( 'tooid' ).length == 0 || (document.getElementById( 'tooid' ).length == 1 && document.getElementById( 'tooid' )[0].value == '') )
		{
			alert("O campo 'Fonte' deve ser preenchido!");
			$('#tooid').focus();
			return;
		}
		
		selectAllOptions( document.getElementById( 'prfid' ) );
		selectAllOptions( document.getElementById( 'moeid' ) );
		selectAllOptions( document.getElementById( 'tobaid' ) );
		selectAllOptions( document.getElementById( 'tooid' ) );
		
		$('#formRelatorio').submit();
	});
});

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function carregaOrgao(orgid){
	document.getElementById( 'submetido' ).value = '';
	$('#formRelatorio').submit();
}

</script>

<form id="formRelatorio" method="post" action="">
	<input type="hidden" value="1" name="submetido" id="submetido" />
	<input type="hidden" value="<?=$_GET['tpoid']?>" name="tpoid" /> 
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<!-- Descrição -->
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Descrição:</td>
			<td>
				<?php 
				if($_GET['tpoid'])
				{
					$tpodsc = $db->pegaUm("SELECT tpodsc FROM obras.tipologiaobra WHERE tpoid = ".$_GET['tpoid']);
				}
				?>
				<?=campo_texto('tpodsc', 'S', 'S', '', '63', '50', '', 'N', 'left', '', '', 'id="tpodsc"', '', null, '' )?>
			</td>
		</tr>
		<!-- Tipo de Ensino -->
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Tipo de Ensino:</td>
			<td>
				<?php 
				if($_GET['tpoid'])
				{
					$orgid = $db->pegaUm("SELECT orgid FROM obras.tipologiaobra WHERE tpoid = ".$_GET['tpoid']);
				}
				$sql = "SELECT orgid as codigo, orgdesc as descricao FROM obras.orgao WHERE orgstatus = 'A'";
				?>
				<?=$db->monta_combo("orgid", $sql, 'S', "Selecione...", 'carregaOrgao', '', '', '', 'S', 'orgid', '', $orgid);?>
			</td>
		</tr>
		<!-- Detalhe -->
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Detalhe:</td>
			<td>
				<?php 
				if($_GET['tpoid'])
				{
					$tpodetalhe = $db->pegaUm("SELECT tpodetalhe FROM obras.tipologiaobra WHERE tpoid = ".$_GET['tpoid']);
				}
				?>
				<?=campo_textarea("tpodetalhe", "S", "S", "", '67', '9', '10000')?>
			</td>
		</tr>
		<!-- Programa -->
		<?php
		if( $orgid ){
			$filtro = ' and orgid = '.$orgid;
		}
		
		$sql = "SELECT DISTINCT
					prfid AS codigo,
					prfdesc AS descricao
				FROM 
					obras.programafonte
				WHERE
					1=1
					$filtro
				ORDER BY
					prfdesc ASC";
		
		if($_GET['tpoid'])
		{
			$sqlCarregados 	= "SELECT DISTINCT
									pf.prfid AS codigo,
									pf.prfdesc AS descricao
								FROM
									obras.tipologiaprograma tp
								INNER JOIN
									obras.programafonte pf ON pf.prfid = tp.prfid
								WHERE
									tp.tpoid = ".$_GET['tpoid'];
		}
		
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Programa:', 'prfid', $sql, $sqlCarregados, 'Selecione o(s) Programa(s)', null, null, false, null, $arrVisivel, $arrOrdem, true, false);
		?>
		
		<?php if($_GET['tpoid']): ?>
		<script>onOffCampo("prfid");</script>
		<?php endif; ?>
		
		<!-- Modalidade de Ensino -->
		<?php
		$sql = "SELECT DISTINCT
					moeid AS codigo,
					moedsc AS descricao
				FROM 
					obras.modalidadeensino
				WHERE
					moestatus = 'A'
				ORDER BY
					moedsc ASC";
		
		if($_GET['tpoid'])
		{
			$sqlCarregados 	= "SELECT DISTINCT
									me.moeid AS codigo,
									me.moedsc AS descricao
								FROM
									obras.tipologiamodalidade tm
								INNER JOIN
									obras.modalidadeensino me ON me.moeid = tm.moeid
								WHERE
									tm.tpoid = ".$_GET['tpoid'];
		}
		
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Modalidade de Ensino:', 'moeid', $sql, $sqlCarregados, 'Selecione a(s) Modalidade(s) de Ensino', null, null, false, null, $arrVisivel, $arrOrdem, true, false);
		?>
		
		<?php if($_GET['tpoid']): ?>
		<script>onOffCampo("moeid");</script>
		<?php endif; ?>
		
		<!-- Tipo de Obra -->
		<?php
		$sql = "SELECT DISTINCT
					tobaid AS codigo,
					tobadesc AS descricao
				FROM 
					obras.tipoobra
				WHERE
					tobstatus = 'A'
				ORDER BY
					tobadesc ASC";
		
		if($_GET['tpoid'])
		{
			$sqlCarregados 	= "SELECT DISTINCT
									tob.tobaid AS codigo,
									tob.tobadesc AS descricao
								FROM
									obras.tipologiatipoobra tt
								INNER JOIN
									obras.tipoobra tob ON tob.tobaid = tt.tobaid
								WHERE
									tt.tpoid = ".$_GET['tpoid'];
		}
		
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Tipo de Obra:', 'tobaid', $sql, $sqlCarregados, 'Selecione o(s) Tipo(s) de Obra', null, null, false, null, $arrVisivel, $arrOrdem, true, false);
		?>
		
		<?php if($_GET['tpoid']): ?>
		<script>onOffCampo("tobaid");</script>
		<?php endif; ?>
		
		<!-- Origem -->
		<?php
		$sql = "SELECT DISTINCT
					tooid AS codigo,
					toodescricao AS descricao
				FROM 
					obras.tipoorigemobra
				WHERE
					toostatus = 'A'
				ORDER BY
					toodescricao ASC";
		
		if($_GET['tpoid'])
		{
			$sqlCarregados 	= "SELECT DISTINCT
									too.tooid AS codigo,
									too.toodescricao AS descricao
								FROM
									obras.tipologiaorigem tor
								INNER JOIN
									obras.tipoorigemobra too ON too.tooid = tor.tooid
								WHERE
									tor.tpoid = ".$_GET['tpoid'];
		}
		
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Fonte:', 'tooid', $sql, $sqlCarregados, 'Selecione a(s) Origem(ns)', null, null, false, null, $arrVisivel, $arrOrdem, true, false);
		?>
		
		<?php if($_GET['tpoid']): ?>
		<script>onOffCampo("tooid");</script>
		<?php endif; ?>
		
		<tr>
			<td class="SubTituloDireita" style="width:250px;"></td>
			<td>
				<input type="button" value="Salvar" id="btSalvar" />
				<input type="button" value="Voltar" onclick="window.location.href='obras.php?modulo=sistema/geral/listaTipologiaObra&acao=A';" />
			</td>
		</tr>
	</table>
</form>