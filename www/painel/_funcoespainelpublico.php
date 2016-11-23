<?php
if($_SESSION['baselogin'] == "simec_espelho_producao" || $_SESSION['baselogin'] == "simec_desenvolvimento"){
    $dbPainelPublico = "dbname=
                        hostaddr=
                        user=
                        password=
                        port=";
}else{
	$dbPainelPublico = "dbname=
						hostaddr=
						user=
						password=
						port=5432";
}

function getDadosIndicador($indid, $publico=null) {
	global $db, $dbPainelPublico;
	
	if(!$publico){
		$sql = "SELECT 
				ind.indid,
				unm.unmdesc,
				tip.tpidsc,
				sec.secdsc as secretaria,
				secgestora.secdsc as secdsc,
				aca.acadsc,
				per.perdsc,
				per2.perdsc as perdata,
				est.estdsc,
				indnome, indobjetivo, indformula, indtermos, indfontetermo, 
				ume.umedesc, reg.regdescricao, exo.exodsc, indobservacao,coldsc, 
				case ind.indescala when 't' then 'Sim' when 'f' then 'Não' end as escala, 
				case ind.indcumulativo when 'S' then 'Sim' when 'N' then 'Não' when 'A' then 'Anual' end as qndtacumulada, 
				case ind.indqtdevalor when 't' then 'Sim' when 'f' then 'Não' end as valormonetario, 
				case ind.indcumulativovalor when 'S' then 'Sim' when 'N' then 'Não' when 'A' then 'Anual' end as valoracumulado, 
				case ind.indpublicado when 't' then 'Sim' when 'f' then 'Não' end as publicado, 
				case ind.indpublico when 't' then 'Sim' when 'f' then 'Não' end as publico
		FROM painel.indicador ind
		INNER JOIN painel.unidademedicao unm ON unm.unmid = ind.unmid
		inner join painel.tipoindicador tip ON tip.tpiid = ind.tpiid
		inner join painel.secretaria sec on ind.secid = sec.secid
		inner join painel.secretaria secgestora on ind.secidgestora = secgestora.secid
		inner join painel.acao aca on ind.acaid = aca.acaid
		inner join painel.periodicidade per on ind.perid = per.perid
		inner join painel.periodicidade per2 on ind.peridatual = per2.perid
		inner join painel.estilo est on est.estid = ind.estid
		inner join painel.unidademeta ume on ume.umeid = ind.umeid
		inner join painel.regionalizacao reg on reg.regid = ind.regid
		inner join painel.eixo exo on exo.exoid = ind.exoid
		inner join painel.coleta col on col.colid = ind.colid
		where ind.indid = $indid";
	}else{
		$sql = "
                SELECT 
					ind.indid,
					unm.unmdesc,
					tip.tpidsc,
					sec.secdsc as secretaria,
					secgestora.secdsc as secdsc,
					aca.acadsc,
					per.perdsc,
					per2.perdsc as perdata,
					est.estdsc,
					indnome, indobjetivo, indformula, indtermos, indfontetermo, 
					ume.umedesc, reg.regdescricao, exo.exodsc, indobservacao,coldsc, 
					(case ind.indescala when 't' then 'Sim' when 'f' then 'Não' end)::varchar(3) as escala, 
					(case ind.indcumulativo when 'S' then 'Sim' when 'N' then 'Não' when 'A' then 'Anual' end)::varchar(3) as qndtacumulada,
					(case ind.indqtdevalor when 't' then 'Sim' when 'f' then 'Não' end)::varchar(3) as valormonetario, 
					(case ind.indcumulativovalor when 'S' then 'Sim' when 'N' then 'Não' when 'A' then 'Anual' end)::varchar(3) as valoracumulado,
					(case ind.indpublicado when 't' then 'Sim' when 'f' then 'Não' end)::varchar(3) as publicado, 
					(case ind.indpublico when 't' then 'Sim' when 'f' then 'Não' end)::varchar(3) as publico
				FROM painel.indicador ind
				INNER JOIN painel.unidademedicao unm ON unm.unmid = ind.unmid
				inner join painel.tipoindicador tip ON tip.tpiid = ind.tpiid
				inner join painel.secretaria sec on ind.secid = sec.secid
				inner join painel.secretaria secgestora on ind.secidgestora = secgestora.secid
				inner join painel.acao aca on ind.acaid = aca.acaid
				inner join painel.periodicidade per on ind.perid = per.perid
				inner join painel.periodicidade per2 on ind.peridatual = per2.perid
				inner join painel.estilo est on est.estid = ind.estid
				inner join painel.unidademeta ume on ume.umeid = ind.umeid
				inner join painel.regionalizacao reg on reg.regid = ind.regid
				inner join painel.eixo exo on exo.exoid = ind.exoid
				inner join painel.coleta col on col.colid = ind.colid
				where
                    ind.indid = $indid";
	}
	return $db->pegaLinha($sql);
}


