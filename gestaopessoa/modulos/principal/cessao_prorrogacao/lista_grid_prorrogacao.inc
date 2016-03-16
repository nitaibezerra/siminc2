<?PHP
    global $db;

    unset($_SESSION['gestao']['cprid']);

    unset($_SESSION['gestao']['Aba']);

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_GET['aba'] == "cessao"){
       $url = "/gestaopessoa/gestaopessoa.php?modulo=principal/cessao_prorrogacao/inicio_cessao_prorrogacao&acao=A&aba=".$_GET['aba'];
    }else{
       $url = "/gestaopessoa/gestaopessoa.php?modulo=principal/cessao_prorrogacao/inicio_cessao_prorrogacao&acao=A&aba=".$_GET['aba'];
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão Pessoas - Cessão/rorrogação', 'Listagem Prorrogação Servidores');

    echo "<br>";

    $abacod_tela    = PAGINA_INICIAL_ABA_LISTAGEM;
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_prorrogacao&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function cadastraProcesso(aba){
        if( aba == 'P' ){
            window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_busca_prorrogacao&acao=A';
        }
    }

    function deletaProcessoCessaoProrrogacao(cprid){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=deletaProcessoCessaoProrrogacao&tipo=P&cprid="+cprid,
                asynchronous: false,
                success: function(resp){
                    if( trim(resp) == 'ok' ){
                        alert('Operação Realizada com sucesso!');
                        $('#formulario').submit();
                    }else{
                        alert('Ocorreu um problema, não foi possiv?e realizar a operação. Tente novamente mais tarde!');
                    }
                }
            });
        }
    }

    function editarCessao(cprid){
        window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_cessao&acao=A&cprid='+cprid;
    }

    function editarConsulta(cprid){
        window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_consulta&acao=A&cprid='+cprid;
    }

    function editarProrrogacao(cprid){
        window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_prorrogacao&acao=A&cprid='+cprid;
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

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="30%">Nº do Processo:</td>
            <td>
                <?= campo_texto('cprnumprocesso', 'N', 'S', '', 33, 20, '', '', '', '', 0, 'id="cprnumprocesso"', '', $cprnumprocesso, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">SIAPE:</td>
            <td>
                <?= campo_texto('nu_matricula_siape', 'N', 'S', '', 33, 10, '', '', '', '', 0, 'id="nu_matricula_siape"', '', $nu_matricula_siape, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">CPF:</td>
            <td>
                <?= campo_texto('nu_cpf', 'N', 'S', '', 33, 16, '###.###.###-##', '', '', '', 0, 'id="nu_cpf"', '', $nu_cpf, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Nome do Servidor:</td>
            <td>
                <?= campo_texto('no_servidor', 'N', 'S', '', 33, 200, '', '', '', '', 0, 'id="no_servidor"', '', $no_servidor, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Período:</td>
            <td>
                <?PHP
                    echo campo_data2('cprperiodo_1','N','S','','DD/MM/YYYY','','', '', '', '', 'cprperiodo_1' );
                    echo "&nbsp; a &nbsp;";
                    echo campo_data2('cprperiodo_2','N','S','','DD/MM/YYYY','','', '', '', '', 'cprperiodo_2' );
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Situação do WorkFlow:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = ".WF_CESSAO_FLUXO_1."
                    ";
                    $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', '255', 'N', 'esdid', false, $esdid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Órgão Cessionário:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  ogcid AS codigo,
                                ogcdsc AS descricao
                        FROM gestaopessoa.orgaocessionario
                    ";
                    $db->monta_combo("ogcid", $sql, 'S', 'Selecione...', '', '', '', '255', 'N', 'ogcid', false, $ogcid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Unidade de Cadastro:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  orcid AS codigo,
                                orcdsc AS descricao
                        FROM gestaopessoa.origemcessao
                        WHERE orcid in (2,3,6)
                        order by orcdsc
                    ";
                    $db->monta_combo("orcid", $sql, 'S', 'Selecione...', '', '', '', '255', 'N', 'orcid', false, $orcid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Vencimento do Período:</td>
            <td>
                <?PHP
                    $sql = array(
                            array('codigo'=>1,'descricao'=>'Prorrogação com perído de 20 dias para vencimento')
                    );
                    $db->monta_combo("venc_periodo", $sql, 'S', 'Selecione...', '', '', '', '255', 'N', 'venc_periodo', false, $venc_periodo, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>

<?PHP
    $sql = "SELECT tpsdsc FROM gestaopessoa.tiposolicitacao";
    $dados = $db->carregar($sql);
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloEsquerda" colspan="2">Legenda</td>
    </tr>
    <tr>
        <td>
            <img border="0" align="absmiddle" src="../imagens/icones/bb.png" title="<?= $dados[0]['tpsdsc'] ?>" > <?= $dados[0]['tpsdsc'] ?> &nbsp;
            <img border="0" align="absmiddle" src="../imagens/icones/br.png" title="<?= $dados[1]['tpsdsc'] ?>" > <?= $dados[1]['tpsdsc'] ?> &nbsp;
            <img border="0" align="absmiddle" src="../imagens/icones/bg.png" title="<?= $dados[2]['tpsdsc'] ?>" > <?= $dados[2]['tpsdsc'] ?> &nbsp;
        </td>
    </tr>
</table>
<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"border="0" >
    <tr>
        <td align="center" width="14px">
            <img border="0" align="absmiddle" src="/imagens/obras/atencao_verde.png" width="18px" title="Prorrogação com período de 30 dias ou menos para vencimento!" >
        </td>
        <td>Prorrogação com perído de 30 dias ou menos para vencimento! &nbsp;</td>
</table>

<br>

<?PHP
    if ($_REQUEST['cprnumprocesso']) {
        $where = " AND cp.cprnumprocesso = '{$_REQUEST['cprnumprocesso']}'";
    }
    if ($_REQUEST['nu_matricula_siape']) {
        $where .= " AND s.nu_matricula_siape = '{$_REQUEST['nu_matricula_siape']}'";
    }
    if ($_REQUEST['nu_cpf']) {
        $where .= " AND s.nu_cpf = '".str_replace( '-', '', str_replace( '.', '', $_REQUEST['nu_cpf'] ) )."'";
    }
    if ($_REQUEST['no_servidor']) {
        $where .= " AND s.no_servidor ilike ('%".trim($_REQUEST['no_servidor'])."%')";
    }
    if ($_REQUEST['cprperiodo_1'] && $_REQUEST['cprperiodo_2']) {
        $where .= " AND cp.cprperiodo BETWEEN '".formataDataBanco($_REQUEST['cprperiodo_2'])."' AND '".formataDataBanco($_REQUEST['cprperiodo_2'])."' ";
    }
    if ($_REQUEST['esdid']) {
        $where .= " AND d.esdid = {$_REQUEST['esdid']}";
    }
    if ($_REQUEST['ogcdsc']) {
        $where .= " AND cp.ogcdsc = {$_REQUEST['ogcdsc']}";
    }
    if ($_REQUEST['orcdsc']) {
        $where .= " AND cp.orcdsc = {$_REQUEST['orcdsc']}";
    }
    if ($_REQUEST['venc_periodo'] == 1) {
        $where .= " AND ( pp.cprid IS NOT NULL ) ";
    }
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td style="padding: 10px;">
            <span style="cursor: pointer" onclick="cadastraProcesso('P');" title="Novo Processo" >
                <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Processo (Prorrogação)
            </span>
        </td>
    </tr>
</table>

<?PHP
    $acao = "
        <center>
            <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarProrrogacao('||cp.cprid||');\" title=\"Editar Servidor\" >
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"deletaProcessoCessaoProrrogacao('||cp.cprid||')\" title=\"Excluir Prorrogação\" >
        </center>
    ";

    $sql = "
        SELECT  DISTINCT '{$acao}' as acao,
                cp.cprid,
                CASE
                    WHEN ( pp.cprid IS NOT NULL ) THEN '<img align=\"absmiddle\" src=\"/imagens/obras/atencao_verde.png\" width=\"18px\" title=\"Prorrogação com perído de 20 dias ou menos para vencimento!\" >'
                END AS vencimento_periodo,
                oc.orcdsc, --Fluxo de origem
                CASE
                    WHEN cp.tpsid = 1 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/bb.png\" title=\"\" >'
                    WHEN cp.tpsid = 2 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/br.png\" title=\"\" >'
                    WHEN cp.tpsid = 3 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/bg.png\" title=\"\" >'
                END AS tpsid,
                cp.cprnumprocesso,

                replace(to_char(cast(s.nu_cpf as bigint), '000:000:000-00'), ':', '.') as fdpcpf,
                s.nu_matricula_siape AS fdpsiape,
                s.no_servidor AS fdpnome,

                '' AS fcmdescricao, --Cargo Efetivo

                os.ogcdsc,
                '<span onclick=\"exibirHistorico('|| cp.docid ||');\" style=\"cursor: pointer; color:#4682B4; \" ><b>'||ed.esddsc ||'</b></span>' as status
        FROM gestaopessoa.cessaoprorrogacao cp

        JOIN siape.tb_servidor_simec s ON cast(s.nu_matricula_siape as character(7)) = cp.nu_matricula_siape AND co_situacao_servidor IN ( 1,8,11,43 )

        JOIN gestaopessoa.origemcessao oc ON oc.orcid = cp.orcid
        JOIN gestaopessoa.orgaocessionario os ON os.ogcid = cp.ogcid

        LEFT JOIN workflow.documento d ON d.docid = cp.docid
        LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid

        LEFT JOIN(
                SELECT	cprid,
                        nu_matricula_siape,
                        orcid,
                        to_char( cprperiodo - now(), 'DD') AS dias_pro
                FROM gestaopessoa.cessaoprorrogacao
                WHERE cprtipo = 'P' AND cprstatus = 'A' AND to_number( to_char( cprperiodo - now(), 'DD'), '999' ) <= 30
        ) AS pp ON pp.orcid = cp.orcid and pp.nu_matricula_siape = cp.nu_matricula_siape

        WHERE cp.cprtipo = 'P' AND cp.cprstatus = 'A' {$where}

        ORDER BY s.no_servidor, cp.cprid
    ";
    //$cabecalho = array("Ação", "Seq", "Origem Fluxo", "Nº do Porcesso", "Tipo", "CPF Servidor", "SIAPE", "Servidor", "Função Desempenhada", "Cargo Efetivo", "Órgão Cessionário", "Situação", "Prazo Até");
    $cabecalho = array("Ação", "Seq", "Venc.<br>30 dias", "Unidade de Cadastro", "Tipo", "Nº do Porcesso", "CPF Servidor", "SIAPE", "Servidor", "Cargo Efetivo", "Órgão Cessionário", "Status");
    $alinhamento = Array('center', 'center', 'left', 'left', 'center');
    //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>
