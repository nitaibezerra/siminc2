<?php

class PreTipoObraControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function carregarPreTipoObra($booSQL = FALSE) {
        $obPreTipoObra = new PreTipoObra();
        $arrPreTipoObra = $obPreTipoObra->carregarPreTipoObra($booSQL);
        return $arrPreTipoObra;
    }
    
    public function carregarPreTipoObraClassificacao($booSQL = FALSE) {
        $obPreTipoObra = new PreTipoObra();
        $arrPreTipoObra = $obPreTipoObra->carregarPreTipoObraClassificacao($booSQL);
        return $arrPreTipoObra;
    }

}