function getDetalhesIndicador($indid, $publico=null) {
	global $db, $dbPainelPublico;
	
	$sql = "select 
				tdidsc, tdistatus, tdiordem, tdinumero, tidid, tiddsc, tidstatus
			from
				painel.detalhetipoindicador det
			inner join
				painel.detalhetipodadosindicador tip ON det.tdiid = tip.tdiid
			where
				indid = $indid
			order by
				det.tdiid,
                tip.tidid";
//	if($publico){
//		$sql = "SELECT * FROM dblink ('".$dbPainelPublico."','".$sql."') AS rs (tdidsc varchar(255), tdistatus varchar(1), tdiordem integer, tdinumero integer, tidid integer, tiddsc varchar(255), tidstatus varchar(1))";
//	}
	$arrDados = $db->carregar($sql);
	
	if($arrDados):
		foreach($arrDados as $dado):
			$arrD[$dado['tdiid']]['tdidsc']    = $dado['tdidsc'];
			$arrD[$dado['tdiid']]['tdistatus'] = $dado['tdistatus'];
			$arrD[$dado['tdiid']]['tdiordem']  = $dado['tdiordem'];
			$arrD[$dado['tdiid']]['tdinumero'] = $dado['tdinumero'];
			$arrD[$dado['tdiid']]['tipo'][]    = array("tidid" => $dado['tidid'], "tiddsc" => $dado['tiddsc'], "tidstatus" => $dado['tidstatus']);
		endforeach;
		return $arrD;
	else:
		return false;
	endif;
}

function dadosSincronizacao() {
	global $db, $dbPainelPublico;
	
	$sql = "SELECT indid FROM painel.indicador WHERE indid = ".$_SESSION['indid'];
//	$sql = "SELECT * FROM dblink ('".$dbPainelPublico."','".$sql."') AS rs (indid integer)";
	$existeIndPP = $db->pegaUm($sql);
	
	// pegando o formato do indicador
	$formatoinput = pegarFormatoInput();
	if($formatoinput['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $formatoinput['unmid'] == UNIDADEMEDICAO_RAZAO){
		$qtde = "'<center><span style=\"color:#990000\" >N/A</span></center>'";
	}else{
		$qtde = "'<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."') || '</div>' as qtde 
				   ".(($formatoinput['campovalor'])?", '<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."') || '</div>' as valor":"");
	}

	$sql = "SELECT '<center><img src=../imagens/excel.gif title=\'Exportar CSV\' style=cursor:pointer; onclick=exportarsehcsv('|| seh.sehid ||',\'nao\');> ".(($existeIndPP)?"<img src=\"/imagens/reject.png\" border=0 title=\"Sincronizar\" style=\"cursor:pointer;\" onclick=\"sincronizarPainelPublico('||sehid||');\">":"")."</center>' as acoes,
				   to_char(seh.sehdtcoleta,'DD/MM/YYYY') as data, 
				   dpe.dpedsc,
				   $qtde
			FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
			ORDER BY dpedatainicio";
				   
	$cabecalho = array("Ações", "Data de Coleta","Referência", $formatoinput['label']);	
	if($formatoinput['campovalor'])$cabecalho[] = $formatoinput['campovalor']['label'];
	
	echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
		  <tr>
		  <td class=SubtituloCentro>Painel de Controle</td>
		  </tr>
		  </table>";
	
	$db->monta_lista($sql,$cabecalho,1000,5,'N','center',$par2);
	
	echo "<br>";
	echo "<br>";
	
	if($existeIndPP) {
		if($formatoinput['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $formatoinput['unmid'] == UNIDADEMEDICAO_RAZAO){
			$qtde = "'<center><span style=\"color:#990000\" >N/A</span></center>' as qtde";
		}else{
			$qtde = "'<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."') || '</div>' as qtde
					   ".(($formatoinput['campovalor'])?", '<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."') || '</div>' as valor":"");
		}
	
		$sql = "SELECT '<center><img src=../imagens/excel.gif style=cursor:pointer; title=''Exportar CSV'' onclick=exportarsehcsv('|| seh.sehid ||',''sim'');> <img border=0 onclick=excluirSerieHistoricaPainelPublico('||sehid||'); style=cursor: pointer; title=Excluir src=/imagens/excluir.gif></center>' as acoes,
					   to_char(seh.sehdtcoleta,'DD/MM/YYYY') as data, 
					   dpe.dpedsc,
					   $qtde
				FROM painel.seriehistorica seh 
				LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				WHERE seh.indid = ".$_SESSION['indid']." AND (sehstatus='A' OR sehstatus='H') 
				ORDER BY dpedatainicio";
//		if($formatoinput['campovalor']){
//			$sql = "SELECT * FROM dblink ('".$dbPainelPublico."','".$sql."') AS rs (acoes varchar, data varchar(10), dpedsc varchar(50), qtde varchar, valor varchar)";
//		}else{
//			$sql = "SELECT * FROM dblink ('".$dbPainelPublico."','".$sql."') AS rs (acoes varchar, data varchar(10), dpedsc varchar(50), qtde varchar)";
//		}
		
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
			  <tr>
			  <td class=SubtituloCentro>Painel Público</td>
			  </tr>
			  </table>";
		
		$db->monta_lista($sql,$cabecalho,1000,5,'N','center',$par2);
		
		$sql = "SELECT seh.sehid
				FROM painel.seriehistorica seh 
				LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
				ORDER BY dpedatainicio";
		
		$sehids = $db->carregar($sql);
		
		if($sehids[0]) {
			foreach($sehids as $seh) {
				$vlr[] = $seh['sehid'];
			}
			echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
			echo "<tr><td align=center><input type=button value='Sincronizar todas séries historicas' onclick=sincronizarTodasSeh('".implode(";",$vlr)."');></td></tr>";
			echo "</table>";
		}
	}
}
					
