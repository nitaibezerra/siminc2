<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topo.inc';

//caso seja via url
if(!empty($_GET['bebid'])){

	$oBensBaixa->excluir($_GET['bebid']);

    if($_GET['procab'])
      direcionar('?modulo=principal/processosabertos/pesquisar&acao=A');
    else
      direcionar('?modulo=principal/baixadebens/pesquisar&acao=A');

}
