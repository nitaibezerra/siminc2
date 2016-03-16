<?php

// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

extract($_GET);

if(!empty($rgpid) && !empty($mvbid)){

	$oBensMovimentacaoRgp->excluir($rgpid, $mvbid);

}

// geral
if($mvbtipo == 'G')
	direcionar("?modulo=principal/bensmovimentacao/".$_GET['pagina']."&acao=A&mvbid=$mvbid");
// siape
else if($mvbtipo == 'S')
	direcionar("?modulo=principal/bensmovimentacao/".$_GET['pagina']."&acao=A&mvbid=$mvbid");
// localidade
else if($mvbtipo == 'L')
	direcionar("?modulo=principal/bensmovimentacao/".$_GET['pagina']."&acao=A&mvbid=$mvbid&rgpid=$rgpid&uendidnova=$uendidnova");
// erro volta para a tela de pesquisa
else
	direcionar("?modulo=principal/bensmovimentacao/pesquisar&acao=A");

