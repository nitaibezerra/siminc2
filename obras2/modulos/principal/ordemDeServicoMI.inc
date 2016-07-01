<?php
$osMi = new OrdemServicoMI();
$strSQL = $osMi->pegaTodasOSMI(true);

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
} 

switch ($_REQUEST['req']) {
    case 'cadastrarOsMi':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras( $obrid );
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/cadOsMi&acao=A&tomid='.$_POST['_tomid'].'&osmid='.$_POST['osmid']);
        die();
    case 'supervisorFNDE':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['empid'] = $_POST['empid'];
        header("Location: obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A");
        exit;
}

if (count($_POST) > 0) {
    $where = array();
    
    if (!empty($_POST['obrid'])) {
        $where[] = " ( UPPER(o.obrnome) ILIKE UPPER('%" . $_POST['obrid'] . "%') OR
            o.obrid::CHARACTER VARYING ILIKE UPPER('%" . $_POST['obrid'] . "%') ) ";
    }
    
    if (!empty($_POST['emiid'])) {
//        array_push($where, " foo.emiid = '{$_POST['emiid']}' ");
        array_push($where, " emi.emiid = '{$_POST['emiid']}' ");
    }
    
    if (!empty($_POST['os_reprovada'])) {
        switch ($_POST["os_reprovada"]) {
            case 'S':
                array_push($where, "d.esdid = ".ESDID_OS_MI_REPROVADA."");
                break;
            case 'N':
                array_push($where, "d.esdid != ".ESDID_OS_MI_REPROVADA."");
                break;
        }
    }
    
    if (!empty($_POST['esdid'])) {
        array_push($where, " ed.esdid = '{$_POST['esdid']}' ");
    }
    
    if (!empty($_POST['tobid'])) {
        array_push($where, " o.tobid = '{$_POST['tobid']}' ");
    }
    
    if (!empty($_POST['esdid'])) {
        array_push($where, "ed.esdid = '{$_POST['esdid']}' ");
    }
    
    if (!empty($_POST['estuf'])) {
        array_push($where, " edo.estuf = '{$_POST['estuf']}' ");
    }
    
    if (!empty($_POST['tomid'])) {
        array_push($where, " os.tomid = '{$_POST['tomid']}' ");
    }
    
    if (!empty($_POST['muncod'])) {
        $muncod  = (array) $_POST['muncod'];
        $where[] = "mun.muncod IN('" . implode("', '", $muncod) . "')";
    }

    if (!empty($_POST['esdidobr'])) {
        array_push($where, "doc.esdid = '{$_POST['esdidobr']}' ");
    }
    
    if (isset($_POST['sobid']) && $_POST['sobid'] == 1) {
        if (!empty($_POST['obr_atrasada'])) {
            switch ($_POST['obr_atrasada']) {
                case 1: //sim
                    $where[] = "os.osmdttermino < NOW()"; //data término no passado
                    break;
                case 2: //não
                    $where[] = "os.osmdttermino > NOW()"; //data de término no futuro
                    break;
                case 3: //todos
                default:
            }
        }
    }
    
    if (!empty($where)) {
        $strSQL = $osMi->pegaTodasOSMI(true, $where);
    }
}

if ($_POST['xls']) {
    
    ob_clean();
    $arquivo = 'ordem_servico_mi';
    $cabecalho = array('ID Obra', 'Tipo OS', 'Situação OS', 'UF', 'Município', 'Empresa', 'Nome da obra', "Situação Obra", 'Data Cadastro', 'Data Início', 'Data Término', 'Prazo', 'Dias em execução');
    $dados = $db->carregar($strSQL);
    if ($dados) {
        foreach ($dados as &$row) {
            unset($row['acao'], $row['os']);
            $row['tomdsc'] = strip_tags($row['tomdsc']);
            $row['osmdttermino'] = strip_tags($row['osmdttermino']);
        }

        $db->sql_to_xml_excel($dados, $arquivo, $cabecalho);
        die;
    }
}

