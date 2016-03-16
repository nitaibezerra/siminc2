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
$sql = array(// 1962	Setor de Emergência	Clínica
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1962') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 //	1968	Setor de Emergência	Pediátrica
			 1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1968') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1969	Setor de Emergência	Cirúrgica
			 2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1969') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1970	Setor de Emergência	Obstétrica
			 3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1970') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 
 			 // 1971	Consultas	Clínica Médica
			 4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1971') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1972	Consultas	Cirurgia
			 5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1972') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1973	Consultas	Pediatria
			 6 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1973') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1974	Consultas	Ginecologia
			 7 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1974') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1975	Consultas	Obstetrícia
			 8 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1975') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1976	Consultas	Psiquiatria
			 9 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1976') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1977	Consultas	Outras Especialidades Não Médicas
			 10 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1977') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 
			 // 1978	Internações	Clínica Médica
			 11 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1978') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1979	Internações	Cirurgia
			 12 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1979') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1980	Internações	Pediatria
			 13 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1980') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1981	Internações	Obstetrícia
			 14 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1981') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1983	Internações	Ginecologia
			 15 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1983') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1984	Internações	Psiquiatria
			 16 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1984') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1986	Internações	Hospital-Dia
			 17 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1986') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1987	Internações	UTI Adulto
			 18 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1987') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1988	Internações	UTI Pediátrica
			 19 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1988') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1989	Internações	UTI Neonatal
			 20 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1989') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 8140	Internações	Unidade Intermediária Adulta
			 21 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('8140') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 8141	Internações	Unidade Intermediária Pediátrica
			 22 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('8141') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 8142	Internações	Unidade Intermediária Neonatal
			 23 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('8142') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 
			 // 1990	Partos de Baixo Risco	Cesárea
			 24 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1990') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1991	Partos de Baixo Risco	Normal
			 25 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1991') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1992	Partos de Alto Risco	Cesárea
			 26 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1992') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 // 1993	Partos de Alto Risco	Normal
		 	 27 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1993') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			 
			 
			// 1994	Transplantes	Medula Óssea
			 28 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1994') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			// 1995	Transplantes	Fígado
			 29 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1995') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			// 1996	Transplantes	Cardíaco
			 30 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1996') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			// 1997	Transplantes	Pulmão
			 31 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1997') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			// 1998	Transplantes	Renal			
			 32 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1998') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			// 1999	Transplantes	Córnea
			 33 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1999') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
		 	 
			 

			//2000	Procedimentos	Exames Laboratoriais
			 34 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2000') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2001	Procedimentos	Endoscopia Digestiva Alta
			 35 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2001') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2002	Procedimentos	Colonoscopia
			 36 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2002') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2003	Procedimentos	Broncoscopia
			 37 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2003') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2004	Procedimentos	Tomografia Computadorizada
			 38 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2004') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2005	Procedimentos	Ressonância Magnética
			 39 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2005') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2006	Procedimentos	Ultrassonografia
			 40 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2006') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2007	Procedimentos	Radiologia Convencional
			 41 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2007') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2008	Procedimentos	Hemodinâmica
			 42 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2008') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2009	Procedimentos	Terapia Renal Substitutiva
			 43 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2009') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2010	Procedimentos	Radioterapia
			 44 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2010') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2011	Procedimentos	Quimioterapia
			 45 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2011') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2012	Procedimentos	Medicina Nuclear in vivo			 
			 46 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2012') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2013	Procedimentos	Radiologia Intervencionista
			 47 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2013') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2014	Procedimentos	Exames de Histocompatibilidade
			 48 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2014') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",
			//2015	Procedimentos	Busca de Órgãos
			 49 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('2015') AND colid='328' AND ctiexercicio='{ano}' AND perid='168'",


			 
			 
			 
			 
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

echo "<h1>Relatório de Procedimentos de Alta Complexidade (2009)</h1>";
foreach($_ano as $ano) {
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='52'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
										 
	
	echo "<td class='SubTituloCentro'>Emergência Clínica</td>";
	echo "<td class='SubTituloCentro'>Emergência Pediátrica</td>";
	echo "<td class='SubTituloCentro'>Emergência Cirúrgica</td>";
	echo "<td class='SubTituloCentro'>Emergência Obstétrica</td>";
	echo "<td class='SubTituloCentro'>Consultas Clínica Médica</td>";
	echo "<td class='SubTituloCentro'>Consultas Cirurgia</td>";
	echo "<td class='SubTituloCentro'>Consultas Pediatria</td>";
	echo "<td class='SubTituloCentro'>Consultas Ginecologia</td>";
	echo "<td class='SubTituloCentro'>Consultas Obstetrícia</td>";
	echo "<td class='SubTituloCentro'>Consultas Psiquiatria</td>";
	echo "<td class='SubTituloCentro'>Consultas Outras</td>";
	
	
	echo "<td class='SubTituloCentro'>Internações Clínica Médica</td>";
	echo "<td class='SubTituloCentro'>Internações Cirurgia</td>";
	echo "<td class='SubTituloCentro'>Internações Pediatria</td>";
	echo "<td class='SubTituloCentro'>Internações Obstetrícia</td>";
	echo "<td class='SubTituloCentro'>Internações Ginecologia</td>";
	echo "<td class='SubTituloCentro'>Internações Psiquiatria</td>";
	echo "<td class='SubTituloCentro'>Internações Hospital-Dia</td>";
	echo "<td class='SubTituloCentro'>Internações UTI Adulto</td>";
	echo "<td class='SubTituloCentro'>Internações UTI Pediátrica</td>";
	echo "<td class='SubTituloCentro'>Internações UTI Neonatal</td>";
	echo "<td class='SubTituloCentro'>Internações Unidade Intermediária Adulta</td>";
	echo "<td class='SubTituloCentro'>Internações Unidade Intermediária Pediátrica</td>";
	echo "<td class='SubTituloCentro'>Internações Unidade Intermediária Neo-Natal</td>";
	
	
	echo "<td class='SubTituloCentro'>Partos de Baixo Risco Cesárea</td>";
	echo "<td class='SubTituloCentro'>Partos de Baixo Risco Normal</td>";
	echo "<td class='SubTituloCentro'>Partos de Alto Risco Cesárea</td>";
	echo "<td class='SubTituloCentro'>Partos de Alto Risco Normal</td>";
	

	echo "<td class='SubTituloCentro'>Transplantes Medula Óssea</td>";
	echo "<td class='SubTituloCentro'>Transplantes Fígado</td>";
	echo "<td class='SubTituloCentro'>Transplantes Cardíaco</td>";
	echo "<td class='SubTituloCentro'>Transplantes Pulmão</td>";
	echo "<td class='SubTituloCentro'>Transplantes Renal</td>";
	echo "<td class='SubTituloCentro'>Transplantes Córnea</td>";
	

	
	echo "<td class='SubTituloCentro'>Exames Laboratoriais</td>";
	echo "<td class='SubTituloCentro'>Endoscopia Digestiva Alta</td>";
	echo "<td class='SubTituloCentro'>Colonoscopia</td>";
	echo "<td class='SubTituloCentro'>Broncoscopia</td>";
	echo "<td class='SubTituloCentro'>Tomografia Computadorizada</td>";
	echo "<td class='SubTituloCentro'>Ressonância Magnética</td>";
	echo "<td class='SubTituloCentro'>Ultrassonográfico</td>";
	echo "<td class='SubTituloCentro'>Radiologia Convencional</td>";
	echo "<td class='SubTituloCentro'>Hemodinâmica</td>";
	echo "<td class='SubTituloCentro'>Terapia Renal Substitutiva</td>";
	echo "<td class='SubTituloCentro'>Radioterapia</td>";
	echo "<td class='SubTituloCentro'>Quimioterapia</td>";
	echo "<td class='SubTituloCentro'>Medicina Nuclear in vivo</td>";
	echo "<td class='SubTituloCentro'>Radiologia Intervencionista</td>";
	echo "<td class='SubTituloCentro'>Exames de Histocompatibilidade</td>";
	echo "<td class='SubTituloCentro'>Busca de Órgãos</td>";
				
	
	
	echo "</tr>";
	
	foreach($esuids as $esuid) {
		$emClinica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[0]));
		$emPediatrica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[1]));
		$emCirurgica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[2]));
		$emObstetrica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[3]));
		
		$conClinicaMedica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[4]));
		$conCirurgica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[5]));
		$conPediatria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[6]));
		$conGinecologia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[7]));
		$conObstetricia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[8]));
		$conPsiquiatria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[9]));
		$conOutras = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[10]));
		
		$intClinicaMedica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[11]));
		$intCirurgia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[12]));
		$intPediatria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[13]));
		$intObstetricia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[14]));
		$intGinecologia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[15]));
		$intPsiquiatria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[16]));	
		$intHospitalDia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[17]));	
		$intUTIAdulto = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[18]));	
		$intUTIPediatrica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[19]));	
		$intUTINeonatal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[20]));	
		$intUnidadeIntermediariaAdulta = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[21]));
		$intUnidadeIntermediariaPediatrica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[22]));
		$intUnidadeIntermediariaNeonatal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[23]));
		
		$parBaixoCesaria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[24]));	
		$parBaixoNormal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[25]));
		$parAltoCesaria = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[26]));
		$parAltoNormal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[27]));
		
		
		$traMedulaOssea = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[28]));
		$traFigado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[29]));
		$traCardiaco = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[30]));
		$traPulmao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[31]));
		$traRenal = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[32]));
		$traCornea = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[33]));
		
		

		$proExLab = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[34]));
		$proEndDigAlta = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[35]));
		$proColonos = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[36]));
		$proBroncos = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[37]));
		$proTomografia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[38]));
		$proRessonanciaMag = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[39]));
		$proUltrasono = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[40]));
		$proRadioConv = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[41]));
		$proHemodi = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[42]));
		$proTerapiaRenalSub = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[43]));
		$proRadioterapia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[44]));
		$proQuimioterapia = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[45]));
		$proMedicinaNuclear = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[46]));
		$proRadiointerven = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[47]));
		$proExhistocom = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[48]));
		$proBuscaorgao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($esuid['esuid'],$ano),$sql[49]));
		
		
		
		
		
		
		echo "<tr>";
		echo "<td nowrap>".$esuid['entnome']."</td>";
		echo "<td>".$esuid['entsig']."</td>";
		echo "<td>".$emClinica."</td>";
		echo "<td>".$emPediatrica."</td>";
		echo "<td>".$emCirurgica."</td>";
		echo "<td>".$emObstetrica."</td>";
		echo "<td>".$conClinicaMedica."</td>";
		echo "<td>".$conCirurgica."</td>";
		echo "<td>".$conPediatria."</td>";
		echo "<td>".$conGinecologia."</td>";
		echo "<td>".$conObstetricia."</td>";
		echo "<td>".$conPsiquiatria."</td>";
		echo "<td>".$conOutras."</td>";
		
		
		echo "<td>".$intClinicaMedica."</td>";
		echo "<td>".$intCirurgia."</td>";
		echo "<td>".$intPediatria."</td>";
		echo "<td>".$intObstetricia."</td>";
		echo "<td>".$intGinecologia."</td>";
		echo "<td>".$intPsiquiatria."</td>";
		echo "<td>".$intHospitalDia."</td>";
		echo "<td>".$intUTIAdulto."</td>";
		echo "<td>".$intUTIPediatrica."</td>";
		echo "<td>".$intUTINeonatal."</td>";
		echo "<td>".$intUnidadeIntermediariaAdulta."</td>";
		echo "<td>".$intUnidadeIntermediariaPediatrica."</td>";
		echo "<td>".$intUnidadeIntermediariaNeonatal."</td>";
		
		echo "<td>".$parBaixoCesaria."</td>";
		echo "<td>".$parBaixoNormal."</td>";
		echo "<td>".$parAltoCesaria."</td>";
		echo "<td>".$parAltoNormal."</td>";
		
		
		echo "<td>".$traMedulaOssea."</td>";
		echo "<td>".$traFigado."</td>";
		echo "<td>".$traCardiaco."</td>";
		echo "<td>".$traPulmao."</td>";
		echo "<td>".$traRenal."</td>";
		echo "<td>".$traCornea."</td>";
		
		
		echo "<td>".$proExLab."</td>";
		echo "<td>".$proEndDigAlta."</td>";
		echo "<td>".$proColonos."</td>";
		echo "<td>".$proBroncos."</td>";
		echo "<td>".$proTomografia."</td>";
		echo "<td>".$proRessonanciaMag."</td>";
		echo "<td>".$proUltrasono."</td>";
		echo "<td>".$proRadioConv."</td>";
		echo "<td>".$proHemodi."</td>";
		echo "<td>".$proTerapiaRenalSub."</td>";
		echo "<td>".$proRadioterapia."</td>";
		echo "<td>".$proQuimioterapia."</td>";
		echo "<td>".$proMedicinaNuclear."</td>";
		echo "<td>".$proRadiointerven."</td>";
		echo "<td>".$proExhistocom."</td>";
		echo "<td>".$proBuscaorgao."</td>";
		
		echo "</tr>";
	}


echo "</table>";

}

?>