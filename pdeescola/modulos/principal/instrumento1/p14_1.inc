<?php
pde_verificaSessao();
/**
 * seguindo modelo do p9_8c.inc
 */
/*----------------- Configuraï¿½ï¿½es de Pï¿½gina ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


/*----------------- Aï¿½ï¿½es - Formulario ---------------------*/ 
$pdeid 		= $_SESSION["pdeid"];
$arCargos = $db->carregar( 'select cafid, cafdescricao from pdeescola.cargofuncao order by cafid' );

echo "<script> var cargos = new Array(); ";
for($i=0; $i<count($arCargos); $i++) {
	echo "cargos.push(".$arCargos[$i]["cafid"]."); ";
}
echo "</script>";

#Verifica se jï¿½ tem dados gravados
$sql = "SELECT
		  *
		FROM
		 pdeescola.pessoaltecnicoformacao
		WHERE
		 pdeid = ".$pdeid;
$dadosBanco = $db->carregar($sql);
$dadosBanco = is_array( $dadosBanco ) ? $dadosBanco : array();

function salvaP9_8a(){
		
	global $db, $pdeid, $arCargos;
	$del = "DELETE FROM pdeescola.pessoaltecnicoformacao WHERE pdeid = '$pdeid'";
			$db->executar( $del );
 
	extract( $_POST );
	foreach( $arCargos as $arCargo ) {
		$arQuantidade[$arCargo['cafid']]= ( empty( $arQuantidade[$arCargo['cafid']] ) || !is_numeric( $arQuantidade[$arCargo['cafid']] ) ) ? 0 : $arQuantidade[$arCargo['cafid']];
		$arFunComp[$arCargo['cafid']]   = ( empty( $arFunComp[$arCargo['cafid']] ) || !is_numeric( $arFunComp[$arCargo['cafid']] ) ) ? 0 : $arFunComp[$arCargo['cafid']];
		$arFunInComp[$arCargo['cafid']] = ( empty( $arFunInComp[$arCargo['cafid']] ) || !is_numeric( $arFunInComp[$arCargo['cafid']] ) ) ? 0 : $arFunInComp[$arCargo['cafid']];
		$arMedComp[$arCargo['cafid']]   = ( empty( $arMedComp[$arCargo['cafid']] ) || !is_numeric( $arMedComp[$arCargo['cafid']] ) ) ? 0 : $arMedComp[$arCargo['cafid']];
		$arMedInComp[$arCargo['cafid']] = ( empty( $arMedInComp[$arCargo['cafid']] ) || !is_numeric( $arMedInComp[$arCargo['cafid']] ) ) ? 0 : $arMedInComp[$arCargo['cafid']];
		$arMedOutra[$arCargo['cafid']]  = ( empty( $arMedOutra[$arCargo['cafid']] ) || !is_numeric( $arMedOutra[$arCargo['cafid']] ) ) ? 0 : $arMedOutra[$arCargo['cafid']];
		$arSupComp[$arCargo['cafid']]   = ( empty( $arSupComp[$arCargo['cafid']] ) || !is_numeric( $arSupComp[$arCargo['cafid']] ) ) ? 0 : $arSupComp[$arCargo['cafid']];
		$arSupInComp[$arCargo['cafid']] = ( empty( $arSupInComp[$arCargo['cafid']] ) || !is_numeric( $arSupInComp[$arCargo['cafid']] ) ) ? 0 : $arSupInComp[$arCargo['cafid']];
		$arSupOutra[$arCargo['cafid']]  = ( empty( $arSupOutra[$arCargo['cafid']] ) || !is_numeric( $arSupOutra[$arCargo['cafid']] ) ) ? 0 : $arSupOutra[$arCargo['cafid']];
		$arEspComp[$arCargo['cafid']]  = ( empty( $arEspComp[$arCargo['cafid']] ) || !is_numeric( $arEspComp[$arCargo['cafid']] ) ) ? 0 : $arEspComp[$arCargo['cafid']];
		$arEspInComp[$arCargo['cafid']]  = ( empty( $arEspInComp[$arCargo['cafid']] ) || !is_numeric( $arEspInComp[$arCargo['cafid']] ) ) ? 0 : $arEspInComp[$arCargo['cafid']];
		$arMestComp[$arCargo['cafid']]  = ( empty( $arMestComp[$arCargo['cafid']] ) || !is_numeric( $arMestComp[$arCargo['cafid']] ) ) ? 0 : $arMestComp[$arCargo['cafid']];
		$arMestInComp[$arCargo['cafid']]  = ( empty( $arMestInComp[$arCargo['cafid']] ) || !is_numeric( $arMestInComp[$arCargo['cafid']] ) ) ? 0 : $arMestInComp[$arCargo['cafid']];
		
				
		if ( count( $dadosBanco ) > 0 ) {
	
		$sql = "UPDATE pdeescola.pessoaltecnicoformacao
					SET 
							ptfquantidade = ".$arQuantidade[$arCargo['cafid']].",  
							ptfqtdensinofundfcompleto   = ".$arFunComp[$arCargo['cafid']].",
							ptfqtdensinofundfincompleto = ".$arFunInComp[$arCargo['cafid']].",
							ptfqtdensinomediocompleto   = ".$arMedComp[$arCargo['cafid']].",
							ptfqtdensinomedioincompleto = ".$arMedInComp[$arCargo['cafid']].",
							ptfqtdensinomediooutrahab   = ".$arMedOutra[$arCargo['cafid']].",
							ptfqtdensinosuperiorcompleto = ".$arSupComp[$arCargo['cafid']].",
							ptfqtdensinosuperiorincompleto = ".$arSupInComp[$arCargo['cafid']].",
							ptfqtdensinosuperiorsemlic     = ".$arSupOutra[$arCargo['cafid']].",
							ptfqtdespecializacaocompleto	= ".$arEspComp[$arCargo['cafid']].",
							pftqtdespecializacaoincompleto  = ".$arEspInComp[$arCargo['cafid']].",
							pftqtdmestradocompleto			= ".$arMestComp[$arCargo['cafid']].",
							pftqtdmestradoincompleto		= ".$arMestInComp[$arCargo['cafid']].",
							ptfdataatualizacao = now() )						
				WHERE pdeid = ".$pdeid." cafid = ".$arCargo['cafid'];			
		}
		else{
 
			$sql = "INSERT INTO pdeescola.pessoaltecnicoformacao( 
								pdeid,
								cafid,
								ptfquantidade,
								ptfqtdensinofundfcompleto,
								ptfqtdensinofundfincompleto,
								ptfqtdensinomediocompleto,
								ptfqtdensinomedioincompleto,
								ptfqtdensinomediooutrahab,
								ptfqtdensinosuperiorcompleto,
								ptfqtdensinosuperiorincompleto,
								ptfqtdensinosuperiorsemlic,
								ptfqtdespecializacaocompleto,
								pftqtdespecializacaoincompleto,
								pftqtdmestradocompleto,
								pftqtdmestradoincompleto,
								ptfanoreferencia,
								ptfdataatualizacao)						
							VALUES(	".
								$pdeid.",".
								$arCargo['cafid'].",".
								$arQuantidade[$arCargo['cafid']].",".
								$arFunComp[$arCargo['cafid']].",".
								$arFunInComp[$arCargo['cafid']].",".
								$arMedComp[$arCargo['cafid']].",".
								$arMedInComp[$arCargo['cafid']].",".
								$arMedOutra[$arCargo['cafid']].",".
								$arSupComp[$arCargo['cafid']].",".
								$arSupInComp[$arCargo['cafid']].",".
								$arSupOutra[$arCargo['cafid']].",".
								$arEspComp[$arCargo['cafid']].",".
								$arEspInComp[$arCargo['cafid']].",".
								$arMestComp[$arCargo['cafid']].",".
								$arMestInComp[$arCargo['cafid']].",".
								ANO_EXERCICIO_PDE_ESCOLA.",".
								" now() )";
		}

			$db->executar( $sql );
	}
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p14_1' AND
						  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
	
	$existe = $db->pegaUm("SELECT
							pepid
						 FROM
						 	pdeescola.pdeepf
						 WHERE
						 	pdeid = ".$pdeid." AND
						 	epfid = '$epfid'");
	
	if($existe == NULL) {
		$sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
		$db->executar($sql);
	}
		 
	$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p14_1'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p14_1','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit(); 
 
}

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

