<?php
$arOrgid = verificaAcessoEmOrgid();
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

//$_SESSION['obras2']['orgid'] = $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
//$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : ORGID_EDUCACAO_SUPERIOR);
//$orgid 						 = $_SESSION['obras2']['orgid'];

// Limpa-se o obrid, pois na edição da licitação, caso haja obrid subentende-se que provém da edição da licitação pela OBRA.
// Logo retornará para obra após ser salvo. 
unset( $_SESSION['obras2']['obrid'] );

switch ( $_POST['op'] ){
	case 'apagar':
		if ( $_POST['crtid'] ){
			$contrato = new Contrato( $_POST['crtid'] );
			$contrato->crtstatus = 'I';
			$contrato->salvar();
			
			$obraContrato = new ObrasContrato();
			$obraContrato->apagaByContrato( $_POST['crtid'] );
			
			$db->commit();
		}
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/listaContrato&acao=A\';
			 </script>');
}

extract( $_POST );

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listacontrato');
echo montarAbasArray($arAba, "?modulo=principal/listaContrato&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Contratos', '');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
function alterarCrt( crtid ){
	window.location = '?modulo=principal/cadContrato&acao=E&crtid=' + crtid;
}

function excluirCrt( crtid ){
	if ( confirm('Deseja apagar este contrato?') ){
		var form = $('<form>').attr('method', 'post')
							  .attr('action', '')
							  .append( 
							   			$('<input>').attr('name', 'crtid')
							   						.attr('type', 'hidden')
							   						.attr('value', crtid)
							   		   )
							  .append( 
							   			$('<input>').attr('name', 'op')
							   						.attr('type', 'hidden')
							   						.attr('value', 'apagar')
							   		   )
		$(document.body).append( form );
		form.submit();

	}			   
}
</script>
<form method="post" name="formPesqContrato" id="formPesqContrato">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Número da Licitação:</td>
			<td>
				<?=campo_texto('licnumero','N','S','',20,10,'','', '', '', '', 'id="licnumero"');?>
			</td>
		</tr>
		<tr>
			<td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
			<td>
				<?php 
					$modalidade = new ModalidadeLicitacao();
					$db->monta_combo('molid', $modalidade->listaCombo(), 'S', "Selecione...", '', '', '', '', 'N', 'molid'); 
				?>
			</td>
		</tr>
		<tr>
			<td width="265" class="subtitulodireita">Contrato Aditivado:</td>
			<td>
				<input type="radio" name="crtaditivado" id="crtaditivo_sim" value="S" <?=($_POST['crtaditivado'] == 'S' ? 'checked="checked"' : '') ?>><label for="crtaditivo_sim"> Sim</label>
				&nbsp;&nbsp;
				<input type="radio" name="crtaditivado" id="crtaditivo_nao" value="N" <?=($_POST['crtaditivado'] == 'N' ? 'checked="checked"' : '') ?>><label for="crtaditivo_nao"> Não</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Assinatura:</td>
			<td>
			<?=campo_data2( 'crtdtassinatura_ini', 'N', 'S', '', 'N' ); ?>
			&nbsp;
			Até
			&nbsp;
			<?=campo_data2( 'crtdtassinatura_fim', 'N', 'S', '', 'N' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Término:</td>
			<td>
			<?=campo_data2( 'crtdttermino_ini', 'N', 'S', '', 'N' ); ?>
			&nbsp;
			Até
			&nbsp;
			<?=campo_data2( 'crtdttermino_fim', 'N', 'S', '', 'N' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor do Contrato:</td>
			<td>
			<?=campo_texto('crtvalorexecucao_ini', 'N', 'S', '', 11, 18, '[###.]###,##', '', 'right', '', 0, 'id="crtvalorexecucao_ini"','', $crtvalorexecucao_ini, '' ); ?>
			&nbsp;
			Até
			&nbsp;
			<?=campo_texto('crtvalorexecucao_fim', 'N', 'S', '', 11, 18, '[###.]###,##', '', 'right', '', 0, 'id="crtvalorexecucao_fim"','', $crtvalorexecucao_fim, '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloEsquerda">
				<input type="submit" value="Pesquisar">
				<input type="button" value="Limpar Formulário" onclick="$('#formPesqContrato').find('input[type=text], textarea, select').val(''); $('#formPesqContrato').find('input[type=radio]').removeAttr('checked');">
			</td>
		</tr>
		<tr>
			<td>
				<a title="Adicionar Novo Contrato" href="?modulo=principal/cadContrato&acao=A">
				<img src="/imagens/gif_inclui.gif" border="0"> Novo Contrato
				</a>
			</td>
		</tr>
	</table>
</form>
<?php
$contrato 		 		 = new Contrato();
$filtro   		 		 = $_POST;
$filtro['orgid'] 		 = $orgid;
$filtro['not(crtidpai)'] = true;
$sql      		 = $contrato->listaSql( $filtro );

$cabecalho = array("Ação", "Licitação","Empresa Contratada", "Aditivado", "Data de Assinatura", "Data de Término", "Valor do Contrato");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>