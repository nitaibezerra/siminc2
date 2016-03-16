<?PHP
    $_SESSION['evento']['aba'] = 'LISTA_EMPENHO';

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Administrativo - Eventos', 'Cadastrar Empenho Unnidade' );

    $abacod_tela    = ABA_CADASTRO_PREGAO_CONTRAT_UNI;
    $url            = 'evento.php?modulo=principal/eventos/lista_grid_eve_und_empenho&acao=A';
    $parametros     = '';

    if( $_SESSION['evento']['aba'] == 'LISTA_EMPENHO' ){
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

    function cadastrarEmpenho( id ){
        window.location.href ='evento.php?modulo=principal/eventos/cad_eve_empenho&acao=A&ureid='+id;
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" name="requisicao" id="requisicao" value="">

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class ="SubTituloDireita"> Unidade Gestora:</td>
            <td>
                <?PHP
                    echo campo_texto('nomeuidade', 'N', 'S', '', 32, 200, '', '', 'left', '', 0, 'id="nomeuidade"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Código da unidade Gestora:</td>
            <td>
                <?PHP
                    echo campo_texto('codunidade', 'N', 'S', '', 32, 200, '', '', 'left', '', 0, 'id="codunidade"');
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="submit" class="pesquisar" name="pesquisar" id="pesquisar" value="Pesquisar">
                <input type="submit" class="todos" name="todos" id="todos" value="Ver todos">
            </td>
        </tr>
    </table>
</form>

<?PHP

    $where = '';

    if($_REQUEST['nomeuidade']){
        $where .= "and ug.ungdsc ilike ('%".$_REQUEST['nomeuidade']."%')";
    }

    if($_REQUEST['codunidade']){
        $where .= "and ug.ungcod = '".$_REQUEST['codunidade']."'";
    }

    $acao = "
        <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"cadastrarEmpenho('||ur.ureid||')\" \" title=\"Inserir Empenho\">
    ";

    $sql = "
        SELECT '{$acao}' AS acao,
                CAST(ug.ungcod AS text) AS ungcod,
                c.coenumcontrato ||' - '|| coerazaosocial AS descricao, 
                ug.ungdsc,
                ureordenador,
                ureordenadorsub
        FROM evento.unidaderecurso AS ur

        JOIN evento.contratopregao AS c ON c.coeid = ur.coeid
        JOIN public.unidadegestora AS ug ON ug.ungcod = ur.ungcod
        WHERE 1 = 1 ".$where."
        ORDER BY ug.ungdsc, descricao
    ";
    //ver($sql, d);
    $cabecalho = array("Ações", "Cód. Unidade","Contrato", "Unidades", "Ordenador de Despesa", "Ordenador de Despesa Substituto");
    $whidth = Array('5%', '7%', '20%', '25%', '15%', '15%');
    $align  = Array('left', 'left', 'left', 'left', 'left');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');

?>