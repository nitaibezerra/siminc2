<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
$(function(){
	$('.gerarPDF').click(function(){
		//$('#requisicao').val('gerarPDF');
		//$('#formulario').submit();
        location.href=location.href+'&pdfAction=true';
	});
});
</script>
<?php

verificaCodigoTermoSessao();

if (!isset($_GET['pdfAction'])) {
	require_once APPRAIZ . 'includes/cabecalho.inc';

    echo "<br>";
    $db->cria_aba ( $abacod_tela, $url, '' );
    monta_titulo(MODULO_NAME, '');
    montaCabecalhoUG(Array(
        'termo' => true
    ));
    echo "<br>";
    montaAbaTermoCooperacao();
}

$sqlCountSolAteracao = " select count(*) from workflow.historicodocumento where aedid = " . WF_ACAO_SOL_ALTERACAO . " and docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}) ";
$rsCountSolAlt = $db->pegaUm ( $sqlCountSolAteracao );

$dadosUG = buscarDadosUndProp ();
$dadosObjeto = buscarObjetoTermo ();
$html = '';

if (!isset($_GET['pdfAction'])) {
$html.= '
	<form method="post" name="formulario" id="formulario">
		<input type="hidden" id="tcpid" name="tcpid" value="' . $_SESSION ['elabrev'] ['tcpid'] . '" />
		<input type="hidden" id="req" name="req" value="" />
		<input type="hidden" id="requisicao" name="requisicao" value="" />
		<input type="hidden" id="aba" name="aba" value="' . $_REQUEST ['aba'] . '" />
		<input type="hidden" id="acaoAba" name="acaoAba" value="' . $_REQUEST ['aba'] . '" />

		<input type="hidden" id="abaAtual" name="abaAtual" value="' . $_REQUEST ['aba'] . '" />

		<!-- DADOS DO ÕRGÃO OU ENTIDADE PROPONENTE -->
		<!--
		<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
			<tr id="tr_titulo">
				<td class="subtitulocentro" >Termo de Execução Descentralizada - Gerar PDF</td>
			</tr>
		</table>
		-->
		';
}

$html.= '
				<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
					<tr id="tr_titulo">
						<td class="subtituloDireita" width="100">Nº do Termo:</td>
						<td>&nbsp;' . ($rsCountSolAlt > 0 ? $_SESSION ['elabrev'] ['tcpid'] . '.' . $rsCountSolAlt : $_SESSION ['elabrev'] ['tcpid']) . '</td>
			';

if ($dadosObjeto ['ungcodconcedente'] == UG_FNDE && in_array ( $dadosObjeto ['ungcodpoliticafnde'], array (
		UG_SECADI,
		UG_SETEC,
		UG_SEB
) )) {
	$html .= '
						<td class="subtituloDireita" width="100">Nº do Processo:</td>
						<td width="200">&nbsp;' . $dadosObjeto ['tcpnumprocessofnde'] . '</td>
					';
}

$html .= '
					</tr>
				</table>
				';

$html .= '
		<table id="dados_entidade_prop" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="30" colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO ÕRGÃO OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;" class="jsCounter">%d. Cód. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Cód. da Gestão</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CNPJ</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Razão Social</td>
		  </tr>
		  <tr>
		    <td>' . $dadosUG ['ungcod'] . '</td>
		    <td colspan="2">' . $dadosUG ['gescod'] . '</td>
		    <td colspan="2">' . formatar_cnpj($dadosUG['ungcnpj']) . '</td>
		    <td colspan="2">' . $dadosUG ['ungdsc'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;" class="jsCounter">%d. Endereço</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Município</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosUG ['ungendereco'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungbairro'] . '</td>
		    <td colspan="2">' . $dadosUG ['muncod'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosUG ['estuf'] . '</td>
		    <td>' . $dadosUG ['ungcep'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungfone'] . '</td>
		    <td colspan="2">' . $dadosUG ['ungemail'] . '</td>
		  </tr>
		</table>';
