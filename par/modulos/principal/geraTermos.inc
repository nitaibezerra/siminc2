<?php
// GERO EX
set_time_limit(0);
ini_set("memory_limit", "500000M");

global $db;

$sql = "select distinct
			dp.dopid, dp.mdoid, dp.dopvalortermo, dp.prpid,
			dp.dopnumerodocumento,
			MAX(substr(dp.dopdatafimvigencia, 4, 4) || '-' || substr(dp.dopdatafimvigencia, 0, 3))
		from 
			par.vm_documentopar_ativos dp
		INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid 
		INNER JOIN par.processopar prp ON prp.prpid = dp.prpid
		INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
		LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
		where
			1 = 1
			AND dp.prpid is not null
			AND dp.mdoid IN (20)
			AND dp.dopdatainiciovigencia IS NOT NULL
			AND dp.dopdatafimvigencia IS NOT NULL
			AND LENGTH( dopdatafimvigencia ) = 7
			AND dp.dopnumerodocumento IS NOT NULL
			AND (substr(dp.dopdatafimvigencia, 4, 4) || '-' || substr(dp.dopdatafimvigencia, 0, 3)) < '2014-08'
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado2 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado3 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado4 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado5 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado6 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado7 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado8 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado9 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado10 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado11 )
			AND dp.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado12 )
			AND dp.dopid not in (select dopidoriginal from par.reprogramacao where repstatus = 'A')
			AND dp.dopid not in ( 37081  )
group by
			dp.dopid, dp.mdoid, dp.dopvalortermo, dp.prpid, dp.dopnumerodocumento";

$dados = $db->carregar($sql);

$sqlPerdido = "";
$sqlGerado = "";

if( $dados[0] ){
	foreach( $dados as $dado ){
	
		if( $dado['mdoid'] == 19 || $dado['mdoid'] == 42 ){	// "PAR_Termo de Compromisso_Estados" ou "PAR_Termo de Compromisso_Estados_EX"
			
			$sql = "SELECT
					  prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
					  mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
					  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
					  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
					  dopreformulacao, dopidpai, dopdatainiciovigencia
					FROM par.documentopar WHERE dopid = {$dado['dopid']} and dopstatus = 'A'";
				  
			$arrDadosDoc = $db->pegaLinha( $sql );
			$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
			
			$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = 42";
			$arrModelo = $db->pegaLinha($sql);
			
			$subacoes = array();
			
			$sql = "SELECT DISTINCT sbdid as chk FROM par.termocomposicao tc INNER JOIN par.documentopar dp ON dp.dopnumerodocumento = tc.dopid WHERE dp.dopid = ".$dado['dopid'];
			$subacoes = $db->carregarColuna($sql);
			
			if( !$subacoes[0] ){
				$sql = "SELECT 
							sd.sbdid 
						FROM 
							par.processopar prp
						INNER JOIN par.processoparcomposicao ppc on ppc.prpid = prp.prpid
						INNER JOIN par.documentopar dp on dp.prpid = prp.prpid
						INNER JOIN par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						WHERE
							dp.dopid = ".$dado['dopid'];
				
				$subacoes = $db->carregarColuna($sql);
				
				$sqlSub = "";
				if( is_array($subacoes) ){
					foreach( $subacoes as $sbdid ){
						$sqlSub .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dado['dopid'].",".$sbdid."); ";
					}
				}
				if($sqlSub){
					$db->executar($sqlSub);
				}
			}
			
			$mdoconteudo = $arrModelo['mdoconteudo'];
			if ( $dado['mdoid'] == 19 ){
				$tpdcod = 102; //$arrModelo['tpdcod'];
			} else {
				$tpdcod = 21;
			}
			
			$post = array('mdoid' => 42, 'tpdcod' => $tpdcod, 'dopid' => $dado['dopid'], 'chk' => $subacoes);
			
			$_SESSION['par']['cronogramaFinal'] = '08/2014';
			$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];
			
			$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['prpid'], $post);
			/*
			if( number_format($dado['dopvalortermo'], 2, ',', '.') != number_format($_SESSION['par']['totalVLR'], 2, ',', '.') ){
				$sqlPerdido = "INSERT INTO par.documentoparvalordivergente (dopid, dopvalortermoantigo, dopvalortermonovo) VALUES (".$dado['dopid'].",".$dado['dopvalortermo'].",".$_SESSION['par']['totalVLR']."); ";
				$db->executar($sqlPerdido);
			} else {
				*/
				if ( $dado['mdoid'] == 19 ){
					$sqlGerado = "INSERT INTO par.documentoparexgerado3 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} else {
					$sqlGerado = "INSERT INTO par.documentoparexgerado4 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				}
				$db->executar($sqlGerado);
				
				unset($vigencia);
				if(strlen($arrDadosDoc['dopdatainiciovigencia']) == 7){
					$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 0, 2);
					$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 4);
					$vigencia = "01/".$mes."/".$ano;
				} else {
					$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 2);
					$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 6, 4);
					$vigencia = "01/".$mes."/".$ano;
				}
				
				$arrDadosDoc['mdoid'] = 42;
				$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
				$arrDadosDoc['dopdatainicio'] = $vigencia;
				$arrDadosDoc['dopdatafim'] = '31/08/2014';
				$arrDadosDoc['dopdatafimvigencia'] = '08/2014';
				
				$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);
