<?php
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
}

if ($_REQUEST['idpdr'] && !$_REQUEST['requisicao']) {
    $sql = "select * from cap.paisdiarias where idpdr = {$_REQUEST['idpdr']}";
    $dadosPais = $db->pegaLinha($sql);
    extract($dadosPais);
}
///Chamada de programa 
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
    function abrirPesquisaPais(opc) {
        if (opc == 'mais') {
            //ABRE AS TR
            jQuery('.tr_orgao').css("display", "");
            jQuery('#tr_tipo_orgao').css("display", "");
            jQuery('#tr_botao').css("display", "");
            //MUDAR A IMAGEM E O VALOR.
            jQuery('#sinal_mais').css("display", "none");
            jQuery('#sinal_menos').css("display", "");
        } else {
            //ESCONDE AS TR
            jQuery('.tr_orgao').css("display", "none");
            jQuery('#tr_tipo_orgao').css("display", "none");
            jQuery('#tr_botao').css("display", "none");
            //MUDAR A IMAGEM E VALOR
            jQuery('#sinal_mais').css("display", "");
            jQuery('#sinal_menos').css("display", "none");
        }
    }

    function pesquisarPais(param) {
//        if (trim(param) == 'fil') {
//            $('#tipo_fil').val('fil');
//            $('#formulario').submit();
//        } else {
//            $('#tipo_fil').val('');
//            $('#formulario').submit();
//        }
        if (trim(param) == 'fil') {
            jQuery('#tipo_fil').val('fil');
        }
        else {
            jQuery('#tipo_fil').val('');
        }
        document.getElementById('formulario').submit();
    }

    function salvarDadosPais() {
        jQuery('#requisicao').val('salvarDadosPais');
        jQuery('#formulario').submit();
    }

    function excluirPais(idpdr) {
        var confirma = confirm("Deseja realmente excluir o Países/Diária?");
        if (confirma) {
            document.getElementById('requisicao').value = 'excluirPais';
            document.getElementById('idpdr').value = idpdr;
            document.getElementById('formulario').submit();
        }
    }

    function limpaCampos() {
        location.href = 'cap.php?modulo=principal/paises_diarias&acao=A';

    }
</script>
<br>
<form name="formulario" id="formulario" action="" method="POST">
    <table  bgcolor="#f5f5f5" align="center" class="tabela" >
        <input type="hidden" name="requisicao" id="requisicao" value="" />
        <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
        <input type="hidden" name="idpdr" id="idpdr" value="<?php echo $idpdr; ?>" />

        <tr>
            <td colspan="2" class="subTituloCentro" style="text-align: left;">PAÍSES/DIÁRIAS</td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Nome do País:</td>
            <td>
                <?PHP
                echo campo_texto('prddsc', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="prddsc"', '', $prddsc);
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Valor DAS 6:</td>
            <td>
                <?PHP
                echo campo_texto('pdrdas6', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="pdrdas6"', '', $pdrdas6);
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Valor DAS 5:</td>
            <td>
                <?PHP
                echo campo_texto('pdrdas5', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="pdrdas5"', '', $pdrdas5);
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Valor DAS 4 e 3:</td>
            <td>
                <?PHP
                echo campo_texto('pdrdas43', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="pdrdas43"', '', $pdrdas43);
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Valor DAS 2 e 1:</td>
            <td>
                <?PHP
                echo campo_texto('pdrdas21nivsup', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="pdrdas21nivsup"', '', $pdrdas21nivsup);
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Valor Nível Médio:</td>
            <td>
                <?PHP
                echo campo_texto('pdrdasnivmedio', 'N', 'S', '', 56, 100, '', '', '', '', '', 'id="pdrdasnivmedio"', '', $pdrdasnivmedio);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2" style="text-align:center">
                <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDadosPais();">
                <input type="reset" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limpaCampos();">
            </td>
        </tr>
    </table>

    <br>

    <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
        <tr>
            <td colspan="2" class="subTituloCentro" style="text-align: left;">
                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisaPais('mais');">
                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisaPais('menos');">
                &nbsp;&nbsp;
                PESQUISAR POR PAÍS
            </td>
        </tr>
        <tr class="tr_orgao" style="display: none;">
            <td class = "subtitulodireita" width="33%">Nome do País:</td>
            <td>
                <?PHP
                echo campo_texto('paisPesquisa', 'N', 'S', '', 56, 30, '', '', '', '', '', 'id="paisPesquisa"', '', $_REQUEST['paisPesquisa']);
                ?>
            </td>
        </tr>
        <tr id="tr_botao" style="display: none;">
            <td class="SubTituloDireita" colspan="2" style="text-align:center">
                <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarPais('fil');">
                <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarPais('tudo');">
            </td>
        </tr>
    </table>

</form>
<br>
<div id="div_viagem"><?php
    if ($_REQUEST['tipo_fil']) {
        listagemPais($_REQUEST);
    } else {
        listagemPais();
    }
    ?></div>