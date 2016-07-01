<?php
$estadoAtualSubacao = wf_pegarEstadoAtual ( $subacao ['docid'] );

cabecalho();

unset ( $docid );
unset ( $esdid );
if ($preid) {
	$obSubacaoControle = new SubacaoControle ();
	$obPreObra = new PreObra();
	$arDados = $obSubacaoControle->recuperarPreObra( $preid );
	$docid = prePegarDocid( $preid );
	$esdid = prePegarEstadoAtual( $docid );
}

$arPerfil = pegaArrayPerfil($_SESSION['usucpf']);

$cnpjEntidade = pegaCnpjInuid( $_SESSION ['par'] ['inuid'] );

$arrObra = $db->pegaLinha("select cast(prevalorobra as numeric(20,2)) as vrlobra, 
							coalesce(prevalorsuplementar, 0) as prevalorsuplementar,
							coalesce(prevalorcomplementarfnde, 0) as prevalorcomplementarfnde 
						from obras.preobra where preid = $preid");
						
$vrlobra 					= $arrObra['vrlobra'];
$prevalorsuplementar 		= $arrObra['prevalorsuplementar'];
$prevalorcomplementarfnde 	= $arrObra['prevalorcomplementarfnde'];

$vrEmpenho = $db->pegaUm("select saldo from par.vm_saldo_empenho_por_obra where preid = $preid");

$sql = "SELECT coalesce(sum(obpvaloremenda), 0) FROM par.obraemenda WHERE preid = $preid and obpstatus = 'A'";
$totObpvaloremenda = $db->pegaUm($sql);
$vrlDisponivel = (float)$vrlobra - ((float)$totObpvaloremenda + (float)$prevalorsuplementar + (float)$prevalorcomplementarfnde); 

?>
<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" /> 
	<input type="hidden" name="ano" value="<?php echo $ano?>" /> 
	<input type="hidden" name="preid" value="<?php echo $preid?>" />
	<input type="hidden" name="cnpjEntidade" value="<?php echo $cnpjEntidade?>" />

	<table class="tabela" style="margin-bottom: 15px" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<th class="SubTituloDireita" colspan="2" style="text-align: center;">Distribuição do Recurso Orçamentário da Emenda</th>
		</tr>
		<tr>
			<td colspan="2">
				<table class="tabela" style="margin-bottom: 15px" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<tr>
							<td class="subtitulodireita" width="45%">Valor da Obra (Planilha Orçamentária):</td>
							<td><input type="text" id="vrlobra" name="vrlobra" class="clsMouseFocus" size="15" maxlength="20" 
								onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" disabled="disabled" onmouseover="MouseOver(this);" value="<?php echo simec_number_format($vrlobra, 2, ',', '.');?>"></td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="45%">Valor Empenhado da Obra:</td>
							<td><input type="text" id="vrlempenhoobra" name="vrlempenhoobra" class="clsMouseFocus" size="15" maxlength="20" 
								onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" disabled="disabled" onmouseover="MouseOver(this);" value="<?php echo simec_number_format($vrEmpenho, 2, ',', '.');?>">
								</td>
						</tr>
						<tr>
							<td class="subtitulodireita">Valor da(s) Emenda(s) para a Obra:</td>
							<td><input type="text" id="totObpvaloremenda" name="totObpvaloremenda" class="clsMouseFocus" size="15" maxlength="20"  value="<?php echo simec_number_format($totObpvaloremenda, 2, ',', '.'); ?>"
								onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" disabled="disabled" onmouseover="MouseOver(this);"></td>
						</tr>
						<tr>
							<td width="35%" class="subtitulodireita">Valor Suplementar de <?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
																				echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);?>:</td>
							<td><input type="text" id="prevalorsuplementar" name="prevalorsuplementar" class="clsMouseFocus" size="15" maxlength="20" value="<?php echo simec_number_format($prevalorsuplementar, 2, ',', '.'); ?>"  
								onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" onblur="calculaTotalRestante(this);"></td>
						</tr>
						<tr>
							<td class="subtitulodireita">Valor Complementar do FNDE:</td>
							<td><input type="text" id="prevalorcomplementarfnde" name="prevalorcomplementarfnde" class="clsMouseFocus" size="15" maxlength="20"  value="<?php echo simec_number_format($prevalorcomplementarfnde, 2, ',', '.'); ?>"
								onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" onblur="calculaTotalRestante(this);"></td>
						</tr>
					</tr>
					<tr>
						<td class="subtitulodireita">Valor Restante a Distribuir:</td>
						<td><input type="text" id="vrlrestante" name="vrlrestante" class="clsMouseFocus" disabled="disabled" size="15" maxlength="20"  
							onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" value="<?php  echo simec_number_format($vrlDisponivel, 2, ',', '.'); ?>"></td>
					</tr>
					<tr bgcolor="#C0C0C0" style="text-align: center;">
						<td colspan="2"><input type="button" class="botao" name="btalterar" value="Salvar" onclick="salvarDistribuicaoObra();"></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td><?php
				if( !in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arPerfil) && !in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arPerfil) ){
					$img = "'<center><img src=\"/imagens/alterar.gif\" title=\"Subacao\" onclick=\"exibeDetalharRecurso('||vede.emdid||', '||vede.emeid||', '||coalesce(oe.obpid, '0')||');\"></center>', ";
				} 
				$sql = "SELECT distinct
						    $img
						    vede.emecod,
						    vede.gndcod||' - '||g.gnddsc as gnd,    
						    a.autnome,
						    vede.edevalor,
						    coalesce(oe.obpvaloremenda,0) as obpvaloremenda
						FROM par.subacaoemendapta sep
						    inner join emenda.v_emendadetalheentidade vede on vede.emdid = sep.emdid
						    inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
						    inner join public.gnd as g on g.gndcod = vede.gndcod and g.gndstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
						    inner join emenda.autor a on a.autid = vede.autid
						    left join par.obraemenda oe on oe.emdid = vede.emdid and oe.preid = $preid and oe.obpstatus = 'A'
						WHERE 
						    sd.sbaid = $sbaid
						    and vede.emeano = '$ano'
						    and sep.sepstatus = 'A'
						    and vede.edestatus = 'A'
						    and vede.emetipo = 'E'
						    and enb.enbcnpj = '".$cnpjEntidade."'
						group by 
						    vede.emdid,
						    vede.gndcod,
						    vede.emecod,
						    a.autnome,
						    vede.edevalor,
						    vede.emeid,
						    g.gnddsc,
						    oe.obpvaloremenda,
							oe.obpid";
				
				$cabecalho = array("Ação","Emenda","GND","Autor","Valor Entidade(R$)", "Valor da Emenda para a Obra");
				$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','S', true, false, false, true);
			?></td>
		</tr>
	</table>