function sicronizarIndicador() {
	global $db, $dbPainelPublico;
	
	/* configurações */
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	
	sincronizarTabelasPrincipais();

	//CONECTANDO O DBLINK COM O PAINEL
	$conexaoAberta = $db->carregar("SELECT dblink_connect('conexao','".$dbPainelPublico."')");
	
	$indid = $_POST['indid'];
	
	//Dados do SIMEC
	$sql = "select * from painel.indicador where indid = $indid";
	$arrDados = $db->pegaLinha($sql);
	
	$sqlCampos = "	SELECT DISTINCT
						pg_attribute.attname AS coluna,
						pg_attribute.attnotnull as naonulo
					FROM 
						pg_class
					JOIN 
						pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE ''pg_%''
					JOIN 
						pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = ''f''
					JOIN
						pg_type ON pg_type.oid = pg_attribute.atttypid
					JOIN 
						pg_index ON pg_index.indrelid=pg_class.oid
					LEFT JOIN
						pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != ''u'')
					WHERE 
						pg_class.relname = ''indicador''
					AND
						pg_namespace.nspname = ''painel''
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid
					";
	$sql = "SELECT * FROM dblink ('conexao','".$sqlCampos."') AS rs (coluna name, naonulo boolean)";
	$arrCampos = $db->carregar($sql);
	
	if($arrCampos && is_array($arrCampos)):
		foreach($arrCampos as $campo):
			$arrSet[] = $campo['coluna']." = ".($arrDados[$campo['coluna']] && $arrDados[$campo['coluna']] != "" ? "''".$arrDados[$campo['coluna']]."''" : "NULL");
			$arrCamp[] = $campo['coluna'];
			$arrVal[] = ($arrDados[$campo['coluna']] && $arrDados[$campo['coluna']] != "" ? "''".$arrDados[$campo['coluna']]."''" : "NULL");
		endforeach;
	endif;
	
	if($arrSet):
			
		$sqlI = "select indid from painel.indicador where indid = $indid";
		$sqlI = "SELECT * FROM dblink ('conexao','".$sqlI."') AS rs (indid integer)";
		$indid2 = $db->pegaUm($sqlI);
		if($indid2){
			//início - painel.indicador
			$sqlInd = "update
					painel.indicador
				set
				".($arrSet ? implode(",",$arrSet) : "")."
				where
					indid = $indid2";
			//fim - painel.indicador
		}else{
			//início - painel.indicador
			$sqlInd = "insert into
					painel.indicador
					(".($arrCamp ? implode(",",$arrCamp) : "").")
				values
					(".($arrVal ? implode(",",$arrVal) : "").")";
			
			$indid2 = $indid;
			//fim - painel.indicador
		}
		$sql = "SELECT dblink_exec('conexao','".$sqlInd."')";
		$db->executar($sql);
		
		//início - painel.detalhetipoindicador
		$sql = "select 
					*
				from
					painel.detalhetipoindicador
				where
					indid = $indid";
		$arrDetalhes = $db->carregar($sql);
		if($arrDetalhes && is_array($arrDetalhes)):
			foreach($arrDetalhes as $detalhe):
				$sql = "select tdiid from painel.detalhetipoindicador where indid = $indid2 and tdiid = {$detalhe['tdiid']}";
				$sql = "SELECT tdiid FROM dblink ('conexao','".$sql."') AS rs (tdiid integer)";
				$tdiid = $db->pegaUm($sql);
				if($tdiid):
					$sqlUD .= "update painel.detalhetipoindicador set tdistatus = ''{$detalhe['tdistatus']}'', tdiordem = ''{$detalhe['tdiordem']}'', tdidsc = ''{$detalhe['tdidsc']}'', tdinumero = ''{$detalhe['tdinumero']}'' where indid = $indid2 and tdiid = {$detalhe['tdiid']};";
				else:
					$sqlUD .= "insert into painel.detalhetipoindicador (tdiid,indid,tdistatus,tdiordem,tdidsc,tdinumero) values({$detalhe['tdiid']},$indid2,''{$detalhe['tdistatus']}'',''{$detalhe['tdiordem']}'',''{$detalhe['tdidsc']}'',''{$detalhe['tdinumero']}'');";
				endif;
				$arrTdiid[] = $detalhe['tdiid'];
			endforeach;
		endif;
		if($sqlUD){
			$sql = "SELECT dblink_exec('conexao','".$sqlUD."')";
			$db->executar($sql);
		}
		//fim - painel.detalhetipoindicador
		
		//início - painel.detalhetipodadosindicador
		if(is_array($arrTdiid)):
			$sql = "select 
						*
					from
						painel.detalhetipodadosindicador
					where
						tdiid in (".implode(",",$arrTdiid).")";
			$arrTipos = $db->carregar($sql);
			if($arrTipos && is_array($arrTipos)):
				foreach($arrTipos as $tipo):
					$sql = "select tidid from painel.detalhetipodadosindicador where tidid = {$tipo['tidid']} and tdiid = {$tipo['tdiid']}";
					$sql = "SELECT * FROM dblink ('conexao','".$sql."') AS rs (tidid integer)";
					$tidid = $db->pegaUm($sql);
					if($tidid):
						$sqlUT .= "update painel.detalhetipodadosindicador set tiddsc = ''{$tipo['tiddsc']}'', tidstatus = ''{$tipo['tidstatus']}'' where tidid = {$tipo['tidid']} and tdiid = {$tipo['tdiid']};";
					else:
						$sqlUT .= "insert into painel.detalhetipodadosindicador (tidid,tdiid,tiddsc,tidstatus) values({$tipo['tidid']},''{$tipo['tdiid']}'',''{$tipo['tiddsc']}'',''{$tipo['tidstatus']}'');";
					endif;
				endforeach;
			endif;
			if($sqlUT){
				$sql = "SELECT dblink_exec('conexao','".$sqlUT."')";
				$db->executar($sql);
			}
		endif;
		//fim - painel.detalhetipodadosindicador
		
		$db->commit();
	endif;
	
	//DESCONECTANDO O DBLINK DO PAINEL
	$conexaoFechada = $db->carregar("SELECT dblink_disconnect('conexao')");
	
	header("Location: http://painel.mec.gov.br/limparCache.php");
	
}

