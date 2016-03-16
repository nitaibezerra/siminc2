<?php
header( 'Content-Type: text/html; charset=iso-8859-1' );

$painelOperacional  = new PainelOperacional( $db );
$recarregarAction   = !empty($_REQUEST['action'])       ? $_REQUEST['action']          : '' ;
$recarregarSituacao = !empty($_REQUEST['esdid'])        ?  $_REQUEST['esdid']          : '' ;
$ordemLista         = !empty($_REQUEST['ordemlista'])   ?  $_REQUEST['ordemlista']     : '' ;

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "PAINEL OPERACIONAL - Fiscal do Contrato";
monta_titulo( $titulo, '&nbsp;' );
?>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/painel-operacional.js"></script>
<link type="text/css" href="./css/painel_operacional.css" rel="stylesheet" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td width="50%" class="TituloTabela center">Empresa 1</td>
        <td class="TituloTabela center">Empresa 2</td>
    </tr>
    <tr>
        <td valign="top" class="center">
            <?php echo $painelOperacional->painelOperacionalEmpresaItem1(); ?>
        </td>
        <td valign="top" class="center">
            <?php echo $painelOperacional->painelOperacionalEmpresaItem2(); ?>
        </td>
    </tr>

    <tr>
        <td colspan="2" id="container-listagem"></td>
    </tr>
</table>

<input type="hidden" name="recarregarAction"   id="recarregarAction"   value="<?php echo($recarregarAction); ?>" />
<input type="hidden" name="recarregarSituacao" id="recarregarSituacao" value="<?php echo($recarregarSituacao); ?>" />
<input type="hidden" name="ordemlista"         id="ordemlista"         value="<?php echo($ordemLista); ?>" />

<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript">
$(document).ready(function(){
    PainelOperacionalView.init();      
});
</script>