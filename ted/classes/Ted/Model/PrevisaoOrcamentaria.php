<?php

/**
 * Class Ted_Model_PrevisaoOrcamentaria
 * @author Lucas Oliveira
 * @author Lindalberto Rufino
 */
class Ted_Model_PrevisaoOrcamentaria extends Modelo
{

    /**
     * @initialize()
     */
    public function __construct($tcpid = null)
	{
		$this->arAtributos['tcpid'] = ($tcpid) ? $tcpid : Ted_Utils_Model::capturaTcpid();
		if(is_null($this->arAtributos['tcpid']))
		{
			throw new Exception("Nenhum Termo encontrado.");
		}
	}
	
	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.previsaoorcamentaria';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('proid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'proid' => NULL,
		'tcpid' => NULL,
		'ptrid' => NULL,			
		'pliid' => NULL,
		'prodsc'=> NULL,
		'ndpid' => NULL,
		'provalor' => NULL,
		'prodata' => NULL,
		'prostatus'=> NULL,
		'crdmesliberacao'=> NULL,
		'crdmesexecucao' => NULL,
		'proanoreferencia'=> NULL,
		'prgidfnde' => NULL,
		'prgfonterecurso' => NULL,
		'espid' => NULL,
		'esfid' => NULL,
		'creditoremanejado' => NULL
	);
	
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'tcpid'
	);
	
	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende - Copiador por Lindalberto Filho
	 * @return bool
	*/
	public function validaCamposObrigatorios()
	{
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
		if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
			return false;
			
		return true;
	}
	
	/**
	 * Cadastrar PO
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	function cadastrar()
	{
		if ($this->validaCamposObrigatorios()) {
			$this->arAtributos['proid'] = $this->inserir();
			return $this->commit();
		}
		 
		return false;
	}
	
	/**
	 * Atualizar PO
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar()
	{
		if ($this->validaCamposObrigatorios()) {
			$this->alterar();
			return $this->commit();
		}
		return false;
	}

    /**
     * @param $dados
     * @return bool
     */
    public function salvarDados($dados)
    {
		$this->popularDadosObjeto($dados);
        $this->arAtributos['provalor'] = str_replace(',', '.', str_replace('.', '', $this->arAtributos['provalor']));
		if (!empty($this->arAtributos['proid'])) {
            return $this->atualizar();
		} else {
            return $this->cadastrar();
        }

		return false;
	}

    /**
     * @param $crdmesexecucao
     * @return bool
     */
    public function updateMonths($crdmesexecucao)
    {
        if (!$crdmesexecucao) return false;

        $strSQL = sprintf("
            update ted.previsaoorcamentaria set crdmesexecucao = %d where tcpid = %d and prostatus = 'A'
        ", $crdmesexecucao, $this->arAtributos['tcpid']);
        //ver($strSQL, d);
        $this->executar($strSQL);
        $this->commit();
    }

    /**
     * Organiza o array com os dados para o metodo populate da classe model
     * @param array $post
     * @return array|bool
     */
    public function prepareData(array $post)
    {
        if (!count($post)) return false;

        $arrayData = $arrTemp = array();
        $keyMaps = array_keys($post);
        $crdmesexecucao = $post['crdmesexecucao'][0];

        foreach ($post['proid'] as $k => $v) {
            foreach ($keyMaps as $postKey) {
                if ($postKey == 'crdmesexecucao') {
                    $arrTemp[$postKey] = $crdmesexecucao;
                } else {
                    $arrTemp[$postKey] = $post[$postKey][$k];
                }
            }
            $arrayData[] = $arrTemp;
            $arrTemp = array();
        }

        return $arrayData;
    }
	
	/**
	 * 
	 * @param unknown $ptres
	 */
	public function buscaPtres()
	{
		$sql = "
			SELECT DISTINCT
				p.ptrid as codigo,
				ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
			FROM monitora.ptres p
			JOIN public.unidadegestora u
				ON u.unicod = p.unicod
			WHERE p.ptrano = '{$_SESSION['exercicio']}'
				AND p.ptrstatus = 'A'
				AND u.unicod IN ( '26101','26298','26291','26290' )
			
		";
		$list = $this->carregar($sql);
        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : null;	
				
	}
	
	/***-------------------Funções Carregadas quando um PTRES for selecionado--------------------*/
	 
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
			WHERE
				ptrid = {$ptrid}
		";

		return $this->pegaUm($strSQL);
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
	 * Retorna o plano interno de acordo com o ptres passado
     * @return string|false
	 */
	public function getPlanoInterno($ptrid)
	{
		if (!$ptrid) return null;

        $strSQL = "
			SELECT DISTINCT 
				p.pliid as codigo,
				plicod||' - '||plidsc as descricao
			FROM
				monitora.pi_planointerno p
			INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
			WHERE
				pt.ptrid = {$ptrid}
			ORDER by 2
		";

		$lista = $this->carregar($strSQL);
        if (!$lista) return false;

        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        foreach ($lista as $ptrid) {
            $html.= "<option value=\"{$ptrid['codigo']}\" label=\"{$ptrid['descricao']}\">{$ptrid['descricao']}</option>";
        }

        return $html;
	}

    /**
     * Retorna um conjunto de dados com todas as previões orçamentárias
     * para um determinado TED
     * @return array|void
     */
    public function getPrevisao($proid = null)
	{
		$query = "
			SELECT DISTINCT
				pro.proid,
				%s
				pro.tcpid as id,
				pro.proanoreferencia,
				a.acacod,
				ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod AS ptrid_descricao,
				SUBSTR(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' AS pliid_descricao,
				CASE 
					WHEN a.acatitulo IS NOT NULL THEN SUBSTR(a.acatitulo, 1, 70)||'...' 
					ELSE SUBSTR(a.acadsc, 1, 70)||'...' 
					END AS acatitulo,
				SUBSTR(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc AS ndp_descricao,
				CASE
	            	WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
	                	COALESCE(pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid), 0)
					ELSE
	                  	COALESCE(pro.provalor, 0)
	                END AS valor,
				CASE
	            	WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
	                	TRIM(TO_CHAR(pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid), '999G999G999G999G999D99'))
					ELSE
	                   	TRIM(TO_CHAR(pro.provalor, '999G999G999G999G999D99'))
					END AS provalor,
				crdmesliberacao,
				crdmesexecucao as crdmesexecucao,
				pro.ptrid,
				pro.pliid,
				pro.ndpid,
				pro.proid,
				pro.prodata,
				(SELECT CASE
                    when codsigefnc::text IS NOT NULL THEN codsigefnc::text
                    when codncsiafi IS NOT NULL THEN codncsiafi
                    ELSE '' END AS lote
				FROM ted.previsaoparcela ppa2
				WHERE ppa2.proid = pro.proid AND (ppa2.ppacancelarnc = 'f' OR ppa2.ppacancelarnc IS NULL)) AS lote,
				pp.codsigefnc,
		        pp.codncsiafi,
		        tc.ungcodconcedente,
		        creditoremanejado
			FROM {$this->stNomeTabela} pro
			LEFT JOIN monitora.pi_planointerno pi 		ON (pi.pliid = pro.pliid)
			LEFT JOIN monitora.pi_planointernoptres pts ON (pts.pliid = pi.pliid)
			LEFT JOIN public.naturezadespesa ndp 		ON (ndp.ndpid = pro.ndpid)
			LEFT JOIN monitora.ptres p 					ON (p.ptrid = pro.ptrid)
			LEFT JOIN monitora.acao a 					ON (a.acaid = p.acaid)
			LEFT JOIN public.unidadegestora u 			ON (u.unicod = p.unicod)
			LEFT JOIN monitora.pi_planointernoptres pt 	ON (pt.ptrid = p.ptrid)
			LEFT JOIN ted.previsaoparcela pp		    ON (pp.proid = pro.proid)
			LEFT JOIN ted.termocompromisso tc       ON (tc.tcpid = pro.tcpid)
			LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
			WHERE pro.prostatus = 'A'
				AND pro.tcpid = {$this->arAtributos['tcpid']}
				%s
			ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao --pro.proid ASC,
        ";

        $proid = (null !== $proid) ? "AND pro.proid = {$proid}" : '';

        $estadoAtual = Ted_Model_TermoExecucaoDescentralizada::pegaEstadoAtual();
        if (!Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE() && $estadoAtual == EM_DESCENTRALIZACAO) {
            $query = sprintf($query, '', $proid);
        }

        if (Ted_Model_TermoExecucaoDescentralizada::concedenteIsFNDE() && $estadoAtual == EM_DESCENTRALIZACAO) {
            $query = sprintf($query, '', $proid);
        }

        if (Ted_Model_TermoExecucaoDescentralizada::emSolicitacaoDeAlteracao()) {
            $query = sprintf($query, 'pro.proid as cod,', $proid);
        }

        if ($estadoAtual != EM_DESCENTRALIZACAO || $estadoAtual == ALTERAR_TERMO_COOPERACAO) {
            $query = sprintf($query, '', $proid);
        }

        //ver($query, d);
        $method = ($proid) ? 'pegaLinha' : 'carregar';
        return $this->{$method}($query);
	}
	
	/**
	 * Função para detalhar Previsões orçamentarias de um Termo na aba de geração de PDF 
	 * @return multitype:
	 */
	public function buscaPrevisaoOrcamentariaPDF()
    {
		$strSQL ="
			SELECT	
				DISTINCT ptres||'-'|| p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod AS plano_trabalho,
				a.acacod AS acao,
				crdmesliberacao,
				proanoreferencia,
				CASE 
					WHEN a.acatitulo IS NOT NULL THEN SUBSTR(a.acatitulo, 1, 70)||'...' 
					ELSE SUBSTR(a.acadsc, 1, 70)||'...' END AS acao_loa,
				SUBSTR(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' AS plano_interno,
				SUBSTR(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc AS nat_despesa,
				CASE
	            	WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
	                	COALESCE(pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid), 0)
					ELSE
	                  	COALESCE(pro.provalor, 0)
	            END AS valor,
				CASE
	            	WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
	                	TRIM(TO_CHAR(pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid), '999G999G999G999G999D99'))
					ELSE
	                   	TRIM(TO_CHAR(pro.provalor, '999G999G999G999G999D99'))
				END AS provalor,
				--TO_CHAR(t.total, '999G999G999G999G999D99') AS total,
				(select CASE
                    when codsigefnc::text IS NOT NULL THEN codsigefnc::text
                    when codncsiafi IS NOT NULL THEN codncsiafi
                    ELSE '' END AS notacredito
                FROM ted.previsaoparcela where proid = pro.proid
                ) as notacredito
			FROM {$this->stNomeTabela} pro
			LEFT JOIN monitora.pi_planointerno pi ON pi.pliid = pro.pliid
			LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
			LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
			LEFT JOIN monitora.ptres p ON p.ptrid = pro.ptrid
			LEFT JOIN monitora.acao a ON a.acaid = p.acaid
			LEFT JOIN public.unidadegestora u ON u.unicod = p.unicod
			LEFT JOIN monitora.pi_planointernoptres pt ON pt.ptrid = p.ptrid
			/*JOIN (
				SELECT SUM(provalor) AS total,
					 tcpid
				FROM {$this->stNomeTabela} 
					WHERE prostatus = 'A'
				GROUP BY tcpid
			) AS t on t.tcpid = pro.tcpid*/

			WHERE pro.prostatus = 'A' AND pro.tcpid = {$_GET['ted']}
			ORDER BY proanoreferencia, crdmesliberacao, notacredito
	    ";
		//ver($strSQL, d);
		return $this->carregar($strSQL);
	}

    /**
     * @return array|bool|void
     */
    public function capturaPrazoTotalPO()
    {
        $strSQL = "
			SELECT *
			FROM {$this->stNomeTabela}
			WHERE tcpid = {$this->arAtributos['tcpid']} 
				AND prostatus = 'A'
				AND crdmesexecucao IS NOT NULL
			ORDER BY proid LIMIT 1
		";
		
		return $this->pegaLinha($strSQL);
	}

    /**
     * @param $proid
     * @return array|bool|void
     */
    public function get($proid)
    {
        $sql = "
            SELECT * FROM {$this->stNomeTabela} WHERE proid = %s
        ";

        $stmt = sprintf($sql, (int) $proid);
        return $this->pegaLinha($stmt);
    }

    /**
     * @param $tcpid
     * @return array|bool|void
     */
    public function getPrevisoes()
    {
        $sql = "
            SELECT * FROM {$this->stNomeTabela} WHERE tcpid = %d and prostatus = 'A'
        ";

        $stmt = sprintf($sql, (int) $this->arAtributos['tcpid']);
        return $this->carregar($stmt);
    }
    
    /**
     * Captura uma lista de Previsão Orçamentária de acordo com o TCPID
     * selecionado para aba FNDE -> Solicitar NC ao SIGEF.
     * @param int $tcpid
     * @return NULL|array
     */
    public function listaPrevisaoOrcamentariaEnviarNC($tcpid)
    {
        if (is_null($tcpid)) {
            return null;
        }

        $sql = "
            SELECT 
                * 
            FROM {$this->stNomeTabela} tp
            INNER JOIN monitora.pi_planointerno pi ON (pi.pliid = tp.pliid)
            WHERE tp.tcpid = {$tcpid}
                AND tp.proid NOT IN(
                    SELECT pre.proid FROM ted.previsaoparcela pre
                    JOIN {$this->stNomeTabela} pro ON pro.proid = pre.proid
                    WHERE pro.tcpid = {$tcpid} AND pro.prostatus = 'A'
                )
                AND tp.prostatus = 'A'
        ";

        //ver($sql, d);
        $lista = $this->carregar($sql);
        if (!$lista) return false;

        foreach ($lista as $k => $v) {
            $arr = array();
            foreach ($v as $i => $d) {
                $arr[$i] = utf8_encode($d);
            }
            $lista[$k] = $arr;
        }

        return $lista;
    }

    /**
     * @param $proid
     * @return array|bool|null|void
     */
    public function pegaPrevisaoOrcamentariaEnviarNC($proid)
    {
        if(is_null($proid)){
            return null;
        }
        $sql = <<<DML
            SELECT DISTINCT
                pro.proid,
                ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
                substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
                substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
                pro.ptrid,
                a.acacod,
                pro.pliid,
                case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
                pro.ndpid,
                to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
                coalesce(pro.provalor, 0) as valor,
                crdmesliberacao,
                crdmesexecucao,
                pro.proid,
                pro.proanoreferencia,
                pro.prgidfnde,
                pro.esfid,
                pro.espid,
                pro.prgfonterecurso
            FROM ted.previsaoorcamentaria pro
            LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
            LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
            LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
            LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
            LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
            LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
            LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
            WHERE pro.prostatus = 'A'
                AND pro.proid = {$proid}
DML;
                
        $po = $this->pegaLinha($sql);
        if (!$po) return false;
     
        return $po;
    }

    /**
     * @param $plicod
     * @return array|bool|null|void
     */
    public function pegaCelulaOrcamentariaEnviarNC($plicod)
    {
        if(is_null($plicod)){
            return null;
        }

        $sql = <<<DML
            SELECT
                prgid as codigo,
                prgcodfnde || ' - ' || plicod || ' - ' || gescod || ' - ' ||
                tpddoccod || ' - ' || obscod || ' - ' ||  eventocontabil as descricao
            FROM ted.dadosprogramasfnde
            WHERE plicod = '{$plicod}'
                AND eventocontabil = '300300'
            ORDER BY prgcodfnde, gescod, obscod
DML;

        //ver($sql, d);
        $resultado = $this->carregar($sql);
        if (!$resultado) return false;
     
        return $resultado;
    }

    /**
     * @return array|bool|void
     */
    public function listaEspecieNC()
    {        
        $sql = <<<DML
            SELECT 
                espid AS codigo, 
                espdsc AS descricao 
            FROM ted.tipoespecie
            ORDER BY espid
DML;
                
        $resultado = $this->carregar($sql);
        if (!$resultado) return false;
     
        return $resultado;
    }

    /**
     * @return array|bool|void
     */
    public function listaEsferaNC()
    {  
        $sql = <<<DML
            SELECT 
                esfid AS codigo, 
                esfdsc AS descricao 
            FROM ted.tipoesfera
            ORDER BY esfid
DML;
                
        $resultado = $this->carregar($sql);
        if (!$resultado) return false;
     
        return $resultado;
    }

    /**
     * @return array|bool|void
     */
    public function listaFonteRecursoNC()
    {
        $sql = <<<DML
            SELECT
                frecodfonte AS codigo,
                frecodfonte || ' - ' || fredscfonte AS descricao
            FROM ted.fonterecurso
            ORDER BY frecodfonte
DML;
                
        $resultado = $this->carregar($sql);
        if (!$resultado) return false;
     
        return $resultado;
    }

    /**
     * Retorna dados para um input[select] de anos
     * @return string
     */
    public function getIntervaloAnos()
    {
        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        foreach (range(2012, 2023) as $year) {
            $html.= "<option value=\"{$year}\" label=\"{$year}\">{$year}</option>";
        }
        return $html;
    }

    /**
     * Retorna dados para um input[select] de meses de execução
     * @return string
     */
    public function getIntervaloMeses()
    {
        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        $html.= '<option value="1" label="1 Mês">1 Mês</option>';
        foreach (range(2, 50) as $month) {
            $html.= "<option value=\"{$month}\" label=\"{$month} Meses\">{$month} Meses</option>";
        }
        return $html;
    }

    /**
     * Retorna o combo de Natureza de Despesas
     * @return bool|string
     */
    public function getNaturezaDespesa()
    {
        $strSQL = "
			SELECT DISTINCT
			    ndpid AS codigo,
			    --substr(ndpcod, 1, 6) AS descricao
				substr(ndpcod, 1, 6) || ' - ' || ndpdsc AS descricao
			FROM public.naturezadespesa
			WHERE ndpstatus = 'A'
    			AND sbecod = '00'
    			AND edpcod != '00'
    			AND SUBSTR(ndpcod,1,2) NOT IN ('31', '32', '46', '34')
				AND (SUBSTR(ndpcod, 3, 2) IN ('80', '90', '91','40') OR SUBSTR(ndpcod, 1, 6) IN ('335041','339147','335039', '445041', '333041'))
			ORDER BY 2
		";

        $list = $this->carregar($strSQL);
        if (!$list) return false;

        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        foreach($list as $item) {
            $html.= "<option value='{$item['codigo']}' label='{$item['descricao']}'>{$item['descricao']}</option>";
        }
        return $html;
    }

    /**
     * @return string
     */
    public function getMesLiberacao()
    {
        $list = array(
            1 => 'Janeiro',
            2 => 'Fevereiro',
            3 => 'Março',
            4 => 'Abril',
            5 => 'Maio',
            6 => 'Junho',
            7 => 'Julho',
            8 => 'Agosto',
            9 => 'Setembro',
            10 => 'Outubro',
            11 => 'Novembro',
            12 => 'Dezembro',
        );

        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        foreach($list as $key => $item) {
            $html.= "<option value='{$key}' label='{$key} - {$item}'>{$key} - {$item}</option>";
        }
        return $html;
    }

    /**
     * Retorna o conjunto de NC ja cadasradas para as previsões orçamentárias de um termo
     * @return null|resource
     */
    public function getGrupoNC()
    {
        $strSQL = "
            SELECT vTable.notacredito FROM (
                SELECT
                    CASE WHEN ppa2.codncsiafi IS NOT NULL THEN ppa2.codncsiafi
                    WHEN ppa2.codsigefnc::text IS NOT NULL THEN ppa2.codsigefnc::text
                    END AS notacredito
                FROM
                    ted.previsaoparcela ppa2
                WHERE
                ppa2.proid in (
                    select proid from ted.previsaoorcamentaria where tcpid = {$this->arAtributos['tcpid']} and prostatus = 'A'
                ) AND (ppa2.ppacancelarnc = 'f' OR ppa2.ppacancelarnc IS NULL)
            ) vTable
            GROUP BY vTable.notacredito
            ORDER BY vTable.notacredito ASC
        ";

        $results = $this->carregar($strSQL);
        if (!$results) return false;

        $arr = array();
        foreach ($results as $nc) {
            array_push($arr, '"'.$nc['notacredito'].'"');
        }
        return implode(',', $arr);
    }

    /**
     * Pega os valores somados a cada Nota de Crédito
     * @return bool|string
     */
    public function getProvalorGroup()
    {
        $strSQL = "
            SELECT
                vTable.lote,
                TRIM(TO_CHAR(SUM(vTable.provalor), '999G999G999G999G999D99')) as provalor
            FROM (
                SELECT DISTINCT
                    pro.proid,
                    CASE
                    WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
                        pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid)
                    ELSE
                        pro.provalor
                    END AS provalor,
                    (SELECT
                        CASE WHEN ppa2.codncsiafi IS NOT NULL THEN ppa2.codncsiafi
                        WHEN ppa2.codsigefnc::text IS NOT NULL THEN ppa2.codsigefnc::text
                        END AS notacredito
                    FROM ted.previsaoparcela ppa2
                    WHERE ppa2.proid = pro.proid AND (ppa2.ppacancelarnc = 'f' OR ppa2.ppacancelarnc IS NULL)) AS lote
                FROM {$this->stNomeTabela} pro
                LEFT JOIN ted.previsaoparcela pp	   ON (pp.proid = pro.proid)
                LEFT JOIN ted.termocompromisso tc      ON (tc.tcpid = pro.tcpid)
                WHERE
                    pro.prostatus = 'A'
                    AND pro.tcpid = {$this->arAtributos['tcpid']}
            ) vTable
            WHERE vTable.lote IS NOT NULL
            GROUP BY vTable.lote
            ORDER BY vTable.lote ASC
        ";

        //ver($strSQL, d);
        $results = $this->carregar($strSQL);
        if (!$results) return false;

        $arr = array();
        foreach ($results as $nc) {
            array_push($arr, '"'.$nc['provalor'].'"');
        }
        return implode(',', $arr);
    }

    /**
     * @param $keyword
     */
    public function searchPtres($keyword)
    {
        $strSQL = "
            SELECT * FROM (
                SELECT DISTINCT
                    p.ptrid as codigo,
                    ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
                FROM monitora.ptres p
                JOIN public.unidadegestora u
                    ON u.unicod = p.unicod
                WHERE p.ptrano = '{$_SESSION['exercicio']}'
                AND p.ptrstatus = 'A'
                AND u.unicod IN ('26101','26298','26291','26290')
            ) AS vTable
            ".($keyword ? "WHERE codigo = {$keyword}" : "")."
		";

        //ver($strSQL, d);
        $itens = $this->carregar($strSQL);
        $arrJson = array();
        if (!$itens) {
            $arrJson[] = array(
                'id' => '',
                'name' => 'Sem registros',
            );

            header('Content-Type: application/json');
            echo simec_json_encode($arrJson);
            die;
        }

        foreach ($itens as $item) {
            $arrJson[] = array(
                'id' => $item['codigo'],
                'name' => $item['descricao'],
            );
        }

        $d = array();
        foreach ($arrJson as $k => $row) {
            $d[] = array_map('utf8_encode', $row);
        }

        header('Content-Type: application/json');
        echo simec_json_encode($d);
        die;
    }

    public function getPtres($currentYear = false)
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
                AND u.unicod IN ('26101','26298','26291','26290')
                %s
            ) AS vTable
            ORDER BY vTable.descricao ASC
        ";

        if (!$currentYear) {
            $nextYear = ($_SESSION['exercicio']+1);
            $stmt = sprintf($strSQL, "AND p.ptrano < '{$nextYear}'");
        } else {
            $stmt = sprintf($strSQL, "AND p.ptrano = '{$_SESSION['exercicio']}'");
        }

        $list = $this->carregar($stmt);

        $html = '<option value="" label="-Selecione-">-Selecione-</option>';
        if (is_array($list)) {
            foreach($list as $key => $item) {
                $html.= "<option value='{$item['codigo']}' label='{$item['descricao']}'>{$item['descricao']}</option>";
            }
        }

        return $html;
    }

    /**
     * @param $proid
     * @return bool
     */
    public function deletePrevisao($proid, $transferencia = false)
    {
        if ($transferencia) {
            $this->executar("delete from ted.creditoremanejado where proid = {$proid}");
        }

        $strSQL = "
            update {$this->stNomeTabela} set prostatus = 'I' where proid = {$proid}
        ";

        $this->executar($strSQL);
        return ($this->commit()) ? true : false;
    }

    /*
    public function deleteNCGroup($nc)
    {
        $strSQL = "
            delete from ted.previsaoparcela where proid in (
                select proid from ted.previsaoorcamentaria where tcpid = 1236
            ) and codncsiafi = '{$nc}';
        ";
    }*/

    /**
     * Retorna dados com extrato dos aditivos e devoluções
     * @param $nc
     * @return array|null|void
     */
    public function pegaExtratoNotaCredito($nc)
    {
        $strSQL = "
            select * from (
                select
                    proid,
                    ppaid,
                    Case
                        when ppamesenvio = '1' then 'Janeiro'
                        when ppamesenvio = '2' then 'fevereiro'
                        when ppamesenvio = '3' then 'Março'
                        when ppamesenvio = '4' then 'Abril'
                        when ppamesenvio = '5' then 'Maio'
                        when ppamesenvio = '6' then 'Junho'
                        when ppamesenvio = '7' then 'Julho'
                        when ppamesenvio = '8' then 'Agosto'
                        when ppamesenvio = '9' then 'Setembro'
                        when ppamesenvio = '10' then 'Outubro'
                        when ppamesenvio = '11' then 'Novembro'
                        when ppamesenvio = '12' then 'Dezembro'
                    end as ppamesenvio,
                    tcpnumtransfsiafi,
                    case when ppanumcancelanc is not null then ppanumcancelanc else codncsiafi end as codncsiafi,
                    ppavlrparcela AS ppavlrparcela,
                    ppacancelarnc,
                    false as devolucao
                from ted.previsaoparcela
                where
                    codncsiafi = '{$nc}'
                union all
                select
                    proid,
                    null as ppaid,
                    null as ppamesenvio,
                    null as tcpnumtransfsiafi,
                    nc_devolucao as codncsiafi,
                    valor AS ppavlrparcela,
                    null as ppacancelarnc,
                    true as devolucao
                from ted.creditoremanejado
                where proid in (
                    select proid from ted.previsaoparcela where codncsiafi = '{$nc}'
                )
            ) vTable
            order by vTable.proid, vTable.tcpnumtransfsiafi ASC
        ";

        $results = $this->carregar($strSQL);
        if (!$results) return false;

        $strSQL = "
            SELECT
                TRIM(TO_CHAR(SUM(vTable.provalor), '999G999G999G999G999D99')) as provalor
            FROM (
                SELECT
                    pro.proid,
                    CASE
                    WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
                    pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid)
                    ELSE
                    pro.provalor
                    END AS provalor,
                    (SELECT ppa2.codncsiafi FROM ted.previsaoparcela ppa2 WHERE ppa2.ppaid = (SELECT MAX(ppa1.ppaid) FROM ted.previsaoparcela ppa1 WHERE ppa1.proid = pro.proid) AND ppa2.ppacancelarnc = 'f') AS lote,
                    pp.codncsiafi
                FROM ted.previsaoorcamentaria pro
                LEFT JOIN ted.previsaoparcela pp       ON (pp.proid = pro.proid)
                LEFT JOIN ted.termocompromisso tc      ON (tc.tcpid = pro.tcpid)
                WHERE
                    pro.prostatus = 'A'
                    AND pro.tcpid = {$this->arAtributos['tcpid']}
            ) vTable
            WHERE vTable.lote = '{$nc}'
            GROUP BY vTable.lote
            ORDER BY vTable.lote ASC
        ";
        $total = $this->pegaUm($strSQL);

        $arrJson = array();
        $arrJson['tcpnumtransfsiafi'] = $results[0]['tcpnumtransfsiafi'];
        $arrJson['total'] = $total;
        $arrJson['extrato'] = array();
        foreach ($results as $item) {
            $arrJson['extrato'][] = array(
                'ppamesenvio' => utf8_encode($item['ppamesenvio']),
                'codncsiafi' => $item['codncsiafi'],
                'ppavlrparcela' => number_format($item['ppavlrparcela'], 2, ',', '.'),
                'devolucao' => ($item['devolucao'] == 'f') ? '' : 'true',
            );
        }

        //ver($arrJson);
        return simec_json_encode($arrJson);
    }

    /**
     * Salva a nota de crédito fazendo o vinculo com as previsões orçamentárias
     * @return boolean
     */
    public function salvarNotaCredito()
    {
        $arrProid = explode(',', $_POST['proid']);

        if (!count($arrProid)) return false;

        foreach ($arrProid as $proid) {

            if ($this->pegaUm("select count(ppaid) from ted.previsaoparcela where proid = {$proid}")) {
                continue;
            }

            $sqlFind = "SELECT provalor, crdmesliberacao FROM ted.previsaoorcamentaria WHERE proid = %d";
            $stmt = sprintf($sqlFind, (int) $proid);
            $dados = $this->pegaLinha($stmt);

            $strSQL = "
                INSERT INTO
                    ted.previsaoparcela(proid, ppavlrparcela, tcpnumtransfsiafi, ppacancelarnc, ppamesenvio, codncsiafi)
                VALUES (%d, '%s', %d, '%s', %d, '%s')
            ";

            $stmt = sprintf($strSQL, $proid, $dados['provalor'], $_POST['tcpnumtransfsiafi'], 'f', $dados['crdmesliberacao'], $_POST['codncsiafi']);
            $this->executar($stmt);
            $this->commit();
        }

        return true;
    }

    /**
     * Verifica se existe previsao orcamentaria sem nota de credito emitida
     * @return array|void
     */
    public function Pega_Nd_Sem_Nc()
    {
        $tedID = Ted_Utils_Model::capturaTcpid();

        $strSQL = sprintf("
            select * from {$this->stNomeTabela} po
            join monitora.pi_planointerno pi on pi.pliid = po.pliid
            where tcpid = %d
            and po.proid not in(
                select pre.proid from ted.previsaoparcela pre
                join {$this->stNomeTabela} pro on pro.proid = pre.proid
                where pro.tcpid = %d
            );
        ", $tedID, $tedID);
        //ver($strSQL, d);

        return $this->carregar($strSQL);
    }

    /**
     * @param array $row
     * @return bool
     */
    public function permiteRemanejamento($proid)
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();
        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();

        //verifica se existe historico de termo em diligencia
        $hstDiligencia = $this->pegaUm("
            select count(*)  from workflow.historicodocumento hst
            where hst.aedid = 1607
            and hst.docid = (
                select docid from ted.termocompromisso where tcpid = {$tcpid}
            )
        ");

        //verifica se existe historico de termo em execucao
        $hstExecucao = $this->pegaUm("
            select count(*) from workflow.historicodocumento hst
            where hst.aedid in (1609, 1618, 1650, 2440)
            and hst.docid = (
                select docid from ted.termocompromisso where tcpid = {$tcpid}
            )
        ");

        //verifica se ja foi solicitado NC para previsao orçamentaria
        $creditoLancado = $this->pegaUm("select * from ted.previsaoparcela where proid = {$proid}");

        if (($estadoAtual['esdid'] == ALTERAR_TERMO_COOPERACAO && Ted_Utils_Model::uoEquipeTecnicaProponente() && $creditoLancado)
           || ($estadoAtual['esdid'] == EM_DILIGENCIA && $hstDiligencia && $hstExecucao && $creditoLancado)) {
            return true;
        }

        return false;
    }

    /**
     *
     */
    public function permiteExcluirND(array $row)
    {
        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();

        if (Ted_Utils_Model::uoEquipeTecnicaProponente()) {
            if (in_array($estadoAtual['esdid'], array(EM_DILIGENCIA, EM_CADASTRAMENTO, ALTERAR_TERMO_COOPERACAO))) {
                if (empty($row['codsigefnc']) && empty($row['codncsiafi'])) {
                    return true;
                }
            }
        }

        return false;
    }

    public function permiteCadastroNC(array $row)
    {
        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();

        if (($estadoAtual['esdid'] == EM_EXECUCAO) && empty($row['codncsiafi']) && ($row['ungcodconcedente'] != UG_FNDE)) {
            return true;
        } else {

            $arEstadosDocCadNC = array(
                EM_DESCENTRALIZACAO,
                EM_EXECUCAO,
                RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR,
                RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA,
                RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD,
                TERMO_FINALIZADO
            );

            if ($row['ungcodconcedente'] != UG_FNDE && !$row['lote'] && in_array($estadoAtual['esdid'], $arEstadosDocCadNC)
               && possui_perfil_gestor(array(PERFIL_UG_REPASSADORA, PERFIL_CGSO, PERFIL_SUPER_USUARIO))) {
                return true;
            }
        }

        return false;
    }

    /**
     * Regra para permitir inserção de novas programações orçamentárias
     * @return bool
     */
    public function permiteInserirPrevisao()
    {
        if (Ted_Utils_Model::possuiPerfil(array(PERFIL_SUPER_USUARIO))) {
            return true;
        }

        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();

        $situacoes = array(
            'cadastramento' => EM_CADASTRAMENTO,
            'alteracao' => ALTERAR_TERMO_COOPERACAO,
            'emDescentralizacao' => EM_DESCENTRALIZACAO,
            'analiseCoordenacao' => EM_ANALISE_OU_PENDENTE,
            'diligencia' => EM_DILIGENCIA,
            'ugRepassadora' => EM_ANALISE_PELA_CGSO
        );

        if (in_array($estadoAtual['esdid'], $situacoes)) {
            if (in_array($estadoAtual['esdid'], array($situacoes['ugRepassadora'], $situacoes['diligencia']))
                && possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA))) {
                return true;
            }

            if (($estadoAtual['esdid'] == $situacoes['cadastramento']) &&
                (Ted_Utils_Model::possuiPerfil(array(UO_EQUIPE_TECNICA, PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
                || (possui_perfil_gestor(array(UO_EQUIPE_TECNICA, PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
            ) {
                return true;
            }

            if (($estadoAtual['esdid'] == $situacoes['analiseCoordenacao']) &&
                (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_COORDENADOR_SEC))
                || Ted_Utils_Model::uoEquipeTecnicaConcedente())) {
                return true;
            }

            if (in_array($estadoAtual['esdid'], array($situacoes['alteracao'], $situacoes['diligencia'])) &&
                (Ted_Utils_Model::possuiPerfil(array(UO_EQUIPE_TECNICA, PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
                || (possui_perfil_gestor(array(UO_EQUIPE_TECNICA, PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
            ) {
                return true;
            }

            if (($estadoAtual['esdid'] == $situacoes['emDescentralizacao']) &&
                (Ted_Utils_Model::possuiPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
                || (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO, PERFIL_CGSO, PERFIL_UG_REPASSADORA)))
            ) {
                return true;
            }
        }

        return false;
    }

    public function transfereCredito(array $post)
    {
        $msg = '';

        if (empty($post['nc_devolucao'])) {
            $msg.= 'Preencha o campo NC Devolução. <br>';
        }

        if (!preg_match('!^\d{4}(NC|nc)\d{6}$!', $post['nc_devolucao'])) {
            $msg.= 'Formato inválido para valor da NC Devolução. <br>';
        }

        if (empty($post['valor_remanejar'])) {
            $msg.= 'Preencha o campo Valor. \n';
        }

        if ((int) $post['valor_remanejar'] == 0) {
            $msg.= 'O valor preenchido precisa ser maior do que zero. <br>';
        }

        $valor_remanejar = str_replace('.', '', $post['valor_remanejar']);
        $valor_remanejar = str_replace(',', '.', $valor_remanejar);
        $observacao = strip_tags($post['observacao']);
        $crobservacao = substr($observacao, 0, 300);
        $nc_devolucao = strip_tags($post['nc_devolucao']);
        $result = ($_POST['provalor']-$valor_remanejar);

        $provalor = $this->pegaUm("
            SELECT
                CASE
                WHEN (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid) IS NOT NULL THEN
                    COALESCE(pro.provalor - (SELECT SUM(cr.valor) FROM ted.creditoremanejado cr WHERE cr.proid = pro.proid), 0)
                ELSE
                    COALESCE(pro.provalor, 0)
                END AS valor
            from ted.previsaoorcamentaria pro
            where pro.proid = {$post['ghost_proid']}
        ");

        if ($valor_remanejar > $provalor) {
            $msg.= 'O valor informado é maior que o disponível na celula. <br>';
        }

        header("Content-Type: application/json");
        if (!empty($msg)) {
            echo simec_json_encode(array('mensagem' => $msg));
            return false;
        } else {

            if ($valor_remanejar == $post['provalor']) {
                $this->executar("UPDATE ted.previsaoorcamentaria SET prostatus = 'I', statusvalorzerado = 't' WHERE proid = {$post['ghost_proid']}");
            }

            $this->executar("INSERT INTO ted.creditoremanejado(proid, valor, nc_devolucao, crdata, crobservacao, usucpf)
                       VALUES ({$post['ghost_proid']}, $valor_remanejar, '{$nc_devolucao}', 'NOW()', '{$crobservacao}', '{$_SESSION['usucpf']}')");
            $this->commit();

            $msg = 'Credito transferido com sucesso!';
            echo simec_json_encode(array('mensagem' => $msg, 'redirect' => true));
            return true;
        }
    }

    /**
     * Busca saldo remanejado por termo de compromisso
     * @param $tcpid
     */
    public function showSaldo($tcpid)
    {
        if (!$tcpid) return false;

        $strSQL = "SELECT (SELECT sum(valor) FROM ted.creditoremanejado
               WHERE proid IN (SELECT proid FROM ted.previsaoorcamentaria WHERE tcpid = {$tcpid} and (prostatus = 'A' OR statusvalorzerado = 't'))) -
               COALESCE((SELECT sum(provalor) FROM ted.previsaoorcamentaria WHERE tcpid = {$tcpid} AND prostatus = 'A' AND creditoremanejado = 't'), 0) AS saldo";

        $result = $this->pegaLinha($strSQL);

        if ($result['saldo'] > 0) {
            echo '
                <div class="col-md-10" style="float:right;background-color:#163A58;color:#fff;padding:4px;border-radius: 0.5em 0.2em 0.2em 0.5em;text-align:right;">
                    <span style="font-weight:bold;font-size:12px;">Saldo disponível a ser remanejado</span>
                </div>
                <div class="col-md-12 text-right" style="color:#163A58;float:right;">
                    <span data-toggle="tooltip" data-placement="left" title="Clique para usar saldo disponível" class="glyphicon glyphicon-plus useBalanceAvailable"></span>&nbsp;
                    <span class="" style="font-weight:bold;font-size:12px;">R$ '
                    .number_format($result['saldo'], 2, ',', '.').
                    '</span>
                </div>
            ';
        }
    }

    /**
     * @param $tcpid
     * @param bool $retornaSaldo
     * @return bool
     */
    public function haveCash($tcpid, $retornaSaldo = false)
    {
        $strSQL = "SELECT (SELECT sum(valor) FROM ted.creditoremanejado
               WHERE proid IN (SELECT proid FROM ted.previsaoorcamentaria WHERE tcpid = {$tcpid} and (prostatus = 'A' OR statusvalorzerado = 't'))) -
               COALESCE((SELECT sum(provalor) FROM ted.previsaoorcamentaria WHERE tcpid = {$tcpid} AND prostatus = 'A' AND creditoremanejado = 't'), 0) AS saldo";

        $result = $this->pegaLinha($strSQL);

        if ($retornaSaldo) {
            return ($result['saldo'] > 0) ? $result['saldo'] : false;
        } else
            return ($result['saldo'] > 0) ? true : false;
    }

    /**
     * Verifica se existe programação orçamentária pendente de Nota de Crédito pelo SIGEF
     * @param $tcpid
     * @return bool
     */
    public function existePrevisaoSemNotaCredito($tcpid)
    {
        $strSQL = sprintf("
            select * from ted.previsaoorcamentaria po
            join monitora.pi_planointerno pi on pi.pliid = po.pliid
            where tcpid = %d and po.sigefid is null and po.codsigefnc is null
            -- retira elementos já enviados atraves de verificacao em ted.previsaoparcela
            and po.proid not in(
                select pre.proid from ted.previsaoparcela pre
                join ted.previsaoorcamentaria pro on pro.proid = pre.proid
                where pro.tcpid = %d
            )
        ", $tcpid, $tcpid);

        //ver($strSQL, d);
        $rs = $this->carregar($strSQL);
        return ($rs) ? true : false;
    }

    /**
     * Verifica se a previsão pode ser editada
     * @return bool
    */
    public static function verificaProgramacaoEditavel()
    {
        $estadoAtual = Ted_Utils_Model::pegaSituacaoTed();
        $situacoesPermitidas = array(
            EM_CADASTRAMENTO,
            EM_ANALISE_OU_PENDENTE, //Em análise pela coordenação
            ALTERAR_TERMO_COOPERACAO,
            EM_DILIGENCIA,
            EM_ANALISE_PELA_CGSO
        );

        if (in_array($estadoAtual['esdid'], $situacoesPermitidas)) {

            switch ($estadoAtual['esdid']) {
                case EM_CADASTRAMENTO:
                    return true;
                    break;
                case EM_ANALISE_OU_PENDENTE:
                    return false;
                    break;
                case ALTERAR_TERMO_COOPERACAO:
                    return true;
                    break;
                case EM_DILIGENCIA:
                    return true;
                    break;
                case EM_ANALISE_PELA_CGSO:
                    return true;
                    break;
            }
        }

        return false;
    }

    /**
     *
     */
    public function verifyEditableCell()
    {
        /*$estadoAtual = Ted_Utils_Model::pegaSituacaoTed();
        $allowed = array(
            ALTERAR_TERMO_COOPERACAO
        );*/

        $strSQL = sprintf("
            select max(hstid)
            from workflow.historicodocumento
            where aedid = %d and docid = (
                select docid from ted.termocompromisso where tcpid = %d
            )
        ", APROVADO_PELO_REPRESENTANTE_LEGAL_PROPONENTE, Ted_Utils_Model::capturaTcpid());
        $representanteLegalProponenteAprovou = $this->pegaUm($strSQL);

        if ($representanteLegalProponenteAprovou) {
            return false;
        } else {
            return true;
        }
    }
}