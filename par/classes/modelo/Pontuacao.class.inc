<?php
	
class Pontuacao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.pontuacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptoid' => null, 
									  	'crtid' => null, 
									  	'ptodata' => null, 
									  	'usucpf' => null,
    									'inuid' => null, 
    									'ptostatus' => null,
    									'ptodemandaestadual' => null,
    									'ptodemandamunicipal' => null,
    									'ptojustificativa' => null, 
									  );

	public function recuperaCriteriosPotuacao($indid)
	{
		$sql = "SELECT 
					d.dimcod as codigodimensao,
					a.arecod as codigoarea,
					i.indcod as codigoindicador,
					d.dimdsc as descricaodimensao,
					a.aredsc as descricaoarea,
					i.inddsc as descricaoindicador,
					c.crtid  as idcriterio,
					c.crtdsc as descricaocriterio,
					c.crtpontuacao as pontuacao,
					p.ptoid
				FROM par.dimensao d
					INNER JOIN par.area a ON a.dimid = d.dimid
					INNER JOIN par.indicador i ON i.areid = a.areid
					INNER JOIN par.criterio c ON c.indid = i.indid and crtstatus = 'A'
					LEFT JOIN par.pontuacao p ON p.crtid = c.crtid and p.inuid = '{$_SESSION['par']['inuid']}' AND p.ptostatus = 'A'
				WHERE i.indid = $indid
				ORDER BY c.crtpontuacao";
	//	ver($sql);
		return $this->carregar($sql);
	}
	
	public function recuperaPontuacaoPorIndid($indid)
	{
		$sql = "SELECT
					pto.ptoid,
				    pto.crtid,				    				    
				    pto.inuid,				    
				    pto.ptojustificativa,
				    pto.ptodemandamunicipal,		
				    pto.ptodemandaestadual		
				FROM par.pontuacao pto
				INNER JOIN par.criterio crt ON pto.crtid = crt.crtid and crtstatus = 'A'
				WHERE pto.ptostatus = 'A' AND crt.indid = {$indid} and pto.inuid = '{$_SESSION['par']['inuid']}'";
		return $this->pegaLinha($sql);
	}
	
	public function recuperaProximoAnteriorPorIndid($indid)
	{
		if( !$_SESSION['par']['itrid'] ){
			echo "<script>alert('Falta o Itrid!');history.back(-1);</script>";
			exit();
		}
		
		if($_SESSION['par']['inuid'] != 1){ // verifica se o inuid é DF
			$where = " and i.indid not in (213, 214, 220, 267, 268, 269, 270) ";
		}else{
			if($_SESSION['par']['estuf'] != "DF"){
				$where = " and i.indid not in (213,214,220,267,268,269,270) ";
			}
		}
		
		$sql = "select
					i.indid
				from par.indicador i
					inner join par.area a on i.areid = a.areid and a.arestatus = 'A'
					inner join par.dimensao d on d.dimid = a.dimid and d.dimstatus = 'A'
					inner join par.instrumento ins on ins.itrid = d.itrid
				where
					i.indstatus = 'A' and ins.itrid = '{$_SESSION['par']['itrid']}'
					{$where}
				order by
					d.dimcod,
					a.arecod,
					i.indcod";
		return $this->carregarColuna( $sql, 'indid' );
	}
	
	public function atualizaPontuacao( $ptoid, $inuid, $crtid ){
		
		$sql = "SELECT 
					crtid
				FROM
					par.criterio 
				WHERE 
					indid IN (SELECT
									c.indid
								FROM
									par.criterio c  
								WHERE
									c.crtid = ".$crtid.")
					AND
						crtstatus = 'A'";
						
		$arrCrtid = $this->carregarColuna( $sql );
		
		$crtids = implode( ",", $arrCrtid );
		
		if( $crtids ){
			$sql = "SELECT
						ptoid
					FROM
						par.pontuacao
					WHERE
						ptostatus = 'A'
						AND inuid = {$inuid}
						AND crtid in (".$crtids.")
						AND ptoid not in (".$ptoid.")";
			
			$arrPtoid = $this->carregarColuna( $sql );
			$ptoids = implode( ",", $arrPtoid );

			if( $ptoids ){
				$sql = "UPDATE par.pontuacao SET ptostatus = 'I' WHERE ptoid IN (".$ptoids.")";
				$this->executar( $sql );
				$this->commit( $sql );
			}
		}
		
	}
}									  