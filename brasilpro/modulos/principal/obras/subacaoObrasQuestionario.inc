<?php 
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

//echo carregaAbasProInfancia("par.php?modulo=principal/subacaoObras&acao=A&tipoAba=questionario&preid=".$_REQUEST['preid'], $_REQUEST['preid'], $descricaoItem);

monta_titulo('QUESTIONÁRIO', 'Preencha o questionário');

$preid = $_REQUEST['preid'];
$qrpid = pegaQrpid( $preid, OBRAS_QUESTIONARIO );

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$sobanotacoes = $db->pegaUm('SELECT sobanotacoes FROM cte.subacaoobra WHERE preid = '.$preid);

$obSubacaoControle = new SubacaoControle();
$obPreObra = new PreObra();

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
}

if($preid){
	$arDados = $obSubacaoControle->recuperarPreObra($preid);
}

// nova situação, se o preobra for uma reformulação... desabilitar
if($arDados['preidpai']) {
	$habilitado = 'N';
	$travaCorrecao = true;
}

?>
<script language="JavaScript">
function salvaAnotacao(){
	document.getElementById('formJustificativa').submit();
}
</script>
<?php echo cabecalho();?>
<?php if($habilitado == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "É necessário o preenchimento completo e apresentação das informações complementares solicitadas no Relatório de Vistoria do Terreno disponibilizado no sistema.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">"; 
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
<?php endif; ?>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td class="SubTituloDireita" width="10%">
				Anotações Gerais:<br>
		</td>
		<td>
			<form id="formJustificativa" method="POST">
				<textarea name="sobanotacoes" class="text_editor_simple" cols="50" rows="6"><?=$sobanotacoes?></textarea>
				<input type="button" value="Salvar Anotações" onclick="salvaAnotacao()"/>
				<input type="hidden" value="salvaAnotacao" name="req"/>
				<input type="hidden" value="<?=$preid ?>" name="preid"/>
			</form>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $habilitado ) );
			?>
		</fieldset>
	</tr>
</table>