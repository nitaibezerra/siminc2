<?php

class HistoricoSolicitacaoReprogramacaoControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function relatorioProrrogacaoParGenerico($arrPost = array(), $booSQL = FALSE) {
        try {
            $obProcessoPar = new ProcessoPar();
            $mixProcessoPar = $obProcessoPar->relatorioProrrogacaoParGenerico($arrPost, $booSQL);
            return $mixProcessoPar;
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }
    
    public function relatorioHistoricoSolicitacaoReprogramacao($arrPost = array(), $booSQL = FALSE) {
        try {
            #Remocao de mascara do numero do processo
            if (isset($arrPost['prpnumeroprocesso'])) {
                $arrPost['prpnumeroprocesso'] = str_replace(array('/', '.', '-'), '', $arrPost['prpnumeroprocesso']);
            }
            
            $obHistSolicitacaoRep = new HistoricoSolicitacaoReprogramacao();
            $mixHistSolicitacaoRep = $obHistSolicitacaoRep->relatorioHistoricoSolicitacaoReprogramacao($arrPost, $booSQL);
            return $mixHistSolicitacaoRep;
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }

}
