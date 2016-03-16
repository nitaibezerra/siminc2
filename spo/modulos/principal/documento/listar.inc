<?php

$caminho_atual = $_SERVER ['REQUEST_URI'];

if ($_GET ['requisicao'] == 'excluir') {
    $sql = "DELETE FROM spo.anexogeral WHERE arqid = {$_GET['arqid']}";
    if ($db->executar( $sql )) {
        $db->commit ();
    }
}

if ($_REQUEST ['download'] == 'S') {
    $file = new FilesSimec();
    $arqid = $_REQUEST ['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo( $arqid );
    exit();
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Comunicados - Listagem de Arquivos</li>
    </ol>
    <?php
    $sql = "
        SELECT
        con.arqid,
        '<a href=\"spo.php?modulo=principal/documento/visualizar&acao=A&download=S&arqid='|| arq.arqid ||'\">' || arq.arqnome || '.' || arq.arqextensao || '</a>' as arquivo,
        con.angdsc,
        arq.arqtamanho,
        to_char(arq.arqdata, 'DD/MM/YYYY') || ' ' || arq.arqhora as arqdata,
        usu.usunome
    FROM
        spo.anexogeral con
        INNER JOIN public.arquivo arq on con.arqid = arq.arqid
        INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
       ORDER BY angtipoanexo
    ";


    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array (
    //    "Alterar/Excluir",
        "Download",
        "Descrição",
        "Tamanho(bytes)",
        "Data inclusão",
        "Responsável"));
    $listagem->setQuery($sql);
    $listagem->setAcoes(array(
        'edit'   => 'editLimite',
        'delete' => 'deleteLimite',
        'send'  => 'comunicadoEmail'
    ));
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>
    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
    <?php endif; ?>

    <div class="form-group">
        <div class="col-lg-12" style="text-align: center;">
            <button class="btn btn-primary" id="voltar" type="button" onclick="voltar();">Voltar</button>
        </div>
    </div>
</div>
<script>
    function voltar(){
        window.location = 'spo.php?modulo=inicio&acao=C';
    }

    function editLimite(arqid) {
        window.location = "spo.php?modulo=principal/documento/cadastrar&acao=A&arqid="+arqid;
    }

    function deleteLimite(arqid){
        if(confirm("Deseja realmente excluir este arquivo?")){
            window.location += '&requisicao=excluir&arqid='+arqid;
        }
    }

    function comunicadoEmail(arqid) {
        window.location = "spo.php?modulo=principal/documento/comunicadoEmail&acao=A&arqid="+arqid;
    }
</script>