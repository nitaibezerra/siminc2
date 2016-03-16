<?php
if(empty($_REQUEST['bloqkEdicao']) && $_REQUEST['bloqkEdicao'] != true ){    
    $perfis = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC);
    if (!possuiPerfil($perfis)) {
        echo '<script > 
                alert(\'Você não possui permissão para acessar essa tela.\');
                window.history(-1);
              </script>';
        die();
    }
}

if($_REQUEST['requisicao'] == 'download'){
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->downloadArquivoTarefa($_REQUEST['arqid']);
    die();
}

if($_REQUEST['requisicao'] == 'espelhoAtividade'){
    $atmid = $_REQUEST['atmid'];
    mostraEspelhoAtividade($atmid);
    die();
}

switch ($_POST['requisicao']) {
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
        $atmid            = $objMonitoramento->cadastrarDadosAtividade($_POST);
        if(empty($atmid)){
            echo '<script > 
                    alert(\'Ocorreu um erro ao cadastrar a Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente.\');
                    window.history(-1);
                  </script>';
            die();
        }else{
            $docid = $objMonitoramento->atualizaDocidAtividade($atmid);
            if(empty($docid)){
                echo '<script > 
                        alert(\'Ocorreu um erro ao cadastrar o Fluxo da Atividade de Monitoramento. Verifique se os dados foram preenchidos corretamente ou entre em contato com a equipe do SIMEC.\');
                        window.history(-1);
                      </script>';
                die();
            }
        }
        
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
        $atmid            = $objMonitoramento->editarDadosAtividade($_POST);
        
        if($atmid){
            $msg = 'Atividade de Monitoramento Editada com sucesso.';
        }else{
            $msg = 'Erro ao editar a Atividade de Monitoramento.';
        }
        
        echo "<script > 
                alert('".$msg."');
                window.location = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid=".$atmid."';
              </script>";
        die();
        break;
    case 'duplicar':
        $perfis = array(PFLCOD_SUPER_USUARIO);
        if (!possuiPerfil($perfis)) {
            echo '<script > 
                    alert(\'Você não possui permissão para alterar os dados.\');
                    window.history(-1);
                  </script>';
            die();
        }
        
        $params['estuf']                = $_POST['estuf'];
        $params['muncod']               = $_POST['muncod'];
        $params['entid']                = $_POST['entid'];
        $params['obrid']                = $_POST['obrid'];
        $params['tamid']                = $_POST['tamid'];
        $params['samid']                = $_POST['samid'];
        $params['atmnome']              = $_POST['atmnome'];
        $params['atmobs']               = $_POST['atmobs'];
        $params['atmdtlimiteconclusao'] = $_POST['atmdtlimiteconclusao'];
        
        $msg              = '';
        $objMonitoramento = new MonitoramentoEspecial();
        $atmid            = $objMonitoramento->cadastrarDadosAtividade($params);
        
        if($atmid){
            $msg   = 'Atividade de Monitoramento duplicada com sucesso.';
            $docid = $objMonitoramento->atualizaDocidAtividade($atmid);
            if(!$docid){ $msg .= 'Erro ao cadastrar o DOCID da Atividade de Monitoramento.'; }
            
            $dadosTarefas = $objMonitoramento->getDadosTarefas(array('atmid'=> $_POST['atmid']), 'array', 'itm.itmid');
            foreach ($dadosTarefas as $key => $value) {
                $itmid = $value['itmid'];
                $respClone = $objMonitoramento->clonarTarefa($itmid, null, $atmid);
                if(!$respClone){ $msg .= 'Erro ao duplicar a Tarefa da Atividade de Monitoramento.'; }
            }
        }else{
            $msg = 'Erro ao duplicar a Atividade de Monitoramento.';
        }
        
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
        $atmid = $_POST['atmid'];
        $objMonitoramento = new MonitoramentoEspecial();
        $atmid            = $objMonitoramento->excluirAtividade($atmid);
        if($atmid){
            echo 'Atividade de Monitoramento excluída com sucesso.';
        }else{
            echo 'Erro ao excluir a Atividade de Monitoramento.';
        }
        die();
        break;
    default :
        break;
}

if(!empty($_REQUEST['atmid']) || !empty($_SESSION['obras2']['atmid']))  {
    $objMonitoramento = new MonitoramentoEspecial();
    $campos = " atm.*,
                tam.*,
                sam.*,
                COALESCE(TO_CHAR(atm.atmdtinclusao, 'dd/mm/YYYY')) AS atmdtinclusao,
                COALESCE(TO_CHAR(atm.atmdtlimiteconclusao, 'dd/mm/YYYY')) AS atmdtlimiteconclusao
                  ";
    
    $atmid          = !empty($_REQUEST['atmid']) ? $_REQUEST['atmid'] : $_SESSION['obras2']['atmid'];
    $dadosAtividade = $objMonitoramento->getDadosAtividade(array('atmid'=>$atmid),'array', $campos);
    $dadosAtividade = $dadosAtividade[0];
    $atmid          = $dadosAtividade['atmid'];
    
    $_SESSION['obras2']['atmid'] = $atmid;
    unset($_SESSION['obras2']['itmid']);
}else{
    $dadosAtividade = array();
    $atmid          = '';
    unset($_SESSION['obras2']['itmid']);
    unset($_SESSION['obras2']['atmid']);
}
$coluna_acoes_tarefas = true;

if($_REQUEST['requisicao'] == 'duplicarAtividade'){
    unset($_SESSION['obras2']['itmid']);
    unset($_SESSION['obras2']['atmid']);
    $dadosAtividade['obrid'] = NULL;
    $coluna_acoes_tarefas    = false;
}

switch ($_REQUEST['ajax']) {
    case 'municipio':
        carrega_campo_municipio();
        reset_campos_ajax(array('entidade','obra'));
        die;
    case 'ajax_carrega_campo_entidade':
        carrega_campo_entidade();
        reset_campos_ajax(array('obra'));
        die;
    case 'ajax_carrega_campo_obras':
        carrega_campo_obras();
        die;
}

include APPRAIZ . "includes/cabecalho.inc";

$arMenuBlock = array();
$id_aba = ID_ABA_MONITORAMENTO_ESPECIAL;
$db->cria_aba($id_aba, $url, $parametros, $arMenuBlock);
$titulo_modulo = 'Cadastro de Atividade de Monitoramento';
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
<script type="text/javascript" language="JavaScript">

    function cancelarCadastro() {
        window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaAtividadeMonitoramento&acao=A';
    }

    function carregarMunicipio(estuf) {
        var td = $('#td_municipio');
        if (estuf != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'municipio', estuf: estuf},
                dataType: "html",
                async: false,
                beforeSend: function() {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function() {
                    divCarregado();
                },
                success: function(data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                    .attr('disabled', true);
        }
    }

    function ajax_carrega_campo_entidade() {
        var td = $('#td_entidade');
        var estuf = $('#estuf').val();
        var muncod = $('#muncod').val();
        if (estuf != '' && muncod != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'ajax_carrega_campo_entidade', estuf: estuf, muncod: muncod},
                dataType: "html",
                async: false,
                beforeSend: function() {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function() {
                    divCarregado();
                },
                success: function(data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                    .attr('disabled', true);
        }
    }

    function ajax_carrega_campo_obras() {
        var td = $('#td_obra');
        var entid = $('#entid').val();
        if (entid != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'ajax_carrega_campo_obras', entid: entid},
                dataType: "html",
                async: false,
                beforeSend: function() {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function() {
                    divCarregado();
                },
                success: function(data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                    .attr('disabled', true);
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
    
    function alterarTarefa(itmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&itmid='+itmid;
    }
    
    function incluirTarefaAtividade(atmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&atmid='+atmid;
    }
    
    function download(arqid){
        $('#requisicao').val('download');
        $('#arqid').val(arqid);
        $('#formulario').submit();
    }

    $(document).ready(function() {
        var bloqueiaForm = <?php if($_REQUEST['bloqkEdicao']) { echo 'true'; } else { echo 'false';} ?>;
        if(bloqueiaForm === true){
            jQuery('#formulario').find('input, textarea, button, select').attr('disabled','disabled');
            jQuery('#formulario').find('a').attr('onclick','alert("Você não pode mudar o estado da Atividade")');
            jQuery('#cancelar').attr('disabled', false);
        }
    });

</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">

<form method="post" id="formulario" name="formulario" action="" enctype="multipart/form-data">
        <?php $acaoform = !empty($_REQUEST['atmid']) ? 'editar' : 'salvar'; ?>
        <?php $acaoform = ($_REQUEST['requisicao'] == 'duplicarAtividade') ? 'duplicar' : $acaoform; ?>
        <input type="hidden" name="requisicao" id="requisicao" value="<?php echo $acaoform; ?>"/>
        <input type="hidden" name="atmid"      id="atmid"   value='<?php echo $atmid; ?>'/>
        <input type="hidden" name="arqid"      id="arqid"      value=''/>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
            <tr>
                <td class="SubTituloDireita" style="width: 40%;">UF:</td>
                <td colspan="2">
                    <?php
                        $dadosAtividade['estuf'] = !empty($dadosAtividade['estuf']) ? $dadosAtividade['estuf'] : '';
                        $sqlUf = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				ORDER BY
					estuf";
		
                        $db->monta_combo("estuf", $sqlUf, 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'S', 'estuf', false, $dadosAtividade['estuf']);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Município:</td>
                <td id="td_municipio">
                    <?php
                        if (!empty($dadosAtividade['muncod'])) {
                            $municipio = new Municipio();
                            $dado      = $municipio->listaCombo(array('muncod' => $dadosAtividade['muncod']));
                            $habMun    = 'S';
                        } else {
                            $dadosAtividade['muncod'] = '';
                            $dado = array();
                            $habMun = 'N';
                        }
                        echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', 'ajax_carrega_campo_entidade', '', '', 200, 'S', 'muncod', false, $dadosAtividade['muncod']);
                    ?>
                </td>
                <td rowspan="5"  align="right" valign="top" width="1">
                    <?php
                        $docid = $dadosAtividade['docid'];
                        if($docid){
                            wf_desenhaBarraNavegacao($docid, array('atmid' =>  $atmid));
                        }else{
                            echo '&nbsp;';
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Entidade:</td>
                <td id="td_entidade" colspan="2">
                    <?php
                        if (!empty($dadosAtividade['entid'])) {
                            carrega_campo_entidade($dadosAtividade['estuf'], $dadosAtividade['muncod'], $dadosAtividade['entid']);
                        } else {
                            $dadosAtividade['entid'] = '';
                            $dado = array();
                            $habEnt = 'N';
                            echo $db->monta_combo("entid", $dado, $habEnt, 'Selecione...', 'ajax_carrega_campo_obras', '', '', 200, 'S', 'entid', false, $dadosAtividade['entid']);
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" style="width: 40%;">Obra:</td>
                <td id="td_obra" colspan="2">
                    <?php
                        if (!empty($dadosAtividade['entid']) && !empty($dadosAtividade['obrid'])) {
                            carrega_campo_obras($dadosAtividade['entid'], $dadosAtividade['obrid']);
                        } else {
                            $dadosAtividade['obrid'] = '';
                            $dado = array();
                            $habObr = 'N';
                            echo $db->monta_combo("obrid", $dado, $habObr, 'Selecione...', '', '', '', 200, 'N', 'obrid', false, $dadosAtividade['obrid']);
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Origem da Atividade:</td>
                <td colspan="2">
                    <?php
                        $val = (!empty($dadosAtividade['atmnome'])) ? stripslashes($dadosAtividade['atmnome']) : '';
                        echo campo_texto('atmnome', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="atmnome"', '', $val);
                    ?>
                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                    
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Tipo da Atividade:</td>
                <?php
                    $dadosAtividade['tamid'] = !empty($dadosAtividade['tamid']) ? $dadosAtividade['tamid'] : '';
                    $sqlTp = " SELECT
                                        tamid as codigo, tamdesc as descricao
                                   FROM obras2.tipoatividademonitoramento 
                                   ORDER BY tamdesc ";
                    echo '<td colspan="2">';
                    $db->monta_combo("tamid", $sqlTp, 'S', 'Selecione...', '', '', '', 200, 'N', 'tamid', false, $dadosAtividade['tamid']);
                    echo '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">';
                    echo '</td>';
                ?>
            </tr>
            <tr>
                <td class="subtituloDireita">Subtipo da Atividade:</td>
                <?php
                    $dadosAtividade['samid'] = !empty($dadosAtividade['samid']) ? $dadosAtividade['samid'] : '';
                    $sqlStp = " SELECT
                                        samid as codigo, samdesc as descricao
                                   FROM obras2.subtipoatividademonitoramento
                                   ORDER BY samdesc ";
                    $sqlStp = $sqlStp;
                    echo '<td colspan="2">';
                    $db->monta_combo("samid", $sqlStp, 'S', 'Selecione...', '', '', '', 200, 'N', 'samid', false, $dadosAtividade['samid']);
                    echo '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">';
                    echo '</td>';
                ?>
            </tr>
            <tr>
                <td class="subtituloDireita">Data Limite:</td>
                <td colspan="2">
                    <?php
                        if (empty($dadosAtividade['atmdtlimiteconclusao'])) {
                            $data = '';
                        } else {
                            $data = $dadosAtividade['atmdtlimiteconclusao'];
                        }
                    ?>
                    <input type="text" id="atmdtlimiteconclusao" name="atmdtlimiteconclusao" value="<?php echo $data; ?>" size="15" maxlength="10" class="normal  obrigatorio "> 
                    <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Observações:</td>
                <td colspan="2">
                    <?php
                        $value = empty($dadosAtividade['atmobs']) ? '' : stripslashes($dadosAtividade['atmobs']);
                        echo campo_textarea("atmobs", 'N', 'S', '', 70, 8, 500, '', 0, '', false, NULL, $value);
                    ?>
                </td>
            </tr>
            <?php if(!empty($atmid)){ ?>
                <tr>
                    <td class="subtituloDireita">Inserir Tarefa:</td>
                    <td colspan="2">
                        <img style="cursor: pointer; vertical-align:middle" src="../imagens/gif_inclui.gif" onclick="javascript: incluirTarefaAtividade(<?php echo $atmid; ?>);" title="Adicionar Tarefa">
                    </td>
                </tr>
            <?php } ?>
            <?php if (!empty($dadosAtividade['atmid'])) { ?>
                <tr>
                    <td colspan="3"> <?php
                            $objMonitoramento = new MonitoramentoEspecial();
                            $objMonitoramento->montaListaTarefasMonitoramento(array('atmid'=> $dadosAtividade['atmid']), 'html', $coluna_acoes_tarefas);
                    ?> </td>
                </tr>
            <?php } ?>
            <tr bgcolor="#C0C0C0" style="text-align: center">
                <td colspan="3">
                    <input type='button' class='botao' name='Salvar'   value='Salvar'   id='Salvar'   onclick="validar()"/>
                    <input type='button' class='botao' name='Cancelar' value='Cancelar' id='cancelar' onclick="cancelarCadastro()"/>
                </td>
            </tr>
        </table>
    </form>
    
    <script type="text/javascript">

        setTimeout(function() {

            jQuery('#atmdtlimiteconclusao').mask('99/99/9999');

            jQuery("#atmdtlimiteconclusao").datepicker({
                numberOfMonths: 1,
                dateFormat: 'dd/mm/yy',
                showWeek: true,
                showAnim: 'drop'
            });

        }, 500);

        function validar() {

            var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
            var validacao = true;
            var estuf     = $('#estuf').val();
            var muncod    = $('#muncod').val();
            var entid     = $('#entid').val();
            var atmnome   = $('#atmnome').val();
            var tamid     = $('#tamid').val();
            var samid     = $('#samid').val();
            var atmdtlimiteconclusao = $('#atmdtlimiteconclusao').val();

            if (estuf == '') {
                mensagem += 'O campo de UF deve ser preenchido.\n';
                validacao = false;
                $('#estuf').focus();
            }
            if (muncod == '') {
                mensagem += 'O campo de Município deve ser preenchido.\n';
                validacao = false;
                $('#muncod').focus();
            }
            if (entid == '') {
                mensagem += 'O campo de Entidade deve ser preenchido.\n';
                validacao = false;
                $('#entid').focus();
            }
            if (atmnome == '') {
                mensagem += 'O campo de Origem da Atividade deve ser preenchido.\n';
                validacao = false;
                $('#atmnome').focus();
            }
            if (tamid == '') {
                mensagem += 'O campo de Tipo da Atividade deve ser preenchido.\n';
                validacao = false;
                $('#tamid').focus();
            }
            if (samid == '') {
                mensagem += 'O campo de Subtipo da Atividade deve ser preenchido.\n';
                validacao = false;
                $('#samid').focus();
            }
            if (atmdtlimiteconclusao == '') {
                mensagem += 'O campo de Data Limite deve ser preenchido.\n';
                validacao = false;
                $('#atmdtlimiteconclusao').focus();
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

function reset_campos_ajax($arrCampos = array()) {
    global $db;
    foreach ($arrCampos as $key => $value) {
        switch ($value) {
            case 'entidade':
                $campo = str_replace("'", '"', $db->monta_combo("entid", array(), 'N', 'Selecione...', '', '', '', 200, 'S', 'entid', true, ''));
                echo ' <script type="text/javascript">
                            setTimeout(function() {
                                var td = $(\'#td_entidade\');
                                var html = \''.$campo.'\';
                                td.html(html); 
                            }, 10);
                        </script> '; 
                break;
            case 'obra':
                $campo = str_replace("'", '"', $db->monta_combo("obrid", array(), 'N', 'Selecione...', '', '', '', 200, 'S', 'obrid', true, ''));
                echo ' <script type="text/javascript">
                            setTimeout(function() {
                                var td = $(\'#td_obra\');
                                var html = \''.$campo.'\';
                                td.html(html); 
                            }, 10);
                        </script> '; 
                break;
            default:
                break;
        }
        
    }
}

function carrega_campo_municipio($estuf = null, $muncod = null) {
    global $db;
//    header('content-type: text/html; charset=ISO-8859-1');
    $municipio = new Municipio();
    if(empty($estuf)){
        $estuf = !empty($_REQUEST['estuf']) ? $_REQUEST['estuf'] : '' ;
    }
    if(empty($muncod)){
        $muncod = !empty($_REQUEST['muncod']) ? $_REQUEST['muncod'] : '' ;
    }
    echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $estuf)), 'S', 'Selecione...', 'ajax_carrega_campo_entidade', '', '', 200, 'S', 'muncod', false, $muncod);
}

function carrega_campo_entidade($estuf = null, $muncod = null, $entid = '') {
    global $db;
//    header('content-type: text/html; charset=ISO-8859-1');
    if (empty($estuf)) {
        $estuf = trim($_REQUEST['estuf']);
    }
    $strEstuf = " AND mun.estuf = '{$estuf}' ";
    
    if (empty($muncod)) {
        $muncod = trim($_REQUEST['muncod']);
    }
    $strMuncod = " AND mun.muncod = '{$muncod}' ";
    
    $sql_obr = "SELECT e.entid 
                FROM obras2.obras o 
                LEFT JOIN entidade.entidade       e ON e.entid    = o.entid
                LEFT JOIN entidade.endereco    ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                WHERE e.entstatus = 'A' 
                    {$strEstuf}
                    {$strMuncod}
                GROUP BY e.entid
               ";
    $arrEntid = $db->carregar($sql_obr);
    
    if (empty($arrEntid)) {
        $arrEntid = array();
        $strEntid = '';
        $sql_ent = array();
    } else {
        $arrEntidF = array();
        foreach ($arrEntid as $key => $value) {
            $arrEntidF[] = $value['entid'];
        }
        $strEntid = ' WHERE e.entid IN (' . implode(',', $arrEntidF) . ') ';
        
        $sql_ent = " SELECT e.entid AS codigo, e.entnome AS descricao
                      FROM entidade.entidade e
                      {$strEntid}
                     ";
    }
    $db->monta_combo("entid", $sql_ent, 'S', "&nbsp;", 'ajax_carrega_campo_obras', '', '', '350', 'S', 'entid', false, $entid);
}

function carrega_campo_obras($entid = null, $obrid = '') {
    global $db;
//    header('content-type: text/html; charset=ISO-8859-1');
    if(empty($entid)){
        $entid = $_REQUEST['entid'];
    }
    $sql = " SELECT
                      o.obrid AS codigo,
                      '(' || o.obrid || ') - ' || o.obrnome AS descricao
               FROM
                      obras2.obras o
               WHERE
                      o.entid = {$entid}
               ORDER BY
                      o.obrnome";
    $db->monta_combo("obrid", $sql, 'S', "&nbsp;", '', '', '', '350', 'N', 'obrid', false, $obrid);
}

function mostraEspelhoAtividade($atmid){
     $campos = " atm.*,
                tam.*,
                sam.*,
                COALESCE(TO_CHAR(atm.atmdtinclusao, 'dd/mm/YYYY')) AS atmdtinclusao,
                COALESCE(TO_CHAR(atm.atmdtlimiteconclusao, 'dd/mm/YYYY')) AS atmdtlimiteconclusao
                  ";
    $objMonitoramento = new MonitoramentoEspecial();
    $dadosAtividade = $objMonitoramento->getDadosAtividade(array('atmid'=>$atmid),'array', $campos);
    $dadosAtividade = $dadosAtividade[0];
    
    ?>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
            <tr>
                <td class="SubTituloDireita" style="width: 40%;">UF:</td>
                <td colspan="2"> <?php echo $dadosAtividade['estuf']; ?> </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Município:</td>
                <td id="td_municipio">
                    <?php
                        $sql = "SELECT mundescricao FROM territorios.municipio  WHERE muncod = " . $dadosAtividade['muncod'] . " ";
                        $mundescricao = (!empty($dadosAtividade['muncod'])) ? $this->pegaUm($sql) : '';
                        echo $mundescricao;
                    ?>
                </td>
                <td rowspan="5"  align="right" valign="top" width="1">
                    <?php
//                        $docid = $dadosAtividade['docid'];
//                        if($docid){
//                            wf_desenhaBarraNavegacao($docid, array('atmid' =>  $atmid));
//                        }else{
//                            echo '&nbsp;';
//                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Entidade:</td>
                <td id="td_entidade" colspan="2">
                    <?php
                        $sql_ent = " SELECT e.entnome FROM entidade.entidade e WHERE e.entid = ".$dadosAtividade['entid'];
                        $entnome = (!empty($dadosAtividade['entid'])) ? $this->pegaUm($sql_ent) : '';
                        echo $entnome;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" style="width: 40%;">Obra:</td>
                <td id="td_obra" colspan="2">
                    <?php
                        $sql_obr = " SELECT '(' || obrid || ')' || obrnome as obrnome FROM obras2.obras e WHERE obrid = ".$dadosAtividade['obrid'];
                        $obrnome = (!empty($dadosAtividade['obrid'])) ? $this->pegaUm($sql_obr) : '';
                        echo $obrnome;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Origem da Atividade:</td>
                <td colspan="2"> <?php echo stripslashes($dadosAtividade['atmnome']); ?> </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Tipo da Atividade:</td>
                <td colspan="2">
                    <?php
                        $sqlTp = " SELECT tamdesc FROM obras2.tipoatividademonitoramento WHERE tamid = ".$dadosAtividade['tamid'];
                        $tamdesc = (!empty($dadosAtividade['tamid'])) ? $this->pegaUm($sqlTp) : '';
                        echo $tamdesc;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Subtipo da Atividade:</td>
                <td colspan="2">
                    <?php
                        $sqlTp = " SELECT samdesc FROM obras2.subtipoatividademonitoramento WHERE samid = ".$dadosAtividade['samid'];
                        $samdesc = (!empty($dadosAtividade['samid'])) ? $this->pegaUm($sqlTp) : '';
                        echo $samdesc;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Data Limite:</td>
                <td colspan="2"> <?php echo $dadosAtividade['atmdtlimiteconclusao']; ?> </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Observações:</td>
                <td colspan="2"> <?php echo stripslashes($dadosAtividade['atmobs']); ?> </td>
            </tr>
            <tr>
                <td colspan="3"> <?php
                        $objMonitoramento->montaListaTarefasMonitoramento(array('atmid'=> $dadosAtividade['atmid']), 'html');
                ?> </td>
            </tr>
        </table>
<?php  
}

?>