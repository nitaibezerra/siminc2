<?php

class Proposta_Controller_PropostaDetalhe
{
    public function salvar($dados)
    {
        try {
            $mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($dados['prdid']);
            $mPropostaDetalhe->popularDadosObjeto($dados);

            $mPropostaDetalhe->prdvalor = $mPropostaDetalhe->prdvalor ? str_replace(array('.', ','), array('', '.'), $mPropostaDetalhe->prdvalor) : 0;
            $mPropostaDetalhe->prdvalorexpansao = $mPropostaDetalhe->prdvalorexpansao ? str_replace(array('.', ','), array('', '.'), $mPropostaDetalhe->prdvalorexpansao) : 0;

            $mPropostaDetalhe->salvar(null, null, ['prdvalorexpansao', ]);
            $mPropostaDetalhe->commit();
            return true;
        } catch (Exception $e){
            $mPropostaDetalhe->rollback();
            return false;
        }
    } //end salvar()
    
    public function excluir($prdid)
    {
        $url = '?modulo=proposta/proposta_form&acao=A';

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
            return $mPropostaDetalhe->proid;
        } catch (Exception $e){
            $mPropostaDetalhe->rollback();
            return false;
        }
    } //end salvar()
}            