?>
<!-- REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE PROPONENTE -->
<?php
$strSQL = "
    select
        u.usunome, wh.usucpf
    from workflow.historicodocumento wh
    inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
    where
        wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        and wh.aedid = 1595
";

$responsavel = $db->pegaLinha($strSQL);

$dadosProp = recuperarResponsavelProponente();
$html .= '
		<table id="dados_proponente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>' . formatar_cpf($dadosProp['usucpf']). '</td>
		    <td colspan="5">' . $dadosProp['usunome'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Endereço</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Município</td>
		  </tr>
		  <tr>
		    <td colspan="3">' . $dadosProp['endereco'] . '</td>
		    <td>' . $dadosProp['bairro'] . '</td>
		    <td colspan="2">' . $dadosProp['municipio'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosProp ['estado'] . '</td>
		    <td colspan="2">' . $dadosProp ['endcep'] . '</td>
		    <td>' . $dadosProp ['fone'] . '</td>
		    <td colspan="2">' . $dadosProp ['usuemail'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. N&ordm; da Cédula da CI</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Õrgão Expeditor</td>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Cargo</td>
		  </tr>
		  <tr>
		    <td>' . $dadosProp ['numeroidentidade'] . '</td>
		    <td colspan="2">' . $dadosProp ['entorgaoexpedidor'] . '</td>
		    <td colspan="3">' . $dadosProp ['usufuncao'] . '</td>
		  </tr>';
    if ($responsavel) {
        $html .= '<tr>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Área Técnica Responsável</td>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		  </tr>
		  <tr>
		    <td colspan="5">' . $responsavel ['usunome'] . '</td>
		    <td>' . formatar_cpf ( $responsavel ['usucpf'] ) . '</td>
		  </tr>';
    }
$html .= '</table>';
?>
<!-- DADOS DO ÕRGÃO OU ENTIDADE CONCEDENTE -->
<?php
$dadoConc = buscarDadosUndConc ();

$html .= '
		<table id="dados_entidade_conc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="30" colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO ÕRGÃO OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;" class="jsCounter">%d. Cód. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Cód. da Gestão</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CNPJ</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Razão Social</td>
		  </tr>
		  <tr>
		    <td>' . $dadoConc ['ungcod'] . '</td>
		    <td colspan="2">' . $dadoConc ['gescod'] . '</td>
		    <td colspan="2">' . formatar_cnpj($dadoConc['ungcnpj']) . '</td>
		    <td colspan="2">' . $dadoConc ['ungdsc'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;" class="jsCounter">%d. Endereço</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Município</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadoConc ['ungendereco'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungbairro'] . '</td>
		    <td colspan="2">' . $dadoConc ['muncod'] . '</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadoConc ['estuf'] . '</td>
		    <td>' . $dadoConc ['ungcep'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungfone'] . '</td>
		    <td colspan="2">' . $dadoConc ['ungemail'] . '</td>
		  </tr>
	</table>';
?>
<!-- REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE CONCEDENTE -->
<?php
$strSQL = "
    select
        u.usunome, wh.usucpf
    from workflow.historicodocumento wh
    inner join seguranca.usuario u on (u.usucpf = wh.usucpf)
    where
        wh.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        and wh.aedid in (1605, 1610)
    order by wh.hstid desc limit 1
";
$responsavel = $db->pegaLinha($strSQL);
$dadosRespConced = recuperarResponsavelConcedente ();

