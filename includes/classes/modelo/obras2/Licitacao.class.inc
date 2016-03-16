<?php
	
class Licitacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.licitacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "licid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'licid' => null, 
									  	'molid' => null, 
									  	'orgid' => null, 
									  	'licnumero' => null, 
									  	'licstatus' => null, 
									  );

	public function depoisSalvar(){
		if ( isset($_POST['flcid']) ){
			$arFlcid = (array) $_POST['flcid'];
			
			$faseLicitacao = new FaseLicitacao();
			foreach ($arFlcid as $flcid){
				$faseLicitacao->carregarPorId( $flcid );;	
				$faseLicitacao->licid = $this->licid;	
				$faseLicitacao->salvar();
				$faseLicitacao->clearDados();
			}
		}
		
		if ( isset($_POST['obrid']) ){
			$arObrid       = (array) $_POST['obrid'];
			$obraLicitacao = new ObraLicitacao();
			$obraLicitacao->apagaPorLicitacao( $this->licid );
			
			foreach ( $arObrid as $obrid ){
                            if(!empty($obrid)){
				$arDados = array('licid' => $this->licid,
                                                 'obrid' => $obrid);
				$obraLicitacao->popularDadosObjeto( $arDados )
							  ->salvar();
				$obraLicitacao->clearDados();
                            }
			}		
		}
		return true;
	}

	public function listaSql( Array $filtro = array() ){
		$arWhere = array();
		if ( $filtro['orgid'] ){
			$arWhere[] = "l.orgid = {$filtro['orgid']}";
		}
		
		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarLic(\'' || l.licid || '\');\"
	 					title=\"Alterar Licitação\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirLic(\'' || l.licid || '\');\"
	 					title=\"Excluir Licitação\">
				  </center>'";
		
		$sql = "SELECT
					{$acao} AS acao,
					l.licnumero,
					COALESCE(m.moldsc, '') AS moldsc
				FROM
					obras2.licitacao l
				LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
				WHERE
					licstatus = 'A'" .
					(count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "");
					
		return $sql;
	}
	
	public function listaCombo( Array $filtro = array() ){
		$arJoin  = array();
		$arWhere = array();
		
		if ( $filtro['orgid'] ){
			$arWhere[] = "l.orgid = {$filtro['orgid']}";
		}
		
		if ( $filtro['obrid'] ){
			$arJoin[] = "JOIN obras2.obralicitacao ol ON ol.licid = l.licid AND
														 ol.obrid = {$filtro['obrid']} AND
														 ol.oblstatus = 'A'";
		}
		
		$sql = "SELECT
					l.licid AS codigo,
					COALESCE(l.licnumero, '') || ' - ' || COALESCE(m.moldsc, '') AS descricao
				FROM
					obras2.licitacao l
				LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
				" . (count($arJoin) ? implode(" ", $arJoin) : "") . "	
				WHERE
					licstatus = 'A'" .
					(count($arWhere) ? " AND " . implode(" AND ", $arWhere) : "");
					
		$dados = $this->carregar($sql);
		return (is_array($dados) ? $dados : array());
	}
	
	public function pegaDescricaoPorLicid( $licid ){
		$sql = "SELECT
					l.licnumero || ' - ' || COALESCE(m.moldsc, '')
				FROM
					obras2.licitacao l
				LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
				WHERE
					licstatus = 'A' AND
					licid = {$licid}";
					
		return $this->pegaUm( $sql );
	}

	public function pegaDadosPorObra( $obrid ){
		$sql = "SELECT
					l.licid,
					l.licnumero,
					COALESCE(m.moldsc, '') AS moldsc
				FROM
					obras2.licitacao l
				LEFT JOIN obras2.modalidadelicitacao m ON m.molid = l.molid
				JOIN obras2.obralicitacao ol ON ol.licid = l.licid AND
												ol.oblstatus = 'A' AND
												ol.obrid = {$obrid}
				WHERE
					l.licstatus = 'A'";
		
		$dados = $this->pegaLinha( $sql );
		return ($dados ? $dados : array());	
	}
}