<?php
    #VERIFICA SE VARIÁVEIS DE SESSÃO FOI INICIADA. cASO NÃO, REDIRECIONA PARA PAGINA INICIAL.
    verificaExisteSessaoMM();
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    if($_REQUEST['requisicao_U']) {
        $_REQUEST['requisicao_U']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_ss_ambulatorio&acao=A';
    $parametros     = '';

    $tipoUnidadeSaude = 'Ambulatórios de Especialidades';
    
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
        
        $('#div_mensagem_selecionar_unidade').css('display', 'none');

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
        //AMBULATÓRIO DE ESPECIALIDADES = 5
        var url = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/cad_unidade_saude&acao=A&tusid=5';
        window.open( url, 'alterarEstado', 'width=650, height=300, scrollbars=yes, scrolling=no, resizebled=no' );
    }
    
    function excluirUnidadeSaude(ussid){
        //AMBULATÓRIO DE ESPECIALIDADES = 5
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=excluirUnidadeSaude&tusid=5&ussid="+ussid,
                success: function(data){
                    $("#div_grid_lista_unidades").html(data);
                    divCarregado();
                }
            });
        }
    }    
    
    function listarUnidadeSaude(){
        $('#div_grid_lista_unidades').css('display', '');
    }

    function salvarExisteUnidadeSaude(){
        var erro;
        var campos = '';
        
       $.each($("#formulario_existe_unidade_saude").find('.obrigatorio'), function(i, v){
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
            $('#requisicao_U').val('salvarAvaliacaoDescricaoGerais');
            $('#pagina_U').val('E');
            $('#formulario_existe_unidade_saude').submit();
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
            $('#formulario').submit();
        }
    }

    function voltarAbaPerguntas(){
        window.location.href = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_ss_ambul&acao=A';
    }

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class ="SubTituloDireita" width="25%">Nome do Ambulatório de Especialidades</td>
        <td id="td_nome_unidade_saude"> <input type="text" class="disabled" id="ussdsc" size="45" disabled="disabled">&nbsp;</td>
        <td class ="SubTituloDireita" width="25%">CNES nº</td>
        <td id="td_cnes_unidade_saude"> <input type="text" class="disabled" id="usscnes" disabled="disabled">&nbsp;</td>
    </tr>
</table>

<!-- PERGUNTA ONDE FAZ A VERIFICAÇÃO SE EXISTE UNIDADE DE SAÚDE -->

<form action="" method="POST" id="formulario_existe_unidade_saude" name="formulario_existe_unidade_saude">
    <input type="hidden" id="requisicao_U" name="requisicao_U" value=""/>
    <input type="hidden" id="pagina_U" name="pagina_U" value=""/>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
        <?PHP
            $arrPerguntas = array(206,207);
            echo montaPerguntasQuestionario( '', 'Existência de Ambulatórios de Especialidades ou Policlínicas', '', $arrPerguntas );
        ?>
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" id="salvarExiste" name="salvarExiste" value="Salvar" onclick="salvarExisteUnidadeSaude();"/>
            </td>
        </tr>
    </table>
</form>

<?PHP
    #AMBULATÓRIO DE ESPECIALIDADES (tusid)
    $resposta = buscaRespostaPergunta( 206 );

    if($resposta != ''){
?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td style="padding: 0px;" width="15%">
                    <span style="cursor: pointer" onclick="cadastraUnidadeSaude('H');" title="Nova Unidade de Saúde" >
                        <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> Adicionar Ambulatório
                    </span>
                </td>
            </tr>
        </table>

    <div id="div_grid_lista_unidades">
<?PHP
        montaListaGridUnidadeSaude( UNIDADE_SAUDE_AMBULATORIO );
    }
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
                            $arrPerguntas = array(180,181);
                            echo montaPerguntasQuestionario( 'E - 1)', 'Consultórios', '', $arrPerguntas, NULL, 'E_1');

                            $arrPerguntas = array(182,183);
                            echo montaPerguntasQuestionario( 'E - 2)', 'Salas de Reuniões', '', $arrPerguntas, NULL, 'E_2');

                            $arrPerguntas = array(184,185);
                            echo montaPerguntasQuestionario( 'E - 3)', 'Acesso a Internet', '', $arrPerguntas, NULL, 'E_3');

                            $arrPerguntas = array(186,187);
                            echo montaPerguntasQuestionario( 'E - 3)', 'Acesso à Telesaúde', '', $arrPerguntas, NULL, 'E_4');

                            $arrPerguntas = array(188);
                            echo montaPerguntasQuestionario( 'E - 5)', 'Especialidades Atendidas', '', $arrPerguntas, NULL, 'E_5');

                            $arrPerguntas = array(189,190,191);
                            echo montaPerguntasQuestionario( 'E - 6)', 'Profissionais em Atividade', '', $arrPerguntas, NULL, 'E_6');

                            $arrPerguntas = array(192,194);
                            echo montaPerguntasQuestionario( 'E - 7)', 'Alunos', '', $arrPerguntas, NULL, 'E_7');

                            $arrPerguntas = array(195);
                            echo montaPerguntasQuestionario( 'E - 8)', 'Dados Estatísticos', '', $arrPerguntas, NULL, 'E_8');
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
                    <input type="button" id="voltar" name="voltar" value="Voltar" onclick="voltarAbaPerguntas();"/>
                    <input type="button" id="salvarQuest" name="salvarQuest" value="Salvar" onclick="salvarQuestUnidadeSaude('S');"/>
                </td>
            </tr>
        </table>
    </div>
</form>