$html .= '
		<table id="dados_concedente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>' . formatar_cpf ( $dadosRespConced ['usucpf'] ) . '</td>
		    <td colspan="5">' . $dadosRespConced ['usunome'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Endereço</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Município</td>
		  </tr>
		  <tr>
		    <td colspan="3">' . $dadosRespConced ['endereco'] . '</td>
		    <td>' . $dadosRespConced ['bairro'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['municipio'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. UF</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. CEP</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Telefone</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. E-Mail</td>
		  </tr>
		  <tr>
		    <td>' . $dadosRespConced ['estado'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['endcep'] . '</td>
		    <td>' . $dadosRespConced ['fone'] . '</td>
		    <td colspan="2">' . $dadosRespConced ['usuemail'] . '</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;" class="jsCounter">%d. N&ordm; da Cédula da CI</td>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Õrgão Expeditor</td>
		    <td colspan="3" style="font-weight: bold;" class="jsCounter">%d. Cargo</td>
		  </tr>
		  <tr>
		    <td>' . $dadosRespConced['numeroidentidade'] . '</td>
		    <td colspan="2">' . $dadosRespConced['entorgaoexpedidor'] . '</td>
		    <td colspan="3">' . $dadosRespConced['usufuncao'] . '</td>
		  </tr>';
        if ($responsavel) {
            $html .= '<tr>
		    <td colspan="5" style="font-weight: bold;" class="jsCounter">%d. Coordenação Responsável</td>
		    <td width="214" style="font-weight: bold;" class="jsCounter">%d. CPF </td>
		  </tr>
		  <tr>
		    <td colspan="5">' . $responsavel['usunome'] . '</td>
		    <td>' . formatar_cpf($responsavel['usucpf']) . '</td>
		  </tr>';
        }
$html .= '</table>';
?>
<!-- OBJETO E JUSTIFICATIVA DA DESCENTRALIZAÇÃO DO CRÉDITO -->
<?php

$html .= '
		<table id="objeto_justificativa" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td colspan="2" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">OBJETO E JUSTIFICATIVA DA DESCENTRALIZAÇÃO DO CRÉDITO</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Identificação (Título/Objeto da Despesa)</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpdscobjetoidentificacao'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Objetivo</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpobjetivoobjeto'] . '</td>
		  </tr>
		  <tr>
		    <td width="350" style="font-weight: bold;" class="jsCounter">%d. UG/Gestão Repassadora</td>
		    <td width="350" style="font-weight: bold;" class="jsCounter">%d. UG/Gestão Recebedora</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">';

if (in_array ( $dadoConc ['ungcod'], array (
		UG_FNDE,
		UG_INEP,
		UG_CAPES
) )) {
	$html .= $dadoConc ['ungcod'] . ' / ' . $dadoConc ['ungcod'];
} else {
	$html .= '152734 / 00001';
}

$html .= '</td>
		    <td>' . $dadosUG ['ungcod'] . ' / ' . $dadosUG ['gescod'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;" class="jsCounter">%d. Justificativa (Motivação/Clientela/Cronograma Fisico)</td>
		  </tr>
		  <tr>
		    <td colspan="2">' . $dadosObjeto ['tcpobjetojustificativa'] . '</td>
		  </tr>
		  <tr>
		    <td colspan="2">';

