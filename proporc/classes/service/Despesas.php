<?php
/**
 * Implementação da service de gestão de grupos e categorias de despesa.
 *
 * $Id: Despesas.php 101251 2015-08-13 22:01:44Z maykelbraz $
 */

/**
 * Service de gestão de despesas.
 * @uses po_Service_Abstract
 */
class Proporc_Service_Despesas extends Spo_Service_Abstract
{
    protected $separadorLinha = '/[\t]/';

    protected $registros = array(
        'invalidos' => array(),
        'processados' => array()
    );

    protected $numRegistrosProcessados = 0;

    public function salvarGrupoDespesas()
    {
        $grpDespesas = new Proporc_Model_Grupodespesa();
        $grpDespesas->popularDadosObjeto($this->getDados());
        if ($id = $grpDespesas->salvar()) {
            $grpDespesas->commit();
            return $id;
        } else {
            throw new Exception('Não foi possível criar um novo grupo de despesas.');
        }
    }

    public function consultarGrupo($retornarJSON = false)
    {
        $grupo = new Proporc_Model_Grupodespesa($this->gdpid);
        if (!$retornarJSON) {
            return $grupo;
        }

        return simec_json_encode($grupo->getDados());
    }

    public function apagarGrupoDespesas()
    {
        $grpDespesas = new Proporc_Model_Grupodespesa();
        if ($excluiu = (bool)$grpDespesas->excluir($this->gdpid)) {
            $grpDespesas->commit();
            return true;
        } else {
            throw new Exception('Não foi possível apagar o grupo de despesas.');
        }
    }

    public function salvarCategoriaDespesas()
    {
        $catDespesas = new Proporc_Model_Despesa();
        $catDespesas->popularDadosObjeto($this->getDados());
        if ($this->dspid = $catDespesas->salvar()) {

            $this->salvarDespesaFonterecurso()
                ->salvarDespesaGnd();

            $catDespesas->commit();
            return $this->dspid;
        } else {
            throw new Exception('Não foi possível criar uma nova categoria de despesas.');
        }
    }

    public function consultarCategoria($retornarJSON = false)
    {
        $categoria = new Proporc_Model_Despesa($this->dspid);
        if (!$retornarJSON) {
            return $categoria;
        }

        $dados = $categoria->getDados();

        if (!empty($dados)) {

            // -- Carregar Fonte
            $fontes = new Proporc_Model_Despesafonterecurso();
            $dadosFonte = $fontes->recuperarTodos('foncod', array("dspid = {$this->dspid}"));
            foreach (is_array($dadosFonte)?$dadosFonte:array() as $fonte) {
                $dados['foncod'][] = $fonte['foncod'];
            }
            unset($fontes, $dadosFonte);

            // -- Carregar GND
            $gnds = new Proporc_Model_Despesagnd();
            $dadosGnd = $gnds->recuperarTodos('gndcod', array("dspid = {$this->dspid}"));
            foreach (is_array($dadosGnd)?$dadosGnd:array() as $gnd) {
                $dados['gndcod'][] = $gnd['gndcod'];
            }
        }

        return simec_json_encode($dados);
    }

    public function apagarCategoriaDespesas()
    {
        $catDespesas = new Proporc_Model_Despesa();
        if ($excluiu = (bool)$catDespesas->excluir($this->dspid)) {
            $catDespesas->commit();
            return true;
        } else {
            throw new Exception('Não foi possível apagar a categoria de despesas.');
        }
    }

    public function salvarDespesasFGU()
    {
        $this->salvarDespesaFonterecurso()
            ->salvarDespesaGnd()
            ->salvarDespesaUnidade();
    }

    public function salvarDespesaFonterecurso()
    {
        $msgErro = 'Não foi possível atualizar as fontes associadas a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesafonterecurso();
        return $this->updateDespesa($mdl, 'foncod', $msgErro);
    }

    public function salvarDespesaGnd()
    {
        $msgErro = 'Não foi possível atualizar os GNDs associados a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesagnd();
        return $this->updateDespesa($mdl, 'gndcod', $msgErro);
    }

    public function salvarDespesaUnidade()
    {
        $msgErro = 'Não foi possível atualizar as Unidades Orçamentárias associadas a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesaunidadeorcamentaria();
        return $this->updateDespesa($mdl, 'unicod', $msgErro);
    }

