<?php
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Listagem de Servidores');
    echo "<br>";

    #VARIAVEL E CONTROLE DE ABAS.
    $abacod_tela    = ABA_CAD_AVALIACAO_SERVIDOR;
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_pess_avaliados&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>

<script type="text/javascript">

    function abrirAvaliacaoServidor( sercpf ){
        var st = verificarTipoAvaliacao( sercpf );
        if(st == 'SUCCESS'){
            $.ajax({
                type: "POST",
                url : window.location,
                data: "requisicao=verificaDadosServidorAtualizados&sercpf="+sercpf,
                async: true,
                success: function( resp ){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                    if( resp == 'OK'){
                        window.location.href = '?modulo=principal/avaliacao_servidor/cad_avaliacao_servidor&acao=A';
                    }else{
                        alert(resp);
                        window.location.href = '?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A&opc=FT';
                    }
                }
            });
        }
    }

    function deletarFecheEquipe( sercpf ){
        var confirma = confirm("A exclução do \"Chefe da Equipe\" faz com que todos os seus membros e as avaliações já feitas, sejam excluidas.\nDeseja realmente fazer a exclução?");

        if( confirma ){
            $.ajax({
                type: "POST",
                url : window.location,
                data: "requisicao=deletarFecheEquipe&sercpf="+sercpf,
                asynchronous: false,
                success: function( resp ){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                    if(resp == 'OK'){
                        alert('Operação Realizada com sucesso!');
                        //window.location.reload();
                        $('#formulario').submit();
                    }
                }
            });
        }
    }

    function pesquisarServidor(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

    function verificarTipoAvaliacao( sercpf ){
        var result;
        $.ajax({
            type: "POST",
            url : window.location,
            data: "requisicao=verificarTipoAvaliacao&sercpf="+sercpf,
            //asynchronous: false,
            async: false,
            success: function( resp ){
                var resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                if(trim(resp) == 'OK'){
                    result = "SUCCESS";
                }else{
                    result = "ERROR";
                }
            }
        });
        return result;
    }

</script>


<form action="" method="POST" id="formulario" name="formulario">

<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="aba" name="aba" value="<?= $_POST['aba']; ?>"/>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="35%">Nome:</td>
            <td>
                <?= campo_texto('sernome', 'N', 'S', '', 45, 200, '', '', '', '', 0, 'id="sernome"', '', $sernome, null, '', null); ?>
            </td>
            <td rowspan="4">
                <fieldset>
                    <legend>INFORMATIVO</legend>
                        <label>
                            <img src="../imagens/recuo_d_d.gif" style="vertical-align: middle"> &nbsp; Quantidade de Servidores Cadastrados: <b> <?=quantidadeServidorCadastrados();?> </b>
                            <br><br>
                            <img src="../imagens/recuo_d_d.gif" style="vertical-align: middle"> &nbsp; Quantidade de Servidores Avaliados: <b> <?=quantidadeServidorAvaliados();?> </b><br>
                        </label>
            </fieldset>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Código SIAPE:</td>
            <td colspan="2">
                <?= campo_texto('sersiape', 'N', 'S', '', 45, 7, '#######', '', '', '', 0, 'id="sersiape"', '', $sersiape, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">CPF:</td>
            <td>
                <?= campo_texto('sercpf', 'N', 'S', '', 45, 16, '###.###.###-##', '', '', '', 0, 'id="sercpf"', '', $sercpf, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Pedido de Reconsideração:</td>
            <td colspan="2">
                <?php
                    $sql = array(
                            array('codigo'=>'S','descricao'=>'Sim - Possui pedido de reconsideração'),
                            array('codigo'=>'N','descricao'=>'Não - Não possui pedido de reconsideração')
                    );
                    $db->monta_combo("ftprid", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'ftprid', false, $ftprid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Avaliação:</td>
            <td colspan="2">
                <?php
                    $sql = "
                        SELECT  tavid AS codigo,
                                tavdescricao AS descricao
                        FROM gestaopessoa.tipoavaliador
                    ";
                    $db->monta_combo("tavid", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'tavid', false, $tavid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Percentual de Avaliação Realizada pela Equipe:</td>
            <td colspan="2">
                <?php
                    $sql = array(
                            array('codigo'=>'0_10','descricao'=>'Avaliação de Equipe já realizada entre 0% à 10%'),
                            array('codigo'=>'11_20','descricao'=>'Avaliação de Equipe já realizada entre 11% à 20%'),
                            array('codigo'=>'21_30','descricao'=>'Avaliação de Equipe já realizada entre 21% à 30%'),
                            array('codigo'=>'31_40','descricao'=>'Avaliação de Equipe já realizada entre 31% à 40%'),
                            array('codigo'=>'41_50','descricao'=>'Avaliação de Equipe já realizada entre 41% à 50%'),
                            array('codigo'=>'51_60','descricao'=>'Avaliação de Equipe já realizada entre 51% à 60%'),
                            array('codigo'=>'61_70','descricao'=>'Avaliação de Equipe já realizada entre 61% à 70%'),
                            array('codigo'=>'71_80','descricao'=>'Avaliação de Equipe já realizada entre 71% à 80%'),
                            array('codigo'=>'81_90','descricao'=>'Avaliação de Equipe já realizada entre 81% à 90%'),
                            array('codigo'=>'91_100','descricao'=>'Avaliação de Equipe já realizada entre 91% à 100%')
                    );
                    $db->monta_combo("percent_id", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'percent_id', false, $percent_id, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="3">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>
<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"border="0" >
    <tr>
        <td class="SubTituloEsquerda" colspan="10">Legenda</td>
    </tr>
    <tr>
        <td align="center" width="14px">
            <img border="0" align="absmiddle" src="/imagens/pd_normal.JPG" width="14px" title="Avaliação já Realizada" >
        </td>
        <td width="10%">Avaliação já Realizada &nbsp;</td>
        <td align="center" width="14px">
            <img border="0" align="absmiddle" src="/imagens/pd_urgente.JPG" width="14px" title="Avaliação Não Realizada" >
        </td>
        <td width="13%">Avaliação Não Realizada &nbsp;</td>
        <td align="center" width="14px">
            <img border="0" align="absmiddle" src="../imagens/icones/bg.png" title="Possui Pedido de Reconsideração" >
        </td>
        <td width="13%">Possui Pedido de Reconsideração &nbsp;</td>
        <td align="center" width="14px">
            <img border="0" align="absmiddle" src="../imagens/icones/br.png" title="Não possui Pedido de Reconsideração" >
        </td>
        <td>Não possui Pedido de Reconsideração &nbsp;</td>
</table>
<br>

<?PHP

    $membros = tratamentoMembrosEquipe( $_SESSION['usucpf'] );

    if ($_REQUEST['sernome']) {
        $sernome = $_REQUEST['sernome'];
        $where .= " AND public.removeacento(s.sernome) ILIKE ('%{$sernome}%') ";
    }
    if ($_REQUEST['sersiape']) {
        $sersiape = $_REQUEST['sersiape'];
        $where .= " AND s.sersiape = '{$_REQUEST['sersiape']}' ";
    }
    if ($_REQUEST['sercpf']) {
        $sercpf = str_replace( '-', '', str_replace( '.', '', $_REQUEST['sercpf'] ) );
        $where .= " AND s.sercpf = '{$sercpf}' ";
    }

    if ($_REQUEST['ftprid'] == 'S') {
        $where .= " AND p.ftprid IS NOT NULL ";
    }elseif ($_REQUEST['ftprid'] == 'N') {
        $where .= " AND p.ftprid IS NULL ";
    }

    if ($_REQUEST['tavid'] == 1) {
        $where .= " AND a.tavid = 1 ";
    }elseif ($_REQUEST['tavid'] == 2) {
        $where .= " AND b.tavid = 2 ";
    }elseif ($_REQUEST['tavid'] == 3) {
        $where .= " AND c.tavid = 3 ";
    }

    if ($_REQUEST['percent_id'] != '') {
        $percent_id = $_REQUEST['percent_id'];
        $valor = explode("_", $percent_id);

        $where .=  " AND (qt.qtd_aval * 100) / ( SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ) BETWEEN {$valor[0]} AND {$valor[1]} ";
    }

    $acao = "
        <center>
            <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abrirAvaliacaoServidor(\''||s.sercpf||'\');\" title=\"Editar Servidor\" >
        </center>
    ";

    $acaoInativo = "
        <center>
            <img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" title=\"Não é possivél Avaliar Servidor\" >
        </center>
    ";

    $equipe_sim = "
        <span style=\"color:green\"> Com Equipe </span>
    ";

    $equipe_nao = "
        <span style=\"color:red\"> Sem Equipe </span>
    ";

    $sql = "
        SELECT  CASE WHEN (s.sernivelfuncao = 1014 OR s.sernivelfuncao = 1015 OR s.sernivelfuncao = 1016 OR s.sernivelfuncao = 1024 OR s.sernivelfuncao = 1025) OR ( s.tssid IN (3, 4, 14, 18, 19) )
                    THEN '{$acaoInativo}'
                    ELSE CASE WHEN ( a.tavid = 1 OR s.sercpf = '{$_SESSION['usucpf']}' ) OR ( s.sercpfchefe = '{$_SESSION['usucpf']}' )
                            THEN '{$acao}'
                            ELSE '{$acaoInativo}'
                         END
                END as acao,

                s.sersiape,
                '<span style=\"color:#1E90FF\">'||replace(to_char(cast(s.sercpf as bigint), '000:000:000-00'), ':', '.')||'</span>' as sercpf,
                s.sernome,

                CASE WHEN (s.sercargo = 'NULL' OR s.sercargo = '')
                    THEN '-'
                    ELSE s.sercargo
                END AS sercargo,

                CASE WHEN s.sercomequipe = TRUE
                    THEN '{$equipe_sim}'
                    ELSE '{$equipe_nao}'
                END AS sercomequipe,

                CASE
                    WHEN (s.sercpfchefe <> '{$_SESSION['usucpf']}') AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) > 0 THEN 'Chefe'
                    WHEN (s.sercpfchefe = '{$_SESSION['usucpf']}')  THEN 'Subordinado'
                    WHEN (s.sercpfchefe <> '{$_SESSION['usucpf']}') THEN 'Equipe'
                END as hierarquia,

                CASE WHEN p.ftprid IS NOT NULL
                    THEN ' <img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/bg.png\" title=\"\" > '
                    ELSE ' <img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/br.png\" title=\"\" > '
                END as ftprid,

                CASE
                    WHEN p.ftprpedido = '' THEN '<span style=\"font-weight: bold;\">Não Avaliado</span>'
                    WHEN p.ftprpedido = 'D' THEN '<span style=\"font-weight: bold;\">Deferido</span>'
                    WHEN p.ftprpedido = 'P' THEN '<span style=\"font-weight: bold;\">D. Parcialmente</span>'
                    WHEN p.ftprpedido = 'I' THEN '<span style=\"font-weight: bold;\">Indeferido</span>'
                END as ftprpedido,

                CASE WHEN a.tavid = 1
                    THEN '<img align=\"absmiddle\" src=\"/imagens/pd_normal.JPG\" title=\"Situação: Avaliação Realizada\" >'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/pd_urgente.JPG\" title=\"Situação: Avaliação Não Realizada\" >'
                END as auto,

                CASE WHEN b.tavid = 2
                    THEN '<img align=\"absmiddle\" src=\"/imagens/pd_normal.JPG\" title=\"Situação: Avaliação Realizada\" >'
                    ELSE '<img align=\"absmiddle\" src=\"/imagens/pd_urgente.JPG\" title=\"Situação: Avaliação Não Realizada\" >'
                END as chefe,
                /*
                --O SUB-SELECT TRAS O NUMERO QUE PESSOAS QUE FAZER PARTE DA EQUIPE DO USUARIO.
                CASE WHEN s.sercomequipe = 't'
                    THEN COALESCE( (qt.qtd_aval * 100) / (SELECT CASE WHEN COUNT(sercpf) = 0 THEN 1 ELSE COUNT(sercpf) END FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpf <> s.sercpf AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ), 0 ) ||' % Avaliaram'
                    ELSE COALESCE( (qt.qtd_aval * 100) / (SELECT CASE WHEN COUNT(sercpf) = 0 THEN 1 ELSE COUNT(sercpf) END FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = s.sercpf), 0 ) ||' % Avaliaram'
                END AS perc_membro,
                */
                CASE WHEN s.sercomequipe = 't'
                    THEN '<span style=\"font-weight: bold;\">'|| (SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercomequipe = TRUE AND sercpf <> s.sercpf AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ) ||' / '|| qt.qtd_aval ||'</span>'
                    ELSE '<span style=\"font-weight: bold;\">'|| (SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = s.sercpf) ||' / '|| qt.qtd_aval  ||'</span>'
                END AS membros_avaliacoes

        FROM gestaopessoa.servidor AS s

        JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid

        LEFT JOIN gestaopessoa.ftpedidoreconsideracao AS p ON p.fdpcpf = s.sercpf AND anoreferencia = {$_SESSION['exercicio']}

        LEFT JOIN(
            SELECT  tavid, sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = 1
            GROUP BY tavid, sercpf
        ) AS a ON a.sercpf = s.sercpf

        LEFT JOIN(
            SELECT  tavid, sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = 2
            GROUP BY tavid, sercpf
        ) AS b ON b.sercpf = s.sercpf

        LEFT JOIN(
            SELECT  tavid, sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND tavid = 3
            GROUP BY tavid, sercpf
        ) AS c ON c.sercpf = s.sercpf

        LEFT JOIN(
            SELECT  COUNT(resavaliacpf) AS qtd_aval,
                sercpf
            FROM gestaopessoa.respostaavaliacao
            WHERE resano = {$_SESSION['exercicio']} AND defid = ".ID_PERG_RESPAVALIACAO_QTR_RESP." AND tavid = ".TIPO_AVAL_CONSENSO."
            GROUP BY sercpf
        ) AS qt ON qt.sercpf = s.sercpf

        WHERE s.serstatus = TRUE AND s.seranoreferencia = {$_SESSION['exercicio']} AND s.sercpf IN {$membros}

        $where

        ORDER BY hierarquia, s.sernome
    ";
    //ver($sql, d);
    $cabecalho = array("Ação", "SIAPE", "CPF", "Nome", "Cargo", "Equipe", "Hierarquia", "Reconsid.", "Reconsid.", "Auto-Aval.", "Aval. Superior", "Memb/Aval");
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'center', 'left', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center');
    $tamanho = Array('3%', '5%', '7%', '14%', '14%', '8%', '8%', '3%', '4%', '4%', '5%', '5%', '5%', '3%' );
    $db->monta_lista($sql, $cabecalho, 60, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>