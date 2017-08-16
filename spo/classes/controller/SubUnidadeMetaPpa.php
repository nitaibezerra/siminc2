<?php

class Spo_Controller_SubUnidadeMetaPpa
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-meta-ppa&acao=A';

        try {
            $oModel = new Spo_Model_SubUnidadeMetaPpa();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $mppid => $vinculos){
                    foreach($vinculos as $suoid){
                        $oModel->mppid = $mppid;
                        $oModel->suoid = $suoid;
                        $oModel->salvar();
                        $oModel->smpid = null;
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