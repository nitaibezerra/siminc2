<?php

if($_REQUEST['anexo']=='download'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}
 
include_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

monta_titulo('Registro de Alterações', '');
$db->cria_aba( $abacod_tela, $url, $parametros);

include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';
$obPt = new Planotrabalho(); 
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('.btnDetalhe').click(function(){
		document.location.href = document.location.href+'&requisicao=montaDetalhePlanoTrabalho&ptrid='+this.id;
	});
	$('.btnSemestre').click(function(){
		//document.location.href = document.location.href+'&requisicao=montaDetalhePlanoTrabalho&semestre='+this.id;
		//alert(this.id);
	});
	$('.btnDownloadAnexo').click(function(){
		url = document.location.href+'&anexo=download&arqid='+this.id;
		var popUp = window.open(url, 'popDownloadAnexo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
		popUp.focus();
	});
	
});
</script>
<?php 
if($_REQUEST['requisicao']){
	
	print $obPt->$_REQUEST['requisicao']($_REQUEST);
	echo '<center><p><button onclick="javascript:history.go(-1)">Voltar</button></p></center>';
	
}else{

	$arDados = $obPt->recuperaPlanosPorCpf($_SESSION['usucpf'], true);
	$arDados = $arDados ? $arDados : array();
	$arrCabecalho = array('Ação', 'Mês Ref.', 'Preenchimento','Período',  'Data de envio', 'Visitas', 'Atividades', 'Pago');
	$db->monta_lista($arDados,$arrCabecalho,50,10,'','','');

}
?>