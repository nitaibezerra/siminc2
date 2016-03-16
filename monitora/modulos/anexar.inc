<?php
//
// $Id$
//
$_SESSION['MSG_AVISO'] = array();
/**
 * Redireciona o navegador para a tela indicada.
 * 
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() ) {
	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

switch( $_REQUEST['evento'] ){

	case 'cadastrar_anexo':

		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) 
			$_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao enviar o arquivo, favor tentar novamente.";

		//caminho (nome do arquivo com data/hora.para nao sobrescrever.)
		$caminho = APPRAIZ . 'arquivos/monitora/'.date("dmY-His").$arquivo['name'];
		
		if( !is_dir( APPRAIZ.'arquivos' ) ) mkdir(APPRAIZ.'arquivos/', 0777);
		if( !is_dir( APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'] ) ) mkdir(APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'], 0777);
		//if( !is_dir( $caminho ) ) mkdir( $caminho, 0777);

		if(move_uploaded_file($arquivo['tmp_name'], $caminho))
		{
			$sql = sprintf(
				"INSERT INTO monitora.documentos(
								  taaid,
								  usucpf,
								  doctamanho,
								  docnome,
								  docdescricao,
								  doccaminho,
								  docmimetype
								) values (
								   %d, 
								  '%s',
								   %d,
								  '%s', 
								  '%s',
								  '%s',
								  '%s'
								) returning docid",
				$_REQUEST['taaid'],
				$_SESSION['usucpf'],
				$arquivo['size'],
				$arquivo['name'],
				substr( $_REQUEST['docdescricao'], 0, 250 ),
				basename($caminho),
				$arquivo['type']
			);
			
			if( $db->executar($sql) ){
				
				$_SESSION['MSG_AVISO'][] = "Documento anexado com êxito!";
				$db->commit();

			} else {
				$_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao cadastrar o arquivo no banco dedados.<br>Verifique as informações e tente novamente.";
				$db->rollback();
			}
		}
		else
		{
			$_SESSION['MSG_AVISO'][] = "Ocorreu um erro ao salvar o arquivo no servidor, favor tentar novamente.";
		}
		
		break;

	case 'excluir_anexo':
		$sql = sprintf( "UPDATE monitora.documentos SET docstatus = 'E' WHERE docid = %d", $_REQUEST['docid'] );
		if( !$db->executar( $sql ) ){
			$_SESSION['MSG_AVISO'][] = "Não foi possível remover o documento.";
			$db->rollback();
		} else {
			// enquanto nao pode ser removido de verdade... fica comentáda a linha
			//if(unlink($this->arquivo_caminho))
			//{
				$db->commit();
				$_SESSION['MSG_AVISO'][] = "Documento excluído com êxito";
			//}
			//else
			//	$_SESSION['MSG_AVISO'][] = "Não foi possível remover o documento.";
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		
		break;

	case 'download':
		$sql ="SELECT * FROM monitora.documentos WHERE docstatus = 'A' AND docid = ".$_REQUEST['arquivo'];
		$arquivo = $db->pegaLinha( $sql );
		$caminho = APPRAIZ . 'arquivos/monitora/'.$arquivo['doccaminho'] ;
		if ( !is_readable( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		header( 'Content-type: '. $arquivo['docmimetype'] );
		header( 'Content-Disposition: attachment; filename='.$arquivo['docnome']);
		readfile( $caminho );
		exit();
                // break;
	default:
		break;

}


include APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
monta_titulo(
	'Certificado de Disponibilidade Orçamentária - CDO',
	'Cadastro de Anexos'
);

?>


<script language="javascript" type="text/javascript">
	function cadastrar_anexo(){
		if ( validar_formulario_anexo() ) {
			document.anexo.submit();
		}
	}
	
	function validar_formulario_anexo(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.anexo.taaid.value = trim( document.anexo.taaid.value );
		document.anexo.docdescricao.value = trim( document.anexo.docdescricao.value );
		if ( 
			document.anexo.taaid.value == '' ||
			document.anexo.taaid.value == 0
			) {
			mensagem += '\nTipo';
			validacao = false;
		}
		if ( document.anexo.docdescricao.value == '' ) {
			mensagem += '\nDescrição';
			validacao = false;
		}
		if ( document.anexo.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluir_anexo( anexo ){
		if ( confirm( 'Deseja excluir o documento?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir_anexo&docid='+ anexo;
		}
	}
	
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
		<!-- NOVO ANEXO -->
		<form method="post" name="anexo" enctype="multipart/form-data">
			<input type="hidden" name="evento" value="cadastrar_anexo"/>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
					<td>
						<input type="file" name="arquivo"/>
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
					<td>
						<select style="width: 200px;" class="CampoEstilo" name="taaid">
							<option value="0">[Selecione]</option>
							<?php
								$sql = "select taaid, taadescricao from pde.tipoanexoatividade order by taadescricao asc";
								foreach( $db->carregar( $sql ) as $tipo )
								{
									echo "<option value='".$tipo['taaid']."'> ".$tipo['taadescricao']."</option>";
								}
							?>
						</select>
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
					<td><?= campo_textarea( 'docdescricao', 'S', 'S', '', 70, 2, 500 ); ?></td>
				</tr>
				<tr style="background-color: #cccccc">
					<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
					<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_anexo();"/></td>
				</tr>
			</table>
		</form>
		</td>
	</tr>		
<table>		



<table cellpadding="2" cellspacing="0" align="center" border="0">
	<tr>
		<td colspan="2">
			<?php if ( $_SESSION['MSG_AVISO'] ): ?>
				<font color="red">
					<p>
						<ul>
							<li><?= implode( '</li><li>', (array) $_SESSION['MSG_AVISO'] ); ?></li>
						</ul>
					</p>
				</font>
			<? endif; ?>
			<?php $_SESSION['MSG_AVISO'] = array(); ?>
		</td>
	</tr>		
<table>