    public function imprimirListaPlanosorcamentarios()
    {
        if (!isset($this->exercicio)) {
            throw new Exception('Para listar planos orçamentários, é necessário informar o valor do exercício.');
        }

        $where = array();
        if (isset($this->acacod) && !empty($this->acacod)) {
            $where[] = "pao.acacod = '{$this->acacod}'";
        }
        if (isset($this->unicod) && !empty($this->unicod)) {
            $where[] = "pao.unicod = '{$this->unicod}'";
        }

        $sql = Proporc_Model_Despesaplanoorcamentario::querySelecaoDePlanoOrcamentario(
            $this->exercicio,
            implode(' AND ', $where)
        );

        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $list->setQuery($sql)
            ->setCampos(array('id', 'programatica', 'descricao'))
            ->turnOffForm()
            ->turnOnPesquisator()
            ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::INVERTER)
            ->setFormFiltros('planoorcamentario')
            ->setCabecalho(array('Programática', 'Plano Orçamentário'))
            ->addCallbackDeCampo('descricao', 'alinharEsquerda')
            ->addAcao('select', array(
                'func' => 'selecionarPlanoOrcamentario',
                'desmarcado' => true,
                'extra-params' => array('programatica', 'descricao')
            ))
            ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function salvarDespesasAdicionais()
    {
        $this->salvarDespesaAcao()
            ->salvarDespesaSubacao()
            ->salvarDespesaPlanoorcamentario();
    }

    public function salvarDespesaAcao()
    {
        $msgErro = 'Não foi possível atualizar as Ações associadas a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesaacao();
        return $this->updateDespesa($mdl, 'acacod', $msgErro);
    }

    public function salvarDespesaSubacao()
    {
        $msgErro = 'Não foi possível atualizar as Subações associadas a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesasubacao();
        return $this->updateDespesa($mdl, 'sbaid', $msgErro);
    }

    public function salvarDespesaPlanoorcamentario()
    {
        $msgErro = 'Não foi possível atualizar os Planos orçamentários associados a esta categoria de despesas.';
        $mdl = new Proporc_Model_Despesaplanoorcamentario();
        return $this->updateDespesa($mdl, 'plocod', $msgErro);
    }

    protected function updateDespesa(Modelo $mdl, $item, $msgErro)
    {
        $where = sprintf("dspid = %d", $this->dspid);
        if (!$mdl->excluirVarios($where)) {
            throw new Exception("{$msgErro} Falha ao remover antigos valores.");
        }

        // -- Caso não tenha nenhuma novo item para inserir, finalize a execução
        if (empty($this->$item)) {
            $mdl->commit();
            return $this;
        }

        foreach ($this->$item as $_item) {
            $mdl->dspid = $this->dspid;
            $mdl->$item = $_item;
            if (!$mdl->salvar()) {
                throw new Exception("{$msgErro} Falha ao inserir novos valores.");
            }
            $mdl->clearDados();
        }

        $mdl->commit();
        return $this;
    }

    public function importarValorDasDespesas()
    {
        if (empty($this->despesas)) {
            throw new Exception('Não foi enviada nenhuma informação de valores.');
        }

        $this->despesas = explode(PHP_EOL, trim($this->despesas));
        if (false === count($this->despesas)) {
            throw new Exception('Os dados enviados são inválidos. Não foi encontrada nenhuma linha de valores.');
        }

        $this->salvarValoresImportados();

        $numRegistrosInvalidos = count($this->registros['invalidos']);
        $mensagem = <<<HTML
Resultado do processamento:
<ul>
    <li><strong>Registros processados</strong>: {$this->numRegistrosProcessados}</li>
    <li><strong>Registros inválidos</strong>: {$numRegistrosInvalidos}</li>
</ul>
HTML;
        $this->flashMessage->addMensagem($mensagem);

        if ($numRegistrosInvalidos > 0) {
            $mensagem = <<<HTML
Registros inválidos:
<ul>
HTML;
            $mensagem .= '<li>' . implode('</li><li>', $this->registros['invalidos']) . '</li>';
            $mensagem .= <<<HTML
</ul>
HTML;
            $this->flashMessage->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
        }
    }

