<?php

include_once '../planacomorc/_funcoesOrcamentario.php';

global $db;

if($_REQUEST['gravar']) {

    if($_REQUEST['sisid']){

        $sql = "delete from painel.acaosistema where sisid = {$_REQUEST['sisid']}";
        $db->executar($sql);

        if (isset($_REQUEST['acoes']) && is_array($_REQUEST['acoes'])) {
            foreach ($_REQUEST['acoes'] as $acaid) {
                $sql = "INSERT INTO painel.acaosistema(sisid, acaid)
                            VALUES ({$_REQUEST['sisid']}, $acaid)";
                $db->executar($sql);
            }
        }
        $db->commit();
    }

    die("<script>
            alert('Operação realizada com sucesso!');
            window.location.href = '$url';
        </script>");
}
?>

<style>
    .label_campo{
        margin-left: 2px;
        margin-right: 10px;
    }
</style>

<?php
extract($_REQUEST);

include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>

<form method="post" name="formulario" id="formulario">
    <input type="hidden" name="gravar"  value="1"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">Módulo:</td>
            <td>
                <?php
                $sql = "select sisid as codigo, sisdsc || ' (' || sisabrev || ')' as descricao
                        from seguranca.sistema
                        where sisid = '{$_SESSION['sisid']}'
                        order by descricao";

                echo $db->monta_combo("sisid",$sql,'S',"Selecione...","","","","400","S","sisid","",$_SESSION['sisid']);
                ?>
            </td>
        </tr>
    </table>
    <?php
    $sql = "select
              acaid, acadsc, acadetalhe,
              (select count(*) from painel.acaosistema where acaid = a.acaid and sisid = '{$_SESSION['sisid']}') as marcado
              from painel.acao a
            where acastatus = 'A'
            order by acadsc";

    $dados = $db->carregar($sql);
    ?>

    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
        <thead>
        <tr align="center">
            <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Ação</strong>
            </td>
            <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Ação Estratégica</strong>
            </td>
            <td valign="top" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                <strong>Detalhe</strong>
            </td>
        </tr>
        </thead>
        <tbody>
        <?php
        foreach ((array)$dados as $count => $dado) {
            $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"';
            ?>
            <tr <?php echo $complemento; ?>>
                <td align="center"><input type="checkbox" name="acoes[]" value="<?php echo $dado['acaid']; ?>" <?php echo $dado['marcado'] ? 'checked="checked"' : ''; ?> /></td>
                <td><?php echo $dado['acadsc']; ?></td>
                <td><?php echo $dado['acadetalhe']; ?></td>

            </tr>
        <?php } ?>


        </tbody>
    </table>


    <div style="text-align: center; margin-top: 10px;">
        <input type="button" name="enviar" id="enviar" value="Enviar"/>
    </div>
</form>

<script type="text/javascript">
    jQuery(function(){
        jQuery('#enviar').click(function(){
            jQuery('#formulario').submit();
        });
    });        
</script>