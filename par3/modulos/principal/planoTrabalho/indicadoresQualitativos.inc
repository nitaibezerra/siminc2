<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$oIndicador      = new Par3_Model_Indicador();
$oPontuacao      = new Par3_Model_Pontuacao();

$itrid  = $controleUnidade->pegarItrid($_REQUEST['inuid']);

switch ($_POST['req']){
    case 'salvar':
        $oPontuacao = new Par3_Controller_Pontuacao;
        $indid = $oPontuacao->preparaSalvar();
        simec_redirecionar('par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid='.$_POST['inuid'].'&dimid='.$_POST['dimid'].'&areid='.$_POST['areid'].'&indid='.$indid, 'success');
        break;
}

$iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
    'style="color:green !important;'.
    'background-color: transparent;"></span>';

require APPRAIZ.'includes/cabecalho.inc';

?>
<style>
.ibox-content-round-gray{
    width:99%;
    background-color: #f3f3f4;
    float: center;
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
    border: 0px solid #000000;
}

.ibox-content-round-gray-border{
    width:99%;
    background-color: #f3f3f4;
    float: center;
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
    border: 1px solid #cacaca;
}

.title-top-round{
    border-radius: 10px 10px 0px 0px;
    -moz-border-radius: 10px 10px 0px 0px;
    -webkit-border-radius: 10px 10px 0px 0px;
}

.content-bottom-round{
    border-radius: 0px 0px 10px 10px;
    -moz-border-radius: 0px 0px 10px 10px;
    -webkit-border-radius: 0px 0px 10px 10px;
}

.content-round{
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
}

.esconde-area{
    display: none;
}

.content-gray-border{
    font-size: 10px;
    border: 1px solid #cacaca;
}

.aba-dimensao{
    font-size: 10px;
}

.title-gray{
    background-color: #f3f3f4;
}

.menuIndicadores{
    border-bottom: 1px solid #e3e3e3;
    padding: 5px;
    cursor:pointer;
}

.menuIndicadores:hover{
    font-weight: bold;
}

.menuSelecionado{
    background-color: #F5F5DC;
    cursor: default !important;
}

.btn-circled {
    background-color: #347AB8;
    font-size: 30px;
    font-weight: bold;
    width: 100px;
    height: 100px;
    border-radius: 50%;
    -webkit-border-radius: 50%;
    -moz-border-radius: 50%;
}

.pontuacao {
    font-size: 20px;
    font-weight: bold;
    color: #347AB8;
}

