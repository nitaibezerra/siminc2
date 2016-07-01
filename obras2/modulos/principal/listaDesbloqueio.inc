<?php
switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

if ($_POST['action'] == 'xls'){
    ob_clean();
    $desbloqueio = new Desbloqueio();
    ini_set("memory_limit", "512M");
    header('content-type: text/html; charset=ISO-8859-1');
    $sql = $desbloqueio->listaSql($_REQUEST);
    $cabecalho = array('Esfera', 'Estado', 'Municipio', 'Justificativa', 'Inserido por', 'Data de início', 'Data término');
    $db->sql_to_excel($sql, 'relatorioListaDeBloqueios', $cabecalho, '');
    exit;
}

if ($_POST['action'] == 'delete') {
    $desbloqueio = new Desbloqueio($_POST['dbqid']);
    $desbloqueio->excluir($_POST['dbqid']);
    $desbloqueio->commit();
    die("<script>
				alert('Excluido com sucesso!');
				window.location = '?modulo=principal/listaDesbloqueio&acao={$_GET['acao']}';
			 </script>");
}

/**
 * @package name
 * @subpackage name
 *
 */
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('Desbloqueio Estadual/Municipal', '');

global $db;

extract($_POST);
?>
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<form method="post" name="form_list" id="form_list" action="" class="formulario" enctype="multipart/form-data">

    <input type="hidden" name="dbqid" id="dbqid" value=""/>
    <input type="hidden" name="action" id="action" value=""/>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

        <tr>
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <?php
                $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                    Array('codigo' => 'M', 'descricao' => 'Municipal'));
                $db->monta_combo('dbqesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'dbqesfera');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">UF:</td>
            <td>
                <?php
                $uf = new Estado();
                $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Município:</td>
            <td id="td_municipio">
                <?php
                if ($estuf) {
                    $municipio = new Municipio();
                    $dado = $municipio->listaCombo(array('estuf' => $estuf));
                    $habMun = 'S';
                } else {
                    $dado = array();
                    $habMun = 'N';
                }
                $habMun = ($disable == 'N' ? $disable : $habMun);
                echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
                ?>
            </td>
        </tr>

        <tr bgcolor="#dcdcdc">
            <td colspan="2" style="text-align:center">
                <input id="button_save" type="submit" value="Filtrar"/>
                <input type="submit" name="btnEexcel" class="btnEexcel" id="btnEexcel" value="Gerar Excel"/>
            </td>
        </tr>
    </table>
</form>

<?php
    $desbloqueio = new Desbloqueio();
    $sql = $desbloqueio->listaSql($_REQUEST);
    $cabecalho = array('Ação', 'Esfera', 'Estado', 'Municipio', 'Justificativa', 'Inserido por', 'Data de início', 'Data término');
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'S', "formulario");
?>
<table>
    <tr>
        <td>

        </td>
    </tr>
</table>

<script type="text/javascript">
    $(function () {
        $('#btnEexcel').click(function () {
            $('#action').val('xls');
        });
    });

    function desbloqueio(dbqid) {
        location.href = '?modulo=principal/cadDesbloqueio&acao=A&dbqid=' + dbqid;
    }
    function excluirDesbloqueio(dbqid) {
        console.info(dbqid);
        $('#action').val('delete');
        $('#dbqid').val(dbqid);
        $('#form_list').submit();
    }

    function carregarMunicipio(estuf) {
        var td = $('#td_municipio');
        if (estuf != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'municipio',
                    estuf: estuf},
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                },
                success: function (data) {
                    console.info(data);
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                .attr('disabled', true);
        }
    }
</script>