    public function salvarValoresImportados()
    {
        global $db;
        $mdlCarga = new Proporc_Model_Cargapreenchimento();

        // -- Apagar valores antes de inserir os novos
        $mdlCarga->excluirVarios('true');
        $mdlCarga->commit();

        /*
         * Carrega os valores para a tabela temporária
         */
        foreach ($this->despesas as $despesa) {
            $pedacos = preg_split($this->separadorLinha, $despesa);

            if (11 !== count($pedacos)) {
                $this->registros['invalidos'][] = "Linha inválida: {$despesa}";
                continue;
            }

            list(
                $mdlCarga->unicod,
                $mdlCarga->ungcod,
                $mdlCarga->acacod,
                $mdlCarga->loccod,
                $mdlCarga->plocod,
                $mdlCarga->sbacod,
                $mdlCarga->ndpcod,
                $mdlCarga->foncod,
                $mdlCarga->metalocalizador,
                $mdlCarga->metapo,
                $mdlCarga->valor
            ) = $pedacos;

            $mdlCarga->salvar();
            $mdlCarga->clearDados();

            $this->numRegistrosProcessados++;
        }

        $mdlCarga->commit();

        /*
         * Realiza a carga nas tabelas
         */

        $prfid = $_REQUEST['cargadespesas']['prfid'];
        $justificativa = $_REQUEST['cargadespesas']['justificativa'];
        $dspid = $_REQUEST['cargadespesas']['dspid'];

        $sql = "
--
--INSERE a Ação e o PO na elabrev.despesaacao --
--
INSERT
INTO
    elabrev.despesaacao
    (
        acaid,
        foncod,
        ndpid,
        ploid,
        dpavalor,
        iducod,
        ungcod,
        ppoid,
        sbaid
    )
SELECT DISTINCT
    carga.acaid,
    carga.foncod,
    carga.ndpid,
    carga.ploid,
    carga.dpavalor,
    0 AS iducod,
    CASE WHEN carga.ungcod <> '' THEN carga.ungcod ELSE NULL END ungcod,
    {$prfid},
    carga.sbaid
FROM
    (
        SELECT
            (
                SELECT
                    acaid
                FROM
                    elabrev.ppaacao_orcamento pao
                WHERE
                     (pao.unicod =
                                (
                                    SELECT
                                        unicod
                                    FROM
                                        public.unidadegestora
                                    WHERE
                                        ungcod = crg.ungcod)
                            OR  pao.unicod = crg.unicod)
                AND pao.acacod = crg.acacod
                AND pao.loccod = crg.loccod
                AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 ) AS acaid,
            crg.foncod,
            (
                SELECT
                    ndpid
                FROM
                    public.naturezadespesa ndp
                WHERE
                    ndp.ndpcod = crg.ndpcod
                AND ndp.ndpano = '{$_SESSION['exercicio']}' limit 1 )AS ndpid,
            (
                SELECT
                    ploid
                FROM
                    elabrev.planoorcamentario plo
                WHERE
                    plo.acaid =
                    (
                        SELECT
                            acaid
                        FROM
                            elabrev.ppaacao_orcamento pao
                        WHERE
                            (
                                pao.unicod =
                                (
                                    SELECT
                                        unicod
                                    FROM
                                        public.unidadegestora
                                    WHERE
                                        ungcod = crg.ungcod)
                            OR  pao.unicod = crg.unicod)
                        AND pao.acacod = crg.acacod
                        AND pao.loccod = crg.loccod
                        AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                AND plo.plocodigo = crg.plocod limit 1 ) AS ploid,
            crg.valor                                    AS dpavalor,
            crg.ungcod,
            (
                SELECT
                    sbaid
                FROM
                    elabrev.subacao
                WHERE
                    sbacod = crg.sbacod
                AND ano='{$_SESSION['exercicio']}') AS sbaid
        FROM
            proporc.cargapreenchimento crg) AS carga
WHERE
    carga.acaid::text||'.'||carga.foncod::text||'.'||carga.ndpid::text||'.'||carga.ploid::text NOT
    IN
        (
        SELECT DISTINCT
            dpa.acaid::text ||'.' || dpa.foncod::text || '.'|| dpa.ndpid::text ||'.'|| dpa.ploid::
            text
        FROM
            elabrev.despesaacao dpa
        WHERE
            dpa.acaid::text ||'.' || dpa.foncod::text || '.'|| dpa.ndpid::text ||'.'|| dpa.ploid::
            text <>'' );
------------------------------------------------------------------------------------------------------------------------
--
-- INSERE o Financeiro --
--
INSERT
INTO
    proporc.ploafinanceiro
    (
        dpaid,
        mtrid,
        usucpf,
        plfvalor
    )
SELECT
    dpaid,
    mtrid,
    usucpf,
    valor
FROM
    (
        SELECT DISTINCT
            (
                SELECT
                    dpaid
                FROM
                    elabrev.despesaacao dea
                WHERE
                    dea.acaid =
                    (
                        SELECT
                            acaid
                        FROM
                            elabrev.ppaacao_orcamento pao
                        WHERE
                            (
                                pao.unicod =
                                (
                                    SELECT
                                        unicod
                                    FROM
                                        public.unidadegestora
                                    WHERE
                                        ungcod = crg.ungcod)
                            OR  pao.unicod = crg.unicod)
                        AND pao.acacod = crg.acacod
                        AND pao.loccod = crg.loccod
                        AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                AND dea.foncod = crg.foncod
                AND dea.ndpid =
                    (
                        SELECT
                            ndpid
                        FROM
                            public.naturezadespesa ndp
                        WHERE
                            ndp.ndpcod = crg.ndpcod
                        AND ndp.ndpano = '{$_SESSION['exercicio']}' limit 1 )
                AND dea.ploid =
                    (
                        SELECT
                            ploid
                        FROM
                            elabrev.planoorcamentario plo
                        WHERE
                            plo.acaid =
                            (
                                SELECT
                                    acaid
                                FROM
                                    elabrev.ppaacao_orcamento pao
                                WHERE
                                    (
                                        pao.unicod =
                                        (
                                            SELECT
                                                unicod
                                            FROM
                                                public.unidadegestora
                                            WHERE
                                                ungcod = crg.ungcod)
                                    OR  pao.unicod = crg.unicod)
                                AND pao.acacod = crg.acacod
                                AND pao.loccod = crg.loccod
                                AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                        AND plo.plocodigo = crg.plocod limit 1 ) LIMIT 1 ) AS dpaid,
            {$dspid}                                                          AS mtrid,
            '{$_SESSION['usucpf']}'                                           AS usucpf,
            valor
        FROM
            proporc.cargapreenchimento crg) cargafin
WHERE
    cargafin.dpaid::text||'.'||cargafin.mtrid::text NOT IN
    (
        SELECT
            dpaid::text||'.'||mtrid::text
        FROM
            proporc.ploafinanceiro
        WHERE
            dpaid::text||'.'||mtrid::text <>'' );
------------------------------------------------------------------------------------------------------------------------
--
-- Update no valor da PLOAFINANCEIRO
--
UPDATE
    proporc.ploafinanceiro plf
SET
    plfvalor = cargafin.valor
FROM
    (
        SELECT DISTINCT
            (
                SELECT
                    dpaid
                FROM
                    elabrev.despesaacao dea
                WHERE
                    dea.acaid =
                    (
                        SELECT
                            acaid
                        FROM
                            elabrev.ppaacao_orcamento pao
                        WHERE
                            (
                                pao.unicod =
                                (
                                    SELECT
                                        unicod
                                    FROM
                                        public.unidadegestora
                                    WHERE
                                        ungcod = crg.ungcod)
                            OR  pao.unicod = crg.unicod)
                        AND pao.acacod = crg.acacod
                        AND pao.loccod = crg.loccod
                        AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                AND dea.foncod = crg.foncod
                AND dea.ndpid =
                    (
                        SELECT
                            ndpid
                        FROM
                            public.naturezadespesa ndp
                        WHERE
                            ndp.ndpcod = crg.ndpcod
                        AND ndp.ndpano = '{$_SESSION['exercicio']}' limit 1 )
                AND dea.ploid =
                    (
                        SELECT
                            ploid
                        FROM
                            elabrev.planoorcamentario plo
                        WHERE
                            plo.acaid =
                            (
                                SELECT
                                    acaid
                                FROM
                                    elabrev.ppaacao_orcamento pao
                                WHERE
                                    (
                                        pao.unicod =
                                        (
                                            SELECT
                                                unicod
                                            FROM
                                                public.unidadegestora
                                            WHERE
                                                ungcod = crg.ungcod)
                                    OR  pao.unicod = crg.unicod)
                                AND pao.acacod = crg.acacod
                                AND pao.loccod = crg.loccod
                                AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                        AND plo.plocodigo = crg.plocod limit 1 ) LIMIT 1 ) AS dpaid,
            {$dspid}                                                       AS mtrid,
            valor
        FROM
            proporc.cargapreenchimento crg ) cargafin
WHERE
    plf.dpaid = cargafin.dpaid
AND plf.mtrid = cargafin.mtrid;
------------------------------------------------------------------------------------------------------------------------
--
-- Altera o valor em despesaacao
--

UPDATE
    elabrev.despesaacao desp
SET
    dpavalor = cargafin.valor
FROM
    (
        SELECT DISTINCT
            (
                SELECT
                    dpaid
                FROM
                    elabrev.despesaacao dea
                WHERE
                    dea.acaid =
                    (
                        SELECT
                            acaid
                        FROM
                            elabrev.ppaacao_orcamento pao
                        WHERE
                            (
                                pao.unicod =
                                (
                                    SELECT
                                        unicod
                                    FROM
                                        public.unidadegestora
                                    WHERE
                                        ungcod = crg.ungcod)
                            OR  pao.unicod = crg.unicod)
                        AND pao.acacod = crg.acacod
                        AND pao.loccod = crg.loccod
                        AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                AND dea.foncod = crg.foncod
                AND dea.ndpid =
                    (
                        SELECT
                            ndpid
                        FROM
                            public.naturezadespesa ndp
                        WHERE
                            ndp.ndpcod = crg.ndpcod
                        AND ndp.ndpano = '{$_SESSION['exercicio']}' limit 1 )
                AND dea.ploid =
                    (
                        SELECT
                            ploid
                        FROM
                            elabrev.planoorcamentario plo
                        WHERE
                            plo.acaid =
                            (
                                SELECT
                                    acaid
                                FROM
                                    elabrev.ppaacao_orcamento pao
                                WHERE
                                    (
                                        pao.unicod =
                                        (
                                            SELECT
                                                unicod
                                            FROM
                                                public.unidadegestora
                                            WHERE
                                                ungcod = crg.ungcod)
                                    OR  pao.unicod = crg.unicod)
                                AND pao.acacod = crg.acacod
                                AND pao.loccod = crg.loccod
                                AND pao.prgano = '{$_SESSION['exercicio']}' LIMIT 1 )
                        AND plo.plocodigo = crg.plocod limit 1 ) LIMIT 1 ) AS dpaid,
            {$dspid}                                                          AS mtrid,
            valor
        FROM
            proporc.cargapreenchimento crg ) cargafin
WHERE
    desp.dpaid = cargafin.dpaid;
------------------------------------------------------------------------------------------------------------------------
--
-- Grava a Justivicativa na PPA_ACAOORCAMENTO
--
UPDATE elabrev.ppaacao_orcamento pao
  SET justificativa = '{$justificativa}',
      acaalteracao = 'A'
  FROM (SELECT (SELECT acaid
                  FROM elabrev.ppaacao_orcamento pao
                  WHERE (pao.unicod = (SELECT unicod
                                         FROM public.unidadegestora
                                         WHERE ungcod = crg.ungcod)
                         OR  pao.unicod = crg.unicod)
                    AND pao.acacod = crg.acacod
                    AND pao.loccod = crg.loccod
                    AND pao.prgano = '{$_SESSION['exercicio']}'
                  LIMIT 1)
          FROM proporc.cargapreenchimento crg) cargafin
  WHERE pao.acaid = cargafin.acaid;
------------------------------------------------------------------------------------------------------------------------
--
-- Tipo de Detalhamento (padrão 1 (rever))
--
INSERT
    INTO
        elabrev.tipodetalhamentoacao
    SELECT
        acaid,
        1
    FROM
        elabrev.ppaacao_orcamento
    WHERE
        prgano = '{$_SESSION['exercicio']}'
    AND acaid NOT IN
        (
            SELECT
                acaid
            FROM
                elabrev.tipodetalhamentoacao )
------------------------------------------------------------------------------------------------------------------------
";

        $db->executar($sql);
        $db->commit();

        /*
         * Carastrar Workflow para as linhas que foram inseridas
         */

    }

    public function contarDetalhamentoCategoria()
    {
        $financeiro = new Proporc_Model_Ploafinanceiro();
        $dados = $financeiro->recuperarTodos('COUNT(1) AS "numFinanceiros"', array("mtrid = {$this->dspid}"));

        return current($dados);
    }

    public function contarDetalhamentoGrupo()
    {
        $financeiro = new Proporc_Model_Ploafinanceiro();
        $dados = $financeiro->contarDetalhamentoGrupo($this->gdpid);

        return current($dados);
    }
}
