<?php

class Proposta_Controller_Proposta
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/proposta_form&acao=A';

        try {
            $mProposta = new Proposta_Model_Proposta($dados['proid']);
            $mProposta->popularDadosObjeto($dados);
            $mProposta->prsano = $mProposta->prsano ? $mProposta->prsano : $_SESSION['exercicio'];
                
            $mProposta->salvar(null, null, ['proquantidade', 'proquantidadeexpansao', 'projustificativa', 'projustificativaexpansao', ]);
            $mProposta->commit();
            simec_redirecionar($url . '&proid='. $mProposta->proid, 'success');
        } catch (Exception $e){
            $mProposta->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($proid)
    {
        $url = '?modulo=principal/proposta&acao=A';

        try {
            $mProposta = new Proposta_Model_Proposta($proid);
            
            if($mProposta->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=principal/modalidade-pactuacao-form&acao=A&capid=' . $mProposta->proid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mProposta->prostatus = 'I';
            $mProposta->salvar();
            $mProposta->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mProposta->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            