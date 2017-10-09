<?php

require_once 'config.inc';

$_REQUEST['baselogin'] = (IS_LOCAL) ? 'simec_espelho_producao' : 'simec';

require_once APPRAIZ . 'includes/funcoes.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'www/ted/_constantes.php';
require_once APPRAIZ . 'www/ted/_funcoes.php';

$db = new cls_banco();

//Seleciona o sistema de segurança
if (!$_SESSION['usucpf']) {
    $_SESSION['sisid'] = 194;
    $_SESSION['usucpf'] = '';
    $_SESSION['usucpforigem'] = '';
}

if (isset($_GET['download']) && is_numeric($_GET['ted'])) {

    include_once APPRAIZ . 'www/ted/_autoload.inc';
    $pdf = new Ted_Form_Pdf();

    //Setando o TCPID da sessão no campo do formulário
    $pdf->getElement('tcpid')->setValue($_GET['ted']);

    include_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';

    $html = $pdf->showForm();
    $pdfObj = new HtmlToPdf($html);
    $pdfObj->setTitle("Termo_De_Execucao_Descentralizada_n_{$pdf->getElement('tcpid')->getValue()}.pdf");
    $pdfObj->getPDF();
    exit;
}


function formataDataBanco($valor) {
    $data = explode("/",$valor);
    $dia = $data[0];
    $mes = $data[1];
    $ano = $data[2];

    if (checkdate($mes, $dia, $ano)) {
        return $ano."-".$mes."-".$dia;
    }

    return false;
}

$where = [];
$stJoin = '';

if (isset($_POST['tcpid']) && !empty($_POST['tcpid'])) {
    $where[] = "tcp.tcpid = {$_POST['tcpid']}";
}

if (isset($_POST['ungcodproponente']) && !empty($_POST['ungcodproponente'])) {
    $where[] = "ung_p.ungabrev ilike ('%".$_POST['ungcodproponente']."%')";
}

if (isset($_POST['ungcodconcedente']) && !empty($_POST['ungcodconcedente'])) {
    $where[] = "ung_c.ungabrev ilike ('%".$_POST['ungcodconcedente']."%')";
}

if (isset($_POST['docdatainclusao']) && !empty($_POST['docdatainclusao'])) {
    $dateTime = formataDataBanco($_POST['docdatainclusao']);
    if ($dateTime) {
        $where[] = "cast(doc.docdatainclusao as date) = '".$dateTime."'";
    }
}

$sqlCountSolAteracao = "
    (select count(*)
    from workflow.historicodocumento hst
    where
        hst.aedid = ".WF_ACAO_SOL_ALTERACAO."
        and hst.docid = tcp.docid)
";

$strSQL = "
    SELECT DISTINCT
        tcp.tcpid,
        tcp.tcpid || case when {$sqlCountSolAteracao} > 0 then '.' || {$sqlCountSolAteracao}::varchar else '' end as decricao,
        coalesce(ung_p.ungabrev,' - ') as ung_propon,
        coalesce(ung_c.ungabrev,' - ') as ung_conced,
        to_char(doc.docdatainclusao, 'DD/MM/YYYY') as docdatainclusao,
        'R$ ' || trim(to_char(sum(prev.provalor), '999G999G999G999G999G999G999D99')) as provalor,
        esd.esddsc as esddsc
    FROM ted.termocompromisso tcp
    LEFT JOIN ted.coordenacao coo         ON (coo.cooid = tcp.cooid)
    LEFT JOIN public.unidadegestora ung_p ON (ung_p.ungcod = tcp.ungcodproponente)
    LEFT JOIN public.unidadegestora ung_c ON (ung_c.ungcod = tcp.ungcodconcedente)
    JOIN ted.previsaoorcamentaria prev    ON (prev.tcpid = tcp.tcpid)
    LEFT JOIN workflow.documento doc      ON (doc.docid = tcp.docid)
    JOIN workflow.estadodocumento esd     ON (esd.esdid = doc.esdid and esd.esdid = ".EM_EXECUCAO.")
    " . ( $where ? 'WHERE ' . implode(' AND ', $where) : '' ) . "
    GROUP BY tcp.tcpid, ung_p.ungabrev, ung_c.ungabrev, doc.docdatainclusao, esd.esddsc, coo.coodsc, tcp.docid
    ORDER BY 2 DESC
