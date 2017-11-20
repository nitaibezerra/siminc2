<?php

class Emendas_Controller_BeneficiarioLocalizacao
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/beneficiariolocalizacao&acao=A';

        try {
            $mBeneficiarioLocalizacao = new Emendas_Model_BeneficiarioLocalizacao($dados['bloid']);
            $mBeneficiarioLocalizacao->popularDadosObjeto($dados);
            
            $mBeneficiarioLocalizacao->salvar(null, null, ['paiid', 'estuf', 'muncod', ]);
            $mBeneficiarioLocalizacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mBeneficiarioLocalizacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($bloid)
    {
        $url = '?modulo=apoio/beneficiariolocalizacao&acao=A';

        try {
            $mBeneficiarioLocalizacao = new Emendas_Model_BeneficiarioLocalizacao($bloid);
            
            if($mBeneficiarioLocalizacao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mBeneficiarioLocalizacao->bloid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mBeneficiarioLocalizacao->status = 'I';
            $mBeneficiarioLocalizacao->salvar();
            $mBeneficiarioLocalizacao->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mBeneficiarioLocalizacao->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}            