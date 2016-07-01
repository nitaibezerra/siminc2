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
	text-align: left;
}
.titulounidade {
	background-color:#CDCDCD;
	text-align:center;
	font-weight: bold;
}
</style>
<?


$sql = array(// Leitos Operacionais
			 0 => "SELECT CASE 
			 				WHEN '{ano}' = '2009' THEN ROUND(SUM(ctivalor)/12) 
			 				WHEN '{ano}' = '2010' THEN ROUND(SUM(ctivalor)/12) 
			 			  END as c FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND gitid='106' AND  cdi.ctiexercicio='{ano}'",
			 // Leitos Desativados
			 1 => "SELECT CASE 
			 				WHEN '{ano}' = '2009' THEN ROUND(SUM(ctivalor)/12) 
			 				WHEN '{ano}' = '2010' THEN ROUND(SUM(ctivalor)/12) 
			 			  END as c FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND gitid='107' AND  cdi.ctiexercicio='{ano}'"
			);
			  
			  

$filtro = (($_REQUEST['entid'][0])?"WHERE ent.entid IN ('".implode("','",$_REQUEST['entid'])."')":"");
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");

$_ano = array('2009','2010');

echo "<h1>Relatório de Leitos (2009-2010)</h1>";
foreach($_ano as $ano) {
 	unset($TotleitosOperacionais,$TotleitosDesativados,$TotTotLeitos);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='5'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Leitos Operacionais</td>";
	echo "<td class='SubTituloCentro'>Leitos Desativados</td>";
	echo "<td class='SubTituloCentro'>Total</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$leitosOperacionais = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]));
		$TotleitosOperacionais += $leitosOperacionais;
		$leitosDesativados = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$TotleitosDesativados += $leitosDesativados;
		
		$TotLeitos = $leitosOperacionais+$leitosDesativados;
		$TotTotLeitos += $TotLeitos;
		
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".(($leitosOperacionais)?$leitosOperacionais:"0")."</td>";
		echo "<td>".(($leitosDesativados)?$leitosDesativados:"0")."</td>";
		echo "<td>".$TotLeitos."</td>";
		echo "</tr>";
	}
	
	echo "<tr bgcolor=\"#C0C0C0\">";
	echo "<td colspan='2' align='right'><b>TOTAL :</b></td>";
	echo "<td><b>".$TotleitosOperacionais."</b></td>";
	echo "<td><b>".$TotleitosDesativados."</b></td>";
	echo "<td><b>".$TotTotLeitos."</b></td>";
	echo "</tr>";
	

echo "</table>";

}

?>