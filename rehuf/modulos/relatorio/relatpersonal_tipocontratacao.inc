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

$sql = array(// Nº de funcionários RJU
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334') AND ctiexercicio='{ano}'",
			 // Nº de funcionários CLT (Recursos próprios)
			 1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('336') AND ctiexercicio='{ano}'",
			 // Nº de funcionários CLT (Recursos SUS + Fundação)
			 2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('346','386') AND ctiexercicio='{ano}'",
			 // Nº de outros (SSPE,Terceirizados,Requisitados,Cedidos,RPA)
			 3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('338','340','342','344','348','350','388','390') AND ctiexercicio='{ano}'"
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

$_ano = array('2004','2005','2006','2007','2008');
echo "<h1>Relatório de quantidade de pessoal</h1>";
foreach($_ano as $ano) {
 	unset($Totrju,$Totcltproprio,$Totcltfundacaosus,$Totoutros);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='7'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>RJU</td>";
	echo "<td class='SubTituloCentro' title='Recursos próprios'>CLT 1</td>";
	echo "<td class='SubTituloCentro' title='Recursos SUS+Fundação'>CLT 2</td>";
	echo "<td class='SubTituloCentro' title='SSPE,Terceirizados, Requisitados, Cedidos, RPA'>Outros</td>";
	echo "<td class='SubTituloCentro'>Total</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$rju = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]));
		$Totrju += $rju;
		$cltproprio = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$Totcltproprio += $cltproprio;
		$cltfundacaosus = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]));
		$Totcltfundacaosus += $cltfundacaosus;
		$outros = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[3]));
		$Totoutros += $outros;
		
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".$rju."</td>";
		echo "<td>".$cltproprio."</td>";
		echo "<td>".$cltfundacaosus."</td>";
		echo "<td>".$outros."</td>";
		echo "<td>".($rju+$cltproprio+$cltfundacaosus+$outros)."</td>";
		echo "</tr>";
	}
	
	echo "<tr bgcolor=\"#C0C0C0\">";
	echo "<td colspan='2' align='right'><b>TOTAL :</b></td>";
	echo "<td>".$Totrju."</td>";
	echo "<td>".$Totcltproprio."</td>";
	echo "<td>".$Totcltfundacaosus."</td>";
	echo "<td>".$Totoutros."</td>";
	echo "<td>".($Totrju+$Totcltproprio+$Totcltfundacaosus+$Totoutros)."</td>";
	echo "</tr>";
	

echo "</table>";

}

?>