<?PHP

    if ( $_POST['requisicao'] == 'gerarRelatorio' ){
        include("relatorio_MediasProcessos_resultado.inc");        
        relatorioMaior_15( $_REQUEST );    
        exit;
    }

    include_once APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Relatório de Médias de Processos', '<b>Mostra o número, média diária de processos e processos como atendimento menor ou maior que 15 dias pelo advogado em determinado período</b>' );

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
    function abrepopupAdvogados(){
        window.open('/conjur/combo_advogados_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
    }

    function abrepopupCoordenacoes(){
        window.open('/conjur/combo_coordenacoes_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
    }

    // limpar combos quando reset do form acionado
    jQuery('document').ready(function(){
	jQuery("[type='reset']").click(function(){
            jQuery('#advid').html('<option value="">Duplo clique para selecionar da lista</option>');
            jQuery('#coonid').html('<option value="">Duplo clique para selecionar da lista</option>');
	});

	jQuery('input.agrupamento[value="advogados"]').change(function(){
            if( jQuery('input.agrupamento[value="advogados"]').is(':checked') == true ){
                jQuery('#linhaAdvogado').show();
            }else{
                jQuery('#linhaAdvogado').hide();
            }
	});

	jQuery('input.agrupamento[value="coordenacao"]').change(function(){
            if( jQuery('input.agrupamento[value="coordenacao"]').is(':checked') == true ){
                jQuery('#linhaCoordenacao').show();
            }else{
                jQuery('#linhaCoordenacao').hide();
            }
	});
    });

    function pesquisar(){
        selectAllOptions(document.getElementById('advid'));
        selectAllOptions(document.getElementById('coonid'));

        if( !validaDatas() ){
            alert('A data inicial deve ser menor que a data final.');
            return false;
        }

        if( jQuery('input.agrupamento').is(':checked') == false ){
            alert('Selecione algum Agrupamento.');
            return false;
        }

        if( !validaAdvogados() || !validaCoordenacoes() ){
            alert('Selecione algum Advogado e/ou Coodenação.');
            return false;
        }

        if( jQuery('#htddatainicial').val() == '' || jQuery('#htddatfinal').val() == '' ){
            alert('Especifique o período.');
            return false;
        }

        jQuery('#maior_15').val('S');

        var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        document.formulario.target = 'relatorio';
        document.formulario.submit();
//        janela.focus();
    }

    function validaAdvogados(){
        return validaInputCheckboxSelecao( 'advid', 'advogados' );
    }

    function validaCoordenacoes(){
        return validaInputCheckboxSelecao( 'coonid', 'coordenacao' );
    }

    function validaInputCheckboxSelecao( campo, valor ){
        // console.log( jQuery('input.agrupamento[value="'+valor+'"]').is(':checked') );
        if( jQuery('input.agrupamento[value="'+valor+'"]').is(':checked') == true && document.getElementById( campo ).selectedOptions[0].value != '' ){
            return true;
        }else{
            if( jQuery('input.agrupamento[value="'+valor+'"]').is(':checked') == false ){
                return true;
            }else{
                return false;
            }
        }
    }

    function validaDatas(){
        // valida data inicial
        var dataInicial = jQuery('#htddatainicial').val();
        var dataInicialParts = dataInicial.split('/');
        var dataInicialObj = new Date(parseInt(dataInicialParts[2])-1,parseInt(dataInicialParts[1])-1,parseInt(dataInicialParts[0]));

        // valida data final
        var dataFinal = jQuery('#htddatfinal').val();
        var dataFinalParts = dataFinal.split('/');
        var dataFinalObj = new Date(parseInt(dataFinalParts[2])-1,parseInt(dataFinalParts[1])-1,parseInt(dataFinalParts[0]));


        // valida relacao das datas
        if( dataInicialObj <= dataFinalObj ){
            return true;
        }else{
            return false;
        }
    }
</script>

<form name="formulario" id="formulario" method="post">
    <input type="hidden" id="requisicao" name="requisicao" value="gerarRelatorio"/>
    <input type="hidden" id="maior_15" name="maior_15" value=""/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td style="width:300px;" class="subtitulodireita">Período:</td>
            <td>
                <?PHP
                    if ($_POST['htddatainicial']) {
                        $htddatainicial = explode('/', $_POST['htddatainicial']);
                        $htddatainicial = $htddatainicial[1] . '/' . $htddatainicial[0] . '' . $htddatainicial[2];
                    }

                    if ($_POST['htddatfinal']) {
                        $htddatfinal = explode('/', $_POST['htddatfinal']);
                        $htddatfinal = $htddatfinal[1] . '/' . $htddatfinal[0] . '' . $htddatfinal[2];
                    }

                    echo campo_data2('htddatainicial', 'S', 'S', 'Período Inicial', '##/##/####', '', '', $htddatainicial, '', '');
                    echo ' à ';
                    echo campo_data2('htddatfinal', 'S', 'S', 'Período Final', '##/##/####', '', '', $htddatfinal, '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Agrupamento:</td>
            <td>
                <table class="agrupamentos">

                    <tr><td><input name="agrupamentoCoordenacao" class="agrupamento" id="agrupamento" type="checkbox" value="coordenacao" /></td><td>Coordenação</td></tr>

                    <tr><td><input name="agrupamentoAdvogados" class="agrupamento" id="agrupamento" type="checkbox" value="advogados" /></td><td>Advogados</td></tr>

                </table>
            </td>
        </tr>

        <!-- COORDENACAO -->
        <tr id="linhaCoordenacao" style="display:none">
            <td class="subtitulodireita">Coordenação(ões):</td>
            <td>
                <select multiple="multiple" name="coonid[]" onDblClick="abrepopupCoordenacoes();" id="coonid" style="width:400px;" >
                    <?php
                    if (!$valoresOptionsCoordenacoes)
                        echo '<option value="">Duplo clique para selecionar da lista</option>';
                    else {
                        foreach ($valoresOptionsCoordenacoes as $key => $value) {
                            echo '<option value="' . $value['codigo'] . '">' . $value['descricao'] . '</option>';
                        }
                    }
                    ?>
                </select>
            </td>
        </tr>

        <!-- ADVOGADOS -->
        <tr id="linhaAdvogado" style="display:none">
            <td class="subtitulodireita">Advogado(s):</td>
            <td>
                <select multiple="multiple" name="advid[]" onDblClick="abrepopupAdvogados();" id="advid" style="width:400px;" >
                    <?php
                    if (!$valoresOptionsAdvogados)
                        echo '<option value="">Duplo clique para selecionar da lista</option>';
                    else {
                        foreach ($valoresOptionsAdvogados as $key => $value) {
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