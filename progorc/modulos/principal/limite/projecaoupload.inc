<?php
/**
 * Upload de arquivo de projeção / receita
 * $Id: projecaoupload.inc 98722 2015-06-17 13:56:11Z lindalbertofilho $
 */
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "www/recorc/_funcoes.php";
ini_set("memory_limit", "2048M");
set_time_limit(300000);

function cadastraCargaProjecao(){
    global $db;
    $data = file($_FILES['limite']['tmp_name']);
    // -- Eliminando cabeçalho
    array_shift($data);
    // -- Quebrando as linhas em colunas
    $data = array_map(explodeCSVLine, $data);

    $sqlTruncate = "DELETE FROM progorc.projecaospo";
    $db->executar($sqlTruncate);
    $db->commit();

    $sql = <<<DML
        INSERT INTO progorc.projecaospo
            (tipo, unicod, exercicio, valor)
        values
DML;
    foreach ($data as $campos) {
        $valor = str_replace(",", ".", $campos[3]);
        $sql .= <<<DML
            ('$campos[0]', '$campos[1]','$campos[2]', '$valor'),
DML;
    }
    $sqlF = substr($sql, 0, -1);
    $db->executar($sqlF);
    if(!$db->commit()){
        setFlashMessage(false, 'Não foi possível executar sua requisição. Algum dado no arquivo está no formato incorreto.');
    }else{
        setFlashMessage(true, 'Carga executada com sucesso!');
    }
    header('Location: progorc.php?modulo=principal/limite/projecaoupload&acao=A');
    die();
}

function explodeCSVLine($line) {
    return explode(';', trim($line));
}

if (isset($_POST['action'])) {
    // -- Processando o arquivo
    if (isset($_FILES['limite']['tmp_name']) && is_file($_FILES['limite']['tmp_name'])) {
        cadastraCargaProjecao();
    }
    header('Location: ' . $_SERVER['REQUEST_URI']);
    die();
}

/**
 * Cabeçalho do sistema
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" src="../library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<link rel="stylesheet" type="text/css" href="../library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<style>
    .pad-12{padding-top:12px!important}
</style>
<script type="text/javascript" language="JavaScript">
    $(document).ready(function() {
        $('input[type=file]').bootstrapFileInput();
        $(document).ready(function() {
            $('#buttonSend').click(function() {
                $('#previsaoupload').submit();
            })
        });
        $("input[type=checkbox]").bootstrapSwitch('setSizeClass', 'switch-mini');
        $('#buttonSend').click(function() {
            $('#previsaoupload').submit();
        });
        $('#import-data').click(function() {
            $('.has-error').removeClass('has-error');
            if ('' == $('#prfid').val()) {
                var label = $('#label').text();
                $('#prfid_group').addClass('has-error');
                var msg = '<div class="alert alert-danger text-center">' + '<p>O campo <strong>' + label.replace(':', '') + '</strong> é obrigatório e não pode ser deixado em branco.</p>' + '</div>';
                $('.modal-body').html(msg);
                $('#modal-alert').modal();
                return false;
            }
            $('#dados_prfid').val($('#prfid').val());

            // -- na segunda etapa, executar a validação de período de referência
            $('#previsaoupload').submit();
        });

    });
</script>

<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Carga de Projeções</li>
    </ol>
    <div class="well">
        <form name="projecaoupload" id="previsaoupload" enctype="multipart/form-data" method="POST">
            <div class="form-group">
                <div class="col-md-10">
                    <input type="file" title="Carregar arquivo de projeção (.csv)"
                        name="limite" class="btn btn-primary start" />
                    <p class="help-block">Todo anexo deve estar *obrigatoriamente no formato CSV e de acordo com o modelo. <br>Em caso de dúvidas o modelo pode ser baixado clicando no botão ao lado.</p>
                    <input type="hidden" name="action" id="action" value="carregar" />
                </div>
                <div class="col-md-2">
                    <div class="alert alert-info" style="margin-bottom:0">
                        <p>
                            <i class="glyphicon glyphicon-download-alt"></i>
                            <a href="arquivos/projecao.csv" target="_blank">Download do modelo</a>.
                        </p>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10">
                        <button class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
                        <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
                    </div>
                </div>
                <div style="clear:both"></div>
            </div>
        </form>
    </div>
    <?php
    echo getFlashMessage() . '<br style="clear:both;">';       
    ?>
</div>
