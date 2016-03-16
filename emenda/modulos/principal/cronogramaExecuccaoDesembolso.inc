<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');

$ptrid = ($_SESSION["emenda"]["ptrid"] ? $_SESSION["emenda"]["ptrid"] : $_SESSION["emenda"]["ptridAnalise"]);
validaSessionPTA( $ptrid );
verificaPermissao();
$tipoChamada = $_REQUEST['chamada'];

$obPTA = new PTA();
$obCronograma = new CronogramaExecucao( $ptrid );

$obCronograma->ptrid = $ptrid;
$obCronograma->tipoChamada = $tipoChamada;

if($_REQUEST["submetido"]) {
	
	if($_REQUEST["parcela_excluir"] && $_REQUEST["tipo_parcela_excluir"]) {
		$obCronograma->excluiCronogramaDesembolso( $_REQUEST );
	} else {
		//update
		$obCronograma->manterCronogramaExecucao( $_POST, $obPTA );
	}
		
}

$cronogramaExecucao = $obCronograma->verificaIniciativaVinculadaEspecificacao();

if( $_SESSION['emenda']['tpopup'] != 'popup' && !$tipoChamada ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
echo "<br />";
exibeInstituicaoBenefiada();
print "<br/>";

$estadoAtual = $obPTA->pegarEstadoAtual( $ptrid );

if(!$tipoChamada){
	validaSessionPTA( $_SESSION['emenda']['federal'] );
	
	montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );
	
	monta_titulo( 'Cronograma de Execução e Desembolso', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
	echo cabecalhoPlanoTrabalho( $ptrid );
	echo "<br />";
	$prdminuta = 'P'; // plano de trabalho
} else{
	monta_titulo( 'Cronograma de Execução e Desembolso', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
	if( $tipoChamada == 'pagamento' ){
		$prdminuta = 'G'; //pagamento
	} else {
		$prdminuta = 'M'; //minuta convenio
	}
}


$obCronograma->prdminuta = $prdminuta;

$verificaParcelaDesembolso = $obCronograma->carregaTipoParcelaDesembolso();

$existeParcelaConcedente = false;
$existeParcelaProponente = false;

foreach ($verificaParcelaDesembolso as $v) {
	if ($v["prdtipo"] == 'C')
		$existeParcelaConcedente = true;
	else if($v["prdtipo"] == 'P')
		$existeParcelaProponente = true;
}

$iniciativas = $obCronograma->carregaIniciativaCronograma();

if($existeParcelaConcedente == false){
	$arData = explode("/", $cronogramaExecucao[0]['inicial']);

	if( $tipoChamada == 'pagamento' ){
		$arParcelas = $obCronograma->verificaParcelaMinutaPagamento();

		if( $arParcelas[0] != $arParcelas[1] ){

			for($i=0; $i<$arParcelas[1]; $i++) {
				$data = '01/'.$arData[0].'/'.$arData[1];
				$data = ($i == 0 ? formata_data_sql( $data ) : formata_data_sql( somaDiasData(30, $data) ));
				$obCronograma->criarParcelaDesembolso($ptrid,  $data , 'C', $prdminuta);	
			}			
		} else {
			$data = $arData[1]."-".$arData[0]."-01";
			$obCronograma->criarParcelaDesembolso($ptrid, $data, 'C', $prdminuta);		
		}
	} else {
		$arData = explode("/", $cronogramaExecucao[0]['inicial']);
		$data = $arData[1]."-".$arData[0]."-01";
		$obCronograma->criarParcelaDesembolso($ptrid, $data, 'C', $prdminuta, $iniciativas);
	}
} else{
	if( $tipoChamada == 'pagamento' && !$_REQUEST['excluir'] ){
		$arParcelas = $obCronograma->verificaParcelaMinutaPagamento();

		if( sizeof( $arParcelas ) > 1 ){
			$arData = explode("/", $cronogramaExecucao[0]['inicial']);
			for($i=$arParcelas[0]; $i<$arParcelas[1]; $i++) {
				$data = '01/'.$arData[0].'/'.$arData[1];
				$data = ($i == 0 ? formata_data_sql( $data ) : formata_data_sql( somaDiasData(30, $data) ));
				$obCronograma->criarParcelaDesembolso($ptrid,  $data , 'C', $prdminuta);
			}			
		}
	}
}


if(!$tipoChamada){
	if($existeParcelaProponente == false){
		$data = explode("/", $cronogramaExecucao[0]['inicial']);
		$data = $data[1]."-".$data[0]."-01";
		$obCronograma->criarParcelaDesembolso($ptrid, $data, 'P', $prdminuta, $iniciativas);
	}
}

$arPerfil = pegaPerfilArray($_SESSION['usucpf']);

// Incluindo parcelas nas iniciativas que ainda nao as possuem
if( !in_array( CONSULTA_GERAL, $arPerfil ) ) $obCronograma->incluirParcelasInexistentes();

$boExcluir = 'excluir.gif';

$tipoPar = (!empty($tipoChamada) ? 'analise' : 'pta');

if( !verificaPermissaoPerfil( $tipoPar, 'boolean' ) ){
	$boExcluir = 'excluir_01.gif';
}
					
?>

<script language="JavaScript" src="includes/funcoes.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<?php
if(!$tipoChamada){
?>
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Cronograma de Execução</td>
	</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<table style="border-color:#ffffff;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
			 <tbody>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">Mês/Ano Inicial:</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$cronogramaExecucao[0]['inicial']?></td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">Mês/Ano Final:</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?=$cronogramaExecucao[0]['final']?></td>
				</tr>
			 </tbody>
			</table>
		</td>
	</tr>
 </tbody>
</table>
<br />
<?php
}
?>
<script type="text/javascript">	
	var id_ptiid = new Array();
</script>
<form id="formCronograma" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="parcela_excluir" id="parcela_excluir" value="" />
<input type="hidden" name="tipo_parcela_excluir" id="tipo_parcela_excluir" value="" />
<input type="hidden" name="tipoChamada" id="tipoChamada" value="<?php echo $tipoChamada; ?>" />
<input type="hidden" name="prdminuta" id="prdminuta" value="<?=$prdminuta;?>">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
<input type="hidden" name="datainicial" id="datainicial" value="<?=$cronogramaExecucao[0]["inicial"];?>">
<input type="hidden" name="datafinal" id="datafinal" value="<?=$cronogramaExecucao[0]["final"];?>">
<input type="hidden" name="federal" id="federal" value="<?=$_SESSION['emenda']['federal'];?>">
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Cronograma de Desembolso</td>
	</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
			 <tbody>
			 	<?
					$sql = "SELECT 
								to_char(prddata, 'MM/YYYY') as data,
								prdid
							FROM 
								emenda.ptparceladesembolso 
							WHERE 
								ptrid = ".($ptrid)." 
								AND prdtipo = 'C'
								AND prdminuta = '$prdminuta'
							ORDER BY
								prddata ASC";
								
					$parcelasConcedente = $db->carregar($sql);
					$parcelasConcedente = $parcelasConcedente ? $parcelasConcedente : array();
			 	?>
			 	<tr>
			 	<td>
			 	<?
			 	if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
				 	if( ($prdminuta == 'M' || $prdminuta == 'G') && verificaPermissaoPerfil( 'analise', 'boolean' ) ){
				 		echo "<a href=\"javascript:void(0);\" onclick=\"adicionarParcela('C', '".$prdminuta."');\"><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" border=\"0\"> Adicionar Parcela</a>";
				 	} else {
				 		if( $_SESSION['exercicio'] == '2009' || $_SESSION['exercicio'] == '2010' ){
						 	if( verificaPermissaoPerfil( 'pta', 'boolean', $estadoAtual ) /*&& $estadoAtual != EM_REFORMULACAO_PROCESSO*/ ) { ?>
						 		<a href="javascript:void(0);" onclick="adicionarParcela('C', '<?= $prdminuta; ?>');"><img src="../imagens/gif_inclui.gif" style="cursor:pointer;" border="0"> Adicionar Parcela</a>
						 	<? } else { ?>
						 		<img src="../imagens/gif_inclui_d.gif" style="cursor:pointer;" border="0"> Adicionar Parcela
						 	<? } 
						}
					}
				} else {?>
					<img src="../imagens/gif_inclui_d.gif" style="cursor:pointer;" border="0"> Adicionar Parcela
				<?}?>
			 	</td>
			 	</tr>
			 	<tr>
			 		<td width="100%" colspan="<?=(4 + count($parcelasConcedente))?>" style="text-align: center;" class="SubTituloEsquerda">DESEMBOLSO DO CONCEDENTE</td>
			 	</tr>
			 	<input type="hidden" id="num_parcelas_concedente" value="<?=count($parcelasConcedente)?>" />
				<tr>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
					<?
						$arrTipRef = verificaTiposReformulacao( $ptrid, 'codigo' );
						
						$arrParcelaPag = $db->carregarColuna("select o.orbnumparcela from emenda.ordembancaria o 
																where exfid in (select exfid from emenda.execucaofinanceira where ptrid = $ptrid)");
						
						foreach ($parcelasConcedente as $key => $v) {
							
							$boAlterar = 'alterar.gif';
							$boExcluir = 'excluir.gif';
							
							if( in_array( APOSTILAMENTO , $arrTipRef) ){
								if( is_array($arrParcelaPag) ){
									if( in_array( ($key+1), $arrParcelaPag ) ){
										$boAlterar = 'alterar_01.gif';
										$boExcluir = 'excluir_01.gif';
									}
								}
								$acaoEditar = '<a href="javascript:void(0);" onclick="editarParcela('.$v["prdid"].',\'C\');"><img src="../imagens/'.$boAlterar.'" style="cursor:pointer;" border="0"></a>&nbsp;';
							}
						
							if( $_SESSION['exercicio'] == '2009' || $_SESSION['exercicio'] == '2010' ){
								$excluirC = $acaoEditar.'<a href="javascript:void(0);" onclick="excluiParcela('.$v["prdid"].',\'C\');"><img src="../imagens/'.$boExcluir.'" style="cursor:pointer;" border="0"></a>&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"];
							} else {
								$excluirC = '&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - 
											<input type="text" id="prddata[]" name="prddata['.$v['prdid'].']" size="10" maxlength="7" onkeyup="formataDataDesembolso(this);" onblur="validaDataDesembolso(this);" value="'.$v["data"].'">';
							}
							if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
								if( $prdminuta == 'M' || $prdminuta == 'G' ){
									echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><a href="javascript:void(0);" onclick="excluiParcela('.$v["prdid"].',\'C\');"><img src="../imagens/'.$boExcluir.'" style="cursor:pointer;" border="0"></a>&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"].'</b></td>';								
								} else {
									if(possuiPermissao( $estadoAtual ) /*&& $estadoAtual != EM_REFORMULACAO_PROCESSO*/ ) {
										echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirC.'</b></td>'; 
									} else {
										echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><img src="../imagens/excluir_01.gif" style="cursor:pointer;" border="0">&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"].'</b></td>';
									}
								}
							} else {
								echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><img src="../imagens/excluir_01.gif" style="cursor:pointer;" border="0">&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"].'</b></td>';
							}
						}
					?>
					
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (concedente)</b></td>
				</tr>
				<?
					$arrTotalParcelas 		= array();
					$totalTotalParcelas 	= 0;
					$totalRestante 			= 0;
					$totalValorIniciativa	= 0;
					
					for($i=0; $i<count($iniciativas); $i++) {
						echo '<tr>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$iniciativas[$i]['ininome'].'</td>';
						
						if($tipoChamada == 'pagamento'){
							$valorConcedente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], 'G', 'C');
							$boValor = false;
							$arPriid = array();
							foreach ($valorConcedente as $v) {
								if( !empty( $v['privalor'] ) && $v['privalor'] != '0.00' ){
									$boValor = true;
								}
								$arPriid[] = $v['priid'];
							}
							if( !$boValor ){
								$valorConcedente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], 'M', 'C');
							}
						} else {
							$valorConcedente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], $prdminuta, 'C');
						}
						
						echo '<script> id_ptiid.push('.$valorConcedente[0]["ptiid"].'); </script>';
						
						$totalParcelas = 0;
						for($k=0; $k<count($valorConcedente); $k++) {
							if( $tipoChamada == 'pagamento' ){
								$name = 'privalor['.$arPriid[$k].']';
							} else {
								$name = 'privalor['.$valorConcedente[$k]["priid"].']';
								$prdtipo = 'prdtipo['.$valorConcedente[$k]["priid"].']'; 	
							}
							echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
									R$ <input class="normal" id="'.$valorConcedente[$k]["ptiid"].'_C_'.$k.'" name="'.$name.'" type="text" maxlength="13" size="15" value="'.number_format($valorConcedente[$k]["privalor"],2,",",".").'" onkeyup="this.value = mascaraglobal(\'###.###.###,##\',this.value);" />
									<input type="hidden" name="'.$prdtipo.'" id="prdtipo" value="C">
								  </td>';
							
							$totalParcelas 			+= $valorConcedente[$k]["privalor"];
							$arrTotalParcelas[$k]	+= $valorConcedente[$k]["privalor"];
							
							$restante = ($valorConcedente[$k]["ptivalorconcedente"] - $totalParcelas);

							$totalValorIniciativa	+= $valorConcedente[$k]["ptivalorconcedente"];
							
							$ptivalorconcedente		= $valorConcedente[$k]["ptivalorconcedente"];
							$ptiid					= $valorConcedente[$k]["ptiid"];
						}
						$totalvalorconcedente	+= $ptivalorconcedente;
						$totalTotalParcelas 	+= $totalParcelas;
						$totalRestante			+= $restante;
						
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.($ptivalorconcedente ? number_format($ptivalorconcedente,2,",",".") : '0,00').'</td>';
						echo '</tr>';
						echo '<input type="hidden" id="total_C_'.$ptiid.'" value="'.(($ptivalorconcedente) ? $ptivalorconcedente : 0).'" />';
					}
					
					//total
					echo '<tr>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
					for($i=0; $i<count($arrTotalParcelas); $i++) {
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
					}
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalvalorconcedente,2,",",".").'</b></td>';
					echo '</tr>';
				?>
			 </tbody>
			</table>
			<br />
			<?php 
			if(!$tipoChamada && $_SESSION['emenda']['federal'] == 'N'){
			?>
			<table style="border-color:#eeeeee;" align="center" class="Tabela" cellpadding="5" cellspacing="0">
			 <tbody>
			 	<?
					$sql = "SELECT 
								to_char(prddata, 'MM/YYYY') as data,
								prdid
							FROM 
								emenda.ptparceladesembolso 
							WHERE 
								ptrid = ".$ptrid." 
								AND prdtipo = 'P'
								AND prdminuta = '$prdminuta'
							ORDER BY
								prddata ASC";
					$parcelasProponente = $db->carregar($sql);
					$parcelasProponente = $parcelasProponente ? $parcelasProponente : array();
			 	?>
			 	<tr>
			 	<td>
			 	<?
				if( in_array( APOSTILAMENTO , $arrTipRef) ){
					$boExcluir = 'excluir_01.gif';
				}
						
			 	if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
				 	if( $_SESSION['exercicio'] == '2009' || $_SESSION['exercicio'] == '2010' ){
				 		if( verificaPermissaoPerfil( 'analise', 'boolean', $estadoAtual ) /*&& $estadoAtual != EM_REFORMULACAO_PROCESSO*/ ) { ?>
					 		<a href="javascript:void(0);" onclick="adicionarParcela('P', '<?= $prdminuta; ?>');"><img src="../imagens/gif_inclui.gif" style="cursor:pointer;" border="0"> Adicionar Parcela</a>
					 	<? } else { ?>
					 		<img src="../imagens/gif_inclui_d.gif" style="cursor:pointer;" border="0"> Adicionar Parcela
					 	<? }
					}?>
				<?} else {?>
					 <img src="../imagens/gif_inclui_d.gif" style="cursor:pointer;" border="0"> Adicionar Parcela
				<?}?>
			 	</td>
			 	</tr>
			 	<tr>
			 		<td width="100%" colspan="<?=(4 + count($parcelasProponente))?>" style="text-align: center;" class="SubTituloEsquerda">DESEMBOLSO DO PROPONENTE</td>
			 	</tr>
			 	<input type="hidden" id="num_parcelas_proponente" value="<?=count($parcelasProponente)?>" />
				<tr>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Iniciativa</b></td>
					<?
						if( $_SESSION['exercicio'] == '2009' || $_SESSION['exercicio'] == '2010' ){
							$excluirP = '<a href="javascript:void(0);" onclick="excluiParcela('.$v["prdid"].',\'P\');"><img src="../imagens/'.$boExcluir.'" style="cursor:pointer;" border="0"></a>';
						} else {
							$excluirP = (sizeof($parcelasProponente) == 1 ? '' : '<a href="javascript:void(0);" onclick="excluiParcela('.$v["prdid"].',\'P\');"><img src="../imagens/'.$boExcluir.'" style="cursor:pointer;" border="0"></a>');
						}
						foreach ($parcelasProponente as $key => $v) {
							if( $_SESSION['exercicio'] == '2009' || $_SESSION['exercicio'] == '2010' ){
								$excluirP = '<a href="javascript:void(0);" onclick="excluiParcela('.$v["prdid"].',\'P\');"><img src="../imagens/'.$boExcluir.'" style="cursor:pointer;" border="0"></a>&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"];
							} else {
								if( in_array( APOSTILAMENTO , $arrTipRef) ){
									$excluirP = '&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"];
								} else {
									$excluirP = '&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - 
											<input type="text" id="prddata[]" name="prddata['.$v['prdid'].']" size="10" onkeyup="formataDataDesembolso(this);" onblur="validaDataDesembolso(this);" maxlength="7" value="'.$v["data"].'">';
								}
							}
							if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
								if(possuiPermissao( $estadoAtual ) /*&& $estadoAtual != EM_REFORMULACAO_PROCESSO*/ ) {
									echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$excluirP.'</b></td>';
								} else {
									echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><img src="../imagens/excluir_01.gif" style="cursor:pointer;" border="0">&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"].'</b></td>'; 
								}
							} else {
								echo '<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><img src="../imagens/excluir_01.gif" style="cursor:pointer;" border="0">&nbsp;&nbsp;<b>'.($key+1).'ª Parcela - '.$v["data"].'</b></td>'; 								
							}
						}
					?>
					
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Total já informado para a iniciativa</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Restante</b></td>
					<td style="border:solid 1px #ffffff; background: rgb(220, 220, 220) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>Valor da Iniciativa (proponente)</b></td>
				</tr>
				<?
					$arrTotalParcelas 		= array();
					$totalTotalParcelas 	= 0;
					$totalRestante 			= 0;
					$totalValorIniciativa	= 0;
					
					for($i=0; $i<count($iniciativas); $i++) {
						echo '<tr>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">'.$iniciativas[$i]['ininome'].'</td>';

						$valorProponente = $obCronograma->carregaParcelaCronograma($iniciativas[$i]["ptiid"], $prdminuta, 'P');
						
						$totalParcelas = 0;
						for($k=0; $k<count($valorProponente); $k++) {
							echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
									R$ <input class="normal" id="'.$valorProponente[$k]["ptiid"].'_P_'.$k.'" name="privalor['.$valorProponente[$k]["priid"].']" type="text" maxlength="13" size="15" value="'.number_format($valorProponente[$k]["privalor"],2,",",".").'" onkeyup="this.value = mascaraglobal(\'###.###.###,##\',this.value);" />
									<input type="hidden" name="prdtipo['.$valorProponente[$k]["priid"].']" id="prdtipo" value="P">
								  </td>';
							
							$totalParcelas 			+= $valorProponente[$k]["privalor"];
							$arrTotalParcelas[$k]	+= $valorProponente[$k]["privalor"];
							
							$restante = ($valorProponente[$k]["ptivalorproponente"] - $totalParcelas);
							
							$totalValorIniciativa	+= $valorProponente[$k]["ptivalorproponente"];
							
							$ptivalorproponente = $valorProponente[$k]["ptivalorproponente"];
							$ptiid				= $valorProponente[$k]["ptiid"];
						}
						$totalvalorproponente	+= $ptivalorproponente;
						$totalTotalParcelas 	+= $totalParcelas;
						$totalRestante			+= $restante;
						
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($totalParcelas,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($restante,2,",",".").'</td>';
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">R$ '.number_format($ptivalorproponente,2,",",".").'</td>';
						echo '</tr>';
						echo '<input type="hidden" id="total_P_'.$ptiid.'" value="'.(($ptivalorproponente) ? $ptivalorproponente : 0).'" />';
					}
					
					//total
					echo '<tr>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: right;" class="SubTituloDireita"><b>Total:</b></td>';
					for($i=0; $i<count($arrTotalParcelas); $i++) {
						echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($arrTotalParcelas[$i],2,",",".").'</b></td>';
					}
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalTotalParcelas,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalRestante,2,",",".").'</b></td>';
					echo '<td style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita"><b>R$ '.number_format($totalvalorproponente,2,",",".").'</b></td>';
					echo '</tr>';
				?>
			 </tbody>
			</table>
			<?php
			} 
			?>
		</td>
	</tr>
	<tr>
		<td width="100" style="text-align: center" class="SubTituloEsquerda">
			<?php
			//if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
			if($_REQUEST['chamada']){?>
				<input type="button" value="Salvar Cronograma" id="bt_salvar" onclick="salvarCronograma();" <?=verificaPermissaoPerfil( 'analise', '', $estadoAtual, true ); ?>/>
				<input type="button" value="Fechar" id="bt_fechar" onclick="window.close();" >
		  <?}else{?>
		  		<input type="button" value="Salvar Cronograma" id="bt_salvar" onclick="salvarCronograma();" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?> />
		  <?}
			?>
			</td>
	</tr>
 </tbody>
