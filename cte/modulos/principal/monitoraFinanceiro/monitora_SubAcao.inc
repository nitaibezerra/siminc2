<?php
/************************* INCLUDES *************************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoConvItemComposicao.class.inc');

/************************* DECLARAÇÃO DE VARIAVEIS **********************************/
$ssuid 				= $_REQUEST['ssuid'];
$sbaid 				= $_REQUEST['sbaid'];
$prsid 				= $_REQUEST['prsid'];
$hmsid 				= $_REQUEST['hmsid'];
$hmsjustificativa 	= $_REQUEST['hmsjustificativa'];
$ano		 		= $_REQUEST['ano'];
$hmcid 				= $_REQUEST['hmcid'];

/************************* INSTANCIA CLASSES E OBJETOS ******************************/
$obHistMonitoraConvSubac 	= new HistoricoMonitoramentoConvSubac();
$obHistConvItem			 	= new HistoricoConvItemComposicao();

/************************* RECUPERA DADOS *********************************/
if($sbaid && $ano){
	$hmstravadoMontAnterior = $obHistMonitoraConvSubac->carregaSubacaoAnteriorPorID($sbaid, $ano, $hmcid);
	if($hmstravadoMontAnterior){
		$idMonitoraSubPassado 	= $hmstravadoMontAnterior['hmsid'];
		$MonitoraPassadoTravado = $hmstravadoMontAnterior['hmstravado'];
	}
}
if( $hmsid != 0 || $hmsid != ''  ){ // se o monitoramento do item já existe carrega dados.
	$obHistMonitoraConvSubac->carregarPorId( $hmsid );
}
//dbg($MonitoraPassadoTravado);

/************************* SALVA DADOS ******************************/
if( $_REQUEST['salvar'] ){
	$hmsid = $obHistMonitoraConvSubac->recuperaHmsid($prsid, $sbaid, $ano, $hmcid);
	$pssid = $obHistMonitoraConvSubac->carregarPssid( $sbaid, $ano, $hmcid);
	$obHistMonitoraConvSubac->pssid = $pssid;
	
	if($hmsid){ // se já existe será dado update caso não insert.
		$obHistMonitoraConvSubac->hmsid = $hmsid;
	}else{
		$obHistMonitoraConvSubac->hmcid = $hmcid;
	}
	
	//Populando Objeto
	$obHistMonitoraConvSubac->ssuid = $ssuid;
	$obHistMonitoraConvSubac->sbaid = $sbaid;
	$obHistMonitoraConvSubac->hmsjustificativa = $hmsjustificativa = $hmsjustificativa != NULL ? $hmsjustificativa : '';
	$obHistMonitoraConvSubac->hmsano = $ano;
	$hmsid = $obHistMonitoraConvSubac->hmsid;
	
	if($ssuid == EXECUTADA || $ssuid == CANCELADA){ // Se status da subação for executado os proximos meses será copiada e o usuario poderá apenas ver.
		$obHistMonitoraConvSubac->hmstravado = 't';
	}else{
		$obHistMonitoraConvSubac->hmstravado = 'f';
	}
	
	$deletarItem = false;
	
	if($ssuid == NAOINICIADA){
		$statusItens = NAOINICIADAITEMCOMP;
		$updateItens = ",hmsquantidadeempenhada   	= null,
					 	 hmsquantidadeliquidada   	= null,
					 	 hmsquantidadepaga   		= null,
					 	 hmsvalorunitario   		= null,
					 	 hmsvalortotalempenhado   	= null,
					 	 hmsvalortotalliquidado   	= null,
					 	 hmsvalortotalpago			= null,
					 	 micid   					= null";
	}
	else if($ssuid == CANCELADA ){
		$statusItens = CANCELADAITEMCOMP;
		$updateItens = ",hmsquantidadeempenhada   	= null,
					 	 hmsquantidadeliquidada   	= null,
					 	 hmsquantidadepaga   		= null,
					 	 hmsvalorunitario   		= null,
					 	 hmsvalortotalempenhado   	= null,
					 	 hmsvalortotalliquidado   	= null,
					 	 hmsvalortotalpago			= null,
					 	 micid   					= null";	
	}else if($ssuid == EXECUTADA){
		if($MonitoraPassadoTravado == 'f' || $MonitoraPassadoTravado == NULL ){
			$deletarItem = true;
		}
		$statusItens = 'NULL';
	}else{
		$statusItens = 'NULL';
		$hmsjustificativa = '';
		$updateItens = '';
	}
	
	//Salvando dados subação
	
	if($obHistMonitoraConvSubac->salvar()){
		/*
		$obMonitora = new HistoricoMonitoramentoConvenio();
		$status = $obMonitora->statusMonitoramento($hmcid);
		if($status == 'F'){
			// atualiza subaçoes pra frente.
		}
	*/
	// INSERE E ATUALIZA ITENS DE COMPOSIÇÃO VINCULADOS A SUBAÇÃO
		$atualizaItens = $obHistConvItem->recuperaItensComposicao($hmsid, $obHistMonitoraConvSubac->sbaid, $ano);
			foreach( $atualizaItens as $itens ){
				if($deletarItem == true && $itens['hciid'] ){
					if($itens['hcitravado'] == 'f'){
						$sqlItens = "DELETE FROM cte.historicoconvitemcomposicao WHERE hciid = ".$itens['hciid'];
						$obHistConvItem->carregar($sqlItens);
					}
				}else if($itens['hciid'] && $deletarItem == false){
					if($itens['hcitravado'] == 'f'){
						$sqlItens = "UPDATE cte.historicoconvitemcomposicao 
									 SET scsid   					= ".$statusItens.",
									 	 hmsobs  					= '".$hmsjustificativa."'
									 ".$updateItens."	 
									 WHERE hciid = ".$itens['hciid']." AND hmsano = ".$ano;
						$obHistConvItem->carregar($sqlItens);
						
					}
				}else if($itens['cosid']){
					$sql = "INSERT INTO cte.historicoconvitemcomposicao (hmsid, scsid, cosid, hmsobs, hmsano)
							VALUES (".$obHistMonitoraConvSubac->hmsid.", ".$statusItens.",".$itens['cosid'].",'".$hmsjustificativa."', ".$ano.")
							";
					$obHistConvItem->carregar($sql);
				}
			}
			$obHistConvItem->salvar();
		
		$obHistMonitoraConvSubac->commit();
		$sql = "select ssudescricao from cte.statussubacao where ssutipostatus = 'P' and ssuid = ".$ssuid;
		$descricaoStatusSub = $obHistMonitoraConvSubac->pegaUm($sql);
		
		echo '<script> 
				alert( "Operação realizada com sucesso. " );
				parent.closeLyteboxSubAcao('.$sbaid.$prsid.$ano.','.$sbaid.','.$ssuid.','.$prsid.','.$obHistMonitoraConvSubac->hmsid.',"ok","'.$descricaoStatusSub.'",'.$ano.','.$hmcid.');
		  </script>';
		die();
	}else{
		echo "<script> 
			alert( 'Ocorreu um erro ao tentar salvar os dados. ' ); 
			parent.closeLyteboxSubAcao('0','0','0','0','0','erro');
	  	  </script>";
		die();
	}
}