@media (max-width: 1450px) {
    .ibox-content-round-gray{
        margin-top: 0.5%
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
}
</style>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <input type="hidden" name="req" id="req" value=""/>
                <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid']?>"/>
                <input type="hidden" name="dimid" id="dimid" value="<?php echo $_REQUEST['dimid']?>"/>
                <input type="hidden" name="areid" id="areid" value="<?php echo $_REQUEST['areid']?>"/>
                <input type="hidden" name="indid" id="indid" value="<?php echo $_REQUEST['indid']?>"/>
                <?php echo $simec->title($controleUnidade->pegarNomeEntidade($_REQUEST['inuid'])); ?>
                <br>
            	<?php $controleUnidade->cabecalhoUnidade(); ?>

                <div class="ibox-content-round-gray ibox-content">
    				<div class="tabs-container">
                    	<ul class="nav nav-tabs">
                    		<?php $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid);
    						if($arDimensoes){
    						    // Imprime as abas da dimensão
                                foreach ($arDimensoes as $dimensao){
                                    $d++;
                                    $ativa = ' ';
                                    if( $_REQUEST['dimid'] ){
                                        $ativa = $_REQUEST['dimid'] == $dimensao['dimid'] ? 'active' : ' ';
                                    } else {
                                        $ativa = $d == 1 ? 'active' : ' ';
                                    }
                                    ?>
                    				<li class=" aba-dimensao <?php echo $ativa; ?>"><a data-toggle="tab" href="#tab-<?php echo $d; ?>" aria-expanded="true"><?php echo $dimensao['dimcod'].' - '.$dimensao['dimdsc'];?><span style="cursor: pointer"> <i class="fa fa-area-chart" data-toggle="modal" data-target="#modal"></i></span></a></li>
							<?php } } ?>
                    	</ul>
                    	<br />
                    	<div class="tab-content">
                    		<?php
                    		$d = 0;
    						if($arDimensoes){
    						    // Imprime as Tabelas de cada aba.
                                foreach ($arDimensoes as $dimensao){
                                    $d++;
                                    $ativa = ' ';
                                    if( $_REQUEST['dimid'] ){
                                        $ativa = $_REQUEST['dimid'] == $dimensao['dimid'] ? 'active' : ' ';
                                    } else {
                                        $ativa = $d == 1 ? 'active' : ' ';
                                    }
                                    ?>
                            		<div id="tab-<?php echo $d; ?>" class="tab-pane <?php echo $ativa; ?>">
										<?php
										$a = 0;
										$arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']);
                                        if($arAreas){
                                            // Imprime as áreas
                                            foreach ($arAreas as $area){
                                                $a++;

                                                if( $_REQUEST['areid'] ){
                                                    if($_REQUEST['areid'] == $area['areid']){
                                                        // Aberto
                                                        $ativaTopoArea = 'content-top-round';
                                                        $ativaArea = '';
                                                    } else {
                                                        // Fechado
                                                        $ativaTopoArea = 'content-round';
                                                        $ativaArea = 'esconde-area';
                                                    }
                                                } else {
                                                    if( $a == 1 ){
                                                        // Aberto
                                                        $ativaTopoArea = 'content-top-round';
                                                        $ativaArea = '';
                                                    } else {
                                                        // Fechado
                                                        $ativaTopoArea = 'content-round';
                                                        $ativaArea = 'esconde-area';
                                                    }
                                                }
                                                ?>
                                            	<div class="ibox-title title-gray  content-gray-border title-top-round indicador <?php echo $ativaTopoArea; ?>" tipo="<?php echo $area['areid']; ?>">
                                                	<?php echo $dimensao['dimcod'].".".$area['arecod'].' - '.$area['aredsc'];?>
                                                </div>
                                                <div class="ibox-content content-bottom-round content-gray-border <?php echo $ativaArea; ?>" id=<?php echo $area['areid']; ?>>
                                                    <div class="row">
                                                        <div class="col-md-3 colunaMenu">
                                                            <div class="ibox-title title-gray">
                                                                <b>INDICADORES</b>
                                                            </div>
                                                            <?php
                                                            $i = 0;
                                                            $indid = 0;
                                                            $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']);
                                                            if($arIndicadores){
                                                                foreach ($arIndicadores as $indicador){
                                                                    $i++;
                                                                    $primeiroSelecao = '';
                                                                    if($i == 1){
                                                                        $indid = $indicador['indid'];
                                                                        $primeiroSelecao = $_REQUEST['indid'] ? ' ' : 'menuSelecionado';
                                                                    }
                                                                    ?>
                                                                    <div class=" title-gray menuIndicadores <?php echo $primeiroSelecao; ?> <?php echo ( $_REQUEST['indid'] === $indicador['indid'] ? 'menuSelecionado' : '') ?>" indid="<?php echo $indicador['indid']; ?>" areid="<?php echo $area['areid']; ?>" dimid="<?php echo $dimensao['dimid']; ?>">
                                                                        <?php echo $oPontuacao->buscaPontuacao($_REQUEST['inuid'], $indicador['indid']) ? $iconPreenchido : $iconNaoPreenchido; ?>
                                                                        <?php echo $indicador[$indicador['indid']] = $dimensao['dimcod'].".".$area['arecod'].".".$indicador['indcod']." - ".$indicador['inddsc']; ?>
                                                                    </div>
                                                            <?php } } else { echo "Não existem indicadores cadastrados"; } ?>
                                                        </div>
                                                        <?php

                                                        if( $_REQUEST['dimid'] == $dimensao['dimid'] && $_REQUEST['areid'] == $area['areid'] ){
                                                            $indid = $_REQUEST['indid'];
                                                        }
                                                        $dadosIndicador = $oIndicador->recuperarDados($indid);
                                                        ?>
                                                        <div class="col-md-6 colunaForm">
                                                            <div class="ibox">
                                                            	<?php if($dadosIndicador['indid']){ ?>
                                                                	<div class="ibox-title">
                                                                	    <h3>Descrição do Indicador</h3>
                                                                	    <?php echo $dadosIndicador['inddsc']; ?>
                                                                	</div>
                                                                	<div class="ibox-content">
                                                                		<?php
                            												$arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($dadosIndicador['indid']);
                            												if($arCriterios && $i > 0){
                            												    foreach ($arCriterios as $criterio){
//                                                                                     $vericicacao = Par3_Controller_Pontuacao::recuperarCriteriosGuia($dadosIndicador['indid']);
                                                                        ?>
                                                                            		<input type="checkbox" name="indicador[<?php echo $dimensao['dimid']; ?>][<?php echo $area['areid']; ?>][<?php echo $dadosIndicador['indid']; ?>][]" value="<?php echo $criterio['crtid']; ?>">&nbsp;<?php echo $criterio['crtdsc']; ?>
                                                                    				<BR />
                                                                	    <?php } } else { echo "Sem critérios cadastrados para o indicador."; } ?>
                                                        				<BR />
                                                        				<BR />
                                                        				<BR />
                                                        				JUSTIFICATIVA:
                                                                		<?php echo $simec->textarea('indjustificativa['.$dadosIndicador['indid'].']', null, $indjustificativa, array())?>
                                                        				<BR />
                                                        				<BR />
                                                        				METAS:
                                                                		<?php echo $simec->textarea('metas', null, $indjustificativa, array())?>
                                                                	</div>
                                                                	<div class="ibox-footer">
                                                                		<div class="col-sm-offset-3 col-md-offset-3 col-lg-offset-3">
                                                                    		<button type="button" class="btn btn-success salvar" areid="<?php echo $area['areid']; ?>" dimid="<?php echo $dimensao['dimid']; ?>" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar</button>
                                                                		</div>
                                                                	</div>
                                                            	<?php } else { echo "Indicador não cadastrado."; } ?>
                                                            </div>
                                                        </div>
                                                        <div class="col-md-3 colunaMenu">
                                                            <div class="ibox-content">
                                                            	<center>
                                                            		<div class='pontuacao'>PONTUAÇÃO</div>
                                                            		<button class="btn btn-primary btn-circled btn-lg" type="button">3.5</button><br />
                                                                	Dados da pontuação dados da pontuação dados da pontuação dados da pontuação dados da pontuação dados da pontuação dados da pontuação dados da pontuação.
                                                            	</center>
                                                            </div>
                                                        </div>
                                                	</div>
                                                </div>
                                				<BR />
                                    	<?php } } ?>
                    				</div>
							<?php } } ?>
            			</div>
            		</div>
            	</div>
            </div>
        </div>
    </div>
