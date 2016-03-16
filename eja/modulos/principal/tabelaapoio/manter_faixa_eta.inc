<?php
require (APPRAIZ . 'includes/library/simec/Listagem.php');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/funcoesspo_componentes.php";

function verificaRefAno($exercicioPar, $id = false, $requisicao) {
    global $db;
    $sql = "SELECT valid FROM  eja.valoraluno WHERE valexercicio = '{$exercicioPar}'";
    $refAno = $db->pegaUm($sql);
    if ($requisicao == 'alterar') {
        if ($refAno) {
            if ($refAno != $id) {
                return die('<script>alert("Já existe o exercício informado.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A"</script>');
            }
        } else {
            return $refAno;
        }
    } else {
        if ($refAno) {
            return die('<script>alert("Já existe o exercício informado.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A"</script>');
        } else {
            return $refAno;
        }
    }
}

function verificaStatus($status){
    if ($status == 'A' || $status == 'I'){
        return true;
    }else{
        return die('<script>alert("Favor informar A(Ativo) ou I(Inativo) na situação.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A"</script>');

    }
}

if ($_REQUEST['requisicao'] == 'listar') {
    $sql = "select * from eja.faixaetaria where fetid = {$_REQUEST['fetid']}";
    $valorFaixa = $db->pegaLinha($sql);
}


if ($_REQUEST['requisicao'] == 'excluir') {

        $sql = "DELETE FROM eja.faixaetaria WHERE fetid = '{$_REQUEST['fetid']}'";
        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A"</script>');
 }

if ($_REQUEST['requisicao'] == 'cadastrar') {
    if (!$_REQUEST['fetinicio'] && !$_REQUEST['fetfim']) {
        die('<script> alert("É necessário preenchimento dos campos.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A";</script>');
    }
    if (is_numeric($_REQUEST['fetinicio']) && is_numeric($_REQUEST['fetfim'])) {
        verificaStatus($_REQUEST['situacao']);
        //$verificaAno = verificaRefAno($_REQUEST['exercicio'],"",$_REQUEST['requisicao'] );
        $sql = "INSERT INTO eja.faixaetaria(
             fetinicio, fetfim, fetstatus)
    VALUES ('{$_REQUEST['fetinicio']}', '{$_REQUEST['fetfim']}', '{$_REQUEST['situacao']}')";
        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A"</script>');
    } else {
        die('<script> alert("É necessário preenchimento dos campos com números.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A";</script>');
    }
}

if ($_REQUEST['requisicao'] == 'alterar') {
    verificaStatus($_REQUEST['situacao']);
    if (is_numeric($_REQUEST['fetinicio']) && is_numeric($_REQUEST['fetfim'])) {

        //$verificaAno = verificaRefAno($_REQUEST['exercicio'], $_REQUEST['fetid'],$_REQUEST['requisicao']);
        $sql = "UPDATE eja.faixaetaria
   SET fetinicio='{$_REQUEST['fetinicio']}', fetfim='{$_REQUEST['fetfim']}', fetstatus='{$_REQUEST['situacao']}'
   WHERE fetid = '{$_REQUEST['fetid']}'";

        $db->executar($sql);
        $db->commit();
        die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A"</script>');
    } else {
        die('<script> alert("É necessário preenchimento dos campos com números.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A";</script>');
    }
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" language="javascript">

    function editFaixaEtaria(id) {
        window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A&requisicao=listar&fetid=" + id;
    }
    
     function deleteFaixaEtaria(id) {
        window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A&requisicao=excluir&fetid=" + id;
    }

    function ChangeCase(campo) {
        campo.value = campo.value.toUpperCase();
    }
    function verificaRefAno(exercicio) {
        $.post("eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A&requisicao=verificar&exercicioPar=" + exercicio, function(html) {
            console.log(html);
        });
    }

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
             window.location = "eja.php?modulo=principal/tabelaapoio/manter_faixa_eta&acao=A";
        });
    });
</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<div class="page-header">
    <h4 id="forms"> Manter Faixa Etária</h4>
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao">
            <input type="hidden" name="fetid" id="fetid" value="<?php echo $valorFaixa['fetid']; ?>">
            <div class="form-group">
                <label for="inputPi" class="col-lg-2 control-label">Faixa Etária Inicial:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="20" style="width:20%;" class="normal form-control" name="fetinicio" id="fetinicio" value="<?php echo $valorFaixa['fetinicio']; ?>" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputPi" class="col-lg-2 control-label">Faixa Etária Final:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="20" style="width:20%;" class="normal form-control" name="fetfim" id="fetfim" value="<?php echo $valorFaixa['fetfim']; ?>" />
                </div>
            </div>
                <div class="form-group">
                    <label for="inputExercicio" class="col-lg-2 control-label">Situação:</label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default <?php if ($valorFaixa['fetstatus'] == 'A' || !$valorFaixa['fetstatus']) {
        echo 'active';
    } ?>" >
                                <input type="radio" name="situacao" id="situacao_a"
                                       value="A" <?php if ($valorFaixa['fetstatus'] == 'A' || !$valorFaixa['fetstatus']) {
        echo 'checked';
    } ?>/> Ativo
                            </label>
                            <label class="btn btn-default  <?php  if ($valorFaixa['fetstatus'] == 'I') {
        echo 'active';
    } ?>">
                                <input type="radio" name="situacao" id="situacao_i"
                                       value="I"   <?php if ($valorFaixa['fetstatus'] == 'I') {
        echo 'checked';
    } ?>/> Inativo
                            </label>
                        </div>
                    </div>
                </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if (!$valorFaixa['fetid']) { ?>
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
SELECT fetid as acao, fetinicio, fetfim,  CASE fetstatus WHEN 'A' THEN 'Ativo'  
WHEN 'I' THEN 'Inativo' END as status from eja.faixaetaria order by fetid
DML;

$cabecalhoTabela = array(
    'Faixa Etária Inicial',
    'Faixa Etária Final',
    'Situação'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->addAcao('edit', 'editFaixaEtaria');
$listagem->addAcao('delete', 'deleteFaixaEtaria');
//$listagem->addCallbackDeCampo('valvaloranual', 'mascaraMoeda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('capvaloruo', 'capvalorsof'));
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>