<?php

class ProcessoParControle extends Controle {

    public function __construct() {
        parent::__construct();
    }

    public function relatorioProrrogacaoParGenerico($arrPost = array(), $booSQL = FALSE) {
        try {
            #Remocao de mascara do numero do processo
            if (isset($arrPost['prpnumeroprocesso'])) {
                $arrPost['prpnumeroprocesso'] = str_replace(array('/', '.', '-'), '', $arrPost['prpnumeroprocesso']);
            }
            
            #Vigencia de
            if (isset($arrPost['dtvigenciainicio'])) {
                $arrPost['dtvigenciainicio'] = formata_data_sql($arrPost['dtvigenciainicio']);
            }
            
            #Vigencia de
            if (isset($arrPost['dtvigenciafim'])) {
                $arrPost['dtvigenciafim'] = formata_data_sql($arrPost['dtvigenciafim']);
            }

            $obProcessoPar = new ProcessoPar();
            $mixProcessoPar = $obProcessoPar->relatorioProrrogacaoParGenerico($arrPost, $booSQL);
            return $mixProcessoPar;
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }
    
    public function relatorioLiberacaoPagamento($arrPost = array(), $booSQL = FALSE) {
        try {
            #Remocao de mascara do numero do processo
            if (isset($arrPost['prpnumeroprocesso'])) {
                $arrPost['prpnumeroprocesso'] = str_replace(array('/', '.', '-'), '', $arrPost['prpnumeroprocesso']);
            }
            
            $obProcessoPar = new ProcessoPar();
            $mixProcessoPar = $obProcessoPar->relatorioLiberacaoPagamento($arrPost, $booSQL);
            return $mixProcessoPar;
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }
    
    public function relatorioGeralSubacao($arrPost = array(), $booSQL = FALSE) {
        try {
            #Remocao de mascara do numero do processo
            $arrPost['destino'] = $this->montarCabecalho($arrPost['arrParam'], $arrPost['destino'], true);
            if (isset($arrPost['prpnumeroprocesso'])) {
                $arrPost['prpnumeroprocesso'] = str_replace(array('/', '.', '-'), '', $arrPost['prpnumeroprocesso']);
            }
            
            $obProcessoPar = new ProcessoPar();
            $mixProcessoPar = $obProcessoPar->relatorioGeralSubacao($arrPost, $booSQL);
            return $mixProcessoPar;
        } catch (Exception $exception) {
            echo $exception->getTraceAsString();
        }
    }

    public function montarCabecalho($arrParam, $arrDestino, $booKey = false) {
        $arrcabecalho = array();
        foreach ($arrParam as $value) {
            foreach ($arrDestino as $destino) {
                if ($value['codigo'] == $destino) {
                    $arrcabecalho[] = ($booKey) ? $value['codigo'] : $value['descricao'];
                }
            }
        }
        return $arrcabecalho;
    }
}
