<?php
// empreendimento || obra || orgao
//verificaSessao( 'empreendimento' );
//$empid = $_SESSION['obras2']['empid'];
//$empreendimento = new Empreendimento( $empid );




require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$somenteLeitura = 'S';
$habilitado = true;
?>
<table class="tabela" bgcolor="" cellSpacing="1" cellPadding="3" align="center" border="0" >
    <tr>
        <td colspan="2">
            <?php echo monta_cabecalho_relatorio(100); ?>
        </td>
    </tr>
    <tr>
        <td colspan="2">
<?php
$sosid = ($_REQUEST['sosid'] ? $_REQUEST['sosid'] : $_SESSION['obras2']['sosid']);

$sue = new SupervisaoEmpresa();
$obra = new Obras();
$obrid = $obra->pegaIdObraPorEmpid($_SESSION['obras2']['empid']);


$dadosMi = pegaDadosTecnologiaMi ($obrid[0], $_SESSION['obras2']['sueid'], $sosid);
$qstescopo = ($dadosMi) ? $dadosMi['qstescopo'] : "SE";

$questao = new Questao();

$os = new Supervisao_Os($sosid);
if($os->sosterreno == 't'){
    $qstescopo = "QSTER";
}

$arFiltro = array("qstescopo" => $qstescopo,
    "orgid" => $_SESSION['obras2']['orgid'],
    "sueid" => ($sueid ? $sueid : 0));
$arDados = $questao->pegaTodaEstrutura($arFiltro);

$etqidUlt = null;
$itcidUlt = null;
$qstidUlt = null;

$subQuestao = new SubQuestao();



foreach ($arDados as $k => $dados) {

    $arDadosSubQuestao = $subQuestao->pegaSubQuestaoPorQstid($dados['qstid']);

    $qstctrlobs = json_decode($dados['qstctrlobs']);
    switch ($dados['qtsresposta']) {
        case 't':
            $obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
            break;
        case 'f':
            $obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
            break;
        case 'n':
            $obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');
            break;
        default:
            $obsClass = 'divQuestaoNivel3_none';
            break;
    }

    $qstctrlimg = json_decode($dados['qstctrlimagem']);
    switch ($dados['qtsresposta']) {
        case 't':
            $imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
            break;
        case 'f':
            $imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
            break;
        case 'n':
            $imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');
            break;
        default:
            $imgClass = 'divQuestaoNivel4_none';
            break;
    }


    // ABRE Questionário
    if ($etqidUlt != $dados['etqid']) {
        if (!is_null($etqidUlt)) {
            $html .= "</div>";
        }
        $etqidUlt = $dados['etqid'];
        $html .= "<div id='fieldgroup'>
										      	<legend>{$dados['etqdsc']}</legend>";
    }

    // ETAPA
    if ($itcidUlt != $dados['itcid']) {
        $itcidNum++;
        $html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdsc']}</div>";
        $itcidUlt = $dados['itcid'];
    }

    // DIVISÃO
    if ($dvqidUlt != $dados['dvqid'] && !empty($dados['dvqid'])) {
        $html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";
        $dvqidUlt = $dados['dvqid'];
    }


    // QUESTÃO
    if ($qstidUlt != $dados['qstid']) {

        // Sub Questão
        $subClass = 'divSubQuestao';
        $subItemClass = 'divSubItemQuestao';

        $subImgClass = "divSubQuestaoNivel4_none";
        $htmlSubQuestao = "";
        if (count($arDadosSubQuestao) > 0) {
            $htmlSubQuestao .= "<div id=\"div_subquestao_{$dados['qstid']}_{$dadosSubQuestao['sqtid']}\" class=\"{$subClass}\">	    									<div class=\"divSubQuestaoNivel1\">Tipo:</div>";
        }
        $obsHtml = '';
        foreach ($arDadosSubQuestao as $dadosSubQuestao) {

            $htmlSubQuestao .= "<div id=\"div_subquestao_item_{$dadosSubQuestao['sqtid']}\" class=\"{$subItemClass}\">
                                    <div class=\"divSubQuestaoNivel2\">
                                        <input type=\"checkbox\" $checked
                                                   value=\"{$dadosSubQuestao['sqtid']}\" 
                                                   name=\"sqtid[{$dados['qstid']}][]\" 
                                                   id=\"sqtid_{$dadosSubQuestao['sqtid']}\" > 
                                        <label for=\"sqtid_{$dadosSubQuestao['sqtid']}\">
                                        {$dadosSubQuestao['sqtdsc']}
                                        </label>
                                    </div>
                                </div>";
                                        
        }
            $obsHtml = "<div class='divSubQuestaoNivel3' >
                            <div class='divSubQuestaoNivel3_1'>Observação:</div>
                            " . campo_textarea('observacao','N', 'S', '', 110,4,'','', '', '', '', '','') . "
                        </div>";
        
        if (count($arDadosSubQuestao) > 0) 
            $htmlSubQuestao .= "</div>";


        $html .= "<div class='divQuestao'>
                    <div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
                    <div class='divQuestaoNivel2'>
                            <div>
                                <input name='qstid[{$dados['qstid']}]' id='qstid_t_{$dados['qstid']}' value='t' type='radio'" . ($dados['qtsresposta'] == 't' ? "checked='checked'" : "") . ">
                                <label for='qstid_t_{$dados['qstid']}'>Sim</label>
                                &nbsp;&nbsp;
                            </div>
                            <div>
                            <input name='qstid[{$dados['qstid']}]' id='qstid_f_{$dados['qstid']}' value='f' type='radio'" . ($dados['qtsresposta'] == 'f' ? "checked='checked'" : "") . ">
                                <label for='qstid_f_{$dados['qstid']}'>Não</label>
                                &nbsp;&nbsp;
                            " . $htmlSubQuestao . "
                            " . $obsHtml . "
                            </div>
                            <div>
                                <input name='qstid[{$dados['qstid']}]' id='qstid_n_{$dados['qstid']}' value='n' type='radio'" . ($dados['qtsresposta'] == 'n' ? "checked='checked'" : "") . ">
                                <label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
                                " . $obsHtml . "
                            </div>
                    </div>
                    ";

        $html .= "</div>";
        $qstidUlt = $dados['qstid'];
    }

    echo $html;
    $html = null;
}
?>
        </td>
    </tr>
    <tr bgcolor="#C0C0C0">
            <td colspan="3" align="center" class="notprint">
                    <input type="button" value="Imprimir" onclick="window.print();">
            </td>
    </tr>
