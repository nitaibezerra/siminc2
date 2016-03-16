<?PHP
    header("Content-Type: text/html; charset=ISO-8859-1");

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    unset($_SESSION['cpfavaliado']);

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Listagem de Chefes/Equipes');
    echo "<br>";

    #VARIAVEL E CONTROLE DE ABAS.
    $abacod_tela    = ABA_LISTA_SERVIDORES_EQUIPES;
    $url            = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_feche_equipe&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    $perfil = pegaPerfilGeral();

    if( in_array( PERFIL_AVAL_EQUIPE_APOIO, $perfil ) ){
        $equipe_apoio = 'S';
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

    function abrirCadServidorEquipe( sercpfchefe ){
        window.open('gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_servidor_equipe&acao=A&sercpfchefe='+sercpfchefe,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=600');
    }

    function abrirAlteraFecheEquipe( sercpf ){
        var confirma = confirm("A alteração de \"Feche da Equipe\" faz com que as avaliações já feitas por toda a equipe, sejam excluidas.\nDeseja realmente fazer a alteração do chefe da Equipe?");

        if(confirma){
            window.open('gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_historico_alteracao_feche&acao=A&tipo_acao=A&sercpf='+sercpf,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=600');
        }
    }

    function abrirAvaliacaoServidor( sercpf ){
        $.ajax({
            type: "POST",
            url : window.location,
            data: "requisicao=verificaDadosServidorAtualizados&sercpf="+sercpf,
            asynchronous: false,
            success: function( resp ){
                var dados = $.parseJSON(resp);
                if(dados.cpf_pessoal == ''){
                    alert('Dados Pessoas não informados. Favor atualizar os dados Pessoais do Servidor!');
                }else if(dados.cpf_funcion == ''){
                    alert('Dados Funcionais não informados. Favor atualizar os dados Funcionais do Servidor!');
                }else if(dados.cpf_pessoal != '' && dados.cpf_funcion != ''){
                    alert('Dados Pessoais e Funcionais Atualizados. De inicio a Avaliação!');
                    window.location.href = '?modulo=principal/avaliacao_servidor/cad_avaliacao_servidor&acao=A&sercpf='+sercpf;
                }
            }
        });

    }

    function abrirListaEquipe(idImg, prpid){
        var img 	= $( '#'+idImg );
        var tr_nome = 'listaEquipe_'+ prpid;
        var td_nome = 'tdE_'+ prpid;

        if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            carregaListaEquipe(prpid, td_nome);
        }
        if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
            $('#'+tr_nome).css('display','');
            img.attr('src','../imagens/menos.gif');
        } else {
            $('#'+tr_nome).css('display','none');
            img.attr('src','/imagens/mais.gif');
        }
    }

    function atualizaServidorEquipeSimNao(obj) {

        var status = obj.checked;
        var cpf = obj.value;
        var existe_aval;
        var exc_aval;

        existe_aval = existeAvaliacaoTipo( cpf );

        if( existe_aval == 'S' ){
            var confirma = confirm('Existe avaliação feita a esse usuário. Esse procedimento irá apagar a avaliação feita a ele e/ou feito por ele. Deseja continuar a operação?');
            if( confirma ){
                exc_aval = 'S';
            }else{
                obj.checked = !obj.checked;
                return false;
            }
        }

        $.ajax({
            type: "POST",
            url : window.location.href,
            data: "requisicao=atualizaServidorEquipeSimNao&status="+status+"&cpf="+cpf+"&exc_aval="+exc_aval,
            asynchronous: false,
            success: function( msg ){
                var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                $('#servidor_'+cpf).html(resp);
            }
        });
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

    function carregaListaEquipe(sercpf, td_nome){
        divCarregando();
        $.ajax({
            type: "POST",
            url : window.location,
            data: "requisicao=carregaListaEquipe&sercpfchefe="+sercpf,
            async: false,
            success: function(msg){
                $('#'+td_nome).html(msg);
                divCarregado();
            }
        });
    }

    function deletarServidorEquipe( sercpf ){
        var confirma = confirm("A exclução deste \"Membro da Equipe\" faz com que as avaliações já feitas por ele e a ele, sejam excluidas.\nDeseja realmente fazer a exclução?");

        if( confirma ){
            window.open('gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_historico_alteracao_feche&acao=A&tipo_acao=D&tipo_serv=S&sercpf='+sercpf,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=410');
        }
    }

    function deletarFecheEquipe( sercpf ){
        var confirma = confirm("A exclução do \"Chefe da Equipe\" faz com que todos os seus membros e as avaliações já feitas, sejam excluidas.\nDeseja realmente fazer a exclução?");

        if( confirma ){
            window.open('gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_historico_alteracao_feche&acao=A&tipo_acao=D&tipo_serv=CH&sercpf='+sercpf,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=410');
        }
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
            <td class ="SubTituloDireita">Avaliado:</td>
            <td>
                <input type="radio" name="columncprprazo" value="I" <?PHP echo $checkSim_ind; echo $habilitado['dis']; ?>> <b>Sim</b>
                <input type="radio" name="columncprprazo" value="U" <?PHP echo $checkSim_ano; echo $habilitado['dis']; ?>> <b>Não</b>
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
        <td class="SubTituloEsquerda" colspan="6">Legenda</td>
    </tr>
    <tr>
        <td align="center" width="12px">
            <img border="0" align="absmiddle" src="/imagens/excluir.gif" width="12px" title="Avaliação já Realizada" >
        </td>
        <td width="15%">Excluir o membro ou chefe da Equipe. &nbsp;</td>
        <td align="center" width="12px">
            <img border="0" align="absmiddle" src="/imagens/gif_inclui.gif" width="12px" title="Avaliação já Realizada" >
        </td>
        <td width="12%">Inserir um novo membro a Equipe. &nbsp;</td>
        <td align="center" width="12px">
            <img border="0" align="absmiddle" src="/imagens/alterar.gif" width="12px" title="Avaliação Não Realizada" >
        </td>
        <td>Alteração do Chefe da Equipe. &nbsp;</td>
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
    if ($_REQUEST['serchefe_id'] == 'C') {
        $where .= " AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) <> 0 ";
    }elseif($_REQUEST['serchefe_id'] == 'S') {
        $where .= " AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) = 0 ";
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

    $acao = "
        <div>
            <img style=\"cursor:pointer; margin-left:0px; \" id=\"equipe_'|| s.sercpf ||'\" src=\"/imagens/mais.gif\" onclick=\"abrirListaEquipe(this.id,\'' || s.sercpf || '\');\" border=\"0\" />
            <img style=\"cursor: pointer; margin-left:1px; \" align=\"absmiddle\" src=\"/imagens/excluir.gif\"  onclick=\"deletarFecheEquipe(\''||s.sercpf||'\')\" title=\"Excluir servidor da Chefia\">
            <img style=\"cursor: pointer; margin-left:1px; \" align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" onclick=\"abrirCadServidorEquipe(\''||s.sercpf||'\');\" title=\"Adicionar Servidor a Equipe\">
            <img style=\"cursor: pointer; margin-left:1px; \" align=\"absmiddle\" src=\"/imagens/alterar.gif\" onclick=\"abrirAlteraFecheEquipe(\''||s.sercpf||'\');\" title=\"Alterar Feche de Equipe\">
        </div>
    ";

    $equipe_sim = "
        <div id=\"servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"atualizaServidorEquipeSimNao(this);\" checked=\"checked\"> <br> <span style=\"color:green\"> Com Equipe </span> </div>
    ";

    $equipe_nao = "
        <div id=\"servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" id=\"servidor_'|| s.sercpf ||'\" name=\"servidor\" value=\"'|| s.sercpf ||'\" onclick=\"atualizaServidorEquipeSimNao(this);\"> <br> <span style=\"color:red\"> Sem Equipe </span> </div>
    ";
    
    $equipe_desab = "
        <div id=\"servidor_'|| s.sercpf ||'\"><input type=\"checkbox\" id=\"servidor_'|| s.sercpf ||'\" name=\"servidor\" value=\"'|| s.sercpf ||'\" disabled=\"disabled\"> <br> <span style=\"color:black\"> Disabilitado </span> </div>
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

                CASE WHEN( sertipogratificacao <> 'PS' AND '{$equipe_apoio}' = 'S' )
                    THEN
                        CASE WHEN s.sercomequipe = TRUE
                            THEN '{$equipe_sim}'
                            ELSE '{$equipe_nao}'
                        END
                    ELSE '{$equipe_desab}'
                END AS sercomequipe,

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
                END AS sertipogratificacao,

                '<tr style=\"display:none; \" id=\"listaEquipe_' || s.sercpf || '\" ><td id=\"tdE_' || s.sercpf || '\" colspan=8 ></td></tr>' as teste

        FROM gestaopessoa.servidor AS s

        JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid

        WHERE s.sercpf IS NOT NULL AND s.seranoreferencia = {$_SESSION['exercicio']} AND s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.",".SITUACAO_CEDIDO.",".SITUACAO_EXCEDENTE.",".SITUACAO_ATIVO_PERM_L.",".SITUACAO_ANISTIADO.",".SITUACAO_EXERC.",".CLT_ANS_DEC_6657_08.",".NOMEADO_CARGO_COMIS.",".EXERC_DESCENT_CARREI.",".REQUISITADO.",".REQ_DE_OUTROS_ORGAOS.")
        AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) <> 0

        $where

        ORDER BY s.sernome
    ";//ver($sql, d);
    $cabecalho = array("Ação", "SIAPE", "CPF", "Nome", "Cargo", "Equipe", "Situação", "Gratificação");
    $alinhamento = Array('left', 'left', 'left', 'left', 'left', 'center', 'left', 'center');
    $tamanho = Array('5%', '10%', '10%', '30%', '15%', '10%', '15%', '7%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>