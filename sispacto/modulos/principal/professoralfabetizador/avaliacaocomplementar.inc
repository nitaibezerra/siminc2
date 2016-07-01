<?
$pflcod_avaliador = PFL_PROFESSORALFABETIZADOR;
$sis 	  		  = "professoralfabetizador";
$goto_ant 		  = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_sal 		  = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliacaocomplementar&fpbid=".$_REQUEST['fpbid'];
$goto_pro 		  = "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=acompanhamentoavaliacaobolsas";


include_once APPRAIZ_SISPACTO."avaliarcomplementarequipe.inc";

?>