</form>

<div id="dialog_acoes" title="Detalhar Distribuição do Recurso" style="display: ''" >
	<div style="padding:5px;text-align:justify;" id="mostraRetorno"></div>
</div>

<div id="debug"></div>
<script>

function carregarListaObras(sbaid, obj) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregarListaObras&sbaid="+sbaid,
	   		async: false,
	   		success: function(msg){
	   		ncol.innerHTML="<div id='listaempenhosbaid_" + sbaid + "' >" + msg + "</div>";
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function exibeDetalharRecurso( emdid, emeid, obpid ){
	var dados = jQuery('#formulario').serialize();
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=detalharRecursoEmenda&emdid="+emdid+"&emeid="+emeid+"&obpid="+obpid+'&'+dados,
		async: false,
		success: function(msg){
			jQuery( "#dialog_acoes" ).show();
			jQuery( "#mostraRetorno" ).html(msg);
			jQuery( '#dialog_acoes' ).dialog({
				resizable: false,
				width: 1000,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Salvar': function() {
						salvarDetalharRecurso( emdid, emeid );
						//jQuery( this ).dialog( 'close' );
					},
					'Cancelar': function() {
						jQuery( this ).dialog( 'close' );
					}
				}
			});
		}
	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
}

function calculaTotalRestanteEmenda(){
	var obpvaloremenda		= jQuery('[name="obpvaloremenda"]').val();
	var vrldisponivelemenda = jQuery('[name="vrldisponivelemenda"]').val();

	if( parseFloat(obpvaloremenda) > 0 ){
		obpvaloremenda = obpvaloremenda.replace(/\./gi,"");
		obpvaloremenda = obpvaloremenda.replace(/\,/gi,".");
	} else {
		obpvaloremenda = 0;
	}
	
	if( parseFloat(obpvaloremenda) > parseFloat(vrldisponivelemenda) ){
		alert('Valor da Emenda não pode ultrapassar o valor disponível.');
		jQuery('[name="obpvaloremenda"]').val( number_format(vrldisponivelemenda, 2, ',', '.' ) );		
		return false;
	}
}

