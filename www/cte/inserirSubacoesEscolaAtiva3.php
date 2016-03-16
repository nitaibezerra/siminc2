<?php

	set_time_limit( 0 );
	
	// Situaчуo da Сrvore na Subaчуo
	define( "CTE_SITUACAO_ARVORE_PRINCIPAL", 1 );
	define( "CTE_SITUACAO_ARVORE_ESCOLA_ATIVA", 2 );
	define( "CTE_SITUACAO_ARVORE_AMBAS", 3 );
	define( "CTE_SITUACAO_ARVORE_PRO_LETRAMENTO", 4 );
	define( "CTE_SITUACAO_ARVORE_PRINCIPAL_E_PRO_LETRAMENTO", 5 );	
	
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";	
	include_once( APPRAIZ. "includes/classes/Modelo.class.inc" );
	include_once( APPRAIZ. "cte/classes/Criterio.class.inc" );
	include_once( APPRAIZ. "cte/classes/Pontuacao.class.inc" );
	include_once( APPRAIZ. "cte/classes/AcaoIndicador.class.inc" );
	include_once( APPRAIZ. "cte/classes/SubacaoIndicador.class.inc" );
	
	
	$db = new cls_banco();	

	$sql = "select distinct p.inuid, d.dimcod, ad.ardcod, i.indcod, pa.ppaid as acaoguia, pa.ppadsc, 
				ps.*, p.ptoid,
				d.itrid, d.dimdsc, d.dimid, ad.ardid, ad.arddsc,
				i.indid, i.inddsc
			from cte.proposicaosubacao ps
				inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
				inner join cte.criterio c on c.crtid = pa.crtid
				inner join cte.pontuacao p on p.crtid = c.crtid 
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				inner join cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			where d.itrid = 2
			and ps.prgid = 549
			and p.inuid in (
					select inuid 
					from cte.instrumentounidade 
					where usucpfescolaativa is not null
					and inusituacaoadesao = 1
			)
			and p.ptostatus = 'A'
			and ps.ppsid in ( 278, 449, 448, 1016, 1023, 1024, 272, 1014, 1015, 1017, 1026, 1025, 818, 467, 1037, 1039, 1041, 89, 88, 1038, 1040, 1042, 273, 410, 411, 1018, 1027, 1028, 413, 412, 1019, 1029, 1030, 274, 275, 414, 415, 1020, 1031, 1032, 417, 416, 1021, 1033, 1034, 276, 418, 419, 1022, 1035, 1036, 277, 1048, 1044, 1045, 1046, 1047 )
			-- and p.inuid in ( 5136, 4379, 3003, 4828, 5175, 3543, 4756, 4579, 5440, 1710, 2312, 4483, 4991, 1139 )
			and iu.mun_estuf in( 'GO', 'MA', 'MS', 'MT' )
			-- DF, MG, PA, PB, PE, PI, PR, RJ, RN, RO, RR, RS ,SC, SE, SP, TO
			order by d.dimcod, ad.ardcod, i.indcod, ps.ppsordem
			";
	
		$resultado = $db->carregar( $sql );
		$coProposicaoSubacao = $resultado ? $resultado : array();
		
		$acilocalizador = 'M';
		foreach( $coProposicaoSubacao as $arProposicaoSubacao ){
			
			$obAcaoIndicador = new AcaoIndicador();

			$arAcao = $obAcaoIndicador->recuperarAcoesPorPpaid( $arProposicaoSubacao["acaoguia"] ,$arProposicaoSubacao["inuid"] );
			if(!is_array($arAcao)){
				$arAcao = array();
			}

			if( count( $arAcao ) == 0 ){
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
			$arSubacao = $obSubacaoIndicador->recuperarSubacoesPorPpsid( $arProposicaoSubacao["ppsid"], $arProposicaoSubacao["inuid"] );
			
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
				$obSubacaoIndicador->sbasituacaoarvore = 2;
				$obSubacaoIndicador->salvar();
			}
			$obSubacaoIndicador->commit();
			
		}
		die();
	
	

?>