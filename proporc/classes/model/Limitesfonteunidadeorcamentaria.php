<?php
/**
 * Classe de mapeamento da entidade proporc.limitesfonteunidadeorcamentaria.
 *
 * $Id: Limitesfonteunidadeorcamentaria.php 101253 2015-08-14 11:48:26Z werteralmeida $
 *
 * @filesource
 */

/**
 * Mapeamento da entidade proporc.limitesfonteunidadeorcamentaria.
 *
 * @uses Modelo
 */
class Proporc_Model_Limitesfonteunidadeorcamentaria extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'proporc.limitesfonteunidadeorcamentaria';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'lfuid',
    );

    /**
     * @var array Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'lfuid' => null,
        'unicod' => null,
        'foncod' => null,
        'dspid' => null,
        'vlrlimite' => null,
    );

    protected $despesaUo;

    protected function getDespesaUnidade()
    {
        if (!isset($this->despesaUo)) {
            $this->despesaUo = new Proporc_Model_Despesaunidadeorcamentaria();
        } else {
            $this->despesaUo->clearDados();
        }

        return $this->despesaUo;
    }

    static public function querySomatorioGeral(array $clausulas = array())
    {
        $where = array();
        foreach ($clausulas as $campo => $valor) {
            switch ($campo) {
                case 'dsp.dspid':
                    $sql = <<<DML
EXISTS (SELECT 1
          FROM proporc.despesa dsp1
            INNER JOIN proporc.grupodespesa gdp1 USING(gdpid)
          WHERE dsp1.dspid = %d
            AND gdp.prfid = gdp1.prfid)
DML;
                    $where[] = sprintf($sql, $valor);
                    break;
                default:
                    $where[] = sprintf("{$campo} = %d", $valor);
            }
        }
        $where = empty($where)?'':'WHERE ' . implode(' AND ', $where);

        $sql = <<<DML
SELECT SUM(somatorio.vlrmontante) AS vlrmontante,
       SUM(somatorio.vlrlimite) AS vlrlimite,
       SUM(somatorio.vlrdetalhado) AS vlrdetalhado,
       SUM(somatorio.saldomontante) AS saldomontante,
       SUM(somatorio.saldolimite) AS saldolimite
  FROM (SELECT SUM(limites.vlrmontante) AS vlrmontante,
               SUM(limites.vlrlimite) AS vlrlimite,
               detalhamento.vlrdetalhado,
               SUM(limites.saldomontante) AS saldomontante,
               SUM(limites.vlrlimite) - detalhamento.vlrdetalhado AS saldolimite
          FROM (SELECT gdp.gdpid,
                       gdp.gdpnome,
                       dsp.vlrmontante,
                       SUM(COALESCE(lfu.vlrlimite, 0.00)) AS vlrlimite,
                       dsp.vlrmontante - SUM(COALESCE(lfu.vlrlimite, 0.00)) AS saldomontante
                  FROM proporc.grupodespesa gdp
                    LEFT JOIN proporc.despesa dsp USING(gdpid)
                    LEFT JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid)
                  {$where}
                  GROUP BY gdp.gdpid,
                           gdp.gdpnome,
                           dsp.vlrmontante
                  ORDER BY gdp.gdpordem) limites
            LEFT JOIN (SELECT gdp.gdpid,
                              SUM(COALESCE(plf.plfvalor, 0))::numeric(17,2) AS vlrdetalhado
                         FROM proporc.grupodespesa gdp
                           LEFT JOIN proporc.despesa dsp USING(gdpid)
                           LEFT JOIN proporc.ploafinanceiro plf ON(dsp.dspid = plf.mtrid)
                           {$where}
                         GROUP BY gdpid) detalhamento USING(gdpid)
          GROUP BY detalhamento.vlrdetalhado) somatorio
DML;
        return $sql;
    }

    static public function querySomatorioGrupos(array $clausulas = array())
    {
        $where = array();
        foreach ($clausulas as $campo => $valor) {
            switch ($campo) {
                case 'dsp.dspid':
                    $sql = <<<DML
EXISTS (SELECT 1
          FROM proporc.despesa dsp1
          WHERE dsp1.dspid = %d
            AND dsp1.gdpid = gdp.gdpid)
DML;
                    $where[] = sprintf($sql, $valor);
                    break;
                default:
                    $where[] = sprintf("{$campo} = %d", $valor);
            }
        }
        $where = empty($where)?'':'WHERE ' . implode(' AND ', $where);

        $sql = <<<DML
SELECT limites.gdpid,
       limites.gdpnome,
       SUM(limites.vlrmontante) AS vlrmontante,
       SUM(limites.vlrlimite) AS vlrlimite,
       detalhamento.vlrdetalhado,
       SUM(limites.saldomontante) AS saldomontante,
       SUM(limites.vlrlimite) - detalhamento.vlrdetalhado AS saldolimite
  FROM (SELECT gdp.gdpid,
               gdp.gdpnome,
               dsp.vlrmontante,
               SUM(COALESCE(lfu.vlrlimite, 0.00)) AS vlrlimite,
               dsp.vlrmontante - SUM(COALESCE(lfu.vlrlimite, 0.00)) AS saldomontante
          FROM proporc.grupodespesa gdp
            LEFT JOIN proporc.despesa dsp USING(gdpid)
            LEFT JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid)
          {$where}
          GROUP BY gdp.gdpid,
                   gdp.gdpnome,
                   dsp.vlrmontante
          ORDER BY gdp.gdpordem) limites
  LEFT JOIN (SELECT gdp.gdpid,
                    SUM(COALESCE(plf.plfvalor, 0))::numeric(17,2) AS vlrdetalhado
               FROM proporc.grupodespesa gdp
                 LEFT JOIN proporc.despesa dsp USING(gdpid)
                 LEFT JOIN proporc.ploafinanceiro plf ON(dsp.dspid = plf.mtrid)
               {$where}
               GROUP BY gdpid) detalhamento USING(gdpid)
  GROUP BY limites.gdpid,
           limites.gdpnome,
           detalhamento.vlrdetalhado
  ORDER BY 2
DML;
        return $sql;
    }

    static public function querySomatorioCategorias(array $clausulas = array())
    {
        $where = array();
        foreach ($clausulas as $campo => $valor) {
            $where[] = sprintf("{$campo} = %d", $valor);
        }
        $where = empty($where)?'':'WHERE ' . implode(' AND ', $where);

        $sql = <<<DML
SELECT limites.dspid,
       limites.dspnome,
       limites.vlrmontante,
       limites.vlrlimite,
       SUM(COALESCE(plf.plfvalor, 0))::numeric(17,2) AS vlrdetalhado,
       limites.saldomontante,
       limites.vlrlimite - SUM(COALESCE(plf.plfvalor, 0)) AS saldolimite
  FROM (SELECT dsp.dspid,
               dsp.dspnome,
               dsp.vlrmontante,
               SUM(COALESCE(lfu.vlrlimite, 0.00)) AS vlrlimite,
               dsp.vlrmontante - SUM(COALESCE(lfu.vlrlimite, 0.00)) AS saldomontante
          FROM proporc.despesa dsp
            LEFT JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid)
          {$where}
          GROUP BY dsp.dspid,
                   dsp.dspnome,
                   dsp.vlrmontante) limites
    LEFT JOIN proporc.ploafinanceiro plf ON(limites.dspid = plf.mtrid)
  GROUP BY limites.dspid,
           limites.dspnome,
           limites.vlrmontante,
           limites.vlrlimite,
           limites.saldomontante
  ORDER BY 2
DML;
        return $sql;
    }

    static public function queryDetalheCategoriaDespesa($dspid)
    {
        $sql = <<<DML
SELECT dspid,
       uni.unicod,
       uni.unidsc,
       foncod,
       lfu.vlrlimite,
       detalhamento.vlrdetalhado
  FROM proporc.despesa dsp
    INNER JOIN proporc.despesaunidadeorcamentaria dpu USING(dspid)
    INNER JOIN public.unidade uni USING(unicod)
    INNER JOIN proporc.despesafonterecurso dsf using(dspid)
    LEFT JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid, unicod, foncod)
       LEFT JOIN (SELECT
                    unicod,
                    foncod,
                    SUM(plfvalor) as vlrdetalhado
                FROM
                    proporc.ploafinanceiro
                JOIN
                    elabrev.despesaacao
                USING
                    (dpaid)
                JOIN
                    elabrev.ppaacao_orcamento
                USING
                    (acaid)
                WHERE
                    mtrid = {$dspid}
                GROUP BY
                    1,2) detalhamento USING(unicod,foncod)
  WHERE dspid = %d
  ORDER BY unicod, foncod
DML;
#ver(sprintf($sql, $dspid),d);
        return sprintf($sql, $dspid);
    }

    public function antesSalvar()
    {
        $this->vlrlimite = str_replace(array('.', ','), array('', '.'), $this->vlrlimite);

        // -- Certificando-se que existe uma configuração despesa/uo
        $despesauo = $this->getDespesaUnidade();
        $despesauo->dspid = $this->dspid;
        $despesauo->unicod = $this->unicod;
        if (!$despesauo->existe()) {
            $despesauo->salvar();
        }
        $despesauo->commit();

        return true;
    }

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array())
    {
        if ($boAntesSalvar) {
            $this->antesSalvar();
        }

        $sql = <<<DML
UPDATE {$this->stNomeTabela}
  SET vlrlimite = %f
  WHERE unicod = '%s'
    AND foncod = '%s'
    AND dspid = %d
  RETURNING lfuid
DML;
        $sql = sprintf(
            $sql,
            $this->vlrlimite,
            pg_escape_string($this->unicod),
            pg_escape_string($this->foncod),
            $this->dspid
        );

        $lfuid = $this->pegaUm($sql);

        if (empty($lfuid)) {
            $lfuid = $this->inserir();
        }

        if ($boDepoisSalvar) {
            $this->depoisSalvar();
        }

        if (empty($lfuid)) {
            throw new Exception('Não foi possível alterar o limite informado.');
        }
    }

    public function carregarResumoGrupo($prfid)
    {
        $sql = self::querySomatorioGeral(array('gdp.prfid' => $prfid));
        $dados = $this->pegaLinha(sprintf($sql, $prfid));
        return $dados;
    }

    public function carregarLimitesPorCategoria()
    {
        $sqlCategoria = self::querySomatorioCategorias(array('dsp.dspid' => $this->dspid));
        $sqlGrupos = self::querySomatorioGrupos(array('dsp.dspid' => $this->dspid));
        $sqlTotal = self::querySomatorioGeral(array('dsp.dspid' => $this->dspid));

        $dados = array(
            'categoria' => $this->pegaLinha($sqlCategoria),
            'grupo' => $this->pegaLinha($sqlGrupos),
            'total' => $this->pegaLinha($sqlTotal)
        );

        // -- Calculando o percentual do montante que foi utilizado
        $total = &$dados['total'];
        $total['pctmontante'] = 0;
        if (0.00 === (double)$total['vlrmontante']) {
            $total['pctmontante'] = null;
        } elseif (0.00 !== (double)$total['vlrlimite']) {
            $total['pctmontante'] = round((double)$total['vlrlimite'] * 100 / (double)$total['vlrmontante'], 2);
        }

        return $dados;
    }

    public function existe()
    {
        $dados = $this->recuperarTodos('t1.vlrlimite', array(
            sprintf("t1.unicod = '%s'", $this->unicod),
            sprintf("t1.foncod = '%s'", $this->foncod),
            sprintf("t1.dspid = %d", $this->dspid)
        ));

        if ($dados) {
            return $dados[0]['vlrlimite'];
        }

        return false;
    }
}
