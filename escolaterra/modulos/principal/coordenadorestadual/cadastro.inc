<?

$perfil = array('perfil' 	   => 'coordenadorestadual',
				'botaoinserir' => 'Inserir Tutores',
				'pflcod' 	   => PFL_TUTOR);

$tipo = $sis = 'coordenadorestadual';

$goto_ant 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";

if(habilitarAbaValidacaoProfessores(array())) {
	$goto_pro 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=validacaoprofessores";
} else {
	$goto_pro 		= false;
}


include_once APPRAIZ_ESCOLATERRA."cadastramento.inc";

?>