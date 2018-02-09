<?php

class Acompanhamento_Controller_Acompanhamentoarquivo
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/acompanhamentoarquivo&acao=A';

        try {
            $mAcompanhamentoarquivo = new Acompanhamento_Model_Acompanhamentoarquivo($dados['acaid']);
            $mAcompanhamentoarquivo->popularDadosObjeto($dados);
            
            $mAcompanhamentoarquivo->salvar(null, null, []);
            $mAcompanhamentoarquivo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamentoarquivo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($acaid)
    {
        $url = '?modulo=apoio/acompanhamentoarquivo&acao=A';

        try {
            $mAcompanhamentoarquivo = new Acompanhamento_Model_Acompanhamentoarquivo($acaid);
            
            if($mAcompanhamentoarquivo->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcompanhamentoarquivo->acaid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAcompanhamentoarquivo->acastatus = 'I';
            $mAcompanhamentoarquivo->salvar();
            $mAcompanhamentoarquivo->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamentoarquivo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            