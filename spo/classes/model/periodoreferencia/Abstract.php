<?php
/**
 * Implementação da classe base de gerenciamento de Períodos de Referência.
 *
 * $Id: Abstract.php 98916 2015-06-22 12:37:10Z maykelbraz $
 */

/**
 * Abstração de períodos de referência.
 *
 * @see Modelo
 * @abstract
 */
abstract class Spo_Model_Periodoreferencia_Abstract extends Modelo
{
    /**
     * Indica o range efetivo do período. Definido pelos campos prfinicio e prffim.
     */
    const PERIODO_EFETIVO = 'E';

    /**
     * Indica o range de preenchimento período. Definido pelos campos prfpreenchimentoinicio e prfpreenchimentofim.
     */
    const PERIODO_PREENCHIMENTO = 'P';

    const DATA_COMPLETA = 'DD/MM/YYYY';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('prfid');

    /**
     * Nome da tabela, incluíndo esquema.
     * @var string
     */
    protected $stNomeTabela;

    protected $tipoAtributos = array(
        'prfinicio' => 'timestamp',
        'prffim' => 'timestamp',
        'prfpreenchimentoinicio' => 'timestamp',
        'prfpreenchimentofim' => 'timestamp'
    );

    /**
     * Lista de atributos da entidade.
     *
     * Para adicionar novos atributos, utilize o método init().
     * @var array
     */
    protected $arAtributos = array(
        'prfid' => null,
        'prsano' => null,
        'prftitulo' => null,
        'prfdescricao' => null,
        'prfinicio' => null,
        'prffim' => null,
        'prfpreenchimentoinicio' => null,
        'prfpreenchimentofim' => null,
        'prfcriacao' => 'NOW()'
    );

    /**
     * Implemente este método para incluir colunas adicionais ao $arAtributos.
     * Deixe vazio para criar apenas os campos padrões definidos aqui.
     *
     * @abstract
     */
    protected abstract function init();

    /**
     * Construtor. Valida se o nome da tabela está definido.
     *
     * @param integer $id Informando um ID, o sistema inicializa o objeto com os dados do id informado.
     */
    public function __construct($id = null)
    {
        if (empty($this->stNomeTabela)) {
            throw new Exception('Define o nome da tabela à qual esta classe se refere.');
        }
        $this->init();
        parent::__construct($id);
    }

    /**
     * Carrega os dados do período atual, com base nas datas efetivas.
     *
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $whereAdicional Condições adicionais para incluir na busca. Modelo: coluna => valor.
     * @return \Spo_Model_PeriodoReferencia
     */
    public function carregarAtual(array $whereAdicional = array())
    {
        $colunas = implode(', ', array_keys($this->arAtributos));

        $query = <<<DML
SELECT {$colunas}
  FROM {$this->stNomeTabela}
  WHERE CURRENT_DATE BETWEEN prfinicio AND prffim
DML;
        $where = '';
        if ($whereAdicional) {
            foreach ($whereAdicional as $coluna => $valor) {
                $where .= " AND {$coluna} = '{$valor}'";
            }
        }
        $query .= $where;
        $this->popularObjeto(
            array_keys($this->arAtributos),
            $this->pegaLinha($query)
        );
        return $this;
    }

	public function carregarPorId($id)
    {
        $campos = array_keys($this->arAtributos);
        $this->aplicarFormatacao($campos);
        $select = implode(', ', $campos);

		$id = trim(str_replace("'", "", (string)$id));

		$sql = <<<DML
SELECT {$select}
  FROM {$this->stNomeTabela}
  WHERE {$this->arChavePrimaria[0]} = '{$id}'
DML;

		$arResultado = $this->pegaLinha($sql);
		$this->popularObjeto(array_keys($this->arAtributos), $arResultado);

        return $this;
	}

    /**
     * Carrega as informações do último periodo de referência.
     *
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @param array $whereAdicional Condições adicionais para incluir na busca. Modelo: coluna => valor.
     * @return \Spo_Model_PeriodoReferencia
     */
    public function carregarUltimo(array $whereAdicional = array())
    {
        $query = <<<DML
SELECT MAX(prfid) AS prfid
  FROM {$this->stNomeTabela} pr
  WHERE pr.prsano = '{$this->arAtributos['prsano']}'
DML;
        $where = '';
        if ($whereAdicional) {
            foreach ($whereAdicional as $coluna => $valor) {
                $where .= " AND {$coluna} = '{$valor}'";
            }
        }

        $query .= $where;
        $this->popularObjeto(
            array_keys($this->arAtributos),
            $this->pegaLinha($query)
        );
        return $this;
    }

