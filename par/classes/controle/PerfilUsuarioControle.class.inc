<?php

class PerfilUsuarioControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function carregarPerfilUsuario($pflcod) {
        $obPerfilUsuario = new PerfilUsuario();
        $arrPerfilUsuario = $obPerfilUsuario->carregarPerfilUsuario($pflcod);
        return $arrPerfilUsuario;
    }

}
