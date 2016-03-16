<?php

$emiHabilitado = emiPossuiPerfil( array( EMI_PERFIL_ADMINISTRADOR, 
										 EMI_PERFIL_ANALISTACOEM, 
										 EMI_PERFIL_APROVADOR,
										 EMI_PERFIL_CADASTRADOR ) );

$emiSomenteLeitura = $emiHabilitado ? 'S' : 'N';
$emiDisabled 	   = $emiHabilitado ? '' : 'disabled';

?>