/***********************************************************
 * Componente que irï¿½ montar os butï¿½es do for*mulï¿½rio
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serï¿½o carregados pelo 
 * 										   __construct "Padrï¿½es ï¿½ classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   nï¿½o precisa dar include do arquivo na pï¿½gina.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos mï¿½todos 
 * 																		1ï¿½ parametro
 * 																		passa a funï¿½ï¿½o que serï¿½ aplicada nos botï¿½es 
 * 																		"anterior, prï¿½ximo, salvar anterior, salvar prï¿½ximo e salvar".
 * 																		2ï¿½ parametro
 * 																		serï¿½ um array de "ï¿½ndice" igual ao parametro da funcï¿½o 
 * 																		passada no 1ï¿½ parametro e o valor serï¿½ o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulï¿½rio "dentro da tabela ainda", este monta os butï¿½es.
 * 											  O parametro passado refere-se a funï¿½ï¿½o javascript que validarï¿½ o formulï¿½rio,
 * 											  devendo esta submeter o formulï¿½rio. 
 * 
 ***********************************************************/

$TotalQuantidade  = 0;
$TotalFunComp     = 0;
$TotalFunInComp   = 0;
$TotalMedComp     = 0;
$TotalMedInComp   = 0;
$TotalMedOutra    = 0;
$TotalSupComp     = 0;
$TotalSupInComp   = 0;
$TotalSupOutra    = 0;
$TotalEspecComp   = 0;
$TotalEspecInComp = 0;
$TotalMestComp    = 0;
$TotalMestInComp  = 0;


