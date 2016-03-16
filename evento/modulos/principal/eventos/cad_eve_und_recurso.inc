<?PHP
    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Administrativo - Eventos', 'Cadastrar Unidade de Recurso' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/cad_eve_und_recurso&acao=A';
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
    function abrirTelaUndRecurso( $dados ) {
        global $db;
        if( $dados['ureid'] != '' ){
            $dadosRecurso = editarUndRecurso( $dados['ureid'] );
        }
?>
        <form action="" method="POST" id="formulario_cadastro" name="formulario_cadastro">
            <input type="hidden" name="requisicao" id="requisicao" value="">
            <input type="hidden" name="ureid" id="ureid" value="<?=$dados['ureid'];?>">

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloCentro" style="font-size:15px;"> Cadastro da Unidade de Recurso </td>
                </tr>
            </table>
            
            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class ="SubTituloDireita" align="right"> Pregão: </td>
                    <td id="td_combo_preid">
                        <?PHP
                            $preid = $dadosRecurso['preid'];
                            $sql = "
                                SELECT  preid AS codigo,
                                        precodpregao ||' - '|| prenumprocesso AS descricao
                                FROM evento.pregaoevento
                                ORDER BY descricao
                            ";
                            $db->monta_combo('preid', $sql, 'S', "Selecione...", '', '', '', 500, 'S', 'preid', '', $preid, 'Pregão', '', 'chosen-select');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Contrato: </td>
                    <td id="td_combo_coeid">
                        <?PHP
                            $coeid = $dadosRecurso['coeid'];
                            $sql = "
                                SELECT  coeid AS codigo,
                                        coenumcontrato ||' - '|| coecnpj ||' - '|| coerazaosocial AS descricao
                                FROM evento.contratopregao
                                ORDER BY descricao
                            ";
                            $db->monta_combo('coeid', $sql, 'S', "Selecione...", '', '', '', 500, 'S', 'coeid', '', $coeid, 'Contrato', '', 'chosen-select');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Unidade Gestora: </td>
                    <td id="td_combo_ungcod">
                        <?PHP
                            $ungcod = $dadosRecurso['ungcod'];
                            $sql = "
                                SELECT  ungcod AS codigo,
                                        upper(ungabrev) ||' - '|| initcap(ungdsc) AS descricao
                                FROM public.unidadegestora
                                ORDER BY descricao
                            ";
                            $db->monta_combo('ungcod', $sql, 'S', "Selecione...", '', '', '', 500, 'S', 'ungcod', '', $ungcod, 'Unidade Gestora', '', 'chosen-select');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Ordernador: </td>
                    <td>
                        <?PHP
                            $ureordenador = $dadosRecurso['ureordenador'];
                            echo campo_texto('ureordenador', 'S', $habilita, 'Ordernador', 64, 200, '', '', 'left', '', 0, 'id="ureordenador"', '', $ureordenador, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Ordenador Substituto: </td>
                    <td>
                        <?PHP
                            $ureordenadorsub = $dadosRecurso['ureordenadorsub'];
                            echo campo_texto('ureordenadorsub', 'S', $habilita, 'Ordernador Substituto', 64, 200, '', '', 'left', '', 0, 'id="ureordenadorsub"', '', $ureordenadorsub, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Orçamento da Unidade: </td>
                    <td>
                        <?PHP
                            $uresaldoinicontrato = $dadosRecurso['uresaldoinicontrato'];
                            echo campo_texto('uresaldoinicontrato', 'S', $habilita, 'Saldo do Início do Contratado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="uresaldoinicontrato"', '', $uresaldoinicontrato, 'verificaSaldoContrato(this.value);', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Valor Utilizado: </td>
                    <td>
                        <?PHP
                            $urevalorutilizado = $dadosRecurso['urevalorutilizado'];
                            echo campo_texto('urevalorutilizado', 'N', 'N', 'Valor Utilizado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="urevalorutilizado"', '', $urevalorutilizado, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Saldo do início do Exercício: </td>
                    <td>
                        <?PHP
                            $uresaldoiniexercicio = $dadosRecurso['uresaldoiniexercicio'];
                            echo campo_texto('uresaldoiniexercicio', 'N', 'N', 'Salado do início do Exercício', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="uresaldoiniexercicio"', '', $uresaldoiniexercicio, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Saldo Fim Contrato: </td>
                    <td>
                        <?PHP
                            $uresaldofimcontrato = $dadosRecurso['uresaldofimcontrato'];
                            echo campo_texto('coevalorutilizado', 'N', 'N', 'Valor utilizado', 24, 18, '###.###.###.###,##', '', 'right', '', 0, 'id="coevalorutilizado"', '', $coevalorutilizado, '', null);
                        ?>
                    </td>
                </tr>    
            </table>

            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
                <tr>
                    <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                        <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosUndRecurso();"/>
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
        height: 480px !important;
        width: 60%  !important;
        color: #bbb  !important;
        /*background-color: #333;*/
        background-color: #FFFFFF  !important;
        border: 4px solid #444  !important;
        padding: 5px  !important;
    }
</style>

<script type="text/javascript">
    
    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11('.chosen-select').chosen();
        
        $1_11('#preid').change(function(){
            atualizaCombosContrato( $1_11(this).val() );
        });
    });
    
    function abrirTelaUndRecurso( ureid ){
        var url;
        if( ureid == '' ){
            url = "requisicao=abrirTelaUndRecurso";
        }else{
            url = "requisicao=abrirTelaUndRecurso&ureid="+ureid;
        }

        $.ajax({
            type: "POST",
            url: window.location.href,
            data: url,
            success: function (resp){
                $('#form_tela_cadastro_und_recurso').html(resp);
                $('#form_tela_cadastro_und_recurso').modal();
                divCarregado();
            }
        });
    }
    
    function atualizaCombosContrato( id ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=atualizaCombosContrato&preid="+id,
            success: function(resp){                
                $('#td_combo_coeid').html(resp);                
                $1_11('.chosen-select').chosen();
            }
        });
    }
    
    function editarUndRecurso( id ){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=editarUndRecurso&ureid="+id,
            success: function(resp){
                var dados = $.parseJSON(resp);
                
                $.each(dados, function(index, value) {
                   $('#'+index).val(value);
                });
                
                var selectValue = $('#preid').find('option[selected]').html();
                $('#td_combo_preid').find('a > span').html(selectValue);
                
                var selectValue = $('#coeid').find('option[selected]').html();
                $('#td_combo_coeid').find('a > span').html(selectValue);
                
                var selectValue = $('#ungcod').find('option[selected]').html();
                $('#td_combo_ungcod').find('a > span').html(selectValue);
            }
        });
    }
    
    function excluirUndRecurso( id ){
        var confirma = confirm("Deseja realmente EXCLUIR o registro?");
        
        if( confirma ){
            $('#formulario_pesquisa').find('#ureid').val(id);
            $('#formulario_pesquisa').find('#requisicao').val('excluirUndRecurso');
            $('#formulario_pesquisa').submit();
        }
    }
    
    function infomeEvento(ureid, tipo){
        var url = 'evento.php?modulo=principal/eventos/popup_info_eventos&acao=A&tipo='+tipo+'&ureid='+ureid;
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

    function salvarDadosUndRecurso(){
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
            $('#formulario_cadastro').find('#requisicao').val('salvarDadosUndRecurso');
            $('#formulario_cadastro').submit();
        }
    }

    function verificaSaldoContrato( valor ){
        var coeid = $('#coeid').val();
        if( coeid != '' ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=verificaSaldoContrato&coeid="+coeid+"&valor="+valor,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    if( dados.result == 'SS' ){
                        alert(dados.msg);
                    }
                }
            });
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A';
        janela.focus();
    }
    
    function pesquisarUndRecurso(){
        $('#formulario_pesquisa').submit();
    }

