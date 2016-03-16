<?php
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    
    $abacod_tela    = 57214;
    $url            = 'gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_servidores_cedidos&acao=A';
    $parametros     = '&cprid='.$cprid;
    $db->cria_aba($abacod_tela, $url, $parametros);
    
    echo "<br>";
    monta_titulo('Força de Trabalho', 'Dados do servidor cedico');
    
    echo cabecalhoPessoa($_SESSION['fdpcpf']);
    
    if( $_SESSION['fdpcpf'] != '' ){
        $dados = buscarDadosServidorCedido($_SESSION['fdpcpf']);

        if( $dados['fdpcargodas'] == 't' ){
            $check_sim = 'checked="checked"';
            $disabled  = '';
        }else{
            $check_nao = 'checked="checked"';
            $disabled  = 'disabled="disabled"';
        }

        switch( $dados['fdptipodas'] ){
            case 4:
                $check_4 = 'checked="checked"';
                break;
            case 5:
                $check_5 = 'checked="checked"';
                break;
            case 6:
                $check_6 = 'checked="checked"';
                break;
        }
    }
    
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

    function desabilitarPerg( opc ){
        if( opc == 'S' ){
            $.each($(".radio_das"), function(i, v){
                $('#'+v.id).attr('disabled', false);
                $('#'+v.id).removeClass('radio_das');
                $('#'+v.id).addClass('obrigatorio radio_das');
            });                
        }else{
            $.each($(".radio_das"), function(i, v){
                console.log(v.id);
                $('#'+v.id).attr('disabled', true);
                $('#'+v.id).removeClass('obrigatorio');
            });
        }
    }
    
    function salvarDadosServCedidos( opc ){
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
                        
                        if(value == '4' || value == '5' || value == '6'){
                            campos = '- ' + $(this).attr('title') + " \n";
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
            $('#requisicao').val('salvarDadosServCedidos');
            $('#formulario').submit();
        }
    }

    function validar_email( email ){
        if( email != '' ){
            var result = validaEmail( email );
            if( !result ){
                alert('O e-mail digitado não é valido!');
            }
        }
    }
    
     
</script>

<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="fdpcpf" name="fdpcpf" value="<?=$_SESSION['fdpcpf'];?>"/>

    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr><td class ="SubTituloCentro" style="text-align: left;" colspan="2">Informações sobre o Cargo</td></tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">Possui cargo de confiança equivalente ao DAS 4, 5 ou 6?:</td>
            <td>
                <input type="radio" class="obrigatorio" id="fdpcargodas" name="fdpcargodas" value="S" title="Possui cargo de confiança" style="margin-left:1px;" onclick="desabilitarPerg('S');" <?=$check_sim;?>>Sim
                <input type="radio" class="obrigatorio" id="fdpcargodas" name="fdpcargodas" value="N" title="Possui cargo de confiança" style="margin-left:10px;" onclick="desabilitarPerg('N');" <?=$check_nao;?>>Não
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Se sim, qual?:</td>
            <td>
                <input type="radio" class="radio_das" id="fdptipodas_0" name="fdptipodas" value="4" title="qual?" style="margin-left:1px;" <?=$disabled;?> <?=$check_4;?>>DAS 4
                <input type="radio" class="radio_das" id="fdptipodas_1" name="fdptipodas" value="5" title="qual?" style="margin-left:10px;" <?=$disabled;?> <?=$check_5;?>>DAS 5
                <input type="radio" class="radio_das" id="fdptipodas_2" name="fdptipodas" value="6" title="qual?" style="margin-left:20px;" <?=$disabled;?> <?=$check_6;?>>DAS 6
            </td>
        </tr>
        <tr><td class ="SubTituloCentro" style="text-align: left;" colspan="2">Local de Trabalho</td></tr>
        <tr>
            <td class ="SubTituloDireita">Nome:</td>
            <td colspan="2">
               <?php
                    $fdplocaltrabalho = $dados['fdplocaltrabalho'];
                    echo campo_texto('fdplocaltrabalho', 'S', 'S', 'Local de Trabalho-nome', 80, 255, '', '', '', '', 0, 'id="fdplocaltrabalho"', '', $fdplocaltrabalho, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Telefone:</td>
            <td colspan="2">
               <?php
                    $fdpfonelocaltrabalho = $dados['fdpfonelocaltrabalho'];
                    echo campo_texto('fdpfonelocaltrabalho', 'S', 'S', 'Local de Trabalho-telefone', 20, 20, '##-####-####', '', '', '', 0, 'id="fdpfonelocaltrabalho"', '', $fdpfonelocaltrabalho, '', null);
                ?>
            </td>
        </tr>
        <tr><td class ="SubTituloCentro" style="text-align: left;" colspan="2">Chefia imediata</td></tr>
        <tr>
            <td class ="SubTituloDireita">Nome:</td>
            <td colspan="2">
                <?php
                    $fdpchefiaimediataloctrab = $dados['fdpchefiaimediataloctrab'];
                    echo campo_texto('fdpchefiaimediataloctrab', 'S', 'S', 'Chefia imediata-nome', 80, 255, '', '', '', '', 0, 'id="fdpchefiaimediataloctrab"', '', $fdpchefiaimediataloctrab , '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">E-mail:</td>
            <td colspan="2">
                <?php
                    $fdpemailchefiaimediata = $dados['fdpemailchefiaimediata'];
                    echo campo_texto('fdpemailchefiaimediata', 'S', 'S', 'Chefia imediata-e-mail', 80, 255, '', '', '', '', 0, 'id="fdpemailchefiaimediata"', '', $fdpemailchefiaimediata , 'validar_email(this.value)', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Telefone:</td>
            <td colspan="2">
                <?php
                    $fdpfonechefiaimediata  = $dados['fdpfonechefiaimediata'];
                    echo campo_texto('fdpfonechefiaimediata', 'S', 'S', 'Chefia imediata-telefone', 20, 20, '##-####-####', '', '', '', 0, 'id="fdpfonechefiaimediata"', '', $fdpfonechefiaimediata , '', null);
                ?>
            </td>
        </tr>
    </table>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" id="salvarProcesso" name="salvar" value="Salvar" onclick="salvarDadosServCedidos();"/>
            </td>
        </tr>
    </table>
</form>

