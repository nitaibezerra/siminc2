<?php
/**
 * Arquivo de controle da configuração da PLOA.
 *
 * Permite ao usuário selecionar uma configuração anterior da PLOA e a copiar para o
 * período atual.
 *
 * $Id: configuracao.inc 98916 2015-06-22 12:37:10Z maykelbraz $
 *
 * @filesource
 */

$fm = new Simec_Helper_FlashMessage('spo/ploa/config');

if (isset($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    $srvPrf = new Proporc_Service_Periodoreferencia();
    $srvPrf->setDados($_REQUEST['configuracao']);
    try {
        switch ($requisicao) {
            case 'visualizarConfigPeriodo':
                $srvPrf->visualizarConfig();
                die();
            default:
                ver($requisicao, d);
        }
    } catch(Exception $e) {
        $fm->addMensagem('Não foi possível executar sua requisição.', Simec_Helper_FlashMessage::ERRO);
    }

    header("Location: {$_SERVER['REQUEST_URI']}");
    die();
}

// -- fluxo normal da app
$prfref = new Proporc_Model_Periodoreferencia();
$prfref->carregarAtual();

/**
 * Cabeçalho padrão do SIMEC.
 * @uses cabecalho.inc
 */
require_once(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
.label-td{text-align:right;font-weight:bold}
</style>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li>
            <a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisdsc']; ?></a>
        </li>
        <li class="active">Configuração da PLOA</li>
    </ol>
<?
$panel = <<<HTML
<table class="table table-bordered">
    <tbody>
        <tr>
            <td class="label-td" style="width:25%">Exercício:</td>
            <td colspan="2">{$_SESSION['exercicio']}</td>
        </tr>
        <tr>
            <td class="label-td">Período:</td>
            <td>{$prfref}</td>
            <td style="width:5%">
                <button class="btn btn-success btn-visualizar-principal"><span class="glyphicon glyphicon-eye-open"></span></button>
            </td>
        </tr>
    </tbody>
</table>
HTML;
bootstrapPanel('Informações do período', $panel);
echo $fm;

$sql = $prfref->queryTodosPeriodosCombo();
$config = array(
    'flabel' => 'Período de origem',
    'titulo' => 'Selecione um período de referência de origem'
);
$form = new Simec_View_Form('configuracao');
$form->addInputCombo('prfidorigem', $sql, null, 'prfidorigem', $config)
    ->addHidden('prfiddestino', $prfref->prfid, 'prfiddestino')
    ->addBotoes(array('visualizar', 'copiar'))->render();

// -- popup de adição de novos itens
bootstrapPopup(
    'Configuração do período',
    'mdl-config',
    '',
    array('fechar'),
    array('tamanho' => 'lg')
);
?>
</div>
<script type="text/javascript">
function visualizarConfigPeriodo(prfid){
    var data = {
        'configuracao[prfid]': prfid,
        requisicao: 'visualizarConfigPeriodo'
    };

    $.get(window.location.href, data, function(html){
        $('#mdl-config .modal-body').empty().html(html);
        $('#mdl-config').modal();
    }, 'html');
}

$(function(){
    $('.btn-visualizar').click(function(){
        var prfid = $('#prfidorigem').val();
        if (!prfid) {
            bootbox.alert('Selecione um período de referência para visualização.');
            return false;
        }
        visualizarConfigPeriodo(prfid);
    });

    $('.btn-visualizar-principal').click(function(){
        var prfid = $('#prfiddestino').val();
        if (!prfid) {
            bootbox.alert('Não há nenhum período de referência aberto.');
            return false;
        }
        visualizarConfigPeriodo(prfid);
    });
});
</script>