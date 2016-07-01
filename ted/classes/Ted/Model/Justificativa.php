<?php

/**
 * Class Ted_Model_Justificativa
 */
class Ted_Model_Justificativa extends Modelo
{
	/**
	 * Nome da Tabela
	 * @var String
	 */
	protected $stNomeTabela = 'ted.justificativa';

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('justid');

	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
		'justid' => NULL,
		'identificacao' => NULL,
		'objetivo' => NULL,
		'justificativa' => NULL,
		'tcpid'=> NULL
	);

	public function __construct($tcpid = null)
    {
		$this->arAtributos['tcpid'] = ($tcpid)? $tcpid : Ted_Utils_Model::capturaTcpid();
		if (is_null($this->arAtributos['tcpid'])) {
			throw new Exception("Nenhum Termo encontrado.");
		}
	}

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
		foreach ($this->arAtributosObrigatorios as $chave => $valor) {
            if (!isset($this->arAtributos[$valor])
             || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor])) {
                return false;
            }
        }
		return true;
	}

	/**
	 * Cadastrar Justificativa para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	function cadastrarJustificativa()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->arAtributos['justid'] = $this->inserir();
			return $this->commit();
		}

		return false;
	}

	/**
	 * Atualizar Justificativa para um termo
	 *
	 * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarJustificativa()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->alterar();
			return $this->commit();
		}
		return false;
	}

	/**
	 * Captura dados para utilização na aba Justificativa. Caso não exista justifica para o termo o sistema o cria e chama o método novamente.
	 * @return Ambigous <boolean, array(titulo,objetivo,justificativaCampo,ugRecebedora,ungcodconcedente,tcptipoemenda):>
	 */
	public function capturaDadosJustificativa()
	{
		$strSQL = "
            SELECT
                jus.justid,
                jus.identificacao,
                jus.objetivo,
                jus.justificativa,
                gestora.ungcod||' / '||gestora.ungdsc as ugrecebedora,
                gestora2.ungcod||' / '||gestora2.ungdsc as ugrepassadora,
                jus.tipoemenda
            FROM {$this->stNomeTabela} jus
            INNER JOIN ted.termocompromisso tcp on(tcp.tcpid = jus.tcpid)
            LEFT JOIN public.unidadegestora gestora on (gestora.ungcod = tcp.ungcodproponente)
            LEFT JOIN public.unidadegestora gestora2 on (gestora2.ungcod = tcp.ungcodconcedente)
            WHERE jus.tcpid = {$this->arAtributos['tcpid']}
		";
		//ver($strSQL, d);
		$consulta = $this->pegaLinha($strSQL);

		if ($consulta != null) {
			return $consulta;
		} else {
			if ($this->criarJustificativa()) {
				return $this->capturaDadosJustificativa();
			}
		}
	}

	/**
	 * Cria justificativa caso não exista e retorna a consulta da tabela através da função capturaDadosJustificativa()
	 */
	public function criarJustificativa()
    {
		if ($this->validaCamposObrigatorios()) {
			$this->cadastrarJustificativa();
			return true;
		}

		return false;
	}

    /**
     * Aba de Justificativa.
     * @param $_POST $dados contendo os campos do formulário referentes às colunas da tabela.
     * @return boolean
     */
    function gravarTermoDescentralizacao($dados)
    {
    	$this->popularDadosObjeto($dados);
    	if (!is_null($this->arAtributos['justid'])) {
            $emenda = new Ted_Model_Emenda();
            $emenda->update($dados);
    		return $this->atualizarJustificativa();
    	}
    	return FALSE;
    }

    /**
     * @param $ungcodproponente
     * @return bool|string|void
     */
    public function tipoEmentda()
    {
        $strSQL = "
            select
                count(e.emeid)
            from emenda.emenda e
                inner join ted.emendas em on (em.emeid = e.emeid)
            where
                em.ungcod = (select ungcodproponente from ted.termocompromisso where tcpid = {$this->arAtributos['tcpid']})
                and em.emeano = '{$_SESSION['exercicio']}'
        ";
        #ver($strSQL,d);
        return $this->pegaUm($strSQL);
    }

    /**
     * @return array|void
     */
    public function getOptions()
    {
        $strSQL = "
            SELECT
                e.emeid AS codigo,
                e.emecod || ' ('|| e.emeano||')' AS descricao
            FROM ted.emendas e
            WHERE
                e.ungcod = (SELECT ungcodproponente FROM ted.termocompromisso WHERE tcpid = {$this->arAtributos['tcpid']})
        ";
        #ver($strSQL,d);
        $return = $this->carregar($strSQL);
        $options = array();
        if ($return) {
            foreach ($return as $v) {
                $options[$v['codigo']] = $v['descricao'];
            }
        }

        return $options;
    }

    /**
     * @return bool|null|string|void
     */
    public function getEmenda()
    {
        $strSQL = "
            select emeid
            from ted.emendas
            where tcpid = {$this->arAtributos['tcpid']}
        ";

        $emeid = $this->pegaUm($strSQL);
        return ($emeid) ? $emeid : null;
    }
}