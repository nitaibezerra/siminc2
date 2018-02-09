<?php
	
class Beneficiario extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.beneficiario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "benid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'benid' => null, 
									  	'bennome' => null, 
									  	'benstatus' => null, 
									  );
									
	public function antesSalvar(){
		
		if( $_POST['benid'] ){
			$sql = " select count(*) from emenda.beneficiario where lower(bennome) = lower('{$_POST["bennome"]}') and benid <> {$_POST['benid']} and benstatus = 'A' ";
		} else {
			$sql = " select count(*) from emenda.beneficiario where lower(bennome) = lower('{$_POST["bennome"]}') and benstatus = 'A' ";
		}
		
		if( $this->pegaUm($sql) ){
			echo "<script>
					alert('Não foi possivel inserir o registro ".$_POST["bennome"].", pois ele já está ativo na base!');
					window.location.href = 'emenda.php?modulo=principal/listaBeneficiarios&acao=A';	
				 </script>";
			
			return false;	
		}
		
		return true; 
	}
}