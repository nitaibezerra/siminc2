<?php
$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];

if ($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls') {
    montaLista($_REQUEST['tipo_relatorio']);
    exit;
}
?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<?php
include APPRAIZ . "includes/cabecalho.inc";
print '<br/>';
$titulo_modulo = 'Lista de Checklist FNDE';
$subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
monta_titulo($titulo_modulo, $subtitulo_modulo);

//Fluxo de Restrição/Inconformidade
$tpdid = TPDID_RESTRICAO_INCONFORMIDADE;
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<script type="text/javascript">

    function getEstados() {
        var estados = '';

        var elemento = document.getElementsByName('slEstado[]')[0];

        for (var i = 0; i < elemento.options.length; i++) {
            if (elemento.options[i].value != '')
            {
                estados += "'" + elemento.options[i].value + "',";
            }
        }
        return estados;
    }

    function ajaxEstado() {
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                jQuery('#idMunicipio').html(retorno);
            }});
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

    jQuery(function($) {
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

    <input type="hidden" name="form" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="subtituloDireita">Nome da Obra/ID:</td>
            <td>
            <?php
                $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val);
            ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" style="width: 40%;">Tipologia:</td>
            <td>
            <?php
                $tipologiaObra = new TipologiaObra();
                $param         = array("orgid" => $_SESSION['obras2']['orgid']);
                $val           = (!empty($_POST['tpoid'])) ? $_POST['tpoid'] : '';
                $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid", false, $val);
            ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Tipo do Checklist:</td>
            <td>
            <?php
                $select_2p = '';
                $select_ad = '';
                $select_tc = '';
                $select_ov = '';
                $select_tt = '';
                switch ($_POST['item_checklist']) {
                    case '2P':
                        $select_2p = 'checked="checked"';
                        break;
                    case 'AD':
                        $select_ad = 'checked="checked"';
                        break;
                    case 'TC':
                        $select_tc = 'checked="checked"';
                        break;
                    case 'OV':
                        $select_ov = 'checked="checked"';
                        break;
                    case 'TT':
                        $select_tt = 'checked="checked"';
                        break;
                }
            ?>
                <input type="radio" name="item_checklist" id="item_checklist_2p" value="2P" <?php echo $select_2p; ?> > 2° Parcela
                <input type="radio" name="item_checklist" id="item_checklist_ad" value="AD" <?php echo $select_ad; ?> > Administrativo
                <input type="radio" name="item_checklist" id="item_checklist_tc" value="TC" <?php echo $select_tc; ?> > Técnico
                <input type="radio" name="item_checklist" id="item_checklist_tc" value="OV" <?php echo $select_ov; ?> > Obra Vinculada
                <input type="radio" name="item_checklist" id="item_checklist_tt" value="TT" <?php echo $select_tt; ?> > Todas
            </td>
        </tr>
        <tr>
        <?php
            $sql = " SELECT esdid as codigo, esddsc as descricao
                     FROM workflow.estadodocumento
                     WHERE  tpdid = " . TPID_CHECKLIST_VALIDACAO . " 
                     AND esdstatus = 'A'
                     ORDER BY esdid";
            $stSqlCarregados = '';
            $arr_ck_wf = array();
            
            if (!empty($_POST['esdid_ck']) && is_array($_POST['esdid_ck']) && $_POST['esdid_ck'][0] != '') {
                foreach ($_POST['esdid_ck'] as $key => $value) {
                    $arr_ck_wf[$key] = "'" . $value . "'";
                }
                $str_colecao = (!empty($arr_ck_wf)) ? " AND esdid IN (" . implode(',', $arr_ck_wf) . ") " : '';
                $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao
                                                        FROM workflow.estadodocumento
                                                        WHERE tpdid = " . TPID_CHECKLIST_VALIDACAO . " 
                                                        {$str_colecao}
                                                        ORDER BY
                                                                esdid";
            }
            mostrarComboPopup('Situação do Checklist:', 'esdid_ck', $sql, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
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
            mostrarComboPopup('Responsável pela análise:', 'usucpf', $sql, $stSqlCarregados, 'Selecione o(s) Responsável(eis)', null, '', false);
            ?>
        </tr>
        <tr>
        <?php
            $sql_obr = "SELECT esdid as codigo, esddsc as descricao 
                                        FROM workflow.estadodocumento 
                                        WHERE tpdid='" . TPDID_OBJETO . "' 
                                          AND esdstatus='A' 
                                        ORDER BY esdordem";
            $stSqlCarregados = '';
            $arr_obr_wf = array();
            if (!empty($_POST['esdid_obr']) && is_array($_POST['esdid_obr']) && $_POST['esdid_obr'][0] != '') {
                foreach ($_POST['esdid_obr'] as $key => $value) {
                    $arr_obr_wf[$key] = "'" . $value . "'";
                }
                $str_colecao = (!empty($arr_obr_wf)) ? " AND esdid IN (" . implode(',', $arr_obr_wf) . ") " : '';
                $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao 
                                                        FROM workflow.estadodocumento 
                                                        WHERE tpdid='" . TPDID_OBJETO . "' 
                                                          AND esdstatus='A' 
                                                          {$str_colecao}
                                                        ORDER BY
                                                                esdid";
            }
            mostrarComboPopup('Situação da Obra:', 'esdid_obr', $sql_obr, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
        ?>
        </tr>
        <tr id="idUF">
        <?php
            #UF
            $ufSql = " SELECT 	estuf as codigo, estdescricao as descricao
                                           FROM territorios.estado est
                                           ORDER BY estdescricao
                                         ";
            $stSqlCarregados = '';
            $arr_uf = array();
            if ($_POST['estuf'][0] != '') {
                foreach ($_POST['estuf'] as $key => $value) {
                    $arr_uf[$key] = "'" . $value . "'";
                }
                $stSqlCarregados = "SELECT
                                                                estuf as codigo, estdescricao as descricao
                                                        FROM territorios.estado est
                                                        WHERE 
                                                                estuf IN (" . implode(',', $arr_uf) . ")
                                                        ORDER BY
                                                                2";
            }
            mostrarComboPopup('UF:', 'estuf', $ufSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', null, 'ajaxEstado', false);
        ?>
        </tr>
        <tr id="idMunicipio">
            <?php
                #Municipio
                $munSql = " SELECT muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                    FROM territorios.municipio 
                                    ORDER BY
                                        mundescricao";
                $stSqlCarregados = '';
                $arr_muncod = array();
                if (is_array($_POST['muncod'])) {
                    foreach ($_POST['muncod'] as $key => $value) {
                        $arr_muncod[$key] = "'" . $value . "'";
                    }
                }

                $where_mun = (!empty($_POST['muncod'])) ? " muncod IN (" . implode(',', $arr_muncod) . ") " : '';
                $where_uf = (!empty($arr_uf)) ? " estuf IN (" . implode(',', $arr_uf) . ") " : '';

                $where = '';
                if (trim($where_mun) != '') {
                    $where .= $where_mun;
                }

                if (trim($where_uf) != '' && trim($where) !== '') {
                    $where .= ' AND ' . $where_uf;
                } elseif (trim($where_uf) !== '') {
                    $where .= $where_uf;
                }

                if (trim($where) !== '') {
                    $stSqlCarregados = "SELECT
                                                        muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                                FROM territorios.municipio
                                                WHERE 
                                                        {$where}
                                                ORDER BY
                                                        mundescricao";
                }
                mostrarComboPopup('Município:', 'muncod', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', false);
            ?>
        </tr>
            <?php
            // Programa
            $stSql = "SELECT
                    prfid AS codigo,
                    prfdesc AS descricao
                 FROM 
                    obras2.programafonte
                 ORDER BY
                    prfdesc ";
            if (!empty($_POST['prfid'][0])) {
                $stSqlSelecionados = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                     FROM 
                        obras2.programafonte
                     WHERE prfid IN (" . implode(', ', $_POST['prfid']) . ")
                     ORDER BY
                        prfdesc ";
            }
            mostrarComboPopup('Programa', 'prfid', $stSql, $stSqlSelecionados, 'Selecione o(s) Programa(s)');
            // Fonte
            $stSql = "SELECT
                    tooid AS codigo,
                    toodescricao AS descricao
                 FROM 
                    obras2.tipoorigemobra
                 WHERE
                    toostatus = 'A'
                 ORDER BY
                    toodescricao ";

            if (!empty($_POST['tooid'][0])) {
                $sql_carregados = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                     FROM 
                        obras2.tipoorigemobra
                     WHERE
                        toostatus = 'A'
                        AND tooid IN (" . implode(', ', $_POST['tooid']) . ")
                     ORDER BY
                        toodescricao ";
            }
            mostrarComboPopup('Fonte', 'tooid', $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)');
            ?>
        <tr>
            <td class="subtitulodireita" width="190px">Data de Cadastro:</td>
            <td>
                <?php
                    $data_de = (empty($_POST['ckfdatainclusao_de'])) ? '' : $_POST['ckfdatainclusao_de'];
                    $data_ate = (empty($_POST['ckfdatainclusao_ate'])) ? '' : $_POST['ckfdatainclusao_ate'];
                ?>
                de: <input type="text" id="ckfdatainclusao_de" name="ckfdatainclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal" > 
                &nbsp;
                até: <input type="text" id="ckfdatainclusao_ate" name="ckfdatainclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data da Situação:</td>
            <td>
                <?php
                    $data_de = (empty($_POST['htddata_de'])) ? '' : $_POST['htddata_de'];
                    $data_ate = (empty($_POST['htddata_ate'])) ? '' : $_POST['htddata_ate'];
                ?>
                de: <input type="text" id="htddata_de" name="htddata_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal"> 
                &nbsp;
                até: <input type="text" id="htddata_ate" name="htddata_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>
            <td class="subtituloDireita">Possui pendência ?</td>
            <td>
            <?php
                $select_s = '';
                $select_n = '';
                $select_t = '';
                switch ($_POST['possui_pendencia']) {
                    case 'S':
                        $select_s = 'checked="checked"';
                        break;
                    case 'N':
                        $select_n = 'checked="checked"';
                        break;
                    case 'T':
                //                        default:
                        $select_t = 'checked="checked"';
                        break;
            }
            ?>
                <input type="radio" name="possui_pendencia" id="possui_pendencia_s" value="S" <?php echo $select_s; ?> > Sim
                <input type="radio" name="possui_pendencia" id="possui_pendencia_n" value="N" <?php echo $select_n; ?> > Não
                <input type="radio" name="possui_pendencia" id="possui_pendencia_t" value="T" <?php echo $select_t; ?> > Todas
            </td>
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

        jQuery('#ckfdatainclusao_de').mask('99/99/9999');
        jQuery('#htddata_de').mask('99/99/9999');
        jQuery('#ckfdatainclusao_ate').mask('99/99/9999');
        jQuery('#htddata_ate').mask('99/99/9999');

        jQuery("#ckfdatainclusao_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        jQuery("#ckfdatainclusao_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        jQuery("#htddata_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });
        jQuery("#htddata_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });

    }, 500);
</script>

<?php
if ($_REQUEST['form'] == '1') {
    unset($_SESSION['obras2']['empid']);
    unset($_SESSION['obras2']['obrid']);
    montaLista($_REQUEST['tipo_relatorio']);
}

function montaLista($tipo) {
    global $db;
    $restricao = new ChecklistFnde();
    $arr = $restricao->getListaChecklistGeral('array');
    //Para o método $db->monta_lista, as chaves do array tem q ser crescente de 0 a X. Se algum número não seguir a ordem, dá erro.
    $arrFinal = array();
    foreach ($arr as $key => $value) {
        $arrFinal[] = $value;
        
    }
    $cabecalho = array("ID da Obra", "Nome da Obra", "UF", "Município", "Situação da Obra", "% Executado", /*"Ação",*/ "ID do Checklist", "Tipo", "Responsável",
                       "Data de Inclusão", "Situação", "Data da Situação", "Possui Pendências");
    if ($tipo == 'xls') {
        $cabecalho = array("ID da Obra", "Nome da Obra", "UF", "Município", "Situação da Obra", "% Executado", /*"Ação",*/ "ID do Checklist", "Tipo", "Responsável",
                           "Data de Inclusão", "Situação", "Data da Situação", "Possui Pendências");
        ob_clean();
        ini_set("memory_limit", "512M");
        header("Content-type: application/excel; name=Lista de Checklists FNDE.xls");
        header("Content-Disposition: attachment; filename=Lista de Checklists FNDE.xls");
        $db->sql_to_xml_excel($arr, 'ListadeChecklistsFNDE', $cabecalho);
    } else {
        $db->monta_lista($arr, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
    }
}
?>