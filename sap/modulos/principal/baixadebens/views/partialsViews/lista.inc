<?php 
($filtroBenStatus == 'I') ? $msg = 'Baixa de Bens Abertos' : $msg = 'Baixa de Bens Cadastrados';
?>
<div class="tituloLista"> <?php echo $msg; ?> </div>

<?php

// verifica se houve filtro
if($_POST['requisicao'] =='filtrar')
	$oBensBaixa->listarBensBaixaPorFiltro($_POST, $filtroBenStatus);
else
	$oBensBaixa->listarBensBaixaPorFiltro(array(), $filtroBenStatus);

?>