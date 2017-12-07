<?php

class Emendas_Controller_BeneficiarioDetalhe
{
    public function salvar($dados)
    {
        try {
            $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe($dados['bedid']);
            $mBeneficiarioDetalhe->popularDadosObjeto($dados);
            $mBeneficiarioDetalhe->bedvalor = $mBeneficiarioDetalhe->bedvalor ? desformata_valor($mBeneficiarioDetalhe->bedvalor) : null;
            $mBeneficiarioDetalhe->bedvalorpriorizado = $mBeneficiarioDetalhe->bedvalorpriorizado ? desformata_valor($mBeneficiarioDetalhe->bedvalorpriorizado) : null;

            $mBeneficiarioDetalhe->salvar(null, null, ['bedvalor', 'bedvalorpriorizado', ]);
            $mBeneficiarioDetalhe->commit();
            return true;
        } catch (Exception $e){
            $mBeneficiarioDetalhe->rollback();
            return false;
        }
    } //end salvar()
    
    public function excluir($bedid)
    {
        try {
            $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe($bedid);

            if($mBeneficiarioDetalhe->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mBeneficiarioDetalhe->bedid;
                simec_redirecionar($url, 'error', $mensagem);
            }
            
            $mBeneficiarioDetalhe->excluir($bedid);
            $mBeneficiarioDetalhe->commit();
            return $mBeneficiarioDetalhe->benid;
        } catch (Exception $e){
            $mBeneficiarioDetalhe->rollback();
            return false;
        }
    } //end salvar()
}            