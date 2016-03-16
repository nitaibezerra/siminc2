<?php
	
class PreEscolasQuadraEsporte extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preescolasquadraesporte";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eqeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eqeid' => null, 
									  	'entcodent' => null, 
									  	'eqedependencianaescola' => null,
    									'eqematriculaef' => null,
    									'eqematriculaem' => null,
    									'eqematriculaefem' => null,
									  );
									  

	public function recuperarEscolasQuadra($preid)
	{
		return $this->carregar($this->recuperarSqlEscolasQuadra($preid));
	}
	
	public function recuperarSqlEscolasQuadra($preid)
	{
		if( $_SESSION['par']['muncod'] ){
			$campo = 'muncod';
			$valor = $_SESSION['par']['muncod'];
			$stWhere .= " AND peq.eqeesfera = 'M'";
			$tpcid = 3;
		}else{
			$campo = 'estuf';
			$valor = $_SESSION['par']['estuf'];
			$stWhere .= " AND peq.eqeesfera = 'E'";
			$tpcid = 1;
		}
		
		switch( $_SESSION['par']['tipo'] ){
			case 'Q':
				$stWhere .= " AND peq.eqepossuiquadra IS FALSE";
				break;
			case 'C':
				$stWhere .= " AND peq.eqepossuiquadra IS TRUE";
				break;
		}
		
		if($preid){
			$stWhere .= " AND ent.entcodent NOT IN (SELECT 
													entcodent 
												  FROM 
													obras.preobra 
												  WHERE 
													preid <> {$preid} 
													AND ".$campo." = '".$valor."' 
													AND entcodent is not null 
													AND prestatus = 'A' 
													AND ptoid in (5,9) )";
		}else{
			$stWhere .= " AND ent.entcodent NOT IN (SELECT 
													entcodent 
												  FROM 
												  	obras.preobra 
												  WHERE 
												  	".$campo." = '".$valor."' 
												  	AND entcodent is not null 
												  	AND prestatus = 'A' 
												  	AND ptoid in (5,9))";
		}
		
		$sql = "SELECT 
					peq.entcodent as codigo,
					ent.entnome as descricao 
				FROM 
					obras.preescolasquadraesporte peq 
				INNER JOIN entidade.entidade ent ON ent.entcodent = peq.entcodent
				INNER JOIN entidade.endereco ed  ON ent.entid 	  = ed.entid
				WHERE 
					ed.".$campo." = '".$valor."'
					{$stWhere}
					AND ent.tpcid = $tpcid";
		return $sql;
	}
	
	public function verificaEscolasQuadraSelecionadas($campo)
	{
		$sql = "select entcodent from obras.preobra where ".$campo['campo']." = '".$campo['valor']."' and entcodent is not null";
		
		return $this->pegaUm($sql);
	}
}