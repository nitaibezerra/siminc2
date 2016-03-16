<?php

/**
 * Classe Modelo PenalidadeAplicada CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo PenalidadeAplicada CPROC
 */
class PenalidadeAplicada extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.penalidadeaplicada";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("papid");

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
			"papid" => null,
			"prcid" => null,
            "opaid" => null,
			"vpaid" => null,
			"papdataaplicacao" => null,
			"papobservacoes" => null,
			"paplinkdou" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"papid",
			"prcid",
            "opaid",
			"vpaid",
			"papdataaplicacao",
			"papobservacoes",
			"paplinkdou"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"vpaid",
			"papdataaplicacao"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid",
			"vpaid",
			"papdataaplicacao"
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
					WHEN vpa.vpavigencia = TRUE THEN 'Sim' 
					WHEN vpa.vpavigencia = FALSE THEN 'Não'
					END as vpavigencia,
				TO_CHAR(papdataaplicacao,'DD/MM/YYYY') as papdataaplicacao,
				'<a href=\"'||pa.paplinkdou||'\" target=\"_blank\">'||pa.paplinkdou||'</a>'
			FROM " . $this->stNomeTabela . " pa
			INNER JOIN cproc.vigenciapenaplicada vpa ON vpa.vpaid = pa.vpaid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor){
				if( $chave == 'vpavigencia' ){
					$sql .= " AND vpavigencia = " . ( ($valor=='t')?'true':'false' );
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
		unset($campos['papid']);
		unset($dados['papid']);

		// dados vigencia
		$dadosVigencia = array(
			'vpavigencia' => ( ($dados['vpavigencia'])?'true':'false' ),
			'vpajustificativa' => $dados['vpajustificativa'],
			'vpadtrevogacao' => $dados['vpadtrevogacao'],
			'vpadatanao' => $dados['vpadatanao'],
			'usucpf' => $_SESSION['usucpf'] );
		$dados['vpaid'] = $this->insereVigencia( $dadosVigencia );

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 *
	 */
	public function insereVigencia( $dados ){
		
		if( !$dados['vpadtrevogacao'] || $dados['vpadtrevogacao'] = '--' )
			$dados['vpadtrevogacao'] = 'null';
		else
			$dados['vpadtrevogacao'] = '"'.$dados['vpadtrevogacao'].'"';

		$dados['vpadatanao'] = 'NULL';

		$sql = "
			INSERT INTO cproc.vigenciapenaplicada 
				( vpavigencia, vpadtrevogacao, vpajustificativa, vpadatanao, usucpf )
			VALUES 
				( {$dados['vpavigencia']}, {$dados['vpadtrevogacao']}, '{$dados['vpajustificativa']}', {$dados['vpadatanao']}, '{$dados['usucpf']}' ) 
			RETURNING vpaid;
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
ver($dados, d);
		if( !$dados['vpadtrevogacao'] )
			$dados['vpadtrevogacao'] = 'null';
		else
			$dados['vpadtrevogacao'] = "'".$dados['vpadtrevogacao']."'";

		$dados['vpadatanao'] = "NOW()";
		
		$vpavigencia = ( ($dados['vpavigencia'])?'true':'false' );

		$sql = "
			UPDATE cproc.vigenciapenaplicada 
			SET
				vpavigencia = {$vpavigencia}, 
				vpadtrevogacao = {$dados['vpadtrevogacao']},
				vpajustificativa = '{$dados['vpajustificativa']}', 
				vpadatanao = {$dados['vpadatanao']}
			WHERE vpaid = {$dados['vpaid']} ;
		";
		// ver($sql,d);
		$this->executar( $sql );

		return $this->commit();
	}
	
}