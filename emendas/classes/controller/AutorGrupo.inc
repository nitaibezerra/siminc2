<?php

class Emendas_Controller_AutorGrupo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/autorgrupo&acao=A';

        try {
            $mAutorGrupo = new Emendas_Model_AutorGrupo($dados['agrid']);
            $mAutorGrupo->popularDadosObjeto($dados);
            
            $mAutorGrupo->salvar(null, null, []);
            $mAutorGrupo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutorGrupo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($agrid)
    {
        $url = '?modulo=apoio/autorgrupo&acao=A';

        try {
            $mAutorGrupo = new Emendas_Model_AutorGrupo($agrid);
            
            if($mAutorGrupo->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAutorGrupo->agrid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAutorGrupo->agrstatus = 'I';
            $mAutorGrupo->salvar();
            $mAutorGrupo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutorGrupo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            