$sqlTotais = "SELECT
		  *
		FROM
		 pdeescola.pessoaltecnicoformacao
		WHERE
		 pdeid = ".$pdeid;
$rsTotais = $db->carregar( $sqlTotais );

//echo '<pre>';
//echo $pdeid;
//print_r( $rsTotais );
//echo '</pre>';
//die();

for( $x = 0; $x < count( $rsTotais ); $x ++)
{
	$TotalQuantidade 	+= $rsTotais[$x]['ptfquantidade']; 
    $TotalFunComp    	+= $rsTotais[$x]['ptfqtdensinofundfcompleto'];
    $TotalFunInComp  	+= $rsTotais[$x]['ptfqtdensinofundfincompleto'];
    $TotalMedComp    	+= $rsTotais[$x]['ptfqtdensinomediocompleto'];
    $TotalMedInComp  	+= $rsTotais[$x]['ptfqtdensinomedioincompleto'];
    $TotalMedOutra   	+= $rsTotais[$x]['ptfqtdensinomediooutrahab'];
    $TotalSupComp    	+= $rsTotais[$x]['ptfqtdensinosuperiorcompleto'];
    $TotalSupInComp  	+= $rsTotais[$x]['ptfqtdensinosuperiorincompleto'];
    $TotalSupOutra   	+= $rsTotais[$x]['ptfqtdensinosuperiorsemlic'];
    $TotalEspecComp  	+= $rsTotais[$x]['ptfqtdespecializacaocompleto'];
    $TotalEspecInComp	+= $rsTotais[$x]['pftqtdespecializacaoincompleto'];
    $TotalMestComp  	+= $rsTotais[$x]['pftqtdmestradocompleto'];
    $TotalMestInComp  	+= $rsTotais[$x]['pftqtdmestradoincompleto'];
}

