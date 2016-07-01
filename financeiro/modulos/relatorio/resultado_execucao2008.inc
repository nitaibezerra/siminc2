<?

//ini_set( "memory_limit", "256M" );
//set_time_limit(0);
function retornaSubTotal($subtotal,$j,$color = 'black')
{
	if($subtotal[4] != 0)
	{
		$GLOBALS['colunah'] = $subtotal[7];
		$GLOBALS['colunae'] = $subtotal[4];
		$GLOBALS['colunak'] = $subtotal[10];
	}
	
		
	if($j==9)
	{
		if($GLOBALS['colunae'] == 0)
			$percent = 0;
		else	
			$percent = ($GLOBALS['colunah']/$GLOBALS['colunae'])*100;
			
		echo "<td align=\"right\"><strong><font color=\"$color\">".number_format($percent, 2, ',', '.')."%</font><strong></td>";
	}	
	else if($j==11 )
	{
		if($GLOBALS['colunae'] == 0)
			$percent = 0;
		else
			$percent = ($GLOBALS['colunak']/$GLOBALS['colunae'])*100;
			
		echo "<td align=\"right\"><strong><font color=\"$color\">".number_format($percent, 2, ',', '.')."%</font><strong></td>";
	}	
	else
		echo "<td align=\"right\"><strong><font color=\"$color\">".number_format($subtotal[$j], 0, ',', '.')."</font></strong></td>";
}

function zeraGlobals()
{
	$GLOBALS['colunah'] = 0;
	$GLOBALS['colunae'] = 0;
	$GLOBALS['colunak'] = 0;
}

function retornaParametros($item)
{
	$campo = '';
	
	if($item['excludente'])
		$campo .="<input type=\'hidden\' name=\'{$item['name']}_campo_excludente\' value=\'1\'>";
		
	if(count($item['value']) > 0)
	{
		for($i = 0; $i<count($item['value']);$i++)
		{
			$campo .="<input type=\'hidden\' name=\'{$item['name']}[]\' value=\'{$item['value'][$i]}\'>";
		}	
	}
	else
		$campo .="<input type=\'hidden\' name=\'{$item['name']}[]\' value=\'\'>";

	return $campo;	
}

function imprimiFormulario($id_form, $colunas, $gnd = null, $fonteSOF = null, $fonteRecurso = null, $acao = null, $uo = null,$ug = null,$contaCorrente = null)
{
	$form = "<form name=\'formJunta\' id=\'{$id_form}\' action=\'financeiro.php?modulo=relatorio/geral_teste&acao=R\' target=\'relatJunta\' method=\'post\'>";
	$form .="<input type=\'hidden\' name=\'submetido\' value=\'1\'>";
	$form .="<input type=\'hidden\' name=\'alterar_ano\' value=\'0\'>";
	$form .="<input type=\'hidden\' name=\'publico\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'prtid\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'carregar\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'excluir\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'titulo\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'ano\' value=\'2009\'>";
	$form .="<input type=\'hidden\' name=\'escala\' value=\'true\'>";
	$form .="<input type=\'hidden\' name=\'buscaagrupadorOrigem\' value=\'org\'>";
	$form .="<input type=\'hidden\' name=\'buscaagrupadorOrigemColunas\' value=\'projeto\'>";
	//AGRUPADORES
	$form .="<input type=\'hidden\' name=\'agrupador[]\' value=\'orgao\'>";
	//COLUNAS
	foreach($colunas as $coluna)
		$form .="<input type=\'hidden\' name=\'agrupadorColunas[]\' value=\'$coluna\'>";
		
	//$form .="<input type=\'hidden\' name=\'agrupadorColunas[]\' value=\'$coluna\'>";
	
	$form .= retornaParametros($gnd);
	$form .= retornaParametros($fonteSOF);
	$form .= retornaParametros($fonteRecurso);
	$form .= retornaParametros($acao);
	$form .= retornaParametros($uo);
	$form .= retornaParametros($ug);
	$form .= retornaParametros($contaCorrente);
	
	//$form .="<input type=\'hidden\' name=\'ug[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'grupouo[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'funcao[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'subfuncao[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'programa[]\' value=\'\'>";		
	$form .="<input type=\'hidden\' name=\'ptres[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'planointerno[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'grf[]\' value=\'\'>";	
	$form .="<input type=\'hidden\' name=\'fontesiafi[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'catecon[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'orgao[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'orgaougexecutora[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'gestaoexecutora[]\' value=\'\'>";
	
	$form .="<input type=\'hidden\' name=\'mapcod[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'elemento[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'natureza[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'iduso[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'vincod[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'loccod[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'enquadramento[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'gestor[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'nivel[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'apropriacao[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'modalidade[]\' value=\'\'>";
	$form .="<input type=\'hidden\' name=\'subacao[]\' value=\'\'>";
	$form .="</form>";
	
	return $form;
}

$sql = "SELECT
			junanexo AS anexos,
			sum(junpl) AS pl2009,
			sum(junemendaaditiva) AS emendas_aditivas,
			sum(junemendasupressiva) AS emendas_supressivas,
			sum(junloa) AS loa2009,
			sum(junlimiteparaempenho) AS limites_empenho,
			sum(junloalimite) AS loa_limite,
			sum(jundotacaoautorizada) AS dotacao_autorizada,
			sum(junempenhopreempenho) AS emprenhos_pre_empenhos,
			sum(junlimitenaoempenhado) AS limite_nao_empenhado,
			sum(junempenhadolimite) AS empenhado_limite,
			sum(junlimiteautilizar) AS limite_utilizar,
			sum(junlimiteutilizado) AS limite_utilizado,
			junordem AS ordem
		FROM
			financeiro.junta
		GROUP BY
			junordem,junanexo
		ORDER BY
			junordem";

$dadosExecucao = $db->carregar($sql);
//dbg($dadosExecucao,1);

