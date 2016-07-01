<?php

class EstadoDocumentoControle extends Controle {

    public function __construct() {
        parent::__construct();
    }
    
    public function carregarEstadoDocumento($esdid, $booSQL = FALSE) {
        $obEstadoDocumento = new EstadoDocumento();
        $arrEstadoDocumento = $obEstadoDocumento->carregarEstadoDocumento($esdid, $booSQL);
        return $arrEstadoDocumento;
    }

}
