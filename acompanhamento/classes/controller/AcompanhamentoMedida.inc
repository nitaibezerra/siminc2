<?php

class Acompanhamento_Controller_AcompanhamentoMedida
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/acompanhamentomedida&acao=A';

        try {
            $mAcompanhamentoMedida = new Acompanhamento_Model_AcompanhamentoMedida($dados['acmid']);
            $mAcompanhamentoMedida->popularDadosObjeto($dados);
            
            $mAcompanhamentoMedida->salvar(null, null, []);
            $mAcompanhamentoMedida->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamentoMedida->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($acmid)
    {
        $url = '?modulo=apoio/acompanhamentomedida&acao=A';

        try {
            $mAcompanhamentoMedida = new Acompanhamento_Model_AcompanhamentoMedida($acmid);
            
            if($mAcompanhamentoMedida->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcompanhamentoMedida->acmid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAcompanhamentoMedida->salvar();
            $mAcompanhamentoMedida->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamentoMedida->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            