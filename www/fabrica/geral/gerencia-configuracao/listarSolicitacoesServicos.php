<?php 
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 


$limit = $_GET['iDisplayLength'];
$offset = $_GET['iDisplayStart'];

switch ($_GET['iSortCol_0']) {
	case 1:
		$orderby = "ss.scsid";
		break;
	case 2:
		$orderby = "ss.sidid";
		break;
	case 3:
		$orderby = "ss.usucpfrequisitante";
		break;
	case 4:
		$orderby = "est.esdid";
		break;
	default:
		//implementar restante dos cases
		$orderby = "ss.scsid";
}

//Em asc ou desc
$sort = $_GET['sSortDir_0'];

$solicitacaoRepositorio = new SolicitacaoRepositorio();
$soliticacoes = $solicitacaoRepositorio->recupereTodosPaginado($limit, $offset, $orderby, $sort);


foreach ($soliticacoes as $solicitacao){
	$idSolicitacao = $solicitacao->getId();
	$atributoHtml = "
		<a href=\"?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=$idSolicitacao\">
			<img src=\"/imagens/consultar.gif\" alt=\"Visualizar\" title=\"Visualizar\"/>
		</a>";
		
	$linha = array(
		$atributoHtml,
		$idSolicitacao,
		utf8_encode($solicitacao->getSistema()->getDescricao()),
		utf8_encode($solicitacao->getRequisitante()->getNome()),
		utf8_encode($solicitacao->getSituacao()->getDescricao()),
		utf8_encode($solicitacao->getQuantidadeProdutosContratados()),
		utf8_encode($solicitacao->getQuantidadeProdutosAuditados()),
		utf8_encode($solicitacao->getPorcentagemAuditada())
	);
	 $registros[] = $linha;
}

$output = array(
	"sEcho" => 3,
	"iTotalRecords" => 3,
	"iTotalDisplayRecords" => 3,
	"aaData" => $registros
);

print simec_json_encode($output);





