<?php
/**
 * Entrada do cadastro de relatórios do TCU.
 *
 * @version $Id: listar_gestao.inc 86890 2014-09-17 14:27:17Z werteralmeida $
 */

/**
 * Funções de apoio aos sistemas SPO.
 * @see funcoesspo.php
 */
require_once(APPRAIZ . 'includes/funcoesspo.php');

/**
 * Gerenciador de mensagens entre requisições.
 * @see Simec_Helper_FlashMessage
 */
include_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('planacomorc/relgestao');

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
function tudoMaiusculo()
{
    var x = document.getElementById("acacod");
    x.value = x.value.toUpperCase();
}
function editarRelatorio(id)
{
    window.location.assign('planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&rlgid='+id);
}
function gerarPdfRelatorio(id)
{
    window.open('planacomorc.php?modulo=principal/relatoriogestao/relgestao&acao=A&pdf=true&rlgid='+id);
}
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Seleção de Ação</li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return validaCampo();">
                <div class="form-group">
                    <div class="col-lg-02">
                        <label for="unidade" class="col-lg-2 control-label">Unidade Orçamentária:</label>
                    </div>
                    <div class="col-lg-10">
                    <?php
                    $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unicod IN (SELECT DISTINCT unicod
                         FROM recorc.vinculacaoexercicio
                         WHERE exercicio = '{$_SESSION['exercicio']}')
    AND uni.unistatus = 'A'
    {$whereUO}
ORDER BY uni.unicod
DML;
                    $unicod = $_REQUEST['unicod'];
                    $db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($unicod) ? $unicod : null));
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-02">
                        <label for="acao" class="col-lg-2 control-label">Ação:</label>
                    </div>
                    <div class="col-lg-10">
                        <input type="text" value="" style="width:10%" class="CampoEstilo normal form-control obrigatorio"
                               name="acacod" id="acacod" id="acacod" maxlength="4" size="5" name="acacod" OnBlur="tudoMaiusculo()" />
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar()">Voltar</button>
                        <button class="btn btn-success" id="inserir" type="submit">
                            <span class="glyphicon glyphicon-search"></span> Pesquisar
                        </button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<div class="col-lg-12">
<?php
$select = <<<DML
SELECT rlg.rlgid,
       rlg.unicod || ' - ' || uni.unidsc AS unicod,
       rlg.acacod,
       TO_CHAR(rlg.dtultimaatualizacao, 'DD/MM/YYYY') AS dtultimaatualizacao,
       usu.usunome
  FROM planacomorc.relgestao rlg
    LEFT JOIN public.unidade uni USING(unicod)
    LEFT JOIN seguranca.usuario usu USING(usucpf)
  WHERE rlg.exercicio = '%s'
DML;
$stmt = sprintf($select, $_SESSION['exercicio']);

$cabecalho = array('Unidade Orçamentária', 'Ação', 'Data criação', 'Responsável');
$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho($cabecalho)
    ->addAcao('edit', 'editarRelatorio')
    ->addAcao('print', 'gerarPdfRelatorio')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>