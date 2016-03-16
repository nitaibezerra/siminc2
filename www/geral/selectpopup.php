<?PHP
    function atualizarCombo_2( $dados ){
        global $db;
        $param  = $dados['param'];

        $titulo_combo = $_SESSION['select_pop']['titulo_combo'];
        $name_combo   = $_SESSION['select_pop']['name_combo'];
        $id_combo     = $_SESSION['select_pop']['id_combo'];

        if( $param != '' ){
            $sql = $_SESSION['select_pop']['combo2_sql'];
            $sql = sprintf($sql, $param);
        }else{
            $sql = array();
        }

        $db->monta_combo($name_combo, $sql, 'S', 'Selecione...', '', '', '', 380, 'S', $id_combo, false, '', $titulo_combo, '', 'chosen-select');
        echo "<script type=\"text/javascript\">";
        echo "var jq = jQuery.noConflict();";
        echo "jq('.chosen-select').chosen({allow_single_deselect:true});";
        echo "</script>";
        die();
    }
    
    function mataSessaoCombo( $dados ){
        global $db;
        
        unset($_SESSION['select_pop']);
        
        die('S');        
    }

    #inicializa sistema
    require_once "config.inc";
    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    $db = new cls_banco();

    #VERIFICA AS REQUISIÇÕES.
    if( $_REQUEST['requisicao'] ){
        $_REQUEST['requisicao']($_REQUEST);
    }

    #CARREGA OS DADOS REFERENTES AO COMBO E QUE SERAO USADOS NO POPUP
    if (!isset($_SESSION['indice_sessao_select_popup'][$_REQUEST['nome']])) {
        print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
        exit();
    }
    $dados_select = $_SESSION['indice_sessao_select_popup'][$_REQUEST['nome']];

    $titulo     = $dados_select['titulo'];
    $maximo     = $dados_select['maximo'];
    $whereArr   = $dados_select['where'];

    $titulo_combo_1 = $dados_select['titulo_combo_1'];
    $sql_combo_1    = $dados_select['sql_combo_1'];
    $name_combo_1   = $dados_select['name_combo_1'];
    $id_combo_1     = $dados_select['id_combo_1'];

    $titulo_combo_2 = $dados_select['titulo_combo_2'];
    $sql_combo_2    = str_replace('%s', '', $dados_select['sql_combo_2']);
    $name_combo_2   = $dados_select['name_combo_2'];
    $id_combo_2     = $dados_select['id_combo_2'];

    #SESSAO PARA SER USADO NO COMBO CRIADO NO AJAX.
    $_SESSION['select_pop']['titulo_combo'] = $dados_select['titulo_combo_2'];
    $_SESSION['select_pop']['name_combo']   = $dados_select['name_combo_2'];
    $_SESSION['select_pop']['id_combo']     = $dados_select['id_combo_2'];

    #CARREGA A SQL E A CLASSULA WHERE PARA SER USADO NO COMBO CRIADO NO AJAX.
    if( $dados_select['where_combo'] != '' ){
        $_SESSION['select_pop']['combo2_sql'] = $dados_select['sql_combo_2']."\n";
        $_SESSION['select_pop']['combo2_sql'] .= $dados_select['where_combo']."\n";
        $_SESSION['select_pop']['combo2_sql'] .= $dados_select['orderby'];
    }

    $nome_popup = $_REQUEST['nome'];
    
    if($titulo == ''){
        $titulo = 'Instrumento de Seleção';
    }

    monta_titulo( $titulo,"" );

?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>
<link href="../library/chosen-1.0.0/chosen.css" type="text/css" rel="stylesheet" media="screen" >

<style>
    a.chosen-single {
        -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
        border-radius: 5px !important;
        box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
        display: block !important;
        height: 25px !important;
        line-height: 25px !important;
        overflow: hidden !important;
        padding: 0 0 0 8px !important;
        position: relative !important;
        text-decoration: none !important;
        white-space: nowrap !important;
    }
</style>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

<script type="text/javascript">
    jQuery(document).ready(function(){
        jQuery(window).unload(
            jQuery('.chosen-select').chosen({allow_single_deselect : true})
        );
    });

    function atualizarCombo_2( param ){
         $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizarCombo_2&param="+param,
            async: false,
            success: function(resp){
                $('#td_combo_2').html(resp);
            }
        });
    }

   function salvarCursosPos(){
        var erro, cont, value;
        var campos = '', texto = '';
        var name;
        var obj_pai = $('#nome_obj_pai').val();

        cont = 0;
        $.each($("#formulario").find('.obrigatorio'), function(i, v){
            if ( $(v).val() == '' ){
                erro = 1;
                campos += '- ' + $(this).attr('title') + " \n";
            }else{
                name  = $(this).attr('name');
                value = $(this).attr('value');
                if( cont == 0 ){
                    texto = $(this).find('option[selected=true]').text();
                    texto += ' - ';
                }else{
                    texto += $(this).find('option[selected=true]').text();
                }
            }
            cont = cont + 1;
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            if ( opener.$('#'+obj_pai).children().eq(0).val() == '' ){
                opener.jQuery('#'+obj_pai).children().eq(0).remove()
            }
            opener.$('#'+obj_pai).append(new Option(texto, value, true, true));
        }
        
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=mataSessaoCombo&limpa=S",
            async: false,
            success: function(resp){
                if( resp == 'S' ){
                    window.close();
                }else{
                    alert('Ocorreu um erro com o componete, fecho o PopUp e tente novamente!');
                }
            }
        });
        
    }

</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="nome_obj_pai" name="nome_obj_pai" value="<?=$_REQUEST['nome'];?>"/>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="SubTituloDireita" width="30%"><?=$titulo_combo_1;?></td>
            <td>
                <?PHP
                    $sql = $sql_combo_1;
                    $db->monta_combo($name_combo_1, $sql, 'S', 'Selecione...', 'atualizarCombo_2', '', '', 380, 'S', $id_combo_1, false, '', $titulo_combo_1, '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"><?=$titulo_combo_2;?></td>
            <td id="td_combo_2">
                <?PHP
                    $sql = $sql_combo_2;
                    $db->monta_combo($name_combo_2, $sql, 'S', 'Selecione...', '', '', '', 380, 'S', $id_combo_2, false, '', $titulo_combo_2, '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2" >
                <input type="button" name="salvar" id="salvar" value="Salvar" onclick="salvarCursosPos();"/>
                <input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();"/>
            </td>
        </tr>
    </table>
</form>