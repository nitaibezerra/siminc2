<?php

class Planacomorc_Controller_HistoricoPi
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/unidade/cadastro_pi&acao=A';

        try {
            $mHistoricoPi = new Planacomorc_Model_HistoricoPi($dados['hpiid']);
            $mHistoricoPi->popularDadosObjeto($dados);
            $mHistoricoPi->salvar(null, null, []);
            $mHistoricoPi->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mHistoricoPi->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

}