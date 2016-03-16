<?php
//
// $Id$
//

require_once "base/EnderecoBase.php";

class Endereco extends EnderecoBase {
    /**
     * 
     */
    static public function carregarEnderecosPorEntidade($entid)
    {
        $sql = "SELECT
                   ee.endid
                FROM
                    entidade.endereco ee
                WHERE
                    ee.entid = ?";

        $rs        = ActiveRecord::ExecSQL($sql, array($entid));
        $enderecos = array();

        while (!$rs->EOF) {
            $enderecos[] = new Endereco($rs->fields['endid']);
            $rs->moveNext();
        }

        return $enderecos;
    }
}





