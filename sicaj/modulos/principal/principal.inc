<?php
/**
 * $Id: principal.inc 100556 2015-07-28 20:37:34Z maykelbraz $
 */


$fm = new Simec_Helper_FlashMessage('sicaj/anexos');

if (isset($_GET['currency']) && $_GET['currency'] == 'true') {
    $total = $_GET['passado'] + $_GET['atual'];
    echo number_format($total, 2, ',', '.');
    die;
}

if ('alterarResponsavel' == $_POST['requisicao']) {
    $sql = <<<DML
SELECT usu.usucpf AS respcpf,
       usu.usunome,
       usu.usufoneddd,
       usu.usufonenum,
        usu.usuemail
  FROM sicaj.pedidocdo pdc
    INNER JOIN seguranca.usuario usu ON(COALESCE(pdc.respcpf, '%s') = usu.usucpf)
  WHERE pdcid = %d
DML;
    $stmt = sprintf($sql, $_SESSION['usucpf'], $_POST['pdcid']);
    $dados = $db->pegaLinha($stmt);

    $inputCpf = inputTexto(
        'respcpf',
        mascaraglobal($dados['respcpf'], "###.###.###-##"),
        'respcpf',
        15,
        false,
        array(
            'return' => true,
            'masc' => '###.###.###-##',
            'evtblur' => "consultarResponsavel()"
        )
    );

    echo <<<HTML
<input type="hidden" id="pdcid" value="{$_POST['pdcid']}" />
<div class="row">
    <label class="control-label col-md-2">CPF:</label>
    <label class="col-md-10">{$inputCpf}</label>
</div>
<div class="row">
    <label class="control-label col-md-2">Nome:</label>
    <p class="col-md-10 form-control-static" id="search-name">{$dados['usunome']}</p>
</div>
<div class="row">
    <label class="control-label col-md-2">Telefone:</label>
    <p class="col-md-10 form-control-static" id="search-telefone">({$dados['usufoneddd']}) {$dados['usufonenum']}</p>
</div>
<div class="row">
    <label class="control-label col-md-2">E-mail:</label>
    <p class="col-md-10 form-control-static" id="search-mail">{$dados['usuemail']}</p>
</div>
<script type="text/javascript">
function consultarResponsavel()
{
    $.post(
        window.location.href,
        {requisicao: 'consultarResponsavel', usucpf:$('#respcpf').val()},
        function(response){
            if (!(response)) {
                $('#search-name').text('-');
                $('#search-telefone').text('-');
                $('#search-mail').text('-');
                $('#modal-confirm .btn-primary').prop('disabled', true);
            } else {
                $('#search-name').text(response.usunome);
                $('#search-telefone').text('(' + response.usufoneddd + ') ' + response.usufonenum);
                $('#search-mail').text(response.usuemail);
                $('#modal-confirm .btn-primary').removeProp('disabled');
            }
        },
        'json'
    );
}
function salvarResponsavel()
{
    $.post(
        window.location.href,
        {requisicao: 'salvarResponsavel',
         usucpf:$('#respcpf').val(),
         pdcid:$('#pdcid').val()},
        function(response){
            $('#modal-confirm').modal('hide');
            if (!response) {
                alert('Não foi possível atualizar o responsável do pedido.');
                return;
            }
            $('#user-name').text($('#search-name').text());
            $('#user-tel').text($('#search-telefone').text());
            $('#user-mail').text($('#search-mail').text());
        },
        'json'
    );
}
</script>
HTML;
    die();
}

if ('consultarResponsavel' == $_POST['requisicao']) {
    $usucpf = str_replace(array('.', '-'), '', $_POST['usucpf']);

    $sql = <<<DML
SELECT usu.usucpf,
       usu.usunome,
       usu.usufoneddd,
       usu.usufonenum,
       usu.usuemail
  FROM seguranca.usuario usu
  WHERE usu.usucpf = '%s'
DML;
    $stmt = sprintf($sql, $usucpf);
    if ($data = $db->pegaLinha($stmt)) {
        die(simec_json_encode($data));
    }
    die(simec_json_encode(array()));
}

