<?php

class Emendas_Controller_Delegacao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/delegacao&acao=A';

        try {
            $mDelegacao = new Emendas_Model_Delegacao($dados['edeid']);
            $mDelegacao->popularDadosObjeto($dados);
            
            $mDelegacao->salvar(null, null, []);
            $mDelegacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mDelegacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($edeid)
    {
        $url = '?modulo=apoio/delegacao&acao=A';

        try {
            $mDelegacao = new Emendas_Model_Delegacao($edeid);
            
            if($mDelegacao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mDelegacao->edeid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mDelegacao->salvar();
            $mDelegacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mDelegacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            