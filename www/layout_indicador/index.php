<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */
$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
require_once "funcoes.php";

$_POST = count($_POST) ? $_POST : array('temas'=>array(1));

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// executa a rotina de autenticação quando o formulário for submetido
if ($_POST['usucpf']) {
    if (AUTHSSD) {
        include_once APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include_once APPRAIZ . "includes/autenticar.inc";
    }
}

$arAgrupadores = array('temid' => 'tema', 'etpid' => 'etapa', 'acaid' => 'acao');
$arAgrupador = array('acaid', 'temid');
$arAgrupador = array('temid', 'etpid', 'acaid');
$arAgrupador = array('etpid', 'temid');
$arAgrupador = array('temid');

if ($_REQUEST['carregarDados']) {

    $where = '';
    foreach ($arAgrupador as $dado) {
        $where .= " and {$dado} = {$_REQUEST[$dado]}";
    }

    $sql = "select i.indid, i.indnome
            from painel.indicador i
                inner join painel.indicadoretapaeducacao ie on ie.indid = i.indid
                inner join painel.indicadortemamec it on it.indid = i.indid
            where indstatus = 'A'
            $where ";
    $dados = $db->carregar($sql);
    ?>

    <table class="table table-hover table-bordered table-striped table-condensed">
        <tr>
            <th>Indicador</th>
            <th>Descrição</th>
        </tr>
        <?php foreach ($dados as $dado) { ?>
            <tr>
                <th><?php echo $dado['indid']; ?></th>
                <th><?php echo $dado['indnome']; ?></th>
            </tr>
        <?php } ?>
    </table>

<?php
    die;
}
if ($_REQUEST['pesquisar_cruzamento']) {
    echo 'success';
    die;
}

