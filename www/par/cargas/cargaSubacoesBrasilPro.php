<?php
ini_set("memory_limit","25000M");
set_time_limit(0);
//"/var/www/simec/simec_dev/simec/"

//include_once "config.inc";
include_once "/var/www/simec/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "par/classes/modelo/Acao.class.inc";
include_once APPRAIZ . "par/classes/modelo/Subacao.class.inc";
include_once APPRAIZ . "par/classes/modelo/SubacaoDetalhe.class.inc";
include_once APPRAIZ . "par/classes/modelo/SubacaoItensComposicao.class.inc";


$db = new cls_banco();
// LISTA CARGA COM PPSID E PONTUAÇÃO
$sql = "SELECT d.dimcod, ar.arecod, i.indcod, bp.ppsid, bp.subacao, iu.inuid, p.ptoid, bp.uf
		FROM par.dimensao d
		INNER JOIN par.area ar ON d.dimid  = ar.dimid
		INNER JOIN par.indicador i ON ar.areid = i.areid
		INNER JOIN par.criterio c ON c.indid = i.indid
		INNER JOIN par.pontuacao p ON p.crtid = c.crtid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
		INNER JOIN carga.brasilprounificado bp ON iu.estuf = bp.uf
		INNER JOIN par.propostasubacao ps ON ps.indid = i.indid and ps.ppsid = bp.ppsid
		WHERE 	d.itrid = 3
			 	AND bp.uf in ('PA', 'RN')
				AND ptostatus = 'A'
				-- AND bp.ppsid in ( 1349 )
		GROUP BY d.dimcod, ar.arecod, i.indcod, bp.ppsid, bp.subacao, iu.inuid, p.ptoid,bp.uf
		";

$dados = $db->carregar($sql);

