<?php
	
class TermoCooperacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.termocooperacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tmcid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tmcid' 			=> null, 
									  	'entidproponente' 	=> null, 
									  	'entidrepprop' 		=> null, 
									  	'entidconcedente' 	=> null, 
									  	'entidrepconc' 		=> null, 
									  	'tmcdtinclusao' 	=> null, 
    									'obrid'				=> null,
    									'plicod'			=> null,
    									'tmcobs'			=> null,
    									'tmcnc'				=> null,
    									'unicod'			=> null,
    									'tmcdtnotacredito'	=> null,
    									'prtid'				=> null,
    									'pgsid'				=> null,
    									'tmcnumtermo'		=> null,
    									'dirid'				=> null,
    									'cooid'				=> null
									  );
									  
	public function excluir( $id ){
		$sql = "UPDATE {$this->stNomeTabela} SET tmcstatus = 'I' WHERE tmcid = $id ";
		if ( !$this->executar($sql)){
			return false;
		}	
	}
	
	/*public function antesExcluir( $id ){
		
		$sql = "select * from academico.termocooperacao where tmcid = {$id}";
		$arTermoCooperacao = $this->pegaLinha($sql);
		
		# exclui entidade proponente 
		if($arTermoCooperacao['entidproponente']){
			$sql = "delete from entidade.funcaoentidade where entid = {$arTermoCooperacao['entidproponente']} and funid = ". FUNCAO_ENTIDADE_PROPONENTE ;
			if ( !$this->executar($sql)){
				return false;
			}
		}
		
		# exclui entidade representante proponente
		if($arTermoCooperacao['entidrepprop']){
			if($arTermoCooperacao['entidproponente']){
				$sql = "delete from entidade.funentassoc where entid = {$arTermoCooperacao['entidproponente']} " ;
				if ( !$this->executar($sql)){
					return false;
				}
			}
			
			$sql = "delete from entidade.funcaoentidade where entid = {$arTermoCooperacao['entidrepprop']} and funid = ". FUNCAO_REP_ENTIDADE_PROPONENTE ;
			if ( !$this->executar($sql)){
				return false;
			}
		}
		
		# exclui entidade concedente		
		if($arTermoCooperacao['entidconcedente']){
			$sql = "delete from entidade.funcaoentidade where entid = {$arTermoCooperacao['entidconcedente']} and funid = ". FUNCAO_ENTIDADE_CONCEDENTE ;
			if ( !$this->executar($sql)){
				return false;
			}
		}
		
		# exclui entidade representante concedente
		if($arTermoCooperacao['entidrepconc']){
			if($arTermoCooperacao['entidconcedente']){
				$sql = "delete from entidade.funentassoc where entid = {$arTermoCooperacao['entidconcedente']} " ;
				if ( !$this->executar($sql)){
					return false;
				}
			}
			$sql = "delete from entidade.funcaoentidade where entid = {$arTermoCooperacao['entidrepconc']} and funid = ". FUNCAO_REP_ENTIDADE_CONCEDENTE ;
			if ( !$this->executar($sql)){
				return false;
			}
		}
		
		# exclui a tabela termo orçamento
		$sql = "delete from academico.termoorcamento where tmcid = {$id} ";
		if ( !$this->executar($sql)){
			return false;
		}
		
		return true;
	}*/
									  
}