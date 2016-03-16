<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
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

$sqls = array(// Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Atenção Básica
			  0 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15006') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Média Complexidade (SIA+AIH)
			  1 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15007') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (3/6) - SIA+AIH Pago
			  2 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('464') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Alta Complexidade (SIA+AIH)
			  3 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15008','15016') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (4/6) - Alta Complexidade (SIA+AIH)
			  4 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('469') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - FAEC
			  5 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15017') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (4/6) - Alta Complexidade (SIA+AIH)
			  6 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('474') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - IAPI - Incentivo de Assistência a População Indígena
			  7 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15011') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - FIDEPS
			  8 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15010') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Recurso Interministerial MS
			  9 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15012') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Convênios do Fundo Nacional de Saúde - FNS (Recursos Liberados)	, Custeio
			  10 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('477') AND linid IN ('15139') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Convênios do Fundo Nacional de Saúde - FNS (Recursos Liberados)	, Capital
			  11 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('478') AND linid IN ('15139') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Emendas Parlamentares (Recursos Liberados)	, Custeio
			  12 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('477') AND linid IN ('15140') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Emendas Parlamentares (Recursos Liberados)	, Capital
			  13 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('478') AND linid IN ('15140') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Programa Interministerial (Ação 6379), Custeio
			  14 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15141') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Programa Interministerial (Ação 6379), Capital
			  15 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15141') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Custeio
			  16 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15144') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Capital
			  17 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15144') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Custeio
			  18 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15143') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Capital
			  19 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15143') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (2/2)  - Residentes
			  20 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15145') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (1/6) - RJU/CLT-MEC
			  21 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('493') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (1/4) - Receita de Ensino
			  22 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15146') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (2/2)  - Receita Total de Pesquisa Executada via HUF
			  23 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15147') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Aluguéis
			  24 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15155') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Estacionamentos
			  25 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15156') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Doações
			  26 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15157') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (2/4) - Capital
			  27 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15153') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (2/4) - Custeio
			  28 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15154') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Convênios Privados
			  29 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15158') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Pacientes Particulares
			  30 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15159') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Contratos para Execução de SADT não Incluídos na Contratualização
			  31 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15161') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Despesas com Materiais (R$)
			  32 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('487') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Contratos de Serviços - Universidade (R$)
			  33 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('488') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Contratos de Serviços - Hospital (R$)
			  34 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('489') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Despesas de Custeio (R$)
			  35 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('490') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (3/6) - RPA
			  36 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('505') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (1/6) e (3/6) - Tercerizados
			  37 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('507','495') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (3/6) - CLT
			  38 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('503') AND ctiexercicio='{ano}' {perid}",
			  // Força de Trabalho (1/6) - RJU
			  39 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('493') AND ctiexercicio='{ano}' {perid}",
			  // Força de Trabalho (2/6) - Tercerizados: Outras Fontes
			  40 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('501') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - IAC (Incentivo à Contratualização)
			  41 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15009') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Outros Recursos
			  42 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15013') AND ctiexercicio='{ano}' {perid}",
			  
			  
			  //  Dados Financeiros de Entrada - Ministério da Educação (1/2)  - Funcionamento dos Hospitais de Ensino (Ação 4086): Capital
			  43 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15142') AND colid IN ('480') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Entrada - Ministério da Educação (1/2)  - Funcionamento dos Hospitais de Ensino (Ação 4086): Custeio
			  44 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15142') AND colid IN ('479') AND ctiexercicio='{ano}' {perid}",
			  
			  // Dados Financeiros de Saída (5/7) - Equipamento e Material Permanentes
			  45 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15203') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Saída (5/7) - Obras (exceto reformas)
			  46 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15204') AND ctiexercicio='{ano}' {perid}",
			  //  Dívidas (1/2) (2/2)
			  47 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('2037','2038','2039','2030','2040','2031','2041','2032','2042','2033','2043','2034','2035','2036') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (7/7) 
  			  48 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15215') AND ctiexercicio='{ano}' {perid}",
  			   // Dívidas (2/2)
  			  49 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('2017','2027','2018','2028','2019','2029','2020','2021','2022','2023','2024','2025','2016','2026') AND ctiexercicio='{ano}' {perid}"
  			  
			  
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

echo "<h1>Receita Corrente</h1>";
foreach($_ano as $ano) {
		
	echo "<table width='100%'>";
	echo "<tr>";
	echo "<td class='SubTituloCentro' colspan='37'>ANO : ".$ano."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td colspan=3 rowspan=2 class='SubTituloCentro'>&nbsp;</td>";
	echo "<td colspan=14 class='SubTituloCentro'>Dados Financeiros (Entrada) - Ministério da Saúde</td>";
	echo "<td colspan=19 class='SubTituloCentro'>Dados Financeiros (Entrada) - Ministério da Educação</td>";
	echo "<td rowspan=3 class='SubTituloCentro'>Total de Receita Corrente</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td colspan=5 class='SubTituloCentro'>Receita Efetiva SUS (R$) - Assistencial (Aprovado/Pago)</td>";
	echo "<td colspan=6 class='SubTituloCentro'>Incentivos R$</td>";
	echo "<td colspan=3 class='SubTituloCentro'>Recursos de Projetos Específicos - Custeio R$</td>";
	
	
	echo "<td colspan=7 class='SubTituloCentro'>Projetos Específicos - Custeio R$</td>";
	echo "<td class='SubTituloCentro' colspan=3>Outras Fontes R$ - Receita Gerada pela Atividade de Pesquisa</td>";
	echo "<td class='SubTituloCentro' colspan=4>Outras Fontes R$ - Receitas Não-Operacionais R$</td>";
	echo "<td class='SubTituloCentro'>Receitas Capitadas pelas Fundações de Apoio (R$)</td>";
	echo "<td class='SubTituloCentro' colspan=4>Receitas Assistenciais (R$)</td>";
	
	
	echo "</tr>";

	echo "<tr>";
	echo "<td class='SubTituloCentro'>Hospital</td>";
	echo "<td class='SubTituloCentro'>IFES</td>";
	echo "<td class='SubTituloCentro'>Tipo do HUF</td>";
	
	echo "<td class='SubTituloCentro'>Atenção Básica</td>";
	echo "<td class='SubTituloCentro'>Média Complexidade - SIA/SIH</td>";
	echo "<td class='SubTituloCentro'>Alta Complexidade - SIA/SIH</td>";
	echo "<td class='SubTituloCentro'>FAEC</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	
	echo "<td class='SubTituloCentro'>API</td>";
	echo "<td class='SubTituloCentro'>FIDEPS</td>";
	echo "<td class='SubTituloCentro'>IAC</td>";
	echo "<td class='SubTituloCentro'>Recurso Interministerial (MS)</td>";
	echo "<td class='SubTituloCentro'>Outros Recursos</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	
	echo "<td class='SubTituloCentro'>Projetos do FNS</td>";
	echo "<td class='SubTituloCentro'>Emenda Parlamentar (MS)</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	

	echo "<td class='SubTituloCentro'>Programa Interministerial - MEC - Ação 6379</td>";
	echo "<td class='SubTituloCentro'>Funcionamento dos Hospitais de Ensino - Ação 4086</td>";
	echo "<td class='SubTituloCentro'>Emenda Parlamentar - Recursos liberados</td>";
	echo "<td class='SubTituloCentro'>Demais Ações</td>";
	echo "<td class='SubTituloCentro'>Bolsas de Residências</td>";
	echo "<td class='SubTituloCentro'>Pagamento de RJU Ativo</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	
	echo "<td class='SubTituloCentro'>Receitas de Ensino</td>";
	echo "<td class='SubTituloCentro'>Receita Total de Pesquisa Executada via HUF</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	
	echo "<td class='SubTituloCentro'>Aluguéis</td>";
	echo "<td class='SubTituloCentro'>Estacionamento</td>";
	echo "<td class='SubTituloCentro'>Doações</td>";
	echo "<td class='SubTituloCentro'>TOTAL</td>";
	
	echo "<td class='SubTituloCentro'>TOTAL Custeio</td>";
	
  	echo "<td class='SubTituloCentro'>Convênios Privados</td>";
  	echo "<td class='SubTituloCentro'>Pacientes Particulares</td>";
  	echo "<td class='SubTituloCentro'>Contratos para execução de SADT não incluídos na contratualização</td>";
  	echo "<td class='SubTituloCentro'>TOTAL</td>";

	echo "</tr>";
	
	foreach($esuids as $esuid) {
		
		
		/* Atenção Básica */
		$AtencaoBasica[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[0]));
		/* Média Complexidade (SIA+AIH) Pago */
		$MediaComplexidadePg[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[2]));
		/* Alta Complexidade (SIA+AIH) Pago */
		$AltaComplexidadePg[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[4]));
		/* FAEC (SIA+AIH) Pago */
		$FAECPg[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[6]));

		$TotalReceitaSUSPg[$ano] = $AtencaoBasica[$ano]+$MediaComplexidadePg[$ano]+$AltaComplexidadePg[$ano]+$FAECPg[$ano];
		
		
		/* IAPI */
		$IAPI[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[7]));
		/* IAC */
		$IAC[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[41]));
		/* Outros Recursos */
		$OutrosRecursos[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[42]));
		/* FIDEPS */
		$FIDEPS[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[8]));
		/* Interministerial */
		$Interministerial[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[9]));
		
		$TotalIncentivosSaude[$ano] = $IAPI[$ano]+$IAC[$ano]+$OutrosRecursos[$ano]+$FIDEPS[$ano]+$Interministerial[$ano];
		
		/* FNS Custeio */
		$FNSCusteio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[10]));
		/* Emendas Parlamentares Custeio */
		$EmendasParCusteio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[12]));
		
		$TotalRecursosPECusteio[$ano] = $FNSCusteio[$ano]+$EmendasParCusteio[$ano];
		
		
		/* Programa Interministerial (Ação 6379) Custeio */
		$Acao6379Custeio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[14]));
		
		/* Programa Interministerial (Ação 6379) Custeio */
		$Acao4086Custeio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[44]));
					  
		/* Emenda Parlamentar - Recursos liberados Custeio */
		$EmendaParLCusteio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[16]));
		/* Emenda Parlamentar - Recursos liberados Custeio */
		$DemaisAcoesCusteio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[18]));
		/* Residentes */
		$Residentes[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[20]));
		/* RJU */
		$PagamentoRJU[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[21]));
		
		$TotalProjetosEspCusteio[$ano] = $Acao6379Custeio[$ano]+$Acao4086Custeio[$ano]+$EmendaParLCusteio[$ano]+$DemaisAcoesCusteio[$ano]+$Residentes[$ano]+$PagamentoRJU[$ano];
		
		/* Receita de Ensino */
		$ReceitaEnsino[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[22]));
		/* Receita Total de Pesquisa Executada via HUF */
		$PesquisaHUF[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[23]));
		
		$TotalPesquisa[$ano] = $ReceitaEnsino[$ano]+$PesquisaHUF[$ano]; 
		
		
		/* Alugueis */
		$Alugueis[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[24]));
		/* Doações */
		$Doacoes[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[26]));
		/* Estacionamento */
		$Estacionamento[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[25]));
		
		$TotalNaoOperacional[$ano] = $Alugueis[$ano]+$Doacoes[$ano]+$Estacionamento[$ano];
		
		/* Custeio */
		$ApoioCusteio[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[28]));
		
		
		/* Convênios Privados */
		$ConveniosPrivados[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[29]));
		/* Pacientes Particulares */
		$PacientesParticulares[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[30]));
		/* Contratos para Execução de SADT não Incluídos na Contratualização */
		$ContratosSADT[$ano] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($esuid['esuid'],$ano,$perids),$sqls[31]));
		
		$TotalAssistenciais[$ano] = $ConveniosPrivados[$ano]+$PacientesParticulares[$ano]+$ContratosSADT[$ano];
		
		
		$TotalReceitaCorrente[$ano] = $TotalReceitaSUSPg[$ano]+$TotalIncentivosSaude[$ano]+$TotalRecursosPECusteio[$ano]+$TotalProjetosEspCusteio[$ano]+$TotalPesquisa[$ano]+$TotalNaoOperacional[$ano]+$ApoioCusteio[$ano]+$TotalAssistenciais[$ano];
	
	
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
		echo "<td align=right>".(($AtencaoBasica[$ano])?number_format($AtencaoBasica[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($MediaComplexidadePg[$ano])?number_format($MediaComplexidadePg[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($AltaComplexidadePg[$ano])?number_format($AltaComplexidadePg[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($FAECPg[$ano])?number_format($FAECPg[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalReceitaSUSPg[$ano])?number_format($TotalReceitaSUSPg[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($IAPI[$ano])?number_format($IAPI[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($FIDEPS[$ano])?number_format($FIDEPS[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($IAC[$ano])?number_format($IAC[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($Interministerial[$ano])?number_format($Interministerial[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($OutrosRecursos[$ano])?number_format($OutrosRecursos[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalIncentivosSaude[$ano])?number_format($TotalIncentivosSaude[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($FNSCusteio[$ano])?number_format($FNSCusteio[$ano], 2, ',', '.'):"0")."</td>";
		echo "<td align=right>".(($EmendasParCusteio[$ano])?number_format($EmendasParCusteio[$ano], 2, ',', '.'):"0")."</td>";
		echo "<td align=right>".(($TotalRecursosPECusteio[$ano])?number_format($TotalRecursosPECusteio[$ano], 2, ',', '.'):"0")."</td>";
		
		
		
		echo "<td align=right>".(($Acao6379Custeio[$ano])?number_format($Acao6379Custeio[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($Acao4086Custeio[$ano])?number_format($Acao4086Custeio[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($EmendaParLCusteio[$ano])?number_format($EmendaParLCusteio[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($DemaisAcoesCusteio[$ano])?number_format($DemaisAcoesCusteio[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($Residentes[$ano])?number_format($Residentes[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($PagamentoRJU[$ano])?number_format($PagamentoRJU[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalProjetosEspCusteio[$ano])?number_format($TotalProjetosEspCusteio[$ano], 2, ',', '.'):"0,00")."</td>";
		
		
		echo "<td align=right>".(($ReceitaEnsino[$ano])?number_format($ReceitaEnsino[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($PesquisaHUF[$ano])?number_format($PesquisaHUF[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalPesquisa[$ano])?number_format($TotalPesquisa[$ano], 2, ',', '.'):"0,00")."</td>";
		
		
		echo "<td align=right>".(($Alugueis[$ano])?number_format($Alugueis[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($Doacoes[$ano])?number_format($Doacoes[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($Estacionamento[$ano])?number_format($Estacionamento[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalNaoOperacional[$ano])?number_format($TotalNaoOperacional[$ano], 2, ',', '.'):"0,00")."</td>";
		
		echo "<td align=right>".(($ApoioCusteio[$ano])?number_format($ApoioCusteio[$ano], 2, ',', '.'):"0,00")."</td>";
		
		echo "<td align=right>".(($ConveniosPrivados[$ano])?number_format($ConveniosPrivados[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($PacientesParticulares[$ano])?number_format($PacientesParticulares[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($ContratosSADT[$ano])?number_format($ContratosSADT[$ano], 2, ',', '.'):"0,00")."</td>";
		echo "<td align=right>".(($TotalAssistenciais[$ano])?number_format($TotalAssistenciais[$ano], 2, ',', '.'):"0,00")."</td>";
		
		echo "<td align=right>".(($TotalReceitaCorrente[$ano])?number_format($TotalReceitaCorrente[$ano], 2, ',', '.'):"0,00")."</td>";
		
		echo "</tr>";
	}


echo "</table>";

}

?>