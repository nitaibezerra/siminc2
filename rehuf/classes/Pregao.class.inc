<?php
class Pregao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.pregao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "preid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'preid' => null,
    									'preobjeto' => null, 
    									'predatainicialpregao' => null, 
    									'predatafinalpregao' => null, 
    									'predatainicialpreenchimento' => null, 
    									'predatafinalpreenchimento' => null, 
    									'prestatus' => null, 
    									'precodigo' => null, 
    									'preins' => null, 
									  );
									  
	
    public function filtraPregao()
    {
    	include_once APPRAIZ . 'includes/funcoes.inc';
    	
    	$ano = $_POST['ano'];
    
    	if(!$ano){
    		$sql = "select preobjeto as descricao,preid as codigo from rehuf.pregao where prestatus = 'A'";
    		campo_popup('preid',$sql,'Selecione..','','400x400',80);
    		return false;
    	}
    	
    	$sql = "select preobjeto as descricao,preid as codigo from rehuf.pregao where EXTRACT(YEAR FROM predatainicialpreenchimento) = $ano and prestatus = 'A'";
    
    echo campo_popup('preid',$sql,'Selecione..','','400x400',80);
    }
}