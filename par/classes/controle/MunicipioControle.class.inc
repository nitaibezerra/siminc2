<?php

class MunicipioControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function carregarMunicipio() {
        $obMunicipio = new Municipio();
        $arrMunicipio = $obMunicipio->carregarMunicipio();
        return $arrMunicipio;
    }
    
    public function carregarMunicipioPorMuncod($muncod) {
        $obMunicipio = new Municipio();
        $arrMunicipio = $obMunicipio->carregarMunicipioPorMuncod($muncod);
        return $arrMunicipio;
    }
    
    public function carregarGrupoMunicipio($booSQL = FALSE) {
        $obMunicipio = new Municipio();
        $arrMunicipio = $obMunicipio->carregarGrupoMunicipio($booSQL);
        return $arrMunicipio;
    }

}