//			}
			
			if( is_array($subacoes) && $subacoes[0] ){
				$sqlS = "";
				foreach($subacoes as $sub){
					$sqlS .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dopid.", ".$sub."); ";
				}
				$db->executar($sqlS);
			
				$tipoEscola = $db->pegaUm("SELECT count(sbacronograma) FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE s.sbacronograma = 2 AND sd.sbdid IN (".implode(",", $subacoes).")");
				if( $tipoEscola > 0 ){
					$relatorio = $db->carregar("SELECT * FROM (
													SELECT
														CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
														CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as entidade,
														ent.entnome as escola,
														ent.entcodent as codinep,
														(par.retornacodigosubacao(sd.sbaid)) as subacao,
														pic.picdescricao as item,
														CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
															THEN -- escolas sem itens
																sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
															ELSE -- escolas com itens
																CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
																	sum(ssi.seiqtdtecnico)
																END
														END as quantidade 
													FROM 
														par.subacaodetalhe sd 
													INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
													INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
													INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
													INNER JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano
													INNER JOIN par.escolas esc on esc.escid = se.escid
													INNER JOIN entidade.entidade ent ON ent.entid = esc.entid
													INNER JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
													WHERE 
														sd.sbdid IN (".implode(",", $subacoes).")
													GROUP BY
														ent.entnome, ent.entcodent, sd.sbaid, pic.picdescricao, s.frmid, s.ptsid, sic.icovalidatecnico, iu.itrid, iu.estuf, mun.estuf, mun.mundescricao
													ORDER BY
														escola, subacao, item
												) foo
												WHERE
													foo.quantidade > 0");
					
					$teste = geraAnexoEscolas($relatorio, $dopid);
				}
			}
			$db->commit();
			
		}elseif( $dado['mdoid'] == 20 || $dado['mdoid'] == 41 || $dado['mdoid'] == 23 || $dado['mdoid'] == 29 || $dado['mdoid'] == 38 || $dado['mdoid'] == 22 ){ // "PAR_Termo de Compromisso_Municipios"
			
			$sql = "SELECT
						 prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
					  mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
					  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
					  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
					  dopreformulacao, dopidpai, dopdatainiciovigencia
					FROM par.documentopar WHERE dopid = {$dado['dopid']} and dopstatus = 'A'";
				  
			$arrDadosDoc = $db->pegaLinha( $sql );
			$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
			
			$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = 41";
			$arrModelo = $db->pegaLinha($sql);
			
			$subacoes = array();
			
			$sql = "SELECT DISTINCT sbdid as chk FROM par.termocomposicao tc INNER JOIN par.documentopar dp ON dp.dopnumerodocumento = tc.dopid WHERE dp.dopid = ".$dado['dopid'];
			$subacoes = $db->carregarColuna($sql);
			
			if( !$subacoes[0] ){
				$sql = "SELECT 
							sd.sbdid 
						FROM 
							par.processopar prp
						INNER JOIN par.processoparcomposicao ppc on ppc.prpid = prp.prpid
						INNER JOIN par.documentopar dp on dp.prpid = prp.prpid
						INNER JOIN par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						WHERE
							dp.dopid = ".$dado['dopid'];
				
				$subacoes = $db->carregarColuna($sql);
				
				$sqlSub = "";
				if( is_array($subacoes) ){
					foreach( $subacoes as $sbdid ){
						$sqlSub .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dado['dopid'].",".$sbdid."); ";
					}
				}
				if($sqlSub){
					$db->executar($sqlSub);
				}
			}
			
			$mdoconteudo = $arrModelo['mdoconteudo'];
			
			$mdoid = 41;
			if ( $dado['mdoid'] == 20 || $dado['mdoid'] == 38 ){
				$tpdcod = 102; //$arrModelo['tpdcod'];
			}elseif ( $dado['mdoid'] == 22 ){
				$tpdcod = 102; //$arrModelo['tpdcod'];
				$mdoid = 65;
			}elseif ( $dado['mdoid'] == 29 ){
				$tpdcod = 102; //$arrModelo['tpdcod'];
				$mdoid = 69;
			} else {
				$tpdcod = 21;
			}
			
			$post = array('mdoid' => $mdoid, 'dopid' => $dado['dopid'], 'tpdcod' => $tpdcod, 'chk' => $subacoes);
			
			$_SESSION['par']['cronogramaFinal'] = '09/2014';
			$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];

			$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['prpid'], $post);
			/*
			if( number_format($dado['dopvalortermo'], 2, ',', '.') != number_format($_SESSION['par']['totalVLR'], 2, ',', '.') ){
				$sqlPerdido = "INSERT INTO par.documentoparvalordivergente (dopid, dopvalortermoantigo, dopvalortermonovo) VALUES (".$dado['dopid'].",".$dado['dopvalortermo'].",".$_SESSION['par']['totalVLR']."); ";
				$db->executar($sqlPerdido);
			} else {
				*/
			
			
				if ( $dado['mdoid'] == 20 ){ // Termo de compromisso comum
//					$sqlGerado = "INSERT INTO par.documentoparexgerado3 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
					$sqlGerado = "INSERT INTO par.documentoparexgerado12 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} elseif ( $dado['mdoid'] == 23 ){ // Termo de compromisso - aditivo
					$sqlGerado = "INSERT INTO par.documentoparexgerado5 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} elseif ( $dado['mdoid'] == 29 ){ // PAR_Termo de Compromisso_Municipios_PROINFÂNCIA_Mob_e_Equip
					//$sqlGerado = "INSERT INTO par.documentoparexgerado6 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
					$sqlGerado = "INSERT INTO par.documentoparexgerado11 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} elseif ( $dado['mdoid'] == 38 ){ // Termo de compromisso comum não visivel
					$sqlGerado = "INSERT INTO par.documentoparexgerado7 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} elseif ( $dado['mdoid'] == 22 ){ // "PAR_Termo de Compromisso_Municipios_Emendas"
					$sqlGerado = "INSERT INTO par.documentoparexgerado8 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				} else { // Termo de compromisso EX
					$sqlGerado = "INSERT INTO par.documentoparexgerado4 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				}
			/*
				$sqlGerado = "INSERT INTO par.documentoparexgerado10 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				*/
				$db->executar($sqlGerado);
				
				unset($vigencia);
				if(strlen($arrDadosDoc['dopdatainiciovigencia']) == 7){
					$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 0, 2);
					$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 4);
					$vigencia = "01/".$mes."/".$ano;
				} else {
					$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 2);
					$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 6, 4);
					$vigencia = "01/".$mes."/".$ano;
				}
				
				$arrDadosDoc['mdoid'] = $mdoid;
				$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
				$arrDadosDoc['dopdatainicio'] = $vigencia;
				$arrDadosDoc['dopdatafim'] = '30/09/2014';
				$arrDadosDoc['dopdatafimvigencia'] = '09/2014';
				$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);
