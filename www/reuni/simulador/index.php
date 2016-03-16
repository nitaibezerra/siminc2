<?php

function strncmp_match($arr)
{
    foreach ($arr as $key => $val)
    {
    //if (substr($key,0,5 == "HTTP_")
        if (!strncmp($key, 'HTTP_', 5))   
        {
            $out[$key] = $val;
        }
    }
    return $out;
}

/*
 * Created on 02/09/2007 by MOC
 *
 */

require_once("config.inc");

$_SESSION["evHoraUltimoAcesso"] = time();

$Logado = $_SESSION['usucpf'];
if ($Logado) 
{
    $_SESSION['USUARIO_INSTITUICAO'] = $_SESSION['unicod'];
    //para nao ter que alterar o original em \lib\Controllers\Controler.php
    $_SESSION['USUARIO_LOGIN']       = $_SESSION['usucpf'];
    $_SESSION['USUARIO_NOME']        = utf8_encode($_SESSION['usunome']);
}
else
{
    echo "<script language='javascript'> alert('Acesso Negado.');window.close(); </script>";
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Pragma: no-cache");
header('Content-Type: text/html; charset=UTF-8');
/* Define o limite de tempo do cache em 100 minutos */

function loadClass($path, $class_name) {
	$file = $path.$class_name . '.php';
	if(file_exists($file)) {
   		require_once $file;
	}
}

function __autoload($class_name) {
	if (stripos($class_name,'Model')!== FALSE) {
		loadClass('lib/Models/',$class_name);
	}
	elseif (stripos($class_name,'View')!== FALSE) {
		loadClass('lib/Views/',$class_name);
	}
	elseif (stripos($class_name,'Controller')!== FALSE) {
		loadClass('lib/Controllers/',$class_name);
	}
	elseif (stripos($class_name,'DataAccess')!== FALSE) {
		loadClass('lib/DataAccess/',$class_name);
	}
	elseif (stripos($class_name,'Widget')!== FALSE) {
		loadClass('lib/Views/Widgets/',$class_name);
	}
	else {
		loadClass('lib/',$class_name);
	}
}

$dao = DataAccessFactory::factory('PostgreSQL','simec-d','sesu','!sesu321','sesu','reuni');

/****
* ATENÇÃO: O nome do banco em produção é diferente da versão em desenvolvimento
*  descomente a linha abaixo, para versão em produção:
*/
//$dao = DataAccessFactory::factory('PostgreSQL',$servidor_bd,'sesu','!sesu321','dbsesu','reuni');


if (isset($_GET["view"])) {
	switch (addslashes($_GET["view"])) {
	case "planilhasPDF":
        $controller = new PlanilhasPDFController($dao);
        break;
    case "unidades":
        $controller = new UnidadesController($dao);
        break;
    case "graduacao":
        $controller = new GraduacaoController($dao);
        break;
    case "pos_graduacao":
        $controller = new PosGraduacaoController($dao);
        break;
    case "custeio":
        $controller = new CusteioController($dao);
        break;
    case "investimento":
        $controller = new InvestimentoController($dao);
        break;
    case "planilhas":
        $controller = new PlanilhasController($dao);
        break;
    case "municipios":
        $controller = new MunicipiosController($dao);
        break;
    default:
        $controller = new LoginController($dao);
        break;
	}
} else {
	$controller = new LoginController($dao);
}
/*
var_dump($dao);

echo "<pre>";
print_r($controller);
print_r($dao);
echo $dao->getError();

dump(print_r($controlle));
*/

$controller->getView()->display();

///phpinfo();



/*

$view=$controller->getView();

$view->display();
*/
?>
