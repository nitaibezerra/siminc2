<?php
/**
 * Desenvolvedor: MESOTEC Informática LTDA
 * Analistas: Adonias Malosso <adonias@mesotec.com.br>, Davison Silva <davison@mesotec.com.br>
 * Programadores: Adonias Malosso <adonias@mesotec.com.br>, Halisson Gomides <halisson@mesotec.com.br>, Paulo Estevao <paulo@mesotec.com.br>
 * Baseado no SIMEC
 */

function __autoload($class_name) {
    $arCaminho = array(
                        APPRAIZ . "includes/classes/modelo/sisplan/",
                        APPRAIZ . "includes/classes/modelo/territorios/",
                        APPRAIZ . "includes/classes/modelo/planointerno/",
                        APPRAIZ . "includes/classes/html/",
                        APPRAIZ . "includes/classes/view/",
                        APPRAIZ . "includes/classes/",
                      );

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}


date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obtém o tempo com precisão de microsegundos. Essa informação é utilizada para
 * calcular o tempo de execução da página.
 *
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec;
}

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";


// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// carrega a página solicitada pelo usuário
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
if ( $chave == 'f' ) {
	// leva o usuário para o formulário de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
} else if ( $_REQUEST['modulo'] ) {
	// leva o usuário para a página solicitada

	include APPRAIZ . 'includes/testa_acesso.inc';

	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	if(!$_REQUEST["AJAX"]){
	include APPRAIZ . "includes/rodape.inc";
	}
} else {
	// leva o usuário para o formulário de login
	header( "Location: login.php" );
}
?>