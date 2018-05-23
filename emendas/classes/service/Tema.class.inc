<?php
	
class Tema extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.tema";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "temid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'temid' => null, 
									  	'temnome' => null, 
									  	'temstatus' => null, 
									  );
									  
	public function antesSalvar(){
		
		$sql = " select count(*) from emenda.tema where temnome = '{$_POST["temnome"]}' and temstatus = 'A' ";		
		
		if( $this->pegaUm($sql) ){
			echo "<script>
					alert('Não foi possivel inserir o registro ".$_POST["temnome"].", pois ele já está ativo na base!');
					window.location.href = 'emenda.php?modulo=principal/listaTema&acao=A';	
				 </script>";
			
			return false;	
		}
		
		
		return true; 
	}
}