<?php
/**
 * Gestão de relatórios do SIAFI.
 *
 * @version $Id: listarns.inc 102660 2015-09-21 19:24:27Z maykelbraz $
 */
include '_funcoesbasesiafi.php';
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    $dados = $_REQUEST;
    switch ($requisicao) {
        case 'verDetalheNs':
            retornaModalDetalheNs($dados);
            break;
    }
}

/**
 * Cabeçalho padrão do Simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$fm = new Simec_Helper_FlashMessage('siafi/principal/dadossiafi/listarns');
$bc = new Simec_View_Breadcrumb('C');
$bc->add('Dados do SIAFI')
        ->add('Listar NS')
        ->render();
?>

<script type="text/javascript" language="javascript">
    $(document).ready(function () {

    });

    /* Botão Ver NS*/
    function verNs(nscod)
    {
        var conteudo = "Regsitro não Encontrado";
        $.ajax({
            url: "siafi.php?modulo=principal/dadossiafi/listarns&acao=A&requisicao=verDetalheNs&nscod=" + nscod,
        }).done(function (html) {
            conteudo = html;
            $('.modal-ver-ns-body').html(conteudo);
        });
        $('#modal-ver-ns').modal();
    }

    /* Botão limpar */
    function reload()
    {
        window.location = 'siafi.php?modulo=principal/dadossiafi/listarns&acao=A';
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
$form->addInputCombo('ungcod', $sqlUgCombo, $dados['ungcod'], 'ungcod', array('flabel' => 'UG de Origem'))
    ->addInputTexto('nscod', $dados['nscod'], 'nscod', '50', false, array('flabel' => 'Número da NE'))
    ->addInputTexto('cpfcnpjfavorecido', $dados['cpfcnpjfavorecido'], 'cpfcnpjfavorecido', '20', false, array('flabel' => 'CPF ou CNPJ de Destino'))
    ->addInputData('datainicio', $dados['datainicio'], 'datainicio', array('flabel' => 'Data Inicial'))
    ->addInputData('datafim', $dados['datafim'], 'datafim', array('flabel' => 'Data Final'))
    ->addBotoes(array('limpar', 'buscar'))
    ->setRequisicao('filtrar')
    ->render();
/*
 * Listagem
 */
$dados['paginacao']['pagina'] = $_POST['listagem']['p'];
retornaListaNs($dados);
?>
<div class="modal fade" id="modal-ver-ns">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da NE</h4>
            </div>
            <div class="modal-ver-ns-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>