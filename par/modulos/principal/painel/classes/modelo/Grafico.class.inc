<?php

require (APPRAIZ . 'par/modulos/principal/painel/classes/modelo/GraficoImp.class.inc');

class GraficoA extends Modelo implements GraficoImp {

    /**
     * Retorna a lista de graficos por Paivel.
     * @return array $arrGrafico
     */
    public function getGraficoByPainel() {
        return $arrGrafico = array(
            array('nome' => 'Obras Aprovadas / Empenhos PAC', 'idGrafico' => 'obrasaprovadaspac', 'funcao' => 'recuperarPercentualObrasAprovadasEmpenhoPac'),
            array('nome' => 'Obras Aprovadas / Empenhos PAR', 'idGrafico' => 'obrasaprovadaspar', 'funcao' => 'recuperarPercentualObrasAprovadasEmpenhoPar'),
            array('nome' => 'Pagamento de Obras PAC', 'idGrafico' => 'pagamentoobrapac', 'funcao' => 'recuperarPercentualObrasAprovadasEmpenhoPac'),
            array('nome' => 'Pagamento de Obras PAR', 'idGrafico' => 'pagamentoobrapar', 'funcao' => 'recuperarPercentualObrasAprovadasEmpenhoPac'),
        );
    }

}