</table>
</form>

<script>

var form 		= document.getElementById("formCronograma");
var btSalvar 	= document.getElementById("bt_salvar");

function adicionarParcela(tipo, estado) {
	if(confirm('Ao inserir uma nova parcela, as informações que ainda não foram salvas serão perdidas.\n                          Deseja continuar a criação da parcela?')) {
		var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereParcela&acao=A&tipo='+tipo+'&estado='+estado, 'parcela', 'width=400,height=200,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
		janela.focus();
	}
}

function excluiParcela(prdid, tipo) {
	if(confirm('Deseja excluir a parcela?')) {
		var parcelaExcluir 	= document.getElementById("parcela_excluir");
		var tipoExcluir		= document.getElementById("tipo_parcela_excluir");
		
		parcelaExcluir.value 	= prdid;
		tipoExcluir.value 		= tipo;
		
		form.submit();
	}
}

function editarParcela(prdid, tipo) {
	if(confirm('Deseja editar a parcela?')) {
		var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereParcela&acao=A&tipo='+tipo+'&estado=P&prdid='+prdid, 'parcela', 'width=400,height=200,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
		janela.focus();
	}
}

function validaDataDesembolso(obj){
	if( obj.value != '' ){
		var data = '01/'+obj.value;
		
		if(!validaDataEmenda( data ) ) {
			alert('A data da parcela está no formato incorreto.');
			obj.value = '';
			return false;
		}
	}
}

