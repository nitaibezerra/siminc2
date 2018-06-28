<?php

class Public_Controller_ObjetivoMetaPpa
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-meta-objetivo-ppa&acao=A';

        try {
            $oModel = new Public_Model_ObjetivoMetaPpa();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $mppid => $vinculos){
                    foreach($vinculos as $oppid){
                        $oModel->mppid = $mppid;
                        $oModel->oppid = $oppid;
                        $oModel->salvar();
                        $oModel->opmid = null;
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