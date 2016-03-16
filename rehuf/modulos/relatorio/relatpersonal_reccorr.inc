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

$sqls = array(// RECEITA : Grupo Quadro de Pessoal (Universidade)
			  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('335','337','339','341','343') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receita Efetiva (gitid=45)
			  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','477','479','480','481','482') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Programa Interministerial - MEC (gitid=33) : somente linha custeio
			  2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='408' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas da Universidade (gitid=35) : todas menos linha Capital
			  3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('412','413','414') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas das Fundações de Apoio (gitid=34) : somente linha custeio
			  4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='410' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Não-Operacionais (gitid=32)
			  5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('404','405','406') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Assistenciais (gitid=30) 
			  6 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('395','396','397','398','399') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Bolsas de Residência
			  7 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1798' AND ctiexercicio='{ano}'",
			  // DESPESA : Despesas com Materiais - Universidade (gitid=41)
			  8 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('432','433','434','435','436','437','438','439','440','441') AND ctiexercicio='{ano}'",
			  // DESPESA : Despesas com Materiais - Fundação (gitid=42)
			  9 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('442','443','444','445','446','447','448','449','450','451') AND ctiexercicio='{ano}'",
			  // DESPESA : Contratos de Serviços - Universidade (gitid=43)
			  10 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('452','453','454','455','456','457','458','459','460','461') AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Bolsas de Residência
			  11 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='1798' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Contratos de Serviços - Fundação
			  12 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('462','463','464','465','466','467','468','469','470','471') AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade)
			  13 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('335','337','339','341','343') AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Fundação)
			  14 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('347','349','351') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receita Efetiva (gitid=45)
			  15 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('475','476','478') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Programa Interministerial - MEC (gitid=33) : somente linha capital
			  16 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='407' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas da Universidade (gitid=35) : somente linha Capital
			  17 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='411' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas das Fundações de Apoio (gitid=34) : somente linha capital
			  18 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='409' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente RJU
			  19 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='335' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente CLT
			  20 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='337' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente SSPE
			  21 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='339' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente Tercerizados
			  22 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='341' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente Requisitados
			  23 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='343' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Projetos Específicos (R$)
			  24 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('415','416','417','418','419','420') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receita Gerada pela Atividade de Pesquisa (R$)
			  25 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('400','402','403') AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade) : somente RJU
			  26 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='335' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade) : somente CLT
			  27 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='337' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade) : somente SSPE
			  28 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='339' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade) : somente Tercerizados
			  29 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='341' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Universidade) : somente Requisitados
			  30 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='343' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Fundação) : somente CLT
			  31 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='347' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Fundação) : somente RPA
			  32 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='349' AND ctiexercicio='{ano}'",
			  // DESPESA : Grupo Quadro de Pessoal (Fundação) : somente Terceirizado
			  33 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='351' AND ctiexercicio='{ano}'",
			  // DIVIDAS : Dívidas (Universidade)
			  34 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2016','2017','2018','2019','2020','2021','2022','2023','2024','2025','2026','2027','2028','2029') AND ctiexercicio='{ano}'",
			  // DIVIDAS : Dívidas (Fundação)
			  35 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2030','2031','2032','2033','2034','2035','2036','2037','2038','2039','2040','2041','2042','2043') AND ctiexercicio='{ano}'",
			  // ...
			  36 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('387') AND ctiexercicio='{ano}'",
			  // ...
			  37 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('389') AND ctiexercicio='{ano}'",
			  // ...
			  38 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('391') AND ctiexercicio='{ano}'",
			  // ...
			  39 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('12797') AND ctiexercicio='{ano}'",
			  // ...
			  40 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('12798') AND ctiexercicio='{ano}'",
			  // ...
			  41 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('429') AND ctiexercicio='{ano}'",
			  // ...
			  42 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('430') AND ctiexercicio='{ano}'",
			  // ...
			  43 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('431') AND ctiexercicio='{ano}'"
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

