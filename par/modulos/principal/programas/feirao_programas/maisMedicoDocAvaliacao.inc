<?PHP
    include_once APPRAIZ."www/maismedicomec/_funcoes.php";
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    unset($_SESSION['maismedicomec']);
    #ATRIBUI O VALOR NAS VARIAVEIS DE SESSAO
    $_SESSION['maismedicomec']['qstid'] = 1;

    if( $_SESSION['par']['muncod'] != '' ){
        iniciaVariaveisSessao( $_SESSION['par']['muncod'], 'M' );

        if( $_SESSION['maismedicomec']['muncod'] != '' ){
            $etqid = cadastraEntidadeQuestionarioMunicipio( $_SESSION['maismedicomec']['muncod'] );

            if( $etqid != '' ){
                iniciaVariaveisSessao( $etqid, 'E' );
            }
        }

    }else{
        if( $_SESSION['maismedicomec']['muncod'] == '' ){
            $db->sucesso('principal/programas/feirao_programas/maisMedicosCondicoesPart', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicoDocAvaliacao&acao=A');
    monta_titulo($titulo_modulo, recuperaMunicipioEstado());

    $_SESSION['maismedicos'] = true;    
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }

</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="arqid" name="arqid" value=""/>
</form>

<?PHP
    $qstid = $_SESSION['maismedicomec']['qstid'];
    $etqid = $_SESSION['maismedicomec']['etqid'];

    $acao = "
        <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/anexo.gif\" />
    ";

    $down = "<a title=\"Baixar arquivo\" href=\"#\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\">' || anx.avadsc || '</a>";

    $sql = "
        SELECT  '{$acao}' as acao,
                '{$down}' as descricao,
                tpadsc,
                arq.arqnome||'.'||arq.arqextensao,
                to_char(avadtinclusao, 'DD/MM/YYYY') as avadtinclusao
        FROM maismedicomec.arquivoavaliacao anx
        JOIN maismedicomec.tipoarquivo td on td.tpaid  = anx.tpaid
        JOIN public.arquivo arq on arq.arqid = anx.arqid
        WHERE avastatus = 'A' AND qstid = {$qstid} AND etqid = {$etqid}
    ";

    $cabecalho = Array("Ação", "Descrição", "Tipo de Documento", "Nome do arquivo", "Data da Inclusão");
    $whidth = Array('2%', '35%', '10%', '35%', '7%');
    $align  = Array('center', '', '', '', '');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');


?>