// $dadosObjeto['ungcodproponente'] == UG_FNDE || (
if ($dadosObjeto ['ungcodconcedente'] == UG_FNDE && in_array ( $dadosObjeto ['ungcodpoliticafnde'], array (
		UG_SECADI,
		UG_SETEC,
		UG_SEB
) )) {
	$html .= '
		    	<p>I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei Complementar nº 101/2000, Lei nº 8.666, de 21 de junho de 1993, no que couber, Lei nº 4.320/1964, Lei nº 10.520/2002, Decreto nº 93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012, bem como o disposto na Resolução CD/FNDE nº 28/2013.<br/></p>
				<p>
		    	II - constituem obrigações da CONCEDENTE:<br/>
				a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho; <br/>
		    	</p>
		    	<p>
				III - constituem obrigações do GESTOR DO PROGRAMA:<br/>
				a) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br/>
				b) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br/>
				c) analisar o relatório de cumprimento do objeto do presente Termo;<br/>
	    		</p>
		    	<p>
				IV - constituem obrigações da PROPONENTE:<br/>
				a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
				b) solicitar à UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
				c) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
				d) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br/>
				e) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br/>
				f) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem a forma de execução da ação a que os créditos estiverem vinculados;<br/>
				g) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br/>
				h) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br/>
				i) emitir o relatório descritivo de cumprimento do objeto proposto;<br/>
				j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcançados;<br/>
				k) assumir todas as obrigações legais decorrentes de contratações necessárias à execução do objeto do termo;<br/>
				l) solicitar ao gestor do projeto , quando for o caso, a prorrogação do prazo para cumprimento do objeto em até quinze (15) dias antes do término previsto no termo de execução descentralizada, ficando tal prorrogação condicionada à aprovação por aquele;<br/>
				m) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br/>
				n) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.<br/>
		    	</p>';
} else {
	$html .= '
		    	<p>I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei Complementar nº 101/2000, Lei nº 8.666, de 21 de junho de 1993, no que couber, Lei nº 4.320/1964, Lei nº 10.520/2002, Decreto nº 93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012, bem como o disposto na Resolução CD/FNDE nº 28/2013.<br/></p>
		    	<p>
				II - constituem obrigações da CONCEDENTE:<br/>
				a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br/>
				b) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br/>
				c) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br/>
				d) analisar o relatório de cumprimento do objeto do presente Termo;<br/>
		    	</p>
		    	<p>
				III - constituem obrigações da PROPONENTE:<br/>
				a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
				b) solicitar à UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
				c) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
				d) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br/>
				e) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br/>
				f) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem a forma de execução da ação a que os créditos estiverem vinculados;<br/>
				g) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br/>
				h) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br/>
				i) emitir o relatório descritivo de cumprimento do objeto proposto;<br/>
				j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcançados;<br/>
				k) assumir todas as obrigações legais decorrentes de contratações necessárias à execução do objeto do termo;<br/>
				l) solicitar ao gestor do projeto , quando for o caso, a prorrogação do prazo para cumprimento do objeto em até quinze (15) dias antes do término previsto no termo de execução descentralizada, ficando tal prorrogação condicionada à aprovação por aquele;<br/>
				m) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br/>
				n) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.<br/>
		    	</p>';
}
$html .= '</td>
		  </tr>
		</table>';
?>
<!-- PREVISÃO ORÇAMENTÁRIA -->
<?php
$dadosPrevisao = buscarPrevisaoOrca ();
$html .= '
		<table id="previsao_orc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100#">
		  <tr>
		    <td height="21" colspan="8"style="background-color: #CFCFCF; text-align:center; font-weight: bold;">PREVISÃO ORÇAMENTÁRIA</td>
		  </tr>
		  <tr>
		  	<td style="font-weight: bold;" class="jsCounter">%d. Ano</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Programa de Trabalho</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Ação</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Plano Interno</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Descrição da Ação constante da LOA</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Nat. da Despesa</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Mês da Liberação</td>
		    <td style="font-weight: bold;" class="jsCounter">%d. Valor (em R$ 1,00)</td>
		  </tr>';

