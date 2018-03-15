<?php

$cSubacao = new Painel_Controller_Subacao();
switch ($_REQUEST['action']) {
    case 'save':
        $cSubacao->salvar($_POST);
        $db->sucesso($modulo);
    case 'delete':
        $cSubacao->excluir($_REQUEST['subid']);
        $db->sucesso($modulo);
}

$subid = $_REQUEST['action'] == 'load' && $_REQUEST['subid'] ? $_REQUEST['subid'] : null;
$mSubacao = new Painel_Model_Subacao($subid);

if($mSubacao->acaid){
    $acaid = $mSubacao->acaid;
    $_SESSION['acaid'] = $acaid;
} else {
    $acaid = $_SESSION['acaid'];
}

include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
    <input type='hidden' name='acaid' value='<?php echo $acaid ?>'>
    <input type='hidden' name='subid' value='<?php echo $mSubacao->subid ?>'>
    <input type='hidden' name = 'action' value='save'>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita">Nome:</td>
            <td>
                <?php echo campo_texto('subdsc' , 'S' , 'S' , '' , 100 , 500 , '' , '' , '' , '' , '' , '' , '' , $mSubacao->subdsc); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">Detalhe:</td>
            <td>
                <?php echo campo_textarea( 'subtexto', 'N', 'S', '', 104, 05, 100000, '', '', '', '', '', $mSubacao->subtexto); ?>
            </td>
        </tr>
        <tr>
            <?php $subdatainicio = $mSubacao->subdatainicio ? formata_data($mSubacao->subdatainicio) : ''; ?>
            <td align='right' class="SubTituloDireita">Data Início:</td>
            <td><?=campo_texto('subdatainicio', "N", "S", "", 12, 10, "##/##/####", "", '', '', 0, 'id="subdatainicio"', '', mascaraglobal($arrDados[subdatainicio],"##/##/####")); ?></td>
        </tr>
        <tr>
            <?php $subdatafim = $mSubacao->subdatafim ? formata_data($mSubacao->subdatafim) : ''; ?>
            <td align='right' class="SubTituloDireita">Data Fim:</td>
            <td><?=campo_texto('subdatafim', "N", "S", "", 12, 10, "##/##/####", "", '', '', 0, 'id="subdatafim"', '', mascaraglobal($arrDados[subdatafim],"##/##/####")); ?></td>
        </tr>

        <tr bgcolor="#cccccc">
            <td></td>
            <td>
                <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
        </tr>
    </table>
</form>

<?php
    if($acaid) {
        $sql = "SELECT '<center>
                        <a style=\"cursor:pointer;\" onclick=\"alterarsubacao(\''||sub.subid||'\');\">
                            <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                        </a>
                        <a  style=\"cursor:pointer;\" onclick=\"removersubacao(\''||sub.subid||'\');\">
                            <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                        </a>
                    </center>' as acao, 
                sub.subdsc, sub.subtexto, sub.subdatainicio, sub.subdatafim
                FROM painel.subacao sub 
                WHERE acaid = $acaid
                and substatus = 'A'
                ORDER BY sub.subdatainicio
                ";

        $cabecalho = array("", "Título", "Detalhe", "Data Início", "Data Fim");
        echo "<h4 style='text-align: center;'>Subações Cadastradas</h4>";
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', '95%', $par2);;

    } ?>


<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css"/>
<script src="./js/planacomorc.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
    function submeter() {
        if (document.formulario.subdsc.value == '') {
            alert('O campo "Nome" é de preenchimento obrigatório!');
            document.formulario.acadsc.focus();
            return false;
        } else {
            document.formulario.submit();
        }
    }

    function removersubacao(subid){
        var conf = confirm("Você realmente deseja excluir esta subação?");
        if(conf) {
            window.location.href = '?modulo=sistema/tabelaapoio/cadastroacoes/subacao&acao=A&action=delete&subid='+subid;
        }
    }

    function alterarsubacao(subid){
        var janela = window.location.href='?modulo=sistema/tabelaapoio/cadastroacoes/subacao&acao=A&action=load&subid='+subid;
    }
</script>