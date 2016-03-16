<?php

/**
 * Classe Mesoregiao
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Mesoregiao
 */
class Mesoregiao extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.mesoregiao";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('mesid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'mesid' => null,
		'oesid' => null,
		'mescod' => null,
		'mesdsc' => null,
		'mescor' => null,
		'messtatus' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'm.mesdsc',
		'm.mescor'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'mesid',
		'oesid',
		'mescod',
		'mesdsc',
		'mescor',
		'messtatus'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'oesid',
		'mesdsc',
		'mescor',
		'messtatus'
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
				'<a style=\"cursor:pointer\" onclick=\"editarMesoregiao( ' || m.mesid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirMesoregiao( ' || m.mesid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				oes.estuf,
				m.mesdsc,
				m.mescor,
				o.orgdsc,
				CASE WHEN m.messtatus = 'I' THEN 'Inativo' WHEN m.messtatus = 'A' THEN 'Ativo' END AS messtatus
			FROM " . $this->stNomeTabela . " m
			JOIN sase.orgaoestado oes on oes.oesid = m.oesid
			JOIN sase.orgao o ON oes.orgid = o.orgid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}
		$sql .= "              ORDER BY m.mesid ASC ";

		return $sql;
	}

	/**
	 * Atualiza Mesoregiao
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarMesoregiao(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Mesoregiao
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirMesoregiao( $mesid ){
		$this->carregarPorId( $mesid );
		$this->arAtributos['messtatus'] = 'I';
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
	public function populaMesoregiao( Array $arDados ){
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
	 * Cadastra Mesoregiao Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarMesoregiao(){
		if( $this->validaCamposObrigatorios() ){
			$this->mesid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

}
