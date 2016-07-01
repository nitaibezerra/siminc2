<?php

    header("Content-Type: text/html; charset=ISO-8859-1");

    unset($_SESSION['cpfavaliado']);

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Listagem de Servidores - Montagem de Equipe');
    echo "<br>";

    #VARIAVEL E CONTROLE DE ABAS.
    $abacod_tela    = ABA_LISTA_SERVIDORES_EQUIPES;
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_monta_equipe&acao=A';
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
    $(document).ready(function(){

    });

    function abrirCadServidorEquipe( sercpfchefe ){
        window.open('gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_servidor_equipe&acao=A&sercpfchefe='+sercpfchefe,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=600');
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
            <td class ="SubTituloDireita" width="35%" style="margin-left:20px; margin-bottom: ">Nome:</td>
            <td>
                <?= campo_texto('sernome', 'N', 'S', '', 45, 200, '', '', '', '', 0, 'id="sernome"', '', $sernome, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Código SIAPE:</td>
            <td>
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
            <td class ="SubTituloDireita">Situação do Servidor:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  tssid AS codigo,
                                tssdescricao AS descricao
                        FROM gestaopessoa.tiposituacaoservidor
                        WHERE tssid IN (".SITUACAO_ATIVO_PERMANENTE.",".SITUACAO_CEDIDO.",".SITUACAO_EXCEDENTE.",".SITUACAO_ATIVO_PERM_L.",".SITUACAO_ANISTIADO.",".SITUACAO_EXERC.",".CLT_ANS_DEC_6657_08.",".NOMEADO_CARGO_COMIS.")
                        ORDER BY tssdescricao
                    ";
                    $db->monta_combo("tssid", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'tssid', false, $tssid, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Gratificação:</td>
            <td colspan="2">
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_PE" value="PE"> <b>GDPGPE</b>
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_CE" value="CE"> <b>GDACE</b>
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_PS" value="PS"> <b>GDAPS</b>
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_OS" value="OS"> <b>CEDIDO</b>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Avaliadores Externos:</td>
            <td colspan="2">
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_FE_S" value="S"> <b> Sim </b>
                <input type="radio" name="sertipogratificacao" id="sertipogratificacao_FE_N" value="N"> <b> Não </b>
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

<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"border="0" >
    <tr>
        <td class="SubTituloEsquerda" colspan="5">Legenda</td>
    </tr>
    <tr>
        <td align="left" width="12px">
            <img border="0" align="absmiddle" src="/imagens/gif_inclui.gif" width="12px" title="Avaliação já Realizada" >
        </td>
        <td align="left">Definir novo chefe de Equipe. &nbsp;</td>
</table>
<br>

<?PHP
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
    if ($_REQUEST['tssid']) {
        $tssid = $_REQUEST['tssid'];
        $where .= " AND s.tssid = {$tssid} ";
    }

    if( $_REQUEST['sertipogratificacao'] != '' ){
        switch( $_REQUEST['sertipogratificacao'] ){
            case 'PE':
                $where .= "AND s.sertipogratificacao = 'PE'";
            break;
            case 'CE':
                $where .= "AND s.sertipogratificacao = 'CE'";
            break;
            case 'PS':
                $where .= "AND s.sertipogratificacao = 'PS'";
            break;
            case 'OS':
                $where .= "AND s.sertipogratificacao = 'OS'";
            break;
            case 'S':
                $where .= "AND s.sertipogratificacao = 'FE'";
            break;
            case 'N':
                $where .= "AND s.sertipogratificacao <> 'FE'";
            break;
        }
    }

    $tssid = SITUACAO_ATIVO_PERMANENTE.",".SITUACAO_CEDIDO.",".SITUACAO_EXCEDENTE.",".SITUACAO_ATIVO_PERM_L.",".SITUACAO_ANISTIADO.",".SITUACAO_EXERC.",".CLT_ANS_DEC_6657_08.",".NOMEADO_CARGO_COMIS.",".EXERC_DESCENT_CARREI.",".REQUISITADO.",".REQ_DE_OUTROS_ORGAOS;
    
    $acao = "
        <div>
            <img style=\"cursor: pointer; margin-left:5px; \" align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" onclick=\"abrirCadServidorEquipe(\''||s.sercpf||'\');\" title=\"Editar Servidor\">
        </div>
    ";

    $sql = "
        SELECT  DISTINCT '{$acao}' as acao,
                s.sersiape,
                '<span style=\"color:#1E90FF\">'||replace(to_char(cast(s.sercpf as bigint), '000:000:000-00'), ':', '.')||'</span>' as sercpf,
                s.sernome,

                CASE WHEN (s.sercargo = 'NULL' OR s.sercargo = '')
                    THEN '-'
                    ELSE s.sercargo
                END AS sercargo,

                CASE WHEN s.tssid IS NULL
                    THEN '-'
                    ELSE t.tssdescricao
                END AS tssdescricao,
                
                CASE 
                    WHEN sertipogratificacao = 'PE' THEN 'GDPGPE'
                    WHEN sertipogratificacao = 'CE' THEN 'GDACE'
                    WHEN sertipogratificacao = 'OS' THEN 'CEDIDO'
                    WHEN sertipogratificacao = 'PS' THEN 'GDAPS'
                    WHEN sertipogratificacao = 'FE' THEN 'Chefia Cedido'
                END AS sertipogratificacao

        FROM gestaopessoa.servidor AS s

        JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid

        WHERE s.sercpf IS NOT NULL AND s.seranoreferencia = {$_SESSION['exercicio']} AND s.tssid IN ({$tssid}) AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) = 0

        $where

        ORDER BY s.sernome, sertipogratificacao
    ";
    $cabecalho = array("Ação", "SIAPE", "CPF", "Nome", "Cargo", "Situação", "Gratificação");
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left', 'center');
    $tamanho = Array('3%', '8%', '8%', '30%', '30%', '15%', '15%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>