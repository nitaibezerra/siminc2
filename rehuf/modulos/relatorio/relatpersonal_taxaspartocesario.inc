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

$sqls = array(// Parto Baixo Risco => Cesáreo
			  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1990' AND ctiexercicio='{ano}'",
			  // Parto Baixo Risco => Normal 
			  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1991' AND ctiexercicio='{ano}'",
			  // Parto Alto Risco => Cesáreo
			  2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1992' AND ctiexercicio='{ano}'",
			  // Parto Alto Risco => Normal
			  3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1993' AND ctiexercicio='{ano}'"
			  );			  
			  

$filtro = (($_REQUEST['entid'][0])?"WHERE ent.entid IN ('".implode("','",$_REQUEST['entid'])."')":"");
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo, esu.esuareacontruida FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.entidadeendereco ene ON ene.entid = ent.entid 
						 LEFT JOIN entidade.endereco ende ON ende.endid = ene.endid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");

$_ano = array('2009');

echo "<h1>Taxas de Partos Cesáreos</h1>";
foreach($_ano as $ano) {
 	unset($TotleitosOperacionais,$TotleitosDesativados,$TotTotLeitos);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='5'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Taxa de partos cesáreos de baixo risco</td>";
	echo "<td class='SubTituloCentro'>Taxa de partos cesáreos de alto risco</td>";
	echo "<td class='SubTituloCentro'>Taxa de partos cesáreos</td>";
	
	echo "</tr>";
	
	foreach($esuids as $esuid) {
	
		/* Parto Baixo Risco => Cesáreo */
		$partoscesariosbaixorisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[0]));
		/* Parto Baixo Risco => Normal */
		$partosnormalbaixorisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[1]));
		/* Parto Alto Risco => Cesáreo */
		$partoscesarioaltorisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[2]));
		/* Parto Alto Risco => Normal */
		$partosnormalaltorisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[3]));
		
		unset($txpartoscesariosbaixorisco);
		if(($partoscesariosbaixorisco+$partosnormalbaixorisco) > 0) {
			$txpartoscesariosbaixorisco = round(($partoscesariosbaixorisco/($partoscesariosbaixorisco+$partosnormalbaixorisco))*100,1);
		}
		
		unset($txpartoscesariosaltorisco);
		if(($partoscesarioaltorisco+$partosnormalaltorisco) > 0) {
			$txpartoscesariosaltorisco = round(($partoscesarioaltorisco/($partoscesarioaltorisco+$partosnormalaltorisco))*100,1);
		}
		
		unset($txpartoscesarios);
		if(($partoscesariosbaixorisco+$partosnormalbaixorisco+$partoscesarioaltorisco+$partosnormalaltorisco) > 0) {
			$txpartoscesarios = round((($partoscesarioaltorisco+$partoscesariosbaixorisco)/($partoscesariosbaixorisco+$partosnormalbaixorisco+$partoscesarioaltorisco+$partosnormalaltorisco))*100,1);
		}
	
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td align=right>".(($txpartoscesariosbaixorisco)?$txpartoscesariosbaixorisco:"0")."%</td>";
		echo "<td align=right>".(($txpartoscesariosaltorisco)?$txpartoscesariosaltorisco:"0")."%</td>";
		echo "<td align=right>".(($txpartoscesarios)?$txpartoscesarios:"0")."%</td>";

		echo "</tr>";
	}

	

echo "</table>";

}

?>