function formataDataDesembolso(obj){
	var data = '';
	if( obj.value.length == 2 ){
		data = obj.value + '/'; 
	} else {
		data = data + obj.value;
	}
	obj.value = data;
}

function verificaDataParcela(){
	var boVazio = false;
	for(i=0; i<form.length; i++){
		if( form.elements[i].type == 'text' ){
			if(form.elements[i].name.indexOf('prddata') != -1 ){
				if( form.elements[i].value == '' ){
					boVazio = true;
				}
			}
		}
	}
	return boVazio;
}

function salvarCronograma() {
	if(verificaDataParcela()){
		alert( 'Existe data da parcela em branco!' );
		return false;
	}
	btSalvar.disabled = true;
	var tipochamada = document.getElementById("tipoChamada").value;
	var federal = document.getElementById("federal").value;
	var numParcelasConcedente 	= document.getElementById("num_parcelas_concedente").value;
	if( !tipochamada && federal == 'N' ){
		var numParcelasProponente 	= document.getElementById("num_parcelas_proponente").value;
	} else {
		var numParcelasProponente = '';
	}
		
	for(var k=0; k<id_ptiid.length; k++) {
	
		var totalConcedente	= Number(document.getElementById("total_C_" + id_ptiid[k]).value);
		if( !tipochamada && federal == 'N' ){
			var totalProponente	= Number(document.getElementById("total_P_" + id_ptiid[k]).value);
		} else {
			var totalProponente	= '';
		}
	
		var somaConcedente = 0;
		for(var i=0; i<numParcelasConcedente; i++) {
			var valor = document.getElementById(id_ptiid[k] + '_C_' + i).value;
			
			valor = valor.replace(/\./gi, "");
			valor = valor.replace(/,/gi, ".");
			
			somaConcedente += Number(valor);
		}
		
		if(somaConcedente > totalConcedente) {
			alert('A soma dos valores das parcelas da iniciativa não pode ser\n        superior ao valor da Iniciativa (concedente).');
			document.getElementById(id_ptiid[k] + '_C_0').focus();
			btSalvar.disabled = false;
			return;
		}
		
		var somaProponente = 0;
		for(var i=0; i<numParcelasProponente; i++) {
			var valor = document.getElementById(id_ptiid[k] + '_P_' + i).value;
		
			valor = valor.replace(/\./gi, "");
			valor = valor.replace(/,/gi, ".");
			
			somaProponente += Number(valor);
		}
		
		if(somaProponente > totalProponente) {
			alert('A soma dos valores das parcelas da iniciativa não pode ser\n        superior ao valor da Iniciativa (proponente).');
			document.getElementById(id_ptiid[k] + '_P_0').focus();
			btSalvar.disabled = false;
			return;
		}
	}
	form.submit();
}

</script>