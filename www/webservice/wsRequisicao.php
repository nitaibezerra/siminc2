<?php
include "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();
set_error_handler("errorHandler", E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED & ~E_WARNING); //Troca o erro padrão do simec

if($_POST['usuario']){
    $_SESSION['usucpf'] = $_SESSION['usucpforigem'] = $_POST['usuario'];
}

function __autoload($class_name) {
    
    include_once APPRAIZ . "includes/funcoes.inc";

	$arCaminho = array(
						APPRAIZ . "includes/classes/modelo/public/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/modelo/seguranca/",
						APPRAIZ . "includes/classes/controller/",
						APPRAIZ . "includes/classes/view/",
						APPRAIZ . "includes/classes/html/",
						APPRAIZ . "includes/classes/",
						APPRAIZ . "www/webservice/",
					  );

	//incluindo arquivos pelo módulo informado
	if(isset($_POST["modulo"])) {
	    $strModulo     = strtolower($_POST["modulo"]);
	    $constantes    = APPRAIZ . "www/" . $strModulo . "/_constantes.php";
	    $funcoes       = APPRAIZ . "www/" . $strModulo . "/_funcoes.php";

	    if ( file_exists( $constantes ) ){
	        include_once( $constantes );
	    }

	    if ( file_exists( $funcoes ) ){
	        include_once( $funcoes );
	    }

	    include_once APPRAIZ . 'includes/workflow.php';

	    $arCaminho[] = APPRAIZ . "includes/classes/modelo/{$strModulo}/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classe/controller/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classe/modelo/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classes/modelo/";

	}

	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;
		}
	}
}

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["funcao"])) {
//    header('Content-type: application/json');

    $retorno = false;

    // funções genéricas
    if($_POST ["funcao"] && empty($_POST["modulo"])){
        switch ($_POST ["funcao"]) {
            case "logar" :
                $retorno = verificarSeguranca($_POST['usuario'],$_POST['senha']);
                break;
        }

    }

    //funções especificas
    $_POST["modulo"] = ucfirst(strtolower($_POST["modulo"]));
    $classe = $_POST["modulo"] . 'Ws';
    if(!empty($_POST["modulo"]) && class_exists($classe)){
        $obj = new $classe;

        //verifica se as credenciais enviadas são válidas
        if(!in_array($_POST['funcao'], $obj->getFuncoesPermitidasSemLogar()) ){
            verificarSeguranca($_POST['usuario'], $_POST['senha'], true);
        }

        if(method_exists ( $obj , $_POST ["funcao"] )){

            $dados = trataDados($_POST['dados']);
            $dados = $dados ? json_decode($dados, true) : array();

            $retorno = $obj->$_POST ["funcao"]($dados);
        }
    }

    echo ($retorno ? gerarJson($retorno) : funcaoInvalida());
    die();
}

function funcaoInvalida(){
    $arDados = array('codigo' => '1', 'mensagem' => "Função inválida");
    return gerarJson($arDados);
}

function verificarSeguranca($usuario, $senha, $valida = false){
    $mUsuario = new Usuario();
    $retorno = $mUsuario->loginWebService($usuario, $senha);

    if($valida && $retorno['codigo'] == '1'){
        die(gerarJson($retorno));
    }

    return $retorno;
}

function codificarUtf8($arDados){
    foreach($arDados as $k => $dados){
        if(is_array($dados)){
            $arDados[$k] = codificarUtf8($dados);
        }else{
            $arDados[$k] = utf8_encode($dados);
        }
    }
    return $arDados;
}

function gerarJson($param){
    $param = codificarUtf8($param);
    return json_encode($param);
}

function errorHandler($errno, $errstr, $errfile, $errline){
       $arErro = array('codigo' => '1', 'mensagem' => "Erro", "numero" => $errno, "erro" => $errstr, "arquivo" => $errfile, "linha" => $errline);
       die(gerarJson($arErro));
}

function trataDados($dados){
    $dados = str_replace('\"','"', $dados);
    return $dados;
}

// Testa o cliente
if(isset($_REQUEST["cliente"])){

	$cpf_teste_ws = '';
	$senha_teste_ws = md5('senhauser');

    $dadosUsuario = array(
        'usucpf' => "086.825.204-27",       //cpf
        'usunome' => "Valtemir Teste Souza", //nome
        'ususexo' => "M", //sexo
        'regcod' => "RS",   //UF
        'muncod' => "4304606", // Municipio
        'usufoneddd' => "61", //DDD
        'usufonenum' => "8210-0134", //Telefone
        'usuemail' => "valtemir.souza168@gmail.com", // email
        'htudsc' => "Teste", //Observacao
    );

    $dadosUf = json_encode($dadosUf);
    $options = array(
                    'http' => array(
                                    'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
                                    'method'  => 'POST',
                                    'content' => "funcao=logar&usuario=" . $cpf_teste_ws . "&senha=" . $senha_teste_ws,
//                                    'content' => "modulo=sic&funcao=cadastrarUsuario&usuario=" . $cpf_teste_ws . "&senha=" . $senha_teste_ws . "&dados={$dadosUsuario}",
//                                    'content' => "modulo=sic&funcao=listarMunicipios&dados={$dadosUf}",
                    ),
    );

    $context  = stream_context_create($options);
    $result = file_get_contents("http://canoas.mesotec.com.br/webservice/wsRequisicao.php", false, $context);
    dbg($result,d);
}