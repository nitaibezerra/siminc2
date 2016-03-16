<?PHP
    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Administrativo - Eventos', 'Cadastrar do Pregão' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/cad_eve_pregao&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral();
    
    /**
     * functionName abrirTelaCadastroPregao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string preid id do pregão
     * @return string "modal - tela" tela do cadastro do pregão.
     *
     * @version v1
    */
    function abrirTelaCadastroPregao( $dados ) {

        if( $dados['preid'] > 0 ){
            $dados = editarPregao( $dados['preid'] );
        }
?>
        <form action="" method="POST" id="formulario_cadastro" name="formulario_cadastro">
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <input type="hidden" name="preid" id="preid" value="<?=$dados['preid'];?>">

             <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloCentro" align="right" style="font-size: 15px;"> CADASTRO PREGÃO </td>
                </tr>
             </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloDireita" align="right"> Número Processo: </td>
                    <td>
                        <?PHP
                            $prenumprocesso = $dadosPregao['prenumprocesso'];
                            echo campo_texto('prenumprocesso', 'S', $somenteLeitura, 'Número Processo', 75, 20, '#####.######/####-##', '', 'left', '', 0, 'id="prenumprocesso"', '', $dados['prenumprocesso'], '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Código do Pregão: </td>
                    <td>
                        <?PHP
                            $precodpregao = $dadosPregao['precodpregao'];
                            echo campo_texto('precodpregao', 'S', $somenteLeitura, 'Código do Pregão', 75, 7, '', '', 'left', '', 0, 'id="precodpregao"', '', $dados['precodpregao'], '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Descrição: </td>
                    <td>
                        <?PHP
                            $predescpregao = $dadosPregao['predescpregao'];
                            echo campo_textarea('predescpregao', 'S', 'S', 'Descrição', 84, 8, '1000', '', '0', '', '', 'Descrição', $dados['predescpregao']);
                        ?>
                    </td>
                </tr>
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class="SubTituloCentro" style="font-weight: bold">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosPregao();"/>
                        <input type="button" id="fechar" name="fechar" value="Fechar" class="modalCloseImg simplemodal-close"/>
                    </td>
                </tr>
            </table>
        </form>
<?PHP
    die();
    }
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<link rel="stylesheet" type="text/css" href="../evento/css/modal_css_basic.css"/>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript" src="../evento/js/jquery.simplemodal.js"></script>

<style type="text/css">
    #simplemodal-container {
        height: 460px !important;
        width: 55%  !important;
        color: #bbb  !important;
        /*background-color: #333;*/
        background-color: #FFFFFF  !important;
        border: 4px solid #444  !important;
        padding: 5px  !important;
    }
</style>

<script type="text/javascript">
    
    function abrirTelaCadastroPregao( preid ){
        var url;
        if( typeof(preid) == 'undefined' ){
            url = "requisicao=abrirTelaCadastroPregao";
        }else{
            url = "requisicao=abrirTelaCadastroPregao&preid="+preid;
        }
        
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: url,
            success: function (resp){
                $('#form_tela_cadastro_pregao').html(resp);
                $('#form_tela_cadastro_pregao').modal();
                divCarregado();
            }
        });
    }
    
    function editarPregao( id ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=editarPregao&preid="+id,
            success: function(resp){
                var dados = $.parseJSON(resp);
                $.each(dados, function(index, value) {
                   $('#'+index).val(value);
                });
            }
        });
    }
    
    function excluirPregao( id ){
        var confirma = confirm("Deseja realmente EXCLUIR o registro?");
        
        if( confirma ){
            $('#preid').val(id);
            $('#formulario_pesquisa').find('#requisicao').val('excluirPregao');
            $('#formulario_pesquisa').submit();
        }
    }
    

    function resetFromulario(){
        $('#selecionar').attr('disabled', false);
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
    }

    function salvarDadosPregao(){
        var erro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val()){
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
            $('#formulario_cadastro').find('#requisicao').val('salvarDadosPregao');
            $('#formulario_cadastro').submit();
        }
    }
    
    function pesquisarPregrao(){
        $('#formulario_pesquisa').submit();
    }

</script>

<div id="form_tela_cadastro_pregao" style="display: none;"></div>

<form action="" method="POST" id="formulario_pesquisa" name="formulario_pesquisa">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="preid" id="preid" value="">

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class ="SubTituloDireita" width="30%"> Número Processo: </td>
            <td>
                <?PHP
                    echo campo_texto('prenumprocesso', 'N', 'S', 'Número Processo', 50, 20, '#####.######/####-##', '', 'left', '', 0, 'id="prenumprocesso"', '', $prenumprocesso, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Código do Pregão: </td>
            <td>
                <?PHP
                    echo campo_texto('precodpregao', 'N', 'S', 'Código do Pregão', 50, 7, '', '', 'left', '', 0, 'id="precodpregao"', '', $precodpregao, '', null);
                ?>
            </td>
        </tr>        
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="button" id="Pesquisar" name="pesquisar" value="pesquisar" onclick="pesquisarPregrao();"/>
                <input type="button" id="todos" name="todos" value="Ver Todos" onclick="resetFromulario(); pesquisarPregrao();"/>
            </td>
        </tr>
    </table>
    
    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class="SubTituloEsquesrdo">
                <input type="button" id="novo" name="novo" value="Novo Pregão" onclick="abrirTelaCadastroPregao();"/>
            </td>
        </tr>
    </table>
</form>

<?PHP
    $prenumprocesso = $_REQUEST['prenumprocesso'];
    $precodpregao   = $_REQUEST['precodpregao'];

    if( $prenumprocesso != '' ){
        $WHERE = " AND prenumprocesso ilike ('%{$prenumprocesso}%')";
    }
    
    if( $precodpregao != '' ){
        $WHERE .= " AND precodpregao ilike ('%{$precodpregao}%')";
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirTelaCadastroPregao('||preid||');\" title=\"Editar Pregão\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirPregao('||preid||');\" title=\"Excluir Pregão\" >
    ";
    
    $sql = "
        SELECT  '{$acao}',
                prenumprocesso AS prenumprocesso, 
                precodpregao AS precodpregao, 
                predescpregao,
                CASE WHEN prestatus = 'A'
                    THEN 'Ativo'
                    ELSE 'Inativo'
                END AS prestatus                    
        FROM evento.pregaoevento
        WHERE prestatus = 'A' {$WHERE}
        ORDER BY preid
    ";
    $cabecalho = array("Ação", "Número Processo", "Código do Pregão", "Descrição", "Status");  
    $alinhamento = Array('center', 'right', 'right', '', 'center');
    $tamanho = Array('5%', '10%', '10%', '50%', '5%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>