	public function recuperarTodos( $stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array())
    {
        $campos = array();
        if ('*' == $stCampos) {
            $campos = array_keys($this->arAtributos);
        } else {
            $campos = array_map('trim', explode(',', $stCampos));
        }

        $this->aplicarFormatacao($campos);

        $select = implode(', ', $campos);
        $where = $arClausulas?' WHERE ' . implode( ' AND ', $arClausulas ):'';
        $orderby = $stOrdem?" ORDER BY {$stOrdem}":'';

        $sql = <<<DML
SELECT {$select}
  FROM {$this->stNomeTabela}
  {$where}
  {$orderby}
DML;
        return $sql;
    }

    protected function aplicarFormatacao(array &$campos)
    {
        foreach ($campos as &$campo) {
            if ($this->ehTimestamp($campo)) {
                $campo = "TO_CHAR({$campo}, '" . self::DATA_COMPLETA . "') AS {$campo}";
            }
        }
        return $campos;
    }

    protected function ehTimestamp($campo)
    {
        return isset($this->tipoAtributos[$campo]) && ('timestamp' == $this->tipoAtributos[$campo]);
    }

    /**
     * Verfica se o periodo identificado pelo ID atualmente armazenado no objeto é valido, considerando o dia atual.
     *
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return bool
     */
    public function periodoValido($tipo = self::PERIODO_PREENCHIMENTO)
    {
        list($inicio, $fim) = $this->colunasDeRange($tipo);

        $query = <<<DML
SELECT CASE WHEN CURRENT_DATE NOT BETWEEN {$inicio} AND {$fim}
              THEN TRUE
            ELSE FALSE
          END AS validade_periodo
  FROM {$this->stNomeTabela}
  WHERE prfid = {$this->arAtributos['prfid']}
DML;
        if ($this->pegaUm($query) == 't') {
            return true;
        }
        return false;
    }

    /**
     * Retorna um array com o conjunto de colunas que definem um tipo de período.
     * Se precisar de novos ranges de período, basta sobreescrever este método e
     * adicionar as validações extras.
     *
     * @param string $tipo O range de período que se procura as colunas delimitadoras.
     * @return array
     * @see Spo_Model_Periodoreferencia::PERIODO_PREENCHIMENTO
     * @see Spo_Model_Periodoreferencia::PERIODO_EFETIVO
     */
    protected function colunasDeRange($tipo)
    {
        switch ($tipo) {
            case self::PERIODO_EFETIVO:
                return array('prfinicio', 'prffim');
            case self::PERIODO_PREENCHIMENTO:
                return array('prfpreenchimentoinicio', 'prfpreenchimentofim');
        }
        return array();
    }

    public function buscaPorId(){
        $query = <<<DML
            SELECT
                prfid,
                prsano,
                prftipo,
                prftitulo,
                prfdescricao,
                to_char(prfinicio, 'DD/MM/YYYY') AS prfinicio,
                to_char(prffim, 'DD/MM/YYYY') AS prffim,
                to_char(prfpreenchimentoinicio, 'DD/MM/YYYY') AS prfpreenchimentoinicio,
                to_char(prfpreenchimentofim, 'DD/MM/YYYY') AS prfpreenchimentofim
            FROM {$this->stNomeTabela}
            WHERE prfid = {$this->arAtributos['prfid']}
DML;
        return $this->pegaLinha($query);
    }

    /**
     * Converte o objeto para sua representação em string.
     *
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return bool
     */
    public function __toString()
    {
        if (!empty($this->arAtributos['prfid']) && (empty($this->arAtributos['prftitulo']) || empty($this->arAtributos['prfinicio']) || empty($this->arAtributos['prffim']))) {
            $this->carregarPorId($this->arAtributos['prfid']);
        }

        $inicio = date('d/m/Y', strtotime($this->prfinicio));
        $fim = date('d/m/Y', strtotime($this->prffim));
        return "{$this->prftitulo}: {$inicio} à {$fim}";
    }

    public function antesSalvar()
    {
        list($dia, $mes, $ano) = explode('/', $this->prfinicio);
        $this->prfinicio = "{$ano}-{$mes}-{$dia}";
        list($dia, $mes, $ano) = explode('/', $this->prffim);
        $this->prffim = "{$ano}-{$mes}-{$dia}";
        list($dia, $mes, $ano) = explode('/', $this->prfpreenchimentoinicio);
        $this->prfpreenchimentoinicio = "{$ano}-{$mes}-{$dia}";
        list($dia, $mes, $ano) = explode('/', $this->prfpreenchimentofim);
        $this->prfpreenchimentofim = "{$ano}-{$mes}-{$dia}";

        return parent::antesSalvar();
    }

    public function carregarPorAno($ano)
    {
        $colunas = implode(', ', array_keys($this->arAtributos));

        $sql = <<<DML
SELECT {$colunas}
  FROM {$this->stNomeTabela}
  WHERE prsano = '{$ano}'
DML;

		$arResultado = $this->pegaLinha($sql);
		$this->popularObjeto(array_keys($this->arAtributos), $arResultado);

        return $this;
    }
}
