<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 30/07/2015
 * Time: 11:02
 */

if ( $_POST['requisicao'] == 'gerarRelatorio' ){
    include("relatorio_ProdApoioDGAA_resultado.inc");
    relatorioMaior_15( $_REQUEST );
    exit;
}

include_once APPRAIZ . "includes/cabecalho.inc";
monta_titulo( 'Relatório de Produtividade do Protocolo Apoio ao DGAA', '<b></b>' );
?>
<style>
    #container{
        width:100%;
        padding:15px;
    }
    #listaProcessosConjur{
        width:100%;
        /*padding:15px;*/
        text-align: center;
    }
    .agrupamentos td{
        vertical-align: middle;
    }
</style>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script>
    function pesquisar(){
        if ($('#dataini').val() != '' && $('#datafim').val() != ''){
            var janela = window.open('', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            document.formulario.target = 'relatorio';
            document.formulario.submit();
        } else {
            alert('Informe o período corretamente.');
        }
    }

    function validaDatas(){
        // valida data inicial
        var dataInicial = jQuery('#dataini').val();
        var dataInicialParts = dataInicial.split('/');
        var dataInicialObj = new Date(parseInt(dataInicialParts[2])-1,parseInt(dataInicialParts[1])-1,parseInt(dataInicialParts[0]));

        // valida data final
        var dataFinal = jQuery('#datafim').val();
        var dataFinalParts = dataFinal.split('/');
        var dataFinalObj = new Date(parseInt(dataFinalParts[2])-1,parseInt(dataFinalParts[1])-1,parseInt(dataFinalParts[0]));


        // valida relacao das datas
        if( dataInicialObj <= dataFinalObj ){
            return true;
        }else{
            return false;
        }
    }

    function abrepopupUsuario(){
        window.open('/conjur/combo_usuarios_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
    }
</script>
<form name="formulario" id="formulario" method="post">
    <input type="hidden" id="requisicao" name="requisicao" value="gerarRelatorio"/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td style="width:300px;" class="subtitulodireita">Período:</td>
            <td>
                <?php
                    echo campo_data2('dataini', 'S', 'S', 'Período Inicial', '##/##/####', '', '', '', '', '');
                    echo ' à ';
                    echo campo_data2('datafim', 'S', 'S', 'Período Final', '##/##/####', '', '', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Usuários:</td>
            <td>
                <select multiple="multiple" name="usuid[]" onDblClick="abrepopupUsuario();" id="usuid" style="width:400px;" >
                    <?php
                    if (!$valoresOptionsUsuarios)
                        echo '<option value="">Duplo clique para selecionar da lista</option>';
                    else {
                        foreach ($valoresOptionsUsuarios as $key => $value) {
                            echo '<option value="' . $value['codigo'] . '">' . $value['descricao'] . '</option>';
                        }
                    }
                    ?>
                </select>
            </td>
        </tr>
        <tr>
            <td align="center" bgcolor="#DCDCDC" colspan="2">
                <input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisar();"/>
                <input type="reset" value="Limpar" id="btnLimpar"/>
            </td>
        </tr>
    </table>
</form>