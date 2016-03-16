<?php

ob_clean();
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function testaMIparaConvencional( $proid ){
	
	global $db;
	
	$sql = "SELECT TRUE
			FROM par.processoobraspaccomposicao ppc
			INNER JOIN obras.preobra 				pre ON pre.preid = ppc.preid AND pre.ptoid IN (73,74)
			INNER JOIN workflow.documento 			doc ON doc.docid = pre.docid
			--INNER JOIN workflow.historicodocumento 	hst ON hst.hstid = doc.hstid AND aedid = 3601
			WHERE proid = $proid AND esdid = 228";
	
	return ( $db->pegaUm( $sql ) == 't' );
}

$_SESSION['par_var']['proid'] = $_REQUEST['proid'];

if (empty($_SESSION['par_var']['proid'])) {
    echo "<script>
			alert('Processo não encontrado.');
			window.close();
		</script>";
    exit();
}

if (!empty($_REQUEST['muncod'])) {
    $filtro = " muncod = '" . $_REQUEST['muncod'] . "'";
    $esfera = 'M';
} else {
    $estuf = $db->pegaUm("select estuf from par.processoobra where prostatus = 'A'  and proid = " . $_SESSION['par_var']['proid']);
    $filtro = " estuf = '" . $estuf . "'";
    $esfera = 'E';
}
if ($_REQUEST['acaoorigem'] != 'GERAR') {
    $sql = "SELECT terid, 'PAC2'||to_char(terid,'00000')||'/'||to_char(terdatainclusao,'YYYY') as termopai, ternumero, terano, tpdcod FROM par.termocompromissopac 
			WHERE $filtro AND terstatus = 'A' and tpdcod in (102, 103) AND proid = '" . $_SESSION['par_var']['proid'] . "'";
    $arrTermoPai = $db->pegaLinha($sql);
    $teridpai = $arrTermoPai['terid'];
    $termopai = $arrTermoPai['termopai'];
    $ternumero = $arrTermoPai['ternumero'];
    $terano = $arrTermoPai['terano'];
    $tpdcod = $arrTermoPai['tpdcod'];

    $sql = "UPDATE par.termocompromissopac SET terstatus = 'I' WHERE teridpai = $teridpai and titid = 2 AND proid = '" . $_SESSION['par_var']['proid'] . "'";
    $db->executar($sql);
}

if ($esfera == 'M') {
    $sql = "SELECT 
				upper(REPLACE(mun.mundescricao,'\'','')) as municipio,
				ent.estuf,
				ent.entnome as prefeitura,
				coalesce(ent.endlog,'#') || '/' || coalesce(ent.endbai,'#') as prefendereco,
				ent.entnumcpfcnpj as cnpjprefeitura,
				ent2.entnome as prefeito,
				ent2.entnumrg as rg,
				ent2.entnumcpfcnpj as cpf,
				est.estdescricao as estado
			FROM 
				par.entidade ent 
			INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = " . DUTID_PREFEITO . " AND ent2.entstatus = 'A'
			INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod
			INNER JOIN territorios.estado est ON est.estuf = ent.estuf
			WHERE
				ent.dutid = " . DUTID_PREFEITURA . " AND
				ent.entstatus = 'A' AND
				ent.muncod = '" . $_REQUEST['muncod'] . "'";

    $munDados = $db->carregar($sql);

    $sql = "SELECT
				e.entnumcpfcnpj as cpf,
				e.entnome as prefeito,
				e.entnumrg as rg
			FROM 
				par.entidade e 
			INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
			WHERE
				e.dutid = 7 AND
				e.entstatus = 'A' AND
				iu.muncod = '" . $_REQUEST['muncod'] . "'";

    $prefeito = $db->pegaLinha($sql);

    $munDados[0]['prefeito'] = $prefeito['prefeito'];
    $munDados[0]['cpf'] = $prefeito['cpf'];
    $munDados[0]['rg'] = $prefeito['rg'];
} else {
    $sql = "SELECT
				upper(REPLACE(est.estdescricao,'\'','')) as municipio,
				ent.estuf,
				ent.entnome as prefeitura,
				coalesce(ent.endlog,'#') || '/' || coalesce(ent.endbai,'#') as prefendereco,
				ent.entnumcpfcnpj as cnpjprefeitura,
				ent2.entnome as prefeito,
				ent2.entnumrg as rg,
				ent2.entnumcpfcnpj as cpf,
				est.estdescricao as estado
			FROM 
				par.entidade ent 
			INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = " . DUTID_SECRETARIO_ESTADUAL . " AND ent2.entstatus = 'A'
			INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod
			INNER JOIN territorios.estado est ON est.estuf = ent.estuf
			WHERE
				ent.dutid = " . DUTID_SECRETARIA_ESTADUAL . " AND
				ent.entstatus = 'A' AND
				ent.estuf = '" . $estuf . "'";

    $munDados = $db->carregar($sql);
}

if(
	$munDados[0]['prefeito'] == ''
	|| $munDados[0]['cpf'] == ''
	|| $munDados[0]['cpf'] == ''
	|| $munDados[0]['municipio'] == ''
	|| $munDados[0]['estuf'] == ''
	|| $munDados[0]['prefeitura'] == ''
	|| $munDados[0]['prefendereco'] == ''
	|| $munDados[0]['cnpjprefeitura'] == ''
	|| $munDados[0]['estado'] == ''
){
	echo "
		<script>
			alert('Favor atualizar os dados da unidade!');
		</script>";
	die();
}

if ($_REQUEST['acaoorigem'] != 'GERAR') {
    $d = "teridpai,";
    $d2 = "ternumero,";
    $w = $teridpai . ", ";
    $w2 = $ternumero . ", ";
    if ($_REQUEST['acaoorigem'] == 'REPROGRAMAR') {
        $tpdcod = 103;
    }
} else{
    $terano = date('Y');
    $tpdcod = 102;
}

$sql = "INSERT INTO par.termocompromissopac(
			muncod, 
			terdatainclusao, 
			usucpf,
			arqid,
			proid,
			estuf,
			titid,
			{$d}
			tpdcod,
                       {$d2}
                        terano,
                        teracaoorigem)
		VALUES (
			" . ($_REQUEST['muncod'] ? $_REQUEST['muncod'] : 'null') . ", 
			'" . date("Y-m-d G:i:s") . "', 
			'" . $_SESSION['usucpf'] . "',
			null,
			'" . $_SESSION['par_var']['proid'] . "',
			" . ($estuf ? "'" . $estuf . "'" : 'null') . ",
			2,
			{$w}
			{$tpdcod},
                       {$w2}
                        $terano,
                        '" . $_REQUEST['acaoorigem'] . "')
		RETURNING
			terid";
