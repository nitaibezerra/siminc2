<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametro);

if($_REQUEST['cpmid']){
    $cpmid = $_REQUEST['cpmid'];
}

if($_REQUEST['excluir']){
    $cpmid = $_REQUEST['excluir'];
    $icm = new Itens_Composicao_PadraoMi();
    $cpm = new Cronograma_PadraoMi();
    
    $icm->excluirPorCpm($cpmid);
    $icm->commit();
    $cpm->excluir($cpmid);
    $cpm->commit();
}

monta_titulo( "Lista Cronograma MI", '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<?php
    $cpm = new Cronograma_PadraoMi();
    $sql = $cpm->listaSql();
?>
<div>
    <br />
    <?php
        $cabecalho = array('Ações', 'UF', 'Descrição da tipologia', 'Tecnologia', 'Valor');
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '');
    ?>
</div>


<script type="text/javascript">
    
    $(function() {
	<?php if($_SESSION['obras2']['pagamento']['alert']): ?>
		alert('<?php echo $_SESSION['obras2']['pagamento']['alert'] ?>');
		<?php unset($_SESSION['obras2']['pagamento']['alert']) ?>
	<?php endif; ?>
    });
    
    function alterar(cpmid){
        window.location = 'obras2.php?modulo=principal/cadItensComposicaoMI&acao=A&cpmid='+cpmid;
    }
    
    function deletar(cpmid){
        window.location = 'obras2.php?modulo=principal/listaItensComposicaoMI&acao=A&excluir='+cpmid;
    }
    function duplicar(cpmid){
        window.location = 'obras2.php?modulo=principal/cadItensComposicaoMI&acao=A&copiar=true&cpmid='+cpmid;
    }
</script>