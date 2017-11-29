<?php

class Emendas_Controller_AlteracaoOrcamentaria
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/alteracaoorcamentaria&acao=A';

        try {
            $mAlteracaoOrcamentaria = new Emendas_Model_AlteracaoOrcamentaria($dados['aloid']);
            $mAlteracaoOrcamentaria->popularDadosObjeto($dados);
            
            $mAlteracaoOrcamentaria->salvar(null, null, []);
            $mAlteracaoOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAlteracaoOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($aloid)
    {
        $url = '?modulo=apoio/alteracaoorcamentaria&acao=A';

        try {
            $mAlteracaoOrcamentaria = new Emendas_Model_AlteracaoOrcamentaria($aloid);
            
            if($mAlteracaoOrcamentaria->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAlteracaoOrcamentaria->aloid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAlteracaoOrcamentaria->alostatus = 'I';
            $mAlteracaoOrcamentaria->salvar();
            $mAlteracaoOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAlteracaoOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            