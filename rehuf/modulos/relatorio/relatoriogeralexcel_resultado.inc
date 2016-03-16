<?
ob_clean();

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(!$_SESSION['rehuf_var']['filano']) {
	echo "<script>alert('Selecione quais anos deseja visualizar');window.close();</script>";
	exit;
}

if($_SESSION['rehuf_var']['filper']) {
	$sql = "SELECT perid FROM rehuf.periodogrupoitem WHERE perdsc IN('".implode("','",$_SESSION['rehuf_var']['filper'])."') ".(($_REQUEST['filano'])?"AND perano IN('".implode("','",$_SESSION['rehuf_var']['filano'])."')":"")." ".(($_REQUEST['agrupador'])?"AND gitid IN(SELECT gitid FROM rehuf.grupoitem WHERE tabtid IN('".implode("','",$_REQUEST['agrupador'])."'))":"");
	$per = $db->carregar($sql);
	if($per[0]) {
		foreach($per as $p) {
			$filtroperiodo[] = $p['perid'];
		}
	} else {
		$filtroperiodo[] = '0';
	}
}

$filtro = (($_SESSION['rehuf_var']['hospitalentid'][0])?" AND ent.entid IN ('".implode("','",$_SESSION['rehuf_var']['hospitalentid'])."')":"");
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND 
			 			 (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)".$filtro." 
			 			 ORDER BY ena.entsig");
$linexls=0;

/* XLS */
$nomeDoArquivoXls = "SIMEC_REHUF_".date("His");
$xls = new GeraExcel();
$xls->MontaConteudoString($linexls, 0, "Unidade");
$xls->MontaConteudoString($linexls, 1, "Hospital");
$xls->MontaConteudoString($linexls, 2, "UF");
$xls->MontaConteudoString($linexls, 3, "Tabela");
$xls->MontaConteudoString($linexls, 4, "Grupo");
$xls->MontaConteudoString($linexls, 5, "Ano");
$xls->MontaConteudoString($linexls, 6, "Período");
$xls->MontaConteudoString($linexls, 7, "Agp. Linha");
$xls->MontaConteudoString($linexls, 8, "Linha");
$xls->MontaConteudoString($linexls, 9, "Agp. Coluna");
$xls->MontaConteudoString($linexls, 10, "Coluna");
$xls->MontaConteudoString($linexls, 11, "Valor");
$linexls++;