if ($_REQUEST['limparCruzamento']) {
    unset($_SESSION['cruzamento']);
    die;
}
if ($_REQUEST['carregarCruzamento']) {
    include_once 'cruzamento.php';
    die;
}
if ($_REQUEST['carregarRegionalizacao']) {
    detalharIndicadorGraficos($_REQUEST);
    die;
}
if ($_REQUEST['carregarUf']) {
    montarUfs($_REQUEST['regcod']);
    die;
}
if ($_REQUEST['carregarMunicipio']) {
    montarMunicipio($_REQUEST);
    die;
}
if ($_REQUEST['carregarIndicadores']) {
    montarIndicadores($_REQUEST);
    die;
}
if ($_REQUEST['carregarTipoMunicipio']) {
    $gtmid = isset($_REQUEST['gtmid']) && is_array($_REQUEST['gtmid']) ? " '" . implode ("', '", $_REQUEST['gtmid']) . "'" : "0";

    $sql = "select tpmid, tpmdsc as descricao
            from territorios.tipomunicipio
            where gtmid in ({$gtmid})
            order by descricao";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();
    ?>

    <select name="tpmid[]" id="tpmid" class="form-control chosen-select" multiple data-placeholder="Selecione">
        <?php foreach ($dados as $dado) { ?>
            <option <?php echo is_array($_POST['tpmid']) && in_array($dado['tpmid'], $_POST['tpmid']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['tpmid']; ?>"><?php echo $dado['descricao']; ?></option>
        <?php } ?>
    </select>
    <?php
    die;
}
if ($_REQUEST['carregarDetalheModal']) {

    $sql = "select mun.mundescricao, mun.estuf, e.no_entidade, num_funcionarios, num_alunos_existentes, num_salas_utilizadas , id_abre_final_semana , num_alunos_existentes
                   id_lixo_recicla , id_agua_poco_artesiano , id_esgoto_rede_publica , id_esgoto_fossa , id_esgoto_inexistente , id_computadores ,
                   num_computadores , num_comp_administrativos , num_comp_alunos , id_internet , id_banda_larga , id_alimentacao , id_ens_fundamental_ciclos ,
                   id_mod_ensino_regular , id_mod_ensino_esp , id_modalidade_eja , id_reg_infantil_creche , id_reg_infantil_preescola , id_reg_fund_8_anos ,
                   id_reg_fund_9_anos , id_reg_medio_medio , id_reg_medio_integrado , id_reg_medio_normal , id_reg_medio_prof , id_esp_infantil_creche , id_esp_infantil_preescola ,
                   id_esp_fund_8_anos , id_esp_fund_9_anos , id_esp_medio_medio , id_esp_medio_integrado , id_esp_medio_normal , id_esp_eja_fundamental ,
                   id_esp_eja_medio , id_eja_fundamental , id_eja_medio , id_esp_medio_profissional , id_eja_fundamental_projovem
            from educacenso_2014.tab_entidade e
                inner join educacenso_2014.tab_dado_escola d on d.fk_cod_entidade = e.pk_cod_entidade
                INNER JOIN territorios.municipio mun on mun.muncod::int = e.fk_cod_municipio
            where pk_cod_entidade = {$_REQUEST['inep']}";

    $dados = $db->pegaLinha($sql);
    $dados = $dados ? $dados : array();

    montarDetalhesEscola($dados);
    die;
}

$sql = "select distinct temid, temdsc from painel.temamec order by temdsc;";
$temas = $db->carregar($sql);

$sql = "select distinct etpid, etpdsc from painel.etapaeducacao order by etpdsc;";
$etapa = $db->carregar($sql);

//$sql = "select distinct acaid, acadsc from painel.acao where acastatus = 'A' order by acadsc;";
//$acoes = $db->carregar($sql);

$sql = "select * from painel.prototipopainel order by tema, etapa, categoria, tipo, indid";

$where = $join = '';
if(is_array($_POST['temas'])){
    $where .= ' and t.temid in (' . implode(', ', $_POST['temas']) . ') ';
}
if(is_array($_POST['indicadores'])){
    $where .= ' and i.indid in (' . implode(', ', $_POST['indicadores']) . ') ';
}
if(is_array($_POST['etapas'])){
    $where .= ' and e.etpid in (' . implode(', ', $_POST['etapas']) . ') ';
}
if(is_array($_POST['acoes'])){
    $where .= ' and a.acaid in (' . implode(', ', $_POST['acoes']) . ') ';
}

if(is_array($_POST['ufs'])){
    $join .= "inner join painel.seriehistorica seh on seh.indid = i.indid ";
    $join .= "inner join painel.detalheseriehistorica dsh on dsh.sehid = seh.sehid ";
    $where .= " and dsh.dshuf in ('" . implode("', '", $_POST['ufs']) . "') ";
}

$where .= " and i.regid = 2";

$sql = "select distinct i.indid, i.indnome as indicador,
                        t.temid, t.temdsc as tema,
                        -- e.etpid, e.etpdsc as etapa,
                        a.acaid, acadsc as acao,
                        i.regid, reg.regdescricao
        from painel.indicador i
            inner join painel.indicadoretapaeducacao ie on ie.indid = i.indid
            inner join painel.indicadortemamec it on it.indid = i.indid
            inner join painel.temamec t on t.temid = it.temid
            -- left join painel.etapaeducacao e on e.etpid = ie.etpid
            left join painel.regionalizacao reg on reg.regid = i.regid and regstatus = 'A'
            left join painel.acao a on a.acaid = i.acaid
            $join
        where indstatus = 'A'
        {$where}
        ";

$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();
$dadosAgrupados = array();

//ver($_POST, $where, $join, $sql, d);
//ver($arAgrupador);

foreach ($dados as $dado) {




    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][] = $dado;
    $dadosAgrupados[$dado[$arAgrupador[0]]][] = $dado;
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][] = $dado;
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]][] = $dado;

    $dadosAgrupados[$dado[$arAgrupador[0]]]['qtd']++;
    $dadosAgrupados[$dado[$arAgrupador[0]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[0]]];

//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]]['qtd']++;
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[1]]];

//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]]['qtd']++;
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[2]]];

