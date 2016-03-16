<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$pne             = new Par3_Controller_Pne();

$inuid    = $_REQUEST['inuid'];
$itrid 	  = $controleUnidade->pegarItrid($inuid);
$menu     = $_REQUEST['menu'];
$menu     = ($menu === 'possuiDiagnostico') ? '' : $menu;
$arrMetas = $pne->retornaArrayMetas();

$unidade  = new Par3_Model_InstrumentoUnidade($inuid);

$_SESSION['par']['itrid'] = $itrid;
$_SESSION['par']['estdescricao'] = $controleUnidade->pegarNomeEntidade($inuid);
$_SESSION['par']['mundescricao'] = $controleUnidade->pegarNomeEntidade($inuid);
$_SESSION['par']['estuf'] = $unidade->estuf;
$_SESSION['par']['muncod'] = $unidade->muncod;

$disabled = 'disabled';

if ($db->testa_superuser()) {
    $disabled = '';
}

$iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
                           'style="color:green !important;'.
                                  'background-color: transparent;"></span>';

require 'pne/funcoes.php';

require APPRAIZ.'includes/cabecalho.inc';
?>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
<script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>

<script language="javascript" src="/estrutura/js/funcoes.js"></script>
<script language="javascript" src="/par/js/javascript.js"></script>
<script src="../includes/funcoes.js"></script>
<style>
.div_lbl_grfAno{
    float: left;
    margin-top: 11px;
    <?php if ($_SESSION['par']['itrid'] == 1) { ?> margin-left: 14px; <?php } else { ?> margin-left: 37px; <?php } ?>
}

.div_combo_grfAno{
    float: left;
    margin-top: 5px;
    <?php if ($_SESSION['par']['itrid'] == 1) { ?> margin-left: 43px; <?php } else { ?> margin-left: 20px; <?php } ?>
	width: 328px;
}

.ui-slider-range-min {
	background-color: #f7b850;
}

.tabela_box_azul_escuro tr td {
	margin: 20px;
}

.div_grfMun{
    float: left;
	margin-right: 15px;
}
        
.tbNormal {
    width: 50%;
    margin-left: -30%;
}

.ibox-content-round-gray{
    border: 4px solid #f3f3f4;
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
}
</style>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox">
            <div class="ibox-content">
                <input type="hidden" name="menu" id="menu" value="<?php echo $menu ?>"/>
                <?php echo $simec->title($controleUnidade->pegarNomeEntidade($inuid)); ?>
                <br>
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <div class="ibox-content-round-gray ibox-content">
                    <?php require_once("grafico.inc"); ?>
                </div>
            </div>
        </div>
    </div>
</div>
