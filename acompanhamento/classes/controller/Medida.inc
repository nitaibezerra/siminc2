<?php

class Acompanhamento_Controller_Medida
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/medida&acao=A';

        try {
            $mMedida = new Acompanhamento_Model_Medida($dados['medid']);
            $mMedida->popularDadosObjeto($dados);
            
            $mMedida->salvar(null, null, []);
            $mMedida->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mMedida->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function excluir($medid)
    {
        try {
            $mMedida = new Acompanhamento_Model_Medida($medid);
            
            $mMedida->excluir($medid);
            $mMedida->commit();
            return $mMedida->claid;
        } catch (Exception $e){
            $mMedida->rollback();
            return false;
        }
    } //end salvar()
}            