<?php

class SolicitacaoDesembolso extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_desembolso";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sldid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'docid' => null,
        'sldid' => null,
        'obrid' => null,
        'usucpf' => null,
        'sldjustificativa' => null,
        'sldobs' => null,
        'sldstatus' => null,
        'slddatainclusao' => null,
        'sldpercsolicitado' => null,
        'supid' => null,
        'sldpercpagamento' => null,
        'sldobstec' => null,
        'arqid' => null,
    );


    public function verificaTramiteSolicitacoes($obrid)
    {
        if(!$obrid)
            return true;

        $sql = "
           SELECT
                sv.*,
                (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid ORDER BY hstid DESC LIMIT 1),
                (
                    SELECT
                    c.cmddsc
                    FROM workflow.historicodocumento h
                    JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid
                    WHERE h.docid = d.docid ORDER BY h.hstid DESC LIMIT 1
                )
            FROM obras2.solicitacao_desembolso sv
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid IN(".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI.", ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL.", ".AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO.") AND sv.sldstatus = 'A' AND sv.obrid = $obrid
        ";

        $solicitacoesEmAnalise = $this->pegaLinha($sql);

        if(!empty($solicitacoesEmAnalise)){
            // Verifica se possui Restrições
            // Indeferido
            $validador = new Validador();
            if(!$validador->restricoesNaoSuperadas($obrid)){
                $doc = wf_pegarDocumento($solicitacoesEmAnalise['docid']);

                if($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI)
                    wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));

                else if($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL)
                    wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));

                else if($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA)
                    wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));
            }

            $sql = "UPDATE workflow.historicodocumento SET usucpf = '00000000191' WHERE docid = {$solicitacoesEmAnalise['docid']} AND aedid IN (" . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO. ", " . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO . ", ".AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO.")";
            $this->executar($sql);
            $this->commit();
        }

        return true;
    }

    public function pegaPercentualSolicitacao($obrid, $supid = null)
    {
        $supervisao = new Supervisao();
        $supid = ($supid) ? $supid : $supervisao->pegaUltSupidByObra($obrid);

        $sql = "SELECT *, (totalpago*100)/vlrobra AS percentualpgo FROM par.v_pagamento_total_por_obra f WHERE f.obrid = $obrid";

        $pgoObra = $this->pegaLinha($sql);

        $obras = new Obras($obrid);
        $percentual = $obras->pegaPercentualExecucaoPactuadoSupervisao($obrid, $supid);

        return $percentual - $pgoObra['percentualpgo'];
    }

    public function verificaObrasEmVermelho($obrid)
    {
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento 		 d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            JOIN entidade.endereco edr ON edr.endid = o.endid
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND e.empesfera = 'M' AND d.esdid IN(691,690) AND e.orgid IN(3) AND
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND
                    ed.esdid IN (690, 691) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) > 60 AND
                    obrpercentultvistoria < 100.00) THEN
                        true
                    ELSE
                        false
                END
                AND
                edr.muncod IN (

                    SELECT
                        m.muncod
                    FROM
                        obras2.obras o
                    JOIN obras2.empreendimento 	 ep ON ep.empid = o.empid AND ep.empstatus = 'A' AND ep.empesfera = 'M'
                    JOIN entidade.endereco ed ON ed.endid = o.endid
                    JOIN territorios.municipio m ON m.muncod = ed.muncod
                    WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid = $obrid

                )";

        $result = $this->pegaUm($sql);

       return (!empty($result)) ? true : false;
    }


    public function percentualCronogramaExecutado($obrid)
    {

        $sql = "
            SELECT
                SUM(ic.icovlritem) valorobra
            FROM obras2.obras o
            JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
            JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
            JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
            WHERE
                o.obrid = $obrid";
        $valorObra = $this->pegaUm($sql);

        $sql = "
        SELECT
            ic.icopercsobreestrutura,
            ic.icovlritem,
            ic.icodtinicioitem,
            ic.icodterminoitem,
            ic.icopercexecutado,

            CASE WHEN ic.icodterminoitem <= NOW() THEN
              DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem))
            ELSE
              DATE_PART ('day', (NOW() - ic.icodtinicioitem))
            END diasexecutados,

			DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem)) totaldiasexecucao
        FROM obras2.obras o
        JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
        JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
        JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
        WHERE
            o.obrid = $obrid
            AND (ic.icodtinicioitem <= NOW() )

        ";

        $itensExecucao = $this->carregar($sql);

        $percentualTotalPrevisto = 0;

        if(!empty($itensExecucao)) {
            foreach ($itensExecucao as $key => $item) {
                $percentualExecucao = ($item['totaldiasexecucao'] == 0) ? 100 : ($item['diasexecutados'] * 100 / $item['totaldiasexecucao']);
                $valorPrevisto = ($item['icovlritem'] * $percentualExecucao) / 100;
                $percentualPrevisto = ($valorPrevisto * 100) / $valorObra;

                $itensExecucao[$key]['percentualprevisto'] = $percentualExecucao;
                $itensExecucao[$key]['percentualprevistoobra'] = $percentualPrevisto;
                $itensExecucao[$key]['valorprevisto'] = $valorPrevisto;

                $percentualTotalPrevisto += $percentualPrevisto;
            }
        }

        // Quando obra vinculada, faz o cálculas para estimativa com o percentual aproveitado
        $obras = new Obras($obrid);
        $dadosPercentual         = $obras->getPercentExecutadoAproveitadoObraAnterior($this->obrid);
        $percentual_aproveitavel = ($dadosPercentual) ? $dadosPercentual['obrperccontratoanterior'] : 0;
        $percentual_do_todo      = 100 - $percentual_aproveitavel;
        $percentual_final        = round((($percentualTotalPrevisto/100)*$percentual_do_todo), 2)+$percentual_aproveitavel;

        return $percentual_final;
    }


}