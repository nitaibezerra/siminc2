<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

include_once APPRAIZ."monitoraseb/classes/Subacao.class.inc";
$sa = new SubAcao();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Consultar Subação','&nbsp;');

?>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
    function pesquisar(acao){
        var url = 'monitoraseb.php?modulo=principal/subacao/consultarsubacao&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }
</script>

<form name="formulario" id="formulario" method="post">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%" >Coordenador Técnico:</td>
            <td colspan="2"><? $sa->comboCoordenadorTecnico($_SESSION['usucpf'], true, $_REQUEST['usucpf']) ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%" >Sigla / Coordenação:</td>
            <td colspan="2">
                <div id="coonid_on">
                    <? $sa->comboCoordenacao(($_REQUEST['coonid']?$_REQUEST['coonid']:null), ($_REQUEST['usucpf']?$_REQUEST['usucpf']:$_SESSION['usucpf'])) ?>
                </div>
            </td>
        </tr>
        <tr>
            <th></th>
            <th style="text-align: left;" colspan="2">
                <input style="cursor: pointer;"  type="button" value="Visualizar" name="visualizar" onclick="pesquisar('filtrar');">
                <input style="cursor: pointer;"  type="button" value="Ver todos" name="vertodos" onclick="pesquisar('todos');">
            </th>
        </tr>
    </table>
</form>
<?php

if(!empty($_REQUEST['pesquisar'])){
    if($_REQUEST['pesquisar'] == 'filtrar'){
        $sa->listarSubacao($_REQUEST['coonid'], $_REQUEST['usucpf'], $_SESSION['usucpf']);
    }elseif ($_REQUEST['pesquisar'] == 'todos'){
        $sa->listarSubacao(null, null, $_SESSION['usucpf']);
    }
}
?>