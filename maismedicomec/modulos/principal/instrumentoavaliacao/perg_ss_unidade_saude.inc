<?php
    #VERIFICA SE VARIÁVEIS DE SESSÃO FOI INICIADA. cASO NÃO, REDIRECIONA PARA PAGINA INICIAL.
    verificaExisteSessaoMM();
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_ss_unidade_saude&acao=A';
    $parametros     = '';

     $tipoUnidadeSaude = 'UBS';

    $db->cria_aba($abacod_tela, $url, $parametros);

?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>


<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">

    function abrirGrupoPergunta(obj, opc){

        var name= $(obj).attr('name');

        if(opc == 'mais'){
            $("#sinal_mais_"+name).css("display", "none");
            $("#sinal_menos_"+name).css("display", "");

            $("."+name).css("display", "");

        }else{
            $("#sinal_mais_"+name).css("display", "");
            $("#sinal_menos_"+name).css("display", "none");

            $("."+name).css("display", "none");
        }
    }

    function buscarDadosUnidadeSaude( ussid ){

        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=buscarDadosUnidadeSaude&ussid="+ussid,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $.each(dados, function(index, value) {
                   $('#'+index).val(value);
                });
            }
        });
        $('#div_formulario_perguntas').css('display', '');

        buscarRespostaUnidadeSaude( ussid );
    }

    function buscarRespostaUnidadeSaude( ussid ){
        $('#formulario').each (function(){
            this.reset();
        });

        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=buscarRespostaUnidadeSaude&ussid="+ussid,
            success: function(resp){
                var dados = $.parseJSON(resp);

                var indice = '';
                var resp = '';
                var tipo = '';

                $.each(dados, function(index, value) {
                    $.each(value, function(index, value) {
                        if(index == 'prgid'){
                            indice = value;
                        }
                        if(index == 'rusresposta'){
                            resp = value;
                        }
                        if(index == 'prgtipo'){
                            tipo = value;
                        }
                    });

                    if(tipo == 'T' || tipo == 'C'){
                        $('textarea[name=pergunta['+indice+']]').val(resp);
                    }

                    if(tipo == 'N'){
                        $('input:text[name=pergunta['+indice+']]').val(resp);
                    }

                    if(tipo == 'B'){
                        $('#pergunta_'+resp+'_'+indice).attr('checked', true);
                    }

                    if(tipo == 'M'){
                        $('#pergunta_'+resp+'_'+indice).attr('checked', true);
                    }

                    $('input:hidden[name=status['+indice+']]').val('U');

                });
            }
        });
    }

    function cadastraUnidadeSaude( param ){
        //UNIDADE BÁSICAS DE SAÚDE (UBS)= 2
        var url = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/cad_unidade_saude&acao=A&tusid=2';
        window.open( url, 'alterarEstado', 'width=650, height=300, scrollbars=yes, scrolling=no, resizebled=no' );
    }

    function excluirUnidadeSaude(ussid){
        //UNIDADE BÁSICAS DE SAÚDE (UBS)= 2
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=excluirUnidadeSaude&tusid=2&ussid="+ussid,
                success: function(data){
                    $("#div_grid_lista_unidades").html(data);
                    divCarregado();
                }
            });
        }
    }

    function salvarQuestUnidadeSaude(){
        var erro;
        var campos = '';

        var ussid = $('#ussid');

        if(!ussid.val()){
            alert('O campo "É necessario Selecionar uma Unidade de Saúde" é um campo obrigatório!');
            erro = 1;
            return false;
        }

        $.each($(".obrigatorio"), function(i, v){
            if( $(this).attr('type') != 'radio' ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else{
                var name  = $(this).attr('name');
                var value = $(this).attr('value');
                var radio_box = $('input:radio[name='+name+']:checked');

                if(!radio_box.val()){
                    erro = 1;
                    if(value == 'S'){
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
                }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarQuestUnidadeSaude');
            $('#pagina').val('U');
            $('#formulario').submit();
        }
    }

    function voltarAbaPerguntas(){
        window.location.href = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_ss_hospital&acao=A';
    }

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class ="SubTituloDireita" width="25%">Nome da Unidade Básica de Saúde </td>
        <td id="td_nome_unidade_saude"> <input type="text" class="disabled" id="ussdsc" size="45" disabled="disabled">&nbsp;</td>
        <td class ="SubTituloDireita" width="25%">CNES nº</td>
        <td id="td_cnes_unidade_saude"> <input type="text" class="disabled" id="usscnes" disabled="disabled">&nbsp;</td>
    </tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td style="padding: 0px;" width="15%">
            <span style="cursor: pointer" onclick="cadastraUnidadeSaude('H');" title="Nova Unidade de Saúde" >
                <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> Adicionar UBS
            </span>
        </td>
    </tr>
</table>

<div id="div_grid_lista_unidades">
    <?PHP
        montaListaGridUnidadeSaude( UNIDADE_SAUDE_UBS );
    ?>
</div>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="ussid" name="ussid" value=""/>

     <div id="div_formulario_perguntas" style="display: none;">
        <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
            <tr>
                <td width="95%" style="vertical-align: top;">
                    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela listagem" >
                        <tr>
                            <td class ="SubTituloCentro" colspan="3"> 3. Serviços de saúde - <?=$tipoUnidadeSaude;?>.</td>
                        </t>

                        <?PHP
                            $arrPerguntas = array(80,81);
                            echo montaPerguntasQuestionario( 'B - 1)', 'Instalações Físicas', '', $arrPerguntas, NULL, 'B_1' );

                            $arrPerguntas = array(82,83);
                            echo montaPerguntasQuestionario( 'B - 2)', 'Consultórios', '', $arrPerguntas, NULL, 'B_2' );

                            $arrPerguntas = array(84,85);
                            echo montaPerguntasQuestionario( 'B - 3)', 'Sala de Curativos', '', $arrPerguntas, NULL, 'B_3' );

                            $arrPerguntas = array(86,87);
                            echo montaPerguntasQuestionario( 'B - 4)', 'Sala de Vacinação', '', $arrPerguntas, NULL, 'B_4' );

                            $arrPerguntas = array(88,89,90);
                            echo montaPerguntasQuestionario( 'B - 5)', 'Farmácia Básica', '', $arrPerguntas, NULL, 'B_5' );

                            $arrPerguntas = array(91,92);
                            echo montaPerguntasQuestionario( 'B - 6)', 'Sala de Reuniões', '', $arrPerguntas, NULL, 'B_6' );

                            $arrPerguntas = array(93,94);
                            echo montaPerguntasQuestionario( 'B - 7)', 'Banheiros', '', $arrPerguntas, NULL, 'B_7' );

                            $arrPerguntas = array(95,96);
                            echo montaPerguntasQuestionario( 'B - 8)', 'Internet', '', $arrPerguntas, NULL, 'B_8' );

                            $arrPerguntas = array(97,98);
                            echo montaPerguntasQuestionario( 'B - 9)', 'Telesaúde', '', $arrPerguntas, NULL, 'B_9' );

                            $arrPerguntas = array(99,100);
                            echo montaPerguntasQuestionario( 'B - 10)', 'Materiais e Instrumentos', '', $arrPerguntas, NULL, 'B_10' );

                            $arrPerguntas = array(104,105,106,107,108);
                            echo montaPerguntasQuestionario( 'B - 11)', 'ESF', '', $arrPerguntas, NULL, 'B_11' );

                            $arrPerguntas = array(112);
                            echo montaPerguntasQuestionario( 'B - 12)', 'População Vinculada', '', $arrPerguntas, NULL, 'B_12' );

                            #perguntas inativadas: 113,115,116,117,119,121,122,123
                            $arrPerguntas = array(113, 119);
                            echo montaPerguntasQuestionario( 'B - 13)', 'Normas de funcionamento', '', $arrPerguntas, NULL, 'B_13' );

                            #perguntas inativadas: 126,128
                            $arrPerguntas = array(125,127,129,130,131,132,133,134,135,136);
                            echo montaPerguntasQuestionario( 'B - 14)', 'Atividades desenvolvidas', '', $arrPerguntas, NULL, 'B_14' );

                            $arrPerguntas = array(137,140,141,142,143);
                            echo montaPerguntasQuestionario( 'B - 15)', 'Profissionais em Atividades', '', $arrPerguntas, NULL, 'B_15' );

                            $arrPerguntas = array(138,139);
                            echo montaPerguntasQuestionario( 'B - 16)', 'Sistema de referência', '', $arrPerguntas, NULL, 'B_16' );

                            $arrPerguntas = array(144,145);
                            echo montaPerguntasQuestionario( 'B - 17)', 'Ações de Educação Permanentes', '', $arrPerguntas, NULL, 'B_17' );

                            $arrPerguntas = array(146);
                            echo montaPerguntasQuestionario( 'B - 18)', 'Dados Estatísticos', '', $arrPerguntas, NULL, 'B_18' );

                            $arrPerguntas = array(147,148);
                            echo montaPerguntasQuestionario( 'B - 19)', 'Alunos', '', $arrPerguntas, NULL, 'B_19' );

                            $arrPerguntas = array(149,150);
                            echo montaPerguntasQuestionario( 'B - 20)', 'NASF', '', $arrPerguntas, NULL, 'B_20' );                
                        ?>
                    </table>
                </td>
                <td width="5%" style="vertical-align: top;">
                    <?PHP
                        require_once APPRAIZ . "includes/workflow.php";
                        $etqid = $_SESSION['maismedicomec']['etqid'];
                        if($etqid != ''){
                            $docid = buscarDocidAvaliacaoMM( $etqid );

                            if($docid != ''){
                                $dados_wf = array("cprid" => $cprid);
                                wf_desenhaBarraNavegacao($docid, $dados_wf);
                            }
                        }
                    ?>
                </td>
            </tr>
        </table>

        <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
            <tr>
                <td class="SubTituloCentro" colspan="4">
                    <?PHP
                        $habilita = habilitaPerfilEstadoAcao();

                        if( $habilita == 'S' ){
                    ?>
                            <input type="button" id="voltar" name="voltar" value="Voltar" title="Volta a Listagem de Municípios" onclick="voltarAbaPerguntas();"/>
                            <input type="button" id="salvarQuest" name="salvarQuest" value="Salvar" onclick="salvarQuestUnidadeSaude();"/>
                    <?PHP
                        }else{
                    ?>
                            <input type="button" id="voltar" name="voltar" value="Voltar" title="Volta a Listagem de Municípios" onclick="voltarAbaPerguntas();"/>
                            <input type="button" id="salvar_sem_continui" name="salvar_sem_continui" value="Salvar" disabled="disabled">
                    <?PHP
                        }
                    ?>
                </td>
            </tr>
        </table>
    </div>
</form>