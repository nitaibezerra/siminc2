<?php

class Proposta_Controller_PropostaDetalhe
{
    public function salvar($dados)
    {
        $url = '?modulo=proposta/propostadetalhe&acao=A';

        try {
            $mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($dados['prdid']);
            $mPropostaDetalhe->popularDadosObjeto($dados);
            
            $mPropostaDetalhe->salvar(null, null, ['prdvalorexpansao', ]);
            $mPropostaDetalhe->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPropostaDetalhe->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($prdid)
    {
        $url = '?modulo=proposta/propostadetalhe&acao=A';

        try {
            $mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($prdid);
            
            if($mPropostaDetalhe->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=proposta/modalidade-pactuacao-form&acao=A&capid=' . $mPropostaDetalhe->prdid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mPropostaDetalhe->prdstatus = 'I';
            $mPropostaDetalhe->salvar();
            $mPropostaDetalhe->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPropostaDetalhe->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            