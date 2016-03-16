<?php
/*** SISID do módulo ***/
define("SISID_AGENDA", 146);

/*** Fluxo de evento - INÍCIO ***/
define("FLUXO_AGENDA_TPDID", 85);

define("EST_AGENDA_CADASTRADO_GABINETE", 570);
define("EST_AGENDA_UNIDADE_MEC", 571);
define("EST_AGENDA_ANALISE_GABINETE", 572);
define("EST_AGENDA_AGENDADO", 573);
define("EST_AGENDA_RECUSADO", 574);
define("EST_AGENDA_ATENDIMENTO_UNIDADE_MEC", 575);
define("EST_AGENDA_FINALIZADO", 576);
/*** Fluxo de evento - FIM ***/

$_ESTADOS_WF_AGENDA_UNIDADE = array( 571, 575 );

/************************/
/*** Perfis do módulo ***/
/************************/

/*** SUPER USUÁRIO ***/
define( "PERFIL_SUPER_USUARIO",	821);
define( "PERFIL_UNIDADE_MEC",	828);
define( "PERFIL_ADMINISTRADOR",	829);
define( "PERFIL_GESTOR_MEC",	830);

?>