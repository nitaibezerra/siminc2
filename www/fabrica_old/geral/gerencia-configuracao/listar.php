<?php 
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 
$cpf = $_SESSION['usucpf'];

if ($_POST['scsid']!=null){
	$idSolicitacao = $_POST['scsid'];
	$where[] = "scsid = $idSolicitacao";
}

if ($_POST['gc-sistemas']!=null){
	$idSistema = $_POST['gc-sistemas'];
	$where[] = "sidid = $idSistema";
}

if ($_POST['gc-fiscais']!=null){
	$cpfFiscal = $_POST['gc-fiscais'];
	$where[] = "cpfauditor = '$cpfFiscal'";
}

if ($_POST['gc-situacoes']!=null){
	$situacao = $_POST['gc-situacoes'];
	$where[] = "esdid = $situacao";
}

if ($_POST['data_abertura_inicio']!=null){
	$dataAberturaInicio = DateTimeUtil::retiraMascara($_POST['data_abertura_inicio']);
	$where[] = "dataabertura::date >= '$dataAberturaInicio'::date";
}

if ($_POST['data_abertura_fim']!=null){
	$dataAberturaInicio = DateTimeUtil::retiraMascara($_POST['data_abertura_fim']);
	$where[] = "dataabertura::date <= '$dataAberturaInicio'::date";
}

if ($_POST['data_finalizacao_inicio']!=null){
	$dataFinalizacaoInicio = DateTimeUtil::retiraMascara($_POST['data_finalizacao_inicio']);
	$where[] = "(
					SELECT htddata 
					from fabrica.solicitacaoservico sss 
					join workflow.documento d 
						on d.docid = solicitacao.docid 
					join workflow.historicodocumento hsd 
						on d.docid = hsd.docid 
					join workflow.acaoestadodoc aed 
						on hsd.aedid = aed.aedid 
					where
						sss.scsid = solicitacao.scsid AND
						aed.esdidorigem = 361 and aed.esdiddestino = 253
					order by htddata desc limit 1
				)::date >= '$dataFinalizacaoInicio'::date";
}

if ($_POST['data_finalizacao_fim']!=null){
	$dataFinalizacaoFim = DateTimeUtil::retiraMascara($_POST['data_finalizacao_fim']);
	$where[] = "(
					SELECT htddata 
					from fabrica.solicitacaoservico sss 
					join workflow.documento d 
						on d.docid = solicitacao.docid 
					join workflow.historicodocumento hsd 
						on d.docid = hsd.docid 
					join workflow.acaoestadodoc aed 
						on hsd.aedid = aed.aedid 
					where
						sss.scsid = solicitacao.scsid AND
						aed.esdidorigem = 361 and aed.esdiddestino = 253
					order by htddata desc limit 1
				)::date <= '$dataFinalizacaoFim'::date";
}

if ($_POST['minhasAuditorias']=="S"){
	$where[] = "cpfauditor = '$cpf'";
}

if($where!=null){
	$where = "where " . implode(" AND ", $where);
} else {
	$where = " where cpfauditor = '$cpf' OR cpfauditor is null";
}

$orderBy = $_POST['campo'] == null ? 'scsid' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

$solicitacaoRepositorio = new SolicitacaoRepositorio();
$solicitacoes = $solicitacaoRepositorio->recupereTodosPassiveisAuditoria($orderBy, $ordem, $limit, $offset, $where);

if ($solicitacoes!=null) {
	for($x = 0; $x < count($solicitacoes) ; $x++) {
		
		$solicitacao = $solicitacoes[$x];
		
		if($x % 2 == 0){
			$cssClass = 'odd';
		} else {
			$cssClass = 'even';
		}
		
		$elemento = "<tr class=\"$cssClass\">";
		$elemento .= 	"<td class=\"botoesDeAcao\">";
		$elemento .= 		"<a href=\"?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao={$solicitacao->getId()}\">";
		$elemento .=			"<img style=\"border: none;\" src=\"/imagens/consultar.gif\" alt=\"Visualizar\" title=\"Visualizar\"/>";
		$elemento .=		"</a>";
		$elemento .= 	"</td>";
		$elemento .=	"<td class=\"alignCenter\" >" . $solicitacao->getId() . "</td>";
		$elemento .=	"<td>" . $solicitacao->getSistema()->getDescricao() . "</td>";
		$elemento .=	"<td>" . $solicitacao->getFiscal()->getNome() . "</td>";
		$elemento .=	"<td>" . $solicitacao->getEstadoDocumento()->getDescricao() . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $solicitacao->getDataAberturaFormatada("d/m/Y") . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $solicitacao->getDataFinalizacaoFormatada("d/m/Y") . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $solicitacao->getQuantidadeProdutosContratados() . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $solicitacao->getQuantidadeProdutosAuditados() . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $solicitacao->getPorcentagemAuditada() . "</td>";
		$elemento .="</tr>";
		
		$elementos .= $elemento;
	}
} else {
	$elementos = "<tr title=\"count($solicitacoes)\"><td colspan=\"8\" class=\"alignCenter\">Não foram encontrados registros</td></tr>";
}

//$elementos .= "<script type=\"text/javascript\"> GerenciaConfiguracao.alterarQtdeRegistrosPesquisa(" . count($listaServicoFaseProduto) .");</script>"; 

//$paginador = new Paginador();
//$paginador->setNomePaginador('solicitacoesServico');
//$paginador->setQtdePorPagina(1);
//$paginador->setQtdeTotalRegistros(count($listaServicoFaseProduto));
//$paginador->setUrlAjax('geral/gerencia-configuracao/listar.php');

print $elementos;

