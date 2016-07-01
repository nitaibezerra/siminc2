<?php 
if(isset($_POST)){
	$tam = count($_POST);
	if($tam > 0){
		
		foreach($_POST as $chave => $valor){
			
			// salvando a justificativa
			if($chave == 'pmajustificativa'){
				$sql = "INSERT 
						INTO par.pfmodalidadeapoioformacao(
						            inuid, 
						            pmaidanoreferencia, 
						            ptmid, 
						            pmatotaldocente, 
						            pmatotalrecurso, 
						            pmajustificativa)
						    VALUES (
						    		{$_SESSION['par']['inuid']}, 
						    		{$_SESSION['exercicio']},
						    		".NENHUMA_MODALIDADE.",
						    		0,
						    		0,
						    		'{$valor}');";
				$db->executar($sql);
			}else{
				
				if( (isset($_POST['qtd_professores_'.$valor])) && (isset($_POST['valor_'.$valor])) ){
					// salvando a Quantidade de Professores e o Valor por Docente
					$sql = "INSERT 
						INTO par.pfmodalidadeapoioformacao(
						            inuid, 
						            pmaidanoreferencia, 
						            ptmid, 
						            pmatotaldocente, 
						            pmatotalrecurso)
						    VALUES (
						    		{$_SESSION['par']['inuid']}, 
						    		{$_SESSION['exercicio']},
						    		{$valor}, 
						    		{$_POST['qtd_professores_'.$valor]},
						    		".str_replace(",",".",$_POST['valor_'.$valor]).");";
						    		
					unset($_POST['qtd_professores_'.$valor]);
					unset($_POST['valor_'.$valor]);
					$db->executar($sql);
					
				}elseif( (isset($_POST['qtd_professores_'.$valor])) && (isset($_POST['porcentagem_'.$valor])) ){
					// salvando a Quantidade de Professores e o Valor por Docente %
					$sql = "INSERT 
						INTO par.pfmodalidadeapoioformacao(
						            inuid, 
						            pmaidanoreferencia, 
						            ptmid, 
						            pmatotaldocente, 
						            pmatotalrecurso)
						    VALUES (
						    		{$_SESSION['par']['inuid']}, 
						    		{$_SESSION['exercicio']},
						    		{$valor}, 
						    		{$_POST['qtd_professores_'.$valor]},
						    		".str_replace(",",".",$_POST['porcentagem_'.$valor]).");";
						    		
					unset($_POST['qtd_professores_'.$valor]);
					unset($_POST['porcentagem_'.$valor]);
					$db->executar($sql);
					
				}else{
					// salvando somente a Quantidade de Professores
					if( $_POST['qtd_professores_'.$valor] ){
						$sql = "INSERT 
							INTO par.pfmodalidadeapoioformacao(
							            inuid, 
							            pmaidanoreferencia, 
							            ptmid, 
							            pmatotaldocente, 
							            pmatotalrecurso)
							    VALUES (
							    		{$_SESSION['par']['inuid']}, 
							    		{$_SESSION['exercicio']},
							    		{$valor}, 
							    		{$_POST['qtd_professores_'.$valor]},
							    		0);";
						$db->executar($sql);
						unset($_POST['qtd_professores_'.$valor]);
					}
				}
			}// fim do if da justificativa
			
		}
		$db->commit();
		echo "<script type='text/javascript'>
				alert('Dados salvos com sucesso.');
				location.href = 'par.php?modulo=principal/planoDeApoio&acao=A';
			  </script>";
		exit();	
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

echo montarAbasArray( criaAbaParApoio(), "par.php?modulo=principal/planoDeApoio&acao=A" );

// cabeçalho semelhante ao utilizado no arquivo principal/planoTrabalho
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo('PARFor ('.$nmEntidadePar.')', '&nbsp;');
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function() {

	// função acionada quando o usuário clica no checkbox
	$("input[type=checkbox]").click(function () {
		if(this.checked){
			// habilitando os campos inputs correspondentes ao checkbox
			$("#qtd_professores_"+this.value).val('');
			$("#valor_"+this.value).val('');
			$("#porcentagem_"+this.value).val('');
			
			$("#qtd_professores_"+this.value).css('background','#FFFFFF');
			$("#valor_"+this.value).css('background','#FFFFFF');
			$("#porcentagem_"+this.value).css('background','#FFFFFF');
			
			$("#qtd_professores_"+this.value).attr('disabled', false);
			$("#valor_"+this.value).attr('disabled', false);
			$("#porcentagem_"+this.value).attr('disabled', false);
		}else{
			// desabilitando os campos inputs correspondentes ao checkbox
			$("#qtd_professores_"+this.value).val('');
			$("#valor_"+this.value).val('');
			$("#porcentagem_"+this.value).val('');
			
			$("#qtd_professores_"+this.value).css('background','#e9e9e9');
			$("#valor_"+this.value).css('background','#e9e9e9');
			$("#porcentagem_"+this.value).css('background','#e9e9e9');
			
			$("#qtd_professores_"+this.value).attr('disabled', true);
			$("#valor_"+this.value).attr('disabled', true);
			$("#porcentagem_"+this.value).attr('disabled', true);
		}
	});
	
	// função acionada quando o usuário marcar o checkbox NENHUM
	$("#ptmid_<?php echo NENHUMA_MODALIDADE; ?>").click(function () {
		
		if(this.checked){
			if(confirm("Atenção! Todos os campos que foram preenchidos por você serão apagados e desabilitados. Deseja realmente fazer isso?")){
				// desmarcando os checkboxes
				$('input[type=checkbox]').each(function () {
				
					$('#'+this.id).attr('checked', false);
					$('#'+this.id).attr('disabled', true);
					
					$("#qtd_professores_"+this.value).css('background','#e9e9e9');
					$("#valor_"+this.value).css('background','#e9e9e9');
					$("#porcentagem_"+this.value).css('background','#e9e9e9');
					
					$("#qtd_professores_"+this.value).val('');
					$("#valor_"+this.value).val('');
					$("#porcentagem_"+this.value).val('');
					
					$("#qtd_professores_"+this.value).attr('disabled', true);
					$("#valor_"+this.value).attr('disabled', true);
					$("#porcentagem_"+this.value).attr('disabled', true);
					
					// habilitando o nenhum
					$('#ptmid_<?php echo NENHUMA_MODALIDADE; ?>').attr('checked', true);
					$('#ptmid_<?php echo NENHUMA_MODALIDADE; ?>').attr('disabled', false);
//					$('#total_<?php echo NENHUMA_MODALIDADE; ?>').attr('disabled', false);
//					$("#qtd_professores_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#FFFFFF');
//					$("#valor_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#FFFFFF');
//					$("#total_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#FFFFFF');
//					$("#qtd_professores_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', false);
//					$("#valor_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', false);
//					$("#total_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', false);
					// exibindo o campo da justificativa
					$('#justificativa').css('display', 'inline');
					
				});
				
			}else{
				// desmarcando o nenhum
				$('#ptmid_<?php echo NENHUMA_MODALIDADE; ?>').attr('checked', false);
				
				$("#qtd_professores_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#e9e9e9');
				$("#valor_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#e9e9e9');
				$("#total_<?php echo NENHUMA_MODALIDADE; ?>").css('background','#e9e9e9');
				
				$("#qtd_professores_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', 'disabled');
				$("#valor_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', 'disabled');
				$("#total_<?php echo NENHUMA_MODALIDADE; ?>").attr('disabled', 'disabled');
				
			}
		}else{
			// se cair aqui significa que o usuário está desmarcando o nenhum, então eu habilito os outros checkboxes e escondo a justificativa
			$('#justificativa').css('display', 'none');
			$('input[type=checkbox]').each(function () {
				$('#'+this.id).attr('disabled', false);
			});
		}

	});
	
	// botão salvar
	$("#salvar").click(function () {
		var cont = 0; // variável que conta a quantidade de itens marcados
		var erro = 0; // variável que conta a quantidade de erros 
		
		// verificando se o checkbox está marcado
		$('input[type=checkbox]').each(function () {
	        if(this.checked) {
	        	cont++;
	        	
	        	// verificando se os inputs estão preenchidos corretamente
	        	if($("#qtd_professores_"+this.value).val() == ''){
	        		alert('Preencha a quantidadade de Professores corretamente.');
	        		$("#qtd_professores_"+this.value).focus();
	        		erro++;
	        		return false;
	        	}else if($("#valor_"+this.value).val() == ''){
	        		alert('Preencha o Valor por Docente corretamente.');
	        		$("#valor_"+this.value).focus();
	        		erro++;
	        		return false;
	        	}else if($("#porcentagem_"+this.value).val() == ''){
	        		alert('Preencha a Porcentagem corretamente.');
	        		$("#porcentagem_"+this.value).focus();
	        		erro++;
	        		return false;
	        	}else if( $("#ptmid_7").is(':checked') && ($("#pmajustificativa").val() == '') ){
	        		//validando a justificativa
	        		alert('Preencha a Justificativa corretamente.');
	        		$("#pmajustificativa").focus();
	        		erro++;
	        		return false;
	        	}

			}
    	});
    	// se cont estiver vazio, significa que o usuário não marcou nenhuma opção
    	if(!cont){
			alert('Por favor, selecione uma das opções.');
			return false;
		}else if(!erro){
			// se o usuário preencheu a justificativa, então eu envio o formulário com a justificativa
			if( $("#pmajustificativa").val() != '' ){
				document.frm.submit();
			}else{
			// senão eu envio o dos checkboxes
				document.teste.submit();
			}

		}
	});
	
	// função que calcula o total, de acordo com a quantidade de professores e o Valor por Docente
	$("[id^='valor_']").blur(function () {
		var n = this.id.replace("valor_","");
		var qtd_prof = $("#qtd_professores_"+n).val();
		
		if(this.value && qtd_prof){
			var total = String( qtd_prof * (this.value.replace(',','.')) );
			total = total.replace('.',',');
			$("#total_"+n).val(total);
		}
		
	});
	
	// função que calcula o total, de acordo com a quantidade de professores e o Valor por Docente
	$("[id^='qtd_professores_']").blur(function () {
		var n = this.id.replace("qtd_professores_","");
		$('#valor_'+n).trigger('blur');
	});
	
	// função que não permite que o usuário coloque mais que 100% no valor
	$("[id^='porcentagem_']").blur(function () {
		var n = this.value.replace(',','.');
		if(n > 100 || n.length > 6){
			alert('O Valor por Docente não pode ultrapassar 100%.');
			this.value = '';
			$("#"+this.id).focus();
			return false;
		}
	});
});	

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?
$sql = "SELECT * FROM par.censo_indicadores";
$indicadores = $db->carregar($sql);

if($indicadores[0]) {
	
	foreach($indicadores as $ind) {
		
		$sql_da = "SELECT cidvalor FROM par.censo_indicadores_dados WHERE muncod='".$_SESSION['par']['muncod']."' AND inaid='".$ind['inaid']."'";
		$va = $db->pegaUm($sql_da);
		
		if($va) {
			
			echo "<tr>
					<td class=SubTituloDireita>".$ind['inadsc'].":</td>
					<td>".$va."</td>
				  </tr>";
		} else {
			
			$sql_in = str_replace(array("{muncod}"),array($_SESSION['par']['muncod']),$ind['inasql']);
			$valor = $db->pegaUm($sql_in);
			
			$sql_ins = "INSERT INTO par.censo_indicadores_dados(
            			inaid, muncod, cidvalor)
    					VALUES ('".$ind['inaid']."', '".$_SESSION['par']['muncod']."', '".$valor."');";
			$db->executar($sql_ins);
			
			echo "<tr>
					<td class=SubTituloDireita>".$ind['inadsc'].":</td>
					<td>".$valor."</td>
				  </tr>";
		}
	}
	
	$db->commit();
}
?>
</table>


	<?php
	
	$sql = "SELECT 
				'<label>
					<input type=\"checkbox\" name=\"ptmid_'|| ptmid ||'\" id=\"ptmid_'|| ptmid ||'\" value=\"'|| ptmid ||'\" onclick=\"\" />'|| ptmdescricao || 
				'</label>' as modalidade,
		
				CASE WHEN ptmid = ".NENHUMA_MODALIDADE." THEN
					'&nbsp;'
				ELSE
					'<center>
						<input type=\"text\" maxlength=\"6\" style=\"background:#e9e9e9;\" name=\"qtd_professores_'|| ptmid ||'\" id=\"qtd_professores_'|| ptmid ||'\" disabled=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" onblur=\"MouseBlur(this); this.value=mascaraglobal(\'[#]\',this.value)\">
					</center>' 
				END as qtd_professores,
			
				CASE WHEN ptmid = ".NENHUMA_MODALIDADE." THEN
					'&nbsp;'
				ELSE
				
					CASE WHEN ptmtipovalor = 'V' THEN
						'<center>
							<input type=\"text\" style=\"background:#e9e9e9;\" name=\"valor_'|| ptmid ||'\" id=\"valor_'|| ptmid ||'\" disabled=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onblur=\"MouseBlur(this); this.value=mascaraglobal(\'[###.]###,##\',this.value)\">
						</center>'
					ELSE
						CASE WHEN ptmtipovalor = 'P' THEN
							'<center>
								&nbsp;&nbsp;&nbsp;<input type=\"text\" style=\"background:#e9e9e9;\" name=\"porcentagem_'|| ptmid ||'\" id=\"porcentagem_'|| ptmid ||'\" disabled=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onblur=\"MouseBlur(this); this.value=mascaraglobal(\'[###.]###,##\',this.value)\" maxlength=\"6\">%
							</center>'
						ELSE
							'&nbsp;'
						END
					END 
				END as valor,
			
				CASE WHEN ptmid = ".NENHUMA_MODALIDADE." THEN
					'&nbsp;'
				ELSE
				
					CASE WHEN ptmtipovalor = 'V' THEN
						'<center>
			
							<input type=\"text\" style=\"background:#e9e9e9;\" name=\"total_'|| ptmid ||'\" id=\"total_'|| ptmid ||'\" disabled=\"disabled\">
						</center>'
					ELSE
						'&nbsp;'
					END 
				END as total
			FROM par.pftipomodalidadeapoio
			WHERE
				ptmstatus = 'A'
			ORDER BY ptmordem;";
	
	$cabecalho = array("Plano de Apoio", "Quantidade de Professores", "Valor por Docente", "Total");
	
//	$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '', 'teste');
	$db->monta_lista_simples($sql, $cabecalho, 50, 20);
	
	?>
<form name="frm" id="frm" action="par.php?modulo=principal/planoDeApoio&acao=A" method="post">	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr id="justificativa" style="display: none;">
			<td>Justificativa</td>
			<td><?php  echo campo_textarea( 'pmajustificativa', 'N', 'S', '', '120', '10', '1000', '' , 0, '', false, NULL, ''); ?></td>
		</tr>
		<tr>
			<td class='SubTituloEsquerda' colspan='6'>
				<?php if( possuiPerfil( array(PAR_PERFIL_EQUIPE_MUNICIPAL,PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, PAR_PERFIL_PREFEITO)) ){ ?>
				<input type="button" value='Salvar' id="salvar">
				<?php }else{ ?>
				<input type="button" value='Salvar' disabled="disabled">
				<?php } ?>
			</td>	
		</tr>
	</table>
</form>