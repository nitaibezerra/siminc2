<?php
/**
 * Cabecalho da Unidade
 *
 * @category Componente
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24/09/2015
 * @link     no link
 */

$esfera = Par3_Controller_InstrumentoUnidade::pegarDescricaoEsfera($_REQUEST['inuid']);

$url = explode('=', $_SESSION['favurl']);
$rastro = explode('&', $url[1]);
$rastro = $rastro[0];

$urlPlanoTrabalho          = 'par3.php?modulo=principal/planoTrabalho/';
$urlBtnDadosUnidade        = $urlPlanoTrabalho.'dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'];

$testaPreenchimentoUnidade = Par3_Controller_InstrumentoUnidade::verificaPreencimentoUnidade($_REQUEST['inuid']);
$testaPreenchimentoUnidade = 100;

if ($testaPreenchimentoUnidade === 100) {
    $urlBtnPNE = $urlPlanoTrabalho.'pne&acao=A&inuid='.$_REQUEST['inuid'];
} else {
    $msgPNE = 'Falta preencher os Dados da Unidade.';
}

$testaPreenchimentoPNE = 100;

if ($testaPreenchimentoPNE === 100) {
    $urlBtnQuestoesEstrategicas = $urlPlanoTrabalho.'questoesEstrategicas&acao=A&inuid='.$_REQUEST['inuid'];
} else {
    $msgQuestoesEstrategicas = 'Falta terminar o PNE.';
}

$testaPreenchimentoQuestoesEstrategicas = 100;

if ($testaPreenchimentoQuestoesEstrategicas === 100) {
    $urlBtnAcompanhamento = $urlPlanoTrabalho.'acompanhamento&acao=A&inuid='.$_REQUEST['inuid'];
} else {
    $msgAcompanhamento = 'Falta finalizar as Questões Estratégicas.';
}

$testaPreenchimentoAcompanhamento = 100;

if ($testaPreenchimentoAcompanhamento === 100) {
    $urlBtnPendencias = $urlPlanoTrabalho.'pendencias&acao=A&inuid='.$_REQUEST['inuid'];
    $testaPendencias = 0;

    if ($testaPendencias === 0) {
        $urlBtnIndicadoresQualitativos = $urlPlanoTrabalho.'indicadoresQualitativos&acao=A&inuid='.$_REQUEST['inuid'];
    } else {
        $msgIndicadoresQualitativos = 'Ainda existem pendências.';
    }

} else {
    $msgPendencias = $msgIndicadoresQualitativos = 'Falta terminar o Nivelamento e Acompanhamento.';
}
$testaPendencias = 100-0;

$testaPreenchimentoIndicadoresQualitativos = 100;

$corSeta = '#e3e3e3';

$corFundoUnidade  = '#e3e3e3';

if ($testaPreenchimentoUnidade > 0) {
    $corFundoUnidade  = '#1ab394';
}

$corFundoPNE = '#e3e3e3';

if ($testaPreenchimentoPNE > 0) {
    $corFundoPNE  = '#1c83c6';
}

$corFundoQuestoesEstrategicas = '#e3e3e3';

if ($testaPreenchimentoQuestoesEstrategicas > 0) {
    $corFundoQuestoesEstrategicas  = '#f8ab59';
}

$corFundoAcompanhamento = '#e3e3e3';

if ($testaPreenchimentoAcompanhamento > 0) {
    $corFundoAcompanhamento  = '#22c6c8';
}

$corFundoPendencias = '#e3e3e3';

if ($testaPendencias > 0) {
    $corFundoPendencias  = '#000000';
}

$corFundoIndicadoresQualitativos  = '#e3e3e3';

if ($testaPreenchimentoIndicadoresQualitativos > 0) {
    $corFundoIndicadoresQualitativos  = '#a84c4d';
}

$setaDadosUnidade             = 'style="display:none;"';
$setaPne                      = 'style="display:none;"';
$setaQuestoesEstrategicas     = 'style="display:none;"';
$setaAcompanhamento           = 'style="display:none;"';
$setaPendencias               = 'style="display:none;"';
$setaIndicadoresQualitativos = 'style="display:none;"';

