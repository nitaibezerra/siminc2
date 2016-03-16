<?php

class ProcessoObrasParControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function buscarPagamentoObrasPar($arrPost = array(), $booSQL = FALSE) {
        try {
            #Remocao de mascara do numero do processo
            if (isset($arrPost['numeroprocesso'])) {
                $arrPost['numeroprocesso'] = str_replace(array('/', '.', '-'), '', $arrPost['numeroprocesso']);
            }
           
            $obProcessoObrasPar = new ProcessoObrasPar();
            return $obProcessoObrasPar->buscarPagamentoObrasPar($arrPost, $booSQL);
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }

}
