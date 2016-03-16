<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

include_once APPRAIZ."monitoraseb/classes/Subacao.class.inc";
include_once APPRAIZ."monitoraseb/classes/InstituicaoHabilitada.class.inc";

$sa = new SubAcao();
$ih = new InstituicaoHabilitada();

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Consultar Instituição Habilitada','&nbsp;');

?>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
    function pesquisar(acao){
        var url = 'monitoraseb.php?modulo=principal/instituicao/consultarinstituicao&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    function listarSubacao(coonid){
        mostraComboSubAcao(coonid,'','','','S','');
    }

    function excluirInstituicao(ihdid){
        if (confirm('Deseja excluir a Instituição?')) {
            //location.href = window.location+'&excluirAnexo='+arqid;
        }
    }
</script>

<form name="formulario" id="formulario" method="post">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%">Coordenação:</td>
            <td colspan="2">
                <? $sa->comboCoordenacao($_REQUEST['coonid'], $_SESSION['usucpf'], "listarSubacao") ?>
            </td>
        </tr>
        <tr>
            <td align='left' class="SubTituloDireita">Subacao:</td>
            <td>
                <div id="sbaid_on" style="display:none;">
                </div>
                <div id="sbaid_off" style="color:#909090;">Selecione uma Coordenacao.</div>
                <script language="javascript" type="text/javascript">
                    listarSubacao(document.getElementById("coonid").value);
                </script>
            </td>
        </tr>
        <tr>
            <th></th>
            <th style="text-align: left;" colspan="2">
                <input style="cursor: pointer;" type="button" value="Visualizar" name="visualizar" onclick="pesquisar('filtrar');">
                <input style="cursor: pointer;" type="button" value="Ver todos" name="vertodos" onclick="pesquisar('todos');">
            </th>
        </tr>
        <tr>
            <td colspan=3>
                <a href="?modulo=principal/instituicao/cadastrarinstituicao&acao=A"><img src="/imagens/gif_inclui.gif" border="0"/> Adicionar</a>
            </td>
        </tr>
    </table>
</form>
<?php

if(!empty($_REQUEST['pesquisar'])){
    if($_REQUEST['pesquisar'] == 'filtrar'){
        $ih->listarInstituicao($_REQUEST['coonid'], $_REQUEST['sbaid'], $_SESSION['usucpf']);
    }elseif ($_REQUEST['pesquisar'] == 'todos'){
        $ih->listarInstituicao(null, null, $_SESSION['usucpf']);
    }
}
?>