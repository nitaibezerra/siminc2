<?php
    
    $_SESSION['gestao']['Aba'] = 'RETIFICACAO';
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    if($_GET['cprid'] != ''){
        $_SESSION['gestao']['cprid'] = $_GET['cprid'];
        $cprid = $_GET['cprid'];
        
        $dadosServ = dadosServidor( $cprid );
        
        if($dadosServ['cprsitestagio'] == 't'){
            $checkSim_est = 'checked = "checked"';
        }else{
            $checkNao_est = 'checked = "checked"';
        }
        if($dadosServ['cprsindpad'] == 't'){
            $checkSim_sind = 'checked = "checked"';
        }else{
            $checkNao_sind = 'checked = "checked"'; 
        }
        if($dadosServ['cprsitdedicacao'] == 't'){
            $checkSim_ded = 'checked = "checked"';
        }else{
            $checkNao_ded = 'checked = "checked"';
        }
        
        if($dadosServ['columncprprazoind'] == 't'){
            $checkSim_ind = 'checked = "checked"';
        }else{
            $checkSim_ind = '';
        }
        
        if($dadosServ['columncprprazoano'] == 't'){
            $checkSim_ano = 'checked = "checked"';
        }else{
            $checkSim_ano = '';
        }
        
        if($dadosServ['columncprprazodata'] != ''){
            $checkSim_dat = 'checked = "checked"';
        }else{
            $checkSim_dat = '';
        }        
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Gestão de Pessoas', '<b>CESSÃO - Cadastro da Retificação</b>');
    echo "<br>";
    
    $abacod_tela    = ABA_CAD_REITERACAO_SERVIDOR;
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_retificacao&acao=A';
    //$parametros     = '&cprid='.$cprid;
    $parametros     = '';
    
    $db->cria_aba($abacod_tela, $url, $parametros);
?>
<!--
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
-->
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    
    $(document).ready(function(){
        $('#salvarProcesso').click(function(){
            var orcid                   = $('#orcid');
            var cprnumprocesso          = $('#cprnumprocesso');
            var tpsid                   = $('#tpsid');
            var ogcid                   = $('#ogcid');
            var cprfuncaodesempenhada   = $('#cprfuncaodesempenhada');
            var cprcodigosimbolo        = $('#cprcodigosimbolo');
            var tonid                   = $('#tonid');
            var cprsitestagio           = $('input:radio[name=cprsitestagio]:checked');
            var cprsindpad              = $('input:radio[name=cprsindpad]:checked');
            var cprsitdedicacao         = $('input:radio[name=cprsitdedicacao]:checked');
            var cprdtinlusao            = $('#cprdtinlusao');
            var aimid                   = $('#aimid');
            
            var columncprprazo          = $('input:radio[name=columncprprazo]:checked');
            var columncprprazodata      = $('#columncprprazodata');
            
            var erro;

            if(!orcid.val()){
                alert('O campo "Unidade de Cadastro" é um campo obrigatório!');
                orcid.focus();
                erro = 1;
                return false;
            }
            if(!aimid.val()){
                alert('O campo "Unidade de MEC" é um campo obrigatório!');
                aimid.focus();
                erro = 1;
                return false;
            }
            if(!cprnumprocesso.val()){
                alert('O campo "O Nº do Processo" é um campo obrigatório!');
                cprnumprocesso.focus();
                erro = 1;
                return false;
            }
            if(!tpsid.val()){
                alert('O campo "Tipo de Solicitação" é um campo obrigatório!');
                tpsid.focus();
                erro = 1;
                return false;
            }
            if(!ogcid.val()){
                alert('O campo "Orgão de Cessionário" é um campo obrigatório!');
                ogcid.focus();
                erro = 1;
                return false;
            }
            if(!cprfuncaodesempenhada.val()){
                alert('O campo "Função a ser Desempenhada" é um campo obrigatório!');
                cprfuncaodesempenhada.focus();
                erro = 1;
                return false;
            }
            if( !columncprprazo.val() ){
                alert('O campo "Prazo" é um campo obrigatório!');
                columncprprazo.focus();
                erro = 1;
                return false;
            }
            if( columncprprazo.val() == 'D' && !columncprprazodata.val() ){
                alert('O campo "Prazo - Data" é um campo obrigatório!');
                columncprprazodata.focus();
                erro = 1;
                return false;
            }
            if(!cprcodigosimbolo.val()){
                alert('O campo "Código/Simbolo" é um campo obrigatório!');
                cprcodigosimbolo.focus();
                erro = 1;
                return false;
            } 
            if(!tonid.val()){
                alert('O campo "Ônus" é um campo obrigatório!');
                tonid.focus();
                erro = 1;
                return false;
            } 
            if(!cprsitestagio.val()){
                alert('O campo "Estágio Probatório" é um campo obrigatório!');
                cprsitestagio.focus();
                erro = 1;
                return false;
            } 
            if(!cprsindpad.val()){
                alert('O campo "Sindicância e PAD" é um campo obrigatório!');
                cprsindpad.focus();
                erro = 1;
                return false;
            } 
            if(!cprsitdedicacao.val()){
                alert('O campo "Dedicação Exclusiva" é um campo obrigatório!');
                cprsitdedicacao.focus();
                erro = 1;
                return false;
            } 
            if(!cprdtinlusao.val()){
                alert('O campo "Data do Lançamento" é um campo obrigatório!');
                cprdtinlusao.focus();
                erro = 1;
                return false;
            } 
            
            if(!erro){
                $('#requisicao').val('salvarDadosRetificacao');
                $('#formulario').submit();
            }
        });
    });
    
    function abrirPopUpCadServidorSiape( nu_cpf ){
        var cprid = $('#cprid').val();
        var busca;
        if(nu_cpf != ''){
            busca = '&busca=S&nu_cpf='+nu_cpf;
        }else{
            busca = '';
        }
        window.open('gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_servidor_siape&acao=A&cprid='+cprid+busca,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=850, height=500');
    }    

    function abrirPopUpServidor(){
        var fdpcpf = replaceAll( replaceAll( $('#fdpcpf').val(), ".", "" ), "-", "" );
        
        if( fdpcpf == ''){
            alert('Para ver detalhes do Servidor é necessário digitar o CPF.');
            $('#fdpcpf').focus();
            return false;
        }
        
        window.open('gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A&pop=sim&fdpcpf='+fdpcpf,'','toolbar=no, location=no, status=yes, menubar=no, scrollbars=yes, resizable=no, width=900, height=600');
    }

    function atualizaComboUndCad( tpsid ){ 
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboUndCad&tpsid="+tpsid,
            async: false,
            success: function(resp){
                $('#unidadeCadastro').html(resp);
            }
        });
    }

    function buscarDadosServidor( nu_matricula_siape, nu_cpf ){
    
        var param;
        if(nu_matricula_siape != ''){
            param = 'nu_matricula_siape='+nu_matricula_siape;
        }else{
            param = 'nu_cpf='+nu_cpf;
        }
        
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosServidor&"+param,
            //asynchronous: false,
            success: function(resp){
                var dados = $.parseJSON(resp);             
                
                if(dados.nu_matricula_siape != '0'){
                    $('#nu_cpf').val(dados.nu_cpf);
                    $('#no_servidor').val(dados.no_servidor);
                    $('#co_orgao').val(dados.co_orgao);
                    $('#co_situacao_servidor').val(dados.co_situacao_servidor);
                    $('#co_funcao').val(dados.co_funcao);
                    $('#co_cargo_emprego').val(dados.co_cargo_emprego);
                }else{
                    $('#nu_cpf').val('');
                    $('#no_servidor').val('');
                    $('#co_orgao').val('');
                    $('#co_situacao_servidor').val('');
                    $('#co_funcao').val('');
                    $('#co_cargo_emprego').val('');
                    $('#addServidor').attr('disabled', false);
                    alert('O SIAPE não foi encontrado em nossa base de dados, se não foi digitado de forma incorreta é necessario cadastrá-lo!');
                }
            }
        });
    }

    function deletaProcessoCessaoProrrogacao(cprid){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=deletaDadosServidorCessao&cprid="+cprid,
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
    
    function mascaraPersonalizada( numContrato ){
        var numContrato = replaceAll( replaceAll( replaceAll( numContrato, ".", ""), "-", ""), "/","") ;
        var tamanho = numContrato.length;
        var contrato;
        
        if(numContrato != ''){
            if( tamanho == 12 ){
                contrato = mascaraglobal('######.####-##', numContrato);
            }else if( tamanho == 17 ){
                contrato = mascaraglobal('#####.######/####-##', numContrato);
            }else{
                alert('Número de Contrato Ivalido, verifique o número digitado!');
            }
        }
        $('#cprnumprocesso').val(contrato);
    }
    
    function voltarListaPessoa(){
        window.location.href = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_retificacao&acao=A';
    }
    
    function verificarDuplicidadeCPF( nu_cpf ){
        if(nu_cpf != ''){
            var valido = validar_cpf( nu_cpf );
        
            if(!valido){
                alert( "CPF inválido! Favor informar um CPF válido!" );
                $('#nu_cpf').focus();
                return false;
            }else{
               $.ajax({
                    type    : "POST",
                    url     : window.location,
                    data    : "requisicao=verificarDuplicidadeCPF&nu_cpf="+nu_cpf,
                    //asynchronous: false,
                    success: function(resp){
                        var dados = $.parseJSON(resp);
                        var qtd  = dados.qtd_cpf;

                        if( qtd == 0 ){
                            $('#nu_matricula_siape').val('');
                            $('#nu_cpf').val('');
                            $('#no_servidor').val('');
                            $('#co_orgao').val('');
                            $('#co_situacao_servidor').val('');
                            $('#co_funcao').val('');
                            $('#co_cargo_emprego').val('');
                            $('#addServidor').attr('disabled', false);
                            alert('O CPF digitado não foi encontrado em nossa base de dados é necessário cadastra-lo!');
                        }

                        if(qtd == 1){
                            buscarDadosServidor( '', nu_cpf );
                        }

                        if(qtd > 1){
                            abrirPopUpCadServidorSiape(nu_cpf);
                        }
                    }
                });
            }
        }
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="requisicao" name="requisicao" value=""/>
<input type="hidden" id="cprid" name="cprid" value="<?=$dadosServ['cprid'];?>"/>
 
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloCentro" style="text-align: left;" colspan="3" >Dados do Servidor</td>
            <!-- COLUNA DO WORKFLOW -->
            <td width="25%" rowspan="20" align="center" class ="SubTituloCentro">
                <?php
                    require_once APPRAIZ . "includes/workflow.php";

                    if($cprid != ''){
                        $docid = buscarDocidCessaoProrrogacao( $cprid );

                        if($docid != ''){
                            $dados_wf = array("cprid" => $cprid);
                            wf_desenhaBarraNavegacao($docid, $dados_wf);
                        }
                    }
                ?>
            </td>
            <!-- FIM COLUNA DO WORKFLOW -->        
        </tr>
        <!--INICIO INFORMAÇÃO SERVIDOR-->
        <tr>
            <td class ="SubTituloDireita" width="30%">Código SIAPE:</td>
            <td width="15%">
                <?php
                    if($_GET['siape']){
                        $nu_matricula_siape = $_GET['siape'];
                        echo '<script>buscarDadosServidor('.$_GET['siape'].', \'\')</script>';
                    }else{
                        $nu_matricula_siape = $dadosServ['nu_matricula_siape'];
                    }
                    echo campo_texto('nu_matricula_siape', 'S', $habilitado['hab'], '', 24, 7, '#######', '', '', '', 0, 'id="nu_matricula_siape"', '', $nu_matricula_siape, 'buscarDadosServidor(this.value, \'\')', null); 
                ?>
            </td>
            <td width="65%">
                <input type="button" name="addServidor" id="addServidor" onclick="abrirPopUpCadServidorSiape('');" value="Adicionar Servidor" title="Adicionar novo Servidor" disabled="disabled">
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">CPF:</td>
            <td>
                <?php
                    $nu_cpf = $dadosServ['nu_cpf'];
                    echo campo_texto('nu_cpf', 'S', $habilitado['hab'], '', 24, 14, '###.###.###-##', '', '', '', 0, 'id="nu_cpf"', '', $nu_cpf, 'verificarDuplicidadeCPF(this.value)', null); 
                ?>
            </td>
            <td width="65%">
                <!--input type="button" name="detServidor" id="detServidor" onclick="abrirPopUpServidor();" value="Detalhes Servidor" title="Consultar Detalhes do Servidor" disabled="disabled"-->
                <input type="button" name="detServidor" id="detServidor" value="Detalhes Servidor" title="Consultar Detalhes do Servidor" disabled="disabled">
                </span>
            </td>
        </tr>        
        <tr>
            <td class ="SubTituloDireita">Nome:</td>
            <td colspan="2">
                <?php
                    $no_servidor = trim($dadosServ['no_servidor']);
                    echo campo_texto('no_servidor', 'N', 'N', '', 45, 16, '', '', '', '', 0, 'id="no_servidor"', '', $no_servidor, null, null); 
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Sigla/Orgão:</td>
            <td colspan="2">
                <?php
                    $co_orgao = $dadosServ['co_orgao'];
                    $sql = "
                        SELECT  co_orgao AS codigo, 
                                --trim(sg_orgao) ||' - '||trim(ds_orgao) AS descricao
                                trim(ds_orgao) AS descricao
                        FROM siape.tb_simec_orgao
                    ";
                    $db->monta_combo('co_orgao', $sql, 'N', 'Selecione...', '', '', '', '336', 'S', 'co_orgao', false, $co_orgao, null);
                ?>	
            </td>
        </tr>
        <tr> 
            <td class ="SubTituloDireita">Situação:</td>
            <td colspan="2">
                <?php
                    $co_situacao_servidor = $dadosServ['co_situacao_servidor'];
                    $sql = "
                        SELECT  co_situacao_servidor AS codigo, 
                                ds_situacao_servidor AS descricao
                        FROM siape.tb_simec_situacao_servidor
                    ";
                    $db->monta_combo("co_situacao_servidor", $sql, 'N', 'Selecione...', '', '', '', '336', 'S', 'co_situacao_servidor', false, $co_situacao_servidor, null);
                ?>	
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Função:</td>
            <td colspan="2">
                <?php
                    $co_funcao = $dadosServ['co_funcao'];
                    $sql = "
                        SELECT  co_funcao AS codigo, 
                                ds_funcao AS descricao
                        FROM siape.tb_siape_funcao
                    ";
                    $db->monta_combo("co_funcao", $sql, 'N', 'Selecione...', '', '', '', '336', 'S', 'co_funcao', false, $co_funcao, null);
                ?>
            </td>
        </tr>
        <tr> 
            <td class ="SubTituloDireita">Cargo:</td>
            <td colspan="2">
                <?php
                    $co_cargo_emprego = $dadosServ['co_cargo_emprego'];
                    $sql = "
                        SELECT  co_cargo_emprego AS codigo, 
                                ds_cargo_emprego AS descricao
                        FROM siape.tb_simec_cargo_emprego
                    ";
                    $db->monta_combo("co_cargo_emprego", $sql, 'N', 'Selecione...', '', '', '', '336', 'S', 'co_cargo_emprego', false, $co_cargo_emprego, null);
                ?>	
            </td>
        </tr>
        <!--FIM IMFORMAÇÃO SERVIDOR-->  
        <tr><td class ="SubTituloCentro" style="text-align: left;" colspan="3">Informação da Cessão/Retificação</td></tr>
        <tr>
            <td class ="SubTituloDireita">Tipo de Solicitação:</td>
            <td colspan="2">
                <?php
                    $tpsid = $dadosServ['tpsid'];
                    $sql = "
                        SELECT  tpsid AS codigo, 
                                tpsdsc AS descricao
                        FROM gestaopessoa.tiposolicitacao
                        WHERE tpsid IN (1, 2)
                    ";
                    $db->monta_combo("tpsid", $sql, 'S', 'Selecione...', 'atualizaComboUndCad', '', '', '336', 'S', 'tpsid', false, $tpsid, null);
                ?>	
            </td>
        </tr>
        <tr> 
            <td class ="SubTituloDireita">Unidade de Cadastro:</td>
            <td colspan="2"  id="unidadeCadastro">
                <?php
                    if($get_orcid == ''){
                        $orcid = $dadosServ['orcid'] ;
                    }else{
                        $orcid = $get_orcid;
                    }

                    $sql = "
                        SELECT  orcid AS codigo, 
                                orcdsc AS descricao
                        FROM gestaopessoa.origemcessao
                        WHERE orcid in (1, 3)
                        order by orcdsc
                    "; 
                    $db->monta_combo("orcid", $sql, 'S', 'Selecione...', '', '', '', '336', 'S', 'orcid', false, $orcid, null);
                ?>	
            </td>
        </tr> 
        <tr>
            <td class ="SubTituloDireita">Unidade MEC:</td>
            <td colspan="2">
                <?PHP
                    $aimid = $dadosServ['aimid'];   
                    $sql = "
                        SELECT  aimid AS codigo, 
                                aimsigla||' - '||aimdsc AS descricao
                        FROM gestaopessoa.areamec
                        order by aimdsc
                    ";
                    $db->monta_combo("aimid", $sql, 'S', 'Selecione...', '', '', '', '336', 'S', 'aimid', false, $aimid, null);                                                                                
                ?>
            </td>
        </tr>        
        <tr>
            <td class ="SubTituloDireita">Nº do processo:</td>
            <td colspan="2">
                <?php
                    $cprnumprocesso = $dadosServ['cprnumprocesso'];
                    echo campo_texto('cprnumprocesso', 'S', 'S', '', 45, 20, '', '', '', '', 0, 'id="cprnumprocesso"', '', $cprnumprocesso, 'mascaraPersonalizada(this.value)', null); 
                ?>
            </td>
        </tr>        
        <tr> 
            <td class ="SubTituloDireita">Õrgão Cessionário:</td>
            <td colspan="2">
                <?PHP
                    $sql = "
                        SELECT  ogcid AS codigo, 
                                ogcsigla||' - '||ogcdsc AS descricao
                        FROM gestaopessoa.orgaocessionario
                        WHERE 1=1
                    ";
                    $arCampos = array(
                        array(
                            "codigo" => "ogcid",
                            "descricao" => "<b>Código:</b>",
                            "tipo" => "1"),
                        array(
                            "codigo" => "ogcdsc",
                            "descricao" => "<b>Descrição:</b>",
                            "tipo" => "0")
                     );
                    
                    $ogcid = array(
                            "value" => $dadosServ['ogcid'],
                            "descricao" => $dadosServ['ogcdsc']
                    );
                    campo_popup('ogcid', $sql, 'Selecione Órgão Cessionário', null, '400x800', '44', $arCampos, 1, false, false, false, 'N');
                                                                                
                ?>                
                
            </td>
        </tr>        
        <tr> 
            <td class ="SubTituloDireita">Função a ser Desempenhada:</td>
            <td colspan="2">
                <?php
                    $cprfuncaodesempenhada = $dadosServ['cprfuncaodesempenhada'];
                    echo campo_textarea('cprfuncaodesempenhada', 'S', 'S', '', 68, 6, '100', '', 0, '', false, null, $cprfuncaodesempenhada); 
                ?>	
            </td>
        </tr>        
        <tr>
            <td class ="SubTituloDireita">Prazo:</td>
            <td colspan="2">
                <input type="radio" name="columncprprazo" value="I" <?=$checkSim_ind?>> <b>Indeterminado</b> <br>
                <input type="radio" name="columncprprazo" value="U" <?=$checkSim_ano?>> <b>(1) Ano</b> <br>
                <input type="radio" name="columncprprazo" value="D" <?=$checkSim_dat?>>
                <?php
                    $columncprprazodata = $dadosServ['columncprprazodata'];
                    echo campo_data2('columncprprazodata','N','S','','DD/MM/YYYY','','', $columncprprazodata, '', '', 'columncprprazodata' );
                ?>
            </td>
        </tr>        
        <tr> 
            <td class ="SubTituloDireita">Código/Simbolo:</td>
            <td colspan="2">
                <?php
                    $cprcodigosimbolo = $dadosServ['cprcodigosimbolo'];
                    echo campo_texto('cprcodigosimbolo', 'S', 'S', '', 45, 20, '', '', '', '', 0, 'id="cprcodigosimbolo"', '', $cprcodigosimbolo, null, null);
                ?>	
            </td>
        </tr>
        <tr> 
            <td class ="SubTituloDireita">Ônus:</td>
            <td colspan="2">
                <?php
                    $tonid = $dadosServ['tonid'];
                    $sql = "
                        SELECT  tonid AS codigo, 
                                tondsc AS descricao
                        FROM gestaopessoa.tipoonus
                    ";
                    $db->monta_combo("tonid", $sql, 'S', 'Selecione...', '', '', '', '336', 'S', 'tonid', false, $tonid, null);
                ?>	
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Estágio Probatório:</td>
            <td colspan="2">
                <input type="radio" name="cprsitestagio" id="cprsitestagio_S" value="S" <?=$checkSim_est;?>> Sim
                <input type="radio" name="cprsitestagio" id="cprsitestagio_N" value="N" <?=$checkNao_est;?>> Não
            </td>
        </tr>       
        <tr>
            <td class ="SubTituloDireita">Sindicância e PAD:</td>
            <td colspan="2">
                <input type="radio" name="cprsindpad" id="cprsindpad_S" value="S" <?=$checkSim_sind;?>> Sim
                <input type="radio" name="cprsindpad" id="cprsindpad_N" value="N" <?=$checkNao_sind;?>> Não
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Dedicação Exclusiva:</td>
            <td colspan="2">
                <input type="radio" name="cprsitdedicacao" id="cprsitdedicacao_S" value="S" <?=$checkSim_ded;?>> Sim
                <input type="radio" name="cprsitdedicacao" id="cprsitdedicacao_N" value="N" <?=$checkNao_ded;?>> Não
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Data do Lançamento:</td>
            <td colspan="2">
                <?php
                    $cprdtinlusao = $dadosServ['cprdtinlusao'] != '' ? $dadosServ['cprdtinlusao'] : date("d/m/Y");
                    echo campo_data2('cprdtinlusao','N','N','','DD/MM/YYYY','','', $cprdtinlusao, '', '', 'cprdtinlusao' );
                ?>
            </td>
        </tr> 
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" id="salvarProcesso" name="salvar" value="Salvar"/>
                <input type="button" id="voltar" name="voltar" value="Voltar" onclick="voltarListaPessoa();"/>
            </td>
        </tr>
    </table>
</form>

