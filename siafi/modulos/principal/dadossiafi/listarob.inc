<?php
/**
 * Gestão de relatórios do SIAFI.
 *
 * @version $Id: listarob.inc 102472 2015-09-15 20:49:29Z werteralmeida $
 */
include '_funcoesbasesiafi.php';
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    $dados = $_REQUEST;
    switch ($requisicao) {
        case 'verDetalheOb':
            retornaModalDetalheOb($dados);
            break;
        case 'verDetalheNe':
            retornaModalDetalheNe($dados);
            break;
    }
}

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$fm = new Simec_Helper_FlashMessage('siafi/principal/dadossiafi/listarob');
$bc = new Simec_View_Breadcrumb('C');
$bc->add('Dados do SIAFI')
        ->add('Listar OB')
        ->render();
?>

<script type="text/javascript" language="javascript">
    $(document).ready(function () {

    });

    /* Botão Ver OB*/
    function verOb(obcod)
    {
        var conteudo = "Regsitro não Encontrado";
        $.ajax({
            url: "siafi.php?modulo=principal/dadossiafi/listarob&acao=A&requisicao=verDetalheOb&obcod=" + obcod,
        }).done(function (html) {
            conteudo = html;
            $('.modal-ver-ob-body').html(conteudo);
        });
        $('#modal-ver-ob').modal();
    }
    /* Botão Ver NE*/
    function verNe(necod)
    {
        var conteudo = "Regsitro não Encontrado";
        $.ajax({
            url: "siafi.php?modulo=principal/dadossiafi/listarob&acao=A&requisicao=verDetalheNe&necod=" + necod,
        }).done(function (html) {
            conteudo = html;
            $('.modal-ver-ne-body').html(conteudo);
        });
        $('#modal-ver-ne').modal();
    }



    /* Botão limpar */
    function reload()
    {
        window.location = 'siafi.php?modulo=principal/dadossiafi/listarob&acao=A';
    }
</script>
<?php
$dados = $_REQUEST['filtrar'];
/*
 * SQLs das Combos
 */
$sqlUoCombo = retornaSqlUoComboCompleta();
$sqlUgCombo = retornaSqlUgComboCompleta($dados['unicod']);

/* Tratando valores padrão de tela */
if ($dados['datainicio'] == '') {
    $dados['datainicio'] = '01' . strftime('/%m/%Y', strtotime('today'));
}
if ($dados['datafim'] == '') {
    $dados['datafim'] = strftime('%d/%m/%Y', strtotime('today'));
}

/*
 * Formulário de Filtros
 */
$form = new Simec_View_Form('filtrar');
/* ->addInputCombo('unicod', $sqlUoCombo , $dados['unicod'], 'unicod', array('flabel' => 'UO de Origem')) */
$form
        ->addInputCombo('ungcod', $sqlUgCombo, $dados['ungcod'], 'ungcod', array('flabel' => 'UG de Origem'))
        ->addInputTexto('obcod', $dados['obcod'], 'obcod', '50', false, array('flabel' => 'Número da OB'))
        ->addInputTexto('cpfcnpjfavorecido', $dados['cpfcnpjfavorecido'], 'cpfcnpjfavorecido', '20', false, array('flabel' => 'CPF ou CNPJ de Destino'))
        ->addInputData('datainicio', $dados['datainicio'], 'datainicio', array('flabel' => 'Data Inicial'))
        ->addInputData('datafim', $dados['datafim'], 'datafim', array('flabel' => 'Data Final'))
        ->addBotoes(array('limpar', 'buscar'))
        ->setRequisicao('filtrar')
        ->render();
/*
 * Listagem
 */
retornaListaOb($dados);
?>
<div class="modal fade" id="modal-ver-ob">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da OB</h4>
            </div>
            <div class="modal-ver-ob-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<div class="modal fade" id="modal-ver-ne">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da NE</h4>
            </div>
            <div class="modal-ver-ne-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>