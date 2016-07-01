<?php
	
class RepreOutSec extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.repreoutsec";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "muncod,idoutrasec" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'muncod' => null, 
									  	'idoutrasec' => null, 
									  );
									  
	public function cadastrarSecretarias ($secretarias)
	{
		for($i = 0; $i< count($secretarias); $i++)
		{
			if($secretarias[$i])
			{
				$this->idoutrasec 	= (int)$secretarias[$i];
				if($this->salvar())
				{
					$this->commit();
				}	
				else
					$this->rollback();	
			}	
		}
	}	

	public function removerSecretarias()
	{
		//$slq = "DELETE FROM pse.repreoutsec WHERE muncod = {$this->muncod}";
//		$this->excluir("'{$this->muncod}'");
		
		if($this->excluir("'{$this->muncod}'"))
			$this->commit();
		else
			$this->rollback();	
	}
}