if ($dadosPrevisao) :
    $tot = 0;
	$arAnosPrevisao = array ();
	$totalPrevisao = count ( $dadosPrevisao ) - 1;

    $indice = 0;
	foreach ( $dadosPrevisao as $k => $d ) :

		if (!in_array ( $d ['proanoreferencia'], $arAnosPrevisao)) {
			if ($subTotalPorAno > 0) {

				$html .= '
                    <tr bgcolor="#f0f0f0">
                        <td colspan="8" align="right">
                            <table>
                                <tr>
                                    <td><b>Subtotal (' . ($anoAnterior ? $anoAnterior : 'ano não informado') . ')</b>&nbsp;</td>
                                    <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, ',', '.') . '</b></td>
                                    <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, '.', ',') . '</b></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                ';
                $indice++;
			}
			array_push ( $arAnosPrevisao, $d ['proanoreferencia'] );
			$subTotalPorAno = 0;
			$anoAnterior = $d ['proanoreferencia'];
		}


        if ((int)$d['valor'] > 0) {
            $valor = $d['valor_f'];
        } else {
            if ((int)$dado['valor'] <= 0) {
                if ($db->pegaLinha("select * from elabrev.creditoremanejado where proid = {$d['proid']}"))
                    continue;
            }
        }

		$html .= '
          <tr>
            <td style="font-size:10px;">' . $d ['proanoreferencia'] . '</td>
            <td style="font-size:10px;">' . $d ['plano_trabalho'] . '</font></td>
            <td style="font-size:10px;">' . $d ['acao'] . '</td>
            <td style="font-size:10px;">' . $d ['plano_interno'] . '</td>
            <td style="font-size:10px;">' . $d ['acao_loa'] . '</td>
            <td style="font-size:10px;">' . $d ['nat_despesa'] . '</td>
            <td style="font-size:10px;">' . mes_extenso ( $d ['crdmesliberacao'] ) . '</td>
            <td style="font-size:10px;" align="right">R$ ' . $valor . '</td>
          </tr>
        ';


		$subTotalPorAno = $subTotalPorAno + trim($d['valor']);

        $tot += trim($d['valor']);
	endforeach
	;

    if ($totalPrevisao == $k) {
        $html .= '
            <tr bgcolor="#f0f0f0">
                <td colspan="8" align="right">
                    <table>
                        <tr>
                            <td><b>Subtotal (' . ($anoAnterior ? $anoAnterior : 'ano não informado') . ')</b>&nbsp;</td>
                            <td align="right" width="110"><b>R$ ' . number_format($subTotalPorAno, 2, '.', ',') . '</b></td>
                        </tr>
                    </table>
                </td>
            </tr>
        ';
    }
endif;

$sql = "select * from monitora.previsaoorcamentaria
		where tcpid = {$_SESSION['elabrev']['tcpid']} AND prostatus = 'A' AND crdmesexecucao IS NOT NULL
		order by proid limit 1";

$rsPOmes = $db->pegaLinha ( $sql );

$html .= '
		  <tr>
		  	<td colspan="2"><b class="jsCounter">%d. Prazo para o cumprimento do objeto</b></td>
		  	<td colspan="2">' . ($rsPOmes ['crdmesexecucao'] ? $rsPOmes ['crdmesexecucao'] . '&nbsp;meses' : '') . '&nbsp;</td>
		    <td colspan="2" align="right" style="font-weight: bold;" class="jsCounter">%d. TOTAL</td>
		    <td colspan="2" align="right">R$ ' . $d ['total'] . '</td>
		  </tr>
		</table>';
?>
<!--  ASSINATURAS -->
<?php
$mes = date ( "m" );

switch ($mes) {
	case 1 :
		$mes = "Janeiro";
		break;
	case 2 :
		$mes = "Fevereiro";
		break;
	case 3 :
		$mes = "Março";
		break;
	case 4 :
		$mes = "Abril";
		break;
	case 5 :
		$mes = "Maio";
		break;
	case 6 :
		$mes = "Junho";
		break;
	case 7 :
		$mes = "Julho";
		break;
	case 8 :
		$mes = "Agosto";
		break;
	case 9 :
		$mes = "Setembro";
		break;
	case 10 :
		$mes = "Outubro";
		break;
	case 11 :
		$mes = "Novembro";
		break;
	case 12 :
		$mes = "Dezembro";
		break;
}

$sqlReitor = "select
				u.usunome,
				u.usucpf,
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			and a.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . "
			order by hstid asc";

$rsReitor = $db->pegaLinha ( $sqlReitor );
$stAnaliseReitor = '';

//mostra assinatura somente quando termo é aprovado pelo representante legal do proponente
$mostraAssinaturaReitor = (!in_array($estadoAtual, array(EM_CADASTRAMENTO, TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP, EM_APROVACAO_DA_REITORIA)));

if ($rsReitor && $mostraAssinaturaReitor) {
	$stAnaliseReitor = "Autorizado pelo(a) {$rsReitor['usunome']} no dia {$rsReitor['htddata']} às {$rsReitor['hora']} <br/>";
}

$sqlPresidente = "select * from monitora.termocooperacao where ungcodconcedente = '153173' and tcpid = {$_SESSION['elabrev']['tcpid']}";
//ver($sqlPresidente, d);
$rsPresidente = $db->pegaLinha($sqlPresidente);

