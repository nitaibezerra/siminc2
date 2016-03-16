<?php
$eveid = $_SESSION['agenda']['eveid'];

if (empty($eveid)){
		die('<script>
				alert(\'Falha! Faltam parâmetros para acessar a tela.\');
				location.href=\'?modulo=principal/anexoEvento&acao=A\';
			 </script>');
}

switch ($_POST['op']){
	case 'salvar': 
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos 	  = array(
								"eveid"  => $eveid,
								"taedsc" => "'" . $_POST['taedsc'] . "'"
							  );
		$file 		  = new FilesSimec("arquivoevento", $arrCampos, "agenda");
		$file->setUpload(null, "arquivo");
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/anexoEvento&acao=A\';
			 </script>');
	case 'excluir': 
		$arqid = $_POST['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec("arquivoevento", array(''), "agenda");
			$file->setRemoveUpload($arqid);
			$msg = 'Operação realizada com sucesso!';
		}else{
			$msg = 'Falha na execução da operação!';
		}
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=principal/anexoEvento&acao=A\';
			 </script>');
	case 'download': 
		$arqid = $_POST['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec("arquivoevento", array(''), "agenda");
			$file->getDownloadArquivo($arqid);
		}
		die('<script>
				location.href=\'?modulo=principal/anexoEvento&acao=A\';
			 </script>');
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
//if ( $_REQUEST['acao'] == 'E' ){
//}
$arAba = abaEdicaoEvento();
echo montarAbasArray($arAba, "?modulo=principal/anexoEvento&acao=A");
monta_titulo( 'Anexos do Evento', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validaFormArquivo(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=arquivo]').val() ) == '' ){
		errorText.push('Arquivo');
	}
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('[name=op]').val('salvar');
	$('[name=anexoEvento]').submit();
}

function excluirArquivo(arqid){
	if ( confirm('Deseja apagar esse arquivo?') ){
		$('[name=op]').val('excluir');
		$('[name=arqid]').val( arqid );
		$('[name=anexoEvento]').submit();
	}
}

function downloadArquivo(arqid){
	$('[name=op]').val('download');
	$('[name=arqid]').val( arqid );
	$('[name=anexoEvento]').submit();
}

//-->
</script>
<form method="post" id="anexoEvento" name="anexoEvento" enctype="multipart/form-data" action="">
	<input type="hidden" name="op" value=""/>
	<input type="hidden" name="arqid" value=""/>
	<?php
 if ( $_SESSION['agenda']['eveid'] ){
	 
	$evento = new Evento();
	$dados = $evento->carregaTodosDados( $_SESSION['agenda']['eveid']);
	
	?>
		<table class="tabela" align="center">
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Código</td>
				<td> <?php echo $dados['eveid']; ?></td>
			</tr>
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Evento</td>
				<td> <?php echo $dados['tpedsc']; ?></td>
			</tr>
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Data de Início</td>
				<td> <?php echo $dados['evedtinicio']; ?></td>
			</tr>
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Solicitante</td>
				<td> <?php echo $dados['sltdsc']; ?></td>
			</tr>
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Entidade</td>
				<td> <?php echo $dados['etadsc']; ?></td>
			</tr>
			<tr>
				<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Assunto</td>
				<td> <?php echo $dados['eveassunto']; ?></td>
			</tr>
		</table>
	<?php } 
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<input type="file" name="arquivo"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?=campo_textarea( 'taedsc', 'N', 'S', '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button" name="botao" value="Salvar" onclick="validaFormArquivo();" <?=($boBloqueioAba ? "disabled=\"disabled\"" : "" ) ?>/>
			</td>
		</tr>
	</table>
</form>
<table class="tabela" align="center">
	<tr>
		<td colspan="2" height="270" style="background-color: #F0F0F0;">
			<fieldset style="height:270px; background-color: #FFFFFF;">
				<legend>Anexos Cadastrados</legend>
				<div style="height:260px; overflow:auto;">
				<?php
				$arquivo = new ArquivoEvento();
				$sql 	= $arquivo->listaSQL( array('eveid' => $eveid) );
				
				$cabecalho = array("Ação", "Descrição", "Arquivo", "Data Inclusão", "Cadastrado por");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
				?>
				</div>
			</fieldset>
		</td>
	</tr>
</table>


