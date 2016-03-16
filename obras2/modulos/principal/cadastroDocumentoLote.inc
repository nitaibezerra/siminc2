<?php

function apagarArquivo($arqid) {
    global $db;
    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    $file = new FilesSimec('arquivo', null, 'obras2');
    $file->excluiArquivoFisico($arqid);
    $db->executar("delete from public.arquivo where arqid = {$arqid}");
}

if (isset($_GET['estuf'])) {
    if (empty($_GET['estuf'])) {
        $querySQL = 'SELECT NULL';
    } else {
        $municipio = new Municipio();
        $querySQL = $municipio->listaMunicipiosMultiplosUfs($_GET);
    }
}

if (isset($_GET['arqid']) && isset($_GET['cancelar'])) {
    apagarArquivo((int) $_GET['arqid']);
    unset($_SESSION['registro']);
    die("<script>
            alert('Operação cancelada com sucesso!');
            window.location.href = window.location.href;
     </script>");
}

//unset($_SESSION['registro']);

if (isset($_POST['oardesc']) || $_POST['rgadscsimplificada'] || $_POST['obrid']) {
    if (!isset($_SESSION['registro']['post'])) {

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $_SESSION['registro']['post'] = $_POST;

        $file = new FilesSimec('arquivo', null, 'obras2');
        if ($_FILES['arquivo']['name']) {

            $file->setPasta('obras2');
            $file->setUpload($_POST['oardesc'], 'arquivo', false);
            $arqid = $file->getIdArquivo();
            if ($arqid) {
                $_SESSION['registro']['post']['arqid'] = $arqid;
            }
        }

        //Cad arquivo Resgistro de Atividade
        unset($file);
        $file = new FilesSimec('arquivo_ra', null, 'obras2');
        if ($_FILES['arquivo_ra']['name']) {

            $file->setPasta('obras2');
            $file->setUpload($_POST['rgadscsimplificada'], 'arquivo_ra', false);
            $arqid = $file->getIdArquivo();
            if ($arqid) {
                $_SESSION['registro']['post']['arqid_ra'] = $arqid;
            }
        }

    } else {

        $obrasArquivos = new ObrasArquivos();
        $arDado = $_SESSION['registro']['post'];

        if($_POST['obrid']){

            foreach ($_POST['obrid'] as $obrid) {

                $arDado['obrid'] = $obrid;

                $arCamposNulo = array();
                if (empty($arDado['oardesc'])) {
                    $arDado['oardesc'] = null;
                    $arCamposNulo[] = 'oardesc';
                }
                if (empty($arDado['tpaid'])) {
                    $arDado['tpaid'] = null;
                    $arCamposNulo[] = 'tpaid';
                }
                if (empty($arDado['arqid'])) {
                    $arDado['arqid'] = null;
                    $arCamposNulo[] = 'arqid';
                }
                if (empty($arDado['oardata'])) {
                    $arDado['oardata'] = "now()";
                    $arCamposNulo[] = 'oardata';
                }

                $obrasArquivos->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);
                $obrasArquivos->clearDados();
            }

            if($_SESSION['registro']['post']['rd_registro_atividade'] == 'S'){
                unset($arDado);
                unset($arCamposNulo);

                $regAtividade = new RegistroAtividade();

                $arDado['rgadscsimplificada'] = $_SESSION['registro']['post']['rgadscsimplificada'];
                $arDado['rgadsccompleta']     = $_SESSION['registro']['post']['rgadsccompleta'];
                $arDado['arqid']              = $_SESSION['registro']['post']['arqid_ra'];
                $arDado['usucpf']             = $_SESSION['usucpf'];

                foreach ($_POST['obrid'] as $obrid) {

                    $arDado['obrid'] = $obrid;

                    $arCamposNulo = array();
                    if (empty($arDado['rgadscsimplificada'])) {
                        $arDado['rgadscsimplificada'] = null;
                        $arCamposNulo[] = 'rgadscsimplificada';
                    }
                    if (empty($arDado['rgadsccompleta'])) {
                        $arDado['rgadsccompleta'] = null;
                        $arCamposNulo[] = 'rgadsccompleta';
                    }
                    if (empty($arDado['arqid'])) {
                        $arDado['arqid'] = null;
                        $arCamposNulo[] = 'arqid';
                    }

                    $regAtividade->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);
                    $regAtividade->clearDados();
                }
            }
        }
        $db->commit();

        unset($_SESSION['registro']);
        die("<script>
                alert('Operação realizada com sucesso!');
                window.location.href = window.location.href;
         </script>");
    }

}