//
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]]['qtd']++;
//    $dadosAgrupados[$dado[$arAgrupador[0]]][$dado[$arAgrupador[1]]][$dado[$arAgrupador[2]]][$dado[$arAgrupador[3]]]['nome'] = $dado[$arAgrupadores[$arAgrupador[3]]];
}
//ver($dadosAgrupados, d);
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <!-- Styles Boostrap -->
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <link href="../library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
    <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="../library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="../estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="../estrutura/temas/default/css/estilo.css" rel="stylesheet">
    <link href="../library/simec/css/custom_login.css" rel="stylesheet">
	<link href='../includes/loading.css' rel='stylesheet'>

    <!-- Custom Fonts -->
    <link href="../library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic"
          rel="stylesheet" type="text/css">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
    <script src="../estrutura/js/html5shiv.js"></script>

    <![endif]-->
    <!--[if IE]>
    <link href="../estrutura/temas/default/css/styleie.css" rel="stylesheet">
    <![endif]-->

    <link href="js/switchery/switchery.css" rel="stylesheet">

     <!-- Boostrap DataTable -->
    <link href="../library/bootstrap-datatable/css/dataTables.bootstrap.css" rel="stylesheet">
    <link href="../library/bootstrap-datatable/css/dataTables.responsive.css" rel="stylesheet">
    <link href="../library/bootstrap-datatable/css/dataTables.tableTools.min.css" rel="stylesheet">
    
    <!-- Boostrap Scripts -->
    <script src="../library/jquery/jquery-1.10.2.js"></script>
    <script src="../library/jquery/jquery.maskedinput.js"></script>
    <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.min.js"></script>
    <script src="../library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
    <script src="../library/jquery/jquery.form.min.js" type="text/javascript"></script>
	<script src="../library/jquery/jquery-isloading.min.js" type="text/javascript"></script>

    <script src="../library/bootstrap-datatable/js/jquery.dataTables.js"></script>
    <script src="../library/bootstrap-datatable/js/dataTables.bootstrap.js"></script>
    <script src="../library/bootstrap-datatable/js/dataTables.responsive.js"></script>
    <script src="../library/bootstrap-datatable/js/dataTables.tableTools.min.js"></script>
	
    <script src="js/switchery/switchery.js"></script>

    <script src="funcoes.js"></script>

    <script src="multiselect.js"></script>
    <script src="jquery.responsiveTabs.js"></script>
    <!-- Custom Scripts -->
    <script type="text/javascript" src="../includes/funcoes.js"></script>

    <!-- FancyBox -->
    <script type="text/javascript" src="../library/fancybox-2.1.5/source/jquery.fancybox.js?v=2.1.5"></script>
    <link rel="stylesheet" type;="text/css" href="../library/fancybox-2.1.5/source/jquery.fancybox.css?v=2.1.5"
          media="screen"/>
    <script type="text/javascript" src="../library/fancybox-2.1.5/lib/jquery.mousewheel-3.0.6.pack.js"></script>

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css"
          href="../library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.css?v=1.0.5"/>
    <script type="text/javascript"
            src="../library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.js?v=1.0.5"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css"
          href="../library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.css?v=1.0.7"/>
    <script type="text/javascript"
            src="../library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.js?v=1.0.7"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript"
            src="../library/fancybox-2.1.5/source/helpers/jquery.fancybox-media.js?v=1.0.6"></script>

    <script language="javascript" src="/includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="/includes/Highcharts-4.0.3/js/modules/exporting.js"></script>
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
</head>

<body>

<!-- begin loader -->
<div class="loading-dialog notprint" id="loading" style="top: 0px;">
	<div id="overlay" class="loading-dialog-content" style="background-color: #000">
		<div class="ui-dialog-content">
			<img src="../library/simec/img/loading.gif">
			<span style="color: #fff !important; ">
				O sistema esta processando as informações. <br/>
				Por favor aguarde um momento...
			</span>
		</div>
	</div>
</div>
<!-- end loader -->

