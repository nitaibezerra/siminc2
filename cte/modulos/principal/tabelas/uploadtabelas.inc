<?php

cte_verificaSessao();

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/arquivo.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo );

$tipos = array(
	'text/richtext',
	'text/plain',
	'application/msword',
	'application/wordperfect',
	'application/mswrite',
	'application/rtf',
	'application/x-wri',
	'application/x-world',
	'application/x-rtf',
	'application/pdf',
	'application/vnd.oasis.opendocument.text',
	'application/vnd.oasis.opendocument.text-web',
	'application/vnd.oasis.opendocument.spreadsheet',
	'application/vnd.sun.xml.writer',
	"application/excel",
	"application/vnd.ms-excel",
	"application/x-excel",
	"application/x-msexcel",
	"application/msword",
	"application/x-compressed",
	"application/x-zip-compressed",
	"application/zip",
	"multipart/x-zip",
	"application/x-gzip",
	"multipart/x-gzip",
	"application/x-bzip",
	"application/x-bzip2"
);

if ( $_REQUEST['tabid'] ) {
	$sql = sprintf( "select arqid, tabdsc from cte.tabelas where tabid = %d", $_REQUEST['tabid'] );
	$tabela = $db->pegaLinha( $sql );
	extract( $tabela );
}

if ( $_REQUEST['evento'] == 'excluir' ) {
	$sql = sprintf( "delete from cte.tabelas where tabid = %d", $_REQUEST['tabid'] );
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		$db->insucesso( "Ocorreu uma falha ao excluir o arquivo." );
	}
	$db->commit();
	$_REQUEST['acao'] = 'I';
	$db->sucesso( "principal/tabelas/uploadtabelas" ); 
}

if ( $_REQUEST['formulario'] ) {
	if ( $_REQUEST['tabid'] ) {
		if ( $_FILES['arquivo']['error'] != 4 ) {
			try {
				$arqid = salvarArquivo( $_SESSION['usucpf'], $_SESSION['sisid'], $_FILES['arquivo'], $tipos );
			} catch ( Exception $excecao ) {
				$db->rollback();
				$db->insucesso( $excecao->getMessage() );
			}
		}
		$sql = sprintf(
			"update cte.tabelas set tabdsc = '%s', arqid = %d where tabid = %d",
			$_REQUEST['tabdsc'],
			$arqid,
			$_REQUEST['tabid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			$db->insucesso( "Ocorreu uma falha ao gravar o arquivo." );
		}
		$db->commit();
		$_REQUEST['acao'] = 'I';
		$db->sucesso( "principal/tabelas/uploadtabelas" );
	} else {
		try {
			$arqid = salvarArquivo( $_SESSION['usucpf'], $_SESSION['sisid'], $_FILES['arquivo'], $tipos );
		} catch ( Exception $excecao ) {
			$db->rollback();
			$db->insucesso( $excecao->getMessage() );
		}
		$sql = sprintf(
			"insert into cte.tabelas ( inuid, arqid, tabdsc ) values ( %d, %d, '%s' )",
			$_SESSION['inuid'],
			$arqid,
			$_REQUEST['tabdsc']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			$db->insucesso( "Ocorreu uma falha ao gravar o arquivo." );
		}
		$db->commit();
		$_REQUEST['acao'] = 'I';
		$db->sucesso( "principal/tabelas/uploadtabelas" );
	}
}

?>
<form action="" method="post" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="formulario" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<colgroup>
			<col style="width: 25%;"/>
			<col style=""/>
		</colgroup>
		<tbody>
			<?php if( $_REQUEST['tabid'] ): ?>
			<tr>
				<td class="SubTituloDireita">&nbsp;</td>
				<td>
					<?php geraLinkParaArquivo( $arqid, true ); ?>
					<input type="hidden" name="tabid" value="<?= $_REQUEST['tabid'] ?>"/>
				</td>
			</tr>
			<?php endif; ?>
			<tr>
				<td class="SubTituloDireita">Arquivo:</td>
				<td><input type="file" name="arquivo"/></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descrição:</td>
				<td><?= campo_textarea( 'tabdsc', true, 'S', '', 70, 5, 500 ); ?></td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td>&nbsp;</td>
				<td>
					<input type='button' class="botao" name='cadastrar' value='Enviar' onclick="enviar();"/>
				</td>
			</tr>
		</tbody>
	</table>
</form>
<script type="text/javascript">
	
	function enviar(){
		if ( validar() ) {
			document.formulario.submit();
		}
	}
	
	function validar(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.formulario.tabdsc.value = trim( document.formulario.tabdsc.value );
		if ( document.formulario.tabdsc.value == '' ) {
			mensagem += '\nDescrição';
			validacao = false;
		}
		<?php if( !$_REQUEST['tabid'] ): ?>
		if ( document.formulario.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		<?php endif; ?>
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluir( tabid ){
		if ( confirm( 'Deseja excluir o arquivo?' ) ) {
			window.location = "?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir&tabid=" + tabid;
		}
	}
	
	function ltrim( value ){
		var re = "/\s*((\S+\s*)*)/";
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = "/((\s*\S+)*)\s*/";
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
	
</script>
<?php

$cabecalho = array( "Ação", "Nome do Arquivo", "Descrição", "Tamanho", "Incluido Por", "Data" );
$sql = "SELECT
	'<a href=\"cte.php?modulo=principal/tabelas/uploadtabelas&acao=I&tabid='|| tabelas.tabid ||'\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>'
	|| '&nbsp;<a href=\"javascript:excluir('|| tabelas.tabid ||');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>'
		as acao,
	arquivo.arqnome || '.' || arquivo.arqextensao as nome,
	tabelas.tabdsc,
	arquivo.arqtamanho || ' bytes' as tamanho,
	arquivo.usucpf,
	arquivo.arqdata || ' ' || arquivo.arqhora as data
	FROM cte.tabelas
	inner join public.arquivo ON cte.tabelas.arqid = public.arquivo.arqid
	where arqstatus='1' and tabelas.inuid=".$_SESSION['inuid'];
$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );

?>