<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center; 
}
.stylelinhas {
	background-color: #dcdcdc;
	text-align: right; 
}
.styleconteudo {
	text-align: right;
}
.titulounidade {
	background-color:#CDCDCD;
	text-align:center;
	font-weight: bold;
}
</style>
<?

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(300);
/* FIM configurações */

if(!$_POST['filano']) {
	echo "<script>alert('Selecione quais anos deseja visualizar');window.close();</script>";
	exit;
}

if($_REQUEST['filper']) {
	$sql = "SELECT perid FROM rehuf.periodogrupoitem WHERE perdsc IN('".implode("','",$_REQUEST['filper'])."') ".(($_REQUEST['filano'])?"AND perano IN('".implode("','",$_REQUEST['filano'])."')":"")." ".(($_REQUEST['agrupador'])?"AND gitid IN(SELECT gitid FROM rehuf.grupoitem WHERE tabtid IN('".implode("','",$_REQUEST['agrupador'])."'))":"");
	$per = $db->carregar($sql);
	if($per[0]) {
		foreach($per as $p) {
			$filtroperiodo[] = $p['perid'];
		}
	}
}

$filtro = (($_REQUEST['entid'][0])?" AND ent.entid IN ('".implode("','",$_REQUEST['entid'])."')":"");

$permissoes = verificaPerfilRehuf();
if(count($permissoes['verhospitais']) > 0) {
	$filtro_pfl = "AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."')";
}