<div id="barra-identidade">
<div id="barra-brasil">
    <div id="wrapper-barra-brasil">
        <div class="brasil-flag"><a class="link-barra" href="http://brasil.gov.br">Brasil</a></div>
        <span class="acesso-info"><a class="link-barra" href="http://brasil.gov.br/barra#acesso-informacao">Acesso à
                informação</a></span>
        <nav><a id="menu-icon" href="#"></a>
            <ul class="list"><a class="link-barra" href="http://brasil.gov.br/barra#participe">
                    <li class="list-item first last-item">Participe</li>
                </a><a class="link-barra" href="http://www.servicos.gov.br/?pk_campaign=barrabrasil">
                    <li class="list-item last-item">Serviços</li>
                </a><a class="link-barra" href="http://www.planalto.gov.br/legislacao">
                    <li class="list-item last-item">Legislação</li>
                </a><a class="link-barra" href="http://brasil.gov.br/barra#orgaos-atuacao-canais">
                    <li class="list-item last last-item">Canais</li>
                </a></ul>
        </nav>
    </div>
</div>
<script async="" defer="" type="text/javascript" src="http://barra.brasil.gov.br/barra.js"></script>
<!-- Header -->
<header id="top" class="header" style="margin-bottom: 0">
    <div class="row">
        <div class="col-lg-6 col-xs-6 col-sm-6" style="margin-top: 5px;">
            <div class="text-left">
                <img src="../estrutura/temas/default/img/logo-simec.png" class="img-responsive" width="200">
            </div>
        </div>

        <div class="col-lg-6 col-xs-6 col-sm-6 pull-right" style="margin-top: 5px;">
            <a href="http://www.brasil.gov.br/" class="brasil pull-right">
                <img style="margin-right: 10px;" src="../estrutura/temas/default/img/brasil.png"
                     alt="Brasil - Governo Federal" class="img-responsive">
            </a>
        </div>
    </div>
</header>

<div class="row" style="height: 70px;background: #fff;margin: 0">
    <div class="col-sm-12">
        <h1 style="color:#414145">
            Painel de Temas
        </h1>
        <hr class="linha_titulo">
    </div>
</div>

