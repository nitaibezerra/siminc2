<?
$pflcod_avaliador = PFL_ORIENTADORESTUDO;
$sis 	  		  = "orientadorestudo";
$goto_ant 		  = "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";
$goto_sal 		  = "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliacaocomplementar&fpbid=".$_REQUEST['fpbid'];
$goto_pro 		  = "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=acompanhamentoavaliacaobolsas";


include_once APPRAIZ_SISMEDIO."avaliarcomplementarequipe.inc";

?>