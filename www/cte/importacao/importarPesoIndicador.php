<?php

	set_time_limit( 0 );
	
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";	

	$db = new cls_banco();	
	
	/****************************************************************************************
	*					IMPORTADOR DE PESOS DE INDICADOR (IDEB E DIMENSÃO)					* 
	****************************************************************************************/	
	
	$countIndicador = 0;	
	$nrSucessoIndicador = 0;
	$arErroIndicador = array();
	
	$arquivo = fopen("pesoIndicador.csv","r");
	
	while( ( $data = fgetcsv( $arquivo ) ) !== FALSE ) {
		
		$countIndicador++;
		$stLinha = str_replace( "\n", "", implode( "\n", $data ) );
		$arIndicador = explode( ";", $stLinha ); 
		
		if( !is_numeric( $arIndicador[1] ) ){
			$arErroIndicador[$countIndicador] = "Campos de Identificação com valores não numéricos.";
			continue;
		}
		
		$indid = $arIndicador[1];
		$nrPesoIDEB     = substr( $arIndicador[3], 0, 1 ).".". substr( $arIndicador[3], 1 );
		$nrPesoDimensao = substr( $arIndicador[4], 0, 1 ).".". substr( $arIndicador[4], 1 );
		
		$sql = "update cte.indicador set 
					indpesoideb = '$nrPesoIDEB',
					indpesodimensao = '$nrPesoDimensao'
				where indid = '$indid';";
		
		if( $db->executar( $sql ) ){
			$nrSucessoIndicador++; 			
		}
		else{
			$arErroIndicador[$countIndicador] = "Por alguma causa, motivo, razão ou circunstância ocorreu um erro na linha $count";;
		}
		
	}
	
	/****************************************************************************************
	*							IMPORTADOR DE PESOS DE DIRETRIZ								* 
	****************************************************************************************/
	
	$countDiretriz = 0;	
	$nrSucessoDiretriz = 0;
	$arErroDiretriz = array();
	
	$arquivo = fopen("pesoDiretriz.csv","r");
	
	while( ( $data = fgetcsv( $arquivo ) ) !== FALSE ) {
		
		$countDiretriz++;
		$stLinha = str_replace( "\n", "", implode( "\n", $data ) );
		$arDiretriz = explode( ";", $stLinha ); 
		
		if( !is_numeric( $arDiretriz[1] ) ){
			$arErroDiretriz[$countDiretriz] = "Campos de Identificação com valores não numéricos.";
			continue;
		}

		$dirid = recuperarIdDiretriz( $arDiretriz[0] );
		$indid = $arDiretriz[1];
		$nrPesoDiretriz =  substr( $arDiretriz[3], 0, 1 ).".". substr( $arDiretriz[3], 1 );
		
		$sql = "select count(*) from cte.diretrizindicador 
				where dirid = '$dirid'
				and indid = '$indid'";
		

		$boExiste = $db->pegaUm( $sql );
		
		if( $boExiste ){
			$arErroDiretriz[$countDiretriz] = "ATENÇÃO!!! Registro duplicado com dirid = $dirid e indid = $indid.";
//			continue;
		}
		
		$sql = "insert into cte.diretrizindicador ( dirid, indid, dinpeso ) values ( '$dirid', '$indid', '$nrPesoDiretriz' ); ";

		
		if( $db->executar( $sql ) ){
			$nrSucessoDiretriz++; 			
		}
		else{
			$arErroDiretriz[$countDiretriz] = "Por alguma causa, motivo, razão ou circunstância ocorreu um erro na linha $count";;
		}
		
	}
	
	$db->commit();
	
	
	function recuperarIdDiretriz( $nrDiretrizRomano ){
		
		switch( $nrDiretrizRomano ){
			case('I'): 		return  1; break;
			case('II'): 	return  3; break;
			case('III'):	return  4; break;
			case('IV'): 	return  5; break;
			case('V'): 		return  6; break;
			case('VI'): 	return  7; break;
			case('VII'): 	return  8; break;
			case('VIII'): 	return  9; break;
			case('IX'): 	return 10; break;
			case('X'): 		return 11; break;
			case('XI'): 	return 12; break;
			case('XII'): 	return 13; break;
			case('XIII'): 	return 14; break;
			case('XIV'): 	return 15; break;
			case('XV'): 	return 16; break;
			case('XVI'): 	return 17; break;
			case('XVII'): 	return 18; break;
			case('XVIII'): 	return 19; break;
			case('XIX'): 	return 20; break;
			case('XX'): 	return 21; break;
			case('XXI'): 	return 22; break;
			case('XXII'): 	return 23; break;
			case('XXIII'): 	return 24; break;
			case('XXIV'): 	return 25; break;
			case('XXV'): 	return 26; break;
			case('XXVI'): 	return 27; break;
			case('XXVII'): 	return 28; break;
			case('XXVIII'): return 29; break;
						
		}
	}
		
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<title>Importador de Peso</title>
	</head>

	<body>
		<div id="geral">
		   <?php

			echo "<h1>Importação de Peso de Indicadores</h1>";
			echo "<h1>Total de Registros: $countIndicador</h1>";
			echo "<h1>Total de registos Inseridos: $nrSucessoIndicador</h1>";
			
			if( count( $arErroIndicador ) ){
				
				echo "<hr />";
				echo "<h2>Relatório de Erros</h2>";
				foreach( $arErroIndicador as $linha => $stErro ){
					echo "<h4>Linha: $linha --> ". $stErro ."</h4>";
				}
			}
		   ?>

			<br /><br />
			<hr /><hr />
			<br /><br />
		
		   <?php

			echo "<h1>Importação de Peso de Diretrizes</h1>";
			echo "<h1>Total de Registros: $countDiretriz</h1>";
			echo "<h1>Total de registos Inseridos: $nrSucessoDiretriz</h1>";
			
			if( count( $arErroDiretriz ) ){
				
				echo "<hr />";
				echo "<h2>Relatório de Erros</h2>";
				foreach( $arErroDiretriz as $linha => $stErro ){
					echo "<h4>Linha: $linha --> ". $stErro ."</h4>";
				}
			}
		   ?>
			
		</div><!-- fim geral -->
	
	</body>
</html>