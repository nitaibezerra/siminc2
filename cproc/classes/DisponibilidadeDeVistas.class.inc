<?php

/**
 * Classe Modelo DisponibilidadeVistas CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo DisponibilidadeVistas CPROC
 */
class DisponibilidadeDeVistas extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.disponibilidadevistas";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("dpvid");

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
			"dpvid" => null,
			"prcid" => null,
			"arqid" => null,
			"apvprocadmininstaurado" => null,
			"dpvprodisponivelvistas" => null,
			"dpvservidorresponsavel" => null,
			"dpvnomesolicitante"     => null,
			"dpvemailsolicitante" => null,
			"dpvtelefonesolicitante" => null,
			"dpvcpfsolicitante" => null,
			"dpvdatavista" => null
			//"dpvdataprevdevolucao" => null,
			//"dpvdatarealdevolucao" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"dpvid",
			"prcid",
			"arqid",
			"apvprocadmininstaurado",
			"dpvprodisponivelvistas",
			"dpvservidorresponsavel",
			"dpvnomesolicitante",
			"dpvemailsolicitante",
			"dpvtelefonesolicitante",
			"dpvcpfsolicitante",
			"dpvdatavista"
			//"dpvdataprevdevolucao",
			//"dpvdatarealdevolucao"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"dpvservidorresponsavel",
			"dpvnomesolicitante",
			"dpvemailsolicitante",
			"dpvtelefonesolicitante",
			"dpvcpfsolicitante",
			"dpvdatavista"
			//"dpvdataprevdevolucao",
			//"dpvdatarealdevolucao"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid",
			"dpvprocadmininstaurado",
			"dpvprodisponivelvistas"
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
				<a style=\"cursor:pointer\" onclick=\"excluir( ' || ".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/excluir.gif\"/></a>
				<a style=\"cursor:pointer\" title=\"Visualizar documentos anexados\" onclick=\"verDocumentosAnexados( ' || ".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/clipe.gif\"/></a>' as acao,
				dpvservidorresponsavel,
				dpvemailsolicitante,
				dpvtelefonesolicitante,
				TO_CHAR(dpvdatavista, 'DD/MM/YYYY')
				--dpvdataprevdevolucao,
				--dpvdatarealdevolucao
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
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
		unset($campos['dpvid']);
		unset($dados['dpvid']);
		
		$dados['dpvcpfsolicitante'] = str_replace('-','',str_replace('.','',$dados['dpvcpfsolicitante']));
		
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
			if( $this->dpvprocadmininstaurado )
			if( $this->dpvprocadmininstaurado === '0' ) $this->dpvprocadmininstaurado = false;
			if( $this->dpvprodisponivelvistas )
			if( $this->dpvprodisponivelvistas === '0' ) $this->dpvprodisponivelvistas = false;

			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

}