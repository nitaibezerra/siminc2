
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
	
$boContaCorrente = $db->pegaUm("select count(c.cocid) from emenda.contacorrente c where ptrid = {$_SESSION["emenda"]["ptridAnalise"]}");
?>

<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script type="text/javascript" src="js/emenda.js"></script>
<link rel="stylesheet" type="text/css" href="css/emenda.css"/>
<!-- LINKS MODAL -->
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
});

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,345);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	function solicitarPagamentoPTA(ptrid, exfid){
		var form = $('formExecPTA');
		var msg  = '';
		
		var boVazio = false;
		
		if( $('numparcela['+exfid+']').value == ''){
			alert('O campo Parcela é de preenchimento obrigatório!');
			$('numparcela['+exfid+']').focus();
			return false;
		} else if ($('orbmesparcela['+exfid+']').value == ''){
			alert('O campo Mês é de preenchimento obrigatório!');
			$('orbmesparcela['+exfid+']').focus();
			return false;
		}else if ($('orbanoparcela['+exfid+']').value == ''){
			alert('O campo Ano é de preenchimento obrigatório!');
			$('orbanoparcela['+exfid+']').focus();
			return false;
		} else if ($('orbvalorparcela['+exfid+']').value == ''){
			alert('O campo Valor é de preenchimento obrigatório!');
			$('orbvalorparcela['+exfid+']').focus();
			return false;
		}
		
		var retorno = 'true';
		var totCampos = form.length;
		
		retorno = validaValorParcela( exfid, totCampos );
		/*var i = 0;
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == 'hidden'){
				if( form.elements[i].id.indexOf('exfid_p') != -1 ){
					retorno = validaValorParcela( form.elements[i].value, totCampos );
				}
			}
		}*/
		
		if( retorno == 'false' ){
			return false;
		}
		var registro = '';
		//if( $('boalterou').value == 'true' ){
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=salvarParcela&'+ $('formExecPTA').serialize();
				
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r){
							//$('divDebug').innerHTML = r.responseText;
							registro = r.responseText;
							/*if( trim(r.responseText) == '1'){
							}else{
								alert('Falha na operação.');
							}*/
						}
					});
		//}
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=original&ptrid='+ptrid+'&exfid='+exfid+'&solicitacao=pagamento&arparcela='+registro);
	}
	function consultarPagamentoPTA(ptrid,exfid, orbid){
		if(ptrid && exfid && orbid){
			displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=consultaPagamento&solicitacao=pagamento&ptrid='+ptrid+'&exfid='+exfid+'&orbid='+orbid);
		} else {
			alert('Operação Consulta Situação pagamento falhou.');
			return false;
		}

	}
	function popUpIncluirCronogramaDesenbolso() {
		document.getElementById('labelCronograma').innerHTML = 'Clique aqui para visualizar o Cronograma Desembolso do Convenio';	
		var janela = window.open( 'emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A&chamada=pagamento', 'popup_Cronograma', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
		janela.focus();
	}
	function cancelarPagamento(ptrid, exfid, nu_seg_pag, orbid){
				
		if(nu_seg_pag){
			if(!confirm('Deseja realmente cancelar o pagamento?')){
				return false;
			}
			displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=anular&solicitacao=pagamento&ptrid='+ptrid+'&exfid='+exfid+'&orbnumsolicitacao='+nu_seg_pag+'&orbid='+orbid);
		}		
	}
	
	function populaHiddenOrbvalorparcela( valor, exfid, id ){
		var valor = valor.replace(/\./gi, '');
		var valor = valor.replace(/\,/gi, '.');
		
		document.getElementById( 'valor1_[' + exfid + ']_'+ id ).value = valor;	
	}
	
	function montaNumeroParcela(exfid){
		var form = document.getElementById('formExecPTA');
		var parcela = 'parcela_'+exfid;
		var count = $(parcela).value;		
		
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == 'text'){
				if(form.elements[i].id.indexOf('numparcela[]') != -1){
					count++;
					form.elements[i].value = count;
				}				
			}
		}
	}
	
	function validaValorParcela( exfid, totCampos ){
		var form1 = $('formExecPTA');
		var valorparcelapaga = $('parcelapaga_'+exfid).value ; 
		//var valorTotal = parseFloat( $('valor1_[' + exfid + ']_1').value );
		var exfvalor = 'exfvalor_'+exfid;
		var valorDigitado = $('orbvalorparcela[' + exfid + ']').value;
		var valorDigitado = valorDigitado.replace(/\./gi, '');
		var valorDigitado = valorDigitado.replace(/\,/gi, '.');
		
		var valorTotal = (parseFloat( valorDigitado ) + parseFloat(valorparcelapaga));
		
		 
		/*var i = 0;
		for(i=0; i<totCampos; i++){
			if(form1.elements[i].type == 'hidden'){
				if(form1.elements[i].id.indexOf('valor1_[' + exfid + ']') != -1){
					valorTotal = valorTotal + parseFloat( form1.elements[i].value );
				}				
			}
		}*/
		//var restante = $(exfvalor).value -  valorparcelapaga.toFixed(2);
		
		if( valorTotal.toFixed(2) > parseFloat( $(exfvalor).value ) ){
			alert('O valor das parcelas não pode ultrapassar o valor do empenho: \n Valor das Parcelas:    R$ '+ mascaraglobal('###.###.###.###,##', valorTotal.toFixed(2)) +' \n Valor do Empenho:    R$ ' +  mascaraglobal('###.###.###.###,##', $(exfvalor).value) );
			return 'false';
		} else {
			return 'true';
		}
	}
	
	function validaMesParcela(mes, exfid){
		var form = document.getElementById('formExecPTA');
		var arMes = new Array('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12');
		
		if( mes.value != '' ){
			if( mes.value.length == 1 ){
				mes.value = '0'+mes.value;
			}
			if( arMes.indexOf(mes.value) == -1 ){
				alert('Mês inválido!');
				mes.value = '';
				return false;
			}
		}
	}
	
	function validaAnoParcela(ano, exfid){
		var form = document.getElementById('formExecPTA');
		var count = 0;
		
		/*if( ano.value != '' ){
			for(i=0; i<form.length; i++){
				if(form.elements[i].type == 'text'){
					if( ( form.elements[i].name.indexOf('orbanoparcela[' + exfid + '][]') != -1 ) && ( form.elements[i].value != '' ) ){
						if( form.elements[i].value == ano.value ){
							count++;
						}
					}				
				}
			}
			if( count > 1 ){
				alert('Já foi informado uma parcela para este mês\ano!');
				ano.value = '';
				return false;
			}
		}*/
	}
	
	function consultarPagamentoHistorico(ptrid, orbid){
		if(ptrid){
			window.open('emenda.php?modulo=principal/analisepta/pagamento/historicoSolicitacao&acao=A&ptrid='+ptrid+'&orbid='+orbid, 'Consulta Pagamento PTA', 'scrollbars=yes,height=800,width=1200,status=no,toolbar=no,menubar=no,location=no');
		} else {
			alert('Operação Consulta Execução falhou.');
			return false;
		}

	}
	function ConsultarAndamentocc(ptrid){
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_consulta=consultarandamentocc&ptrid='+ptrid+'&tipo=p');
	}
	function solicitarContaCorrente(ptrid){
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_consulta=solicitarcontacorrente&ptrid='+ptrid+'&tipo=p');
	}
