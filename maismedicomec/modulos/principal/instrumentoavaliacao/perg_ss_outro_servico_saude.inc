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
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_ss_outro_servico_saude&acao=A';
    $parametros     = '';

    $tipoUnidadeSaude = 'Hospital';

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
    
    function salvarAvaliacaoDescricaoGerais( param ){
        var erro;
        var campos = '';

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

    function voltarAbaPerguntas(){
        window.location.href = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_desc_apectos_gerais&acao=A';
    }

</script>

<div id="div_grid_lista_unidades" style="display: none;">
    <?PHP
        montaListaGridUnidadeSaude( UNIDADE_SAUDE_HOSPITAL );
    ?>
</div>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <div id="div_formulario_perguntas">
        <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
            <tr>
                <td width="95%" style="vertical-align: top;">
                    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela listagem" >
                        <tr>
                            <td class ="SubTituloCentro" colspan="3"> 4. Outros Serviços de Saúde.</td>
                        </t>
                        <?PHP
                            $arrPerguntas = array(196);
                            echo montaPerguntasQuestionario( '', '', '', $arrPerguntas, NULL, NULL);
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
    </div>
</form>