<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class Model {
    protected $dao;
    protected $erro;
    public $instituicao;
    public $usuario;
    public $nome;
    public $accept_encoding;

    protected function __construct($dao) {
        $this->dao = $dao;
    }

    public function getErro() {
        return $this->erro;
    }
}

?>