//			}
			$repid = "";
			$sqlR  = "";
			$sqlRU = "";

			$sqlR = "SELECT repid FROM par.reprogramacao WHERE dopidoriginal = ".$dado['dopid'];
			$repid = $db->pegaUm($sqlR);
			
			if( $repid ){
				$sqlRU = "UPDATE par.reprogramacao SET dopidoriginal = ".$dopid." WHERE repid = ".$repid."; 
						  UPDATE par.documentoparreprogramacaosubacao SET dopid = ".$dopid." WHERE repid = ".$repid."; ";
				
				$db->executar( $sqlRU );
			}
			
			if( is_array($subacoes) && $subacoes[0] ){
				$sqlS = "";
				foreach($subacoes as $sub){
					$sqlS .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dopid.", ".$sub."); ";
				}
				$db->executar($sqlS);
				
				$tipoEscola = $db->pegaUm("SELECT count(sbacronograma) FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE s.sbacronograma = 2 AND sd.sbdid IN (".implode(",", $subacoes).")");
				if( $tipoEscola > 0 ){
					$relatorio = $db->carregar("SELECT * FROM (
													SELECT
														CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
														CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as entidade,
														ent.entnome as escola,
														ent.entcodent as codinep,
														(par.retornacodigosubacao(sd.sbaid)) as subacao,
														pic.picdescricao as item,
														CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
															THEN -- escolas sem itens
																sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
															ELSE -- escolas com itens
																CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
																	sum(ssi.seiqtdtecnico)
																END
														END as quantidade 
													FROM 
														par.subacaodetalhe sd 
													INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
													INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
													INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
													INNER JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano
													INNER JOIN par.escolas esc on esc.escid = se.escid
													INNER JOIN entidade.entidade ent ON ent.entid = esc.entid
													INNER JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
													WHERE 
														sd.sbdid IN (".implode(",", $subacoes).")
													GROUP BY
														ent.entnome, ent.entcodent, sd.sbaid, pic.picdescricao, s.frmid, s.ptsid, sic.icovalidatecnico, iu.itrid, iu.estuf, mun.estuf, mun.mundescricao
													ORDER BY
														escola, subacao, item
												) foo
												WHERE
													foo.quantidade > 0");
					
					$teste = geraAnexoEscolas($relatorio, $dopid);
				}
			}
			
			$db->commit();
			
		}
		unset($_SESSION['par']['totalVLR']);
	}
	
	
} else {
	echo "ACABOU";
	return false;
}

echo "FIM:".date("d/m/Y h:i:s");
echo "<script>window.location=window.location;</script>";
?>