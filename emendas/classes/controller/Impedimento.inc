<?php

class Emendas_Controller_Impedimento
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/impedimento&acao=A';

        try {
            $mImpedimento = new Emendas_Model_Impedimento($dados['impid']);
            $mImpedimento->popularDadosObjeto($dados);
            
            $mImpedimento->salvar(null, null, []);
            $mImpedimento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mImpedimento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($impid)
    {
        $url = '?modulo=apoio/impedimento&acao=A';

        try {
            $mImpedimento = new Emendas_Model_Impedimento($impid);
            
            if($mImpedimento->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mImpedimento->impid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mImpedimento->impstatus = 'I';
            $mImpedimento->salvar();
            $mImpedimento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mImpedimento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            