$termo = $db->pegaUm($sql);


if ($_REQUEST['acaoorigem'] == 'GERAR') {
    $sql = "UPDATE par.termocompromissopac SET ternumero='{$termo}' WHERE terid = {$termo} and proid = {$_SESSION['par_var']['proid']}";
    $db->executar($sql);
}
#Conta os itens em reporogramacao
// $sql = "select count(ter.terid) 
// 		from par.termocompromissopac ter 
// 		where ter.proid = {$_SESSION['par_var']['proid']} 
// 			AND teracaoorigem = 'REPROGRAMAR'
// 			and ter.tpdcod = 103";
$sql = "SELECT COUNT(ter.terid)
		FROM par.termocompromissopac ter
		WHERE ter.proid = {$_SESSION['par_var']['proid']} AND teridpai IS NOT NULL AND teracaoorigem NOT LIKE 'REGERAR' ";
      
$totTermo = $db->pegaUm($sql);

$sql = "SELECT DISTINCT TRUE FROM par.termocompromissopac WHERE terid = $termo AND teracaoorigem = 'REPROGRAMAR'";
$booReprogramacao = $db->pegaUm($sql);

$totTermo = ( ((int) $totTermo == 0 && $booReprogramacao == 't' ) ? 1 : (int) $totTermo);

if( $totTermo > 0 ){
    $termoExtenso = str_to_upper(numeroCardinalOrdinal($totTermo, false, 'ordinal', 'fem'));
}else{
    $termoExtenso = '';
}

$ano = $db->pegaUm("SELECT to_char(terdatainclusao,'YYYY') FROM par.termocompromissopac WHERE terid = {$termo}");

if ($_REQUEST['acaoorigem'] != 'GERAR') {
    $anoPai = $db->pegaUm("SELECT to_char(terdatainclusao,'YYYY') FROM par.termocompromissopac WHERE terid = {$teridpai}");
}

// Inativando Termo original (pai)
if ($termo && $_REQUEST['acaoorigem'] != 'GERAR') {

    $db->executar("UPDATE par.termocompromissopac SET terstatus = 'I' where terid = {$teridpai}");
    $db->commit();
}

#terid Original
$teridOriginal = $db->pegaUm("select par.retornanumerotermopac({$_SESSION['par_var']['proid']})");

if ($_REQUEST['acaoorigem'] == 'REPROGRAMAR') {
    $nometermo = $termoExtenso . ' REFORMULA&Ccedil;&Atilde;O AO TERMO DE COMPROMISSO ' . $teridOriginal;
} else {
	if( $_REQUEST['acaoorigem'] != 'GERAR' ){
	    $db->executar("UPDATE par.termocompromissopac SET ternumero = $ternumero where terid = {$termo}");
	    $db->commit();
	}
	$sql = "SELECT teridpai FROM par.termocompromissopac WHERE terid = $termo";
	$teridpai = $db->pegaUm($sql);
	if( $teridpai != '' && $termoExtenso != '' ){
	    $nometermo = $termoExtenso . ' REFORMULA&Ccedil;&Atilde;O AO TERMO DE COMPROMISSO ' . $teridOriginal;
	}else{
	    $teridpai = $termo;
	    $nometermo = 'TERMO DE COMPROMISSO<br /> ' . $teridOriginal;
	}
}

$preids = explode(',', $_REQUEST['arPreid']);
$tabela = "";
$tabelaMI = "";
$x = 1;
$y = 1;
$sqlInsert = "";
$testaPreid = array();

$arrMI = array(OBRA_TIPO_ESCOLA_PROINFANCIA_B_MI, OBRA_TIPO_ESCOLA_PROINFANCIA_C_MI, OBRA_TIPO_ESCOLA_PROINFANCIA_B_MI_EMENDA, OBRA_TIPO_ESCOLA_PROINFANCIA_C_MI_EMENDA);

foreach ($preids as $preid) {

    if (!in_array($preid, $testaPreid)) {
        $testaPreid[] = $preid;
        $sqlInsert .= "INSERT INTO par.termoobraspaccomposicao ( terid, preid ) VALUES ( " . $termo . ", " . $preid . " ); ";

        $sql = "SELECT 
					preid || ' - ' || predescricao as obra,
					prelogradouro as logradouro,
					precomplemento as complemento,
					ptodescricao as tipoobra,
					pto.ptoid,
					prevalorobra as vlrobra
				FROM 
					obras.preobra pre
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE
					preid = " . $preid;
        $dado = $db->pegaLinha($sql);
        $obra = $dado['obra'] != '' ? "" . wordwrap($dado['obra'], 50, '<br />') . "<br />" : "";
        $logradouro = $dado['logradouro'] != '' ? "" . wordwrap($dado['logradouro'], 50, '<br />') . "<br />" : "";
        $complemento = $dado['complemento'] != '' ? "" . wordwrap($dado['complemento'], 50, '<br />') . "<br />" : "";
        
        if ($tpdcod == '103') {
            if (in_array($dado['ptoid'], $arrMI)) {
                $tabelaMI .= "<div style=\"display: table;\">	
                       <div class=\"P_2\">{$x} )</div>
                       <div class=\"P_1\">" . $obra . "
                       " . $logradouro . "
                       " . $complemento . "
                       " . $dado['tipoobra'] . " R$ " . number_format($dado['vlrobra'], 2, ',', '.') . "</div>
                       </div><br />";
                       $x++;
            } else {
                $tabela .= "<div style=\"display: table;\">	
                       <div class=\"P_2\">{$y} )</div>
                       <div class=\"P_1\">" . $obra . "
                       " . $logradouro . "
                       " . $complemento . "
                       " . $dado['tipoobra'] . " R$ " . number_format($dado['vlrobra'], 2, ',', '.') . "</div>
                       </div><br />";
                       $y++;
            }
        }else{
             $tabela .= "<div style=\"display: table;\">	
                       <div class=\"P_2\">{$y} )</div>
                       <div class=\"P_1\">" . $obra . "
                       " . $logradouro . "
                       " . $complemento . "
                       " . $dado['tipoobra'] . " R$ " . number_format($dado['vlrobra'], 2, ',', '.') . "</div>
                       </div><br />";
                       $y++;
        }
    }
}
$db->executar($sqlInsert);

if ($_REQUEST['tipoobra'] == 'Q') {
    $tipoObra = "à Quadras";
    $tipoObra2 = "quadra(s) esportiva(s) escolar(es) coberta(s)";
} elseif ($_REQUEST['tipoobra'] == 'C') {
    $tipoObra = "à Coberturas";
    $tipoObra2 = "cobertura(s) de quadra(s) esportiva(s) escolar(es)";
} else {
    $tipoObra = "ao Pró-Infância";
    $tipoObra2 = "unidade(s) de educação infantil";
}
//echo "<pre>";
//var_dump($_REQUEST);exit;

