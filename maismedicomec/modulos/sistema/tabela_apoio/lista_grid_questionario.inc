<?PHP

    #MATA AS SESSÕES DE INDENTIFICAÇÃO DO MUNICÍPÍO.
    unset( $_SESSION['maismedicomec']['muncod'], $_SESSION['maismedicomec']['etqid'] );
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";

    monta_titulo('Mais Médico - MEC', 'Instrumento de Avaliação - Listagem de municípios');

    #ATRIBUI O VALOR NAS VARIAVEIS DE SESSAO
    /*
    if( $_REQUEST['qstid'] != '' ){
        iniciaVariaveisSessao( $_REQUEST['qstid'], 'Q' );
    }else{
        if( $_SESSION['maismedicomec']['qstid'] == '' ){ 
            $db->sucesso('principal/inicio_direcionamento', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }
*/
    
    $abacod_tela    = ABA_GERAR_QUESTIONARIO_PERGUNTA;
    $url            = 'maismedicomec.php?modulo=sistema/tabela_apoio/lista_grid_questionario&acao=A';
    $parametros     = '';

    //$db->cria_aba($abacod_tela, $url, $parametros);
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function cadastraPerguntasQuest(qstid){
        window.location.href ='maismedicomec.php?modulo=sistema/tabela_apoio/cad_monta_perguntas&acao=A&qstid='+qstid;
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
            <td class ="SubTituloDireita" width="35%">Nome do Questionário:</td>
            <td>
                <?PHP
                    echo campo_texto('qstdsc', 'N', 'S', '', 33, 20, '', '', '', '', 0, 'id="qstdsc"', '', $qstdsc, null, '', 'Nome do Questionário'); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Status do Quetionário:</td>
            <td id="td_combo_municipio">
                <?PHP
                    $sql = array(
                        array(codigo => "A", descricao => "Ativo"),
                        array(codigo => "I", descricao => "Inativo")
                    );
                    $db->monta_combo("qststatus", $sql, 'S', 'Selecione...', '', '', '', '333', 'S', 'qststatus', false, $qststatus, 'Status do Quetionário');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">Nome do Sistema:</td>
            <td>
                <?PHP
                    $sisdsc = '';
                    echo campo_texto('sisdsc', 'N', 'S', '', 33, 20, '', '', '', '', 0, 'id="sisdsc"', '', $sisdsc, null, '', 'Nome do Sistema'); 
                ?>
            </td>
        </tr>
    </table>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarQuestionario('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarQuestionario('ver');"/>
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP

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
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"cadastraPerguntasQuest('||q.qstid||');\" title=\"Editar Servidor\" >
    ";

    $sql = "
        SELECT  '{$acao}',
                qstid, 
                qstdsc, 
                qstdtinclusao, 
                qststatus, 
                sisdsc
        FROM maismedicomec.questionario AS q
        JOIN seguranca.sistema AS s ON s.sisid = q.sisid
        
        ORDER BY q.qstid, q.qstdsc
    ";
    $cabecalho = array("Ação", "ID Quest", "Nome do Questionário", "Inclusão", "Status", "Sistema");
    $alinhamento = Array('center', '', '', '');
    $tamanho = Array('5%', '10%', '', '');
    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', 'N', $tamanho, $alinhamento);

?>