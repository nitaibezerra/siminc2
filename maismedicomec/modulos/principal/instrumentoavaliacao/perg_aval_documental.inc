<?PHP

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    #ATRIBUI O VALOR NAS VARIAVEIS DE SESSAO
    if( $_REQUEST['muncod'] != '' ){
        iniciaVariaveisSessao( $_REQUEST['muncod'], 'M' );
        
        if( $_SESSION['maismedicomec']['muncod'] != '' ){ 
            $etqid = cadastraEntidadeQuestionarioMunicipio( $_SESSION['maismedicomec']['muncod'] );
            
            if( $etqid != '' ){
                iniciaVariaveisSessao( $etqid, 'E' );
            }
        }
        
    }else{
        if( $_SESSION['maismedicomec']['muncod'] == '' ){ 
            $db->sucesso('principal/inicio_direcionamento', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_aval_documental&acao=A';
    $parametros     = '';

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

    function salvarAvaliacaoDescricaoGerais(){
        var erro;
        var campos = '';
        
        var dataIni = $('#pergunta_1').val();
        var dataFim = $('#pergunta_2').val();
        
        var dataValida = validaDataInicialMaior(dataIni, dataFim);
        if(trim( dataValida ) == 'erro'){
            alert('Por favor CORRIJA. Data Inicial maior que a data Final!');
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
            $('#requisicao').val('salvarAvaliacaoDescricaoGerais');
            $('#formulario').submit();
        }
    }
    
    function validaDataInicialMaior(dataIni, dataFim){
        var dataValida;
        
	$.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=validaDataInicialMaior&dataIni="+dataIni+'&dataFim='+dataFim,
            async: false,
            success: function(resp){                
                dataValida = resp;
            }
        });
        return dataValida;
    }
    
    function voltarAbaPerguntas(){
        window.location.href = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/lista_grid_municipio&acao=A';
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="muncod" name="muncod" value="<?=$_SESSION['maismedicomec']['muncod'];?>"/>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
        <tr>
            <td width="95%" style="vertical-align: top;">
                <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela listagem" >
                    <?PHP
                        $arrPerguntas = array(1,2,3,4,5);
                        echo montaPerguntasQuestionario( '', '', '', $arrPerguntas, NULL, NULL );
                   ?>
                    <tr>
                        <td class ="SubTituloCentro" colspan="2" style="height: 35px;"> 1. Avaliação documental. Relatar os documentos avaliados e a sua pertinência/importância.</td>
                    </t>
                    <?PHP
                        #perguntas inativadas: 7,8,9
                        $arrPerguntas = array(6,10,11,12,13,14,15);
                        echo montaPerguntasQuestionario( '', '', '', $arrPerguntas, NULL, NULL );
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
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <?PHP
                    $habilita = habilitaPerfilEstadoAcao();
                    
                    if( $habilita == 'S' ){
                ?>
                        <input type="button" id="voltar" name="voltar" value="Voltar" title="Volta a Listagem de Municípios" onclick="voltarAbaPerguntas();"/>
                        <input type="button" id="salvar_sem_continui" name="salvar_sem_continui" value="Salvar" onclick="salvarAvaliacaoDescricaoGerais();"/>
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
</form>