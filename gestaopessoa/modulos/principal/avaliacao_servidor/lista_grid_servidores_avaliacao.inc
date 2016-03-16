<?php
    header("Content-Type: text/html; charset=ISO-8859-1");

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Listagem de Servidores');
    echo "<br>";

    #VARIAVEL E CONTROLE DE ABAS.
    $abacod_tela    = ABA_LISTA_SERVIDORES_EQUIPES;
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_servidores_avaliacao&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
    
    function autorizaBotaoLimparAval( $dados ){
        global $db;
        
        $autoriza   = $dados['autoriza'];
        $cpf        = $dados['cpf'];

        if( $cpf != '' ){
            $sql = " UPDATE gestaopessoa.servidor SET serlimpaaval = '{$autoriza}' WHERE sercpf = '{$cpf}' AND seranoreferencia = {$_SESSION['exercicio']} RETURNING sercpf;";
            $resp = $db->pegaUm($sql);
        }

        if( $resp > 0 ){
            $db->commit();
            
            if($autoriza == 'true'){
                echo "<resp> <div id=\"div_servidor_{$cpf}\"><input type=\"checkbox\" id=\"servidor_{$cpf}\" name=\"servidor\" value=\"{$cpf}\" onclick=\"autorizaBotaoLimparAval(this);\" checked=\"checked\"> <br> <span style=\"color:green\"> Autorizado </span> </div> </resp> ";
            }else{
                echo "<resp> <div id=\"div_servidor_{$cpf}\"><input type=\"checkbox\" id=\"servidor_{$cpf}\" name=\"servidor\" value=\"{$cpf}\" onclick=\"autorizaBotaoLimparAval(this);\"> <br> <span style=\"color:red\"> Não Autorizado </span> </div> </resp> ";                
            }
            die();
        }
    }

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
    
    function autorizaBotaoLimparAval(obj){
        var status = obj.checked;
        var cpf = obj.value;
        var existe_aval;
        var exc_operacao;

        existe_aval = existeAvaliacaoTipo( cpf );
        
        if( status ){
            if( existe_aval == 'S' ){
                var confirma = confirm('Essa ação possibilitara que o usuário limpe a avaliação, apando a avaliação feita a ele e/ou feito por ele. Deseja continuar a operação?');
                if( confirma ){
                    exc_operacao = 'S';
                }else{
                    obj.checked = !obj.checked;
                    return false;
                }
            }else{
                alert('Não é possível realizar essa operação. Não há avaliação feita a esse usuário!');
                obj.checked = !obj.checked;
                return false;
            }
        }else{
            var msg = confirm('Retirar a autorização do usuário para limpar a avaliação?');
            if( !msg ){
                return false;
            }else{
                exc_operacao = 'S';
            }
        }

        if( exc_operacao == 'S' ){
            $.ajax({
                type: "POST",
                url : window.location.href,
                data: "requisicao=autorizaBotaoLimparAval&autoriza="+status+"&cpf="+cpf,
                asynchronous: false,
                success: function( msg ){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                    $('#div_servidor_'+cpf).html(resp);
                }
            });
        }
    }

    function abrirAvaliacaoServidor( sercpf ){
        var st = verificarTipoAvaliacao( sercpf );
        if(st == 'SUCCESS'){
            $.ajax({
                type: "POST",
                url     : window.location,
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
    
    function existeAvaliacaoTipo( cpf ) {
        var result;
        $.ajax({
            type: "POST",
            url : window.location.href,
            data: "requisicao=existeAvaliacaoTipo&cpf="+cpf,
            async: false,//esse opção é necessária usar quando se quer fazer o returno de algum valor;
            success: function( resp ){
                if( trim(resp) == 'S' ){
                    result = 'S';
                }else{
                    result = 'N';
                }
            }
        });
        return result;
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
            <td width="35%">
                <?= campo_texto('sernome', 'N', 'S', '', 45, 200, '', '', '', '', 0, 'id="sernome"', '', $sernome, null, '', null); ?>
            </td>
            <td rowspan="4">
                <fieldset>
                    <legend>INFORMATIVO</legend>
                        <label>
                            <img src="../imagens/recuo_d_d.gif" style="vertical-align: middle"> &nbsp; Qtd. de Servidores Cadastrados: <b> <?=quantidadeServidorCadastrados();?> </b>
                            <br><br>
                            <img src="../imagens/recuo_d_d.gif" style="vertical-align: middle"> &nbsp; Qtd. de Servidores com a Avaliação Iniciada: <b> <?=quantidadeServidorAvaliados();?> </b>
                            <br><br>
                            <img src="../imagens/recuo_d_d.gif" style="vertical-align: middle"> &nbsp; Qtd. de Servidores com a Avaliação Finalizada: <b> <?=quantidadeServidorAvaliacaoFinalizada();?> </b>
                            <br>
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
            <td colspan="2">
                <?= campo_texto('sercpf', 'N', 'S', '', 45, 16, '###.###.###-##', '', '', '', 0, 'id="sercpf"', '', $sercpf, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Hierarquia:</td>
            <td colspan="2">
                <?php
                    $arrChefe = array(
                        array("codigo"=>"C", "descricao"=>"Chefe"),
                        array("codigo"=>"S", "descricao"=>"Subordinado")
                    );
                    $db->monta_combo("serchefe_id", $arrChefe, 'S', 'Selecione...', '', '', '', '340', 'N', 'serchefe_id', false, $serchefe_id, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Situação do Servidor:</td>
            <td colspan="2">
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
                        SELECT  9999 AS codigo,
                                'Não Avaliado pelo Chefe' AS descricao
                        UNION
                        SELECT  999 AS codigo,
                                'Sem Auto Avaliação' AS descricao
                        UNION
                        SELECT  tavid AS codigo,
                                tavdescricao AS descricao
                        FROM gestaopessoa.tipoavaliador
                        ORDER BY 1
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
                            array('codigo'=>'91_100','descricao'=>'Avaliação de Equipe já realizada entre 91% à 100%'),
                            array('codigo'=>'101_1000','descricao'=>'Avaliação de Equipe acima de 100%')
                    );
                    $db->monta_combo("percent_id", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'percent_id', false, $percent_id, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Equipe:</td>
            <td colspan="2">
                <input type="radio" name="sercomequipe" value="S" <?PHP echo $checkSim_ind; echo $habilitado['dis']; ?>> <b>Sim</b>
                <input type="radio" name="sercomequipe" value="N" <?PHP echo $checkSim_ano; echo $habilitado['dis']; ?>> <b>Não</b>
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

    $perfil = pegaPerfilGeral();

    if( in_array( PERFIL_SUPER_USER, $perfil ) ){
        $membrosEquipe = "s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.",".SITUACAO_CEDIDO.",".SITUACAO_EXCEDENTE.",".SITUACAO_ATIVO_PERM_L.",".SITUACAO_ANISTIADO.",".SITUACAO_EXERC.",".CLT_ANS_DEC_6657_08.",".NOMEADO_CARGO_COMIS.",".EXERC_DESCENT_CARREI.",".REQUISITADO.",".REQ_DE_OUTROS_ORGAOS.")";
    }else{
        $membros = tratamentoMembrosEquipe( $_SESSION['usucpf'] );

        $membrosEquipe = "s.sercpf IN {$membros}";
    }

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
    if ($_REQUEST['serchefe_id'] == 'C') {
        $where .= " AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) <> 0 ";
    }elseif($_REQUEST['serchefe_id'] == 'S') {
        $where .= " AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) = 0 ";
    }
    if ($_REQUEST['tssid']) {
        $tssid = $_REQUEST['tssid'];
        $where .= " AND s.tssid = {$tssid} ";
    }
    if ($_REQUEST['sercomequipe'] == 'S'){
        $where .= " AND s.sercomequipe = TRUE";
    }elseif($_REQUEST['sercomequipe'] == 'N'){
        $where .= " AND s.sercomequipe = FALSE";
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
    }elseif ($_REQUEST['tavid'] == 999) {
        $where .= " AND a.tavid IS NULL ";
    }elseif( $_REQUEST['tavid'] == 9999 ){
        $where .= " AND a.tavid = 1 AND c.tavid = 3 AND b.tavid IS NULL  ";
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

    if ($_REQUEST['percent_id'] != '') {
        $percent_id = $_REQUEST['percent_id'];
        $valor = explode("_", $percent_id);

        $where .=  "
            AND(
                CASE WHEN s.sercomequipe = 't'
                    THEN ( (qt.qtd_aval * 100) / (SELECT COUNT(sercpf) FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercomequipe = TRUE AND (sercpfchefe = s.sercpfchefe OR sercpfchefe = s.sercpf) ) )  BETWEEN {$valor[0]} AND {$valor[1]}
                    ELSE ( (qt.qtd_aval * 100) / (SELECT CASE WHEN COUNT(sercpf) = 0 THEN 1 ELSE COUNT(sercpf) END FROM gestaopessoa.servidor WHERE seranoreferencia = {$_SESSION['exercicio']} AND sercpfchefe = s.sercpf) ) BETWEEN {$valor[0]} AND {$valor[1]}
                END
            )
        ";
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
    
    if( ( in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil) || in_array(PERFIL_SUPER_USER, $perfil) ) ){
        $autor_sim = "
            <div id=\"div_servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"autorizaBotaoLimparAval(this);\" checked=\"checked\"> <br> <span style=\"color:green\"> Autorizado </span> </div>
        ";
        $autor_nao = "
            <div id=\"div_servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"autorizaBotaoLimparAval(this);\"> <br> <span style=\"color:red\"> Não Autorizado </span> </div>
        ";
    }else{
        $autor_sim = "
            <div id=\"div_servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" disabled=\"disabled\" title=\"Seu perfil não da permissão para essa ação\"> <br> <span style=\"color:green\"> Autorizado </span> </div>
        ";
        $autor_nao = "
            <div id=\"div_servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" disabled=\"disabled\" title=\"Seu perfil não da permissão para essa ação\"> <br> <span style=\"color:red\"> Não Autorizado </span> </div>
        ";
    }

    $sql = "
        SELECT  DISTINCT
                CASE WHEN (s.sernivelfuncao = 1014 OR s.sernivelfuncao = 1015 OR s.sernivelfuncao = 1016 OR s.sernivelfuncao = 1024 OR s.sernivelfuncao = 1025) OR ( s.tssid IN (3, 4, 14, 18, 19) )
                    THEN '{$acaoInativo}'
                    ELSE
                        CASE WHEN ( a.tavid = 1 OR s.sercpf = '{$_SESSION['usucpf']}' ) OR ( s.sercpfchefe = '{$_SESSION['usucpf']}' ) or 1=1
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

                CASE WHEN s.tssid IS NULL
                    THEN '-'
                    ELSE t.tssdescricao
                END AS tssdescricao,

                '<span style=\"color:#1E90FF\">'||( SELECT DISTINCT sernome FROM gestaopessoa.servidor WHERE sercpf = s.sercpfchefe AND seranoreferencia = {$_SESSION['exercicio']} )||'</span>' as sercpfchefe,

                CASE
                    WHEN (s.sercpfchefe <> '{$_SESSION['usucpf']}') AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) > 0 THEN 'Chefe'
                    WHEN (s.sercpfchefe = '{$_SESSION['usucpf']}')  THEN 'Subordinado'
                    WHEN (s.sercpfchefe <> '{$_SESSION['usucpf']}') THEN 'Equipe'
                END as hierarquia,
                
                sergratificacao,
                
                CASE WHEN s.serlimpaaval = 't'
                    THEN '{$autor_sim}'
                    ELSE '{$autor_nao}'
                END as serlimpaaval,               

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

                --O SUB-SELECT TRAS O NUMERO QUE PESSOAS QUE FAZER PARTE DA EQUIPE DO USUARIO.
                /*
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

        LEFT JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid

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
            WHERE resano = {$_SESSION['exercicio']} AND defid = ".(int)ID_PERG_RESPAVALIACAO_QTR_RESP." AND tavid = ".TIPO_AVAL_CONSENSO."
            GROUP BY sercpf
        ) AS qt ON qt.sercpf = s.sercpf

        WHERE s.serstatus = TRUE AND s.seranoreferencia = {$_SESSION['exercicio']} AND $membrosEquipe

        $where
            
        ORDER BY hierarquia, s.sernome 
    ";//ver($sql, d);
    $cabecalho = array("Ação", "SIAPE", "CPF", "Nome", "Cargo", "Equipe", "Situação", "Chefia", "Hierarquia", "Gratif.", "Autorizado", "Reconsid.", "Status Reconsid.", "Auto-Aval.", "Aval. Superior", "Memb/Aval");
    $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'center', 'left', 'left', 'left', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center');
    //$tamanho = Array('2%', '3%', '4%', '9%', '9%', '4%', '7%', '9%', '4%', '3%', '3%', '4%', '4%', '5%', '5%', '3%' );
    $db->monta_lista($sql, $cabecalho, 60, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>