</table>
<?php

//Chamada de programa
//include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";


//$empreendimento = new Empreendimento( $empid );
//$empreendimento->montaCabecalho();

?>
<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>jQuery.noConflict();</script>
<style>
    .div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
    .div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
    html>body .div_fotos li { height: 80px; line-height: 1.2em; }
    .field_fotos{padding:10px;width:90%}
    .div_fotos{height:499px;overflow:auto;}
    .div_fotos2{height:90px;overflow:auto;}
    .draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
    .nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
    .draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
    .f_selected{border: solid 1px lightgreen;background-color: lightgreen;}
    .fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
    .img_foto{z-index:2;}
    .img_class{margin-top:5px;}
    .img_class2{margin-top:-10px;}


    .divItemComposicao{
        font-weight:bold;
    }

    .divDivisao{
        margin-left:5px;
        font-weight:bold;
        padding:5px;
    }

    .divQuestao{
        margin-left:10px;
        padding:5px;
    }
    .divQuestaoNivel1{
    }
    .divQuestaoNivel2{
    }
    .divQuestaoNivel3{
        margin-top:3px;
    }
    .divQuestaoNivel4{
        margin-top:3px;
    }
    .divQuestaoNivel4_none{
        margin-top:3px;
    }
    .divQuestaoNivel4_fieldset{
        width:98%
    }
    .divQuestaoNivel4_1{
        margin-top:3px;
    }
    .divQuestaoNivel4_2{
        margin-top:3px;
    }
    .divQuestaoNivel4_3{
        margin-top:3px;
        padding:2px 2px 2px 2px;
        border: 1px dashed #ccc;
        height: 70px;
    }
    .divQuestaoNivel3{
        margin-top:3px;
    }
    .divQuestaoNivel3_1{
        font-weight:bold;
    }

    .divSubQuestao{
       
        padding-left: 20px;
        padding-top: 10px;
        padding-bottom: 5px;
    }

    .divSubItemQuestao{
        padding: 5px, 5px, 5px, 5px;
    }

    .divSubQuestaoNivel1{
        font-weight: bold;	
    }
    .divSubQuestaoNivel1_none{}

    .divSubQuestaoNivel2{
        font-weight: bold;
    }
    .divSubQuestaoNivel2_none{}

    .divSubQuestaoNivel3{
        padding-left: 30px;
    }

    .divSubQuestaoNivel4{
        padding-left: 30px;
        width: 90% 
    }
    #fieldgroup{
        width: 638px;
    }
</style>

