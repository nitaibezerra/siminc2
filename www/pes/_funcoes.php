<?php

function alertlocation($dados)
{
	die("<script>
		".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
		".(($dados['location'])?"window.location='".$dados['location']."';":"")."
		".(($dados['javascript'])?$dados['javascript']:"")."
		 </script>");
}

function real2Db($valor)
{
	return str_replace(array('.', ','), array('', '.'), $valor);
}

/**
 * Metodo responsavel por pegar a action da controller e renderizar na tela.
 *
 * @name urlAction
 * @return void
 *
 * @throws Exception Controller não foi informada!
 * @throws Exception Action não foi informada!
 * @throws Exception Controller não existe!
 * @throws Exception Action não existe!
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
 * @since 10/06/2013
 */
function urlAction()
{
    // Se tiver o nome da controller e o nome da action,
    // Significa que e uma requisicao.
    if(isset($_POST['controller']) && isset($_POST['action'])){

        // Encapsulando dados post
        $nameController = 'Controller_' . $_POST['controller'];
        $nameAction = $_POST['action'] . 'Action';

        // Validacoes
        if(!$nameController) throw new Exception('Controller não foi informada!');
        if(!$nameAction) throw new Exception('Action não foi informada!');
        if(!class_exists($nameController)) throw new Exception('Controller não existe!');
        if(!method_exists( $nameController, $nameAction )) throw new Exception('Action não existe!');

        // Estanciando class Controller e exibindo na tela a action.
        $controller = new $nameController;
        echo $controller->$nameAction();
        exit;
    }
}

function pegaArrayPerfil($usucpf = null){

    $usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];

    global $db;

    $sql = "SELECT
                pu.pflcod
            FROM
                seguranca.perfil AS p
            LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
            WHERE
                p.sisid = '{$_SESSION['sisid']}'
                AND pu.usucpf = '$usucpf'";

    $pflcod = $db->carregar( $sql );

    foreach($pflcod as $dados){
        $arPflcod[] = $dados['pflcod'];
    }
    return $arPflcod;
}

function textoUltimaCargaSIAFI()
{
    global $db;
    $sql = <<<DML
SELECT TO_CHAR(MAX(accdatacarga), 'DD/MM/YYYY às HH24:MI:SS') as accdatacarga
  FROM pes.pesacompconsolidado
  WHERE accano = '%s'
DML;
    $stmt = sprintf($sql, $_SESSION['exercicio']);
    $datacargasiafi = $db->pegaUm($stmt);
    if (!$datacargasiafi) {
        $datacargasiafi = 'não consta';
    }

    return <<<TXT
- Última carga do SIAFI referente ao exercício {$_SESSION['exercicio']}: {$datacargasiafi}.
TXT;
}