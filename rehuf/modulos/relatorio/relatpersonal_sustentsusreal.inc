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
$sql = array(// DESPESA : Despesas com Materiais - Universidade (gitid=41)
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('432','433','434','435','436','437','438','439','440','441') AND ctiexercicio='{ano}'",
			 // DESPESA : Despesas com Materiais - Fundação (gitid=42)
			 1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('442','443','444','445','446','447','448','449','450','451') AND ctiexercicio='{ano}'",
			 // DESPESA : Contratos de Serviços - Universidade (gitid=43)
			 2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('452','453','454','455','456','457','458','459','460','461') AND ctiexercicio='{ano}'",
			 // DESPESA : Grupo Contratos de Serviços - Fundação
			 3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('462','463','464','465','466','467','468','469','470','471') AND ctiexercicio='{ano}'",
			 // RECEITA : RECEITA Efetiva
			 4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid='217' AND linid IN('473','474','475','476','477','478','479','480','481','482') AND ctiexercicio='{ano}'",
			 // DESPESA : Quadro de Pessoal (Universidade - recursos SUS)
			 5 => "SELECT CASE WHEN SUM(ctivalor) IS NULL THEN 0 ELSE SUM(ctivalor) END FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN ('387','389','391') AND ctiexercicio='{ano}'",
			 // DESPESA : Grupo Quadro de Pessoal (Fundação)
			 6 => "SELECT CASE WHEN SUM(ctivalor) IS NULL THEN 0 ELSE SUM(ctivalor) END FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN ('347','349','351') AND ctiexercicio='{ano}'");
			  
			  

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

echo "<h1>Relatório de Sustentabilidade SUS (Real)</h1>";
foreach($_ano as $ano) {
 	unset($TotdespesaMateriais,$TotdespesaEfetiva,$TotdespesaPessoal);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='6'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Despesa Material e Serviço(Fundação+Universidade)</td>";
	echo "<td class='SubTituloCentro'>Despesa Pessoal(Fundação+Universidade com Recursos SUS)</td>";
	echo "<td class='SubTituloCentro'>Receita</td>";
	echo "<td class='SubTituloCentro'>Indicador</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$despesaMateriais = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]))+$db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]))+$db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]))+$db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[3]));
		$TotdespesaMateriais += $despesaMateriais;
		$despesaPessoal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[5]))+$db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[6]));
		$TotdespesaPessoal += $despesaPessoal;
		
		$receitaEfetiva = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[4]));
		$TotdespesaEfetiva += $receitaEfetiva;
		
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".number_format($despesaMateriais, 2, ',', '.')."</td>";
		echo "<td>".number_format($despesaPessoal, 2, ',', '.')."</td>";
		echo "<td>".number_format($receitaEfetiva, 2, ',', '.')."</td>";
		if($despesaMateriais) {
			echo "<td>".number_format(($receitaEfetiva/($despesaMateriais+$despesaPessoal))*100, 2, ',', '')."%</td>";
		} else {
			echo "<td>0,00%</td>";
		}
		echo "</tr>";
	}
	
	echo "<tr bgcolor=\"#C0C0C0\">";
	echo "<td colspan='2' align='right'><b>TOTAL :</b></td>";
	echo "<td><b>".number_format($TotdespesaMateriais,2,',','.')."</b></td>";
	echo "<td><b>".number_format($TotdespesaPessoal,2,',','.')."</b></td>";
	echo "<td><b>".number_format($TotdespesaEfetiva,2,',','.')."</b></td>";
	if($TotdespesaMateriais) {
		echo "<td><b>".number_format(($TotdespesaEfetiva/($TotdespesaMateriais+$TotdespesaPessoal))*100, 2, ',', '')."%</b></td>";
	} else {
		echo "<td><b>0,00%</b></td>";
	}
	echo "</tr>";
	

echo "</table>";

}

?>