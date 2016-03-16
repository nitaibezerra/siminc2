<?php
$perfis = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC);
if(!possuiPerfil($perfis)){
    echo '<script > 
            alert(\'Você não possui permissão para acessar essa tela.\');
            window.hostory(-1);
          </script>';
}

if($_REQUEST['requisicao'] == 'download' || $_REQUEST['requisicaoAnalise'] == 'download'){
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->downloadArquivoTarefa($_REQUEST['arqid']);
    die();
}

if($_REQUEST['requisicao'] == 'espelhoTarefa'){
    $itmid = $_REQUEST['itmid'];
    mostraEspelhoTarefa($itmid);
    die();
}

switch ( $_POST['requisicao'] ){
	case 'salvar':
            $perfis = array(PFLCOD_SUPER_USUARIO);
            if (!possuiPerfil($perfis)) {
                echo '<script > 
                        alert(\'Você não possui permissão para alterar os dados.\');
                        window.history(-1);
                      </script>';
                die();
            }
            $objMonitoramento = new MonitoramentoEspecial();
            $itmid            = $objMonitoramento->cadastrarDadosTarefas($_POST);
            if(empty($itmid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao cadastrar o Tarefa de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                        window.history(-1);
                      </script>';
                die();
            }else{
                $docid = $objMonitoramento->atualizaDocidTarefa($itmid);
                if(empty($docid)){
                    echo '<script > 
                            alert(\'Ocorreu um erro ao cadastrar o Fluxo do Tarefa de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                            window.history(-1);
                          </script>';
                    die();
                }
            }
            
            $atmid = $_POST['atmid'];
            echo "<script > 
                    alert('Dados cadastrados com sucesso.');
                    window.location = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid=".$atmid."';
                  </script>";
            die();
            break;
	case 'editar':
            $perfis = array(PFLCOD_SUPER_USUARIO);
            if (!possuiPerfil($perfis)) {
                echo '<script > 
                        alert(\'Você não possui permissão para alterar os dados.\');
                        window.history(-1);
                      </script>';
                die();
            }
            $objMonitoramento = new MonitoramentoEspecial();
            $itmid            = $objMonitoramento->editarDadosTarefa($_POST);
            
            if($itmid){
                $msg = 'Tarefa de Monitoramento editado com sucesso.';
            }else{
                $msg = 'Erro ao editar o Tarefa de Monitoramento.';
            }
            
            $atmid = $_POST['atmid'];
            echo "<script > 
                    alert('".$msg."');
                    window.location = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid=".$atmid."';
                  </script>";
            die();
            break;
	case 'excluir':
            $perfis = array(PFLCOD_SUPER_USUARIO);
            if (!possuiPerfil($perfis)) {
                echo '<script > 
                        alert(\'Você não possui permissão para alterar os dados.\');
                        window.history(-1);
                      </script>';
                die();
            }
            $itmid = $_POST['itmid'];
            $objMonitoramento = new MonitoramentoEspecial();
            $itmid            = $objMonitoramento->excluirTarefa($itmid);
            if($itmid){
                echo 'Tarefa de Monitoramento excluída com sucesso.';
            }else{
                echo 'Erro ao excluir o Tarefa de Monitoramento.';
            }
            die();
            break;
        case 'verificaAtmid':
            $atmid = $_POST['atmid'];
            $objMonitoramento = new MonitoramentoEspecial();
            $resp = $objMonitoramento->verificaAtmid($atmid);
            if(!empty($resp)){
                echo $resp;
            }else{
                echo 'false';
            }
            die();
            break;
        case 'mostraDadosAtividade':
            $atmid = $_POST['atmid'];
            $objMonitoramento = new MonitoramentoEspecial();
            $html = $objMonitoramento->mostraDadosAtividadeNoTarefa($atmid);
            echo $html;
            die();
            break;
        case 'salvarAnaliseTarefa':
            $perfis = array(PFLCOD_SUPER_USUARIO);
            if (!possuiPerfil($perfis)) {
                echo '<script > 
                        alert(\'Você não possui permissão para alterar os dados.\');
                        window.history(-1);
                      </script>';
                die();
            }
            
            $params = $_POST;
            $objMonitoramento = new MonitoramentoEspecial();
            
            if(!empty($_POST['anmid'])){
                $anmid = $objMonitoramento->editarDadosAnaliseTarefa($params);
            }else{
                $anmid = $objMonitoramento->cadastrarDadosAnaliseTarefa($params);
            }
            
            if($params['anmrespostaconclusao'] == 'N'){
                $dadosTarefa = $objMonitoramento->getDadosTarefas(array('itmid'=>$params['itmid']), 'array');
                if($dadosTarefa[0]['itmstatusrepetenaoconcluido'] == 'S'){
                    $respostaClonagem = $objMonitoramento->clonarTarefa($params['itmid']);
                }
            }
            if($anmid){
                echo "<script > 
                        alert('Dados salvos com sucesso.');
                        window.location = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&requisicao=espelhoTarefa&itmid=".$_REQUEST['itmid']."';
                      </script>";
            }else{
                echo "<script > 
                        alert('Ocorreu um erro ao salvar os dados de Análise.');
                        window.location.href = window.location.href;
                      </script>";
                
            }
            
            die();
            break;
        case 'excluirAnalise':
            $perfis = array(PFLCOD_SUPER_USUARIO);
            if (!possuiPerfil($perfis)) {
                echo '<script > 
                        alert(\'Você não possui permissão para alterar os dados.\');
                        window.history(-1);
                      </script>';
                die();
            }
            $anmid = $_POST['anmid'];
            $objMonitoramento = new MonitoramentoEspecial();
            $itmid            = $objMonitoramento->excluirDadosAnaliseTarefa($anmid);
            if($itmid){
                echo 'Análise da Tarefa de Monitoramento excluída com sucesso.';
            }else{
                echo 'Erro ao excluir o Análise da Tarefa de Monitoramento.';
            }
            die();
            break;
        default :
            break;
            
}

if(!empty($_REQUEST['itmid']) || !empty($_SESSION['obras2']['itmid'])){
    $objMonitoramento = new MonitoramentoEspecial();
    $campos = " itm.*, resp.*, usu_resp_ent.entid as usu_resp_entid";
    
    $itmid            = !empty($_REQUEST['itmid']) ? $_REQUEST['itmid'] : $_SESSION['obras2']['itmid'];
    $dadosTarefa        = $objMonitoramento->getDadosTarefas(array('itmid'=>$itmid),'array', $campos);
    $dadosTarefa        = $dadosTarefa[0];
    $itmid            = $dadosTarefa['itmid'];
    $atmid            = $dadosTarefa['atmid'];
    
    $_SESSION['obras2']['itmid'] = $itmid;
    $_SESSION['obras2']['atmid'] = $atmid;
}else{
    $dadosTarefa = array();
    $itmid     = '';
    unset($_SESSION['obras2']['itmid']);
}


include APPRAIZ . "includes/cabecalho.inc";

$arMenuBlock = array();
$id_aba           = ID_ABA_MONITORAMENTO_ESPECIAL;
$db->cria_aba($id_aba, $url, $parametros, $arMenuBlock);
$titulo_modulo    = 'Cadastro de Tarefas de Monitoramento';
$subtitulo_modulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';
monta_titulo($titulo_modulo, $subtitulo_modulo);

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script language="JavaScript">
    
    function cancelarCadastro(){
        window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaTarefaMonitoramento&acao=A';
    }
    
    function voltarParaAtividade(atmid){
        if(atmid != null && atmid != ''){
            window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid='+atmid;
        }else{
            window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaAtividadeMonitoramento&acao=A';
        }
    }
    
    function fechaJanela(){
        var url_opener  = window.opener.location.href;
        var verificador = url_opener.indexOf("listaRestricao&acao=L");
        if(verificador == -1){
            window.opener.location.reload(); 
        }
        window.close();
    }
    
    function toogleLinhaAnexo(acao){
        if(acao == 'show'){
            $('#linha_anexos1').show();
            $('#linha_anexos2').show();
        }else if(acao == 'hide'){
            $('#linha_anexos1').hide();
            $('#linha_anexos2').hide();
        }
    }
    
    function toogleLinhaDiasRepeticao(acao){
        if(acao == 'show'){
            $('#linha_dias_repeticao').show();
        }else if(acao == 'hide'){
            $('#linha_dias_repeticao').hide();
        }
    }
    
    function inserirResponsavel(entid) {
        var caminho_atual = '/obras2/obras2.php';
        if (entid) {
            return windowOpen(caminho_atual + '?modulo=principal/monitoramentoespecial/inserir_responsavel&acao=A&busca=entnumcpfcnpj&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen(caminho_atual + '?modulo=principal/monitoramentoespecial/inserir_responsavel&acao=A', 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }
    
    function onOffCampo(campo) {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
    
    function verificaExistenciaAtmid(atmid){
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: {atmid: atmid, requisicao: 'verificaAtmid'},
            dataType: "html",
            async: false,
            success: function(data) {
                if(data == false || data == 'false'){
                    alert('Essa atividade não existe. Informe o ID correto da Atividade.');
                    $('#atmid').val('');
                    $('#atmid').focus();
                    $('#dados_atividade').html('');
                    return false;
                }else{
                    mostraDadosAtividade($('#atmid').val());
                    return data;
                }
            }
        });
    }
    
    function mostraDivHistoricoResponsaveis(){
        $( "#historico_responsaveis" ).toggle();
    }
    
    function mostraDadosAtividade(atmid){
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: {atmid: atmid, requisicao: 'mostraDadosAtividade'},
            dataType: "html",
            async: false,
            success: function(html) {
                $('#dados_atividade').html(html);
            }
        });
    }
    
    function download(arqid){
        var req = $('#requisicao').val();
        $('#requisicao').val('download');
        $('#arqid').val(arqid);
        $('#formulario').submit();
        $('#requisicao').val(req);
    }
    
    $(document).ready(function(){
        
        $('#atmid').focusout(function(){
            verificaExistenciaAtmid(this.value);            
        });
        
        var bloqueiaForm = <?php if($_REQUEST['bloqkEdicao']) { echo 'true'; } else { echo 'false';} ?>;
        if(bloqueiaForm === true){
            $('#formulario').find('input, textarea, button, select').attr('disabled','disabled');
            $('#formulario').find('a').attr('onclick','alert("Você não pode mudar o estado da Atividade")');
            $('#cancelar').attr('disabled', false);
        }
        
        if($('[name=itmcoletaranexostatus]:checked').val() == 'S'){
            toogleLinhaAnexo('show');
        }
        
        if($('[name=itmstatusrepetenaoconcluido]:checked').val() == 'S'){
            toogleLinhaDiasRepeticao('show');
        }
        
        $('[name=itmqtddiasstatusrepeticao]').click(function(){
            if($('[name=itmqtddiasstatusrepeticao]:checked').val() == ''){
                $('#especificar_dias').show();
            }else{
                $('#especificar_dias').hide();
            }
        });
        
        <?php
            if($dadosTarefa['atmid'] != ''){
                echo 'mostraDadosAtividade('.$dadosTarefa['atmid'].');';
            }
        ?>
        
    });
    
</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">

<form method="post" id="formulario" name="formulario" action="" enctype="multipart/form-data">
        <?php if(!empty($itmid)){ $acaoform = 'editar'; }else{ $acaoform = 'salvar'; } ?>
        <input type="hidden" name="requisicao" id="requisicao" value="<?php echo $acaoform;?>"/>
        <input type="hidden" name="itmid"      id="itmid"      value='<?php echo $itmid;?>'/>
        <input type="hidden" name="arqid"      id="arqid"      value=''/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
            <tr>
                <td class="subtituloDireita" style="width: 40%;">ID da Atividade de Monitoramento:</td>
                <td colspan="2">
                    <?php
                        if(!empty($dadosTarefa['itmid'])){
                            $atmid   = $dadosTarefa['atmid'];
                            $habil = 'S';
                        }elseif(!empty($_GET['atmid'])){
                            $atmid   = $_GET['atmid'];
                            $habil = 'N';
                        }elseif(!empty($_POST['atmid'])){
                            $atmid   = $_POST['atmid'];
                            $habil = 'S';
                        }else{
                            $atmid   = '';
                            $habil = 'S';
                        }
                        echo campo_texto('atmid', 'S', $habil, '', 70, 80, '######', '', '', '', '', 'id="atmid"', '', $atmid);
                        if(!empty($atmid)){ echo '<script language="JavaScript"> $(document).ready(function(){ verificaExistenciaAtmid('.$atmid.'); }); </script>'; }
                    ?>
                    <br>
                    
                    <span id="dados_atividade" style="width: 400px"></span>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Nome da Tarefa:</td>
                <td>
                    <?php
                        $val = (!empty($dadosTarefa['itmnome'])) ? stripslashes($dadosTarefa['itmnome']) : '';
                        echo campo_texto('itmnome', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="itmnome"', '', $val);
                    ?>
                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                </td>
                <td rowspan="5"  align="right" valign="top" width="1">
                    <?php
                        $docid = $dadosTarefa['docid'];
                        if($docid){
                            wf_desenhaBarraNavegacao($docid, array('itmid' =>  $itmid));
                        }else{
                            echo '&nbsp;';
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Coletar Anexo de Evidência ?</td>
                <td colspan="2">
                    <?php
                    $select_s = '';
                    $select_n = '';
                    switch ($dadosTarefa['itmcoletaranexostatus']) {
                        case 'S':
                            $select_s = 'checked="checked"';
                            break;
                        case 'N':
                            $select_n = 'checked="checked"';
                            break;
                        default :
                            $select_n = 'checked="checked"';
                            break;
                    }
                ?>
                    <input type="radio" name="itmcoletaranexostatus" id="itmcoletaranexostatus_s" value="S" <?php echo $select_s; ?> onclick="toogleLinhaAnexo('show')" > Sim
                    <input type="radio" name="itmcoletaranexostatus" id="itmcoletaranexostatus_n" value="N" <?php echo $select_n; ?> onclick="toogleLinhaAnexo('hide')" > Não
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr id="linha_anexos1" style="display: none;">
                <td class="SubTituloDireita" width="20%">Nome do Anexo:</td>
                <td colspan="2">
                    <?php 
                        $val = (!empty($dadosTarefa['itmnomeanexo'])) ? $dadosTarefa['itmnomeanexo'] : '';
                        echo campo_texto('itmnomeanexo','S','S','',43,100,'','', '', '', '', 'id="itmnomeanexo"', '',$val);
                        if(!empty($dadosTarefa['arqidanexo'])){
                            echo '<span onclick="download('.$dadosTarefa['arqidanexo'].')" style="cursor: pointer; vertical-align:middle" title="Baixar Arquivo">';
                            echo '<img align="absmiddle" src="/imagens/icone_lupa.png" style="cursor: pointer; vertical-align:middle" id="'.$dadosTarefa['arqidanexo'].'" title="Baixar Arquivo"> ' ;
                            echo $dadosTarefa['itmnomeanexo'] ;
                            echo '</span>' ;
                        }
                    ?>
                </td>
            </tr>

            <tr id="linha_anexos2" style="display: none;">
                <td class="SubTituloDireita">Arquivo:</td>
                <td colspan="2">
                    <input type="file" name="arqidanexo" />
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Data Limite:</td>
                <td colspan="2">
                  <?php 
                    if(empty($dadosTarefa['itmdtlimiteconclusao'])){
                        $data = ''; 
                      }else{
                        $data = date_create($dadosTarefa['itmdtlimiteconclusao']); 
                        $data = date_format ( $data, 'd/m/Y' );
                      }
                  ?>
                  <input type="text" id="itmdtlimiteconclusao" name="itmdtlimiteconclusao" value="<?php echo $data;?>" size="15" maxlength="10" class="normal  obrigatorio "> 
                  <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Repetir a Tarefa automaticamente sem conclusão ?</td>
                <td colspan="2">
                    <?php
                        $select_s = '';
                        $select_n = '';
                        switch ($dadosTarefa['itmstatusrepetenaoconcluido']) {
                            case 'S':
                                $select_s = 'checked="checked"';
                                break;
                            case 'N':
                                $select_n = 'checked="checked"';
                                break;
                            default :
                                $select_n = 'checked="checked"';
                                break;
                        }
                    ?>
                    <input type="radio" name="itmstatusrepetenaoconcluido" id="itmstatusrepetenaoconcluido_s" value="S" <?php echo $select_s; ?> onclick="toogleLinhaDiasRepeticao('show')" > Sim
                    <input type="radio" name="itmstatusrepetenaoconcluido" id="itmstatusrepetenaoconcluido_n" value="N" <?php echo $select_n; ?> onclick="toogleLinhaDiasRepeticao('hide')" > Não
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr id="linha_dias_repeticao" style="display: none;">
                <td class="SubTituloDireita">Prazo em dias para repetição:</td>
                <td colspan="2">
                    <?php
                        $select_01 = '';
                        $select_07 = '';
                        $select_10 = '';
                        $select_30 = '';
                        $select_60 = '';
                        $select_90 = '';
                        $select_espec = '';
                        $select_especificado = '';
                        $display = ' style="display: none" ';
                        switch ($dadosTarefa['itmqtddiasstatusrepeticao']) {
                            case '1':
                                $select_01 = 'checked="checked"';
                                break;
                            case '7':
                                $select_07 = 'checked="checked"';
                                break;
                            case '10':
                                $select_10 = 'checked="checked"';
                                break;
                            case '30':
                                $select_30 = 'checked="checked"';
                                break;
                            case '60':
                                $select_60 = 'checked="checked"';
                                break;
                            case '90':
                                $select_90 = 'checked="checked"';
                                break;
                            default :
                                $select_espec = 'checked="checked"';
                                $select_especificado = $dadosTarefa['itmqtddiasstatusrepeticao'];
                                $display = ' style="display: inline" ';
                                break;
                        }
                    ?>
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_01" value="1"  <?php echo $select_01; ?> > 1  &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_07" value="7"  <?php echo $select_07; ?> > 7  &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_10" value="10" <?php echo $select_10; ?> > 10 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_30" value="30" <?php echo $select_30; ?> > 30 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_60" value="60" <?php echo $select_60; ?> > 60 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_90" value="90" <?php echo $select_90; ?> > 90 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_espec" value="" <?php echo $select_espec; ?> > Especificar &nbsp;&nbsp;
                    <br />
                    <span id="especificar_dias" <?php echo $display;?>>
                        <br />Especificar o número de dias:
                        <?php echo campo_texto('itmqtddiasstatusrepeticao_especificado','S','S','',43,100,'####','', '', '', '', 'id="itmnomeanexo"', '', $select_especificado); ?>
                    </span>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Nome do Responsável:</td>
                <td colspan="2">
                    <?php
                        $usucpf_reponsavel  = (!empty($dadosTarefa)) ? $dadosTarefa['usu_resp_entid']: '';
                        if(empty($usucpf_reponsavel)){
                            $entidresponsavel = '';
                        }else{
                            $responsavel        = new Entidade($usucpf_reponsavel);
                            $entnomeresponsavel = $responsavel->entnome;
                            $entidresponsavel   = $dadosTarefa['usu_resp_entid'];
                        }
                    ?>
                    <span id="entnomeresponsavel"><?php echo $entnomeresponsavel; ?></span>
                    <input type="hidden" name="entidresponsavel" id="entidresponsavel" value="<?php echo $entidresponsavel; ?>">
                    <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirResponsavel(document.getElementById('entidresponsavel').value);"/>
                    <?php 
                        print obrigatorio(); 
                        $objMon= new MonitoramentoEspecial();
                        $html = $objMon->getHistoricoResponsaveis($itmid);
                        if(!empty($html)){
                            echo '<br>';
                            echo '<a href="#" onclick="mostraDivHistoricoResponsaveis()" id="link_historico">Histórico de Responsáveis</a>';
                            echo '<div id="historico_responsaveis" style="display: none">';
                            echo $html;
                            echo '</div>';
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Observações:</td>
                <td colspan="2">
                    <?php 
                        $value = empty($dadosTarefa['itmobs']) ? '' : stripslashes($dadosTarefa['itmobs']);
                        echo campo_textarea("itmobs",'N','S','', 70, 8, 500, '', 0, '', false, NULL, $value);
                    ?>
                </td>
            </tr>
            <tr bgcolor="#C0C0C0" style="text-align: center">
                <td colspan="3">
                    <input type='button' class='botao' name='Salvar' value='Salvar' onclick="validar()"/>
                    <input type='button' class='botao' name='Cancelar' value='Cancelar' onclick="cancelarCadastro()" id='cancelar'/>
                    <input type='button' class='botao' name='voltar' value='Voltar para Atividade' onclick="voltarParaAtividade(<?php echo $atmid;?>)" id='voltar'/>
                </td>
            </tr>
	</table>
</form>

<script type="text/javascript">
    
    setTimeout(function(){
        jQuery('#itmdtlimiteconclusao').mask('99/99/9999');

        jQuery("#itmdtlimiteconclusao").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim:'drop'
        });
        
    }, 500);
    
    function validar() {

            var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
            var validacao = true;

            var atmid   = $('#atmid').val();
            if (atmid == '') {
                mensagem += 'O campo de ID da Atividade deve ser preenchido.\n';
                validacao = false;
                $('#atmid').focus();
            }
            
            if(verificaExistenciaAtmid(atmid) == false){
                mensagem += 'O campo de ID da Atividade deve ser preenchido.\n';
                validacao = false;
                $('#atmid').val('');
                $('#atmid').focus();
            }
            
            var itmnome = $.trim($('#itmnome').val());
            if (itmnome == '') {
                mensagem += 'O campo de Nome do Tarefa deve ser preenchido.\n';
                validacao = false;
                $('#itmnome').focus();
            }
            
            var itmcoletaranexostatus = $('#itmcoletaranexostatus').val();
            if (itmcoletaranexostatus == '') {
                mensagem += 'O campo Coletar Anexo de Evidência deve ser preenchido.\n';
                validacao = false;
                $('#itmcoletaranexostatus').focus();
            }
            if (itmcoletaranexostatus == 'S') {
                var itmnomeanexo = $.trim($('#itmnomeanexo').val());
                if (itmnomeanexo == '') {
                    mensagem += 'O campo Nome do Anexo deve ser preenchido.\n';
                    validacao = false;
                    $('#itmnomeanexo').focus();
                }
                var arqidanexo   = $('#arqidanexo').val();
                if (arqidanexo == '') {
                    mensagem += 'O campo Nome do Anexo deve ser preenchido.\n';
                    validacao = false;
                    $('#arqidanexo').focus();
                }
            }
            
            var itmdtlimiteconclusao  = $('#itmdtlimiteconclusao').val();
            if (itmdtlimiteconclusao == '') {
                mensagem += 'O campo de Data Limite deve ser preenchido.\n';
                validacao = false;
                $('#itmdtlimiteconclusao').focus();
            }
            
            var itmstatusrepetenaoconcluido            = $('#itmstatusrepetenaoconcluido').val();
            if (itmstatusrepetenaoconcluido == '') {
                mensagem += 'O campo Repetir o Tarefa automaticamente sem conclusão deve ser preenchido.\n';
                validacao = false;
                $('#itmstatusrepetenaoconcluido').focus();
            }
            if (itmstatusrepetenaoconcluido == 'S') {
                var itmqtddiasstatusrepeticao              = $('#itmqtddiasstatusrepeticao').val();
                var itmqtddiasstatusrepeticao_especificado = $('#itmqtddiasstatusrepeticao_especificado').val();
                if(itmqtddiasstatusrepeticao_especificado == '' && itmqtddiasstatusrepeticao == ''){
                    mensagem += 'O campo Prazo em dias para repetição deve ser preenchido.\n';
                    validacao = false;
                    $('#itmqtddiasstatusrepeticao_especificado').focus();
                }
            }
            
            var entidresponsavel = $('#entidresponsavel').val();
            if (entidresponsavel == '') {
                mensagem += 'O campo Nome do Responsável deve ser preenchido.\n';
                validacao = false;
                $('#entidresponsavel').focus();
            }
            
            var itmobs           = $.trim($('#itmobs').val());            
            if(itmobs.length > 500){
                itmobs = itmobs.substr(0, 496)+'...';
                $('#itmobs').val(itmobs);
            }

            if (!validacao) {
                alert(mensagem);
                return false;
            } else {
                $('#formulario').submit();
            }
    }

</script>

</body>


<?php

function carrega_campo_unidade(){
	global $entid, $db;
        $uo_total = 0;
        if ( !empty($entid) ){
                $uo_total = $db->pegaUm( "SELECT
                                                count(u.unicod)
                                          FROM
                                                unidade u
                                          INNER JOIN
                                                entidade.entidade e ON
                                                u.orgcod = e.entorgcod
                                          WHERE
                                                unistatus='A' AND
                                                unitpocod='U' AND
                                                e.entid ='{$entid}'" );

        }

        if ( $uo_total > 0 ){

                $sql = "SELECT DISTINCT
                                        u.unicod AS codigo,
                                        u.unicod||' - '||unidsc AS descricao
                                FROM
                                        unidade u
                                INNER JOIN
                                        entidade.entidade e ON
                                        orgcod = entorgcod
                                WHERE
                                        unistatus='A' AND
                                        u.unitpocod='U' AND
                                        entid = '{$entid}'
                                ORDER BY
                                        u.unicod";
                echo $db->monta_combo("unicod",$sql,'S',"&nbsp;",'ajax_carrega_campo_unidade_gestora', '', '','','S','unicod');

        } else {
                echo '<font style="color:#909090;">Este órgão não possui uma unidade.</font>';
        }
                        
}

function carrega_campo_orgao(){

    global $muncod, $regcod, $tpocod, $db;

    // Caso o tipo seja municipal, verifica o municipio escolhido
    $inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN entidade.endereco eed ON eed.entid = ee.entid ' : '';

    $uniao = ( $tpocod == 3 || $tpocod == 2 ) ? " UNION ALL ( SELECT 999999 AS codigo, 'OUTROS' AS descricao )" : '';

    $clausula = '';
    
    if ($tpocod == 2) {
        $estuf = ($_REQUEST['estuf']) ? $_REQUEST['estuf'] : $estuf;
        $clausula = "AND eed.estuf = '" . $estuf . "'";
    } elseif ($tpocod == 3) {
        $clausula = "AND eed.muncod = '" . $muncod . "'";
    }

    $sql = "(SELECT
                    ee.entid AS codigo,
                    CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
                    ELSE ee.entnome END AS descricao
             FROM
                    entidade.entidade ee
             INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
             INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                    " . $inner . "
             WHERE
                ee.entstatus = 'A' and
                    tpf.tpocod = '{$tpocod}'
                    " . $clausula . " AND
                    ( ee.entorgcod is null or ee.entorgcod <> '73000' )

             ORDER BY
                    ee.entnome)" . $uniao;

    $db->monta_combo("entid", $sql, 'S', "&nbsp;", 'ajax_carrega_campo_unidade', '', '', '350', 'S', 'entid');
}

function mostraEspelhoTarefa($itmid){    
    $objMonitoramento = new MonitoramentoEspecial();
    $campos           = " itm.*, resp.*, usu_resp_ent.entid as usu_resp_entid, esd.esddsc";
    $dadosTarefa      = $objMonitoramento->getDadosTarefas(array('itmid'=>$itmid),'array', $campos);
    $dadosTarefa      = $dadosTarefa[0];
    $itmid            = $dadosTarefa['itmid'];
    $atmid            = $dadosTarefa['atmid'];    
    
    $dadosAnalise = $objMonitoramento->getDadosAnaliseTarefa($itmid);
    $dadosAnalise = $dadosAnalise[0];
    
    $titulo_modulo    = 'Espelho de Tarefa de Monitoramento';
    $subtitulo_modulo = '';
    monta_titulo($titulo_modulo, $subtitulo_modulo);
    
?>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script language="JavaScript">

        function download(arqid, form){
            var req = $('#requisicao').val();
            $('#requisicao').val('download');
            $('#arqid').val(arqid);
            $('#formulario').submit();
            $('#requisicao').val(req);
        }
        
        function mostraDadosAtividade(atmid){
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {atmid: atmid, requisicao: 'mostraDadosAtividade'},
                dataType: "html",
                async: false,
                success: function(html) {
                    $('#dados_atividade').html(html);
                }
            });
        }

        function verificaExistenciaAtmid(atmid){
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {atmid: atmid, requisicao: 'verificaAtmid'},
                dataType: "html",
                async: false,
                success: function(data) {
                    if(data == false || data == 'false'){
                        alert('Essa atividade não existe. Informe o ID correto da Atividade.');
                        $('#atmid').val('');
                        $('#atmid').focus();
                        $('#dados_atividade').html('');
                        return false;
                    }else{
                        mostraDadosAtividade($('#atmid').val());
                        return data;
                    }
                }
            });
        }
        
        function carregaDivFormAnaliseTarefa(){
            $('#div_analise_tarefa').toggle();
            $('#tr_add_analise').toggle();
            resizeMe();
        }
        
        function resizeMe(){
            var height = document.body.offsetHeight;
            var width  = document.body.offsetWidth;
            self.resizeTo(width+70, height+100);
        }

        function mostraDivHistoricoResponsaveis(){
            $( "#historico_responsaveis" ).toggle();
        }

    </script>
    <form method="post" id="formulario" name="formulario" action="" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value=""/>
        <input type="hidden" name="itmid"      id="itmid"      value='<?php echo $itmid;?>'/>
        <input type="hidden" name="arqid"      id="arqid"      value=''/>
    </form>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
        <tr>
            <td class="subtituloDireita" style="width: 40%;">Atividade de Monitoramento:</td>
            <td >
                <?php echo /*$atmid.*/'<script language="JavaScript"> $(document).ready(function(){ verificaExistenciaAtmid('.$atmid.'); }); </script>'; ?>
                <br>
                <span id="dados_atividade" style="width: 400px"></span>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Nome da Tarefa:</td>
            <td> <?php echo stripslashes($dadosTarefa['itmnome']); ?> </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Situação da Tarefa:</td>
            <td> <?php echo $dadosTarefa['esddsc']; ?> </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Coletar Anexo de Evidência ?</td>
            <td > <?php switch ($dadosTarefa['itmcoletaranexostatus']) { case 'S': echo 'Sim'; break; case 'N': echo 'Não'; break; } ?> </td>
        </tr>
        <tr id="linha_anexos1">
            <td class="SubTituloDireita" width="20%">Nome do Anexo:</td>
            <td >
                <?php 
                    if(!empty($dadosTarefa['arqidanexo'])){
                        echo '<span onclick="download('.$dadosTarefa['arqidanexo'].')" style="cursor: pointer; vertical-align:middle" title="Baixar Arquivo">';
                        echo '<img align="absmiddle" src="/imagens/icone_lupa.png" style="cursor: pointer; vertical-align:middle" id="'.$dadosTarefa['arqidanexo'].'" title="Baixar Arquivo"> ' ;
                        echo $dadosTarefa['itmnomeanexo'] ;
                        echo '</span>' ;
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Data Limite:</td>
            <td >
                <?php 
                    $data = date_create($dadosTarefa['itmdtlimiteconclusao']); 
                    $data = date_format ( $data, 'd/m/Y' );
                    echo $data;
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Repetir a Tarefa automaticamente sem conclusão ?</td>
            <td > <?php switch ($dadosTarefa['itmstatusrepetenaoconcluido']) { case 'S': echo 'Sim'; break; case 'N': echo 'Não'; break; } ?> </td>
        </tr>
        <tr id="linha_dias_repeticao">
            <td class="SubTituloDireita">Prazo em dias para repetição:</td>
            <td > <?php echo (!empty($dadosTarefa['itmqtddiasstatusrepeticao'])) ? $dadosTarefa['itmqtddiasstatusrepeticao'] : 0; ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do Responsável:</td>
            <td >
                <?php
                    $responsavel = new Entidade($dadosTarefa['usu_resp_entid']);
                    echo $responsavel->entnome;
                    $objMon= new MonitoramentoEspecial();
                    $html = $objMon->getHistoricoResponsaveis($itmid);
                    if(!empty($html)){
                        echo '<br>';
                        echo '<a href="#" onclick="mostraDivHistoricoResponsaveis()" id="link_historico">Histórico de Responsáveis</a>';
                        echo '<div id="historico_responsaveis" style="display: none">';
                        echo $html;
                        echo '</div>';
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Observações:</td>
            <td >
                <?php echo stripslashes($dadosTarefa['itmobs']); ?>
            </td>
        </tr>
        
        <?php
            if(!empty($dadosAnalise)){
                echo '<script language="JavaScript"> $(document).ready(function(){ carregaDivFormAnaliseTarefa(); }); </script>';
            }else{
            } ?>
            <tr id="tr_add_analise" >
                <td class="subtituloDireita">Adicionar Análise:</td>
                <td>
                    <img style="cursor: pointer; vertical-align:middle" src="../imagens/gif_inclui.gif" onclick="javascript: carregaDivFormAnaliseTarefa();" title="Adicionar Análise">
                </td>
            </tr>
        
    </table>
    <div id="div_analise_tarefa" style="display: none">
        <?php
            mostraFormAnaliseTarefa($itmid, $dadosAnalise);
        ?>
    </div>
    <center><input type='button' class='botao' name='fechar' value='Fechar' onclick="window.close()" id='Fechar'/></center>
<?php
}


function mostraFormAnaliseTarefa($itmid, $dadosAnalise = array()){
    
    $objMonitoramento = new MonitoramentoEspecial();
    $campos           = " itm.*, resp.*, usu_resp_ent.entid as usu_resp_entid";
    $dadosTarefa      = $objMonitoramento->getDadosTarefas(array('itmid'=>$itmid),'array', $campos);
    $dadosTarefa      = $dadosTarefa[0];
    $itmid            = $dadosTarefa['itmid'];
    
    $titulo_modulo    = 'Cadastro de Análise de Tarefa de Monitoramento';
    $subtitulo_modulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';
    monta_titulo($titulo_modulo, $subtitulo_modulo);
    
    if(empty($dadosAnalise)){
        $dadosAnalise = $objMonitoramento->getDadosAnaliseTarefa($itmid);
        $dadosAnalise = !empty($dadosAnalise) ? $dadosAnalise[0] : array();
    }
    
    $anmid = $dadosAnalise['anmid'];
    
?>
    
<form method="post" id="formAnalise" name="formulario" action="" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="salvarAnaliseTarefa"/>
        <input type="hidden" name="itmid"      id="itmid"      value='<?php echo $itmid;?>'/>
        <input type="hidden" name="anmid"      id="anmid"      value='<?php echo $anmid;?>'/>
        
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
            <tr>
                <td class="subtituloDireita" style="width: 40%;"> Tarefa concluída ?</td>
                <td>
                    <?php
                        $select_s = '';
                        $select_n = '';
                        $select_r = '';
                        switch ($dadosAnalise['anmrespostaconclusao']) {
                            case 'S':
                                $select_s = 'checked="checked"';
                                break;
                            case 'R':
                                $select_r = 'checked="checked"';
                                break;
                            case 'N':
                                $select_n = 'checked="checked"';
                                break;
                            default :
                                break;
                        }
                    ?>
                    <input type="radio" name="anmrespostaconclusao" id="anmrespostaconclusao_s" value="S" <?php echo $select_s; ?> > Sim
                    <input type="radio" name="anmrespostaconclusao" id="anmrespostaconclusao_r" value="R" <?php echo $select_r; ?> > Sim, com ressalva
                    <input type="radio" name="anmrespostaconclusao" id="anmrespostaconclusao_n" value="N" <?php echo $select_n; ?> > Não
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" style="width: 40%;"> Observações:</td>
                <td>
                    <?php
                        echo campo_textarea("anmobs",'N','S','', 70, 8, 500, '', 0, '', false, NULL, $dadosAnalise['anmobs']);
                    ?>
                </td>    
            </tr>
            <tr>
                <td class="subtituloDireita" style="width: 40%;">Arquivo anexo:</td>
                <td>
                    <input type="file" name="arqidanexo" /> 
                    <br>
                    <?php 
                        if(!empty($dadosAnalise['arqidanexo'])){
                            echo '<span onclick="download('.$dadosAnalise['arqidanexo'].')" style="cursor: pointer; vertical-align:middle" title="Baixar Arquivo">';
                            echo '<img align="absmiddle" src="/imagens/icone_lupa.png" style="cursor: pointer; vertical-align:middle" id="'.$dadosAnalise['arqidanexo'].'" title="Baixar Arquivo"> ' ;
                            echo $dadosAnalise['arqnome'] ;
                            echo '</span>' ;
                        }
                    ?>
                </td>    
            </tr>
            <tr>
                <td colspan="2">
                    <center>
                        <input type='button' class='botao' name='Salvar' value='Salvar' onclick="validarFormAnalise()"/>
                        <input type='button' class='botao' name='Cancelar' value='Cancelar' onclick="carregaDivFormAnaliseTarefa()" id='cancelar'/>
                        <?php
                            $perfis = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC);
                            if(possuiPerfil($perfis) && !empty($anmid)){?>
                                <input type='button' class='botao' name='excluirAnalise' value='Excluir Análise' onclick="excluirAnaliseTarefa(<?php echo $anmid; ?>)" id='excluirAnalise'/>
                            <?php
                                }
                            ?>
                    </center>
                </td>    
            </tr>
        </table>
</form>
    
<script type="text/javascript">
    
    function validarFormAnalise() {

            var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
            var validacao = true;

            var anmrespostaconclusao   = $('[name=anmrespostaconclusao]:checked').val();
            if (anmrespostaconclusao == undefined || anmrespostaconclusao == '') {
                mensagem += 'O campo "Tarefa concluída ?" deve ser preenchido.\n';
                validacao = false;
                $('#anmrespostaconclusao').focus();
            }
            
            var anmobs = '';
            if (anmrespostaconclusao == 'N' || anmrespostaconclusao == 'R') {
                anmobs = $.trim($('#anmobs').val());
                if (anmobs == '') {
                    mensagem += 'O campo "Observações" deve ser preenchido.\n';
                    validacao = false;
                    $('#anmobs').focus();
                }else{
                    if(anmobs.length > 500){
                        anmobs = anmobs.substr(0, 496)+'...';
                        $('#anmobs').val(anmobs);
                    }
                }
            }
            
            if (!validacao) {
                alert(mensagem);
                return false;
            } else {
                $('#formAnalise').submit();
            }
        }
        
        function excluirAnaliseTarefa(anmid){
        var r = confirm("Você tem certeza que deseja excluir a Análise ?");
        if (r == true) {
            var url = 'obras2.php?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A';
            $.ajax({
                url: url,
                type: 'post',
                data: {anmid: anmid, requisicao: 'excluirAnalise'},
                dataType: "html",
                async: false,
                beforeSend: function() {
                    divCarregando();
                },
                error: function() {
                    divCarregado();
                },
                success: function(data) {
                    alert(data);
                    divCarregado();
                    window.location.href = window.location.href;
                }
            });
        }
    }
        
</script>
    
    
<?php    

}

?>