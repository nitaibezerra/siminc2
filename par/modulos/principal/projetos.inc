<?php

if( !$_POST['requisicaoAjax'] ){
	include  APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	// Este parâmetro ($arrAbas) irá servir para retirar a aba "Acompanhamento" que só deverá estar visível quando ela mesma estiver ativa
	$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO );
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	if( in_array( array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO), $perfil )){
		$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO, 15356 );
	}
	$db->cria_aba( $abacod_tela, $url, '' , $arrAbas);
}
?>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
<tr>
	<td>
		<?php
		$_REQUEST['abas'] = (empty($_REQUEST['abas']) ? 'administrarProjeto' : $_REQUEST['abas']);
		
		include_once APPRAIZ . "par/modulos/principal/{$_REQUEST['abas']}.inc";
		?>
	</td>
</tr>
</table>