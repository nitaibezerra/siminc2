<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$itemAuditoriaRepositorio = new ItemAuditoriaRepositorio();
$qtdeTotalRegistros = $itemAuditoriaRepositorio->recupereQtdeRegistros();

$orderBy = $_POST['campo'] == null ? 'itemnome' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

$itemPesquisado = utf8_decode($_POST['itemAuditoria']);
$situacaoItemPesquisado = $_POST['situacaoItemAuditoria'];

$where = '';
if ($itemPesquisado != ''){
	$where = " where (itemnome ilike '%$itemPesquisado%' OR itemdsc ilike '%$itemPesquisado%')";
}

if ($where != '' && $situacaoItemPesquisado!=''){
	$where .= " AND itemsituacao is true ";
} else if ($where == '' && $situacaoItemPesquisado!=''){
	$where = " where itemsituacao is true" ;
}

$itensAuditoria = $itemAuditoriaRepositorio->recupereTodos($orderBy, $ordem, $limit, $offset, $where);

if ($itensAuditoria!=null){
	for($x = 0; $x < count($itensAuditoria) ; $x++) {
		
		$itemAuditoria = $itensAuditoria[$x];
		
		if($x % 2 == 0){
			$cssClass = 'odd';
		} else {
			$cssClass = 'even';
		}
		
		$idItemAuditoria = $itemAuditoria->getId();
		
		$elemento = "<tr class=\"$cssClass\">";
		$elemento .= 	"<td class=\"botoesDeAcao\">";
		$elemento .=		"<a href=\"?modulo=sistema/geral/itens-auditoria/visualizar-item-auditoria&acao=A&idItemAuditoria=$idItemAuditoria\">";
		$elemento .=			"<img class=\"botao-visualizar botoes-tabela\" title=\"Visualizar Item de auditoria\" src=\"../imagens/consultar.gif\">";
		$elemento .=		"</a>";
		$elemento .= 	"</td>";
		$elemento .=	"<td>" . $itemAuditoria->getNome() . "</td>";
		$elemento .=	"<td>" . $itemAuditoria->getDescricao() . "</td>";
		$elemento .=	"<td class=\"alignCenter\">" . $itemAuditoria->getSituacao() . "</td>";
		$elemento .="</tr>";
		
		$elementos .= $elemento;
	}
} else {
	$elementos = "<tr><td colspan=\"4\" class=\"alignCenter\">Não foram encontrados registros</td></tr>";
}
print $elementos;