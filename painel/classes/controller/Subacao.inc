<?php

class Painel_Controller_Subacao
{
    public function salvar($dados)
    {
        try {
            $mSubacao = new Painel_Model_Subacao($dados['subid']);
            $mSubacao->popularDadosObjeto($dados);

            $mSubacao->subdatainicio = $mSubacao->subdatainicio ? formata_data_sql($mSubacao->subdatainicio) : null;
            $mSubacao->subdatafim = $mSubacao->subdatafim ? formata_data_sql($mSubacao->subdatafim) : null;

            $mSubacao->salvar(null, null, ['subdatainicio', 'subdatafim', 'subtexto']);
            $mSubacao->commit();
            return true;
        } catch (Exception $e){
            $mSubacao->rollback();
            return false;
        }
    } //end salvar()

    public function excluir($subid)
    {
        try {
            $mSubacao = new Painel_Model_Subacao($subid);

            if($mSubacao->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com outro registro.';
                return false;
            }

            $mSubacao->substatus = 'I';
            $mSubacao->salvar();
            $mSubacao->commit();
            return true;
        } catch (Exception $e){
            $mSubacao->rollback();
            return false;
        }
    } //end salvar()
}