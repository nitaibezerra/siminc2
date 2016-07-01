<?php
include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class HistoricoMonitoramentoConvenio extends Regrasfinanceiro{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.historicomonitoramentoconvenio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hmcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hmcid' => null, 
									  	'prsid' => null, 
									  	'hmcdatamonitoramento' => null, 
									  	'hmcstatus' => null, 
    									'hmcdatareferenciamonitoramento' => null,
    									'hmdid' => null
									  );
									  
	/**
	 * function recuperaDadosDoMonitoramento($idConvenio);
	 * @desc   : Carrega dados de monitoramento por convênio. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $idConvenio (id do convênio)
	 * @return : array $dadosConvenios
	 * @since 01/04/2009
	 */
	public function recuperaDadosDoMonitoramento($idConvenio, $mes = NULL){
		global $db;
		if($mes != 0){
			$buscaComMes = " AND to_char(hmcdatamonitoramento, 'MM') = '".$mes."'";
		}else{
			$buscaComMes = '';
		}
		
		$sql = "SELECT hmcid, prsid, hmcstatus
				FROM cte.historicomonitoramentoconvenio
				WHERE prsid = ".$idConvenio.$buscaComMes;
		
		$dadosConvenios 	= $db->carregar($sql);
		return $dadosConvenios;
	}
}