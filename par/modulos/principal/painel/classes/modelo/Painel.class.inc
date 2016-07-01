<?php
require (APPRAIZ . 'par/modulos/principal/painel/classes/modelo/PainelImp.class.inc');

class Painel extends Modelo implements PainelImp {
    /**
     * Retorna a lista de paineis de acordo com o usuario logado.
     * @return array $arrPainel
     */
    public function getPainelByUsuario() {
//        $usuCpf = $_SESSION['usucpf'];
        return $arrPainel = array(
            array('nome' => 'Gráficos', 'idPainel' => 1),
//            array('nome' => 'Novo Painel', 'idPainel' => 2),
        );
    }
}