</script>

<div id="form_tela_cadastro_und_recurso" style="display: none;"></div>

<form action="" method="POST" id="formulario_pesquisa" name="formulario_pesquisa">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="ureid" id="ureid" value="">

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class ="SubTituloDireita" width="30%"> Contrato: </td>
            <td id="td_combo_coeid">
                <?PHP
                    $coeid = $dadosRecurso['coeid'];
                    $sql = "
                        SELECT  coeid AS codigo,
                                coenumcontrato ||' - '|| coecnpj ||' - '|| coerazaosocial AS descricao
                        FROM evento.contratopregao
                        ORDER BY descricao
                    ";
                    $db->monta_combo('coeid', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'coeid', '', $preid, 'Contrato', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Unidade Gestora: </td>
            <td id="td_combo_ungcod">
                <?PHP
                    $coeid = $dadosRecurso['coeid'];
                    $sql = "
                        SELECT  ungcod AS codigo,
                                upper(ungabrev) ||' - '|| initcap(ungdsc) AS descricao
                        FROM public.unidadegestora
                        ORDER BY descricao
                    ";
                    $db->monta_combo('ungcod', $sql, 'S', "Selecione...", '', '', '', 500, 'N', 'ungcod', '', $preid, 'Unidade Gestora', '', 'chosen-select');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Ordernador: </td>
            <td>
                <?PHP
                    $ureordenador = $dadosRecurso['ureordenador'];
                    echo campo_texto('ureordenador', 'N', 'S', 'Ordernador', 64, 200, '', '', 'left', '', 0, 'id="ureordenador"', '', $ureordenador, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> Ordenador Substituto: </td>
            <td>
                <?PHP
                    $ureordenadorsub = $dadosRecurso['ureordenadorsub'];
                    echo campo_texto('ureordenadorsub', 'N', 'S', 'Ordernador Substituto', 64, 200, '', '', 'left', '', 0, 'id="ureordenadorsub"', '', $ureordenadorsub, '', null);
                ?>
            </td>
        </tr>            
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
       <tr>
            <td class="SubTituloCentro">
                <input type="button" id="Pesquisar" name="pesquisar" value="pesquisar" onclick="pesquisarUndRecurso();"/>
                <input type="button" id="todos" name="todos" value="Ver Todos" onclick="resetFromulario(); pesquisarUndRecurso();"/>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td class="SubTituloEsquesrdo">
                <input type="button" id="novo" name="novo" value="Nova Und. Recurso" onclick="abrirTelaUndRecurso('');"/>
            </td>
        </tr>
    </table>
</form>


<?PHP
/*
 [coeid] => 4
    [ungcod] => 110010
    [ureordenador] => asdfasdf
    [ureordenadorsub] => teste - 2
*/

    if( $_REQUEST['coeid'] != '' ){
        $WHERE .= " AND c.coeid = {$_REQUEST['coeid']}";
    }
    
    if( $_REQUEST['ungcod'] != '' ){
        $WHERE .= " AND ug.ungcod = '{$_REQUEST['ungcod']}'";
    }
    
    if( $_REQUEST['ureordenador'] != '' ){
        $WHERE .= " AND u.ureordenador ilike ('%{$_REQUEST['ureordenador']}%')";
    }
    
    if( $_REQUEST['ureordenadorsub'] != '' ){
        $WHERE .= " AND u.ureordenadorsub ilike ('%{$_REQUEST['ureordenadorsub']}%')";
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirTelaUndRecurso('||ureid||');\" title=\"Editar Unidade de Recurso\" >
        <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirUndRecurso('||ureid||');\" title=\"Excluir Unidade de Recurso\" >
        <img src=\"/imagens/icon_campus_2.png\" style=\"cursor: pointer\" onclick=\"infomeEvento('||u.ureid||',\'und\');\" border=0 alt=\"Ir\" title=\"Visualizar Eventos dessa Unidade de Recurso\">
    ";
    
    $sql = "
        SELECT  '{$acao}',
                precodpregao ||' - '|| prenumprocesso AS predescpregao, 
                c.coenumcontrato, 
                ug.ungdsc, 
                ureordenador,
                ureordenadorsub, 
                --to_char(uresaldoinicontrato, 'L999G999G999G990D99') AS uresaldoinicontrato, 
                uresaldoinicontrato,
                --to_char(urevalorutilizado, 'L999G999G999G990D99') AS urevalorutilizado, 
                urevalorutilizado, 
                --to_char(uresaldoiniexercicio, 'L999G999G999G990D99') AS uresaldoiniexercicio, 
                uresaldoiniexercicio, 
                --to_char(uresaldofimcontrato, 'L999G999G999G990D99') AS uresaldofimcontrato
                uresaldofimcontrato
        FROM evento.unidaderecurso AS u
        
        JOIN evento.pregaoevento AS p ON p.preid = u.preid
        JOIN evento.contratopregao AS c ON c.coeid = u.coeid
        JOIN public.unidadegestora AS ug ON ug.ungcod = u.ungcod
        
        WHERE 1=1 ${WHERE}
    ";
    $cabecalho = array("Ação", "Pregão", "Contrato", "Und. Gestora", "Ordenador", "Ordernador Substituto", "Saldo Inícial do Contrato", "Valor Utilizado",  "Saldo Exercicio", "Saldo Fim Contrato");
    $alinhamento = Array('center', '', '', '', '', '','right','right','right','right','right');
    $tamanho = Array('4%', '12%', '7%', '15%', '10%', '10%', '10%', '7%', '7%', '7%', '7%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'S', 'center', 'S', '', $tamanho, $alinhamento);

?>