function sincronizarPainelPublico($dados) {
	global $db, $dbPainelPublico;
	
	$inicioSinc = getmicrotime();
	
	/* configurações */
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	/* FIM configurações */
	
	sincronizarTabelasPrincipais();

	//CONECTANDO O DBLINK COM O PAINEL
	$conexaoAberta = $db->carregar("SELECT dblink_connect('conexao','".$dbPainelPublico."')");
	
	// CARREGANDO DADOS NOVOS
	$sql = "SELECT * FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'";
	$dadosseh = $db->pegaLinha($sql);
	
	$sql = "SELECT * FROM painel.seriehistorica WHERE indid in (select indid from painel.seriehistorica where sehid=".$dados['sehid'].")";
	$serieshistoricas = $db->carregar($sql);
	
	if($serieshistoricas[0]) {
		foreach($serieshistoricas as $sh) {
			$_serieshistoricas_status[$sh['sehid']] = $sh['sehstatus'];
		}
	}
	
	$sql = "SELECT * FROM painel.detalheseriehistorica WHERE sehid='".$dados['sehid']."'";
	$dadosdtseh = $db->carregar($sql);

	$sql = "SELECT * FROM painel.detalhetipoindicador WHERE indid in (select indid from painel.seriehistorica where sehid='".$dados['sehid']."')";
	$detalhetipoindicador = $db->carregar($sql);
	
	if($detalhetipoindicador[0]) {
		foreach($detalhetipoindicador as $dti) {
			$sql = "SELECT * FROM painel.detalhetipodadosindicador WHERE tdiid='".$dti['tdiid']."'";
			$detalhetipodadosindicador[$dti['tdiid']] = $db->carregar($sql);
		}
	}

	// APAGANDO PAINEL PUBLICO
	$arrSerie = $db->pegaLinha("SELECT dpeid, indid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'");
	
	$sql = "DELETE FROM painel.detalheseriehistorica WHERE sehid IN(SELECT sehid FROM painel.seriehistorica WHERE dpeid=".$arrSerie['dpeid']." AND indid=".$arrSerie['indid'].")";
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	
	$sql = "DELETE FROM painel.seriehistorica WHERE sehid IN(SELECT sehid FROM painel.seriehistorica WHERE dpeid=".$arrSerie['dpeid']." AND indid=".$arrSerie['indid'].")";
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	
	// INSERINDO PAINEL PUBLICO
	$sql = "INSERT INTO painel.seriehistorica(
            		sehid, indid, sehvalor, sehstatus, sehqtde, dpeid, sehdtcoleta, regid, sehbloqueado)
    			   VALUES (".(($dadosseh['sehid'])?"''".$dadosseh['sehid']."''":"NULL").", 
    			   		   ".(($dadosseh['indid'])?"''".$dadosseh['indid']."''":"NULL").", 
    			   		   ".(($dadosseh['sehvalor'])?"''".$dadosseh['sehvalor']."''":"NULL").", 
    			   		   ".(($dadosseh['sehstatus'])?"''".$dadosseh['sehstatus']."''":"NULL").", 
    			   		   ".(($dadosseh['sehqtde'])?"''".$dadosseh['sehqtde']."''":"NULL").", 
    			   		   ".(($dadosseh['dpeid'])?"''".$dadosseh['dpeid']."''":"NULL").", 
    			   		   ".(($dadosseh['sehdtcoleta'])?"''".$dadosseh['sehdtcoleta']."''":"NULL").", 
    			   		   ".(($dadosseh['regid'])?"''".$dadosseh['regid']."''":"NULL").", 
    			   		   ".(($dadosseh['sehbloqueado']=="t")?"TRUE":"FALSE").")";
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	
	if($dadosdtseh[0]) {
		foreach($dadosdtseh as $dt) {
			$sql = "INSERT INTO painel.detalheseriehistorica(
		            	dshid, ddiid, sehid, dshvalor, dshcod, dshcodmunicipio, dshuf, 
		            	dshqtde, tidid1, tidid2, iepid, entid, unicod, polid, iecid)
				      VALUES (".(($dt['dshid'])?"''".$dt['dshid']."''":"NULL").", 
				      		  ".(($dt['ddiid'])?"''".$dt['ddiid']."''":"NULL").", 
				      		  ".(($dt['sehid'])?"''".$dt['sehid']."''":"NULL").", 
				      		  ".(($dt['dshvalor'])?"''".$dt['dshvalor']."''":"NULL").", 
				      		  ".(($dt['dshcod'])?"''".$dt['dshcod']."''":"NULL").", 
				      		  ".(($dt['dshcodmunicipio'])?"''".$dt['dshcodmunicipio']."''":"NULL").", 
				      		  ".(($dt['dshuf'])?"''".$dt['dshuf']."''":"NULL").",
				      		  ".(($dt['dshqtde'])?"''".$dt['dshqtde']."''":"NULL").", 
				              ".(($dt['tidid1'])?"''".$dt['tidid1']."''":"NULL").", 
				              ".(($dt['tidid2'])?"''".$dt['tidid2']."''":"NULL").", 
				              ".(($dt['iepid'])?"''".$dt['iepid']."''":"NULL").", 
				              ".(($dt['entid'])?"''".$dt['entid']."''":"NULL").", 
				              ".(($dt['unicod'])?"''".$dt['unicod']."''":"NULL").", 
				              ".(($dt['polid'])?"''".$dt['polid']."''":"NULL").", 
				              ".(($dt['iecid'])?"''".$dt['iecid']."''":"NULL")." 
				              )";

			$sql = "SELECT dblink_exec('conexao','".$sql."')";
			$db->executar($sql);
		}
	}
	
	// atualizando status serie historica
	$sql = "SELECT sehid, sehstatus FROM painel.seriehistorica WHERE indid in (select indid from painel.seriehistorica where sehid=".$dados['sehid'].")";
	$sql = "SELECT sehid FROM dblink ('conexao','".$sql."') AS rs (sehid integer, sehstatus varchar(1))";
	$serieshistoricas = $db->carregar($sql);
	
	if($serieshistoricas[0]) {
		foreach($serieshistoricas as $sh) {
			if($_serieshistoricas_status[$sh['sehid']] != $sh['sehstatus'] && $_serieshistoricas_status[$sh['sehid']] != "") {
				$sql = "UPDATE painel.seriehistorica SET sehstatus=''".$_serieshistoricas_status[$sh['sehid']]."'' WHERE sehid=".$sh['sehid'];
				$sql = "SELECT dblink_exec('conexao','".$sql."')";
				$db->executar($sql);
			}
		}
	}
	
	
	//atualizando o status da última série histórica atualizada
	$sql = "SELECT sehid
			FROM painel.seriehistorica sh
			WHERE sh.indid = ".$_SESSION['indid']."
			AND sh.dpeid = (SELECT dpe.dpeid FROM painel.seriehistorica seh
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						WHERE seh.indid = sh.indid
						ORDER BY dpe.dpeanoref DESC, dpe.dpemesref DESC, dpe.dpeordem DESC
						LIMIT 1)";
	$sql = "SELECT sehid FROM dblink ('conexao','".$sql."') AS rs (sehid integer)";
	$retornaUltimoAtualizado = $db->pegaUm($sql);
	
	$sql = "UPDATE painel.seriehistorica SET sehstatus = ''A'' WHERE sehid = " . $retornaUltimoAtualizado;
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	$db->commit();
	
	//DESCONECTANDO O DBLINK DO PAINEL
	$conexaoFechada = $db->carregar("SELECT dblink_disconnect('conexao')");
	
	echo "Sincronização efetuada com sucesso em ".(getmicrotime() - $inicioSinc)." segundos";	
	
}

