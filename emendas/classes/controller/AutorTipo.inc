<?php

class Emendas_Controller_AutorTipo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/autortipo&acao=A';

        try {
            $mAutorTipo = new Emendas_Model_AutorTipo($dados['atpid']);
            $mAutorTipo->popularDadosObjeto($dados);
            
            $mAutorTipo->salvar(null, null, []);
            $mAutorTipo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutorTipo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($atpid)
    {
        $url = '?modulo=apoio/autortipo&acao=A';

        try {
            $mAutorTipo = new Emendas_Model_AutorTipo($atpid);
            
            if($mAutorTipo->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAutorTipo->atpid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAutorTipo->atpstatus = 'I';
            $mAutorTipo->salvar();
            $mAutorTipo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutorTipo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            