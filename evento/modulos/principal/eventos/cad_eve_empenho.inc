<?PHP
    $_SESSION['evento']['aba'] = 'CAD_EMPENHO';

    if( $_REQUEST['ureid'] != '' ){
        $sql = "
            SELECT  ureid,
                    ur.coeid,
                    ur.preid,
                    ug.ungcod,
                    ug.ungdsc
            FROM evento.unidaderecurso AS ur
            JOIN public.unidadegestora AS ug ON ug.ungcod = ur.ungcod
            WHERE ureid = {$_REQUEST['ureid']}
            ORDER BY 1
        ";
        $unidade = $db->pegaLinha($sql);
    }else{
        $db->insucesso('Não foi possível acessar o Cadastramento do Empenho, tente novamente, selecione uma Unidade Gestora!', '', 'principal/eventos/lista_grid_eve_und_empenho&acao=A');
    }

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Administrativo - Eventos', 'Cadastrar Contrato' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/lista_grid_eve_und_empenho&acao=A';
    $parametros     = '';

    if( $_SESSION['evento']['aba'] == 'CAD_EMPENHO' ){
        $db->cria_aba($abacod_tela, $url, $parametros);
    }
    $perfil = pegaPerfilGeral();
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

    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11(window).unload(
            $1_11('.chosen-select').chosen()
        );
    });

    function editarEmpenho( id ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=editarEmpenho&emuid="+id,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $.each(dados, function(index, value) {
                   $('#'+index).val(value);
                });

                var selectValue = $('#preid').find('option[selected]').html();
                $('#td_combo_preid').find('a > span').html(selectValue);
                
                var selectValue = $('#coeid').find('option[selected]').html();
                $('#td_combo_coeid').find('a > span').html(selectValue);
            }
        });
    }

    function exclirDadosEmpenho( id ){
        var confirma = confirm("Deseja realmente EXCLUIR o registro?");

        if( confirma ){
            $('#emuid').val(id);
            $('#requisicao').val('exclirDadosEmpenho');
            $('#formulario').submit();
        }
    }
    
    function infomeEvento(emuid, tipo){
        var url = 'evento.php?modulo=principal/eventos/popup_info_eventos&acao=A&tipo='+tipo+'&emuid='+emuid;
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

    function salvarDadosEmpenho(){
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
            $('#requisicao').val('salvarDadosEmpenho');
            $('#formulario').submit();
        }
    }

    function voltarPagina(){
        var janela = window.location.href = 'evento.php?modulo=principal/eventos/lista_grid_eve_und_empenho&acao=A';
        janela.focus();
    }
    
    function verificaSaldoUnidade( valor ){
        var ureid = $('#ureid').val();
        if( ureid != '' ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=verificaSaldoUnidade&ureid="+ureid+"&valor="+valor,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    if( dados.result == 'SS' ){
                        alert(dados.msg);
                    }
                }
            });
        }
    }

</script>


