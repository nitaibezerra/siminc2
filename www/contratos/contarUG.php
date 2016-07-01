<?php
    /**
     * Created by PhpStorm.
     * User: irian.villalba
     * Date: 28/01/15
     * Time: 11:30
     */



    include "config.inc";
    header('Content-Type: text/html; charset=iso-8859-1');
    include APPRAIZ."includes/classes_simec.inc";
    include APPRAIZ."includes/funcoes.inc";

    // carrega as funções específicas do módulo
    // include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once '_constantes.php';
    include_once '_funcoes.php';
    include_once '_componentes.php';

    $db = new cls_banco();

    $hspid = $_POST['hspid'];

    $sql = "SELECT
              unidadegestora.ungdsc,
              unidadegestora.ungcod
            FROM
              contratos.hospitalug,
              public.unidadegestora
            WHERE
              unidadegestora.ungcod = hospitalug.ungcod AND
              hospitalug.hspid = $hspid";

    $rs = $db->carregar($sql);

    echo sig_json_encode($rs);

?>