foreach($dados as $dado){
 	$ppsid = $dado['ppsid'];
 	$inuid = $dado['inuid'];
 	$ptoid = $dado['ptoid'];
 	$uf	   = $dado['uf'];
 	$sql = "SELECT count(*) 
 			FROM par.subacao s
			INNER JOIN par.acao a on a.aciid = s.aciid
			INNER JOIN par.pontuacao p on p.ptoid = a.ptoid
			WHERE ppsid = ".$ppsid."
				AND ptostatus = 'A'
				AND inuid = ".$inuid." AND s.sbastatus != 'I'";
 	
 	$temsubacao = $db->pegaUm($sql);

 	if( $temsubacao == false ){
 		// VERIFICA SE TEM AÇÃO
 		$sql = "SELECT a.aciid
 			FROM  par.acao a 
			INNER JOIN par.pontuacao p on p.ptoid = a.ptoid
			INNER JOIN par.propostaacao pa ON pa.crtid = p.crtid
			WHERE 	p.ptoid = ".$ptoid."
				AND inuid = ".$inuid;
 		$acao = $db->pegaUm($sql);

 		if($acao == false){ // Se não tem ação insere
 			$sql = "	SELECT distinct a.ppaid, a.crtid, a.ppadsc
						FROM carga.brasilprounificado bp
						INNER JOIN par.propostasubacao ps ON ps.ppsid = bp.ppsid
						INNER JOIN par.criteriopropostasubacao cps ON ps.ppsid = cps.ppsid
						INNER JOIN par.propostaacao a ON a.crtid = cps.crtid
						INNER JOIN par.criterio c on c.crtid = a.crtid
						INNER JOIN par.pontuacao p on c.crtid = p.crtid
						WHERE 	bp.ppsid = '".$ppsid."'  
								and p.ptoid = ".$ptoid;
 			$dadosAcao = $db->carregar($sql);
 			
 			$oAcao = new Acao();
 			$oAcao->ppaid  	  = $dadosAcao['ppaid'];
			$oAcao->ptoid  	  = $ptoid;
			$oAcao->acistatus = 'A';
			$oAcao->acidsc 	  = $dadosAcao['ppadsc'];
			$aciid 		   	  = $oAcao->salvar();
			$oAcao->commit();
 		}else{
 			$aciid = $acao;
 		}
 		//---------------- INSERE SUBAÇÃO ------------- //
 		
 		//Insere documento
 		$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
							VALUES (62, 451, 'Em Elaboração', now()) returning docid ";
		$docid = $db->pegaUm($sql);
		
		//insere subacao
		$oSubacao = new subacao();
		$sql = "SELECT * FROM par.propostasubacao WHERE ppsid = ".$ppsid;
		$dadosGuiaSubacao = $db->pegaLinha($sql);

		$oSubacao->sbaid 					  = null;
		$oSubacao->sbadsc 					  = $dadosGuiaSubacao['ppsdsc'];
		$oSubacao->sbaordem 				  = $dadosGuiaSubacao['ppsordem'];
		$oSubacao->sbaestrategiaimplementacao = $dadosGuiaSubacao['ppsestrategiaimplementacao'];
		$oSubacao->sbamonitoratecnico 		  = $dadosGuiaSubacao['ppsmonitora'];
		$oSubacao->frmid 					  = $dadosGuiaSubacao['frmid'];
		$oSubacao->prgid 					  = $dadosGuiaSubacao['prgid'];
		$oSubacao->ptsid 					  = $dadosGuiaSubacao['ptsid'];
		$oSubacao->indid 					  = $dadosGuiaSubacao['indid'];
		$oSubacao->foaid 					  = $dadosGuiaSubacao['foaid'];
		$oSubacao->sbastatus				  = 'A';
		$oSubacao->undid 					  = $dadosGuiaSubacao['undid'];
		$oSubacao->docid 					  = $docid;
		$oSubacao->ppsid 					  = $dadosGuiaSubacao['ppsid'];
		$oSubacao->sbacronograma 			  = $dadosGuiaSubacao['ppscronograma'];
		$oSubacao->sbaextraordinaria 		  = NULL;
		$oSubacao->aciid 					  = $aciid;
		
		$sbaid = $oSubacao->salvar();
		$oSubacao->commit();
		
 		//$sql = "select sbaid from par.subacao where ppsid = ".$ppsid ;
		//$sbaid = $db->pegaUm($sql);
		
 		if($sbaid){
			//insere subacaodetalhe
			$oSbd = new SubacaoDetalhe();
			$oSbd->sbdid 				= NULL;	
			$oSbd->sbaid 				= $sbaid;
			$oSbd->sbdparecer 			= NULL;
			$oSbd->sbdparecerdemerito	= NULL;
			$oSbd->sbdquantidade 		= NULL;
			$oSbd->sbdinicio 			= NULL;
			$oSbd->sbdfim 				= NULL;
			$oSbd->sbdplanointerno 		= NULL;
			$oSbd->sbdptres 			= NULL;
			$oSbd->sbddetalhamento		= NULL;
			$oSbd->ssuid 				= NULL;
			$oSbd->sbdanotermino 		= NULL;
			$oSbd->sbdano 				= 2014;
			$oSbd->salvar();
			$oSbd->commit();
		
			//INSERE OS ITENS DE COMPOSIÇÃO
			//ITENS
			$obItem = new SubacaoItensComposicao();
			$sql = "SELECT DISTINCT
						 pic.picid,
						 psi.ppsid,
						 pic.picdescricao,
						 umiid,
						 picdetalhe
					FROM par.propostaitemcomposicao pic
					LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A'
					LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
					LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid AND ptpstatus = 'A'
					LEFT JOIN par.pregaouf p   ON p.ptpid   = ptp.ptpid
					WHERE psi.ppsid = '".$ppsid."'
					AND pic.picstatus = 'A'
					ORDER BY pic.picdescricao";
			$itens = $db->carregar($sql);
		
 			foreach( $itens as $item){
 			
	 			$sql = "SELECT DISTINCT
							dic.dicvalor as valoritem,
							dic.dicid
						FROM
							par.propostaitemcomposicao pic
						LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
						INNER JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
						LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid 
						LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
						WHERE pic.picid = ".$item['picid'];
	 			$dadosItens = $db->pegaLinha($sql);
	 			
 			if(!$dadosItens['dicid'] ){
 				$dadosItens['dicid'] = NULL;
 			}
 			if(!$valorItens){
				$valorItens = 0;
 			}
			$obItem->icoid 			 		= null;
			$obItem->icoano 			 	= 2014;
			$obItem->icodescricao 	 	 	= $item['picdescricao'];
			$obItem->icoquantidade		 	= 0;
			$obItem->icoquantidadetecnico 	= 0;
			$obItem->icovalidatecnico 	 	= NULL; 
			$obItem->icovalortotal		 	= NULL;
			$obItem->icovalor 			 	= $dadosItens['valoritem'];
			$obItem->dicid 			 		= $dadosItens['dicid'];
			$obItem->icostatus	 		 	= "A";
			$obItem->sbaid 			 		= $sbaid;
			$obItem->gicid 			 		= 0;
			$obItem->unddid 			 	= $item['umiid'];
			$obItem->icodetalhe 		 	= $item['picdetalhe'];
			$obItem->usucpf 	 		 	= '';
			$obItem->dtatualizacao 	 		= "'now()'";
			$obItem->picid 			 		= $item['picid'];
			$obItem->salvar();
			$obItem->commit();
		}
		
		// GRUPO DE ITEM
		$sql = "SELECT DISTINCT
			  gic.gicid, 
			  pgi.ppsid, 
			  gic.gicdescricao
		FROM par.grupo_itemcomposicao gic
		LEFT JOIN par.propostagrupoitem pgi ON pgi.gicid = gic.gicid
		WHERE pgi.ppsid = ".$ppsid."
		AND gic.gicstatus = 'A'
		ORDER BY gic.gicdescricao";
		$grupoItens = $db->carregar($sql);
		
		//se tem grupo
		if($grupoItens){
			foreach( $grupoItens as $grupo){
			$sql = "SELECT 
						pg.picid,
						pic.picdescricao,
						pg.pgiqtd,
						umiid,
					 	picdetalhe,
					 	pg.gicid 
					FROM 
						par.propostaitem_grupoitem pg
					INNER JOIN par.propostaitemcomposicao pic ON pic.picid = pg.picid AND pic.picstatus = 'A' 
					WHERE 
						pg.gicid = ".$grupo['gicid']."
					ORDER BY 
						pg.picid";
			$itens = $db->carregar($sql);
				
				foreach( $itens as $item){
		 				$sql = "SELECT DISTINCT
								dic.dicvalor as valoritem,
								dic.dicid
							FROM
								par.propostaitemcomposicao pic
							LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
							LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
							LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid 
							LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
							WHERE pic.picid = ".$item['picid'];
							$dadosItens = $db->pegaLinha($sql);
			 			
			 			if(!$dadosItens['dicid'] ){
			 				$dadosItens['dicid'] = NULL;
			 			}
						if(!$valorItens){
							$valorItens = 0;
			 			}
		 			
			 			$obItem->icoid 			 		= null;
						$obItem->icoano 			 	= 2014;
						$obItem->icodescricao 	 	 	= $item['picdescricao'];
						$obItem->icoquantidade		 	= 0;
						$obItem->icoquantidadetecnico 	= 0;
						$obItem->icovalidatecnico 	 	= NULL; 
						$obItem->icovalortotal		 	= NULL;
						$obItem->icovalor 			 	= $dadosItens['valoritem'];
						$obItem->dicid 			 		= $dadosItens['dicid'];
						$obItem->icostatus	 		 	= "A";
						$obItem->sbaid 			 		= $sbaid;
						$obItem->gicid 			 		= $grupo['gicid'];
						$obItem->unddid 			 	= $item['umiid'];
						$obItem->icodetalhe 		 	= $item['picdetalhe'];
						$obItem->usucpf 	 		 	= '';
						$obItem->dtatualizacao 	 		= "'now()'";
						$obItem->picid 			 		= $item['picid'];
						$obItem->salvar();
						$obItem->commit();	
						
						
						
						//echo "Cadastrou item".$item['picid']."<br>"; 
		 			
				} //itens do grupo
			} 
		} // se tem grupo de item
		
		// INSERE OBRAS
		$sql = "SELECT id FROM carga.brasilprounificado cb
				INNER JOIN par.instrumentounidade iu ON iu.estuf = cb.uf
				WHERE cb.uf = '".$uf."' AND cb.ppsid = '".$ppsid."'";
		$dadosObras = $db->carregar($sql);
		
		foreach($dadosObras as $obrid){
			//dbg($obrid,1);
			$sql = "INSERT INTO par.subacaoobravinculacao (sbaid, sovano, obrid) VALUES ( ".$sbaid.", 2014, ".$obrid['id']." )";
			$db->executar($sql);
			$db->commit();
			//echo "Obra ".$obrid['id']." foi inserida<br>";
		}
 		}
 	} //se não tem subação
 	echo "A subação do Estado do ".$uf." foi inserido com sucesso.<br>";
}
echo "Inserido com sucesso";


?>