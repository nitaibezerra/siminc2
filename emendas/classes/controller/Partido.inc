<?php

class Emendas_Controller_Partido
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/partido&acao=A';

        try {
            $mPartido = new Emendas_Model_Partido($dados['parid']);
            $mPartido->popularDadosObjeto($dados);
            
            $mPartido->salvar(null, null, []);
            $mPartido->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPartido->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($parid)
    {
        $url = '?modulo=apoio/partido&acao=A';

        try {
            $mPartido = new Emendas_Model_Partido($parid);
            
            if($mPartido->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mPartido->parid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mPartido->parstatus = 'I';
            $mPartido->salvar();
            $mPartido->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mPartido->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            