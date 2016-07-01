<?php
$arrCabecalho = array("Ação", "Empresa", "Estado", "Município", "Situação do Pagamento", "Número da OS", "Número da Nota Fiscal", "Ordem Bancária", "Data de Solicitação do Pagamento", "Valor Total", "Data de Envio");
if($_POST['xls']){
    $pagamento = new Pagamento();
    $sql = $pagamento->listaSql($_POST);
    unset($arrCabecalho[0]);
    $db->sql_to_xml_excel($sql, 'listaPagamentos', $arrCabecalho);
}

if ($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']) {
    $n = new $_REQUEST['classe'];
    $n->$_REQUEST['requisicaoAjax']();
    die;
}
if ($_REQUEST['requisicao'] && $_REQUEST['classe']) {
    $n = new $_REQUEST['classe'];
    $n->$_REQUEST['requisicao']();
}
if ($_POST['requisicaoAjax']) {

    if($_POST['requisicaoAjax'] == 'carregaEmpresaAndListaObra' && $_POST['sgrid'] == 'null'){
        $grupoEmpresa = new Supervisao_Grupo_Empresa();
        $dados = $grupoEmpresa->listComboEmpresas();
        $db->monta_combo("entid", $dados, 'S', "Selecione...", '', '', '', '', 'S', 'entid');
        exit;
    } else {
        $_POST['requisicaoAjax']();
    }

    die;
}

