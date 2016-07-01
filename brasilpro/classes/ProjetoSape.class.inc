<?php
include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class ProjetoSape extends Regrasfinanceiro{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.projetosape";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prsid' => null, 
									  	'prsnumeroprocesso' => null, 
									  	'prstipo' => null, 
									  	'prsnumconvsape' => null, 
									  	'prsdata' => null, 
									  	'prsano' => null, 
									  	'prsvalorconvenio' => null, 
									  	'prsvalorcontrapartida' => null, 
									  	'prsiniciovigencia' => null, 
									  	'prsfimvigencia' => null, 
									  	'prsdatadeposito' => null, 
									  	'prsnumagencia' => null, 
									  	'prsnumconta' => null, 
									  	'inuid' => null, 
									  	'usucpf' => null
									  );
									  
	public function carregaListaDeConvenio($ano, $instrumentoUnidade){
		global $db;
		$sql = "SELECT p.prsid, p.prsano, p.prsnumconvsape
				FROM cte.projetosape p
				WHERE p.prsano =".$ano." 
				AND p.inuid = ".$instrumentoUnidade;
	
		$convenios 	= $db->carregar($sql);
		return $convenios;
	}
}