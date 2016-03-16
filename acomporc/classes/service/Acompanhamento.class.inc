<?php
/**
 * Description of Acompanhamento
 *
 * $Id: Acompanhamento.class.inc 100482 2015-07-27 14:22:27Z maykelbraz $
 *
 * @author LindalbertoFilho
 * @see Acomporc_Service_Acompanhamento
 */
class Acomporc_Service_Acompanhamento extends Spo_Service_Abstract {

    /**
     * @see Acomporc_Service_Questionario
     */
    protected $serviceQuestionario;

    /**
     * @see Acomporc_Model_AcompanhamentoSubacao
     */
    protected $modelAcompanhamentoSubacao;

    /**
     * @see Acomporc_Model_AcompanhamentoLocalizador
     */
    protected $modelAcompanhamentoLocalizador;

    /**
     * @see Acomporc_Model_AcompanhamentoSubacaoPtres
     */
    protected $modelAcompanhamentoSubacaoPtres;

    /**
     * @see Acomporc_Model_AcompanhamentoPlanoOrcamentario
     */
    protected $modelAcompanhamentoPlanoOrcamentario;

    /**
     * @see Acomporc_Model_RelGestao
     */
    protected $modelRelGestao;

    /**
     * @see Acomporc_Model_UsuarioResponsabilidade
     */
    protected $modelUsuarioResponsabilidade;

    /**
     * @see Acomporc_Model_SnapshotLocalizador
     */
    protected $modelSnapshotLocalizador;

    /**
     * @see Acomporc_Model_SnapshotSubacao
     */
    protected $modelSnapshotSubacao;

    /**
     * @see Acomporc_Model_SnapshotPlanoOrcamentario
     */
    protected $modelSnapshotPlanoOrcamentario;

    protected function getAcpLocalizador() {
        if (!isset($this->modelAcompanhamentoLocalizador)) {
            $this->modelAcompanhamentoLocalizador = new Acomporc_Model_AcompanhamentoLocalizador();
        }

        return $this->modelAcompanhamentoLocalizador;
    }

    /**
     * Estima e retorna o período de ordem do acompanhamento de acordo com o mês em que acontece o envio do acompanhamento.
     * @return int
     */
    protected function periodoOrdemAtualSiop() {
        $mesAtual = (int) date('n');
        return ($mesAtual >= 4 && $mesAtual <= 9 ? 1 : 2);
    }

    /**
     * Retorna o momento que o SIOP recebe os dados do acompanhamento.
     * @return int
     */
    protected function momentoAcompanhamentoSiop() {
        return 2000;
    }