function removerSerieHistoricaPainelPublico($dados) {
	global $db, $dbPainelPublico;

	//CONECTANDO O DBLINK COM O PAINEL
	$conexaoAberta = $db->carregar("SELECT dblink_connect('conexao','".$dbPainelPublico."')");
	
	$sql = "delete from painel.detalheseriehistorica where sehid = " . $dados['sehid'];
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	
	$sql = "delete from painel.seriehistorica where sehid = " . $dados['sehid'];
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	
	//atualizando o status da última série histórica atualizada
	$sql = "SELECT sehid
			FROM painel.seriehistorica sh
			WHERE sh.indid = ".$_SESSION['indid']."
			AND sh.dpeid = (SELECT dpe.dpeid FROM painel.seriehistorica seh
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						WHERE seh.indid = sh.indid
						ORDER BY dpe.dpeanoref DESC, dpe.dpemesref DESC, dpe.dpeordem DESC
						LIMIT 1)";
	$sql = "SELECT sehid FROM dblink ('conexao','".$sql."') AS rs (sehid bigint)";
	$retornaUltimoAtualizado = $db->pegaUm($sql);
	
	$sql = "UPDATE painel.seriehistorica SET sehstatus = ''A'' WHERE sehid = " . $retornaUltimoAtualizado;
	$sql = "SELECT dblink_exec('conexao','".$sql."')";
	$db->executar($sql);
	$db->commit();
	
	//DESCONECTANDO O DBLINK DO PAINEL
	$conexaoFechada = $db->carregar("SELECT dblink_disconnect('conexao')");
}

