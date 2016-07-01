<?php

$cpfs = array('','','','');

if (!in_array($_SESSION['usucpf'], $cpfs)) {
    echo '<script>
        alert("Voce não possuí permissão para acessar está tela!");
        location.href="elabrev.php?modulo=inicio&acao=C";
    </script>';
    exit;
}

if (isset($_GET['deleta'])) {

    //1--apaga todos os comentarios ate o momento selecionado
    $strSQL = "
        delete from workflow.comentariodocumento
        where docid = (select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']})
        and hstid > {$_GET['hstid']}
    ";
    $db->executar($strSQL);

    //2--retorna o termo para um determinado estado
    $strSQL = "
        update workflow.documento
        set esdid = 639, hstid = {$_GET['hstid']}
        where docid = (select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']})
    ";
    $db->executar($strSQL);

    //3--apaga historico apartir de um determinado ponto
    $strSQL = "
        delete from workflow.historicodocumento
        where docid = (select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']})
        and hstid > {$_GET['hstid']}
    ";
    $db->executar($strSQL);

    $db->commit();
    header('location:elabrev.php?modulo=principal/termoCooperacao/retornartermo&acao=A');
}


//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
monta_titulo(MODULO_NAME, 'Retornar situação do TED');
?>

<table id="form-momento" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
    <tr>
        <td width="40%" align="right">Número do Termo:</td>
        <td width="15%">
            <?php echo campo_texto('tcpid','S','S','',17,17,'','','left','','','id="tcpid"','',$_GET['tcpid']); ?>
        </td>
        <td align="right">Situação do Termo:</td>
        <td>
            <?php
            $strSQL = "
                select
                    aedid AS codigo, aedid ||'. '|| ed.esddsc ||' -> '|| aeddscrealizar as descricao
                from workflow.acaoestadodoc aed
                join workflow.estadodocumento ed on (ed.esdid = aed.esdidorigem)
                where esdidorigem in (
                    select esdid from workflow.estadodocumento where tpdid = 97 and esdstatus = 'A'
                ) and aedstatus = 'A' order by aedid asc
            ";

            echo $db->monta_combo('aedid', $strSQL, 'S', 'Selecione...', '', $opc, '', '250', 'S', 'aedid', '', $_GET['aedid'], $title=null);
            ?>
        </td>
    </tr>
    <tr>
        <td colspan="4" align="right">
            <input type="button" name="btnSearch" id="btnSearch" value="Pesquisar"/>
        </td>
    </tr>
    <?php if (isset($_GET['aedid']) && isset($_GET['tcpid'])) : ?>
    <tr>
        <td colspan="4" align="center">
            <h4>Histórico da situação selecionada</h4>
        </td>
    </tr>
    <tr>
        <td colspan="4">
            <?php
            $strSQL = "
                select
                    '<input type=\"radio\" name=\"hstid[]\" value=\"'||hstid||'\"/>' as acao,
                    hstid, aedid, docid, usucpf, htddata
                from workflow.historicodocumento
                where aedid = {$_GET['aedid']} and docid = (
                    select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']}
                ) order by hstid desc
            ";

            $cellWidth = array('5%','8%','5%','12%','15%','40%');
            $cellAlign = array();
            $cabecalho = array('Ação','hstid','aedid','docid','usucpf','htddata');
            $db->monta_lista($strSQL, $cabecalho, 50, 20, '', '', '', '', $cellWidth, $cellAlign);
            ?>
        </td>
    </tr>
    <tr>
        <td colspan="4" align="right">
            <input type="button" name="btnNext" id="btnNext" value="Avançar"/>
        </td>
    </tr>
    <?php endif; ?>
    <?php
    if (isset($_GET['hstid']) && !empty($_GET['hstid']) && is_numeric($_GET['hstid']) && $_GET['tcpid']) {

        echo '<tr>
                <td colspan="4" align="center">
                    <h4>Opções de situações para deletar</h4>
                </td>
            </tr>';

        $strSQL = "
            select
                --'<input type=\"checkbox\" name=\"history[]\" value=\"'||hstid||'\"/>' as acao,
                hstid, aedid, docid, usucpf, htddata
            from workflow.historicodocumento
            where docid = (select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']})
            and hstid > {$_GET['hstid']}
        ";
        $cellWidth = array('8%','5%','12%','15%','40%');
        $cellAlign = array();
        $cabecalho = array('hstid','aedid','docid','usucpf','htddata');

        echo '<tr><td colspan="4">';
        $db->monta_lista($strSQL, $cabecalho, 50, 20, '', '', '', '', $cellWidth, $cellAlign);
        echo '</td></tr>';


        $strSQL = "
            select
                --'<input type=\"checkbox\" name=\"cmdid[]\" value=\"'||cmdid||'\"/>' as acao,
                cmdid, docid, cmddsc, cmddata, hstid
            from workflow.comentariodocumento
            where docid = (select docid from monitora.termocooperacao where tcpid = {$_GET['tcpid']})
            and hstid > {$_GET['hstid']}
        ";
        $cellWidth = array('8%','5%','12%','15%','40%');
        $cellAlign = array();
        $cabecalho = array('cmdid','docid','cmddsc','cmddata','hstid');

        echo '<tr><td colspan="4">';
        $db->monta_lista($strSQL, $cabecalho, 50, 20, '', '', '', '', $cellWidth, $cellAlign);
        echo '</td></tr>';

        echo '<tr>
                <td colspan="4" align="right">
                    <input type="button" name="btnDelete" id="btnDelete" value="Apagar"/>
                </td>
            </tr>';
    }
    ?>
</table>

<script type="text/javascript">
jQuery(function(){
    jQuery("#btnSearch").click(function(){
        if (jQuery("#tcpid").val() && jQuery("#aedid").val()) {
            location.href="elabrev.php?modulo=principal/termoCooperacao/retornartermo&acao=A&aedid="+jQuery("#aedid").val()+"&tcpid="+jQuery("#tcpid").val();
        } else {
            alert("Todos os campos são de preencimento obrigatório");
        }
    });

    jQuery("#btnNext").click(function(){
        if (typeof jQuery('input[type="radio"][name="hstid[]"]:checked').val() === "string") {
            location.href=location.href+"&hstid="+jQuery('input[type="radio"][name="hstid[]"]:checked').val();
        } else {
            alert("Selecione uma opção na situação do histórico");
        }
    });

    jQuery("#btnDelete").click(function(){
        if (confirm("Deseja realmente apagar o conteúdo?")) {
            location.href=location.href+"&deleta=true";
        }
    });
});

<?php if (isset($_GET['hstid'])) : ?>
    jQuery('input[type="radio"][name="hstid[]"][value="<?=$_GET['hstid']?>"]').attr("checked", true);
<?php endif; ?>
</script>