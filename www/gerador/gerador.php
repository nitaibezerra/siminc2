<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include "classes/Gerador.php";
include "classes/ModelGenerator.php";
include "classes/FormGenerator.php";
$db = new cls_banco();

$schema = $_GET['schema'];
$tables = $_GET['tables'];
$gerarArquivos = $_GET['gerar_arquivos'];
$appraiz = APPRAIZ;

$menuEsquema = empty($schema);
$menuTabela = empty($tables) && !empty($schema);
$menuInfo = empty($gerarArquivos) && !empty($tables);
$menuConclusao = $gerarArquivos == 'sim';

if (!empty($schema)) {
    $dml = "SELECT tablename AS table FROM pg_catalog.pg_tables WHERE schemaname = '%s'";
    $dml = sprintf($dml, $schema);
    $tabelas = $db->carregar($dml);
    $tabelas = is_array($tabelas) ? $tabelas : array();
}
?>

<html>
<head>
    <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen"></link>
    <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
    <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css" rel="stylesheet"
          integrity="sha256-k2/8zcNbxVIh5mnQ52A0r3a6jAgMGxFJFE2707UxGCk= sha512-ZV9KawG2Legkwp3nAlxLIVFudTauWuBpC10uEafMHYL0Sarrz5A7G79kXh5+5+woxQ5HM559XX2UZjMJ36Wplg==" crossorigin="anonymous">
</head>

<body style="font-family: 'Open Sans', sans-serif;">

<div class="container">

    <div class="row form-group">
        <div class="col-xs-12">
            <ul class="nav nav-pills nav-justified thumbnail">
                <li class="<?php echo($menuEsquema ? 'active"' : 'disabled') ?>">
                    <a href="#step-1">
                        <h4 class="list-group-item-heading">Etapa 1</h4>

                        <p class="list-group-item-text">Selecione um Esquema</p>
                    </a>
                </li>
                <li class="<?php echo($menuTabela ? 'active"' : 'disabled') ?>">
                    <a href="#step-2">
                        <h4 class="list-group-item-heading">Etapa 2</h4>

                        <p class="list-group-item-text">Selecione uma Tabela</p>
                    </a>
                </li>
                <li class="<?php echo($menuInfo ? 'active"' : 'disabled') ?>">
                    <a href="#step-3">
                        <h4 class="list-group-item-heading">Etapa 3</h4>

                        <p class="list-group-item-text">Informações Adicionais</p>
                    </a>
                </li>
                <li class="<?php echo($menuConclusao ? 'active"' : 'disabled') ?>">
                    <a href="#step-3">
                        <h4 class="list-group-item-heading">Conclusão</h4>

                        <p class="list-group-item-text">Arquivos Gerados</p>
                    </a>
                </li>
            </ul>
        </div>
    </div>

    <?php
    if ($menuEsquema): ?>
        <div class="row setup-content" id="step-1">
            <div class="col-lg-12 well text-center">
                <?php include_once('views/formEsquema.php'); ?>
            </div>
        </div>

    <?php elseif ($menuTabela): ?>
        <div class="row setup-content" id="step-2">
            <div class="col-lg-12 well text-center">
                <?php include_once('views/formTabela.php'); ?>
            </div>
        </div>

    <?php elseif ($menuInfo): ?>
        <div class="row setup-content" id="step-3">
            <div class="col-lg-12 well">
                <?php include_once('views/formInfo.php'); ?>
            </div>
        </div>

    <?php elseif ($menuConclusao):  ?>
        <div class="row setup-content" id="step-2">
            <div class="col-md-12 well text-center">
                <?php include_once('gerarArquivos.php'); ?>
            </div>
        </div>
    <?php endif; ?>
</div>

<?php include_once('views/rodape.php'); ?>
</body>
</html>
