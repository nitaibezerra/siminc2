<?php
if ($_REQUEST['form_lista'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls') {
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->montaListaAtividadesMonitoramento($_POST, $_REQUEST['tipo_relatorio']);
    exit;
}

switch ($_REQUEST['ajax']) {
    case 'municipio':
        carrega_campo_municipio();
        reset_campos_ajax(array('entidade'));
        exit;
    case 'ajax_carrega_campo_entidade':
        carrega_campo_entidade();
        die;
    case 'ajax_carrega_campo_obras':
        carrega_campo_obras();
        die;
}

include APPRAIZ . "includes/cabecalho.inc";

$arMenuBlock = array();
$id_aba = ID_ABA_MONITORAMENTO_ESPECIAL;
$db->cria_aba($id_aba, $url, $parametros, $arMenuBlock);

$titulo_modulo = 'Lista de Atividades de Monitoramento';
$subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
monta_titulo($titulo_modulo, $subtitulo_modulo);

unset($_SESSION['obras2']['itmid']);
unset($_SESSION['obras2']['atmid']);

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<style>
    .div_abas_ajax{width:99%;background-color: #FFFFFF;overflow:fixed;}
</style>
<script type="text/javascript">

    function getEstados() {
        var estados  = '';
        var elemento = document.getElementsByName('slEstado[]')[0];
        for (var i = 0; i < elemento.options.length; i++) {
            if (elemento.options[i].value != ''){
                estados += "'" + elemento.options[i].value + "',";
            }
        }
        return estados;
    }

    function ajaxEstado() {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                $('#idMunicipio').html(retorno);
            }});
    }
    
    function alterarAtividade(atmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid='+atmid;
    }
    
    function duplicarAtividade(atmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid='+atmid+'&requisicao=duplicarAtividade';
    }
    
    function visualizarAtividade(atmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A&atmid='+atmid+'&bloqkEdicao=true';
    }
    
    function incluirTarefaAtividade(atmid){
        window.location.href = '?modulo=principal/monitoramentoespecial/cadTarefaMonitoramento&acao=A&atmid='+atmid;
    }
    
    function excluirAtividade(atmid){
        var r = confirm("Você tem certeza que deseja excluir a Atividade "+atmid+" ?");
        if (r == true) {
            var url = 'obras2.php?modulo=principal/monitoramentoespecial/cadAtividadeMonitoramento&acao=A';
            $.ajax({
                url: url,
                type: 'post',
                data: {atmid: atmid, requisicao: 'excluir'},
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
                    window.location.href = 'obras2.php?modulo=principal/monitoramentoespecial/listaAtividadeMonitoramento&acao=A';
                }
            });
        }
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

</script>

