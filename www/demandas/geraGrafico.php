<?php
// carrega as funções gerais
include_once "config.inc";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include ("../../includes/open_flash_chart/open-flash-chart.php");
include ("../../includes/open_flash_chart/ofc_sugar.php");

include ( APPRAIZ. "www/demandas/_constantes.php");

if($_REQUEST['tipo']){
	switch($_REQUEST['tipo']){
		case "barra" :
			
			$bar_stack = new bar_stack();
			
			$bar_stack->set_colours( $arr_cores_painel );
			
			$bar_stack->append_stack( array( 10, 8, 3 ));
			
			$bar_stack->set_keys(
			    array(
			        	new bar_stack_key( $arr_cores_painel[2], 'Prioridade Alta', 12 ),
			        	new bar_stack_key( $arr_cores_painel[1], 'Prioridade Media', 12 ),
			        	new bar_stack_key( $arr_cores_painel[0], 'Prioridade Baixa', 12 )
			         )
			    );
			$bar_stack->set_tooltip( '#val# de #total#' );
			
			$y = new y_axis();
			$y->set_range( 0, 25, 2 );
			
			$x = new x_axis();
			$x->set_labels_from_array( array( 'Celula 1' ) );
			
			$tooltip = new tooltip();
			$tooltip->set_hover();
			
			$chart = new open_flash_chart();
			$chart->add_element( $bar_stack );
			$chart->set_x_axis( $x );
			$chart->add_y_axis( $y );
			$chart->set_tooltip( $tooltip );
			$chart->set_bg_colour( '#FFFFFF' );
			
			echo $chart->toPrettyString();
			
			/*
			// generate some random data
			srand((double)microtime()*5000000);
			
			$max = 10;
			$tmp = array();
			for( $i=0; $i<6; $i++ )
			{
			  $tmp[] = rand(0,$max);
			}
			
			$title = new title( "Barra" );
			$title->set_style( "{font-size: 16px; font-weight: bold; text-align: center}" );
			
			$bar = new bar();
			$bar->set_values( array(1,2,3,4,5) );
			
			$chart = new open_flash_chart();
			$chart->add_element( $bar );
			
			$chart->set_bg_colour( '#ffffff' );
			                    
			echo $chart->toString();
			*/
		break;
		
		case "pizza" :
			$title = new title( 'Pizza' );
			$title->set_style( "{font-size: 16px; font-weight: bold; text-align: center}" );
			
			$pie = new pie();
			$pie->set_alpha(1.0);
			$pie->set_start_angle( 35 );
			$pie->add_animation( new pie_fade() );
			$pie->set_tooltip( '#val# de #total#<br>#percent# de 100%' );
			$pie->set_colours( array('#6495ED','#66CDAA') );
			$pie->set_values( array(8,7,2,3,4,new pie_value(6.5, "legenda (6.5)")) );
			
			$chart = new open_flash_chart();
			//$chart->set_title( $title );
			$chart->add_element( $pie );
			
			$chart->set_bg_colour( '#ffffff' );
			
			$chart->x_axis = null;
			
			echo $chart->toPrettyString();
			
		break;
		
		case "radar":
			
			$chart = new open_flash_chart();
			$title = new title( 'Radar' );
			$title->set_style( "{font-size: 16px; font-weight: bold; text-align: center}" );
			
			$area = new area();
			// set the circle line width:
			$area->set_width( 1 );
			$area->set_default_dot_style( new s_hollow_dot('#45909F', 5) );
			$area->set_colour( '#45909F' );
			$area->set_fill_colour( '#45909F' );
			$area->set_fill_alpha( 1.0 );
			$area->set_loop();
			$area->set_values(array(3, 4, 5, 4, 3, 3, 2.5));
			
			// add the area object to the chart:
			$chart->add_element( $area );
			
			$r = new radar_axis( 5 );
			
			$r->set_colour( '#EFD1EF' );
			$r->set_grid_colour( '#EFD1EF' );
			
			$labels = new radar_axis_labels( array('0','1','2','3','4','5') );
			$labels->set_colour( '#9F819F' );
			$r->set_labels( $labels );
			
			$chart->set_radar_axis( $r );
			
			$tooltip = new tooltip();
			$tooltip->set_proximity();
			$chart->set_tooltip( $tooltip );
			//$chart->set_title( $title );
			
			$chart->set_bg_colour( '#ffffff' );
			
			echo $chart->toPrettyString();
			
		break;
		
		case "linha":

			$data_1 = array();
			$data_2 = array();
			$data_3 = array();
			
			for( $i=0; $i<4.2; $i+=0.2 )
			{
			  $data_1[] = (sin($i) * 1.9) + 10;
			  $data_2[] = (sin($i) * 1.9) + 7;
			  $data_3[] = (sin($i) * 1.9) + 4;
			}
			
			$title = new title( "Linha" );
			
			$line_1_default_dot = new dot();
			$line_1_default_dot->colour('#f00000');
			
			$line_1 = new line();
			$line_1->set_default_dot_style($line_1_default_dot);
			$line_1->set_values( $data_1 );
			$line_1->set_width( 2 );
			
			// ------- LINE 2 -----
			$line_2_default_dot = new dot();
			$line_2_default_dot->size(1)->halo_size(1)->colour('#3D5C56');
			
			$line_2 = new line();
			$line_2->set_default_dot_style($line_2_default_dot);
			$line_2->set_values( $data_2 );
			$line_2->set_width( 2 );
			$line_2->set_colour( '#3D5C56' );
			
			// ------- LINE 2 -----
			$line_3_default_dot = new dot();
			$line_3_default_dot->size(1)->halo_size(1);
			
			$line_3 = new line();
			$line_3->set_default_dot_style($line_3_default_dot);
			$line_3->set_values( $data_3 );
			$line_3->set_width( 2 );
			
			$y = new y_axis();
			$y->set_range( 0, 15, 5 );
			
			
			$chart = new open_flash_chart();
			//$chart->set_title( $title );
			$chart->add_element( $line_1 );
			$chart->add_element( $line_2 );
			$chart->add_element( $line_3 );
			$chart->set_y_axis( $y );
			$chart->set_bg_colour( '#ffffff' );
			
			echo $chart->toPrettyString();
			
		break;
		
	}
}

function removeacentosGrafico ($var)
{
       $ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
       $SEMACENTOS= array("A","A","A","A","a","a","a","a");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
       $SEMACENTOS= array("E","E","E","E","e","e","e","e");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
       $SEMACENTOS= array("I","I","I","I","i","i","i","i");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
       $SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
     
       $ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
       $SEMACENTOS= array("U","U","U","U","u","u","u","u");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ç","ç","ª","º","°");
       $SEMACENTOS= array("C","c","a.","o.","o.");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);      

       return $var;
}
?>