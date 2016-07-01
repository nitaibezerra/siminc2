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

switch ($_REQUEST['req']) {
    case 'abaFotos':
//		$_SESSION['imgparams'] = array("filtro" => "obrid=".$_GET['obrid'],
//									   "tabela" => "obras2.obras_arquivos");
//		header('Location: /slideshow/slideshow/index.php?pagina=&_sisarquivo=obras2');
        $url = '/slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&obrid=' . $_GET['obrid'];
        header('Location: ' . $url);
        die();
    case 'abaRestricao':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras($obrid);
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/listaRestricao&acao=O');
        die();
    case 'abaDocumento':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras($obrid);
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/cadObraDocumentos&acao=A');
        die();
//    case 'apagar':
//        $obra = new Obras($_REQUEST['obrid']);
//        if ($obra->obrid) {
//            $obra->obrstatus = 'I';
//            $obra->salvar();
//        }
//        $db->commit();
//        die('<script type="text/javascript">
//				alert(\'Operação realizada com sucesso!\');
//				location.href=\'?modulo=principal/listaObras&acao=A\';
//			 </script>');
    case 'pegaHtmlPagamento':
        if ($_REQUEST['preid']) {
            /* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
             * Analista: Daniel Areas Programador Eduardo Duniceu
            * */
            $sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobra emo
                    INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'
                    UNION ALL
                    SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobrapar emo
                    INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'";

            $pagamentoobra = $db->pegaLinha($sql);

            if ($pagamentoobra) {
                echo "Pago<br>
                      Valor pagamento(R$): " . number_format($pagamentoobra['pagvalorparcela'], 2, ",", ".") . "<br>
                      Nº da Ordem Bancária: " . $pagamentoobra['pagnumeroob'] . "<br>
                      Data do pagamento: " . $pagamentoobra['pagdatapagamento'] . "<br>
                      Banco: " . $pagamentoobra['probanco'] . ", Agência: " . $pagamentoobra['proagencia'] . "
                        ";
            } else {
                echo "Não pago";
            }
        }
        die();
    case 'listaObrasUnidade':
        $obras = new Obras();
        $filtro = $_POST;
        $filtro['not(obridpai)'] = true;
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = ($_POST ? $obras->listaSql($filtro) : array());
        //$cabecalho = array("Ação","Objeto", "Classificação do Objeto", "Tipo do Objeto", "Valor Previsto");
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "formulario");
        die();
    case 'supervisorFNDE':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['empid'] = $_POST['empid'];
        header("Location: obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A");
        exit;
        break;
}

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgao');
echo montarAbasArray($arAba, "?modulo=principal/listaObras&acao=A&orgid=" . $orgid);