extract($_POST);
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo('Lista de OS MI', 'Filtre a ordem de serviço');
?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
function carregarMunicipio(estuf) {
    
    var td = $("#td_municipio")
      , url = location.href;
    
    if (!estuf) {
        td.find("select option:first").attr("selected", true);
        td.find("select").attr("selected", true).attr("disabled", true);
        return false;
    }
    
    $.ajax({
        url: url,
        type: "POST",
        data: {ajax: "municipio", estuf: estuf},
        dataType: "html",
        async: false,
        beforeSend: function() {
            divCarregando();
            td.find("select option:first").attr("selected", true);
        },
        error: function() {
            divCarregado();
        },
        success: function(data) {
            td.html(data);
            divCarregado();
        }
    });
}
function cadastrarOS(obrid , tomid, osmid){
    $("#req").val("cadastrarOsMi");
    $("#obrid").val(obrid);
    $("#_tomid").val(tomid);
    $("#osmid").val(osmid);

    janela("" ,800, 650, "cadastrarOsMi");	

    $("#formListaObra").attr("target", "cadastrarOsMi");
    $("#formListaObra").submit();
}

function abreEvolucaoFinan(obrid) {
    janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=' + obrid, 800, 650);
}

function alterarObr(obrid) {
    location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function abreListaSupervisaoFnde( obrid, empid ){
    $('[name=req]').val( 'supervisorFNDE' );
    $('[name=obrid]').val( obrid );
    $('[name=empid]').val( empid );
    $('#formListaObra').submit();
}

$(function(){
    $("#esdid").on("change", function(){
        
        //console.log($(this).val());
        
        if ($(this).val() == 905) {
            $("tr.filter_range").show();
        } else {
            $("tr.filter_range").hide();
        }
    });
    
    $("input[name='btn_salvar']").on("click", function(){
        
        /*if ( $("#sobid").val() == 1 ) {
            if (!$("#inicioos_i").val()) {
                alert("Campo de preenchimento obrigatório");
                $("#inicioos_i").focus();
                return false;
            }

            if (!$("#inicioos_f").val()) {
                alert("Campo de preenchimento obrigatório");
                $("#inicioos_f").focus();
                return false;
            }

            if (!$("#terminoos_i").val()) {
                alert("Campo de preenchimento obrigatório");
                $("#terminoos_i").focus();
                return false;
            }

            if (!$("#terminoos_f").val()) {
                alert("Campo de preenchimento obrigatório");
                $("#terminoos_f").focus();
                return false;
            }
        }*/
        
        $("#frmOrdemOSMI").submit();
    });
    
    $("input[name='btn_export_xls']").on("click", function(){
        $("#xls").val(1);
        $("#frmOrdemOSMI").submit();
    });
})
</script>

<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="osmid" id="osmid" value="">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="_tomid" id="_tomid" value="">
</form>

<form name="frmOrdemOSMI" id="frmOrdemOSMI" method="post">
    <input type="hidden" name="xls" id="xls" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="35%">Empresa</td>
            <td>
                <?php
                                $empresami = new EmpresaMi();
                                $sql = $empresami->listaCombo(false);
//                                $sql = $empresami->listaComboComEstuf();
                                $db->monta_combo("emiid", $sql, 'S', "Selecione...", "", '', '', '', 'N', 'emiid');
                            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="35%">Nome da Obra/ID</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo da OS</td>
            <td>
                <?php 
                $tipo = "SELECT tomid as codigo, tomdsc as descricao FROM obras2.tipoosmi WHERE tomstatus = 'A'";
                $db->monta_combo('tomid', $tipo, 'S', 'Todos', '', '', '', 200, 'N', 'tomid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo da Obra</td>
            <td>
                <?php
                    $tipoObra = new TipoObra();
                    $db->monta_combo('tobid', $tipoObra->listaCombo(), 'S', 'Todos', '', '', '', 200, 'N', 'tobid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">UF</td>
            <td>
                <?php
                    $uf = new Estado();
                    $db->monta_combo('estuf', $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Município</td>
            <td id="td_municipio">
                <?php
                    if ($estuf) {
                        $municipio = new Municipio();
                        $dado = $municipio->listaCombo(array('estuf' => $estuf));
                        $habMun = 'S';
                    } else {
                        $dado = array();
                        $habMun = 'N';
                    }
                    $habMun = ($disable == 'N' ? $disable : $habMun);
                    echo $db->monta_combo('muncod', $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
                ?>
            </td>
        </tr>
        <?php $stateContainer = ($esdid == ESDID_OS_MI_EXECUCAO /*905*/) ? 'style="display:;"' : 'style="display:none;"' ?>
        <tr class="filter_range" <?=$stateContainer?>>
            <td class="SubTituloDireita">OS atrasada</td>
            <td>
                <input type="radio" name="obr_atrasada" value="1" <?= ($_REQUEST['obr_atrasada']==1) ? 'checked="checked"':'' ?> />&nbsp;Sim&nbsp;
                <input type="radio" name="obr_atrasada" value="2" <?= ($_REQUEST['obr_atrasada']==2) ? 'checked="checked"':'' ?> />&nbsp;Não&nbsp;
                <input type="radio" name="obr_atrasada" value="3" <?= ($_REQUEST['obr_atrasada']==3 || empty($_REQUEST['obr_atrasada'])) ? 'checked="checked"':'' ?> />&nbsp;Todas&nbsp;
            </td>
        </tr>
        <tr class="filter_range" <?=$stateContainer?>>
            <td class="SubTituloDireita">Inicio da OS</td>
            <td>de&nbsp;
                <?php echo campo_data2('inicioos_i', 'S', true, '', null, '', ' ', '', ''); ?>
                &nbsp;até&nbsp;
                <?php echo campo_data2('inicioos_f', 'S', true, '', null, '', ' ', '', ''); ?>
            </td>
        </tr>
        <tr class="filter_range" <?=$stateContainer?>>
            <td class="SubTituloDireita">Término da OS</td>
            <td>
                de&nbsp;
                <?php echo campo_data2('terminoos_i', 'S', true, '', null, '', ' ', '', ''); ?>
                &nbsp;até&nbsp;
                <?php echo campo_data2('terminoos_f', 'S', true, '', null, '', ' ', '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação da OS</td>
            <td>
                <?php 
                $situacaoOS = "select esdid AS codigo, esddsc AS descricao 
                               from workflow.estadodocumento 
                               where esdstatus = 'A' 
                                 and tpdid     = ".TPDID_OS_MI." 
                               order by esdordem ASC";
                
                $db->monta_combo('esdid', $situacaoOS, 'S', 'Todos', '', '', '', 200, 'N', 'esdid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação da Obra:</td>
            <td>
                <?php
                $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                $db->monta_combo("esdidobr", $sql, "S", "Todos", "", "", "", "200", "N", "esdidobr");
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita"  style="width: 45%">O.S. Reprovada ?</td>
            <td>
                <?php
                    switch ($_POST["os_reprovada"]) {
                        case 'S':
                            $os_reprovada_s = 'checked="checked"';
                            $os_reprovada_n = '';
                            $os_reprovada_t = '';
                            break;
                        case 'N':
                            $os_reprovada_n = 'checked="checked"';
                            $os_reprovada_s = '';
                            $os_reprovada_t = '';
                            break;
                        case 'T':
                            $os_reprovada_t = 'checked="checked"';
                            $os_reprovada_s = '';
                            $os_reprovada_n = '';
                            break;
                        default:
                            $os_reprovada_n = 'checked="checked"';
                            $os_reprovada_s = '';
                            $os_reprovada_t = '';
                            break;
                    }
                ?>
                <input type="radio" name="os_reprovada" id="" value="S" <?php echo $os_reprovada_s;?> > Sim
                <input type="radio" name="os_reprovada" id="" value="N" <?php echo $os_reprovada_n;?> > Não
                <input type="radio" name="os_reprovada" id="" value="T" <?php echo $os_reprovada_t;?> > Todas
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloCentro">
                <input type="submit" value="Filtrar" name="btn_salvar">&nbsp;&nbsp;
                <input type="submit" value="Exportar XLS" name="btn_export_xls">
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2" align="center">&nbsp;</td>
        </tr>
    </table>
</form>
<?php 
$_param = array('managerOrder' => array(
    10 => 'osmdtcadastro'
  , 11 => 'osmdtinicio'
  , 12 => 'osmdttermino'
));
$cabecalho = array('Ação', 'Id Obra', 'Tipo OS', 'OS','Situação OS', 'UF', 'Município', 'Empresa', 'Nome da obra',"Situação Obra", 'Data Cadastro', 'Data Início', 'Data Término', 'Prazo', 'Dias em execução');
$db->monta_lista($strSQL, $cabecalho, 100, 5, 'N', 'center', 'N', 'formulario', '', '', null, $_param); 
?>