<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form_lista" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="subtituloDireita" style="width: 40%;">Nome da Obra/ID:</td>
            <td>
                <?php
                    $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                    echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" style="width: 40%;">CNPJ:</td>
            <td>
                <?php
                    $val = (!empty($_POST['cnpj'])) ? $_POST['cnpj'] : '';
                    echo campo_texto('cnpj', 'N', 'S', '', 70, 80, '##.###.###/####-##', '', '', '', '', 'id="cnpj"', '', $val);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Origem da Atividade:</td>
            <td>
                <?php
                    $val = (!empty($_POST['atmnome'])) ? $_POST['atmnome'] : '';
                    echo campo_texto('atmnome', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="atmnome"', '', $val);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Tipo da Atividade:</td>
            <?php
                $_POST['tamid'] = !empty($_POST['tamid']) ? $_POST['tamid'] : '';
                $sqlTp = " SELECT
                                    tamid as codigo, tamdesc as descricao
                               FROM obras2.tipoatividademonitoramento 
                               ORDER BY tamdesc ";

                echo '<td>';
                $db->monta_combo("tamid", $sqlTp, 'S', 'Selecione...', '', '', '', 200, 'N', 'tamid', false, $_POST['tamid']);
                echo '</td>';
            ?>
        </tr>
        <tr>
            <td class="subtituloDireita">Subtipo da Atividade:</td>
            <?php
                $_POST['samid'] = !empty($_POST['samid']) ? $_POST['samid'] : '';
                $sqlStp = " SELECT
                                    samid as codigo, samdesc as descricao
                               FROM obras2.subtipoatividademonitoramento
                               ORDER BY samdesc
                                                 ";

                $sqlStp = $sqlStp;
                echo '<td>';
                $db->monta_combo("samid", $sqlStp, 'S', 'Selecione...', '', '', '', 200, 'N', 'samid', false, $_POST['samid']);
                echo '</td>';
            ?>
        </tr>
        <tr>
            <td class="subtituloDireita">Situação da Atividade:</td>
            <td>
                <?php
                    $sql = ' 
                            SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = '.TPID_ME_ATIVIDADE_MONITORAMENTO.' 
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
                       AND usuariosistema.sisid = '".$_SESSION['sisid']."' 
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
                                       AND usuariosistema.sisid  = '".$_SESSION['sisid']."' 
                                           {$str_colecao}
                                     ORDER BY usuario.usunome";
            }
            mostrarComboPopup('Responsável pelo Cadastro:', 'usucpf_inclusao', $sql, $stSqlCarregados, 'Selecione o(s) Responsável(eis)', null, '', false);
            ?>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 40%;">UF:</td>
            <td>
                <?php
                    $_POST['estuf'] = !empty($_POST['estuf']) ? $_POST['estuf'] : '';
                    $sqlUf = "SELECT
                                    estuf AS codigo,
                                    estuf || ' - ' || estdescricao AS descricao
                            FROM
                                    territorios.estado
                            ORDER BY
                                    estuf";
                    $db->monta_combo("estuf", $sqlUf, 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'S', 'estuf', false, $_POST['estuf']);
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
        </tr>
        <tr>
            <td class="subtituloDireita">Entidade:</td>
            <td id="td_entidade">
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
            <td class="subtitulodireita" width="190px">Data de Cadastro:</td>
            <td>
                <?php
                    $data_de = (empty($_POST['atmdtinclusao_de'])) ? '' : $_POST['atmdtinclusao_de'];
                    $data_ate = (empty($_POST['atmdtinclusao_ate'])) ? '' : $_POST['atmdtinclusao_ate'];
                ?>
                de: <input type="text" id="atmdtinclusao_de" name="atmdtinclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal" > 
                &nbsp;
                até: <input type="text" id="atmdtinclusao_ate" name="atmdtinclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>            
            <td class="subtitulodireita" width="190px">Data de Limite:</td>
            <td>
                <?php
                    $data_de = (empty($_POST['atmdtlimiteconclusao_de'])) ? '' : $_POST['atmdtlimiteconclusao_de'];
                    $data_ate = (empty($_POST['atmdtlimiteconclusao_ate'])) ? '' : $_POST['atmdtlimiteconclusao_ate'];
                ?>
                de: <input type="text" id="atmdtlimiteconclusao_de" name="atmdtlimiteconclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal" > 
                &nbsp;
                até: <input type="text" id="atmdtlimiteconclusao_ate" name="atmdtlimiteconclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>
            <td colspan="2" class="subtituloDireita">&nbsp;</td>
        </tr>
        <tr>
            <td colspan="2">
                <center>
                    <input type="button" id="btn_filtrar" name="btn_filtrar" value="Filtrar"      onclick="javascript:getLista('visual');"/>
                    <input type="button" id="btn_excel"   name="btn_excel"   value="Exportar XLS" onclick="javascript:getLista('xls');"/>
                </center>
            </td>
        </tr>
    </table>
</form>

<script lang="javascript">
    setTimeout(function() {

        $('#atmdtinclusao_de').mask('99/99/9999');
        $('#atmdtlimiteconclusao_de').mask('99/99/9999');
        $('#atmdtinclusao_ate').mask('99/99/9999');
        $('#atmdtlimiteconclusao_ate').mask('99/99/9999');

        $("#atmdtinclusao_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        $("#atmdtinclusao_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        $("#atmdtlimiteconclusao_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });
        $("#atmdtlimiteconclusao_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });

    }, 500);
</script>

<?php

if ($_REQUEST['form_lista'] == '1') {
    $objMonitoramento = new MonitoramentoEspecial();
    $objMonitoramento->montaListaAtividadesMonitoramento($_POST, $_REQUEST['tipo_relatorio']);
}

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
    header('content-type: text/html; charset=ISO-8859-1');
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

?>