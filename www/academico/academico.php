<?PHP
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

#CARREGA FUNÇÕES ESPECIFICAS DO DADOS DOS DIRIGENTES E TODAS AS TELAS RELACIONADAS
include_once '_funcoes_dados_dirigentes.php';

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";

define('CLASSES_GERAL',    APPRAIZ . "/includes/classes/");
define('CLASSES_CONTROLE', APPRAIZ . 'academico/classes/controle/');
define('CLASSES_MODELO'  , APPRAIZ . 'academico/classes/modelo/');
define('CLASSES_VISAO'  , APPRAIZ . 'includes/classes/view/');
define('CLASSES_HTML'  , APPRAIZ . 'includes/classes/html/');

set_include_path(CLASSES_GERAL. PATH_SEPARATOR .
				 CLASSES_CONTROLE . PATH_SEPARATOR . 
				 CLASSES_MODELO . PATH_SEPARATOR . 
				 CLASSES_VISAO . PATH_SEPARATOR . 
				 CLASSES_HTML . PATH_SEPARATOR . 
				 get_include_path() );
				 
function __autoload($class) {
    if(PHP_OS != "WINNT") { // Se "não for Windows"
    	$separaDiretorio = ":";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	} else { // Se for Windows
    	$separaDiretorio = ";c:";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	    $include_path_tokens = str_replace("//", "/", $include_path_tokens);
    	$include_path_tokens = explode(";", $include_path_tokens[0]);
	}

    foreach($include_path_tokens as $prefix){
            $path[0] = $prefix . $class . '.class.inc';
            $path[1] = $prefix . $class . '.php';
     
     	foreach($path as $thisPath){
        	if(file_exists($thisPath)){
            	require_once $thisPath;
                return;
            }
		}
    }
}

$habilitado = academico_possui_perfil( array(PERFIL_IFESCADASTRO,
											 PERFIL_IFESAPROVACAO,
											 PERFIL_MECCADASTRO,
											 PERFIL_ADMINISTRADOR) );
								  
$habil = $habilitado ? 'S' : 'N';
$disabled = $habilitado ? '' : 'disabled';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
