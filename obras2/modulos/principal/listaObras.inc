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
        $estuf = $_REQUEST['estuf'];
        ?>

        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>

        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
    case 'unidade':
        header('content-type: text/html; charset=ISO-8859-1');
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="entid[]"]').chosen();

            });
        </script>

        <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php
            $entidade = new Entidade();
            foreach ($entidade->listaComboMulti($_POST['muncod']) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php

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
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css"/>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

var u = '?modulo=principal/listaObras&acao=A&req=pegaHtmlPagamento&preid=', fieldsValue2 = ['moedtprevinauguracao_i', 'moedtprevinauguracao_f'], fieldsValue3 = ['moedtpreviniciofunc_i', 'moedtpreviniciofunc_f'], OPERATION = 1, INAUGURATION = 2, INITIATION = 3;

$(document).ready(function () {


//    $('.estados').change(function() {
//        alert(1);
//        // can now use params.selected and params.deselected
//     //   carregarMunicipio();
//    });


    if ($("input:radio[name='funcionamentoflag']").is(":checked")) {

        var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent().parent()[0]), $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent().parent()[0]);

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

    $('.pesquisar').click(function () {
        $('#req').val('');


        if ($("input:radio[name='funcionamentoflag']:checked").val() == 2) {
            if (!$("input[name='moedtprevinauguracao_i']").val() || !$("input[name='moedtprevinauguracao_f']").val()) {
                alert("Voce precisa preencher o intervalo com as datas");
                $("input[name='moedtprevinauguracao_i']").focus();
                return false;
            }
        }

        if ($("input:radio[name='funcionamentoflag']:checked").val() == 3) {
            if (!$("input[name='moedtpreviniciofunc_i']").val() || !$("input[name='moedtpreviniciofunc_f']").val()) {
                alert("Voce precisa preencher o intervalo com as datas");
                $("input[name='moedtpreviniciofunc_i']").focus();
                return false;
            }
        }

        $('#formListaObra').submit();
    });

    $('.btnEexcel').click(function () {
        $('#req').val('excel');
        $('#formListaObra').submit();
    });

    $('.novaObra').click(function () {
        window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';
    });

    <?php
    if ($abreBuscaAvancada) {
        echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";
    }
    ?>

    $("input:radio[name='funcionamentoflag']").on("click", function (e) {

        for (var i = 0; i < fieldsValue2.length; i++) {
            $("input[name='" + fieldsValue2[i] + "']").val("");
            $("input[name='" + fieldsValue3[i] + "']").val("");
        }

        var $spaceDate1 = $($("input[name='moedtprevinauguracao_i']").parent()[0]), $spaceDate2 = $($("input[name='moedtpreviniciofunc_i']").parent()[0]);

        $spaceDate1.find("img").css("display", "");
        $spaceDate2.find("img").css("display", "");

        switch (parseInt($(this).val())) {
            case OPERATION:
                $spaceDate1.find("img").css("display", "none");
                $spaceDate2.find("img").css("display", "none");
                for (var i = 0; i < fieldsValue2.length; i++) {
                    $("input[name='" + fieldsValue2[i] + "']").attr("disabled", true);
                    $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                }
                break;
            case INAUGURATION:
                $spaceDate2.find("img").css("display", "none");
                for (var i = 0; i < fieldsValue3.length; i++) {
                    $("input[name='" + fieldsValue2[i] + "']").attr("disabled", false);
                    $("input[name='" + fieldsValue3[i] + "']").attr("disabled", true);
                }
                break;
            case INITIATION:
                $spaceDate1.find("img").css("display", "none");
                for (var i = 0; i < fieldsValue3.length; i++) {
                    $("input[name='" + fieldsValue2[i] + "']").attr("disabled", true);
                    $("input[name='" + fieldsValue3[i] + "']").attr("disabled", false);
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


    var td = $1_11('.td_municipio');
    if (estuf != '') {
        var url = location.href;
        $1_11.ajax({
            url: url,
            type: 'post',
            data: {
                ajax: 'municipio',
                estuf: values
            },
            dataType: "html",
            async: false,
            beforeSend: function () {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function () {
                divCarregado();
                alert(2);
            },
            success: function (data) {
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
    var td = $1_11('#td_unidade');

    if (muncod != '') {
        var url = location.href;
        $1_11.ajax({
            url: url,
            type: 'post',
            data: {ajax: 'unidade', muncod: muncod},
            dataType: "html",
            async: false,
            beforeSend: function () {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function () {
                divCarregado();
            },
            success: function (data) {
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
/**
 * Alterar visibilidade de um campo.
 *
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo(campo) {
    var div_on = document.getElementById(campo + '_campo_on');
    var div_off = document.getElementById(campo + '_campo_off');
    var input = document.getElementById(campo + '_campo_flag');
    if (div_on.style.display == 'none') {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
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
    <td class="SubTituloDireita">
        Situação:
    </td>
    <td>
        <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php $situacaoRegistro = new SituacaoRegistro();
            foreach ($situacaoRegistro->listaCombo() as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita">
        Tipo de Obra:
    </td>
    <td>
        <select name="tobid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php  $tipoObra = new TipoObra();
            foreach ($tipoObra->listaCombo() as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($tobid) && in_array($key['codigo'], $tobid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita">
        Classificação da Obra:
    </td>
    <td>
        <select name="cloid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php  $classificacaoObra = new ClassificacaoObra();
            foreach ($classificacaoObra->listaCombo() as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($cloid) && in_array($key['codigo'], $cloid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
    </td>
</tr>
<?php
if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL):
    ?>
    <tr>

        <td class="SubTituloDireita">
            Tipologia:
        </td>
        <td>
            <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php   $tipologiaObra = new TipologiaObra();
                $param = array("orgid" => $_SESSION['obras2']['orgid']);
                $dados = $tipologiaObra->listaCombo($param, false);


                foreach ($dados as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($tpoid) && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>
    </tr>
<?php
endif;
?>
<tr>
    <td class="SubTituloDireita">
        Programa:
    </td>
    <td>
        <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php  $programa = new ProgramaFonte();
            $param = array("orgid" => $_SESSION['obras2']['orgid']);
            foreach ($programa->listacombo($param, false) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita">
        Fonte:
    </td>
    <td>
        <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php  $tipoOrigemObra = new TipoOrigemObra();
            $param = array();
            foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
    </td>
</tr>
<?php if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
    <tr>
        <?php


        ?>
        <td class="SubTituloDireita">
            Modalidade de Ensino:
        </td>
        <td>
            <select name="moeid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php  $modalidade = new ModalidadeEnsino();
                foreach ($modalidade->listaCombo() as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($moeid) && in_array($key['codigo'], $moeid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>

    </tr>
<?php endif; ?>
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

    <td class="SubTituloDireita">UF(s):</td>
    <td>

        <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
            <?php  $uf = new Estado();
            foreach ($uf->listaCombo() as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($estuf) && in_array($key['codigo'], $estuf)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>


    </td>


</tr>
<tr>
    <td class="SubTituloDireita">Municípios(s):</td>
    <td class="td_municipio">
        <?php if (!empty($estuf)) { ?>

            <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                <?php   $municipio = new Municipio();
                foreach ($municipio->listaComboMulti($estuf) as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        <?php } ?>
    </td>
</tr>

<tr>
    <td class="SubTituloDireita">Unidade:</td>
    <td id="td_unidade">
        <?php if(!empty($muncod)){?>
            <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php
                $entidade = new Entidade();
                foreach ($entidade->listaComboMulti($muncod) as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        <?php } ?>
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

<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
        <td>
            <input type="radio" name="totalfoto" id=""
                   value="S" <?= ($_POST["totalfoto"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="totalfoto" id=""
                   value="N" <?= ($_POST["totalfoto"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="totalfoto" id=""
                   value=""  <?= ($_POST["totalfoto"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Metodologia Inovadora:</td>
        <td>
            <input type="radio" name="obrami" id=""
                   value="S" <?= ($_POST["obrami"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="obrami" id=""
                   value="N" <?= ($_POST["obrami"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="obrami" id=""
                   value=""  <?= ($_POST["obrami"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
        <td>
            <input type="radio" name="possui_vistoria" id=""
                   value="S" <?= ($_POST["possui_vistoria"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="possui_vistoria" id=""
                   value="N" <?= ($_POST["possui_vistoria"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="possui_vistoria" id=""
                   value=""  <?= ($_POST["possui_vistoria"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
        <td>
            <input type="radio" name="res_estado" id=""
                   value="S" <?= ($_POST["res_estado"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="res_estado" id=""
                   value="N" <?= ($_POST["res_estado"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="res_estado" id=""
                   value=""  <?= ($_POST["res_estado"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Restrição superada:</td>
        <td>
            <input type="radio" name="res_superada" id=""
                   value="S" <?= ($_POST["res_superada"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="res_superada" id=""
                   value="N" <?= ($_POST["res_superada"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="res_superada" id=""
                   value=""  <?= ($_POST["res_superada"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
        <td>
            <input type="radio" name="ocraditivado" id=""
                   value="S" <?= ($_POST["ocraditivado"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="ocraditivado" id=""
                   value="N" <?= ($_POST["ocraditivado"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="ocraditivado" id=""
                   value=""  <?= ($_POST["ocraditivado"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui Supervisão por O.S:</td>
        <td>
            <input type="radio" name="responsavel" id=""
                   value="S" <?= ($_POST["responsavel"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="responsavel" id=""
                   value="N" <?= ($_POST["responsavel"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="responsavel" id=""
                   value=""  <?= ($_POST["responsavel"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui Evolução MI:</td>
        <td>
            <input type="radio" name="evomi" id=""
                   value="S" <?= ($_POST["evomi"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="evomi" id=""
                   value="N" <?= ($_POST["evomi"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="evomi" id=""
                   value=""  <?= ($_POST["evomi"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Listar as Obras Vinculantes?</td>
        <td>
            <input type="radio" name="obrvinculada" id=""
                   value="S" <?= ($_POST["obrvinculada"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="obrvinculada" id=""
                   value="N" <?= ($_POST["obrvinculada"] == "N" || $_POST["obrvinculada"] == "" ? "checked='checked'" : "") ?>/>
            Não
            <input type="radio" name="obrvinculada" id=""
                   value="T"  <?= ($_POST["obrvinculada"] == "T" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Recebeu o primeiro repasse?</td>
        <td>
            <input type="radio" name="repasse" id=""
                   value="S" <?= ($_POST["repasse"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="repasse" id=""
                   value="N" <?= ($_POST["repasse"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="repasse" id=""
                   value="T"  <?= ($_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Medidas de Exceçao?</td>
        <td>
            <input type="radio" name="medidasexcecao" id=""
                   value="S" <?= ($_POST["medidasexcecao"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="medidasexcecao" id=""
                   value="N" <?= ($_POST["medidasexcecao"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="medidasexcecao" id=""
                   value="T"  <?= ($_POST["medidasexcecao"] == "T" || $_POST["medidasexcecao"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui mais de um fiscal ativo?</td>
        <td>
            <input type="radio" name="fiscal_ativo" id=""
                   value="S" <?= ($_POST["fiscal_ativo"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="fiscal_ativo" id=""
                   value="N" <?= ($_POST["fiscal_ativo"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="fiscal_ativo" id=""
                   value="T"  <?= ($_POST["fiscal_ativo"] == "T" || $_POST["fiscal_ativo"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist administrativo?</td>
        <td>
            <input type="radio" name="chk_adm" id=""
                   value="S" <?= ($_POST["chk_adm"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_adm" id=""
                   value="N" <?= ($_POST["chk_adm"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_adm" id=""
                   value="T"  <?= ($_POST["chk_adm"] == "T" || $_POST["chk_adm"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist 2° Parcela?</td>
        <td>
            <input type="radio" name="chk_2pc" id=""
                   value="S" <?= ($_POST["chk_2pc"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_2pc" id=""
                   value="N" <?= ($_POST["chk_2pc"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_2pc" id=""
                   value="T"  <?= ($_POST["chk_2pc"] == "T" || $_POST["chk_2pc"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist Técnico?</td>
        <td>
            <input type="radio" name="chk_tec" id=""
                   value="S" <?= ($_POST["chk_tec"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_tec" id=""
                   value="N" <?= ($_POST["chk_tec"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_tec" id=""
                   value="T"  <?= ($_POST["chk_tec"] == "T" || $_POST["chk_tec"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui checklist Obra Vinculada?</td>
        <td>
            <input type="radio" name="chk_vinc" id=""
                   value="S" <?= ($_POST["chk_vinc"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_vinc" id=""
                   value="N" <?= ($_POST["chk_vinc"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_vinc" id=""
                   value="T"  <?= ($_POST["chk_vinc"] == "T" || $_POST["chk_vinc"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui Marcador de Órgão de Controle?</td>
        <td>
            <input type="radio" name="chk_org_controle" id=""
                   value="S" <?= ($_POST["chk_org_controle"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_org_controle" id=""
                   value="N" <?= ($_POST["chk_org_controle"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_org_controle" id=""
                   value="T"  <?= ($_POST["chk_org_controle"] == "T" || $_POST["chk_org_controle"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Possui conta bloqueada?</td>
        <td>
            <input type="radio" name="chk_conta_bloqueada" id=""
                   value="S" <?= ($_POST["chk_conta_bloqueada"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_conta_bloqueada" id=""
                   value="N" <?= ($_POST["chk_conta_bloqueada"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_conta_bloqueada" id=""
                   value="T"  <?= ($_POST["chk_conta_bloqueada"] == "T" || $_POST["chk_conta_bloqueada"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Obra de processo anterior à implantação do sistema?</td>
        <td>
            <input type="radio" name="chk_obrprocessoanterior" id=""
                   value="S" <?= ($_POST["chk_obrprocessoanterior"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="chk_obrprocessoanterior" id=""
                   value="N" <?= ($_POST["chk_obrprocessoanterior"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="chk_obrprocessoanterior" id=""
                   value="T"  <?= ($_POST["chk_obrprocessoanterior"] == "T" || $_POST["chk_obrprocessoanterior"] == "" ? "checked='checked'" : "") ?> />
            Todas
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>

        <td class="SubTituloDireita">
            Situação do Instrumento:
        </td>
        <td>
            <select name="stiid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php   $tipoSituacaoInstrumento = new SituacaoInstrumento();
                foreach ($tipoSituacaoInstrumento->listaCombo() as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($stiid) && in_array($key['codigo'], $stiid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita">
            Situação do Registro
        </td>
        <td>
            <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php
                $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                $dados = $db->carregar($sql);
                foreach ($dados as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($esdid) && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita">
            Tipo Vistoria (Realizado por):
        </td>
        <td>
            <select name="rsuid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php
                $realizacaosupervisao = new RealizacaoSupervisao();
                foreach ($realizacaosupervisao->listaCombo() as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($rsuid) && in_array($key['codigo'], $rsuid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>

    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
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
<? endif; ?>
<? if (!verificaTecnico()) : ?>
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
<? endif; ?>

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
                $percentualfinal = $_POST['percentualfinal'];
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
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Funcionamento:</td>
        <td>
            <table>
                <tr>
                    <td>
                        <input type="radio" <?= ($_POST['funcionamentoflag'] == 1) ? 'checked' : ''; ?>
                               name="funcionamentoflag" id="moefuncionamentoflag_s" value="1"/>
                        Em Funcionamento:
                    </td>
                </tr>
                <tr>
                    <td>
                        <input type="radio" <?= ($_POST['funcionamentoflag'] == 2) ? 'checked' : ''; ?>
                               name="funcionamentoflag" id="moefuncionamentoflag_pi" value="2"/>
                        Previsão de Inauguração:
                        <?php echo campo_data2('moedtprevinauguracao_i', 'S', true, '', null, '', ' ', '', ''); ?>
                        &nbsp;até&nbsp;
                        <?php echo campo_data2('moedtprevinauguracao_f', 'S', true, '', null, '', ' ', '', ''); ?>
                    </td>
                </tr>
                <tr>
                    <td>
                        <input type="radio" <?= ($_POST['funcionamentoflag'] == 3) ? 'checked' : ''; ?>
                               name="funcionamentoflag" id="moefuncionamentoflag_if" value="3"/>
                        Previsão de Início de Funcionamento:
                        <?php echo campo_data2('moedtpreviniciofunc_i', 'S', true, '', null, '', ' ', '', ''); ?>
                        &nbsp;até&nbsp;
                        <?php echo campo_data2('moedtpreviniciofunc_f', 'S', true, '', null, '', ' ', '', ''); ?>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
<? endif; ?>
<? if (!verificaTecnico()) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Mobiliário Empenhado:</td>
        <td>
            <input type="radio" name="moeempenhadoflag" id=""
                   value="S" <?= ($_POST["moeempenhadoflag"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="moeempenhadoflag" id=""
                   value="N" <?= ($_POST["moeempenhadoflag"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="moeempenhadoflag" id=""
                   value=""  <?= ($_POST["moeempenhadoflag"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>

<? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">Concluídas após vigência:</td>
        <td>
            <input type="radio" name="posvigencia" id=""
                   value="S" <?= ($_POST["posvigencia"] == "S" ? "checked='checked'" : "") ?>/> Sim
            <input type="radio" name="posvigencia" id=""
                   value="N" <?= ($_POST["posvigencia"] == "N" ? "checked='checked'" : "") ?>/> Não
            <input type="radio" name="posvigencia" id=""
                   value=""  <?= ($_POST["posvigencia"] == "" ? "checked='checked'" : "") ?> /> Todas
        </td>
    </tr>
<? endif; ?>
</table>
</td>
</tr>
<tr>
    <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
        <?php
        if (possui_perfil(PFLCOD_SUPER_USUARIO)):
            ?>
            <div style="float:left" class="novaObra">
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

if ((array_key_exists('obrid', $_POST) || array_key_exists('esdid', $_POST) || array_key_exists('estuf', $_POST)) || !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC))) {

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
                Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for
                escolhido serão apresentados todos os registros que você pode acessar.
            </td>
        </tr>
    </table>
<?php
}
?>

<script type='text/javascript'>
    $(function () {
        $('#esdid').change(function () {
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
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($param['obrbuscatexto']))));
        $obrbuscatextoTemp = trim($obrbuscatextoTemp);

        if (!strpos($obrbuscatextoTemp, ',')) {
            $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . trim($param['obrbuscatexto']) . "%') ) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c) {
                $c = trim($c);
                $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($param['empid'])
        $where['empid'] = "o.empid IN(" . $param['empid'] . ")";


    if ($param['strid'] && $param['strid'][0] != '')
        $where['strid'] = "str.strid IN(" . implode(',', $param['strid']) . ")";
    if ($param['tobid'] && $param['tobid'][0] != '')
        $where['tobid'] = "o.tobid IN(" . implode(',', $param['tobid']) . ")";
    if ($param['tooid'] && $param['tooid'][0] != '')
        $where['tooid'] = "o.tooid IN(" . implode(',', $param['tooid']) . ")";
    if ($param['cloid'] && $param['cloid'][0] != '')
        $where['cloid'] = "o.cloid IN(" . implode(',', $param['cloid']) . ")";
    if ($param['prfid'] && $param['prfid'][0] != '')
        $where['prfid'] = "e.prfid IN(" . implode(',', $param['prfid']) . ")";
    if ($param['moeid'] && $param['moeid'][0] != '')
        $where['moeid'] = "e.moeid IN(" . implode(',', $param['moeid']) . ")";
    if ($param['entid'])
        $where['entid'] = "o.entid IN(" . implode(',', $param['entid']) . ")";
    if ($param['ultatualizacao']) {
        switch ($param['ultatualizacao']) {
            case 1:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) < 45";
                break;
            case 2:
                $where['ultatualizacao'] = "( DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) >= 45 AND
                                  DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 60 )";
                break;
            case 3:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 60";
                break;
        }
    }

    if ($param['tpoid'] && $param['tpoid'][0] != '')
        $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $param['tpoid']) . "')";
    if ($param['estuf'] && $param['estuf'][0] != '')
        $where['estuf'] = "mun.estuf IN ('" . implode("', '", $param['estuf']) . "')";
    if ($param['muncod'] && $param['muncod'][0] != '')
        $where['muncod'] = "mun.muncod IN ('" . implode("', '", $param['muncod']) . "')";
    if ($param['empesfera'])
        $where['empesfera'] = "e.empesfera IN('" . $param['empesfera'] . "')";
    if ($param['processo'])
        $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
    if ($param['ano_processo'])
        $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
    if ($param['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
    if ($param['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";
    if ($param['totalfoto'] == 'S')
        $where['totalfoto'] = "obrsnfotos = true";
    elseif ($param['totalfoto'] == 'N')
        $where['totalfoto'] = "obrsnfotos = false OR obrsnfotos IS NULL";
    if ($param['obrami'] == 'S')
        $where['obrami'] = "o.tpoid IN (104, 105)";
    elseif ($param['obrami'] == 'N')
        $where['obrami'] = "o.tpoid NOT IN (104, 105)";
    if ($param['possui_vistoria'] == 'S')
        $where['possui_vistoria'] = "obrdtultvistoria IS NOT NULL";
    elseif ($param['possui_vistoria'] == 'N')
        $where['possui_vistoria'] = "o.obrdtultvistoria IS NULL";
    if ($param['res_estado'] == 'S')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_estado'] == 'N')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['res_superada'] == 'S')
        $where['res_superada'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_superada'] == 'N')
        $where['res_superad'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['ocraditivado'] == 'S')
        $where['ocraditivado'] = "c.ttaid > 0";
    elseif ($param['ocraditivado'] == 'N')
        $where['ocraditivado'] = " (c.ttaid IS NULL OR c.ttaid=0) ";
    if ($param['responsavel'] == 'S')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NOT NULL";
    elseif ($param['responsavel'] == 'N')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NULL";
    if ($param['evomi'] == 'S') {
        $where['evmi'] = "evmi.emidtinclusao IS NOT NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    } elseif ($param['evomi'] == 'N') {
        $where['evmi'] = "evmi.emidtinclusao IS NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    }
    switch ($param['obrvinculada']) {
        case 'S':
            $where['obrvinculada'] = " o.obrstatus = 'P' ";
            break;
        case 'N':
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
        case 'T':
            $where['obrvinculada'] = " o.obrstatus IN ('A', 'P') ";
            break;
        default:
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
    }
    if ($param['repasse'] == 'S') {
        $where['repasse'] = "o.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
    } elseif ($param['repasse'] == 'N') {
        $where['repasse'] = "o.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')";
    }
    if ($param['medidasexcecao'] == 'S')
        $where['medidasexcecao'] = "medidasexcecao = TRUE ";
    elseif ($param['medidasexcecao'] == 'N')
        $where['medidasexcecao'] = "medidasexcecao = FALSE ";
    if ($param['fiscal_ativo'] == 'S')
        $where['fiscal_ativo'] = "e.empid IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";
    elseif ($param['fiscal_ativo'] == 'N')
        $where['fiscal_ativo'] = " e.empid NOT IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";


    if ($param['posvigencia'] == 'S')
        $where['posvigencia'] = "o.obrid IN (select distinct
                                obrid
                                from obras2.supervisao sup where posvigencia = 't' and supstatus = 'A')";
    elseif ($param['posvigencia'] == 'N')
        $where['posvigencia'] = "o.obrid IN (select  distinct
                                sup.obrid
                                from obras2.supervisao sup where posvigencia = 'f' and supstatus = 'A')";





    $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";
    if ($param['chk_adm'] == 'S')
        $where['chk_adm'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    elseif ($param['chk_adm'] == 'N')
        $where['chk_adm'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    if ($param['chk_2pc'] == 'S')
        $where['chk_2pc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    elseif ($param['chk_2pc'] == 'N')
        $where['chk_2pc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    if ($param['chk_tec'] == 'S')
        $where['chk_tec'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    elseif ($param['chk_tec'] == 'N')
        $where['chk_tec'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    if ($param['chk_vinc'] == 'S')
        $where['chk_vinc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";
    elseif ($param['chk_vinc'] == 'N')
        $where['chk_vinc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";

    if ($param['chk_org_controle'] == 'S')
        $where['chk_org_controle'] = " o.obrorgcontrole = 't' ";
    elseif ($param['chk_org_controle'] == 'N')
        $where['chk_org_controle'] = " o.obrorgcontrole = 'f'";

    if ($param['chk_conta_bloqueada'] == 'S')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 't'";
    elseif ($param['chk_conta_bloqueada'] == 'N')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 'f'";

    if ($param['chk_obrprocessoanterior'] == 'S')
        $where['chk_obrprocessoanterior'] = " o.obrprocessoanterior = 't'";
    elseif ($param['chk_obrprocessoanterior'] == 'N')
        $where['chk_cobrprocessoanterior'] = " o.obrprocessoanterior = 'f'";

    if ($param['stiid'] && $param['stiid'][0] != '')
        $where['stiid'] = "o.stiid IN (" . implode(', ', $param['stiid']) . ")";

    if ($param['esdid'] && $param['esdid'][0] != '')
        $where['esdid'] = "d.esdid IN (" . implode(', ', $param['esdid']) . ")";

    if ($param['rsuid'] && $param['rsuid'][0] != '')
        $where['rsuid'] = "rsuid IN (" . implode(', ', $param['rsuid']) . ")";

    if ($param['obrvalorprevisto_menor'])
        $where['obrvalorprevisto_menor'] = "o.obrvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_menor']);
    if ($param['obrvalorprevisto_maior'])
        $where['obrvalorprevisto_maior'] = "o.obrvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_maior']);
    switch ($param['nivelpreenchimento']) {
        /* 1 - Verde    -> x < 45              */
        /* 2 - Amarelo  -> 45 > x < 60         */
        /* 3 - Vermelho -> x > 60              */
        case 1 : //Verde
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) < 45 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) < 45 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
        case 2 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( (DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 45) AND DATE_PART('days', NOW() - o.obrdtultvistoria ) < 60 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( (DATE_PART('days', NOW() - sup.supdata  ) >= 45) AND DATE_PART('days', NOW() - sup.supdata ) < 60  )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END";
            break;
        case 3 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) >= 60 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) >= 60 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
    }
    if ($param['nivelpreenchimento'] || $param['rsuid'])
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";

    if ($param['percentualinicial'] != '')
        $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " . $param['percentualinicial'];
    if ($param['percentualfinal'] != '')
        if ($param['percentualfinal'] < 100)
            $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
    if ($param['funcionamentoflag']) {
        switch ($param['funcionamentoflag']) {
            case 1: // em funcionamento
                $queryComplement = 'AND moedtiniciofunc < NOW()';
                break;
            case 2: // previsão de inauguração
                $queryComplement = 'AND moedtprevinauguracao BETWEEN \'' . ajusta_data($param['moedtprevinauguracao_i']) . '\' AND \'' . ajusta_data($param['moedtprevinauguracao_f']) . '\'';
                break;
            case 3: //previsão inicio de funcionamento
                $queryComplement = 'AND moedtpreviniciofunc BETWEEN \'' . ajusta_data($param['moedtpreviniciofunc_i']) . '\' AND \'' . ajusta_data($param['moedtpreviniciofunc_f']) . '\'';
                break;
        }

        $where['funcionamentoflag'] = sprintf("CASE WHEN (SELECT
                            CASE WHEN moeid IS NOT NULL THEN TRUE ELSE FALSE END
                            FROM obras2.mobiliarioempenhado
                            WHERE obrid = o.obrid %s
                            ORDER BY moeid DESC
                            LIMIT 1) THEN true
                         ELSE false END", $queryComplement);
    }
    if ($param['moeempenhadoflag'] == 'S') {
        $where['moeempenhadoflag'] = "o.obrid IN (select obrid from(
													    select distinct sov.obrid, sum(es.saldo) as saldo
													    from par.subacaoobravinculacao sov
													        inner join par.v_saldo_empenho_por_subacao es on es.sbaid = sov.sbaid and es.eobano = sov.sovano
													    where
													        sov.obrid is not null
													    group by sov.obrid
													) as foo
													where saldo > 0
    												union all
    												SELECT obrid FROM obras2.mobiliarioempenhado
			                                          WHERE moeid IN (
			                                                          SELECT MAX(moeid)
			                                                          FROM obras2.mobiliarioempenhado
			                                                          GROUP BY obrid) AND moeempenhadoflag = 'S')";
    } elseif ($param['moeempenhadoflag'] == 'N') {
        $where['moeempenhadoflag'] = "o.obrid NOT IN (select obrid from(
														    select distinct sov.obrid, sum(es.saldo) as saldo
														    from par.subacaoobravinculacao sov
														        inner join par.v_saldo_empenho_por_subacao es on es.sbaid = sov.sbaid and es.eobano = sov.sovano
														    where
														        sov.obrid is not null
														    group by sov.obrid
														) as foo
														where saldo > 0
    													union all
    													SELECT obrid FROM obras2.mobiliarioempenhado
														  WHERE moeid IN (
																		  SELECT MAX(moeid)
																		  FROM obras2.mobiliarioempenhado
																		  GROUP BY obrid) AND moeempenhadoflag = 'S')";
    }
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

        $arrObr = array(
            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
        );

        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = Array(PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC);

        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

        $resp = array();
        $arPflcod = array();
        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
        }

    }

    $imgNovoObj = '';
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
        $imgNovoObj = "' ||
                                    CASE WHEN ed.esdid = " . ESDID_OBJ_PARALISADO . " AND o.obrstatus != 'P' THEN
                                        '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/editar_nome.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: novaObrVinculada(\'' || o.obrid || '\');\"
                                                title=\"Cadastrar nova obra vinculada\">'
                                    ELSE
                                            '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                                    END || '";
    }

    $coluns['acao'] = "'<center><div style=\"width:100px\">
                    {$imgNovoObj}
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/edit_on.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreListaSupervisaoFnde(\'' || o.obrid || '\',\'' || o.empid || '\');\"
                            title=\"Supervisão FNDE\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/seriehistorica_ativa.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreEvolucaoFinan(\'' || o.obrid || '\');\"
                            title=\"Evolução Financeira\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                            title=\"Alterar Obra\" />
              </div></center>' AS acao";

    $coluns['r'] = "
                        CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as r ";
    $coluns['i'] = "
                         CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as i";
    $coluns['vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            '<img title=\"Com obra vinculada\" src=\"/imagens/check_p.gif\"/>'
                        ELSE
                            ''
                        END AS vinculada";


    $coluns['excel_r'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END as excel_r";

    $coluns['excel_i'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END as excel_i";

    $coluns['excel_vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            'SIM'
                        ELSE
                            'NÃO'
                        END AS vinculada";


    $coluns['obrid'] = "o.obrid";
    $coluns['preid'] = "o.preid";
    $coluns['pronumeroprocesso'] = (verificaTecnico()) ? 'p_conv.pronumeroprocesso' : "'<span class=\"processo_detalhe\">'||p_conv.pronumeroprocesso||'</span>'";
    $coluns['excel_pronumeroprocesso'] = "p_conv.pronumeroprocesso";
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";
    $coluns['obrnome'] = "
                        CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                            '<b style=\"color:green\">(EP) </b>'
                        ELSE
                            ''
                        END || '<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['excel_obrnome'] = "'(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['ocrdtordemservico'] = "TO_CHAR(oc.ocrdtordemservico, 'dd/mm/YYYY') AS ocrdtordemservico";
    $coluns['mesdsc'] = "meso.mesdsc";
    $coluns['entnome'] = "uni.entnome";
    $coluns['mundescricao'] = "mun.mundescricao";
    $coluns['estuf'] = "mun.estuf";
    $coluns['inicio'] = "TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio";
    $coluns['termino'] = "TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino";
    $coluns['strdsc'] = "
                    CASE
                        WHEN str.strid = " . STRID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                        ELSE str.strdsc
                    END AS situacao
                    ";
    $coluns['excel_strdsc'] = "str.strdsc AS situacao";
    $coluns['percentual_execucao'] = "((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao";
    $coluns['tpldsc'] = "sup.tpldsc";
    $coluns['ultima_atualizacao'] = "
                    -- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '<span style=\"color:#3276B1\">' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</span>'
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '<span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '<span>'
                    ELSE
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    END as ultima_atualizacao";
    $coluns['excel_ultima_atualizacao'] = "to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao";
    $coluns['qtddias'] = " DATE_PART( 'days', NOW() - o.obrdtultvistoria )::text AS qtddias";
    $coluns['obrpercentultvistoria'] = "o.obrpercentultvistoria || '%' as obrpercentultvistoria";
    $coluns['empdtultvistoriaempresa'] = "to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa";
    $coluns['programa'] = "pf.prfdesc programa";
    $coluns['fonte'] = "too.toodescricao fonte";
    $coluns['esfera'] = "
                    CASE
                        WHEN e.empesfera = 'M' THEN 'Municipal'
                        WHEN e.empesfera = 'E' THEN 'Estadual'
                        WHEN e.empesfera = 'F' THEN 'Federal'
                    ELSE ''
                    END as esfera";
    $coluns['tpodsc'] = "tpo.tpodsc";
    $coluns['ocrvalorexecucao'] = "ocrvalorexecucao";
    $coluns['obrvalorprevisto'] = "o.obrvalorprevisto";


    if ($param['excel']) {
        $join['mesoregiao'] = "LEFT JOIN territorios.mesoregiao meso ON meso.mescod = mun.mescod";
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";

        unset($coluns['r']);
        unset($coluns['i']);
        unset($coluns['vinculada']);
        unset($coluns['acao']);
        unset($coluns['obrnome']);
        unset($coluns['ultima_atualizacao']);
        unset($coluns['strdsc']);
        unset($coluns['pronumeroprocesso']);
    } else {
        unset($coluns['ocrdtordemservico']);
        unset($coluns['tipoparalisacao']);
        unset($coluns['mesoregiao']);
        unset($coluns['programa']);
        unset($coluns['fonte']);
        unset($coluns['esfera']);
        unset($coluns['termo']);
        unset($coluns['situacao']);
        unset($coluns['qtddias']);
        unset($coluns['processoobra']);
        unset($coluns['anoprocesso']);
        unset($coluns['qtddias']);
        unset($coluns['mesdsc']);
        unset($coluns['tpldsc']);
        unset($coluns['obrvalorprevisto']);
        unset($coluns['excel_r']);
        unset($coluns['excel_pronumeroprocesso']);
//        unset($coluns['excel_r_pendentes']);
//        unset($coluns['excel_r_atraso']);
        unset($coluns['excel_i']);
//        unset($coluns['excel_i_pendentes']);
//        unset($coluns['excel_i_atraso']);
        unset($coluns['excel_vinculada']);
        unset($coluns['excel_obrnome']);
        unset($coluns['excel_ultima_atualizacao']);
        unset($coluns['excel_strdsc']);
    }


    if (verificaTecnico()) {
        unset($coluns['percentual_execucao']);
    }


    $sqlBase = "
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade

                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                    o.obridpai IS NULL
                    " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
            ) as f
            ORDER BY f.obrid ASC
    ";
    return $sqlBase;
}

function getCabecalho($xls = false)
{
    $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Ano Processo', 'Termo/Nº Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregião', 'Unidade Implantadora', 'Município', 'UF', 'Data de Inícioda Execução', 'Data de Términoda Execução', 'Situação da Obra', 'Percentual Execução', 'Tipo de Paralisação', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto');

    if (!$xls)
        $cabecalho = array('Ação', 'R', 'I', 'V', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data de Término da Execução', 'Situação da Obra', 'Percentual Execução', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    if (verificaTecnico()) {
        $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Ano Processo', 'Termo/Nº Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregião', 'Unidade Implantadora', 'Município', 'UF', 'Data de Inícioda Execução', 'Data de Términoda Execução', 'Situação da Obra', 'Tipo de Paralisação', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto');

        if (!$xls)
            $cabecalho = array('Ação', 'R', 'I', 'V', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data de Término da Execução', 'Situação da Obra', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    }

    return $cabecalho;
}

function verificaTecnico()
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CGIMP_GESTOR, PFLCOD_CGIMP_TECNICO))) {
        return false;
    } else {
        return true;
    }
}

?>

<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }
</style>


<script>
    $1_11(document).ready(function () {
        $1_11('select[name="strid[]"]').chosen();
        $1_11('select[name="tobid[]"]').chosen();
        $1_11('select[name="cloid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="moeid[]"]').chosen();
        $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="entid[]"]').chosen();
        $1_11('select[name="stiid[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();
        $1_11('select[name="rsuid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();


        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });

        $1_11(".td_municipio").on( "change", ".municipios",function() {
            carregarUnidade($1_11(this).chosen().val());
        });

    });

</script>