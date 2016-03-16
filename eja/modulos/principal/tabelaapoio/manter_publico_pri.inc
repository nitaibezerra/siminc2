<?php
require (APPRAIZ . 'includes/library/simec/Listagem.php');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";

if ($_REQUEST['requisicao'] == 'listar') {
    $sql = "select * from eja.publicoprioritario where pupid = {$_REQUEST['pupid']}";
    $pubPri = $db->pegaLinha($sql);
}

if ($_REQUEST['requisicao'] == 'excluir') {
        $sql = "DELETE FROM eja.publicoprioritario WHERE pupid = '{$_REQUEST['pupid']}'";
        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A"</script>');
}

if ($_REQUEST['requisicao'] == 'cadastrar') {
    if (!$_REQUEST['descricao']) {
        die('<script> alert("É necessário preenchimento dos campos.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A";</script>');
    }
        $sql = "INSERT INTO eja.publicoprioritario(pupdescricao)
                VALUES ('{$_REQUEST['descricao']}')";
        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A"</script>');
}

if ($_REQUEST['requisicao'] == 'alterar') {
        $sql = "UPDATE eja.publicoprioritario
                SET pupdescricao='{$_REQUEST['descricao']}' WHERE pupid = '{$_REQUEST['pupid']}'";

        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A"</script>');
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" language="javascript">

    function editPubPri(id) {
        window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A&requisicao=listar&pupid=" + id;
    }

     function deletePubPri(id) {
        window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A&requisicao=excluir&pupid=" + id;
    }

    function ChangeCase(campo) {
        campo.value = campo.value.toUpperCase();
    }
//    function verificaRefAno(exercicio) {
//        $.post("eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A&requisicao=verificar&exercicioPar=" + exercicio, function(html) {
//            console.log(html);
//        });
//    }

    jQuery(document).ready(function() {
        $('#cadastrar').click(function() {
            $('#requisicao').val('cadastrar');
            $("#formPrincipal").submit();
        });
        $('#alterar').click(function() {
            $('#requisicao').val('alterar');
            $("#formPrincipal").submit();
        });
        $('#novo').click(function() {
             window.location = "eja.php?modulo=principal/tabelaapoio/manter_publico_pri&acao=A";
        });
    });
</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<div class="page-header">
    <h4 id="forms"> Manter Público Prioritário</h4>
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao">
            <input type="hidden" name="pupid" id="pupid" value="<?php echo $pubPri['pupid']; ?>">
            <div class="form-group">
                <label for="inputPi" class="col-lg-2 control-label">Descrição:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="20" style="width:20%;" class="normal form-control" name="descricao" id="descricao" value="<?php echo $pubPri['pupdescricao']; ?>" />
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if (!$pubPri['pupid']) { ?>
                        <button class="btn btn-info" id="cadastrar" type="button">Cadastrar</button>
                    <?php } else { ?>
                        <button class="btn btn-warning" id="alterar" type="button">Alterar</button>
                        <button class="btn btn-success" id="novo" type="button">Novo registro</button>
<?php } ?>
                </div>
            </div>
        </form>
    </fieldset>
</div>
<br>
<?php
$sql = <<<DML
SELECT pupid as acao, pupdescricao from eja.publicoprioritario order by pupid
DML;

$cabecalhoTabela = array(
    'Descrição'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->addAcao('edit', 'editPubPri');
$listagem->addAcao('delete', 'deletePubPri');
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>