switch($rastro){
	case 'principal/planoTrabalho/pne':
	    $setaPne       = '';
	    $corPreenchido = '#1c83c6';
	    $ProxCor       = '#f8ab59';
        unset($urlBtnPNE);

	    if ($corFundoPNE !== '') {
    	    $corSeta = $corFundoPNE;
	    }

	    break;
	case 'principal/planoTrabalho/questoesEstrategicas':
	    $setaQuestoesEstrategicas = '';
	    $posicaoSeta          = '58.35%';
	    $corPreenchido        = '#f8ab59';
	    $ProxCor              = '#a84c4d';
        unset($urlBtnQuestoesEstrategicas);

	    if ($corFundoQuestoesEstrategicas !== '') {
    	    $corSeta = $corFundoQuestoesEstrategicas;
	    }

	    break;
	case 'principal/planoTrabalho/acompanhamento':
	    $setaAcompanhamento = '';
	    $corPreenchido      = '#22c6c8';
	    $ProxCor            = '#f8ab59';
        unset($urlBtnAcompanhamento);

	    if ($corFundoAcompanhamento !== '') {
    	    $corSeta = $corFundoAcompanhamento;
	    }

	    break;
	case 'principal/planoTrabalho/pendencias':
	    $setaPendencias = '';
	    $corPreenchido      = '#000000';
	    $ProxCor            = '#f8ab59';
        unset($urlBtnPendencias);

	    if ($corFundoPendencias !== '') {
    	    $corSeta = $corFundoPendencias;
	    }

	    break;
	case 'principal/planoTrabalho/indicadoresQualitativos':
	    $setaIndicadoresQualitativos = '';
	    $corPreenchido                = $ProxCor = '#a84c4d';
        unset($urlBtnIndicadoresQualidativos);

	    if ($corFundoIndicadoresQualidativos !== '') {
    	    $corSeta = $corFundoIndicadoresQualidativos;
	    }

	    break;
	case 'principal/planoTrabalho/dadosUnidade':
	default:
	    $setaDadosUnidade = '';
	    $corPreenchido    = '#1ab394';
	    $ProxCor          = '#1c83c6';

	    if ($corFundoUnidade !== '') {
    	    $corSeta = $corFundoUnidade;
	    }
        unset($urlBtnDadosUnidade);
	    break;
}

if ($testaPreenchimentoUnidade > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoUnidade === 100) {
        $corFim = '#1c83c6';
    } else {
        $testaPreenchimentoPNE = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoUnidade > 20) $corIni = $testaPreenchimentoUnidade -20;

    $corFundoBarraUnidade = '
            background: -moz-linear-gradient(left, #1ab394 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoUnidade.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#1ab394), color-stop('.$testaPreenchimentoUnidade.
                '%,'.$corFim.'), color-stop(100%,'.$corFim.')); /*Chrome,Safari4+*/';
}

if ($testaPreenchimentoPNE > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoPNE === 100) {
        $corFim = '#f8ab59';
    } else {
        $testaPreenchimentoAcompanhamento = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoPNE > 20) $corIni = $testaPreenchimentoPNE -20;

    $corFundoBarraPNE = '
            background: -moz-linear-gradient(left, #1c83c6 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoPNE.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#1c83c6), color-stop('.$testaPreenchimentoPNE.
                '%,'.$corFim.'), color-stop(100%,'.$corFim.')); /*Chrome,Safari4+*/';
}