echo "<h1>Relatório de Leitos</h1>";
foreach($_ano as $ano) {
 	unset($TotleitosOperacionais,$TotleitosDesativados,$TotTotLeitos);
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='5'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Total receita corrente</td>";
	echo "<td class='SubTituloCentro'>Total despesa corrente</td>";
	echo "<td class='SubTituloCentro'>Saldo ou Déficit</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
	
		/* Receita Efetiva SUS */
		$ReceitaEfetivaSUS = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[1]));
		/* Incentivos (FIDEPS +ICG +IMMS) */
		$ReceitaIncentivos = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[15]));
		/* Programa Interministerial MEC (custeio) */
		$ReceitaProgramaInterministerialCusteio = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[2]));
		/* Receita de custeio da Universidade */
		$ReceitaCusteioUniversidade = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[3]));
		/* Receita de custeio da Fundação */
		$ReceitaCusteioFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[4]));
		/* Receita Não Operacional */
		$ReceitaNaoOperacional = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[5]));
		/* Receita Assistencial */
		$ReceitaAssistencial = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[6]));
		/* Bolsas de Residência */
		$ReceitaBolsasResidencia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[7]));
		/* Receita para pagamento RJU */
		$ReceitaPagamentoRJU = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[19]));
		/* Receita para pagamento CLT */
		$ReceitaPagamentoCLT = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[20]));
		/* Receita para pagamento SSPE */
		$ReceitaPagamentoSSPE = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[21]));
		/* Receita para pagamento Terceirizados */
		$ReceitaPagamentoTerceirizados = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[22]));
		/* Receita para pagamento Requisitados */
		$ReceitaPagamentoRequisitados = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[23]));
		/* Total da receita de corrente */
		$TotalReceitaCorrente = $ReceitaEfetivaSUS+$ReceitaIncentivos+$ReceitaProgramaInterministerialCusteio+$ReceitaCusteioUniversidade+$ReceitaCusteioFundacao+$ReceitaNaoOperacional+$ReceitaAssistencial+$ReceitaBolsasResidencia+$ReceitaPagamentoRJU+$ReceitaPagamentoCLT+$ReceitaPagamentoSSPE+$ReceitaPagamentoTerceirizados+$ReceitaPagamentoRequisitados;
		
		/* Depesa Materiais Universidade */
		$DespesaMateriaisUniversidade = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[8]));
		/* Depesa Materiais Fundação */
		$DespesaMateriaisFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[9]));
		/* Depesa Serviços Universidade */
		$DespesaServicosUniversidade = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[10]));
		/* Depesa Serviços Fundação */
		$DespesaServicosFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[12]));
		/* Despesa para pagamento RJU */
		$DespesaPagamentoRJU = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[26]));
		/* Despesa para pagamento CLT */
		$DespesaPagamentoCLT = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[27]));
		/* Despesa para pagamento SSPE */
		$DespesaPagamentoSSPE = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[28]));
		/* Despesa para pagamento Terceirizado */
		$DespesaPagamentoTerceirizado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[29]));
		/* Despesa para pagamento Requisitado */
		$DespesaPagamentoRequisitado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[30]));
		/* Despesa para Pagamento da bolsas de residência médica */
		$DespesaBolsaMedica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[11]));
		/* Despesa para Despesa para pagamento CLT(fundação) */
		$DespesaPagamentoCLTFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[31]));
		/* Despesa para Despesa para pagamento RPA(fundação) */
		$DespesaPagamentoRPAFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[32]));
		/* Despesa para Despesa para pagamento RPA(fundação) */
		$DespesaPagamentoTerceirizadoFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[33]));
		/* .... */
		$DespesaUniversidadeSUSCLT = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[36]));
		/* .... */
		$DespesaUniversidadeSUSSSPE = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[37]));
		/* .... */
		$DespesaUniversidadeSUSTercerizados = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[38]));
		/* .... */
		$DespesaOutrasCBolsa = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[41]));
		/* .... */
		$DespesaOutrasCSentenca = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[42]));
		/* .... */
		$DespesaOutrasCOutras = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sqls[43]));
		/* Total de outras despesas */
		$TotalOutrasDespesas = $DespesaOutrasCBolsa+$DespesaOutrasCSentenca+$DespesaOutrasCOutras;
		/* Total das despesas com materiais */
		$TotalDespesaMateriais = $DespesaMateriaisUniversidade+$DespesaMateriaisFundacao;
		/* Total das despesas com serviços */
		$TotalDespesaServicos = $DespesaServicosUniversidade+$DespesaServicosFundacao;
		/* Total das despesas de pessoal */
		$TotalDespesaPessoal = $DespesaPagamentoRJU+$DespesaPagamentoCLT+$DespesaPagamentoSSPE+$DespesaPagamentoTerceirizado+$DespesaPagamentoRequisitado+$DespesaBolsaMedica+$DespesaPagamentoCLTFundacao+$DespesaPagamentoRPAFundacao+$DespesaPagamentoTerceirizadoFundacao+$DespesaUniversidadeSUSCLT+$DespesaUniversidadeSUSSSPE+$DespesaUniversidadeSUSTercerizados;
		/* Total das despesas */
		$TotalDespesaCorrente = $TotalDespesaMateriais+$TotalDespesaServicos+$TotalDespesaPessoal+$TotalOutrasDespesas;

		
	
		echo "<tr>";
		echo "<td>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".number_format($TotalReceitaCorrente,2,',','.')."</td>";
		echo "<td>".number_format($TotalDespesaCorrente,2,',','.')."</td>";
		echo "<td>".number_format(($TotalReceitaCorrente-$TotalDespesaCorrente),2,',','.')."</td>";
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