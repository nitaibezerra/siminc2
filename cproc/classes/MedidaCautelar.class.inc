<?php

/**
 * Classe Modelo MedidaCautelar CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo MedidaCautelar CPROC
 */
class MedidaCautelar extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.medidacautelar";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("mdcid");

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
			"mdcid" => null,
            "omcid" => null,            
			"vmcid" => null,
			"prcid" => null,
			"mdcdatainicial" => null,
			"mdcdatafinal" => null,
			"mdcobservacoes" => null,
			"mdclinkdou" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"mdcid",
			"prcid",
            "omcid",
			"vmcid",
			"mdcdatainicial",
			"mdcdatafinal",
			"mdcobservacoes",
			"mdclinkdou"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"mdcdatafinal",
			"mdcdatainicial",
			"mdclinkdou"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 * @internal datas entram aqui pq esse campo serve para evitar o parametro LIKE nos filtros para o campo data
	 */
	protected $arAtributosInt = array(
			"prcid",
			"vmcid",
			"mdcdatafinal",
			"mdcdatainicial"
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
				'<a style=\"cursor:pointer\" onclick=\"editar( ' || mc.".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluir( ' || mc.".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				CASE 
					WHEN vig.vmcvigencia = TRUE THEN 'Sim' 
					WHEN vig.vmcvigencia = FALSE THEN 'Não'
					END as vmcvigencia,
				TO_CHAR(mc.mdcdatainicial,'DD/MM/YYYY') as mcdatainicial,
				TO_CHAR(mc.mdcdatafinal,'DD/MM/YYYY') as mdcdatafinal,
				'<a href=\"'||mc.mdclinkdou||'\" target=\"_blank\">'||mc.mdclinkdou||'</a>'
			FROM " . $this->stNomeTabela . " mc
			INNER JOIN cproc.vigenciamedcautelar vig ON mc.vmcid = vig.vmcid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor){
				if( $chave == 'vmcvigencia' ){
					$sql .= " AND vmcvigencia = " . ( ($valor=='t')?'true':'false' );
				}else{
					$sql .= " AND " . $chave . ((!in_array($chave, $this->arAtributosInt))?" LIKE ":" = ") . ((!in_array($chave, $this->arAtributosInt))?"'%".$valor."%'":"'".$valor."'") . " ";
				}
			}
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

		// ver($dados,$_SESSION,d);
		$campos = $this->arCampos;
		unset($campos['mdcid']);
		unset($dados['mdcid']);
		
		// Victor Martins Machado
		// Valida se o campo data final está vazio
		// O valor "--" é colocado pela máscara
		if ($dados['mdcdatafinal'] == '--'){ 
			$dados['mdcdatafinal'] = null;
		}
		
		// dados vigencia
		$dadosVigencia = array(
			'mdcvigencia' => ( ($dados['mdcvigencia'])?'true':'false' ),
			'vmcjustificativa' => $dados['vmcjustificativa'],
			'vmcdtrevogacao' => $dados['vmcdtrevogacao'],
			'usucpf' => $_SESSION['usucpf'] );
		$dados['vmcid'] = $this->insereVigencia( $dadosVigencia );

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 *
	 */
	public function insereVigencia( $dados ){
		
		if( !$dados['vmcdtrevogacao'] || $dados['vmcdtrevogacao'] == '--' )
			$dados['vmcdtrevogacao'] = 'null';
		else
			$dados['vmcdtrevogacao'] = '"'.$dados['vmcdtrevogacao'].'"';

		$dados['vmcdatanao'] = 'NULL';

		$sql = "
			INSERT INTO cproc.vigenciamedcautelar 
				( vmcvigencia, vmcdtrevogacao, vmcjustificativa, vmcdatanao, usucpf )
			VALUES 
				( {$dados['mdcvigencia']}, {$dados['vmcdtrevogacao']}, '{$dados['vmcjustificativa']}', {$dados['vmcdatanao']}, '{$dados['usucpf']}' ) 
			RETURNING vmcid;
		";
		// ver($sql,d);
		return $this->pegaUm( $sql );
	}

	/**
	 * Atualiza
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar(){
		
		if( $this->validaCamposObrigatorios() ){

			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}


	/**
	 *
	 */
	public function atualizaVigencia( $dados ){

		if( !$dados['vmcdtrevogacao'] )
			$dados['vmcdtrevogacao'] = 'null';
		else
			$dados['vmcdtrevogacao'] = "'".$dados['vmcdtrevogacao']."'";

		$dados['vmcdatanao'] = "NOW()";

		$sql = "
			UPDATE cproc.vigenciamedcautelar 
			SET
				vmcvigencia = {$dados['vmcvigencia']}, 
				vmcdtrevogacao = {$dados['vmcdtrevogacao']},
				vmcjustificativa = '{$dados['vmcjustificativa']}', 
				vmcdatanao = {$dados['vmcdatanao']}
			WHERE vmcid = {$dados['vmcid']} ;
		";
		// ver($sql,d);
		$this->executar( $sql );

		return $this->commit();
	}
	
}