//Chamada de programa
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Cadastro de documento em massa', '');
?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>

<?php if (!isset($_SESSION['registro']['post'])) : ?>
<form method="post" name="formCadDocumento" id="formCadDocumento" enctype="multipart/form-data">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita">Nº Processo/Convênio:</td>
            <td>
                <?php
                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Esfera:</td>
            <td>
                <input type='radio' id='' name='esfera' value='M' <?= ( $_POST["esfera"] == "M" ? "checked='checked'" : "" ) ?>/> Municipal
                <input type='radio' id='' name='esfera' value='E' <?= ( $_POST["esfera"] == "E" ? "checked='checked'" : "" ) ?>/> Estadual
            </td>
        </tr>

        <?php
        $strSQL = "SELECT estuf AS codigo, estuf || ' - ' || estdescricao AS descricao FROM territorios.estado ORDER BY estuf";
        $rsUfs = $db->carregar($strSQL);
        if ($rsUfs) {
            $checkOptions = '<ul class="ufs" style="margin:0;padding:0;display:block;width:700px;">';
            foreach ($rsUfs as $uf) {
                $checkOptions .= '<li style="float:left;margin-right:5px;font-size:11px;width:155px;height:15px;padding:5px 5px;">';
                $checkOptions .= '<input type="checkbox" class="selectaUf" name="estuf[]" id="'.$uf['codigo'].'" value="'.$uf['codigo'].'" />&nbsp;<label for="'.$uf['codigo'].'">'.$uf['descricao'].'</label>';
                $checkOptions .= '</li>';
            }
            $checkOptions .= '</ul>';
        }
        ?>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">UF:</td>
            <td><?php echo $checkOptions; ?></td>
        </tr>
        <?php
        if (empty($querySQL)) {
            $querySQL = 'SELECT NULL';
        }
        mostrarComboPopup('Município', 'muncod', $querySQL, null, '', null, null, false, false, 1, false, true);
        ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Descrição:</td>
            <td><?php echo campo_texto('oardesc','S','S','',43,100,'','', '', '', '', 'id="oardesc"', '');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo:</td>
            <td>
            <?php
                $tipoArquivo = new TipoArquivo();
                $sql = $tipoArquivo->listaCombo();
                $db->monta_combo("tpaid", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpaid');
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Arquivo:</td>
            <td>
                <input type="file" name="arquivo" id="arquivo" class="obrigatorio"/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <!-- Registro de Atividade -->
        <tr>
            <td class="SubTituloDireita">Registro de Atividade:</td>
            <td>
                <input type="radio" name="rd_registro_atividade" id="rd_registro_atividade_s" value="S">
                <label for="rd_registro_atividade_s"> Sim </label>
                <br />
                <input type="radio" name="rd_registro_atividade" id="rd_registro_atividade_n" value="N" checked="checked">
                <label for="rd_registro_atividade_n"> Não </label>
            </td>
        </tr>

        <tr id="tr_ra_1" style="display: none">
            <td class="SubTituloDireita" style="width: 190px;">Descrição simplificada:</td>
            <td> <?php echo campo_textarea('rgadscsimplificada', 'S', 'S', '', 80, 4, 250); ?> </td>
        </tr>

        <tr id="tr_ra_2" style="display: none">
            <td class="SubTituloDireita" style="width: 190px;">Descrição detalhada:</td>
            <td> <?php echo campo_textarea('rgadsccompleta', 'S', 'S', '', 80, 6, 5000);	?> </td>
        </tr>

        <tr  id="tr_ra_3" style="display: none">
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo do Registro de Atividades:</td>
            <td>
                <input type="hidden" name="arqid_ra" id="arqid_ra" value=""/>
                <input type="file" name="arquivo_ra" id="arquivo_ra"/>
                <br>
            </td>
        </tr>
        <!-- /Registro de Atividade -->
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Avançar"/>
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
function onOffCampo(campo) {
    var div_on  = document.getElementById(campo + '_campo_on');
    var div_off = document.getElementById(campo + '_campo_off');
    var input   = document.getElementById(campo + '_campo_flag');
    if (div_on.style.display == 'none') {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    } else {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}

$(function(){
    jQuery("input[name='pesquisar']").on("click", function(){

        if(jQuery('input[name=processo]').val() == '') {
            if (!jQuery("input.selectaUf").is(":checked")) {
                alert("Voce precisa selecionar pelo menos uma UF");
                return false;
            }
        }

//        if (jQuery("#muncod option").length==1 && !jQuery("#muncod option").attr("value")) {
//            alert("Voce precisa selecionar pelo menos um município");
//            return false;
//        }

        if (!jQuery("#oardesc").val()) {
            alert("Campo Descrição precisa ser preechido!");
            jQuery("#rgadscsimplificada").focus();
            return false;
        }

        if (!jQuery("#tpaid").val()) {
            alert("Campo Tipo precisa ser preechido!");
            jQuery("#rgadsccompleta").focus();
            return false;
        }

        selectAllOptions(document.getElementById("muncod"));
        jQuery("#formCadDocumento").submit();
    });

    jQuery("input[name='pesquisarObras']").on("click", function(){

        if (!jQuery("input.checkboxChildren").is(":checked")) {
            alert("Voce precisa selecionar pelo menos uma obra");
            return false;
        }

        jQuery("#formFiltroObra").submit();
    });

    jQuery("input.chackboxParent").on("click", function(){
        if ($(this).is(":checked")) {
            $("input.checkboxChildren").each(function(i, element){
                $(element).attr("checked", true);
            });
        } else {
            $("input.checkboxChildren").each(function(i, element){
                $(element).attr("checked", false);
            });
        }
    });
});

jQuery("#rd_registro_atividade_s").on("click", function(){
    jQuery("#tr_ra_1").fadeIn();
    jQuery("#tr_ra_2").fadeIn();
    jQuery("#tr_ra_3").fadeIn();
});

jQuery("#rd_registro_atividade_n").on("click", function(){
    jQuery("#tr_ra_1").fadeOut('fast');
    jQuery("#tr_ra_2").fadeOut('fast');
    jQuery("#tr_ra_3").fadeOut('fast');
});



jQuery("input.selectaUf").on("click", function(){

    var params = [];
    jQuery("input.selectaUf:checked").each(function(i, element){
        params.push($(element).val());
    });

    jQuery.ajax({
        url: window.location.href+"&estuf="+params.join(","),
        beforeSend:function(){},
        success:function(data){},
        complete:function(){},
        error:function(){}
    });
});
</script>
<?php else:


$whereEstuf = "";
if(isset($_SESSION['registro']['post']['estuf']) && is_array($_SESSION['registro']['post']['estuf'])){
    $whereEstuf = "'".implode("','", $_SESSION['registro']['post']['estuf'])."'";
}
$whereMuncod = "";
if(isset($_SESSION['registro']['post']['muncod']) && is_array($_SESSION['registro']['post']['muncod'])){
    $whereMuncod = "'".implode("','", $_SESSION['registro']['post']['muncod'])."'";
}


if (strlen($whereEstuf) && !empty($_SESSION['registro']['post']['estuf'][0])) {
    $_whereEstuf = " ent.estuf IN ({$whereEstuf}) AND ";
}

if (strlen($whereMuncod) &&  !empty($_SESSION['registro']['post']['muncod'][0])) {
    $_whereMuncod = " ent.muncod IN ({$whereMuncod}) AND ";
}

$_POST["esfera"] = (empty($_POST["esfera"])) ? 'M' : $_POST["esfera"];
if (!empty($_POST['esfera'])) {
    $_whereEsfera = "  e.empesfera = '{$_POST['esfera']}' AND";
}

if (!empty($_SESSION['registro']['post']['processo'])) {
    $_whereProcesso = "  CASE WHEN o.tooid = 2 AND TRIM(o.obrnumprocessoconv) != '' THEN
                                to_char(Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN po1.pronumeroprocesso IS NOT NULL THEN
                                to_char(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN po2.pronumeroprocesso IS NOT NULL THEN
                                to_char(Replace(Replace(Replace(po2.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            END ILIKE to_char(Replace(Replace(Replace('{$_SESSION['registro']['post']['processo']}','.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') AND";
}

if ($_whereEstuf || $_whereProcesso) {
    $strSQLObras = "SELECT
                        '<center><input type=\"checkbox\" class=\"checkboxChildren\" value=\"'||o.obrid||'\" name=\"obrid[]\"/></center>' as acao,
                        o.obrid, o.obrnome
                    FROM
                        obras2.obras o
                        INNER JOIN entidade.endereco ent ON (ent.endid = o.endid)
                        INNER JOIN obras2.empreendimento e ON e.empid = o.empid

                        LEFT JOIN par.processoobrasparcomposicao pop1 ON pop1.preid = o.preid
                        LEFT JOIN par.processoobraspar po1 on po1.proid = pop1.proid and po1.prostatus = 'A'

                        LEFT JOIN par.processoobraspaccomposicao pop2 ON pop2.preid = o.preid
                        LEFT JOIN par.processoobra po2 on po2.proid = pop2.proid and po2.prostatus = 'A'
                    WHERE
                        {$_whereEstuf}
                        {$_whereMuncod}
                        {$_whereEsfera}
                        {$_whereProcesso}
                        o.obrstatus = 'A' AND o.obridpai IS NULL ORDER BY o.obrid DESC";
//    echo $strSQLObras; die;
    $checkboxParent = "<center><input type=\"checkbox\" class=\"chackboxParent\" value=\"\" name=\"obridPai[]\"/></center>";
    $cabecalho = array($checkboxParent, 'ObraID', 'Obra nome');
    echo '<form method="post" name="formFiltrarObra" id="formFiltrarObra">';
    echo '<input type="hidden" name="registrar" value="1"/>';
    echo '<input type="hidden" name="_arqid" id="_arqid" value="'.$_SESSION['registro']['post']['arqid'].'"/>';
    echo '<div style="width:97%; height:300px; overflow:auto;padding:2px 2px;">';
    $db->monta_lista($strSQLObras, $cabecalho, 100000000, 5, 'N', 'center', 'N', '', null, null, null, array('ordena' => false));
    echo '</div>';
    echo '<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">';
    echo '<tr>';
    echo '<td>';
    echo '<input type="button" name="salvar" class="pesquisar" value="Salvar"/>&nbsp;&nbsp;';
    echo '<input type="button" name="cancelar" id="cancelar" class="pesquisar" value="Cancelar"/>';
    echo '</td>';
    echo '</tr>';
    echo '</table>';
    echo '</form>';
}
?>
<script type="text/javascript">
jQuery(function(){
    jQuery("input[name='salvar']").on("click", function(){

        if (!jQuery("input.checkboxChildren").is(":checked")) {
            alert("Voce precisa selecionar pelo menos uma obra");
            return false;
        }

        jQuery("#formFiltrarObra").submit();
    });

    jQuery("input.chackboxParent").on("click", function(){
        if ($(this).is(":checked")) {
            $("input.checkboxChildren").each(function(i, element){
                $(element).attr("checked", true);
            });
        } else {
            $("input.checkboxChildren").each(function(i, element){
                $(element).attr("checked", false);
            });
        }
    });

    jQuery("#cancelar").on("click", function(){
        if (confirm("Voce tem certeza que deseja cancelar essa ação?")) {

            jQuery.ajax({
                url: window.location.href+"&cancelar=true&arqid="+jQuery("#_arqid").val(),
                success:function(data) {
                   jQuery("body").append(data);
                }
            });

        }
    });
});
</script>
 <?php endif;