<?

$perfil = array('perfil' 	   => 'tutor',
				'botaoinserir' => 'Inserir Professores',
				'pflcod' 	   => PFL_PROFESSOR);

$tipo = $sis = 'tutor';

$goto_ant 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "escolaterra.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= false;


include_once APPRAIZ_ESCOLATERRA."cadastramento.inc";

?>