";

?>
<html>
    <head>
        <meta name="description" content="<?php echo NOME_SISTEMA; ?>, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">
        <meta name="keywords" content="SIMEC, MEC, PDE, Ministério da Educação, Analistas: Cristiano Cabral, Adonias Malosso, Gilberto Xavier">
        <META name="Author" content="Cristiano Cabral, cristiano.cabral@gmail.com">
        <meta name="audience" content="all">
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Expires" content="-1">
        <meta content="IE=9" http-equiv="X-UA-Compatible" />

        <!-- Bootstrap CSS -->
        <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
        <script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen" >
        <!-- End Bootstrap CSS -->
        <title><?php echo NOME_SISTEMA; ?></title>
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">

        <link href='/library/simec/css/listagem.css' rel='stylesheet' type='text/css'/>

        <script type="text/javascript">
            var getPdf = function(tcpid) {
                if (tcpid) {
                    location.href="/ted/termo-de-execucao-descentralizada.php?download=true&ted="+tcpid;
                }
            };

            $(function(){
                $("#searchAll").on("click", function(e){
                    e.preventDefault();
                    location.href="/ted/termo-de-execucao-descentralizada.php";
                });
            });
        </script>
    </head>
    <body>

        <div class="well col-md-12">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <ul class="nav">
                    <li>
                        <a class="navbar-brand" href="#" onclick="javascript:changeSystem(194)" style="left: 0px;">
                            <!--SiMEC-->
                            <img width="100px" src="../includes/layout/planeta/img/logo.png">
                        </a>
                    </li>
                </ul>
            </div>
            <h2 class="text-center">Termos de Execução Descentralizada</h2>
        </div>

        <div class="well col-md-12">
            <form class="form-horizontal"
                  name="filtroTed"
                  id="filtroTed"
                  action=""
                  method="post"
                  role="form">

                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="tcpid">Número do termo:</label>
                    </div>
                    <div class="col-md-9">
                        <input type="text" name="tcpid" id="tcpid" value="" class="form-control" maxlength="4"
                         onkeyup="this.value=mascaraglobal('#######',this.value);"
                         onblur="this.value=mascaraglobal('#######',this.value);">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="ungcodproponente">Sigla - Unidade Gestora Proponente:</label>
                    </div>
                    <div class="col-md-9">
                        <input type="text" name="ungcodproponente" id="ungcodproponente" value="" class="form-control">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="ungcodconcedente">Sigla - Unidade Gestora Concedente:</label>
                    </div>
                    <div class="col-md-9">
                        <input type="text" name="ungcodconcedente" id="ungcodconcedente" value="" class="form-control">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="docdatainclusao">Data de inclusão do termo:</label>
                    </div>
                    <div class="col-md-9">
                        <input type="text" name="docdatainclusao" id="docdatainclusao" value="" class="form-control">
                    </div>
                </div>

                <div class="form-group">
                    <div class="col-md-offset-2">
                        <button type="submit" class="btn btn-primary" name="search" id="search">Pesquisar</button>
                        <button type="submit" class="btn btn-primary" name="searchAll" id="searchAll">Ver todos</button>
                    </div>
                </div>
            </form>
        </div>

        <div class="col-md-12">
            <?php
                require APPRAIZ . 'includes/library/simec/Listagem.php';

                $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $list->setCabecalho(array(
                    'Termo',
                    'Unidade Gestora Proponente',
                    'Unidade Gestora Concedente',
                    'Data da Inclusão',
                    'Previsão Orçamentaria - Valor',
                    'Situação Documento'
                ));
                $list->addAcao('download', 'getPdf');
                $list->setQuery($strSQL);
                $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                $list->turnOnPesquisator();
                $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

                $db->close();
            ?>
        </div>
    </body>
</html>