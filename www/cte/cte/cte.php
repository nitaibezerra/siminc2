<?php
/**
 * Rotina que controla o acesso рs pсginas do mѓdulo. Carrega as bibliotecas
 * padrѕes do sistema e executa tarefas de inicializaчуo. 
 *
 * @author Renъ de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 22/03/2207
 */

/**
 * Obtщm o tempo comprecisуo de microsegundos. Essa informaчуo щ utilizada para
 * calcular o tempo de execuчуo da pсgina.  
 * 
 * @return float
 * @see /includes/rodape.inc
 */

function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec; 
}

// obtщm o tempo inicial da execuчуo
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funчѕes gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

/*
if ( $_SESSION['usucpf'] != '' && $_SESSION['usucpf'] != '' )
{
header( "Location: ../manutencao.htm" );
die();
}
*/

// carrega as funчѕes do mѓdulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexуo com o servidor de banco de dados
$db = new cls_banco();

// carrega os dados do mѓdulo
$modulo = $_REQUEST['modulo'];
$sql= "select ittemail, orgcod, ittabrev from instituicao where ittstatus = 'A'";
foreach( (array) $db->pegaLinha( $sql ) as $campo => $valor ) {
	$_SESSION[$campo]= trim( $valor );
}

// carrega a pсgina solicitada pelo usuсrio
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
if ( $chave == 'f' ) {
	// leva o usuсrio para o formulсrio de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
} else if ( $_REQUEST['modulo'] ) {
	// leva o usuсrio para a pсgina solicitada
	include APPRAIZ . 'includes/testa_acesso.inc';
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	include APPRAIZ . "includes/rodape.inc";
} else {
	// leva o usuсrio para o formulсrio de login
	header( "Location: login.php" );
}

?>