</div>
</form>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 95%;">
        <div class="modal-content animated flipInY">
            <div class="ibox-title esconde " tipo="integrantes">
                <h3>Informações de indicador</h3>
            </div>
            <div class="ibox-content">
                <?php include 'graficoIndicadores.inc';?>
            </div>
        </div>
    </div>
</div>
<script>
$(document).ready(function()
{
	$('.indicador').click(function()
	{
		var tipo = $(this).attr('tipo');

		if ( $('#'+tipo).css('display') == 'none' ) {
			$('#'+tipo).show();
			$(this).removeClass('content-round');
			$(this).addClass('content-top-round');
		} else {
			$('#'+tipo).hide();
			$(this).addClass('content-round');
			$(this).removeClass('content-top-round');
		}
	});

	$('.salvar').click(function(){
		$('#dimid').val($(this).attr('dimid'));
		$('#areid').val($(this).attr('areid'));
		$('#req').val('salvar');
		$('#formulario').submit();

    });

	$('.menuIndicadores').click(function(){

        var dimid       = $(this).attr('dimid');
        var areid       = $(this).attr('areid');
        var indid       = $(this).attr('indid');
        var indidAntigo = $('#indid').val();
        var url         = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A'
                        +'&inuid=<?php echo $_REQUEST['inuid']?>&dimid='+dimid+'&areid='+areid+'&indid='+indid;

        if (indid != indidAntigo) {
             $(location).attr('href',url);
        }

    });
});
</script>