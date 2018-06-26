<?php

class Acompanhamento_Controller_Acompanhamentopnc
{
    public function salvar($dados, $urlModulo = NULL)
    {
        $url = '?modulo=principal/monitorar-pnc&acao=A';

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc($dados['acpid']);
            $mAcompanhamento->popularDadosObjeto($dados);

            $mAcompanhamento->acoquantidade = $mAcompanhamento->acoquantidade ? desformata_valor($mAcompanhamento->acoquantidade) : null;
            $mAcompanhamento->acodata = $mAcompanhamento->acodata ? formata_data_sql($mAcompanhamento->acodata) : null;

            $mAcompanhamento->prsano = $mAcompanhamento->prsano ? $mAcompanhamento->prsano : $_SESSION['exercicio'];

            $mAcompanhamento->salvar(null, null, [
                'acoanalise',
                'acoprovidencias',
                'acojustificativa',
                'claid',
                'unocod',
                'suocod',
                'ipncod'
            ]);

            $mAcompanhamento->associarAnexo($dados);
            $this->_vincularMedidas($mAcompanhamento->acpid, $dados);
            $this->_gravarHistorico($mAcompanhamento->acpid);

            $mAcompanhamento->commit();
            simec_redirecionar($url. '&acpid='. $mAcompanhamento->acpid, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public function salvarIndicadorPnc($dados)
    {
//ver($dados, d);
        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc($dados['acpid']);
            $mAcompanhamento->popularDadosObjeto($dados);

            $mAcompanhamento->acoquantidade = $mAcompanhamento->acoquantidade? desformata_valor($mAcompanhamento->acoquantidade) : null;
            $mAcompanhamento->acodata = $mAcompanhamento->acodata?formata_data_sql($mAcompanhamento->acodata): null;

//            $mAcompanhamento->prsano = $mAcompanhamento->prsano? $mAcompanhamento->prsano: $_SESSION['exercicio'];

            $mAcompanhamento->salvar(null, null, [
                'acoanalise',
                'acoprovidencias',
                'acojustificativa',
                'claid',
                'suoid'
            ]);

//            $this->_gravarHistorico($mAcompanhamento->acpid);

            $mAcompanhamento->commit();
//ver($mAcompanhamento->acpid, d);
            return array(
                'success' => utf8_encode('Operação realizada com sucesso!'),
                'acpid' => $mAcompanhamento->acpid
            );
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            return array('error' => utf8_encode('Ocorreu um erro ao executar operação!'));
        }
    }

    /**
     * Lista os indicadores PNC do ano atual e anos anteriores
     * 
     * @param int $janid
     * @param int $suoid
     * @param int $mpnid
     * @param int $exercicio
     */
    public function listarIndicadoresPNC($janid = NULL, $suoid = NULL, $mpnid = NULL, $exercicio = NULL){
        
        include APPRAIZ. 'acompanhamento/modulos/principal/listar-indicadores-pnc.inc';
    }
    
    public function excluir($acpid)
    {
        $url = '?modulo=principal/monitorar-ppa&acao=A';

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc($acpid);
            
            if($mAcompanhamento->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcompanhamento->acpid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAcompanhamento->acostatus = 'I';
            $mAcompanhamento->salvar();
            $mAcompanhamento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    private function _gravarHistorico($acpid)
    {
        $mHistorico = new Acompanhamento_Model_Historico();

        $mHistorico->acpid = $acpid;
        $mHistorico->usucpf = $_SESSION['usucpforigem'];
        $mHistorico->hisdata = date('Y-m-d H:i:s');
        
        $mHistorico->salvar();
    }

    public function recuperarPiPorMetaESubunidade($mppid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorMetaESubunidade($mppid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarPiPorIndicadorESubunidade($ipnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorIndicadorESubunidade($ipnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }
    
    public function recuperarPiPorMetaPncESubunidade($mpnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorMetaPncESubunidade($mpnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarHistorico($acpid)
    {
        $aHistorico = (new Acompanhamento_Model_Historico())->recuperarHistorico($acpid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/historico.inc';
    }

    public function listarMetasPnc($suoid, $janid = null)
    {
        $mAcompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc();

        $mJanela = new Acompanhamento_Model_Janela();
        $janelaAtiva = $mJanela->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
        $aJanelas = $mJanela->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "janstatus = 'A'", "tipid = " . Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC], 'janinicio desc');

        include_once APPRAIZ . 'acompanhamento/modulos/principal/listar-metas-pnc.inc';
    }

}