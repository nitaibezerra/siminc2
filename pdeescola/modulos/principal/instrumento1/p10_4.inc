<?php
pde_verificaSessao();
/**
 * seguindo modelo do p9_8c.inc
 */
/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*----------------- Ações - Formulario ---------------------*/ 
$pdeid 		= $_SESSION["pdeid"];
$arCiclos = $db->carregar( 'select * from pdeescola.seriecicloescolar where scetipo = \'C\' order by sceid' );

#Verifica se já tem dados gravados
$sql = "SELECT
		 *
		FROM
		 pdeescola.aproveitamentoalunocicloetapa
		WHERE
		 pdeid = ".$pdeid;
$dadosBanco = $db->carregar($sql);
$dadosBanco = is_array( $dadosBanco ) ? $dadosBanco : array();

function salvaP9_4(){
	global $db, $pdeid, $dadosBanco;
	#dbg( $_POST, true );
	extract( $_POST );
	foreach( $arMatriculaInicial as $sceid => $dados ) {
		$arMatriculaInicial[$sceid] = ( empty( $arMatriculaInicial[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arMatriculaInicial[$sceid];
		$arAdmitMarco[$sceid]     = ( empty( $arAdmitMarco[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAdmitMarco[$sceid]; 
		$arAfastAbandono[$sceid]  = ( empty( $arAfastAbandono[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAfastAbandono[$sceid];
		$arAfastTransf[$sceid]    = ( empty( $arAfastTransf[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAfastTransf[$sceid];
		$arAvaPS[$sceid]          = ( empty( $arAvaPS[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAvaPS[$sceid];
		$arAvaPPDA[$sceid]        = ( empty( $arAvaPPDA[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAvaPPDA[$sceid];
		$arAvaRFC[$sceid]         = ( empty( $arAvaRFC[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAvaRFC[$sceid];
		$arAvaPMAE[$sceid]        = ( empty( $arAvaPMAE[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arAvaPMAE[$sceid];
		$arMatAtual[$sceid]       = ( empty( $arMatAtual[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arMatAtual[$sceid];
		$arTxProgPS[$sceid]       = ( empty( $arTxProgPS[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arTxProgPS[$sceid];
		$arTxProgPPDA[$sceid]     = ( empty( $arTxProgPPDA[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arTxProgPPDA[$sceid];
		$arTxProgPMAE[$sceid]     = ( empty( $arTxProgPMAE[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arTxProgPMAE[$sceid];
		$arTxProgRetencao[$sceid] = ( empty( $arTxProgRetencao[$sceid] ) || !is_numeric( $arMatriculaInicial[$sceid] ) )  ? 0 : $arTxProgRetencao[$sceid];
		
		
		if ( count( $dadosBanco ) > 0 ) {
		$sql = "UPDATE pdeescola.aproveitamentoalunocicloetapa
			   SET pdeid=%s, sceid=%s, apaqtdmatriculainicial=%s, apaqtdadmitidoaposmarco=%s, 
			       apaqtdafastadoabandono=%s, apaqtdafastadotransferencia=%s, apaqtdavalps=%s, 
			       apaqtdavalppda=%s, apaqtdavalrfc=%s, apaqtdavalpmae=%s, apaqtdmatriculaatual=%s, 
			       apaqtdtxpps=%s, apaqtdtxpppda=%s, apaqtdtxppmae=%s, apaqtdtxpretencao=%s, 
			       apaanoreferencia=%s, apadataatualizacao= now()
				WHERE pdeid = ".$pdeid." and sceid = ".$sceid;			
		}
		else{
			$sql = "INSERT INTO pdeescola.aproveitamentoalunocicloetapa(
            pdeid, sceid, apaqtdmatriculainicial, apaqtdadmitidoaposmarco, 
            apaqtdafastadoabandono, apaqtdafastadotransferencia, apaqtdavalps, 
            apaqtdavalppda, apaqtdavalrfc, apaqtdavalpmae, apaqtdmatriculaatual, 
            apaqtdtxpps, apaqtdtxpppda, apaqtdtxppmae, apaqtdtxpretencao, 
            apaanoreferencia, apadataatualizacao)
    VALUES (%s, %s, %s, %s, 
            %s, %s, %s, 
            %s, %s, %s, %s, 
            %s, %s, %s, %s, 
            %s,  now());";
		}
		$sql = sprintf( $sql,
					$pdeid, $sceid, $arMatriculaInicial[$sceid], $arAdmitMarco[$sceid],
					$arAfastAbandono[$sceid], $arAfastTransf[$sceid], $arAvaPS[$sceid],
					$arAvaPPDA[$sceid], $arAvaRFC[$sceid], $arAvaPMAE[$sceid], $arMatAtual[$sceid],
					$arTxProgPS[$sceid], $arTxProgPPDA[$sceid], $arTxProgPMAE[$sceid], $arTxProgRetencao[$sceid],
					ANO_EXERCICIO_PDE_ESCOLA );
		#dbg( $sql, true );
		$db->executar( $sql );
	}
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p10_4' AND
						  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
	
	$existe = $db->pegaUm("SELECT
							pepid
						 FROM
						 	pdeescola.pdeepf
						 WHERE
						 	pdeid = ".$pdeid." AND
						 	epfid = ".$epfid);
	
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
											 	ppritem = 'p10_4'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p10_4', '{$_REQUEST['anoreferencia']}')";
			$db->executar( $sql_p ); 
		}
		
	 	$db->commit();  
}



$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag( 'salvaP9_4()' );

#dbg( $dadosBanco, true );
foreach( $dadosBanco as $arBanco ){
    $arDadosBanco[$arBanco['sceid']]['apaid'] = $arBanco['apaid'];
    $arDadosBanco[$arBanco['sceid']]['pdeid'] = $arBanco['pdeid'];
    $arDadosBanco[$arBanco['sceid']]['sceid'] = $arBanco['sceid'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdmatriculainicial'] = $arBanco['apaqtdmatriculainicial'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdadmitidoaposmarco'] = $arBanco['apaqtdadmitidoaposmarco'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdafastadoabandono'] = $arBanco['apaqtdafastadoabandono'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdafastadotransferencia'] = $arBanco['apaqtdafastadotransferencia'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdavalps'] = $arBanco['apaqtdavalps'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdavalppda'] = $arBanco['apaqtdavalppda'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdavalrfc'] = $arBanco['apaqtdavalrfc'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdavalpmae'] = $arBanco['apaqtdavalpmae'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdmatriculaatual'] = $arBanco['apaqtdmatriculaatual'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdtxpps'] = $arBanco['apaqtdtxpps'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdtxpppda'] = $arBanco['apaqtdtxpppda'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdtxppmae'] = $arBanco['apaqtdtxppmae'];
    $arDadosBanco[$arBanco['sceid']]['apaqtdtxpretencao'] = $arBanco['apaqtdtxpretencao'];
}
unset( $dadosBanco );
#dbg( $arDadosBanco, true );


/*----------------- Cabeçalho ---------------------*/ 
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
echo cabecalhoPDE();
?>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<?=subCabecalho('10.4 Aproveitamento dos alunos(ano anterior)'); ?>
<table class="tabela listagem" id="itensDeMatricula" border="2" align="center" cellpadding="0" cellspacing="4" width="100%" style="text-align: center">
 
 	<tr>
  	<td colspan="3">
  		
  		 <?php 
  		 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
  		 $anoex2  = $anoex +1;
 		 $anoex3  = $anoex2 +1;
//  		 $anoex4  = $anoex3 -1;
  		 $arrUrl  = $_SERVER['argv'][0];
	  	 $arrUrl  = explode("&", $arrUrl);
		 $arrItem = explode("/", $arrUrl[0]);	 
		 $indice  = (count($arrItem) - 1);
	  	 $ppritem = $arrItem[$indice];
  		 
  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
  		 					  FROM pdeescola.pdepreenchimento 
  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
  		 					  AND ppritem = '$ppritem'";
  		 
		 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>    
    </td>
  </tr>
  <tr>
    <th rowspan="2">Série / Etapa </th>
    <th rowspan="2">Matrícula Inicial</th>
    <th rowspan="2">Admitidos após mês de março</th>
    <th rowspan="2">Afastados por Abandono</th>
    <th rowspan="2">Afastados por Transferência</th>
    <th colspan="4">Avaliação</th>
    <th rowspan="2">Matrícula Atual</th>
    <th colspan="5">Taxa de Progressão</th>
  </tr>
  <tr>
    <th>PS</th>
    <th>PPDA</th>
    <th>RFC</th>
    <th>PMAE</th>
    <th>PS</th>
    <th>PPDA</th>
    <th>PMAE</th>
    <th>Retenção</th>
    <th>Taxa de abandono</th>
  </tr>
<?php
$letra = '1';
$arIdsCiclo = array();
# Inicializa variáveis de subtotal com valor 0
$subTotalMatriculaInicial = $subTotalMatAtual = $subTotalAdmitMarco = $subTotalAfastAbandono = $subTotalAfastTransf = $subTotalAvaPS = $subTotalAvaPPDA = $subTotalAvaRFC = $subTotalAvaPMAE = $subTotalTxProgPS = $subTotalTxProgPPDA = $subTotalTxProgPMAE = $subTotalTxProgRetencao = $subTotalTotalTxAbandono = 0;
foreach( $arCiclos as $arCiclo ) {
		#dbg( $arCiclo, true );
		$descricao = $arCiclo['scedescricao'];

		# Verifica se deve mostrar total parcial 
		if ( $letra != $descricao[0] ) {
?>
	<tr id="subTotal1">
		<th><b>TOTAL</b></th>
		<th><span id="subTotalMatriculaInicial_<?php echo $letra; ?>"><?php echo $subTotalMatriculaInicial; ?></span></th>
		<th><span id="subTotalAdmitMarco_<?php echo $letra; ?>"><?php echo $subTotalAdmitMarco; ?></span></th>
		<th><span id="subTotalAfastAbandono_<?php echo $letra; ?>"><?php echo $subTotalAfastAbandono; ?></span></th>
		<th><span id="subTotalAfastTransf_<?php echo $letra; ?>"><?php echo $subTotalAfastTransf; ?></span></th>
		<th><span id="subTotalAvaPS_<?php echo $letra; ?>"><?php echo $subTotalAvaPS; ?></span></th>
		<th><span id="subTotalAvaPPDA_<?php echo $letra; ?>"><?php echo $subTotalAvaPPDA; ?></span></th>
		<th><span id="subTotalAvaRFC_<?php echo $letra; ?>"><?php echo $subTotalAvaRFC; ?></span></th>
		<th><span id="subTotalAvaPMAE_<?php echo $letra; ?>"><?php echo $subTotalAvaPMAE; ?></span></th>
		<th><span id="subTotalMatAtual_<?php echo $letra; ?>"><?php echo $subTotalMatAtual; ?></span></th>
		<th><span id="subTotalTxProgPS_<?php echo $letra; ?>"><?php echo $subTotalTxProgPS; ?></span></th>
		<th><span id="subTotalTxProgPPDA_<?php echo $letra; ?>"><?php echo $subTotalTxProgPPDA; ?></span></th>
		<th><span id="subTotalTxProgPMAE_<?php echo $letra; ?>"><?php echo $subTotalTxProgPMAE; ?></span></th>
		<th><span id="subTotalTxProgRetencao_<?php echo $letra; ?>"><?php echo $subTotalTxProgRetencao; ?></span></th>
		<th><span id="subTotalTotalTxAbandono_<?php echo $letra; ?>"><?php echo round( $subTotalTotalTxAbandono/3, 2 ); ?> %</span></th>
	</tr>
<?php
			$letra = $descricao[0];
		
			# Inicializa variáveis de subtotal com valor 0
			$subTotalMatriculaInicial = $subTotalMatAtual = $subTotalAdmitMarco = $subTotalAfastAbandono = $subTotalAfastTransf = $subTotalAvaPS = $subTotalAvaPPDA = $subTotalAvaRFC = $subTotalAvaPMAE = $subTotalTxProgPS = $subTotalTxProgPPDA = $subTotalTxProgPMAE = $subTotalTxProgRetencao = $subTotalTotalTxAbandono = 0;
		 } // if imprime total
		# Soma subtotais
	    $subTotalMatriculaInicial += $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'];
	    $subTotalAdmitMarco       += $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'];
	    $subTotalAfastAbandono    += $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'];
	    $subTotalAfastTransf      += $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'];
	    $subTotalAvaPS            += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalps'];
	    $subTotalAvaPPDA          += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalppda'];
	    $subTotalAvaRFC           += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalrfc'];
	    $subTotalAvaPMAE          += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalpmae'];
	    $subTotalMatAtual         += $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculaatual'];
	    $subTotalTxProgPS         += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpps'];
	    $subTotalTxProgPPDA       += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpppda'];
	    $subTotalTxProgPMAE       += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxppmae'];
	    $subTotalTxProgRetencao   += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpretencao'];
	    $subTotalTotalTxAbandono  += $txAbandono;
	    
	    $valor_divisor = ( $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'] + 
					      $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'] -
					      $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'] );
		
		if($valor_divisor == 0) { 
			$resultado = 0;
		} else {
			$resultado = ($arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'] / $valor_divisor);
		}
			
	    $txAbandono = round( $resultado * 100, 2 );
	    		  
		$arIdsCiclo[$letra][] = $arCiclo['sceid'];
?>
  <tr>
    <th><?php echo $descricao; ?></th>
    <td align="center"><?php echo campo_texto('arMatriculaInicial['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'MatriculaInicial\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAdmitMarco['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AdmitMarco\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAfastAbandono['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AfastAbandono\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAfastTransf['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AfastTransf\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPS['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPS\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalps'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPPDA['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPPDA\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalppda'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaRFC['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaRFC\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalrfc'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPMAE['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPMAE\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalpmae'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMatAtual['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'MatAtual\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculaatual'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPS['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPS\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpps'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPPDA['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPPDA\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpppda'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPMAE['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPMAE\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxppmae'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgRetencao['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgRetencao\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpretencao'] ); ?></td>
    <th><span id="txAbandono[<?php echo $arCiclo['sceid']; ?>]"><?php echo $txAbandono; ?> %</span></th>
  </tr>
<?php 

	} ?>
	<tr id="subTotal1">
		<th><b>TOTAL</b></th>
		<th><span id="subTotalMatriculaInicial_<?php echo $letra; ?>"><?php echo $subTotalMatriculaInicial; ?></span></th>
		<th><span id="subTotalAdmitMarco_<?php echo $letra; ?>"><?php echo $subTotalAdmitMarco; ?></span></th>
		<th><span id="subTotalAfastAbandono_<?php echo $letra; ?>"><?php echo $subTotalAfastAbandono; ?></span></th>
		<th><span id="subTotalAfastTransf_<?php echo $letra; ?>"><?php echo $subTotalAfastTransf; ?></span></th>
		<th><span id="subTotalAvaPS_<?php echo $letra; ?>"><?php echo $subTotalAvaPS; ?></span></th>
		<th><span id="subTotalAvaPPDA_<?php echo $letra; ?>"><?php echo $subTotalAvaPPDA; ?></span></th>
		<th><span id="subTotalAvaRFC_<?php echo $letra; ?>"><?php echo $subTotalAvaRFC; ?></span></th>
		<th><span id="subTotalAvaPMAE_<?php echo $letra; ?>"><?php echo $subTotalAvaPMAE; ?></span></th>
		<th><span id="subTotalMatAtual_<?php echo $letra; ?>"><?php echo $subTotalMatAtual; ?></span></th>
		<th><span id="subTotalTxProgPS_<?php echo $letra; ?>"><?php echo $subTotalTxProgPS; ?></span></th>
		<th><span id="subTotalTxProgPPDA_<?php echo $letra; ?>"><?php echo $subTotalTxProgPPDA; ?></span></th>
		<th><span id="subTotalTxProgPMAE_<?php echo $letra; ?>"><?php echo $subTotalTxProgPMAE; ?></span></th>
		<th><span id="subTotalTxProgRetencao_<?php echo $letra; ?>"><?php echo $subTotalTxProgRetencao; ?></span></th>
		<th><span id="subTotalTotalTxAbandono_<?php echo $letra; ?>"><?php echo round( $subTotalTotalTxAbandono/3, 2 ) ?> %</span></th>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" align="center">
<?php echo $cForm->montarbuttons("submeterDados()");?>
</table>
</form>
<script language="javascript" type="text/javascript">
function totalizar( stColuna ){
	var i = 11;
	var total = 0;
	var ids1 = new Array();
	var ids2 = new Array();
	var ids3 = new Array();
	
	/*
	// Totaliza a coluna
	for(i; i <= 19; i++ ){
		var stCampoFonteElemento = "ar"+stColuna+"["+i+"]";
		if ( document.getElementsByName( stCampoFonteElemento ).item(0) ){
			total = Number( total ) + Number( document.getElementsByName( stCampoFonteElemento ).item(0).value );
		}
	}
	document.getElementById( 'total'+stColuna ).innerHTML = total;
	*/
	
	
<?php foreach ( $arIdsCiclo[1] as $sceid ){ ?>
	ids1.push( '<?php echo $sceid; ?>' );
<?php } ?>
<?php foreach ( $arIdsCiclo[2] as $sceid ){ ?>
	ids2.push( '<?php echo $sceid; ?>' );
<?php } ?>
<?php foreach ( $arIdsCiclo[3] as $sceid ){ ?>
	ids3.push( '<?php echo $sceid; ?>' );
<?php } ?>

	subTotalizar( 1, ids1, stColuna );
	subTotalizar( 2, ids2, stColuna );
	subTotalizar( 3, ids3, stColuna );
}

function subTotalizar( numero, ids, stColuna ){
	var total = 0;
	// Totaliza a coluna
	for(i=0; i <= ids.length; i++ ){
		var stCampoFonteElemento = "ar"+stColuna+"["+ids[i]+"]";
		if ( document.getElementsByName( stCampoFonteElemento ).item(0) ){
			total = Number( total ) + Number( document.getElementsByName( stCampoFonteElemento ).item(0).value );
		}
	}
	var nomeSubTotal = 'subTotal'+stColuna+'_'+numero;
	//alert( nomeSubTotal );
	document.getElementById( nomeSubTotal ).innerHTML = total;
}

function calcularTaxaAbandono( sceid ){
	var total = 0;
	//arAfastAbandono[sceid]
	// % ( arMatriculaInicial + arAdmitMarco - arAfastTransf;
	var nrAfastAbandono = Number( document.getElementsByName( 'arAfastAbandono['+sceid+']' ).item(0).value );
	var nrMatriculaInicial = Number( document.getElementsByName( 'arMatriculaInicial['+sceid+']' ).item(0).value );
	var nrAdmitMarco = Number( document.getElementsByName( 'arAdmitMarco['+sceid+']' ).item(0).value );
	var nrAfastTransf = Number( document.getElementsByName( 'arAfastTransf['+sceid+']' ).item(0).value );
	// Formula
	total = ( nrAfastAbandono / ( nrMatriculaInicial + nrAdmitMarco - nrAfastTransf ) * 10000 );
	
	if ( isNaN( total ) ) {
		document.getElementById('txAbandono['+sceid+']').innerHTML = '--';
	}else{
		document.getElementById('txAbandono['+sceid+']').innerHTML = Math.round( Number(total) )/ 100+' %';
	}
}

function submeterDados(){
	document.getElementById('submetido').value = 1;
	document.getElementById('formulario').submit();
	return true;
}


</script>