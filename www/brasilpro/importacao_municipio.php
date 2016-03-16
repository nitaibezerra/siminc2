<?php

/**
 * Rotina que controla o acesso às páginas do módulo. Carrega as bibliotecas
 * padrões do sistema e executa tarefas de inicialização. 
 *
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 22/03/2207
 */

/**
 * Obtém o tempo comprecisão de microsegundos. Essa informação é utilizada para
 * calcular o tempo de execução da página.  
 * 
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec; 
}

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

// carrega as funções do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$arquivo = fopen( "./carga.csv", "r" );
$instrumento = array();
while ( $registro = fgetcsv( $arquivo, 1000, ";" ) ) {
	$dimensao = $registro[0];
	$area = $registro[1];
	$indicador = $registro[2];
	$criterio = $registro[3];
	$instrumento[$dimensao][$area][$indicador][] = $criterio;
}
fclose( $arquivo );

$cd = 1;
echo "<pre>";
foreach ( $instrumento as $dimensao => $areas ) {
	$sql = "insert into cte.dimensao ( dimcod, dimdsc, itrid ) values ( {$cd}, '{$dimensao}', 4 )";
	echo str_replace( "\n", " ", str_replace( "\t", " ", $sql ) ) . ";\n";
//	$db->executar( $sql );
	$cd++;
	$ca = 1;
	foreach ( $areas as $area => $indicadores ) {
		$sql = "insert into cte.areadimensao ( ardcod, arddsc, dimid ) values ( {$ca}, '{$area}', (
			select d.dimid from cte.dimensao d
			where d.dimdsc = '{$dimensao}' and d.itrid = 4
		) )";
		echo str_replace( "\n", " ", str_replace( "\t", " ", $sql ) ) . ";\n";
//		$db->executar( $sql );
		$ca++;
		$ci = 1;
		foreach ( $indicadores as $indicador => $criterios ) {
			$sql = "insert into cte.indicador ( indcod, inddsc, ardid ) values ( {$ci}, '{$indicador}', (
				select a.ardid from cte.areadimensao a
					inner join cte.dimensao d on d.dimid = a.dimid and d.dimdsc = '{$dimensao}' and d.itrid = 4
				where a.arddsc = '{$area}' 
			) )";
			echo str_replace( "\n", " ", str_replace( "\t", " ", $sql ) ) . ";\n";
//			$db->executar( $sql );
			$ci++;
			$cc = 1;
			$criterios = array_reverse( $criterios );
			foreach ( $criterios as $indice => $criterio ) {
				if ( $indice == 0 ) {
					$sql = "insert into cte.criterio ( ctrord, ctrpontuacao, crtdsc, indid ) values ( 0, 0, 'Não se aplica', (
						select i.indid from cte.indicador i
							inner join cte.areadimensao a on a.ardid = i.ardid and a.arddsc = '{$area}'
							inner join cte.dimensao d on d.dimid = a.dimid and d.dimdsc = '{$dimensao}' and d.itrid = 4
						where inddsc = '{$indicador}'
					) )";
					echo str_replace( "\n", " ", str_replace( "\t", " ", $sql ) ) . ";\n";
				}
				$criterio = trim( $criterio );
				$ctrord = $indice + 1;
				$ctrpontuacao = $indice + 1;
				$sql = "insert into cte.criterio ( ctrord, ctrpontuacao, crtdsc, indid ) values ( {$ctrord}, {$ctrpontuacao}, '{$criterio}', (
					select i.indid from cte.indicador i
						inner join cte.areadimensao a on a.ardid = i.ardid and a.arddsc = '{$area}'
						inner join cte.dimensao d on d.dimid = a.dimid and d.dimdsc = '{$dimensao}' and d.itrid = 4
					where inddsc = '{$indicador}'
				) )";
				echo str_replace( "\n", " ", str_replace( "\t", " ", $sql ) ) . ";\n";
//				$db->executar( $sql );
				$cc++;
			}
		}
	}
}