if ($testaPreenchimentoQuestoesEstrategicas > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoQuestoesEstrategicas === 100) {
        $corFim = '#22c6c8';
    } else {
        $testaPreenchimentoIndicadoresQualitativos = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoQuestoesEstrategicas > 20) $corIni = $testaPreenchimentoQuestoesEstrategicas -20;

    $corFundoBarraQuestoesEstrategicas = '
            background: -moz-linear-gradient(left, #f8ab59 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoQuestoesEstrategicas.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#f8ab59), color-stop('.
                $testaPreenchimentoQuestoesEstrategicas.'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPreenchimentoAcompanhamento > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoAcompanhamento === 100) {
        $corFim = '#000000';
    } else {
        $testaPreenchimentoQuestoesEstrategicas = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoAcompanhamento > 20) $corIni = $testaPreenchimentoAcompanhamento -20;

    $corFundoBarraAcompanhamento = '
            background: -moz-linear-gradient(left, #22c6c8 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoAcompanhamento.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#22c6c8), color-stop('.
                $testaPreenchimentoAcompanhamento.'%,'.$corFim.'), color-stop(100%,'.
                $corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPendencias > 0) {
    $corFim = '#e3e3e3';

    if ($testaPendencias === 100) {
        $corFim = '#a84c4d';
    }

    $corIni = 0;

    if ($testaPendencias > 20) $corIni = $testaPendencias -20;

    $corFundoBarraPendencias = '
            background: -moz-linear-gradient(left, #000000 '.$corIni.'%, '.$corFim.' '.
                $testaPendencias.'%, '.$corFim.
                ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#000000), color-stop('.
                $testaPendencias.'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPreenchimentoIndicadoresQualitativos > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoIndicadoresQualitativos === 100) {
        $corFim = '#a84c4d';
    }

    $corIni = 0;

    if ($testaPreenchimentoIndicadoresQualitativos > 20) $corIni = $testaPreenchimentoIndicadoresQualitativos -20;

    $corFundoBarraIndicadoresQualitativos = '
            background: -moz-linear-gradient(left, #a84c4d '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoIndicadoresQualitativos.'%, '.$corFim.
                ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#a84c4d), color-stop('.
                $testaPreenchimentoIndicadoresQualitativos.'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}
?>
<style>
.corFundoUnidade{
    background-color: <?php echo $corFundoUnidade; ?>;
}

.barraUnidade{
    <?php echo $corFundoBarraUnidade; ?>
}

.corFundoPNE{
    background-color: <?php echo $corFundoPNE; ?>;
}

.barraPNE{
    <?php echo $corFundoBarraPNE; ?>
}

.corFundoQuestoesEstrategicas{
    background-color: <?php echo $corFundoQuestoesEstrategicas; ?>;
}

.barraQuestoesEstrategicas{
    <?php echo $corFundoBarraQuestoesEstrategicas; ?>
}

.corFundoAcompanhamento{
    background-color: <?php echo $corFundoAcompanhamento; ?>;
}

.barraAcompanhamento{
    <?php echo $corFundoBarraAcompanhamento; ?>
}

.corFundoPendencias{
    background-color: <?php echo $corFundoPendencias; ?>;
}

.barraPendencias{
    <?php echo $corFundoBarraPendencias; ?>
}

.corFundoIndicadoresQualitativos{
    background-color: <?php echo $corFundoIndicadoresQualitativos; ?>;
}

.barraIndicadoresQualitativos{
    <?php echo $corFundoBarraIndicadoresQualitativos; ?>
    width: 103%;
}

.btnRedondo{
    float:left;
    cursor:pointer;
    border-radius: 100% 100% 100% 100%;
    -moz-border-radius: 100% 100% 100% 100%;
    -webkit-border-radius: 100% 100% 100% 100%;
}

.btnInterno{
    width:44px;
    height:44px;
    margin-top:3px;
    margin-left:3px;
    display:inline-block;
    position:absolute;
    z-index: +1 !important;
}

.marcador{
    position:absolute;
    border-radius: 0% 0% 100% 100%;
    -moz-border-radius: 0% 0% 100% 100%;
    -webkit-border-radius: 0% 0% 100% 100%;
    border: 0px;
    width:10px;
    height:50px;
    margin-top:5px;
    margin-left:50%;
}

.btnExterno{
    width:50px;
    height:50px;
    display:block;
}

.btnInterno span {
    cursor:pointer;
    font-size:30px;
    color: white;
    margin-top:15%;
    margin-left:22.5%;
}

.textIcon{
    cursor:pointer;
    font-family: "Palatino Linotype";
    font-size:14px;
    color: white;
    margin-top:27%;
    margin-left:15%;
}

.characterIcon{
    cursor:pointer;
    font-family: "Palatino Linotype";
    font-size:30px;
    color: white;
    margin-left:20%;
}

.barraEvolucao{
    height:5px;
    background-color: #e3e3e3;
    margin-bottom:2px;
}

.dscBtn{
    font-size: 11px;
    font-family: "Arial";
    display: inline;
    margin-bottom: 3px;
    margin-left: 4%
}

.detalheDscBtn{
    font-size: 9px;
    font-family: "Arial";
    margin-left: 4%;
}

.custom-col-2{
    padding:0px;
}

.custom-col-10{
    padding: 5px 0px 0px 0px;
}

.linhaWizard{
    margin-bottom: 2px;
}

@media (max-width: 1600px) {
    .custom-col-10{
        padding-left:6px;
    }
}

@media (max-width: 1515px) {
    .i1450 {
        display: none;
    }
    .custom-col-10{
        padding-left:8px;
    }
    .marcador{
        margin-left:20px;;
    }
    .linhaWizard{
        margin-bottom: 2px;
    }
}

@media (max-width: 1400px) {
    .custom-col-10{
        padding-left:10px;
    }
}

@media (max-width: 1350px) {
    .custom-col-10{
        padding-left:13px;
    }
}

@media (max-width: 1300px) {
    .custom-col-10{
        padding-left:17px;
    }
}

@media (max-width: 1200px) {
    .i1450 {
        display: inline;
    }
    .custom-col-10{
        padding: 5px 0px 0px 3.8%;
    }
    .marcador{
        position:absolute;
        border-radius: 100% 0% 0% 100%;
        -moz-border-radius: 100% 0% 0% 100%;
        -webkit-border-radius: 100% 0% 0% 100%;
        border: 0px;
        width:50px;
        height:10px;
        margin-top:21px;
        margin-left:-1%;
    }
    .barraIndicadoresQualitativos{
        width: 100%;
    }
}
</style>
<div class="row linhaWizard">
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2">
                <div class="marcador corFundoUnidade" <?php echo $setaDadosUnidade;?> > </div>
                <div class="btnRedondo btnExterno corFundoUnidade">
                    <div class="btnRedondo btnInterno" style="background-color:#1ab394">
                        <span class="fa fa-file-text-o url" aria-hidden="true" url="<?php echo $urlBtnDadosUnidade;?>"></span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Dados da Unidade</label>
                <div class="barraEvolucao barraUnidade"></div>
                <label class="detalheDscBtn" style="color:#1ab394">Informações básicas <span class="i1450">sobre você</span></label>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2">
                <div class="marcador corFundoPNE" <?php echo $setaPne;?> > </div>
                <div class="btnRedondo btnExterno corFundoPNE">
                    <div class="btnRedondo btnInterno" style="background-color:#1c83c6">
                        <label class="textIcon url" url="<?php echo $urlBtnPNE;?>" msg="<?php echo $msgPNE; ?>">PNE</span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Plano Nacional <span class="i1450">de Educação</span></label>
                <div class="barraEvolucao barraPNE"></div>
                <label class="detalheDscBtn" style="color:#1c83c6">
                    Metas do Plano <span class="i1450"><?php echo $esfera ?> de Educação</span></label>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2">
                <div class="marcador corFundoQuestoesEstrategicas" <?php echo $setaQuestoesEstrategicas;?> > </div>
                <div class="btnRedondo btnExterno corFundoQuestoesEstrategicas">
                    <div class="btnRedondo btnInterno" style="background-color:#f8ab59" >
                        <label class="characterIcon url"
                               url="<?php echo $urlBtnQuestoesEstrategicas;?>"
                               msg="<?php echo $msgQuestoesEstrategicas; ?>">Q</span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Questões Estratégicas</label>
                <div class="barraEvolucao barraQuestoesEstrategicas"></div>
                <label class="detalheDscBtn" style="color:#f8ab59">
                    Questões pertinentes <span class="i1450">a sua situação</span></label>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2" >
                <div class="marcador corFundoAcompanhamento" <?php echo $setaAcompanhamento;?> > </div>
                <div class="btnRedondo btnExterno corFundoAcompanhamento" >
                    <div class="btnRedondo btnInterno" style="background-color:#22c6c8;" >
                        <span class="fa fa-bar-chart url" aria-hidden="true"
                              style="font-size:26px;margin: 10px 0px 0px 7px;"
                              url="<?php echo $urlBtnAcompanhamento;?>"
                              msg="<?php echo $msgAcompanhamento; ?>"></span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Acompanhamento</label>
                <div class="barraEvolucao barraAcompanhamento"></div>
                <label class="detalheDscBtn" style="color:#22c6c8">
                    Exccuções anteriores</label>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2">
                <div class="marcador corFundoPendencias" <?php echo $setaPendencias;?> > </div>
                <div class="btnRedondo btnExterno corFundoPendencias">
                    <div class="btnRedondo btnInterno" style="background-color:#000000" >
                        <span class="fa fa-exclamation-triangle url" aria-hidden="true"
                              style="font-size:34px;margin: 7% 0px 0px 12%;"
                              url="<?php echo $urlBtnPendencias;?>"
                              msg="<?php echo $msgPendencias; ?>"></span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Pendencias</label>
                <div class="barraEvolucao barraPendencias"></div>
                <label class="detalheDscBtn" style="color:#000000"></label>
            </div>
        </div>
    </div>
    <div class="col-lg-2">
        <div class="row">
            <div class="col-lg-2 custom-col-2">
                <div class="marcador corFundoIndicadoresQualitativos" <?php echo $setaIndicadoresQualitativos;?> > </div>
                <div class="btnRedondo btnExterno corFundoIndicadoresQualitativos">
                    <div class="btnRedondo btnInterno" style="background-color:#a84c4d" >
                        <span class="fa fa-user-md url" aria-hidden="true"
                              url="<?php echo $urlBtnIndicadoresQualitativos;?>"
                              msg="<?php echo $msgIndicadoresQualitativos; ?>"></span>
                    </div>
                </div>
            </div>
            <div class="col-lg-10 custom-col-10">
                <label class="dscBtn">Indicadores <span class="i1450">Qualitativos</span></label>
                <div class="barraEvolucao barraIndicadoresQualitativos"></div>
                <label class="detalheDscBtn" style="color:#a84c4d">
                    Analise da situação <span class="i1450">atual do seu município</span></label>
            </div>
        </div>
    </div>
</div>
<script>
$(document).ready(function()
{
    $('.url').click(function(){
        var url = $(this).attr('url');
        var msg = $(this).attr('msg');
		console.log(url);
        if (url != '') {
            $(location).attr('href',url);
        }
    });
});
</script>