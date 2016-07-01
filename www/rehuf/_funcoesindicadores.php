<?
function formulaindicador_ecofin1($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('479','480','481') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','477','479','480','481') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin2($itm1, $itm2, $ano) {
	global $db;
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('479','480','481') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='352' AND linid IN('479','480','481') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin3($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('347','349','351') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','475','476','477','478','479','480','481') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2*100),2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin4($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('474','480') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('669','670','671','672','673','674','675','676','677','678','679','680','6170','6174','6176') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return "R$ ".number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "R$ 0,00";
	}
}

function formulaindicador_ecofin5($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('474','480') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('1978','1979','1980','1981','1982','1983','1984') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return "R$ ".number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "R$ 0,00";
	}
}

function formulaindicador_ecofin6($ano) {
	global $db;
	
	$sqls = array(// RECEITA : Grupo Quadro de Pessoal (Universidade)
				  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('335','337','339','341','343') AND ctiexercicio='{ano}'",
				  // RECEITA : Grupo Receita Efetiva (gitid=45)
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','475','476','477','478','479','480','481','482') AND ctiexercicio='{ano}'",
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
				  14 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('347','349','351') AND ctiexercicio='{ano}'");
	
	$vlr_rec_0 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[0]));
	$vlr_rec_1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[1]));
	$vlr_rec_2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[2]));
	$vlr_rec_3 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[3]));
	$vlr_rec_4 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[4]));
	$vlr_rec_5 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[5]));
	$vlr_rec_6 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[6]));
	$vlr_rec_7 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[7]));
	$receitatotal = $vlr_rec_0+$vlr_rec_1+$vlr_rec_2+$vlr_rec_3+$vlr_rec_4+$vlr_rec_5+$vlr_rec_6+$vlr_rec_7;
	$vlr_des_8 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[8]));
	$vlr_des_9 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[9]));
	$vlr_des_10 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[10]));
	$vlr_des_11 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[11]));
	$vlr_des_12 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[12]));
	$vlr_des_13 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[13]));
	$vlr_des_14 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[14]));
	$despesatotal = $vlr_des_8+$vlr_des_9+$vlr_des_10+$vlr_des_11+$vlr_des_12+$vlr_des_13+$vlr_des_14;
	
	if($despesatotal != 0) {
		return number_format(round(($receitatotal/$despesatotal*100),2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin7($ano) {
	global $db;
	$sqls = array(// RECEITA : Grupo Receita Efetiva (gitid=45)
				  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','475','476','477','478','479','480','481','482') AND ctiexercicio='{ano}'",
				  // DESPESA : Despesas com Materiais - Universidade (gitid=41)
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('432','433','434','435','436','437','438','439','440','441') AND ctiexercicio='{ano}'",
				  // DESPESA : Despesas com Materiais - Fundação (gitid=42)
				  2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('442','443','444','445','446','447','448','449','450','451') AND ctiexercicio='{ano}'",
				  // DESPESA : Contratos de Serviços - Universidade (gitid=43)
				  3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('452','453','454','455','456','457','458','459','460','461') AND ctiexercicio='{ano}'",
				  // DESPESA : Grupo Contratos de Serviços - Fundação
				  4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN('462','463','464','465','466','467','468','469','470','471') AND ctiexercicio='{ano}'"
				  );
	
	
	$vlr_rec_0 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[0]));
	$receitatotal = $vlr_rec_0;
	$vlr_des_1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[1]));
	$vlr_des_2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[2]));
	$vlr_des_3 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[3]));
	$vlr_des_4 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[4]));
	$despesatotal = $vlr_des_1+$vlr_des_2+$vlr_des_3+$vlr_des_4;
	
	if($despesatotal != 0) {
		return number_format(round(($receitatotal/$despesatotal*100),2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin8($ano) {
	global $db;
	$sqls = array(// Grupo Movimentação de Pessoal (RJU) (gitid=61) : linha 653 "Ingresso por Concurso"
				  0 => "SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='653' AND ctiexercicio='{ano}'",
				  // Grupo Movimentação de Pessoal (RJU) (gitid=61) : linha 654 "Aposentados"
				  1 => "SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='654' AND ctiexercicio='{ano}'");
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[0]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[1]));
	
	if($vlr2 != 0) {
		return number_format(round(($vlr1/$vlr2*100),2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ecofin9($ano) {
	global $db;
	$sqls = array(// RECEITA : Grupo Receita Efetiva (gitid=45)
				  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('473','474','477','479','480','481','482') AND ctiexercicio='{ano}'",
				  // RECEITA : Incentivos
		  		  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='217' AND linid IN('475','476','478') AND ctiexercicio='{ano}'",
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
				  33 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='351' AND ctiexercicio='{ano}'"
				  
		  		  );
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[0]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[1]));
	
	
	/* Depesa Materiais Universidade */
	$DespesaMateriaisUniversidade = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[8]));
	/* Depesa Materiais Fundação */
	$DespesaMateriaisFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[9]));
	/* Depesa Serviços Universidade */
	$DespesaServicosUniversidade = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[10]));
	/* Depesa Serviços Fundação */
	$DespesaServicosFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[12]));
	/* Despesa para pagamento RJU */
	$DespesaPagamentoRJU = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[26]));
	/* Despesa para pagamento CLT */
	$DespesaPagamentoCLT = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[27]));
	/* Despesa para pagamento SSPE */
	$DespesaPagamentoSSPE = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[28]));
	/* Despesa para pagamento Terceirizado */
	$DespesaPagamentoTerceirizado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[29]));
	/* Despesa para pagamento Requisitado */
	$DespesaPagamentoRequisitado = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[30]));
	/* Despesa para Pagamento da bolsas de residência médica */
	$DespesaBolsaMedica = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[11]));
	/* Despesa para Despesa para pagamento CLT(fundação) */
	$DespesaPagamentoCLTFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[31]));
	/* Despesa para Despesa para pagamento RPA(fundação) */
	$DespesaPagamentoRPAFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[32]));
	/* Despesa para Despesa para pagamento RPA(fundação) */
	$DespesaPagamentoTerceirizadoFundacao = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[33]));
	/* Total das despesas com materiais */
	$TotalDespesaMateriais = $DespesaMateriaisUniversidade+$DespesaMateriaisFundacao;
	/* Total das despesas com serviços */
	$TotalDespesaServicos = $DespesaServicosUniversidade+$DespesaServicosFundacao;
	/* Total das despesas de pessoal */
	$TotalDespesaPessoal = $DespesaPagamentoRJU+$DespesaPagamentoCLT+$DespesaPagamentoSSPE+$DespesaPagamentoTerceirizado+$DespesaPagamentoRequisitado+$DespesaBolsaMedica+$DespesaPagamentoCLTFundacao+$DespesaPagamentoRPAFundacao+$DespesaPagamentoTerceirizadoFundacao;
	/* Total das despesas */
	$TotalDespesa = $TotalDespesaMateriais+$TotalDespesaServicos+$TotalDespesaPessoal;
	
	if($TotalDespesa != 0) {
		return number_format(round((($vlr1+$vlr2)/$TotalDespesa),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_ensinopesquisa1($item1,$item2,$ano) {
	global $db;
	/* Mapeamento dos dados (EM PRODUÇÃO)
	 * TABELA : Alunado
	 * GRUPO  : Curso
	 * DADO1  : Somatorio de todas as linhas da coluna 'Graduação' */
	$dados['itens1'][0] = array("gitid" => 20, "coluna" => array(79,80,81,83,84,85)); 	 	 	 	 	
	/* Mapeamento dos dados (EM PRODUÇÃO)
	 * TABELA : Docentes
	 * GRUPO  : Titulação Máxima
	 * DADO2  : Somatorio de todas as linhas da coluna 'Docente' */
	$dados['itens2'][0] = array("linha" =>  array(213,214,215,216), "coluna" => 61);	 	
	$vlr1 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
						 LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
						 LEFT JOIN rehuf.coluna col ON col.colid = cdi.colid
						 WHERE cdi.esuid='".$_SESSION['rehuf_var']['esuid']."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND col.colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND lin.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND colid='".$dados['itens2'][$item2[0]]["coluna"]."' AND ctiexercicio='".$ano."'");
	if($vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_ensinopesquisa2($item1,$item2,$ano) {
	global $db;
	/* Tabela : Alunado 
	 * Grupo  : Curso */
	$dados['itens1'][0] = array("gitid" => 20, "coluna" => array(79,80,81,83,84,85)); 	 	 	 	 	
	/* Tabela : DADOS Produção Assistencial SUS
	 * Grupo  : Número de leitos */
	$dados['itens2'][0] = array("linha" =>  283, "coluna" => array(94,95,96,97,353));
	$vlr1 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
						 LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
						 LEFT JOIN rehuf.coluna col ON col.colid = cdi.colid
						 WHERE cdi.esuid='".$_SESSION['rehuf_var']['esuid']."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND col.colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND lin.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if($vlr1 > 0) {
		return number_format(round(($vlr2/$vlr1)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ensinopesquisa3($item1,$item2,$ano) {
	global $db, $dados;
	/* Alunado
	 * Curso */
	$dados['itens1'][0] = array("gitid" => 20, "coluna" => array(79,80,81,83,84,85)); 	 	 	 	 	
	/* DADOS Produção Assistencial SUS
	 * Número de leitos */
	$dados['itens2'][0] = array("linha" =>  284, "coluna" => array(94,95,96,97,353));
	$vlr1 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
						 LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
						 LEFT JOIN rehuf.coluna col ON col.colid = cdi.colid
						 WHERE cdi.esuid='".$_SESSION['rehuf_var']['esuid']."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND col.colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND col.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND lin.gitid='".$dados['itens1'][$item1[0]]["gitid"]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if($vlr1 > 0) {
		return number_format(round(($vlr2/$vlr1)*100,2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_ensinopesquisa4($item1,$item2,$ano) {
	global $db;
	$dados['itens1'][0] = array("linha" => 286, "coluna" => array(94,95,96,97,353)); 	 	 	 	 	
	$dados['itens2'][0] = array("linha" =>  285, "coluna" => array(94,95,96,97,353));	 	
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if(($vlr2+$vlr1) > 0) {
		return number_format(round(($vlr1/($vlr2+$vlr1))*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ensinopesquisa5($item1,$item2,$ano) {
	global $db, $dados;
	$dados['itens1'][0] = array("linha" => 286, "coluna" => array(94,95,96,97,353)); 	 	 	 	 	
	$dados['itens2'][0] = array("linha" =>  213, "coluna" => array(61));	 	
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if($vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_ensinopesquisa6($item1,$item2,$ano) {
	global $db, $dados;
	$dados['itens1'][0] = array("linha" => array(288,289), "coluna" => array(94,95,96,97,353)); 	 	 	 	 	
	$dados['itens2'][0] = array("linha" =>  213, "coluna" => array(61));	 	
	
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"][0]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"][1]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr3 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");

	if($vlr3 > 0) {
		return number_format(round(($vlr1+$vlr2)/$vlr3*1000,2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_gestaoassistencial1($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * Total de Dias/Ano
	 * 
	 */
	/* Clínica Médica */ 		$dados['itens1'][0] = array("linha" => 669, "coluna" => array(281,282,283,284,376)); 	 	 	 	 	
	/* Cirurgia */       		$dados['itens1'][1] = array("linha" => 670, "coluna" => array(281,282,283,284,376));
	/* Pediatria */ 	 		$dados['itens1'][2] = array("linha" => 671, "coluna" => array(281,282,283,284,376));
	/* Obstetrícia */    		$dados['itens1'][3] = array("linha" => 672, "coluna" => array(281,282,283,284,376));
	/* Berçário */       		$dados['itens1'][4] = array("linha" => 673, "coluna" => array(281,282,283,284,376));
	/* Ginecologia */ 	 		$dados['itens1'][5] = array("linha" => 674, "coluna" => array(281,282,283,284,376));				
	/* Psiquiatria */			$dados['itens1'][6] = array("linha" => 675, "coluna" => array(281,282,283,284,376));
	/* Hospital-Dia */ 			$dados['itens1'][7] = array("linha" => 676, "coluna" => array(281,282,283,284,376));
	/* UTI Adulto */ 			$dados['itens1'][8] = array("linha" => 677, "coluna" => array(281,282,283,284,376));
	/* UTI Pediátrica */ 		$dados['itens1'][9] = array("linha" => 678, "coluna" => array(281,282,283,284,376));
	/* UTI Neonatal */ 			$dados['itens1'][10] = array("linha" => 679, "coluna" => array(281,282,283,284,376));
	/* Outras Especialidades */ $dados['itens1'][11] = array("linha" => 680, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Adulta */ $dados['itens1'][12] = array("linha" => 6170, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Pediátrica */ $dados['itens1'][13] = array("linha" => 6174, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Neo-Natal */ $dados['itens1'][14] = array("linha" => 6176, "coluna" => array(281,282,283,284,376));
	/*
	 * DADOS Produção Assistencial SUS
	 * Número de leitos
	 * 
	 */
	/* Clínica Cirúrgica */                $dados['itens2'][0] = array("linha" =>  486, "coluna" => array(232,233,234,235,370));	 	
	/* Clínica Médica */				   $dados['itens2'][1] = array("linha" =>  487, "coluna" => array(232,233,234,235,370));				
	/* UTI Adulto */					   $dados['itens2'][2] = array("linha" =>  488, "coluna" => array(232,233,234,235,370));
	/* UTI Pediátrica */				   $dados['itens2'][3] = array("linha" =>  489, "coluna" => array(232,233,234,235,370));				
	/* UTI Neonatal */					   $dados['itens2'][4] = array("linha" =>  490, "coluna" => array(232,233,234,235,370));
	/* UTI de Queimados */				   $dados['itens2'][5] = array("linha" =>  491, "coluna" => array(232,233,234,235,370));
	/* Unidade Intermediária */			   $dados['itens2'][6] = array("linha" =>  492, "coluna" => array(232,233,234,235,370));				
	/* Unidade Intermediária Neonatal */   $dados['itens2'][7] = array("linha" =>  493, "coluna" => array(232,233,234,235,370));
	/* Unidade de Isolamento */			   $dados['itens2'][8] = array("linha" =>  494, "coluna" => array(232,233,234,235,370));
	/* Obstétrico Cirúrgico */			   $dados['itens2'][9] = array("linha" =>  495, "coluna" => array(232,233,234,235,370));
	/* Obstétrico Clínico */			   $dados['itens2'][10] = array("linha" => 496, "coluna" => array(232,233,234,235,370));
	/* Psiquiátrico */					   $dados['itens2'][11] = array("linha" => 498, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Clínica Médica) */    $dados['itens2'][12] = array("linha" => 499, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Clínica Cirúrgica) */ $dados['itens2'][13] = array("linha" => 500, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Saúde Mental) */	   $dados['itens2'][14] = array("linha" => 501, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Pediatria) */		   $dados['itens2'][15] = array("linha" => 502, "coluna" => array(232,233,234,235,370));
	/* Emergência (Observação) */		   $dados['itens2'][16] = array("linha" => 503, "coluna" => array(232,233,234,235,370));
	/* Emergência (Internação) */		   $dados['itens2'][17] = array("linha" => 504, "coluna" => array(232,233,234,235,370));
	/* Pediátrico */					   $dados['itens2'][18] = array("linha" => 533, "coluna" => array(232,233,234,235,370));
	/* Ginecológico */					   $dados['itens2'][19] = array("linha" => 1577, "coluna" => array(232,233,234,235,370));
		
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/($vlr2*365))*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicadortotal_gestaoassistencial1($ano) {
	global $db;
	
	$sqls = array('1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='281' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='282' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='283' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='284' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='376' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='232' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='233' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='234' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='235' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='370' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");
	
	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round(($vlr1/($vlr2*365))*100, 2), 2, ',', '')." %";
	} else {
		return "0,00 %";	
	}
}

function formulaindicador_gestaoassistencial2($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 */
	/* Clínica Médica */ 		$dados['itens1'][0] = array("linha" => 669, "coluna" => array(281,282,283,284,376)); 	 	 	 	 	
	/* Cirurgia */       		$dados['itens1'][1] = array("linha" => 670, "coluna" => array(281,282,283,284,376));
	/* Pediatria */ 	 		$dados['itens1'][2] = array("linha" => 671, "coluna" => array(281,282,283,284,376));
	/* Obstetrícia */    		$dados['itens1'][3] = array("linha" => 672, "coluna" => array(281,282,283,284,376));
	/* Berçário */       		$dados['itens1'][4] = array("linha" => 673, "coluna" => array(281,282,283,284,376));
	/* Ginecologia */ 	 		$dados['itens1'][5] = array("linha" => 674, "coluna" => array(281,282,283,284,376));				
	/* Psiquiatria */			$dados['itens1'][6] = array("linha" => 675, "coluna" => array(281,282,283,284,376));
	/* Hospital-Dia */ 			$dados['itens1'][7] = array("linha" => 676, "coluna" => array(281,282,283,284,376));
	/* UTI Adulto */ 			$dados['itens1'][8] = array("linha" => 677, "coluna" => array(281,282,283,284,376));
	/* UTI Pediátrica */ 		$dados['itens1'][9] = array("linha" => 678, "coluna" => array(281,282,283,284,376));
	/* UTI Neonatal */ 			$dados['itens1'][10] = array("linha" => 679, "coluna" => array(281,282,283,284,376));
	/* Outras Especialidades */ $dados['itens1'][11] = array("linha" => 680, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Adulta */ $dados['itens1'][12] = array("linha" => 6170, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Pediátrica */ $dados['itens1'][13] = array("linha" => 6174, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Neo-Natal */ $dados['itens1'][14] = array("linha" => 6176, "coluna" => array(281,282,283,284,376));
	
	/*
	 * DADOS Produção Assistencial SUS
	 * Número de leitos
	 * $dados2['gitid'] = 51;
	 * $dados2['tabtid'] = 2;
	 * 
	 */
	
	
	/* Clínica Médica */			$dados['itens2'][0] = array("linha" =>  700, "coluna" => array(289,290,291,292,378));
	/* Cirurgia */					$dados['itens2'][1] = array("linha" =>  701, "coluna" => array(289,290,291,292,378));
	/* Pediatria */					$dados['itens2'][2] = array("linha" =>  702, "coluna" => array(289,290,291,292,378));
	/* Obstetrícia */				$dados['itens2'][3] = array("linha" =>  703, "coluna" => array(289,290,291,292,378));
	/* Berçário */					$dados['itens2'][4] = array("linha" =>  704, "coluna" => array(289,290,291,292,378));
	/* Ginecologia */				$dados['itens2'][5] = array("linha" =>  705, "coluna" => array(289,290,291,292,378));
	/* Psiquiatria */				$dados['itens2'][6] = array("linha" =>  706, "coluna" => array(289,290,291,292,378));
	/* Hospital-Dia */				$dados['itens2'][7] = array("linha" =>  707, "coluna" => array(289,290,291,292,378));
	/* UTI Adulto */				$dados['itens2'][8] = array("linha" =>  708, "coluna" => array(289,290,291,292,378));
	/* UTI Pediátrica */			$dados['itens2'][9] = array("linha" =>  709, "coluna" => array(289,290,291,292,378));
	/* UTI Neonatal */				$dados['itens2'][10] = array("linha" =>  710, "coluna" => array(289,290,291,292,378));
	/* Outras Especialidades */		$dados['itens2'][11] = array("linha" =>  711, "coluna" => array(289,290,291,292,378));
	/* Unidade Intermediária Adulta */		$dados['itens2'][12] = array("linha" =>  6184, "coluna" => array(289,290,291,292,378));
	/* Unidade Intermediária Pediátrica */		$dados['itens2'][13] = array("linha" =>  6185, "coluna" => array(289,290,291,292,378));
	/* Unidade Intermediária Neo-Natal */		$dados['itens2'][14] = array("linha" =>  6186, "coluna" => array(289,290,291,292,378));
		
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if(($vlr1 !== "") && $vlr1 > 0) {
		return number_format(round(($vlr2/$vlr1)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicadortotal_gestaoassistencial2($ano) {
	global $db;
	
	$sqls = array('1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('700','701','702','703','704','705','706','707','708','709','710','711','6184','6185','6186') AND colid='289' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('700','701','702','703','704','705','706','707','708','709','710','711','6184','6185','6186') AND colid='290' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('700','701','702','703','704','705','706','707','708','709','710','711','6184','6185','6186') AND colid='291' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('700','701','702','703','704','705','706','707','708','709','710','711','6184','6185','6186') AND colid='292' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('700','701','702','703','704','705','706','707','708','709','710','711','6184','6185','6186') AND colid='378' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");

	
	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round(($vlr1/$vlr2)*100, 2), 2, ',', '')." %";
	} else {
		return "0,00 %";	
	}
}

function formulaindicador_gestaoassistencial3($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * Total de Dias/Ano Internações
	 * 
	 */
	
	/* Clínica Médica */ 		$dados['itens1'][0] = array("linha" => 669, "coluna" => array(281,282,283,284,376)); 	 	 	 	 	
	/* Cirurgia */       		$dados['itens1'][1] = array("linha" => 670, "coluna" => array(281,282,283,284,376));
	/* Pediatria */ 	 		$dados['itens1'][2] = array("linha" => 671, "coluna" => array(281,282,283,284,376));
	/* Obstetrícia */    		$dados['itens1'][3] = array("linha" => 672, "coluna" => array(281,282,283,284,376));
	/* Berçário */       		$dados['itens1'][4] = array("linha" => 673, "coluna" => array(281,282,283,284,376));
	/* Ginecologia */ 	 		$dados['itens1'][5] = array("linha" => 674, "coluna" => array(281,282,283,284,376));				
	/* Psiquiatria */			$dados['itens1'][6] = array("linha" => 675, "coluna" => array(281,282,283,284,376));
	/* Hospital-Dia */ 			$dados['itens1'][7] = array("linha" => 676, "coluna" => array(281,282,283,284,376));
	/* UTI Adulto */ 			$dados['itens1'][8] = array("linha" => 677, "coluna" => array(281,282,283,284,376));
	/* UTI Pediátrica */ 		$dados['itens1'][9] = array("linha" => 678, "coluna" => array(281,282,283,284,376));
	/* UTI Neonatal */ 			$dados['itens1'][10] = array("linha" => 679, "coluna" => array(281,282,283,284,376));
	/* Outras Especialidades */ $dados['itens1'][11] = array("linha" => 680, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Adulta */ $dados['itens1'][12] = array("linha" => 6170, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Pediátrica */ $dados['itens1'][13] = array("linha" => 6174, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Neo-Natal */ $dados['itens1'][14] = array("linha" => 6176, "coluna" => array(281,282,283,284,376));
	
	/*
	 * DADOS Produção Assistencial SUS
	 * Número de obitos
	 * 
	 */
	/* Clínica Médica */			$dados['itens2'][0] = array("linha" =>  681, "coluna" => array(285,286,287,288,377));
	/* Cirurgia */					$dados['itens2'][1] = array("linha" =>  682, "coluna" => array(285,286,287,288,377));
	/* Pediatria */					$dados['itens2'][2] = array("linha" =>  683, "coluna" => array(285,286,287,288,377));
	/* Obstetrícia */				$dados['itens2'][3] = array("linha" =>  684, "coluna" => array(285,286,287,288,377));
	/* Berçário */					$dados['itens2'][4] = array("linha" =>  685, "coluna" => array(285,286,287,288,377));
	/* Ginecologia */				$dados['itens2'][5] = array("linha" =>  686, "coluna" => array(285,286,287,288,377));
	/* Psiquiatria */				$dados['itens2'][6] = array("linha" =>  687, "coluna" => array(285,286,287,288,377));
	/* Hospital-Dia */				$dados['itens2'][7] = array("linha" =>  688, "coluna" => array(285,286,287,288,377));
	/* UTI Adulto */				$dados['itens2'][8] = array("linha" =>  689, "coluna" => array(285,286,287,288,377));
	/* UTI Pediátrica */			$dados['itens2'][9] = array("linha" =>  690, "coluna" => array(285,286,287,288,377));
	/* UTI Neonatal */				$dados['itens2'][10] = array("linha" =>  691, "coluna" => array(285,286,287,288,377));
	/* Outras Especialidades */		$dados['itens2'][11] = array("linha" =>  692, "coluna" => array(285,286,287,288,377));
	/* Unidade Intermediária Adulta */		$dados['itens2'][12] = array("linha" =>  8147, "coluna" => array(285,286,287,288,377));
	/* Unidade Intermediária Pediátrica */		$dados['itens2'][13] = array("linha" =>  8148, "coluna" => array(285,286,287,288,377));
	/* Unidade Intermediária Neo-Natal */		$dados['itens2'][14] = array("linha" =>  8149, "coluna" => array(285,286,287,288,377));
		
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND colid='".$dados['itens2'][$item2[0]]["coluna"][$item2[1]]."' AND ctiexercicio='".$ano."'");
	if(($vlr1 !== "") && $vlr1 > 0) {
		return number_format(round(($vlr2/$vlr1)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicadortotal_gestaoassistencial3($ano) {
	global $db;
	
	$sqls = array('1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('681','682','683','684','685','686','687','688','689','690','691','692') AND colid='285' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('681','682','683','684','685','686','687','688','689','690','691','692') AND colid='286' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('681','682','683','684','685','686','687','688','689','690','691','692') AND colid='287' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('681','682','683','684','685','686','687','688','689','690','691','692') AND colid='288' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('681','682','683','684','685','686','687','688','689','690','691','692') AND colid='377' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");
	
	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round(($vlr1/$vlr2)*100, 2), 2, ',', '')." %";
	} else {
		return "0,00 %";	
	}
}

function formulaindicador_gestaoassistencial4($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * Total de Dias/Ano
	 * 
	 */
	
	/* Clínica Médica */ 		$dados['itens1'][0] = array("linha" => 669, "coluna" => array(281,282,283,284,376)); 	 	 	 	 	
	/* Cirurgia */       		$dados['itens1'][1] = array("linha" => 670, "coluna" => array(281,282,283,284,376));
	/* Pediatria */ 	 		$dados['itens1'][2] = array("linha" => 671, "coluna" => array(281,282,283,284,376));
	/* Obstetrícia */    		$dados['itens1'][3] = array("linha" => 672, "coluna" => array(281,282,283,284,376));
	/* Berçário */       		$dados['itens1'][4] = array("linha" => 673, "coluna" => array(281,282,283,284,376));
	/* Ginecologia */ 	 		$dados['itens1'][5] = array("linha" => 674, "coluna" => array(281,282,283,284,376));				
	/* Psiquiatria */			$dados['itens1'][6] = array("linha" => 675, "coluna" => array(281,282,283,284,376));
	/* Hospital-Dia */ 			$dados['itens1'][7] = array("linha" => 676, "coluna" => array(281,282,283,284,376));
	/* UTI Adulto */ 			$dados['itens1'][8] = array("linha" => 677, "coluna" => array(281,282,283,284,376));
	/* UTI Pediátrica */ 		$dados['itens1'][9] = array("linha" => 678, "coluna" => array(281,282,283,284,376));
	/* UTI Neonatal */ 			$dados['itens1'][10] = array("linha" => 679, "coluna" => array(281,282,283,284,376));
	/* Outras Especialidades */ $dados['itens1'][11] = array("linha" => 680, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Adulta */ $dados['itens1'][12] = array("linha" => 6170, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Pediátrica */ $dados['itens1'][13] = array("linha" => 6174, "coluna" => array(281,282,283,284,376));
	/* Unidade Intermediária Neo-Natal */ $dados['itens1'][14] = array("linha" => 6176, "coluna" => array(281,282,283,284,376));
	
	/*
	 * DADOS Produção Assistencial SUS
	 * SUS
	 * 
	 */
	
	/* Clínica Médica */			$dados['itens2'][0] = array("linha" =>  1978, "coluna" => array(327,328,329));
	/* Cirurgia */					$dados['itens2'][1] = array("linha" =>  1979, "coluna" => array(327,328,329));
	/* Pediatria */					$dados['itens2'][2] = array("linha" =>  1980, "coluna" => array(327,328,329));
	/* Obstetrícia */				$dados['itens2'][3] = array("linha" =>  1981, "coluna" => array(327,328,329));
	/* Berçário */					$dados['itens2'][4] = array("linha" =>  1982, "coluna" => array(327,328,329));
	/* Ginecologia */				$dados['itens2'][5] = array("linha" =>  1983, "coluna" => array(327,328,329));
	/* Psiquiatria */				$dados['itens2'][6] = array("linha" =>  1984, "coluna" => array(327,328,329));
	/* Hospital-Dia */				$dados['itens2'][7] = array("linha" =>  1986, "coluna" => array(327,328,329));
	/* UTI Adulto */				$dados['itens2'][8] = array("linha" =>  1987, "coluna" => array(327,328,329));
	/* UTI Pediátrica */			$dados['itens2'][9] = array("linha" =>  1988, "coluna" => array(327,328,329));
	/* UTI Neonatal */				$dados['itens2'][10] = array("linha" =>  1989, "coluna" => array(327,328,329));
	/* Outras Especialidades */		$dados['itens2'][11] = array("linha" =>  1990, "coluna" => array(327,328,329));
	/* Unidade Intermediária Adulta */		$dados['itens2'][12] = array("linha" =>  8140, "coluna" => array(327,328,329));
	/* Unidade Intermediária Pediátrica */		$dados['itens2'][13] = array("linha" =>  8141, "coluna" => array(327,328,329));
	/* Unidade Intermediária Neo-Natal */		$dados['itens2'][14] = array("linha" =>  8142, "coluna" => array(327,328,329));
		
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND ctiexercicio='".$ano."'");
	if(($vlr2 !== "") && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicadortotal_gestaoassistencial4($ano) {
	global $db;
	
	$sqls = array('1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='281' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='282' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='283' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='284' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='376' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");

	
	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round($vlr1/$vlr2, 2), 2, ',', '');
	} else {
		return "0,00";	
	}
}

function formulaindicador_gestaoassistencial5($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * SUS - Partos Normal
	 * 
	 */
	
	/* Normal - Baixo Risco */ $dados['itens1'][0] = array("linha" => 1991, "coluna" => array(327,328,329));
	/* Normal - Alto Risco */  $dados['itens1'][1] = array("linha" => 1993, "coluna" => array(327,328,329));
	
	/*
	 * DADOS Produção Assistencial SUS
	 * SUS - Partos Cesárea
	 * 
	 */
	/* Cesárea - Baixo Risco */ $dados['itens2'][0] = array("linha" => 1990, "coluna" => array(327,328,329));
	/* Cesárea - Alto Risco */ $dados['itens2'][1] = array("linha" => 1992, "coluna" => array(327,328,329));
	
	$vlr1 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND ctiexercicio='".$ano."'");
	if(($vlr2+$vlr1) > 0) {
		return number_format(round(($vlr2/($vlr2+$vlr1))*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicador_gestaoassistencial6($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * Número de leitos
	 * 
	 */
	
	/* Clínica Médica */				   $dados['itens1'][0] = array("linha" =>  487, "coluna" => array(232,233,234,235,370));
	/* Clínica Cirúrgica */                $dados['itens1'][1] = array("linha" =>  486, "coluna" => array(232,233,234,235,370));	 	
	/* UTI Adulto */					   $dados['itens1'][2] = array("linha" =>  488, "coluna" => array(232,233,234,235,370));
	/* UTI Pediátrica */				   $dados['itens1'][3] = array("linha" =>  489, "coluna" => array(232,233,234,235,370));				
	/* UTI Neonatal */					   $dados['itens1'][4] = array("linha" =>  490, "coluna" => array(232,233,234,235,370));
	/* UTI de Queimados */				   $dados['itens1'][5] = array("linha" =>  491, "coluna" => array(232,233,234,235,370));
	/* Unidade Intermediária */			   $dados['itens1'][6] = array("linha" =>  492, "coluna" => array(232,233,234,235,370));				
	/* Unidade Intermediária Neonatal */   $dados['itens1'][7] = array("linha" =>  493, "coluna" => array(232,233,234,235,370));
	/* Unidade de Isolamento */			   $dados['itens1'][8] = array("linha" =>  494, "coluna" => array(232,233,234,235,370));
	/* Obstétrico Cirúrgico */			   $dados['itens1'][9] = array("linha" =>  495, "coluna" => array(232,233,234,235,370));
	/* Obstétrico Clínico */			   $dados['itens1'][10] = array("linha" => 496, "coluna" => array(232,233,234,235,370));
	/* Psiquiátrico */					   $dados['itens1'][11] = array("linha" => 498, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Clínica Médica) */    $dados['itens1'][12] = array("linha" => 499, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Clínica Cirúrgica) */ $dados['itens1'][13] = array("linha" => 500, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Saúde Mental) */	   $dados['itens1'][14] = array("linha" => 501, "coluna" => array(232,233,234,235,370));
	/* Hospital-Dia (Pediatria) */		   $dados['itens1'][15] = array("linha" => 502, "coluna" => array(232,233,234,235,370));
	/* Emergência (Observação) */		   $dados['itens1'][16] = array("linha" => 503, "coluna" => array(232,233,234,235,370));
	/* Emergência (Internação) */		   $dados['itens1'][17] = array("linha" => 504, "coluna" => array(232,233,234,235,370));
	/* Pediátrico */					   $dados['itens1'][18] = array("linha" => 533, "coluna" => array(232,233,234,235,370));
	/* Ginecológico */					   $dados['itens1'][19] = array("linha" => 1577, "coluna" => array(232,233,234,235,370));
	
	/*
	 * DADOS Produção Assistencial SUS
	 * SUS
	 * 
	 */
	
	/* Clínica Médica */			$dados['itens2'][0] = array("linha" =>  1978, "coluna" => array(327,328,329));
	/* Cirurgia */					$dados['itens2'][1] = array("linha" =>  1979, "coluna" => array(327,328,329));
	/* Pediatria */					$dados['itens2'][2] = array("linha" =>  1980, "coluna" => array(327,328,329));
	/* Obstetrícia */				$dados['itens2'][3] = array("linha" =>  1981, "coluna" => array(327,328,329));
	/* Berçário */					$dados['itens2'][4] = array("linha" =>  1982, "coluna" => array(327,328,329));
	/* Ginecologia */				$dados['itens2'][5] = array("linha" =>  1983, "coluna" => array(327,328,329));
	/* Psiquiatria */				$dados['itens2'][6] = array("linha" =>  1984, "coluna" => array(327,328,329));
	/* Hospital-Dia */				$dados['itens2'][7] = array("linha" =>  1986, "coluna" => array(327,328,329));
	/* UTI Adulto */				$dados['itens2'][8] = array("linha" =>  1987, "coluna" => array(327,328,329));
	/* UTI Pediátrica */			$dados['itens2'][9] = array("linha" =>  1988, "coluna" => array(327,328,329));
	/* UTI Neonatal */				$dados['itens2'][10] = array("linha" =>  1989, "coluna" => array(327,328,329));
	/* Outras Especialidades */		$dados['itens2'][11] = array("linha" =>  1990, "coluna" => array(327,328,329));
	/* Unidade Intermediária Adulta */	  $dados['itens2'][12] = array("linha" =>  8140, "coluna" => array(327,328,329));
	/* Unidade Intermediária Pediátrica */$dados['itens2'][13] = array("linha" =>  8141, "coluna" => array(327,328,329));
	/* Unidade Intermediária Neo-Natal */ $dados['itens2'][14] = array("linha" =>  8142, "coluna" => array(327,328,329));
		
	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND ctiexercicio='".$ano."'");
	
	if($vlr1 > 0) {
		return number_format(round(($vlr2/$vlr1)/12,2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicadortotal_gestaoassistencial6($ano) {
	global $db;
	
	$sqls = array(
				  '1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid IN('327','328','329') AND linid IN('1978','1979','1980','1981','1982','1983','1984','1986','1987','1988','1989') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
			  	  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='232' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='233' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='234' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='235' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE colid='370' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");
	
	
	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round($vlr1/$vlr2/12, 2), 2, ',', '');
	} else {
		return "0,00";	
	}
}

function formulaindicador_gestaoassistencial7($item1,$item2,$ano) {
	global $db;
	/*
	 * DADOS Produção Assistencial SUS
	 * Número de obitos Transplante
	 * 
	 */
	/* Medula Óssea */			$dados['itens1'][0] = array("linha" =>  693, "coluna" => array(285,286,287,288,377));
	/* Fígado */				$dados['itens1'][1] = array("linha" =>  694, "coluna" => array(285,286,287,288,377));
	/* Cardíaco */				$dados['itens1'][2] = array("linha" =>  695, "coluna" => array(285,286,287,288,377));
	/* Pulmão */				$dados['itens1'][3] = array("linha" =>  696, "coluna" => array(285,286,287,288,377));
	/* Renal */					$dados['itens1'][4] = array("linha" =>  697, "coluna" => array(285,286,287,288,377));
	/* Córnea */				$dados['itens1'][5] = array("linha" =>  698, "coluna" => array(285,286,287,288,377));
	/* Outros */				$dados['itens1'][6] = array("linha" =>  699, "coluna" => array(285,286,287,288,377));
	/*
	 * DADOS Produção Assistencial SUS
	 * SUS Transplantes
	 * 
	 */
	/* Medula Óssea */ 		$dados['itens2'][0] = array("linha" => 1994, "coluna" => array(327,328,329)); 	 	 	 	 	
	/* Fígado */       		$dados['itens2'][1] = array("linha" => 1995, "coluna" => array(327,328,329));
	/* Cardíaco */ 	 		$dados['itens2'][2] = array("linha" => 1996, "coluna" => array(327,328,329));
	/* Pulmão */    		$dados['itens2'][3] = array("linha" => 1997, "coluna" => array(327,328,329));
	/* Renal */       		$dados['itens2'][4] = array("linha" => 1998, "coluna" => array(327,328,329));
	/* Córnea */ 	 		$dados['itens2'][5] = array("linha" => 1999, "coluna" => array(327,328,329));

	$vlr1 = $db->pegaUm("SELECT ctivalor FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens1'][$item1[0]]["linha"]."' AND colid='".$dados['itens1'][$item1[0]]["coluna"][$item1[1]]."' AND ctiexercicio='".$ano."'");
	$vlr2 = $db->pegaUm("SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='".$_SESSION['rehuf_var']['esuid']."' AND linid='".$dados['itens2'][$item2[0]]["linha"]."' AND ctiexercicio='".$ano."'");
	if($vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2)*100,2), 2, ',', '')." %";
	} else {
		return "0,00 %";
	}
}

function formulaindicadortotal_gestaoassistencial7($ano) {
	global $db;
	
	$sqls = array('1_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('693','694','695','696','697','698','699') AND colid='285' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('693','694','695','696','697','698','699') AND colid='286' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('693','694','695','696','697','698','699') AND colid='287' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('693','694','695','696','697','698','699') AND colid='288' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '1_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('693','694','695','696','697','698','699') AND colid='377' AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2004' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1994','1995','1996','1997','1998','1999') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2005' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1994','1995','1996','1997','1998','1999') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2006' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1994','1995','1996','1997','1998','1999') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2007' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1994','1995','1996','1997','1998','1999') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'",
				  '2_2008' => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE linid IN('1994','1995','1996','1997','1998','1999') AND colid IN('327','328','329') AND ctiexercicio='".$ano."' AND esuid='".$_SESSION['rehuf_var']['esuid']."'");

	$vlr1 = $db->pegaUm($sqls['1_'.$ano]);
	$vlr2 = $db->pegaUm($sqls['2_'.$ano]);
	if($vlr2 != 0) {
		return number_format(round(($vlr1/$vlr2)*100, 2), 2, ',', '')." %";
	} else {
		return "0,00 %";	
	}
}

function formulaindicador_infragestao1($itm1, $itm2, $ano) {
	global $db;
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND opcid='275' AND ctiexercicio='{ano}'",
				  2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND opcid='224' AND ctiexercicio='{ano}'",
				  3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND opcid='106' AND ctiexercicio='{ano}'",
				  4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND opcid='391' AND ctiexercicio='{ano}'",
				  5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('232','233','234','235') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_infragestao2($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				  		LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				  		WHERE cdi.esuid='{esuid}' AND cdi.colid IN('334','336','338','340','342','344','346','348','350') AND lin.opcid='275' AND cdi.ctiexercicio='{ano}'",
				  2 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi
				  		LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				  		WHERE cdi.esuid='{esuid}' AND cdi.colid IN('334','336','338','340','342','344','346','348','350') AND lin.opcid='224' AND cdi.ctiexercicio='{ano}'",
				  3 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				  		LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				  		WHERE cdi.esuid='{esuid}' AND cdi.colid IN('334','336','338','340','342','344','346','348','350') AND lin.opcid='106' AND cdi.ctiexercicio='{ano}'",
				  4 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				  		LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				  		WHERE cdi.esuid='{esuid}' AND cdi.colid IN('334','336','338','340','342','344','346','348','350') AND lin.opcid='391' AND cdi.ctiexercicio='{ano}'",
				  5 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('232','233','234','235') AND ctiexercicio='{ano}'");
	
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_infragestao3($itm1, $itm2, $ano) {
	global $db;
	
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('327','328','329') AND linid='1979' AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='542' AND ctiexercicio='{ano}'");
	
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2)/12,2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_infragestao4($itm1, $itm2, $ano) {
	global $db;
	$sqls = array(0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid='660' AND ctiexercicio='{ano}'",
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN('334','336','338','340','342','344','346','348','350') AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm1]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[$itm2]));
	if($vlr2 && $vlr2 > 0) {
		return number_format(round(($vlr1/$vlr2),2), 2, ',', '');
	} else {
		return "0,00";
	}
}

function formulaindicador_infragestao5($ano) {
	global $db;
	$sqls = array(// Grupo Tipo de Equipamento (gitid=16) : coluna 74 "Quantidade com contrato de manutenção"
				  0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='74' AND ctiexercicio='{ano}'",
				  // Grupo Tipo de Equipamento (gitid=16) : linha 69 "Próprio"
				  1 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid='69' AND ctiexercicio='{ano}'");
	
	$vlr1 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[0]));
	$vlr2 = $db->pegaUm(str_replace(array("{esuid}","{ano}"),array($_SESSION['rehuf_var']['esuid'],$ano),$sqls[1]));
	
	if($vlr2 != 0) {
		return number_format(round(($vlr1/$vlr2)*100,2), 2, ',', '');
	} else {
		return "0,00 %";
	}
}


/*Funções para montar os graficos*/
function geraGrafico($indicador){
	
	$arquivo_xml = ($_SESSION['rehuf_var']['entid'])."_".($_REQUEST['indid'])."_".$indicador.".xml";
	
	$caminho = '/graficos/rehuf/xml/'; //OK
	$script = ("<script type=\"text/javascript\">
	var grafico_$indicador = new FusionCharts(\"/includes/FusionCharts/FusionChartsFree_V3/Charts/MSColumn3D.swf\", \"graf_$indicador\", \"850\", \"350\", \"0\", \"0\");
	grafico_$indicador.setDataURL('{$caminho}$arquivo_xml');
	grafico_$indicador.render(\"graf_$indicador\");
   </script>");
   
   return $script;
}

function geraXML_Grafico($arrValores,$indicador,$titulo,$arrCriticas){
	if(!is_dir(APPRAIZ."www/graficos")) {
		mkdir(APPRAIZ."www/graficos", 0777);
	}
	if(!is_dir(APPRAIZ."www/graficos/rehuf")) {
		mkdir(APPRAIZ."www/graficos/rehuf", 0777);
		mkdir(APPRAIZ."www/graficos/rehuf/xml", 0777);
	}
	if(!is_dir(APPRAIZ."www/graficos/rehuf/xml")) {
		mkdir(APPRAIZ."www/graficos/rehuf/xml", 0777);
	}
	$caminho = APPRAIZ."www/graficos/rehuf/xml/"; //
	//endid + indid + sub-indicador
	$arquivo_xml = ($_SESSION['rehuf_var']['entid'])."_".($_REQUEST['indid'])."_".$indicador.".xml";
		
	
	foreach($arrValores[$indicador] as $vl){
		$vl = str_replace(',','.',$vl);
		$vl = str_replace('%','',$vl);
		$vl = trim($vl);
		$vl = (float)$vl;
		$arrMax[] = $vl;
	}
	//valor máximo do eixo
	$max = max($arrMax);
	
	$dataset .= "<categories>";
		
	foreach($arrValores[$indicador] as $ano => $ind){
		$valor = str_replace(',','.',$ind);
		$valor = str_replace('%','',$valor);
		$valor = trim($valor);
		$valor = (float)$valor;
		$dataset .= '<category label="'.$ano.'"/>';
				
		foreach($arrCriticas as $tipo){
			$item = explode('|',$tipo);
			($item[1] == 'max')? $item[1] = $max : $item[1] = $item[1];
			$v1 = (float)$item[0];
			$v2 = (float)$item[1];
			
			if($valor >= $v1 && $valor <= $v2){
				$arrCt[$item[3]][] = '<set value="'.$valor.'"/>';
			}
			else{
				$arrCt[$item[3]][] = '<set value=""/>';
			}
		}
	}
	
	//Formação do XML
	$dataset .= "</categories>";
	
	foreach($arrCriticas as $tipo){
		$item = explode('|',$tipo);
		$dataset .= "<category label=\"".$item[3]."\"/>";
		
		$dataset .= "<dataset seriesName=\"{$item[3]}\" color=\"{$item[2]}\" showValues=\"1\">";
		foreach($arrCt[$item[3]] as $sv){
			$dataset .= $sv;
		}
		$dataset .= "</dataset>";		
	}
	
	$conteudo_xml  = '<chart canvasbgAlpha="50" showLegend="0" numberSuffix="%" shownames="1" decimals="2" >';
	$conteudo_xml .= "$dataset";
	
	$conteudo_xml .= "<styles>
        <definition>
            <style name='myLabelsFont' type='font' font='Arial' size='16' color='666666' bold='1' underline='0'/>
            <style name='myShadow' type='Shadow' color='999999' angle='45'/> 
        </definition>
        <application>
            <apply toObject='DataLabels' styles='myLabelsFont' />
             <apply toObject='DataValues' styles='myShadow' />
        </application>
    </styles>";
	
	$conteudo_xml .= '</chart>';
	
	$xml = fopen($caminho.$arquivo_xml, 'w');
	fwrite($xml, $conteudo_xml);
	fclose($xml);
}

function criaGraficos($arrValores,$arrInd,$arrAnos,$indicador){
	
	if(count($arrValores) == 0){
		echo "Valores não cadastrados.";
		return;
	}
	
	if($indicador != 'total'){
		switch($indicador){
			case 0:
				//array de críticas para valores e cores = min | max | cor | legenda
				$arrCriticas = array("0|70|AA0000|Ruim","70.01|110|FFFF00|Bom","110.01|max|00FF00|Ótimo");
				break;
			case 1:
				//array de críticas para valores e cores = min | max | cor | legenda
				$arrCriticas = array("0|50|AA0000|Ruim","50.01|75|FFFF00|Bom","75.01|max|00FF00|Ótimo");
				break;
			case 2:
				//array de críticas para valores e cores = min | max | cor | legenda
				$arrCriticas = array("0|60|AA0000|Ruim","60.01|80|FFFF00|Bom","80.01|max|00FF00|Ótimo");
				break;
			default:
				//array de críticas para valores e cores = min | max | cor | legenda
				$arrCriticas = array("0|50|AA0000|Ruim","50.01|80|FFFF00|Bom","80.01|max|00FF00|Ótimo");
				break;
		}
	}elseif($indicador == 'total'){
		//array de críticas para valores e cores = min | max | cor | legenda
		$arrCriticas = array("0|70|AA0000|Ruim","70.01|75|FFFF00|Bom","75.01|max|00FF00|Ótimo");
	}
	
	$legenda = "<center><table><tr>";
	foreach($arrCriticas as $tipo){
			$item = explode('|',$tipo);
			$legenda .= "<td bgcolor={$item[2]} width=10 ></td><td>{$item[3]}</td><td width=20 ></td>";
		}
	$legenda .= "</tr></table></center>";
	
	
	geraXML_Grafico($arrValores,$indicador,$arrInd[$indicador],$arrCriticas);
	$graficos .= geraGrafico($indicador);
	$div.= ("<div class=\"grafico\"><div class=\"grafico_flash\" id=\"graf_$indicador\" ></div><div class=\"legenda\">$legenda</div></div>");
	echo "<script>document.getElementById('exibe_graficos').innerHTML += '<div class=\"titulo_sub_indicador\">{$arrInd[$indicador]}</div>$div';</script>";
	echo $graficos;	
}

function montaGraficos($indicador,$arrInd, $arrAnos, $funcao,$sub1,$sub2){
	
	$caminho = "/graficos/rehuf/xml/"; //
	
	echo ("<script type=\"text/javascript\" src=\"/includes/FusionCharts/FusionChartsFree_V3/JSClass/FusionCharts.js\"></script>");
	
	if(count($arrInd) == 0){
		echo "<script>document.getElementById('exibe_graficos').innerHTML = 'Indicadores não informados.';</script>";
		return;
	}
	if(count($arrAnos) == 0){
		echo "<script>document.getElementById('exibe_graficos').innerHTML = 'Datas não informadas.';</script>";
		return;
	}
	
	if($sub1 != 'false' && $sub2 != 'false'){
		$num = 0;
		foreach($arrAnos as $ano){
			$arrValores[$indicador][$ano] = $funcao(array($sub1,$num),array($sub2,$num),$ano);
			$num++;
		}
	}
	if($sub1 == 'false' && $sub2 == 'false'){
		foreach($arrAnos as $ano){
			$arrValores['total'][$ano] = $funcao($ano);
		}
	}

	criaGraficos($arrValores,$arrInd,$arrAnos,$indicador);
	
}
?>