<?php

class Solicitacao extends Modelo
{

    CONST PERGUNTA_DILIGENCIA = 3934;
    CONST PERGUNTA_CONCLUSAO = 4100;
    CONST PERGUNTA_TIPO_QUESTIONARIO = 4083;

    CONST PERGUNTA_CHECKLIST_USO_DE_SALDO = 5410;
    CONST PERGUNTA_CHECKLIST_ALTERACAO_DE_PROJETO = 5411;
    CONST PERGUNTA_CHECKLIST_TROCA_DE_SERVICO = 5412;
    CONST PERGUNTA_CHECKLIST_ALTERACAO_DE_LOCACAO = 5413;
    CONST PERGUNTA_CHECKLIST_MOBILIARIO = 5467;

    CONST TSLID_USO_DE_SALDO = 2;
    CONST TSLID_ALTERACAO_DE_PROJETO = 3;
    CONST TSLID_TROCA_DE_SERVICO = 4;
    CONST TSLID_ALTERACAO_DE_LOCACAO = 5;
    CONST TSLID_MOBILIARIO = 1;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("slcid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'slcid' => null,
        'obrid' => null,
        'slcobservacao' => null,
        'slcjustificativa' => null,
        'slcdatainclusao' => null,
        'usucpf' => null,
        'slcstatus' => null,
        'docid' => null,
        'ckfid' => null,
        'slcaprovado' => null,
    );

    /**
     * Retorna a resposta da primeira pergunta, pois a mesma é usada para enviar a solicitação para deferida quando a resposta for nao
     * @param $ckfid
     * @return array|bool|void
     */
    public function pegaRespostaTipoChecklist($ckfid)
    {
        $sql = "
            SELECT
                ckf.ckfid,
                ckf.obrid,
                ckf.usucpf,
                ckf.docid,
                ckf.ckfdatainclusao,
                qre.perid,
                itp.itptitulo,
                r2.resdsc as observacaoresposta
            FROM obras2.checklistfnde ckf
            INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
            INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
            INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
            LEFT JOIN questionario.itempergunta          i2  ON i2.itpid = qre.itpid
            LEFT JOIN questionario.pergunta              p2  ON p2.itpid = i2.itpid
            LEFT JOIN questionario.resposta              r2  ON p2.perid = r2.perid
            WHERE ckf.ckfstatus = 'A' AND ckfid = $ckfid AND qre.perid = " . self::PERGUNTA_DILIGENCIA . " -- Primeira pergunta
        ";
        $resp = $this->pegaLinha($sql);
        $resp = (empty($resp)) ? array() : $resp;
        return $resp;
    }

    /**
     * Retorna a resposta da pergunta de conclusão
     * @param $ckfid
     * @return array|bool|void
     */
    public function pegaRespostaConclusao($ckfid)
    {
        $sql = "
            SELECT
                ckf.ckfid,
                ckf.obrid,
                ckf.usucpf,
                ckf.docid,
                ckf.ckfdatainclusao,
                qre.perid,
                itp.itptitulo,
                r2.resdsc as observacaoresposta
            FROM obras2.checklistfnde ckf
            INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
            INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
            INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
            LEFT JOIN questionario.itempergunta          i2  ON i2.itpid = qre.itpid
            LEFT JOIN questionario.pergunta              p2  ON p2.itpid = i2.itpid
            LEFT JOIN questionario.resposta              r2  ON p2.perid = r2.perid
            WHERE ckf.ckfstatus = 'A' AND ckfid = $ckfid AND qre.perid = " . self::PERGUNTA_CONCLUSAO . "
        ";
        $resp = $this->pegaLinha($sql);
        $resp = (empty($resp)) ? array() : $resp;
        return $resp;
    }


