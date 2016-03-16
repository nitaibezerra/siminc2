<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once 'autoload.php';

include_once APPRAIZ . 'includes/library/simec/view/Helper.php';

initAutoload();

$simec = new Simec_View_Helper();

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

$_SESSION['sislayoutbootstrap'] = 'zimec';
?>
<script language="javascript" src="js/par3.js"></script>