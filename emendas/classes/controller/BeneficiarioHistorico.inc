<?php

class Emendas_Controller_BeneficiarioHistorico
{
    public function salvar($dados)
    {
        try {
            $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico($dados['behid']);
            $mBeneficiarioHistorico->popularDadosObjeto($dados);

            $mBeneficiarioHistorico->usucpf = $_SESSION['usucpforigem'];
            $mBeneficiarioHistorico->behdata = $mBeneficiarioHistorico->behdata ? formata_data_sql($mBeneficiarioHistorico->behdata) : null;

            $mBeneficiarioHistorico->salvar(null, null, []);
            $mBeneficiarioHistorico->commit();
            return true;
        } catch (Exception $e){
            $mBeneficiarioHistorico->rollback();
            return false;
        }
    } //end salvar()
    
    public function excluir($behid)
    {
        try {
            $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico($behid);

            $mBeneficiarioHistorico->behstatus = 'I';
            $mBeneficiarioHistorico->usucpf = $_SESSION['usucpforigem'];

            $mBeneficiarioHistorico->salvar();
            $mBeneficiarioHistorico->commit();
            return $mBeneficiarioHistorico->benid;
        } catch (Exception $e){
            $mBeneficiarioHistorico->rollback();
            return false;
        }
    } //end salvar()
}            