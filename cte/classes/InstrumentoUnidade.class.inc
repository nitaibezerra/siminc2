<?php
	
include_once( APPRAIZ. "cte/classes/Criterio.class.inc" );
include_once( APPRAIZ. "cte/classes/Pontuacao.class.inc" );
include_once( APPRAIZ. "cte/classes/AcaoIndicador.class.inc" );
include_once( APPRAIZ. "cte/classes/SubacaoIndicador.class.inc" );

class InstrumentoUnidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.instrumentounidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "inuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'inuid' => null, 
									  	'itrid' => null, 
									  	'estuf' => null, 
									  	'muncod' => null, 
									  	'mun_estuf' => null, 
									  	'docid' => null, 
									  	'inueqploc' => null, 
									  	'inuddsdmgr' => null, 
									  	'inucadsec' => null, 
									  	'inucadcmt' => null, 
									  	'inudata' => null, 
									  	'usucpf' => null, 
									  	'usucpfescolaativa' => null, 
									  	'inusituacaoadesao' => null,
									  	'usucpfproletramento' => null,
    									'usucpfprofuncionario' => null,
    									'usucpfformacaoescola' => null
    
									  );
									  
	public function criarArvoreEscolaAtiva(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
		
		$sql = "update cte.subacaoindicador s
					set sbasituacaoarvore = ". CTE_SITUACAO_ARVORE_PRINCIPAL ."
				from cte.subacaoindicador sa
					INNER JOIN cte.acaoindicador ai on sa.aciid = ai.aciid
					INNER JOIN cte.pontuacao p on ai.ptoid = p.ptoid
				where p.inuid = ". $_SESSION["inuid"] ."
				and p.ptostatus = 'A'
				and s.sbasituacaoarvore is null
				and s.sbaid = sa.sbaid ";
		$this->executar( $sql );
		
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, pa.ppaid as acaoguia, pa.ppadsc, 
					ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc
				from cte.proposicaosubacao ps
					inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
					inner join cte.criterio c on c.crtid = pa.crtid
					inner join cte.pontuacao p on p.crtid = c.crtid
					inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
				where d.itrid = $itrid
				and p.ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$obAcaoIndicador = new AcaoIndicador();
			$arAcao = $obAcaoIndicador->recuperarAcoesPorPpaid( $arProposicaoSubacao["acaoguia"] );
				
//			ver( $arProposicaoSubacao, $arAcao, $arA );
			
			if( !count( $arAcao ) ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["acaoguia"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
			}
			else{
				$obAcaoIndicador->carregarPorId( $arAcao[0]["aciid"] );
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_ESCOLA_ATIVA ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_AMBAS;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_ESCOLA_ATIVA;
				$obSubacaoIndicador->salvar();
			}
//			ver( $obSubacaoIndicador );
			
			$obSubacaoIndicador->commit();
		}
//		ver( 123, d );
	}
	
	public function criarArvoreProLetramento(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, pa.ppaid, pa.ppadsc, 
					ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc
				from cte.proposicaosubacao ps
					inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
					inner join cte.criterio c on c.crtid = pa.crtid
					inner join cte.pontuacao p on p.crtid = c.crtid
					inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
				where d.itrid = $itrid
				and ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 1057, 1058, 1059, 1060, 1061, 1049, 1051, 1053, 1055, 1056, 904, 666, 1052, 1050, 1054 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$obAcaoIndicador = new AcaoIndicador();
			$arAcao = $obAcaoIndicador->recuperarAcoesPorPpaid( $arProposicaoSubacao["ppaid"] );
				
			if( !count( $arAcao ) ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["ppaid"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
			}
			else{
				$obAcaoIndicador->carregarPorId( $arAcao[0]["aciid"] );
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_ESCOLA_ATIVA ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_PRO_LETRAMENTO;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRO_LETRAMENTO;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreProFuncionario(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, pa.ppaid, pa.ppadsc, 
					ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc
				from cte.proposicaosubacao ps
					inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
					inner join cte.criterio c on c.crtid = pa.crtid
					inner join cte.pontuacao p on p.crtid = c.crtid
					inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
				where d.itrid = $itrid
				and ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 924, 689, 494, 743 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
//		ver( $sql, $coProposicaoSubacao, d );
		
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$obAcaoIndicador = new AcaoIndicador();
			$arAcao = $obAcaoIndicador->recuperarAcoesPorPpaid( $arProposicaoSubacao["ppaid"] );
				
//			ver( $arProposicaoSubacao, $arAcao, $arA, d );
			
			if( !count( $arAcao ) ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["ppaid"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
			}
			else{
				$obAcaoIndicador->carregarPorId( $arAcao[0]["aciid"] );
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_ESCOLA_ATIVA ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_PRO_FUNCIONARIO;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRO_FUNCIONARIO;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreFormacaoEscola(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
//		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, pa.ppaid, pa.ppadsc, 
//					ps.*, p.ptoid,
//					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
//					i.indid, i.inddsc
//				from cte.proposicaosubacao ps
//					inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
//					inner join cte.criterio c on c.crtid = pa.crtid
//					inner join cte.pontuacao p on p.crtid = c.crtid
//					inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
//					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
//					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
//				where d.itrid = $itrid
//				and ptostatus = 'A'
//				and p.inuid = ". $_SESSION["inuid"] ."
//				and ps.ppsid in ( 456, 1235, 1240, 1245, 1250, 1255, 1260, 1265, 1270, 1275 )
//				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem";

		$sql = "select distinct 
					d.dimcod, ad.ardcod, i.indcod, pa.ppaid, pa.ppadsc, 
					ps.*, p.ptoid, d.itrid, d.dimdsc, d.dimid, ad.ardid, 
					ad.arddsc, i.indid, i.inddsc
				from cte.pontuacao p 
					inner join cte.criterio c 		on p.crtid = p.crtid
					inner join cte.proposicaoacao pa 	on pa.crtid = c.crtid
					inner join cte.proposicaosubacao ps 	on ps.ppaid = pa.ppaid
				inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
				where p.inuid = ". $_SESSION["inuid"] ."
					and ptostatus = 'A'
					and ps.ppsid in ( 1232, 1237, 1242, 1247, 1252, 1257, 1262, 1267, 1272, 1277 )
					--and ps.ppsid in ( 1233, 1238, 1243, 1248, 1253, 1258, 1263, 1268, 1273, 1278 )
					--and ps.ppsid in ( 1234, 1239, 1244, 1249, 1254, 1259, 1264, 1269, 1274, 1279 )
					and d.itrid = $itrid
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
//		ver( $sql, $coProposicaoSubacao, d );
		
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$obAcaoIndicador = new AcaoIndicador();
			$arAcao = $obAcaoIndicador->recuperarAcoesPorPpaid( $arProposicaoSubacao["ppaid"] );
			$sql = "select * from cte.pontuacao where inuid = ". $_SESSION["inuid"] ." and ptostatus = 'A' and indid = 292";
			$stPtoid = $this->carregar($sql);
				
//			ver( $arProposicaoSubacao, $arAcao, $arA, d );
			
			if( !count( $arAcao ) ){
				
				if(count($stPtoid)){					
					$obAcaoIndicador->ptoid = $stPtoid[0]["ptoid"];					
				} else {					
					$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				}
				
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["ppaid"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
			}
			else{
				$obAcaoIndicador->carregarPorId( $arAcao[0]["aciid"] );
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_ESCOLA_ATIVA ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_FORMACAO_ESCOLA;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_FORMACAO_ESCOLA;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreEscolaAtivaEstadual(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
		
		$sql = "update cte.subacaoindicador s
					set sbasituacaoarvore = ". CTE_SITUACAO_ARVORE_PRINCIPAL ."
				from cte.subacaoindicador sa
					INNER JOIN cte.acaoindicador ai on sa.aciid = ai.aciid
					INNER JOIN cte.pontuacao p on ai.ptoid = p.ptoid
				where p.inuid = ". $_SESSION["inuid"] ."
				and p.ptostatus = 'A'
				and s.sbasituacaoarvore is null
				and s.sbaid = sa.sbaid ";
		$this->executar( $sql );
		
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc, c.ctrpontuacao, a.aciid
				from cte.proposicaosubacao ps
					inner join cte.indicador i on i.indid = ps.indid and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					left join cte.acaoindicador a on a.ptoid = p.ptoid
				where d.itrid = 1
				and p.ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 1064, 1065, 278, 272, 273, 274, 275, 276, 277, 1066 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem ";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		$aciid = null;
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){

			$aciid = $arProposicaoSubacao["aciid"] ? $arProposicaoSubacao["aciid"] : $aciid;
			
			$obAcaoIndicador = new AcaoIndicador( $aciid );

			if( !$obAcaoIndicador->aciid ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["acaoguia"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
				$aciid = $obAcaoIndicador->aciid;
			}
		
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_ESCOLA_ATIVA ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_AMBAS;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_ESCOLA_ATIVA;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreProLetramentoEstadual(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc, c.ctrpontuacao, a.aciid
				from cte.proposicaosubacao ps
					inner join cte.indicador i on i.indid = ps.indid and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					left join cte.acaoindicador a on a.ptoid = p.ptoid
				where d.itrid = 1
				and p.ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 348, 1062, 1063 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem ";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		$aciid = null;
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$aciid = $arProposicaoSubacao["aciid"] ? $arProposicaoSubacao["aciid"] : $aciid;
			
			$obAcaoIndicador = new AcaoIndicador( $aciid );

			if( !$obAcaoIndicador->aciid ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["acaoguia"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
				$aciid = $obAcaoIndicador->aciid;
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_PRO_LETRAMENTO ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_PRO_LETRAMENTO;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRO_LETRAMENTO;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreProFuncionarioEstadual(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc, c.ctrpontuacao, a.aciid
				from cte.proposicaosubacao ps
					inner join cte.indicador i on i.indid = ps.indid and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					left join cte.acaoindicador a on a.ptoid = p.ptoid
				where d.itrid = 1
				and p.ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 348, 1062, 1063 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem ";

		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		$aciid = null;
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$aciid = $arProposicaoSubacao["aciid"] ? $arProposicaoSubacao["aciid"] : $aciid;
			
			$obAcaoIndicador = new AcaoIndicador( $aciid );

			if( !$obAcaoIndicador->aciid ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["acaoguia"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
				$aciid = $obAcaoIndicador->aciid;
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_PRO_FUNCIONARIO ){
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_PRO_FUNCIONARIO;
					$obSubacaoIndicador->salvar();
				}
			}
			else{
				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRO_FUNCIONARIO;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
		}
	}
	
	public function criarArvoreFormacaoEscolaEstadual(){
		
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$acilocalizador = $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";
				
		$sql = "select distinct d.dimcod, ad.ardcod, i.indcod, ps.*, p.ptoid,
					d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
					i.indid, i.inddsc, c.ctrpontuacao, a.aciid
				from cte.proposicaosubacao ps
					inner join cte.indicador i on i.indid = ps.indid and i.indstatus = 'A'
					inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
					inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
					inner join cte.pontuacao p on p.indid = i.indid
					inner join cte.criterio c on c.crtid = p.crtid
					left join cte.acaoindicador a on a.ptoid = p.ptoid
				where d.itrid = 1
				and p.ptostatus = 'A'
				and p.inuid = ". $_SESSION["inuid"] ."
				and ps.ppsid in ( 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291 )
				order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem ";
		
		$resultado = $this->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
//		ver($sql, count($coProposicaoSubacao));
//		die();
		
		$aciid = null;
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$aciid = $arProposicaoSubacao["aciid"] ? $arProposicaoSubacao["aciid"] : $aciid;
			
			$obAcaoIndicador = new AcaoIndicador( $aciid );

			if( !$obAcaoIndicador->aciid ){
				
				$obAcaoIndicador->ptoid = $arProposicaoSubacao["ptoid"];
				$obAcaoIndicador->acidsc = $arProposicaoSubacao["ppadsc"];
				$obAcaoIndicador->acidata = date( 'Y-m-d' );
				$obAcaoIndicador->acilocalizador = $acilocalizador;
//				$obAcaoIndicador->usucpf = $_SESSION["usucpf"];
				$obAcaoIndicador->ppaid = $arProposicaoSubacao["acaoguia"];
				$obAcaoIndicador->salvar();
				$obAcaoIndicador->commit();
				$aciid = $obAcaoIndicador->aciid;
			}
				
			$obSubacaoIndicador = new SubacaoIndicador();
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"] );
			
			if( count( $arSubacao ) ){
				$obSubacaoIndicador->carregarPorId( $arSubacao[0]["sbaid"] );
				if( $obSubacaoIndicador->sbasituacaoarvore != CTE_SITUACAO_ARVORE_FORMACAO_ESCOLA ){				
					$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_FORMACAO_ESCOLA;
					$obSubacaoIndicador->salvar();
				}
			}
			else{				
				$obSubacaoIndicador->aciid = $obAcaoIndicador->aciid;
				$obSubacaoIndicador->undid = $arProposicaoSubacao["undid"];
				$obSubacaoIndicador->sbadsc = $arProposicaoSubacao["ppsdsc"];
				$obSubacaoIndicador->sbastgmpl = $arProposicaoSubacao["ppsmetodologia"];
				$obSubacaoIndicador->frmid = $arProposicaoSubacao["frmid"];
				$obSubacaoIndicador->prgid = $arProposicaoSubacao["prgid"];
				$obSubacaoIndicador->sbatexto = $arProposicaoSubacao["ppstexto"];
				$obSubacaoIndicador->sbadata = date( 'Y-m-d' );
				//$obSubacaoIndicador->usucpf = $_SESSION["usucpf"];
				$obSubacaoIndicador->sbaobjetivo = $arProposicaoSubacao["ppsobjetivo"];
				$obSubacaoIndicador->sbaporescola = $arProposicaoSubacao["indqtdporescola"];
				$obSubacaoIndicador->ppsid = $arProposicaoSubacao["ppsid"];
				$obSubacaoIndicador->sbaordem = $arProposicaoSubacao["ppsordem"];
				$obSubacaoIndicador->sbasituacaoarvore = CTE_SITUACAO_ARVORE_PRINCIPAL_E_FORMACAO_ESCOLA;
				$obSubacaoIndicador->salvar();
			}
			
			$obSubacaoIndicador->commit();
			
		}
	}
	
	/****************************************************************************************
	*										MUNICÍPIOS 										* 
	****************************************************************************************/	
	
	public function recuperarTabelaEstados( $tipo, $ano = '2011' ){

		$esdid = "";		
		switch( $tipo ){
			case( 'geral' ):
				$sql = "select distinct e.estuf, e.estdescricao
						from cte.instrumentounidade iu
							inner join territorios.estado e on e.estuf = iu.mun_estuf
						--where itrid = 2
						order by e.estuf";
				break;
			case( 'aderiram' ):
				$sql = "select distinct e.estuf, e.estdescricao
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
							inner join territorios.estado e on e.estuf = iu.mun_estuf
							inner join territorios.municipio m on m.muncod = iu.muncod
							where ea.esaano = {$ano}
							and ea.esasituacaoadesao = 1
							--and iu.itrid = 2
							order by e.estuf";
				break;
			case( 'naoaderiram' ):				
				$sql = "select distinct e.estuf, e.estdescricao
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
							inner join territorios.estado e on e.estuf = iu.mun_estuf
							inner join territorios.municipio m on m.muncod = iu.muncod
							where ea.esaano = {$ano}
							and ea.esasituacaoadesao = 2
							--and iu.itrid = 2
							order by e.estuf";				
				break;
			case( 'classesmultiseriadas' ):				
				$sql = "select distinct e.estuf, e.estdescricao
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
							inner join territorios.estado e on e.estuf = iu.mun_estuf
							inner join territorios.municipio m on m.muncod = iu.muncod
							where ea.esaano = {$ano}
							and ea.esasituacaoadesao = 3
							--and iu.itrid = 2
							order by e.estuf";	

				break;
			case( 'naomanifestados' ):	
				$sql = "select distinct 
							e.estuf, 
							e.estdescricao							
						from cte.instrumentounidade iu
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod				
						left join cte.escolaativa ea on ea.inuid = iu.inuid and ea.esaano = '{$ano}'
						where ea.esaid is null
						--and iu.itrid = 2
						order by e.estuf";	

				break;
			case( 'em_preenchimento' ): $esdid = 47; break;
			case( 'em_analise' 		 ): $esdid = 48; break;
			case( 'em_revisao' 		 ): $esdid = 50; break;
			case( 'em_correcao' 	 ): $esdid = 51; break;
			case( 'analisado'  		 ): $esdid = 49; break;
		}
		if( $esdid ){
			
			$sql = "select distinct e.estuf, e.estdescricao
					from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod
						left join workflow.documento d on d.docid = ea.docid
					where coalesce( d.esdid, 47 ) = $esdid
					and ea.esaano = {$ano}
					and esasituacaoadesao in (1)
					--and iu.itrid = 2
					order by e.estuf";
		}
		
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? $arDados : array();
//		ver($sql, $arDados, d);

		return $this->montarTabelaEstados( $arDados, $tipo, $ano );
	}
	
	public function recuperarTabelaMunicipios( $tipo, $estuf, $ano = '2011' ){		
		
		$esdid = "";
		switch( $tipo ){
			case( 'geral' ):
				$sql = "select distinct ea.estuf, ea.muncod, ea.mundescricao
						from cte.escola_ativa ea
						where ea.estuf = '$estuf'
						order by ea.estuf, ea.mundescricao";				
				break;
			case( 'aderiram' ):
				$sql = "select distinct e.estuf, m.muncod, m.mundescricao
						from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod						
						where ea.esasituacaoadesao = 1
						and e.estuf = '$estuf'
						and ea.esaano = '{$ano}'
						order by e.estuf, m.mundescricao";				
				break;
			case( 'naoaderiram' ):
				$sql = "select distinct e.estuf, m.muncod, m.mundescricao
						from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod						
						where ea.esasituacaoadesao = 2
						and e.estuf = '$estuf'
						and ea.esaano = '{$ano}'
						and iu.itrid = 2
						order by e.estuf, m.mundescricao";	
				break;
			case( 'classesmultiseriadas' ):
				$sql = "select distinct e.estuf, m.muncod, m.mundescricao
						from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod						
						where ea.esasituacaoadesao = 3
						and e.estuf = '$estuf'
						and ea.esaano = '{$ano}'
						and iu.itrid = 2
						order by e.estuf, m.mundescricao";	
				break;
			case( 'naomanifestados' ):	
				$sql = "select distinct 
							e.estuf, 
							m.muncod, 
							m.mundescricao							
						from cte.instrumentounidade iu
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod				
						left join cte.escolaativa ea on ea.inuid = iu.inuid and ea.esaano = '{$ano}'
						where ea.esaid is null
						and e.estuf = '$estuf'
						and iu.itrid = 2
						order by e.estuf, m.mundescricao";	

				break;
			case( 'em_preenchimento' ): $esdid = 47; break;
			case( 'em_analise' 		 ): $esdid = 48; break;
			case( 'em_revisao' 		 ): $esdid = 50; break;
			case( 'em_correcao' 	 ): $esdid = 51; break;
			case( 'analisado'  		 ): $esdid = 49; break;
		}
		if( $esdid ){
			
			$sql = "select distinct iu.mun_estuf as estuf, iu.muncod, m.mundescricao
					from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join territorios.estado e on e.estuf = iu.mun_estuf
						inner join territorios.municipio m on m.muncod = iu.muncod
						left join workflow.documento d on d.docid = ea.docid
					where coalesce( d.esdid, 47 ) = $esdid
					and iu.mun_estuf = '$estuf'
					and ea.esaano = '{$ano}'
					and esasituacaoadesao in (1)
					order by iu.mun_estuf, m.mundescricao";
		}
		
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? $arDados : array();
	
		return $this->montarTabelaMunicipios( $arDados, $tipo, $ano );
		
	}
	
	public function recuperarTabelaEscolas( $tipo, $muncod, $ano = '2011' ){
		
		if( $tipo == 'geral' ){
			$sql = "select distinct e.entid, e.entnome, e.entcodent, ea.esasituacaoadesao --ed.entdmultiseriada
					from entidade.entidade e
						inner join entidade.funcaoentidade ef on e.entid = ef.entid 
						inner join entidade.endereco ende on e.entid = ende.entid
						inner join cte.instrumentounidade iu on iu.muncod = ende.muncod
						inner join cte.escolaativa ea on ea.inuid = iu.inuid
						--left join entidade.entidadedetalhe ed on ed.entid = e.entid 
					where funid = 3 and entstatus = 'A'
					and ende.muncod = '$muncod'
					order by e.entnome";
		}
		else{
			$sql = "select distinct eae.entid, e.entnome, e.entcodent, ea.esasituacaoadesao --ed.entdmultiseriada
					from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						inner join cte.escolaativaescolas eae on eae.esaid = ea.esaid
						inner join entidade.entidade e on e.entid = eae.entid
						--left join entidade.entidadedetalhe ed on ed.entid = e.entid 
					where iu.muncod = '$muncod'
					and ea.esaano = '{$ano}'
					order by e.entnome";
		}
		
//		ver($sql, d);
		
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? $arDados : array();
	
		return $this->montarTabelaEscolas( $arDados, $tipo, $ano );
	}
	
	public function recuperarResumo( $tipo, $estuf = null, $muncod = null, $entid = null ){

	$arInner = array();
	$arInner2 = array();
	$arWhere = array( "1 = 1" );
	$arWhere2 = array( "1 = 1" );
	
	switch( $tipo ){
		case( 'aderiram' ): 
			$arInner[] = "	inner join territorios.municipio m on m.muncod = ende.muncod 
							inner join cte.instrumentounidade iu on iu.muncod = m.muncod  and iu.itrid = 2 "; 
			$arWhere[] = " iu.inusituacaoadesao = 1";
			$arWhere2[] = " iu.inusituacaoadesao = 1";
			break;
		case( 'preenchidos' ): 
			$arInner[] = "	inner join territorios.municipio m on m.muncod = ende.muncod 
							inner join cte.instrumentounidade iu on iu.muncod = m.muncod  and iu.itrid = 2 
							inner join cte.escola_ativa_preenchida eap on eap.inuid = iu.inuid "; 
			
			$arInner2[] = "	inner join cte.escola_ativa_preenchida eap on eap.inuid = iu.inuid "; 
			
			$arWhere[] = " eap.preenchida >= 10";
			$arWhere2[] = " eap.preenchida >= 10";
			break;
		case( 'nao_preenchidos' ): 
			$arInner[] = "	inner join territorios.municipio m on m.muncod = ende.muncod 
							inner join cte.instrumentounidade iu on iu.muncod = m.muncod  and iu.itrid = 2 
							inner join cte.escola_ativa_preenchida eap on eap.inuid = iu.inuid ";
			 
			$arInner2[] = "	inner join cte.escola_ativa_preenchida eap on eap.inuid = iu.inuid "; 
			
			$arWhere[] = " eap.preenchida < 10";
			$arWhere2[] = " eap.preenchida < 10";
			break;
		case( 'analisados' ): 
			$arInner[] = "	inner join territorios.municipio m on m.muncod = ende.muncod 
							inner join cte.instrumentounidade iu on iu.muncod = m.muncod  and iu.itrid = 2 
							inner join cte.escola_ativa_analisada eaa on eaa.inuid = iu.inuid "; 
			
			$arInner2[] = "	inner join cte.escola_ativa_analisada eaa on eaa.inuid = iu.inuid "; 
			
			$arWhere[] = " eaa.analisada >= 10";
			$arWhere2[] = " eaa.analisada >= 10";
			break;
		case( 'nao_analisados' ): 
			$arInner[] = "	inner join territorios.municipio m on m.muncod = ende.muncod 
							inner join cte.instrumentounidade iu on iu.muncod = m.muncod  and iu.itrid = 2 
							inner join cte.escola_ativa_analisada eaa on eaa.inuid = iu.inuid ";
			 
			$arInner2[] = "	inner join cte.escola_ativa_analisada eaa on eaa.inuid = iu.inuid "; 
			
			$arWhere[] = " eaa.analisada < 10";
			$arWhere2[] = " eaa.analisada < 10";
			break;
	}
	
	if( $muncod ){		
		$arWhere[] = " ende.muncod = '$muncod'";
		$arWhere2[] = " iu.muncod = '$muncod'";
	}
	if( $estuf ){
		$arWhere[] = " ende.estuf = '$estuf'";		
		$arWhere2[] = " iu.mun_estuf = '$estuf'";
	}
	
	$sql = "SELECT  SUM(totalEscolas) AS totalEscolas,
				SUM(totalEscolasMultisseriadas) AS totalEscolasMultisseriadas,
				SUM(qtdTotalTurmas) AS qtdTotalTurmas,
				SUM(qtdTotalAlunos) AS qtdTotalAlunos,
				SUM(qtdTotalProfessores) AS qtdTotalProfessores
			FROM (
				select count(1) as totalEscolas,
					0 AS totalEscolasMultisseriadas,
					0 AS qtdTotalTurmas,
					0 AS qtdTotalAlunos,
					0 AS qtdTotalProfessores
				from entidade.entidade e
					inner join entidade.funcaoentidade ef on e.entid = ef.entid 
					inner join entidade.endereco ende on e.entid = ende.entid
					". implode( '  ', $arInner ) ." 
				where funid = 3 and entstatus = 'A'
				and ". implode( ' and ', $arWhere ) ."
				--and ende.estuf = 'AC'
				--and ende.estuf = 'AC'
				
				UNION ALL
				
				select 0 AS totalEscolas,
					count(1) as totalEscolasMultisseriadas,
					0 AS qtdTotalTurmas,
					0 AS qtdTotalAlunos,
					0 AS qtdTotalProfessores
				from entidade.entidade e
					inner join entidade.entidadedetalhe ed on ed.entid = e.entid 
					inner join entidade.endereco ende on e.entid = ende.entid
					". implode( '  ', $arInner ) ."  
				where ed.entdmultiseriada = 't'
				and ". implode( ' and ', $arWhere ) ."
				
				UNION ALL
				
				select 	0 AS totalEscolas,
					0 AS totalEscolasMultisseriadas,
					sum( entdmultiseriqtdtur1 + entdmultiseriqtdtur2 + entdmultiseriqtdtur3 + entdmultiseriqtdtur4  + entdmultiseriqtdtur5 + entdmultiseriqtdtur6 + entdmultiseriqtdtur7 + entdmultiseriqtdtur8 + entdmultiseriqtdtur9 ) as qtdTotalTurmas,
					0 AS qtdTotalAlunos,
					0 AS qtdTotalProfessores
				from entidade.entidadedetalhe ed
					inner join entidade.endereco ende on ed.entid = ende.entid
					". implode( '  ', $arInner ) ."  	
				where ". implode( ' and ', $arWhere ) ."					
				
				UNION ALL
				
				select 	0 AS totalEscolas,
					0 AS totalEscolasMultisseriadas,
					0 AS qtdTotalTurmas,
					sum( entdmultiseriqtdalu1 + entdmultiseriqtdalu2 + entdmultiseriqtdalu3 + entdmultiseriqtdalu4  + entdmultiseriqtdalu5 + entdmultiseriqtdalu6 + entdmultiseriqtdalu7 + entdmultiseriqtdalu8 + entdmultiseriqtdalu9 ) as qtdTotalAlunos,
					0 AS qtdTotalProfessores
				from entidade.entidadedetalhe ed
					inner join entidade.endereco ende on ed.entid = ende.entid
					". implode( '  ', $arInner ) ." 
				where ". implode( ' and ', $arWhere ) ."					 	
			
				UNION ALL
			
				select  0 AS totalEscolas,
					0 AS totalEscolasMultisseriadas,
					0 AS qtdTotalTurmas,
					0 AS qtdTotalAlunos,
					sum(entdmultiseriqtdprofessor) as qtdTotalProfessores
				from entidade.entidadedetalhe ed
					inner join entidade.endereco ende on ed.entid = ende.entid
					". implode( '  ', $arInner ) ."  	
				where ". implode( ' and ', $arWhere ) ."					 	
				
			) AS FOO";
	
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? $arDados[0] : array();
		
		$sql = "select 	iu.inuid, esaqtdprofessores, sum( eaeqtdturmas ) as qtdturmas, 
						sum( eaeqtdalunos1 + eaeqtdalunos2 + eaeqtdalunos3 + eaeqtdalunos4 + eaeqtdalunos5 ) as totalalunos
				from cte.escolaativa ea
					left join cte.escolaativaescolas eae on eae.esaid = ea.esaid
					inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
				where iu.muncod = '1200013'
				group by iu.inuid, esaqtdprofessores";
		
		/*
		$sql = "select sum( quantidadeturmas ) as quantidadeturmas
				from(
					SELECT  es.estuf  as estuf,
						iu.inuid,
						mu.muncod as muncod,
						entid,
						sum(qt.qfaqtd) as quantidadeturmas
					FROM cte.instrumentounidade iu
					INNER JOIN territorios.municipio mu ON mu.muncod = iu.muncod
					INNER JOIN territorios.estado 	 es ON es.estuf = mu.estuf
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.qtdfisicoano 	 qt ON qt.sbaid = si.sbaid
					". implode( '  ', $arInner2 ) ."
					
					WHERE si.sbaordem = 29
					AND pt.ptostatus = 'A'
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )					
					AND qt.qfaano = 2010
					and ". implode( ' and ', $arWhere2 ) ."
					-- AND si.sbaid = 2881406
					-- AND iu.inuid = 1509
					GROUP BY mu.muncod, iu.inuid, es.estuf , entid
				) as foo";		
		
//		ver( $sql );
		
		$arDados["qtdescolaturmas"] = $this->pegaUm( $sql );
	
		$sql = "select sum( qtdalunos ) as qtdalunos
				from(
					SELECT  es.estuf  as estuf,
						iu.inuid,
						mu.muncod as muncod,
						entid,
						sum(qt.qfaqtd) as qtdalunos
					FROM cte.instrumentounidade iu
					INNER JOIN territorios.municipio mu ON mu.muncod = iu.muncod
					INNER JOIN territorios.estado 	 es ON es.estuf = mu.estuf
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.qtdfisicoano 	 qt ON qt.sbaid = si.sbaid
					". implode( '  ', $arInner2 ) ."
					WHERE si.sbaordem in(17, 18, 19, 20, 21)
					AND pt.ptostatus = 'A'
					and ". implode( ' and ', $arWhere2 ) ."
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					AND qt.qfaano = 2010
					--AND si.sbaid = 2881406
					--AND iu.inuid = 1509
					GROUP BY mu.muncod, iu.inuid, es.estuf , entid
				) as foo";		
		
		$arDados["qtdescolaalunos"] = $this->pegaUm( $sql );
		
		$sql = "select sum( quantidadeprofessores ) as quantidadeprofessores
				from(
					SELECT  es.estuf  as estuf,
						iu.inuid,
						mu.muncod as muncod,
						sum(cp.sptunt) as quantidadeprofessores
					FROM cte.instrumentounidade iu
					INNER JOIN territorios.municipio mu ON mu.muncod = iu.muncod
					INNER JOIN territorios.estado 	 es ON es.estuf = mu.estuf
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.subacaoparecertecnico cp ON cp.sbaid = si.sbaid
					". implode( '  ', $arInner2 ) ."
					WHERE si.sbaordem in(13)
					AND pt.ptostatus = 'A'
					AND cp.sptano = 2010
					and ". implode( ' and ', $arWhere2 ) ."
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					--AND si.sbaid = 2881406
					--AND iu.inuid = 1509
					GROUP BY mu.muncod, iu.inuid, es.estuf
					ORDER BY es.estuf
				) as foo";
		
		*/
		
		$arDados["qtdescolaprofessores"] = $this->pegaUm( $sql );
		
		$sql = "select sum( escolasaderiram ) as escolasaderiram
				from(
					SELECT iu.inuid, iu.mun_estuf as estuf, iu.muncod, count(distinct qt.entid) as escolasaderiram
					FROM cte.instrumentounidade iu 
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.qtdfisicoano 	 qt ON qt.sbaid = si.sbaid
					". implode( '  ', $arInner2 ) ."
					WHERE iu.inusituacaoadesao = 1
					and ". implode( ' and ', $arWhere2 ) ."
					AND iu.itrid = 2
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					AND si.sbaporescola = 't'
					AND qt.qfaano = 2010
					GROUP BY iu.mun_estuf, iu.inuid,  iu.muncod
				) as foo";
		
		$arDados["escolasaderiram"] = $this->pegaUm( $sql );
		
		
		return $this->montarTabelaResumo( $arDados, $tipo );
		
	}
	
	public function recuperarResumoEscola( $tipo, $estuf = null, $muncod = null, $entid = null ){

	$arInner = array();
	$arWhere = array( "e.entid = '$entid'" );
	$arWhere2 = array( "qt.entid = '$entid'" );
	
	if( $muncod ){		
		$arWhere[] = " ende.muncod = '$muncod'";
		$arWhere2[] = " iu.muncod = '$muncod'";
	}
	if( $estuf ){
		$arWhere[] = " ende.estuf = '$estuf'";		
		$arWhere2[] = " iu.mun_estuf = '$estuf'";		
	}	
	
	$sql = "-- TOTAL DE TUDO
			SELECT  
				SUM(qtdTotalTurmas) AS qtdTotalTurmas,
				SUM(qtdTotalAlunos) AS qtdTotalAlunos
				
			FROM (
				select 	sum( entdmultiseriqtdtur1 + entdmultiseriqtdtur2 + entdmultiseriqtdtur3 + entdmultiseriqtdtur4  + entdmultiseriqtdtur5 + entdmultiseriqtdtur6 + entdmultiseriqtdtur7 + entdmultiseriqtdtur8 + entdmultiseriqtdtur9 ) as qtdTotalTurmas,
						0 AS qtdTotalAlunos
				from entidade.entidadedetalhe e
					inner join entidade.endereco ende on e.entid = ende.entid 
				where ". implode( ' and ', $arWhere ) ."	
				
				UNION ALL
				
				select 	0 AS qtdTotalTurmas,
						sum( entdmultiseriqtdalu1 + entdmultiseriqtdalu2 + entdmultiseriqtdalu3 + entdmultiseriqtdalu4  + entdmultiseriqtdalu5 + entdmultiseriqtdalu6 + entdmultiseriqtdalu7 + entdmultiseriqtdalu8 + entdmultiseriqtdalu9 ) as qtdTotalAlunos
				from entidade.entidadedetalhe e
					inner join entidade.endereco ende on e.entid = ende.entid
				where ". implode( ' and ', $arWhere ) ."	  
				
			) AS FOO";
	
		$arDados = $this->carregar( $sql );
		$arDados = $arDados ? $arDados[0] : array();
	
		$sql = "select sum( quantidadeturmas ) as quantidadeturmas
				from(
					SELECT  es.estuf  as estuf,
						iu.inuid,
						mu.muncod as muncod,
						entid,
						sum(qt.qfaqtd) as quantidadeturmas
					FROM cte.instrumentounidade iu
					INNER JOIN territorios.municipio mu ON mu.muncod = iu.muncod
					INNER JOIN territorios.estado 	 es ON es.estuf = mu.estuf
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.qtdfisicoano 	 qt ON qt.sbaid = si.sbaid
					WHERE si.sbaordem = 29
					AND pt.ptostatus = 'A'
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )					
					AND qt.qfaano = 2010
					and ". implode( ' and ', $arWhere2 ) ."
					-- AND si.sbaid = 2881406
					-- AND iu.inuid = 1509
					GROUP BY mu.muncod, iu.inuid, es.estuf , entid
				) as foo";		
		
		$arDados["qtdescolaturmas"] = $this->pegaUm( $sql );
	
		$sql = "select sum( qtdalunos ) as qtdalunos
				from(
					SELECT  es.estuf  as estuf,
						iu.inuid,
						mu.muncod as muncod,
						entid,
						sum(qt.qfaqtd) as qtdalunos
					FROM cte.instrumentounidade iu
					INNER JOIN territorios.municipio mu ON mu.muncod = iu.muncod
					INNER JOIN territorios.estado 	 es ON es.estuf = mu.estuf
					INNER JOIN cte.pontuacao 	 pt ON pt.inuid = iu.inuid
					INNER JOIN cte.acaoindicador 	 ac ON ac.ptoid = pt.ptoid
					INNER JOIN cte.subacaoindicador  si ON si.aciid = ac.aciid
					INNER JOIN cte.qtdfisicoano 	 qt ON qt.sbaid = si.sbaid
					WHERE si.sbaordem in(17, 18, 19, 20, 21)
					AND pt.ptostatus = 'A'
					and ". implode( ' and ', $arWhere2 ) ."
					AND ( si.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 
							   1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 
							   1036, 277, 1048, 1044, 1045, 1046, 1047 ) )
					AND qt.qfaano = 2010
					--AND si.sbaid = 2881406
					--AND iu.inuid = 1509
					GROUP BY mu.muncod, iu.inuid, es.estuf , entid
				) as foo";		
		
		$arDados["qtdescolaalunos"] = $this->pegaUm( $sql );		
		
		$sql = "select entdmultiseriada from cte.escola_ativa where entid = '$entid'";
		$entdmultiseriada = $this->pegaUm( $sql );
		$entdmultiseriada = $entdmultiseriada == 't' ? "Sim" : "Não"; 
		
		$arDados["multisseriada"] = $entdmultiseriada;
		
		return $this->montarTabelaResumoEscola( $arDados, $tipo );
		
	}
	
	/****************************************************************************************
	*									MONTAGEM DAS TABELAS								* 
	****************************************************************************************/
	
	public function montarTabelaResumo( $arDados, $tipo ){
		
		$stTabela  = "";
		if( count( $arDados ) ){
			$stTabela .= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">';
			$stTabela .= '	<tr style="background: #e3e3e3; text-align: center;" >';
			$stTabela .= '		<td>Qtd. Escolas dos Municípios</td>';
			$stTabela .= '		<td>Qtd. Escolas Multisseriadas</td>';
			$stTabela .= '		<td>Qtd. Escolas que aderiram</td>';
			$stTabela .= '		<td>Qtd. Turmas Censo</td>';
			$stTabela .= '		<td>Qtd. Turmas Adesão 2009</td>';
			$stTabela .= '		<td>Qtd. Alunos Censo</td>';
			$stTabela .= '		<td>Qtd. Alunos Adesão 2009</td>';
			$stTabela .= '		<td>Qtd. Professores Censo</td>';
			$stTabela .= '		<td>Qtd. Professores Adesão 2009</td>';
			$stTabela .= '	</tr>';
			
			$stTabela .= '	<tr style="background: #f7f7f7; text-align: center;" >';
			$stTabela .= '		<td>'.number_format( $arDados["totalescolas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["totalescolasmultisseriadas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["escolasaderiram"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdtotalturmas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdescolaturmas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdtotalalunos"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdescolaalunos"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdtotalprofessores"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdescolaprofessores"], 0, "", "." ).'</td>';
			$stTabela .= '	</tr>';
			$corFilho = '#f7f7f7';
			
			$stTabela .= '</table>';
		
		}
		return $stTabela;		
	}
	
	public function montarTabelaResumoEscola( $arDados, $tipo ){
		
		$stTabela  = "";
		if( count( $arDados ) ){
			$stTabela .= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">';
			$stTabela .= '	<tr style="background: #e3e3e3; text-align: center;" >';
			$stTabela .= '		<td>Escolas Multisseriada</td>';
			$stTabela .= '		<td>Qtd. Turmas Multisseriadas Censo</td>';
			$stTabela .= '		<td>Qtd. Turmas Adesão 2009</td>';
			$stTabela .= '		<td>Qtd. Alunos Censo</td>';
			$stTabela .= '		<td>Qtd. Alunos Adesão 2009</td>';
			$stTabela .= '	</tr>';
			
			$stTabela .= '	<tr style="background: #f7f7f7; text-align: center;" >';
			$stTabela .= '		<td>'.$arDados["multisseriada"].'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdtotalturmas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdescolaturmas"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdtotalalunos"], 0, "", "." ).'</td>';
			$stTabela .= '		<td>'.number_format( $arDados["qtdescolaalunos"], 0, "", "." ).'</td>';
			$stTabela .= '	</tr>';
			$corFilho = '#f7f7f7';
			
			$stTabela .= '</table>';
		
		}
		return $stTabela;		
	}
	
	public function montarTabelaEstados( $arDados, $tipo, $ano = 2011 ){

		$stTabela  = "";
		if( count( $arDados ) ){
			$stTabela .= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">';
			$stTabela .= '	<tr style="background: #e3e3e3; text-align: center;" >';
			$stTabela .= '		<td>Estado</td>';
			$stTabela .= '	</tr>';
			$corFilho = '#f7f7f7';
			
			foreach( $arDados as $arEstado ){
										
				$count = $this->recuperarCountMunicipiosPorEstuf( $arEstado["estuf"], $tipo, $ano );
				$count2 += $count; 
				$corFilho = $corFilho == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7';
				$stTabela .= '	<tr class="filho1" style="background: '.$corFilho.'" >';			
				$stTabela .= '		<td>
										<img src="../imagens/mais.gif" name="img_estados_'.$tipo.'[]" id="img_estados_'.$tipo.'_'.$arEstado["estuf"].'" 
											onclick="toggleDiv( this.id, \'linha_estados_'.$tipo.'_'.$arEstado["estuf"].'\', \'div_estados_'.$tipo.'_'.$arEstado["estuf"].'\', \''.$tipo.'\', \''.$arEstado["estuf"].'\', 2 );"/>
										&nbsp;&nbsp;&nbsp;&nbsp;<span onclick="abrirResumo( \'resumo_estados_'.$tipo.'_'.$arEstado["estuf"].'\', \''.$tipo.'\', \''.$arEstado["estuf"].'\', 2 );">'.$arEstado["estuf"]." - ".$arEstado["estdescricao"].' ( '.$count.' ) </span>
										<div id="resumo_estados_'.$tipo.'_'.$arEstado["estuf"].'" class="resumo" style="display: none"></div>
									</td>';			
				$stTabela .= '	</tr>';		
				$stTabela .= '	<tr class="title" style="display: none" id="linha_estados_'.$tipo.'_'.$arEstado["estuf"].'">
									<td>
										<div id="div_estados_'.$tipo.'_'.$arEstado["estuf"].'"></div>
									</td>
								</tr>';
			}
			$stTabela .= '</table>';
		
		}
		else{
			$stTabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">
							<tr>
								<td>Não foram encontrados dados!</td>
							</tr>
						</table>';
		}
		return $stTabela;		
	}
	
	public function montarTabelaMunicipios( $arDados, $tipo, $ano = '2011' ){
		
		$stTabela  = "";
		if( count( $arDados ) ){
			$stTabela .= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">';
			$stTabela .= '	<tr style="background: #e3e3e3; text-align: center;" >';
			$stTabela .= '		<td>Município</td>';
			$stTabela .= '	</tr>';
			$corFilho = '#f7f7f7';
			
//			ver($arDados, $tipo, $ano, d);
			
			foreach( $arDados as $arMunicipio ){
								
				$count = $this->recuperarCountEscolasPorMuncod( $arMunicipio["muncod"], $tipo, $ano );						
				
				$corFilho = $corFilho == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7';
				$stTabela .= '	<tr class="filho1" style="background: '.$corFilho.'" >';			
				$stTabela .= '		<td>
										<img src="../imagens/mais.gif" name="img_municipios_'.$tipo.'[]" id="img_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'" 
											onclick="toggleDiv( this.id, \'linha_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'\', \'div_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'\', \''.$tipo.'\', '.$arMunicipio["muncod"].', 3 );"/>
										&nbsp;&nbsp;&nbsp;&nbsp;<span onclick="abrirResumo( \'resumo_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'\', \''.$tipo.'\', \''.$arMunicipio["muncod"].'\', 3 );" >'.$arMunicipio["estuf"]." - ".$arMunicipio["mundescricao"].' ( '.$count.' )</span>
										<div id="resumo_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'" class="resumo" style="display: none"></div>
									</td>';			
				$stTabela .= '	</tr>';		
				$stTabela .= '	<tr class="title" style="display: none" id="linha_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'">
									<td>
										<div id="div_municipios_'.$tipo.'_'.$arMunicipio["muncod"].'"></div>
									</td>
								</tr>';
			}
			$stTabela .= '</table>';
		}
		else{
			$stTabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">
							<tr>
								<td>Não foram encontrados dados!</td>
							</tr>
						</table>';
		}
		return $stTabela;
	}
	
	public function montarTabelaEscolas( $arDados, $tipo, $ano = '2011' ){
		
		$stTabela  = "";
		if( count( $arDados ) ){
			$stTabela .= '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">';
			$stTabela .= '	<tr style="background: #e3e3e3; text-align: center;" >';
			$stTabela .= '		<td>Escolas</td>';
			$stTabela .= '	</tr>';
			$corFilho = '#f7f7f7';
			
			foreach( $arDados as $arEscola ){
				
				//$icone = $arEscola["entdmultiseriada"] ? '<img src="../imagens/multisseriada.png" />' : "";
				$icone = $arEscola["esasituacaoadesao"] == 3 ? '<img src="../imagens/multisseriada.png" />' : "";
				$corFilho = $corFilho == '#f7f7f7' ? '#f5f5f5' : '#f7f7f7';
				$stTabela .= '	<tr class="filho1" style="background: '.$corFilho.'" >';			
				$stTabela .= '		<td>
										<img src="../imagens/seta_filho.gif" />
										&nbsp;&nbsp;&nbsp;&nbsp;<span onclick="abrirResumo( \'resumo_escolas_'.$tipo.'_'.$arEscola["entid"].'\', \''.$tipo.'\', \''.$arEscola["entid"].'\', 4 );">'.$arEscola["entcodent"]." - ".$arEscola["entnome"]."   ".$icone.'</span>
										<div id="resumo_escolas_'.$tipo.'_'.$arEscola["entid"].'" class="resumo" style="display: none"></div>										
									</td>';			
				$stTabela .= '	</tr>';		
				$stTabela .= '	<tr class="title" style="display: none" id="linha_escolas_'.$tipo.'_'.$arEscola["entid"].'">
									<td>
										<div id="div_escolas_'.$tipo.'_'.$arEscola["entid"].'"></div>
									</td>
								</tr>';
			}
			$stTabela .= '</table>';
		
		}
		else{
			$stTabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem tabela">
							<tr>
								<td>Não foram encontrados dados!</td>
							</tr>
						</table>';
		}
		return $stTabela;		
	}
	
	public function recuperarCountEscolasPorMuncod( $muncod, $tipo, $ano = '2011' ){
		
		if( $tipo == 'geral' ){
			$sql = "select count(1) as totalEscolas
					from entidade.entidade e
						inner join entidade.funcaoentidade ef on e.entid = ef.entid 
						inner join entidade.endereco ende on e.entid = ende.entid 
					where funid = 3 and entstatus = 'A'
					and ende.muncod = '$muncod'";
		}
		else{
			$sql = "select count( distinct eae.entid )
					from cte.escolaativa ea
						inner join cte.escolaativaescolas eae on eae.esaid = ea.esaid
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid and iu.muncod = '$muncod'
						where ea.esaano = '{$ano}'";
		}
		
		return  $this->pegaUm( $sql );			
	}
	
	public function recuperarCountMunicipiosPorEstuf( $estuf, $tipo, $ano = '2011' ){
		
		$esdid = "";
		switch( $tipo ){
			case( 'geral' ):
				$sql = "select count( distinct iu.muncod )
						from cte.instrumentounidade iu
						where iu.mun_estuf = '$estuf'
						and iu.itrid = 2";
				break;
			case( 'aderiram' ):
				$sql = "select count( distinct ea.inuid )
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						where iu.mun_estuf = '$estuf'
						and ea.esaano = '{$ano}'
						and ea.esasituacaoadesao = 1
						and iu.itrid = 2";
				break;
			case( 'naoaderiram' ):
				$sql = "select count( distinct ea.inuid )
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						where iu.mun_estuf = '$estuf'
						and ea.esaano = '{$ano}'
						and ea.esasituacaoadesao = 2
						and iu.itrid = 2";
				break;
			case( 'classesmultiseriadas' ):
				$sql = "select count( distinct ea.inuid )
						from cte.escolaativa ea
							inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						where iu.mun_estuf = '$estuf'
						and ea.esaano = '{$ano}'
						and ea.esasituacaoadesao = 3
						and iu.itrid = 2";
				break;
			case( 'naomanifestados' ):	
				$sql = "select distinct count( iu.inuid )							
						from cte.instrumentounidade iu
							inner join territorios.estado e on e.estuf = iu.mun_estuf
							inner join territorios.municipio m on m.muncod = iu.muncod				
							left join cte.escolaativa ea on ea.inuid = iu.inuid and ea.esaano = '{$ano}'
						where iu.mun_estuf = '{$estuf}'
						and ea.esaid is null
						and iu.itrid = 2";
				break;	
			case( 'em_preenchimento' ): $esdid = 47; break;
			case( 'em_analise' 		 ): $esdid = 48; break;
			case( 'em_revisao' 		 ): $esdid = 50; break;
			case( 'em_correcao' 	 ): $esdid = 51; break;
			case( 'analisado'  		 ): $esdid = 49; break;
		}
		
		if( $esdid ){
			
			$sql = "select count( distinct iu.muncod )
					from cte.escolaativa ea
						inner join cte.instrumentounidade iu on iu.inuid = ea.inuid
						left join workflow.documento d on d.docid = ea.docid
					where coalesce( d.esdid, 47 ) = $esdid
					and iu.mun_estuf = '$estuf'
					and esasituacaoadesao in (1)
					and ea.esaano = '{$ano}'
					and iu.itrid = 2";
		}
		
		return  $this->pegaUm( $sql );
		
	}
	
}