if ('salvarResponsavel' == $_POST['requisicao']) {
    $usucpf = str_replace(array('.', '-'), '', $_POST['usucpf']);
    $pdcid = $_POST['pdcid'];

    $sql = <<<DML
UPDATE sicaj.pedidocdo
  SET respcpf = '%s'
  WHERE pdcid = %d
DML;
    $stmt = sprintf($sql, $usucpf, $pdcid);
    $db->executar($stmt);
    $db->commit();
    die(simec_json_encode(true));
}

/**
 *
 */
if ($_GET['downloadFile']) {
    $arqid = $db->pegaUm("select arqid from sicaj.anexogeral where arqid = {$_GET['downloadFile']}");
    if ($arqid) {
        $file = new FilesSimec('anexogeral', null, 'sicaj');
        $file->getDownloadArquivo($arqid);
        $fm->addMensagem('Download enviado com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar efetuar o download!', Simec_Helper_FlashMessage::ERRO);
    }
}

/**
 *
 */
if (isset($_GET['resend']) && isset($_GET['id'])) {
    $docid = pegaDocid($_GET['id']);
    if (envia_email_pedido_segep($docid)) {
        $fm->addMensagem('E-mail enviado à UO!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar enviar e-mail à UO.', Simec_Helper_FlashMessage::ERRO);
    }
    die();
}

/**
 * dados para flashmessage capturado da sessão
 */
if (!empty($_SESSION['flashmessagem']['message'])) {
    $fm->addMensagem($_SESSION['flashmessagem']['message'], $_SESSION['flashmessagem']['type']);
    unset($_SESSION['flashmessagem']);
}

$metaDataCombo = array(
    'size' => 10,
    'classe' => 'datepicker',
    'autocomplete' => 'off'
);

//Chamada para cabeçalho padrão SIMEC
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" language="javascript" src="/planacomorc/js/moment-with-locales.js"></script>
<script type="text/javascript" language="javascript" src="/planacomorc/js/bootstrap-datetimepicker.min.js"></script>
<script type="text/javascript" language="javascript" src="/ted/js/jquery.livequery.js"></script>
<link href="/planacomorc/css/bootstrap-datetimepicker.min.css" rel="stylesheet" type="text/css">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script>
$(function(){
    /**
     * botão de upload no padrão bootstrap
     */
    $("input[type=file]").bootstrapFileInput();

    /**
     * datePicker no padrão bootstrap
     */
    $(".datepicker").datetimepicker({
        language: "pt-br",
        pickTime: false,
        useCurrent: false
    });
});
</script>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li><a href="sicaj.php?modulo=principal/listar&acao=A">Listagem de pedidos</a></li>
        <li class="active">Pedidos</li>
    </ol>
<?php
// -- Identificando a aba ativa
$abaAtiva = (isset($_GET['aba']) ? $_GET['aba'] : 'pedidos');
$unicod = (isset($_GET['unicod'])) ? (int) $_GET['unicod'] : null;
$id = (isset($_GET['id'])) ? (int) $_GET['id'] : null;
if (!$unicod) {
    echo '<script type="text/javascript"> location.href="sicaj.php?modulo=inicio&acao=C"; </script>';
}

// -- Código e nome da Unidade Orçamentária
$unidade = $db->pegaUm("select u.unicod ||' - '|| u.unidsc AS unidade from public.unidade u where unicod = '{$unicod}'");

// -- URL base das abas
$urlBaseDasAbas = 'sicaj.php?modulo=principal/principal&acao=A&unicod='.$unicod;

/**
 * Caso existe id do pedido
 * captura os dados do pedido e ajusta a url com id do pedido no $_GET
 */
if ($id) {
    $dados = pegaDadosPedido($id);
    if ($dados)
        $urlBaseDasAbas.="&id={$id}";
} else {
    if ($_SESSION['request']['post']) {
        $dados = $_SESSION['request']['post'];
    }
    unset($_SESSION['request']['post']);
}

