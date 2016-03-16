<?php

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';
include_once APPRAIZ . 'includes/library/simec/Autoload.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
<link rel="stylesheet" href="/contratogestao/js/ludo-jquery-treetable/css/jquery.treetable.css" />
<link rel="stylesheet" href="/contratogestao/js/ludo-jquery-treetable/css/jquery.treetable.theme.default.css" />
<link rel="stylesheet" href="/contratogestao/css/contrato_gestao.css">
<script language="javascript" src="/contratogestao/js/ludo-jquery-treetable/jquery.treetable.js"></script>
<script language="javascript" src="/contratogestao/js/contrato_gestao_funcoes.js"></script>
<script language="javascript" src="/contratogestao/js/jquery.mask.min.js"></script>