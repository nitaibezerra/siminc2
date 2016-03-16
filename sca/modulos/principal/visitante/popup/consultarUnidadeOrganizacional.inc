<?php

include_once APPRAIZ."sca/classes/Visita.class.inc";
$oVisita = new Visita();

monta_titulo('Consulta de Unidades Organizacionais', '&nbsp;');

?>
<html>
<head></head>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language='javascript' type="text/javascript" src="./js/sca.js"></script>
<script language="javascript" type="text/javascript">

    //função que dispara a pesquisa no banco
    function pesquisarUnidade(acao){
        var url = 'sca.php?modulo=principal/visitante/popup/consultarUnidadeOrganizacional&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    function selecionarUnidade(co_interno_uorg, no_unidade_org){
        executarScriptPai("carregarUnidade(" + co_interno_uorg + ", '" + no_unidade_org + "');");
        self.close();
    }

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<body>
<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td><?=campo_texto('no_unidade_org','','','',60,100,'','','left','',0,'id="no_unidade_org"');?></td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2" align="center">
                <input type="button" name="pesquisar" value="Pesquisar" style="cursor:pointer;" onclick="pesquisarUnidade('filtrar');">
                <input type="button" name="fechar" value="Fechar" style="cursor:pointer;" onclick="javascript:self.close()">
            </td>
        </tr>
    </table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("no_unidade_org").focus();
      });
</script>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan='2'>
            <div style='height:120px;overflow-y:scroll;'>
            <?php
            if($_REQUEST['pesquisar'] == 'filtrar'){
                $oVisita->listarUnidadePorFiltro($_REQUEST['no_unidade_org']);
            }
            ?>
            </div>
        </td>
    </tr>
</table>
</body>
</html>