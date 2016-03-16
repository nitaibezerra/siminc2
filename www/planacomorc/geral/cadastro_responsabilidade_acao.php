<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

function gravarResponsabilidadeAcao($dados) {
    global $db;
    $sql = "UPDATE planacomorc.usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='" . $dados['usucpf'] . "' AND pflcod='" . $dados['pflcod'] . "' AND id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}";
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach ($dados['usuacaresp'] as $id_acao_programatica) {
            $sql = "
                INSERT INTO planacomorc.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, id_acao_programatica, id_periodo_referencia)
                    VALUES ('" . $dados['pflcod'] . "', '" . $dados['usucpf'] . "', 'A', NOW(), '" . $id_acao_programatica . "', {$_REQUEST['id_periodo_referencia']});";

            $db->executar($sql);
        }
    }

    $db->commit();

    echo "
        <script language=\"javascript\">
            alert(\"Operação realizada com sucesso!\");
            opener.location.reload();
            self.close();
        </script>";
}

function carregarAcoesUnidade($dados) {
    global $db;

    $sql = "
        select distinct
            '<input type=\"checkbox\" name=\"id_acao_programatica\" id=\"chk_'||apr.id_acao_programatica||'\" value=\"'||apr.id_acao_programatica||'\" onclick=\"marcarAcao(this);\">' as acao,
            prg.codigo || '.' || aca.codigo || '.' || org.codigo || ' ('|| apr.id_exercicio ||')' as descricao
        from planacomorc.acao aca
        join planacomorc.acao_programatica apr on apr.id_acao = aca.id_acao
        join planacomorc.dados_acao_exercicio dae on dae.id_acao = aca.id_acao and dae.id_exercicio=apr.id_exercicio
        join planacomorc.orgao org on org.id_orgao = apr.id_orgao
        join planacomorc.programa prg on prg.id_programa =  aca.id_programa
        join planacomorc.localizador_programatica lpr using(id_acao_programatica)
        join planacomorc.snapshot_dotacao_localizador_programatica sdlp ON sdlp.id_localizador_programatica = lpr.id_localizador_programatica
        where org.codigo='" . $dados['orgcod'] . "'
            AND apr.id_exercicio = {$_SESSION['exercicio']}
            and sdlp.id_periodo_referencia = (SELECT id_periodo_referencia AS codigo FROM planacomorc.periodo_referencia p WHERE id_exercicio = '{$_SESSION['exercicio']}'
            ORDER BY id_periodo_referencia desc LIMIT 1) ORDER BY descricao";
    $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '100%', 'N');
}

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

$sql = "
    select
        id_periodo_referencia as codigo,
        titulo || ' - ' || to_char(inicio_validade, 'DD/MM/YYYY') || ' a ' || to_char(fim_validade, 'DD/MM/YYYY') as descricao
    from planacomorc.periodo_referencia
    where id_exercicio = {$_SESSION['exercicio']}
    order by id_exercicio desc, inicio_validade desc, fim_validade desc";