$where = '';
if ($rsPresidente) {
	$where .= " and a.esdiddestino in ( " . EM_EMISSAO_NOTA_CREDITO . ", ". EM_ANALISE_PELA_SPO ." )";
} else {
	$where .= " and a.esdiddestino = " . EM_ANALISE_PELA_CGSO . " ";
}

$sqlSecretario = "select
				u.usunome,
				u.usucpf,
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			{$where}
			order by hstid asc";
//ver($sqlSecretario, d);
$rsSecretario = $db->pegaLinha ( $sqlSecretario );
$stAnaliseSecretaria = '';

$arrayPull = array(
    EM_CADASTRAMENTO,
    TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
    EM_APROVACAO_DA_REITORIA,
    EM_ANALISE_OU_PENDENTE,
    AGUARDANDO_APROVACAO_DIRETORIA,
    AGUARDANDO_APROVACAO_SECRETARIO,
    TERMO_EM_ANALISE_ORCAMENTARIA_FNDE,
    AGUARDANDO_APROVACAO_DIRETORIA,
);

/**
 * mostra assinatura somente quando termo é
 * autorizado pelo representante legal do concedente
 * ou
 * encaminhado para validação da diretoria do FNDE
 */
$mostraAssConcedente = (!in_array($estadoAtual, $arrayPull));

if ($rsSecretario && $mostraAssConcedente) {
	if ($rsPresidente) {
		$stAnaliseSecretaria = "Autorizado pelo(a) presidente(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} às {$rsSecretario['hora']}";
	} else {
		$stAnaliseSecretaria = "Autorizado pelo(a) secretário(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} às {$rsSecretario['hora']}";
	}
}
$html .= '
		<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100#">
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <td align="center">
		    	' . $stAnaliseReitor . '
		    	' . $stAnaliseSecretaria . '
		    </td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <!-- <td colspan="2" style="text-align:center; font-weight: bold;">Brasília, ' . date ( "d" ) . ' de ' . $mes . ' de ' . date ( "Y" ) . '</td> -->
    		<td colspan="2" style="text-align:center; font-weight: bold;">' . recuperaDataGeraPdf () . '</td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		</table>';

$arr = array();
$count = substr_count($html, '%d');
for ($i=1; $i<=$count; $i++) {
    $pos = strpos($html, '%d');

    if ($pos !== false) {
        $html = substr_replace($html, $i, $pos, 2);
    }
}

$html = str_replace('100#', '100%', $html);

if ($_GET['pdfAction'] == true) {

    /*define('_MPDF_URI', APPRAIZ . 'includes/mpdf60/');
    define('_MPDF_PATH', APPRAIZ . 'includes/mpdf60/');
    define('_MPDF_TEMP_PATH', sys_get_temp_dir().'/');
    define('_MPDF_TTFONTDATAPATH', sys_get_temp_dir().'/');

    include_once APPRAIZ . 'includes/mpdf60/mpdf.php';
    $html = utf8_encode($html);

    $mpdf = new mPDF();
    $mpdf->SetDisplayMode('fullpage');
    $mpdf->text_input_as_HTML = true;
    $mpdf->charset_in = 'utf-8';
    $mpdf->WriteHTML($html);
    $name = "Termo_De_Execucao_Descentralizada_n_{$_SESSION['elabrev']['tcpid']}.pdf";
    $mpdf->Output($name, 'D');*/

    include_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';
    $pdfObj = new HtmlToPdf($html);
    $pdfObj->setTitle("Termo_De_Execucao_Descentralizada_n_{$_SESSION['elabrev']['tcpid']}.pdf");
    $pdfObj->getPDF();
    exit;

} else {
    echo $html;
}
?>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela"
	cellpadding="3" cellspacing="1">
	<tr>
		<td colspan="3" align="center"><input type="button" value="Gerar PDF" class="gerarPDF" /></td>
	</tr>
</table>
</form>