//INICIO DAS LEGENDAS DAS CELULAS
$constAcao 	= array('0E36','0005','0022','0716','2010','2011','2012','2004','0515','0969','8744','8790');
$contUO		= array("26101","26102","26104","26105","26201","26202","26203","26204","26205","26206","26207","26208","26210","26211","26212","26213","26214","26215","26216","26217","26218","26219","26220","26221","26222","26223","26224","26230","26231","26232","26233","26234","26235","26236","26237","26238","26239","26240","26241","26242","26243","26244","26245","26246","26247","26248","26249","26250","26251","26252","26253","26254","26255","26256","26257","26258","26259","26260","26261","26262","26263","26264","26265","26266","26268","26269","26270","26271","26272","26273","26274","26275","26276","26277","26278","26279","26280","26281","26282","26283","26284","26285","26286","26287","26288","26289","26290","26291","26292","26293","26294","26295","26298","26301","26302","26303","26304","26305","26306","26307","26308","26309","26310","26311","26312","26313","26314","26315","26316","26317","26318","26319","26320","26321","26322","26323","26324","26325","26326","26327","26328","26329","26330","26331","26332","26333","26334","26335","26336","26337","26338","26339","26340","26341","26342","26343","26344","26345","26346","26347","26350","26351","26352","26353","26354","26355","26356","26357","26358","26359","26360","26361","26362","26363","26364","26365","26366","26367","26368","26369","26370","26371","26372","26373","26374","26375","26376","26377","26378","26379","26380","26381","26382","26383","26384","26385","26386","26387","26388","26389","26391","26392","26393","26394","26395","26396","26397","26398","26399","26400","26401");
//1ª Coluna - PL 2009 (a)
$gnd 							= array();
$gnd['name'] 					= 'gnd';
$gnd['value'] 					= array(3,4,5);
$gnd['excludente'] 				= false;

$fonteSOF 						= array();
$fonteSOF['name'] 				= 'fonte';
$fonteSOF['value'] 				= array(100,113,175,249,280,293);
$fonteSOF['excludente']			= false;

$fonteR 						= array();
$fonteR['name'] 				= 'fonterecurso';
$fonteR['value'] 				= array();
$fonteR['excludente']			= false;

$acao 							= array();
$acao['name'] 					= 'acacod';
$acao['value'] 					= $constAcao;
$acao['excludente']				= true;

$uo 							= array();
$uo['name'] 					= 'uo';
$uo['value'] 					= $contUO;
$uo['excludente']				= false;

$ug 							= array();
$ug['name'] 					= 'ug';
$ug['value'] 					= array();
$ug['excludente']				= false;

$sldcontacorrente				= array();
$sldcontacorrente['name'] 		= 'sldcontacorrente';
$sldcontacorrente['value'] 		= array();
$sldcontacorrente['excludente']	= false;

//id do form formado pelo numero da linha  e coluna
$idformcelula			= 00;

$colunaRelat = array(10);

