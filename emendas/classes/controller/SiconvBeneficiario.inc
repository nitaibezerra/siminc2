<?php

class Emendas_Controller_SiconvBeneficiario
{
    public function salvar($dados)
    {
        try {
            $mSiconvBeneficiario = new Emendas_Model_SiconvBeneficiario($dados['sibid']);
            $mSiconvBeneficiario->popularDadosObjeto($dados->benid);

            $mSiconvBeneficiario->salvar(['benid']);
            $mSiconvBeneficiario->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSiconvBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($sibid)
    {
        $url = '?modulo=apoio/siconvsituacao&acao=A';

        try {
            $mSiconvBeneficiario = new Emendas_Model_SiconvBeneficiario($sibid);

            if($mSiconvBeneficiario->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mSiconvBeneficiario->sibid;
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mSiconvBeneficiario->sitstatus = 'I';
            $mSiconvBeneficiario->salvar();
            $mSiconvBeneficiario->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mSiconvBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
