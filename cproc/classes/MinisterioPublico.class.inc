<?php

/**
 * Classe Modelo MinisterioPublico CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo MinisterioPublico CPROC
 */
class MinisterioPublico extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.ministeriopublico";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("mpuid");

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
			"mpuid" => null,
			"prcid" => null,
			"pmpid" => null,
			"mpuindicarsidoc" => null,
			"mpuobservacoes" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"mpuid",
			"prcid",
			"pmpid",
			"mpuindicarsidoc",
			"mpuobservacoes",
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"mpuindicarsidoc",
			"mpudataaplicacao"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid",
			"pmpid",
			"mpudataaplicacao"
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
				CASE 
					WHEN pmp.pmpvigencia = TRUE THEN 'Sim' 
					WHEN pmp.pmpvigencia = FALSE THEN 'Não'
					END as pmpvigencia,
				mpuindicarsidoc
			FROM " . $this->stNomeTabela . " mp
			INNER JOIN cproc.pendenciaministpublico pmp ON pmp.pmpid = mp.pmpid ";
		$sql .= " WHERE mp.prcid = ".$_SESSION['cproc']['prcid']." ";
		if( $filtros != false ){
			foreach ($filtros as $chave => $valor){
				if( $chave == 'pmpvigencia' ){
					$sql .= " AND pmpvigencia = " . ( ($valor=='t')?'true':'false' );
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

		$campos = $this->arCampos;
		unset($campos['mpuid']);
		unset($dados['mpuid']);

		// dados vigencia
		$dadosVigencia = array(
			'pmpvigencia' => ( ($dados['pmpvigencia'])?'true':'false' ),
			'pmpjustificativa' => $dados['pmpjustificativa'],
			'pmpdtrevogacao' => $dados['pmpdtrevogacao'],
			'usucpf' => $_SESSION['usucpf'] );
		$dados['pmpid'] = $this->insereVigencia( $dadosVigencia );

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 *
	 */
	public function insereVigencia( $dados ){
		
		if( !$dados['pmpdtrevogacao'] || $dados['pmpdtrevogacao'] = '--' )
			$dados['pmpdtrevogacao'] = 'null';
		else
			$dados['pmpdtrevogacao'] = '"'.$dados['pmpdtrevogacao'].'"';

		$dados['pmpdatanao'] = 'NULL';

		$sql = "
			INSERT INTO cproc.pendenciaministpublico 
				( pmpvigencia, pmpdtrevogacao, pmpjustificativa, pmpdatanao, usucpf )
			VALUES 
				( {$dados['pmpvigencia']}, {$dados['pmpdtrevogacao']}, '{$dados['pmpjustificativa']}', {$dados['pmpdatanao']}, '{$dados['usucpf']}' ) 
			RETURNING pmpid;
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

		if( !$dados['pmpdtrevogacao'] )
			$dados['pmpdtrevogacao'] = 'null';
		else
			$dados['pmpdtrevogacao'] = "'".$dados['pmpdtrevogacao']."'";

		$dados['pmpdatanao'] = "NOW()";

		$sql = "
			UPDATE cproc.pendenciaministpublico 
			SET
				pmpvigencia = {$dados['pmpvigencia']}, 
				pmpdtrevogacao = {$dados['pmpdtrevogacao']},
				pmpjustificativa = '{$dados['pmpjustificativa']}', 
				pmpdatanao = {$dados['pmpdatanao']}
			WHERE pmpid = {$dados['pmpid']} ;
		";
		// ver($sql,d);
		$this->executar( $sql );

		return $this->commit();
	}
	
}