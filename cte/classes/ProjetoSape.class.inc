<?php
//include_once(APPRAIZ .'cte/classes/RegrasFinanceiro.class.inc');
class ProjetoSape extends modelo{
	
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
									  
	/**
	 * function carregaListaDeConvenio($ano, $instrumentoUnidade);
	 * @desc   : Recupera a lista de convênios. 
	 * @author : Thiago Tasca Barbosa
	 * @param  : numeric $instrumentoUnidade (Instrumento unidade)
	 * @param  : numeric $ano (ano de referencia)
	 * @return : array $convenios (array contendo os convênios)
	 * @since 17/03/2009
	 */
	public function carregaListaDeConvenio($ano, $instrumentoUnidade){
		global $db;
		$sql = "SELECT p.prsid, p.prsano, p.prsnumconvsape
				FROM cte.projetosape p
				WHERE p.prsano =".$ano." 
				AND p.inuid = ".$instrumentoUnidade.
				" AND  prstipo = 'original';";
	
		$convenios 	= $db->carregar($sql);
		return $convenios;
	}

}