$pdctipo = $dados['pdctipo']?$dados['pdctipo']:$_REQUEST['pdctipo'];
?>
<div class="col-md-8 col-md-offset-2">
    <div class="panel panel-info">
        <div class="panel-heading">
            <h3 class="panel-title">
                Informações do pedido<?php echo $dados['pdcid']?': ' . mostra_numero_pedido($dados['pdcid']):''; ?>
            </h3>
        </div>
        <table class="table">
            <tbody>
                <tr>
                    <td style="font-weight:bold;text-align:right;width:25%">Código SICAJ:</td>
                    <td><?php echo $dados['pdccodacaojudicial']; ?></td>
                    <td style="font-weight:bold;text-align:right;width:25%">Tipo pedido:</td>
                    <td colspan="2"><?php echo formataTipoPedido($pdctipo); ?></td>
                </tr>
                <tr>
                    <td style="font-weight:bold;text-align:right;width:25%">Unidade Orçamentária:</td>
                    <td colspan="4"><?php echo $unidade; ?></td>
                </tr>
                <?php if ($dados['pdcid']): ?>
                <tr>
                    <td style="font-weight:bold;text-align:right;width:25%">Responsável:</td>
                    <td><span id="user-name"><?php echo $dados['usunome']?$dados['usunome']:'-'; ?></span>
                    <td>
                        <span class="label label-success"><span class="glyphicon glyphicon-earphone"></span></span>
                        <span id="user-tel"><?php echo $dados['usufonenum']?"({$dados['usufoneddd']}) {$dados['usufonenum']}":'-'; ?></span>
                    </td>
                    <td>
                        <span class="label label-success"><span class="glyphicon glyphicon-envelope"></span></span>
                        <span id="user-mail"><?php echo $dados['usuemail']?$dados['usuemail']:'-'; ?></span></td>
                    <td style="width:5px!important">
                        <button type="button" class="btn btn-warning btn-sm" data-toggle="popover"
                                data-content="Atribuir novo responsável" id="btn-responsavel">
                            <span class="glyphicon glyphicon-user"></span>
                        </button>
                    </td>
                </tr>
                <?php endif; ?>
            </tbody>
        </table>
    </div>
</div>
<br style="clear:both" />
<br />
<?php
$urlBaseDasAbas.='&aba=';
echo getAbasPorPerfil($urlBaseDasAbas, $abaAtiva, $pdctipo, $id);
switch ($abaAtiva) {
    case 'pedidos':
        require dirname(__FILE__) . '/abas/request/pedidos.inc';
        $arquivo = 'aba_pedidos';
        break;
    case 'reavaliacao':
        require dirname(__FILE__) . '/abas/request/pedidos.inc';
        $arquivo = 'aba_reavaliacao';
        break;
    case 'anexos':
        require dirname(__FILE__) . '/abas/request/anexos.inc';
        $arquivo = 'aba_anexos';
        break;
    case 'cdo':
        require dirname(__FILE__) . '/abas/request/cdo.inc';
        $arquivo = 'aba_cdo';
        break;
}
echo $fm->getMensagens();
require(dirname(__FILE__) . "/abas/{$arquivo}.inc");
momento_spo_segep();
pedido_concluido();
?>
</div>
<Script type="text/javascript">
$(function(){
    $('[data-toggle="popover"]').popover({trigger:'hover',placement:'left'});

    $('#btn-responsavel').click(function(){
        $.post(window.location.href, {requisicao: 'alterarResponsavel', pdcid: <?php echo $dados['pdcid']; ?>}, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('.modal-dialog').css('width', '50%');
            $('#modal-confirm .modal-title').html('Alterar responsável do pedido: <?php echo mostra_numero_pedido($dados['pdcid']); ?>');
            $('#modal-confirm .btn-default').html('Fechar');
            $('#modal-confirm .btn-primary').html('Salvar').attr('onclick','salvarResponsavel();');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    });
});
</script>