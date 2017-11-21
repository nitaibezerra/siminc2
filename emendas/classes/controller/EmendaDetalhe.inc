<?php

class Emendas_Controller_EmendaDetalhe
{
    public function salvar($dados)
    {
        try {
            $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe($dados['emdid']);
            $mEmendaDetalhe->popularDadosObjeto($dados);
            $mEmendaDetalhe->emdvalor = $mEmendaDetalhe->emdvalor ? desformata_valor($mEmendaDetalhe->emdvalor) : null;
            $mEmendaDetalhe->salvar();
            $mEmendaDetalhe->commit();
            return true;
        } catch (Exception $e){
            $mEmendaDetalhe->rollback();
            return false;
        }
    } //end salvar()
    
    public function excluir($emdid)
    {
        try {
            $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe($emdid);
            
            $mEmendaDetalhe->excluir($emdid);
            $mEmendaDetalhe->commit();
            return $mEmendaDetalhe->emeid;
        } catch (Exception $e){
            $mEmendaDetalhe->rollback();
            return false;
        }
    } //end salvar()
}            