<?php
ini_set("memory_limit", "512M");
ob_start();

$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; // $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];



switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', 'carregarUnidade', '', '', 200, 'N', 'muncod');
        exit;
    case 'unidade':
        $entidade = new Entidade();
        header('content-type: text/html; charset=ISO-8859-1');
        $dado = $entidade->listaComboObras2(array('muncod'=>$_POST['muncod']));
        echo $db->monta_combo("entid", $dado, "S", "Todos", "", "", "", 200, "N", "entid");
        exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgao');
echo montarAbasArray($arAba, "?modulo=principal/listaObras&acao=A&orgid=" . $orgid);

monta_titulo('Lista de Obras Tecnica', 'Filtre as Obras');

/**
 * EXTRAI OS DADOS DA PESQUISA PARA RECARREGAR O FORM
 */
extract($_POST);
?>
    <link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
    <script type="text/javascript" src="/includes/remedial.js"></script>
    <script type="text/javascript" src="/includes/superTitle.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="/obras2/js/obras2.js"></script>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript">

        var u = '?modulo=principal/listaObras&acao=A&req=pegaHtmlPagamento&preid='
            , fieldsValue2 = ['moedtprevinauguracao_i', 'moedtprevinauguracao_f']
            , fieldsValue3 = ['moedtpreviniciofunc_i', 'moedtpreviniciofunc_f']
            , OPERATION = 1
            , INAUGURATION = 2
            , INITIATION = 3;

        $(document).ready(function() {

            if ($("input:radio[name='funcionamentoflag']").is(":checked")) {

                var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent().parent()[0])
                    , $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent().parent()[0]);

                $spaceDate1.find("img").css("display", "");
                $spaceDate2.find("img").css("display", "");

                switch (parseInt($("input:radio[name='funcionamentoflag']:checked").val())) {
                    case OPERATION:
                        $spaceDate1.find("img").css("display", "none");
                        $spaceDate2.find("img").css("display", "none");
                        break;
                    case INAUGURATION:
                        $spaceDate2.find("img").css("display", "none");
                        break;
                    case INITIATION:
                        $spaceDate1.find("img").css("display", "none");
                        break;
                }
            }

            $('.pesquisar').click(function() {
                $('#req').val('');

                if ($("input:radio[name='funcionamentoflag']:checked").val()==2) {
                    if (!$("input[name='moedtprevinauguracao_i']").val() || !$("input[name='moedtprevinauguracao_f']").val()) {
                        alert("Voce precisa preencher o intervalo com as datas");
                        $("input[name='moedtprevinauguracao_i']").focus();
                        return false;
                    }
                }

                if ($("input:radio[name='funcionamentoflag']:checked").val()==3) {
                    if (!$("input[name='moedtpreviniciofunc_i']").val() || !$("input[name='moedtpreviniciofunc_f']").val()) {
                        alert("Voce precisa preencher o intervalo com as datas");
                        $("input[name='moedtpreviniciofunc_i']").focus();
                        return false;
                    }
                }

                $('#formListaObra').submit();
            });

            $('.btnEexcel').click(function() {
                $('#req').val('excel');
                $('#formListaObra').submit();
            });


            <?php
            if ($abreBuscaAvancada) {
                echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";
            }
            ?>

            $("input:radio[name='funcionamentoflag']").on("click", function(e) {

                for (var i=0; i<fieldsValue2.length; i++) {
                    $("input[name='"+fieldsValue2[i]+"']").val("");
                    $("input[name='"+fieldsValue3[i]+"']").val("");
                }

                var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent()[0])
                    , $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent()[0]);

                $spaceDate1.find("img").css("display", "");
                $spaceDate2.find("img").css("display", "");

                switch (parseInt($(this).val())) {
                    case OPERATION:
                        $spaceDate1.find("img").css("display", "none");
                        $spaceDate2.find("img").css("display", "none");
                        for (var i=0; i<fieldsValue2.length; i++) {
                            $("input[name='"+fieldsValue2[i]+"']").attr("disabled",true);
                            $("input[name='"+fieldsValue3[i]+"']").attr("disabled",true);
                        }
                        break;
                    case INAUGURATION:
                        $spaceDate2.find("img").css("display", "none");
                        for (var i=0; i<fieldsValue3.length; i++) {
                            $("input[name='"+fieldsValue2[i]+"']").attr("disabled",false);
                            $("input[name='"+fieldsValue3[i]+"']").attr("disabled",true);
                        }
                        break;
                    case INITIATION:
                        $spaceDate1.find("img").css("display", "none");
                        for (var i=0; i<fieldsValue3.length; i++) {
                            $("input[name='"+fieldsValue2[i]+"']").attr("disabled",true);
                            $("input[name='"+fieldsValue3[i]+"']").attr("disabled",false);
                        }
                        break;
                }
            });
        });

        function exibeBuscaAvancada(visivel) {
            if (visivel == true) {
                $('#tr_busca_avancada').show();
                $('#labelBuscaAvancada').hide();
                $('#abreBuscaAvancada').val('t');
            } else {
                $('#tr_busca_avancada').hide();
                $('#labelBuscaAvancada').show();
                $('#abreBuscaAvancada').val('f');
            }
        }

        function carregarMunicipio(estuf) {
            var td = $('#td_municipio');
            if (estuf != '') {
                var url = location.href;
                $.ajax({
                    url: url,
                    type: 'post',
                    data: {ajax: 'municipio',
                        estuf: estuf},
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

        function carregarUnidade(muncod) {
            var td = $('#td_unidade');
            if (muncod != '') {
                var url = location.href;
                $.ajax({
                    url: url,
                    type: 'post',
                    data: {ajax: 'unidade',muncod: muncod},
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

        function abreEvolucaoFinan(obrid) {
            janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=' + obrid, 800, 650);
        }

        function alterarObr(obrid) {
            location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
        }

        function novaObrVinculada(obrid) {
            location.href = '?modulo=principal/cadObra&acao=A&obridVinculado=' + obrid;
        }

        //    function excluirObr(obrid) {
        //        if (confirm('Deseja apagar este obra?')) {
        //            $('#obrid').val(obrid);
        //            $('#req').val('apagar');
        //            $('#formListaObra').submit();
        //        }
        //    }

        function abreDocumentoObjeto(obrid) {
            $('#req').val('abaDocumento');
            $('#obrid').val(obrid);

            $('#formListaObra').submit();
        }

        function abreRestricao(obrid) {
            $('#req').val('abaRestricao');
            $('#obrid').val(obrid);

            $('#formListaObra').submit();
        }

        function abreFotoObjeto(obrid) {
            janela('?modulo=principal/listaObras&acao=A&req=abaFotos&obrid=' + obrid, 800, 650);
        }

    </script>



    <form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
        <td>
            <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
        </td>
    </tr>
    <tr>
    <td id="tr_busca_avancada" colspan="2" style="display: none;">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <th colspan="2">
            Busca Avançada
            <a style="float:right;" onclick="exibeBuscaAvancada(false);">[Fechar]</a>
        </th>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
        <td>
            <?php
            $situacaoRegistro = new SituacaoRegistro();
            $sql                      = $situacaoRegistro->listaCombo();
            $db->monta_combo('strid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'strid');
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
        <td>
            <?php
            $tipoObra = new TipoObra();
            $db->monta_combo("tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid");
            ?>
        </td>
    </tr>
    <?php
    if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL):
        ?>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
            <td>
                <?php
                $tipologiaObra = new TipologiaObra();
                $param = array("orgid" => $_SESSION['obras2']['orgid']);
                $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid");
                ?>
            </td>
        </tr>
    <?php
    endif;
    ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Programa:</td>
        <td>
            <?php
            $programa = new ProgramaFonte();
            $db->monta_combo("prfid", $programa->listaCombo(array("orgid" => $_SESSION['obras2']['orgid'])), "S", "Todos", "", "", "", 200, "N", "prfid");
            ?>
        </td>
    </tr>
    <tr>
        <td style="width:20%" class="SubTituloDireita">Fonte</td>
        <td>
            <?php
            $tipoOrigemObra = new TipoOrigemObra();
            $sql = $tipoOrigemObra->listaCombo(true);
            $db->monta_combo('tooid', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'tooid', '', '', '');
            //                                $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
            //                                $db->monta_combo('frpid', $tipoFormaRepasseRecursos->listaCombo(), $habilita, "Selecione...", 'abreCamposRecursos', '', '', '200', 'N', 'frpid');
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">UF:</td>
        <td>
            <?php
            $uf = new Estado();
            $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Município:</td>
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
            echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', 'carregarUnidade', '', '', 200, 'N', 'muncod');
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
        <td  id="td_unidade">
            <?php
            $entidade = new Entidade();
            if ($muncod) {
                $dado = $entidade->listaComboObras2(array('muncod'=>$muncod));
                $habUn = 'S';
            } else {
                $dado = array();
                $habUn = 'N';
            }
            $habUn = ($disable == 'N' ? $disable : $habUn);
            $db->monta_combo("entid", $dado, $habUn, "Todos", "", "", "", 200, "N", "entid");
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Esfera:</td>
        <td>
            <?php
            $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                Array('codigo' => 'M', 'descricao' => 'Municipal'));
            $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
            ?>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Convênio/Termo:</td>
        <td>
            Número:&nbsp;
            <?php
            echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
            ?>
            Ano:&nbsp;
            <?php
            echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
            ?>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Processo:</td>
        <td>
            Número:&nbsp;
            <?php
            echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
            ?>
            Ano:&nbsp;
            <?php
            echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
            ?>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist administrativo?</td>
        <td>
            <input type="radio" name="chk_adm" id="" value="S" <?= ( $_POST["chk_adm"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="chk_adm" id="" value="N" <?= ( $_POST["chk_adm"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="chk_adm" id="" value="T"  <?= ( $_POST["chk_adm"] == "T" || $_POST["chk_adm"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist 2° Parcela?</td>
        <td>
            <input type="radio" name="chk_2pc" id="" value="S" <?= ( $_POST["chk_2pc"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="chk_2pc" id="" value="N" <?= ( $_POST["chk_2pc"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="chk_2pc" id="" value="T"  <?= ( $_POST["chk_2pc"] == "T" || $_POST["chk_2pc"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist Técnico?</td>
        <td>
            <input type="radio" name="chk_tec" id="" value="S" <?= ( $_POST["chk_tec"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="chk_tec" id="" value="N" <?= ( $_POST["chk_tec"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="chk_tec" id="" value="T"  <?= ( $_POST["chk_tec"] == "T" || $_POST["chk_tec"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist Obra Vinculada?</td>
        <td>
            <input type="radio" name="chk_vinc" id="" value="S" <?= ( $_POST["chk_vinc"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="chk_vinc" id="" value="N" <?= ( $_POST["chk_vinc"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="chk_vinc" id="" value="T"  <?= ( $_POST["chk_vinc"] == "T" || $_POST["chk_vinc"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Situação do Registro:</td>
        <td>
            <?php
            $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
            $db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid");
            ?>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
        <td>
            <table>
                <tr>
                    <th>Mínimo</th>
                    <th>Máximo</th>
                </tr>
                <tr>
                    <?php
                    for ($i = 0; $i <= 100; $i++) {
                        $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                    }
                    $percentualinicial = $_POST['percentualinicial'];
                    $percentualfinal   = $_POST['percentualfinal'];
                    $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                    print '<td>';
                    $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                    print '</td><td>';
                    $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                    print '</td>';
                    ?>
                </tr>
            </table>
        </td>
    </tr>


    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui vinculação?</td>
        <td>
            <input type="radio" name="vinc" id="" value="S" <?= ( $_POST["vinc"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="vinc" id="" value="N" <?= ( $_POST["vinc"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="vinc" id="" value="T"  <?= ( $_POST["vinc"] == "T" || $_POST["vinc"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Equilíbrio Físico Financeiro</td>
        <td>
            <input type="radio" name="eq_fis_fin" id="" value="S" <?= ( $_POST["eq_fis_fin"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="eq_fis_fin" id="" value="N" <?= ( $_POST["eq_fis_fin"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="eq_fis_fin" id="" value="T"  <?= ( $_POST["eq_fis_fin"] == "T" || $_POST["eq_fis_fin"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui Bloqueio PAR?</td>
        <td>
            <input type="radio" name="bloqueio" id="" value="S" <?= ( $_POST["bloqueio"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="bloqueio" id="" value="N" <?= ( $_POST["bloqueio"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="bloqueio" id="" value="T"  <?= ( $_POST["bloqueio"] == "T" || $_POST["bloqueio"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Obra reformulada?</td>
        <td>
            <input type="radio" name="reformulada" id="" value="S" <?= ( $_POST["reformulada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="reformulada" id="" value="N" <?= ( $_POST["reformulada"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="reformulada" id="" value="T"  <?= ( $_POST["reformulada"] == "T" || $_POST["reformulada"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

<!--    <tr>-->
<!--        <td class="SubTituloDireita" style="width: 190px;">Vigência da Obra?</td>-->
<!--        <td>-->
<!--            --><?php //echo campo_data2('vig_obra_i', 'S', true, '', null, '', ' ', '', ''); ?>
<!--            &nbsp;até&nbsp;-->
<!--            --><?php //echo campo_data2('vig_obra_f', 'S', true, '', null, '', ' ', '', ''); ?>
<!--        </td>-->
<!--    </tr>-->
<!---->
<!--    <tr>-->
<!--        <td class="SubTituloDireita" style="width: 190px;">Vigência do Termo</td>-->
<!--        <td>-->
<!--            de-->
<!--            --><?php //echo campo_data2('vig_termo_i', 'S', true, '', null, '', ' ', '', ''); ?>
<!--            &nbsp;até&nbsp;-->
<!--            --><?php //echo campo_data2('vig_termo_f', 'S', true, '', null, '', ' ', '', ''); ?>
<!--        </td>-->
<!--    </tr>-->

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Metodologia Inovadora:</td>
        <td>
            <input type="radio" name="obrami" id="" value="S" <?= ( $_POST["obrami"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="obrami" id="" value="N" <?= ( $_POST["obrami"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="obrami" id="" value=""  <?= ( $_POST["obrami"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Conta bancária bloqueada:</td>
        <td>
            <input type="radio" name="conta_bancaria_bloqueada" id="" value="S" <?= ( $_POST["conta_bancaria_bloqueada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
            <input type="radio" name="conta_bancaria_bloqueada" id="" value="N" <?= ( $_POST["conta_bancaria_bloqueada"] == "N" ? "checked='checked'" : "" ) ?>/> Não
            <input type="radio" name="conta_bancaria_bloqueada" id="" value=""  <?= ( $_POST["conta_bancaria_bloqueada"] == "" ? "checked='checked'" : "" ) ?> /> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Cronograma:</td>
        <td>
            <?php
            $sql = array(
                        array('codigo' => '1', 'descricao' => 'Cronograma OK'),
                        array('codigo' => '2', 'descricao' => 'Atraso no cronograma'),
                        array('codigo' => '3', 'descricao' => 'Atraso crítico no cronograma')
                   );
            $db->monta_combo('atraso', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'atraso');
            ?>
        </td>
    </tr>

    </table>
    </td>
    </tr>
    <tr>
        <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
            <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
            <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
        </td>
    </tr>
    </table>
    </form>
<?php

if ((array_key_exists('obrid',$_POST) || array_key_exists('esdid',$_POST) || array_key_exists('estuf',$_POST)) || !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC))) {

    $obras = new Obras();
    $filtro = $_POST;

    if ($filtro['req'] == 'excel') {
        ini_set("memory_limit", "1024M");
        $filtro['excel'] = true;
        $cabecalho = getCabecalho(true);
        $sql = listaSql($filtro);
        $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
    } else {
        $cabecalho = getCabecalho();
        $sql = listaSql($filtro);

        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", '', '', 600);
    }
} else {
    ?>
    <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
        <tr>
            <td style="text-align: justify;">
                Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.
            </td>
        </tr>
    </table>
<?php
}
?>

    <script type='text/javascript'>
        $(function() {
            $('#esdid').change(function() {
                var val = $('#esdid').val();
                if (val == '<?= ESDID_OBJ_PARALISADO ?>' || val == '<?= ESDID_OBJ_EXECUCAO ?>') {
                    $('#nivelpreenchimento').removeAttr("disabled");
                } else {
                    $('#nivelpreenchimento').attr("disabled", "disabled");
                    $('#nivelpreenchimento').val(0);
                }
            });
        });
    </script>


<?
function listaSql(Array $param = array())
{
    $coluns = array();
    $join = array();
    $where = array();

    if ($param['obrbuscatexto']) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-"," ",(trim($param['obrbuscatexto']) )));

        if(!strpos ($obrbuscatextoTemp, ',')){
            $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') ) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c){
                $c = trim($c);
                $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($param['reformulada'] == 'S')
        $where['reformulada'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN true ELSE false END";
    if ($param['reformulada'] == 'N')
        $where['reformulada'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN false ELSE true END";

    if ($param['vinc'] == 'S')
        $where['vinc'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN true ELSE false END";
    if ($param['vinc'] == 'N')
        $where['vinc'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN false ELSE true END";

    if ($param['eq_fis_fin'] == 'S')
        $where['eq_fis_fin'] =  "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN true ELSE false END";
    if ($param['eq_fis_fin'] == 'N')
        $where['eq_fis_fin'] =  "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN false ELSE true END";

    if ($param['conta_bancaria_bloqueada'] == 'S')
        $where1['conta_bancaria_bloqueada'] =  "conta_bancaria_bloqueada = 'SIM' ";
    if ($param['conta_bancaria_bloqueada'] == 'N')
        $where1['conta_bancaria_bloqueada'] =  "conta_bancaria_bloqueada = 'NÃO'";

    if ($param['bloqueio'] == 'S')
        $where['bloqueio'] =  "(CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END) = 'SIM' ";
    if ($param['bloqueio'] == 'N')
        $where['bloqueio'] =  "(CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END) = 'NAO'";



    if ($param['empid'])
        $where['empid'] =  "o.empid IN(" . $param['empid'] . ")";
    if ($param['strid'])
        $where['strid'] = "str.strid IN (" . $param['strid'] . ")";
    if ($param['tobid'])
        $where['tobid'] = "o.tobid IN(" . $param['tobid'] . ")";
    if ($param['tooid'])
        $where['tooid'] = "o.tooid IN(" . $param['tooid'] . ")";
    if ($param['tpoid'])
        $where['tpoid'] = "o.tpoid IN(" . $param['tpoid'] . ")";
//    if ($param['cloid'])
//        $where['cloid'] = "o.cloid IN(" . $param['cloid'] . ")";
    if ($param['prfid'])
        $where['prfid'] = "emp.prfid IN(" . $param['prfid'] . ")";
//    if ($param['moeid'])
//        $where['moeid'] = "e.moeid IN(" . $param['moeid'] . ")";
    if ($param['entid'])
        $where['entid'] = "o.entid IN(" . $param['entid'] . ")";
//    if ($param['ultatualizacao']) {
//        switch ($param['ultatualizacao']) {
//            case 1:
//                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) < 45";
//                break;
//            case 2:
//                $where['ultatualizacao'] = "( DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) >= 45 AND
//                                  DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 60 )";
//                break;
//            case 3:
//                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 60";
//                break;
//        }
//    }
    if ($param['estuf'])
        $where['estuf'] = "m.estuf IN('" . $param['estuf'] . "')";
    if ($param['muncod'])
        $where['muncod'] = "m.muncod IN('" . $param['muncod'] . "')";
    if ($param['empesfera'])
        $where['empesfera'] = "emp.empesfera IN('" . $param['empesfera'] . "')";
    if ($param['processo'])
        $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
    if ($param['ano_processo'])
        $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
    if ($param['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
    if ($param['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";
//    if ($param['totalfoto'] == 'S')
//        $where['totalfoto'] = "obrsnfotos = true";
//    elseif ($param['totalfoto'] == 'N')
//        $where['totalfoto'] = "obrsnfotos = false OR obrsnfotos IS NULL";
    if ($param['obrami'] == 'S')
        $where['obrami'] = "o.tpoid IN (104, 105)";
    elseif ($param['obrami'] == 'N')
        $where['obrami'] = "o.tpoid NOT IN (104, 105)";
//    if ($param['possui_vistoria'] == 'S')
//        $where['possui_vistoria'] = "obrdtultvistoria IS NOT NULL";
//    elseif ($param['possui_vistoria'] == 'N')
//        $where['possui_vistoria'] = "o.obrdtultvistoria IS NULL";
//    if ($param['res_estado'] == 'S')
//        $where['res_estado'] = "res_estado IS NOT NULL";
//    elseif ($param['res_estado'] == 'N')
//        $where['res_estado'] = "res_estado IS NULL";
//    if ($param['res_superada'] == 'S')
//        $where['res_superada'] = "res_estado = 1";
//    elseif ($param['res_superada'] == 'N')
//        $where['res_superad'] = "res_estado > 1";
//    if ($param['ocraditivado'] == 'S')
//        $where['ocraditivado'] = "ocraditivado = 't'";
//    elseif ($param['ocraditivado'] == 'N')
//        $where['ocraditivado'] = " (ocraditivado IS NULL OR ocraditivado='f') ";
//    if ($param['responsavel'] == 'S')
//        $where['responsavel'] = "e.empdtultvistoriaempresa IS NOT NULL";
//    elseif ($param['responsavel'] == 'N')
//        $where['responsavel'] = "e.empdtultvistoriaempresa IS NULL";
//    if ($param['evomi'] == 'S') {
//        $where['evmi'] = "evmi.emidtinclusao IS NOT NULL";
//        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
//    } elseif ($param['evomi'] == 'N') {
//        $where['evmi'] = "evmi.emidtinclusao IS NULL";
//        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
//    }
//    switch ($param['obrvinculada']) {
//        case 'S':
//            $where['obrvinculada'] = " o.obrstatus = 'P' ";
//            break;
//        case 'N':
//            $where['obrvinculada'] = " o.obrstatus = 'A' ";
//            break;
//        case 'T':
//            $where['obrvinculada'] = " o.obrstatus IN ('A', 'P') ";
//            break;
//        default:
//            $where['obrvinculada'] = " o.obrstatus = 'A' ";
//            break;
//    }
//    if ($param['repasse'] == 'S') {
//        $where['repasse'] = "o.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
//                                        JOIN par.pagamentoobra po ON po.preid = o.preid
//                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
//                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
//    } elseif ($param['repasse'] == 'N') {
//        $where['repasse'] = "o.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
//                                        JOIN par.pagamentoobra po ON po.preid = o.preid
//                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
//                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
//    }
//    if ($param['medidasexcecao'] == 'S')
//        $where['medidasexcecao'] = "medidasexcecao = TRUE ";
//    elseif ($param['medidasexcecao'] == 'N')
//        $where['medidasexcecao'] = "medidasexcecao = FALSE ";
//    if ($param['fiscal_ativo'] == 'S')
//        $where['fiscal_ativo'] = "e.empid IN (select
//                                ur.empid
//                            from obras2.usuarioresponsabilidade ur
//                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
//                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
//                            where
//                            ur.rpustatus = 'A' AND
//                            u.suscod = 'A' AND
//                            us.suscod = 'A' AND ur.empid IS NOT NULL
//                            GROUP BY ur.empid
//                            HAVING COUNT(*) > 1)";
//    elseif ($param['fiscal_ativo'] == 'N')
//        $where['fiscal_ativo'] = " e.empid NOT IN (select
//                                ur.empid
//                            from obras2.usuarioresponsabilidade ur
//                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
//                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
//                            where
//                            ur.rpustatus = 'A' AND
//                            u.suscod = 'A' AND
//                            us.suscod = 'A' AND ur.empid IS NOT NULL
//                            GROUP BY ur.empid
//                            HAVING COUNT(*) > 1)";
    $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";
    if ($param['chk_adm'] == 'S')
        $where['chk_adm'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
    elseif ($param['chk_adm'] == 'N')
        $where['chk_adm'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
    if ($param['chk_2pc'] == 'S')
        $where['chk_2pc'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
    elseif ($param['chk_2pc'] == 'N')
        $where['chk_2pc'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
    if ($param['chk_tec'] == 'S')
        $where['chk_tec'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
    elseif ($param['chk_tec'] == 'N')
        $where['chk_tec'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
    if ($param['chk_vinc'] == 'S')
        $where['chk_vinc'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
    elseif ($param['chk_vinc'] == 'N')
        $where['chk_vinc'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
//    if ($param['stiid'])
//        $where['stiid'] = "o.stiid = " . $param['stiid'];
    if ($param['esdid'])
        $where['esdid'] = "d.esdid IN(" . $param['esdid'] . ")";
//    if ($param['rsuid'])
//        $where['rsuid'] = "rsuid = " . $param['rsuid'];
//    if ($param['obrvalorprevisto_menor'])
//        $where['obrvalorprevisto_menor'] = "o.obrvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_menor']);
//    if ($param['obrvalorprevisto_maior'])
//        $where['obrvalorprevisto_maior'] = "o.obrvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_maior']);
//    switch ($param['nivelpreenchimento']) {
//        /* 1 - Verde    -> x < 45              */
//        /* 2 - Amarelo  -> 45 > x < 60         */
//        /* 3 - Vermelho -> x > 60              */
//        case 1 : //Verde
//            $where['nivelpreenchimento'] = " CASE
//                                WHEN o.obrdtultvistoria IS NOT NULL THEN
//                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) < 45 )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                ELSE
//                                    ( DATE_PART('days', NOW() - sup.supdata  ) < 45 )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                END  ";
//            break;
//        case 2 :
//            $where['nivelpreenchimento'] = " CASE
//                                WHEN o.obrdtultvistoria IS NOT NULL THEN
//                                    ( (DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 45) AND DATE_PART('days', NOW() - o.obrdtultvistoria ) < 60 )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                ELSE
//                                    ( (DATE_PART('days', NOW() - sup.supdata  ) >= 45) AND DATE_PART('days', NOW() - sup.supdata ) < 60  )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                END";
//            break;
//        case 3 :
//            $where['nivelpreenchimento'] = " CASE
//                                WHEN o.obrdtultvistoria IS NOT NULL THEN
//                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 60 )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                ELSE
//                                    ( DATE_PART('days', NOW() - sup.supdata  ) >= 60 )
//                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
//                                END  ";
//            break;
//    }
//    if($param['nivelpreenchimento'] || $param['rsuid'])
//        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";
//
    if ($param['percentualinicial'] != '')
        $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " .  $param['percentualinicial'];
    if ($param['percentualfinal'] != '')
        if ($param['percentualfinal'] < 100)
            $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
    if ($param['atraso'] == '1')
        $where['atraso'] = "numdias <= 0";
    if ($param['atraso'] == '2')
        $where['atraso'] = "numdias > 0 AND numdias < 15";
    if ($param['atraso'] == '3')
        $where['atraso'] = "numdias >= 15";
//    if ($param['funcionamentoflag']) {
//        switch ($param['funcionamentoflag']) {
//            case 1: // em funcionamento
//                $queryComplement = 'AND moedtiniciofunc < NOW()';
//                break;
//            case 2: // previsão de inauguração
//                $queryComplement = 'AND moedtprevinauguracao BETWEEN \''.ajusta_data($param['moedtprevinauguracao_i']).'\' AND \''.ajusta_data($param['moedtprevinauguracao_f']).'\'';
//                break;
//            case 3: //previsão inicio de funcionamento
//                $queryComplement = 'AND moedtpreviniciofunc BETWEEN \''.ajusta_data($param['moedtpreviniciofunc_i']).'\' AND \''.ajusta_data($param['moedtpreviniciofunc_f']).'\'';
//                break;
//        }
//
//        $where['funcionamentoflag'] = sprintf("CASE WHEN (SELECT
//                            CASE WHEN moeid IS NOT NULL THEN TRUE ELSE FALSE END
//                            FROM obras2.mobiliarioempenhado
//                            WHERE obrid = o.obrid %s
//                            ORDER BY moeid DESC
//                            LIMIT 1) THEN true
//                         ELSE false END", $queryComplement);
//    }
//    if ($param['moeempenhadoflag'] == 'S')
//        $where['moeempenhadoflag'] = "o.obrid IN (SELECT obrid FROM obras2.mobiliarioempenhado
//                                          WHERE moeid IN (
//                                                          SELECT MAX(moeid)
//                                                          FROM obras2.mobiliarioempenhado
//                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')";
//    elseif ($param['moeempenhadoflag'] == 'N')
//        $where['moeempenhadoflag'] = "o.obrid NOT IN (SELECT obrid FROM obras2.mobiliarioempenhado
//                                          WHERE moeid IN (
//                                                          SELECT MAX(moeid)
//                                                          FROM obras2.mobiliarioempenhado
//                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')";
//    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
//
//        $arrObr = array(
//            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
//            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
//        );
//
//        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);
//
//        $arrOrg = Array(PFLCOD_ADMINISTRADOR,
//            PFLCOD_CADASTRADOR_INSTITUCIONAL,
//            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
//            PFLCOD_SUPERVISOR_MEC,
//            PFLCOD_GESTOR_MEC);
//
//        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);
//
//        $resp = array();
//        $arPflcod = array();
//
//        if (possui_perfil($arrEst)) {
//            $resp[] = "urs.estuf = edr.estuf ";
//            $arPflcod = array_merge($arPflcod, $arrEst);
//        }
//
//        if (possui_perfil($arrObr)) {
//            $resp[] = "urs.empid = e.empid ";
//            $arPflcod = array_merge($arPflcod, $arrObr);
//        }
//
//        if (possui_perfil($arrOrg)) {
//            $resp[] = "urs.orgid = e.orgid ";
//            $arPflcod = array_merge($arPflcod, $arrOrg);
//        }
//
//        if (possui_perfil($arrUni)) {
//            $resp[] = "urs.entid = e.entidunidade ";
//            $arPflcod = array_merge($arPflcod, $arrUni);
//        }
//
//        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
//            $usuarioResp = new UsuarioResponsabilidade();
//            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
//            $arEmpid = ($arEmpid ? $arEmpid : array(0));
//
//            $resp[] = " urs.entid = e.entidunidade AND e.empid IN('" . implode("', '", $arEmpid) . "')";
//            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
//            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;
//        }
//
//        if (count($resp)) {
//            $arPflcod = array_unique($arPflcod);
//            $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
//                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
//                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
//                                                                    (" . implode(' OR ', $resp) . ")";
//        }
//    }

//    $imgNovoObj = '';
//    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
//        $imgNovoObj = "' ||
//                                    CASE WHEN ed.esdid = " . ESDID_OBJ_PARALISADO . " AND o.obrstatus != 'P' THEN
//                                        '<img
//                                                align=\"absmiddle\"
//                                                src=\"/imagens/editar_nome.gif\"
//                                                style=\"cursor: pointer\"
//                                                onclick=\"javascript: novaObrVinculada(\'' || o.obrid || '\');\"
//                                                title=\"Cadastrar nova obra vinculada\">'
//                                    ELSE
//                                            '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
//                                    END || '";
//    }

    $coluns['acao'] = "'
            <center><div>
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                            title=\"Alterar Obra\" />
            </div></center>' AS acao";


    $coluns['obrid'] = "o.obrid";
    $coluns['preid'] = "o.preid";
    $coluns['mi'] = "CASE WHEN o.tpoid IN (104, 105) THEN 'SIM' ELSE 'NÃO' END as mi";
    $coluns['vinculada'] = "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END AS vinculada";
    $coluns['mundescricao'] = "m.mundescricao";
    $coluns['estuf'] = "m.estuf";
    $coluns['obrnome'] = "'<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['tipologia'] = "tpo.tpodsc";
    $coluns['programa'] = "pf.prfdesc";
    $coluns['fonte'] = "too.toodescricao";
    $coluns['esfera'] = "CASE WHEN emp.empesfera = 'M' THEN 'Municipal' ELSE 'Estadual' END empesfera";
    $coluns['valor'] = "coalesce(TRUNC (pre.prevalorobra, 2), 0)";
    $coluns['situacao'] = "et.esddsc";
    $coluns['excel_obrnome'] = "'(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['vig_obra'] = "ter.\"Fim Vigência da Obra\"";
    $coluns['vig_ter'] = "CASE WHEN ter.\"Fim Vigência Termo\" IS NULL AND o.tooid = 2 THEN TO_CHAR(dc.dcodatafim,'DD/MM/YYYY')::text ELSE ter.\"Fim Vigência Termo\" END";
    $coluns['osmi'] = "CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem cadastrada para essa obra\">'
				 			 WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem aguardando aceite para essa obra\">'
                             WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem concluída para essa obra\">'
                             WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Sondagem recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Sondagem cadastrada para essa obra\">'
                             END
                             ||
                             CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação cadastrada para essa obra\">'
				 			 WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação aguaradando aceite para essa obra\">'
                             WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação concluída para essa obra\">'
                             WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Implantação recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Implantação cadastrada para essa obra\">'
                             END
						     ||
						     CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução cadastrada para essa obra\">'
				 			 WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução aguardando aceite para essa obra\">'
                             WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução concluida para essa obra\">'
                             WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Execução recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Execução cadastrada para essa obra\">'
                             END as OS";
    $coluns['ckfadm'] = "(SELECT TO_CHAR(MAX(ckf.ckfdatainclusao), 'DD/MM/YYYY')
                FROM obras2.checklistfnde          ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                WHERE que.queid = 96 AND ckf.obrid = o.obrid
                GROUP BY ckf.obrid) as ckfadm";
    $coluns['ckftec'] = "(SELECT TO_CHAR(MAX(ckf.ckfdatainclusao), 'DD/MM/YYYY')
                FROM obras2.checklistfnde          ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                WHERE que.queid = 98 AND ckf.obrid = o.obrid
                GROUP BY ckf.obrid) as ckftec";
    $coluns['obrdtultvistoria'] = "TO_CHAR(o.obrdtultvistoria, 'DD/MM/YYYY') as obrdtultvistoria";
    $coluns['percentual_execucao'] = "((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)  as percentual_execucao";
    $coluns['cronograma_atraso'] = "crono.flag";
    $coluns['cronograma_atraso_excel'] = "crono.flag_xls";
    $coluns['empdtultvistoriaempresa'] = "(


                        SELECT  TO_CHAR(suedtsupervisao, 'DD/MM/YYYY')
                        FROM obras2.supervisaoempresa  se
                        LEFT JOIN obras2.supervisao s ON s.sueid = se.sueid AND s.supstatus = 'A'
                        WHERE se.suestatus = 'A' AND se.empid = emp.empid
                        ORDER BY suedtsupervisao DESC
                        LIMIT 1

                    ) as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "(

                        SELECT ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total
                                FROM
                                obras2.itenscomposicaoobra i
                                JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A', 'H') AND s.croid = cro.croid
                                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                WHERE
                                i.icostatus = 'A' AND
                                i.relativoedificacao = 'D' AND
                                cro.obrid = s.obrid AND i.obrid = cro.obrid
                            ) as percentual
                        FROM obras2.supervisaoempresa  se
                        LEFT JOIN obras2.supervisao s ON s.sueid = se.sueid AND s.supstatus = 'A'
                        WHERE se.suestatus = 'A' AND se.empid = emp.empid
                        ORDER BY suedtsupervisao DESC
                        LIMIT 1

                    ) as emppercentultvistoriaempresa";

    $coluns['perc_pago'] = "(
                                SELECT (

                                    ( SELECT case when pe.prevalorobra > 0 then
                                        ROUND(SUM( po.pobvalorpagamento * 100 ) / pe.prevalorobra)
                                        else 0 end
                                        FROM par.pagamento p
                                        INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                                        WHERE p.pagstatus = 'A'
                                            AND p.pagsituacaopagamento ILIKE '%EFETIVADO%'
                                            AND po.preid = pre.preid
                                        GROUP BY pe.prevalorobra
                                    )
                                    UNION
                                    ( SELECT
                                        CASE WHEN pe.prevalorobra > 0 then
                                        ROUND(SUM( po.popvalorpagamento * 100 ) / pe.prevalorobra)
                                        ELSE 0 END
                                        FROM par.pagamento p
                                        INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                                        WHERE p.pagstatus = 'A'
                                        AND p.pagsituacaopagamento ilike '%EFETIVADO%'
                                        AND po.preid = pre.preid
                                        GROUP BY pe.prevalorobra
                                    )
                                )
                            )
                            ";
    $coluns['execucao_fisica'] = "

        (1+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as exec_fisica

    ";

    $coluns['equilibrio'] = "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN 'SIM' ELSE 'NÃO' END AS equilibrio";
    $coluns['qtdobras'] = "ff.qtdobras";
    $coluns['bloqueio'] = "CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END AS bloqueio";
    $coluns['reformulacao'] = "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END AS reformulacao";

    $coluns['ca_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CA

    ";
    $coluns['cv_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CV

    ";
    $coluns['ct_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CT

    ";
    $coluns['co_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as Outros

    ";

    $coluns['ca'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CA

    ";
    $coluns['cv'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CV

    ";
    $coluns['ct'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CT

    ";
    $coluns['co'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as Outros

    ";

    $coluns['pronumeroprocesso'] = "p_conv.pronumeroprocesso";
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";

    $coluns['conta_bancaria_bloqueada'] = "(
    SELECT CASE WHEN prosituacaoconta IS NULL THEN '-' WHEN prosituacaoconta IN (25,24,14) THEN 'SIM' ELSE 'NÃO' END prosituacaoconta FROM (

    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobraspar
                    UNION
                    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobra

                ) as p
                WHERE pronumeroprocesso = vo.processo ) as conta_bancaria_bloqueada";
    if ($param['excel']) {
        unset($coluns['cv']);
        unset($coluns['ct']);
        unset($coluns['ca']);
        unset($coluns['co']);
        unset($coluns['acao']);
        unset($coluns['cronograma_atraso']);
    } else {
        unset($coluns['cronograma_atraso_excel']);
        unset($coluns['excel_obrnome']);
        unset($coluns['cv_e']);
        unset($coluns['ct_e']);
        unset($coluns['ca_e']);
        unset($coluns['co_e']);
    }


    $sqlBase = "
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN painel.dadosconvenios dc on dc.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
                LEFT JOIN obras.preobra pre                          ON pre.preid = o.preid
                LEFT JOIN obras2.empreendimento emp                  ON emp.empid = o.empid
                LEFT JOIN entidade.endereco ed                       ON ed.endid = o.endid
                LEFT JOIN territorios.municipio m                    ON m.muncod = ed.muncod
                LEFT JOIN territorios.estado est                     ON est.estuf = m.estuf
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = emp.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento et                ON et.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                LEFT JOIN obras2.vm_termo_obras  ter                  ON ter.\"Preid\" = o.preid
                LEFT JOIN par.vm_saldo_empenho_por_obra vo ON vo.obrid = o.obrid

                -- Join com a ultima OS Execucao
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 1 GROUP BY osmt.tomid, osmt.obrid ) AS osm1 ON osm1.obrid = o.obrid
                LEFT JOIN workflow.documento osmd1 ON osm1.docid = osmd1.docid

                -- Join com a ultima OS Sondagem
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 2 GROUP BY osmt.tomid, osmt.obrid ) AS osm2 ON osm2.obrid = o.obrid
                LEFT JOIN workflow.documento osmd2 ON osm2.docid = osmd2.docid

                -- Join com a ultima OS Inplantação
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 3 GROUP BY osmt.tomid, osmt.obrid ) AS osm3 ON osm3.obrid = o.obrid
                LEFT JOIN workflow.documento osmd3 ON osm3.docid = osmd3.docid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                LEFT JOIN (
                        SELECT
                            *,
                            CASE WHEN diff = 'N/A' OR diff IS NULL THEN '0 ' ELSE formataMonetario(trunc(diff::numeric, 2))  END as diff_formatado,
                            CASE WHEN diff = 'N/A' OR diff IS NULL THEN 0 ELSE trunc(diff::numeric, 2)  END as diff_number
                        FROM obras2.vm_fisico_financeiro_processo) ff ON ff.pronumeroprocesso = p_conv.pronumeroprocesso
                
                LEFT JOIN (
                    SELECT subcrono.obrid, subcrono.numdias,
                        CASE WHEN subcrono.numdias >= 15 THEN
                            '<center><img src=\"/imagens/icone_critico.png\" style=\"width:15px;\" title=\"Atraso crítico no cronograma\"></center>'
                        WHEN subcrono.numdias <= 0 THEN
                            '<center><img src=\"/imagens/icone_ok.png\" style=\"width:15px;\" title=\"Cronograma OK\"></center>'
                        ELSE
                            '<center><img src=\"/imagens/icone_atencao.png\" style=\"width:15px;\" title=\"Atraso no cronograma\"></center>'
                        END AS flag,
                        CASE WHEN subcrono.numdias >= 15 THEN
                            'Atraso crítico no cronograma'
                        WHEN subcrono.numdias <= 0 THEN
                            'Cronograma OK'
                        ELSE
                            'Atraso no cronograma'
                        END AS flag_xls
                    FROM (
                        SELECT o.obrid, EXTRACT(DAY FROM now() - MIN(i.icodterminoitem)) as numdias
                        FROM obras2.obras o
                        JOIN workflow.documento d ON d.docid = o.docid AND d.esdid = 690
                        JOIN obras2.cronograma cro ON cro.obrid = o.obrid 
                        JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid AND i.icopercexecutado < 100
                        WHERE cro.crostatus = 'A' AND i.icostatus = 'A'::bpchar
                        group by o.obrid 
                    ) subcrono
                ) crono ON crono.obrid = o.obrid

                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                    o.obridpai IS NULL AND o.obrstatus = 'A'
                    " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
            ) as f ";

    if($param['conta_bancaria_bloqueada'] == 'S')
    {
        $sqlBase .= "WHERE conta_bancaria_bloqueada = 'SIM'";
    }
    if($param['conta_bancaria_bloqueada'] == 'N')
    {
        $sqlBase .= "WHERE conta_bancaria_bloqueada = 'NÃO'";
    }
    $sqlBase .="
            ORDER BY f.obrid ASC
    ";
    return $sqlBase;
}

function getCabecalho($xls = false)
{
    if (!$xls)
        $cabecalho[] = 'Ação';
    $cabecalho[] = 'ID';
    $cabecalho[] = 'ID Pré-Obra';
    $cabecalho[] = 'MI';
    $cabecalho[] = 'Vinculada';
    $cabecalho[] = 'Município';
    $cabecalho[] = 'UF';
    $cabecalho[] = 'Nome';
    $cabecalho[] = 'Tipologia';
    $cabecalho[] = 'Programa';
    $cabecalho[] = 'Fonte';
    $cabecalho[] = 'Esfera';
    $cabecalho[] = 'Valor Pactuado';
    $cabecalho[] = 'Situaçao';
    $cabecalho[] = 'Vigência Obra';
    $cabecalho[] = 'Vigência Termo';
    $cabecalho[] = 'OS MI';
    $cabecalho[] = 'Checklist Adminsitrativo';
    $cabecalho[] = 'Checklist Técnico';
    $cabecalho[] = 'Dt Últ Vistoria';
    $cabecalho[] = '% Últ Vistoria';
    $cabecalho[] = 'Atraso Cronograma';
    $cabecalho[] = 'Dt Últ Supervisão';
    $cabecalho[] = '% Últ Supervisão';

    $cabecalho[] = '% Valor Pago';
    $cabecalho[] = 'Validação';

    $cabecalho[] = 'Equilíbrio Fis-Fin';
    $cabecalho[] = 'Qtd Obras';
    $cabecalho[] = 'Bloqueio PAR';
    $cabecalho[] = 'Reformulada';
    $cabecalho[] = 'CA';
    $cabecalho[] = 'CV';
    $cabecalho[] = 'CT';
    $cabecalho[] = 'CO';
    $cabecalho[] = 'Nº Processo';
    $cabecalho[] = 'Ano Processo';
    $cabecalho[] = 'Nº Termo/Convênio';
    $cabecalho[] = 'Ano Termo/Convênio';
    $cabecalho[] = 'Conta Bancária Bloqueada';


    return $cabecalho;
}

?>
