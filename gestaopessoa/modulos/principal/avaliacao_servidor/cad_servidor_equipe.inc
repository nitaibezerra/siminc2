<?php
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    echo "<br>";
    monta_titulo('Gestão Pessoas - Avaliação de Servidores', 'Listagem de Servidores');
    echo "<br>";

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

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
    
    function abrirAvaliacaoServidor( sercpf ){
        $.ajax({
            type: "POST",
            url : window.location.href,
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
    
    function atualizaServidorEquipeSimNao( obj ) {

        var status = obj.checked;
        var cpf = obj.value;

        $.ajax({
            type: "POST",
            url : window.location.href,
            data: "requisicao=atualizaServidorEquipeSimNao&status="+status+"&cpf="+cpf,
            asynchronous: false,
            success: function( msg ){ 
                var resp = pegaRetornoAjax('<resp>', '</resp>', msg, true);
                $('#servidor_'+cpf).html(resp);
            }
        });
    }
    
    function salvarServidorEquipe( obj ){
        var sercpfchefe = $('#sercpfchefe').val();
        var sercpf = obj.value;
        
        $.ajax({
            type: "POST",
            url : window.location.href,
            data: "requisicao=salvarServidorEquipe&sercpfchefe="+sercpfchefe+"&sercpf="+sercpf,
            asynchronous: false,
            success: function( resp ){ 
                var resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                if(resp == 'OK'){
                    divCarregando();
                    var confirma = confirm("Operação realizada com sucesso!\nDeseja inserir um novo membro a equipe?");

                    if( confirma ){
                        $('#formulario').submit();
                        divCarregado();
                    }else{
                        window.opener.location.reload();
                        this.close;
                    }
                }
            }
        });
    }
    
    function fecharJanela(){
        window.opener.location.reload();
        window.close();
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
<input type="hidden" id="sercpfchefe" name="sercpfchefe" value="<?= $_GET['sercpfchefe']; ?>"/>
    
    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="35%">Nome:</td>
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
            <td class="SubTituloCentro" colspan="2"> 
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
                <input type="button" name="fecharjanela" value="Fechar Janela" onclick="fecharJanela();"/>
            </td>
        </tr>
    </table>
</form>

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
    
    if ($_REQUEST['sercpfchefe']) {
        $sercpf = str_replace( '-', '', str_replace( '.', '', $_REQUEST['sercpfchefe'] ) );
        $where .= " AND s.sercpf <> '{$sercpf}' ";
    }
    
    $acao = "<input type=\"radio\" name=\"ogsid\" id=\"sercpf_'||s.sercpf||'\" value=\"'||s.sercpf||'\" onclick=\"salvarServidorEquipe(this);\">";
    
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
                END AS sertipogratificacao,
                
                '<span style=\"color:#1E90FF\">'||( SELECT DISTINCT sernome FROM gestaopessoa.servidor WHERE sercpf = s.sercpfchefe AND seranoreferencia = {$_SESSION['exercicio']} )||'</span>' as sercpfchefe

        FROM gestaopessoa.servidor AS s
        
        JOIN gestaopessoa.tiposituacaoservidor AS t ON t.tssid = s.tssid
        
        WHERE s.sercpf IS NOT NULL AND s.seranoreferencia = {$_SESSION['exercicio']} AND s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.",".SITUACAO_CEDIDO.",".SITUACAO_EXCEDENTE.",".SITUACAO_ATIVO_PERM_L.",".SITUACAO_ANISTIADO.",".SITUACAO_EXERC.",".CLT_ANS_DEC_6657_08.",".NOMEADO_CARGO_COMIS.",".EXERC_DESCENT_CARREI.",".REQUISITADO.",".REQ_DE_OUTROS_ORGAOS.")
        AND (SELECT count(sercpf) FROM gestaopessoa.servidor WHERE sercpfchefe = s.sercpf AND seranoreferencia = s.seranoreferencia) = 0    
        
        $where    
            
        ORDER BY s.sernome        
    ";
    //ver($sql);
    $cabecalho = array("", "SIAPE", "CPF", "Nome", "Cargo", "Situação", "Gratificação", "Chefe" );  
    //$alinhamento = Array('center', 'right', 'right', 'left', 'left', 'center');
    //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>