<?php

$arCampos = array();

$boInstrumento 	= 'disabled';
$boDimensao 	= 'disabled';
$boArea 		= 'disabled';
$boIndicador 	= 'disabled';
$boCriterio 	= 'disabled';
$boAcao		 	= 'disabled';

switch($_POST['tipoGuia']){

	case 'dimensao':
		Par3_Controller_ConfiguracaoControle::recuperaDadosFormGuiaDimensao($_POST['codigo'], $boDimensao, $stTitulo, $itrdsc, $itrid, $dimid, $dimdsc, $ordcod, $arCampos);
		break;
	case 'area':
		Par3_Controller_ConfiguracaoControle::recuperaDadosFormGuiaArea($_POST['codigo'], $boArea, $stTitulo, $itrdsc, $dimdsc, $dimid, $areid, $aredsc, $ordcod, $arCampos);
		break;
	case 'indicador':
		Par3_Controller_ConfiguracaoControle::recuperaDadosFormGuiaIndicador($_POST['codigo'], $boIndicador, $stTitulo, $itrdsc, $dimdsc, $aredsc, $areid, $indid, $inddsc, $indajuda, $metas, $ordcod, $arCampos);
		break;
	case 'criterio':
		Par3_Controller_ConfiguracaoControle::recuperaDadosFormGuiaCriterio($_POST['codigo'], $boCriterio, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $indajuda, $metas, $indid, $crtid, $crtdsc, $ordcod, $arCampos);
		break;
	case 'acao':
		Par3_Controller_ConfiguracaoControle::recuperaDadosFormGuiaAcao($_POST['codigo'], $boAcao, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $crtdsc, $crtid, $ppaid, $ppadsc, $arCampos);
		break;

}

?>

<div class="ibox-content">
	<form method="post" name="formulario" id="formulario" class="form form-horizontal">
		<input type='hidden' value='<?php echo $ordcod;?>' id='ordcod' name='ordcod'/>
		<input type='hidden' value='<?php echo $itrid;?>' id='itrid' name='itrid'/>
		<input type='hidden' value='<?php echo $dimid;?>' id='dimid' name='dimid'/>
		<input type='hidden' value='<?php echo $areid;?>' id='areid' name='areid'/>
		<input type='hidden' value='<?php echo $indid;?>' id='indid' name='indid'/>
		<input type='hidden' value='<?php echo $crtid;?>' id='crtid' name='crtid'/>
		<input type='hidden' value='<?php echo $ppaid;?>' id='ppaid' name='ppaid'/>
		<input type='hidden' value='<?php echo $_POST['acaoGuia'];?>' id='acao' name='acao'/>
		<input type='hidden' value='<?php echo $_POST['tipoGuia'];?>' id='tipo' name='tipo'/>

		<?PHP

            $titulo = $_POST['acaoGuia'] == 'incluir' ? 'Inclusão ' .$stTitulo : 'Alteração ' .$stTitulo;

			echo $simec->title($titulo);

			echo $simec->input('itrdsc', 'Instrumento', $itrdsc, array('placeHolder' => 'Instrumento', 'maxlengh' => 1000, $boInstrumento));

			echo $simec->input('dimdsc', 'Dimensão', $dimdsc, array('placeHolder' => 'Dimensão', 'maxlengh' => 500, $boDimensao));

			if(in_array('area', $arCampos)){
				echo $simec->input('aredsc', 'Área', $aredsc, array('placeHolder' => 'Área', 'maxlengh' => 500, $boArea));
			}

			if(in_array('indicador', $arCampos)){
				echo $simec->input('inddsc', 'Indicador', $inddsc, array('placeHolder' => 'Indicador', 'maxlengh' => 1200, $boIndicador));

				echo $simec->textarea('indajuda', 'Descrição do indicador', $indajuda, array('placeHolder' => 'Descrição do indicador', 'maxlengh' => 1200, $boIndicador));

				$dadosPNE = array(
				    "1" => "Meta 01",
				    "2" => "Meta 02",
				    "3" => "Meta 03",
				    "4" => "Meta 04",
				    "5" => "Meta 05",
				    "6" => "Meta 06",
				    "7" => "Meta 07",
				    "8" => "Meta 08",
				    "9" => "Meta 09",
				    "10" => "Meta 10",
				    "11" => "Meta 11",
				    "12" => "Meta 12",
				    "13" => "Meta 13",
				    "14" => "Meta 14",
				    "15" => "Meta 15",
				    "16" => "Meta 16",
				    "17" => "Meta 17",
				    "18" => "Meta 18",
				    "19" => "Meta 19",
				    "20" => "Meta 20"
				);
				if( is_array($metas) ){
				    $metas = array_column($metas, 'inmmeta');
				}
				echo $simec->select('metas[]', 'Metas do PNE', $metas, $dadosPNE, array('placeHolder' => 'Metas do PNE', 'maxlengh' => 1200, $boIndicador));
			}

			if(in_array('criterio', $arCampos)){
				if( is_array($metas) ){
				    $metas = array_column($metas, 'inmmeta');
				}
				echo $simec->input('crtdsc', 'Componente', $crtdsc, array('placeHolder' => 'Componente', 'maxlengh' => 1200, $boCriterio));
			}

			echo $simec->input('ordcod', 'Ordem', $ordcod, array('placeHolder' => 'Ordem', 'maxlengh' => 5));
		?>

		<div class="form-group">
			<div class="col-lg-12 text-center">
			<?php if($_POST['acaoGuia'] == 'incluir'){ ?>
				<button type="submit" class="btn btn-sm btn-success btn-lg">
					<span class="glyphicon glyphicon-search"> </span> Salvar
				</button>
			<?php } else { ?>
				<button type="submit" class="btn btn-sm btn-success btn-lg">
					<span class="glyphicon glyphicon-search"> </span> Alterar
				</button>
			<?php } ?>
			</div>
		</div>
	</form>
	</div>

<?php die(); ?>