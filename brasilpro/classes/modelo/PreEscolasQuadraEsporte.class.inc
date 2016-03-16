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
			$desc = "peq.entcodent || ' - ' || ent.entnome as descricao";
		}else{
			$campo = 'estuf';
			$valor = $_SESSION['par']['estuf'];
			$stWhere .= " AND peq.eqeesfera = 'E'";
			$tpcid = 1;
			$desc = "m.mundescricao || ' - ' || peq.entcodent || ' - ' || ent.entnome as descricao";
		}
		
		if($_SESSION['par']['preid']){
			$sql = "SELECT ptoclassificacaoobra 
					FROM obras.preobra   p
					INNER JOIN obras.pretipoobra pt ON pt.ptoid = p.ptoid
					where preid = ".$_SESSION['par']['preid'];

			$ptoclassificacaoobra = $this->pegaUm($sql);
			
		}
		
		$ptoclassificacaoobra = $ptoclassificacaoobra ? $ptoclassificacaoobra : $_SESSION['par']['tipo'];
		
		switch( $ptoclassificacaoobra ){
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
													obras.preobra p2
												  INNER JOIN obras.pretipoobra pt2 ON pt2.ptoid = p2.ptoid AND pt2.ptoclassificacaoobra IN ('Q','C')
												  INNER JOIN workflow.documento d2 ON d2.docid = p2.docid AND d2.esdid = ".WF_TIPO_EM_CADASTRAMENTO."
												  WHERE 
													preid <> {$preid} 
													AND ".$campo." = '".$valor."' 
													AND entcodent is not null 
													AND prestatus = 'A' )";
		}else{
			$stWhere .= " AND ent.entcodent NOT IN (SELECT 
													entcodent 
												  FROM 
												  	obras.preobra p2
												  INNER JOIN obras.pretipoobra pt2 ON pt2.ptoid = p2.ptoid AND pt2.ptoclassificacaoobra IN ('Q','C')
												  INNER JOIN workflow.documento d2 ON d2.docid = p2.docid AND d2.esdid = ".WF_TIPO_EM_CADASTRAMENTO." 
												  WHERE 
												  	".$campo." = '".$valor."' 
												  	AND entcodent is not null 
												  	AND prestatus = 'A')";
		}
		
		$sql = "SELECT 
					peq.entcodent as codigo,
					{$desc}
				FROM 
					obras.preescolasquadraesporte peq 
				INNER JOIN entidade.entidade ent ON ent.entcodent = peq.entcodent
				INNER JOIN entidade.endereco ed  ON ent.entid 	  = ed.entid
				INNER JOIN territorios.municipio m  ON ed.muncod	  = m.muncod
				WHERE 
					ed.".$campo." = '".$valor."'
					AND peq.eqestatus = 'A'
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