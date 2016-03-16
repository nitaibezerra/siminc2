<?php

class ModelosDocumentosControle extends Controle {

    public function __construct() {
        parent::__construct();
    }
    
    public function carregarModeloDocumento($tpdcod, $booSQL = FALSE, $strObra = '', $arrCondicao = array()) {
        $obModelosDocumentos = new ModelosDocumentos();
        $arrModelosDocumentos = $obModelosDocumentos->carregarModeloDocumento($tpdcod, $booSQL, $strObra, $arrCondicao);
        return $arrModelosDocumentos;
    }

}