if( in_array($_REQUEST['acaoorigem'], Array('REPROGRAMAR', 'REGERAR') ) && testaMIparaConvencional( $_SESSION['par_var']['proid'] ) ){
	
	$proid = $_SESSION['par_var']['proid'];
	
	$sql = "SELECT estuf FROM par.processoobra WHERE prostatus = 'A' and proid = $proid;";
	
	$dadoObraMI = $db->pegaUm( $sql );
	
	if ($dadoObraMI == 'DF') {
		$tipoEntidadeMI3 = 'NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
	} else {
		$tipoEntidadeMI3 = 'PREFEITO(A) MUNICIPAL DE <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
	}
	
	$html = '<html xmlns="http://www.w3.org/1999/xhtml">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=utf-8" />
				<title>Termo de Compomisso</title>
				<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
				<meta name="author" content="Neuza" />
				<meta name="created" content="2008-01-31T17:54:00" />
				<meta name="changedby" content="mec" />
				<meta name="changed" content="2008-02-07T18:09:00" />
				<style type="text/css">
					@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
					.divT_1{ margin-left: 2.5598in; text-align: center; }
					.P_1{ font-family: Arial; font-size: 12pt; margin-left: 45px; margin-right: 0in; text-align: left ! important; clear: both; }
					.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 30px; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
					.P_3{ font-family: Arial; font-size: 12pt; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
					.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
					.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
					.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
					.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
					.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
					.N{ font-weight: bold; }
					.I{ font-style: italic; }
					.S{ text-decoration: underline; }
					p.quebra    { page-break-before: always }
				</style>
				<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
			</head>
			<body>
				<div>
					<table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
						<tr>
	                        <td colspan="100">
		                        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
			                        <tr bgcolor="#ffffff">
				                        <td valign="top" align="center">
					                        <img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0">
					                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
					                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />
				                        </td>
			                        </tr>
		                        </table><br><br>
	                        </td>
                        </tr>
					</table>
					<p class="T_1">' . $nometermo . '</p>
				<p class="P_3">
					O Município de '.strtoupper($munDados[0]['municipio']).'/'.$munDados[0]['estuf'].', com sede na '.$munDados[0]['prefendereco'].', inscrito no CNPJ/MF sob o n° '.$munDados[0]['cnpjprefeitura'].', 
					representado (a) pelo (a) Prefeito (a) '.$munDados[0]['prefeito'].', brasileiro, portador da carteira de identidade n° '.$munDados[0]['rg'].'e do CPF n° '.$munDados[0]['cpf'].', 
					domiciliado no município em apreço, considerando o que dispõem a Lei nº 11.578, de 26 de novembro de 2007 e o Decreto nº 7.488/2011e as Resoluções/CD/FNDE nº 13/2012 e 41/2013, 
					se compromete a executar as ações relativas ao Termo de Compromisso supracitado, acrescido das seguintes condições:
				</p>
				<p class="P_3">
					<br />
					I - A(s) obra(s) abaixo relacionada(s) serão executadas no método convencional de construção, consoante às regras definidas na Resolução/CD/FNDE nº 13/2012:
				</p>
					'.$tabela.'
				<p class="P_3">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação, no âmbito do PAC 2 de acordo com os projetos fornecidos ou 
					aprovados (desenhos técnicos, memoriais descritivos e especificações), observando os critérios de qualidade técnica que atendam às determinações da Associação Brasileira de 
					Normas Técnicas (ABNT), bem como os prazos e os custos previstos;
				</p>
				<p class="P_3">
					<br />
					III - Os recursos para execução da(s) obra(s) serão transferidos em parcelas, de acordo com a execução de cada obra individualmente, podendo haver vários repasses para um mesmo
					Termo de Compromisso;
				</p>
				<p class="P_3">
					<br />
					IV - Utilizar os recursos financeiros transferidos pelo FNDE exclusivamente no cumprimento do objeto firmado neste Termo de Compromisso e dentro do prazo de execução definido 
					no art. 11 da Resolução CD/FNDE nº 13/2012, assim como responsabilizar-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste 
					Termo de Compromisso ou para aplicação financeira devendo a movimentação realizar-se, restritivamente, por meio eletrônico, no qual seja devidamente identificada a titularidade 
					das contas correntes de fornecedores ou prestadores de serviços, beneficiários dos pagamentos realizados pelo município, sendo proibida a utilização de cheques, conforme dispõe 
					o Decreto nº 7.507/2011;
				</p>
				<p class="P_3">
					<br />
					V - Indicar profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de fiscalização da(s) obra(s), com emissão da respectiva 
					Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_3">
					<br />
					VI - Responsabilizar-se, com recursos próprios, pela implementação de obras e serviços de terraplenagem e contenções, infraestrutura de redes (água potável, esgotamento sanitário, 
					energia elétrica e telefonia), assim como aqueles necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os valores a serem 
					repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s);
				</p>
				<p class="P_3">
					<br />
					VII - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população, no caso de os valores transferidos se revelarem 
					insuficientes para conclusão;
				</p>
				<p class="P_3">
					<br />
					VIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto, conforme o previsto, por meio do preenchimento dos dados e 
					informações sobre a(s) obra(s) no Módulo de Obras 2.0 do SIMEC (Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço 
					eletrônico http://simec.mec.gov.br;
				</p>
				<p class="P_3">
					<br />
					IX - Realizar licitação para as contratações necessárias à execução das obras, obedecendo à legislação vigente, às disposições do Decreto 7.983/2013, e observar 
					que os preços unitários de materiais e serviços utilizados não poderão ser superiores ao que consta dos custos de obras e serviços de engenharia na forma prevista 
					na Lei de Diretrizes Orçamentárias - LDO;
				</p>
				<p class="P_3">
					<br />
					X - Executar as obras no terreno ou na unidade escolar pactuadas no Termo de Compromisso, não sendo autorizada alteração do local que receberá as benfeitorias, 
					salvo em caso excepcional a ser avaliado e aprovado pelo FNDE;
				</p>
				<p class="P_3">
					<br />
					XI - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não, relacionada com a execução do 
					objeto pactuado, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal em placas de identificação da(s) obra(s) custeada(s) com os 
					recursos transferidos à conta do Programa, obedecendo ao disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo 
					e Gestão Estratégica da Presidência da República;
				</p>
				<p class="P_3">
					<br />
					XII - Submeter-se às orientações expedidas pelo Governo Federal acerca das condutas vedadas no período eleitoral;
				</p>
				<p class="P_3">
					<br />
					XIII - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso, para fins de fiscalização, 
					de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_3">
					<br />
					XIV - Permitir ao FNDE o acompanhamento da execução da(s) obra(s), fornecendo as informações e os documentos relacionados à execução do objeto, no que se refere ao exame da documentação;
				</p>
				<p class="P_3">
					<br />
					XV - Permitir o livre acesso aos órgãos de Controle e à Auditoria do FNDE, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente 
					com o objeto pactuado;
				</p>
				<p class="P_3">
					<br />
					XVI - Prestar esclarecimentos sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema de Controle 
					Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_3">
					<br />
					XVII - Enquanto não utilizados, os recursos transferidos serão obrigatoriamente aplicados em caderneta de poupança aberta especificamente para essa finalidade, quando a 
					previsão do seu uso for igual ou superior a um mês, e em fundo de aplicação financeira de curto prazo ou em operação de mercado aberto, lastreada em títulos da dívida pública 
					federal, se a sua utilização ocorrer em prazo inferior a um ano;
				</p>
				<p class="P_3">
					<br />
					XVIII - Os municípios, estados e o Distrito Federal deverão restituir ao FNDE os saldos financeiros remanescentes, inclusive os provenientes das receitas obtidas em aplicações 
					financeiras realizadas no prazo improrrogável de trinta dias, a contar do término do prazo estabelecido no artigo 11 da Resolução CD/FNDE nº 13/2012; 
				</p>
				<p class="P_3">
					<br />
					XIX - Prestar contas ao FNDE/MEC dos recursos recebidos, de acordo com o capítulo IV da Resolução CD/FNDE Nº 13/2012;
				</p>
				<p class="P_3">
					<br />
					XX - Lavrar o termo de aceitação definitiva da obra e registrá-lo no Módulo de Monitoramento de Obras no SIMEC;
				</p>
				<p class="P_3">
					<br />
					XXI - Emitir os documentos comprobatórios das despesas em nome do município, do estado ou do Distrito federal, com a identificação do FNDE/MEC e do Programa e arquivar as vias 
					originais em sua sede, ainda que utilize serviços de contabilidade de terceiros, juntamente com os documentos de prestação de contas referidos no Capítulo IV da Resolução CD/FNDE 
					Nº 13/2012 pelo prazo de vinte anos contados da data da aprovação da respectiva prestação de contas ou do julgamento da Tomada de Contas Especial pelo Tribunal de Contas da União (TCU), 
					quando for o caso;
				</p>
				<p class="P_3">
					<br />
					XXII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório de despesa efetuada 
					à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério desta Autarquia Federal;
				</p>
				<p class="P_3">
					<br />
					XXIII - Incluir no orçamento anual do Município os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no § 1º, do art. 6º, da Lei nº 4.320, 
					de 17 de março de 1964;
				</p>
				<p class="P_3">
					<br />
					XXIV - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao desenvolvimento do ensino, por 
					força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_3">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais demandas judiciais relativas a recursos humanos utilizados 
					na execução do objeto deste Termo de Compromisso, bem como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de 
					natureza compulsória, lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_3">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso, em atendimento, ainda, às disposições da Resolução CD/FNDE Nº 13/2012 e normativos 
					pertinentes à matéria.
				</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que o Município cumpre com as exigências do art. 169 da Constituição Federal que trata dos 
					limites de despesa com pessoal e que os recursos próprios de responsabilidade do Município estão assegurados, conforme a Lei Orgânica Municipal cuja cópia segue anexa.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de '.Date('Y').'.
				</p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
					<br />
					<br />
					<strong>' . $munDados[0]['prefeito'] . '</strong>
					<br />
					' . $tipoEntidadeMI3 . '
				</p>
			</div>
		</body>
	</html>';
	
}elseif (    ($_REQUEST['acaoorigem'] == 'GERAR') 
        || ($_REQUEST['acaoorigem'] == 'REGERAR' && ($_REQUEST['tipoobra'] == 'Q' || $_REQUEST['tipoobra'] == 'C')) 
        || ($_REQUEST['acaoorigem'] == 'REPROGRAMAR' && ($_REQUEST['tipoobra'] == 'Q' || $_REQUEST['tipoobra'] == 'C'))) {
    # Reformulação MI
    $proid = $db->pegaUm("select 
                            ppc.proid
                        from 
                            par.termocompromissopac tcp
                        INNER JOIN par.processoobraspaccomposicao ppc ON ppc.proid = tcp.proid and ppc.pocstatus = 'A'
                        INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
                        INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                        WHERE
                            tcp.terstatus = 'A' AND pto.ptocategoria IS NOT NULL AND ppc.proid = {$_SESSION['par_var']['proid']}");
    if ($proid) {
        $dadoObraMI = $db->pegaUm("SELECT estuf FROM par.processoobra WHERE prostatus = 'A' and proid = " . $_SESSION['par_var']['proid']);
        if ($dadoObraMI == 'DF') {
            $entidade = "<strong>SEDUC-DF</strong>";
            $tipoEntidadeMI = 'Distrital';
            $tipoEntidadeMI2 = 'Secretário';
            $tipoEntidadeMI3 = 'NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
        } else {
            $entidade = 'Prefeitura Municipal de <strong>' . $munDados[0]['municipio'] . '(' . $munDados[0]['estuf'] . ')</strong>';
            $tipoEntidadeMI = 'Municipal';
            $tipoEntidadeMI2 = 'Prefeito(a)';
            $tipoEntidadeMI3 = 'PREFEITO(A) MUNICIPAL DE <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
        }
        $html = '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 45px; margin-right: 0in; text-align: left ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 30px; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
			</style>
			<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
			<div>
                            <table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
                                <tr>
                                    <td colspan="100">
                                        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                            <tr bgcolor="#ffffff">		
                                                <td valign="top" align="center">
                                                    <img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0">			
                                                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>			
                                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />		
                                                </td>	
                                            </tr>
                                        </table><br><br>
                                    </td>
                                </tr>
                            </table>
				<p class="T_1">' . $nometermo . '</p>
				<p class="P_3">
					A ' . $entidade . ', com sede na <strong>' . $munDados[0]['prefendereco'] . '</strong>,	inscrita no CNPJ/MF sob o n° <strong>' . $munDados[0]['cnpjprefeitura'] . '</strong>, 
					representada pelo(a) ' . $tipoEntidadeMI2 . ' <strong>' . $munDados[0]['prefeito'] . '</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>' . $munDados[0]['rg'] . '</strong> e do 
					CPF n° <strong>' . $munDados[0]['cpf'] . '</strong>, residente e domiciliado(a) no estado de <strong>' . $munDados[0]['estado'] . '</strong>, considerando o que dispõe 
					a Lei n° 12.695, de 26 de novembro de 2007, compromete-se a executar as ações relativas a(s) unidade(s) de educação infantil, de acordo com as especificações 
					do(s) projeto(s) aprovado(s) pelo Fundo Nacional de Desenvolvimento da Educação - FNDE e em conformidade com os requisitos da lei supramencionada e demais 
					condicionantes, a seguir descritas:
				</p>
				<p class="P_3">
					<br />
					I - Executar todas as atividades inerentes à construção de ' . count($preids) . ' (' . strtolower(extenso(count($preids), true, false)) . ') ' . $tipoObra2 . ', situada(s) em:
				</p>
					' . $tabela . '
				<p class="P_3">
					<br />
					II - executar os recursos financeiros recebidos do FNDE/MEC, de acordo com os projetos executivos fornecidos, observando 
					os critérios de qualidade técnica, os prazos e os custos previstos no contrato firmado com a empresa responsável pela execução, 
					conforme Registro de Preços Nacional realizado pelo FNDE;
				</p>
				<p class="P_3">
					<br />
					III - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE, de forma a garantir a aplicação 
					tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_3">
					<br />
					IV - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança, 
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de 
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização 
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente 
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança, cuja aplicação 
					poderá se dar mediante vinculação do correspondente número de operação à conta já existente;
				</p>
				<p class="P_3">
					<br />
					V - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas
					mesmas condições de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente 
					computados a crédito da conta corrente específica; 
				</p>
				<p class="P_3">
					<br />
					VI - indicar profissional devidamente habilitado, da área de Engenharia Civil ou Arquitetura, para exercer as funções de fiscalização 
					da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_3">
					<br />
					VII - promover o acompanhamento e a fiscalização da sondagem e elaboração do Projeto Executivo de Implantação, assim como da construção 
					da escola, sob o aspecto quantitativo e qualitativo e de acordo com os termos do Edital e seus anexos, registrando imediatamente todos 
					os passos no SIMEC;
				</p>
				<p class="P_3">
					<br />
					VIII - comunicar prontamente à CONTRATADA qualquer anormalidade no objeto do(s) instrumento(s) de contrato, podendo recusar o seu 
					recebimento caso não esteja(m) de acordo com as especificações e condições estabelecidas no Edital e seus anexos;
				</p>
				<p class="P_3">
					<br />
					IX - responsabilizar-se, com recursos próprios, pela execução dos serviços de terraplenagem e contenções, infraestrutura de redes 
					(água potável, energia elétrica, e esgotamento sanitário, quando couber);
				</p>
				<p class="P_3">
					<br />
					X - cientificar o FNDE sobre a aplicação dos recursos e a consecução do objeto conforme o previsto, por meio do preenchimento dos 
					dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC (Sistema Integrado de Monitoramento, Execução e 
					Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>;
				</p>
				<p class="P_3">
					<br />
					XI - assegurar e destacar obrigatoriamente a participação do Governo Federal, Ministério da Educação e do FNDE em toda e qualquer ação, 
					promocional ou não, relacionada com a execução do objeto pactuado, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do 
					Governo Federal em placas, de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa, obedecendo ao 
					que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e Gestão Estratégica 
					da Presidência da República;
				</p>
				<p class="P_3">
					<br />
					XII - manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução do Termo de Compromisso pactuado, para 
					fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_3">
					<br />
					XIII - prestar esclarecimentos sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por 
					órgão do Sistema de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão 
					ou entidade com delegação para esse fim;
				</p>
				<p class="P_3">
					<br />
					XIV - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle - SFC/MF, 
					Delegacia Federal de Controle - DFC ou sua representação no Estado, Secretaria de Controle Interno - CISET) e da Auditoria do FNDE, a qualquer 
					tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o objeto pactuado neste Termo 
					de Compromisso , bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de informações junto à comunidade local sobre 
					os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria; 
				</p>
				<p class="P_3">
					<br />
					XV - conferir toda a documentação técnica gerada e apresentada durante a execução dos serviços, efetuando o atesto quando a mesma estiver em 
					conformidade com os padrões de informação e qualidade exigidos;
				</p>
				<p class="P_3">
					<br />
					XVI - lavrar o termo de aceitação definitiva da(s) obra(s) e registrá-lo no Módulo de Monitoramento de Obras no SIMEC;
				</p>
				<p class="P_3">
					<br />
					XVII - prestar contas ao FNDE/MEC dos recursos recebidos, de acordo com capítulo IV da Resolução Nº 25, DE 14 DE JUNHO DE 2013;
				</p>
				<p class="P_3">
					<br />
					XVIII - Emitir os documentos comprobatórios das despesas em nome do município ou do Distrito Federal, com a identificação do FNDE/MEC e do 
					Programa e arquivar as vias originais em sua sede, ainda que utilize serviços de contabilidade de terceiros, juntamente com os documentos de 
					prestação de contas referidos no Capítulo IV, pelo prazo de vinte anos contados da data da aprovação da respectiva prestação de contas ou do 
					julgamento da Tomada de Contas Especial pelo Tribunal de Contas da União (TCU), quando for o caso; 
				</p>
				<p class="P_3">
					<br />
					XIX - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais demandas judiciais relativas 
					a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem como por todos os ônus tributários ou extraordinários que 
					incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória, lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_3">
					<br />
					XX - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e 
					ao desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_3">
					<br />
					XXI - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento 
					comprobatório de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal;
				</p>
				<p class="P_3">
					<br />
					XXII - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos 
					estabelecidos no § 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964; 
				</p>
				<p class="P_3">
					<br />
					XXIII - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso;
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que a ' . $entidade . ' cumpre com as 
					exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e, que os recursos próprios de responsabilidade da 
					(Prefeitura Municipal/Secretaria de Educação) estão assegurados, conforme a Lei Orgânica ' . $tipoEntidadeMI . '.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de '.Date('Y').'.
				</p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
					<br />
					<br />
					<strong>' . $munDados[0]['prefeito'] . '</strong>
					<br />
					' . $tipoEntidadeMI3 . '
				</p>
			</div>
		</body>
	</html>';
    } else {
        if ($esfera == 'M') {


            $html = '<html xmlns="http://www.w3.org/1999/xhtml">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=utf-8" />
				<title>Termo de Compomisso</title>
				<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
				<meta name="author" content="Neuza" />
				<meta name="created" content="2008-01-31T17:54:00" />
				<meta name="changedby" content="mec" />
				<meta name="changed" content="2008-02-07T18:09:00" />
				<style type="text/css">
					@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
					.divT_1{ margin-left: 2.5598in; text-align: center; }
					.P_1{ font-family: Arial; font-size: 12pt; margin-left: 45px; margin-right: 0in; text-align: left ! important; clear: both; }
                                        .P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 30px; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
                                        .P_3{ font-family: Arial; font-size: 12pt; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
                                        .P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
                                        .Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
                                        .T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
                                        .T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
					.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
					.N{ font-weight: bold; }
					.I{ font-style: italic; }
					.S{ text-decoration: underline; }
					p.quebra    { page-break-before: always }
				</style>
				<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
			</head>
			<body>
				<div>
                                        <table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
                                            <tr>
                                                <td colspan="100">
                                                    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                                        <tr bgcolor="#ffffff">		
                                                            <td valign="top" align="center">
                                                                <img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0">			
                                                                    <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>			
                                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />		
                                                            </td>	
                                                        </tr>
                                                    </table><br><br>
                                                </td>
                                            </tr>
                                        </table>
					<p class="T_1">' . $nometermo . '</p>
					<p class="P_3">
						A Prefeitura Municipal de <strong>' . $munDados[0]['municipio'] . '(' . $munDados[0]['estuf'] . ')</strong>, com sede na <strong>' . $munDados[0]['prefendereco'] . '</strong>,	inscrita no CNPJ/MF sob o n° <strong>' . $munDados[0]['cnpjprefeitura'] . '</strong>, 
						representada pelo(a) prefeito(a) <strong>' . $munDados[0]['prefeito'] . '</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>' . $munDados[0]['rg'] . '</strong> e do 
						CPF n° <strong>' . $munDados[0]['cpf'] . '</strong>, residente e domiciliado(a) no estado de <strong>' . $munDados[0]['estado'] . '</strong>, considerando o que dispõe a Lei n° 11.578, 
						de 26 de novembro de 2007, compromete-se a executar as ações relativas ' . $tipoObra . ', 
						no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de 
						Desenvolvimento da Educação - FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
					</p>
					<p class="P_3">
						<br />
						I - Executar todas as atividades inerentes à construção de ' . count($preids) . ' (' . strtolower(extenso(count($preids), true, false)) . ') ' . $tipoObra2 . ', situada(s) em:
					</p>
						' . $tabela . '
					<p class="P_3">
						<br />
						II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2 
						em estrito acordo com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações), 
						observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT), 
						bem como os prazos e os custos previstos;
					</p>
					<p class="P_3">
						<br />
						III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado; 
						responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste 
						Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque 
						nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque 
						autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
					</p>
					<p class="P_3">
						<br />
						IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de 
						fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
					</p>
					<p class="P_3">
						<br />
						V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de 
						redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços 
						necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os 
						valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas 
						orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s); 
					</p>
					<p class="P_3">
						<br />
						VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população, 
						no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
					</p>
					<p class="P_3">
						<br />
						VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s) 
						obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC, 
						a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE, 
						disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
					</p>
					<p class="P_3">
						<br />
						VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos 
						necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário, 
						obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar 
						seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este 
						Termo de Compromisso constitui anexo;
					</p>
					<p class="P_3">
						<br />
						IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE, 
						de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
					</p>
					<p class="P_3">
						<br />
						X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança, 
						aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de 
						aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização 
						ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente 
						na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança, 
						cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
					</p>
					<p class="P_3">
						<br />
						XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições 
						de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
					</p>
					<p class="P_3">
						<br />
						XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de 
						junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes 
						do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil - SINAPI, mantido pela Caixa Econômica Federal. Em condições 
						especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais 
						(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos, 
						sem prejuízo da avaliação dos órgãos de controle internos e externos;
					</p>
					<p class="P_3">
						<br />
						XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto, 
						por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC 
						(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>, 
						utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
					</p>
					<p class="P_3">
						<br />
						XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não, 
						relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal 
						em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa, 
						obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e 
						Gestão Estratégica da Presidência da República; 
					</p>
					<p class="P_3">
						<br />
						XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso, 
						para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
					</p>
					<p class="P_3">
						<br />
						XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os 
						documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
					</p>
					<p class="P_3">
						<br />
						XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle 
						- SFC/MF, Delegacia Federal de Controle - DFC ou sua representação no Estado, Secretaria de Controle Interno - CISET) e da Auditoria do 
						FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o 
						objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de 
						informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
					</p>
					<p class="P_3">
						<br />
						XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório 
						de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal; 
					</p>
					<p class="P_3">
						<br />
						XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema 
						de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
					</p>
					<p class="P_3">
						<br />
						XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no 
						§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
					</p>
					<p class="P_3">
						<br />
						XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao 
						desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
					</p>
					<p class="P_3">
						<br />
						XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s) 
						à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso; 
					</p>
					<p class="P_3">
						<br />
						XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30 
						da Resolução CD/FNDE Nº 13/2011;
					</p>
					<p class="P_3">
						<br />
						XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
						Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados 
						da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos, 
						a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
					</p>
					<p class="P_3">
						<br />
						XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais 
						demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem 
						como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória, 
						lançados automaticamente pela rede bancária arrecadadora; 
					</p>
					<p class="P_3">
						<br />
						XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
					</p>
					<p class="quebra">&nbsp;</p>
					<p class="P_3">
						<br />
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que o município 
						cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e, 
						que os recursos próprios de responsabilidade do Município estão assegurados, conforme a 
						Lei Orgânica Municipal.
					</p>
					<p class="Data">
						<br />
						Brasília/DF, ___ de ____________________ de '.Date('Y').'.
					</p>
					<p class="T_1">
						<br />
						<br />
						<br />
						____________________________________________
						<br />
						<br />
						<strong>' . $munDados[0]['prefeito'] . '</strong>
						<br />
						PREFEITO(A) MUNICIPAL DE <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>
					</p>
				</div>
			</body>
		</html>';
        } else {
            $html = '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 45px; margin-right: 0in; text-align: left ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 30px; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
			</style>
			<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
			<div>
                                <table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
                                    <tr>
                                        <td colspan="100">
                                            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                                <tr bgcolor="#ffffff">		
                                                    <td valign="top" align="center">
                                                        <img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0">			
                                                            <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>			
                                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />		
                                                    </td>	
                                                </tr>
                                            </table><br><br>
                                        </td>
                                    </tr>
                                </table>
				<p class="T_1">' . $nometermo . '</p>
				<p class="P_3">				
					A Secretaria de Educação do Estado de <strong>' . $munDados[0]['municipio'] . '</strong>, com sede na <strong>' . $munDados[0]['prefendereco'] . '</strong>, inscrita no CNPJ/MF sob o n° <strong>' . $munDados[0]['cnpjprefeitura'] . '</strong>, 
					representada pelo(a) Secretário(a) <strong>' . $munDados[0]['prefeito'] . '</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>' . $munDados[0]['rg'] . '</strong> e do 
					CPF n° <strong>' . $munDados[0]['cpf'] . '</strong>, residente e domiciliado(a) no estado de <strong>' . $munDados[0]['estado'] . '</strong>, considerando o que dispõe a Lei n° 11.578, 
					de 26 de novembro de 2007, compromete-se a executar as ações relativas ' . $tipoObra2 . ', 
					no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de 
					Desenvolvimento da Educação - FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
				</p>
				<p class="P_3">
					<br />
					I - Executar todas as atividades inerentes à construção de ' . count($preids) . ' (' . strtolower(extenso(count($preids), true, false)) . ') ' . $tipoObra2 . ', situada(s) em:
				</p>
					' . $tabela . '
				<p class="P_3">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2 em estrito acordo 
					com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações), 
					observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT), 
					bem como os prazos e os custos previstos;
				</p>
				<p class="P_3">
					<br />
					III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado; 
					responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste 
					Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque 
					nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque 
					autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
				</p>
				<p class="P_3">
					<br />
					IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de 
					fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_3">
					<br />
					V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de 
					redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços 
					necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os 
					valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas 
					orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s); 
				</p>
				<p class="P_3">
					<br />
					VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população, 
					no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
				</p>
				<p class="P_3">
					<br />
					VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s) 
					obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC, 
					a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE, 
					disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
				</p>
				<p class="P_3">
					<br />
					VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos 
					necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário, 
					obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar 
					seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este 
					Termo de Compromisso constitui anexo;
				</p>
				<p class="P_3">
					<br />
					IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE, 
					de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_3">
					<br />
					X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança, 
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de 
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização 
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente 
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança, 
					cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
				</p>
				<p class="P_3">
					<br />
					XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições 
					de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
				</p>
				<p class="P_3">
					<br />
					XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de 
					junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes 
					do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil - SINAPI, mantido pela Caixa Econômica Federal. Em condições 
					especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais 
					(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos, 
					sem prejuízo da avaliação dos órgãos de controle internos e externos;
				</p>
				<p class="P_3">
					<br />
					XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto, 
					por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC 
					(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>, 
					utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
				</p>
				<p class="P_3">
					<br />
					XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não, 
					relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal 
					em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa, 
					obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e 
					Gestão Estratégica da Presidência da República; 
				</p>
				<p class="P_3">
					<br />
					XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso, 
					para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_3">
					<br />
					XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os 
					documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
				</p>
				<p class="P_3">
					<br />
					XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle 
					- SFC/MF, Delegacia Federal de Controle - DFC ou sua representação no Estado, Secretaria de Controle Interno - CISET) e da Auditoria do 
					FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o 
					objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de 
					informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
				</p>
				<p class="P_3">
					<br />
					XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório 
					de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal; 
				</p>
				<p class="P_3">
					<br />
					XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema 
					de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_3">
					<br />
					XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no 
					§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
				</p>
				<p class="P_3">
					<br />
					XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao 
					desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_3">
					<br />
					XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s) 
					à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso; 
				</p>
				<p class="P_3">
					<br />
					XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30 
					da Resolução CD/FNDE Nº 13/2011;
				</p>
				<p class="P_3">
					<br />
					XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
					Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados 
					da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos, 
					a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
				</p>
				<p class="P_3">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais 
					demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem 
					como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória, 
					lançados automaticamente pela rede bancária arrecadadora; 
				</p>
				<p class="P_3">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que a Secretaria de Educação  
					cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e, 
					que os recursos próprios de responsabilidade do Secretaria estão assegurados, conforme a Lei Orgânica Municipal.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de '.Date('Y').'.
				</p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
					<br />
					<br />
					<strong>' . $munDados[0]['prefeito'] . '</strong>
					<br />
					NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>
				</p>
			</div>
		</body>
	</html>';
        }
    }
} else {
    $ObraTipos = $db->pegaLinha("select(
						select count(pre.preid) as obrasmi from par.processoobra p
						    inner join par.processoobraspaccomposicao pop on pop.proid = p.proid and pop.pocstatus = 'A'
						inner join obras.preobra pre on pre.preid = pop.preid 
						where p.prostatus = 'A'  and pre.ptoid in (select ptoid from obras.pretipoobra p where p.ptodescricao ilike '%Metodologias Inovadoras%')
						    and p.proid = {$_SESSION['par_var']['proid']}) as obrasmi,
						(
						select count(pre.preid) from par.processoobra p
						    inner join par.processoobraspaccomposicao pop on pop.proid = p.proid and pop.pocstatus = 'A'
						inner join obras.preobra pre on pre.preid = pop.preid 
						where pre.ptoid not in (select ptoid from obras.pretipoobra p where p.ptodescricao ilike '%Metodologias Inovadoras%')
						    and p.prostatus = 'A'  and p.proid = {$_SESSION['par_var']['proid']}
						) as obrasnormal");

    $dadoObraMI = $db->pegaUm("SELECT estuf FROM par.processoobra WHERE prostatus = 'A'  and proid = " . $_SESSION['par_var']['proid']);
    if ($dadoObraMI == 'DF') {
        $entidade = "<strong>SEDUC-DF</strong>";
        $tipoEntidadeMI = 'Distrital';
        $tipoEntidadeMI2 = 'Secretário';
        $tipoEntidadeMI3 = 'NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
        $ano = $anoPai ? $anoPai : $ano;
    } else {
        if ($esfera == 'M') {
            $entidade = 'Prefeitura Municipal de <strong>' . $munDados[0]['municipio'] . '/' . $munDados[0]['estuf'] . '</strong>';
            $tipoEntidadeMI = 'Municipal';
            $tipoEntidadeMI2 = 'Prefeito(a)';
            $tipoEntidadeMI3 = 'PREFEITO(A) MUNICIPAL DE <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
        } else {
            $entidade = '<strong>SEDUC-' . $munDados[0]['estuf'] . '</strong>';
            $tipoEntidadeMI = 'Estadual';
            $tipoEntidadeMI2 = 'Secretário(a)';
            $tipoEntidadeMI3 = 'SECRET&Aacute;RIO(A) ESTADUAL DE <strong>' . strtoupper($munDados[0]['municipio']) . '/' . $munDados[0]['estuf'] . '</strong>';
        }
        $ano = $anoPai ? $anoPai : $ano;
    }
    $count = 1;
    $html = '
        <table width="95%" cellspacing="1" cellpadding="5" border="0" align="center" >
            <tr>
                <td colspan="100">
                    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                        <tr bgcolor="#ffffff">		
                            <td valign="top" align="center">
                                <img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0">			
                                    <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>			
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />		
                            </td>	
                        </tr>
                    </table><br><br>
                </td>
            </tr>
        </table>
<p style="text-align: center;"><strong><span style="font-size: medium;">' . $nometermo . '</span></strong></p>
<p style="text-align: center;"><span style="font-size: medium;"><br /></span></p>

<div style="font-weight:normal;vertical-align:top; FONT: 10pt Arial,verdana">
<p style="text-align: justify;">A ' . $entidade . ', com sede na <strong>' . $munDados[0]['prefendereco'] . '</strong>, inscrito no CNPJ/MF sob o n&deg; <strong>' . formatar_cpf_cnpj($munDados[0]['cnpjprefeitura']) . '</strong>, representado (a) pelo (a) ' . utf8_decode($tipoEntidadeMI2) . ' <strong>' . utf8_decode($munDados[0]['prefeito']) . '</strong>, brasileiro, portador da carteira de identidade n&deg; <strong>' . $munDados[0]['rg'] . '</strong> e do CPF n&deg; <strong>' . formatar_cpf_cnpj($munDados[0]['cpf']) . '</strong>, domiciliado no munic&iacute;pio em apre&ccedil;o, considerando o que disp&otilde;em as Leis n&ordm; 12.462/2011 e 12.695/2012, &nbsp;&nbsp;Decreto n&ordm; 7.488/2011; 7581/2011 e as Resolu&ccedil;&otilde;es/CD/FNDE n&ordm; 13/2012 e 25/2013, &nbsp;se compromete a executar as a&ccedil;&otilde;es relativas ao Termo de Compromisso supracitado, acrescido das seguintes condi&ccedil;&otilde;es:</p>';
    if ($ObraTipos['obrasmi'] > 0) {
        $html .='<p style="text-align: justify;">' . decimal2romano($count++) . ' - As obras abaixo relacionadas ser&atilde;o executadas por interm&eacute;dio de metodologias inovadoras de constru&ccedil;&atilde;o, consoante as regras definidas na Resolu&ccedil;&atilde;o/CD/FNDE n&ordm; 25/2013, mediante participa&ccedil;&atilde;o do Convenente na Ata de Registro de Pre&ccedil;os gerenciada pelo Concedente, resultante do Regime Diferenciado de Contrata&ccedil;&otilde;es P&uacute;blicas &ndash; RDC, com utiliza&ccedil;&atilde;o de Preg&atilde;o Eletr&ocirc;nico e Sistema de Registro de Pre&ccedil;os &ndash; SR, aplic&aacute;vel &agrave; &aacute;rea geogr&aacute;fica em que se localiza o Convenente:</p>
<ol><div>
' . $tabelaMI . '
</div></ol>
<p style="text-align: justify;">' . decimal2romano($count++) . ' &ndash; Responsabilizar-se, com recursos pr&oacute;prios, pela execu&ccedil;&atilde;o dos servi&ccedil;os de terraplanagem, conten&ccedil;&otilde;es e infraestrutura de redes (&aacute;gua pot&aacute;vel, energia el&eacute;trica e esgotamento sanit&aacute;rio, quando couber), apresentando terreno adequado para in&iacute;cio das constru&ccedil;&otilde;es, dentro do prazo estabelecido no art. 11 da Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 25, de 14 de junho de 2013, respeitada a data de validade da Ata de Registro de Pre&ccedil;os &ndash; aplic&aacute;vel ao ente federado &ndash; para a constru&ccedil;&atilde;o de unidades de educa&ccedil;&atilde;o infantil do ProInf&acirc;ncia, com utiliza&ccedil;&atilde;o de metodologias inovadoras, resultante das licita&ccedil;&otilde;es sob o Regime Diferenciado de Contrata&ccedil;&otilde;es P&uacute;blicas - RDC realizadas pelo FNDE;</p>';
    }
    if ($ObraTipos['obrasnormal'] > 0) {
        $html .='<p style="text-align: justify;">' . decimal2romano($count++) . ' &ndash; As obras abaixo dever&atilde;o ser executadas por meio de metodologia convencional, permanecendo regidas pelas cl&aacute;usulas estabelecidas no Termo de Compromisso ' . $termopai . ';</p>
<ol><div>
' . $tabela . '
</div></ol>';
    }
    $html .='<p>&nbsp;</p>
<p style="text-align: justify;">' . decimal2romano($count++) . ' &ndash; Ficam ratificadas as demais condi&ccedil;&otilde;es expressas nas cl&aacute;usulas do instrumento ora reformulado.</p>
<p>&nbsp;</p>
<p style="text-align: center;">Bras&iacute;lia/DF, __ de __________ de '.Date('Y').'.</p>
<p style="text-align: center;">&nbsp;</p>
<p style="text-align: center;">&nbsp;</p>
<p style="text-align: center;">&nbsp;</p>
<p style="text-align: center;">____________________________</p>
<p style="text-align: center;"><strong>' . utf8_decode($munDados[0]['prefeito']) . '</strong></p>
<p style="text-align: center;">' . $tipoEntidadeMI3 . '</strong></p>
		</div>
		';
}

$html = str_replace("'", '', $html);

$pdf = pdf(utf8_encode($html));

if ($pdf && $termo) {

    $descricaoArquivo = $teridOriginal;

    $campos = array("terdatainclusao" => "'" . date("Y-m-d G:i:s") . "'", "usucpf" => "'" . $_SESSION['usucpf'] . "'");

    $file = new FilesSimec("termocompromissopac", $campos, 'par');

    $file->arquivo['name'] = $descricaoArquivo . ".pdf";
    $arquivoSalvo = $file->setStream($descricaoArquivo, $pdf, "text/pdf", ".pdf", false);
    
    gravaHtmlDocumento($html, $termo, $_REQUEST['proid'], 'PAC');

    $sql = "UPDATE par.termocompromissopac SET arqid = " . $arquivoSalvo . " WHERE terid = " . $termo;
    $terid = $db->executar($sql);

    if (is_array($preids)) {
        $sql = "";
        foreach ($preids as $preid) {
            $sql .= "INSERT INTO par.termoobra(
						preid, 
						terid)
					 VALUES (
						{$preid}, 
						{$termo});";
        }
        $db->executar($sql);
    }

    $db->commit();

    if ($arquivoSalvo) {
        pdf(utf8_encode($html), 1);
    }
}
?>