$legenda[0][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	3 - Outras Despesas Correntes<li>	4 – Investimentos<li>	5 - Inversões Financeiras </td></tr><tr><td>Ação</td><td>diferente de: <li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos </td> </tr><tr><td>Fonte SOF</td><td> <li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 10;
$gnd['value'] 			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array('2004','2010','2011','2012','0515','0969','8744','8790');
$acao['excludente']		= false;
$fonteSOF['value'] 		= array();

$legenda[1][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td><li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 20;
$acao['value'] 			= $constAcao;
$acao['excludente']		= true;
$fonteSOF['value'] 		= array();
$fonteR['value'] 		= array(50);

$legenda[2][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td>50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 30;
$acao['value']			= $constAcao;
$acao['excludente']		= true;
$fonteR['value'] 		= array(82);

$legenda[3][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td>82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 40;
$acao['value']			= $constAcao;
$acao['excludente']		= true;
$fonteR['value'] 		= array(12);

$legenda[4][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td>12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 50;
$acao['value']			= array("0E36");
$acao['excludente']		= false;
$gnd['value'] 			= array();
$fonteR['value'] 		= array();
$fonteSOF['value'] 		= array();
$uo['value'] 			= array();


$legenda[5][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Ação</td><td>0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 60;
$gnd['value'] 			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value']			= $constAcao;
$acao['excludente']		= true;
$uo['value'] 			= $contUO;
$fonteR['value'] 		= array(95,96,81);

$legenda[6][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td><li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 70;
$gnd['value'] 			= array();
$acao['value'] 			= array("0005","0022","0716");
$acao['excludente'] 	= false;
$fonteR['value'] 		= array(95,96,81,82,80,50,12,79);
$fonteR['excludente'] 	= true;

$legenda[7][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>Ação</td><td><li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais </td> </tr><tr><td>Fonte Recurso</td><td>diferente de:<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	80 - RECURSOS FINANCEIROS DIRETAMENTE ARRECADADOS<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 80;
$gnd['value'] 			= array(1);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteR['value'] 		= array();

$legenda[8][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>1 - Pessoal e Encargos Sociais </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 90;
$gnd['value'] 			= array(2,6);

$legenda[9][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[10][0] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][0] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][0] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][0] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][0] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula 			= 150;
$gnd['value'] 			= array(9);
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");

$legenda[15][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT. </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>9 - Reserva de Contingência </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos</td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 160;
$uo['value'] 			= array("74902");
$gnd['value'] 			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array("4556");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[16][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT </td></tr><tr><td>Unidade Orçamentária</td><td>74902 - Fundo de Financiamento ao Estudante do Ensino Superior </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>4556 - Administração do Financiamento concedido a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte Recurso</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 170;
$colunaRelat			= array(27); // COLUNA : Créditos Recebidos pela Adm. Direta (1b)
$ug['value'] 			= array("155002");
$acao['value'] 			= array("0579");
$gnd['value'] 			= array();
$uo['value'] 			= array();
$fonteSOF['value'] 		= array();

$legenda[17][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192210101 - CREDITOS RECEBIDOS </td></tr><tr><td>Unidade Gestora</td><td>155002 - FUNDO DE FINANC. AO ESTUDANTE DO ENS.SUPERIOR </td></tr><tr><td>Ação</td><td>0579 - Concessão de Financiamento a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte Recurso</td><td> Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 180;
$colunaRelat			= array(10); // COLUNA: Projeto de lei
$uo['value'] 			= array("73107");
$gnd['value'] 			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$ug['value'] 			= array();

$legenda[18][0] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192520100 - PROJ.INICIAL DA LEI ORCAMENT.S/ INDIC.RESULT </td></tr><tr><td>Unidade Orçamentária</td><td>73107 - Recursos sob supervisão do MEC - Cota-Parte dos Estados e DF do Salário-Educação </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos<li>	0969 - Apoio ao Transporte Escolar na Educação Básica  </td> </tr><tr><td>Fonte Recurso</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

//2ª Coluna - Emendas Aditivas (b)
$legenda[0][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 509.814.320,00</td></tr></table>";
$legenda[1][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[2][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[3][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 97.369.306,00</td></tr></table>";
$legenda[4][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 217.575.023,00</td></tr></table>";
$legenda[5][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[6][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[7][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[8][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[9][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[10][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[15][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 126.876.594,00</td></tr></table>";
$legenda[16][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[17][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[18][1] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

//3ª Coluna - Emendas Supressivas (c)
$legenda[0][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a -462.596.052,00</td></tr></table>";
$legenda[1][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[2][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[3][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[4][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a -1.566.572.107,00</td></tr></table>";
$legenda[5][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[6][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[7][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[8][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[9][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[10][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[15][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[16][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[17][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[18][2] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

//4ª Coluna - LOA 2009 (d)
$colunaRelat			= array(3); //COLUNA: Dotação Inicial
$idformcelula			= 03;
$uo['value'] 			= $contUO;
$gnd['value']			= array(3,4,5);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= true;
 
$legenda[0][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	3 - Outras Despesas Correntes<li>	4 – Investimentos<li>	5 - Inversões Financeiras </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 13;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array("2004","2010","2011","2012","0515","0969","8744","8790");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= true;

$legenda[1][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian</td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td><li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 23;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;

$fonteR['value'] 		= array(50);
$fonteR['excludente'] 	= false;

$legenda[2][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td>50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 33;
$fonteR['value'] 		= array(82);

$legenda[3][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte</td><td>82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 43;
$fonteR['value'] 		= array(12);

$legenda[4][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian   </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td>12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 53;
$gnd['value']			= array();
$gnd['excludente'] 		= true;
$acao['value'] 			= array("0E36");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= true;
$uo['value'] 			= array();
$ug['value'] 			= array();

$legenda[5][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Ação</td><td>0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 63;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteR['value'] 		= array("95","96","81");
$fonteR['excludente'] 	= false;

$legenda[6][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian   </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte Recurso</td><td><li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 73;
$gnd['value']			= array();
$gnd['excludente'] 		= true;
$acao['value'] 			= array("0005","0022","0716");
$acao['excludente'] 	= false;
$fonteR['value'] 		= array("95","96","81","82","80","50","12","79");
$fonteR['excludente'] 	= true;

$legenda[7][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian   </td></tr><tr><td>Ação</td><td><li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais </td> </tr><tr><td>Fonte Recurso</td><td>Diferente de:<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	80 - RECURSOS FINANCEIROS DIRETAMENTE ARRECADADOS<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 83;
$gnd['value']			= array(1);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= true;

$legenda[8][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>1 - Pessoal e Encargos Sociais </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 93;
$gnd['value']			= array(2,6);
$gnd['excludente'] 		= false;

$legenda[9][3]  = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian   </td></tr><tr><td>GND</td><td><li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[10][3] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][3] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][3] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][3] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][3] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula			= 153;
$gnd['value']			= array(9);
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;

$legenda[15][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian   </td></tr><tr><td>GND</td><td>9 - Reserva de Contingência </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 163;
$gnd['value']			= array(1,2,6);
$gnd['excludente']		= true;
$acao['value'] 			= array("4556");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[16][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>74902 - Fundo de Financiamento ao Estudante do Ensino Superior </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>4556 - Administração do Financiamento concedido a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte Recurso</td><td> Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 173;
$colunaRelat			= array(27); // COLUNA : Créditos Recebidos pela Adm. Direta (1b)
$ug['value'] 			= array("155002");
$gnd['value'] 			= array();
$acao['value'] 			= array("0579");
$gnd['excludente'] 		= false;
$uo['value'] 			= array();
$fonteSOF['value'] 		= array();

$legenda[17][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192210101 - CREDITOS RECEBIDOS </td></tr><tr><td>Unidade Gestora</td><td>155002 - FUNDO DE FINANC. AO ESTUDANTE DO ENS.SUPERIOR </td></tr><tr><td>Ação</td><td>0579 - Concessão de Financiamento a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte recurso</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula 			= 180;
$colunaRelat			= array(3); //Coluna dotação inicial
$uo['value'] 			= array("73107");
$gnd['value'] 			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$ug['value'] 			= array();

$legenda[18][3] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>192110101 - ORIGINARIO DO OGU </td></tr><tr><td>Unidade Orçamentária</td><td>73107 - Recursos sob supervisão do MEC - Cota-Parte dos Estados e DF do Salário-Educação </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida </td></tr><tr><td>Ação</td><td>diferente de:<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos<li>	0969 - Apoio ao Transporte Escolar na Educação Básica  </td> </tr><tr><td>Fonte Recurso</td><td> Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

//5ª Coluna - Limites para Empenho (e)

$colunaRelat			= array(149); //COLUNA: Limites para empenho
$idformcelula			= 04;
$uo['value'] 			= array("26101");
$sldcontacorrente['value'] = array('NCA', 'NIA');
$ug['value']			= array();
$ug['excludente']		= false;
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array();
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[0][4] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td> 293110699 - DIVERSAS COTAS DE LIMITE ORCAMENTARIO</td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NCA\', \'NIA\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 14;
$sldcontacorrente['value'] = array('NOA', 'NOB', 'NOD');	

$legenda[1][4] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td> 293110699 - DIVERSAS COTAS DE LIMITE ORCAMENTARIO </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NOA\', \'NOB\', \'NOD\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 24;
$sldcontacorrente['value'] = array('NCB', 'NIB');	

$legenda[2][4] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td> 293110699 - DIVERSAS COTAS DE LIMITE ORCAMENTARIO </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NCB\', \'NIB\' </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 34;
$sldcontacorrente['value'] = array('NCC', 'NIC');	

$legenda[3][4] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110699 - DIVERSAS COTAS DE LIMITE ORCAMENTARIO </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td> \'NCC\', \'NIC\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 44;
$sldcontacorrente['value'] = array(	'NCD', 'NID');	

$legenda[4][4] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110699 - DIVERSAS COTAS DE LIMITE ORCAMENTARIO</td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta</td></tr><tr><td>Conta Corrente</td><td>\'NCD\', \'NID\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[5][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna PL 2009</td></tr></table>";
$legenda[6][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna PL 2009</td></tr></table>";
$legenda[7][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna PL 2009</td></tr></table>";
$legenda[8][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>: Valor igual à coluna PL 2009 Valor igual à coluna PL 2009</td></tr></table>";
$legenda[9][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna PL 2009</td></tr></table>";
$legenda[10][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[15][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[16][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[17][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna LOA 2009</td></tr></table>";
$legenda[18][4] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor igual a coluna LOA 2009</td></tr></table>";

//6ª Coluna - (f) LOA - LIMITE (f) = (d) - (e)
$legenda[0][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[1][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[2][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[3][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[4][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[5][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[6][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[7][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[8][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[9][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[10][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[11][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[12][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[13][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[14][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[15][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[16][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[17][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";
$legenda[18][5] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'LOA 2009 (D) \'  e \'Limites para empenho (E) \' <br>Fórmula: \'LOA 2009\' - \'Limites para empenho\'  </td></tr></table>";

//7ª Coluna - Dotação Autorizada (Lei + Créditos) (g)
$colunaRelat			= array(19); //COLUNA: Dotação autorizada
$idformcelula			= 06;
$uo['value'] 			= $contUO;
$gnd['value']			= array(3,4,5);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[0][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	3 - Outras Despesas Correntes<li>	4 – Investimentos<li>	5 - Inversões Financeiras </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 16;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array("2004","2010","2011","2012","0515","0969","8744","8790");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();

$legenda[1][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida   </td></tr><tr><td>Ação</td><td><li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos    </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 26;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteR['value'] 		= array("50");

$legenda[2][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de Recurso</td><td>50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 36;
$fonteR['value'] 		= array("82");

$legenda[3][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td>82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 46;
$fonteR['value'] 		= array("12");

$legenda[4][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td>12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 56;
$uo['value'] 			= array();
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array("0E36");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[5][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Ação</td><td>0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 66;
$uo['value'] 			= $contUO;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("95","96","81");
$fonteR['excludente'] 	= false;

$legenda[6][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td><li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS    </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 76;
$gnd['value']			= array();
$gnd['excludente'] 		= true;
$acao['value'] 			= array("0005","0022","0716");
$acao['excludente'] 	= false;
$fonteR['value'] 		= array("95","96","81","82","80","50","12","79");
$fonteR['excludente'] 	= true;


$legenda[7][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais  </td></tr><tr><td>Ação</td><td><li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	80 - RECURSOS FINANCEIROS DIRETAMENTE ARRECADADOS<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 86;
$uo['value'] 			= $contUO;
$gnd['value']			= array(1);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[8][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>1 - Pessoal e Encargos Sociais </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 96;
$gnd['value']			= array(2,6);

$legenda[9][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[10][6] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula			= 116;
$uo['value'] 			= array();
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array();;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array(300);
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[11][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Fonte SOF</td><td>300 - Recursos Ordinários </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 126;
$fonteSOF['value'] 		= array(312);
$acao['excludente'] 	= false;

$legenda[12][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><td>Fonte SOF</td><td>312 - Recursos Destinados a Manutenção e Desenvolvimento do Ensino </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 136;
$fonteSOF['value'] 		= array(650);

$legenda[13][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td><tr><td>Fonte SOF</td><td>650 - Recursos Próprios Não-Financeiros </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[14][6] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula			= 156;
$uo['value'] 			= $contUO;
$gnd['value']			= array(9);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[15][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>9 - Reserva de Contingência </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 166;
$uo['value'] 			= array(74902);
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array(4556);
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[16][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGUli>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>74902 - Fundo de Financiamento ao Estudante do Ensino Superior </td></tr><tr><td>GND</td><td>Diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>4556 - Administração do Financiamento concedido a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 176;
$uo['value'] 			= array(155002);
$acao['value'] 			= array("0579");


$legenda[17][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>155002 - FUNDO DE FINANC. AO ESTUDANTE DO ENS.SUPERIOR </td></tr><td>Ação</td><td>0579 - Concessão de Financiamento a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA  </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 186;
$uo['value'] 			= array(73107);
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[18][6] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	192110101 - ORIGINÁRIO DO OGU<li>	192190301 – ACRÉSCIMO<li>	192190209 – REDUÇÃO<li>	192110201 - ANTECIPAÇÃO – LDO<li>	192130201 - CRÉDITOS ESPECIAIS REABERTOS<li>	192140100 - CRÉDITOS EXTRAORD. ABERTOS<li>	192140200 - CRÉDITOS EXTRAORD. REABERTOS<li>	192190302 – REDUÇÃO<li>	192110303 - ORIGINÁRIO DO OGU - SUPLEMENTAÇÃO AUTOMÁTICA<li>	192110301 - ORIGINÁRIO DO OGU<li>	192110209 - ANULAÇÃO DA ANTECIPAÇÃO – LDO<li>	192130101 - CRÉDITOS ESPECIAIS<li>	192130102 - CRÉDITOS ESPECIAIS – SUPLEMENTAÇÃO<li>	192190201 - ACRÉSCIMO </td></tr><tr><td>Unidade Orçamentária</td><td>73107 - Recursos sob supervisão do MEC - Cota-Parte dos Estados e DF do Salário-Educação </td></tr><tr><td>GND</td><td>Diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos<li>	0969 - Apoio ao Transporte Escolar na Educação Básica   </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA    </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

//8ª Coluna - Empenhos + Pré-Empenhos (h)

$colunaRelat			= array(151); //COLUNA: Empenhos + Pré-Empenhos
$idformcelula			= 07;
$uo['value'] 			= $contUO;
$gnd['value']			= array(3,4,5);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[0][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	3 - Outras Despesas Correntes<li>	4 – Investimentos<li>	5 - Inversões Financeiras  </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação    </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 17;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array("2004","2010","2011","2012","0515","0969","8744","8790");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();

$legenda[1][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>Diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td><li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 27;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteR['value'] 		= array("50");

$legenda[2][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte</td><td>50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 37;
$fonteR['value'] 		= array("82");

$legenda[3][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td>82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 36;
$fonteR['value'] 		= array("12");

$legenda[4][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td>12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 57;
$uo['value'] 			= array();
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array("0E36");
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[5][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Ação</td><td>0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 67;
$uo['value'] 			= $contUO;
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("95","96","81");
$fonteR['excludente'] 	= false;

$legenda[6][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Fonte de recursos</td><td><li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS    </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 77;
$gnd['value']			= array();
$gnd['excludente'] 		= true;
$acao['value'] 			= array("0005","0022","0716");
$acao['excludente'] 	= false;
$fonteR['value'] 		= array("95","96","81","82","80","50","12","79");
$fonteR['excludente'] 	= true;

$legenda[7][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>Ação</td><td><li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais  </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	80 - RECURSOS FINANCEIROS DIRETAMENTE ARRECADADOS<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 87;
$uo['value'] 			= $contUO;
$gnd['value']			= array(1);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[8][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>1 - Pessoal e Encargos Sociais </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 97;
$gnd['value']			= array(2,6);

$legenda[9][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td><li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos   </td> </tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[10][7] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula			= 117;
$uo['value'] 			= array();
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array();;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array(300);
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[11][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Fonte SOF</td><td>300 - Recursos Ordinários </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 127;
$fonteSOF['value'] 		= array(312);
$acao['excludente'] 	= false;

$legenda[12][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Fonte SOF</td><td>312 - Recursos Destinados a Manutenção e Desenvolvimento do Ensino</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 137;
$fonteSOF['value'] 		= array(650);

$legenda[13][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Fonte SOF</td><td>650 - Recursos Próprios Não-Financeiros </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[14][7] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

$idformcelula			= 157;
$uo['value'] 			= $contUO;
$gnd['value']			= array(9);
$gnd['excludente'] 		= false;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array("100","113","175","249","280","293");
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[15][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>Entre as unidades 26101- Administração Direta e 26401- Hospital Universitário Maria Aparecida Pedrossian  </td></tr><tr><td>GND</td><td>9 - Reserva de Contingência </td></tr><tr><td>Ação</td><td>diferente de:<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	0969 - Apoio ao Transporte Escolar na Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos  </td> </tr><tr><td>Fonte SOF</td><td><li>	100 - Recursos Ordinários<li>	113 - Contribuição do Salário-Educação<li>	175 - Taxas por Serviços Públicos<li>	249 - Operações de Crédito Externas - em Bens e/ou Serviços<li>	280 - Recursos Próprios Financeiros<li>	293 - Produto da Aplicação dos Recursos a Conta do Salário - Educação   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 167;
$uo['value'] 			= array(74902);
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= array(4556);
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[16][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente).imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>74902 - Fundo de Financiamento ao Estudante do Ensino Superior </td></tr><tr><td>GND</td><td>Diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>4556 - Administração do Financiamento concedido a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA   </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$idformcelula			= 177;
$uo['value'] 			= array(155002);
$acao['value'] 			= array("0579");


$legenda[17][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td> <li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR</td></tr><tr><td>Unidade Orçamentária</td><td>155002 - FUNDO DE FINANC. AO ESTUDANTE DO ENS.SUPERIOR </td></tr><tr><td>Ação</td><td>0579 - Concessão de Financiamento a Estudantes do Ensino Superior Não-Gratuito </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS 79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 187;
$uo['value'] 			= array(73107);
$gnd['value']			= array(1,2,6);
$gnd['excludente'] 		= true;
$acao['value'] 			= $constAcao;
$acao['excludente'] 	= true;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$sldcontacorrente['value'] = array();
$ug['value']			= array();
$ug['excludente']		= false;
$fonteR['value'] 		= array("50","12","81","82","96","95","79");
$fonteR['excludente'] 	= true;

$legenda[18][7] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td><li>	292120500 - CRÉDITO PRÉ-EMPENHADO LíQUIDO<li>	292130201 - CRÉDITO EMPENHADO – LIQUIDADO<li>	292130202 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130203 - CRED.EMPENHADO-EXECUTADO POR INSCRIÇÃO DE RP<li>	292130209 - CRÉDITO REALIZADO DE ENTIDADE POR INTEGRAÇÃO<li>	292130301 - CRÉDITO PAGO – FOLHA<li>	292130100 - CRÉDITO EMPENHADO A LIQUIDAR </td></tr><tr><td>Unidade Orçamentária</td><td>73107 - Recursos sob supervisão do MEC - Cota-Parte dos Estados e DF do Salário-Educação </td></tr><tr><td>GND</td><td>Diferente de:<li>	1 - Pessoal e Encargos Sociais<li>	2 - Juros e Encargos da Dívida<li>	6 - Amortização da Dívida  </td></tr><tr><td>Ação</td><td>Diferente de:<li>	0005 - Cumprimento de Sentença Judicial Transitada em Julgado (Precatórios) devida pela União, Autarquias e Fundações Públicas<li>	0022 - Cumprimento de Sentença Judicial Transitada em Julgado Devida por Empresas Públicas e Sociedades de Economia Mista<li>	0716 - Cumprimento de Débitos Judiciais Periódicos Vincendos Devidos pela União, Autarquias e Fundações Públicas Federais<li>	0E36 - Complementação da União ao Fundo de Manutenção e Desenvolvimento da Educação Básica e de Valorização dos Profissionais da Educação – FUNDEB<li>	2004 - Assistência Médica e Odontológica aos Servidores, Empregados e seus Dependentes<li>	2010 - Assistência Pré-Escolar aos Dependentes dos Servidores e Empregados<li>	2011 - Auxílio-Transporte aos Servidores e Empregados<li>	2012 - Auxílio-Alimentação aos Servidores e Empregados<li>	0515 - Dinheiro Direto na Escola para a Educação Básica<li>	8744 - Apoio à Alimentação Escolar na Educação Básica<li>	8790 - Apoio à Alfabetização e à Educação de Jovens e Adultos<li>	0969 - Apoio ao Transporte Escolar na Educação Básica   </td> </tr><tr><td>Fonte de recursos</td><td>Diferente de:<li>	50 - RECURSOS NAO-FINANCEIROS DIRETAM. ARRECADADOS<li>	12 - RECURSOS DEST.A MANUT.E DES.DO ENSINO<li>	81 - RECURSOS DE CONVENIOS<li>	82 - RESTIT. RECURSOS DE CONVENIOS E CONGENERES<li>	96 - DOACOES DE PESSOAS OU INSTIT. PRIV. NACIONAIS<li>	95 - DOACOES DE ENTIDADES INTERNACIONAIS<li>	79 - FUNDO DE COMBATE A ERRADICACAO DA POBREZA    </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

//9ª Coluna - Sd de Limite não Empenhado (i) = (e) - (h)
$legenda[0][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[1][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[2][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[3][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[4][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[5][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[6][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[7][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[8][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[9][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[10][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[11][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[12][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[13][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[14][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[15][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[16][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[17][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";
$legenda[18][8] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a diferença das colunas \'Limites para Empenho(E)\'  e \'Empenhos + Pré-Empenhos (H)\' <br>Fórmula: \'Limites para Empenho\' - \'Empenhos + Pré-Empenhos\'  </td></tr></table>";

//10ª Coluna - Emp. x Limite (%) (j) = (h)/(e)*100
$legenda[0][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[1][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[2][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[3][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[4][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[5][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[6][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[7][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[8][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[9][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[10][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[11][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[12][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[13][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[14][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[15][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[16][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[17][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[18][9] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Empenhos + Pré-Empenhos (H)\' e \'Limites para Empenho (E)\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0	<br>Fórmula:  ( (\'Empenhos + Pré-Empenhos\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";

//11ª Coluna - Cota Limite a Utilizar (k)
$colunaRelat			= array(152); //COLUNA: Cota limite a utilizar
$idformcelula			= 010;
$uo['value'] 			= array("26101");
$sldcontacorrente['value'] = array('NCA', 'NIA');
$ug['value']			= array();
$ug['excludente']		= false;
$gnd['value']			= array();
$gnd['excludente'] 		= false;
$acao['value'] 			= array();
$acao['excludente'] 	= false;
$fonteSOF['value'] 		= array();
$fonteSOF['excludente'] = false;
$fonteR['value'] 		= array();
$fonteR['excludente'] 	= false;

$legenda[0][10] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110601 - COTA DE LIMITE ORÇAMENTÁRIO A UTILIZAR </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NCA\', \'NIA\' </td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 110;
$sldcontacorrente['value'] = array('NOA', 'NOB', 'NOD');	

$legenda[1][10] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110601 - COTA DE LIMITE ORÇAMENTÁRIO A UTILIZAR </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NOA\', \'NOB\', \'NOD\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 210;
$sldcontacorrente['value'] = array('NCB', 'NIB');	

$legenda[2][10] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110601 - COTA DE LIMITE ORÇAMENTÁRIO A UTILIZAR</td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta</td></tr><tr><td>Conta Corrente</td><td>\'NCB\', \'NIB\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 310;
$sldcontacorrente['value'] = array('NCC', 'NIC');	

$legenda[3][10] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110601 - COTA DE LIMITE ORÇAMENTÁRIO A UTILIZAR </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NCC\', \'NIC\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";

$idformcelula			= 410;
$sldcontacorrente['value'] = array(	'NCD', 'NID');	

$legenda[4][10] = imprimiFormulario($idformcelula,$colunaRelat,$gnd,$fonteSOF,$fonteR,$acao,$uo, $ug, $sldcontacorrente)."<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Conta Contábil:</td><td>293110601 - COTA DE LIMITE ORÇAMENTÁRIO A UTILIZAR </td></tr><tr><td>Unidade Orçamentária</td><td>26101 - Administração Direta </td></tr><tr><td>Conta Corrente</td><td>\'NCD\', \'NID\'</td></tr><tr><td>Gerar Relatório</td><td><input type=\'button\' name=\'relatorio\' value=\'Gerar Relatório\' onclick=\'gerarRelatorio($idformcelula);\' /></td></tr></table>";
$legenda[5][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[6][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[7][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[8][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[9][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[10][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[11][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[12][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[13][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[14][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[15][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[16][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[17][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";
$legenda[18][10] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Valor fixo igual a 0,00 (zero)</td></tr></table>";

//12ª Coluna - % Limite Utilizado (l) = (k)/(e)*100
$legenda[0][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[1][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[2][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[3][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[4][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[5][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[6][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[7][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[8][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[9][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[10][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[11][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[12][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[13][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[14][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[15][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[16][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[17][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";
$legenda[18][11] = "<table style=\'width:100%;  border-collapse: collapse;\' cellspacing=\'0\' cellpadding=\'3\' border=1><tr><td>Regra</td><td>Efetua a divisão das colunas \'Cota Limite a Utilizar\' e \'Limites para Empenho\' e depois multiplica por 100, caso a coluna \'Limites para Empenho\' for maior que 0. <br>Fórmula:  ( (\'Cota Limite a Utilizar\' /  \'Limites para Empenho\') * 100 )</td></tr></table>";


$linha[0] = "Linha: &nbsp;&nbsp;Anexo I (NCA+NIA) - Demais não incluidas nas classificações abaixo <br>";
$linha[1] = "Linha: &nbsp;&nbsp; Obrigatórias (NOA, NOB, NOD) - PNAE, PNATE, EJA, PDDE, Ass. Méd, Aux Creche, aux alim, Aux Transporte<br>";
$linha[2] = "Linha: &nbsp;&nbsp; Anexo II (NCB+NIB) - 50<br>";
$linha[3] = "Linha: &nbsp;&nbsp; Anexo III (NCC+NIC) - 82<br>";
$linha[4] = "Linha: &nbsp;&nbsp; Anexo IV (NCD+NID) - 112<br>";
$linha[5] = "Linha: &nbsp;&nbsp; FUNDEB<br>";
$linha[6] = "Linha: &nbsp;&nbsp; Convênios(81) e Doações(95 e 96)<br>";
$linha[7] = "Linha: &nbsp;&nbsp; Precatórios - Ações 0005, 0022 e 0716<br> ";
$linha[8] = "Linha: &nbsp;&nbsp; Pessoal e Encargos Sociais - GND 1<br>";
$linha[9] = "Linha: &nbsp;&nbsp; Serviços da Dívida - GND 2 e 6<br>";
$linha[10] = "Linha: &nbsp;&nbsp; Créditos Especiais - Abertos e Reabertos<br>";
$linha[11] = "Linha: &nbsp;&nbsp; Fnt 300 - Anexo I<br>";
$linha[12] = "Linha: &nbsp;&nbsp; Fnt 312 - Anexo IV<br>";
$linha[13] = "Linha: &nbsp;&nbsp; Fnt 650 - Anexo II<br>";
$linha[14] = "Linha: &nbsp;&nbsp; Créditos Extraordinários - Abertos e Reabertos<br>";
$linha[15] = "Linha: &nbsp;&nbsp; Reserva de Contingência - FNDE (Fte 113)<br>";
$linha[16] = "Linha: &nbsp;&nbsp; Administração FIES(LOA)<br>";
$linha[17] = "Linha: &nbsp;&nbsp; Concessão de Financiamento ao Esutdante - FIES(LOA)<br>";
$linha[18] = "Linha: &nbsp;&nbsp; Cota Parte do Salário Educação<br>";



$coluna[0] = "Coluna: PL 2009";
$coluna[1] = "Coluna: Emendas Aditivas";
$coluna[2] = "Coluna: Emendas Supressivas";
$coluna[3] = "Coluna: LOA 2009";
$coluna[4] = "Coluna: Limites para Empenho";
$coluna[5] = "Coluna: LOA - LIMITE";
$coluna[6] = "Coluna: Dotação Autorizada ";
$coluna[7] = "Coluna: Empenhos + Pré-Empenhos";
$coluna[8] = "Coluna: Sd de Limite não Empenhado";
$coluna[9] = "Coluna: Emp. x Limite (%)";
$coluna[10] = "Coluna: Cota Limite a Utilizar";
$coluna[11] = "Coluna: % Limite Utilizado";

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
a:hover{
	cursor: pointer;
	text-decoration: none;
}/*
.listagem td:hover{
cursor: pointer;
}*/
</style>
<script type="text/javascript">
	function gerarRelatorio(idform)
	{
		formulario = document.getElementById(idform);
		var janela = window.open( 'financeiro.php?modulo=relatorio/geral_teste&acao=R', 'relatJunta', 'width=530,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		formulario.submit();
	}			
</script>
<!-- 
<form name="formJunta" id='formulario' action="financeiro.php?modulo=relatorio/geral_teste&acao=R" target="relatJunta" method="post">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="alterar_ano" value="0">
<input type="hidden" name="publico" value="">
<input type="hidden" name="prtid" value="">
<input type="hidden" name="carregar" value="">
<input type="hidden" name="excluir" value="">
<input type="hidden" name="titulo" value="">
<input type="hidden" name="ano" value="2009">
<input type="hidden" name="escala" value="true">
<input type="hidden" name="buscaagrupadorOrigem" value="">
<input type="hidden" name="agrupador[]" value="orgao">
<input type="hidden" name="buscaagrupadorOrigemColunas" value="projeto">
<input type="hidden" name="agrupadorColunas[]" value="10">
<input type="hidden" name="orgao[]" value="">
<input type="hidden" name="orgaougexecutora[]" value="">
<input type="hidden" name="gestaoexecutora[]" value="">
<input type="hidden" name="ug" value="">
<input type="hidden" name="ug[]" value="">
<input type="hidden" name="grupouo[]" value="">
<input type="hidden" name="funcao[]" value="">
<input type="hidden" name="subfuncao[]" value="">
<input type="hidden" name="programa[]" value="">
<input type="hidden" name="orgao[]" value="">
<input type="hidden" name="acacod[]" value="">
<input type="hidden" name="ptres[]" value="">
<input type="hidden" name="planointerno[]" value="">
<input type="hidden" name="grf[]" value="">
<input type="hidden" name="fonte[]" value="">
<input type="hidden" name="fonterecurso[]" value="">
<input type="hidden" name="fontesiafi[]" value="">
<input type="hidden" name="catecon[]" value="">
<input type="hidden" name="gnd[]" value="">
<input type="hidden" name="mapcod[]" value="">
<input type="hidden" name="elemento[]" value="">
<input type="hidden" name="natureza[]" value="">
<input type="hidden" name="iduso[]" value="">
<input type="hidden" name="vincod[]" value="">
<input type="hidden" name="loccod[]" value="">
<input type="hidden" name="enquadramento[]" value="">
<input type="hidden" name="gestor[]" value="">
<input type="hidden" name="nivel[]" value="">
<input type="hidden" name="apropriacao[]" value="">
<input type="hidden" name="modalidade[]" value="">
<input type="hidden" name="subacao[]" value="">
</form> -->
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2">
			<img src="../imagens/brasao.gif" width="45" height="45" border="0">
		</td>
		<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				SIMEC- Sistema Integrado do Ministério da Educação<br/>
				Acompanhamento da Execução Orçamentária<br/>
				MEC / SE - Secretaria Executiva <br />
				SPO - Subsecretaria de Planejamento e Orçamento
			</font>
		</td>
		<?
			$sql = "SELECT
						to_char(max(jundata)- interval'1 day', 'DD/MM/YYYY')
					FROM
						financeiro.junta";
			$dataAualizados = $db->pegaUm($sql);
		?>
		<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				<b>Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
				Dados atualizados até: <?= $dataAualizados ?>
			</font>
		</td>
		<tr><td>&nbsp;</td></tr>
		<tr>
			<td colspan="3" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:18px;">Relatório da Junta 2009</font></b>
			</td>
		</tr>
		<tr>
			<td colspan="3" align="center"  valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:12px;" color='red'>Em Homologação</font></b>
			</td>
		</tr>
	
</table>
<table style="border-bottom: 2px solid #000000;" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Anexos</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();"><strong>PL 2009<br>(a)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados informados pela SPO',WIDTH, 250);" onmouseout="return nd();" ><strong>Emendas Aditivas<br>(b)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados informados pela SPO',WIDTH, 250);" onmouseout="return nd();" ><strong>Emendas Supressivas<br>(c)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();" ><strong>LOA 2009<br>(d)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();" ><strong>Limites para Empenho<br>(e)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('(LOA 2009) - (Limites para Empenho)',WIDTH, 250);" onmouseout="return nd();" ><strong>LOA - LIMITE<br>(f) = (d) - (e)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();" ><strong>Dotação Autorizada (Lei + Créditos)<br>(g)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();"><strong>Empenhos + Pré-Empenhos<br>(h)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('(Limites para Empenho) - (Empenhos + Pré-Empenhos)',WIDTH, 350);" onmouseout="return nd();"><strong>Sd de Limite não Empenhado<br>(i) = (e) - (h)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('( ((Empenhos + Pré-Empenhos) / (Limites para Empenho)) * 100 )',WIDTH, 390);" onmouseout="return nd();" ><strong>Emp. x Limite (%)<br>(j) = (h)/(e)*100 </strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('Dados extraídos do SIAFI Gerencial',WIDTH, 250);" onmouseout="return nd();"><strong>Cota Limite a Utilizar<br>(k)</strong></td>
			<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="return overlib('( ((Cota Limite a Utilizar) / (Limites para Empenho)) * 100 )',WIDTH, 350);" onmouseout="return nd();"><strong>% Limite Utilizado<br>(l) = (k)/(e)*100 </strong></td>
		</tr>
	</thead>
	<tbody>
		<?
			$subtotal = array();
			$total = array();
			
			
			
			for($i=0; $i<count($dadosExecucao); $i++) {
				$cont = 0;
				echo "<tr bgcolor=\"#f9f9f9\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#f9f9f9';\">";
				echo "<td align=\"left\">".$dadosExecucao[$i]["anexos"]."</td>";
				
				$total[$cont] 	+= $dadosExecucao[$i]["pl2009"];
				$subtotal[$cont] += $dadosExecucao[$i]["pl2009"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][0]}',CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[0]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["pl2009"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["emendas_aditivas"];
				$subtotal[$cont] += $dadosExecucao[$i]["emendas_aditivas"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][1]}',WIDTH, 600 ,CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[1]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["emendas_aditivas"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["emendas_supressivas"];
				$subtotal[$cont] += $dadosExecucao[$i]["emendas_supressivas"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][2]}',WIDTH, 600 ,CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[2]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["emendas_supressivas"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["loa2009"];
				$subtotal[$cont] += $dadosExecucao[$i]["loa2009"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][3]}',WIDTH, 600 ,CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[3]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["loa2009"], 0, ',', '.')."</a></td>";
				$cont++;
				
				if((integer)$dadosExecucao[$i]["ordem"] == 6 || (integer)$dadosExecucao[$i]["ordem"] == 7 || (integer)$dadosExecucao[$i]["ordem"] == 8 || 
				   (integer)$dadosExecucao[$i]["ordem"] == 9 || (integer)$dadosExecucao[$i]["ordem"] == 10) {
				   	$dadosExecucao[$i]["limites_empenho"] = $dadosExecucao[$i]["pl2009"];
				}
				if((integer)$dadosExecucao[$i]["ordem"] == 18 || (integer)$dadosExecucao[$i]["ordem"] == 19) {
				   	$dadosExecucao[$i]["limites_empenho"] = $dadosExecucao[$i]["loa2009"];
				}
				
				$total[$cont] 	+= $dadosExecucao[$i]["limites_empenho"];
				$subtotal[$cont] += $dadosExecucao[$i]["limites_empenho"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][4]}',WIDTH, 600 ,CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[4]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["limites_empenho"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$loa_limite = ($dadosExecucao[$i]["loa2009"] - $dadosExecucao[$i]["limites_empenho"]);
				$total[$cont] 	+= $loa_limite;
				$subtotal[$cont] += $loa_limite;
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][5]}',WIDTH, 600 ,CENTER,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[5]}');\" onmouseout=\"return nd();\"><a>".number_format($loa_limite, 0, ',', '.')."</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["dotacao_autorizada"];
				$subtotal[$cont] += $dadosExecucao[$i]["dotacao_autorizada"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][6]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[6]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["dotacao_autorizada"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["emprenhos_pre_empenhos"];
				$subtotal[$cont] += $dadosExecucao[$i]["emprenhos_pre_empenhos"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][7]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]} {$coluna[7]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["emprenhos_pre_empenhos"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$sd_limite_empenhado = ($dadosExecucao[$i]["limites_empenho"] - $dadosExecucao[$i]["emprenhos_pre_empenhos"]);
				$total[$cont] 	+= $sd_limite_empenhado;
				$subtotal[$cont] += $sd_limite_empenhado;
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][8]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]} {$coluna[8]}');\" onmouseout=\"return nd();\"><a>".number_format($sd_limite_empenhado, 0, ',', '.')."</a></td>";
				$cont++;

				$empenhado_limite = ($dadosExecucao[$i]["limites_empenho"] != 0) ? (($dadosExecucao[$i]["emprenhos_pre_empenhos"] / $dadosExecucao[$i]["limites_empenho"]) * 100) : 0;
				$total[$cont] 	+= $empenhado_limite;
				$subtotal[$cont] += $empenhado_limite;
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][9]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}{$coluna[9]}');\" onmouseout=\"return nd();\"><a>".number_format($empenhado_limite, 2, ',', '.')."%</a></td>";
				$cont++;
				
				$total[$cont] 	+= $dadosExecucao[$i]["limite_utilizar"];
				$subtotal[$cont] += $dadosExecucao[$i]["limite_utilizar"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][10]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]}  {$coluna[10]}');\" onmouseout=\"return nd();\"><a>".number_format($dadosExecucao[$i]["limite_utilizar"], 0, ',', '.')."</a></td>";
				$cont++;
				
				$limite_utilizado = ($dadosExecucao[$i]["limites_empenho"] != 0) ? (($dadosExecucao[$i]["limite_utilizar"] / $dadosExecucao[$i]["limites_empenho"]) * 100) : 0;
				$total[$cont] 	+= $dadosExecucao[$i]["limite_utilizado"];
				$subtotal[$cont] += $dadosExecucao[$i]["limite_utilizado"];
				echo "<td align=\"right\" onclick=\"return overlib('{$legenda[$i][11]}',WIDTH, 600 ,OFFSETY, 0, STICKY, CAPTION, '{$linha[$i]} {$coluna[11]}');\" onmouseout=\"return nd();\"><a>".number_format($limite_utilizado, 2, ',', '.')."%</a></td>";
				
				echo "<tr>";
								
				if(($i==1) || ($i==4) || ($i==9) || ($i==14) || ($i==15) || ($i==17) || ($i==18)) {
					echo "<tr bgcolor=\"#BFBFBF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#BFBFBF';\">";
					echo "<td align=\"right\"><strong>Subtotal</strong></td>";
					for($j=0; $j<count($subtotal); $j++) 
					{					
						retornaSubTotal($subtotal,$j,'');
						$subtotal[$j] = 0;	
					}
					//zera variaveis que contém os valores da coluna
					zeraGlobals();
					echo "</tr>";
					
					if($i==9) {
						echo "<tr bgcolor=\"#FFF79F\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#FFF79F';\">";
						echo "<td align=\"right\"><strong>Total(Discricionárias + Obrigatórias)</strong></td>";
						for($j=0; $j<count($total); $j++) {
							
							//echo "<td align=\"right\"><strong>".number_format($total[$j], 0, ',', '.')."</strong></td>";
							retornaSubTotal($total,$j);
						}
						//zera variaveis que contém os valores da coluna	
						zeraGlobals();
						echo "</tr>";
					}
					
					if($i==15) {
						echo "<tr bgcolor=\"#AFCCFF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#AFCCFF';\">";
						echo "<td align=\"center\"><strong>Total MEC</strong></td>";
						for($j=0; $j<count($total); $j++) {
							//echo "<td align=\"right\"><strong><font color=\"red\">".number_format($total[$j], 0, ',', '.')."</font></strong></td>";
							retornaSubTotal($total,$j,'red');
						}
						//zera variaveis que contém os valores da coluna	
						zeraGlobals();
						echo "</tr>";
					}
					
					if($i==18) {
						echo "<tr bgcolor=\"#AFCCFF\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#AFCCFF';\">";
						echo "<td align=\"center\"><strong>Total MEC + FIES + Cota Parte do Salário Educação</strong></td>";
						for($j=0; $j<count($total); $j++) {
							//echo "<td align=\"right\"><strong><font color=\"red\">".number_format($total[$j], 0, ',', '.')."</font></strong></td>";
							retornaSubTotal($total,$j,'red');
						}
						zeraGlobals();
						echo "</tr>";
					}
				}
			}
		?>
	</tbody>
</table>
<br />
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left"><strong><font style="font-size:11px;">Notas:</font></strong></td>
	</tr>
	<tr>
		<td align="left"><font style="font-size:11px;">1. Administração FIES - Limite de NCA no valor de R$54.122.000 recebido nos termos do Decreto 6.808/2009 e Portaria Insterministerial n° 64 de 30/03/09 - 74000  Operação Oficial de Crédito - 2009NL000978 de 31/03/09.</font></td>
	</tr>
</table>
<script language="JavaScript" src="../includes/overlib/overlib.js"></script>
