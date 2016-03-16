<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, "&nbsp;" );

# configuração
$diretorio = APPRAIZ . "arquivos/SIGPLAN/exportacao/";
$servicos = array(
	"RetornaAcao" => "recebeAcao",
	"RetornaPrograma" => "recebePrograma",
	"RetornaDadoFisico" => "recebeDadoFisico",
	"RetornaRestricaoPrograma" => "recebeRestricaoPrograma",
	"RetornaRestricaoAcao" => "recebeRestricaoAcao",
	"RetornaValidacaoAcao" => "recebeValidacaoTrimestral",
);

// HOMOLOGAÇÃO
//$wsdl = "https://homsigplan.serpro.gov.br/infrasig/INFRASIG.ASMX?WSDL";

// PRODUÇÃO
$wsdl = "https://www.sigplan.gov.br/infrasig/INFRASIG.ASMX?WSDL";

$configuracao = array(
	"encoding" => "ISO-8859-1",
	"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
	"trace" => true
);

if ( $_REQUEST["formulario"] ) {
	try {
		
		# verifica se o arquivo existe
		$arquivo = $diretorio . basename( $_REQUEST["arquivo"] ) . ".xml";
		if( !file_exists( $arquivo ) ) {
			throw new Exception( "O arquivo indicado não existe." );
		}
		
		# identifica o serviço
		preg_match( "/[a-zA-Z]{1,}/", $_REQUEST["arquivo"], $match );
		$servico = $servicos[$match[0]];
		
		# carrega os serviços do sigplan
		$sigplan = new SoapClient( $wsdl, $configuracao );
		
		# verifica se o método existe
		$verificacao = false;
		$padrao = "/[a-zA-Z ]({$servico})\(/";
		foreach( $sigplan->__getFunctions() as $assinatura ) {
			preg_match( $padrao, $assinatura, $match );
			if ( $servico == $match[1] ) {
				# identifica a estrutura de retorno
				$estrutura = substr( $assinatura, 0, strpos( $assinatura, " " ));
				$estrutura = str_replace( "Response", "Result", $estrutura );
				$verificacao = true;
				break;
			}
		}
		if ( !$verificacao ) {
			throw new Exception( "O serviço solicitado não existe." );
		}

		# monta parâmetros
		$objeto = new stdClass();
		$objeto->usuario = $_REQUEST["usuario"];
		$objeto->senha = $_REQUEST["senha"];
		$objeto->dados = file_get_contents( $arquivo );
		
		# realiza a requisição
		$retorno = $sigplan->$servico( $objeto )->$estrutura;
		
		if ( $retorno->numerocarga == -1 ) {
			throw new Exception( "{$retorno->mensagem} {$retorno->descricao}" );
		}
		
		# registra na sessão as informações recebidas do sigplan
		$_SESSION["retorno_sigplan"] = $retorno;
		
		$db->commit();
		$_REQUEST["acao"] = "E";
		$db->sucesso( $_REQUEST["modulo"] );
		
	} catch ( Exception $erro ) {
		$db->rollback();
		dump( $erro, true );
		?>
		<html>
			<head>
				<script type="text/javascript">
				alert( '<?= $erro->getMessage() ?>' );
				location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=E";
				</script>
			</head>
			<body>&nbsp;</body>
		</html>
		<?
		exit();
	}
}

$lista = array();
$arquivos = glob( $diretorio . "*.xml" );
foreach ( $arquivos as $arquivo ) {
	# identifica o grupo
	$nome = basename( $arquivo, ".xml" );
	$nome = substr( $nome, 7, strpos( $nome, "-" ) - 7 );
	if ( empty( $lista[$nome] ) ) {
		$lista[$nome] = array();
	}
	# organiza a matriz
	$data = explode( "-", substr( $arquivo, -12, 8 ) );
	$data = date( "d . m . Y", mktime( 0, 0, 0, $data[1], $data[0], $data[2] ) );
	$lista[$nome][] = array(
		"caminho" => $arquivo,
		"data"    => $data,
	);
}
unset( $arquivos );

?>
<style>
	label { cursor: pointer; }
</style>
<form method="POST" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">Usuário:</td>
			<td><input class="normal" type="text" size="20" maxlength="20" name="usuario"/></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Senha:</td>
			<td><input class="normal" type="password" size="20" maxlength="20" name="senha"/></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Arquivo:</td>
			<td>
				<select class="normal" id="arquivo" name="arquivo" style="width: 115px;">
					<option value="" selected="selected">&nbsp;</option>
					<?php foreach( $lista as $nome => $arquivos ): ?>
						<optgroup id="arquivo_<?= $nome ?>" label="<?= $nome ?>" style="font-style: normal;">
							<?php foreach( $arquivos as $item ): ?>
								<option value="<?= basename( $item["caminho"], ".xml" ) ?>"><?= $item["data"] ?></option>
							<?php endforeach; ?>
						</optgroup>
					<?php endforeach; ?>
				</select>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td style="width: 20%;">&nbsp;</td>
			<td>
				<input type='button' class="botao" name='consultar' value='Enviar' onclick="enviar();"/>
					<span style="cursor: pointer;" onclick="this.style.display = 'none';">
					<?php
					if( isset( $_SESSION["retorno_sigplan"] ) ) {
						echo $_SESSION["retorno_sigplan"]->mensagem;
						unset( $_SESSION["retorno_sigplan"] );
					}
					?>
				</span>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
	
	function enviar(){
		document.formulario.submit();
	}
		
</script>