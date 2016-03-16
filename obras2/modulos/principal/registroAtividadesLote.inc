<?php

function apagarArquivo($arqid) {
    global $db;
    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    $file = new FilesSimec('arquivo', null, 'obras2');
    $file->excluiArquivoFisico($arqid);
    $db->executar("delete from public.arquivo where arqid = {$arqid}");
}

//unset($_SESSION['_registro']);
switch (isset($_REQUEST['ajax'])) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo('muncod', $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
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
    unset($_SESSION['_registro']);
    die("<script>
            alert('Operação cancelada com sucesso!'); 
            window.location.href = window.location.href; 
     </script>");
}

if (isset($_POST['obrid']) || isset($_POST['registrar']) || isset($_POST['estuf']) || isset($_POST['processo']) || isset($_POST['esfera']) || isset($_POST['esfera'])) {
    extract($_POST);
    
    if ($registrar == 1) {
        
        $regAtividade = new RegistroAtividade();
        $arDado = $_SESSION['_registro']['post'];
        $arqid = null;
        
        $arDado['usucpf'] = $_SESSION['usucpf'];
        
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

        $db->commit();
        
        //ver($_POST['obrid']); die;
        unset($_SESSION['_registro']);
        die("<script>
                alert('Operação realizada com sucesso!'); 
                window.location.href = window.location.href; 
         </script>");
        
    } else {
        if (!empty($rgadscsimplificada) && !empty($rgadsccompleta)) {
            if (is_array($_SESSION['_registro']['post']) && array_key_exists('arqid', $_SESSION['_registro']['post'])) {
                apagarArquivo($_SESSION['_registro']['post']['arqid']);
            }
            $_SESSION['_registro']['post'] = $_POST;
            
            include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
            $arquivo = $_FILES['arquivo'];
            if ($_FILES['arquivo'] && $arquivo['name'] && $arquivo['type'] && $arquivo['size']) {
                $file = new FilesSimec('arquivo', null, 'obras2');
                $file->setPasta('obras2');
                $file->setUpload(null, 'arquivo', false);
                $_SESSION['_registro']['post']['arqid'] = $file->getIdArquivo();
            }
        }
    }
}

//Chamada de programa
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Registro de atividade em massa', '');
?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>

<?php if (!isset($_SESSION['_registro']['post'])) : ?>
    <form method="post" name="formCadRegAtividade" id="formCadRegAtividade" enctype="multipart/form-data">
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
                <td class="SubTituloDireita" style="width: 190px;">Descrição simplificada:</td>
                <td>
                    <?php echo campo_textarea('rgadscsimplificada', 'S', 'S', '', 80, 4, 250); ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
                <td>
                    <input type="hidden" name="arqid" id="arqid" value=""/>
                    <input type="file" name="arquivo" id="arquivo"/>
                    <br>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" style="width: 190px;">Descrição detalhada:</td>
                <td>
                    <?php echo campo_textarea('rgadsccompleta', 'S', 'S', '', 80, 6, 5000);	?>
                </td>
            </tr>
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

//            if (jQuery("#muncod option").length==1 && !jQuery("#muncod option").attr("value")) {
//                alert("Voce precisa selecionar pelo menos um município");
//                return false;
//            }

            if (!jQuery("#rgadscsimplificada").val()) {
                alert("Campo Descrição simplificada precisa ser preechido!");
                jQuery("#rgadscsimplificada").focus();
                return false;
            }

            if (!jQuery("#rgadsccompleta").val()) {
                alert("Campo Descrição detalhada precisa ser preechido!");
                jQuery("#rgadsccompleta").focus();
                return false;
            }

            selectAllOptions(document.getElementById("muncod"));
            jQuery("#formCadRegAtividade").submit();
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
<?php 
else:
    //ver($_SESSION['_registro']['post']);
    if($_SESSION['_registro']['post']['estuf'])
        $whereEstuf = "'".implode("','", $_SESSION['_registro']['post']['estuf'])."'";
    $whereMuncod = "'".implode("','", $_SESSION['_registro']['post']['muncod'])."'";

    if (strlen($whereEstuf) && !empty($_SESSION['_registro']['post']['estuf'][0])) {
        $_whereEstuf = " ent.estuf IN ({$whereEstuf}) AND ";
    }

    if (strlen($whereMuncod) &&  !empty($_SESSION['_registro']['post']['muncod'][0])) {
        $_whereMuncod = " ent.muncod IN ({$whereMuncod}) AND ";
    }

    $_POST["esfera"] = (empty($_POST["esfera"])) ? 'M' : $_POST["esfera"];
    if (!empty($_POST['esfera'])) {
        $_whereEsfera = "  e.empesfera = '{$_POST['esfera']}' AND";
    }

    if (!empty($_SESSION['_registro']['post']['processo'])) {
        $_whereProcesso = "  CASE WHEN o.tooid = 2 AND TRIM(o.obrnumprocessoconv) != '' THEN
                                to_char(Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN po1.pronumeroprocesso IS NOT NULL THEN
                                to_char(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN po2.pronumeroprocesso IS NOT NULL THEN
                                to_char(Replace(Replace(Replace(po2.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            END ILIKE to_char(Replace(Replace(Replace('{$_SESSION['_registro']['post']['processo']}','.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') AND";
    }

    if ($_whereEstuf || $_whereProcesso) {
        $strSQLObras = "SELECT 
                            '<center><input type=\"checkbox\" class=\"checkboxChildren\" value=\"'||o.obrid||'\" name=\"obrid[]\"/></center>' as acao,
                            o.obrid, o.obrnome
                        FROM
                            obras2.obras o
                            INNER JOIN obras2.empreendimento e ON e.empid = o.empid
                            INNER JOIN entidade.endereco ent ON (ent.endid = o.endid)

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
//        echo $strSQLObras; die;
        $checkboxParent = "<center><input type=\"checkbox\" class=\"chackboxParent\" value=\"\" name=\"obridPai[]\"/></center>";
        $cabecalho = array($checkboxParent, 'ObraID', 'Obra nome');
        echo '<form method="post" name="formFiltrarObra" id="formFiltrarObra">';
        echo '<input type="hidden" name="registrar" value="1"/>';
        echo '<input type="hidden" name="_arqid" id="_arqid" value="'.$_SESSION['_registro']['post']['arqid'].'"/>';
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
<?php
endif;
?>