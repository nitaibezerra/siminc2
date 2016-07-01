<?

$sqls = array(// RECEITA : Grupo Quadro de Pessoal (Universidade)
			  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('335','337','339','341','343') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receita Efetiva (gitid=45)
			  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','477','479','480','481') AND ctiexercicio='{ano}'",
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
			  15 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('475','476','478','482') AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Programa Interministerial - MEC (gitid=33) : somente linha capital
			  16 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='407' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas da Universidade (gitid=35) : somente linha Capital
			  17 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='411' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Receitas Oriundas das Fundações de Apoio (gitid=34) : somente linha capital
			  18 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='409' AND ctiexercicio='{ano}'",
			  // RECEITA : Grupo Quadro de Pessoal (Universidade) : somente RJU
			  19 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='14447' AND ctiexercicio='{ano}'",
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
			  26 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='14447' AND ctiexercicio='{ano}'",
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
			  43 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('431') AND ctiexercicio='{ano}'",
			  // ...
			  44 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('215') AND ctiexercicio='{ano}'",
			  // ...
			  45 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('216') AND ctiexercicio='{ano}'",
			  // ...
			  46 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('217') AND ctiexercicio='{ano}'",
			  // ...
			  47 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('352') AND ctiexercicio='{ano}'"
			  
			  );
			  