monta_titulo('Lista de Obras', 'Filtre as Obras');

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

        $('.novaObra').click(function() {
            window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';
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
    <td style="width: 190px;" class="SubTituloDireita">Agrupar por:</td>
    <td>

        <input type="radio" value="U" name="agrupamento" <?= ($_POST['agrupamento'] == 'U' ? "checked='checked'" : "") ?>> Unidades
        <input type="radio" value="O" name="agrupamento" <?= ($_POST['agrupamento'] == 'U' ? "" : "checked='checked'") ?>> Obras
    </td>
</tr>
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
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
    <td>
        <?php
        $classificacaoObra = new ClassificacaoObra();
        $db->monta_combo("cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid");
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
<?php if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
        <td>
            <?php
            $modalidade = new ModalidadeEnsino();
            $db->monta_combo("moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid");
            ?>
        </td>
    </tr>
<?php endif; ?>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
    <td>
        <?php
        $entidade = new Entidade();
        $db->monta_combo("entid", $entidade->listaComboObras2(), "S", "Todos", "", "", "", 200, "N", "entid");
        ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Última atualização:</td>
    <td>
        <?php
        $ultatualizacao = $_POST["ultatualizacao"];
        $arSel = array(
            array("codigo" => 1, "descricao" => "Em até 45 dias"),
            array("codigo" => 2, "descricao" => "Entre 45 e 60 dias"),
            array("codigo" => 3, "descricao" => "Mais de 60 dias"),
        );
        $db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "200", "N", "ultatualizacao");
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
        echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
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
    <td class="SubTituloDireita">Convênio:</td>
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
    <td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
    <td>
        <input type="radio" name="totalfoto" id="" value="S" <?= ( $_POST["totalfoto"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="totalfoto" id="" value="N" <?= ( $_POST["totalfoto"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="totalfoto" id="" value=""  <?= ( $_POST["totalfoto"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Metodologia Inovadora:</td>
    <td>
        <input type="radio" name="obrami" id="" value="S" <?= ( $_POST["obrami"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="obrami" id="" value="N" <?= ( $_POST["obrami"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="obrami" id="" value=""  <?= ( $_POST["obrami"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
    <td>
        <input type="radio" name="possui_vistoria" id="" value="S" <?= ( $_POST["possui_vistoria"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="possui_vistoria" id="" value="N" <?= ( $_POST["possui_vistoria"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="possui_vistoria" id="" value=""  <?= ( $_POST["possui_vistoria"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
    <td>
        <input type="radio" name="res_estado" id="" value="S" <?= ( $_POST["res_estado"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="res_estado" id="" value="N" <?= ( $_POST["res_estado"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="res_estado" id="" value=""  <?= ( $_POST["res_estado"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Restrição superada:</td>
    <td>
        <input type="radio" name="res_superada" id="" value="S" <?= ( $_POST["res_superada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="res_superada" id="" value="N" <?= ( $_POST["res_superada"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="res_superada" id="" value=""  <?= ( $_POST["res_superada"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
    <td>
        <input type="radio" name="ocraditivado" id="" value="S" <?= ( $_POST["ocraditivado"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="ocraditivado" id="" value="N" <?= ( $_POST["ocraditivado"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="ocraditivado" id="" value=""  <?= ( $_POST["ocraditivado"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui Supervisão por O.S:</td>
    <td>
        <input type="radio" name="responsavel" id="" value="S" <?= ( $_POST["responsavel"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="responsavel" id="" value="N" <?= ( $_POST["responsavel"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="responsavel" id="" value=""  <?= ( $_POST["responsavel"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui Evolução MI:</td>
    <td>
        <input type="radio" name="evomi" id="" value="S" <?= ( $_POST["evomi"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="evomi" id="" value="N" <?= ( $_POST["evomi"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="evomi" id="" value=""  <?= ( $_POST["evomi"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Listar as Obras Vinculantes?</td>
    <td>
        <input type="radio" name="obrvinculada" id="" value="S" <?= ( $_POST["obrvinculada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="obrvinculada" id="" value="N" <?= ( $_POST["obrvinculada"] == "N" || $_POST["obrvinculada"] == "" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="obrvinculada" id="" value="T"  <?= ( $_POST["obrvinculada"] == "T" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Recebeu o primeiro repasse?</td>
    <td>
        <input type="radio" name="repasse" id="" value="S" <?= ( $_POST["repasse"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="repasse" id="" value="N" <?= ( $_POST["repasse"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="repasse" id="" value="T"  <?= ( $_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Medidas de Exceçao?</td>
    <td>
        <input type="radio" name="medidasexcecao" id="" value="S" <?= ( $_POST["medidasexcecao"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="medidasexcecao" id="" value="N" <?= ( $_POST["medidasexcecao"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="medidasexcecao" id="" value="T"  <?= ( $_POST["medidasexcecao"] == "T" || $_POST["medidasexcecao"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>



<tr>
    <td class="SubTituloDireita" style="width: 190px;">Possui mais de um fiscal ativo?</td>
    <td>
        <input type="radio" name="fiscal_ativo" id="" value="S" <?= ( $_POST["fiscal_ativo"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="fiscal_ativo" id="" value="N" <?= ( $_POST["fiscal_ativo"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="fiscal_ativo" id="" value="T"  <?= ( $_POST["fiscal_ativo"] == "T" || $_POST["fiscal_ativo"] == "" ? "checked='checked'" : "" ) ?> /> Todas
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
    <td class="SubTituloDireita" style="width: 190px;">Situação do Instrumento:</td>
    <td>
        <?php
        $tipoSituacaoInstrumento = new SituacaoInstrumento();
        $sql                      = $tipoSituacaoInstrumento->listaCombo();
        $db->monta_combo('stiid', $sql, 'S', "Selecione...", '', 'N', '', '150', 'S', 'stiid');
        ?>
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
    <td class="SubTituloDireita" style="width: 190px;">Tipo Vistoria (Realizado por):</td>
    <td>
        <?php
        $realizacaosupervisao = new RealizacaoSupervisao();
        $db->monta_combo("rsuid", $realizacaosupervisao->listaCombo(), "S", "Todos", "", "", "", 200, "N", "rsuid");
        ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
    <td>
        De:&nbsp;
        <?php
        echo campo_texto('obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
        ?>
        Até:&nbsp;
        <?php
        echo campo_texto('obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
        ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Nível de Preenchimento:</td>
    <td>
        <?php
        $nivelpreenchimento = $_POST["nivelpreenchimento"];
        $arrNivel = array(
            array("codigo" => 1, "descricao" => "Verde ( Obras atualizadas há menos de 44 dias atrás )"),
            array("codigo" => 2, "descricao" => "Amarelo ( Obras atualizadas entre 45 e 59 dias )"),
            array("codigo" => 3, "descricao" => "Vermelho ( Obras atualizadas há 60 dias ou mais )"),
        );
        $db->monta_combo("nivelpreenchimento", $arrNivel, "S", "Todos", "", "", "", "", "N", "nivelpreenchimento");
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
    <td class="SubTituloDireita" style="width: 190px;">Funcionamento:</td>
    <td>
        <table>
            <tr>
                <td>
                    <input type="radio" <?= ($_POST['funcionamentoflag']==1)?'checked':''; ?> name="funcionamentoflag" id="moefuncionamentoflag_s" value="1" />
                    Em Funcionamento:
                </td>
            </tr>
            <tr>
                <td>
                    <input type="radio" <?= ($_POST['funcionamentoflag']==2)?'checked':''; ?> name="funcionamentoflag" id="moefuncionamentoflag_pi" value="2" />
                    Previsão de Inauguração:
                    <?php echo campo_data2('moedtprevinauguracao_i', 'S', true, '', null, '', ' ', '', ''); ?>
                    &nbsp;até&nbsp;
                    <?php echo campo_data2('moedtprevinauguracao_f', 'S', true, '', null, '', ' ', '', ''); ?>
                </td>
            </tr>
            <tr>
                <td>
                    <input type="radio" <?= ($_POST['funcionamentoflag']==3)?'checked':''; ?> name="funcionamentoflag" id="moefuncionamentoflag_if" value="3" />
                    Previsão de Início de Funcionamento:
                    <?php echo campo_data2('moedtpreviniciofunc_i', 'S', true, '', null, '', ' ', '', ''); ?>
                    &nbsp;até&nbsp;
                    <?php echo campo_data2('moedtpreviniciofunc_f', 'S', true, '', null, '', ' ', '', ''); ?>
                </td>
            </tr>
        </table>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" style="width: 190px;">Mobiliário Empenhado:</td>
    <td>
        <input type="radio" name="moeempenhadoflag" id="" value="S" <?= ( $_POST["moeempenhadoflag"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
        <input type="radio" name="moeempenhadoflag" id="" value="N" <?= ( $_POST["moeempenhadoflag"] == "N" ? "checked='checked'" : "" ) ?>/> Não
        <input type="radio" name="moeempenhadoflag" id="" value=""  <?= ( $_POST["moeempenhadoflag"] == "" ? "checked='checked'" : "" ) ?> /> Todas
    </td>
</tr>
</table>
</td>
</tr>
<tr>
    <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
        <?php
        if (possui_perfil(PFLCOD_SUPER_USUARIO) ):
            ?>
            <div style="float:left" class="novaObra" >
                <a style="cursor: pointer;">
                    <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Nova obra"> Nova Obra
                </a>
            </div>
        <?php endif; ?>
        <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
        <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
    </td>
</tr>
</table>
</form>
<?php

if ((array_key_exists('obrid',$_POST) || array_key_exists('esdid',$_POST) || array_key_exists('estuf',$_POST)) || !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC))) {

    if ($_POST['agrupamento'] == 'U') {
        $obras = new Obras();
        $filtro = $_POST;
//		$filtro['NOT(mi)'] 		 = true;
        if ($filtro['req'] == 'excel') {
            $obras->listaUnidadesObrasExcel($filtro);
        } else {
            $obras->listaUnidadesObras($filtro);
        }


    } else {

        $obras = new Obras();
        $filtro = $_POST;

        //ver($filtro); die;

        $filtro['not(obridpai)'] = true;
//		$filtro['NOT(mi)'] 		 = true;

        if ($filtro['req'] == 'excel') {
            ini_set("memory_limit", "1024M");
            $filtro['excel'] = true;
            $cabecalho = $obras->cabecalhoListaSql(true);
            $sql = $obras->listaSql($filtro);
            $obras->listaObrasExcel($sql, $cabecalho);
        } else {
            $cabecalho = $obras->cabecalhoListaSql();
            $sql = $obras->listaSql($filtro);

//            ver(simec_htmlentities($sql),d);

            $paramLista['managerOrder'] = array(13 => 'obrnome',
                17 => 'ocrdtinicioexecucao',
                18 => 'ocrdtterminoexecucao',
                20 => array('campo' => "DATE_PART('days', NOW() - o.obrdtultvistoria)::numeric", 'alias' => "obrdtultvistoria"),
                21 => 'obrdtultvistoria');
            $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", null, null, 300, $paramLista);
            //dbg(simec_htmlentities($sql));
        }
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