$periodos = $db->carregar($sql);
?>
<html>
    <head>
        <META http-equiv="Pragma" content="no-cache">
        <title>Atribuir Ações</title>
        <script language="JavaScript" src="/includes/funcoes.js"></script>
        <script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
        <script src="../js/planacomorc.js"></script>
        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
    </head>
    <body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" onload="self.focus()">
        <script>

            jQuery(document).ready(function() {

                jQuery('#id_periodo_referencia').change(function() {

                    url = 'cadastro_responsabilidade_acao.php?pflcod=' + jQuery('[name=pflcod]').val();
                    url += '&usucpf=' + jQuery('[name=usucpf]').val();
                    url += '&id_periodo_referencia=' + jQuery(this).val()

                    document.location.href = url;
                });

            });

            function carregarAcoesUnidade(orgcod, obj) {
                var tabela = obj.parentNode.parentNode.parentNode;
                var linha = obj.parentNode.parentNode;
                if (obj.title == "mais") {
                    obj.title = "menos";
                    obj.src = "../../imagens/menos.gif";
                    var nlinha = tabela.insertRow(linha.rowIndex + 1);
                    var ncol = nlinha.insertCell(0);
                    ncol.colSpan = 8;
                    ncol.id = 'tr_' + nlinha.rowIndex;
                    ajaxatualizar('requisicao=carregarAcoesUnidade&orgcod=' + orgcod, ncol.id);
                    jQuery("#usuacaresp option").each(function() {
                        if (jQuery("#chk_" + jQuery(this).val()).length) {
                            jQuery("#chk_" + jQuery(this).val()).attr('checked', true);
                        }
                    });
                } else {
                    obj.title = "mais";
                    obj.src = "../../imagens/mais.gif";
                    tabela.deleteRow(linha.rowIndex + 1);
                }

            }

            function marcarAcao(obj) {

                var periodo = document.getElementById('id_periodo_referencia');

                if (periodo.value == '') {
                    obj.checked = false;
                    alert('Escolha um Período antes!');
                    periodo.focus();
                    return false;
                }

                if (obj.checked) {
                    if (!jQuery('#usuacaresp option[value=' + obj.value + ']')[0]) {
                        jQuery("#usuacaresp").append('<option value=' + obj.value + '>' + obj.parentNode.parentNode.cells[1].innerHTML + '</option>');
                    }
                } else {
                    jQuery('#usuacaresp option[value=' + obj.value + ']').remove();
                }
            }

            function enviarFormulario()
            {

                var periodo = document.getElementById('id_periodo_referencia');

                if (periodo.value == '') {
                    alert('O campo Período é obrigatório!');
                    periodo.focus();
                    return false;
                }

                selectAllOptions(document.getElementById('usuacaresp'));
                document.formassocia.submit();
            }
        </script>
        <div style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
            <?
            $sql = "
                SELECT
                    '<img src=\"/imagens/mais.gif\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" title=\"mais\" vspace=\"3\" id=\"img'||codigo||'\" style=\"cursor:pointer;\" onclick=\"carregarAcoesUnidade('||codigo||',this);\">' as acao,
                    codigo,
                    descricao
		FROM planacomorc.orgao
            ";

            $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N', '100%', 'N');
            ?>
        </div>
        <?php
        $codigo_periodo_selecionado = ($_GET['id_periodo_referencia'] ? $_GET['id_periodo_referencia'] : $periodos[0]["codigo"]);

        $sql = "
            SELECT
                DISTINCT
                apr.id_acao_programatica as codigo,
                prg.codigo || '.' || aca.codigo || '.' || org.codigo || ' ('|| apr.id_exercicio ||')' as descricao,
                u.id_periodo_referencia
            FROM planacomorc.usuarioresponsabilidade u
            join planacomorc.acao_programatica apr on apr.id_acao_programatica = u.id_acao_programatica
            join planacomorc.acao aca on aca.id_acao = apr.id_acao
            join planacomorc.orgao org on org.id_orgao = apr.id_orgao
            join planacomorc.programa prg on prg.id_programa =  aca.id_programa AND apr.id_exercicio = {$_SESSION['exercicio']}
            join planacomorc.snapshot_dotacao_localizador_programatica sdlp ON sdlp.id_localizador_programatica = apr.id_acao_programatica
            WHERE rpustatus='A' AND usucpf = '$usucpf' AND u.pflcod=$pflcod" . ( $codigo_periodo_selecionado ? " AND u.id_periodo_referencia = {$codigo_periodo_selecionado} " : " AND u.id_periodo_referencia IS NULL ");

        $usuarioresponsabilidade = $db->carregar($sql);
        ?>
        <form name="formassocia" style="margin:0px;" method="POST">
            <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
                <tr>
                    <td style="padding:3px;" class="subtituloDireita">
                        Período:
                    </td>
                    <td style="padding:3px;" align="left">
                        <?php
                        $id_periodo_referencia = $_GET['id_periodo_referencia'];
                        if(!$periodos) $periodos = array();
                        $db->monta_combo('id_periodo_referencia', $periodos, 'S', NULL, '', '', '', 260, 'N', 'id_periodo_referencia', '', '', '');
                        ?>
                    </td>
                </tr>
            </table>
            <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
            <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
            <input type="hidden" name="requisicao" value="gravarResponsabilidadeAcao">
            <select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo">
                <?php

                if ($usuarioresponsabilidade[0]) {
                    foreach ($usuarioresponsabilidade as $ur) {
                        echo '<option value="' . $ur['codigo'] . '">' . $ur['descricao'] . '</option>';
                    }
                }
                ?>
            </select>
        </form>
        <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr bgcolor="#c0c0c0">
                <td align="right" style="padding:3px;" colspan="3">
                    <input type="Button" name="ok" value="OK" onclick="enviarFormulario();" id="ok">
                </td>
            </tr>
        </table>