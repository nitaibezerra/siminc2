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
/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

$sql = array(// Total de leitos
			 0 => "SELECT ROUND(SUM(ctivalor)/12) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('441','442','443','444') AND ctiexercicio='{ano}'",
			 // Total de leitos UTI
			 1 => "SELECT ROUND(SUM(ctivalor)/12) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('15050','15051','15052') AND colid IN('441','442','443','444') AND ctiexercicio='{ano}'",
			 // Salas Cirúrgicas Ativas
			 2 => "SELECT ROUND(SUM(ctivalor)/12) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('15083','15088') AND ctiexercicio='{ano}'",
			 // Total Alunos (Graduação)
			 3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('402','517') AND ctiexercicio='{ano}' AND perid IN('119','272')",
			 // Total Alunos de Medicina (Graduação)
			 4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN('402','517') AND linid IN('14829','15018') AND ctiexercicio='{ano}' AND perid IN('119','272')",
			 // Total Alunos (Residência Médica)
			 5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN('397','398','399','400')  AND ctiexercicio='{ano}' AND perid IN('120','273')",
			 // Total Docentes
			 6 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN('410','521') AND ctiexercicio='{ano}' AND perid IN('149','301')",
			 // Total Internações
			 7 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN('457','540') AND ctiexercicio='{ano}'",
			 // Número de Projetos Aprovados no CEP
			 8 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND linid='287' AND colid IN('424','518') AND ctiexercicio='{ano}'",
			 // Partos de Alto Risco
			 9 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND linid IN('1992','1993') AND ctiexercicio='{ano}'",
			 // Total Pessoal
			 10 => "SELECT ROUND(SUM(ctivalor)/3) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND colid IN('492','494','496','515','500','502','504','506') AND ctiexercicio='{ano}'",
			// Total de leitos-Dia
			 11 => "SELECT ROUND(SUM(ctivalor)/12) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('15055','15056','15057','15058') AND colid IN('441','442','443','444') AND ctiexercicio='{ano}'",
			 // Total Internações-Dia
			 12 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND linid='1986' AND ctiexercicio='{ano}'",
			 // Total Internações
			 13 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid IN('{esuid}') AND linid IN('8142','8141','8140','15110','1978','1989','1987','1986','1984','1983','1981','1980','1979','15111','1988','1993','1992','1990','1991') AND ctiexercicio='{ano}'"
			 
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

$_ano = array('2009');

echo "<h1>Matriz de distribuição</h1>";
foreach($_ano as $ano) {
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='20'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Tipo do HUF</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos-Dia</td>";
	echo "<td class='SubTituloCentro'>Total de Leitos UTI</td>";
	
	echo "<td class='SubTituloCentro'>Partos de Alto Risco</td>";
	echo "<td class='SubTituloCentro'>Salas Cirúrgicas Ativas</td>";
	echo "<td class='SubTituloCentro'>Total Alunos (Graduação)</td>";
	echo "<td class='SubTituloCentro'>Total Alunos de Medicina (Graduação)</td>";
	echo "<td class='SubTituloCentro'>Total Alunos (Residência Médica)</td>";
	

	echo "<td class='SubTituloCentro'>Total Docentes</td>";
	echo "<td class='SubTituloCentro'>Total Pessoal</td>";
	echo "<td class='SubTituloCentro'>Total de Internações</td>";
	echo "<td class='SubTituloCentro'>Total de Internações Leitos Dia</td>";
	echo "<td class='SubTituloCentro'>Total Internações Dia</td>";
	echo "<td class='SubTituloCentro'>Projetos Aprovados pelo CEP</td>";
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$totalLeitos = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]));
		$totalLeitosDia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[11]));
		$totalLeitosUTI = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$totalSalasCirurgicasAtivas = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]));
		
		$totalAlunosGraduacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[3]));
		
		$totalAlunosGraduacaoMedicina = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[4]));

		$totalAlunosMedicinaResidencia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[5]));
		$totalDocentes = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[6]));
		$totalInternacoesLeitosDia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[7]));
		$totalInternacoesDia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[12]));
		$totalInternacoes = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[13]));
		
		$totalProjetosCEP = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[8]));
		$totalPartosAltoRisco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[9]));
		
		$totalPessoal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[10]));
		
		switch($esuid['esutipo']) {
			case 'E':
				$esutipo = "Especialidade";
				break;
			case 'G':
				$esutipo = "Geral";
				break;
			case 'M':
				$esutipo = "Maternidade";
				break;
				
		}
		
		
		echo "<tr>";
		echo "<td nowrap>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".$esutipo."</td>";
		echo "<td align=right>".(($totalLeitos)?$totalLeitos:"0")."</td>";
		echo "<td align=right>".(($totalLeitosDia)?$totalLeitosDia:"0")."</td>";
		echo "<td align=right>".(($totalLeitosUTI)?$totalLeitosUTI:"0")."</td>";
		echo "<td align=right>".(($totalPartosAltoRisco)?$totalPartosAltoRisco:"0")."</td>";
		echo "<td align=right>".(($totalSalasCirurgicasAtivas)?$totalSalasCirurgicasAtivas:"0")."</td>";
		echo "<td align=right>".(($totalAlunosGraduacao)?$totalAlunosGraduacao:"0")."</td>";
		echo "<td align=right>".(($totalAlunosGraduacaoMedicina)?$totalAlunosGraduacaoMedicina:"0")."</td>";
		echo "<td align=right>".(($totalAlunosMedicinaResidencia)?$totalAlunosMedicinaResidencia:"0")."</td>";
		echo "<td align=right>".(($totalDocentes)?$totalDocentes:"0")."</td>";
		echo "<td align=right>".(($totalPessoal)?$totalPessoal:"0")."</td>";
		echo "<td align=right>".(($totalInternacoes)?$totalInternacoes:"0")."</td>";
		echo "<td align=right>".(($totalInternacoesLeitosDia)?$totalInternacoesLeitosDia:"0")."</td>";
		echo "<td align=right>".(($totalInternacoesDia)?$totalInternacoesDia:"0")."</td>";
		echo "<td align=right>".(($totalProjetosCEP)?$totalProjetosCEP:"0")."</td>";
		echo "</tr>";
	}


echo "</table>";

}

?>