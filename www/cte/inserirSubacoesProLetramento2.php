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
			and p.inuid in (
					select inuid 
					from cte.instrumentounidade 
					where usucpfproletramento is not null
			)
			and p.ptostatus = 'A'
			and ps.ppsid in ( 1057, 1058, 1059, 1060, 1061, 1049, 1051, 1053, 1055, 1056, 904, 666, 1052, 1050, 1054 )
			-- and p.inuid in ( 4765, 5588 )
			and iu.mun_estuf not in( 'PI', 'PR', 'RJ', 'RN', 'RO', 'RR', 'RS' ,'SC', 'SE', 'SP', 'TO', 'MG' )
			-- 'AC', 'AL', 'AM', 'AP', 'BA' , 'CE', 'DF', 'ES', 'GO', 'MA' ,'MG', 'MS', 'MT', 'PA', 'PB', 'PE', 'PI', 'PR', 'RJ', 'RN', 'RO', 'RR', 'RS' ,'SC', 'SE', 'SP', 'TO'
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
		die();
	
	

?>