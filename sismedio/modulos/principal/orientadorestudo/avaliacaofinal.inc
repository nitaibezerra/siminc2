<?
			
$es = estruturaAvaliacaoOE(array());
			
$titulo 		 = 'Avaliação Final da Formação - Orientador de Estudo';

echo '<form method="post" id="formulario" enctype="multipart/form-data">';
echo '<input type="hidden" name="requisicao" value="gravarAvaliacaoFinalOE">';

$avaliacaofinaloe = $db->pegaLinha("SELECT * FROM sismedio.avaliacaofinaloe WHERE iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."'");
if($avaliacaofinaloe) extract($avaliacaofinaloe);
	

include_once APPRAIZ_SISMEDIO."/montarQuestionario.inc";

echo '</form>';
			
			


?>