    /**
     * Retorna os dados da solicitação
     * @param $slcid
     */
    public function pegaSolicitacaoPorId($slcid)
    {

        $sql = "
                    SELECT
                        s.*,
                        t.*,
                        esd.esddsc,
                        u.usunome, u.usuemail
                    FROM obras2.solicitacao s
                    JOIN obras2.tiposolicitacao_solicitacao ts ON ts.slcid = s.slcid
                    JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                    JOIN workflow.documento doc ON doc.docid = s.docid
                    JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                    JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                    WHERE
                      s.slcstatus = 'A'
                      AND s.slcid = $slcid
              ";
        $resp = $this->pegaLinha($sql);
        $resp = (empty($resp)) ? array() : $resp;

        return $resp;
    }

    /**
     * Retorna os dados da solicitação de acordo com a responsta da pergunta PERGUNTA_TIPO_QUESTIONARIO
     * @param $obrid
     * @param $itpid
     * @return array|bool|void
     */
    public function pegaSolicitacaoPorTipoQuestionario($obrid, $itpid)
    {
        $tslid = null;

        $tslid = ($itpid == self::PERGUNTA_CHECKLIST_USO_DE_SALDO) ? self::TSLID_USO_DE_SALDO : $tslid;
        $tslid = ($itpid == self::PERGUNTA_CHECKLIST_ALTERACAO_DE_PROJETO) ? self::TSLID_ALTERACAO_DE_PROJETO : $tslid;
        $tslid = ($itpid == self::PERGUNTA_CHECKLIST_TROCA_DE_SERVICO) ? self::TSLID_TROCA_DE_SERVICO : $tslid;
        $tslid = ($itpid == self::PERGUNTA_CHECKLIST_ALTERACAO_DE_LOCACAO) ? self::TSLID_ALTERACAO_DE_LOCACAO : $tslid;
        $tslid = ($itpid == self::PERGUNTA_CHECKLIST_MOBILIARIO) ? self::TSLID_MOBILIARIO : $tslid;

        $sql = "
                    SELECT
                        s.*,
                        t.*,
                        esd.esddsc,
                        esd.esdid
                    FROM obras2.solicitacao s
                    JOIN obras2.tiposolicitacao_solicitacao ts ON ts.slcid = s.slcid
                    JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                    JOIN workflow.documento doc ON doc.docid = s.docid
                    JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                    WHERE
                      s.slcstatus = 'A'
                      AND doc.esdid IN (" . ESDID_SOLICITACOES_AGUARDANDO_ANALISE . ", " . ESDID_SOLICITACOES_DILIGENCIA . ")
                      AND t.tslid = $tslid
                      AND s.obrid = $obrid
                    ORDER BY s.slcid DESC
              ";
        $resp = $this->pegaLinha($sql);
        $resp = (empty($resp)) ? array() : $resp;

        return $resp;
    }

    /**
     * Retorna a resposda da pergunta PERGUNTA_TIPO_QUESTIONARIO
     *
     * @param $ckfid
     * @return array|bool|void
     */
    public function pegaTipoQuestionario($ckfid)
    {
        $sql = "
            SELECT
                qre.perid,
                qre.*,
                itp.itptitulo
            FROM obras2.checklistfnde ckf
            INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
            INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
            INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
            LEFT JOIN questionario.itempergunta          i2  ON i2.itpid = qre.itpid
            LEFT JOIN questionario.pergunta              p2  ON p2.itpid = i2.itpid
            LEFT JOIN questionario.resposta              r2  ON p2.perid = r2.perid
            WHERE ckf.ckfstatus = 'A' AND ckfid = $ckfid AND qre.perid = " . self::PERGUNTA_TIPO_QUESTIONARIO . " -- Primeira pergunta
        ";
        $resp = $this->pegaLinha($sql);
        $resp = (empty($resp)) ? array() : $resp;

        return $resp;
    }

