<?php
	

class Programa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.programa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'proid' => null, 
									  	'tpeid' => null,
									  	'pronome' => null, 
									  	'proinstrucoes' => null, 
									  	'procontrapartidaminima' => null, 
									  	'prostatus' => null,
    									'provigenciainicial' => null,
    									'provigenciafinal' => null,
									  );
									  
									  
	public function antesSalvar(){

//		$stComplemento = $this->proid ? " and proid != ". $this->proid : "";
//		$sql = " select count(*) from emenda.programa where proexercicio = ". $this->proexercicio .$stComplemento;
//		if( $this->pegaUm( $sql ) ){
//			echo "<script>alert( 'Número já existe!' );</script>";
//			return false;
//		}
		return true;	
	}
	
	public function depoisSalvar(){

		if( isset( $_SESSION['emenda']['InsereAcaoPrograma'] ) ){
			
			$sql = " update emenda.programaacao set pacstatus = 'I' where proid = ". $this->proid;
			$this->executar( $sql );
			$this->commit();
			
			//ver($_SESSION['emenda']['InsereAcaoPrograma'], d);
			
			foreach( $_SESSION['emenda']['InsereAcaoPrograma'] as $arProgramaAcao ){
				$sql = "SELECT 
						  pacid
						FROM 
						  emenda.programaacao
						WHERE proid = ". $this->proid.
						  " AND acaid = ".$arProgramaAcao['acaid'];
				
				$result = $this->pegaUm( $sql );
				//ver($result, $sql, d);
				$obProgramaAcao = new ProgramaAcao($result);
				if($arProgramaAcao["proid"]){					
					$obProgramaAcao->proid = $arProgramaAcao["proid"];
				}else{
					$obProgramaAcao->proid = $this->proid;//$arProgramaAcao["proid"];
				} 
				$obProgramaAcao->acaid = $arProgramaAcao["acaid"];
				$obProgramaAcao->pacstatus = 'A';
				$obProgramaAcao->salvar();
				$obProgramaAcao->commit();
			}	
			unset( $_SESSION['emenda']['InsereAcaoPrograma'] );
		}
		return true;	
	}	
}