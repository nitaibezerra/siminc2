<?PHP
    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Administrativo - Eventos', 'Cadastrar Contrato' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/cad_eve_contrato&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral();

    /**
     * functionName abrirTelaCadastroContrato
     *
     * @author Luciano F. Ribeiro
     *
     * @param string coedi id do contrato
     * @return string "modal - tela" tela do cadastro do contrato.
     *
     * @version v1
    */
    function abrirTelaCadastroContrato( $dados ) {
        global $db;
        if( $dados['coeid'] != '' ){
            $dadosContrato = editarContrato( $dados['coeid'] );
        }
?>
        <form action="" method="POST" id="formulario_cadastro" name="formulario_cadastro">
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <input type="hidden" name="coeid" id="coeid" value="<?=$dadosContrato['coeid'];?>">

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloCentro" style="font-size:15px;"> Cadastro de Contrato </td>
                </tr>
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloDireita" align="right"> Pregão: </td>
                    <td id="td_combo_preid">
                        <?PHP
                            $preid = $dadosContrato['preid'];
                            $sql = "
                                SELECT  preid AS codigo,
                                        precodpregao ||' - '|| prenumprocesso AS descricao
                                FROM evento.pregaoevento
                                ORDER BY descricao
                            ";
                            $db->monta_combo('preid', $sql, 'S', "Selecione...", '', '', '', 600, 'S', 'preid', '', $preid, 'Pregão', '', '');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Número do Contrato: </td>
                    <td>
                        <?PHP
                            $coenumcontrato = $dadosContrato['coenumcontrato'];
                            echo campo_texto('coenumcontrato', 'S', $habilita, 'Número do Contrato', 24, 20, '', '', 'left', '', 0, 'id="coenumcontrato"', '', $coenumcontrato, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Data de início da Vigência: </td>
                    <td>
                        <?PHP
                            $coeiniciovig = $dadosContrato['coeiniciovig'];
                            echo campo_data2('coeiniciovig','S','S','Data de início da Vigência','DD/MM/YYYY','','', $coeiniciovig, '', '', 'coeiniciovig' );
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Data fim da Vigência: </td>
                    <td>
                        <?PHP
                            $coefimvig = $dadosContrato['coefimvig'];
                            echo campo_data2('coefimvig','S','S','Data fim da Vigência','DD/MM/YYYY','','verificaDatafinal();', $coefimvig, 'verificaDatafinal();', '', 'coefimvig' );
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Valor Contratado: </td>
                    <td>
                        <?PHP
                            $coevalorcontratado = $dadosContrato['coevalorcontratado'];
                            echo campo_texto('coevalorcontratado', 'N', $habilita, 'Valor Contratado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coevalorcontratado"', '', $coevalorcontratado, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Valor Empenhado: </td>
                    <td>
                        <?PHP
                            $coevalorempenhado = $dadosContrato['coevalorempenhado'];
                            echo campo_texto('coevalorempenhado', 'N', $habilita, 'Valor Empenhado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coevalorempenhado"', '', $coevalorempenhado, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Salado do início do Contrato: </td>
                    <td>
                        <?PHP
                            $coesaldoinicontrato = $dadosContrato['coesaldoinicontrato'];
                            echo campo_texto('coesaldoinicontrato', 'S', $habilita, 'Valor do início do Contrato', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coesaldoinicontrato"', '', $coesaldoinicontrato, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Valor utilizado: </td>
                    <td>
                        <?PHP
                            $coevalorutilizado = $dadosContrato['coevalorutilizado'];
                            echo campo_texto('coevalorutilizado', 'N', 'N', 'Valor utilizado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coevalorutilizado"', '', $coevalorutilizado, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Saldo do início do exercício: </td>
                    <td>
                        <?PHP
                            $coesaldoiniexercicio = $dadosContrato['coesaldoiniexercicio'];
                            echo campo_texto('coesaldoiniexercicio', 'N', 'N', 'Valor do início do exercício', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coesaldoiniexercicio"', '', $coesaldoiniexercicio, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Saldo do fim do Contrato: </td>
                    <td>
                        <?PHP
                            $coesaldofimcontrato = $dadosContrato['coesaldofimcontrato'];
                            echo campo_texto('coesaldofimcontrato', 'N', 'N', 'Valor utilizado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coesaldofimcontrato"', '', $coesaldofimcontrato, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Número do Processo: </td>
                    <td>
                        <?PHP
                            $coenumprocesso = $dadosContrato['coenumprocesso'];
                            echo campo_texto('coenumprocesso', 'S', 'S', 'Número do Processo', 24, 20, '#####.######/####-##', '', 'right', '', 0, 'id="coenumprocesso"', '', $coenumprocesso, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> CNPJ: </td>
                    <td>
                        <?PHP
                            $coecnpj = $dadosContrato['coecnpj'];
                            echo campo_texto('coecnpj', 'S', 'S', 'CNPJ', 24, 20, '##.###.###/####-##', '', 'left', '', 0, 'id="coecnpj"', '', $coecnpj, 'verificarCNPJValido(this.value);', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Razão Social: </td>
                    <td>
                        <?PHP
                            $coerazaosocial = $dadosContrato['coerazaosocial'];
                            echo campo_texto('coerazaosocial', 'S', 'S', 'Razão Social', 70, 100, '', '', 'left', '', 0, 'id="coerazaosocial"', '', $coerazaosocial, '', null);
                        ?>
                    </td>
                </tr>
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosContrato();"/>
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

<link rel="stylesheet" type="text/css" href="../evento/css/modal_css_basic.css"/></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript" src="../evento/js/jquery.simplemodal.js"></script>

<style type="text/css">
    #simplemodal-container {
        height: 620px !important;
        width: 60%  !important;
        color: #bbb  !important;
        /*background-color: #333;*/
        background-color: #FFFFFF  !important;
        border: 4px solid #444  !important;
        padding: 5px  !important;
    }
</style>

<script type="text/javascript">

    function abrirTelaCadastroContrato( coeid ){
        var url;
        if( coeid == '' ){
            url = "requisicao=abrirTelaCadastroContrato";
        }else{
            url = "requisicao=abrirTelaCadastroContrato&coeid="+coeid;
        }

        $.ajax({
            type: "POST",
            url: window.location.href,
            data: url,
            success: function (resp){
                $('#form_tela_cadastro_contrato').html(resp);
                $('#form_tela_cadastro_contrato').modal();
                divCarregado();
            }
        });
    }

    function excluirContrato( id ){
        var confirma = confirm("Deseja realmente EXCLUIR o registro?");

        if( confirma ){
            $('#formulario_pesquisa').find('#coeid').val(id);
            $('#formulario_pesquisa').find('#requisicao').val('excluirContrato');
            $('#formulario_pesquisa').submit();
        }
    }

    function infomeEvento(coeid, tipo){
        var url = 'evento.php?modulo=principal/eventos/popup_info_eventos&acao=A&tipo='+tipo+'&coeid='+coeid;
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=900,height=450');
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

    function salvarDadosContrato(){
        var erro;
        var campos = '';
        var formulario = document.formulario;

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
            $('#formulario_cadastro').find('#requisicao').val('salvarDadosContrato');
            $('#formulario_cadastro').submit();
        }
    }

    function verificaDatafinal(){
        var objData = new Data();

        var dataInicio = $('#coeiniciovig').val();
        var dataFim = $('#coefimvig').val();
        var result;

        result = objData.comparaData( dataInicio, dataFim, '>' );

        if( result == true ){
            alert('A data de "Início da Vigência é maior que a data "Fim da Vigência"');
            $('#coeiniciovig').val('');
            $('#coefimvig').val('');
            $('#coeiniciovig').focus();
            return false;
        }
    }

    function verificarCNPJValido( cnpj ){
        var valido = validarCnpj( cnpj );

        if(!valido){
            alert( "CNPJ inválido! Favor informar um CNPJ válido!" );
            $('#hspcnpj').focus();
            return false;
        }
    }
    
    function pesquisarContrato(){
        $('#formulario_pesquisa').submit();
    }

</script>

<div id="form_tela_cadastro_contrato" style="display: none;"></div>

<form action="" method="POST" id="formulario_pesquisa" name="formulario_pesquisa">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="coeid" id="coeid" value="">

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class ="SubTituloDireita" width="35%"> Número do Contrato: </td>
            <td>
                <?PHP
                    $coenumcontrato = $dadosContrato['coenumcontrato'];
                    echo campo_texto('coenumcontrato', 'N', 'S', 'Número do Contrato', 24, 20, '', '', 'left', '', 0, 'id="coenumcontrato"', '', $coenumcontrato, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Número do Processo: </td>
            <td>
                <?PHP
                    echo campo_texto('coenumprocesso', 'N', 'S', 'Número do Processo', 24, 20, '#####.######/####-##', '', 'right', '', 0, 'id="coenumprocesso"', '', $coenumprocesso, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> CNPJ: </td>
            <td>
                <?PHP
                    echo campo_texto('coecnpj', 'N', 'S', 'CNPJ', 24, 20, '##.###.###/####-##', '', 'left', '', 0, 'id="coecnpj"', '', $coecnpj, 'verificarCNPJValido(this.value);', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Razão Social: </td>
            <td>
                <?PHP
                    echo campo_texto('coerazaosocial', 'N', 'S', 'Razão Social', 70, 100, '', '', 'left', '', 0, 'id="coerazaosocial"', '', $coerazaosocial, '', null);
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
       <tr>
            <td class="SubTituloCentro">
                <input type="button" id="Pesquisar" name="pesquisar" value="pesquisar" onclick="pesquisarContrato();"/>
                <input type="button" id="todos" name="todos" value="Ver Todos" onclick="resetFromulario(); pesquisarContrato();"/>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class="SubTituloEsquesrdo">
                <input type="button" id="novo" name="novo" value="Novo Contrato" onclick="abrirTelaCadastroContrato('');"/>
            </td>
        </tr>
    </table>

</form>

<?PHP

    if( $_REQUEST['coenumcontrato'] != '' ){
        $WHERE .= " AND c.coenumcontrato ilike ('%{$_REQUEST['coenumcontrato']}%')";
    }
    
    if( $_REQUEST['coenumprocesso'] != '' ){
        $WHERE .= " AND c.coenumprocesso ilike ('%{$_REQUEST['coenumprocesso']}%')";
    }
    
    if( $_REQUEST['coecnpj'] != '' ){
        $WHERE .= " AND c.coecnpj ilike ('%{$_REQUEST['coecnpj']}%')";
    }
        
    if( $_REQUEST['coerazaosocial'] != '' ){
        $WHERE .= " AND c.coerazaosocial ilike ('%{$_REQUEST['coerazaosocial']}%')";
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirTelaCadastroContrato('||coeid||');\" title=\"Editar Contrato\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirContrato('||coeid||');\" title=\"Excluir Contrato\" >
        <img src=\"/imagens/icon_campus_2.png\" style=\"cursor: pointer\" onclick=\"infomeEvento('||c.coeid||',\'cont\');\" border=0 alt=\"Ir\" title=\"Visualizar Eventos desses Contrato\">
    ";

    $sql = "
        SELECT  '{$acao}',
                precodpregao ||' - '|| prenumprocesso AS predescpregao,
                coenumcontrato,
                to_char(coeiniciovig, 'DD/MM/YYYY') AS coeiniciovig,
                to_char(coefimvig, 'DD/MM/YYYY') AS coefimvig,
                --to_char(coevalorcontratado, 'L999G999G999G990D99') AS coevalorcontratado,
                coevalorcontratado,
                --to_char(coevalorempenhado, 'L999G999G999G990D99') AS coevalorempenhado,
                coevalorempenhado,
                --to_char(coesaldoinicontrato, 'L999G999G999G990D99') AS coesaldoinicontrato,
                coesaldoinicontrato,
                --to_char(coevalorutilizado, 'L999G999G999G990D99') AS coevalorutilizado,
                coevalorutilizado,
                --to_char(coesaldoiniexercicio, 'L999G999G999G990D99') AS coesaldoiniexercicio,
                coesaldoiniexercicio,
                --to_char(coesaldofimcontrato, 'L999G999G999G990D99') AS coesaldofimcontrato,
                coesaldofimcontrato,
                coerazaosocial
        FROM evento.contratopregao AS c
        JOIN evento.pregaoevento AS p ON p.preid = c.preid
        WHERE coestatus = 'A' {$WHERE}
        ORDER BY coeid
    ";
    $cabecalho = array("Ação", "Pregão", "Número do Contrato", "Início da Vigência", "Fim da Vigência", "Valor Contratado", "Valor Empenhado", "Saldo Inicial", "Valor Utilizado", "Saldo Exercicio", "Saldo Fim de Contrado","Razão Social");
    $alinhamento = Array('center', '', '', 'center', 'center','right','right','right','right','right','right', 'left');
    $tamanho = Array('4%', '13%', '13%', '5%', '5%', '7%', '7%', '7%', '7%', '7%', '7%', '25%' );
    $db->monta_lista($sql, $cabecalho, 50, 10, 'S', 'center', 'S', '', $tamanho, $alinhamento);

?>