    /**
     * Retorna todo o questionario respondido com exceção da pergunta PERGUNTA_TIPO_QUESTIONARIO
     *
     * @param $ckfid
     * @return array|void
     */
    public function pegaQuestionarioRespondido($ckfid)
    {
        $sql = "SELECT ckf.*, qrp.*, qre.*, itp.*, r2.resdsc as observacaoresposta
                FROM obras2.checklistfnde ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid = qrp.qrpid
                INNER JOIN questionario.resposta             qre ON qrp.qrpid = qre.qrpid
                INNER JOIN questionario.itempergunta         itp ON qre.itpid = itp.itpid
                LEFT JOIN questionario.itempergunta          i2  ON i2.itpid = qre.itpid
                LEFT JOIN questionario.pergunta              p2  ON p2.itpid = i2.itpid
                LEFT JOIN questionario.resposta              r2  ON p2.perid = r2.perid
                WHERE ckf.ckfstatus = 'A' AND ckf.ckfid = $ckfid AND qre.perid != " . self::PERGUNTA_TIPO_QUESTIONARIO;

        $resp = $this->carregar($sql);
        $resp = (empty($resp)) ? array() : $resp;
        return $resp;
    }

    /**
     * Função responsável por manter a sincronia entre as solicitações e o checklist
     *
     * @param $ckfid
     */
    public function acaoChecklistSolicitacao($ckfid)
    {
        $checklistFnde = new ChecklistFnde($ckfid);
        $questionario = $this->pegaQuestionarioRespondido($ckfid);// Resposta de todas as perguntas restantes
        $tipoQuestionario = $this->pegaTipoQuestionario($ckfid); // Pergunta referente ao tipo da solicitação
        $perguntaDeferimento = $this->pegaRespostaTipoChecklist($ckfid); // A primeira pergunta do questionário é responsável por enviar para indeferido se marcado como não
        $perguntaConclusao = $this->pegaRespostaConclusao($ckfid); // A primeira pergunta do questionário é responsável por enviar para indeferido se marcado como não
        $solicitacaoObra = $this->pegaSolicitacaoPorTipoQuestionario($checklistFnde->obrid, $tipoQuestionario['itpid']); // Todos os dados da última solicitação para o mesmo tipo do checklist
        $this->popularDadosObjeto($solicitacaoObra);

        if (empty($solicitacaoObra))
            return true;

        // Sempre atualizar a última solicitação em diligincia ou aguaradndo analise com o ID do último checklist tramitado para concluido.
        $this->ckfid = $checklistFnde->ckfid;
        $this->salvar();
        $this->commit();

        $pendencias = '';
        $esdidDestino = '';
        $aedid = '';
        $situacaoSolicitacao = '';

        if (trim(str_to_upper($perguntaConclusao['itptitulo'])) == 'INDEFERIDO') {
            $pendencias = $perguntaConclusao['observacaoresposta'];
            $esdidDestino = ESDID_SOLICITACOES_INDEFERIDO;
        } else
        if (trim(str_to_upper($perguntaConclusao['itptitulo'])) == 'DEFERIDO') {
            $pendencias = $perguntaConclusao['observacaoresposta'];
            $esdidDestino = ESDID_SOLICITACOES_DEFERIDO;
        } else
        // Se a pergunta PERGUNTA_DILIGENCIA ter Não como resposta
        if (trim(str_to_upper($perguntaDeferimento['itptitulo'])) == 'NÃO') {
            $pendencias = $perguntaDeferimento['observacaoresposta'];
            $esdidDestino = ESDID_SOLICITACOES_INDEFERIDO;
        } else {
            foreach ($questionario as $questao) {
                if (trim(str_to_upper($questao['itptitulo'])) == 'NÃO') {
                    if(!empty($questao['observacaoresposta']))
                        $pendencias .= ' - ' . $questao['observacaoresposta'] . '<br>';
                }
            }
            if (!empty($pendencias)) {
                $esdidDestino = ESDID_SOLICITACOES_DILIGENCIA;
            } else {
                $esdidDestino = ESDID_SOLICITACOES_DEFERIDO;
            }
        }

        if ($esdidDestino == ESDID_SOLICITACOES_INDEFERIDO) {
            wf_alterarEstado($this->docid, AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO, strip_tags($pendencias), array());
            $this->enviaAlertaAlteracaoSolicitacao($this->slcid, AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO, $pendencias);
            $situacaoSolicitacao = 'I';
        }
        if ($esdidDestino == ESDID_SOLICITACOES_DEFERIDO) {
            $aedid = ($solicitacaoObra['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) ? AEDID_SOLICITACOES_DILIGENCIA_PARA_DEFERIDO : AEDID_SOLICITACOES_ANALISE_PARA_DEFERIDO;
            wf_alterarEstado($this->docid, $aedid, strip_tags($pendencias), array());
            $this->enviaAlertaAlteracaoSolicitacao($this->slcid, $aedid, $pendencias);
            $situacaoSolicitacao = 'D';
        }
        if ($esdidDestino == ESDID_SOLICITACOES_DILIGENCIA) {
            $v = wf_alterarEstado($this->docid, AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA, strip_tags($pendencias), array());
            $this->enviaAlertaAlteracaoSolicitacao($this->slcid, AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA, $pendencias);
            $situacaoSolicitacao = 'F';
        }

        $dadosSolicitacao = new DadosSolicitacao();
        $dadosSolicitacao->slcid = $this->slcid;
        $dadosSolicitacao->dsldata = 'NOW()';
        $dadosSolicitacao->usucpf = $_SESSION['usucpf'];
        $dadosSolicitacao->dslpendencias = $pendencias;
        $dadosSolicitacao->dsltipodesbloqueio = $situacaoSolicitacao;
        $dadosSolicitacao->salvar();

        $this->commit();
    }

    protected function enviaAlertaAlteracaoSolicitacao($slcid, $aedid, $pendencias)
    {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        $solicitacao = $this->pegaSolicitacaoPorId($slcid);
        $obra = new Obras($solicitacao['obrid']);
        $data = new Data();
        $email = new Email();
        $conteudo = '';

        switch ($aedid) {
            case AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO:
                $conteudo = "
                        <p>Sua solicitação de {$solicitacao['tslnome']}, da obra acima foi indeferida conforme análise técnica.</p>
                        <p>$pendencias</p>
                        <p></p>
                    ";
                break;
            case AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA:
                $conteudo = "
                        <p>Para nova avaliação da proposta apresentada de {$solicitacao['tslnome']} solicitamos, num prazo máximo de 20 dias a partir do recebimento desta, a resolução das pendências indicadas que segue:</p>
                        <p>$pendencias</p>
                        <p></p>
                    ";
                break;
            case AEDID_SOLICITACOES_ANALISE_PARA_DEFERIDO:
                $conteudo = "
                        <p>Sua solicitação de {$solicitacao['tslnome']}, da obra acima foi aprovada conforme análise técnica.</p>
                        <p>$pendencias</p>
                        <p></p>
                    ";
                break;
        }

        $dataF = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $ano = $data->formataData($data->dataAtual(), 'YYYY');

        $dados = array(
            'usucpf' => '',
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $email->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $dataF . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Referência: (' . $solicitacao['slcid'] . '/' . $ano . ')<br />
                                              Assunto: <b>Obra (' . $obra->obrid . ') ' . $obra->obrnome . ' - Solicitação de ' . $solicitacao['tslnome'] . ' </b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Sr. Prefeito,</p>
                                                <p>
                                                    ' . $conteudo . '
                                                </p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabio.png')) . '" />
                                                <br />
                                                <b>Fábio Lúcio de Almeida Cardoso<b>
                                                <br />
                                                Coordenador CGEST
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Obra (' . $obra->obrid . ') ' . $obra->obrnome . ' - Solicitação de ' . $solicitacao['tslnome'],
            'temid' => 36,
            'emlregistroatividade' => true,
            'obrid' => $obra->obrid
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array($solicitacao['usuemail']));
        $email->enviar();

    }
}