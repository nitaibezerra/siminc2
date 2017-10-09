<?php

if( $_REQUEST['versao'] != '' ){
	echo '1.10';
	die();
}

set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once "autoload.php";
include_once "_constantes.php";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
$_SESSION['usucpf'] = '';

$sql = "SELECT DISTINCT * FROM carga.par_obras_reformulacao_mi_convencional crf
		INNER JOIN obras.preobra pre ON pre.preid = crf.preid
		INNER JOIN workflow.documento doc ON doc.docid = pre.docid
		WHERE preidbkp IS NULL" ;

$arrObras = $db->carregar( $sql );
$arrObras = is_array($arrObras) ? $arrObras : Array();

foreach( $arrObras as $obra ){

	if( $obra['tooid'] == 1 ){
		$esdidDestino = 1486;
	}else{
		$esdidDestino = 1488;
	}

	if( $obra['esdid'] !=  $esdidDestino ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = $esdidDestino
					AND esdidorigem = {$obra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa, aedposacao)
					VALUES(
						{$obra['esdid']}, $esdidDestino, 'Enviar para Em reformulação MI para convencional', 'A', 'Enviada para Em reformulação MI para convencional', true, false, false, 'wf_pos_refurmulaPreObra_miparaconvencional( preid )' )
					RETURNING
						aedid";

			$aedid = $db->pegaUm($sql);
		}

		$teste = wf_alterarEstado( $obra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$obra['preid'], array( 'docid' => $obra['docid'], 'preid' => $obra['preid'] ) );
		$db->commit();

		if( $teste ){

			$objPreObra = new PreObra( $obra['preid'] );
			$novoPreid = $objPreObra->criarBkp();
			$db->commit();

			$sql = "UPDATE obras.preobra SET ptoid = NULL, preusucpfreformulacao = '', predatareformulacao = now() WHERE preid = {$obra['preid']};";
			$db->executar( $sql );

			$sql = "UPDATE carga.par_obras_reformulacao_mi_convencional SET preidbkp = $novoPreid WHERE preid = {$obra['preid']}";
			$db->executar($sql);
		}
	}
	$db->commit();

	$sql = "SELECT DISTINCT
				ent.entemail
			FROM
				obras.preobra pre
			INNER JOIN par.instrumentounidade 			inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
			INNER JOIN par.instrumentounidadeentidade	iue ON iue.inuid = inu.inuid
			INNER JOIN entidade.entidade				ent ON ent.entnumcpfcnpj = iue.iuecnpj AND ent.entemail IS NOT NULL
			WHERE
				pre.preid = {$obra['preid']}
			ORDER BY
				ent.entemail DESC";

	$entemail = $db->pegaUm( $sql );

	$texto = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO<br/>
									DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS<br/>
									COORDENAÇÃO GERAL DE INFRAESTRUTURA EDUCACIONAL<br/>
									SBS Quadra 02 - Bloco F - 14º andar - Edifício FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Prezado (a) Senhor (a) Gestor,</p>

									<p>Informamos que a(s) ação (ões) referente à construção de creche em Metodologias Inovadoras está (ão) aberta (s) para reformulação, possibilitando a alteração do
									projeto para construção em metodologia convencional.</p>

									<p>Com a disponibilização de dois novos projetos, que aumentam a capacidade de atendimento das unidades escolares, o município deverá fazer os ajustes necessários
									às obras, considerando o termo pactuado.</p>

									<p>Desta forma, comunicamos que a partir de agora a ação encontra-se na situação "<b>Em reformulação MI para Convencional</b>" e o sistema <b>SIMEC - Módulo PAR
									(Plano Trabalho / Árvore / Lista de Obras)</b> já se encontra aberto para alteração, sendo necessária a substituição dos seguintes documentos técnicos nas respectivas abas:</p>


									<p><b>Aba Dados do terreno:</b><br>
									º	<b>Tipo de obra</b> - <i>Projeto 1 ou Projeto 2 Convencional, conforme o caso;</i><br>
									º	<b>Ponto de Referência</b> - <i>Local conhecido de referência próximo ao terreno.</i></p>

									<p><b>Aba Documentos anexos</b><br>
									º	<b>Planta de locação</b> - <i>planta com o projeto padrão FNDE da Creche (Tipo 1 ou Tipo 2, conforme o caso) inserido no terreno;</i><br>
									º	<b>Declaração de Compatibilidade de Fundação</b> do projeto com o terreno.</p>


									<p>Em caso de Troca de terreno, além de alterar o tipo de obra, todas as informações (<i>endereço e relatório de vistoria</i>) e documentos anexos<br>
									(<i>fotos, plantas técnicas, estudo de demanda e declarações</i>) também deverão ser atualizados, conforme segue:</p>

									<p><b>Aba Dados do terreno:</b><br>
									º	<b>Nome do Terreno;</b><br>
									º	<b>Endereço Completo</b> - <i>Logradouro, Número, Complemento, Ponto de Referência e Bairro do novo terreno;</i><br>
									º	<b>Coordenadas</b> <i>do novo terreno.</i></p>

									<p><b>Aba Relatório de vistoria:</b><br>
									º	Informações técnicas de infraestrutura existentes do novo terreno.</p>

									<p><b>Aba Cadastro de fotos do terreno:</b><br>
									º	Relatório fotográfico - <i>fotos do novo terreno, das ruas de acesso, lotes vizinhos, todas com legenda.</i></p>

									<p><b>Abas Planilha Orçamentária:</b><br>
									º	<i>Não é passível de preenchimento, a planilha será carregada de acordo com o valor da unidade federativa.</i></p>

									<p><b>Aba Documentos anexos</b><br>
									º	<b>Estudo de Demanda</b> - <i>novo estudo de demanda caso o terreno esteja em outro bairro;</i><br>
									º	<b>Planta de localização</b> - <i>planta indicando a localização do novo terreno na malha urbana do município;</i><br>
									º	<b>Planta de situação</b> - <i>planta indicando as dimensões totais, lotes vizinhos e ruas de acesso do novo terreno;</i><br>
									º	<b>Planta de locação</b> - <i>planta com o projeto padrão FNDE da Creche (Tipo 1 ou Tipo 2) inserido no novo terreno;</i><br>
									º	<b>Levantamento planialtimétrico</b> - <i>planta com indicação das curvas de nível do novo terreno a cada metro de altura;</i><br>
									º	<b>Declaração de fornecimento de infraestrutura;</b><br>
									º	<b>Declaração de Compatibilidade de Fundação</b> <i>do projeto com o novo terreno;</i><br>
									º	<b>Declaração de Dominialidade</b>.</p>

									<p>Enquanto o município aguarda a finalização do procedimento de reformulação, com a validação do Prefeito no Termo de Compromisso reformulado,
									poderá dar início ao processo licitatório, baixando a documentação técnica referente ao projeto escolhido, Tipo 1 ou Tipo 2, do site do FNDE, no link
									<a href="http://www.fnde.gov.br/programas/proinfancia" >http://www.fnde.gov.br/programas/proinfancia</a>, e providenciando a elaboração dos projetos de implantação, bem como adequação da planilha orçamentária, caso necessária.</p>

									<p>Esclarecemos que:</p>

									<p>1.	Qualquer dúvida siga o passo-a-passo do Manual para Reformulação de Obras Metodologia Inovadora para Metodologia Convencional disponibilizado no portal do FNDE, no link:<br>
									http://www.fnde.gov.br/arquivos/category/130-proinfancia?download=9490:proinfancia-creche-de-tipo-1-e-2-manual-reformulacao-mi-convencional<br>
									2.	Após anexar os novos documentos, o município deve entrar na aba "Enviar para análise" da ação e clicar no botão "<b>enviar para análise de reformulação MI para Convencional</b>".<br>
									3.	Caso a análise de engenharia constate a necessidade de correção ou complementação da proposta, a ação sairá da situação "<b>Em análise de reformulação..."</b> e retornará
										para a situação "<b>Em Diligência de Reformulação...</b>" e as pendências do que deverá ser corrigido estarão descritas na aba "Análise de engenharia". Neste caso, o proponente deve
										ler as observações descritas nos itens marcados com <u>Não</u>, sanar as pendências e envie para análise novamente.<br>
									4.	É responsabilidade do município monitorar periodicamente o sistema SIMEC módulo PAR e verificar se a ação retornou para a situação "Em diligência de reformulação". Nesse caso, siga os passos 3 e 2 respectivamente.<br>
									5.	Está vedado o envio de documentos pelo correio. Todos os documentos referentes a alteração solicitada deverão ser substituídos no sistema.<br>
									6.	Não serão toleradas modificações no projeto padrão do FNDE.<br>
									7.	O prazo para envio dos documentos para análise de reformulação é <u>7 dias após sua liberação no SIMEC.</u><br>
									8.	Propostas que permanecerem na situação "<b>Em reformulação</b>" por mais de 60 dias travam o PAR municipal, impedindo a liberação de recursos e aprovação de novos pleitos.</p>

									<p>Durante a análise por parte do FNDE se for verificado que o novo terreno não cumpre as exigências, será solicitada a correção ou
									complementação da documentação ou em caso de inviabilidade detectada a apresentação de um terceiro terreno. Caso seja constatada a
									inviabilidade técnica de implantação da unidade em todos os terrenos apresentados, os novos e o original, será recomendado o cancelamento
									do Termo de Compromisso e a devolução dos recursos.</p>

									<p>Informações complementares poderão ser prestadas pelo endereço eletrônico reformulacao.obras@fnde.gov.br.</p>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									<b>Fábio Lúcio de Almeida Cardoso</b><br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educação-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';


	$assunto  = "Confirmação de abertura de Reformulação de obras de construção de creches em metodologias inovadoras para convencional.";

	if( $_SERVER['SERVER_NAME'] == 'simec-d' || $_SERVER['SERVER_NAME'] == 'simec-d.mec.gov.br' ){
		$email = Array($_SESSION['email_sistema']);
	}else{
		$email = Array($entemail);
	}
	// 	$email = Array($_SESSION['email_sistema']);
	if( $entemail ){
// 		enviar_email(array('nome'=>SIGLA_SISTEMA. ' - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $texto, $cc, $cco );
	}



	echo $obra['preid']." enviada para reformulação.<br>";
// 	echo "$preid - BKP criado.<br>";
}

$sql = "SELECT DISTINCT preid FROM carga.par_obras_reformulacao_mi_convencional";

$arrPreid = $db->carregarColuna( $sql );
$arrPreid = is_array($arrPreid) ? $arrPreid : Array();

foreach( $arrPreid as $preid ){


	$sql = "SELECT
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha( $sql );

	if( $arObra['obrid'] && $arObra['esdid'] != 768 ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = 768
					AND esdidorigem = {$arObra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES
						({$arObra['esdid']}, 768, 'Enviar para reformulação', 'A', 'Enviada para reformulação',
						true, false, false )
					RETURNING
						aedid";

				$aedid = $db->pegaUm($sql);
		}

		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$preid, array( 'docid' => $arObra['docid'] ) );
		$db->commit();
		if( !$teste ){
			return false;
		}
	}
}
?>