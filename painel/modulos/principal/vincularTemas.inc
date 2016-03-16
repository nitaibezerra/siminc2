<?php

if($_POST['salvar']){
    if($_POST['indid']){

        if (isset($_POST['etapas']) && is_array($_POST['etapas'])) {
            $sql = "delete from painel.indicadoretapaeducacao where indid = {$_POST['indid']};";
            foreach ($_POST['etapas'] as $etpid) {
                $sql .= "insert into painel.indicadoretapaeducacao (indid, etpid) values ({$_POST['indid']}, $etpid);";
            }
            if($sql){
                $db->executar($sql);
                $db->commit();
            }
        }
        
        if (isset($_POST['temas']) && is_array($_POST['temas'])) {
            $sql = "delete from painel.indicadortemamec where indid = {$_POST['indid']};";
            foreach ($_POST['temas'] as $temid) {
                $sql .= "insert into painel.indicadortemamec (indid, temid) values ({$_POST['indid']}, $temid);";
            }
            if($sql){
                $db->executar($sql);
                $db->commit();
            }
        }
    }
    echo "<script>alert('Operação realizada com sucesso');window.location.href=window.location.href;</script>";
    exit;
}

if($_REQUEST['carregar-modal']){
    $sql = "select indid, indnome from painel.indicador where indstatus = 'A' and indid = '{$_REQUEST['indid']}'";
    $dados = $db->pegaLinha($sql);
    $dados = $dados ? $dados : array();

    $sql = "select temid,
                   temdsc
            from painel.temamec
            where temstatus = 'A'
            order by temdsc";
    $temas = $db->carregar($sql);
    $temas = $temas ? $temas : array();

    $sql = "select etpid,
                   etpdsc
            from painel.etapaeducacao
            order by etpdsc";
    $etapas = $db->carregar($sql);
    $etapas = $etapas ? $etapas : array();

    $sql = "select temid, indid
            from painel.indicadortemamec
            where indid = {$_REQUEST['indid']}";
    $temasSel = $db->carregar($sql);
    $temasSel = $temasSel ? $temasSel : array();

    $temasSelecionados = array();
    foreach ($temasSel as $tema) {
        $temasSelecionados[] = $tema['temid'];
    }

    $sql = "select etpid, indid
            from painel.indicadoretapaeducacao
            where indid = {$_REQUEST['indid']}";
    $etapasSel = $db->carregar($sql);
    $etapasSel = $etapasSel ? $etapasSel : array();

    $etapasSelecionadas = array();
    foreach ($etapasSel as $etapa) {
        $etapasSelecionadas[] = $etapa['etpid'];
    }
    ?>

    <h3 style='color: red; text-align: center;'><?php echo $dados['indnome']; ?></h3>

    <form name="formulario_vinculo" id="formulario_vinculo" method="post">
            <input type="hidden" name="indid" value="<?php echo $dados['indid']; ?>" />
            <input type="hidden" name="salvar" value="1" />

        <h4 style='margin-left: 20px;'>Etapas de Ensino</h4>

        <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 20px;">
            <thead>
            <tr align="center">
                <td  width="5%" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Ação</strong>
                </td>
                <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Etapas</strong>
                </td>
            </tr>
            </thead>
            <?php
            foreach ($etapas as $count => $dado) {
                $checked = in_array($dado['etpid'], $etapasSelecionadas) ? 'checked="checked"' : '';
                $complemento = ($count % 2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left">
                        <input type="checkbox" name="etapas[]" value="<?php echo $dado['etpid']; ?>" <?php echo $checked; ?> />
                    </td>
                    <td align="left"><?php echo $dado['etpdsc']; ?></td>
                </tr>
            <?php } ?>
        </table>

        <h4 style='margin-left: 20px;'>Temas</h4>

        <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 20px;">
            <thead>
            <tr align="center">
                <td  width="5%" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Ação</strong>
                </td>
                <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <strong>Etapas</strong>
                </td>
            </tr>
            </thead>
            <?php
            foreach ($temas as $count => $dado) {
                $checked = in_array($dado['temid'], $temasSelecionados) ? 'checked="checked"' : '';
                $complemento = ($count % 2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
                ?>
                <tr align="center" <?php echo $complemento; ?>>
                    <td align="left">
                        <input type="checkbox" name="temas[]" value="<?php echo $dado['temid']; ?>" <?php echo $checked; ?> />
                    </td>
                    <td align="left"><?php echo $dado['temdsc']; ?></td>
                </tr>
            <?php } ?>
        </table>

    </form>
    <?php die;
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// atribuindo as permissões
$permissoes = verificaPerfilPainel();
echo montarAbasArray($permissoes['menu'], $_SERVER['REQUEST_URI']);

// titulos da tela
$titulo = "Vincular Indicadores";

monta_titulo( $titulo, '&nbsp;' );

?>

<link href="/library/jquery/jquery-ui-1.10.3/themes/custom-theme/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
<script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<?php

$sql = "select  i.indid, i.indnome,
                array_to_string(array(select etpdsc from painel.indicadoretapaeducacao ie inner join painel.etapaeducacao e on e.etpid = ie.etpid where ie.indid = i.indid), ', ') as etapa,
                array_to_string(array(select temdsc from painel.indicadortemamec it inner join painel.temamec t on t.temid = it.temid where it.indid = i.indid), ', ') as tema
        from painel.indicador i
        where indstatus = 'A'
        order by indnome
--        limit 10";
$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();
?>
<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 20px;">
    <thead>
        <tr align="center">
            <td width="5%" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Vincular</strong>
            </td>
            <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Indicador</strong>
            </td>
            <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Temas</strong>
            </td>
            <td width="300px" valign="top" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Etapas</strong>
            </td>
        </tr>
    </thead>
    <?php
        foreach ($dados as $count => $dado) {
            $complemento = ($count % 2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
            <tr align="center" <?php echo $complemento; ?>>
                <td><img class="vincular_temas" data-indid="<?php echo $dado['indid']; ?>" src="/imagens/alterar.gif" border=0 title=\"Vincular\"></td>
                <td align="left"><?php echo $dado['indid'] . ' - ' . $dado['indnome']; ?></td>
                <td align="left"><?php echo $dado['tema']; ?></td>
                <td align="left"><?php echo $dado['etapa']; ?></td>
            </tr>
        <?php } ?>
</table>

<div id="dialog_vincular_temas"></div>
<script type="text/javascript">
    $1_11(function(){
        $1_11(".vincular_temas").click(function(){

            $1_11("#dialog_vincular_temas").load('/painel/painel.php?modulo=principal/vincularTemas&acao=A&carregar-modal=1&indid=' + jQuery(this).attr('data-indid'), function(){
                $1_11("#dialog_vincular_temas").dialog({
                    modal: true,
                    width: 880,
                    position: 'top',
                    buttons: {
                        Salvar: function() {
                            $1_11("#formulario_vinculo").submit();
                        },
                        Fechar: function() {
                            $1_11("#dialog_vincular_temas").dialog( "close" );
                        }
                    }
                });
            });
        });
    });


</script>