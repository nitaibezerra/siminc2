<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

    $db->cria_aba($abacod_tela,$url,$parametro);

monta_titulo( "Lista de Formulários MI", '' );

if($_REQUEST['excluir']){
    $ftiid = $_REQUEST['excluir'];
    $form = new FormularioTecnologiaMi();
	$form->excluirFormulario($ftiid);
}

$formularioMI = new FormularioTecnologiaMi();
$sql = $formularioMI->listaSql($_POST);

$arrCabecalho = array("Ação","Tecnologia", "Nome", "Ordem", "Arquivo","Obrigatório");
$db->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
    
excluirPergunta = function (ftiid){
    if(confirm("Deseja realmente exluir este formulário?"))
	{
        window.location = 'obras2.php?modulo=principal/listaFormularioMI&acao=A&excluir='+ftiid;
	}
}
abrirPergunta = function (ftiid){
    window.location = 'obras2.php?modulo=principal/cadFormularioMI&acao=A&ftiid='+ftiid;
}

downloadArquivo = function (arqid)
{
	window.location.href="obras2.php?modulo=principal/cadFormularioMI&acao=A&arquivo="+arqid;
}

$(function() {
	<?php if($_SESSION['obras2']['pagamento']['alert']): ?>
		alert('<?php echo $_SESSION['obras2']['pagamento']['alert'] ?>');
		<?php unset($_SESSION['obras2']['pagamento']['alert']) ?>
	<?php endif; ?>
});
</script>