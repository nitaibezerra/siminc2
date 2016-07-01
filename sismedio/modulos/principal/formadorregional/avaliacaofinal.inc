<?
			
$es = estruturaAvaliacaoFR(array());
			
$titulo 		 = 'Avaliação Final da Formação - Formador Regional';

echo '<form method="post" id="formulario" enctype="multipart/form-data">';
echo '<input type="hidden" name="requisicao" value="gravarAvaliacaoFinalFR">';



$avaliacaofinalfr = $db->pegaLinha("SELECT * FROM sismedio.avaliacaofinalfr WHERE iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."'");
if($avaliacaofinalfr) extract($avaliacaofinalfr);
	

include_once APPRAIZ_SISMEDIO."/montarQuestionario.inc";

echo '</form>';
			
			


?>