/************************* REGRAS DE NEGOCIO E CARREGA DADOS PARA MOSTRAR NA TELA *************/
if($ssuid == NAOINICIADA){
	$titulo = "Porque a subção não foi iniciada?";
}else if($ssuid == CANCELADA ){
	$titulo = "Porque a subção foi cancelada?";
}

$comboStatusSubacao = $obHistMonitoraConvSubac->carregaComboStatusSubacao($sbaid,$ssuid,$prsid,$hmsid, $hmcid, $idMonitoraSubPassado);
$hmsjustificativa 	= $obHistMonitoraConvSubac->hmsjustificativa;
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type=hidden name="salvar" value="false" />
	<input type=hidden name="sbaid" value="<?php echo $sbaid; ?>" />
	<input type=hidden name="ano" value="<?php echo $ano; ?>" />
	<input type=hidden name="prsid" value="<?php echo $prsid; ?>" />
	<input type=hidden name="hmsid" value="<?php echo $hmsid; ?>" />
	<input type=hidden name="hmcid" value="<?php echo $hmcid; ?>" />
	<?php if( $MonitoraPassadoTravado == 'f' || $MonitoraPassadoTravado == NULL ){ 	?>
	<table id="form3" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th colspan="2" >Cadastro de Status da Sub-ação</th>
		</tr>
		<tr>
			<th id="texto" colspan="2" style="display:none;" ><?=$titulo; ?></th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Status da Subação:</td>
			<td><?=$comboStatusSubacao; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" id="JustificativaTexto" style="display:none;" >Justificativa:</td>
			<td id="JustificativaCampo" style="display:none;" ><?=campo_textarea('hmsjustificativa', "S", 'S', '', 70, 3, 2000 ); ?></td>
		</tr>
		<tr>
			<td></td>
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados(<?=$hmcid; ?>);" /></td>
		</tr>
	</table>
	<?php  }else{ 
			$sql = "SELECT ssudescricao 
					FROM cte.historicomonitoramentoconvsubac hms
					INNER JOIN cte.statussubacao ss ON ss.ssuid = hms.ssuid 
					WHERE hms.hmsid = ".$idMonitoraSubPassado;
			$descStatusSub = $obHistMonitoraConvSubac->pegaUm($sql);
	?>
		<table id="form3" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
			<tr>
				<th colspan="2" >Status da Sub-ação</th>
			</tr>
			<tr>
				<td><?php echo $descStatusSub; ?></td>
			</tr>
		</table>
	<?php  } ?>