#dbg( $dadosBanco, true );
foreach( $dadosBanco as $arBanco ){
    $arDadosBanco[$arBanco['cafid']]['ptfid'] = $arBanco['ptfid'];
    $arDadosBanco[$arBanco['cafid']]['pdeid'] = $arBanco['pdeid'];
    $arDadosBanco[$arBanco['cafid']]['cafid'] = $arBanco['cafid'];
    $arDadosBanco[$arBanco['cafid']]['ptfquantidade'] = $arBanco['ptfquantidade'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinofundfcompleto'] = $arBanco['ptfqtdensinofundfcompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinofundfincompleto'] = $arBanco['ptfqtdensinofundfincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomediocompleto'] = $arBanco['ptfqtdensinomediocompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomedioincompleto'] = $arBanco['ptfqtdensinomedioincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomediooutrahab'] = $arBanco['ptfqtdensinomediooutrahab'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorcompleto'] = $arBanco['ptfqtdensinosuperiorcompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorincompleto'] = $arBanco['ptfqtdensinosuperiorincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorsemlic'] = $arBanco['ptfqtdensinosuperiorsemlic'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdespecializacaocompleto'] = $arBanco['ptfqtdespecializacaocompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdespecializacaoincompleto'] = $arBanco['pftqtdespecializacaoincompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdmestradocompleto'] = $arBanco['pftqtdmestradocompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdmestradoincompleto'] = $arBanco['pftqtdmestradoincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfanoreferencia'] = $arBanco['ptfanoreferencia'];
    $arDadosBanco[$arBanco['cafid']]['ptfdataatualizacao'] = $arBanco['ptfdataatualizacao'];
    
	# Soma totais
	
    /*
	echo $pdeid;
	echo 'Quantidade  = :'.$TotalQuantidade;
    echo '<br>';
    
    $TotalQuantidade 	+= $arBanco['ptfquantidade']; 
    $TotalQuantidade 	+= $dadosbanco['ptfquantidade']; 
    $TotalFunComp    	+= $arBanco['ptfqtdensinofundfcompleto'];
    $TotalFunInComp  	+= $arBanco['ptfqtdensinofundfincompleto'];
    $TotalMedComp    	+= $arBanco['ptfqtdensinomediocompleto'];
    $TotalMedInComp  	+= $arBanco['ptfqtdensinomedioincompleto'];
    $TotalMedOutra   	+= $arBanco['ptfqtdensinomediooutrahab'];
    $TotalSupComp    	+= $arBanco['ptfqtdensinosuperiorcompleto'];
    $TotalSupInComp  	+= $arBanco['ptfqtdensinosuperiorincompleto'];
    $TotalSupOutra   	+= $arBanco['ptfqtdensinosuperiorsemlic'];
    $TotalEspecComp  	+= $arBanco['ptfqtdespecializacaocompleto'];
    $TotalEspecInComp	+= $arBanco['pftqtdespecializacaoincompleto'];
    $TotalMestComp  	+= $arBanco['pftqtdmestradocompleto'];
    $TotalMestInComp  	+= $arBanco['pftqtdmestradoincompleto'];
	*/
}
unset( $dadosBanco );
#dbg( $arDadosBanco, true );



/*----------------- Cabeï¿½alho ---------------------*/ 
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
echo cabecalhoPDE();

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salvaP9_8a()');
?>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<?=subCabecalho('14.1 - Pessoal Técnico de acordo com a formalização'); ?>
<table class="tabela" id="itensDeMatricula" border="0" align="center" cellpadding="0" cellspacing="2" width="100%" style="text-align: center">
  <tr>
    <th>Cargo / Função</th>
    <th>Quantidade</th>
    <th colspan="2">Ensino Fundamental</th>
    <th colspan="3">Ensino Médio</th>
    <th colspan="3">Ensino Superior</th>
    <th colspan="2">Especialização</th>
    <th colspan="2">Mestrado</th>
  </tr>
  <tr>
  <th>&nbsp;</th>
  <th colspan="3">&nbsp;</th>
  <th colspan="2">Habilitação Magisterio</th>
  <th rowspan="2">Outra<br> Habilitação</th>
  <th colspan="2">Com Licenciatura</th>
  <th rowspan="2">Sem<br> licenciatura</th>
  <th colspan="5">&nbsp;</th>
  </tr>
  <tr>
    <th>&nbsp;</th>
    <th>&nbsp;</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
  </tr>
  
<?php
//inicializando contador
$count = 1;
foreach( $arCargos as $arCargo ) {

if ($count % 2){
	$cor = "#dcdcdc";
}else{
	$cor = "#f5f5f5";
}
		
?>
  <tr style="background-color: <?=$cor ?>" >
    <th><?php echo $arCargo['cafdescricao']; ?></th>
    <td align="center"><?php echo campo_texto('arQuantidade['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, 'onblur="totalizar( \'spanTotalQuantidade\', \'arQuantidade\' );"', '', $arDadosBanco[$arCargo['cafid']]['ptfquantidade'] ); ?></td>
    <td align="center"><?php echo campo_texto('arFunComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalFunComp\', \'arFunComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinofundfcompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arFunInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalFunInComp\', \'arFunInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinofundfincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedComp\', \'arMedComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomediocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedInComp\', \'arMedInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomedioincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedOutra['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedOutra\', \'arMedOutra\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomediooutrahab'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupComp\', \'arSupComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorcompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupInComp\', \'arSupInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupOutra['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupOutra\', \'arSupOutra\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorsemlic'] ); ?></td>
    <td align="center"><?php echo campo_texto('arEspComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalEspComp\', \'arEspComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdespecializacaocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arEspInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalEspInComp\', \'arEspInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdespecializacaoincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMestComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMestComp\', \'arMestComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdmestradocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMestInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMestInComp\', \'arMestInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdmestradoincompleto'] ); ?></td>
  </tr>
  