function calculaTotalRestante( input ){
	var totObpvaloremenda		 = jQuery('[name="totObpvaloremenda"]').val();
	var prevalorsuplementar 	 = jQuery('[name="prevalorsuplementar"]').val();
	var prevalorcomplementarfnde = jQuery('[name="prevalorcomplementarfnde"]').val();
	var vrlobra 				 = jQuery('[name="vrlobra"]').val();
	var vrldisponivel 			 = jQuery('[name="vrldisponivel"]').val();
	
	if( parseFloat(prevalorsuplementar) > 0 ){
		prevalorsuplementar = prevalorsuplementar.replace(/\./gi,"");
		prevalorsuplementar = prevalorsuplementar.replace(/\,/gi,".");
	} else {
		prevalorsuplementar = 0;
	}

	if( parseFloat(totObpvaloremenda) > 0 ){
		totObpvaloremenda = totObpvaloremenda.replace(/\./gi,"");
		totObpvaloremenda = totObpvaloremenda.replace(/\,/gi,".");
	} else {
		totObpvaloremenda = 0;
	}
	if( parseFloat(prevalorcomplementarfnde) > 0 ){
		prevalorcomplementarfnde = prevalorcomplementarfnde.replace(/\./gi,"");
		prevalorcomplementarfnde = prevalorcomplementarfnde.replace(/\,/gi,".");
	} else {
		prevalorcomplementarfnde = 0;
	}
		
	vrlobra = vrlobra.replace(/\./gi,"");
	vrlobra = vrlobra.replace(/\,/gi,".");

	if( parseFloat(totObpvaloremenda) > parseFloat(vrlobra) ){
		alert('Valor da Emenda não pode ultrapassar o valor disponível.');
		jQuery('[name="totObpvaloremenda"]').val( number_format(vrlobra, 2, ',', '.' ) );
		calculaTotalRestante( input );
		return false;
	}

	var totalInformado = (parseFloat(prevalorsuplementar) + parseFloat(totObpvaloremenda) + parseFloat(prevalorcomplementarfnde));
	totalInformado = number_format(totalInformado, 2, ',', '.' );

	totalInformado = totalInformado.replace(/\./gi,"");
	totalInformado = totalInformado.replace(/\,/gi,".");

	if( parseFloat(totalInformado) > parseFloat(vrlobra)){
		alert('A soma dos valores informado ultrapassar o valor da obra.');		
		jQuery('[name="'+input.name+'"]').val( number_format('0', 2, ',', '.' ) );
		calculaTotalRestante( input );
		return false;
	}
	
	totalInformado = parseFloat(vrlobra) - (parseFloat(prevalorsuplementar) + parseFloat(totObpvaloremenda) + parseFloat(prevalorcomplementarfnde));

	jQuery('[name="vrlrestante"]').val( number_format(totalInformado, 2, ',', '.' ) );
}

