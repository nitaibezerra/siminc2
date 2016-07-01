<?php
//Chamada de programa
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

monta_titulo('Lista de pedidos', $linha2);

if ($_POST) {
    extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>

<form method="post" name="formulario" id="formulario" action="">
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
        <tr>
            <td class="subtituloDireita">Unidade</td>
            <td>
                <?php
                $sql = "
                        Select
                            unicod as codigo, 
                            unidsc as descricao
                        From public.unidade
                            Inner Join sic.secretaria using(unicod)
                        group by 1,2
                        order by 2
                    ";

                $db->monta_combo('unicod', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <!--
                <tr>
                    <td class="subtituloDireita">Secretaria</td>
                    <td id="td_secretaria">
        <?php
        $sql = array();

        $sql = "
                    SELECT
                        s.secid as codigo,
                        s.secdsc as descricao
                    FROM
                        sic.secretaria s
                    JOIN public.unidade u using(unicod)
                    JOIN public.orgao o using(orgcod)
                    GROUP BY
                        1,2
                    ORDER BY
                        2
                    ";

        $db->monta_combo('secid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
        ?>
                    </td>
                </tr>
        -->
        <tr>
            <td class="subtituloDireita">Período de inclusão</td>
            <td>
                <?php echo campo_data('slcdtinclusaoinicio', 'N', 'S', '', 'N'); ?>
                &nbsp;ate&nbsp;
                <?php echo campo_data('slcdtinclusaofim', 'N', 'S', '', 'N'); ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda"></td>
            <td class="subtituloEsquerda">
                <input type="submit" value="Pesquisar" />
                <input type="button" value="Limpar" id="bt_limpar" />
            </td>
        </tr>
    </table>
</form>
<?php
$arWhere = array();
$arWhereQuantitativos = array();

if ($_REQUEST['slcdtinclusaoinicio'] || $_REQUEST['slcdtinclusaofim']) {

    $_REQUEST['slcdtinclusaofim'] = $_REQUEST['slcdtinclusaofim'] ? formata_data_sql($_REQUEST['slcdtinclusaofim']) : '';
    $_REQUEST['slcdtinclusaoinicio'] = $_REQUEST['slcdtinclusaoinicio'] ? formata_data_sql($_REQUEST['slcdtinclusaoinicio']) : '';

    if ($_REQUEST['slcdtinclusaoinicio'] && $_REQUEST['slcdtinclusaofim']) {

        $arWhereQuantitativos[] = "slcdtinclusao BETWEEN '{$_REQUEST['slcdtinclusaoinicio']}' AND '{$_REQUEST['slcdtinclusaofim']}'";
    } else if ($_REQUEST['slcdtinclusaoinicio']) {

        $arWhereQuantitativos[] = "slcdtinclusao = '{$_REQUEST['slcdtinclusaoinicio']}'";
    } else if ($_REQUEST['slcdtinclusaofim']) {

        $arWhereQuantitativos[] = "slcdtinclusao = '{$_REQUEST['slcdtinclusaofim']}'";
    }
}

if ($_REQUEST['unicod']) {
    $arWhere[] = "s.unicod = '{$_REQUEST['unicod']}'";
//    $arWhere[] = "(case when sc.unicod is null then sec.unicod else sc.unicod end) = '{$_REQUEST['unicod']}'";
}

$sql = "
    SELECT DISTINCT
        s.secdsc AS descricao,
        (
        SELECT
            COUNT(sc.slcid)
        FROM
            sic.solicitacao sc
        WHERE
            sc.secid = s.secid
            " . ( count($arWhereQuantitativos) ? ' AND ' . implode(' AND ', $arWhereQuantitativos) : '') . "
        ) AS valor
    FROM
        sic.secretaria s
    WHERE
        1 = 1
    " . ( count($arWhere) ? ' AND ' . implode(' AND ', $arWhere) : '') . "
    ORDER BY
        valor DESC
";

$listaPedidos = $db->carregar($sql);

$cabecalho = array('Secretaria', 'Número de Demandas');
$db->monta_lista($listaPedidos, $cabecalho, 10000, 10, 'S', '', '', '', '');

if ($_SESSION['sic']['slcid'])
    unset($_SESSION['sic']['slcid']);
?>

<?php
$grafico = new Grafico();
$grafico->gerarGrafico($listaPedidos);
?>

<script>
    $(function() {

        $('#bt_limpar').click(function() {
            document.location.href = 'sic.php?modulo=relatorio/relatorio-pedidos&acao=A';
        });

    });
</script>
