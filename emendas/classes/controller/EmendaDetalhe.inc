<?php

class Emendas_Controller_EmendaDetalhe
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/emendadetalhe&acao=A';

        try {
            $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe($dados['emdid']);
            $mEmendaDetalhe->popularDadosObjeto($dados);
            
            $mEmendaDetalhe->salvar(null, null, []);
            $mEmendaDetalhe->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEmendaDetalhe->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($emdid)
    {
        $url = '?modulo=apoio/emendadetalhe&acao=A';

        try {
            $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe($emdid);
            
            if($mEmendaDetalhe->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mEmendaDetalhe->emdid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mEmendaDetalhe->status = 'I';
            $mEmendaDetalhe->salvar();
            $mEmendaDetalhe->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mEmendaDetalhe->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            