<div style="color: black;">
    <div class="row" style="background: #fff">
        <form method="post" name="formulario" id="formulario">
            <div class="lateral col-sm-2">

                <div class="panel panel-success" style="border: #323232 1px solid">
                    <div class="panel-heading titulo-filtros" style="background: #323232;border: #323232">
                        Temas
                        <span class="pull-right" style="margin-top: -3px;">
                            <button type="submit" class="btn btn-xs btn-success" style="margin-bottom: 10px;">
                                <i class="fa fa-refresh"></i> Filtrar
                            </button>
                        </span>
                    </div>
                    <div class="panel-body" style="color:#333;font-size: 12px;">
                        <ul>
                            <?php foreach ($temas as $dado) { ?>
                                <li><input name="temas[]" type="checkbox" class="icheckbox_square-green campos_filtro" value="<?php echo $dado['temid']; ?>" <?php echo is_array($_POST['temas']) && in_array($dado['temid'], $_POST['temas']) ? 'checked="checked"' : ''; ?>><?php echo $dado['temdsc']; ?></li>
                            <?php } ?>
                        </ul>
                    </div>
                </div>

                <?php /*
                <div class="panel panel-success" style="border: #323232 1px solid;margin-top: 20px;">
                    <div class="panel-heading titulo-filtros" style="background: #323232;border: #323232">
                        Etapa
                        <span class="pull-right" style="margin-top: -3px;">
                            <button type="submit" class="btn btn-xs btn-success" style="margin-bottom: 10px;">
                                <i class="fa fa-refresh"></i> Filtrar
                            </button>
                        </span>
                    </div>
                    <div class="panel-body" style="color:#333;font-size: 12px;">
                        <ul>
                            <?php foreach ($etapa as $dado) { ?>
                                <li><input name="etapas[]" type="checkbox" class="icheckbox_square-green campos_filtro" value="<?php echo $dado['etpid']; ?>" <?php echo is_array($_POST['etapas']) && in_array($dado['etpid'], $_POST['etapas']) ? 'checked="checked"' : ''; ?>><?php echo $dado['etpdsc']; ?></li>
                            <?php } ?>
                        </ul>
                    </div>
                </div>

                <div class="panel panel-success" style="border: #323232 1px solid;margin-top: 20px;">
                    <div class="panel-heading titulo-filtros" style="background: #323232;border: #323232">
                        Indicadores
                        <span class="pull-right" style="margin-top: -3px;">
                            <button type="submit" class="btn btn-xs btn-success" style="margin-bottom: 10px;">
                                <i class="fa fa-refresh"></i> Filtrar
                            </button>
                        </span>
                    </div>
                    <div class="panel-body" style="color:#333;font-size: 12px;">
                        <div class="col-md-12" style="margin: 0; padding: 0" id="div_filtro_indicadores">
                            <?php echo montarIndicadores($_POST); ?>
                        </div>
                    </div>
                </div>


                <div class="panel panel-success" style="border: #323232 1px solid;margin-top: 20px;">
                    <div class="panel-heading titulo-filtros" style="background: #323232;border: #323232">
                        Ações estratégicas
                        <span class="pull-right" style="margin-top: -3px;">
                            <button type="submit" class="btn btn-xs btn-success" style="margin-bottom: 10px;">
                                <i class="fa fa-refresh"></i> Filtrar
                            </button>
                        </span>
                    </div>
                    <div class="panel-body" style="color:#333;font-size: 12px;">
                        <select name="acoes[]" value="<?php echo $dado['acaid']; ?>" style="width: 300px;" class="chosen" multiple data-placeholder="Selecione">
                            <?php foreach ($acoes as $dado) { ?>
                                <option value="<?php echo $dado['acaid']; ?>" <?php echo is_array($_POST['acoes']) && in_array($dado['acaid'], $_POST['acoes']) ? 'selected="selected"' : ''; ?>><?php echo $dado['acadsc']; ?></option>
                            <?php } ?>
                        </select>
                    </div>
                </div>
                 */ ?>
            </div>
        </form>

        <div class="col-md-10">


            <div id="horizontalTab">
                <ul>
                    <li><a href="#tab-1">Diagnósticos</a></li>
                    <li><a href="#tab-2">Programas e Iniciativas</a></li>
                    <li><a href="#tab-3">PNE</a></li>
                    <li><a href="#tab-4">Orçamento</a></li>

                </ul>

                <div id="tab-1">

                        <div class="panel-group" role="tablist" aria-multiselectable="true">
                            <div class="panel panel-default">
                                <div class="panel-heading" role="tab" id="headingOne">
                                    <h4 class="panel-title">
                                        <a role="button" data-toggle="collapse" data-parent="#accordion" href="#collapse1" aria-expanded="true" aria-controls="collapse1">
                                            <i class="fa fa-random"></i> Cruzamento de Indicadores
                                        </a>
                                    </h4>
                                </div>
                                <div id="collapse1" class="panel-collapse collapse in" role="tabpanel" aria-labelledby="headingOne">
                                    <div class="panel-body">
                                        <div id="div_cruzamento" style="margin-bottom: 0px;">
                                            <div class="alert alert-warning">
                                                <i class="fa fa-hand-o-down"></i> Araste para aqui os indicadores
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                    <div class="panel-group" role="tablist" aria-multiselectable="true">
                        <div class="panel panel-default">
                            <div class="panel-heading" role="tab" id="headingOne">
                                <h4 class="panel-title">
                                    <a role="button" data-toggle="collapse" data-parent="#accordion" href="#collapse2" aria-expanded="true" aria-controls="collapse2">
                                        <i class="fa fa-sort-alpha-asc"></i> Filtro de Indicadores
                                    </a>
                                </h4>
                            </div>
                            <div id="collapse2" class="panel-collapse collapse in" role="tabpanel" aria-labelledby="headingOne">
                                <div class="panel-body">

                                    <div class="row">
                                        <div class="col-md-12">

                                            <h1 style="color:#414145">
                                                Indicadores
                                            </h1>

                                            <hr class="linha_titulo">

