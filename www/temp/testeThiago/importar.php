<?php

set_time_limit( 0 );
ini_set( 'display_errors', E_ALL );

// carrega as bibliotecas
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require_once( 'SnoopyBufferSocket.class.php' );

// atribuições requeridas para que a auditoria do sistema funcione
$_SESSION['sisid'] = 1; # seleciona o sistema de segurança
$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';
$ano = $_REQUEST['ano'];

$db = new cls_banco();

// carrega a biblioteca da barra de progressão
include APPRAIZ . "includes/PogProgressBar.php";
$barra = new PogProgressBar( 'bar' );

?>
<html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<script type="text/javascript">
			self.focus();
		</script>
	</head>
	<body>
		<table align="center" cellpadding="0" cellspacing="20" border="0">
			<tr>
				<td align="center" style="font-family: verdana; font-size: 10pt;">
					Importação SIGPLAN
				</td>
			</tr>
			<tr>
				<td>
					<? $barra->draw(); ?>
				</td>
			</tr>
		</table>
	</body>
</html>
<?php

$snoopy =  new Snoopy();
$snoopy->_isproxy = true;
$snoopy->proxy_host = 'proxy.mec.gov.br';
$snoopy->proxy_port = '8080';
$snoopy->proxy_user = 'thiagomata';
$snoopy->proxy_pass = 'bilunga';

$sql = sprintf( "select acacod, loccod, prgcod from monitora.acao where prgano = '". $ano ."' group by acacod, loccod, prgcod" );
$acoes = $db->carregar( $sql );

// inicializa a barra de progressão geral
$total = count( $acoes );
$porcentagem = 0;

foreach ( (array) $acoes as $indice => $acao ) {
	$post = array(
		'usuario' => 'leo.kessel',
		'senha' => 'kessel',
		'PRGAno' => $ano,
		'PRGCod' => $acao['prgcod'],
		'ACACod' => $acao['acacod'],
		'LOCCod' => $acao['loccod'],
	);
	$snoopy->submit( 'http://www.sigplan.gov.br/infrasig/sigtoinfra.asmx/geracaoPorLocalizador', $post );
	$xml = simplexml_load_string( $snoopy->results );
	if ( !$xml ) {
		continue;
	}
	foreach( $xml->ArrayOfDadoFinanceiro->DadoFinanceiro as $a => $registro ) {
		$registro = (array) $registro;
		$registro['loccod'] = $acao['loccod'];
		$sql = sprintf(
			"select acaid from monitora.acao where prgcod = '%s' and acacod = '%s' and loccod = '%s' and saccod = '%s' and acasnrap = 'f'",
			$registro['PRGCod'],
			$registro['ACACod'],
			$registro['loccod'],
			$registro['SACCod']
		);
		$registro['acaid'] = $db->pegaUm( $sql );
		
		if ( $registro['acaid'] ) {

			// remove o registro para evitar duplicação
			$sql = sprintf(
				"delete from monitora.dadofinanceiro where acaid = %d and prgano = '%s'",
				$registro['acaid'],
				$registro['PRGAno']
			);
			if ( !$db->executar( $sql ) ) {
				$db->rollback();
				dbg( $sql ,1 );
			}
		
			// insere o registro novo
			if ( $registro['acaid'] != 0 ) {
				$sql = sprintf(
					"insert into monitora.dadofinanceiro ( %s ) values ( '%s' )",
					implode( ',', array_keys( $registro ) ),
					implode( "','", array_values( $registro ) )
				);
				if ( !$db->executar( $sql ) ) {
					$db->rollback();
					dbg( $sql ,1 );
				}
			}
		}
	}
	
	// atualiza barra de progreção geral
	$porcentagem = ceil( ( $indice / $total ) * 100 );
	$barra->setProgress( $porcentagem );
}

$db->commit();

?>