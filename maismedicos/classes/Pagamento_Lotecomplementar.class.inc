<?php
	
class Pagamento_lotecomplementar extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.pagamento_lotecomplementar";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'locid' => null, 
									  	'loccpfresponsavel' => null, 
									  	'loccpfalteracao' => null, 
									  	'locmespagamento' => null, 
									  	'locanopagamento' => null, 
									  	'locobservacoes' => null, 
									  	'locdataregistro' => null, 
									  	'locdataalteracao' => null, 
									  	'locstatus' => null, 
									  	'locdatafechamento' => null, 
									  );
}