<!--                                            <p data-toggle="modal" data-target="#myModal" class="alert alert-success">-->
<!--                                                <i class="fa fa-sitemap"></i> Você pode reagrupar as colunas da sua pesquisa clicando <a href="#">aqui</a>-->
<!--                                            </p>-->

                                            <div class="ibox float-e-margins">
                                                <div class="ibox-content">
                                                    <?php
                                                    echo '<table class="table">';
                                                    $attr = array();
                                                    montarRelatorio($dadosAgrupados, $arAgrupadores, $arAgrupador, 0, $attr);
                                                    echo '</table>';
                                                    ?>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div id="tab-2">
                    <h1 style="color:#414145">
                        Programas e Iniciativas
                    </h1>
                    <hr class="linha_titulo">
                </div>
                <div id="tab-3">
                    <h1 style="color:#414145">
                        PNE
                    </h1>
                    <hr class="linha_titulo">
                </div>
                <div id="tab-4">
                    <h1 style="color:#414145">
                        Orçamento
                    </h1>

                    <hr class="linha_titulo">
                </div>


            </div>
        </div>

    </div>
</div>

<!-- Login -->

<!--/LOGIN -->

</body>

<link href='http://fonts.googleapis.com/css?family=Lato:700' rel='stylesheet' type='text/css'>


<script src="http://code.highcharts.com/highcharts.js"></script>
<script src="http://code.highcharts.com/modules/exporting.js"></script>
<script>

    $(function () {
        $('.campos_filtro').on('click', function(e) {
            $('#div_filtro_indicadores').load('?carregarIndicadores=1&'+$('#formulario').serialize(), function(){
                $('#campo_indicadores').chosen();
            });
        });
    });

</script>

<?php $db->close(); ?>

<?php

function montarRelatorio($arDadosAgrupados, $arAgrupadores, $arAgrupador, $nrCount, $attr)
{
    if (key_exists($arAgrupador[$nrCount], $arAgrupadores)) {
        foreach ($arDadosAgrupados as $id => $arDetalhe) {

            if ($id == 'nome' || $id == 'qtd') continue;

            $nome = $arDadosAgrupados[$id]['nome'];
            $qtd = $arDadosAgrupados[$id]['qtd'];

            $cor = $nrCount + 6;
            $attr[$nrCount] = $id;

            $classe = 'linha';
            $complementoSpan = '';
            $classeSpan = $nrCount == (count($arAgrupador) -1) ? 'detalhe_linha' : '';
            
            for ($i=0; $i<=$nrCount; $i++) {
                $classe .= '_' . $attr[$i];
                $complementoSpan .= " {$arAgrupador[$i]}='{$attr[$i]}' ";
            }

            ?>
            <tr class="linha <?php echo $classe; ?> <?php echo $classeSpan; ?>" linha="<?php echo $classe; ?>">
                <th colspan="2">
                    <?php if($classeSpan){ ?>
                        <span class="<?php echo $classeSpan; ?>" <?php echo $complementoSpan; ?>>
                            <span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
                        </span>
                    <?php } ?>
                    <?php echo $nome . ' <span style="color: red !important;">(' . $qtd . ')</span>' ?>
                </th>
            </tr>
            <?php
            montarRelatorio($arDetalhe, $arAgrupadores, $arAgrupador, $nrCount + 1, $attr);
        }
    } else {

        $classe = 'linha';
        for ($i=0; $i<$nrCount; $i++) {
            $classe .= '_' . $attr[$i];
        }

        $cor = "#eee";

        ?>

        <tr style="display: none" id="<?php echo $classe; ?>" >
            <td width="100%">
                <table class="table table-hover" style="margin-top: -3px;">
                    <tr class="grupo">
                        <th>Ações</th>
                        <th>Indicador</th>
                        <th width="90%">Descrição</th>
                        <th>Regionalização</th>
                    </tr>

                    <?php
                    foreach ($arDadosAgrupados as $id => $arDados) {

                        if ($id === 'nome' || $id === 'qtd') {
                            continue;
                        }

                        $cor = $cor == "#fff" ? "#eee" : "#fff" ?>

                        <tr class="detalhe_regionalizacao" indid="<?php echo $arDados['indid']; ?>">
                            <th class="text-center"><a target="_blank" href="/painel/painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=<?php echo $arDados['indid']; ?>&abreMapa=1&cockpit=1"><img border="0" src="/imagens/icone_br.png" title="Exibir Mapa"></a></th>
                            <th class="text-center"><?php echo $arDados['indid']; ?></th>
                            <th><?php echo $arDados['indicador']; ?></th>
                            <th><?php echo $arDados['regdescricao']; ?></th>
                        </tr>
                    <?php } ?>
                </table>
            </td>
        </tr>
        <?php


        return true;
    }
}

