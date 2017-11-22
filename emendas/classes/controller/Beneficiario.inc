<?php

class Emendas_Controller_Beneficiario
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($dados['benid']);
            $mBeneficiario->popularDadosObjeto($dados);
            
            $mBeneficiario->salvar(null, null, ['suoid', 'proid', 'esfid', 'benformaexecucao', 'bennumeroprocesso', 'beninicio', 'benpprogramanumero', 'benpropostanumero', 'benpropostatitulo', 'benpropostaobjeto']);

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
}