<?php 

$count++;

} ?>
  <tr>
    <th><b>TOTAL</b></th>
    <td align="center"><span id="spanTotalQuantidade"><?php echo $TotalQuantidade; ?></span></td>
    <td align="center"><span id="spanTotalFunComp"><?php echo $TotalFunComp; ?></span></td>
    <td align="center"><span id="spanTotalFunInComp"><?php echo $TotalFunInComp; ?></span></td>
    <td align="center"><span id="spanTotalMedComp"><?php echo $TotalMedComp; ?></span></td>
    <td align="center"><span id="spanTotalMedInComp"><?php echo $TotalMedInComp; ?></span></td>
    <td align="center"><span id="spanTotalMedOutra"><?php echo $TotalMedOutra; ?></span></td>
    <td align="center"><span id="spanTotalSupComp"><?php echo $TotalSupComp; ?></span></td>
    <td align="center"><span id="spanTotalSupInComp"><?php echo $TotalSupInComp; ?></span></td>
    <td align="center"><span id="spanTotalSupOutra"><?php echo $TotalSupOutra; ?></span></td>
    <td align="center"><span id="spanTotalEspComp"><?php echo $TotalEspecComp; ?></span></td>
    <td align="center"><span id="spanTotalEspInComp"><?php echo $TotalEspecInComp; ?></span></td>
    <td align="center"><span id="spanTotalMestComp"><?php echo $TotalMestComp; ?></span></td>
    <td align="center"><span id="spanTotalMestInComp"><?php echo $TotalMestInComp; ?></span></td>
  </tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" align="center">
  <?php echo $cForm->montarbuttons("submeterDados()");?>
</table>
</form>
<script language="javascript" type="text/javascript">
function totalizar( stCampoTotal, stCampoFonte ) {
	var total = 0;
	
	// Totaliza a coluna
	for(i=0; i<cargos.length; i++ ) {
		var stCampoFonteElemento = stCampoFonte+"["+cargos[i]+"]";
		
		if (document.getElementsByName( stCampoFonteElemento ).item(0)) {
			total = Number( total ) + Number( document.getElementsByName( stCampoFonteElemento ).item(0).value );
		}
	}
	document.getElementById(stCampoTotal).innerHTML = total;
}

function submeterDados(){
	document.getElementById('submetido').value = 1;
	document.getElementById('formulario').submit();
	return true;
}


</script>