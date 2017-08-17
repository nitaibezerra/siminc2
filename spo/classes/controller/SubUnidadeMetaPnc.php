<?php

class Spo_Controller_SubUnidadeMetaPnc
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-meta-pnc&acao=A';

        try {
            $oModel = new Spo_Model_SubUnidadeMetaPnc();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $mpnid => $vinculos){
                    foreach($vinculos as $suoid){
                        $oModel->mpnid = $mpnid;
                        $oModel->suoid = $suoid;
                        $oModel->salvar();
                        $oModel->smcid = null;
                    }
                }
            }
            $oModel->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $prefeitura->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}