function sincronizarTabelasPrincipais(){
	global $db, $dbPainelPublico;

	//CONECTANDO O DBLINK COM O PAINEL
	$conexaoAberta = $db->carregar("SELECT dblink_connect('conexao','".$dbPainelPublico."')");
	
	//Sincroniza 3 tabelas necessárias (acao,unidademeta,secretaria) do painel público
	/* Tabela painel.acao */
	$sql = "SELECT acaid, acadsc, acastatus FROM painel.acao;";
	$arrAcao = $db->carregar($sql);
	$arrAcao = !$arrAcao ? array() : $arrAcao;
	foreach($arrAcao as $acao){
		$sql = "select acaid from painel.acao where acaid = {$acao['acaid']}";
		$sql = "SELECT acaid FROM dblink ('conexao','".$sql."') AS rs (acaid integer)";
		$acaid = $db->pegaUm($sql);
		if($acaid){
			$sqlIAcao .= "	update 
								painel.acao 
							set 
								acadsc = ''{$acao['acadsc']}'', 
								acastatus = ''{$acao['acastatus']}''
							where 
								acaid = $acaid;";
		}else{
			$sqlIAcao .= "
						insert into painel.acao values (''".implode("'',''",$acao)."'');";	
		}
	}
	$sql = "SELECT dblink_exec('conexao','".$sqlIAcao."')";
	$db->executar($sql);
	
	/* Tabela painel.unidademeta */
	$sql = "SELECT * FROM painel.unidademeta;";
	$arrUMeta = $db->carregar($sql);
	$arrUMeta = !$arrUMeta ? array() : $arrUMeta;
	foreach($arrUMeta as $um){
		$sql = "select umeid from painel.unidademeta where umeid = {$um['umeid']}";
		$sql = "SELECT umeid FROM dblink ('conexao','".$sql."') AS rs (umeid integer)";
		$umeid = $db->pegaUm($sql);
		if($umeid){
			$sqlIUM .= "	update 
								painel.unidademeta 
							set 
								umedesc = ''{$um['umedesc']}'', 
								umestatus = ''{$um['umestatus']}'',
								umecasadecimal = ".($um['umecasadecimal'] ? "''{$um['umecasadecimal']}''" : "null")." 
							where 
								umeid = $umeid;";
		}else{
			$um['umecasadecimal'] = $um['umecasadecimal'] ? "''{$um['umecasadecimal']}''" : "null";
			$um['umedesc'] = $um['umedesc'] ? "''{$um['umedesc']}''" : "null";
			$um['umestatus'] = $um['umestatus'] ? "''{$um['umestatus']}''" : "null";
			$sqlIUM .= "insert into painel.unidademeta values  (".implode(",",$um).");";	
		}
	}
	$sql = "SELECT dblink_exec('conexao','".$sqlIUM."')";
	$db->executar($sql);
	
	/* Tabela painel.scretaria */
	$sql = "SELECT * FROM painel.secretaria;";
	$arrSec = $db->carregar($sql);
	$arrSec = !$arrSec ? array() : $arrSec;
	foreach($arrSec as $sec){
		$sql = "select secid from painel.secretaria where secid = {$sec['secid']}";
		$sql = "SELECT secid FROM dblink ('conexao','".$sql."') AS rs (secid integer)";
		$secid = $db->pegaUm($sql);
		if($secid){
			$sqlISec .= "	update 
								painel.secretaria 
							set 
								secdsc = ''{$sec['secdsc']}'', 
								secstatus = ''{$sec['secstatus']}'',
								entid = ".($sec['entid'] ? "''{$sec['entid']}''" : "null")." 
							where 
								secid = $secid;";
		}else{
			$sec['secdsc'] = $sec['secdsc'] ? "''{$sec['secdsc']}''" : "null";
			$sec['secstatus'] = $sec['secstatus'] ? "''{$sec['secstatus']}''" : "null";
			$sec['entid'] = $sec['entid'] ? "{$sec['entid']}" : "null";
			$sqlISec .= "insert into painel.secretaria values  (".implode(",",$sec).");";	
		}
	}
	$sql = "SELECT dblink_exec('conexao','".$sqlISec."')";
	$db->executar($sql);
	
	//DESCONECTANDO O DBLINK DO PAINEL
	$conexaoFechada = $db->carregar("SELECT dblink_disconnect('conexao')");
}
?>
