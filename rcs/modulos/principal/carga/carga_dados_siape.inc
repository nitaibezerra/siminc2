<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Reconhecimento de Saberes e Competências', 'Carga dos dados SIAPE' );

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_aval_documental&acao=A';
    $parametros     = '';
    //$db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral($_SESSION['usucpf']);
    
    $arryServidor = $_SESSION['rsc']['array']['lista_servidor'];
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">

    function exportarDados(){
        var erro;
        var campos = '';
        
        var result = verificaExtensao();
        
        if(!result){
            return false;
        }

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
            $('#requisicao').val('exportarDados');
            $('#formulario').submit();
        }
    }

    function verificaExtensao() {
        var arquivo = $('#arquivo').val();
        
        if ( arquivo != '' ){
            var extensao = arquivo.split(".")[1].toLowerCase();
            
            if (extensao != 'csv') {
                alert('Extensão não permitida! Envie apenas arquivo com extensão CSV.');
                $('#arquivo').val('');
                return false;
            }
        }
        return true;
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'rcs.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }

    function info_carga(){
        var janela = window.open('rcs.php?modulo=principal/carga/info_carga&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
        janela.focus();
    }
</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="srpid" name="srpid" value="<?=$srpid?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;">Upload do arquivo de Carga</td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Informativo para a carga: </td>
            <td colspan="2" style="text-transform:uppercase;">
                <a style="font-size:12px; font-weight: bold; cursor:pointer; color:#133368;" href="#" onclick="info_carga();"> clique aqui </a>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Data de Realização da Carga: </td>
            <td colspan="2">
                <?PHP
                    $srpdtnascimento = date('d/m/Y');
                    echo campo_data2('srpdtnascimento', 'S', 'N', 'Data de Nascimento', '##/##/####', '', '', $srpdtnascimento, '', '', 'srpdtnascimento');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Arquivo: </td>
            <td colspan="2">
                <input type="file" name="arquivo" id="arquivo" class="obrigatorio" title="Arquivo">
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
               <input type="button" id="salvar" name="salvar" value="Importar" onclick="exportarDados();"/>
               <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="voltarPaginaPrincipal();"/>
               <?PHP
               
               ?>
            </td>
        </tr>
    </table>
</form>

<?PHP
    if( $_SESSION['rsc']['array']['executar_listagem'] == 'S' ){
        if( $arryServidor != '' ){
            //$arryServidor = $_SESSION['rsc']['array']['lista_servidor'];

            $html = "<table id=\"table_msg\" align=\"center\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" cellspacing=\"1\" class=\"tabela\"> ";
            $html .= "<tr>";
            $html .= "<th style=\"color: red;\">Lista de todos os servidores que tiveram os seus dados inseridos na Base de Dados do RSC.</th>";
            $html .= "</tr>";
            $html .= "</table>";

            $html .= "<table id=\"table_listagem\" align=\"center\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" cellspacing=\"1\" class=\"listagem tabela\"> ";
            $html .= "<tr>";
            $html .= "<th>SIAPE</th><th>CPF</th><th>Servidor</th>";
            $html .= "</tr>";

            foreach($arryServidor as $servidor){
                $html .= "<tr>";
                $html .= "<td>{$servidor['dsamatricula']}</td>";
                $html .= "<td>{$servidor['dsacpf']}</td>";
                $html .= "<td>{$servidor['dsadsc']}</td>";
                $html .= "</tr>";;
            }
            $html .= "</table>";            
            
        }else{
            $html = "<table id=\"table_msg\" align=\"center\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" cellspacing=\"1\" class=\"tabela\"> ";
            $html .= "<tr>";
            $html .= "<th style=\"color: red;\">Não há dados a serem carregados na Base de Dados do RSC. Possívelmente os Servidores já estavam cadastrados no RSC.</th>";
            $html .= "</tr>";
            $html .= "</table>";
        }
//        unset($_SESSION['rsc']['array']);
        unset($_SESSION['rsc']);
    }
    echo $html;
?>