<form name ="formulario" id="formulario" method="POST">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="emuid" id="emuid" value="">

    <input type="hidden" name="ureid" id="ureid" value="<?=$unidade['ureid']?>">
    <input type="hidden" name="ungcod" id="ungcod" value="<?=$unidade['ungcod']?>">

    <table class="tabela listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
        <tr>
            <td class ="SubTituloDireita" width="30%"> Unidade Gestora:</td>
            <td>
                <?PHP
                    echo campo_texto('ungdsc', 'S', 'N', '', 77, 200, '', '', 'left', '', 0, 'id="ungdsc"', '', $unidade['ungdsc']);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Pregão: </td>
            <td id="td_combo_preid">
                <?PHP
                    $preid = $unidade['preid'];
                    $sql = "
                        SELECT  preid AS codigo,
                                precodpregao ||' - '|| prenumprocesso AS descricao
                        FROM evento.pregaoevento
                        ORDER BY descricao
                    ";
                    $db->monta_combo('preid', $sql, 'N', "Selecione...", '', '', '', 500, 'S', 'preid', '', $preid, 'Pregão', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Contrato: </td>
            <td id="td_combo_coeid">
                <?PHP
                    $coeid = $unidade['coeid'];
                    $sql = "
                        SELECT  coeid AS codigo,
                                coenumcontrato ||' - '|| coecnpj ||' - '|| coerazaosocial AS descricao
                        FROM evento.contratopregao
                        ORDER BY descricao
                    ";
                    $db->monta_combo('coeid', $sql, 'N', "Selecione...", '', '', '', 500, 'S', 'coeid', '', $coeid, 'Contrato', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Descrição do Empenho:</td>
            <td>
                <?PHP echo campo_textarea('empdescricao', 'S', 'S', '', 80, 4, 200, '', 0, '', 'Descrição do Empenho', null, $valor, '46%'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Número do Empenho:</td>
            <td>
                <?PHP echo campo_texto('empnumero', 'S', 'S', '', 35, 15, '', '', 'left', '', 0, 'id="empnumero"'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Número do PI:</td>
            <td>
                <?PHP echo campo_texto('empnumeropi', 'S', 'S', '', 35, 15, '', '', 'left', '', 0, 'id="empnumeropi"'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Ano do PI:</td>
            <td>
                <?PHP echo campo_texto('empano', 'S', 'S', '', 35, 4, '####', '', 'left', '', 0, 'id="empano"'); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Valor Empenhado: </td>
            <td>
                <?PHP
                    $empsaldoinicontrato = $dadosRecurso['empsaldoinicontrato'];
                    echo campo_texto('empsaldoinicontrato', 'S', 'S', 'Saldo Início Contrato', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="empsaldoinicontrato"', '', $empsaldoinicontrato, 'verificaSaldoUnidade(this.value);', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Valor Utilizado: </td>
            <td>
                <?PHP
                    $empvalorutilizado = $dadosRecurso['empvalorutilizado'];
                    echo campo_texto('empvalorutilizado', 'N', 'N', 'Valor utilizado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="empvalorutilizado"', '', $empvalorutilizado, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Status:</td>
            <td>
                <input type="radio" value="A" name="empstatus" id="empstatusA" /> Ativo
                &nbsp;&nbsp;
                <input type="radio" value="I" name="empstatus" id="empstatusI" /> Inativo
            </td>
        </tr>
    </table>
    <br>
    <table class="tabela" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
        <tr>
            <td class="SubTituloCentro">
                <input type="button" name="salvar" id="salvar" value="Salvar" onclick="salvarDadosEmpenho();">
                <input type="button" name="voltar" id="voltar" value="Voltar" onclick="voltarPagina();">
            </td>
        </tr>
    </table>
</form>

<?PHP
    $acao = "
        <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarEmpenho('||ep.emuid||')\" \" border=0 alt=\"Ir\" title=\"Alterar\">
        <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"exclirDadosEmpenho('||ep.emuid||');\" border=0 alt=\"Ir\" title=\"Excluir\">
        <img src=\"/imagens/icon_campus_2.png\" style=\"cursor: pointer\" onclick=\"infomeEvento('||ep.emuid||',\'emp\');\" border=0 alt=\"Ir\" title=\"Visualizar Eventos desses Empenho\">
    ";

    $sql = "
        SELECT  '$acao' as acoes,
                empnumero,
                empdescricao,
                empnumeropi,
                empano || ' ' AS empano,
                empsaldoinicontrato,
                empvalorutilizado,
                ( empsaldoinicontrato - empvalorutilizado ) AS disponivel,
                CASE WHEN empstatus = 'A'
                    THEN 'Ativo'
                    ELSE 'Inativo'
                END as empstatus
        FROM evento.empenho_unidade ep

        JOIN evento.unidaderecurso ug ON ug.ureid = ep.ureid

        WHERE ep.ureid = '{$unidade['ureid']}'

        ORDER BY ep.emuid
    ";
    $cabecalho = array("Ações", "Número do Empenho", "Descrição", "Número do Empenho PI", "Ano do Empenho", "Saldo Inicial Contrato", "Valor Utilizado", "Saldo Disponivel", "Status");
    $alinhamento = Array('center','', '', '', 'right', 'right','right','right', 'center');
    $tamanho = Array('5%', '10%', '30%', '10%', '10%', '10%', '10%', '10%', '5%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'S', 'center', 'S', '', $tamanho, $alinhamento);
?>