?>
<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                        aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel" style="color: #000">Agrupador</h4>
            </div>
            <div class="modal-body">
                <div class="row">
                    <select class="form-control" name="agrupador[]" id="agrupador" multiple="multiple"
                            style="color:#2f2f2f">
                        <option>Tema</option>
                        <option>Etapa</option>
                        <option>Categoria</option>
                    </select>
                </div>

                <button type="button" class="btn btn-primary text-right"
                        style="margin-top: 20px;float: right;position: relative">Pesquisar
                </button>
                <div style="clear: both"></div>
            </div>

        </div>
    </div>
</div>

<div class="modal fade" id="modal-detalhe" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" style="color: #000 !important;">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Detalhes do Item</h4>
            </div>
            <div class="modal-body">
                <div id="modal-conteudo"></div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>
    $('#agrupador').multiSelect({
        keepOrder: false,
        selectableHeader: "<h5 style='margin: 7px 0 5px 0; font-weight: bold'>SELECIONAR</h5><div class='input-group'><input type='text' class='form-control input-sm selectableSearch' autocomplete='off' placeholder='pesquisar itens'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
        selectionHeader: "<h5 style='margin: 5px 0 5px 0; font-weight: bold'>SELECIONADOS</h5><div class='input-group'><input type='text' class='form-control input-sm selectionSearch' autocomplete='off' placeholder='pesquisar itens selecionados'><div class='input-group-addon'><span class='glyphicon glyphicon-search'></span></div></div>",
        afterInit: function (ms) {
            var that = this,
                $selectableSearch = that.$selectableUl.prev().children('input'),
                $selectionSearch = that.$selectionUl.prev().children('input'),
                selectableSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selectable:not(.ms-selected)',
                selectionSearchString = '#' + that.$container.attr('id') + ' .ms-elem-selection.ms-selected';


        }, afterSelect: function (acaid) {
            carregarIndicador(acaid, 'adicionar');
            return false;

        }, afterDeselect: function (acaid) {
            carregarIndicador(acaid, 'remover');
        }
    });

    /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
    $(document).ajaxSend(function (e, jqxhr, settings) {
    	 jQuery("#loading").fadeIn();

    }).ajaxStop(function(){
    	 jQuery("#loading").fadeOut();
	});

    /** Mensagem de carregando quando houver requisições em ajax */
    $.ajaxSetup({
        timeout: 0,
        error: function(xhr, status, error) {
            console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
            jQuery("#loading").fadeOut();
            jQuery("#loading").fadeIn();

            setTimeout(function(){ jQuery("#loading").fadeOut();}, 1300);
        }
    });

    // -- Substituíndo o alert do browser.
    window.alert = function(texto)
    {
        jQuery('#modal-alert .modal-body').html(texto);
        jQuery('#modal-alert').modal();
    };
</script>

<link href="multi-select.css" rel="stylesheet" type="text/css">
<link href="style.css" rel="stylesheet" type="text/css">
<link type="text/css" rel="stylesheet" href="responsive-tabs.css"/>