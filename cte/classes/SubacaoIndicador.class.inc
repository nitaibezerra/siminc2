<?php
	
class SubacaoIndicador extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.subacaoindicador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sbaid' => null, 
									  	'aciid' => null, 
									  	'undid' => null, 
									  	'frmid' => null, 
									  	'sbadsc' => null, 
									  	'sbastgmpl' => null, 
									  	'sbaprm' => null, 
									  	'sbapcr' => null, 
									  	'sba1ano' => null, 
									  	'sba2ano' => null, 
									  	'sba3ano' => null, 
									  	'sba4ano' => null, 
									  	'sbaunt' => null, 
									  	'sbauntdsc' => null, 
									  	'sba1ini' => null, 
									  	'sba1fim' => null, 
									  	'sba2ini' => null, 
									  	'sba2fim' => null, 
									  	'sba3ini' => null, 
									  	'sba3fim' => null, 
									  	'sba4ini' => null, 
									  	'sba4fim' => null, 
									  	'sbadata' => null, 
									  	'usucpf' => null, 
									  	'sbaparecer' => null, 
									  	'psuid' => null, 
									  	'ssuid' => null, 
									  	'sba0ano' => null, 
									  	'sba0ini' => null, 
									  	'sba0fim' => null, 
									  	'foaid' => null, 
									  	'prgid' => null, 
									  	'sbaporescola' => null, 
									  	'ppsid' => null, 
									  	'sbaordem' => null, 
									  	'sbaobjetivo' => null, 
									  	'sbatexto' => null, 
									  	'sbacategoria' => null, 
									  	'sbaidpai' => null, 
									  	'sbaanoaditivo' => null, 
									  	'sbaseqaditivo' => null, 
									  	'sbasituacaoarvore' => null, 
    									'sbaformulario' => null
									  );
									  
	public function recuperarSubacoesPorPpsid( $arPpsid, $inuid = null ){
											  
		$inuid = $inuid ? $inuid : $_SESSION["inuid"];
		
		if( !is_array( $arPpsid ) ){
			$arPpsid = explode( ", ", $arPpsid ? $arPpsid : 0 );
		}
		
		$sql = "select s.sbaid, s.ppsid
				from cte.subacaoindicador s
					inner join cte.acaoindicador ai on ai.aciid = s.aciid
					inner join cte.pontuacao p on p.ptoid = ai.ptoid
				where ppsid in ( '". implode( "', '", $arPpsid ) ."' )
				and p.inuid = $inuid
				and ptostatus = 'A'";

		$resultado = $this->carregar( $sql );
		return $resultado ? $resultado : array();
	}
	
	public function recuperaUndid($sbaid = NULL){
		if($sbaid != NULL ){
			$sql 	= "	SELECT undid 
						FROM cte.subacaoindicador 
						WHERE sbaid = ".$sbaid;
			$undid 	= $this->pegaUm($sql);
			return $undid;
		}else{
			return '';
		}
	}
	
	public function recuperaPpsid($sbaid){
		$sql = " 	SELECT ppsid 
					FROM cte.subacaoindicador 
					WHERE sbaid = '{$sbaid}' ";
		$ppsid = $this->pegaUm( $sql );
		return $ppsid;
	}
	
	public function insereAditivoSubacao($sbaid, $ano = null){
			$ano = $ano ? $ano : date(Y);
				$sql = " INSERT INTO cte.subacaoindicador (aciid,
								  undid,
								  frmid,
								  sbadsc,
								  sbastgmpl,
								  sbaprm,
								  sbapcr,
								  sbadata,
								  usucpf,
								  sbaparecer,
								  psuid,
								  ssuid,
								  foaid,
								  prgid,
								  sbaporescola,
								  ppsid,
								  sbaordem,
								  sbaobjetivo,
								  sbatexto,
								  sbacategoria,
								  sbaidpai,
								  sbaanoaditivo
								  ) 
				(
						SELECT  aciid,
							undid,
							frmid,
							sbadsc,
							sbastgmpl,
							sbaprm,
							sbapcr,
							sbadata,
							usucpf,
							sbaparecer,
							psuid,
							ssuid,
							foaid,
							prgid,
							sbaporescola,
							ppsid,
							sbaordem,
							sbaobjetivo,
							sbatexto,
							sbacategoria,
							sbaid,
							".$ano." as ano
						FROM cte.subacaoindicador WHERE sbaid = '".$sbaid."'
				) returning sbaid";
		$sbaidAdtivo = $this->pegaUm($sql);
		return $sbaidAdtivo;
	}
	
	public function recuperaUltimoNumAditivo ($sbaid, $ano = NULL){
		$ano = $ano ? $ano : "date_part('year', current_date)";
		$sql="	SELECT CASE WHEN max(sbaseqaditivo) IS NULL THEN 1 ELSE max(sbaseqaditivo)+1 END as sbaseqaditivo 
				FROM cte.subacaoindicador 
				WHERE sbaidpai = ".$sbaid." 
				AND sbaanoaditivo = ".$ano;
		$numAditivo = $this->pegaUm($sql);
		return $numAditivo;
	}
	
	public function atualizaUltimoNumAditivo($sbaidAdtivo, $numAditivo, $ano = null){
		$ano = $ano ? $ano : date(Y);
		$sql = "update cte.subacaoindicador 
				set sbaanoaditivo = ".$ano.", 
					sbaseqaditivo = ".$numAditivo."
				where sbaid = ".$sbaidAdtivo;
		
		$this->executar($sql);
		$this->commit();
		return true;
	}
	
	
}