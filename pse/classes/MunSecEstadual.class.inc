<?php
	
class MunSecEstadual extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.munsecestadual";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "muncod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'muncod' => null, 
									  	'nomesecestsaude' => null, 
									  	'endsecestsaude' => null, 
									  	'nomerepsecestsaude' => null, 
									  	'emailrepsecestsaude' => null, 
									  	'telrepsecestsaude' => null, 
									  	'carfuncrepsecestsaude' => null, 
									  	'nomesecestedu' => null, 
									  	'endsecestedu' => null, 
									  	'nomerepsecestedu' => null, 
									  	'emailrepsecestedu' => null, 
									  	'telrepsecestedu' => null, 
									  	'carfuncrepsecestedu' => null, 
									  );
	public function __construct($muncod = null ){
		
		parent::__construct();
	
		$this->carregarPorId( "'".$muncod."'" );
		
	}	

	public function insereAspas()
	{
		$this->muncod = "'" .$this->muncod."'";
	}
									  
	public function inserirCadastro()
	{
		
		if($this->consultarMunicipio($this->muncod))
		{
			$sql  = $this->editarMunicipio();
		}
		else
		{
			$sql = $this->salvarMunicipio();
		}	
		$sql = iconv( "UTF-8", "ISO-8859-1", $sql );
		
		if($this->executar($sql))
			return true;
		else
			return false;	
			
	}
	
	public function consultarMunicipio($muncod)
	{
		$sql = "SELECT muncod FROM pse.munsecestadual WHERE muncod = {$muncod}";
		$rs = $this->pegaUm($sql);
		
		if($rs)			
			return true;
		else
			return false;
	}
	
	public function editarMunicipio()
	{	
		 $sql = "UPDATE 
					pse.munsecestadual
				SET 
					muncod = {$this->muncod}, 
					nomesecestsaude = '{$this->nomesecestsaude}',
					endsecestsaude = '".($this->endsecestsaude)."', 
					nomerepsecestsaude = '{$this->nomerepsecestsaude}', 
					emailrepsecestsaude = '{$this->emailrepsecestsaude }', 
			     	telrepsecestsaude = '{$this->telrepsecestsaude}', 
			      	carfuncrepsecestsaude = '{$this->carfuncrepsecestsaude}', 
			        nomesecestedu = '{$this->nomesecestedu}', 
			        endsecestedu = '{$this->endsecestedu}', 
			        nomerepsecestedu = '{$this->nomerepsecestedu}', 
			        emailrepsecestedu = '{$this->emailrepsecestedu}', 
			        telrepsecestedu = '{$this->telrepsecestedu}', 
			        carfuncrepsecestedu = '{$this->carfuncrepsecestedu}'
		        WHERE 
					muncod = {$this->muncod}";
		return $sql;		
	}
	
	public function salvarMunicipio()
	{
		return $sql = "INSERT INTO 
					pse.munsecestadual
				(
				   muncod,nomesecestsaude, endsecestsaude, nomerepsecestsaude, emailrepsecestsaude, 
			       telrepsecestsaude, carfuncrepsecestsaude, nomesecestedu,endsecestedu,nomerepsecestedu, emailrepsecestedu, 
			       telrepsecestedu, carfuncrepsecestedu 
			      )	
			     VALUES
			     (
			     	{$this->muncod},'{$this->nomesecestsaude}','{$this->endsecestsaude}','{$this->nomerepsecestsaude}','{$this->emailrepsecestsaude}',
				  	'{$this->telrepsecestsaude}', '{$this->carfuncrepsecestsaude}','{$this->nomesecestedu}','{$this->endsecestedu}','{$this->nomerepsecestedu}',
				  	'{$this->emailrepsecestedu}','{$this->telrepsecestedu}','{$this->carfuncrepsecestedu}')";
	}								  
}