/* Programa Interministerial MEC (capital) */
$ReceitaProgramaInterministerialCapital['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[16]));
$ReceitaProgramaInterministerialCapital['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[16]));
$ReceitaProgramaInterministerialCapital['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[16]));
$ReceitaProgramaInterministerialCapital['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[16]));
$ReceitaProgramaInterministerialCapital['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[16]));
/* Receita de capital oriunda da universidade */
$ReceitaCapitalUniversidade['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[17]));
$ReceitaCapitalUniversidade['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[17]));
$ReceitaCapitalUniversidade['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[17]));
$ReceitaCapitalUniversidade['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[17]));
$ReceitaCapitalUniversidade['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[17]));
/* Receita de capital oriunda da fundação */
$ReceitaCapitalFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[18]));
$ReceitaCapitalFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[18]));
$ReceitaCapitalFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[18]));
$ReceitaCapitalFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[18]));
$ReceitaCapitalFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[18]));
/* Receita Efetiva SUS */
$ReceitaEfetivaSUS['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[1]));
$ReceitaEfetivaSUS['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[1]));
$ReceitaEfetivaSUS['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[1]));
$ReceitaEfetivaSUS['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[1]));
$ReceitaEfetivaSUS['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[1]));
/* Incentivos (FIDEPS +ICG +IMMS) */
$ReceitaIncentivos['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[15]));
$ReceitaIncentivos['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[15]));
$ReceitaIncentivos['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[15]));
$ReceitaIncentivos['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[15]));
$ReceitaIncentivos['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[15]));
/* Programa Interministerial MEC (custeio) */
$ReceitaProgramaInterministerialCusteio['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[2]));
$ReceitaProgramaInterministerialCusteio['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[2]));
$ReceitaProgramaInterministerialCusteio['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[2]));
$ReceitaProgramaInterministerialCusteio['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[2]));
$ReceitaProgramaInterministerialCusteio['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[2]));
/* Receita de custeio da Universidade */
$ReceitaCusteioUniversidade['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[3]));
$ReceitaCusteioUniversidade['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[3]));
$ReceitaCusteioUniversidade['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[3]));
$ReceitaCusteioUniversidade['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[3]));
$ReceitaCusteioUniversidade['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[3]));
/* Receita de custeio da Fundação */
$ReceitaCusteioFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[4]));
$ReceitaCusteioFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[4]));
$ReceitaCusteioFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[4]));
$ReceitaCusteioFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[4]));
$ReceitaCusteioFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[4]));
/* Receita Não Operacional */
$ReceitaNaoOperacional['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[5]));
$ReceitaNaoOperacional['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[5]));
$ReceitaNaoOperacional['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[5]));
$ReceitaNaoOperacional['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[5]));
$ReceitaNaoOperacional['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[5]));
/* Receita Assistencial */
$ReceitaAssistencial['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[6]));
$ReceitaAssistencial['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[6]));
$ReceitaAssistencial['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[6]));
$ReceitaAssistencial['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[6]));
$ReceitaAssistencial['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[6]));
/* Bolsas de Residência */
$ReceitaBolsasResidencia['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[7]));
$ReceitaBolsasResidencia['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[7]));
$ReceitaBolsasResidencia['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[7]));
$ReceitaBolsasResidencia['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[7]));
$ReceitaBolsasResidencia['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[7]));
/* Receita para pagamento RJU */
$ReceitaPagamentoRJU['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[19]));
$ReceitaPagamentoRJU['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[19]));
$ReceitaPagamentoRJU['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[19]));
$ReceitaPagamentoRJU['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[19]));
$ReceitaPagamentoRJU['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[19]));

/*
 * SOLICITAÇÃO FEITA PELO DR. CELSO  NO DIA 13/05/2009
 - RETIRAR AS LINHAS
 	@ Receita para pagamento CLT;
 	@ Receita para pagamento SSPE;
 	@ Receita para pagamento Terceirizado;
 	@ Receita para pagamento Requisitados;
 *  
 */
/* Receita para pagamento CLT */
/*
$ReceitaPagamentoCLT['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[20]));
$ReceitaPagamentoCLT['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[20]));
$ReceitaPagamentoCLT['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[20]));
$ReceitaPagamentoCLT['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[20]));
$ReceitaPagamentoCLT['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[20]));
*/
/* Receita para pagamento SSPE */
/*
$ReceitaPagamentoSSPE['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[21]));
$ReceitaPagamentoSSPE['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[21]));
$ReceitaPagamentoSSPE['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[21]));
$ReceitaPagamentoSSPE['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[21]));
$ReceitaPagamentoSSPE['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[21]));
*/
/* Receita para pagamento Terceirizados */
/*
$ReceitaPagamentoTerceirizados['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[22]));
$ReceitaPagamentoTerceirizados['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[22]));
$ReceitaPagamentoTerceirizados['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[22]));
$ReceitaPagamentoTerceirizados['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[22]));
$ReceitaPagamentoTerceirizados['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[22]));
*
/* Receita para pagamento Requisitados */
/*
$ReceitaPagamentoRequisitados['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[23]));
$ReceitaPagamentoRequisitados['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[23]));
$ReceitaPagamentoRequisitados['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[23]));
$ReceitaPagamentoRequisitados['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[23]));
$ReceitaPagamentoRequisitados['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[23]));
*/

/* Projetos Específicos */
$ReceitaProjetosEspecificos['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[24]));
$ReceitaProjetosEspecificos['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[24]));
$ReceitaProjetosEspecificos['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[24]));
$ReceitaProjetosEspecificos['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[24]));
$ReceitaProjetosEspecificos['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[24]));
/* Receita gerada pela pesquisa e ensino */
$ReceitaPesquisaEnsino['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[25]));
$ReceitaPesquisaEnsino['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[25]));
$ReceitaPesquisaEnsino['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[25]));
$ReceitaPesquisaEnsino['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[25]));
$ReceitaPesquisaEnsino['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[25]));
/* Total da receita de capital */
$TotalReceitaCapital['2004'] = $ReceitaProgramaInterministerialCapital['2004']+$ReceitaCapitalUniversidade['2004']+$ReceitaCapitalFundacao['2004'];
$TotalReceitaCapital['2005'] = $ReceitaProgramaInterministerialCapital['2005']+$ReceitaCapitalUniversidade['2005']+$ReceitaCapitalFundacao['2005'];
$TotalReceitaCapital['2006'] = $ReceitaProgramaInterministerialCapital['2006']+$ReceitaCapitalUniversidade['2006']+$ReceitaCapitalFundacao['2006'];
$TotalReceitaCapital['2007'] = $ReceitaProgramaInterministerialCapital['2007']+$ReceitaCapitalUniversidade['2007']+$ReceitaCapitalFundacao['2007'];
$TotalReceitaCapital['2008'] = $ReceitaProgramaInterministerialCapital['2008']+$ReceitaCapitalUniversidade['2008']+$ReceitaCapitalFundacao['2008'];
/* Total da receita de corrente */
$TotalReceitaCorrente['2004'] = $ReceitaEfetivaSUS['2004']+$ReceitaIncentivos['2004']+$ReceitaProgramaInterministerialCusteio['2004']+$ReceitaCusteioUniversidade['2004']+$ReceitaCusteioFundacao['2004']+$ReceitaNaoOperacional['2004']+$ReceitaAssistencial['2004']+$ReceitaBolsasResidencia['2004']+$ReceitaPagamentoRJU['2004'];
$TotalReceitaCorrente['2005'] = $ReceitaEfetivaSUS['2005']+$ReceitaIncentivos['2005']+$ReceitaProgramaInterministerialCusteio['2005']+$ReceitaCusteioUniversidade['2005']+$ReceitaCusteioFundacao['2005']+$ReceitaNaoOperacional['2005']+$ReceitaAssistencial['2005']+$ReceitaBolsasResidencia['2005']+$ReceitaPagamentoRJU['2005'];
$TotalReceitaCorrente['2006'] = $ReceitaEfetivaSUS['2006']+$ReceitaIncentivos['2006']+$ReceitaProgramaInterministerialCusteio['2006']+$ReceitaCusteioUniversidade['2006']+$ReceitaCusteioFundacao['2006']+$ReceitaNaoOperacional['2006']+$ReceitaAssistencial['2006']+$ReceitaBolsasResidencia['2006']+$ReceitaPagamentoRJU['2006'];
$TotalReceitaCorrente['2007'] = $ReceitaEfetivaSUS['2007']+$ReceitaIncentivos['2007']+$ReceitaProgramaInterministerialCusteio['2007']+$ReceitaCusteioUniversidade['2007']+$ReceitaCusteioFundacao['2007']+$ReceitaNaoOperacional['2007']+$ReceitaAssistencial['2007']+$ReceitaBolsasResidencia['2007']+$ReceitaPagamentoRJU['2007'];
$TotalReceitaCorrente['2008'] = $ReceitaEfetivaSUS['2008']+$ReceitaIncentivos['2008']+$ReceitaProgramaInterministerialCusteio['2008']+$ReceitaCusteioUniversidade['2008']+$ReceitaCusteioFundacao['2008']+$ReceitaNaoOperacional['2008']+$ReceitaAssistencial['2008']+$ReceitaBolsasResidencia['2008']+$ReceitaPagamentoRJU['2008'];
/*
$TotalReceitaCorrente['2004'] = $ReceitaEfetivaSUS['2004']+$ReceitaIncentivos['2004']+$ReceitaProgramaInterministerialCusteio['2004']+$ReceitaCusteioUniversidade['2004']+$ReceitaCusteioFundacao['2004']+$ReceitaNaoOperacional['2004']+$ReceitaAssistencial['2004']+$ReceitaBolsasResidencia['2004']+$ReceitaPagamentoRJU['2004']+$ReceitaPagamentoCLT['2004']+$ReceitaPagamentoSSPE['2004']+$ReceitaPagamentoTerceirizados['2004']+$ReceitaPagamentoRequisitados['2004'];
$TotalReceitaCorrente['2005'] = $ReceitaEfetivaSUS['2005']+$ReceitaIncentivos['2005']+$ReceitaProgramaInterministerialCusteio['2005']+$ReceitaCusteioUniversidade['2005']+$ReceitaCusteioFundacao['2005']+$ReceitaNaoOperacional['2005']+$ReceitaAssistencial['2005']+$ReceitaBolsasResidencia['2005']+$ReceitaPagamentoRJU['2005']+$ReceitaPagamentoCLT['2005']+$ReceitaPagamentoSSPE['2005']+$ReceitaPagamentoTerceirizados['2005']+$ReceitaPagamentoRequisitados['2005'];
$TotalReceitaCorrente['2006'] = $ReceitaEfetivaSUS['2006']+$ReceitaIncentivos['2006']+$ReceitaProgramaInterministerialCusteio['2006']+$ReceitaCusteioUniversidade['2006']+$ReceitaCusteioFundacao['2006']+$ReceitaNaoOperacional['2006']+$ReceitaAssistencial['2006']+$ReceitaBolsasResidencia['2006']+$ReceitaPagamentoRJU['2006']+$ReceitaPagamentoCLT['2006']+$ReceitaPagamentoSSPE['2006']+$ReceitaPagamentoTerceirizados['2006']+$ReceitaPagamentoRequisitados['2006'];
$TotalReceitaCorrente['2007'] = $ReceitaEfetivaSUS['2007']+$ReceitaIncentivos['2007']+$ReceitaProgramaInterministerialCusteio['2007']+$ReceitaCusteioUniversidade['2007']+$ReceitaCusteioFundacao['2007']+$ReceitaNaoOperacional['2007']+$ReceitaAssistencial['2007']+$ReceitaBolsasResidencia['2007']+$ReceitaPagamentoRJU['2007']+$ReceitaPagamentoCLT['2007']+$ReceitaPagamentoSSPE['2007']+$ReceitaPagamentoTerceirizados['2007']+$ReceitaPagamentoRequisitados['2007'];
$TotalReceitaCorrente['2008'] = $ReceitaEfetivaSUS['2008']+$ReceitaIncentivos['2008']+$ReceitaProgramaInterministerialCusteio['2008']+$ReceitaCusteioUniversidade['2008']+$ReceitaCusteioFundacao['2008']+$ReceitaNaoOperacional['2008']+$ReceitaAssistencial['2008']+$ReceitaBolsasResidencia['2008']+$ReceitaPagamentoRJU['2008']+$ReceitaPagamentoCLT['2008']+$ReceitaPagamentoSSPE['2008']+$ReceitaPagamentoTerceirizados['2008']+$ReceitaPagamentoRequisitados['2008'];
*/

/* Total da receita não-classificada */
$TotalReceitaNaoClassificada['2004'] = $ReceitaProjetosEspecificos['2004']+$ReceitaPesquisaEnsino['2004'];
$TotalReceitaNaoClassificada['2005'] = $ReceitaProjetosEspecificos['2005']+$ReceitaPesquisaEnsino['2005'];
$TotalReceitaNaoClassificada['2006'] = $ReceitaProjetosEspecificos['2006']+$ReceitaPesquisaEnsino['2006'];
$TotalReceitaNaoClassificada['2007'] = $ReceitaProjetosEspecificos['2007']+$ReceitaPesquisaEnsino['2007'];
$TotalReceitaNaoClassificada['2008'] = $ReceitaProjetosEspecificos['2008']+$ReceitaPesquisaEnsino['2008'];
/* Receita total */
$TotalReceita['2004'] = $TotalReceitaCapital['2004']+$TotalReceitaCorrente['2004']+$TotalReceitaNaoClassificada['2004'];
$TotalReceita['2005'] = $TotalReceitaCapital['2005']+$TotalReceitaCorrente['2005']+$TotalReceitaNaoClassificada['2005'];
$TotalReceita['2006'] = $TotalReceitaCapital['2006']+$TotalReceitaCorrente['2006']+$TotalReceitaNaoClassificada['2006'];
$TotalReceita['2007'] = $TotalReceitaCapital['2007']+$TotalReceitaCorrente['2007']+$TotalReceitaNaoClassificada['2007'];
$TotalReceita['2008'] = $TotalReceitaCapital['2008']+$TotalReceitaCorrente['2008']+$TotalReceitaNaoClassificada['2008'];


/* Depesa Materiais Universidade */
$DespesaMateriaisUniversidade['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[8]));
$DespesaMateriaisUniversidade['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[8]));
$DespesaMateriaisUniversidade['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[8]));
$DespesaMateriaisUniversidade['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[8]));
$DespesaMateriaisUniversidade['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[8]));
/* Depesa Materiais Fundação */
$DespesaMateriaisFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[9]));
$DespesaMateriaisFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[9]));
$DespesaMateriaisFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[9]));
$DespesaMateriaisFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[9]));
$DespesaMateriaisFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[9]));
/* Depesa Serviços Universidade */
$DespesaServicosUniversidade['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[10]));
$DespesaServicosUniversidade['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[10]));
$DespesaServicosUniversidade['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[10]));
$DespesaServicosUniversidade['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[10]));
$DespesaServicosUniversidade['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[10]));
/* Depesa Serviços Fundação */
$DespesaServicosFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[12]));
$DespesaServicosFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[12]));
$DespesaServicosFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[12]));
$DespesaServicosFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[12]));
$DespesaServicosFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[12]));
/* Despesa para pagamento RJU */
$DespesaPagamentoRJU['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[26]));
$DespesaPagamentoRJU['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[26]));
$DespesaPagamentoRJU['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[26]));
$DespesaPagamentoRJU['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[26]));
$DespesaPagamentoRJU['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[26]));
/* Despesa para pagamento CLT */
$DespesaPagamentoCLT['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[27]));
$DespesaPagamentoCLT['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[27]));
$DespesaPagamentoCLT['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[27]));
$DespesaPagamentoCLT['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[27]));
$DespesaPagamentoCLT['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[27]));
/* Despesa para pagamento SSPE */
$DespesaPagamentoSSPE['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[28]));
$DespesaPagamentoSSPE['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[28]));
$DespesaPagamentoSSPE['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[28]));
$DespesaPagamentoSSPE['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[28]));
$DespesaPagamentoSSPE['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[28]));
/* Despesa para pagamento Terceirizado */
$DespesaPagamentoTerceirizado['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[29]));
$DespesaPagamentoTerceirizado['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[29]));
$DespesaPagamentoTerceirizado['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[29]));
$DespesaPagamentoTerceirizado['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[29]));
$DespesaPagamentoTerceirizado['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[29]));
/* Despesa para pagamento Requisitado */
$DespesaPagamentoRequisitado['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[30]));
$DespesaPagamentoRequisitado['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[30]));
$DespesaPagamentoRequisitado['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[30]));
$DespesaPagamentoRequisitado['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[30]));
$DespesaPagamentoRequisitado['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[30]));
/* Despesa para Pagamento da bolsas de residência médica */
$DespesaBolsaMedica['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[11]));
$DespesaBolsaMedica['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[11]));
$DespesaBolsaMedica['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[11]));
$DespesaBolsaMedica['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[11]));
$DespesaBolsaMedica['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[11]));
/* Despesa para Despesa para pagamento CLT(fundação) */
$DespesaPagamentoCLTFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[31]));
$DespesaPagamentoCLTFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[31]));
$DespesaPagamentoCLTFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[31]));
$DespesaPagamentoCLTFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[31]));
$DespesaPagamentoCLTFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[31]));
/* Despesa para Despesa para pagamento RPA(fundação) */
$DespesaPagamentoRPAFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[32]));
$DespesaPagamentoRPAFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[32]));
$DespesaPagamentoRPAFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[32]));
$DespesaPagamentoRPAFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[32]));
$DespesaPagamentoRPAFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[32]));
/* Despesa para Despesa para pagamento RPA(fundação) */
$DespesaPagamentoTerceirizadoFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[33]));
$DespesaPagamentoTerceirizadoFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[33]));
$DespesaPagamentoTerceirizadoFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[33]));
$DespesaPagamentoTerceirizadoFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[33]));
$DespesaPagamentoTerceirizadoFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[33]));
/* .... */
$DespesaUniversidadeSUSCLT['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[36]));
$DespesaUniversidadeSUSCLT['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[36]));
$DespesaUniversidadeSUSCLT['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[36]));
$DespesaUniversidadeSUSCLT['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[36]));
$DespesaUniversidadeSUSCLT['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[36]));
/* .... */
$DespesaUniversidadeSUSSSPE['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[37]));
$DespesaUniversidadeSUSSSPE['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[37]));
$DespesaUniversidadeSUSSSPE['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[37]));
$DespesaUniversidadeSUSSSPE['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[37]));
$DespesaUniversidadeSUSSSPE['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[37]));
/* .... */
$DespesaUniversidadeSUSTercerizados['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[38]));
$DespesaUniversidadeSUSTercerizados['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[38]));
$DespesaUniversidadeSUSTercerizados['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[38]));
$DespesaUniversidadeSUSTercerizados['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[38]));
$DespesaUniversidadeSUSTercerizados['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[38]));
/* .... */
$DespesaOutrasCBolsa['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[41]));
$DespesaOutrasCBolsa['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[41]));
$DespesaOutrasCBolsa['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[41]));
$DespesaOutrasCBolsa['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[41]));
$DespesaOutrasCBolsa['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[41]));
/* .... */
$DespesaOutrasCSentenca['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[42]));
$DespesaOutrasCSentenca['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[42]));
$DespesaOutrasCSentenca['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[42]));
$DespesaOutrasCSentenca['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[42]));
$DespesaOutrasCSentenca['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[42]));
/* .... */
$DespesaOutrasCOutras['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[43]));
$DespesaOutrasCOutras['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[43]));
$DespesaOutrasCOutras['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[43]));
$DespesaOutrasCOutras['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[43]));
$DespesaOutrasCOutras['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[43]));
/* Total de outras despesas */
$TotalOutrasDespesas['2004'] = $DespesaOutrasCBolsa['2004']+$DespesaOutrasCSentenca['2004']+$DespesaOutrasCOutras['2004'];
$TotalOutrasDespesas['2005'] = $DespesaOutrasCBolsa['2005']+$DespesaOutrasCSentenca['2005']+$DespesaOutrasCOutras['2005'];
$TotalOutrasDespesas['2006'] = $DespesaOutrasCBolsa['2006']+$DespesaOutrasCSentenca['2006']+$DespesaOutrasCOutras['2006'];
$TotalOutrasDespesas['2007'] = $DespesaOutrasCBolsa['2007']+$DespesaOutrasCSentenca['2007']+$DespesaOutrasCOutras['2007'];
$TotalOutrasDespesas['2008'] = $DespesaOutrasCBolsa['2008']+$DespesaOutrasCSentenca['2008']+$DespesaOutrasCOutras['2008'];

/* Total das despesas com materiais */
$TotalDespesaMateriais['2004'] = $DespesaMateriaisUniversidade['2004']+$DespesaMateriaisFundacao['2004'];
$TotalDespesaMateriais['2005'] = $DespesaMateriaisUniversidade['2005']+$DespesaMateriaisFundacao['2005'];
$TotalDespesaMateriais['2006'] = $DespesaMateriaisUniversidade['2006']+$DespesaMateriaisFundacao['2006'];
$TotalDespesaMateriais['2007'] = $DespesaMateriaisUniversidade['2007']+$DespesaMateriaisFundacao['2007'];
$TotalDespesaMateriais['2008'] = $DespesaMateriaisUniversidade['2008']+$DespesaMateriaisFundacao['2008'];
/* Total das despesas com serviços */
$TotalDespesaServicos['2004'] = $DespesaServicosUniversidade['2004']+$DespesaServicosFundacao['2004'];
$TotalDespesaServicos['2005'] = $DespesaServicosUniversidade['2005']+$DespesaServicosFundacao['2005'];
$TotalDespesaServicos['2006'] = $DespesaServicosUniversidade['2006']+$DespesaServicosFundacao['2006'];
$TotalDespesaServicos['2007'] = $DespesaServicosUniversidade['2007']+$DespesaServicosFundacao['2007'];
$TotalDespesaServicos['2008'] = $DespesaServicosUniversidade['2008']+$DespesaServicosFundacao['2008'];
/* Total das despesas de pessoal */
$TotalDespesaPessoal['2004'] = $DespesaPagamentoRJU['2004']+$DespesaPagamentoCLT['2004']+$DespesaPagamentoSSPE['2004']+$DespesaPagamentoTerceirizado['2004']+$DespesaPagamentoRequisitado['2004']+$DespesaBolsaMedica['2004']+$DespesaPagamentoCLTFundacao['2004']+$DespesaPagamentoRPAFundacao['2004']+$DespesaPagamentoTerceirizadoFundacao['2004']+$DespesaUniversidadeSUSCLT['2004']+$DespesaUniversidadeSUSSSPE['2004']+$DespesaUniversidadeSUSTercerizados['2004'];
$TotalDespesaPessoal['2005'] = $DespesaPagamentoRJU['2005']+$DespesaPagamentoCLT['2005']+$DespesaPagamentoSSPE['2005']+$DespesaPagamentoTerceirizado['2005']+$DespesaPagamentoRequisitado['2005']+$DespesaBolsaMedica['2005']+$DespesaPagamentoCLTFundacao['2005']+$DespesaPagamentoRPAFundacao['2005']+$DespesaPagamentoTerceirizadoFundacao['2005']+$DespesaUniversidadeSUSCLT['2005']+$DespesaUniversidadeSUSSSPE['2005']+$DespesaUniversidadeSUSTercerizados['2005'];
$TotalDespesaPessoal['2006'] = $DespesaPagamentoRJU['2006']+$DespesaPagamentoCLT['2006']+$DespesaPagamentoSSPE['2006']+$DespesaPagamentoTerceirizado['2006']+$DespesaPagamentoRequisitado['2006']+$DespesaBolsaMedica['2006']+$DespesaPagamentoCLTFundacao['2006']+$DespesaPagamentoRPAFundacao['2006']+$DespesaPagamentoTerceirizadoFundacao['2006']+$DespesaUniversidadeSUSCLT['2006']+$DespesaUniversidadeSUSSSPE['2006']+$DespesaUniversidadeSUSTercerizados['2006'];
$TotalDespesaPessoal['2007'] = $DespesaPagamentoRJU['2007']+$DespesaPagamentoCLT['2007']+$DespesaPagamentoSSPE['2007']+$DespesaPagamentoTerceirizado['2007']+$DespesaPagamentoRequisitado['2007']+$DespesaBolsaMedica['2007']+$DespesaPagamentoCLTFundacao['2007']+$DespesaPagamentoRPAFundacao['2007']+$DespesaPagamentoTerceirizadoFundacao['2007']+$DespesaUniversidadeSUSCLT['2007']+$DespesaUniversidadeSUSSSPE['2007']+$DespesaUniversidadeSUSTercerizados['2007'];
$TotalDespesaPessoal['2008'] = $DespesaPagamentoRJU['2008']+$DespesaPagamentoCLT['2008']+$DespesaPagamentoSSPE['2008']+$DespesaPagamentoTerceirizado['2008']+$DespesaPagamentoRequisitado['2008']+$DespesaBolsaMedica['2008']+$DespesaPagamentoCLTFundacao['2008']+$DespesaPagamentoRPAFundacao['2008']+$DespesaPagamentoTerceirizadoFundacao['2008']+$DespesaUniversidadeSUSCLT['2008']+$DespesaUniversidadeSUSSSPE['2008']+$DespesaUniversidadeSUSTercerizados['2008'];


/* .... */
$DespesaCapitalEquipMat['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[39]));
$DespesaCapitalEquipMat['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[39]));
$DespesaCapitalEquipMat['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[39]));
$DespesaCapitalEquipMat['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[39]));
$DespesaCapitalEquipMat['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[39]));

/* .... */
$DespesaCapitalObras['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[40]));
$DespesaCapitalObras['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[40]));
$DespesaCapitalObras['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[40]));
$DespesaCapitalObras['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[40]));
$DespesaCapitalObras['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[40]));



/* Total das despesas */
$TotalDespesa['2004'] = $TotalDespesaMateriais['2004']+$TotalDespesaServicos['2004']+$TotalDespesaPessoal['2004']+$TotalOutrasDespesas['2004'];
$TotalDespesa['2005'] = $TotalDespesaMateriais['2005']+$TotalDespesaServicos['2005']+$TotalDespesaPessoal['2005']+$TotalOutrasDespesas['2005'];
$TotalDespesa['2006'] = $TotalDespesaMateriais['2006']+$TotalDespesaServicos['2006']+$TotalDespesaPessoal['2006']+$TotalOutrasDespesas['2006'];
$TotalDespesa['2007'] = $TotalDespesaMateriais['2007']+$TotalDespesaServicos['2007']+$TotalDespesaPessoal['2007']+$TotalOutrasDespesas['2007'];
$TotalDespesa['2008'] = $TotalDespesaMateriais['2008']+$TotalDespesaServicos['2008']+$TotalDespesaPessoal['2008']+$TotalOutrasDespesas['2008'];
/*  */
$DividasUniversidade['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[34]));
$DividasUniversidade['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[34]));
$DividasUniversidade['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[34]));
$DividasUniversidade['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[34]));
$DividasUniversidade['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[34]));
/*  */
$DividasFundacao['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[35]));
$DividasFundacao['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[35]));
$DividasFundacao['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[35]));
$DividasFundacao['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[35]));
$DividasFundacao['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[35]));

/* Deficit ou saldo corrente */
$SaldoCorrente['2004'] = $TotalReceitaCorrente['2004']-$TotalDespesa['2004'];
$SaldoCorrente['2005'] = $TotalReceitaCorrente['2005']-$TotalDespesa['2005'];
$SaldoCorrente['2006'] = $TotalReceitaCorrente['2006']-$TotalDespesa['2006'];
$SaldoCorrente['2007'] = $TotalReceitaCorrente['2007']-$TotalDespesa['2007'];
$SaldoCorrente['2008'] = $TotalReceitaCorrente['2008']-$TotalDespesa['2008'];

/* .... */
$SaldoCapital['2004'] = $DespesaCapitalEquipMat['2004']+$DespesaCapitalObras['2004'];
$SaldoCapital['2005'] = $DespesaCapitalEquipMat['2005']+$DespesaCapitalObras['2005'];
$SaldoCapital['2006'] = $DespesaCapitalEquipMat['2006']+$DespesaCapitalObras['2006'];
$SaldoCapital['2007'] = $DespesaCapitalEquipMat['2007']+$DespesaCapitalObras['2007'];
$SaldoCapital['2008'] = $DespesaCapitalEquipMat['2008']+$DespesaCapitalObras['2008'];
/* Deficit ou saldo */
$SaldoTotal['2004'] = $TotalReceita['2004']-$TotalDespesa['2004']-$SaldoCapital['2004'];
$SaldoTotal['2005'] = $TotalReceita['2005']-$TotalDespesa['2005']-$SaldoCapital['2005'];
$SaldoTotal['2006'] = $TotalReceita['2006']-$TotalDespesa['2006']-$SaldoCapital['2006'];
$SaldoTotal['2007'] = $TotalReceita['2007']-$TotalDespesa['2007']-$SaldoCapital['2007'];
$SaldoTotal['2008'] = $TotalReceita['2008']-$TotalDespesa['2008']-$SaldoCapital['2008'];


/* "Contratualizado" */
$RecContratualizado['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[44]));
$RecContratualizado['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[44]));
$RecContratualizado['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[44]));
$RecContratualizado['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[44]));
$RecContratualizado['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[44]));
/* "Apresentado" */
$RecApresentado['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[45]));
$RecApresentado['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[45]));
$RecApresentado['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[45]));
$RecApresentado['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[45]));
$RecApresentado['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[45]));
/* "Aprovado/Pago" */
$RecAprovadopago['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[46]));
$RecAprovadopago['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[46]));
$RecAprovadopago['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[46]));
$RecAprovadopago['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[46]));
$RecAprovadopago['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[46]));
/* ""Produzido"" */
$RecProduzido['2004'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2004'),$sqls[47]));
$RecProduzido['2005'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2005'),$sqls[47]));
$RecProduzido['2006'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2006'),$sqls[47]));
$RecProduzido['2007'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2007'),$sqls[47]));
$RecProduzido['2008'] = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],'2008'),$sqls[47]));



?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td colspan="11" class="SubTituloCentro">Anos</td>
</tr>
<tr>
	<td class="SubTituloCentro">Item</td>
	<td class="SubTituloCentro">2004</td>
	<td class="SubTituloCentro">%</td>
	<td class="SubTituloCentro">2005</td>
	<td class="SubTituloCentro">%</td>
	<td class="SubTituloCentro">2006</td>
	<td class="SubTituloCentro">%</td>
	<td class="SubTituloCentro">2007</td>
	<td class="SubTituloCentro">%</td>
	<td class="SubTituloCentro">2008</td>
	<td class="SubTituloCentro">%</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Receita de capital</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Programa Interministerial MEC (capital) :</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCapital['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCapital['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCapital['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCapital['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCapital['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita de capital oriunda da universidade :</td>
	<td align="right"><? echo number_format($ReceitaCapitalUniversidade['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalUniversidade['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalUniversidade['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalUniversidade['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalUniversidade['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita de capital oriunda da fundação :</td>
	<td align="right"><? echo number_format($ReceitaCapitalFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCapitalFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Receita corrente</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita Efetiva SUS :</td>
	<td align="right"><? echo number_format($ReceitaEfetivaSUS['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaEfetivaSUS['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaEfetivaSUS['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaEfetivaSUS['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaEfetivaSUS['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Incentivos (FIDEPS +ICG +IMMS) :</td>
	<td align="right"><? echo number_format($ReceitaIncentivos['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaIncentivos['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaIncentivos['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaIncentivos['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaIncentivos['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Programa Interministerial MEC (custeio) :</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCusteio['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCusteio['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCusteio['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCusteio['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProgramaInterministerialCusteio['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita de custeio da Universidade :</td>
	<td align="right"><? echo number_format($ReceitaCusteioUniversidade['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioUniversidade['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioUniversidade['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioUniversidade['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioUniversidade['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita de custeio da Fundação :</td>
	<td align="right"><? echo number_format($ReceitaCusteioFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaCusteioFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita Não Operacional :</td>
	<td align="right"><? echo number_format($ReceitaNaoOperacional['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaNaoOperacional['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaNaoOperacional['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaNaoOperacional['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaNaoOperacional['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita Assistencial não SUS:</td>
	<td align="right"><? echo number_format($ReceitaAssistencial['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaAssistencial['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaAssistencial['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaAssistencial['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaAssistencial['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Bolsas de Residência :</td>
	<td align="right"><? echo number_format($ReceitaBolsasResidencia['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaBolsasResidencia['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaBolsasResidencia['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaBolsasResidencia['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaBolsasResidencia['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita para pagamento RJU :</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRJU['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRJU['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRJU['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRJU['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRJU['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<!-- 
SOLICITAÇÃO FEITA PELO DR. CELSO  NO DIA 13/05/2009
 - RETIRAR AS LINHAS
 	* Receita para pagamento CLT;
 	* Receita para pagamento SSPE;
 	* Receita para pagamento Terceirizado;
 	* Receita para pagamento Requisitados;

<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita para pagamento CLT :</td>
	<td align="right"><? echo number_format($ReceitaPagamentoCLT['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoCLT['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoCLT['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoCLT['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoCLT['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita para pagamento SSPE :</td>
	<td align="right"><? echo number_format($ReceitaPagamentoSSPE['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoSSPE['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoSSPE['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoSSPE['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoSSPE['2008'], 2, ',', '.'); ?></td>
	<td>-</td>

</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita para pagamento Terceirizado :</td>
	<td align="right"><? echo number_format($ReceitaPagamentoTerceirizados['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoTerceirizados['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoTerceirizados['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoTerceirizados['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoTerceirizados['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita para pagamento Requisitados :</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRequisitados['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRequisitados['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRequisitados['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRequisitados['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPagamentoRequisitados['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
-->
<tr>
	<td colspan="11" class="SubTituloEsquerda">Receita não-classificada</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Projetos Específicos :</td>
	<td align="right"><? echo number_format($ReceitaProjetosEspecificos['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProjetosEspecificos['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProjetosEspecificos['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProjetosEspecificos['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaProjetosEspecificos['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Receita gerada pela pesquisa e ensino :</td>
	<td align="right"><? echo number_format($ReceitaPesquisaEnsino['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPesquisaEnsino['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPesquisaEnsino['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPesquisaEnsino['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($ReceitaPesquisaEnsino['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Totais receita</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(R1)</font> Total da receita de capital :</b></td>
	<td align="right"><b><? echo number_format($TotalReceitaCapital['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2004'])?round(($TotalReceitaCapital['2004']/$TotalReceita['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCapital['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2005'])?round(($TotalReceitaCapital['2005']/$TotalReceita['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCapital['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2006'])?round(($TotalReceitaCapital['2006']/$TotalReceita['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCapital['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2007'])?round(($TotalReceitaCapital['2007']/$TotalReceita['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCapital['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2008'])?round(($TotalReceitaCapital['2008']/$TotalReceita['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(R2)</font> Total da receita de corrente :</b></td>
	<td align="right"><b><? echo number_format($TotalReceitaCorrente['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2004'])?round(($TotalReceitaCorrente['2004']/$TotalReceita['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCorrente['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2005'])?round(($TotalReceitaCorrente['2005']/$TotalReceita['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCorrente['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2006'])?round(($TotalReceitaCorrente['2006']/$TotalReceita['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCorrente['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2007'])?round(($TotalReceitaCorrente['2007']/$TotalReceita['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaCorrente['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2008'])?round(($TotalReceitaCorrente['2008']/$TotalReceita['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(R3)</font> Total da receita não-classificada :</b></td>
	<td align="right"><b><? echo number_format($TotalReceitaNaoClassificada['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2004'])?round(($TotalReceitaNaoClassificada['2004']/$TotalReceita['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaNaoClassificada['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2005'])?round(($TotalReceitaNaoClassificada['2005']/$TotalReceita['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaNaoClassificada['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2006'])?round(($TotalReceitaNaoClassificada['2006']/$TotalReceita['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaNaoClassificada['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2007'])?round(($TotalReceitaNaoClassificada['2007']/$TotalReceita['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalReceitaNaoClassificada['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalReceita['2008'])?round(($TotalReceitaNaoClassificada['2008']/$TotalReceita['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(TR1)</font> Receita Total <font size=1>(R1+R2+R3)</font> :</b></td>
	<td align="right"><b><? echo number_format($TotalReceita['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalReceita['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalReceita['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalReceita['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalReceita['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesas correntes com materiais</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Materiais Universidade :</td>
	<td align="right"><? echo number_format($DespesaMateriaisUniversidade['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisUniversidade['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisUniversidade['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisUniversidade['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisUniversidade['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Materiais Fundação :</td>
	<td align="right"><? echo number_format($DespesaMateriaisFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaMateriaisFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesas correntes com serviços</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Serviços Universidade :</td>
	<td align="right"><? echo number_format($DespesaServicosUniversidade['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosUniversidade['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosUniversidade['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosUniversidade['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosUniversidade['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Serviços Fundação :</td>
	<td align="right"><? echo number_format($DespesaServicosFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaServicosFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesas corrente de pessoal executada via Universidade (recursos SUS)</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento CLT :</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSCLT['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSCLT['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSCLT['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSCLT['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSCLT['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento SSPE :</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSSSPE['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSSSPE['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSSSPE['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSSSPE['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSSSPE['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento Terceirizado :</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSTercerizados['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSTercerizados['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSTercerizados['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSTercerizados['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaUniversidadeSUSTercerizados['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesas corrente de pessoal executada via Universidade (recursos próprios)</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento RJU :</td>
	<td align="right"><? echo number_format($DespesaPagamentoRJU['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRJU['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRJU['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRJU['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRJU['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento CLT :</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLT['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLT['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLT['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLT['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLT['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento SSPE :</td>
	<td align="right"><? echo number_format($DespesaPagamentoSSPE['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoSSPE['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoSSPE['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoSSPE['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoSSPE['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento Terceirizado :</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizado['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizado['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizado['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizado['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizado['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento Requisitado :</td>
	<td align="right"><? echo number_format($DespesaPagamentoRequisitado['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRequisitado['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRequisitado['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRequisitado['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRequisitado['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Pagamento da bolsas de residência médica :</td>
	<td align="right"><? echo number_format($DespesaBolsaMedica['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaBolsaMedica['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaBolsaMedica['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaBolsaMedica['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaBolsaMedica['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesas corrente de pessoal executada via Fundação</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento CLT :</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLTFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLTFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLTFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLTFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoCLTFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento RPA :</td>
	<td align="right"><? echo number_format($DespesaPagamentoRPAFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRPAFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRPAFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRPAFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoRPAFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Despesa para pagamento Terceirizado :</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizadoFundacao['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizadoFundacao['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizadoFundacao['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizadoFundacao['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaPagamentoTerceirizadoFundacao['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Outras Despesas correntes</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Bolsas pagas pelo HUF :</td>
	<td align="right"><? echo number_format($DespesaOutrasCBolsa['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCBolsa['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCBolsa['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCBolsa['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCBolsa['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Sentenças Judiciais :</td>
	<td align="right"><? echo number_format($DespesaOutrasCSentenca['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCSentenca['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCSentenca['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCSentenca['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCSentenca['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Outros :</td>
	<td align="right"><? echo number_format($DespesaOutrasCOutras['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCOutras['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCOutras['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCOutras['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaOutrasCOutras['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Despesa de Capital</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Equipamento e Material Permanente :</td>
	<td align="right"><? echo number_format($DespesaCapitalEquipMat['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalEquipMat['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalEquipMat['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalEquipMat['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalEquipMat['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita">Obras (exceto reforma) :</td>
	<td align="right"><? echo number_format($DespesaCapitalObras['2004'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalObras['2005'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalObras['2006'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalObras['2007'], 2, ',', '.'); ?></td>
	<td>-</td>
	<td align="right"><? echo number_format($DespesaCapitalObras['2008'], 2, ',', '.'); ?></td>
	<td>-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Totais despesa</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(D1)</font> Total das depesas com materiais :</b></td>
	<td align="right"><b><? echo number_format($TotalDespesaMateriais['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2004'])?round(($TotalDespesaMateriais['2004']/$TotalDespesa['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaMateriais['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2005'])?round(($TotalDespesaMateriais['2005']/$TotalDespesa['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaMateriais['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2006'])?round(($TotalDespesaMateriais['2006']/$TotalDespesa['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaMateriais['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2007'])?round(($TotalDespesaMateriais['2007']/$TotalDespesa['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaMateriais['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2008'])?round(($TotalDespesaMateriais['2008']/$TotalDespesa['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(D2)</font> Total das despesas de serviços :</b></td>
	<td align="right"><b><? echo number_format($TotalDespesaServicos['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2004'])?round(($TotalDespesaServicos['2004']/$TotalDespesa['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaServicos['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2005'])?round(($TotalDespesaServicos['2005']/$TotalDespesa['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaServicos['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2006'])?round(($TotalDespesaServicos['2006']/$TotalDespesa['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaServicos['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2007'])?round(($TotalDespesaServicos['2007']/$TotalDespesa['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaServicos['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2008'])?round(($TotalDespesaServicos['2008']/$TotalDespesa['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(D3)</font> Total das despesas de pessoal :</b></td>
	<td align="right"><b><? echo number_format($TotalDespesaPessoal['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2004'])?round(($TotalDespesaPessoal['2004']/$TotalDespesa['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaPessoal['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2005'])?round(($TotalDespesaPessoal['2005']/$TotalDespesa['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaPessoal['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2006'])?round(($TotalDespesaPessoal['2006']/$TotalDespesa['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaPessoal['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2007'])?round(($TotalDespesaPessoal['2007']/$TotalDespesa['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalDespesaPessoal['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2008'])?round(($TotalDespesaPessoal['2008']/$TotalDespesa['2008']*100),1):"-"); ?>%</i></td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(D4)</font> Total de outras despesas :</b></td>
	<td align="right"><b><? echo number_format($TotalOutrasDespesas['2004'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2004'])?round(($TotalOutrasDespesas['2004']/$TotalDespesa['2004']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalOutrasDespesas['2005'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2005'])?round(($TotalOutrasDespesas['2005']/$TotalDespesa['2005']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalOutrasDespesas['2006'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2006'])?round(($TotalOutrasDespesas['2006']/$TotalDespesa['2006']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalOutrasDespesas['2007'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2007'])?round(($TotalOutrasDespesas['2007']/$TotalDespesa['2007']*100),1):"-"); ?>%</i></td>
	<td align="right"><b><? echo number_format($TotalOutrasDespesas['2008'], 2, ',', '.'); ?></b></td>
	<td align="center"><i><? echo (($TotalDespesa['2008'])?round(($TotalOutrasDespesas['2008']/$TotalDespesa['2008']*100),1):"-"); ?>%</i></td>
</tr>

<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(TD1)</font> Despesa corrente total <font size=1>(D1+D2+D3+D4+TO1)</font> :</b></td>
	<td align="right"><b><? echo number_format($TotalDespesa['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalDespesa['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalDespesa['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalDespesa['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($TotalDespesa['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(D5)</font> Despesa capital total :</b></td>
	<td align="right"><b><? echo number_format($SaldoCapital['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCapital['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCapital['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCapital['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCapital['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Obrigações</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(O1)</font> Dívidas (Universidade) :</b></td>
	<td align="right"><? echo number_format($DividasUniversidade['2004'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasUniversidade['2005'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasUniversidade['2006'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasUniversidade['2007'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasUniversidade['2008'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b><font size=1>(O2)</font> Dívidas (Fundação) :</b></td>
	<td align="right"><? echo number_format($DividasFundacao['2004'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasFundacao['2005'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasFundacao['2006'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasFundacao['2007'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format($DividasFundacao['2008'], 2, ',', '.'); ?></td>
	<td align="center">-</td>
</tr>
<tr bgcolor="#C0C0C0">
	<td class="SubTituloDireita"><b><font size=1>(TO1)</font> Total de obrigações <font size=1>(O1+O2)</font> :</b></td>
	<td align="right"><? echo number_format(($DividasUniversidade['2004']+$DividasFundacao['2004']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2005']+$DividasFundacao['2005']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2006']+$DividasFundacao['2006']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2007']+$DividasFundacao['2007']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2008']+$DividasFundacao['2008']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
</tr>
<tr bgcolor="#C0C0C0">
	<td class="SubTituloDireita"><b><font size=1>(DE1)</font> Dívida efetiva <font size=1>(TO1:ano atual-TO1:ano anterior)</font> :</b></td>
	<td align="right"><? echo number_format(($DividasUniversidade['2004']+$DividasFundacao['2003']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2005']+$DividasFundacao['2005']-$DividasUniversidade['2004']-$DividasFundacao['2004']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2006']+$DividasFundacao['2006']-$DividasUniversidade['2005']-$DividasFundacao['2005']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2007']+$DividasFundacao['2007']-$DividasUniversidade['2006']-$DividasFundacao['2006']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
	<td align="right"><? echo number_format(($DividasUniversidade['2008']+$DividasFundacao['2008']-$DividasUniversidade['2007']-$DividasFundacao['2007']), 2, ',', '.'); ?></td>
	<td align="center">-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Saldos ou Déficits</td>
</tr>
<tr bgcolor="#C0C0C0">
	<td class="SubTituloDireita"><b>Deficit ou saldo corrente <font size=1>(R2-TD1)</font> :</b></td>
	<td align="right"><b><? echo number_format($SaldoCorrente['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCorrente['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCorrente['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCorrente['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoCorrente['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr bgcolor="#C0C0C0">
	<td class="SubTituloDireita"><b>Deficit ou saldo <font size=1>(TR1-TD1-D5)</font> :</b></td>
	<td align="right"><b><? echo number_format($SaldoTotal['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoTotal['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoTotal['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoTotal['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($SaldoTotal['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr>
	<td colspan="11" class="SubTituloEsquerda">Receita Efetiva SUS</td>
</tr>

<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b>Contratualizado :</b></td>
	<td align="right"><b><? echo number_format($RecContratualizado['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecContratualizado['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecContratualizado['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecContratualizado['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecContratualizado['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b>Produzido :</b></td>
	<td align="right"><b><? echo number_format($RecProduzido['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecProduzido['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecProduzido['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecProduzido['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecProduzido['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b>Apresentado :</b></td>
	<td align="right"><b><? echo number_format($RecApresentado['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecApresentado['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecApresentado['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecApresentado['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecApresentado['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b>Aprovado/Pago :</b></td>
	<td align="right"><b><? echo number_format($RecAprovadopago['2004'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecAprovadopago['2005'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecAprovadopago['2006'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecAprovadopago['2007'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format($RecAprovadopago['2008'], 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>
<tr onmouseover="this.bgColor='#ffffcc';"  onmouseout="this.bgColor='';">
	<td class="SubTituloDireita"><b>Saldo :</b></td>
	<td align="right"><b><? echo number_format(($RecAprovadopago['2004']-$RecProduzido['2004']), 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format(($RecAprovadopago['2005']-$RecProduzido['2005']), 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format(($RecAprovadopago['2006']-$RecProduzido['2006']), 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format(($RecAprovadopago['2007']-$RecProduzido['2007']), 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
	<td align="right"><b><? echo number_format(($RecAprovadopago['2008']-$RecProduzido['2008']), 2, ',', '.'); ?></b></td>
	<td align="center">-</td>
</tr>

</table>