</form>
<script type="text/javascript">
	ssuid = <?php echo $ssuid; ?>;
	if(ssuid == <?=NAOINICIADA; ?> || ssuid == <?=CANCELADA;?> ){
		document.getElementById("texto").style.display = '';
		document.getElementById("JustificativaCampo").style.display = '';
		document.getElementById("JustificativaTexto").style.display = '';
		if(ssuid == <?=NAOINICIADA; ?>){
			document.getElementById("texto").innerHTML = "Porque a subção não foi iniciada?";
		}else{
			document.getElementById("texto").innerHTML = "Porque a subção foi cancelada?";
		}
	}
	
	/**
	 * function salvarDados();
	 * descscricao 	: Salva dados do form no bd.
	 * author 		: Thiago Tasca Barbosa
	 */
	function salvarDados(hmcid){
		if(hmcid){
			var ssuid 		= document.getElementById("ssuid").value;
			var ssuidTexto 	= document.getElementById("ssuid").options[document.getElementById("ssuid").selectedIndex].text;
			if(ssuid){
				if(ssuid == <?=NAOINICIADA; ?> || ssuid == <?=CANCELADA;?>){
					if(confirm('Todos os itens de composição desta subação serão monitorado com o status " '+ssuidTexto+' ". \n Deseja salvar o status da subação?')){
						var justificativa = document.getElementById("hmsjustificativa").value;
						if(!justificativa){
							alert("Obrigatório informar a justificativa");
							return false;
						}
						document.formulario.salvar.value = true;
						document.formulario.submit();
					}
				}else{
					document.formulario.salvar.value = true;
					document.formulario.submit();
				}
			}else{
				alert("Obrigatório informar um status.");
				return false;
			}
		}else{
			alert("Para poder editar a subação inicie o monitoramento.");
			parent.closeLyteboxSubAcao('0','0','0','0','0','erro');
			return false;
		}
		
	}
	
	/**
			 * function escolhaStatusSubacao(sbaid, ssuid, prsid, hmcid, sbaidPrsid, historiocoSub);
			 * descscricao 	: De acordo com o status da subação a função habilita ou não os itens de composição.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: sbaid (Id da subação) 
			 * parametros 	: ssuid (status da subação)
			 * parametros 	: prsid (id do convênio)
			 * parametros 	: hmcid (id do Historico do Monitoramento)
			 * parametros 	: sbaidPrsid (atualiza campos do html)
			 * parametros 	: historiocoSub (id do monitoramento da subação)
			 */
			function escolhaStatusSubacao(ssuid, hmcid){
				if(hmcid){ // se existe monitoramento.
					if(ssuid == <?=NAOINICIADA; ?> || ssuid == <?=CANCELADA;?> ){
						document.getElementById("texto").style.display = '';
						document.getElementById("JustificativaCampo").style.display = '';
						document.getElementById("JustificativaTexto").style.display = '';
						if(ssuid == <?=NAOINICIADA; ?>){
							document.getElementById("texto").innerHTML = "Porque a subção não foi iniciada?";
						}else{
							document.getElementById("texto").innerHTML = "Porque a subção foi cancelada?";
						}
					}
					if(ssuid == <?=EXECUTADA; ?> || ssuid == <?=EMEXECUCAO; ?>){
						document.getElementById("texto").style.display = 'none';
						document.getElementById("JustificativaCampo").style.display = 'none';
						document.getElementById("JustificativaTexto").style.display = 'none';
					}
					if(ssuid == <?=STATUSNAOSELECIONADO; ?> ){
						alert("Selecione um status valido.");
					}
				}else{
					alert("Para poder editar a subação inicie o monitoramento.");
					return false;
				}
			}
			
			/*
			* Burlando monta_combo simec
			*/
			function dummies( valor ){
				return false;
			}
</script>
</body>
</html>