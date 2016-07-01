<?php 
function comboEstado($estuf = null, $habilitado = "S")
{
	global $db;

	$estuf = $_POST['estuf'] ? $_POST['estuf'] : $estuf;
	
	$sql = "select 
				estuf as codigo,
				estuf as descricao
			from
				territorios.estado
			order by
				estuf";
	$db->monta_combo("estuf", $sql, $habilitado, "Selecione...", 'filtrarMunicipio', '', '', '', 'N','estuf');
}

function comboMunicipio($estuf = null, $habilitado = "S")
{
	global $db;

	$estuf = $_POST['estuf'] ? $_POST['estuf'] : $estuf;
	$muncod = $_POST['muncod'] ? $_POST['muncod'] : $muncod;
	
	if(!$estuf){
		echo "Selecione a UF.";
	}else{
		$sql = "select
					muncod as codigo,
					mundescricao as descricao
				from
					territorios.municipio
				where
					estuf = '$estuf'
				order by
					mundescricao";
		$db->monta_combo("muncod", $sql, $habilitado, "Selecione...", '', '', '', '', 'N','muncod');
	}
}

function textoHTML2($entid)
{
	global $db;
	$sql = "select * from entidade.entidade  ent
	inner join entidade.endereco ende ON ende.entid = ent.entid
	inner join territorios.municipio mun ON mun.muncod = ende.muncod
	where ent.entid = $entid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select
	usunome as nome
	from
	seguranca.usuario usu
	inner join
	maismedicos.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
	where
	ure.entid = $entid
	and
	ure.pflcod = ".PERFIL_REITOR."
	and
				ure.rpustatus = 'A'
			and
				usu.usustatus = 'A'
			order by
				rpudata_inc desc";
	$arrReitor = $db->pegaLinha($sql);
	if(!$arrReitor){
		$sql = "SELECT
		DISTINCT fun.funid, fun.fundsc, ent.entnome as nome, ent.entid
		FROM
		entidade.funcao fun
		LEFT JOIN
		entidade.funcaoentidade fen ON fen.funid = fun.funid
		AND fen.entid IN (SELECT
		fen2.entid
		FROM
		entidade.funentassoc fea2
		LEFT JOIN
		entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
		WHERE
		fea2.entid='$entid'
		AND fun.funid = fen2.funid)
		LEFT JOIN
		entidade.entidade ent ON fen.entid = ent.entid
		WHERE
		fun.funid IN('21')";
		$arrReitor = $db->pegaLinha($sql);
	}
	?>
<div style="text-align:justify;">
<center>
<h2>PORTARIA NORMATIVA Nº 14, DE 9 DE JULHO DE 2013</h2>
<p><b>MINISTÉRIO DA EDUCAÇÃO</b></p>
<p><b>GABINETE DO MINISTRO</b></p>
<p><b>DOU de 10/07/2013 (nº 131, Seção 1, pág. 18)</b></p>
</center>
<p>Dispõe sobre os procedimentos de adesão das instituições federais de educação superior ao Projeto Mais Médicos e dá outras providências.</p>
<p>O MINISTRO DE ESTADO DA EDUCAÇÃO no uso da atribuição que lhe confere o art. 87, inciso II da Constituição Federal, e tendo em vista o disposto na Medida Provisória nº 621, de 8 de julho de 2013, bem como na Portaria Interministerial MS/MEC nº 1.369, de 8 de julho de 2013, resolve:</p>
<p>Art. 1º - Poderão aderir ao Projeto Mais Médicos as instituições federais de educação superior que ofereçam curso de Medicina.</p>
<p>§ 1º - As instituições federais de educação superior interessadas em aderir ao Projeto Mais Médicos deverão apresentar termo de pré-adesão, conforme o modelo do <a href="javascript:abreLinkAnexo(<?php echo $entid ?>)" >Anexo I</a> desta Portaria, no período de 11 a 15 de julho de 2013, ao Ministério da Educação.</p>
<p>§ 2º - As instituições deverão indicar, no momento da préadesão, um tutor acadêmico responsável pelas atividades e, no mínimo, três tutores acadêmicos para fins de cadastro de reserva, que atendam aos requisitos da Portaria Interministerial MS/MEC nº 1.369, de 8 de julho de 2013 e desta Portaria.</p>
<p>§ 3º - As instituições deverão cadastrar via sistema SIMEC, no módulo rede federal, por meio do endereço eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, os tutores indicados no termo de pré-adesão.</p>
<p>§ 4º - No momento da pré-adesão as instituições deverão indicar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas.</p>
<p>Art. 2º - O Ministério da Educação decidirá sobre a validação do termo de pré-adesão das instituições que atenderem aos requisitos previstos no art. 1º desta Portaria, observadas as necessidades do Projeto Mais Médicos.</p>
<p>Parágrafo único - Em caso de manifestação de interesse de mais de uma instituição por unidade da federação, será dada preferência àquela sediada na capital, caso persista o empate, será selecionada àquela que ofertar curso de Medicina há mais tempo.</p>
<p>Art. 3º - As instituições que tiverem seus termos de pré-adesão validados pelo Ministério da Educação deverão firmar termo de adesão no prazo máximo de 10 (dez) dias após a divulgação das instituições selecionadas.</p>
<p>Parágrafo único - O termo de adesão estará disponível para assinatura das instituições selecionadas no sistema SIMEC, no módulo rede federal, por meio do endereço eletrônico <a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, e conterá, no mínimo, as seguintes obrigações para a instituição:</p>
<p>I - atuar em cooperação com os entes federativos, as Coordenações Estaduais do Projeto e organismos internacionais, no âmbito de sua competência, para execução do Projeto Mais Médicos;</p>
<p>II - coordenar o acompanhamento acadêmico do Projeto;</p>
<p>III - ratificar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas, indicada no termo de pré-adesão;</p>
<p>IV - definir mecanismo de avaliação e autorização de pagamento das bolsas de tutoria e supervisão;</p>
<p>V - ratificar a indicação dos tutores acadêmicos do Projeto, feita no termo de pré-adesão;</p>
<p>VI - definir critérios e mecanismo de seleção de supervisores;</p>
<p>VII - realizar seleção dos supervisores do Projeto;</p>
<p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acadêmicos no âmbito do Projeto;</p>
<p>IX - ofertar os módulos de acolhimento e avaliação aos médicos intercambistas; e</p>
<p>X - ofertar cursos de especialização e atividades de pesquisa, ensino e extensão aos médicos participantes.</p>
<p>Art. 4º - Os tutores acadêmicos serão selecionados pela instituição entre os docentes da área médica, preferencialmente vinculados à área de saúde coletiva ou correlata, ou à área de clínica médica.</p>
<p>§ 1º - Os tutores acadêmicos perceberão bolsa-tutoria, na forma prevista no termo de adesão.</p>
<p>§ 2º - Os tutores acadêmicos serão responsáveis pela orientação acadêmica e pelo planejamento das atividades do supervisor, trabalhando em parceria com as Coordenações Estaduais do Projeto, e tendo, no mínimo, as seguintes atribuições:</p>
<p>I - coordenar as atividades acadêmicas da integração ensinoserviço, atuando em cooperação com os supervisores e os gestores do SUS;</p>
<p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos médicos participantes e supervisores, bem como a metodologia de acompanhamento e avaliação;</p>
<p>III - monitorar o processo de acompanhamento e avaliação a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>IV - integrar as atividades do curso de especialização às atividades de integração ensino-serviço;</p>
<p>V - relatar à instituição pública de ensino superior à qual esteja vinculado a ocorrência de situações nas quais seja necessária a adoção de providência pela instituição; e</p>
<p>VI - apresentar relatórios periódicos da execução de suas atividades no Projeto à instituição à qual esteja vinculado e à Coordenação do Projeto.</p>
<p>Art. 5º - Os supervisores serão selecionados entre profissionais médicos por meio de edital conforme critérios e mecanismos estabelecidos pela instituição aderente e validados pela Coordenação Estadual do Projeto Mais Médicos.</p>
<p>§ 1º - Os supervisores selecionados perceberão bolsa, conforme avaliação e autorização das instituições aderentes, na forma prevista no termo de adesão.</p>
<p>§ 2º - Os supervisores selecionados serão responsáveis pelo acompanhamento e fiscalização das atividades de ensino-serviço do médico participante, em conjunto com o gestor do SUS no Município, e terão, no mínimo, as seguintes atribuições:</p>
<p>I - realizar visita periódica para acompanhar atividades dos médicos participantes;</p>
<p>II - estar disponível para os médicos participantes, por meio de telefone e internet;</p>
<p>III - aplicar instrumentos de avaliação presencialmente; e</p>
<p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga horária de 40 horas semanais prevista pelo Projeto para os médicos participantes, por meio de sistema de informação disponibilizado pela Coordenação do Programa.</p>
<p>Art. 6º - Esta Portaria entra em vigor na data de sua publicação.</p>
<p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function textoAdesaoHTML2($entid)
{
	global $db;
	$sql = "select * from entidade.entidade  ent
	inner join entidade.endereco ende ON ende.entid = ent.entid
	inner join territorios.municipio mun ON mun.muncod = ende.muncod
	where ent.entid = $entid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select
	usunome as nome
	from
	seguranca.usuario usu
	inner join
	maismedicos.usuarioresponsabilidade ure ON ure.usucpf = usu.usucpf
	where
	ure.entid = $entid
	and
	ure.pflcod = ".PERFIL_REITOR."
	and
				ure.rpustatus = 'A'
			and
				usu.usustatus = 'A'
			order by
				rpudata_inc desc";
	$arrReitor = $db->pegaLinha($sql);
	if(!$arrReitor){
		$sql = "SELECT
		DISTINCT fun.funid, fun.fundsc, ent.entnome as nome, ent.entid
		FROM
		entidade.funcao fun
		LEFT JOIN
		entidade.funcaoentidade fen ON fen.funid = fun.funid
		AND fen.entid IN (SELECT
		fen2.entid
		FROM
		entidade.funentassoc fea2
		LEFT JOIN
		entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
		WHERE
		fea2.entid='$entid'
		AND fun.funid = fen2.funid)
		LEFT JOIN
		entidade.entidade ent ON fen.entid = ent.entid
		WHERE
		fun.funid IN('21')";
		$arrReitor = $db->pegaLinha($sql);
	}
	?>
<div style="text-align:justify;">
<center>
<h2>PORTARIA NORMATIVA Nº 17, DE 31 DE JULHO DE 2013</h2>
<p><b>MINISTÉRIO DA EDUCAÇÃO</b></p>
<p><b>GABINETE DO MINISTRO</b></p>
</center>
<p>Dispõe sobre os procedimentos de adesão das instituições públicas estaduais e municipais de educação superior e de saúde; programas de residência em Medicina de Família e Comunidade Medicina Preventiva e Social e Clínica Médica; e de escolas de governo em saúde pública ao Programa Mais Médicos para o Brasil e dá outras providências.</p>
<p>O MINISTRO DE ESTADO DA EDUCAÇÃO no uso da atribuição que lhe confere o art. 87, inciso II da Constituição Federal, e tendo em vista o disposto na Medida Provisória no 621, de 8 de julho de 2013, bem como na Portaria Interministerial MS/MEC no 1.369, de 8 de julho de 2013, resolve:</p>
<p>Art. 1º Poderão aderir ao Programa Mais Médicos para o Brasil:</p>
<p>I - as instituições públicas estaduais e municipais de educação superior, que ofereçam curso de Medicina gratuitamente;</p>
<p>II - os programas de residência em Medicina de Família e Comunidade, de Medicina Preventiva e Social e Clínica Médica que estejam devidamente credenciados pela Comissão Nacional de Residência Médica (CNRM);</p>
<p>III - as escolas de governo em saúde pública, que possuam no mínimo um programa residência médica ou de pós-graduação na área de saúde coletiva ou afins; e</p>
<p>IV - as secretarias municipais e estaduais de saúde que tenham ao menos um programa de residência médica vinculado às mesmas.</p>
<p>§1º As instituições, escolas e programas de residência interessados em aderir ao Programa Mais Médicos para o Brasil deverão apresentar termo de pré-adesão, conforme o modelo do Anexo I desta Portaria, no período de 05 a 12 de agosto de 2013, ao Ministério da Educação.</p>
<p>§2º As instituições, escolas e programas de residência deverão indicar, no momento da pré-adesão, um tutor acadêmico responsável pelas atividades e, no máximo, três tutores acadêmicos para fins de cadastro de reserva, que atendam aos requisitos da Portaria Interministerial MS/MEC nº 1.369, de 8 de julho de 2013 e desta Portaria.</p>
<p>§3º As instituições, escolas e programas de residência deverão enviar o termo de pré-adesão devidamente assinado pela autoridade local responsável pela instituição, escola ou programa de residência, e digitalizado, até as 23:59 hs do dia 12/08/2013, para o endereço <?php echo $_SESSION['email_sistema']; ?>.</p>
<p>§4º As instituições, escolas e programas de residência deverão, no prazo estipulado no parágrafo anterior, enviar através de postagem pelo correio cópia impressa e assinada do termo de pré-adesão, com aviso de recebimento (AR), para o endereço Ministério da Educação, Edifício Sede, Bloco L, Esplanada dos Ministérios, 3º Andar, Sala 303 CEP: 70047-900.</p>
<p>§5º No momento da pré-adesão instituições, escolas e programas de residência deverão indicar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas.</p>
<p>Art. 2º O Ministério da Educação decidirá sobre a validação do termo de pré-adesão das instituições, escolas e programas de residência que atenderem aos requisitos previstos no art. 1º desta Portaria, observadas as necessidades do Programa Mais Médicos para o Brasil.</p>
<p>§1º Serão selecionadas instituições, escolas e programas de residência apenas nas unidades da federação onde não houver adesão de instituição federal de educação superior, nos termos da Portaria Normativa nº 14, de 10 de julho de 2013.</p>
<p>§2º Em caso de manifestação de interesse de mais de uma instituição, escola ou programa de residência por unidade da federação, será dada preferência àquele sediado na capital.</p>
<p>§3º Caso persista o empate, será selecionado aquele que ofertar programa de residência médica ou especialização na área de saúde coletiva, medicina de família e comunidade ou áreas afins.</p>
<p>§4º Se ainda persistir o empate, será selecionado aquele programa de residência vinculado a instituições estaduais e municipais de educação superior, de acordo com critérios do art. 1º.</p>
<p>§5º As instituições, escolas e programas de residência não selecionados neste primeiro momento de pré-adesão irão compor um banco de entidades supervisoras, que poderão ser mobilizadas a qualquer momento para composição do quadro de tutoria do Programa Mais Médicos para o Brasil.</p>
<p>Art. 3º As instituições, escolas e programas de residência que tiverem seus termos de pré-adesão validados pelo Ministério da Educação deverão firmar termo de adesão no prazo máximo de 10 (dez) dias após a divulgação das entidades selecionadas.</p>
<p>Parágrafo único. O termo de adesão estará disponível para assinatura das instituições, escolas e programas de residência selecionados por meio de comunicação via endereço eletrônico e expediente de ofício do MEC a ser enviado e conterá, no mínimo, as seguintes obrigações para a entidade:</p>
<p>I - atuar em cooperação com os entes federativos, as Coordenações Estaduais do Programa e organismos internacionais, no âmbito de sua competência, para execução do Programa Mais Médicos para o Brasil;</p></p>
<p>II - coordenar o acompanhamento acadêmico do Programa;</p>
<p>III - ratificar a unidade responsável pela avaliação e autorização de pagamento das bolsas de tutoria e supervisão acadêmicas, indicada no termo de pré-adesão;</p>
<p>IV - definir mecanismo de avaliação e autorização de pagamento das bolsas de tutoria e supervisão;</p>
<p>V - ratificar a indicação dos tutores acadêmicos do Programa, feita no termo de pré-adesão;</p>
<p>VI - definir critérios e mecanismo de seleção de supervisores;</p>
<p>VII - realizar seleção dos supervisores do Programa;</p>
<p>VIII - monitorar e acompanhar as atividades dos supervisores e tutores acadêmicos no âmbito do Programa;</p>
<p>IX - ofertar os módulos de acolhimento e avaliação aos médicos intercambistas; e</p>
<p>Art. 4º Os tutores acadêmicos serão selecionados pela instituições, escolas e programas de residência entre os docentes da área médica, preferencialmente vinculados à área de saúde coletiva ou correlata, à área de medicina de família e comunidade, ou à área de clínica médica.</p>
<p>§1º Os tutores acadêmicos perceberão bolsa-tutoria, na forma prevista no termo de adesão.</p>
<p>§2º Os tutores acadêmicos serão responsáveis pela orientação acadêmica e pelo planejamento das atividades do supervisor, trabalhando em parceria com as Coordenações Estaduais do Programa, e tendo, no mínimo, as seguintes atribuições:</p>
<p>I - coordenar as atividades acadêmicas da integração ensinoserviço, atuando em cooperação com os supervisores e os gestores do SUS;</p>
<p>II - indicar, em plano de trabalho, as atividades a serem executadas pelos médicos participantes e supervisores, bem como a metodologia de acompanhamento e avaliação;</p>
<p>III - monitorar o processo de acompanhamento e avaliação a ser executado pelos supervisores, garantindo sua continuidade;</p>
<p>IV - integrar as atividades do curso de especialização às atividades de integração ensino-serviço;</p>
<p>V - relatar à instituição ou escola à qual esteja vinculado a ocorrência de situações nas quais seja necessária a adoção de providência pela instituição; e</p>
<p>VI - apresentar relatórios periódicos da execução de suas atividades no Programa à instituição à qual esteja vinculado e à Coordenação do Programa.</p>
<p>Art. 5º Os supervisores serão selecionados entre profissionais médicos por meio de edital conforme critérios e mecanismos estabelecidos pelas instituições, escolas e programas de residência aderente e validados pela Coordenação Estadual do Programa Mais Médicos para o Brasil.</p>
<p>§1º Os supervisores selecionados perceberão bolsa, conforme avaliação e autorização das instituições, escolas e programas de residência aderentes, na forma prevista no termo de adesão.</p>
<p>§2º Os supervisores selecionados serão responsáveis pelo acompanhamento e fiscalização das atividades de ensino-serviço do médico participante, em conjunto com o gestor do SUS no Município, e terão, no mínimo, as seguintes atribuições:</p>
<p>I - realizar visita periódica para acompanhar atividades dos médicos participantes;</p>
<p>II - estar disponível para os médicos participantes, por meio de telefone e internet;</p>
<p>III - aplicar instrumentos de avaliação presencialmente; e</p>
<p>IV - acompanhar e fiscalizar, em conjunto com o gestor do SUS, o cumprimento da carga horária de 40 horas semanais prevista pelo Programa para os médicos participantes, por meio de sistema de informação disponibilizado pela Coordenação do Programa.</p>
<p>Art. 6º Os prazos desta Portaria poderão ser alterados mediante ato do Secretário de Educação Superior.</p>
<p>Art. 7º Esta Portaria entra em vigor na data de sua publicação.</p>
<p>ALOIZIO MERCADANTE OLIVA</p>
<?php
}

function documentoAdesao($uniid,$valida_eletronicamente = false)
{
	global $db;
	$sql = "select
				usu.usucpf,
				usu.usunome,
				usu.ususexo,
				uni.*,
				mun.mundescricao,
				mun.estuf,
				tpu.tpudsc
			from
				maismedicos.universidade uni
			inner join
				maismedicos.tipouniversidade tpu ON tpu.tpuid = uni.tpuid
			inner join
				maismedicos.usuarioresponsabilidade ure ON ure.uniid = uni.uniid
			inner join
				seguranca.usuario usu ON usu.usucpf = ure.usucpf
			inner join
				territorios.municipio mun ON mun.muncod = uni.muncod
			where
				ure.pflcod = '".PERFIL_REITOR."'
			and
				rpustatus = 'A'
			and
				uni.uniid = $uniid";
	$arrDados = $db->pegaLinha($sql);
	//dbg($arrDados);
?>
<div style="text-align:justify;font-size:11px">
<center>
<table width="100%" >
	<tr>
		<td><img src="../imagens/logo_brasil_mais_medicos.png" /></td>
		<td width="50%" ></td>
		<td><img src="../imagens/logo_mais_medicos.png" /></td>
	</tr>
</table>
<div><img src="../imagens/brasao_mais_medicos.JPG" /></div>

<h2>MINISTÉRIO DA EDUCAÇÃO</h2>
<h2>SECRETARIA DE EDUCAÇÃO SUPERIOR</h2>
<h2>PROGRAMA MAIS MÉDICOS</h2>
<h2>TERMO DE ADESÃO AO PROGRAMA MAIS MÉDICOS</h2>
</center>

<p>TERMO DE ADESÃO E COMPROMISSO QUE ENTRE SI CELEBRAM O MINISTÉRIO DA EDUCAÇÃO E <?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?> PARA ADESÃO À SUPERVISÃO ACADÊMICA DO PROGRAMA MAIS MÉDICOS. O MINISTÉRIO DA EDUCAÇÃO, CNPJ  00.394.445/0003-65, neste ato representado por Jesualdo Pereira Farias, Secretário de Educação Superior, com endereço na Esplanada dos Ministérios, Bloco "L", 3º andar, sala 300 - CEP 70.047-900, Brasília (DF), e <?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?>, com sede na cidade de <?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?>, inscrita no CNPJ/MF sob o nº <?php echo mascara_global_maismedicos_tela($arrDados['unicnpj'],"##.###.###/####-##") ?>, doravante intitulada INSTITUIÇÃO SUPERVISORA, neste ato representado por seu Reitor(a) <?php echo $arrDados['usunome'] ?>, nos termos da Lei nº 12.871, de 22 de outubro de 2013, da Portaria Interministerial nº 1.369/MS/MEC, de 8 de julho de 2013 e da Portaria Interministerial nº 2.087/MS/MEC, de 1º de setembro de 2011 resolvem celebrar o presente Termo de Adesão e Compromisso para adesão à  Supervisão Acadêmica ao Programa Mais Médicos, mediante as cláusulas e condições seguintes:</p>

<p>CLÁUSULA PRIMEIRA - DO OBJETO</p>
<p>O presente Termo de Adesão tem por objeto viabilizar a tutoria e supervisão acadêmica a médicos formados em instituições de educação superior brasileiras ou com diploma revalidado no Brasil e médicos formados em instituições de educação superior estrangeiras, por meio de intercâmbio médico internacional inscritos no Projeto Mais Médicos para o Brasil, nos termos da Lei 12.871/2013 e na Portaria Interministerial MS/MEC nº 1.369/2013.</p>
<p>CLÁUSULA SEGUNDA - DAS OBRIGAÇÕES</p>
<p>Para consecução do objeto do presente Termo a INSTITUIÇÃO SUPERVISORA deverá efetuar procedimento de adesão por meio do Sistema Integrado de Monitoramento do Ministério da Educação (SIMEC), com as credenciais do dirigente máximo da INSTITUIÇÃO SUPERVISORA e compromete-se a assumir as seguintes obrigações:</p>
<p>I - atuar em cooperação com os entes federativos, as Coordenações Estaduais do Projeto e instituições internacionais, no âmbito de sua competência, para execução do Projeto Mais Médicos para o Brasil;</p>
<p>II - seguir as orientações e diretrizes do Ministério da Educação, bem como receber representantes do mesmo nos espaços de execução da supervisão;</p>
<p>III - coordenar o acompanhamento acadêmico do Programa Mais Médicos;</p>
<p>IV - ratificar a indicação dos tutores acadêmicos do Projeto Mais Médicos para o Brasil;</p>
<p>V - acompanhar os mecanismos de avaliação e autorização de pagamento das bolsas de tutoria e supervisão com o núcleo gestor do Projeto Mais Médicos para o Brasil;</p>
<p>VI - definir critérios e mecanismo de seleção de supervisores, não ferindo regulamentação vigente do Projeto Mais Médicos para o Brasil;</p>
<p>VII - realizar seleção dos primeiros supervisores do Projeto, no período de 30 (trinta) dias, a contar da data de assinatura do Termo de Adesão. </p>
<p>VIII - estabelecer calendário de fluxo contínuo para seleção de novos supervisores, conforme as necessidades expressas pela Diretoria de Desenvolvimento da Educação em Saúde DDES/SESu/MEC;</p>
<p>IX - monitorar e acompanhar as atividades dos tutores e supervisores acadêmicos e médicos participantes do Projeto;</p>
<p>X - Acompanhar o pagamento das bolsas de tutores e supervisores acadêmicos;</p>
<p>XI - Apoiar a execução dos Módulos de Acolhimento e Avaliação aos médicos intercambistas no local indicado pela Coordenação Nacional do Projeto;</p>
<p>XII - ofertar atividades de pesquisa, ensino e extensão aos médicos participantes do Projeto Mais Médicos para o Brasil; e</p>
<p>XIII - As instituições interessadas também deverão cadastrar 2 (dois) Tutores Acadêmicos, dentre os profissionais com perfil docente da área médica, vinculado à mesma, e preferencialmente atuante em alguma das seguintes áreas de conhecimento: Saúde Coletiva, Medicina de Família e Comunidade, Clínica Médica, Pediatria, ou áreas afins.</p>
</p>XIV - Um dos tutores será será cadatrado para fins de cadastro reserva, atendendo aos requisitos da Portaria Interministerial MS/MEC nº 1.369, de 08 de julho de 2013, conforme procedimentos estabelecidos pela Diretoria de Desenvolvimento da Educação em Saúde, da Secretaria de Educação Superior do Ministério da Educação.</p>
<p>CLÁUSULA TERCEIRA - DOS TUTORES ACADÊMICOS</p>
<p>O Tutor Acadêmico será indicado pela INSTITUIÇÃO SUPERVISORA dentre os profissionais com perfil docente da área médica, vinculado à mesma, e preferencialmente atuante em alguma das seguintes áreas de conhecimento: Saúde Coletiva, Medicina de Família e Comunidade, Clínica Médica, Pediatria, ou áreas afins.</p>
<p>SUBCLÁUSULA 3.1</p>
<p>O Tutor Acadêmico é responsável pela orientação acadêmica e pelo planejamento das atividades do supervisor, observadas as orientações gerais da Diretoria de Desenvolvimento da Educação em Saúde DDES/SESu/MEC.</p>
<p>SUBCLÁUSULA 3.2</p>
<p>O(s) Tutor(es) do cadastro reserva poderá(ão) ser convocado(s), de acordo com o número de médicos selecionados para o Projeto Mais Médicos para o Brasil, observada a proporção de supervisores por Tutor definida pela Diretoria de Desenvolvimento da Educação em Saúde DDES/SESu/MEC.</p>
<p>SUBCLÁUSULA 3.3</p>
<p>As Instituições Supervisoras deverão garantir a dispensa dos professores que atuarão como Tutores Acadêmicos, de atividades perante as mesmas, para o desempenho das atividades de tutoria de forma adequada, sem prejuízos de qualquer ordem para os mesmos.</p>
<p>SUBCLÁUSULA 3.4</p>
<p>As Instituições Supervisoras deverão computar a atividades de tutoria em seu plano institucional sem prejuízos para o docente designado.</p>
<p>CLÁUSULA QUARTA - DA BOLSA-TUTORIA</p>
<p>Para o desenvolvimento de suas atividades o Tutor Acadêmico receberá bolsa-tutoria no valor de R$ 5.000,00 (cinco mil reais) mensais, mediante cumprimento das respectivas atribuições  durante o prazo de vinculação ao Projeto Mais Médicos para o Brasil.</p>
<p>CLÁUSULA QUINTA - DAS ATRIBUIÇÕES DO TUTOR ACADÊMICO</p>
<p>O tutor Acadêmico deverá seguir atribuições estabelecidas na regulamentação vigente do Projeto conforme orientação da Coordenação Nacional.</p>
<p>CLÁUSULA SEXTA - DA SELEÇÃO DE SUPERVISORES</p>
<p>Os supervisores serão selecionados pela INSTITUIÇÃO dentre profissionais médicos com perfil docente da área médica, vinculado à mesma, e preferencialmente atuante em alguma das seguintes áreas de conhecimento: Saúde Coletiva, Medicina de Família e Comunidade, Clínica Médica, Pediatria, ou áreas afins.</p>
<p>SUBCLÁUSULA 6.1</p>
<p>Os supervisores selecionados serão responsáveis pelo acompanhamento das atividades de integração ensino-serviço do médico participante, em conjunto com o gestor do SUS no Município ou Coordenador de Distrito Sanitário Especial Indígena;</p>
<p>CLÁUSULA SÉTIMA - DA BOLSA SUPERVISÃO</p>
<p>Os supervisores selecionados perceberão bolsa no valor de R$ 4.000,00 (quatro mil reais) mensais, mediante cumprimento das atribuições de supervisão acadêmica e durante o prazo de vinculação ao Projeto Mais Médicos para o Brasil.</p>
<p>CLÁUSULA OITAVA - DAS ATRIBUIÇÕES DO SUPERVISOR</p>
<p>O tutor Acadêmico deverá seguir atribuições estabelecidas na regulamentação vigente do Projeto conforme orientação da Coordenação Nacional.</p>
<p>CLÁUSULA NONA - DA VIGÊNCIA</p>
<p>O presente TERMO DE ADESÃO terá vigência de 3 (três) anos, podendo ser prorrogado por igual período, respeitando o tempo de vigência do Projeto Mais Médicos para o Brasil.</p>
<p>CLÁUSULA DÉCIMA - DISPOSIÇÕES FINAIS</p>
<p>As INSTITUIÇÕES SUPERVISORAS com adesão ao Projeto Mais Médicos para o Brasil, que manifestarem formalmente sua impossibilidade de atenderem aos determinantes deste Termo, deverão encaminhar ofício à DDES/SESu/MEC com o prazo de 30 (trinta) dias de antecedência, para que se proceda seu desligamento perante o sistema SIMEC.</p>
<p>Compete à SESu/MEC decidir sobre eventuais casos omissos.</p>
<p>&nbsp;</p>
<center>
<p>Brasília, <?php echo date('d');?> de <?php echo mes_extenso(date('m'))?> de <?php echo date('Y'); ?>.</p>
<p>&nbsp;</p>
<p>___________________________</p>
<p><?php echo $arrDados['usunome'] ?></p>
<p><?php echo retornaNome($arrDados['tpuid'],$arrDados['ususexo']) ?> de <?php echo $arrDados['tpudsc']?></p>
<p><?php echo $arrDados['uninome'] ?><?php echo $arrDados['unisigla'] ? " - ".$arrDados['unisigla'] : "" ?></p>
</center>

<?php if ($valida_eletronicamente): ?>
	<p><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO</b></p>
	<p><b>Validado por <?php echo $_SESSION['usunome'] ?> - CPF: <?php echo mascara_global_maismedicos_tela($_SESSION['usucpf'],"###.###.###-##") ?> em <?php echo date("d/m/Y H:m:s")?>.</b></p>
<?php endif; ?> 
</center>
<?php 

}

function retornaMesMaisMedicos($mes){

	switch ($mes){
		case '01' :
			return 'Janeiro';
			break;
		case '02' :
			return 'Fevereiro';
			break;
		case '03' :
			return 'Março';
			break;
		case '04' :
			return 'Abril';
			break;
		case '05' :
			return 'Maio';
			break;
		case '06' :
			return 'Junho';
			break;
		case '07' :
			return 'Julho';
			break;
		case '08' :
			return 'Agosto';
			break;
		case '09' :
			return 'Setembro';
			break;
		case '10' :
			return 'Outubro';
			break;
		case '11' :
			return 'Novembro';
			break;
		case '12' :
			return 'Dezembro';
			break;
	}
}

function recuperaArquivo($arqid)
{
	global $db;
	$sql = "select * from public.arquivo where arqid = $arqid";
	return $db->pegaLinha($sql);
}

function mascara_global_maismedicos_tela($string,$mascara)
{
	$string = str_replace(" ","",$string);
	for($i=0;$i<strlen($string);$i++)
	{
		$mascara[strpos($mascara,"#")] = $string[$i];
	}
	return $mascara;
}

function comboBanco($bncid = null,$habilitado = "S")
{
	global $db;

	$bncid = $_POST['bncid'] ? $_POST['bncid'] : $bncid;

	$sql = "select
				bncid as codigo,
				bncdsc as descricao
			from
				maismedicos.banco
			where
				bncstatus = 'A'
			order by
				bncdsc";
	$db->monta_combo("bncid", $sql, $habilitado, "Selecione...", '', '', '', '', 'S','bncid');
}

function cabecalhoUniversidade($uniid)
{
	global $db;
	$sql = "select * from maismedicos.universidade uni left join territorios.municipio mun ON mun.muncod = uni.muncod where uni.uniid = $uniid";
	$arrDados = $db->pegaLinha($sql); ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita" width='25%' >Instituição Supervisora</td>
			<td>
				<?php echo $arrDados['uninome'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >CNPJ</td>
			<td>
				<?php echo formatar_cnpj($arrDados['unicnpj']) ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Localização</td>
			<td>
				<?php echo $arrDados['mundescricao'] ?> - <?php echo $arrDados['estuf'] ?>
			</td>
		</tr>
	</table>
	<?php 
	
}

function importarDadosAcademico()
{
	global $db;
	
	$sql = "select * from academico.maismedicos"; //Espelho de Produção
	
	$arrDados = $db->carregar($sql);
	
	foreach($arrDados as $arrD){
		$arrCamposNaoInseridos = array();
		foreach($arrD as $chave => $valor){
			$arrCamposNaoInseridos[] = "msmid";
			//$arrCamposNaoInseridos[] = "arqidtermo"; //Só funciona no mesmo ambiente
			//$arrCamposNaoInseridos[] = "arqid"; //Só funciona no mesmo ambiente
			if(!in_array($chave,$arrCamposNaoInseridos)){				
				if($valor){
					$arrCampos[] = $chave;
					$arrValores[] = "'$valor'";
				}
			}
		}
		$sqlI.="insert into maismedicos.maismedicos (".implode(",",$arrCampos).") values (".implode(",",$arrValores).");<br/>";
		$arrCampos = array();
		$arrValores = array();
	}
	dbg($sqlI,1);
	
	//Tutores
	$sql = "select
			ent.entid as entidade,
			*
		from
			academico.maismedicosresponsabilidade tut
		inner join
			entidade.entidade ent ON ent.entid = tut.entidresponsavel
		left join
			entidade.endereco ende ON ende.entid = ent.entid";
	
	$arrDados = $db->carregar($sql);
	
	foreach($arrDados as $arrD){
	
		$arrCampos['entid'] = $arrD['entidunidade'];
		$arrCampos['tuttipo'] = "T";
		$arrCampos['tutcpf'] = $arrD['entnumcpfcnpj'];
		$arrCampos['tutnome'] = $arrD['entnome'];
		$arrCampos['tutdatanascimento'] = $arrD['entdatanasc'] ? $arrD['entdatanasc'] : null;
		$arrCampos['tutemail'] = $arrD['entemail'];
		$arrCampos['tuttelefone'] = "(".trim($arrD['entnumdddresidencial']).")".$arrD['entnumresidencial'];
		$arrCampos['tutcep'] = $arrD['endcep'];
		$arrCampos['tutlogradouro'] = $arrD['endlog'];
		$arrCampos['tutnumero'] = $arrD['endnum'];
		$arrCampos['tutcomplemento'] = $arrD['endcom'];
		$arrCampos['tutbairro'] = $arrD['endbai'];
		$arrCampos['estuf'] = $arrD['estuf'];
		$arrCampos['muncod'] = $arrD['muncod'];
	
		foreach($arrCampos as $chave => $valor){
			$arrColunas[] = $chave;
			if($valor){
				$arrValores[] = "'$valor'";
			}else{
				$arrValores[] = "null";
			}
		}
	
		$sqlI2.="insert into maismedicos.tutor (".implode(",",$arrColunas).") values (".implode(",",$arrValores).");<br/>";
		$arrCampos = array();
		$arrColunas = array();
		$arrValores = array();
	}
	
	$sql = "select distinct
			ure.*
		from
			academico.maismedicosresponsabilidade tut
		inner join
			entidade.entidade ent ON ent.entid = tut.entidresponsavel
		inner join
			academico.usuarioresponsabilidade ure ON ure.entid = tut.entidunidade
		where
			rpustatus = 'A'
		and
			pflcod = 526";
	
	$arrDados = $db->carregar($sql);
	
	
	foreach($arrDados as $arrD){
	
		$arrCampos['pflcod'] = "947";
		$arrCampos['usucpf'] = $arrD['usucpf'];
		$arrCampos['entid'] = $arrD['entid'];
	
		foreach($arrCampos as $chave => $valor){
			$arrColunas[] = $chave;
			if($valor){
				$arrValores[] = "'$valor'";
			}else{
				$arrValores[] = "null";
			}
		}
	
		$sqlI3.="insert into maismedicos.usuarioresponsabilidade (".implode(",",$arrColunas).") values (".implode(",",$arrValores).");
		insert into seguranca.usuario_sistema (usucpf,sisid,suscod,susstatus,pflcod) values ('{$arrD['usucpf']}','168','A','A','947');
		insert into seguranca.perfilusuario (usucpf,pflcod) values ('{$arrD['usucpf']}','947'); <br/>";
		$arrCampos = array();
		$arrColunas = array();
		$arrValores = array();
	}
		dbg($sqlI3);
	
	die;
}

function preencherComZero($valor,$tamanho)
{
	$tamanho_string = strlen($valor);
	if($tamanho_string < $tamanho){
		$i = $tamanho_string;
		for($i;$i<$tamanho;$i++){
			$valor = "0".$valor;
		}
	}
	return $valor;
}

function preencherComEspacoVazio($valor,$tamanho,$direita = false)
{
	$tamanho_string = strlen($valor);
	if($tamanho_string < $tamanho){
		$i = $tamanho_string;
		for($i;$i<$tamanho;$i++){
			if($direita){
				$valor = $valor." ";
			}else{
				$valor = " ".$valor;
			}			
		}
	}
	return $valor;
}

function modulo11($num, $base=9, $r=0)
{
	/**
	 *   Autor:
	 *           Pablo Costa <hide@address.com>
	 *
	 *
	 *   Entrada:
	 *     $num: string numérica para a qual se deseja calcularo digito verificador;
	 *     $base: valor maximo de multiplicacao [2-$base]
	 *     $r: quando especificado um devolve somente o resto
	 *
	 *   Saída:
	 *     Retorna o Digito verificador.
	 *
	 */
	$soma = 0;
	$fator = 2;

	/* Separacao dos numeros */
	for ($i = strlen($num); $i > 0; $i--) {
		// pega cada numero isoladamente
		$numeros[$i] = substr($num,$i-1,1);
		// Efetua multiplicacao do numero pelo falor
		$parcial[$i] = $numeros[$i] * $fator;
		// Soma dos digitos
		$soma += $parcial[$i];
		if ($fator == $base) {
			// restaura fator de multiplicacao para 2
			$fator = 1;
		}
		$fator++;
	}

	/* Calculo do modulo 11 */
	if ($r == 0) {
		$soma *= 10;
		$digito = $soma % 11;
		if ($digito == 10) {
			$digito = 0;
		}
		return $digito;
	} elseif ($r == 1){
		$resto = $soma % 11;
		return $resto;
	}
}

function removeCaracteres($string,$arrCaracteres = array())
{
	return str_replace($arrCaracteres,"",$string);
}

function removeAcentosRemessa($str){
	$str = trim($str);
	$str = strtr($str,"¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ!@#%&*()[]{}+=?",
			"YuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy_______________");
	$str = str_replace("..","",str_replace("/","",str_replace("\\","",str_replace("\$","",$str))));
	return $str;
}

function verificaTransmissaoLoteDataAtual()
{
	global $db;
	$sql = "select count(*) from maismedicos.remessacabecalho where to_char(dt_hora,'DD/MM/YYYY') = to_char(now(),'DD/MM/YYYY')";
	return ($db->pegaUm($sql)+1);
}

function verificaTransmissaoLote()
{
	global $db;
	$sql = "select count(*) from maismedicos.remessacabecalho";
	return ($db->pegaUm($sql)+1);
}

function retornaNome($tpuid,$sexo = "M")
{
	switch($tpuid)
	{
		case TPUID_UNIVERSIDADE;
			$nome = "Reitor".($sexo == "F" ? "a" : "");
			break;
		case TPUID_INSTITUICAO;
			$nome = "Dirigente Máxim".($sexo == "F" ? "a" : "o");
			break;
		case TPUID_ESCOLA;
			$nome = "Dirigente Máxim".($sexo == "F" ? "a" : "o");
			break;
		case TPUID_PROGRAMA;
			$nome = "Supervisor".($sexo == "F" ? "a" : "");
			break;
		case TPUID_COMISSAO;
			$nome = "Presidente";
			break;
		default:
			$nome = "Reitor".($sexo == "F" ? "a" : "");
	}
	return $nome;
}

function filtrarDemandaMaisMedicos()
{
	global $db;
	
	if($_POST['uf_campo_flag'] == "1" && $_POST['uf'][0]){
		$arrWhere[] = "est.estuf in ('".implode("','",$_POST['uf'])."')";
	}
	if($_POST['uniid_campo_flag'] == "1" && $_POST['uniid'][0]){
		$arrWhere[] = "uni.uniid in ('".implode("','",$_POST['uniid'])."')";
	}
	if($_POST['tpmid_campo_flag'] == "1" && $_POST['tpmid'][0]){
		$arrWhere[] = "tpm.tpmid in ('".implode("','",$_POST['tpmid'])."')";
	}
	if($_POST['muncod_campo_flag'] == "1" && $_POST['muncod'][0]){
		$arrWhere[] = "mun.muncod in ('".implode("','",$_POST['muncod'])."')";
	}
	
	$sql = "select 
				est.estuf,
				est.estdescricao as estado,
				tpm.tpmid,
				tpm.tpmdsc as regiaosaude,
				mun.mundescricao as municipio,
				mun.muncod as ibge,
				count(mdcid) as qtde_medicos,
				uni.uniid,
				case when uni.unisigla is not null
					then uni.unisigla || ' - ' || uni.uninome
					else uni.uninome
				end as uninome,
				(select count(distinct tut.tutid) from maismedicos.tutor tut where tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true) as qtde_tutores,
				(select count(distinct sup.tutid) from maismedicos.tutor sup where sup.uniid = uni.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true) as qtde_supervisores
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			inner join
				territorios.municipio mun ON mun.muncod = unm.muncod
			inner join
				territorios.estado est ON est.estuf = mun.estuf
			inner join
				territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
			inner join
				territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid
			inner join
				maismedicos.medico med ON med.muncod = mun.muncod
			where
				tpm.gtmid = 1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by
				est.estuf,est.estdescricao,tpm.tpmid,tpm.tpmdsc,mun.muncod,mun.mundescricao,uni.uninome,uni.unisigla,uni.uniid
			order by
				est.estuf,tpm.tpmdsc,mun.mundescricao";
	
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado)
		{
			$arrEstado[$dado['estuf']]['estuf'] = $dado['estuf'];
			$arrEstado[$dado['estuf']]['estado'] = $dado['estado'];
			$arrEstado[$dado['estuf']]['regiaoes_saude'][$dado['tpmid']]['regiaosaude'] = $dado['regiaosaude'];
			$arrEstado[$dado['estuf']]['regiaoes_saude'][$dado['tpmid']]['total_medicos'] += $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['municipios'][$dado['ibge']] = $dado['municipio'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['uninome'] = $dado['uninome'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_tutores'] = $dado['qtde_tutores'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_supervisores'] = $dado['qtde_supervisores'];
			$arrEstado[$dado['estuf']]['universidades'][$dado['uniid']]['qtde_medicos'] += $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['municipios_por_regiao_saude'][$dado['tpmid']][$dado['ibge']]['municipio'] = $dado['municipio'];
			$arrEstado[$dado['estuf']]['municipios_por_regiao_saude'][$dado['tpmid']][$dado['ibge']]['qtde_medicos'] = $dado['qtde_medicos'];
			$arrEstado[$dado['estuf']]['total_medicos'] += $dado['qtde_medicos'];
		}

		foreach($arrEstado as $estuf => $arrD){

			$supervisores = recuperaTotalSupervisoresPorUf($arrD['estuf']);
			$medicos = $arrD['total_medicos'];
			
			$proporcao1 = $_POST['proporcao1'] ? $_POST['proporcao1'] : 1;
			$proporcao2 = $_POST['proporcao2'] ? $_POST['proporcao2'] : 10;
			/*
			 * Existem 7 supervisores
			* Existem 91 médicos
			*
			* Fórmula 1 : para cada 98 médico, deve haver 1 , portanto, sobram 6 supervisores
			* Fórmula 2 : para cada 10 médico, deve haver 1 médico, portanto, faltam 3 supervisores
			*/
			$deficit = $proporcao2*$supervisores;
			$deficit = $medicos - $deficit;
			$deficit = $deficit/$proporcao2;
			if($deficit < 0){
				$deficit = $deficit * (-1);
				$deficit = floor($deficit);
				$sinal = "+";
			}elseif($deficit == 0){
				$sinal = "";
			}else{
				$sinal = "-";
				$deficit = ceil($deficit);
			}
			$style_siplay = "";
			if($sinal == "-"){
				if(strlen($_POST['deficil_inicio']) > 0 && !$_POST['deficil_fim']){
					if($deficit == $_POST['deficil_inicio']){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['deficil_inicio'])> 0 && $_POST['deficil_fim']){
					if($_POST['deficil_inicio'] <= $deficit && $_POST['deficil_fim'] >= $deficit){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['excedente_inicio'])> 0 && strlen($_POST['deficil_inicio']) == 0){
					$style_siplay = "none";
				}
			}else{
				if(strlen($_POST['excedente_inicio']) && !$_POST['excedente_fim']){
					if($deficit == $_POST['excedente_inicio']){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['excedente_inicio'])> 0 && $_POST['excedente_fim']){
					if($_POST['excedente_inicio'] <= $deficit && $_POST['excedente_fim'] >= $deficit){
						$style_siplay = "";
					}else{
						$style_siplay = "none";
					}
				}
				if(strlen($_POST['deficil_inicio'])> 0 && strlen($_POST['excedente_inicio']) == 0 ){
					$style_siplay = "none";
				}
			}
			 ?>
			 <?php if($style_siplay == ""): ?>
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
					<tr>
						<td colspan="2" bgcolor="#DCDCDC" align="center">
							<b><?php echo $arrD['estuf'] ?> - <?php echo $arrD['estado'] ?></b> 
						</td>
					</tr>
					<tr>
						<td width="25%" class="subtituloDireita" >Qtde. Tutores Validados:</td>
						<td><?php echo number_format(recuperaTotalTutoresPorUf($arrD['estuf']),0,",",".") ?></td>
					</tr>
						<td class="subtituloDireita" >Qtde. Supervisores Validados:</td>
						<td><?php echo number_format($supervisores,0,",",".") ?></td>
					</tr>
					</tr>
						<td class="subtituloDireita" >Qtde. de Médicos Ativos na Área de Atuação:</td>
						<td><?php echo number_format($medicos,0,",",".") ?></td>
					</tr>
					</tr>
						<td class="subtituloDireita" ><?php echo $sinal == "-" || $deficit == "0" ? "Déficit" : "Excedente"?> de Supervisores na Área de Atuação:</td>
						<td>
							<?php echo ($deficit == "0" ? "" : $sinal).$deficit ?>
						</td>
					</tr>
					<?php foreach($arrEstado[$arrD['estuf']]['universidades'] as $uniid => $uni): ?>
						<?php 
						$deficit_uni = $proporcao2*$uni['qtde_supervisores'];
						$deficit_uni = $uni['qtde_medicos'] - $deficit_uni;
						$deficit_uni = $deficit_uni/$proporcao2;
						if($deficit_uni < 0){
							$deficit_uni = $deficit_uni * (-1);
							$deficit_uni = floor($deficit_uni);
							$sinal_uni = "+";
						}elseif($deficit_uni == 0){
							$sinal_uni = "";
						}else{
							$sinal_uni = "-";
							$deficit_uni = ceil($deficit_uni);
						}
						?>
						<tr bgcolor="#d5d5d5">
							<td class="subtituloDireita" ><b>Insituição Supervisora:</b></td>
							<td><?php echo $uni['uninome'] ?></td>
						</tr>
						<tr>
							<td width="25%" class="subtituloDireita" >Qtde. Tutores Validados:</td>
							<td><?php echo number_format($uni['qtde_tutores'],0,",",".") ?></td>
						</tr>
							<td class="subtituloDireita" >Qtde. Supervisores Validados:</td>
							<td><?php echo number_format($uni['qtde_supervisores'],0,",",".") ?></td>
						</tr>
						</tr>
							<td class="subtituloDireita" >Qtde. de Médicos Ativos na Área de Atuação:</td>
							<td><?php echo number_format($uni['qtde_medicos'],0,",",".") ?></td>
						</tr>
						</tr>
							<td class="subtituloDireita" ><?php echo $sinal_uni == "-" || $deficit_uni == "0" ? "Déficit" : "Excedente"?> de Supervisores na Área de Atuação:</td>
							<td>
								<?php echo ($deficit_uni == "0" ? "" : $sinal_uni).$deficit_uni ?>
							</td>
						</tr>
					<?php endforeach; ?>
				</table>
				<table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" width="95%" >
					<tr>
						<th>Região da Saúde</th>
						<th>IBGE</th>
						<th>Município</th>
						<th>Qtde. Médicos</th>
					</tr>
					<?php if($arrEstado[$arrD['estuf']]['regiaoes_saude']): ?>
						<?php $n=0; foreach($arrEstado[$arrD['estuf']]['regiaoes_saude'] as $tpmid => $arrReg): ?>
							<?php $cor = $n%2 ? "#fff" : ""?>
							<tr onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" bgcolor="<?php echo $cor ?>" >
								<td><img class="link" src="../imagens/mais.gif" id="img_reg_<?php echo $tpmid ?>" onclick="abreMunicipios('<?php echo $tpmid ?>')"  /> <?php echo $arrReg['regiaosaude'] ?></td>
								<td>-</td>
								<td><?php echo number_format(count($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid]),0,",",".").(count($arrEstado[$arrD['estuf']]['municipios']) != 1 ? " municípios" : " município") ?></td>
								<td class="number" ><?php echo number_format($arrReg['total_medicos'],0,",",".") ?></td>
							</tr>
							<?php if($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid]): ?>
								<?php $y=0;foreach($arrEstado[$arrD['estuf']]['municipios_por_regiao_saude'][$tpmid] as $ibge => $arrMun): ?>
									<?php $cor = $y%2 ? "#f9f9f9" : "#f3f3f3" ?>
									<tr onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" bgcolor="<?php echo $cor ?>" class="tr_mun_<?php echo $tpmid ?>" style="display:none"  >
										<td><img style="margin-left:30px" src="../imagens/seta_filho.gif" /></td>
										<td><?php echo $ibge ?></td>
										<td><?php echo $arrMun['municipio'] ?></td>
										<td class="number" ><?php echo number_format($arrMun['qtde_medicos'],0,",",".") ?></td>
									</tr>
								<?php $y++;endforeach; ?>
							<?php endif; ?>
						<?php $n++;endforeach; ?>
							<tr bgcolor="#CCCCCC" >
								<td class="bold" >Total</td>
								<td class="bold" >-</td>
								<td class="bold" ><?php echo number_format(count($arrEstado[$arrD['estuf']]['municipios']),0,",",".").(count($arrEstado[$arrD['estuf']]['municipios']) != 1 ? " municípios" : " município") ?></td>
								<td class="bold number" ><?php echo number_format($arrEstado[$arrD['estuf']]['total_medicos'],0,",",".") ?></td>
							</tr>
					<?php else: ?>
						<tr>
							<td colspan="4" >Não existem registros.</td>
						</tr>
					<?php endif; ?>
				</table> <br/>
			<?php endif; ?>
			<?php
		}
	}
}


function recuperaTotalMedicosPorUf($estuf)
{
	global $db;
	$sql = "select 
				count(mdcid)
			from
				maismedicos.medico med 
			inner join
				territorios.municipio mun on mun.muncod = med.muncod
			where
				mun.estuf = '$estuf'";
	return $db->pegaUm($sql);
}
?>