    /**
     * Grava acompanhamento de subação.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array(string msg,bool sucesso)
     */
    public function salvarAcompanhamento($dados) {
        global $db;
        $modelAcompanhamentoSubacao = new Acomporc_Model_AcompanhamentoSubacao();
        $modelAcompanhamentoSubacaoPtres = new Acomporc_Model_AcompanhamentoSubacaoPtres();
        $serviceQuestionario = new Acomporc_Service_Questionario();

        $_REQUEST['prfid'] = $_REQUEST['periodo'];
        $_REQUEST['usucpf'] = $_SESSION['usucpf'];
        $dados['prfid'] = $_REQUEST['prfid'];

        $resultado['msg'] = 'Erro ao salvar acompanhamento.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;

        /* Salva o Acompanhamento */
        $_REQUEST['docid'] = wf_cadastrarDocumento(TPDID_ACOMPANHAMENTO_SUBACAO, 'Em Elaboração');
        $modelAcompanhamentoSubacao->populardadosobjeto($_REQUEST);
        $asaid = $modelAcompanhamentoSubacao->salvar();
        $modelAcompanhamentoSubacao->commit();

        /* Salva os PTRES e o Questionário */
        if ($asaid) {

            /* PTRES */
            foreach ($_REQUEST['aspid'] as $ptres => $valor) {
                $sql = "SELECT
                            aspid
                        FROM
                            acomporc.acompanhamentosubacaoptres s
                        WHERE
                            sbacod = '{$_REQUEST['sbacod']}'
                        AND prfid = '{$_REQUEST['prfid']}'
                        AND ptres = '{$ptres}'";

                $dadosPtres['aspid'] = $db->pegaUm($sql);
                $dadosPtres['prfid'] = $_REQUEST['prfid'];
                $dadosPtres['sbacod'] = $_REQUEST['sbacod'];
                $dadosPtres['metafisicareprogramada'] = $_REQUEST['metafisicareprogramada'][$ptres];
                $dadosPtres['analiseexecucao'] = $_REQUEST['analiseexecucao'][$ptres];
                $dadosPtres['usucpf'] = $_SESSION['usucpf'];
                $dadosPtres['ptres'] = $ptres;
                $dadosPtres['asaid'] = $asaid;


                $modelAcompanhamentoSubacaoPtres->populardadosobjeto($dadosPtres);
                $aspid = $modelAcompanhamentoSubacaoPtres->salvar();
                $modelAcompanhamentoSubacaoPtres->commit();
            }
            /* Questionário  */

            $camposResposta = $this->_organizaCamposAcompanhamentoResposta($dados);
            $serviceQuestionario->__set('tipoAcesso', 'S');
            $serviceQuestionario->salvarResposta($camposResposta);
            #ver($_REQUEST['qprid'],d);
        }

        $resultado['msg'] = 'Acompanhamento salvo com sucesso!';
        $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;


        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    /**
     * Grava acompanhamento de ação.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array(string msg,bool sucesso)
     */
    public function salvarAcompanhamentoAcao($dados) {
        $this->modelAcompanhamentoLocalizador = new Acomporc_Model_AcompanhamentoLocalizador();
        $this->modelAcompanhamentoPlanoOrcamentario = new Acomporc_Model_AcompanhamentoPlanoOrcamentario();
        $this->serviceQuestionario = new Acomporc_Service_Questionario();

        $resultado['msg'] = 'Erro ao salvar acompanhamento.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;

        $acompanhamentoLocalizador = array(
            'aclid' => $dados['aclid'],
            'acaid' => $dados['acaid'],
            'prfid' => $dados['prfid'],
            'docid' => $dados['docid'],
            'fisicoexecutado' => $this->formatarValorMonetarioParaBanco($dados['acoexecutadofisico']),
            'fisicoexecutadorap' => $this->formatarValorMonetarioParaBanco($dados['acoexecutadorapfisico']),
            'dataapuracao' => $dados['acodataapuracao'],
            'reprogramacaofisica' => $this->formatarValorMonetarioParaBanco($dados['acoreprogramadofisico']),
            'reprogramacaofinanceira' => $this->formatarValorMonetarioParaBanco($dados['acoreprogramadofinanceiro']),
            'ultimoretornosiop' => 'N');

        if (!$acompanhamentoLocalizador['aclid']) {
            $acompanhamentoLocalizador['docid'] = wf_cadastrarDocumento(TPDID_ACOMPANHAMENTO_ACAO, 'Em Elaboração');
        }
        $this->modelAcompanhamentoLocalizador->popularDadosObjeto($acompanhamentoLocalizador);
        $acompanhamentoLocalizador['aclid'] = $dados['aclid'] = $this->modelAcompanhamentoLocalizador->gravarAcompanhamento();

        if ($acompanhamentoLocalizador['aclid']) {
            $resultado['msg'] = 'Acompanhamento salvo com sucesso!';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;

            $camposResposta = $this->_organizaCamposAcompanhamentoRespostaAcao($dados);
            if ($camposAcompanhamentoPO = $this->_organizaCamposAcompanhamentoPO($dados)) {
                if (!$this->modelAcompanhamentoPlanoOrcamentario->gravarAcompanhamento($camposAcompanhamentoPO)) {
                    $resultado['msg'] .= 'Falha ao salvar Acompanhamento dos POs';
                    $resultado['type'] = Simec_Helper_FlashMessage::AVISO;
                }
            }

            $this->serviceQuestionario->__set('tipoAcesso', 'A');
            $this->serviceQuestionario->salvarResposta($camposResposta);
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    /**
     * Organiza o array de dados para envio do acompanhamento.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array
     */
    protected function _organizaCamposAcompanhamento($dados) {
        $chaves = array_keys($dados['analiseexecucao']);
        $camposAcompanhamento = array('asaid' => $dados['asaid'], 'prfid' => $dados['prfid'], 'sbacod' => $dados['sbacod'], 'acompanhamento' => array());
        foreach ($chaves as $chave) {
            $camposAcompanhamento['acompanhamento'][] = array(
                'aspid' => $dados['aspid'][$chave],
                'ptres' => $chave,
                'analiseexecucao' => $dados['analiseexecucao'][$chave],
                'metafisicareprogramada' => str_replace('.', '', $dados['metafisicareprogramada'][$chave]));
        }

        return $camposAcompanhamento;
    }

    /**
     * Organiza o array de dados para envio do acompanhamento.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array
     */
    protected function _organizaCamposAcompanhamentoPO($dados) {
        if (!$dados['realizadoloa']) {
            return false;
        }
        $chaves = array_keys($dados['realizadoloa']);
        $camposAcompanhamento = array();
        foreach ($chaves as $chave) {
            $camposAcompanhamento[] = array(
                'acpid' => $dados['acpidHidden'][$chave],
                'sspid' => $chave,
                'aclid' => $dados['aclid'],
                'prfid' => $dados['prfid'],
                'metafisicarealizada' => $dados['realizadoloa'][$chave],
                'dataultimatualizacao' => 'now()'
            );
        }

        return $camposAcompanhamento;
    }

    /**
     * Organiza o array de respostas para envio do acompanhamento.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array
     */
    protected function _organizaCamposAcompanhamentoResposta($dados) {
        if (!$dados['qprid']) {
            return;
        }
        $chavesQuestao = array_keys($dados['qprid']);
        foreach ($chavesQuestao as $chave) {
            $camposResposta[] = array(
                'qsrid' => $dados['qpridHidden'][$chave],
                'asaid' => $dados['asaid'],
                'prfid' => $dados['prfid'],
                'qprid' => $chave,
                'sbacod' => $dados['sbacod'],
                'qsrresposta' => $dados['qprid'][$chave]);
        }
        return $camposResposta;
    }

    /**
     * Organiza o array de respostas para envio do acompanhamento.
     * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $dados
     * @return array
     */
    protected function _organizaCamposAcompanhamentoRespostaAcao($dados) {
        if (!$dados['qprid']) {
            return;
        }
        $chavesQuestao = array_keys($dados['qprid']);
        foreach ($chavesQuestao as $chave) {
            $camposResposta[] = array(
                'qlrid' => $dados['qpridHidden'][$chave],
                'aclid' => $dados['aclid'],
                'prfid' => $dados['prfid'],
                'qprid' => $chave,
                'acacod' => $dados['acacod'],
                'unicod' => $dados['unicod'],
                'loccod' => $dados['loccod'],
                'qlrresposta' => $dados['qprid'][$chave]);
        }
        return $camposResposta;
    }

    public function recuperaDadosSubacao($sbacod, $prfid) {
        $this->modelAcompanhamentoSubacao = new Acomporc_Model_AcompanhamentoSubacao();
        $this->modelAcompanhamentoSubacao->__set('sbacod', $sbacod);
        $this->modelAcompanhamentoSubacao->__set('prfid', $prfid);
        return $this->modelAcompanhamentoSubacao->pegaDadosSubacao();
    }

    public function recuperaDadosAcao($acacod, $unicod, $prfid) {
        $this->modelSnapshotLocalizador = new Acomporc_Model_SnapshotLocalizador();
        $this->modelSnapshotLocalizador->__set('acacod', $acacod);
        $this->modelSnapshotLocalizador->__set('unicod', $unicod);
        $this->modelSnapshotLocalizador->__set('prfid', $prfid);
        return $this->modelSnapshotLocalizador->pegaDadosAcao();
    }

    public function listarAcaoSubacao($sbacod, $prfid) {
        $this->modelAcompanhamentoSubacaoPtres = new Acomporc_Model_AcompanhamentoSubacaoPtres();
        $this->modelAcompanhamentoSubacaoPtres->__set('sbacod', $sbacod);
        $this->modelAcompanhamentoSubacaoPtres->__set('prfid', $prfid);
        return $this->modelAcompanhamentoSubacaoPtres->listar();
    }

    public function listarSubacao($prfid) {
        $this->modelAcompanhamentoSubacao = new Acomporc_Model_AcompanhamentoSubacao();
        $this->modelAcompanhamentoSubacao->__set('prfid', $prfid);
        return $this->modelAcompanhamentoSubacao->listar();
    }

    public function listarLocalizadores($unicod, $prfid) {
        $_REQUEST['prfid'] = $prfid;

        /**
         * Filtra apenas as unidades com ações vinculadas ao coordenador de ações
         */
        $perfil = pegaPerfilGeral();
        $permissoes = Acomporc_Model_AcompanhamentoLocalizador::getPermissaoAcoes($_SESSION['usucpf'], $perfil);

        $sql = <<<DML
            SELECT DISTINCT ssl.sslid AS codigo,
                acl.aclid,
                aca.esfcod,
                aca.funcod,
                aca.sfucod,
                ssl.unicod,
                ssl.prgcod,
                ssl.acacod,
                ssl.loccod,
                aca.acatitulo,
                aca.sacdsc,
                to_char(acl.dataultimaatualizacao, 'DD/MM/YYYY') as ultimaatualizacao,
                coalesce(esd.esddsc, 'Não Iniciado') as estado,
                acl.ultimoretornosiop as siop
            FROM acomporc.snapshotlocalizador ssl
            LEFT JOIN monitora.acao aca ON aca.acaid = ssl.acaid
            LEFT JOIN acomporc.acompanhamentolocalizador acl ON acl.acaid = ssl.acaid AND acl.prfid = ssl.prfid
            LEFT JOIN workflow.documento doc ON doc.docid = acl.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE ssl.unicod = '{$unicod}'
                AND ssl.prfid = '{$prfid}'
                {$permissoes}
            ORDER BY ssl.unicod,ssl.prgcod,ssl.acacod,ssl.loccod
DML;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array('Programática', 'Localizador', 'Última Atualização', 'Status do Acomponhamento', 'Status no SIOP'));
        $listagem->addAcao('edit', array('func' => 'acessarLocalizador', 'extra-params' => array('acacod', 'unicod')))
                ->addAcao('send', array('func' => 'enviarAcaoParaSiop', 'extra-params' => array('acacod', 'unicod')))
                ->setAcaoComoAgrupada('send', array('unicod', 'acacod'));
        $listagem->addCallbackDeCampo('siop', 'statusNoSIOP')
                ->addCallbackDeCampo('esfcod', 'comporProgramatica')
                ->addCallbackDeCampo('loccod', 'formatarAcao');
        $listagem->esconderColunas('aclid', 'unicod', 'funcod', 'sfucod', 'prgcod', 'acacod', 'sacdsc', 'acatitulo');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function resumoLocalizadores($unicod, $prfid, $acacod)
    {
        $this->modelSnapshotLocalizador = new Acomporc_Model_SnapshotLocalizador();
        $this->modelSnapshotLocalizador->unicod = $unicod;
        $this->modelSnapshotLocalizador->prfid = $prfid;
        $this->modelSnapshotLocalizador->acacod = $acacod;
        $dados = $this->modelSnapshotLocalizador->resumoLocalizadores();

        // -- Verificando status dos documentos de cada uma dos localizadores retornados
        $todosLocalizadoresOk = true;
        foreach ($dados as $localizador) {
            if (!($todosLocalizadoresOk = (ESDID_ACP_ACAO_FINALIZADO == $localizador['esdid']))) {
                break;
            }
        }

        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $list->addCallbackDeCampo('esfcod', 'comporProgramatica')
                ->setDados($dados)
                ->setFormOff()
                ->esconderColunas('unicod', 'funcod', 'sfucod', 'prgcod', 'acacod', 'loccod')
                ->setTitulo("Status dos localizadores - Ação: {$acacod}")
                ->setCabecalho(array('Programática', 'Pronto para envio?'));

        $lista = $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        $alerta = '<div class="alert alert-%s">%s</div>';
        if ($todosLocalizadoresOk) {
            $alerta = sprintf($alerta, 'success', 'Todos os localizadores já estão prontos para o envio, basta clicar em <strong>confirmar</strong> para iniciar o envio ao SIOP.');
        } else {
            $alerta = sprintf($alerta, 'danger', 'Um ou mais localizadores da ação não estão prontos para o envio.');
        }

        return array(
            'html' => $lista . $alerta,
            'podeEnviar' => $todosLocalizadoresOk,
            'acacod' => $acacod,
            'unicod' => $unicod
        );
    }

    public function listarRelatorioTCU($prfid) {
        $this->modelRelGestao = new Acomporc_Model_RelGestao();
        $this->modelRelGestao->__set('prfid', $prfid);
        return $this->modelRelGestao->listarAcompanhamento();
    }

    public function listarSubacaoPorPlocod($prfid, $prgcod, $acacod, $loccod, $unicod) {
        $this->modelSnapshotSubacao = new Acomporc_Model_SnapshotSubacao();
        $this->modelSnapshotSubacao->__set('prfid', $prfid);
        $dados = $this->modelSnapshotSubacao->listarSubacoesVinculadas($prgcod, $acacod, $loccod, $unicod);
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array('Subação', 'Sigla', 'Descrição', 'Exercício'));
        $listagem->setDados($dados);
        $listagem->esconderColuna('codigo');
        $listagem->addCallbackDeCampo('titulo', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda');
        $listagem->addAcao('view', array('func' => 'exibirSubacao', 'extra-params' => array('codigo')));
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function listarPlanoOrcamentarioPorAcao($sslid, $prfid, $aclid = null) {
        $this->modelSnapshotPlanoOrcamentario = new Acomporc_Model_SnapshotPlanoOrcamentario();
        $this->modelSnapshotPlanoOrcamentario->__set('sslid', $sslid);
        $this->modelSnapshotPlanoOrcamentario->__set('prfid', $prfid);
        $this->modelSnapshotPlanoOrcamentario->__set('aclid', $aclid);
        return $this->modelSnapshotPlanoOrcamentario->listaPlanoOrcamentario();
    }

    public function atribuirResponsavel($dados) {
        $resultado['msg'] = 'Não foi possível executar a associação do responsável.';
        $resultado['type'] = Simec_Helper_FlashMessage::ERRO;

        $this->modelUsuarioResponsabilidade = new Acomporc_Model_UsuarioResponsabilidade();
        $this->modelUsuarioResponsabilidade->usucpf = $dados['usucpf'];
        $this->modelUsuarioResponsabilidade->prfid = $dados['prfid'];
        if ($dados['sbacod']) {
            $this->modelUsuarioResponsabilidade->sbacod = $dados['sbacod'];
        } else if ($dados['unicod']) {
            $this->modelUsuarioResponsabilidade->unicod = $dados['unicod'];
        }

        if ($dados['acacod']) {
            $this->modelUsuarioResponsabilidade->acacod = $dados['acacod'];
            $this->modelUsuarioResponsabilidade->pflcod = $dados['pflcod'];
        }

        if ($this->modelUsuarioResponsabilidade->atribuirResponsavel()) {
            $resultado['msg'] = 'Associação do responsável executada com sucesso.';
            $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
        }
        $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
    }

    public function formatarValorMonetarioParaBanco($valor) {
        return str_replace(',', '.', str_replace('.', '', $valor));
    }

    public function enviarAcompanhamentoAoSiop() {
        try {
            $ambiente = Spo_Ws_Sof_Quantitativo::DEVELOPMENT;

            $acomp = new acompanhamentoOrcamentarioAcaoDTO();
            $acomp->periodoOrdem = $this->periodoOrdemAtualSiop();
            $acomp->exercicio = $_SESSION['exercicio'];
            $acomp->codigoMomento = $this->momentoAcompanhamentoSiop();

            $this->preencherAcompanhamento($acomp);
            $quantitativoSof = new Spo_Ws_Sof_Quantitativo('', $ambiente);
            $quantitativoSof->cadastrarAcompanhamentoOrcamentario($acomp);
        } catch (Exception $ex) {

        }
    }

    protected function preencherAcompanhamento(acompanhamentoOrcamentarioAcaoDTO $acomp) {
        $acpLocalizador = $this->getAcpLocalizador();
        $dados = $acpLocalizador->pegaDadosAcompanhamento(array(
            'acacod' => $this->acacod,
            'unicod' => $this->unicod,
            'prfid' => $this->prfid
        ));
        $this->preencherAcompanhamentoAcao($acomp, $dados);
        $this->preencherAcompanhamentoLocalizador($acomp, $dados);
    }

    protected function preencherAcompanhamentoAcao(acompanhamentoOrcamentarioAcaoDTO $acomp, array $dados) {
        if (empty($dados)) {
            throw new Exception("Dados do acompanhamento estão vazios: Ação {$this->acacod}; Unidade {$this->unicod}.");
        }

        $acomp->esfera = $dados[0]['esfcod'];
        $acomp->unidadeOrcamentaria = $dados[0]['unicod'];
        $acomp->funcao = $dados[0]['funcod'];
        $acomp->subFuncao = $dados[0]['sfncod'];
        $acomp->programa = $dados[0]['prgcod'];
        $acomp->acao = $dados[0]['acacod'];
        $acomp->codigoTipoInclusaoAcao = $dados[0]['tipoinclusaoacao'];
        $acomp->acompanhamentosLocalizadores = array();
    }

    protected function preencherAcompanhamentoLocalizador(acompanhamentoOrcamentarioAcaoDTO $acomp, array $dados) {
        $resumoAnalises = $this->resumoAnaliseLocalizadores();
        $resumoPlanosOrcamentarios = $this->resumoAcompanhamentoPlanosOrcamentarios($dados);

        // -- Preenchendo os dados do localizador
        foreach ($dados as $dadosLoc) {
            $acpLocalizadorDTO = new AcompanhamentoOrcamentarioLocalizadorDTO();
            $acpLocalizadorDTO->localizador = $dadosLoc['loccod'];
            $acpLocalizadorDTO->dataApuracaoLOA = $acpLocalizadorDTO->dataApuracaoRAP = $dadosLoc['dataapuracao'];
            $acpLocalizadorDTO->realizadoLOA = $dadosLoc['fisicoexecutado'];
            $acpLocalizadorDTO->realizadoRAP = $dadosLoc['fisicoexecutadorap'];
            $acpLocalizadorDTO->reprogramado = $dadosLoc['reprogramacaofisica'];
            $acpLocalizadorDTO->limite = $dadosLoc['reprogramacaofinanceira'];
            $acpLocalizadorDTO->analisesLocalizador = array();
            $this->preencherAcompanhamentoAnalises(
                    $acpLocalizadorDTO, $resumoAnalises[$dadosLoc['loccod']]
            );

            $acpLocalizadorDTO->acompanhamentosPlanoOrcamentario = array();
            $listaPos = $resumoPlanosOrcamentarios[$dadosLoc['loccod']] ? $resumoPlanosOrcamentarios[$dadosLoc['loccod']] : array();

            $this->preencherAcompanhamentoPlanoOrcamentario($acpLocalizadorDTO, $listaPos);
            $acomp->acompanhamentosLocalizadores[] = $acpLocalizadorDTO;
        }
    }

    protected function resumoAnaliseLocalizadores() {
        // -- Consultando e consolidando a análise de TODOS os localizadores
        $qstLocRespostas = new Acomporc_Model_QuestionarioLocalizadorRespostas();
        $dados = $qstLocRespostas->pegaRespostasQuetionario(array(
            'acacod' => $this->acacod,
            'unicod' => $this->unicod,
            'prfid' => $this->prfid
        ));

        $resumoAnaliseLocalizador = array();
        foreach ($dados ? $dados : array() as $dadosAnalise) {
            $resumoAnaliseLocalizador[$dadosAnalise['loccod']] = trim($dadosAnalise['qprpergunta']) . ' ' . trim($dadosAnalise['qlrresposta']);
        }

        return $resumoAnaliseLocalizador;
    }

    protected function resumoAcompanhamentoPlanosOrcamentarios(array $dados) {
        $listaAcompLoc = array();
        foreach ($dados as $dadosAcomp) {
            $listaAcompLoc[] = $dadosAcomp['aclid'];
        }

        $acpPlanoOrcamentario = new Acomporc_Model_AcompanhamentoPlanoOrcamentario();
        $dados = $acpPlanoOrcamentario->pegaAcompanhamentoPlanosOrcamentarios(
                array('aclid' => $listaAcompLoc)
        );

        $resumoAcpPlanoOrc = array();
        foreach ($dados ? $dados : array() as $dadosAcpPlanOrc) {
            if (!isset($resumoAcpPlanoOrc[$dadosAcpPlanOrc['loccod']])) {
                $resumoAcpPlanoOrc[$dadosAcpPlanOrc['loccod']] = array();
            }
            $resumoAcpPlanoOrc[$dadosAcpPlanOrc['loccod']][] = array(
                'metafisicarealizada' => $dadosAcpPlanOrc['metafisicarealizada'],
                'plocod' => $dadosAcpPlanOrc['plocod']
            );
        }

        return $resumoAcpPlanoOrc;
    }

    protected function preencherAcompanhamentoAnalises(AcompanhamentoOrcamentarioLocalizadorDTO $acpLocalizador, $analise) {
        $anlAcompOrcDTO = new AnaliseAcompanhamentoOrcamentarioDTO();
        $anlAcompOrcDTO->periodoOrdem = $this->periodoOrdemAtualSiop();
        $anlAcompOrcDTO->analise = $analise;

        $acpLocalizador->analisesLocalizador[] = $anlAcompOrcDTO;
    }

    protected function preencherAcompanhamentoPlanoOrcamentario(
    AcompanhamentoOrcamentarioLocalizadorDTO $acpLocalizador, array $listaPos
    ) {
        foreach ($listaPos as $po) {
            $acpPoDTO = new acompanhamentoPlanoOrcamentarioDTO();
            $acpPoDTO->planoOrcamentario = $po['plocod'];
            $acpPoDTO->realizadoLOA = $po['metafisicarealizada'];

            $acpLocalizador->acompanhamentosPlanoOrcamentario[] = $acpPoDTO;
        }
    }

    public function apagarSubacao($params) {
        global $db;
        $perfis = pegaPerfilGeral();

        if (in_array(PFL_CGP_GESTAO, $perfis) || in_array(PFL_SUPERUSUARIO, $perfis)) {
            #ver($params,d);
            $prfid = $params['periodo'];
            $sbacod = $params['sbacod'];

            $sql = "DELETE FROM acomporc.usuarioresponsabilidade"
                    . " WHERE prfid = $prfid "
                    . " AND upper(sbacod) = upper('{$sbacod}')";
            $db->executar($sql);

            $sql = "DELETE FROM acomporc.acompanhamentosubacao"
                    . " WHERE prfid = $prfid "
                    . " AND upper(sbacod) = upper('{$sbacod}')";
            $db->executar($sql);

            $sql = "DELETE FROM acomporc.snapshotsubacao"
                    . " WHERE prfid = $prfid "
                    . " AND upper(sbacod) = upper('{$sbacod}')";
            $db->executar($sql);
            $db->commit();
            #ver($sql,d);

            $retorno['mensagem'] = 'Apagado com sucesso.';
            return $retorno;
        } else {
            $retorno['mensagem'] = 'Acesso Negado';
            return $retorno;
        }
    }
}
