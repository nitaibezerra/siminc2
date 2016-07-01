<?PHP

    #MATA AS SESSÕES DE INDENTIFICAÇÃO DO MUNICÍPÍO.
    unset( $_SESSION['maismedicomec'] );

    #CRIA VARIAVEL DE SESSÃO PARA IDENTIFICAR SE O ACESSO A ESSE ARQUIVO SE PELO RELATÓRIO OU PELO QUESTIONÁRIO (ABAS).
    $_SESSION['maismedicomec']['relatorio'] = 'S';

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";

    monta_titulo('Mais Médico - MEC', 'Instrumento de Avaliação - Listagem de municípios com Questionário Respondido');

    #ATRIBUI O VALOR NAS VARIAVEIS DE SESSAO
    iniciaVariaveisSessao( QUEST_INSTRUMENTO_AVAL_CURSO_MEDICINA, 'Q' );
    /*
    if( $_REQUEST['qstid'] != '' ){
        iniciaVariaveisSessao( $_REQUEST['qstid'], 'Q' );
    }else{
        if( $_SESSION['maismedicomec']['qstid'] == '' ){
            $db->sucesso('principal/inicio_direcionamento', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }
    */
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function atualizaComboMunicipio( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipio&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function abrirQuestionario( parm ){
        //http://simec-local/maismedicomec/maismedicomec.php?modulo=relatorio/relatorio_lista_grid_municipio&acao=A
        window.location.href ='maismedicomec.php?modulo=relatorio/relatorio_resposta_questionario&acao=A&muncod='+parm;
    }

    function exibirHistorico( docid ){
        var url = 'http://<?PHP echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid='+docid;
        window.open( url, 'alterarEstado', 'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no' );
    }

    function pesquisarServidor(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">

<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="aba" name="aba" value="<?= $_POST['aba']; ?>"/>

    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita">Unidade Federativa (UF):</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  estuf AS codigo,
                                estuf ||' - '|| estdescricao AS descricao
                        FROM territorios.estado
                        ORDER BY estuf
                    ";
                    $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaComboMunicipio', '', '', 450, 'N', 'estuf', false, $estuf, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Município:</td>
            <td id="td_combo_municipio">
                <?PHP
                    $sql = "
                        SELECT  m.muncod AS codigo,
                                m.mundescricao AS descricao
                        FROM maismedicomec.municipioliberado AS l
                        LEFT JOIN territorios.municipio AS m ON substr(m.muncod, 1, 6) = l.muncod
                        LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
                        ORDER BY u.estuf, m.mundescricao
                    ";
                    $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 450, 'N', 'muncod', false, $muncod, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">Município - Descrição:</td>
            <td>
                <?= campo_texto('mundescricao', 'N', 'S', '', 69, 20, '', '', '', '', 0, 'id="mundescricao"', '', $mundescricao, null, '', null); ?>
            </td>
        </tr>
    </table>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP

    $perfil = pegaPerfilGeral();

    if( !( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ) ){
        $JOIN = "LEFT JOIN maismedicomec.usuarioresponsabilidade AS ur ON substr(ur.muncod, 1, 6) = l.muncod";
        $where[] = " ur.usucpf = '{$_SESSION['usucpf']}' ";
    }

    if ($_REQUEST['estuf']) {
        $estuf = $_REQUEST['estuf'];
        $where[] = " u.estuf = '{$estuf}'";
    }
    if ($_REQUEST['muncod']) {
        $muncod = $_REQUEST['muncod'];
        $where[] = " m.muncod = '{$muncod}'";
    }
    if ($_REQUEST['mundescricao']) {
        $mundescricao = $_REQUEST['mundescricao'];
        $where[] = " public.removeacento(m.mundescricao) ILIKE public.removeacento( ('%{$mundescricao}%') ) ";
    }

    $WHERE = "WHERE l.qstid = {$_SESSION['maismedicomec']['qstid']} ";

    if($where != ""){
        $WHERE .= "AND " . implode(' AND ', $where);
    }

    $acao = "
        <center>
            <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirQuestionario('||m.muncod||');\" title=\"Editar Servidor\" >
        </center>
    ";

    $sql = "
        SELECT  DISTINCT '{$acao}',
                m.muncod,
                m.mundescricao,
                u.estuf

        FROM maismedicomec.municipioliberado AS l

        {$JOIN}

        LEFT JOIN territorios.municipio AS m ON substr(m.muncod, 1, 6) = l.muncod
        LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf

        JOIN maismedicomec.entidadequestionario et ON et.muncod = m.muncod AND et.docid IS NOT NULL

        {$WHERE}

        ORDER BY u.estuf, m.mundescricao
    ";
    $cabecalho = array("Ação", "Código IBGE", "Município", "Estado");
    $alinhamento = Array('center', '', '', '');
    $tamanho = Array('5%', '10%', '', '');
    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', 'N', $tamanho, $alinhamento);

?>