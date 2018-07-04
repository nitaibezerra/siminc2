<?php

class Public_Controller_ArquivoModulo
{
    public function salvar($dados)
    {
        try {
            $mdarquivoModulo = new Public_Model_ArquivoModulo($dados['arqmdid']);
            $mdarquivoModulo->popularDadosObjeto($dados);
            $mdarquivoModulo->salvar();
            $mdarquivoModulo->commit();
        } catch (Exception $e){
            $mdarquivoModulo->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($dados)
    {
        try {
            $mdarquivoModulo = new Public_Model_ArquivoModulo($dados['arqmdid']);
            $mdarquivoModulo->excluirArquivo();
            $mdarquivoModulo->commit();
        } catch (Exception $e){
            $mdarquivoModulo->rollback();
        }
    } //end salvar()
}
