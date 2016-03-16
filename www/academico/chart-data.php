<?php
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// use the chart class to build the chart:
include_once "../../includes/open_flash_chart/open-flash-chart.php";

/*
$title = new title( date("D M d Y") );

$bar = new bar_filled( '#E2D66A', '#577261' );
$bar->set_values( array(9,8,7,6,5,4,3,2,1) );

$chart = new open_flash_chart();
$chart->set_title( $title );
$chart->add_element( $bar );
$chart->set_bg_colour( '#FFFFFF' );

echo $chart->toPrettyString();
*/
 
$_arrValores = array('Brasil'   => '9.8',
					 'Acre'     => '26.9',
					 'Amapa'    => '13.4',
					 'Amazonas' => '13.8',
					 'Para' => '7.5',
					 'Rondonia' => '14.6',
					 'Roraima' => '23.5',
					 'Tocantins' => '19.3',
					 'Alagoas' => '13.1',
					 'Bahia' => '6.8',
					 'Ceara' => '5.7',
					 'Maranhao' => '6.3',
					 'Paraнba' => '27.4',
					 'Pernambuco' => '11.0',
					 'Piaui' => '18.6',
					 'Rio Grande do Norte' => '21.8',
					 'Sergipe' => '21.8',
					 'Distrito Federal' => '22.8',
					 'Goias' => '9.6',
					 'Mato Grosso' => '14.4',
					 'Mato Grosso do Sul' => '19.7',
					 'Espirito Santo' => '13.2',
					 'Minas Gerais' => '13.8',
					 'Rio de Janeiro' => '10.7',
					 'Sao Paulo' => '1.4',
					 'Parana' => '10.1',
					 'Rio Grande do Sul' => '15.5',
					 'Santa Catarina' => '8.4');

arsort($_arrValores);

$i=0;
$cor='#000088';
foreach($_arrValores as $estado => $valor) {
	$_ESTADOS[$i] = $estado;
	if($estado == 'Brasil') {
		$_VALORES[$i] = new bar_value((float) $valor);
		$_VALORES[$i]->set_colour( '#008800' );
		$cor='#880000';
	} else {
		if(strtolower($estado) == $_REQUEST['uf']) {
			$_VALORES[$i] = new bar_value((float) $valor);
			$_VALORES[$i]->set_colour( '#FFAA00' );
		} else {
			$_VALORES[$i] = new bar_value((float) $valor);
			$_VALORES[$i]->set_colour( $cor );
		}
	}
	$i++;
}

$x_labels = new x_axis_labels();
//adiciona a cor dos elementos do eixo x
$x_labels->set_colour( '#990000' );
//adiciona os elementos do eixo x
$x_labels->set_labels( $_ESTADOS );
//rotaciona o eixo x
$x_labels->rotate(-45);
//Eixo x
$x = new x_axis();
$x->set_3d( 5 );
$x->colour = '#d0d0d0';
//Adiciona a definiзгo do eixo x
$x->set_labels($x_labels) ;
//	Instвncia do Eixo Y
$eixo_y = new y_axis();
//Atribui os valores ao Eixo y
$eixo_y->set_range( 0, 30 , 2);

$bar = new bar();
$bar->set_values( $_VALORES );

$chart = new open_flash_chart();
$chart->add_element( $bar );	

//$chart->add_element( $bar );
$chart->set_x_axis( $x );
$chart->add_y_axis( $eixo_y );
$chart->set_bg_colour( '#ffffff' );
                      
ob_clean();
//echo $chart->toPrettyString();
//echo '{ "elements": [ { "type": "bar_filled", "colour": "#E2D66A", "outline-colour": "#577261", "values": [ 9, 8, 7, 6, 5, 4, 3, 2, 1 ] } ], "title": { "text": "Tue Aug 02 2011" }, "bg_colour": "#FFFFFF" }';
echo '{ "elements": [ { "type": "bar", "values": [ { "top": 27.4, "colour": "#000088" }, { "top": 26.9, "colour": "#000088" }, { "top": 23.5, "colour": "#000088" }, { "top": 22.8, "colour": "#000088" }, { "top": 21.8, "colour": "#000088" }, { "top": 21.8, "colour": "#000088" }, { "top": 19.7, "colour": "#000088" }, { "top": 19.3, "colour": "#000088" }, { "top": 18.6, "colour": "#000088" }, { "top": 15.5, "colour": "#000088" }, { "top": 14.6, "colour": "#000088" }, { "top": 14.4, "colour": "#000088" }, { "top": 13.8, "colour": "#000088" }, { "top": 13.8, "colour": "#000088" }, { "top": 13.4, "colour": "#000088" }, { "top": 13.2, "colour": "#000088" }, { "top": 13.1, "colour": "#000088" }, { "top": 11, "colour": "#000088" }, { "top": 10.7, "colour": "#000088" }, { "top": 10.1, "colour": "#000088" }, { "top": 9.8, "colour": "#008800" }, { "top": 9.6, "colour": "#880000" }, { "top": 8.4, "colour": "#880000" }, { "top": 7.5, "colour": "#880000" }, { "top": 6.8, "colour": "#880000" }, { "top": 6.3, "colour": "#880000" }, { "top": 5.7, "colour": "#880000" }, { "top": 1.4, "colour": "#880000" } ] } ], "x_axis": { "3d": 5, "colour": "#d0d0d0", "labels": { "colour": "#990000", "labels": [ "Para", "Acre", "Roraima", "Distrito Federal", "Sergipe", "Rio Grande do Norte", "Mato Grosso do Sul", "Tocantins", "Piaui", "Rio Grande do Sul", "Rondonia", "Mato Grosso", "Minas Gerais", "Amazonas", "Amapa", "Espirito Santo", "Alagoas", "Pernambuco", "Rio de Janeiro", "Parana", "Brasil", "Goias", "Santa Catarina", "Para", "Bahia", "Maranhao", "Ceara", "Sao Paulo" ], "rotate": -45 } }, "y_axis": { "min": 0, "max": 30, "steps": 2 }, "bg_colour": "#ffffff" }';
?>