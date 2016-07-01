<?php 
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 
$orderBy = $_POST['campo'] == null ? 'scsid' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

$solicitacaoRepositorio = new SolicitacaoRepositorio();
$solicitacoes = $solicitacaoRepositorio->recupereTodos($orderBy, $ordem, $limit, $offset);

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
		
		if ($solicitacao->isPassivelAuditoria()) {
			$elemento .= 		"<a href=\"?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao={$solicitacao->getId()}\">";
			$elemento .=			"<img src=\"/imagens/consultar.gif\" alt=\"Visualizar\" title=\"Visualizar\"/>";
			$elemento .=		"</a>";
		}
		
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
	$elementos = "<tr><td colspan=\"8\" class=\"alignCenter\">Não foram encontrados registros</td></tr>";
}

print $elementos;
