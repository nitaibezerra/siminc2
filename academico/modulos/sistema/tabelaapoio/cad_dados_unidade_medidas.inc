<?PHP

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }
    
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
    function abrirTelaCadastroUndMedida( $dados ) {

        if( $dados['undid'] > 0 ){
            $dados = editarUndMedida( $dados['undid'] );
        }
?>
        <form action="" method="POST" id="formulario_cadastro" name="formulario_cadastro">
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <input type="hidden" name="undid" id="undid" value="<?=$dados['undid'];?>">

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloCentro" align="right" style="font-size: 15px;"> CADASTRO DE UNIDADE DE MEDIDA </td>
                </tr>
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloDireita" width="35%"> Código Und. Medida: </td>
                    <td>
                        <?PHP
                            $undid = $dados['undid'];
                            echo campo_texto('undid', 'N', 'N', 'Código Und. Medida', 20, 20, '', '', 'left', '', 0, 'id="undid"', '', $dados['undid'], '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita"> Descrição da Und. Medida: </td>
                    <td>
                        <?PHP
                            $unddsc = $dados['unddsc'];
                            echo campo_texto('unddsc', 'S', 'S', 'Descrição da Und. Medida', 60, 100, '', '', 'left', '', 0, 'id="$unddsc"', '', $dados['unddsc'], '', null);
                        ?>
                    </td>
                </tr>
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class="SubTituloCentro" style="font-weight: bold">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDados();"/>
                        <input type="button" id="fechar" name="fechar" value="Fechar" class="modalCloseImg simplemodal-close"/>
                    </td>
                </tr>
            </table>
        </form>
<?PHP
    die();
    }
    
    /**
    * functionName editarUndMedida
    *
    * @author Luciano F. Ribeiro
    *
    * @param string $undid id da tabela unidademed
    * @return string registro de dados referentes ao "id".
    *
    * @version v1
    */
    function editarUndMedida( $undid ){
        global $db;

        $sql = "
            SELECT  undid,
                    unddsc
            FROM academico.unidademed

            WHERE undid = '{$undid}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            return $dados;
        }
    }
    
    function excluirUndMedida( $dados ){
        global $db;
        
        $undid = $dados['undid'];
        
        if( $undid > 0 ){
            $sql = "DELETE FROM academico.unidademed WHERE undid = {$undid} RETURNING undid";
            $result = $db->pegaUm($sql);
        }
        
        if( $result > 0){
            $db->commit();
            $db->sucesso( 'sistema/tabelaapoio/cad_dados_unidade_medidas', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'sistema/tabelaapoio/cad_dados_unidade_medidas', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }
    
    function salvarDados( $dados ){
        global $db;
        
        $undid = $dados['undid'];
        $unddsc = trim( addslashes($dados['unddsc']) );
        
        if( $undid > 0 ){
            $sql = "
                UPDATE academico.unidademed
                    SET unddsc = '{$unddsc}'
                WHERE undid = {$undid} RETURNING undid
            ";
            $result = $db->pegaUm($sql);
        }else{
            $sql = "
                INSERT INTO academico.unidademed( unddsc )VALUES( '{$unddsc}' ) RETURNING undid
            ";
            $result = $db->pegaUm($sql);
        }
        
        if( $result > 0){
            $db->commit();
            $db->sucesso( 'sistema/tabelaapoio/cad_dados_unidade_medidas', '', "A operação foi realizada com sucesso!");
        }else{
            $db->rollback();
            $db->sucesso( 'sistema/tabelaapoio/cad_dados_unidade_medidas', '', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Cadastro de Unidades de Medida', 'Rede Federal' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/cad_eve_pregao&acao=A';
    $parametros     = '';

    //$db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral();

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
        height: 250px !important;
        width: 55%  !important;
        color: #bbb  !important;
        /*background-color: #333;*/
        background-color: #FFFFFF  !important;
        border: 4px solid #444  !important;
        padding: 5px  !important;
    }
</style>

<script type="text/javascript">
    
    function abrirTelaCadastroUndMedida( undid ){
        var url;
        if( typeof(undid) == 'undefined' ){
            url = "requisicao=abrirTelaCadastroUndMedida";
        }else{
            url = "requisicao=abrirTelaCadastroUndMedida&undid="+undid;
        }
        
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: url,
            success: function (resp){
                $('#form_tela_cadastro_uni_medida').html(resp);
                $('#form_tela_cadastro_uni_medida').modal();
                divCarregado();
            }
        });
    }
    
    function excluirUndMedida( id ){
        var confirma = confirm("Deseja realmente EXCLUIR o registro?");
        if( confirma ){
            $('#formulario_pesquisa').find('#undid').val(id);
            $('#formulario_pesquisa').find('#requisicao').val('excluirUndMedida');
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

    function salvarDados(){
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
            $('#formulario_cadastro').find('#requisicao').val('salvarDados');
            $('#formulario_cadastro').submit();
        }
    }
    
    function pesquisarUndMedida(){
        $('#formulario_pesquisa').submit();
    }

</script>

<div id="form_tela_cadastro_uni_medida" style="display: none;"></div>

<form action="" method="POST" id="formulario_pesquisa" name="formulario_pesquisa">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="undid" id="undid" value="">

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class ="SubTituloDireita" width="35%"> Código Und. Medida: </td>
            <td>
                <?PHP
                    echo campo_texto('undid', 'N', 'S', 'Código Und. Medida', 20, 20, '', '', 'left', '', 0, 'id="undid"', '', '', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Descrição da Und. Medida: </td>
            <td>
                <?PHP
                    echo campo_texto('unddsc', 'N', 'S', 'Descrição da Und. Medida', 60, 100, '', '', 'left', '', 0, 'id="$unddsc"', '', '', '', null);
                ?>
            </td>
        </tr>       
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="button" id="Pesquisar" name="pesquisar" value="pesquisar" onclick="pesquisarUndMedida();"/>
                <input type="button" id="todos" name="todos" value="Ver Todos" onclick="resetFromulario(); pesquisarUndMedida();"/>
            </td>
        </tr>
    </table>
    
    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class="SubTituloEsquesrdo">
                <input type="button" id="novo" name="novo" value="Nova Und. Medida" onclick="abrirTelaCadastroUndMedida();"/>
            </td>
        </tr>
    </table>
</form>

<?PHP
    $undid  = $_REQUEST['undid'];
    $unddsc = $_REQUEST['unddsc'];

    if( $undid != '' ){
        $WHERE = " AND undid = {$undid} ";
    }
    
    if( $unddsc != '' ){
        $WHERE .= " AND unddsc ilike ('%{$unddsc}%')";
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirTelaCadastroUndMedida('||undid||');\" title=\"Editar Unidade de Medida\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirUndMedida('||undid||');\" title=\"Excluir Unidade de Medida\" >
    ";
    
    $sql = "
        SELECT  '{$acao}',
                undid AS codigo,
                unddsc                    
        FROM academico.unidademed
        WHERE 1 = 1 {$WHERE}
        ORDER BY undid
    ";
    $cabecalho = array("Ação", "Código", "Descrição");  
    $alinhamento = Array('center', 'center', '');
    $tamanho = Array('5%', '5%', '');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>
