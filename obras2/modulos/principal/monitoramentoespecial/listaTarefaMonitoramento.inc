<?php
if ($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls') {
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->montaListaTarefasMonitoramento($_POST, $_REQUEST['tipo_relatorio']);
    exit;
}

if($_REQUEST['requisicao'] == 'download'){
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->downloadArquivoTarefa($_REQUEST['arqid']);
    die();
}

unset($_SESSION['obras2']['itmid']);
unset($_SESSION['obras2']['atmid']);

include APPRAIZ . "includes/cabecalho.inc";

$arMenuBlock = array();
$id_aba = ID_ABA_MONITORAMENTO_ESPECIAL;
$db->cria_aba($id_aba, $url, $parametros, $arMenuBlock);

if($_GET['acao'] === 'A'){

    $titulo_modulo = 'Lista de Tarefas de Monitoramento';
    $subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
    monta_titulo($titulo_modulo, $subtitulo_modulo);

    ?>

    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

    <script type="text/javascript">

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

        /**
         * Alterar visibilidade de um bloco.	 
         * @param string indica o bloco a ser mostrado/escondido
         * @return void
         */
        function onOffBloco(bloco) {
            var div_on = document.getElementById(bloco + '_div_filtros_on');
            var div_off = document.getElementById(bloco + '_div_filtros_off');
            var img = document.getElementById(bloco + '_img');
            var input = document.getElementById(bloco + '_flag');
            if (div_on.style.display == 'none')
            {
                div_on.style.display = 'block';
                div_off.style.display = 'none';
                input.value = '0';
                img.src = '/imagens/menos.gif';
            }
            else
            {
                div_on.style.display = 'none';
                div_off.style.display = 'block';
                input.value = '1';
                img.src = '/imagens/mais.gif';
            }
        }

        function getLista(tipo) {
            var formulario = document.formulario;
            var tipo_relatorio = tipo;
            prepara_formulario();
            document.getElementById('tipo_relatorio').value = tipo_relatorio;
            formulario.submit();
            document.getElementById('tipo_relatorio').value = '';
        }

        function visualizarTarefa(itmid){
            window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&itmid='+itmid+'&bloqkEdicao=true';
        }

        function alterarTarefa(itmid){
            window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&itmid='+itmid;
        }

        function excluirTarefa(itmid){
            var r = confirm("Você tem certeza que deseja excluir a Tarefa "+itmid+" ?");
            if (r == true) {
                var url = 'obras2.php?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A';
                $.ajax({
                    url: url,
                    type: 'post',
                    data: {itmid: itmid, requisicao: 'excluir'},
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
                        window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaTarefaMonitoramento&acao=A';
                    }
                });
            }
        }

        $(function($) {
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro', 'Fevereiro', 'Mar&ccedil;o', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
                monthNamesShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
                dayNames: ['Domingo', 'Segunda-feira', 'Ter&ccedil;a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', 'S&aacute;bado'],
                dayNamesShort: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                dayNamesMin: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });

        function download(arqid){
            var req = $('#requisicao').val();
            $('#requisicao').val('download');
            $('#arqid').val(arqid);
            $('#formulario').submit();
            $('#requisicao').val(req);
        }


    </script>

    <form name="formulario" id="formulario" action="" method="post">

        <input type="hidden" name="form" value="1" /> 
        <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 
        <input type="hidden" name="requisicao" id="requisicao" value=""/>
        <input type="hidden" name="arqid"      id="arqid"      value=''/>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
            <tr>
                <td class="subtituloDireita" style="width: 40%;">ID da Atividade de Monitoramento:</td>
                <td>
                    <?php
                        if (!empty($_SESSION['obras2']['atmid'])) {
                            $val = $_SESSION['obras2']['atmid'];
                        } else {
                            $val = (!empty($_POST['atmid'])) ? $_POST['atmid'] : '';
                        }
                        echo campo_texto('atmid', 'N', 'S', '', 70, 80, '######', '', '', '', '', 'id="atmid"', '', $val);
                    ?>

                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Nome da Tarefa:</td>
                <td>
                <?php
                    $val = (!empty($_POST['itmnome'])) ? $_POST['itmnome'] : '';
                    echo campo_texto('itmnome', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="itmnome"', '', $val);
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Coletar Anexo de Evidência ?</td>
                <td>
                    <?php
                        $select_s = '';
                        $select_n = '';
                        $select_t = '';
                        switch ($_POST['itmcoletaranexostatus']) {
                            case 'S':
                                $select_s = 'checked="checked"';
                                break;
                            case 'N':
                                $select_n = 'checked="checked"';
                                break;
                            case 'T':
                                $select_t = 'checked="checked"';
                                break;
                            default :
                                $select_t = 'checked="checked"';
                                break;
                        }
                    ?>
                    <input type="radio" name="itmcoletaranexostatus" id="itmcoletaranexostatus_s" value="S" <?php echo $select_s; ?> > Sim
                    <input type="radio" name="itmcoletaranexostatus" id="itmcoletaranexostatus_n" value="N" <?php echo $select_n; ?> > Não
                    <input type="radio" name="itmcoletaranexostatus" id="itmcoletaranexostatus_n" value="T" <?php echo $select_t; ?> > Todos
                </td>
            </tr>
            <tr id="linha_anexos1">
                <td class="SubTituloDireita" width="20%">Nome do Anexo:</td>
                <td>
                <?php
                    echo campo_texto('itmnomeanexo', 'N', $habilita, '', 43, 100, '', '', '', '', '', 'id="itmnomeanexo"', '', $_POST['itmnomeanexo']);
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Repetir a Tarefa automaticamente sem conclusão ?</td>
                <td>
                <?php
                    $select_s = '';
                    $select_n = '';
                    $select_t = '';
                    switch ($_POST['itmstatusrepetenaoconcluido']) {
                        case 'S':
                            $select_s = 'checked="checked"';
                            break;
                        case 'N':
                            $select_n = 'checked="checked"';
                            break;
                        case 'T':
                            $select_t = 'checked="checked"';
                            break;
                        default :
                            $select_t = 'checked="checked"';
                            break;
                    }
                ?>
                    <input type="radio" name="itmstatusrepetenaoconcluido" id="itmstatusrepetenaoconcluido_s" value="S" <?php echo $select_s; ?> > Sim
                    <input type="radio" name="itmstatusrepetenaoconcluido" id="itmstatusrepetenaoconcluido_n" value="N" <?php echo $select_n; ?> > Não
                    <input type="radio" name="itmstatusrepetenaoconcluido" id="itmstatusrepetenaoconcluido_t" value="T" <?php echo $select_t; ?> > Todos
                </td>
            </tr>
            <tr id="linha_dias_repeticao">
                <td class="SubTituloDireita">Prazo em dias para repetição:</td>
                <td>
                    <?php
                        $select_01 = '';
                        $select_07 = '';
                        $select_10 = '';
                        $select_30 = '';
                        $select_60 = '';
                        $select_90 = '';
                        $select_t = '';
                        $select_especificado = '';
                        switch ($_POST['itmqtddiasstatusrepeticao']) {
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
                            case 'T':
                                $select_t = 'checked="checked"';
                                break;
                            default :
                                $select_t = 'checked="checked"';
                                break;
                        }
                    ?>
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_01" value="1"  <?php echo $select_01; ?> > 1  &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_07" value="7"  <?php echo $select_07; ?> > 7  &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_10" value="10" <?php echo $select_10; ?> > 10 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_30" value="30" <?php echo $select_30; ?> > 30 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_60" value="60" <?php echo $select_60; ?> > 60 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_90" value="90" <?php echo $select_90; ?> > 90 &nbsp;&nbsp;
                    <input type="radio" name="itmqtddiasstatusrepeticao" id="itmqtddiasstatusrepeticao_t"  value="T"  <?php echo $select_t; ?>  > Todos &nbsp;&nbsp;
                    <br />
                    <br />Especificar o número de dias:
                    <?php
                        $select_especificado = trim($_POST['itmqtddiasstatusrepeticao_especificado']);
                        echo campo_texto('itmqtddiasstatusrepeticao_especificado', 'N', 'S', '', 43, 100, '####', '', '', '', '', 'id="itmnomeanexo"', '', $select_especificado);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Situação da Tarefa:</td>
                <td>
                <?php
                    $sql = ' 
                            SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = ' . TPID_ME_ITEM_MONITORAMENTO . ' 
                            ORDER BY esdid
                           ';
                    $val = (!empty($_POST['esdid'])) ? $_POST['esdid'] : '';
                    $db->monta_combo("esdid", $sql, "S", "Todas", "", "", "", 200, "N", "esdid", false, $val);
                ?>
                </td>
            </tr>
            <tr>
                <?php
                    $sql = " SELECT DISTINCT usuario.usucpf as codigo, usuario.usunome as descricao
                                                 FROM seguranca.usuario usuario
                                                 LEFT JOIN seguranca.perfilusuario perfilusuario     ON usuario.usucpf = perfilusuario.usucpf
                                                 LEFT JOIN seguranca.perfil perfil                   ON perfil.pflcod  = perfilusuario.pflcod 
                                                 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                                                 WHERE usuario.usustatus = 'A'
                                                   AND usuariosistema.suscod = 'A' 
                                                   AND usuariosistema.sisid = '" . $_SESSION['sisid'] . "' 
                                                 ORDER BY usuario.usunome";
                    $stSqlCarregados = '';
                    $arr_usu_wf = array();
                    if (!empty($_POST['usucpf']) && is_array($_POST['usucpf']) && $_POST['usucpf'][0] != '') {
                        foreach ($_POST['usucpf'] as $key => $value) {
                            $arr_usu_wf[$key] = "'" . $value . "'";
                        }
                        $str_colecao = (!empty($arr_usu_wf)) ? " AND usuario.usucpf IN (" . implode(',', $arr_usu_wf) . ") " : '';
                        $stSqlCarregados = " SELECT DISTINCT usuario.usucpf as codigo, usuario.usunome as descricao
                                                                 FROM seguranca.usuario usuario
                                                                 LEFT  JOIN seguranca.perfilusuario perfilusuario     ON usuario.usucpf = perfilusuario.usucpf
                                                                 LEFT  JOIN seguranca.perfil perfil                   ON perfil.pflcod  = perfilusuario.pflcod 
                                                                 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                                                                 WHERE usuario.usustatus     = 'A'
                                                                   AND usuariosistema.suscod = 'A' 
                                                                   AND usuariosistema.sisid  = '" . $_SESSION['sisid'] . "' 
                                                                       {$str_colecao}
                                                                 ORDER BY usuario.usunome";
                    }
                    mostrarComboPopup('Responsável pelo Monitoramento:', 'usucpf_responsavel', $sql, $stSqlCarregados, 'Selecione o(s) Responsável(eis)', null, '', false);
                ?>
            </tr>
            <tr>
                <?php
                    $sql = " SELECT DISTINCT usuario.usucpf as codigo, usuario.usunome as descricao
                                                 FROM seguranca.usuario usuario
                                                 LEFT JOIN seguranca.perfilusuario perfilusuario     ON usuario.usucpf = perfilusuario.usucpf
                                                 LEFT JOIN seguranca.perfil perfil                   ON perfil.pflcod  = perfilusuario.pflcod 
                                                 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                                                 WHERE usuario.usustatus = 'A'
                                                   AND usuariosistema.suscod = 'A' 
                                                   AND usuariosistema.sisid = '" . $_SESSION['sisid'] . "' 
                                                 ORDER BY usuario.usunome";
                    $stSqlCarregados = '';
                    $arr_usu_wf = array();
                    if (!empty($_POST['usucpf']) && is_array($_POST['usucpf']) && $_POST['usucpf'][0] != '') {
                        foreach ($_POST['usucpf'] as $key => $value) {
                            $arr_usu_wf[$key] = "'" . $value . "'";
                        }
                        $str_colecao = (!empty($arr_usu_wf)) ? " AND usuario.usucpf IN (" . implode(',', $arr_usu_wf) . ") " : '';
                        $stSqlCarregados = " SELECT DISTINCT usuario.usucpf as codigo, usuario.usunome as descricao
                                                                 FROM seguranca.usuario usuario
                                                                 LEFT  JOIN seguranca.perfilusuario perfilusuario     ON usuario.usucpf = perfilusuario.usucpf
                                                                 LEFT  JOIN seguranca.perfil perfil                   ON perfil.pflcod  = perfilusuario.pflcod 
                                                                 INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                                                                 WHERE usuario.usustatus     = 'A'
                                                                   AND usuariosistema.suscod = 'A' 
                                                                   AND usuariosistema.sisid  = '" . $_SESSION['sisid'] . "' 
                                                                       {$str_colecao}
                                                                 ORDER BY usuario.usunome";
                    }
                    mostrarComboPopup('Responsável pelo Cadastro:', 'usucpf_inclusao', $sql, $stSqlCarregados, 'Selecione o(s) Responsável(eis)', null, '', false);
                ?>
            </tr>
            <tr>
                <td class="subtitulodireita" width="190px">Data de Cadastro:</td>
                <td>
                    <?php
                        $data_de = (empty($_POST['itmdtinclusao_de'])) ? '' : $_POST['itmdtinclusao_de'];
                        $data_ate = (empty($_POST['itmdtinclusao_ate'])) ? '' : $_POST['itmdtinclusao_ate'];
                    ?>
                    de: <input type="text" id="itmdtinclusao_de" name="itmdtinclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal" > 
                    &nbsp;
                    até: <input type="text" id="itmdtinclusao_ate" name="itmdtinclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
                </td>
                <td>&nbsp;</td>                
            </tr>
            <tr>
                <td class="subtitulodireita" width="190px">Data Limite:</td>
                <td>
                    <?php
                        $data_de = (empty($_POST['itmdtlimiteconclusao_de'])) ? '' : $_POST['itmdtlimiteconclusao_de'];
                        $data_ate = (empty($_POST['itmdtlimiteconclusao_ate'])) ? '' : $_POST['itmdtlimiteconclusao_ate'];
                    ?>
                    de: <input type="text" id="itmdtlimiteconclusao_de" name="itmdtlimiteconclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal"> 
                    &nbsp;
                    até: <input type="text" id="itmdtlimiteconclusao_ate" name="itmdtlimiteconclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
                </td>
                <td>&nbsp;</td>                
            </tr>
            <tr>
                <td colspan="2" class="subtituloDireita">&nbsp;</td>
            </tr>
            <tr>
                <td colspan="2">
                    <center>
                        <input type="button" id="btn_filtrar" name="btn_filtrar" value="Filtrar"      onclick="javascript:getLista('html');"/>
                        <input type="button" id="btn_excel"   name="btn_excel"   value="Exportar XLS" onclick="javascript:getLista('xls');"/>
                    </center>
                </td>
            </tr>
        </table>
    </form>

    <script lang="javascript">
        setTimeout(function() {

            $('#itmdtinclusao_de').mask('99/99/9999');
            $('#itmdtlimiteconclusao_de').mask('99/99/9999');
            $('#itmdtinclusao_ate').mask('99/99/9999');
            $('#itmdtlimiteconclusao_ate').mask('99/99/9999');

            $("#itmdtinclusao_de").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                showAnim: 'drop'
            });

            $("#itmdtinclusao_ate").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                showAnim: 'drop'
            });
            $("#itmdtlimiteconclusao_de").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                'showAnim': 'drop'
            });
            $("#itmdtlimiteconclusao_ate").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                'showAnim': 'drop'
            });

        }, 500);
    </script>

