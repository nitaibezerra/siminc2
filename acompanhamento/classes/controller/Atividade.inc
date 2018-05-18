<?php

class Acompanhamento_Controller_Atividade
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/atividade&acao=A';

        try {
            $mAtividade = new Acompanhamento_Model_Atividade($dados['atiid']);
            $mAtividade->popularDadosObjeto($dados);
            
            $mAtividade->salvar(null, null, ['atidata', 'atiproduto', 'atiquantidade', 'estuf', 'muncod', ]);
            $mAtividade->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAtividade->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($atiid)
    {
        $url = '?modulo=apoio/atividade&acao=A';

        try {
            $mAtividade = new Acompanhamento_Model_Atividade($atiid);
            
            if($mAtividade->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAtividade->atiid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAtividade->atistatus = 'I';
            $mAtividade->salvar();
            $mAtividade->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAtividade->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function montarFormulario($atiid = null, $bedid = null)
    {
        global $simec;
        $mAtividade = new Acompanhamento_Model_Atividade($bedid);
        if(!$mAtividade->atiid){ $mAtividade->atiid = $atiid; }
        include_once APPRAIZ . 'acompanhamento/modulos/principal/atividade_form.inc';
    }

    public function montarListagem($atiid)
    {
        $aAtividade = (new Acompanhamento_Model_Atividade)->recuperarListagem($atiid);
        include_once APPRAIZ . 'acompanhamento/modulos/principal/atividade.inc';
    }
}            