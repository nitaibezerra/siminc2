<?php

require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

include_once APPRAIZ . "demandasfies/classes/html_table.class.php";
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaClassificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaEntrega.class.inc";
include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaClassificacao = new DemandaClassificacao();
$modelDemandaEntrega = new DemandaEntrega($_REQUEST['dmeid']);

$camposDaTabela = getCamposDaTabela();

$modelDemanda = new Demanda();
$modelDemanda->popularDadosObjeto();

$claid = $_POST['claid'];
$perfis = $_SESSION['perfilusuario'][$_SESSION['usucpf']];

if ($_POST['btn_relatorio'] == 'relatorio') {
	$tabelaArvore = new HTML_Table('relaorio_pdf_demandas_fies', 'table table-condensed table-bordered  table-responsive conteudo');
	$dados = recuperarListagem($modelDemanda, $modelDemandaClassificacao);


	setHeader($camposDaTabela, $tabelaArvore);
	setBody($dados, $camposDaTabela, $tabelaArvore);
	$html = $tabelaArvore->display();
	html2Pdf($html);
}

if ($_POST['action'] == 'pesquisar') {
	$esdid_entrega = $_POST['esdid_entrega'];
	$area = $_POST['area'];
	$usucpf = $_POST['usucpf'];
	$dmdstatusdecisao = $_POST['dmdstatusdecisao'];
	$tiporelatorio = $_POST['tiporelatorio'];
	$dt_inicio = $_POST['dt_inicio'];
	$dt_fim = $_POST['dt_fim'];
	$esdid = $_POST['esdid'];
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');


function html2Pdf($html)
{
	$img_file = APPRAIZ . 'www/imagens/brasao.gif';
	$imgData = base64_encode(file_get_contents($img_file));
	$src = 'data:' . mime_content_type($img_file) . ';base64,' . $imgData;

	$data = date('d/m/Y H:i:s');
	$topo = "<html>
		<head>
		<style>
			.text-left {
				text-align: left;
			}
			.text-right {
				text-align: right;
			}
			.text-center {
				text-align: center;
			}
			.text-justify {
				text-align: justify;
			}
			th {
				text-align: left;
			}
			table {
				border-collapse: collapse;
				border-spacing: 0;
			}
			td, th {
				padding: 0;
			}
			.table {
				margin-bottom: 20px;
				max-width: 100%;
				width: 100%;
			}
			.table > tbody > tr > td, .table > tbody > tr > th, .table > tfoot > tr > td, .table > tfoot > tr > th, .table > thead > tr > td, .table > thead > tr > th {
				border-top: 1px solid #ddd;
				line-height: 1.42857;
				padding: 8px;
				vertical-align: top;
			}
			.table > thead > tr > th {
				border-bottom: 2px solid #ddd;
				vertical-align: bottom;
			}
			.table > caption + thead > tr:first-child > td, .table > caption + thead > tr:first-child > th, .table > colgroup + thead > tr:first-child > td, .table > colgroup + thead > tr:first-child > th, .table > thead:first-child > tr:first-child > td, .table > thead:first-child > tr:first-child > th {
				border-top: 0 none;
			}
			.table > tbody + tbody {
				border-top: 2px solid #ddd;
			}
			.table-condensed > tbody > tr > td, .table-condensed > tbody > tr > th, .table-condensed > tfoot > tr > td, .table-condensed > tfoot > tr > th, .table-condensed > thead > tr > td, .table-condensed > thead > tr > th {
				padding: 5px;
			}
			.table-bordered {
				border: 1px solid #ddd;
			}
			.table-bordered > tbody > tr > td, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > td, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > thead > tr > th {
				border: 1px solid #ddd;
			}
			.table-bordered > thead > tr > td, .table-bordered > thead > tr > th {
				border-bottom-width: 2px;
			}
			.table-hover > tbody > tr:hover {
				background-color: #f5f5f5;
			}
			.thead{
				text-align: center;
				background-color: #d5d5d5;
				vertical-align: middle;
			}
			.topo{
				font-size: 13px;
			}
			.conteudo td{
				font-size: 11px;
			}
			.conteudo th{
				font-size: 13px;
				display: table-cell;
				text-align: center;
   				vertical-align: middle
			}
		</style>
		</head>
		<body>
			<table width='100%' class='table table-condensed topo'>
				<tr>
					<td class='text-center'>
						<img src='{$src}' width='60'>
					</td>
					<td>
						SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação<br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						Sistema - Demandas FIES
					</td>
					<td>
						Impresso por: <b>{$_SESSION['usunome']}</b><br> Hora da Impressão:{$data}<br>
					</td>
				</tr>
			</table>
";
	$rodape = ' </body>	</html> ';
	$conteudo = $topo . $html . $rodape;

//	echo $conteudo;
//	exit;

	$dompdf = new DOMPDF();
	$dompdf->load_html($conteudo);
	$dompdf->set_paper('A4', 'landscape');
	$dompdf->render();
	$arqnome = "relatorio_demadas_fies_vencidas_a_vencer";

	if (isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows") {
		$caminhoArq = "C:\\temp" . $arqnome . ".pdf";
	} else {
		$caminhoArq = "/tmp/" . $arqnome . ".pdf";
	}

	file_put_contents($caminhoArq, $dompdf->output());
	$file = $caminhoArq;
	header("Content-type: application/pdf");
	header("Content-Disposition: attachment;filename=$file");
	readfile($file);
	exit;
}

function setHeader($camposDaTabela, $tabelaArvore)
{
	$tabelaArvore->addTSection('thead');
	$class = 'thead';
	$tabelaArvore->addRow($class);
	foreach ($camposDaTabela as $campo) {
		$tabelaArvore->addCell($campo, '', 'header', array('class' => 'text-center th_demanda'));
	}
}

function setHeaderEntrega($tabelaArvore, $tipoRelatorio = null)
{
	$camposDaTabela = getCamposDaTabelaEntrega();
	if($tipoRelatorio == 'pdf'){
		unset($camposDaTabela['data_estado_entrega']);
		unset($camposDaTabela['dmerelocorrencia']);
	}

	$tabelaArvore->addTSection('thead');
	$class = 'thead';
	$tabelaArvore->addRow($class);
	foreach ($camposDaTabela as $campo) {
		$tabelaArvore->addCell($campo, '', 'header', array('class' => 'text-center th_entrega'));
	}
}

function setBody($dados, $camposDaTabela, $tabelaArvore, $tipoRelatorio = null)
{
	$idRaiz = '';
	$classeCssLinha = '';
	$colunasEscondidas = getColunasEscondidas();
	$cont = 0;
	foreach ($dados as $key => $dado) {
		$cont ++;
		$tabelaArvore->addTSection('tbody');
		$tabelaArvore->addRow();
		foreach ($colunasEscondidas as $colunaEscondida) {
			unset($dado[$colunaEscondida]);
		}

		foreach ($camposDaTabela as $atributo => $campo) {
			$class = 'text-center';
			$valor = $dado[$atributo];

			if($atributo == 'cont'){
				$valor = $cont;
			}
			$tabelaArvore->addCell($valor, $class);
		}

		getTabelaEntrega($dado, $tabelaArvore, $tipoRelatorio);

	}
}

function recuperarListagem(Demanda $modelDemanda, DemandaClassificacao $modelDemandaClassificacao)
{
	$sql = getSqlListagemProfe();

	$claids = $_POST['claid'];
	if ( $_POST ['action'] == 'pesquisar' ) {

		$modelDemanda->popularDadosObjeto();
		$esdid_entrega = $_POST['esdid_entrega'];
		$where = $join = array();

		$tiporelatorio = $_POST['tiporelatorio'];
		$usucpf = $_POST['usucpf'];
		$pendecia = (int)$_POST['pendecia'];
		$dt_inicio = formata_data_sql($_POST['dt_inicio']);
		$dt_fim = formata_data_sql($_POST['dt_fim']);

		if (!empty($usucpf)) {
			$where [] = " usu.usucpf = '{$usucpf}' ";
			$join[] = " INNER JOIN seguranca.usuario usu ON usu.usucpf = rea.usucpf  ";
		}

		if (!empty($pendecia)) {
			$where [] = " est_entrega.esdid = {$pendecia} ";
		}
		if (!empty($dt_inicio)) {
			$where[] = " hd_entrega.htddata >= '{$dt_inicio} 00:00:00' ";
		}
		if (!empty($dt_fim)) {
			$where[] = " hd_entrega.htddata <=  '{$dt_fim} 23:00:00' ";
		}

		if ($modelDemanda->dmdtipo) {
			$where [] = "d.dmdtipo = '{$modelDemanda->dmdtipo}' ";
		}
		if (!empty($claids)) {
			$stringClaids = implode(' , ', $claids);
			if (!empty($stringClaids)) {
				$where [] = " 	cl.claid IN ({$stringClaids}) ";
				$where [] = " 	clatipo = 'G' ";
				$join[] = " INNER JOIN demandasfies.demandaclassificacao dcl ON d.dmdid = dcl.dmdid
						INNER JOIN demandasfies.classificacao cl ON cl.claid = dcl.claid ";
			}
		}
		if ($modelDemanda->dmdstatusdecisao) {
			$where [] = "d.dmdstatusdecisao = '{$modelDemanda->dmdstatusdecisao}' ";
		}
		if ($modelDemanda->trbid) {
			$where [] = "d.trbid = {$modelDemanda->trbid}";
		}
		if ($modelDemanda->dmduf) {
			$where [] = "d.dmduf = '{$modelDemanda->dmduf}' ";
		}
		if ($modelDemanda->dmdcumprimento) {
			$where [] = "d.dmdcumprimento = '{$modelDemanda->dmdcumprimento}' ";
		}
		if ($modelDemanda->dmdmulta) {
			$where [] = "d.dmdmulta = '{$modelDemanda->dmdmulta}' ";
		}
		if (!empty($_REQUEST['esdid'])) {
			$where [] = " doc.esdid = {$_REQUEST['esdid']} ";
		}

		if (!empty($_POST['area'])) {

			switch ($_POST['area']) {
				case (PFL_PROCURADOR_FEDERAL):
					$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_EM_CADASTRAMENTO . ') ';
					break;
				case (PFL_NUCLEO_JURIDICO):
					$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_NUCLEO_JURIDICO . ', ' . ESD_DEMANDA_ADVOGADO . ') ';
					break;
				case (PFL_DTI_MEC):
					$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_DTI_MEC . ', ' . ESD_DEMANDA_EXECUCAO_DTI_MEC . ', ' . ESD_DEMANDA_ANALISTA_DTI_MEC . ') ';
					break;
				case (PFL_4_NIVEL):
					$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_4_NIVEL . ') ';
					break;
				case (PFL_GESTOR_FIES):
					$pesquisa = ' doc.esdid in (' . ESD_DEMANDA_GESTOR_FIES . ') ';
					break;
				default:
					$pesquisa = ' doc.esdid in (' . $_POST['area'] . ') ';
			}
			$where [] = $pesquisa;
		}
		if (!empty($tiporelatorio)) {
			if( $tiporelatorio == 'V'){
				$where [] = " EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer > 0 ";
			}elseif($tiporelatorio == 'AV'){
				$where [] = " EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer <= 0 ";
			}
		}

		$join = array_filter($join);

		if (!empty ($join)) {
			$join = implode('  ', $join);
			$sql .= $join;
		}

		$sql .= " WHERE d.dmdstatus = 'A' AND doc.esdid != ". ESD_DEMANDA_FINALIZADA ." ";

		$where = array_filter($where);
		if (!empty ($where)) {
			$where = implode(' AND ', $where);
			$sql .= ' AND ' . $where;
		}
	}else{
		$sql .= " WHERE d.dmdstatus = 'A' AND doc.esdid != ". ESD_DEMANDA_FINALIZADA ." ";
	}
	$sql .= ' ORDER BY reaProfe.reaprazo';

//	ver($sql, d);

	$dados = $modelDemanda->carregar($sql);
	$dados = $dados ? $dados : array();
	$demanda = organizarDemandasEntregas($dados);
	return $demanda;
}

function getColunasEscondidas()
{
	return array('reaprazo', 'dmdid', 'responsavel', 'cods', 'dmeid');
}

function getCamposDaTabela()
{
	return array(
		'cont'=>'',
		'autor' => 'Autor',
		'dmdprocesso' => 'Nº Processo',
		'objnome' => 'Objeto',
		'acjnome' => 'Tipo Ação',
		'dmduf' => 'UF',
		'dpanome' => 'Réus',
		'prazo_resp_nucleo_juridico' => 'Data Núcleo Juridico',
		'dias_em_atraso' => 'Dias em Atraso Total',
		'esddsc' => 'Situação',
		'dmdcumprimento' => 'Cumprimento da área técnica',
		'prazo_resp_advogado' => 'Prazo Advogado',
		'responsavel_advogado' => 'Advogado Responsável',
		'codigo' => 'Código',
	);
}

function getSqlListagemProfe()
{
	$perfis = pegaPerfilGeral($_SESSION['usucpf']);

	$esd_em_cadastramento = ESD_DEMANDA_EM_CADASTRAMENTO;
	$esd_profe = ESD_DEMANDA_PROFE;
	$esd_advogado = ESD_DEMANDA_ADVOGADO;
	$esd_nucleo_juridico = ESD_DEMANDA_NUCLEO_JURIDICO;

	$sql = "SELECT DISTINCT (d.dmdid, de.dmeid) as cods ,
		1 as cont,
		---- ENTREGA
		de.dmeid,
		de.dmedsc,
		to_char(de.dmedtinclusao, 'DD/MM/YYYY HH24:mi:ss') as dmedtinclusao
		,ed.esddsc,
		CASE
			WHEN de.dmetipo = 'ET' THEN 'Entrega Total'
			WHEN de.dmetipo = 'EP' THEN 'Entrega Parcial'
		END as dmetipo,
		de.usucpfinclusao,
		CASE WHEN de.dmerelocorrencia = 'f' THEN 'NÃO'
			WHEN de.dmerelocorrencia = 't' THEN 'SIM'
		END as dmerelocorrencia,
		est_entrega.esddsc as esddsc_entrega,
		to_char(hd_entrega.htddata , 'DD/MM/YYYY HH24:mi:ss') as data_estado_entrega,

		---- DEMANDA
		d.dmdid as codigo,
		de.dmeid,
	   ( SELECT array_to_string(array_agg(clanome), ', ') FROM demandasfies.demandaclassificacao dcl
			INNER JOIN demandasfies.classificacao cl on (cl.claid, d.dmdid, dmcstatus, clastatus) = ( dcl.claid, dcl.dmdid,'A','A'))  as objnome,
	   array_to_string(array(select (
	   			(coalesce( dpanome || ' - ' || (public.formata_cpf_cnpj(dpacpf)),'') || coalesce((public.formata_cpf_cnpj(dpacnpj)),'')) ) as autor
	   				from demandasfies.demandapartesacao dpa where dpastatus = 'A' and dpatipo = 'A' and dpa.dmdid = d.dmdid), '<br />') as autor,
	   ( select usunome from seguranca.usuario usu where usu.suscod != 'B' and  usu.usucpf =  rea.usucpf ) AS responsavel,
	   dmdprocesso,
	   ed.esddsc,
	   acjnome,
		CASE
			WHEN EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer > 0
			THEN EXTRACT(DAYS FROM(NOW() - reaNucleoJuridico.reaprazo ))::integer  || ' dias'
			ELSE ''
		end as dias_em_atraso,
		TO_CHAR(reaProfe.reaprazo, 'DD/MM/YYYY') as prazo_resp_prof,
		TO_CHAR(reaNucleoJuridico.reaprazo, 'DD/MM/YYYY') as prazo_resp_nucleo_juridico,
		TO_CHAR(reaAdvogado.reaprazo, 'DD/MM/YYYY') as prazo_resp_advogado,
		( select usunome from seguranca.usuario usu where usu.suscod != 'B' and  usu.usucpf =  reaAdvogado.usucpf ) AS responsavel_advogado,
		d.dmduf,
		CASE
			WHEN d.dmdstatusdecisao = 'DF' THEN 'Deferida'
			WHEN d.dmdstatusdecisao = 'ID' THEN 'Indeferida'
			WHEN d.dmdstatusdecisao = 'PD' THEN 'Parcialmente Deferida'
		end as dmdstatusdecisao,

		CASE
			WHEN d.dmdcumprimento = 'DC' THEN 'Decisão Cumprida'
			WHEN d.dmdcumprimento = 'DCP' THEN 'Decisão Cumprida Parcialmente'
			WHEN d.dmdcumprimento = 'DNC' THEN 'Decisão Não Cumprida'
		end as dmdcumprimento,
		rea.reaprazo,
		reaProfe.reaprazo,
		( SELECT array_to_string(array_agg(dpanome), ', ') FROM demandasfies.demandapartesacao dpa WHERE dpa.dmdid = d.dmdid AND dpastatus = 'A' AND dpatipo = 'R' )  as dpanome
	FROM demandasfies.demanda d
		INNER JOIN workflow.documento doc ON doc.docid = d.docid
		INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
		LEFT JOIN workflow.historicodocumento hd ON hd.hstid = doc.hstid

		LEFT  JOIN demandasfies.responsavelarea rea on case when doc.esdid = {$esd_profe} then (rea.dmdid, rea.esdid, rea.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A') else (rea.dmdid, rea.esdid, rea.reastatus) = (d.dmdid, doc.esdid, 'A'
	) end
		LEFT  JOIN demandasfies.responsavelarea reaProfe ON (reaProfe.dmdid, reaProfe.esdid, reaProfe.reastatus) = (d.dmdid, {$esd_em_cadastramento}, 'A')
		LEFT  JOIN demandasfies.responsavelarea reaAdvogado ON (reaAdvogado.dmdid, reaAdvogado.esdid, reaAdvogado.reastatus) = (d.dmdid, {$esd_advogado}, 'A')
		LEFT  JOIN demandasfies.responsavelarea reaNucleoJuridico ON (reaNucleoJuridico.dmdid, reaNucleoJuridico.esdid, reaNucleoJuridico.reastatus) = (d.dmdid, {$esd_nucleo_juridico}, 'A')

		LEFT  JOIN demandasfies.acaojudicial acj ON acj.acjid = d.acjid
		LEFT  JOIN demandasfies.demandaentrega de ON de.dmdid = d.dmdid

		LEFT JOIN workflow.documento doc_entrega ON doc_entrega.docid = de.docid
		LEFT JOIN workflow.estadodocumento est_entrega on est_entrega.esdid =doc_entrega.esdid
		LEFT JOIN workflow.historicodocumento hd_entrega ON hd_entrega.docid = doc_entrega.docid AND hd_entrega.hstid = doc_entrega.hstid
              ";
	return $sql;
}

function getAdvogados()
{
	$sisid = SIS_DEMANDASFIES;
	$pfl_adv = PFL_ADVOGADO;
	$sql = "
			SELECT usu.usucpf as codigo, usu.usunome as descricao FROM seguranca.usuario AS usu
				INNER JOIN seguranca.usuario_sistema AS ususis ON ususis.usucpf = usu.usucpf
				INNER JOIN seguranca.perfilusuario AS perf ON perf.usucpf = usu.usucpf
				WHERE perf.pflcod = {$pfl_adv} AND ususis.sisid = {$sisid}
			";
	return $sql;
}

function getTabelaEntrega($dado, $tabelaArvore, $tipoRelatorio = null)
{
	$camposDaTabelaEntrega = getCamposDaTabelaEntrega();

	if($tipoRelatorio){
		unset($camposDaTabelaEntrega['data_estado_entrega']);
		unset($camposDaTabelaEntrega['dmerelocorrencia']);
	}

	$class = '';
	if (is_array($dado['entrega'])) {
		$tabelaEntrega = '';
		$tabelaArvoreEntrega = new HTML_Table('relaorio_entrega_demandas_fies', "table table-bordered table-condensed tabela-listagem tabela-entrega", array('data-id-demanda' => $dado['codigo']));
		setHeaderEntrega($tabelaArvoreEntrega, $tipoRelatorio);
		$tabelaArvoreEntrega->addTSection('tbody');

		foreach ($dado['entrega'] as $entrega) {
			$tabelaArvoreEntrega->addRow();
			foreach ($camposDaTabelaEntrega as $atributo2 => $campo2) {
				$tabelaArvoreEntrega->addCell($entrega[$atributo2]);
			}
		}

		$tabelaEntrega = $tabelaArvoreEntrega->display();
		$tabelaArvore->addRow();
		$tabelaArvore->addCell($tabelaEntrega, 'td_entregas', 'data', array('colspan' => 14));
	}
}

function organizarDemandasEntregas($dados)
{
	$entrega = $demanda = array();
	foreach ($dados as $key => $valor) {
		$demanda[$valor['codigo']] = $valor;
		if (!empty($valor['dmeid'])) {
			$entrega[$valor['codigo']][$valor['dmeid']] = $valor;
		}
	}
	foreach ($entrega as $dmdid => $entregas) {
		$demanda[$dmdid]['entrega'] = $entrega[$dmdid];
	}
	
	return $demanda;
}

function getCamposDaTabelaEntrega()
{
	return array('dmedsc' => 'Descrição', 'dmedtinclusao' => 'Dt. Inclusão', 'dmetipo' => 'Tipo', 'dmerelocorrencia' => 'Relatório de Ocorrência', 'esddsc_entrega' => 'Estado', 'data_estado_entrega' => 'Data Alteração Estado');
}