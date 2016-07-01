<?php 

include_once APPRAIZ.'maismedicos/classes/Form.class.inc';
$form = new Form();

include APPRAIZ.'includes/cabecalho.inc';
echo '</br>';

$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo('Lista dos Relatórios de Supervisão Periódica', $linha2);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('.btnEdita').click(function(){
		id = this.id;
		document.location.href = 'maismedicos.php?modulo=principal/grupoespecial/cadRelatorioSupervisao&acao=A&resid='+id;
	});
});
</script>
<?php 
$form->recuperaRelatorios();
?>