<?php
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

include_once APPRAIZ."monitoraseb/classes/InstituicaoHabilitada.class.inc";

$ih = new InstituicaoHabilitada();

monta_titulo('Instituição Habilitada', 'Pesquisar Instituição');

?>
<html>
    <head>
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Connection" content="Keep-Alive">
        <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
        <title><?= $titulo ?></title>

        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script language="javascript" type="text/javascript">
            function pesquisar(acao){
                if(document.formulario.elements['entnome'].value == "") {
                    document.formulario.elements['entnome'].focus();
                    alert('O campo "Nome" é obrigatório!');
                    return false;
                }

                return true;
            }

            function selecionarInstituicao(entid){
                window.opener.document.getElementById('entid').value = entid;
                window.opener.carregarInstituicao();
                window.close();
            }

        </script>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <script type="text/javascript">
            this._closeWindows = false;
        </script>
    </head>
    <body style="margin:10px; padding:0; background-color: #fff;">
        <input type="hidden" name="requisicao" value="pesquisar">
        <form name="formulario" action="" method="post" id="formulario" enctype="multipart/form-data">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class ="SubTituloDireita" align="right">Nome:</td>
                    <td>
                         <?= campo_texto('entnome', 'S', $entnome, '', 80, 200, '', '', 'left', '', 0, 'id="entnome"', '', $_REQUEST['entnome']); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" align="left"></td>
                    <td>
                        <input type="submit" value="Pesquisar" name="btPesquisar" id="btPesquisar" onclick="pesquisar();">
                        <input type="button" value="Fechar" style="cursor:pointer" id="botaovoltar" onclick="window.close();">
                    </td>
                </tr>
            </table>
        </form>
        <?php

        if($_REQUEST['entnome']){
            $ih->pesquisarInstituicao($_REQUEST['entnome']);
        }
        ?>
    </body>
</html>