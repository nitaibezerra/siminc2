<?php

class TipoDocumentoControle extends Controle {

    public function __construct() {
        parent::__construct();
    }
    
    public function carregarTipoDocumento($booSQL = FALSE, $arrTpdCod = array()) {
        $obTipoDocumento = new TipoDocumento();
        $arrTipoDocumento = $obTipoDocumento->carregarTipoDocumento($booSQL, $arrTpdCod);
        return $arrTipoDocumento;
    }

}
