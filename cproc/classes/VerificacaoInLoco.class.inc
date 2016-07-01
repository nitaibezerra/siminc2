<?php

/**
 * Classe Modelo VerificacaoInLoco CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo VerificacaoInLoco CPROC
 */
class VerificacaoInLoco extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.verificacaoinloco";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("vrlid");

	/**
	 * Status
	 * @name $status
	 * @var array
	 * @access protected
	 */
	protected $status = false;
	
	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
			"vrlid" => null,
			"prcid" => null,
			"vrltipo" => null,
			"vrldata" => null,
			"vrlobservacoes" => null,
			"vrlnumprocemec" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"vrlid",
			"prcid",
			"vrltipo",
			"vrldata",
			"vrlobservacoes",
			"vrlnumprocemec"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"vrltipo",
			"vrldata"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid",
			"vrldata"
		);

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	// public abastract function montaListaQuery( $filtros = false );
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT 
				'<a style=\"cursor:pointer\" onclick=\"editar( ' || ".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluir( ' || ".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				vrltipo,
				vrldata,
				vrlobservacoes
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((!in_array($chave, $this->arAtributosInt))?" LIKE ":" = ") . ((!in_array($chave, $this->arAtributosInt))?"'%".$valor."%'":"'".$valor."'") . " ";
		}

		return $sql;
	}

	/**
	 * Insert
	 * 
	 * @param array $dados
	 * @author Sávio Resende
	 * @return $this
	 */
	public function cadastrar( Array $dados ){

		// if( !$this->validaCamposObrigatorios() ) return false;

		$campos = $this->arCampos;
		unset($campos['vrlid']);
		unset($dados['vrlid']);

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}
	
}