function salvarDistribuicaoObra(){
	var totObpvaloremenda 		 = jQuery('[name="totObpvaloremenda"]').val();
	var prevalorsuplementar 	 = jQuery('[name="prevalorsuplementar"]').val();
	var prevalorcomplementarfnde = jQuery('[name="prevalorcomplementarfnde"]').val();
	var vrlobra 				 = jQuery('[name="vrlobra"]').val();
	var vrlempenhoobra 			 = jQuery('[name="vrlempenhoobra"]').val();
	var preid 					 = jQuery('[name="preid"]').val();

	if( parseFloat(totObpvaloremenda) > 0 ){
		totObpvaloremenda = totObpvaloremenda.replace(/\./gi,"");
		totObpvaloremenda = totObpvaloremenda.replace(/\,/gi,".");
	} else {
		totObpvaloremenda = 0;
	}
	
	if( parseFloat(vrlempenhoobra) > 0 ){
		vrlempenhoobra = vrlempenhoobra.replace(/\./gi,"");
		vrlempenhoobra = vrlempenhoobra.replace(/\,/gi,".");
	} else {
		vrlempenhoobra = 0;
	}

	if( parseFloat(prevalorcomplementarfnde) > 0 ){
		prevalorcomplementarfnde = prevalorcomplementarfnde.replace(/\./gi,"");
		prevalorcomplementarfnde = prevalorcomplementarfnde.replace(/\,/gi,".");
	} else {
		prevalorcomplementarfnde = 0;
	}

	if( parseFloat(prevalorsuplementar) > 0 ){
		prevalorsuplementar = prevalorsuplementar.replace(/\./gi,"");
		prevalorsuplementar = prevalorsuplementar.replace(/\,/gi,".");
	} else {
		prevalorsuplementar = 0;
	}
		
	vrlobra = vrlobra.replace(/\./gi,"");
	vrlobra = vrlobra.replace(/\,/gi,".");

	var totalInformado = (parseFloat(prevalorsuplementar) + parseFloat(totObpvaloremenda) + parseFloat(prevalorcomplementarfnde));
	totalInformado = number_format(totalInformado, 2, ',', '.' );

	totalInformado = totalInformado.replace(/\./gi,"");
	totalInformado = totalInformado.replace(/\,/gi,".");
	
	if( parseFloat(totalInformado) != parseFloat(vrlobra)){
		alert('A soma dos valores informado está diferente do valor da obra.');
		return false;
	}
	
	totalInformado = parseFloat(vrlobra) - (parseFloat(prevalorsuplementar) + parseFloat(totObpvaloremenda) + parseFloat(prevalorcomplementarfnde));

	jQuery('[name="vrlrestante"]').val( number_format(totalInformado, 2, ',', '.' ) );

	var totalEmendaFnde = (parseFloat(totObpvaloremenda) + parseFloat(prevalorcomplementarfnde));
	totalEmendaFnde = number_format(totalEmendaFnde, 2, ',', '.' );

	totalEmendaFnde = totalEmendaFnde.replace(/\./gi,"");
	totalEmendaFnde = totalEmendaFnde.replace(/\,/gi,".");
	
	if( parseFloat(totalEmendaFnde) < parseFloat(vrlempenhoobra)){
		alert('A soma dos valores(Valor da Emenda para a Obra + Valor Complementar do FNDE) não pode ser MENOR que o Valor Empenhado da Obra.');
		return false;
	}
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=salvarDistribuicaoObra&preid="+preid+"&prevalorsuplementar="+prevalorsuplementar+"&prevalorcomplementarfnde="+prevalorcomplementarfnde,
		async: false,
		success: function(msg){
			jQuery('#debug').html(msg);
			if(msg == 1){
				alert('Operação Realizada com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
			window.location.href = window.location;
		}
	});
}
function salvarDetalharRecurso( emdid, emeid ){
	var dados = jQuery('#formulario').serialize();

	var obpvaloremenda		= jQuery('[name="obpvaloremenda"]').val();
	var obpid 				= jQuery('[name="obpid"]').val();

	if( parseFloat(obpvaloremenda) > 0 ){
		obpvaloremenda = obpvaloremenda.replace(/\./gi,"");
		obpvaloremenda = obpvaloremenda.replace(/\,/gi,".");
	} else {
		obpvaloremenda = 0;
	}

	calculaTotalRestanteEmenda();

	if( obpvaloremenda == '' || obpvaloremenda < '1' ){
		alert('Valor da Emenda para a Obra não pode ser vazio.');
		jQuery('[name="obpvaloremenda"]').focus();
		return false;
	}
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=salvarDetalharRecurso&emdid="+emdid+"&emeid="+emeid+"&obpid="+obpid+"&obpvaloremenda="+jQuery('[name="obpvaloremenda"]').val()+"&"+dados,
		async: false,
		success: function(msg){
			jQuery('#debug').html(msg);
			if(msg == 1){
				alert('Operação Realizada com Sucesso!');
			} else {
				alert('Falha na Operação!');
			}
			window.location.href = window.location;
		}
	});
}

</script>
