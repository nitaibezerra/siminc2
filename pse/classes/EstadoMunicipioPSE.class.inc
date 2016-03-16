<?php
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );

class EstadoMunicipioPSE extends Modelo{
	
	public function __construct($empid,$muncod = null, $nmMunEst = null ){
	
		parent::__construct();
		
		if( !$empid && $muncod && $nmMunEst ){
			$empid = $this->pegaUm( "SELECT empid from pse.estadomunicipiopse WHERE muncod = '{$muncod}' AND empflagestmun = '{$nmMunEst}' and empano = ".$_SESSION['exercicio'] );
		}
		
		if( !$empid && $muncod && !$nmMunEst ){
			$empid = $this->pegaUm( "SELECT empid from pse.estadomunicipiopse WHERE muncod = '{$muncod}' and empano = ".$_SESSION['exercicio'] );
		}
		
		if( $empid ){
			
			$this->carregarPorId( $empid );
			
			$sql = "SELECT semid FROM pse.secretariaestmun WHERE empid = {$empid}";
			
			$rs = $this->carregar($sql);
				
				$matriz = array();
				if($rs)
				{
					foreach($rs as $value)
					{
						$this->secretariaEstMun[] = new SecretariaEstMun($value["semid"]);
					}
					
				}
		}
	
		//$this->carregarPorId( "'".$muncod."'" );
		
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.estadomunicipiopse";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "empid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    public $arAtributos     = array(
									  	'empid' => null, 
									  	'muncod' => null, 
									  	'empparticipapse2009' => null, 
									  	'empquantescolapse2009' => null,
    									'empflagestmun' => null,
    									'empano' => null
									  );
									  
	protected $secretariaEstMun = array();								  
									  
	public function verificaCapital ()
	{
		$capital = '';
		
		if($this->muncod)
			$capital = $this->muncod;
		
		$sql = "SELECT muncodcapital FROM territorios.estado WHERE muncodcapital = '{$this->muncod}'";
		
		$rs = $this->carregar($sql);
		
		if($rs)
			return true;
		else
			return false;
	} 	

	public function verEmp($empid){
		if ($empid == ''){
			alert("Você não pode acessar essa área até fazer o cadastro em Identificação.");
			echo "<script>window.location.href='pse.php?modulo=principal/identificacaoEstadoMunicipio&acao=A';</script>";
			exit;
		}
	}
	
	public function retornaEMPID($muncod)
	{
		$empid = $this->pegaUm( "SELECT empid from pse.estadomunicipiopse WHERE muncod = '{$muncod}' and empano = ".$_SESSION['exercicio'] );
		return $empid;
	}	
}