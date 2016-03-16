<?php
	
class Indicador extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.indicador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "indid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'indid' => null, 
									  	'ardid' => null, 
									  	'inddsc' => null, 
									  	'indstatus' => null, 
									  	'indcod' => null, 
									  	'indtipo' => null, 
									  	'indqtdporescola' => null, 
									  	'indpesoideb' => null, 
									  	'indpesodimensao' => null, 
									  );
									  
	public function recuperarPorcentagemDiretrizIndicador( $inuid ){
											  
		$sql = "select di.dirid, sum(dinpeso) as porcentagem
				from cte.indicador i
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					inner join cte.diretrizindicador di on di.indid = i.indid
				where p.inuid = '$inuid'
				and p.ptostatus = 'A'
				and c.ctrpontuacao in ( 0, 3, 4 )
				-- and dirid = 1
				group by di.dirid
				order by di.dirid";
		
		$rsResultado = $this->carregar( $sql );

		return $rsResultado ? $rsResultado : array();
		
	}
	
	public function recuperarPorcentagemDimensao( $inuid ){
											  
		$sql = "select dimcod, sum( indpesodimensao ) as porcentagem
				from cte.indicador i
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					inner join cte.areadimensao a on a.ardid = i.ardid
					inner join cte.dimensao d on d.dimid = a.dimid
				where p.inuid = '$inuid'
				and p.ptostatus = 'A'
				and c.ctrpontuacao in ( 0, 3, 4 )
				group by dimcod
				order by dimcod";
		
		$rsResultado = $this->carregar( $sql );
		
		return $rsResultado ? $rsResultado : array();
		
	}
	
	public function recuperarPorcentagemIDEB( $inuid ){
											  
		$sql = "select sum( indpesoideb ) as porcentagem
				from cte.indicador i
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
				where p.inuid = '$inuid'
				and p.ptostatus = 'A'
				and c.ctrpontuacao in ( 0, 3, 4 )";
		
		$rsResultado = $this->pegaUm( $sql );
		
		return $rsResultado ? $rsResultado : array();
		
	}
	
	public function recuperarTodosPorDiretriz( $dirid, $inuid = null ){
											  
		$inuid = $inuid ? $inuid : $_SESSION["inuid"];
		
		$sql = "select dirid, d.dimcod || '. ' || a.ardcod || '. ' || i.indcod || '. ' || i.inddsc as indicador,
				case 
					when ctrpontuacao in ( 0, 3, 4 ) then dinpeso
					else 0
				end as porcentagem,
				dinpeso				
				from cte.indicador i
					inner join cte.areadimensao a on a .ardid = i.ardid
					inner join cte.dimensao d on d.dimid = a.dimid
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					inner join cte.diretrizindicador di on di.indid = i.indid
				where p.inuid = '$inuid'
				and p.ptostatus = 'A'
				and dirid = '$dirid'
				order by d.dimcod, a.ardcod, indcod";
		
		$rsResultado = $this->carregar( $sql );
		
		return $rsResultado ? $rsResultado : array();
		
	}
		
	public function recuperarTodosPorDimensao( $dimid, $inuid = null ){

		$inuid = $inuid ? $inuid : $_SESSION["inuid"];
		
		$sql = "select d.dimid, d.dimcod || '. ' || a.ardcod || '. ' || i.indcod || '. ' || i.inddsc as indicador,
				case 
					when ctrpontuacao in ( 0, 3, 4 ) then indpesodimensao
					else 0
				end as porcentagem,
				indpesodimensao				
				from cte.indicador i
					inner join cte.areadimensao a on a .ardid = i.ardid
					inner join cte.dimensao d on d.dimid = a.dimid
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
				where p.inuid = '$inuid'
				and p.ptostatus = 'A'
				and d.dimid = '$dimid'
				order by d.dimcod, a.ardcod, indcod";
		
		$rsResultado = $this->carregar( $sql );
		
		return $rsResultado ? $rsResultado : array();
		
	}	
}