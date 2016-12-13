<?php
include_once '_funcoesproduto.php';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>

<script src="./js/planacomorc.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script>        
    function abrirProduto(id_produto_spo) {
	var url ='planacomorc.php?modulo=principal/produto/formularioproduto&acao=A&id_produto_spo='+id_produto_spo;

        $.post(url,function(data){
            $('#modal-confirm .modal-body p').html(data);
            $('.modal-dialog').css('width','60%'); 
            $('#modal-confirm .modal-title').html('Novo questionário');
            $('#modal-confirm .btn-primary').html('Salvar');
            $('#modal-confirm .btn-primary').attr('onclick','inserirProduto();');                     

            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function inserirProduto() {
        if(jQuery('#codigo').val()=='') {
            alert('Código do produto em branco');
            return false;
        }

        if(jQuery('#nome').val()=='') {
            alert('Nome do produto em branco');
            return false;
        }

        if(jQuery('#descricao').val()=='') {
            alert('Descrição em branco');
            return false;
        }
        document.getElementById('formproduto').submit();
        //jQuery('#formproduto').submit();

    }

    function excluirProduto(id_produto_spo) {
        var conf = confirm('Deseja realmente excluir o Produto?');
        if(conf) {
            window.location = 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A&requisicao=excluirProduto&id_produto_spo='+id_produto_spo;
        }
    }

</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li>Produto</li>
        <li class="active">Lista de produtos</li>
    </ol>		
</section>

<div id="modalInserirProduto" style="display:none;">

</div>

<section class="col-md-12">
    <input type="button" class="btn btn-success" value="Inserir novo" onclick="abrirProduto('');">
    <br>
    <br>
    <?
    //add condição por ano de exercicio, solicitado por Werter, até decidir se os dados se manterão desta tabela ou do cookpit de subacoes
    $sql = "
        SELECT 
            id_produto_spo as acao,
            codigo,
            nome,
            descricao
        FROM planacomorc.produto_spo psp
        WHERE st_ativo = 'A' 
            AND  id_exercicio = ".$_SESSION['exercicio'];

    $cabecalho = array("Código","Nome","Descrição");
    //$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
    $listagem = new Simec_Listagem();
    $listagem->setQuery($sql);
    $listagem->setCabecalho($cabecalho);
    $listagem->setAcoes(array('edit' => 'abrirProduto','delete'=>'excluirProduto'));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);	
    ?>
</section>

