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
        return die('<script>alert("Favor informar A(Ativo) ou I(Inativo) na situação.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A"</script>');

    }
}

if ($_REQUEST['requisicao'] == 'listar') {
    $sql = "select * from eja.valoraluno where valid = {$_REQUEST['valid']}";
    $valorRef = $db->pegaLinha($sql);
}

if ($_REQUEST['requisicao'] == 'cadastrar') {
    if(!$_REQUEST['exercicio'] && !$_REQUEST['portaria'] && !$_REQUEST['valorReferencia']){
         die('<script> alert("É necessário preenchimento dos campos.");window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A";</script>');
    }
    verificaStatus($_REQUEST['situacao']);
    $verificaAno = verificaRefAno($_REQUEST['exercicio'],"",$_REQUEST['requisicao'] );
    $valor = str_replace(".", "", $_REQUEST['valorReferencia']);
    $valorRef = str_replace(",", ".", $valor);

    $sql = "INSERT INTO eja.valoraluno(
             valexercicio, valportaria, valvaloranual, valstatus, valpi)
    VALUES ('{$_REQUEST['exercicio']}', '{$_REQUEST['portaria']}', {$valorRef}, '{$_REQUEST['situacao']}', '{$_REQUEST['pi']}')";
    $db->executar($sql);
    $db->commit();
    die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A"</script>');
}

if ($_REQUEST['requisicao'] == 'alterar') {
    verificaStatus($_REQUEST['situacao']);
    $verificaAno = verificaRefAno($_REQUEST['exercicio'], $_REQUEST['valid'],$_REQUEST['requisicao']);
    
    $valor = str_replace(".", "", $_REQUEST['valorReferencia']);
    $valorRef = str_replace(",", ".", $valor);
    $sql = "UPDATE eja.valoraluno
   SET valexercicio='{$_REQUEST['exercicio']}', valportaria='{$_REQUEST['portaria']}', valvaloranual={$valorRef}, valstatus='{$_REQUEST['situacao']}', 
       valpi='{$_REQUEST['pi']}' WHERE valid = '{$_REQUEST['valid']}'";

    $db->executar($sql);
    $db->commit();
    die('<script> window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A"</script>');
}
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" language="javascript">

    function editValorReferencia(id) {
        window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A&requisicao=listar&valid=" + id;
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
             window.location = "eja.php?modulo=principal/tabelaapoio/manter_valor_ref&acao=A";
        });
    });
</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<div class="page-header">
    <h4 id="forms"> Manter Valor de Referência Ano</h4>
</div>
<br>
<div class="well">
    <fieldset>
        <form id="formPrincipal" name="formPrincipal" method="POST"
              class="form-horizontal">
            <input type="hidden" name="requisicao" id="requisicao">
            <input type="hidden" name="valid" id="valid" value="<?php echo $valorRef['valid']; ?>">
            <div class="form-group">
                <label for="inputExercicio" class="col-lg-2 control-label">Exercício:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="4" style="width:80px;" class="normal form-control" name="exercicio" id="exercicio"  value="<?php echo $valorRef['valexercicio']; ?>" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputPortaria" class="col-lg-2 control-label">Portaria:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="100" class="normal form-control" name="portaria" id="portaria" value="<?php echo $valorRef['valportaria']; ?>" />
                </div>
            </div>
            <div class="form-group">
                <label for="inputNatureza" class="col-lg-2 control-label">Valor Referência:</label>
                <div class="col-lg-10">
                    <input type="text" onmouseout="MouseOut(this)" onfocus="MouseClick(this);
                            this.select()" onmouseover="MouseOver(this)"  style="width:20%;"
                           onkeyup="this.value = mascaraglobal('###.###.###.###,##', this.value);"  onblur="this.value = mascaraglobal('###.###.###.###,##', this.value);"
                           name="valorReferencia" id="valorReferencia" class="normal form-control" value="<?php if ($valorRef['valvaloranual']) {
    echo number_format($valorRef['valvaloranual'], 2, ",", ".");
} ?>">
                </div>
            </div>
            <div class="form-group">
                <label for="inputPi" class="col-lg-2 control-label">PI:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="20" style="width:20%;" class="normal form-control" name="pi" id="pi" value="<?php echo $valorRef['valpi']; ?>" />
                </div>
            </div>
<!--            <div class="form-group">
                <label for="inputSituacao" class="col-lg-2 control-label">Situação:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="1" style="width:40px;" class="normal form-control" name="situacao" id="situacao" value="<?php echo $valorRef['valstatus']; ?>" onblur="ChangeCase(this);" />
                </div>
            </div>-->
                <div class="form-group">
                    <label for="inputExercicio" class="col-lg-2 control-label">Situação:</label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default <?php if ($valorRef['valstatus'] == 'A' || !$valorRef['valstatus']) {
        echo 'active';
    } ?>" >
                                <input type="radio" name="situacao" id="situacao_a"
                                       value="A" <?php if ($valorRef['valstatus'] == 'A' || !$valorRef['valstatus']) {
        echo 'checked';
    } ?>/> Ativo
                            </label>
                            <label class="btn btn-default  <?php  if ($valorRef['valstatus'] == 'I') {
        echo 'active';
    } ?>">
                                <input type="radio" name="situacao" id="situacao_i"
                                       value="I"   <?php if ($valorRef['valstatus'] == 'I') {
        echo 'checked';
    } ?>/> Inativo
                            </label>
                        </div>
                    </div>
                </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if (!$valorRef['valid']) { ?>
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
SELECT valid as acao, valexercicio, valportaria, valvaloranual, valpi, CASE valstatus WHEN 'A' THEN 'Ativo'  
WHEN 'I' THEN 'Inativo' END as status from eja.valoraluno order by valexercicio desc, valportaria
DML;

$cabecalhoTabela = array(
    'Exercício',
    'Portaria',
    'Valor Referência',
    'PI',
    'Situação'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sql);
$listagem->addAcao('edit', 'editValorReferencia');
$listagem->addCallbackDeCampo('valvaloranual', 'mascaraMoeda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('capvaloruo', 'capvalorsof'));
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>