<?php
	
class TecnicoMonitoraEscola extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "escolaativa.tecnicomonitoraescola";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "espid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tecid' => null, 
									  	'espid' => null, 
									  );
									  
	protected $tabelaAssociativa = true;
	
	/*public function excluiMonitoriaEscolaPorTecid($tecid, $muncod){
		if($tecid && $muncod){
			$sql = "delete from 
						{$this->stNomeTabela} 
					where espid in (
									select ep.espid from cte.escolaativa ea
										inner join cte.instrumentounidade inu on inu.inuid = ea.inuid 
										inner join cte.escolaativaescolas eae on ea.esaid = eae.esaid
										inner join entidade.entidade e on eae.entid = e.entid
										inner join escolaativa.escolaparticipante ep on eae.eaeid = ep.eaeid
										inner join escolaativa.tecnicomonitoraescola tme on ep.espid = tme.espid
									where inu.muncod = '{$muncod}' AND tme.tecid = {$tecid}
									)";
			if ( !$this->executar($sql)){
				return false;
			}
		}
	}*/
	
	public function excluiMonitoriaEscolaPorTecid($tecid){
		if($tecid){
			$sql = "delete from {$this->stNomeTabela} where tecid = {$tecid} ";
			if ( !$this->executar($sql)){
				return false;
			}
		}
	}
	
	public function excluiMonitoriaEscolaPorEspid($espid, $tecid){
		if($tecid && $espid){
			$sql = " delete from {$this->stNomeTabela} where espid = $espid and tecid = $tecid ";
			if ( !$this->executar($sql)){
				return false;
			}
		}
	}
	
}