if ($_GET['arquivo']) {
    include_once APPRAIZ . "includes/classes/file.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $file->getDownloadArquivo($_GET['arquivo']);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

//criaAbaPagamento();
$db->cria_aba($abacod_tela, $url, $parametro);
monta_titulo("Lista de Pagamentos", '');

if ($_POST) {
    extract($_POST);
    $sgeid = $_POST['sgeid_disable'] ? $_POST['sgeid_disable'] : $_POST['sgeid'];
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    function carregaDependenciaGrupo(sgrid) {
        if (sgrid) {
            $.post("?modulo=principal/listaPagamento&acao=A", {"sgrid": sgrid, "requisicaoAjax": "carregaEmpresaAndListaObra", "not(listaObras)": true}, function(data) {
                var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
                $('#tdComboEmpresa').html(comboEmpresa);
            });
        } else {
            $.post("?modulo=principal/listaPagamento&acao=A", {"sgrid": null, "requisicaoAjax": "carregaEmpresaAndListaObra", "not(listaObras)": true}, function(data) {
                $('#tdComboEmpresa').html(data);
            });
        }
    }
    function carregaDependenciaEmpresa(sgeid) {

        if (sgeid) {
            $.ajax({
                type: "POST",
                url: window.location,
                data: "requisicaoAjax=carregarSupervisaoPorEmpresa&classe=Pagamento&sgeid=" + sgeid,
                success: function(msg) {
                    $('#div_lista_supervisao').html(msg);
                }
            });
        } else {
            $('#div_lista_supervisao').html('Selecione a Empresa.');
        }
    }

    function pesquisarPagamentos()
    {
        $('#formulario_busca').submit();
    }

    function gerarXls()
    {
        $('#formulario_busca input#xls').val('true');
        $('#formulario_busca').submit();
    }

    function limparPagamentos()
    {
        window.location.href = window.location;
    }

    function wf_exibirHistorico(docid)
    {
        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
                '?modulo=principal/tramitacao' +
                '&acao=C' +
                '&docid=' + docid;
        window.open(
                url,
                'alterarEstado',
                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
                );
    }

    $(function() {
    <?php if ($_SESSION['obras2']['pagamento']['alert']): ?>
                alert('<?php echo $_SESSION['obras2']['pagamento']['alert'] ?>');
    <?php unset($_SESSION['obras2']['pagamento']['alert']) ?>
    <?php endif; ?>
    });

    function downloadNotaFiscal(arqid) {
        window.location.href = 'obras2.php?modulo=principal/listaPagamento&acao=A&arquivo=' + arqid;
    }

    function abrirPagamento(pagid)
    {
        window.location.href = 'obras2.php?modulo=principal/pagamento&acao=A&pagid=' + pagid;
    }
    function excluirPagamento(pagid)
    {
        if (confirm("Deseja realmente excluir o Pagamento?"))
        {
            $("#pagid").val(pagid);
            $("#requisicao").val("excluirPagamento");
            $("#classe").val("Pagamento");
            $('#formulario_busca').submit();
        }
    }
    function realizarPagamento(pagid)
    {
        window.location.href = 'obras2.php?modulo=principal/realizarPagamento&acao=A&pagid=' + pagid;
    }
</script>
<style>.link{cursor:pointer}</style>
<form id="formulario_busca" name="formulario_busca" method="post" action="">
    <input type="hidden" name="requisicao" id="requisicao" value="" />
    <input type="hidden" name="classe" id="classe" value="" />
    <input type="hidden" name="pagid" id="pagid" value="" />
    <input type="hidden" name="xls" id="xls" value="" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td class="SubTituloDireita" width="35%">Grupo</td>
            <td>
                <?php
                $grupo = new Supervisao_Grupo();
                $dados = $grupo->listaCombo();

                $db->monta_combo("sgrid", $dados, 'S', "Selecione...", "carregaDependenciaGrupo", '', '', '', 'N', 'sgrid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Empresa</td>
            <td id="tdComboEmpresa">
                <?php
                if (!empty($sgrid)) {
                    $grupoEmpresa = new Supervisao_Grupo_Empresa();
                    $dados = $grupoEmpresa->listaCombo(array('sgrid' => $sgrid));

                    $db->monta_combo("sgeid", $dados, 'S', "Selecione...", "carregaDependenciaEmpresa", '', '', '', 'S', 'sgeid');
                } else {
                    $grupoEmpresa = new Supervisao_Grupo_Empresa();
                    $dados = $grupoEmpresa->listComboEmpresas();
                    $db->monta_combo("entid", $dados, 'S', "Selecione...", '', '', '', '', 'S', 'entid');
                }
                ?>
            </td>
        </tr>
        <?php
        montaComboTerritorio("listapagamento", array("estuf" => array("obrigatorio" => "N", "value" => $_POST['listapagamentoestuf']), "mescod" => array("obrigatorio" => "N", "disabled" => "S", "value" => $_POST['listapagamentomescod']), "miccod" => array("disabled" => "S", "value" => $_POST['listapagamentomiccod']), "muncod" => array("disabled" => "S", "value" => $_POST['listapagamentomuncod'])));
        ?>
        <tr>
            <td class="SubTituloDireita">Situação do Pagamento:</td>
            <td>
                <?php
                criaComboWorkflow(WF_TPDID_PAGAMENTO_SUPERVISAO, array("nome" => "pagidsituaco", "id" => "pagidsituaco"));
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Número da OS:</td>
            <td>
                <?php
                echo campo_texto('sosnum', 'N', 'S', '', 11, 30, '################', '', 'right', '', 0, '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Número da Nota Fiscal:</td> 
            <td>
                <?php echo campo_texto('pagnrnotafiscal', 'N', 'false', '', 17, 35, '###########', '', 'left', '', 0, 'id="pagnrnotafiscal"', '', null, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">ID da obra:</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'false', '', 17, 35, '###########', '', 'left', '', 0, '', '', null, ''); ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">
                <input type="button" value="Pesquisar" name="btn_pesquisar" onclick="pesquisarPagamentos();">
                <input type="button" value="Gerar XLS"    name="btn_xls"    onclick="gerarXls()">
                <input type="button" value="Limpar"    name="btn_limpar"    onclick="limparPagamentos()">
            </td>
        </tr>
    </table>
</form>
<?php
$pagamento = new Pagamento();
$sql = $pagamento->listaSql($_POST);

$db->monta_lista($sql, $arrCabecalho, 100, 10, "N", "center", "N");
?>