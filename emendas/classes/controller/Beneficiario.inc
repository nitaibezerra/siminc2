<?php

class Emendas_Controller_Beneficiario
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($dados['benid']);
            $mBeneficiario->popularDadosObjeto($dados);

            $mBeneficiario->benempenhado = $dados['benempenhado'] == 't' ? 't' : 'f';
            $mBeneficiario->benconveniado = $dados['benconveniado'] == 't' ? 't' : 'f';
            $mBeneficiario->benpago = $dados['benpago'] == 't' ? 't' : 'f';
            $mBeneficiario->bented = $dados['bented'] == 't' ? 't' : 'f';

            $mBeneficiario->salvar(null, null, ['suoid', 'proid', 'esfid', 'benformaexecucao', 'bennumeroprocesso', 'beninicio', 'benpprogramanumero', 'benpropostanumero', 'benpropostatitulo', 'benpropostaobjeto', 'impid']);

            $mBeneficiario->associarDelegacao($dados);
            $mBeneficiario->associarAlteracao($dados);

            // Salvando dados da Localização
            $dados['benid'] = $mBeneficiario->benid;
            (new Emendas_Controller_BeneficiarioLocalizacao())->salvar($dados);

            $mBeneficiario->commit();
            simec_redirecionar($url . '&benid=' . $mBeneficiario->benid, 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($benid)
    {
        $url = '?modulo=apoio/beneficiario&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($benid);
            
            if($mBeneficiario->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mBeneficiario->benid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mBeneficiario->benstatus = 'I';
            $mBeneficiario->salvar();
            $mBeneficiario->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarFormularioHistorico($benid = null, $behid = null)
    {
        global $simec;
        $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico($behid);
        if(!$mBeneficiarioHistorico->benid){ $mBeneficiarioHistorico->benid = $benid; }
        $mBeneficiarioHistorico->behdata = $mBeneficiarioHistorico->behdata = date('Y-m-d');
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariohistorico_form.inc';
    }

    public function montarListagemHistorico($benid)
    {
        $aBeneficiarioHistorico = (new Emendas_Model_BeneficiarioHistorico)->recuperarTodos('*', ['benid = ' . (int)$benid, " behstatus = 'A' "], 'behdata desc');
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariohistorico.inc';
    }

    public function montarFormularioDetalhe($benid = null, $bedid = null)
    {
        global $simec;
        $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe($bedid);
        if(!$mBeneficiarioDetalhe->benid){ $mBeneficiarioDetalhe->benid = $benid; }
        $mBeneficiario = new Emendas_Model_Beneficiario($mBeneficiarioDetalhe->benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe_form.inc';
    }

    public function montarListagemDetalhe($benid)
    {
        $aBeneficiarioDetalhe = (new Emendas_Model_BeneficiarioDetalhe)->recuperarTodos('*', ['benid = ' . (int)$benid, " bedstatus = 'A' "]);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe.inc';
    }
}