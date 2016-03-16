<?php
global $db;

if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'excluir') {
    if ($_REQUEST['concodigo']) {
        $sql = "SELECT sum(ceavalor) as valor FROM pes.pescelulaacompanhamento where concodigo = {$_REQUEST['concodigo']}";
        $result = $db->pegaUm($sql);

        if (!empty($result) && $result > 0) {
            echo '<script type="text/javascript">alert("Não pode excluir o contrato, pois o mesmo existe acompanhamento!")</script>';
        } else {
            $sql = "DELETE FROM pes.pescelulacontrato WHERE concodigo = {$_REQUEST['concodigo']};";
            $sql .= "DELETE FROM pes.pescelulaacompanhamento WHERE concodigo = {$_REQUEST['concodigo']};";
            $sql .= "DELETE FROM pes.pescontrato WHERE concodigo = {$_REQUEST['concodigo']};";
            $db->executar($sql);
            $db->commit();

            echo '<script type="text/javascript">alert("Contrato excluido com sucesso!")</script>';
        }
    } elseif ($_REQUEST['cndcodigo']) {
        $sql = "SELECT sum(canvalor) FROM pes.pescelulaacompnatdespesa where cndcodigo = {$_REQUEST['cndcodigo']}";

        $result = $db->pegaUm($sql);
        if ($result) {
            echo '<script type="text/javascript">alert("Não pode excluir o contrato, pois o mesmo existe acompanhamento!")</script>';
        } else {
            $sql = "DELETE FROM pes.pescelulaacompnatdespesa WHERE cndcodigo = {$_REQUEST['cndcodigo']} ;";
            $sql .= "DELETE FROM pes.pescontratonaturezadespesa WHERE cndcodigo = {$_REQUEST['cndcodigo']} ;";

            $db->executar($sql);
            $db->commit();

            echo '<script type="text/javascript">alert("Contrato excluido com sucesso!")</script>';
        }
    }
}

$controllerGeral = new Controller_Geral();
?>

<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>
<?php
include_once( APPRAIZ . "pes/classes/PesContrato.class.inc" );

$entcodigo = $_SESSION['pes']['codigo_entidade'];

extract($_REQUEST);

include APPRAIZ . 'includes/cabecalho.inc';

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
print '<br/>';

$db->cria_aba($abacod_tela, $url, '');
monta_titulo($titulo_modulo, '&nbsp;');
?>

<form action="" method="post" name="formulario" id="formulario">
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Despesa:</td>
            <td>
            <?php
            $sql = "select tidnome as descricao, tidcodigo as codigo
                                                       from  pes.pestipodespesa
                                                       where tidcodigo not in (" . K_DESPESA_DIARIAS . ", " . K_DESPESA_PASSAGENS . ", " . K_DESPESA_COLETA_SELETIVA . ", " . K_DESPESA_GENERICA . ")
                                   order by tidordem desc, descricao";
            echo $db->monta_combo("tidcodigo", $sql, 'S', "Selecione...", "", "", "", "200", "S", "tidcodigo", "", $tidcodigo);
            ?>
            </td>
        </tr>
        <tr id="tr_botoes_acao" style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
            <td>
                <input type="button" name="botao_enviar" id="botao_enviar" value="Buscar" />
            </td>
        </tr>
    </table>
</form>

<?php
$where = '';
if ($_POST['tidcodigo']) {
    $where = " and tid.tidcodigo = {$_POST['tidcodigo']} ";
}

// Se tiver permissao de excluir
if ($controllerGeral->permission() < 3) {

    $sqlAcao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"pes.php?modulo=principal/contrato/contrato&acao=A&concodigo=' || con.concodigo || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>'
                || ' <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"javascript:excluir('|| con.concodigo || ', \'concodigo\');\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>' ";
    $sqlAcaoSegundo = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"pes.php?modulo=principal/contrato/contrato&acao=A&cndcodigo=' || cnd.cndcodigo || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>'
            || ' <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"javascript:excluir('|| cnd.cndcodigo || ', \'cndcodigo\');\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'";
} else {
    $sqlAcao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"pes.php?modulo=principal/contrato/contrato&acao=A&concodigo=' || con.concodigo || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>'";
    $sqlAcaoSegundo = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"pes.php?modulo=principal/contrato/contrato&acao=A&cndcodigo=' || cnd.cndcodigo || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>'";
}

$sql = "select acao, tidnome, contitulo, data_inicio, data_fim
        from (
            select
    		{$sqlAcao} as acao,
    		tid.tidnome, contitulo, to_char(coniniciovigencia, 'DD/MM/YYYY') as data_inicio, to_char(confimvigencia, 'DD/MM/YYYY') as data_fim
    		from pes.pescontrato con
    			inner join pes.pestipodespesa tid on tid.tidcodigo = con.tidcodigo
    		where entcodigo = $entcodigo
    		and tid.tidcodigo != " . K_DESPESA_COLETA_SELETIVA . "
    		and tid.tidcodigo != " . K_DESPESA_MATERIAL_CONSUMO . "
    		$where

    		union

            select
            {$sqlAcaoSegundo}
            as acao,
            tid.tidnome,
            cndtitulo || ' - ' || uni.unititulo ||  ' (' || cnd.natcodigo || ' - ' || natdescricao || ')' as contitulo,
            '' as data_inicio, '' as data_fim
            from pes.pescontratonaturezadespesa cnd
                inner join pes.pesnaturezadespesa nat on nat.natcodigo = cnd.natcodigo
                inner join pes.pesunidademedida uni on uni.unicodigo = cnd.unicodigo
                inner join pes.pestipodespesa tid on tid.tidcodigo = cnd.tidcodigo
            where entcodigo = $entcodigo
            and cnd.tidcodigo = " . K_DESPESA_MATERIAL_CONSUMO . "
            $where
        ) as contratos
        order by tidnome, contitulo
		";

$cabecalho = array("Ação", "Despesa", "Contrato", "Data Início", "Data Fim");
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '');
?>

<script type="text/javascript">
    jQuery(function() {
        jQuery('#botao_enviar').click(function() {
            jQuery('#formulario').submit();
        });
    });

    function excluir(id, tipo)
    {

        var msg = 'Deseja excluir este contrato?';
        if (confirm(msg)) {
            var url = "pes.php?modulo=principal/contrato/listacontrato&acao=A&action=excluir&" + tipo + "=" + id;
            window.location.href = url
        }
    }
</script>