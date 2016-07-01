<?php
switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

if(!empty($_POST)){
    $desbloqueio = new Desbloqueio();
    $_POST['usucpf'] = $_SESSION['usucpf'];
    $desbloqueio->popularDadosObjeto($_POST);
    $desbloqueio->salvar();
    $desbloqueio->commit();

    if(!$_POST['dbqid'])
        die("<script>
                    alert('Desbloqueio inserido com sucesso!');
                    window.location = '?modulo=principal/listaDesbloqueio&acao={$_GET['acao']}';
                 </script>");
    else
        die("<script>
                    alert('Desbloqueio alterado com sucesso!');
                    window.location = '?modulo=principal/listaDesbloqueio&acao={$_GET['acao']}';
                 </script>");
}

if($_REQUEST['dbqid']){
    $desbloqueio = new Desbloqueio($_REQUEST['dbqid']);
    extract($desbloqueio->getDados());
}

/**
 * @package name
 * @subpackage name
 *
 */
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('Cadastro de Desbloqueio Estadual/Municipal', '');

global $db;

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="form_save" id="form_save" action="" class="formulario" enctype="multipart/form-data">

    <input type="hidden" name="xls" id="xls" />

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

        <tr>
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <?php
                $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                    Array('codigo' => 'M', 'descricao' => 'Municipal'));
                $db->monta_combo('dbqesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'S', 'dbqesfera');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">UF:</td>
            <td>
                <?php
                $uf = new Estado();
                $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'S', 'estuf');
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

        <tr>
            <td class="SubTituloDireita" width="20%">Justificativa:</td>
            <td><?= campo_textarea('dbqjustificativa', 'S', 'S', 'Justificativa', 98, 5, 4000, '', '', '', '', 'Justificativa'); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Observação:</td>
            <td><?= campo_textarea('dbqobservacao', 'N', 'S', 'Observação', 98, 5, 4000, '', '', '', '', 'Observação'); ?></td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Data de Início:</td>
            <td><?
                $dbqdtinicio = formata_data(date('Y-m-d'));
                echo campo_data2('dbqdtinicio', 'S', 'S', '', 'N', '', '', $dbqdtinicio, '', '', 'dbqdtinicio');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Data de Término:</td>
            <td><?
                $dbqdttermino = formata_data(date('Y-m-d'));
                echo campo_data2('dbqdttermino', 'S', 'S', '', 'N', '', '', $dbqdttermino, '', '', 'dbqdttermino');?></td>
        </tr>
        </tr>
        <tr bgcolor="#dcdcdc">
            <td  colspan="2" style="text-align:center">
                    <input id="button_save" type="submit" value="Salvar"/>
            </td>
        </tr>
    </table>
</form>


<script type="text/javascript">

    $(function(){

        $('#dbqesfera').change(function(){
            if($('#dbqesfera').val() == 'M'){
                $('#muncod').removeAttr('disabled');
            } else {
                $('#muncod').attr('disabled','disabled');
                $('#muncod').val('');
            }
        });

        $('#dbqesfera').change();

        $('#form_save').submit(function(){

            var msg = [];
            if($('#dbqesfera').val() == ''){
                msg.push('Seleciona a esfera.')
            }

            if($('#estuf').val() == ''){
                msg.push('Seleciona uma UF.')
            }

            if($('#dbqjustificativa').val() == ''){
                msg.push('A justificativa não pode estar vazia.')
            }

            if($('#dbqesfera').val() == 'M' && $('#muncod').val() == ''){
                msg.push('Selecione o município.')
            }

            if(msg.length > 0){
                alert(msg.join('\n'));
                return false;
            }
        });
    })

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
        $('#dbqesfera').change();
    }
</script>