<?php

    if ($_REQUEST['form'] == '1') {
        $objMonitoramento = new MonitoramentoEspecial();
        $objMonitoramento->montaListaTarefasMonitoramento($_POST, $_REQUEST['tipo_relatorio']);
    }

}
elseif($_GET['acao'] === 'M'){ // M = Minhas Tarefas
    
    
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

    <script type="text/javascript">

        function mostraEspelhoTarefa(itmid) {            
            var url     = 'obras2.php?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&requisicao=espelhoTarefa&itmid='+itmid;
            var name    = 'Espelho Tarefa';
            var options = 'height=500,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes';            
            window.open(url,name,options).focus();
        }
        
        function download(arqid){
            var req = $('#requisicao').val();
            $('#requisicao').val('download');
            $('#arqid').val(arqid);
            $('#formulario').submit();
            $('#requisicao').val(req);
        }
        
        function alterarTarefa(itmid){
            window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&itmid='+itmid;
        }

        function excluirTarefa(itmid){
            var r = confirm("Você tem certeza que deseja excluir a Tarefa "+itmid+" ?");
            if (r == true) {
                var url = 'obras2.php?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A';
                $.ajax({
                    url: url,
                    type: 'post',
                    data: {itmid: itmid, requisicao: 'excluir'},
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
                        window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaTarefaMonitoramento&acao=A';
                    }
                });
            }
        }
    
    </script>
    
    <form method="post" id="formulario" name="formulario" action="" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value=""/>
        <input type="hidden" name="itmid"      id="itmid"      value='<?php echo $itmid;?>'/>
        <input type="hidden" name="arqid"      id="arqid"      value=''/>
    </form>
    
    <?php
    
    $titulo_modulo = 'Lista de Tarefas de Monitoramento';
    $subtitulo_modulo = 'Minhas tarefas';
    monta_titulo($titulo_modulo, $subtitulo_modulo);
    
    $usucpf_inclusao = $_SESSION['usucpf'];
    
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->montaListaTarefasMonitoramento(array('usucpf_inclusao' => $usucpf_inclusao), 'minhasTarefas' );
    
    
}

?>