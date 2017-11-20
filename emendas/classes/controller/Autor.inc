<?php

class Emendas_Controller_Autor
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/autor&acao=A';

        try {
            $mAutor = new Emendas_Model_Autor($dados['autid']);
            $mAutor->popularDadosObjeto($dados);
            
            $mAutor->salvar(null, null, ['autemail', 'autcod', ]);
            $mAutor->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutor->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($autid)
    {
        $url = '?modulo=apoio/autor&acao=A';

        try {
            $mAutor = new Emendas_Model_Autor($autid);
            
            if($mAutor->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAutor->autid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAutor->autstatus = 'I';
            $mAutor->salvar();
            $mAutor->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAutor->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            