<?php

/**
 * Classe Submetas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Submetas
 * TODO classe não testada
 */
class Submetas extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.submeta";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('subid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'subid' => null,
		'metid' => null,
		'subtitulo' => null,
		'subtaxa' => null,
		'subtaxacrescnacional' => null,
		'submetanacional' => null,
		'subtempoconvergencia' => null,
		'subtaxaconvergencia' => null,
		'subpatamarminimo' => null,
		'substatus' => null,
		'subformulatcn' => null,
		'subformulatpc' => null,
		'subformulatxc' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'subtitulo'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'subid',
		'metid',
		'subtitulo',
		'subtaxa',
		'subtaxacrescnacional',
		'submetanacional',
		'subtempoconvergencia',
		'subtaxaconvergencia',
		'subpatamarminimo',
		'substatus',
		'subformulatcn',
		'subformulatpc',
		'subformulatxc'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'subtitulo',
		'metid',
		'substatus'
	);

	/**
	 * Faz calculo da Taxa de Crescimento Nacional (subtaxacrescnacional)
	 *
	 * @author Sávio Resende
	 * @return float $subtaxacrescnacional
	 */
	protected function calculaTCN(){
		$m = new EvalMath;

		if( !$this->subformulatcn )
			return true;

		$formula = str_replace('submetanacional', $this->submetanacional, $this->subformulatcn);
		$formula = str_replace('subtaxa', $this->subtaxa, $formula);
		$this->subtaxacrescnacional = $m->evaluate($formula);

		return $this->subtaxacrescnacional;
	}

	/**
	 * Faz calculo do Tempo de Convergência (subtempoconvergencia)
	 * 
	 * @author Sávio Resende
	 * @return float $subtempoconvergencia
	 */
	protected function calculaTPC(){
		$m = new EvalMath;

		if( !$this->subformulatpc )
			return true;
		$formula = str_replace('subtaxa', $this->subtaxa, $this->subformulatpc);
		$formula = str_replace('submetanacional', $this->submetanacional, $formula);
		$formula = str_replace('subtaxacrescnacional', $this->subtaxacrescnacional, $formula);
		$this->subtempoconvergencia = $m->evaluate($formula);

		return $this->subtempoconvergencia;
	}

	/**
	 * Faz calculo da Taxa de Convergência (subtaxaconvergencia)
	 *
	 * @author Sávio Resende
	 * @return float subtaxaconvergencia
	 */
	protected function calculaTXC(){
		$m = new EvalMath;

		if( !$this->subformulatxc )
			return true;
		$formula = str_replace('subtaxa', $this->subtaxa, $this->subformulatxc);
		$formula = str_replace('submetanacional', $this->submetanacional, $formula);
		$formula = str_replace('subtempoconvergencia', $this->subtempoconvergencia, $formula);
		$this->subtaxaconvergencia = $m->evaluate($formula);

		return $this->subtaxaconvergencia;
	}
 	
	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT 
				'<a style=\"cursor:pointer\" onclick=\"editarSubmetas( ' || s.subid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirSubmetas( ' || s.subid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				s.subtitulo as subtitulo,
				m.mettitulo as mettitulo,
				s.subtaxa as subtaxa,
				s.submetanacional as submetanacional,
				s.subpatamarminimo as subpatamarminimo,
				CASE WHEN s.substatus = 'I' THEN 'Inativo' WHEN s.substatus = 'A' THEN 'Ativo' END AS substatus
			FROM " . $this->stNomeTabela . " s
			LEFT JOIN sase.meta m ON m.metid = s.metid " . ( ($filtros[''])?"":"" );
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}

		return $sql;
	}

	/**
	 * Atualiza Submetas
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarSubmetas(){
		if( $this->validaCamposObrigatorios() ){

			$this->calculaTXC(); // subtaxacrescnacional
			$this->calculaTPC(); // subtempoconvergencia
			$this->calculaTCN(); // subtaxaconvergencia
			
			// $this->alterar();
			// return $this->commit();
			return true;
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Submetas
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirSubmetas( $subid ){
		$this->carregarPorId( $subid );
		$this->arAtributos['substatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaSubmetas( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Submetas Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarSubmetas(){
		if( $this->validaCamposObrigatorios() ){
			$this->subid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

}
