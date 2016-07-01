<?php

/**
 * Classe Situação Questões Pontuais PAR
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Situação das Questoes Pontuais PAR
 */
class SituacaoQuestoesPontuaisPar extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.situacaoquestoespontuais";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('sqpcod');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'sqpcod' => null,
		'sqpdsc' => null,
		'sqpcor' => null,
		'sqpstatus' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'sqpdsc'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'sqpcod',
		'sqpdsc',
		'sqpcor',
		'sqpstatus'	
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'sqpdsc',
		'sqpcor',
		'sqpstatus'
	);

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
				'<a style=\"cursor:pointer\" onclick=\"editarSituacaoQuestoesPontuais( ' || sqpcod || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirSituacaoQuestoesPontuais( ' || sqpcod || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				sqpdsc,
				sqpcor,
				CASE WHEN sqpstatus = 'I' THEN 'Inativo' WHEN sqpstatus = 'A' THEN 'Ativo' END AS sqpstatus
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
		$sql .= "        ORDER BY sqpcod ";

		return $sql;
	}

	/**
	 * Atualiza Situacao de Questoes Pontuais
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarSituacaoQuestoesPontuais(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente a situacao de Questoes Pontuais
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirSituacaoQuestoesPontuais( $sqpcod ){
		$this->carregarPorId( $sqpcod );
		$this->arAtributos['sqpstatus'] = 'I';
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
	public function populaSituacaoQuestoesPontuais( Array $arDados ){
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
	 * Cadastra Situacao Questoes Pontuais Montada no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarSituacaoQuestoesPontuais(){
		if( $this->validaCamposObrigatorios() ){
			$this->sqpcod = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

}