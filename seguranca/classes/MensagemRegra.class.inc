<?php
	
class MensagemRegra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.mensagemregra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mreid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mreid' => null, 
									  	'sisid' => null, 
									  	'mredescricao' => null,
									  	'mretextoemail' => null,
									  	'mretextocelular' => null,
									  	'mreenviaemail' => null, 
									  	'mreenviasms' => null, 
									  	'mretipo' => null, 
									  	'mrestatus' => null,
									  	'mretituloemail' => null,
									  	'mredatacriacao' => null,
									  );
}