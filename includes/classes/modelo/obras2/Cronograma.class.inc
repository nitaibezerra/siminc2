<?php

class Cronograma extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cronograma";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("croid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'croid' => null,
        'obrid' => null,
        'usucpf' => null,
        'crodtinclusao' => null,
        'crostatus' => null,
        'croobs' => null
    );


    public function cronogramaObra($obrid)
    {
        $croid = $this->getIdCronogramaObra($obrid);

        if (!$croid) {
            $dados = array(
                'obrid' => $obrid,
                'usucpf' => $_SESSION['usuario']['usucpf'],
                'crodtinclusao' => $_SESSION['usuario']['usucpf'],
                'crodtinclusao' => 'NOW()',
            );

            $this->popularDadosObjeto($dados);
            $this->salvar();
        } else {
            $this->carregarPorId($croid);
        }
    }

    public function getIdCronogramaObra($obrid)
    {
        $sql = "SELECT croid FROM obras2.cronograma WHERE obrid = $obrid AND crostatus = 'A'";
        return $this->pegaUm($sql);
    }

    public function getCronogramaObra($obrid)
    {
        $sql = "SELECT * FROM obras2.cronograma WHERE obrid = $obrid AND crostatus = 'A'";
        return $this->pegaLinha($sql);
    }

    public function getItensCronograma($croid)
    {
        $sql = "
            SELECT * FROM obras2.itenscomposicaoobra WHERE croid = $croid
        ";

        return $this->carregar($sql);
    }

    public function verificaCronogramaEdicao($obrid)
    {
        $sql = "SELECT * FROM obras2.cronograma WHERE obrid = $obrid AND crostatus = 'E'";
        return $this->pegaLinha($sql);
    }

    public function getCronogramaEdicao($obrid)
    {
        $crongoramaAtual = $this->getCronogramaObra($obrid);
        $cronogramaEdicao = $this->verificaCronogramaEdicao($obrid);

        // Se não existir um crongorama em edição, cria um novo
        if (empty($cronogramaEdicao)) {

            $arDados = array(
                'obrid' => $obrid,
                'usucpf' => $_SESSION['usucpf'],
                'crodtinclusao' => 'now()',
                'crostatus' => 'E'
            );

            $this->popularDadosObjeto($arDados);
            $this->salvar();
            $cronogramaEdicao = $this->getDados();

            $itens = $this->getItensCronograma($crongoramaAtual['croid']);

            // Duplica os itens do cronograma atual
            if (is_array($itens)) {
                foreach ($itens as $item) {
                    unset($item['icoid']);
                    $item['croid'] = $cronogramaEdicao['croid'];
                    $itensComposicao = new ItensComposicaoObras();
                    $itensComposicao->popularDadosObjeto($item);
                    $itensComposicao->salvar();
                }
            }

            $this->commit();
        }

        $this->popularDadosObjeto($cronogramaEdicao);
        return $cronogramaEdicao;
    }

    public function verificaCronogramaExecucao($obrid, $icoid = null, $date = null)
    {
        $date = (!$date) ? "NOW()" : "'$date'::date";
        $icoid = ($icoid) ? "AND ic.icoid = $icoid" : '';

        $sql = "
                SELECT
                    ic.icoid,
                    ic.icopercsobreestrutura as percsobreestrutura,
                    ic.icovlritem as valor,
                    ic.icodtinicioitem as inicioitem,
                    ic.icodterminoitem as terminoitem,
                    ic.icopercexecutado as percexecutado,
                    CASE WHEN ic.icodterminoitem <= $date THEN
                      DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem))
                    ELSE
                      DATE_PART ('day', ($date - ic.icodtinicioitem))
                    END diasexecutados,
                    DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem)) totaldiasexecucao
                FROM obras2.obras o
                JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
                JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
                JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
                WHERE
                    o.obrid = $obrid
                    AND (ic.icodtinicioitem <= $date)
                    $icoid
        ";

        $itensExecucao = $this->carregar($sql);

        if (!empty($itensExecucao)) {
            foreach ($itensExecucao as $key => $item) {
                $percentualPrevistoItem = ($item['totaldiasexecucao'] == 0) ? 100 : ($item['diasexecutados'] * 100 / $item['totaldiasexecucao']);
                $valorPrevistoItem = ($item['valor'] * $percentualPrevistoItem) / 100;

                $itensExecucao[$key]['percentualprevistoitem'] = $percentualPrevistoItem;
                $itensExecucao[$key]['valorprevistoitem'] = $valorPrevistoItem;
                $itensExecucao[$key]['percentualprevistosobreobra'] = $percentualPrevistoSobreObra;

                $percentualPrevistoObra += $percentualPrevistoSobreObra;

                if (($item['totaldiasexecucao'] == $item['diasexecutados']) && $item['percexecutado'] != 100) {
                    $itensExecucao[$key]['situacao'] = 'V';
                    $itensExecucao[$key]['observacao'] = 'Esta etapa não atingiu 100% de execução dentro do prazo determinado.';
                } else if ( ($item['percexecutado'] + 20) < $percentualPrevistoItem) {
                    $itensExecucao[$key]['situacao'] = 'D';
                    $itensExecucao[$key]['observacao'] = 'De acordo com o cronograma esta etapa deveria ter o percentual de ' . number_format($percentualPrevistoItem, 2, ",", ".") . '% mas o percentual apresentado na última vistoria foi '.  number_format($item['percexecutado'], 2, ",", ".") .'%.';
                } else {
                    $itensExecucao[$key]['situacao'] = 'A';
                    $itensExecucao[$key]['observacao'] = 'Execução de acordo com o cronograma';
                }
            }
        }

        return $itensExecucao;
    }

    public function pegaDataPrevistaPorPercentExecutado($inicio, $fim, $data, $percentualAtual)
    {
        $inicio = '2015-01-01';
        $fim = '2015-12-31';
        $data = 'NOW()';
        $percentualAtual = 50;

        $data = ($data == 'NOW()') ? new DateTime() : new DateTime($data);
        $inicio = new DateTime($inicio);
        $fim = new DateTime($fim);

        // Quando a etapa já esta vencida e com 100%, não faz os calculos
        if($fim <= $data && $percentualAtual == 100)
            return $fim->format('Y-m-d');

        $totalDias = $fim->diff($inicio)->days;
        $diasExecutados = $data->diff($inicio)->days;
        $percentualIdeal = ($diasExecutados * 100) / $totalDias;

        if ($percentualAtual > $percentualIdeal) {
            $dias = (int)($diasExecutados * 100 / $percentualAtual);
            $inicio->add(new DateInterval("P{$dias}D"));
            return $inicio->format('Y-m-d');
        } else if ($percentualAtual < $percentualIdeal) {
            $dias = (int) ($diasExecutados * 100 / $percentualAtual);
            $inicio->add(new DateInterval("P{$dias}D"));
            return $inicio->format('Y-m-d');
        } else {
            return 0;
//            return $fim->format('Y-m-d');
        }

    }

    public function verificaInconformidadesAtrasoCronograma($obrid)
    {
        $solicitacaoDesembolso  = new SolicitacaoDesembolso();
        $obra = new Obras();

        $percentualTotalPrevisto = $solicitacaoDesembolso->percentualCronogramaExecutado($obrid);
        $percentTotal = $obra->pegaPercentualExecucao($obrid);

        if (($percentTotal + 20) > $percentualTotalPrevisto) {
            // Supera as restrições

            $sql = "
                SELECT
                    r.docid
                FROM obras2.restricao r
                WHERE
                   r.obrid = $obrid
                   AND rstitem IN ('I')
                   AND rststatus = 'A'
                   AND usucpf = '00000000191'
                   AND rstdsc = 'A execução física da obra está divergente do cronograma preenchido.';
            ";

            $docids = $this->carregarColuna($sql);
            foreach ($docids as $docid) {
                $this->executar("UPDATE workflow.documento SET esdid = 1142 WHERE docid = $docid");
            }
        }

        $this->commit();
    }
}
