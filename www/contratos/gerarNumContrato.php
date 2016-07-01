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

    $ctrano = $_POST['ctrano'];

    $sql = "SELECT
              ctcontrato.ctrnum
            FROM
              contratos.ctcontrato
            WHERE
            ctcontrato.ctrano = '$ctrano'
            order by ctrid desc
            limit 1";

    $rs = $db->carregar($sql);

    if (count($rs) > 0) {
        echo $rs[0]['ctrnum'] + 1;
    } else {
        echo "false";
    }

    /*if (count($rs) > 0)
        echo "true";
    else
        echo "false";*/

?>