<?php

/**
 * Classe Modelo MedidaSaneadora CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo MedidaSaneadora CPROC
 */
class MedidaSaneadora extends ModeloCproc{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "cproc.medidasaneadora";

	/**
	 * Chave primaria
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("msaid");

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
			"msaid" => null,
			"prcid" => null,
			"vmsid" => null,
			"msadatainicial" => null,
			"msadatalimite" => null,
			"msadatafinal" => null,
			"msaobservacoes" => null
		);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
			"msaid",
			"prcid",
			"vmsid",
			"msadatainicial",
			"msadatalimite",
			"msadatafinal",
			"msaobservacoes"
		);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
			"vmsid",
			"msadatainicial",
			"msadatalimite"
		);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
			"prcid",
			"vmsid",
			"msadatainicial",
			"msadatalimite",
			"msadatafinal"
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
				'<a style=\"cursor:pointer\" onclick=\"editar( ' || ms.".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluir( ' || ms.".$this->arChavePrimaria[0]." || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				CASE 
					WHEN vms.vmsvigencia = TRUE THEN 'Sim' 
					WHEN vms.vmsvigencia = FALSE THEN 'Não'
					END as vmsvigencia,
				TO_CHAR(ms.msadatainicial,'DD/MM/YYYY') as msadatainicial,
				TO_CHAR(ms.msadatalimite, 'DD/MM/YYYY') as msadatalimite,
				TO_CHAR(ms.msadatafinal,'DD/MM/YYYY') as msadatafinal
			FROM " . $this->stNomeTabela . " ms
			INNER JOIN cproc.vigenciamedsaneadora vms ON vms.vmsid = ms.vmsid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor){
				if( $chave == 'msavigencia' ){
					$sql .= " AND vmsvigencia = " . ( ($valor=='t')?'true':'false' );
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
		unset($campos['msaid']);
		unset($dados['msaid']);
		
		// Victor Martins Machado
		// Valida se o campo de data final da vigência foi informado
		// O valor '--' é gerado pela máscara, quando o campo está vazio
		if ($dados['msadatafinal'] == '--') {
			$dados['msadatafinal'] = null;
		}
		
		// dados vigencia
		$dadosVigencia = array(
			'msavigencia' => ( ($dados['msavigencia'])?'true':'false' ),
			'vmsjustificativa' => $dados['vmcjustificativa'],
			'vmsdtrevogacao' => $dados['vmcdtrevogacao'],
			'usucpf' => $_SESSION['usucpf'] );
		$dados['vmsid'] = $this->insereVigencia( $dadosVigencia );

		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 *
	 */
	public function insereVigencia( $dados ){
		
		if( !$dados['vmsdtrevogacao'] || $dados['vmsdtrevogacao'] = '--' )
			$dados['vmsdtrevogacao'] = 'null';
		else
			$dados['vmsdtrevogacao'] = '"'.$dados['vmsdtrevogacao'].'"';

		$dados['vmcdatanao'] = 'NULL';

		$sql = "
			INSERT INTO cproc.vigenciamedsaneadora 
				( vmsvigencia, vmsdtrevogacao, vmsjustificativa, vmcdatanao, usucpf )
			VALUES 
				( {$dados['msavigencia']}, {$dados['vmsdtrevogacao']}, '{$dados['vmsjustificativa']}', {$dados['vmcdatanao']}, '{$dados['usucpf']}' ) 
			RETURNING vmsid;
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

		if( !$dados['vmsdtrevogacao'] )
			$dados['vmsdtrevogacao'] = 'null';
		else
			$dados['vmsdtrevogacao'] = "'".$dados['vmsdtrevogacao']."'";

		$dados['vmcdatanao'] = "NOW()";

		$sql = "
			UPDATE cproc.vigenciamedsaneadora 
			SET
				vmsvigencia = {$dados['vmsvigencia']}, 
				vmsdtrevogacao = {$dados['vmsdtrevogacao']},
				vmsjustificativa = '{$dados['vmsjustificativa']}', 
				vmcdatanao = {$dados['vmcdatanao']}
			WHERE vmsid = {$dados['vmsid']} ;
		";
		// ver($sql,d);
		$this->executar( $sql );

		return $this->commit();
	}
	
}