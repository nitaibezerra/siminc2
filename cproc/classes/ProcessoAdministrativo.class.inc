<?php

/**
 * Classe Modelo ProcessoAdministrativo CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo ProcessoAdministrativo CPROC
 */
class ProcessoAdministrativo extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.processoadmin";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("padid");

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
			"padid" 	     => null,
			"prcid" 	     => null,
			"padinstaurado"  => null,
			"padlinkdou"      => null,
			"padobservacoes" => null,
			"paddtinstauracao" => null,
			"paddtpublicacao" => null,
			"paddtrevogacao" => null,
			"paddtsuspensao" => null,
			"paddtarquivamento" => null

		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"padid",
			"prcid",
			"padinstaurado",
			"padlinkdou",
			"padobservacoes",
			"paddtinstauracao",
			"paddtpublicacao",
			"paddtrevogacao",
			"paddtsuspensao",
			"paddtarquivamento"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"padinstaurado"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"padid",
			"prcid",
			"padinstaurado"
		);

	/**
	 * Array de lista de assessoramentos
	 * 
	 * @var array
	 */
	public $lista;

	/**
	 * Id do último assessorado inserido
	 * 
	 * @var integer
	 */
	public $inserido;

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
				'<a href=\"'||padlinkdou||'\" target=\"_blank\">'||padlinkdou||'</a>',
				to_char(paddtinstauracao, 'DD/MM/YYYY') as paddtinstauracao,
				to_char(paddtpublicacao, 'DD/MM/YYYY') as paddtpublicacao,
				to_char(paddtrevogacao, 'DD/MM/YYYY') as paddtrevogacao,
				to_char(paddtsuspensao, 'DD/MM/YYYY') as paddtsuspensao,
				to_char(paddtarquivamento, 'DD/MM/YYYY') as paddtarquivamento,
				padobservacoes
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((!in_array($chave, $this->arAtributosInt))?" LIKE ":" = ") . ((!in_array($chave, $this->arAtributosInt))?"'%".$valor."%'":"'".$valor."'") . " ";
		}
		//ver($sql,d);

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
		unset($campos['padid']);
		unset($dados['padid']);

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 * Atualiza
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar(){
		
		if( $this->validaCamposObrigatorios() ){

			// trata campo booleano
			if( $this->padinstaurado === '0' ) $this->padinstaurado = false;

			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

}