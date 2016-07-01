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
<script type="text/javascript" src="/pto/js/jquery.quicksearch.js"></script>
<script type="text/javascript" src="/pto/js/multi-select-master/js/jquery.multi-select.js"></script>
<script language="javascript" src="/contratogestao/js/jquery.mask.min.js"></script>
<script type="text/javascript" src="/pto/js/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="/pto/js/multi-select-master/css/multi-select.css" />
<link rel="stylesheet" type="text/css" href="/pto/css/pto.css" />
