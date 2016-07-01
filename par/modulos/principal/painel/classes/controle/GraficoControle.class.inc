<?php
require (APPRAIZ . 'par/modulos/principal/painel/classes/modelo/Grafico.class.inc');

class GraficoControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    /**
     * Retorna a lista de paineis de acordo com o usuario logado.
     * @return array $arrPainel
     */
    public function getGraficoByPainel() {
        $obGrafico = new GraficoA();
        return $obGrafico->getGraficoByPainel();
    }

}
