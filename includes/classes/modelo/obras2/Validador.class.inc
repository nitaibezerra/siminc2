<?php

class Validador extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.validador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vdrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(

        'vdrid' => 'null',
        'vdrfuncao' => 'null',
        'vdrdescricao' => 'null',
        'vdrstatus' => 'null',
        'vdrdatainclusao' => 'null',
        'vdrobrigatorio' => 'null'
    );

    protected $_message = '';

    public function getMessage()
    {
        return $this->vdrdescricao;
    }

    public function check($obrid, $vdrid = null)
    {
        if ($vdrid)
            $this->carregarPorId($vdrid);
        return $this->{$this->vdrfuncao}($obrid);
    }

    public function notasFiscais($obrid)
    {
        $sql = "
            SELECT
                obrid
            FROM obras2.pagamentosexecucaoorcamentaria peo
            WHERE peostatus = 'A' AND obrid = $obrid AND peoarqid_notafiscal IS NOT NULL
            GROUP BY obrid
        ";
        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    public function comprovantesPagamentos($obrid)
    {
        $sql = "
            SELECT
                obrid
            FROM obras2.pagamentosexecucaoorcamentaria peo
            WHERE peostatus = 'A' AND obrid = $obrid AND peoarqid_ordembancaria IS NOT NULL
            GROUP BY obrid
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * Homologação da licitação deve estar anexada na aba licitação ou na aba documentos com o tipo de arquivo "Homologação da Licitação"?
     * @param $obrid
     */
    public function homologacaoLicitacaoAnexo($obrid)
    {
        $sql = "
            SELECT
                o.obrid
            FROM obras2.obras o
            JOIN obras2.obralicitacao ol ON ol.obrid = o.obrid AND oblstatus = 'A'
            JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND fl.flcstatus = 'A' AND tflid = 9
            JOIN obras2.arquivolicitacao al ON al.flcid = fl.flcid
            WHERE o.obrid = $obrid
        ";
        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * O contrato assinado deve estar anexado na aba contratação?
     * @param $obrid
     */
    public function contratoAssinadoAnexo($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                c.arqid
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid AND c.arqid IS NOT NULL
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A ordem de serviços deve estar anexada na aba contratação?
     * @param $obrid
     */
    public function ordemDeServicoAnexo($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                oc.arqidos
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A planilha vencedora da licitação deve estar anexada na aba contratação?
     * @param $obrid
     */
    public function planilhaVencedoraAnexo($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                oc.arqidcusto
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * A rescisão contratual deve estar anexada na vistoria de paralisação que informa a rescisão contratual?
     * @param $obrid
     */
    public function rescisaoContraturalAnexo($obrid)
    {
        $sql = "
                SELECT
                  h.arqid
                FROM obras2.supervisao s
                INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                INNER JOIN obras2.historicoparalisacao h ON h.supidparalisacao = s.supid
                WHERE
                    s.obrid = $obrid
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'::bpchar
                    AND s.validadapelosupervisorunidade = 'S'::bpchar
                    AND s.usucpf IS NOT NULL AND s.rsuid = 1
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY
                supdata DESC LIMIT 1
            ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * O boletim de medição acumulada deve ser anexo na vistoria de paralisação que informa a rescisão contratual?
     * @param $obrid
     */
    public function boletimMedicaoAcumuladaAnexo($obrid)
    {

        $sql = "
                SELECT
                  h.arqidboletim
                FROM obras2.supervisao s
                INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                INNER JOIN obras2.historicoparalisacao h ON h.supidparalisacao = s.supid
                WHERE
                    s.obrid = $obrid
                    AND s.emsid IS NULL
                    AND s.smiid IS NULL
                    AND s.supstatus = 'A'::bpchar
                    AND s.validadapelosupervisorunidade = 'S'::bpchar
                    AND s.usucpf IS NOT NULL AND s.rsuid = 1
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY
                supdata DESC LIMIT 1
            ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * ART de execução deve ser anexada na aba vistoria?
     * @param $obrid
     */
    public function ARTExecucaoAnexo($obrid)
    {
        $sql = "
            SELECT obrid FROM obras2.obras_arquivos  WHERE obrid = $obrid AND tpaid = 25;
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * ART de fiscalização deve ser anexada na aba documentos?
     * @param $obrid
     */
    public function ARTFiscalizacaoAnexo($obrid)
    {
        $sql = "
            SELECT obrid FROM obras2.obras_arquivos  WHERE obrid = $obrid AND tpaid = 26;
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    /**
     * o contrato com a construtora está vigente?
     * @param $obrid
     * @return bool
     */
    public function contratoVigente($obrid)
    {
        if(obraMi($obrid))
            return true;

        $obra     = new Obras( $obrid );
        $crtid    = $obra->pegaContratoPorObra( $obrid );
        $contrato = new Contrato( $crtid );
        $dados    = $contrato->getDados();

        $dateT = new DateTime($dados['crtdttermino']);
        $date = new DateTime();
        return ($dateT > $date) ? true : false;
    }

    /**
     * Os campos da Ordem de Serviço estão preenchidos?
     * @param $obrid
     * @return bool
     */
    public function dadosOrdemDeServico($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                oc.*
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid
        ";

        $result = $this->pegaLinha($sql);

        if(empty($result['ocrdtordemservico'])) return false;
        if(empty($result['ocrdtinicioexecucao'])) return false;
        if(empty($result['ocrcustounitario'])) return false;
        if(empty($result['ocrdtterminoexecucao'])) return false;
        if(empty($result['arqidos'])) return false;
        if(empty($result['ocrvalorexecucao'])) return false;
//        if(empty($result['umdid'])) return false;
        if(empty($result['ocrcustounitario'])) return false;
        if(empty($result['ocrpercentualdbi'])) return false;
        if(empty($result['arqidcusto'])) return false;

        return true;
    }

    /**
     * O campo "Valor do contrato e Valor contratado", estão preenchidos com o mesmo valor?
     * @param $obrid
     * @return bool
     */
    public function valorDoContratoValorContratado($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                c.crtvalorexecucao,
                oc.ocrvalorexecucao
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid
        ";

        $result = $this->pegaLinha($sql);

        return ($result['crtvalorexecucao'] = $result['ocrvalorexecucao']) ? true : false;
    }

    /**
     * O campo "Valor Restante" esta zerado?
     * Se a obra não tiver nenhum desembolso (situação efetivado ou solicitado) permitir que o município encaminhe a solicitação, pois se trata da 1ª parcela que deverá ocorrer por ocasião da emissão da OS.
     *
     * @param $obrid
     * @return bool
     */
    public function valorContratadoEValorCronograma($obrid)
    {
        if(obraMi($obrid))
            return true;

        $sql = "
            SELECT
                SUM(ico.icovlritem)
            FROM obras2.cronograma c
            JOIN obras2.itenscomposicaoobra ico ON ico.obrid = c.obrid AND ico.icostatus = 'A' AND c.croid = ico.croid
            JOIN  obras2.itenscomposicao i ON i.itcid = ico.itcid AND i.itcstatus = 'A'
            WHERE c.obrid = $obrid AND c.crostatus = 'A'
        ";

        $icovlritem = $this->pegaUm($sql);

        $obra     = new Obras( $obrid );

        /*
        Se a obra não tiver nenhum desembolso (situação efetivado ou solicitado) permitir que o município encaminhe a solicitação,
        pois se trata da 1ª parcela que deverá ocorrer por ocasião da emissão da OS.
        */

        $repasse = $obra->pegaUm("
            SELECT
                o.obrid
            FROM obras2.obras o
            WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid NOT IN (

                SELECT DISTINCT o.obrid FROM obras2.obras o
                JOIN par.pagamentoobra po ON po.preid = o.preid
                JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                UNION
                SELECT DISTINCT o.obrid FROM obras2.obras o
                JOIN par.pagamentoobrapar po ON po.preid = o.preid
                JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'

            ) AND o.obrid = $obrid
        ");

        if($repasse){
            return true;
        }

        $obraContrato = new ObrasContrato();
        $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);

        return ($ocrvalorexecucao == $icovlritem) ? true : false;
    }

    /**
     * A última vistoria possui fotos em quantidade adequada?
     * Quando a tipologia da obra for quadra e cobertura => igual ou maior que 6 fotos
     * Quando for as demais tipologias de obras => igual ou maior do que 12 fotos
     *
     * @param $obrid
     * @return bool
     */
    public function qtdFotosUltVistoria($obrid)
    {
        $supervisao = new Supervisao();
        $supData = $supervisao->pegaUltimaVistoriaUnidade($obrid);

        $sql = "SELECT
                			fot.*, arq.arqdescricao , c.itcdesc
                		FROM
                			obras2.fotos AS fot
                        LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
                        LEFT JOIN obras2.itenscomposicaoobra i ON i.icoid = fot.icoid
                        LEFT JOIN obras2.itenscomposicao c ON c.itcid = i.itcid
                        WHERE
                        	fot.obrid =" . $obrid . " AND
                        	supid=" . $supData['supid'] . "
                        ORDER BY fotordem;";

        $fotos = $this->carregar($sql);
        $total = count($fotos);

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obra->empid);
        $arrTpoQuadraCobertura = array(18, 21, 17, 22, 20, 19);

        if(in_array($empreendimento->tpoid, $arrTpoQuadraCobertura)){
            $this->vdrdescricao = 'Inserir vistoria com 6 fotos, no mínimo, em ângulos que permitam a verificação pelo FNDE dos itens executados citados na vistoria, além de uma foto geral da obra. Também nessa vistoria, no "relatório técnico de acompanhamento" comentários sobre cada item executado e a situação geral da obra.';
            return ($total >= 6) ? true : false;
        } else {
            $this->vdrdescricao = 'Inserir vistoria com 12 fotos, no mínimo, em ângulos que permitam a verificação pelo FNDE dos itens executados citados na vistoria, além de uma foto geral da obra. Também nessa vistoria, no "relatório técnico de acompanhamento" comentários sobre cada item executado e a situação geral da obra.';
            return ($total >= 12) ? true : false;
        }

    }

    /**
     * o percentual de execução da obra, informado na última vistoria, está maior ou igual ao valor desembolsado pelo FNDE?
     *
     * b.	Considerar como desembolso FNDE somente a situação 2-EFETIVADO;
     * c.	A obra tem que estar com % de execução maior do que 0%
     * d.	OU caso a obra esteja com 0% de execução, não deve existir valor desembolsado pelo FNDE na situação 2-EFETIVADO
     *
     * @param $obrid
     * @return bool
     */
    public function percUltVistoriaEDesembolso($obrid)
    {
        $sql = "
                SELECT
                  totalpago, vlrobra, (totalpago*100) / vlrobra as percentual
                FROM (
                    SELECT DISTINCT pre.preid,
                        obra.obrid,
                        pre.preesfera AS esfera,
                        pre.estuf AS uf,
                        pre.muncod,
                        pre.predescricao AS nomeobra,
                        pre.prevalorobra AS vlrobra,
                        sum(pop.popvalorpagamento) AS totalpago,
                        'PAR'::text AS tipopagamento
                       FROM obras.preobra pre
                         JOIN par.pagamentoobrapar pop ON pop.preid = pre.preid
                         JOIN par.pagamento pag ON pop.pagid = pag.pagid AND pag.pagstatus = 'A'::bpchar AND pag.pagsituacaopagamento::text = '2 - EFETIVADO'::text
                         LEFT JOIN obras2.obras obra ON pre.preid = obra.preid
                      WHERE pre.prestatus = 'A'::bpchar
                      GROUP BY pre.preid, obra.obrid, pre.preesfera, pre.estuf, pre.muncod, pre.predescricao, pre.prevalorobra
                    UNION ALL
                     SELECT DISTINCT pre.preid,
                        obra.obrid,
                        pre.preesfera AS esfera,
                        pre.estuf AS uf,
                        pre.muncod,
                        pre.predescricao AS nomeobra,
                        pre.prevalorobra AS vlrobra,
                        sum(pop.pobvalorpagamento) AS totalpago,
                        'PAC'::text AS tipopagamento
                       FROM obras.preobra pre
                         JOIN par.pagamentoobra pop ON pop.preid = pre.preid
                         JOIN par.pagamento pag ON pop.pagid = pag.pagid AND pag.pagstatus = 'A'::bpchar AND pag.pagsituacaopagamento::text = '2 - EFETIVADO'::text
                         LEFT JOIN obras2.obras obra ON pre.preid = obra.preid
                      WHERE pre.prestatus = 'A'::bpchar
                      GROUP BY pre.preid, obra.obrid, pre.preesfera, pre.estuf, pre.muncod, pre.predescricao, pre.prevalorobra
                ) as pg
                WHERE pg.obrid = $obrid
        ";

        $pagamento = $this->pegaLinha($sql);
        $obra = new Obras($obrid);
        $perc = $obra->pegaPercentualExecucao($obrid);

        if ($perc > 0 || $pagamento['percentual'] <> 0) {
            if ($perc >= $pagamento['percentual']) {
                return true;
            }
        }

        return false;
    }

    /**
     * As restrições já foram superadas, ou encaminhadas para análise do FNDE?
     * a.	Mesmo que a restrição não esteja superada, desde que o município tenha encaminhado a solicitação ao FNDE (que não analisou), deve ser permitido a remessa do pedido de desembolso.
     * @param $obrid
     * @return bool
     */
    public function restricoesNaoSuperadas($obrid)
    {
        $sql = "
            SELECT
                r.obrid
            FROM obras2.restricao r
            JOIN workflow.documento d ON d.docid = r.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497, 1141)
            WHERE r.obrid = $obrid AND rstitem IN ('R') AND rststatus = 'A'

        ";

        $result = $this->pegaUm($sql);
        return (!$result) ? true : false;
    }

    public function vigenciaTermo($obrid)
    {
        $sql = "
        
            SELECT 
                (((string_to_array(\"Fim Vigência Termo\", '/'))[3] || '-' || (string_to_array(\"Fim Vigência Termo\", '/'))[2] || '-' || (string_to_array(\"Fim Vigência Termo\", '/'))[1])::date ) <= NOW() + '30 days'::interval
            FROM obras2.vm_termo_obras
            WHERE \"ID Obra\" = $obrid
            AND \"Preid\" NOT IN (SELECT preid FROM obras.preobraprorrogacao  WHERE popstatus != 'I')
        ";

        $result = $this->pegaUm($sql);
        return ($result !== 't') ? true : false;
    }

    /**
     * O contrato com a construtora está vigente?
     * @param $obrid
     */
    public function contratoAnexo($obrid)
    {
        $sql = "
            SELECT
                c.arqidcontrato
            FROM obras2.obras o
            JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
            JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            WHERE o.obrid = $obrid AND c.arqidcontrato IS NOT NULL
        ";

        $result = $this->pegaUm($sql);

        return ($result) ? true : false;
    }

    public function verificacaoGeralParPac($obrid, $podidPARPP = array(), $podidPARAR = array(), $podidPACPP = array())
    {
        $sql = "SELECT preid, tpoid, tooid FROM obras2.obras WHERE obrid = $obrid AND obrstatus = 'A'";
        $res = $this->pegaLinha($sql);
        if(!$res['preid']) {
            return 'n';
        }

        $preObra = $this->buscaPtoid($res['preid']);
        $ptoid = $preObra['ptoid'];
        if ($preObra['ptoprojetofnde'] == 't') {
            return 'n';
        }
        
        if ($ptoid == '16' || $ptoid == '17' || $ptoid == '18' || $ptoid == '56' || $ptoid == '64' || $ptoid == '65'|| $ptoid == '66') {
            //Verificação Projeto Proprio PAR
            if($ptoid == '16' || $ptoid == '56') {
                if(count($podidPARPP) == 0) {
                    return 'n';
                }
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARPP);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARPP);
                $resposta = $result == count($podidPARPP) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);
            }
            if(count($podidPARAR) == 0) {
                return 'n';
            }
            //Ampliação e Reforma
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARAR);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARAR);
            $resposta = $result == count($podidPARAR) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);

        } else if ($ptoid == '1' || $ptoid == '21' || $ptoid == '9') {
            //Projeto Proprio PAC
            if(count($podidPACPP) == 0) {
                return 'n';
            }
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACPP);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACPP);
            $resposta = $result == count($podidPACPP) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else return 'n';
    }

    public function buscaPtoid($preid)
    {
        $sql = <<<DML
            SELECT
                po.ptoid,
                pto.ptoprojetofnde,
                po.tooid
            FROM obras.preobra po
            JOIN obras.pretipoobra pto ON (po.ptoid = pto.ptoid)
            WHERE po.preid = $preid
DML;
        return $this->pegaLinha($sql);
    }

    public function buscaPreObraAnexo($preid,$podid)
    {
        $in = implode(', ', $podid);
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras.preobraanexo anx
            INNER JOIN public.arquivo arq ON arq.arqid = anx.arqid AND anx.poasituacao = 'A'
            WHERE anx.preid = $preid
                AND anx.podid IN ($in)
                AND anx.poasituacao = 'A'
DML;
        $resp = $this->carregar($sql);
        return $resp ? $resp : array();
    }

    public function verificaQuantidadePreObra($preid,$podid)
    {
        $in = implode(', ', $podid);
        $sql = <<<DML
            SELECT
                COUNT(DISTINCT anx.podid)
            FROM obras.preobraanexo anx
            INNER JOIN public.arquivo arq ON arq.arqid = anx.arqid AND anx.poasituacao = 'A'
            WHERE anx.preid = $preid
                AND anx.podid IN ($in)
                AND anx.poasituacao = 'A'
DML;
        return $this->pegaUm($sql);
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 1) O Projeto Pactuado está inserido?
     * Possui arquivos.
     */
    public function questaoProjetoPactuado($obrid)
    {
        return $this->verificacaoGeralParPac($obrid,array(13,14,15,16),array(13),array(13,14,15,16));
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 2) O Memorial Descritivo e Especificações Técnicas estão inseridas?
     */
    public function questaoMDET($obrid)
    {
        return $this->verificacaoGeralParPac($obrid,array(17),array(17),array(17));
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 3) A Planilha pactuada está inserida?
     */
    public function questaoPlanilhaPactuada($obrid)
    {
        return $this->verificacaoGeralParPac($obrid,array(10),array(10),array(10));
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * #Não pode retornar Não se aplica
     * Item Composição: Documentos da Obra.
     * 8) O Comprovante de dominialidade definitivo está inserido?
     */
    public function questaoComprovanteDominialidade($obrid)
    {
        $sql = "
            SELECT obr.preid, obr.tpoid, obr.tooid, emp.empesfera
            FROM obras2.obras obr
            INNER JOIN obras2.empreendimento emp ON (obr.empid = emp.empid)
            WHERE obrid = $obrid
                AND obrstatus = 'A'
            ";
        $res = $this->pegaLinha($sql);
        if(!$res['preid']) {
            return 'f';
        }
        $podidPACFNDE = array(9);
        $podidPARFNDE = array(21);
        $podidPARPP = array(21);
        $podidPARAR = array(21);
        $podidPACPP = array(19);

        $preObra = $this->buscaPtoid($res['preid']);
        $ptoid = $preObra['ptoid'];
        if ($res['tooid'] == '1' && $preObra['ptoprojetofnde'] == 't') {
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACFNDE);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACFNDE);
            $resposta = $result == count($podidPACFNDE) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else if ($preObra['ptoprojetofnde'] == 't'){
            $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARFNDE);
            $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARFNDE);
            $resposta = $result == count($podidPARFNDE) ? 't' : 'f';
            return array('resposta' => $resposta, 'arquivos' => $arquivos);
        } else {
            if ($ptoid == '16' || $ptoid == '17' || $ptoid == '18' || $ptoid == '56' || $ptoid == '64' || $ptoid == '65'|| $ptoid == '66') {
                //Verificação Projeto Proprio PAR
                if($ptoid == '16' || $ptoid == '56') {
                    $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARPP);
                    $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARPP);
                    $resposta = $result == count($podidPARPP) ? 't' : 'f';
                    return array('resposta' => $resposta, 'arquivos' => $arquivos);
                }
                //Ampliação e Reforma
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPARAR);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPARAR);
                $resposta = $result == count($podidPARAR) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);

            } else if (($ptoid == '1' || $ptoid == '21' || $ptoid == '9') && $res['empesfera'] == 'E') {
                //Projeto Proprio PAC
                $arquivos = $this->buscaPreObraAnexo($res['preid'], $podidPACPP);
                $result = $this->verificaQuantidadePreObra($res['preid'], $podidPACPP);
                $resposta = $result == count($podidPACPP) ? 't' : 'f';
                return array('resposta' => $resposta, 'arquivos' => $arquivos);
            }
        }
        return 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 4) A Vistória Final da obra concluída está inserida?
     * Não necessita devolver arquivos.
     */
    public function questaoVistoriaFObraC($obrid)
    {
        $sql = <<<DML
            SELECT
                s.staid
            FROM obras2.supervisao s
            WHERE s.obrid = '$obrid'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
                AND validadaPeloSupervisorUnidade = 'S'
            ORDER BY s.supdata DESC, s.supdtinclusao DESC
            LIMIT 1
DML;
        return ($this->pegaUm($sql) == '3') ? 't' : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 5) O Termo de Recebimento Definitivo da Obra está inserido?
     * Possui Arquivos.
     */
    public function questaoTermoRecebimentoDO($obrid)
    {
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.supervisao s
            INNER JOIN public.arquivo arq ON (s.arqidtermo = arq.arqid)
            WHERE s.obrid = '$obrid'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
            ORDER BY s.supdata DESC, s.supdtinclusao DESC
            LIMIT 1
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 6) Existem restrições e inconformidades da obra?
     * Não necessita devolver arquivos.
     */
    public function questaoRestricoesInconformidades($obrid)
    {
        $sql = <<<DML
            SELECT count(*) AS res
            FROM obras2.restricao r
            INNER JOIN workflow.documento doc ON doc.docid = r.docid
            INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE r.rststatus = 'A'
                AND r.obrid IN ($obrid)
                AND esd.esdid NOT IN (1141,1497,1142,1143)
                --Exceção
                --AND NOT (r.fsrid = 7 AND r.tprid = 16 AND r.rstdsc = 'Aguardando preenchimento do Cumprimento do Objeto.' AND r.usucpf = '')
DML;
        $res = $this->pegaUm($sql);
        return ($res) ? 't' : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Documentos da Obra.
     * 7) O Relatório de Cumprimento do Objeto está inserido?
     * ******** Não necessita de validador ********
     */
    public function questaoRelatorioCumprimento()
    {
        //resposta default
        return 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 1) O Contrato Principal da Obra está inserido?
     * Possui arquivos.
     */
    public function questaoContratoPrincipalObra($obrid)
    {
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.contrato cr
            INNER JOIN public.arquivo arq ON (cr.arqidcontrato = arq.arqid)
            WHERE cr.crtid =
                (SELECT DISTINCT crtid
                FROM obras2.obrascontrato
                WHERE ocrstatus = 'A'
                    AND obrid = $obrid)
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 2) A Planilha de Medições Acumuladas Consolidadas está inserida?
     * Possui arquivos.
     */
    public function questaoPlanilhaMedicaoAcumulada($obrid)
    {
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.supervisao s
            INNER JOIN public.arquivo arq ON (s.arqidplanilha = arq.arqid)
            WHERE s.obrid = '$obrid'
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'
            ORDER BY s.supdata DESC, s.supdtinclusao DESC
            LIMIT 1
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 3) A Planilha Contratada está inserida?
     * Possui arquivos. Não pode retornar Não se Aplica
     */
    public function questaoPlanilhaContratada($obrid)
    {
        $sql = <<<DML
            SELECT true FROM obras2.obras WHERE obrid = $obrid AND tpoid IN (104,105)
DML;
        if($this->pegaUm($sql)){
            return 'f';
        }
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.obrascontrato oc
            JOIN obras2.obras o ON o.obrid = oc.obrid AND o.obrstatus = 'A'
            INNER JOIN public.arquivo arq ON (oc.arqidcusto = arq.arqid)
            WHERE ocrstatus = 'A'
                AND o.obrid = $obrid
                AND oc.crtid = (SELECT DISTINCT crtid
                    FROM obras2.obrascontrato
                    WHERE ocrstatus = 'A'
                        AND obrid = $obrid)
            ORDER BY oc.ocrid
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 4) As Notas Fiscais, relativas as Medições, estão inseridas?
     * Possui arquivos.
     */
    public function questaoNotasFiscais($obrid)
    {
        $sql = <<<DML
            SELECT
                a.arqid,
                a.arqdescricao as arqnome,
                a.arqextensao
            FROM
                (
                SELECT
                    arq.arqid,
                    arq.arqdescricao,
                    arq.arqextensao
                FROM obras2.pagamentosexecucaoorcamentaria peo
                INNER JOIN arquivo arq on arq.arqid = peo.peoarqid_ordembancaria
                WHERE peostatus = 'A'
                    AND obrid = $obrid

                UNION

                SELECT
                    arq.arqid,
                    arq.arqdescricao,
                    arq.arqextensao
                FROM obras2.pagamentosexecucaoorcamentaria peo
                INNER JOIN arquivo arq on arq.arqid = peo.peoarqid_notafiscal
                WHERE peostatus = 'A'
                    AND obrid = $obrid

                UNION

                SELECT
                    arq.arqid,
                    arq.arqdescricao,
                    arq.arqextensao
                FROM obras2.pagamentosexecucaoorcamentaria peo
                LEFT  JOIN arquivo arq on arq.arqid = peo.peoarqid_boletimmedicao
                WHERE peostatus = 'A'
                    AND obrid = $obrid
                ) a
            ORDER BY a.arqdescricao
DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }

    /**
     * Questionário: Quetionário - Documentos Obra - Cumprimento do Objeto.
     * Item Composição: Construtora.
     * 5) Os aditivos de valores estão inseridos?
     * Possui arquivos.
     */
    public function questaoAditivosValores($obrid)
    {
        $sql = <<<DML
            SELECT
                arq.arqid,
                arq.arqnome,
                arq.arqextensao
            FROM obras2.contrato cr
            INNER JOIN public.arquivo arq ON (cr.arqid = arq.arqid)
            WHERE (cr.crtid = (SELECT DISTINCT crtid FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obrid)
                    OR cr.crtidpai = (SELECT DISTINCT crtid FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obrid))
                AND ttaid = 2;

DML;
        $arquivo = $this->carregar($sql);
        return ($arquivo) ? array('resposta' => 't', 'arquivos' => $arquivo) : 'f';
    }
}
