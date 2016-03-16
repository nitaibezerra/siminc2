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
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND gitid='51' AND  cdi.ctiexercicio='{ano}'",
			 // Leitos Hospital-Dia
			 1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('499','500','501','502') AND cdi.ctiexercicio='{ano}'",
			 // Leitos UTI
			 2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('488','489','490','491') AND cdi.ctiexercicio='{ano}'",
			 // Partos de Alto Risco
			 3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('1992','1993') AND cdi.ctiexercicio='{ano}'",
			 // Sala de cirurgias ativas
			 4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('542') AND cdi.ctiexercicio='{ano}'",
			 // SUS Alta
			 5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.colid IN('328') AND cdi.ctiexercicio='{ano}'",
			 // Alunos de graduação
			 6 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.colid IN('79') AND cdi.ctiexercicio='{ano}'",
			 // Alunos de Medicina (graduação)
			 7 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
			 	   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid
				   WHERE cdi.esuid IN('{esuid}') AND cdi.colid IN('79') AND lin.opcid='6655' AND cdi.ctiexercicio='{ano}'",
			 // Alunos residência médica (graduação)
			 8 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.colid IN('80') AND cdi.ctiexercicio='{ano}'",
			 // Docente com titulação máxima
			 9 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND gitid='14' AND  cdi.ctiexercicio='{ano}'",
			 // Total pessoal
			 10 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.colid IN('334','336','338','340','342','344','386','388','390','346','348','350') AND cdi.ctiexercicio='{ano}'",
			 // Total internações
			 11 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
			 	    LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid
				   WHERE cdi.esuid IN('{esuid}') AND lin.agpid='81' AND cdi.ctiexercicio='{ano}'",
			 // Internações Hospital-Dia
			 12 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('1986') AND cdi.ctiexercicio='{ano}'",
			 // Projetos Aprovados pelo CEP
			 13 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   WHERE cdi.esuid IN('{esuid}') AND cdi.linid IN('287') AND cdi.ctiexercicio='{ano}'",
			 // 		Total Dias/Ano Internações
			 14 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND gitid='65' AND  cdi.ctiexercicio='{ano}'"
			);
			  

$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");
			
$_ano = array('2008');
$_esutipo = array("E" => "Especialidade", "G" => "Geral", "M" => "Maternidade");

echo "<h1>Relatório de Fatores Estratégicos de Desempenho</h1>";
foreach($_ano as $ano) {
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='20'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>Tipo do HUF</td>";
	echo "<td class='SubTituloCentro'>Região</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos-Dia</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos UTI</td>";
	echo "<td class='SubTituloCentro'>Partos de Alto Risco</td>";
	echo "<td class='SubTituloCentro'>Salas Cirúrgicas Ativas</td>";
	echo "<td class='SubTituloCentro'>SUS Alta</td>";
	echo "<td class='SubTituloCentro'>Total Alunos (Graduação)</td>";
	echo "<td class='SubTituloCentro'>Total Alunos de Medicina (Graduação)</td>";
	echo "<td class='SubTituloCentro'>Total Alunos (Residência Médica)</td>";
	echo "<td class='SubTituloCentro'>Total Docentes</td>";
	echo "<td class='SubTituloCentro'>Total Pessoal</td>";
	echo "<td class='SubTituloCentro'>Total Internações</td>";
	echo "<td class='SubTituloCentro'>Total Internações (Leito-Dia)</td>";
	echo "<td class='SubTituloCentro'>Projetos Aprovados pelo CEP</td>";
	echo "<td class='SubTituloCentro'>Total de Taxa de Ocupação</td>";
	echo "<td class='SubTituloCentro'>Total de Média de Permanância</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$totleitos = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]))+"0";
		$totleitoshospitaldia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$totleitosuti = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]));
		$totpartosaltorisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[3]));
		$totsalacirurgiaativa = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[4]));
		$totsusalta = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[5]));
		$totgraduacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[6]));
		$totgraduacaomedicina = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[7]));
		$totresidenciamedica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[8]));
		$tottitulacaomaxima = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[9]));
		$totpessoal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[10]));
		$totinternacoes = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[11]));
		$totinternacoesleitodia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[12]));
		/*
		 * Retirando as decimais caso, seja .00 (exemplo : 127.00), este erro ocorre devido ao fato de primeiramente ter
		 * sido configurado o sistema como float (moeda), onde aceitava casas decimais e depois ter mudado para inteiro
		 * Alguns resgistros foram gravados com .00
		 */ 
		$totprojetosapcep = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[13]))+"0";
		// calculando taxa de ocupação anual
		$totanodiainternacaoes = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[14]));
		
		echo "<tr>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td nowrap>".$esuid['entnome']."</td>";
		echo "<td>".$_esutipo[$esuid['esutipo']]."</td>";
		echo "<td>".$esuid['estuf']."</td>";
		echo "<td align='right'>".$totleitos."</td>";
		echo "<td align='right'>".$totleitoshospitaldia."</td>";
		echo "<td align='right'>".$totleitosuti."</td>";
		echo "<td align='right'>".$totpartosaltorisco."</td>";
		echo "<td align='right'>".$totsalacirurgiaativa."</td>";
		echo "<td align='right'>".$totsusalta."</td>";
		echo "<td align='right'>".$totgraduacao."</td>";
		echo "<td align='right'>".$totgraduacaomedicina."</td>";
		echo "<td align='right'>".$totresidenciamedica."</td>";
		echo "<td align='right'>".$tottitulacaomaxima."</td>";
		echo "<td align='right'>".$totpessoal."</td>";
		echo "<td align='right'>".$totinternacoes."</td>";
		echo "<td align='right'>".$totinternacoesleitodia."</td>";
		// Forçando utilizar a mascara inteira (valores antigamente colocados como 2.66 (errado) virarão 266 (correto))
		echo "<td align='right'>".mascaraglobal($totprojetosapcep,"#############")."</td>";
		echo "<td align='right'>".(($totleitos)?round(($totanodiainternacaoes/($totleitos*365))*100,2):"0.00")."%</td>";
		echo "<td align='right'>".(($totinternacoes)?round($totanodiainternacaoes/$totinternacoes,2):"0.00")."</td>";
		echo "</tr>";
	}
echo "</table>";

}

?>