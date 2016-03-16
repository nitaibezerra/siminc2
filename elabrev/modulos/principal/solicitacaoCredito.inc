<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}
$abas = menuAbasLiberacao();
echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);

$sql = "SELECT socid, lbrid, socvalorcontratacao, socdatinilicitacao, socdatfimlicitacao
		FROM 
		  elabrev.solicitacaocredito WHERE lbrid = {$_SESSION['elabrev_var']['lbrid']} AND socstatus = 'A'";
$arrRegistro = $db->pegaLinha($sql);
$arrRegistro = $arrRegistro ? $arrRegistro : array();
extract($arrRegistro);

$socvalorcontratacao = $socvalorcontratacao ? number_format($socvalorcontratacao, '2', ',', '.') : ''; 
$socdatinilicitacao = $socdatinilicitacao ? formata_data($socdatinilicitacao) : '';
$socdatfimlicitacao = $socdatfimlicitacao ? formata_data($socdatfimlicitacao) : '';

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td style="width: 95%">
		<form method='post' name='formulario' id="formulario">
			<input type='hidden' name='requisicao' value='gravarSolicitacaoCredito'>
			<input type='hidden' name='socid' value='<?=$socid; ?>'>
		
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita">Valor da Contratação:</td>
					<td><?=campo_texto('socvalorcontratacao', 'N', 'S', 'Valor da Contratação ', 15, 13, '[###.]###,##', '','','','','id="socvalorcontratacao"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value); valorExercicioProximo();"); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Data Início da Licitação:</td>
					<td><?=campo_data2('socdatinilicitacao', 'N','S','Data Início da Licitação','','',''); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Data Final da Licitação:</td>
					<td><?=campo_data2('socdatfimlicitacao', 'N','S','Data Final da Licitação','','',''); ?></td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td></td>
					<td>
						<input type="button" value="Salvar" name="btnSalvar" <?=disabled($estadoAtual); ?> id="btnSalvar" onclick="gravarSolicitacaoCredito();" style="cursor: pointer;">
						<input type="button" value="Limpar Campos" name="btnLimpar" id="btnLimpar" onclick="window.location.href = 'elabrev.php?modulo=principal/solicitacaoCredito&acao=A';" style="cursor: pointer;">
					</td>
				</tr>
			</table>
		</form>
	</td>
	<td valign="top"><?
		$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
		wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );
	?></td>
</tr>
</table>
<script>
function gravarSolicitacaoCredito(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "socvalorcontratacao";
	campos[1] 			= "socdatinilicitacao";
	campos[2] 			= "socdatfimlicitacao";
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "texto";
	tiposDeCampos[2] 	= "texto";
	
	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
	
		if(!validaData($('socdatinilicitacao') ) ) {
			alert('Data Início da Licitação está no formato incorreto.');
			$('socdatinilicitacao').focus();
			return false;
		}
		else if(!validaData($('socdatfimlicitacao') ) ) {
			alert('Data Final da Licitação está no formato incorreto.');
			$('socdatfimlicitacao').focus();
			return false;
		}
		else if( !validaDataMaior( $('socdatinilicitacao'), $('socdatfimlicitacao') ) ){
			alert("A data inicial não pode ser maior que data final.");
				$('socdatinilicitacao').focus();
			return false;
		}
		$('formulario').submit();
	}
}
</script>