$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)".$filtro." ".$filtro_pfl." ORDER BY ena.entsig");
if($esuids[0]) {
	foreach($esuids as $esuid) {
		$hospitaisFiltro[] = $esuid['esuid'];
	}
}

	// Pegando dados da TABELA (pai de toda a estrutura)
	$tabelas = $db->carregar("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid IN('". implode("','",$_REQUEST['agrupador']) ."')");
	// 	Verifica se existe a TABELA
	if($tabelas) {
		foreach($tabelas as $tabela) {
		// Caso exista, pegar o primeiro registro
		//$tabela = current($tabela);
		echo "<table align='center' class='tabela'>
					<tr><td class='titulounidade'>".$tabela['tabtdsc']."</td></tr>
		  	  </table>";
		// Se existir o filtro de 'grupo', buscar somente tal grupo
		if($_REQUEST['gitid']) {
			if(is_array($_REQUEST['gitid'])) {
				if($_REQUEST['gitid'][0]) $filgrupo = "AND gitid IN('".implode("','",$_REQUEST['gitid'])."')";
			} else {
				$filgrupo = "AND gitid = '". $_REQUEST['gitid'] ."'";
			}
		}
		
		// Pegar um registro (ou registro filtrado, ou primeiro registro da ordem)
		$grupoitemloop = (array) $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ".$filgrupo." ORDER BY gitordem");
		if(!$grupoitemloop[0]) {
			echo "<script>alert('Não existem subgrupos dentro da tabela: ".$tabela['tabtdsc']."');window.location='?modulo=inicio&acao=C';</script>";
			exit;
		}
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
													   WHERE gitid = '". $grupoitem['gitid'] ."' AND coldsc IN('".implode("','",$_POST['filano'])."') ORDER BY coldsc");
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
						$linhaDinTx = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' ".(($hospitaisFiltro[0])?" AND esuid IN('".implode("','",$hospitaisFiltro)."')":"")." ORDER BY linordem");
						break;
					case TPG_LDINAM_OPCOES:
						$linhaDinOp = (array) $db->carregar("SELECT lin.opcid as linid, opc.opcdsc FROM rehuf.linha lin
															 LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid  
															 WHERE gitid = '". $grupoitem['gitid'] ."' ".(($hospitaisFiltro[0])?" AND lin.esuid IN('".implode("','",$hospitaisFiltro)."')":"")."
															 GROUP BY lin.opcid, opc.opcdsc 
															 ORDER BY opc.opcdsc");
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
				
				foreach($_POST['filano'] as $i) {
					if($i >= $tabela['tabanoini'] && $i <= $tabela['tabanofim']) {
						echo "<table align='center' class='tabela'>
								<tr><td class='SubTituloCentro'>".$i."</td></tr>
			  	  			  </table>";
						$_REQUEST['ano'] = $i;
						$fano = "cti.ctiexercicio = '". $i ."' AND ";
						// Carregando as respostas dadas no itens
						if($grupoitem['tpgidlinha'] == TPG_LDINAM_OPCOES) {
							$sql = "SELECT '".$_REQUEST['ano']."' AS ctiexercicio, SUM(cti.ctivalor) as ctivalor, lin.opcid as linid, col.colid FROM rehuf.linha lin 
									LEFT JOIN rehuf.conteudoitem cti ON lin.linid = cti.linid 
									LEFT JOIN rehuf.coluna col ON col.colid = cti.colid 
									WHERE cti.ctiexercicio = '".$_REQUEST['ano']."' AND lin.gitid = '".$grupoitem['gitid']."' ".(($hospitaisFiltro[0])?" AND cti.esuid IN('".implode("','",$hospitaisFiltro)."')":"")." ".(($filtroperiodo)?"AND cti.perid IN('".implode("','",$filtroperiodo)."')":"")."
									GROUP BY lin.opcid, col.colid 
									ORDER BY lin.opcid";
						} else {
							$sql = "SELECT '".$_REQUEST['ano']."' AS ctiexercicio, lin.linid, col.colid, (SELECT SUM(cti.ctivalor) FROM rehuf.conteudoitem cti WHERE ". $fano ." cti.linid = lin.linid AND cti.colid = col.colid  ".(($hospitaisFiltro[0])?"AND cti.esuid IN('".implode("','",$hospitaisFiltro)."')":"")." ".(($filtroperiodo)?"AND cti.perid IN('".implode("','",$filtroperiodo)."')":"").") as ctivalor 
									FROM rehuf.grupoitem git 
									LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid 
									LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid 
									WHERE git.gitid = '".$grupoitem['gitid']."'";
						}
						$rsps = (array) $db->carregar($sql);
						unset($obsgit,$rspgit);
						foreach($rsps as $rsp) {
							$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
							$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
						}
						include "estruturatabelarelatorio.php";
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
				// Carregando as respostas dadas no itens
				if($grupoitem['tpgidlinha'] == TPG_LDINAM_OPCOES) {
					$sql = "SELECT coldsc AS ctiexercicio, SUM(cti.ctivalor) as ctivalor, lin.opcid as linid, col.colid FROM rehuf.linha lin 
							LEFT JOIN rehuf.conteudoitem cti ON lin.linid = cti.linid 
							LEFT JOIN rehuf.coluna col ON col.colid = cti.colid 
							WHERE lin.gitid = '".$grupoitem['gitid']."' ".(($hospitaisFiltro[0])?"AND cti.esuid IN('".implode("','",$hospitaisFiltro)."')":"")." ".(($filtroperiodo)?"AND cti.perid IN('".implode("','",$filtroperiodo)."')":"")."
							GROUP BY lin.opcid, col.colid, col.coldsc 
							ORDER BY lin.opcid";
				} else {
					$sql = "SELECT lin.linid, col.colid, col.coldsc as ctiexercicio, (SELECT SUM(cti.ctivalor) FROM rehuf.conteudoitem cti WHERE cti.linid = lin.linid AND cti.colid = col.colid ".(($hospitaisFiltro[0])?" AND cti.esuid IN('".implode("','",$hospitaisFiltro)."')":"")." ".(($filtroperiodo)?"AND cti.perid IN('".implode("','",$filtroperiodo)."')":"").") as ctivalor 
							FROM rehuf.grupoitem git 
							LEFT JOIN rehuf.linha lin ON lin.gitid = git.gitid 
							LEFT JOIN rehuf.coluna col ON col.gitid = git.gitid 
							WHERE git.gitid = '".$grupoitem['gitid']."'";
				}
				$rsps = (array) $db->carregar($sql);
				unset($obsgit,$rspgit);
				foreach($rsps as $rsp) {
					$obsgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = $rsp['ctiobs'];
					$rspgit[$rsp['linid']][$rsp['colid']][$rsp['ctiexercicio']] = ($rsp['ctivalor'].$rsp['opcid'].$rsp['ctibooleano']);
				}
				include "estruturatabelarelatorio.php";
				
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
?>