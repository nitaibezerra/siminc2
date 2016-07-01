<?php
/************************* INCLUDES *************************************************/
include_once(APPRAIZ.'www/brasilpro/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'brasilpro/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'brasilpro/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ.'brasilpro/classes/HistoricoConvItemComposicao.class.inc');

/************************* DECLARAÇÃO DE VARIAVEIS **********************************/
$ssuid 				= $_REQUEST['ssuid'];
$sbaid 				= $_REQUEST['sbaid'];
$prsid 				= $_REQUEST['prsid'];
$hmsid 				= $_REQUEST['hmsid'];
$hmsjustificativa 	= $_REQUEST['hmsjustificativa'];
$ano		 		= $_REQUEST['ano'];

/************************* INSTANCIA CLASSES E OBJETOS ******************************/
$obHistMonitoraConvSubac 	= new HistoricoMonitoramentoConvSubac();
$obHistConvItem			 	= new HistoricoConvItemComposicao();

/************************* RECUPERA DADOS *********************************/
if( $hmsid != 0 || $hmsid != ''  ){ // se o monitoramento do item já existe carrega dados.
	$obHistMonitoraConvSubac->carregarPorId( $hmsid );
}

/************************* SALVA DADOS ******************************/
if( $_REQUEST['salvar'] ){
	$hmsid = $obHistMonitoraConvSubac->recuperaHmsid($prsid, $sbaid, $ano);
	if($hmsid){ // se já existe será dado update caso não insert.
		$obHistMonitoraConvSubac->hmsid = $hmsid;
	}else{
		$obHistMonitoraConvSubac->hmcid = $_SESSION['hmc'][$prsid];
	}
	
	//Populando Objeto
	$obHistMonitoraConvSubac->ssuid = $ssuid;
	$obHistMonitoraConvSubac->sbaid = $sbaid;
	$obHistMonitoraConvSubac->hmsjustificativa = $hmsjustificativa = $hmsjustificativa != NULL ? $hmsjustificativa : '';
	$obHistMonitoraConvSubac->hmsano = $ano;
	$hmsid = $obHistMonitoraConvSubac->hmsid; 
	
	
	if($ssuid == NAOINICIADA){
		$statusItens = NAOINICIADAITEMCOMP;
	}
	else if($ssuid == CANCELADA ){
		$statusItens = CANCELADAITEMCOMP;	
	}else{
		$statusItens = 'NULL';
	}
	
	//Salvando dados subação
	$obHistMonitoraConvSubac->salvar();
	
	// INSERE E ATUALIZA ITENS DE COMPOSIÇÃO VINCULADOS A SUBAÇÃO
	if($obHistMonitoraConvSubac->hmsid){
		if($hmsid){ // SE EXISTE E UPDATE (atualiza e insere itens composição)
			$atualizaItens = $obHistConvItem->recuperaDadosItensComposicao($hmsid);

			if(is_array($atualizaItens)){
				foreach( $atualizaItens as $itens ){
					if($itens['hciid']){
						$sqlItens = "UPDATE cte.historicoconvitemcomposicao 
									 SET scsid   = ".$statusItens.",
									 	 hmsobs  = '".$hmsjustificativa."'
									 WHERE hciid = ".$itens['hciid']." AND hmsano = ".$ano;
						$obHistConvItem->carregar($sqlItens);
					}
				}
				$obHistConvItem->salvar();
			}
		}else{ // INSERT (atualiza e insere itens composição)
			$sql = "SELECT cosid 
					FROM cte.composicaosubacao cs 
					WHERE cs.sbaid = ".$sbaid." and cosano = ".$ano;
			$arrycosid = $obHistConvItem->carregar($sql);
			if(is_array($arrycosid)){
				foreach($arrycosid as $cosid ){
					$sql = "INSERT INTO cte.historicoconvitemcomposicao (hmsid, scsid, cosid, hmsobs, hmsano)
							VALUES (".$obHistMonitoraConvSubac->hmsid.", ".$statusItens.",".$cosid['cosid'].",'".$hmsjustificativa."', ".$ano.")
							";
					$obHistConvItem->carregar($sql);
				}
			}
		}
		$obHistMonitoraConvSubac->commit();
		$sql = "select ssudescricao from cte.statussubacao where ssutipostatus = 'P' and ssuid = ".$ssuid;
		$descricaoStatusSub = $obHistMonitoraConvSubac->pegaUm($sql);
		
		echo '<script> 
				alert( "Operação realizada com sucesso. " );
				parent.closeLyteboxSubAcao('.$sbaid.$prsid.$ano.','.$sbaid.','.$ssuid.','.$prsid.','.$obHistMonitoraConvSubac->hmsid.',"ok","'.$descricaoStatusSub.'",'.$ano.');
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

$comboStatusSubacao = carregaComboStatusSubacao($sbaid,$ssuid,$prsid,$hmsid);
$hmsjustificativa 	= $obHistMonitoraConvSubac->hmsjustificativa;
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../brasilpro/monitoraFinanceiro/includes/monitora_financeiro.css"/>
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
			<td><input type="button" name="salvar" value="salvar"  onclick="salvarDados(<?=$_SESSION['hmc'][$prsid]; ?>);" /></td>
		</tr>
	</table>

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
		if(hmcid != 0){
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
				if(hmcid != 0){ // se existe monitoramento.
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