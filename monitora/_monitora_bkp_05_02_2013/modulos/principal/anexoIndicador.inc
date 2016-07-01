<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php

$idpid = $_REQUEST['idpid'];

if (empty($idpid)){
	die('<script>
			alert(\'Falha! Faltam parâmetros para acessar a tela.\');
			window.close();
			</script>');
}

switch ($_POST['op']){
	case 'salvar':
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos 	  = array(
				"idpid"  		=> $idpid,
				"idadescricao" 	=> "'" . $_POST['idadescricao'] . "'"
		);
		$file 		  = new FilesSimec("indicadoranexo", $arrCampos, "monitora");
		$file->setUpload(null, "arquivo");
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod='.$_REQUEST['prgcod'].'&idpid='.$_REQUEST['idpid'].'\';
			 </script>');
	case 'excluir':
		$arqid = $_POST['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec("indicadoranexo", array(''), "monitora");
			$file->setRemoveUpload($arqid);
			$msg = 'Operação realizada com sucesso!';
		}else{
			$msg = 'Falha na execução da operação!';
		}
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod='.$_REQUEST['prgcod'].'&idpid='.$_REQUEST['idpid'].'\';
			 </script>');
	case 'download':
		$arqid = $_POST['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec("indicadoranexo", array(''), "monitora");
			$file->getDownloadArquivo($arqid);
		}
		die('<script>
				location.href=\'monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod='.$_REQUEST['prgcod'].'&idpid='.$_REQUEST['idpid'].'\';
			 </script>');
}

$arAba = array(0 => array("id" => 1, "descricao" => "Indicadores", 	"link" => "/monitora/monitora.php?modulo=principal/telaprograma&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}"),
			   1 => array("id" => 2, "descricao" => "Anexos",    	"link" => "/monitora/monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}")
			);

echo montarAbasArray($arAba, "/monitora/monitora.php?modulo=principal/anexoIndicador&acao=A&requisicao=gerenciarIndicadores&atualizartodapaginapai=true&prgcod={$_REQUEST['prgcod']}&idpid={$_REQUEST['idpid']}");

$linha1 = 'Incluir Anexos'; 
$linha2 = 'Indicador';
monta_titulo($linha1, $linha2);
?>
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
			<td><?=campo_textarea( 'idadescricao', 'N', 'S', '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button" name="botao" value="Salvar" onclick="validaFormArquivo();" <?=($boBloqueioAba ? "disabled=\"disabled\"" : "" ) ?>/>
			</td>
		</tr>
	</table>
</form>
<?php 

$sql = "
	select 
		'
		<img src=\"../imagens/clipe.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"downloadArquivo(' || aq.arqid || ')\" />
		<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"excluirArquivo(' || aq.arqid || ')\" />
		' as acao,
		idadescricao,
		arqnome,
		to_char(arqdata, 'DD/MM/YYYY') as data,
		usunome
	from 
		monitora.indicadoranexo ia
	inner join 
		public.arquivo aq on ia.arqid = aq.arqid
	inner join 
		seguranca.usuario us on us.usucpf = aq.usucpf
	where
		idpid = {$idpid}  
";

$cabecalho = array("Ação", "Descrição", "Arquivo", "Data Inclusão", "Cadastrado por");
$db->monta_lista($sql, $cabecalho, 10, 10, '', '', '', '');

?>