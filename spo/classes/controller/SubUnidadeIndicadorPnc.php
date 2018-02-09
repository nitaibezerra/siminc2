<?php

class Spo_Controller_SubUnidadeIndicadorPnc
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-indicador-pnc&acao=A';

        try {
            $oModel = new Spo_Model_SubUnidadeIndicadorPnc();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $ipnid => $vinculos){
                    foreach($vinculos as $suoid){
                        $oModel->ipnid = $ipnid;
                        $oModel->suoid = $suoid;
                        $oModel->salvar();
                        $oModel->sicid = null;
                    }
                }
            }
            $oModel->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $oModel->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}