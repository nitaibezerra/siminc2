<?
include ("../../includes/open_flash_chart/open-flash-chart.php");
include ("../../includes/open_flash_chart/ofc_sugar.php");

include "resultComponentesGraficoBack.php";

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
		<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/json/json2.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<style>
			.tabela {
				font-weight: bold;
			}
		</style>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

<?php 
	$sqlGrafico = obterSqlGrafico($_POST);
	$registrosParaGrafico = $db->carregar( $sqlGrafico );
	
	foreach( $registrosParaGrafico as $values  ) {
		if($values['representacaomaior']){
			$arrDados[ $values['representacaomaior'] ][] = array(  
																"campoporcaosomatorio" => $values['campoporcaosomatorio'] , 
																"quantidade" => $values['quantidade'] );
		}
	}
	
	$n = 1;
	foreach( $arrDados as $title => $dados ){
		
		$js.= ' var data_'.$title.' = '.retornaGrafico( $title , $dados  ).'; ';
		
		if($n == count($arrDados)){
			echo "<div id=\"grafico\" ></div>";
			$js.= 'swfobject.embedSWF(';
			$js.= '"/includes/open_flash_chart/open-flash-chart.swf", "grafico",';
			$js.= '"35%", "35%", "9.0.0" ,';
			$js.= '{"loading":"Carregando gráfico..."} );';
			$js.= 'function open_flash_chart_data(){return JSONOFC.stringify( data_'.$title.' );}';
		}
		
		$n++;
		
		$link.= "<a href=\"javascript:mudaGrafico(data_{$title},'{$title}')\" >$title</a> | ";
	}
	
?>
<script type="text/javascript">
function mudaGrafico(data,titulo){
	tmp = findSWF("grafico");
	x = tmp.load( JSON.stringify(data) );

	var elementosTabelas = document.getElementsByTagName('div'); 
	for (var i=0; i<elementosTabelas.length; i++){
  		var elemento = elementosTabelas[i];
	  	if (elemento.className == 'tabela') {
	    	elemento.style.display = "none";
  		}
	}
	document.getElementById("tabela_data_" + titulo ).style.display = "block";
}

function findSWF(movieName) {
	  if (navigator.appName.indexOf("Microsoft")!= -1) {
	    return window[movieName];
	  } else {
	    return document[movieName];
	  }
	}

	<?php  echo $js; ?>
</script>
<br />
<?php echo $link?>


<h3>Legenda</h3>

<?php 
	$n2 = 1;
	foreach( $arrDados as $title => $dados ){
		$contador = 1;
		$visibility  =  $n2==count($arrDados)  ?  "block" : "none";
		echo "<div class='tabela' id='tabela_data_{$title}' style='display:{$visibility}'>";
		
		$arrayParaMontaLista = array();
		foreach( $dados as $valor ){
			$quantidadeFormatada = formata_numero($valor['quantidade']);
			$arrayParaMontaLista[] = array(
										"contador" 			   => $contador , 
										"campoporcaosomatorio" => $valor['campoporcaosomatorio'] , 
										"quantidade" 		   => $quantidadeFormatada." ");
			$contador++;
		}		
		
		$cabecalho = array( 'Posição', 'Descrição', 'Quantitativo de estudantes contemplados');
		$db->monta_lista_simples( $arrayParaMontaLista, $cabecalho, 500000, 10, 'N', '100%', 'N' );
		
		echo "</div>";

		$n2++;
	}

?>
<br><br>
</body>
</html>

