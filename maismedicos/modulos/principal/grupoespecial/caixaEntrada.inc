<?php 

include_once APPRAIZ.'maismedicos/classes/Form.class.inc';
$form = new Form();

include APPRAIZ.'includes/cabecalho.inc';
echo '</br>';

if($_REQUEST['respostas']){
	$form->montaRelatorioRespostas($_REQUEST['resid']);	
}else{


$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo('CAIXA DE ENTRADA','RELATÓRIOS DE SUPERVISÃO PERÓDICA');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('.btnVisualiza').click(function(){
		id = this.id;
		document.location.href = 'maismedicos.php?modulo=principal/grupoespecial/caixaEntrada&acao=A&respostas=true&resid='+id;
	});
});
</script>
<?php 
$form->recuperaRelatoriosCaixaEntrada();
}
?>