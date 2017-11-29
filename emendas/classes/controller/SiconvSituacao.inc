<?php

class Emendas_Controller_SiconvSituacao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/siconvsituacao&acao=A';

        try {
            $mSiconvSituacao = new Emendas_Model_SiconvSituacao($dados['sitid']);
            $mSiconvSituacao->popularDadosObjeto($dados);
            
            $mSiconvSituacao->salvar(null, null, ['sitcodigo', ]);
            $mSiconvSituacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSiconvSituacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($sitid)
    {
        $url = '?modulo=apoio/siconvsituacao&acao=A';

        try {
            $mSiconvSituacao = new Emendas_Model_SiconvSituacao($sitid);
            
            if($mSiconvSituacao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mSiconvSituacao->sitid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mSiconvSituacao->sitstatus = 'I';
            $mSiconvSituacao->salvar();
            $mSiconvSituacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSiconvSituacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            