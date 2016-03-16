<?php
/**
 * Arquivo de consulta das informações de captação da PLOA.
 *
 * $Id: obterInformacaoCaptacaoPLOA.inc 101245 2015-08-13 19:35:06Z maykelbraz $
 * @filesource
 */
$fm = new Simec_Helper_FlashMessage('spo/integracao/infocaptacao');
if (!empty($_POST) && isset($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    $dados = $_POST['infocaptacao'];

    switch ($requisicao) {
        case 'carregar':
            obterInformacaoCaptacaoPLOA($dados, $fm);
            break;
    }
//    header('Location: ' . $_SERVER['REQUEST_URI']);
//    die();
}

/**
 * Cabeçalho padrão do Simec.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
function onInfocaptacaoSubmit()
{
    if ('' === $('#infocaptacao_codigoMomento').val()) {
        alert('O código do momento não pode ser deixado em branco.');
        return false;
    }
    return true;
}
</script>
<div class="row col-md-12">
<?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integração')
        ->add('SOF Quantitativo')
        ->add('Obter Informações de Captação da PLOA')
        ->render();

    echo $fm;

    $form = new Simec_View_Form('infocaptacao');
    $form->carregarDados($_POST['infocaptacao'])
        ->addCombo('Momento', 'codigomomento', Spo_Model_Momento::queryCombo())
        ->addHidden('exercicio', $_SESSION['exercicio'])
        ->addHidden('codigoOrgao', Spo_Model_Unidade::CODIGO_ORGAO)
        ->addTexto('Cód. Tipo Detalhamento', 'codigoTipoDetalhamento', 2)
        ->addCombo('Unidade Orçamentária', 'codigounidadeorcamentaria', Spo_Model_Unidade::queryCombo())
        ->addChoices('Captados', 'captados', array('Todos' => null, 'Sim' => 't', 'Não' => 'f'))
        ->addChoices('Captáveis', 'captaveis', array('Todos' => null, 'Sim' => 't', 'Não' => 'f'))
        ->addCheckbox('Gravar log', 'loggar')
        ->addCheckbox('Limpar tabela', 'limpar')
        ->addBotoes(array('executar'))
        ->setRequisicao('carregar')
        ->render();
    ?>
</div>
