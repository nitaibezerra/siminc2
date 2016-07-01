<?php
	
class TipoProcesso extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.tipoprocesso";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tipid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tipid' => null, 
									  	'tipdescricao' => null, 
									  	'tipstatus' => null, 
									  	'sisid' => null, 
									  	'tpdid' => null, 
									  	'tipvaiparasigarp' => null, 
									  	'tprid' => null, 
									  	'tipcodigoobservacaosigef' => null, 
									  	'tiptipoempenhosigef' => null, 
									  	'tipcodigodescricaoempenhosigef' => null, 
									  	'tipnumerosistemasigef' => null, 
									  	'tipvaiparamonitoramentodeobras' => null, 
									  	'tipprogramafnde' => null, 
									  	'tiptipoprocessosigef' => null, 
									  );
}