</script>
<body>
<?php abrePTAconsolidado($ptrid); ?>
<form action="emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A" name="formPagamentoPTA" id="formPagamentoPTA">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td colspan="2" class="subtitulocentro">Dados para o Pagamento</td>
	</tr>
				
	<!-- <tr>
		<td class="SubTituloDireita" valign="middle"><b>Cronograma Desembolso:</b></td>
		<td >
			<a href="#" onclick="popUpIncluirCronogramaDesenbolso();" style="cursor:pointer">
				<label id="labelCronograma">Clique aqui para preencher o Cronograma Desembolso do Convenio</label>
			</a>
			<input type="hidden" name="boCronograma" id="boCronograma" value="<?=$boCronograma; ?>" />
		</td>
	</tr> -->
</table>
</form>
<?if($arDados){ ?>
<form action="" method="post" id="formExecPTA" name="formExecPTA">
<input type="hidden" name="boalterou" id="boalterou" value="true">	
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td>
			<table class="tabela" width="40%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th>Situação Habilitação</th>
				</tr>
				<tr>
					<td><? echo $obHabilita->consultaHabilita($cnpjHabilita,true); ?></td>
				</tr>
			</table>
		</td>
	</tr>
<?php	if( $boContaCorrente > 0 ){ ?>
			<tr>
				<td>
					<table class="tabela" width="40%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
						<tr>
							<td valign="middle" class="subtitulodireita" style="width: 25%"><b>Deseja atualizar dados da Conta Corrente:</b></td>
							<td><input type="button" id="btnCC" name="btnCC" value="Clique Aqui" style="cursor: pointer;" onclick="ConsultarAndamentocc(<?=$ptrid; ?>);"></td>
						</tr>
					</table>
				</td>
			</tr>
<?php 	} else {?>
			<tr>
				<td>
					<table class="tabela" width="40%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
						<tr>
							<td valign="middle" class="subtitulodireita" style="width: 25%"><b>Dados Conta Corrente:</b></td>
							<td><input type="button" id="btnCC" name="btnCC" value="Solicitar Conta Corrente" style="cursor: pointer;" onclick="solicitarContaCorrente(<?=$ptrid; ?>);"></td>
						</tr>
					</table>
				</td>
			</tr>
<?php	}?>
	<tr>
		<td>
			<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<thead>
					<tr bgcolor="dedfde">
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Funcional</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Código</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Autor</strong></td>
						<td align="Center" class="title" width="5%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>GND</strong></td>
						<td align="Center" class="title" width="5%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mod</strong></td>
						<td align="Center" class="title" width="5%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></td>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nível de Ensino</strong></td>
						<?php if($boAlterarPagamento == 1){ ?>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Original</strong></td>
						<?php } ?>
						<td align="Center" class="title" width="10%"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
					</tr>
				</thead>
<?

	foreach ($arDados as $chave => $valor) {
		$arParcela = $obPagamento->carregaParcelas( $valor['exfid'] );
		//ver($arParcela);
		$totalParcela = sizeof( $arParcela );
		$arParcelaPaga = array();
		$valorTotalParcela = 0;
		
		foreach ($arParcela as $v) {
			if( $v['orbnumsolicitacao'] ) $arParcelaPaga[] = $v['orbnumsolicitacao'];	
			$valorTotalParcela += (float)$v['orbvalorparcela'];
		}
			$chave % 2 ? $cor = "" : $cor = "#ffffff";
			?>	
			<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['fupfuncionalprogramatica']; ?></td>
				<td><?=$valor['tipoemenda']; ?></td>
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['emecod']; ?></td>
				<td><?=$valor['autnome']; ?></td>
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['gndcod']; ?></td>
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['mapcod']; ?></td>
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['foncod']; ?></td>
				<td><?=$valor['tpedsc']; ?></td>
				<?php
				if($boAlterarPagamento == 1){
					$boSomenteAnulacao = false;
					// Verifica se o Valor e igual ao valor original
					
					if($valor['pervalor'] == $valor['exfvalor']){
						$boSomenteAnulacao = true;
					}
					
					$pervalor 	   = number_format($valor['pervalor'],2,',','.');
					$valorOriginal = number_format($valor['exfvalor'],2,',','.');
					$disabled = "disabled=\"disabled\"";
					$colspan = "colspan=\"10\"";
				?>
					<td style="text-align: center; color: rgb(0, 102, 204);">
						<input type="hidden" id="pervalorOriginal_<?=$valor['exfid']; ?>" value="<?php echo $valorOriginal; ?>">
						R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?>
					</td>
					
					<td style="text-align: center; color: rgb(0, 102, 204);">
						<input type="hidden" id="valor_<?=$valor['exfid']; ?>" value="<?php echo $pervalor; ?>">
						R$ <?php echo number_format($valor['pervalor'],2,',','.'); ?>
					</td>
				<?php 
				} else {
					$colspan = "colspan=\"9\"";
					$disabled = "";
					?>
				
					<td style="text-align: center; color: rgb(0, 102, 204);">R$ <?php echo number_format($valor['exfvalor'],2,',','.'); ?></td>
				
				<?php } ?>
			</tr>
			<tr bgcolor="<?=$cor; ?>">
				<td <?php echo $colspan;?>>
					<table width="93%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
						<tr bgcolor="dedfde">
							<td width="02%"><img border="0" src="../imagens/seta_filho.gif"></td>
							<td align="Center" class="title" width="05%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PTRES</strong></td>
							<td align="Center" class="title" width="25%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PI</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Esfera ADM</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Esfera Orçamentária</strong></td>
							<td align="Center" class="title" width="10%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza de Despesa</strong></td>
							<td align="Center" class="title" width="25%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte SIAFI</strong></td>
							<td align="Center" class="title" width="25%"
								style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><?=$label; ?></strong></td>
						</tr>
						<tr bgcolor="<?=$cor; ?>">
							<td>&nbsp;</td>
							<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['pliptres']; ?></td>
							<td><?=$valor['plicod'].' - '.$valor['plititulo']; ?></td>								
							<td><?=$valor['esferaadm']; ?></td>								
							<td><?=$valor['esfcod'].' - '.$valor['esfdsc'];; ?></td>								
							<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['naturezadesp']; ?></td>								
							<td><?=$valor['fondsc']; ?></td>								
							<td><?=$valor['exfnumempenhooriginal']; ?></td>								
														
						</tr>
						<?
						$boValorIgual = false;
						
						if( ((float)$valorTotalParcela < (float)$valor['exfvalor']) || $valor['spgcodigo'] == 9 ){
							$boValorIgual = true;
						?>
						<tr bgcolor="<?=$cor; ?>">
							<td colspan="8">
								<input type="hidden" name="parcela_<?=$valor['exfid']; ?>" id="parcela_<?=$valor['exfid']; ?>" value="<?=sizeof( $arParcelaPaga ); ?>">
								<input type="hidden" name="parcelapaga_<?=$valor['exfid']; ?>" id="parcelapaga_<?=$valor['exfid']; ?>" value="<?=$valorTotalParcela; ?>">
								<input type="hidden" name="valorigual_<?=$valor['exfid']; ?>" id="valorigual_<?=$valor['exfid']; ?>" value="<?=$boValorIgual; ?>">
								<input type="hidden" name="exfid_p[]" id="exfid_p[<?php echo $valor['exfid']; ?>][]" value="<?=$valor['exfid']; ?>">
								<input type="hidden" name="exfvalor_<?=$valor['exfid']; ?>" id="exfvalor_<?=$valor['exfid']; ?>" value="<?=$valor['exfvalor']; ?>">
								<table width="50%" bgcolor="#f5f5f5" id="tabelaParcela_<?php echo $valor['exfid']; ?>" cellspacing="1" cellpadding="2">
								<tr>
									<tbody id="tbodyTabela>">
									<td colspan="5">
										<?
										/*if( !$boReformulacao ){
											if( $boDisabled ){
												echo '<span id="linkInserirLinha_'.$valor['exfid'].'" style="margin-left: 3%; cursor: pointer;">
														<img align="top" style="border: medium none ;" src="/imagens/gif_inclui_d.gif"/>Inserir Parcelas
													</span>';								
											} else {
												echo '<span id="linkInserirLinha_'.$valor['exfid'].'" onclick="return carregarParcela( \'\', \'\', \'\', \'\', \''.$valor['exfid'].'\', \'0\', \'\', \'\');" style="margin-left: 3%; cursor: pointer;">
														<img align="top" style="border: medium none ;" src="/imagens/gif_inclui.gif"/>Inserir Parcelas
													</span>';
											}
										}*/
										?>
									</td>
								</tr>
								<tr>
								</tr>
								<tr bgcolor="dedfde">
									<td align="Center" class="title" width="5%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Parcela</strong></td>
									<td align="Center" class="title" width="5%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mês</strong></td>
									<td align="Center" class="title" width="5%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ano</strong></td>
									<td align="Center" class="title" width="15%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></td>
									<td align="Center" class="title" width="5%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
								</tr>
								<tbody id="tbodyTabela_<?php echo $valor['exfid']; ?>">
								<?
								/*$arrParcelaPaga = $db->pegaLinha( "select spgcodigo, valor, valortotal, parcela from(
																				SELECT 
																			        o.spgcodigo, sum(o.orbvalorparcela) as valor, max(o.orbnumparcela) as parcela,
																			        (select e.exfvalor from emenda.execucaofinanceira e where e.exfid = o.exfid) valortotal,
																			        o.exfid
																			    FROM emenda.ordembancaria o 
																			    WHERE 
																			        o.orbdatainclusao = (SELECT max(orbdatainclusao) FROM emenda.ordembancaria WHERE exfid = o.exfid)
																			group by o.spgcodigo, o.exfid, o.orbnumparcela
																) as foo
																	where
																		exfid = {$valor['exfid']}" );
								
								if( $arrParcelaPaga['valor'] == $arrParcelaPaga['valortotal'] ){
									$spgcodigoAtual = $arrParcelaPaga['spgcodigo'];
								}
								$valorTotalParcela = $arrParcelaPaga['valor'];
								$totalParcela = $arrParcelaPaga['parcela'];
								
								#desabilita os campos e os botões caso ja tenha sido pago
								$disabilitaEstado = '';
								if($spgcodigoAtual == 2 ){
									$disabilitaEstado = 'disabled="disabled"';
								}*/
								//ver($valor['exfid'], $totalParcela, $spgcodigoAtual, $valorTotalParcela, $valor['exfvalor']);
								//if( ($totalParcela == 0 || $spgcodigoAtual == 9) || ((float)$valorTotalParcela < (float)$valor['exfvalor']) ){
									$valorDisponivel = ($valorTotalParcela) ? $valor['exfvalor'] - $valorTotalParcela : $valor['exfvalor'];
									//$valorDisponivel = ($valorTotalParcela) ? $valorTotalParcela : $valor['exfvalor'];
								?>
								<tr align="center" id="linha_2" style="background: none repeat scroll 0% 0% rgb(245, 245, 245);">
									<td><input type="text" <?=$disabilitaEstado; ?> name="numparcela[<?php echo $valor['exfid']; ?>][]" id="numparcela[<?php echo $valor['exfid']; ?>]" value="<?=$totalParcela+1; ?>" size="2"></td>
									<td>
										<input type="hidden" name="exfid[<?php echo $valor['exfid']; ?>][]" id="exfid[1]" value="<?php echo $valor['exfid']; ?>">
										<input type="hidden" name="orbid[<?php echo $valor['exfid']; ?>][]" id="orbid[1]" value="">
										<input type="text" <?=$disabilitaEstado; ?> name="orbmesparcela[<?php echo $valor['exfid']; ?>][]" id="orbmesparcela[<?php echo $valor['exfid']; ?>]" value="" maxlength="2" size="10" onkeyup="this.value=mascaraglobal('[#]',this.value);" 
											onblur="this.value=mascaraglobal('[#]',this.value); validaMesParcela(this,'<?php echo $valor['exfid']; ?>');">
											<img src="../imagens/obrig.gif" title="indica campo obrigatório."></td>
									<td><input type="text" <?=$disabilitaEstado; ?> name="orbanoparcela[<?php echo $valor['exfid']; ?>][]" id="orbanoparcela[<?php echo $valor['exfid']; ?>]" value="" maxlength="4" size="10" 
											onkeyup="this.value=mascaraglobal('[#]',this.value);" onblur="this.value=mascaraglobal('[#]',this.value); validaAnoParcela(this,'<?php echo $valor['exfid']; ?>');">
										<img src="../imagens/obrig.gif" title="indica campo obrigatório."></td>
									<td><input type="text" <?=$disabilitaEstado; ?> name="orbvalorparcela[<?php echo $valor['exfid']; ?>][]" id="orbvalorparcela[<?php echo $valor['exfid']; ?>]" value="<?php echo number_format($valorDisponivel, 2, ',', '.'); ?>" maxlength="13" size="15" 
											onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);" 
											onblur="this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenOrbvalorparcela(this.value,'<?php echo $valor['exfid']; ?>', 1);">
										<img src="../imagens/obrig.gif" title="indica campo obrigatório.">
										<input type="hidden" name="valor1_[<?php echo $valor['exfid']; ?>]_1" id="valor1_[<?php echo $valor['exfid']; ?>]_1" value="<?php echo $valor['exfvalor']; ?>"></td>
									<td><? 
										if ( empty($boDisabled) && $boValorIgual && $spgcodigoAtual <> 2 && !$arrPendencia[$valor['exfid']] && $obHabilita->consultaHabilitaEntidade($cnpjHabilita, true) == 3 ) {
											echo '<input type="button" value="Solicitar" '.verificaPermissaoPerfil('analise', '', $estadoAtual, true).' name="btnSalvar" id="btnSalvar" onclick="solicitarPagamentoPTA('.$ptrid.', '.$valor['exfid'].');">'; 
										} else {
											echo '<input type="button" value="Solicitar" disabled="disabled" name="btnSalvar" id="btnSalvar">';
										}?></td>
								</tr>
								<?//} ?>
							</table></td>
						</tr>
						<?if( $arrPendencia[$valor['exfid']] ){ ?>
						<tr>
							<td colspan="5">
								<table class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="left">
									<tr>
										<th>Pendências</th>
									</tr>
									<tr>
										<td><?=implode( '<br>', $arrPendencia[$valor['exfid']] ); ?></td>
									</tr>
								</table>
							</td>
						</tr>
						<?}
						} else {?>
						<input type="hidden" name="valorigual_<?=$valor['exfid']; ?>" id="valorigual_<?=$valor['exfid']; ?>" value="<?=$boValorIgual; ?>">
						<table width="40%" bgcolor="#f5f5f5" id="tabelaParcela_<?php echo $valor['exfid']; ?>" cellspacing="1" cellpadding="2">
								<tr>
									<tbody id="tbodyTabela>">
								</tr>
						<tbody id="tbodyTabela_<?php echo $valor['exfid']; ?>">
						<?} ?>
					</table>
				</td>
			</tr>
			<?					
	}	// fim do foreach $arDadosX
	?>
		</table>
		</td>
		<td align="right" rowspan="3">
			<?php 
			$docid = criarDocumento( $ptrid );
			wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'tipo' => "" ) );
			?>
		</td>
		
	</tr>
</table>
</form>	
<?
} else {// fim do if?>
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td align="right">
		<?php 
		$docid = criarDocumento( $ptrid );
		wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'tipo' => "" ) );
		?>
	</td>
		</tr>
	</table>
	<?
}?>	
<div id="divDebug"></div>