<?php

class Acompanhamento_Controller_Classificacao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/classificacao_form&acao=A';

        try {
            $mClassificacao = new Acompanhamento_Model_Classificacao($dados['claid']);
            $mClassificacao->popularDadosObjeto($dados);
            $mClassificacao->clamedida = $dados['clamedida']? 't': 'f';
            
            $mClassificacao->salvar(null, null, []);

            $mClassificacao->commit();
            simec_redirecionar($url. '&claid='. $mClassificacao->claid, 'success');
        } catch (Exception $e){
            $mClassificacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($claid)
    {
        $url = '?modulo=apoio/classificacao&acao=A';

        try {
            $mClassificacao = new Acompanhamento_Model_Classificacao($claid);
            
            if($mClassificacao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mClassificacao->claid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mClassificacao->clastatus = 'I';
            $mClassificacao->salvar();
            $mClassificacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mClassificacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarFormularioMedida($claid = null, $medid = null)
    {
        global $simec;
        
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        $simec->setPodeEditar($podeEditar);
        
        $listaMedidas = (new Acompanhamento_Model_Medida())->recuperarTodos('*', ['claid = ' . (int)$claid]);
        
        if(!$podeEditar && $listaMedidas){
            $medid = $medid? $medid: $listaMedidas[0]['medid'];
        }
        
        $mMedida = new Acompanhamento_Model_Medida($medid);

        if(!$mMedida->claid){ $mMedida->claid = $claid; }
        include_once APPRAIZ. 'acompanhamento/modulos/apoio/medida_form.inc';
    }
    
    public function montarListagemMedida($claid)
    {
        $perfis = pegaPerfilGeral();
        /**
         * @todo colcoar a constante correta do acompanhamento para atualizar a regra.
         */
        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
        
        $listaMedida = (new Acompanhamento_Model_Medida)->recuperarTodos('*', ['claid = ' . (int)$claid]);
        include_once APPRAIZ . 'acompanhamento/modulos/apoio/medida.inc';
    }


}