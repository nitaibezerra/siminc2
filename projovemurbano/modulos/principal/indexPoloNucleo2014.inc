<?php 
if($_SESSION['projovemurbano']['ppuid'] != '3'){
	echo "<script>
			alert('Falha na verificação do ano de exercicio.');
			window.location.href = 'projovemurbano.php?modulo=inicio&acao=C';
		  </script>";
	die();
}

if(!$_SESSION['projovemurbano']['pjuid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemurbano.php?modulo=inicio&acao=C';</script>");

include_once APPRAIZ."/www/projovemurbano/_funcoes_planoImplementacao2014.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$docid = $db->pegaUm("SELECT docid FROM projovemurbano.projovemurbano WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemurbano.projovemurbano SET docid='".$docid."' WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Projovem Urbano', montaTituloEstMun());

echo montarAbasArray(montaMenuProJovemUrbano(), "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A");


$us = pegarUsuarioProJovem();
$rsMetas = recuperaMetasPorUfMuncod($us);

switch( $_GET['aba'] )
{
	case 'Publico_Juventude_Viva':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=Publico_Juventude_Viva";
		$_SESSION['projovemurbano']['tprid'] = 1;
		$arrOrientacoes = Array('','',
				'<p>A seguir apresenta-se a meta final do termo de adesão para o exercício de 
				2014, bem como o período de matrícula e a data do início de aula.</p>',
				'<p>O Polo só pode ser constituído na localidade onde a Secretaria de 
				Educação for organizada em coordenadorias ou regionais de ensino. 
				Os Núcleos, localizados nos municípios da área de abrangência de cada 
				regional de ensino, independentemente de seu número, formam um Polo. 
				O Polo é, portanto, uma instância de gestão do Projovem Urbano, que 
				funciona no espaço físico da própria regional de ensino.</p>
				<p>Um núcleo é composto por 5 (cinco) turmas. Se o núcleo for igual a 1 
				(um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for 
				maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 e 200.
				A turma pode ter entre 30 e 40 alunos cada uma.</p>',
				'<p>A equipe é composta por:</p>
				<p> 1 coordenador geral.</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> 1 diretor de polo (onde houver regional de ensino).</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> Até 2 profissionais por núcleo de apoio para a etapa de matrícula.</p>
				<p> 5 educadores de ensino fundamental (Ciências Humanas, Língua Portuguesa, Inglês, Matemática e Ciências da Natureza).</p>
				<p> 1 educador de qualificação profissional.</p>
				<p> 1 educador de participação cidadã.</p>
				<p> Até 2 educadores para as salas de acolhimento das crianças de 0 a 8 anos dos jovens atendidos pelo programa.</p>
				<p> 1 tradutor e intérprete de libras.</p>
				<p> 1 educador para atendimento educacional especializado.</p>
				<p>A seguir será discriminado o pagamento dos educadores, dos assistentes, 
				da coordenação geral e do diretor de polo, quando houver, e dos 
				profissionais para a etapa da matrícula, de acordo com percentual máximo 
				permitido.</p>
				<p>Ao final do lançamento deverá ser informado o instrumento legal que 
				rege as remunerações dos profissionais.</p>
				<p>Para maiores esclarecimentos, consulte a Resolução n° 54 de 21 de 
				novembro de 2012.</p>',
				'<p>A seguir será discriminado o custeio da formação continuada de 
				educadores de ensino fundamental, qualificação profissional e 
				participação cidadã, bem como pagamento de auxílio financeiro aos 
				educadores durante a primeira etapa de formação, conforme os 
				percentuais determinados na Resolução CD/FNDE n° 54/2012.</p>
				<p>ATENÇÃO: somente os educadores NÃO contratados até o período da 
				formação poderão receber o auxílio financeiro.</p>
				<p>Carga horária da formação de educadores:</p>
				<p>Formação Continuada - Primeira etapa (realizada antes do início das 
				aulas) - 96 horas presenciais e 64 horas de atividades não presenciais</p>
				<p>Formação Continuada - Demais etapas (realizadas em formas de 
				encontros durante os 18 meses de curso) - 216 horas presenciais</p>
				<p>Os municípios com menos de 20 educadores poderão designar do quadro 
				da rede de ensino local e/ou contratar até dois formadores para garantir 
				a continuidade das ações da formação até o final do curso, observando o 
				percentual permitido para o custeio dessa ação.</p>
				<p>As demais localidades deverão designar e/ou contratar um formador para 
				cada grupo de 20 a 30 educadores também observando o percentual 
				permitido para o custeio dessa ação.</p>',
				'<p>Aquisição de gêneros alimentícios destinados para fornecimento de 
				lanche ou refeição aos jovens matriculados e frequentes, no âmbito 
				do Programa, bem como para filhos de beneficiários do Programa que 
				tenham até oito anos e sejam atendidos nas salas de acolhimento, até que 
				o ente executor passe a receber os recursos procedentes do Programa 
				Nacional de Alimentação Escolar (PNAE).</p>',
				'<p>A Qualificação Profissional será ofertada por meio da Formação Técnica 
				Geral - FTG e da Formação Técnica Específica FTE e do Projeto de 
				Orientação Profissional - POP. A Formação Técnica Específica, por sua 
				vez, poderá ser desenvolvida por meio de programas nacionais de 
				educação técnica em formação inicial e continuada ou por meio de 
				arcos ocupacionais ou por meio das duas opções (Arcos Ocupacionais e 
				PRONATEC).</p>
				<p>Para a oferta da FTG, por meio dos arcos ocupacionais, apresenta-se, a 
				seguir, as despesas possíveis para esta execução: locação de espaços e 
				equipamentos, aquisição de material de consumo, bem como pagamento 
				de monitores para as atividades práticas da qualificação profissional 
				quando for necessário, conforme definido na Resolução CD/FNDE nº 54/
				2012.</p>',
				'<p>Para os Estados e DF: discrimina-se a seguir até 1,5% do montante 
				repassado para pagamento do transporte do material didático-pedagógico 
				entregue pelo Governo Federal, da capital até os Municípios.</p>',
				'<p>Caso o EEx não atinja os percentuais máximos previstos na Resolução 
				CD/FNDE nº 54/2012 nas ações descritas nas abas anteriores, poderá 
				empregar o restante dos recursos transferidos para custear as ações 
				discriminadas abaixo, de acordo com percentuais máximo permitidos 
				definidos na mesma Resolução.</p>',
				'<p>Este Município/Estado esclarece que tem ciência de que o Plano de 
				Implementação apresentado é um instrumento de planejamento de 
				ações e controle de gastos e que, para efeito de repasse da 1ª parcela de 
				recursos, é considerado o número da meta de estudantes.</p>
				<p>Da mesma forma, este Município/Estado tem ciência de que as demais 
				parcelas serão efetuadas com base na fórmula descrita no Anexo VI da 
				Resolução CD/FNDE Nº 54/2012. Na mesma resolução determina-se 
				que esses repasses terão como base de cálculo o número de estudantes 
				efetivamente matriculados e frequentes, de acordo com o Sistema de 
				Matrícula, Acompanhamento de Frequência e Certificação do Projovem 
				Urbano.</p>
				<p>Por fim, este Município/Estado compromete-se a atualizar estas 
				informações, constantemente, conforme prazos e por meio dos 
				instrumentos estabelecidos pela DPEJUV/SECADI/MEC.</p>',
				'<p>Esta planilha demonstra os gastos a serem realizados por este Município. 
				É importante ressaltar que os repasses a serem efetuados são calculados 
				com base no número de alunos (per capita) e na fórmula descrita no 
				Anexo VI à Resolução do FNDE acerca do ProJovem Urbano, para o 
				exercício de 2014.</p>'
				);
		break;
	case 'Publico_Unidades_Prisionais':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=Publico_Unidades_Prisionais";
		$_SESSION['projovemurbano']['tprid'] = 2;
		$arrOrientacoes = Array('','',
				'<p>A seguir apresenta-se a meta final do termo de adesão para o exercício de
				2014, bem como o período de matrícula e a data do início de aula.</p>',
				'<p>O Polo só pode ser constituído na localidade onde a Secretaria de
				Educação for organizada em coordenadorias ou regionais de ensino.
				Os Núcleos, localizados nos municípios da área de abrangência de cada
				regional de ensino, independentemente de seu número, formam um Polo.
				O Polo é, portanto, uma instância de gestão do Projovem Urbano, que
				funciona no espaço físico da própria regional de ensino.</p>
				<p>O núcleo da Unidade Prisional é composto de no mínimo 60 estudantes e no máximo 150.</p>',
				'<p>A equipe é composta por:</p>
				<p> 1 coordenador geral.</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> 1 diretor de polo (onde houver regional de ensino).</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> Até 2 profissionais por núcleo de apoio para a etapa de matrícula.</p>
				<p> 5 educadores de ensino fundamental (Ciências Humanas, Língua Portuguesa, Inglês, Matemática e Ciências da Natureza).</p>
				<p> 1 educador de qualificação profissional.</p>
				<p> 1 educador de participação cidadã.</p>
				<p> Até 2 educadores para as salas de acolhimento das crianças de 0 a 8 anos dos jovens atendidos pelo programa.</p>
				<p> 1 tradutor e intérprete de libras.</p>
				<p> 1 educador para atendimento educacional especializado.</p>
				<p>A seguir será discriminado o pagamento dos educadores, dos assistentes,
				da coordenação geral e do diretor de polo, quando houver, e dos
				profissionais para a etapa da matrícula, de acordo com percentual máximo
				permitido.</p>
				<p>Ao final do lançamento deverá ser informado o instrumento legal que
				rege as remunerações dos profissionais.</p>
				<p>Para maiores esclarecimentos, consulte a Resolução n° 54 de 21 de
				novembro de 2012.</p>',
				'<p>A seguir será discriminado o custeio da formação continuada de
				educadores de ensino fundamental, qualificação profissional e
				participação cidadã, bem como pagamento de auxílio financeiro aos
				educadores durante a primeira etapa de formação, conforme os
				percentuais determinados na Resolução CD/FNDE n° 54/2012.</p>
				<p>ATENÇÃO: somente os educadores NÃO contratados até o período da
				formação poderão receber o auxílio financeiro.</p>
				<p>Carga horária da formação de educadores:</p>
				<p>Formação Continuada - Primeira etapa (realizada antes do início das
				aulas) - 96 horas presenciais e 64 horas de atividades não presenciais</p>
				<p>Formação Continuada - Demais etapas (realizadas em formas de
				encontros durante os 18 meses de curso) - 216 horas presenciais</p>
				<p>Os municípios com menos de 20 educadores poderão designar do quadro
				da rede de ensino local e/ou contratar até dois formadores para garantir
				a continuidade das ações da formação até o final do curso, observando o
				percentual permitido para o custeio dessa ação.</p>
				<p>As demais localidades deverão designar e/ou contratar um formador para
				cada grupo de 20 a 30 educadores também observando o percentual
				permitido para o custeio dessa ação.</p>',
				'<p>Aquisição de gêneros alimentícios destinados para fornecimento de
				lanche ou refeição aos jovens matriculados e frequentes, no âmbito
				do Programa, bem como para filhos de beneficiários do Programa que
				tenham até oito anos e sejam atendidos nas salas de acolhimento, até que
				o ente executor passe a receber os recursos procedentes do Programa
				Nacional de Alimentação Escolar (PNAE).</p>',
				'<p>A Qualificação Profissional será ofertada por meio da Formação Técnica
				Geral - FTG e da Formação Técnica Específica FTE e do Projeto de
				Orientação Profissional - POP. A Formação Técnica Específica, por sua
				vez, poderá ser desenvolvida por meio de programas nacionais de
				educação técnica em formação inicial e continuada ou por meio de
				arcos ocupacionais ou por meio das duas opções (Arcos Ocupacionais e
				PRONATEC).</p>
				<p>Para a oferta da FTG, por meio dos arcos ocupacionais, apresenta-se, a
				seguir, as despesas possíveis para esta execução: locação de espaços e
				equipamentos, aquisição de material de consumo, bem como pagamento
				de monitores para as atividades práticas da qualificação profissional
				quando for necessário, conforme definido na Resolução CD/FNDE nº 54/
				2012.</p>',
				'<p>Para os Estados e DF: discrimina-se a seguir até 1,5% do montante
				repassado para pagamento do transporte do material didático-pedagógico
				entregue pelo Governo Federal, da capital até os Municípios.</p>',
				'<p>Caso o EEx não atinja os percentuais máximos previstos na Resolução
				CD/FNDE nº 54/2012 nas ações descritas nas abas anteriores, poderá
				empregar o restante dos recursos transferidos para custear as ações
				discriminadas abaixo, de acordo com percentuais máximo permitidos
				definidos na mesma Resolução.</p>',
				'<p>Este Município/Estado esclarece que tem ciência de que o Plano de
				Implementação apresentado é um instrumento de planejamento de
				ações e controle de gastos e que, para efeito de repasse da 1ª parcela de
				recursos, é considerado o número da meta de estudantes.</p>
				<p>Da mesma forma, este Município/Estado tem ciência de que as demais
				parcelas serão efetuadas com base na fórmula descrita no Anexo VI da
				Resolução CD/FNDE Nº 54/2012. Na mesma resolução determina-se
				que esses repasses terão como base de cálculo o número de estudantes
				efetivamente matriculados e frequentes, de acordo com o Sistema de
				Matrícula, Acompanhamento de Frequência e Certificação do Projovem
				Urbano.</p>
				<p>Por fim, este Município/Estado compromete-se a atualizar estas
				informações, constantemente, conforme prazos e por meio dos
				instrumentos estabelecidos pela DPEJUV/SECADI/MEC.</p>',
				'<p>Esta planilha demonstra os gastos a serem realizados por este Município.
				É importante ressaltar que os repasses a serem efetuados são calculados
				com base no número de alunos (per capita) e na fórmula descrita no
				Anexo VI à Resolução do FNDE acerca do ProJovem Urbano, para o
				exercício de 2014.</p>'
		);
		break;
	case 'Publico_Geral':
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=Publico_Geral";
		$_SESSION['projovemurbano']['tprid'] = 3;
		$arrOrientacoes = Array('','',
				'<p>A seguir apresenta-se a meta final do termo de adesão para o exercício de
				2014, bem como o período de matrícula e a data do início de aula.</p>',
				'<p>O Polo só pode ser constituído na localidade onde a Secretaria de
				Educação for organizada em coordenadorias ou regionais de ensino.
				Os Núcleos, localizados nos municípios da área de abrangência de cada
				regional de ensino, independentemente de seu número, formam um Polo.
				O Polo é, portanto, uma instância de gestão do Projovem Urbano, que
				funciona no espaço físico da própria regional de ensino.</p>
				<p>Um núcleo é composto por 5 (cinco) turmas. Se o núcleo for igual a 1
				(um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for
				maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 e 200.
				A turma pode ter entre 30 e 40 alunos cada uma.</p>',
				'<p>A equipe é composta por:</p>
				<p> 1 coordenador geral.</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> 1 diretor de polo (onde houver regional de ensino).</p>
				<p> 1 assistente pedagógico.</p>
				<p> 1 assistente administrativo.</p>
				<p> Até 2 profissionais por núcleo de apoio para a etapa de matrícula.</p>
				<p> 5 educadores de ensino fundamental (Ciências Humanas, Língua Portuguesa, Inglês, Matemática e Ciências da Natureza).</p>
				<p> 1 educador de qualificação profissional.</p>
				<p> 1 educador de participação cidadã.</p>
				<p> Até 2 educadores para as salas de acolhimento das crianças de 0 a 8 anos dos jovens atendidos pelo programa.</p>
				<p> 1 tradutor e intérprete de libras.</p>
				<p> 1 educador para atendimento educacional especializado.</p>
				<p>A seguir será discriminado o pagamento dos educadores, dos assistentes,
				da coordenação geral e do diretor de polo, quando houver, e dos
				profissionais para a etapa da matrícula, de acordo com percentual máximo
				permitido.</p>
				<p>Ao final do lançamento deverá ser informado o instrumento legal que
				rege as remunerações dos profissionais.</p>
				<p>Para maiores esclarecimentos, consulte a Resolução n° 54 de 21 de
				novembro de 2012.</p>',
				'<p>A seguir será discriminado o custeio da formação continuada de
				educadores de ensino fundamental, qualificação profissional e
				participação cidadã, bem como pagamento de auxílio financeiro aos
				educadores durante a primeira etapa de formação, conforme os
				percentuais determinados na Resolução CD/FNDE n° 54/2012.</p>
				<p>ATENÇÃO: somente os educadores NÃO contratados até o período da
				formação poderão receber o auxílio financeiro.</p>
				<p>Carga horária da formação de educadores:</p>
				<p>Formação Continuada - Primeira etapa (realizada antes do início das
				aulas) - 96 horas presenciais e 64 horas de atividades não presenciais</p>
				<p>Formação Continuada - Demais etapas (realizadas em formas de
				encontros durante os 18 meses de curso) - 216 horas presenciais</p>
				<p>Os municípios com menos de 20 educadores poderão designar do quadro
				da rede de ensino local e/ou contratar até dois formadores para garantir
				a continuidade das ações da formação até o final do curso, observando o
				percentual permitido para o custeio dessa ação.</p>
				<p>As demais localidades deverão designar e/ou contratar um formador para
				cada grupo de 20 a 30 educadores também observando o percentual
				permitido para o custeio dessa ação.</p>',
				'<p>Aquisição de gêneros alimentícios destinados para fornecimento de
				lanche ou refeição aos jovens matriculados e frequentes, no âmbito
				do Programa, bem como para filhos de beneficiários do Programa que
				tenham até oito anos e sejam atendidos nas salas de acolhimento, até que
				o ente executor passe a receber os recursos procedentes do Programa
				Nacional de Alimentação Escolar (PNAE).</p>',
				'<p>A Qualificação Profissional será ofertada por meio da Formação Técnica
				Geral - FTG e da Formação Técnica Específica FTE e do Projeto de
				Orientação Profissional - POP. A Formação Técnica Específica, por sua
				vez, poderá ser desenvolvida por meio de programas nacionais de
				educação técnica em formação inicial e continuada ou por meio de
				arcos ocupacionais ou por meio das duas opções (Arcos Ocupacionais e
				PRONATEC).</p>
				<p>Para a oferta da FTG, por meio dos arcos ocupacionais, apresenta-se, a
				seguir, as despesas possíveis para esta execução: locação de espaços e
				equipamentos, aquisição de material de consumo, bem como pagamento
				de monitores para as atividades práticas da qualificação profissional
				quando for necessário, conforme definido na Resolução CD/FNDE nº 54/
				2012.</p>',
				'<p>Para os Estados e DF: discrimina-se a seguir até 1,5% do montante
				repassado para pagamento do transporte do material didático-pedagógico
				entregue pelo Governo Federal, da capital até os Municípios.</p>',
				'<p>Caso o EEx não atinja os percentuais máximos previstos na Resolução
				CD/FNDE nº 54/2012 nas ações descritas nas abas anteriores, poderá
				empregar o restante dos recursos transferidos para custear as ações
				discriminadas abaixo, de acordo com percentuais máximo permitidos
				definidos na mesma Resolução.</p>',
				'<p>Este Município/Estado esclarece que tem ciência de que o Plano de
				Implementação apresentado é um instrumento de planejamento de
				ações e controle de gastos e que, para efeito de repasse da 1ª parcela de
				recursos, é considerado o número da meta de estudantes.</p>
				<p>Da mesma forma, este Município/Estado tem ciência de que as demais
				parcelas serão efetuadas com base na fórmula descrita no Anexo VI da
				Resolução CD/FNDE Nº 54/2012. Na mesma resolução determina-se
				que esses repasses terão como base de cálculo o número de estudantes
				efetivamente matriculados e frequentes, de acordo com o Sistema de
				Matrícula, Acompanhamento de Frequência e Certificação do Projovem
				Urbano.</p>
				<p>Por fim, este Município/Estado compromete-se a atualizar estas
				informações, constantemente, conforme prazos e por meio dos
				instrumentos estabelecidos pela DPEJUV/SECADI/MEC.</p>',
				'<p>Esta planilha demonstra os gastos a serem realizados por este Município.
				É importante ressaltar que os repasses a serem efetuados são calculados
				com base no número de alunos (per capita) e na fórmula descrita no
				Anexo VI à Resolução do FNDE acerca do ProJovem Urbano, para o
				exercício de 2014.</p>'
		);
		break;
	default:
		$abaAtiva = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=coordenadorResponsavel";
		$pagAtiva = "coordenadorResponsavel";
		unset($_SESSION['projovemurbano']['tprid']);
		break;
}

$menu[] = array("id" => 1,
		"descricao" => "Coordenador Responsável",
		"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=coordenadorResponsavel");

$sql = "SELECT * FROM projovemurbano.coordenadorresponsavel WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$coordenadorresponsavel = $db->pegaLinha($sql);

if ($coordenadorresponsavel) {
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$cmecodibge = $_SESSION ['projovemurbano'] ['muncod'];
	} else {
		$cmecodibge = $db->pegaUm ( "select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'" );
	}
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		
		$sql = "SELECT cam.* FROM projovemurbano.cargameta cam
		LEFT JOIN territorios.municipio mun ON (cam.cmecodibge = mun.muncod::INT)
		LEFT JOIN projovemurbano.projovemurbano ppu ON cam.cmecodibge = ppu.muncod::INT
		where cam.cmecodibge = '{$cmecodibge}' and ppu.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND pjuid = {$_SESSION['projovemurbano']['pjuid']}";
	} else {
		
		$sql = "SELECT cam.* FROM projovemurbano.cargameta cam
		LEFT JOIN territorios.estado est ON (cam.cmecodibge = est.estcod::INT)
		LEFT JOIN projovemurbano.projovemurbano ppu ON cam.cmecodibge = est.estcod::INT
		where cam.cmecodibge = '{$cmecodibge}' and cam.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND pjuid = {$_SESSION['projovemurbano']['pjuid']}";
	}
	
	$rsCargaMeta = $db->pegaLinha ( $sql );
	
	$sql = "select
		t.tpmid,
		mtpvalor,
		tpmdescricao,
		pjuid
		from
		projovemurbano.tipometadoprograma t
		left join
		projovemurbano.metasdoprograma m on m.tpmid = t.tpmid
		and cmeid = {$rsCargaMeta['cmeid']}
		and ppuid = {$_SESSION['projovemurbano']['ppuid']}
				where
				t.tpmid in (7,10,13)";
	
	$rsMetas = $db->carregar ( $sql );
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == 3) {
		
		$sql = '';
		foreach ( $rsMetas as $value ) {
			if (! $value ['pjuid']) {
				if ($value ['mtpvalor'] != '') {
					$sql .= "insert into projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor, pjuid)
					values ({$value['tpmid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$value['mtpvalor']}', '{$_SESSION['projovemurbano']['pjuid']}');";
				}
			}
		}
		
		if (! empty ( $sql )) {
			$sqlDelete = 'delete from projovemurbano.metasdoprograma where cmeid = ' . $rsCargaMeta ['cmeid'] . ' and tpmid in (7,10,13);';
			$db->executar ( $sqlDelete );
			$db->executar ( $sql );
			$db->commit ();
		}
	}
	
// 	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']} and ppuid = 3";
		$ajustado = $db->pegaUm ( $sql );
// 	}
	if ($ajustado == 't') {
		$not = ",7,10,13";
	} else {
		$not = ",9,12,15";
	}
	
	$sql = "SELECT DISTINCT
		removeacento(replace(tprdesc,' ','_')) as aba,
		tprdesc as descricao, mtpvalor
		FROM
		projovemurbano.tipoprograma tpr
		INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
		INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
		INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
		WHERE
		mtp.ppuid = 3
		AND mtp.pjuid = {$_SESSION['projovemurbano']['pjuid']}
		AND mtpvalor > 0
				AND tmp.tpmid NOT IN  (2,5,8,11,14$not)";
	$programas = $db->carregar ( $sql );
	if ($programas) {
		
		foreach ( $programas as $k => $programa ) {
			$menu [] = array (
					"id" => $k + 2,
					"descricao" => $programa ['descricao'],
					"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba=" . $programa ['aba'] 
			);
		}
		
// 		if (! $_GET ['aba']) {
// 			foreach ( $programas as $k => $programa ) {
// 				if ($programa ['aba'] != '') {
// 					$_GET ['aba'] = $programa ['aba'];
					
// 					break;
// 				}
// 			}
// 		}
	}
}
	
echo montarAbasArray($menu, $abaAtiva);
// monta_titulo(' ', '');
if( $_GET['aba'] != '' && $_GET['aba'] != 'coordenadorResponsavel'){
	switch( $_REQUEST['aba2'] )
	{
		case 'poloNucleo':
			$abaAtiva2 = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo2014";
			$pagAtiva = "poloNucleo";
			break;
		default:
			$abaAtiva2 = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo2014";
			$pagAtiva = "poloNucleo";
			break;
	}

// 	echo "<br>";
	
	$menu2[] = array("id" => 1,"descricao" => "Pólo/Núcleo","link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo2014");
// 	ver($menu2, $abaAtiva2,d);
// echo montarAbasArray($menu2, $abaAtiva2);
// 	monta_titulo(' ', '');

}
	include 'planoImplementacao'.$_SESSION['projovemurbano']['ppuano'].'/'.$pagAtiva.$_SESSION['projovemurbano']['ppuano'].'.inc';
?>
