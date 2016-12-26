<?php

class Ted_Model_Monitora extends modelo
{
    /**
     * Retorna o combo de Natureza de Despesas
     * @return bool|string
     */
    public function getNaturezaDespesa()
    {
        $list = $this->carregar(self::getQueryNaturezaDespesa());
        if (!$list) return false;

        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        foreach($list as $item) {
            $html.= "<option value='{$item['codigo']}' label='{$item['descricao']}'>{$item['descricao']}</option>";
        }
        return $html;
    }

    public function getPtres($currentYear = false, $formato = 'html')
    {
        $strSQL = "
            SELECT * FROM (
                SELECT DISTINCT
                    p.ptrid as codigo,
                    ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao,
                    ptres
                FROM monitora.ptres p
                JOIN public.unidadegestora u
                    ON u.unicod = p.unicod
                WHERE p.ptrstatus = 'A'
                AND u.unicod IN (". UNIDADES_OBRIGATORIAS. ")
                %s
            ) AS vTable
            ORDER BY vTable.descricao ASC
        ";

        if (is_numeric($currentYear)) {
            $stmt = sprintf($strSQL, "AND p.ptrano = '{$currentYear}'");
        } elseif (!$currentYear) {
            $nextYear = ($_SESSION['exercicio']+1);
            $stmt = sprintf($strSQL, "AND p.ptrano < '{$nextYear}'");
        } else {
            $stmt = sprintf($strSQL, "AND p.ptrano = '{$_SESSION['exercicio']}'");
        }

        $list = $this->carregar($stmt);
        switch ($formato) {
            case 'html':
                $html = '<option value="" label="-Selecione-">-Selecione-</option>';
                if (is_array($list)) {
                    foreach($list as $key => $item) {
                        $html.= "<option value='{$item['codigo']}' label='{$item['descricao']}'>{$item['descricao']}</option>";
                    }
                }

                return $html;
                // -- no break
            case 'json':
                return simec_json_encode($list);
        }

        return $list;
    }

    /**
     * Retorna o plano interno de acordo com o ptres passado
     * @return string|false
     */
    public function getPlanoInterno($ptrid, $formato = 'html')
    {
        if (!$ptrid) {
            return null;
        }

        $params = array("plp.ptrid = {$ptrid}");
        $strSQL = self::getQueryPi($params);
        $lista = $this->carregar($strSQL);
        if (!$lista) {
            return false;
        }

        switch ($formato) {
            case 'html':
                $html = '<option value="" label="-Selecione-">-Selecione-</option>';
                foreach ($lista as $ptrid) {
                    $html.= "<option value=\"{$ptrid['codigo']}\" label=\"{$ptrid['descricao']}\">{$ptrid['descricao']}</option>";
                }

                return $html;
                // -- no break
            case 'json':
                return simec_json_encode(($lista));
                // -- no break
        }

        return $lista;
    }

    /**
     * Captura o Nome da ação e seta no formulário de acordo com o PTRES selecionado.
     * @param unknown $ptrid
     * @return NULL|Ambigous <boolean, string>
     */
    public function getAcaoPtrid($ptrid)
    {
        if (is_null($ptrid)) return null;

        $strSQL = "
			SELECT DISTINCT
				a.acacod
			FROM
				monitora.ptres p
			INNER JOIN monitora.acao a ON a.acaid = p.acaid
			INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
			WHERE
				ptrid = $ptrid
		";

        return $this->pegaUm($strSQL);
    }

    /**
     * Captura a Descrição da ação e seta no formulário de acordo com o PTRES selecionado.
     * @param unknown $ptrid
     * @return NULL|Ambigous <boolean, string>
     */
    public function getDescricaoAcao($ptrid)
    {
        if (is_null($ptrid)) return null;

        $strSQL = "
			SELECT DISTINCT
				case when acatitulo is null then substr(acadsc, 1, 70)||'...'
				else substr(acatitulo, 1, 70)||'...' end as acatitulo
			FROM
				monitora.ptres p
			INNER JOIN monitora.acao a ON a.acaid = p.acaid
			INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
			WHERE ptrid = {$ptrid}
		";

        return $this->pegaUm($strSQL);
    }

    public static function getQueryExercicioPtres()
    {
        $sql = <<<DML
SELECT DISTINCT ptrano AS codigo,
                ptrano AS descricao
  FROM monitora.ptres ptr
  ORDER BY ptrano DESC
DML;
        return $sql;
    }

    public static function getQueryPtres(array $params = array())
    {
        $sql = <<<DML
SELECT ptr.ptrid AS codigo,
       ptr.ptres AS descricao
  FROM monitora.ptres ptr
DML;
        return $sql;
    }

    public static function getQueryPi(array $params = array())
    {
        $where = '';
        if (!empty($params)) {
            $where = 'WHERE ' . implode(' AND ', $params);
        }

        $sql = <<<DML
SELECT DISTINCT pli.pliid AS codigo,
				pli.plicod ||' - '|| plidsc AS descricao
  FROM monitora.pi_planointerno pli
    INNER JOIN monitora.pi_planointernoptres plp ON (plp.pliid = pli.pliid)
  {$where}
  ORDER by descricao
DML;
        return $sql;
    }

    public static function getQueryNaturezaDespesa()
    {
        $sql = <<<DML
SELECT DISTINCT ndpid AS codigo,
			    substr(ndpcod, 1, 6) || ' - ' || ndpdsc AS descricao
  FROM public.naturezadespesa
  WHERE ndpstatus = 'A'
    AND sbecod = '00'
    AND edpcod != '00'
    AND SUBSTR(ndpcod, 1, 2) NOT IN('31', '32', '46', '34')
	AND (SUBSTR(ndpcod, 3, 2) IN('80', '90', '91','40')
          OR SUBSTR(ndpcod, 1, 6) IN ('335041', '339147', '335039', '445041', '333041'))
  ORDER BY descricao
DML;
        return $sql;
    }
}
