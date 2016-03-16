<?php
	
class Cadestadomunicipio extends Modelo{
	
	public function __construct($muncod = null ){
		
		parent::__construct();
	
		$this->carregarPorId( "'".$muncod."'" );
		
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
    protected $arChavePrimaria = array( "muncod" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array( 
									  	'muncod' => null, 
									  	'stmunpse2009' => null, 
									  	'nomesecmunsaude' => null, 
									  	'endsecmunsaude' => null, 
									  	'nomerepsecmunsaude' => null, 
									  	'emailrepsecmunsaude' => null, 
									  	'telrepsecmunsaude' => null, 
									  	'carfuncrepsecmunsaude' => null, 
									  	'nomesecmunedu' => null, 
									  	'endsecmunedu' => null, 
									  	'nomerepsecmunedu' => null, 
									  	'emailrepsecmunedu' => null, 
									  	'telrepsecmunedu' => null, 
									  	'carfuncrepsecmunedu' => null, 
									  	'repoutrasec' => null, 
									  	'quantpse2009' => null, 
									  	'ggeggmconst' => null, 
									  	'adesaoolharbrasil' => null, 
									  	'manoritriacuidvis' => null, 
									  	'usousitsaude' => null, 
									  	'usouinfcenso' => null, 
									  	'usouprogacaoativ' => null, 
									  	'usouparcintersetor' => null, 
									  	'freqcomunica' => null, 
									  	'difcomunicainterlocutor' => null, 
									  	'acompcomp01' => null, 
									  	'acompcomp02' => null, 
									  	'quantavalclipsi01' => null, 
									  	'quantavalclipsi02' => null, 
									  	'quantavalclipsi03' => null, 
									  	'quantavalclipsi04' => null, 
									  	'quantavalclipsi05' => null, 
									  	'quantavalnutri01' => null, 
									  	'quantavalnutri02' => null, 
									  	'quantavalnutri03' => null, 
									  	'quantavalnutri04' => null, 
									  	'quantavalsaudebucal01' => null, 
									  	'quantavalsaudebucal02' => null, 
									  	'quantavalsaudebucal03' => null, 
									  	'quantavalsaudebucal04' => null, 
									  	'quantpromsaudeprev01' => null, 
									  	'quantpromsaudeprev02' => null, 
									  	'quantpromsaudeprev03' => null, 
									  	'quantpromsaudeprev04' => null, 
									  	'quantpromsaudeprev05' => null, 
									  	'quantedupermcaprof01' => null, 
									  	'quantedupermcaprof02' => null, 
									  	'quantedupermcaprof03' => null, 
									  	'quantestmovaval01' => null, 
									  );
									  
	public function verificaCapital()
	{
		global $db;
		
		$capital = '';
		
		if($this->muncod)
			$capital = $this->muncod;
		
		$sql = "SELECT muncodcapital FROM territorios.estado WHERE muncodcapital = '{$this->muncod}'";
		
		$rs = $db->carregar($sql);
		
		if($rs)
			return true;
		else
			return false;
	}	

	public function insereAspas()
	{
		$this->muncod = "'" .$this->muncod."'";
	}
	
	/*public function inserir()
	{
		ver("edson",d);
		$arCampos  = array();
		$arValores = array();
		$arSimbolos = array();
		
		foreach( $this->arAtributos as $campo => $valor ){
			
			//if( $campo == $this->arChavePrimaria[0] ) continue;
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$arValores[] = trim($valor);
			}
		}

		if( count( $arValores ) ){
			
			$sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." ) 
											  values ( '". implode( "', '", $arValores ) ."' ) ";
	
			
			return $this->pegaUm( $sql );
		}
	}*/
	
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
		
		if($this->executar(iconv( "UTF-8", "ISO-8859-1", $sql )))
			return true;
		else
			return false;	
			
	}
	
	public function consultarMunicipio($muncod)
	{
		$sql = "SELECT muncod FROM pse.estadomunicipiopse WHERE muncod = {$muncod}";
		$rs = $this->pegaUm($sql);
		
		if($rs)			
			return true;
		else
			return false;
	}
	
	public function editarMunicipio()
	{	
		 $sql = "UPDATE 
					pse.estadomunicipiopse
				SET 
					stmunpse2009 = {$this->stmunpse2009},
					muncod = {$this->muncod}, 
					nomesecmunsaude = '".($this->nomesecmunsaude)."', 
					endsecmunsaude = '{$this->endsecmunsaude}', 
					nomerepsecmunsaude = '{$this->nomerepsecmunsaude }', 
			     	emailrepsecmunsaude = '{$this->emailrepsecmunsaude}', 
			      	telrepsecmunsaude = '{$this->telrepsecmunsaude}', 
			        carfuncrepsecmunsaude = '{$this->carfuncrepsecmunsaude}', 
			        nomesecmunedu = '{$this->nomesecmunedu}', 
			        endsecmunedu = '{$this->endsecmunedu}', 
			        nomerepsecmunedu = '{$this->nomerepsecmunedu}', 
			        emailrepsecmunedu = '{$this->emailrepsecmunedu}', 
			        telrepsecmunedu = '{$this->telrepsecmunedu}', 
			        carfuncrepsecmunedu = '{$this->carfuncrepsecmunedu}'
		        WHERE 
					muncod = {$this->muncod}";
		return $sql;		
	}
	
	public function salvarMunicipio()
	{
		return $sql = "INSERT INTO 
					pse.estadomunicipiopse
				(
				   stmunpse2009,muncod, nomesecmunsaude, endsecmunsaude, nomerepsecmunsaude, 
			       emailrepsecmunsaude, telrepsecmunsaude, carfuncrepsecmunsaude, 
			       nomesecmunedu, endsecmunedu, nomerepsecmunedu, emailrepsecmunedu, 
			       telrepsecmunedu, carfuncrepsecmunedu
			      )	
			     VALUES
			     (
			     	{$this->stmunpse2009},{$this->muncod},'{$this->nomesecmunsaude}','{$this->endsecmunsaude}','{$this->nomerepsecmunsaude}',
				  	'{$this->emailrepsecmunsaude}', '{$this->telrepsecmunsaude}','{$this->carfuncrepsecmunsaude}','{$this->nomesecmunedu}', '{$this->endsecmunedu}',
				  	'{$this->nomerepsecmunedu}','{$this->emailrepsecmunedu}','{$this->telrepsecmunedu}','{$this->carfuncrepsecmunedu}')";
	}
}