foreach($esuids as $esuid) {
	$_SESSION['rehuf_var']['esuid'] = $esuid['esuid'];
	// Pegando dados da TABELA (pai de toda a estrutura)
	$tabelas = $db->carregar("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid IN('". implode("','",$_REQUEST['agrupador']) ."')");
	// 	Verifica se existe a TABELA
	if($tabelas) {
		foreach($tabelas as $tabela) {
		
		// Se existir o filtro de 'grupo', buscar somente tal grupo
		if($_REQUEST['gitid']) {
			$filgrupo = "AND gitid = '". $_REQUEST['gitid'] ."'";
		}
		// Pegar um registro (ou registro filtrado, ou primeiro registro da ordem)
		$grupoitemloop = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ".$filgrupo." ORDER BY gitordem");
		foreach($grupoitemloop as $grpitm) {
		
			$grupoitem[0] = $grpitm;
			if($grupoitem) {
				$grupoitem = $grupoitem[0];
				unset($colunapa,$coluna,$agrupadorescoluna);
				// Analisando o tipo de coluna (definido em constantes.php)
				switch($grupoitem['tpgidcoluna']) {
					case TPG_CFIXAS_PA: // Colunas fixas referentes por ano
					$colunapa = (array) $db->carregar("SELECT * FROM rehuf.coluna col
													   LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
													   WHERE gitid = '". $grupoitem['gitid'] ."' AND coldsc IN('".implode("','",$_SESSION['rehuf_var']['filano'])."') ORDER BY coldsc");
					break;
					case TPG_CFIXAS_SN: // Colunas fixas sem agrupadores
					$coluna = (array) $db->carregar("SELECT * FROM rehuf.coluna col
													 LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
													 WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY colordem");
					break;
					case TPG_CFIXAS_CN: // Colunas fixas agrupadores agrupadores
					// Carregando os agrupadores de coluna
					$agrupadorescoluna = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = false ORDER BY agpordem");
					// Verifica se o agrupador esta vazio (a função carregar retorna o indice 0 vazio: array(0 =>))
					if($agrupadorescoluna[0]) {
						foreach($agrupadorescoluna as $agpcoluna) {
							// Carregando as colunas de cada agrupador
							$coluna[$agpcoluna['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.coluna col
																			  LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
																			  WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agpcoluna['agpid'] ."' ORDER BY colordem");
							// Caso não tenha coluna em um agrupador, criando coluna indicando 'Sem coluna' 
							if(!$coluna[$agpcoluna['agpid']][0]) {
								$coluna[$agpcoluna['agpid']][0] = array("coldsc" => "Sem coluna");
							}
						}
					} else { // Caso tenha apenas o indice 0, limpar o agrupador
						unset($agrupadorescoluna);
					}
					break;
				}
				// Analisando o tipo de linha (definido em constantes.php)
				unset($linhaDinTx,$linhaDinOp,$linhas,$agrupadoreslinha);
				switch($grupoitem['tpgidlinha']) {
					case TPG_LDINAM_TEXT:
						$linhaDinTx = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' AND esuid = '". $_SESSION['rehuf_var']['esuid'] ."' AND (linano IN('".(($_SESSION['rehuf_var']['filano'])?implode("','",$_SESSION['rehuf_var']['filano']):$tabela['tabanofim'])."') ".(($_REQUEST['ano']<=MUDANCA_ANO1)?"OR linano IS NULL":"").") ORDER BY linordem");
						break;
					case TPG_LDINAM_OPCOES:
						$linhaDinOp = (array) $db->carregar("SELECT * FROM rehuf.linha lin
															 LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid  
															 WHERE gitid = '". $grupoitem['gitid'] ."' AND esuid = '". $_SESSION['rehuf_var']['esuid'] ."' AND (lin.linano IN('".(($_SESSION['rehuf_var']['filano'])?implode("','",$_SESSION['rehuf_var']['filano']):$tabela['tabanofim'])."') ".(($_REQUEST['ano']<=MUDANCA_ANO1)?"OR linano IS NULL":"").") ORDER BY linordem");
						break;
					case TPG_LFIXAS_SN:
						$linhas = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY linordem");
						break;
					case TPG_LFIXAS_CN:
						$agrupadoreslinha = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = true ORDER BY agpordem");
						if($agrupadoreslinha[0]) {
							foreach($agrupadoreslinha as $agplinha) {
								$linhas[$agplinha['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agplinha['agpid'] ."' ORDER BY linordem");
								if(!$linhas[$agplinha['agpid']][0]) {
									$linhas[$agplinha['agpid']][0] = array("lindsc" => "Sem coluna");;
								}
							}
						} else {
							unset($agrupadoreslinha);
						}
						break;
				}
			}
	
			/* Se o tipo de coluna for igual a 'referentes ao ano', não imprime abas.
	 		 * Tratamento especial para o tipo 'colunas fixas referente por ano', onde
	 		 * o controle do exercicio é feito descrição da coluna (coldsc) 
	 		 */
			 if($grupoitem['tpgidcoluna'] != TPG_CFIXAS_PA) {
				$agrupadorescoluna_BK = $agrupadorescoluna;
				$coluna_BK = $coluna;
				$colunapa_BK = $colunapa;
				$linhaDinTx_BK = $linhaDinTx;
				$linhaDinOp_BK = $linhaDinOp;
				$linhas_BK = $linhas;
				$agrupadoreslinha_BK = $agrupadoreslinha;
				
				foreach($_SESSION['rehuf_var']['filano'] as $i) {
					if($i >= $tabela['tabanoini'] && $i <= $tabela['tabanofim']) {
						
						/*
						 * Funcionalidade para filtrar as linhas por ano
						 */
						if($linhaDinTx) {
							foreach($linhaDinTx as $l) {
								if($l['linano'] == $i || !$l['linano']) {
									$ar[] = $l;
								}
							}
							$linhaDinTx = $ar;
							unset($ar);
						}
						
						
						/*
						 * Funcionalidade para filtrar as linhas por ano
						 */
						if($linhaDinOp) {
							foreach($linhaDinOp as $l) {
								if($l['linano'] == $i || !$l['linano']) {
									$ar[] = $l;
								}
							}
							$linhaDinOp = $ar;
							unset($ar);
						}
						
						$_REQUEST['ano'] = $i;
						$fano = "cti.ctiexercicio = '". $i ."' AND ";
						
						
						/*
						 * Código que trata a divisão do ano em períodos nas tabelas por ano
						 */
						$sql = "SELECT perid as codigo, perdsc as descricao, perano FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' AND perano='".$i."' ".(($filtroperiodo)?"AND perid IN('".implode("','",$filtroperiodo)."')":"")." ORDER BY perid";
						$periodogrupoitem = $db->carregar($sql);
						unset($dadosperiodopa,$periodopa);
						if($periodogrupoitem[0]) {
							foreach($periodogrupoitem as $c => $periodo) {
								$periodopa[$i][] = array("codigo"=>$periodo['codigo'],"descricao"=>$periodo['descricao']);
							}
						}
						/*
						 * FIM Código que trata a divisão do ano em períodos nas tabelas por ano
						 */
						
						
						// Carregando as respostas dadas no itens
						$sql = "SELECT cti.* FROM rehuf.grupoitem git 
								LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid 
								LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid 
								LEFT JOIN rehuf.conteudoitem cti ON cti.linid = lin.linid AND cti.colid = col.colid  
								WHERE ". $fano ." git.gitid = '".$grupoitem['gitid']."' AND cti.esuid = '". $_SESSION['rehuf_var']['esuid'] ."'";
						$rsps = (array) $db->carregar($sql);
						unset($obsgit,$rspgit);
						foreach($rsps as $rsp) {
							if($rsp['perid']) {
								$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = $rsp['ctiobs'];
								$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
							} else {
								$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
								$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
							}
							//$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
							//$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
						}
						include "estruturatabelarelatorioxls.php";
						$agrupadorescoluna = $agrupadorescoluna_BK;
						$coluna = $coluna_BK;
						$colunapa = $colunapa_BK;
						$linhaDinTx = $linhaDinTx_BK;
						$linhaDinOp = $linhaDinOp_BK;
						$linhas = $linhas_BK;
						$agrupadoreslinha = $agrupadoreslinha_BK;
					}
				}
				
			} else {
				
				/*
				 * Código que trata a divisão do ano em períodos nas tabelas por ano
				 */
				$sql = "SELECT perid as codigo, perdsc as descricao, perano FROM rehuf.periodogrupoitem WHERE gitid='".$grupoitem['gitid']."' ".(($filtroperiodo)?"AND perid IN('".implode("','",$filtroperiodo)."')":"")." ORDER BY perid";
				$periodogrupoitem = $db->carregar($sql);
				unset($dadosperiodopa,$periodopa);
				if($periodogrupoitem[0]) {
					foreach($periodogrupoitem as $c => $periodo) {
						$periodopa[$periodo['perano']][] = array("codigo"=>$periodo['codigo'],"descricao"=>$periodo['descricao']);
					}
				}
				/*
				 * FIM Código que trata a divisão do ano em períodos nas tabelas por ano
				 */
				
				
				// Carregando as respostas dadas no itens
				$sql = "SELECT cti.* FROM rehuf.grupoitem git 
						LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid 
						LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid 
						LEFT JOIN rehuf.conteudoitem cti ON cti.linid = lin.linid AND cti.colid = col.colid  
						WHERE git.gitid = '".$grupoitem['gitid']."' AND cti.esuid = '". $_SESSION['rehuf_var']['esuid'] ."'";
				
				$rsps = (array) $db->carregar($sql);
				
				unset($obsgit,$rspgit);
				
				foreach($rsps as $rsp) {
					if($rsp['perid']) {
						$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = $rsp['ctiobs'];
						$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']][$rsp['perid']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
					} else {
						$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
						$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
					}
					/*
					$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
					$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
					*/
				}

				include "estruturatabelarelatorioxls.php";
			}
		}
		}
	} else {
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			  <tr>
				<td colspan=\"2\">
				<div style=\"float: left;\"><strong>Estrutura não encontrada.</strong></div>
				</td>
		  	</tr>
		  	</table>";
	}
}

$xls->GeraArquivo();
exit;

?>