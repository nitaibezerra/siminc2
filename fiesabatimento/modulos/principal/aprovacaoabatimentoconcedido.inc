<?
if($_REQUEST['requisicao'])
{
	$_REQUEST['requisicao']();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

if($_GET['atpid']){
	$arrDadosProfessor = recuperaDadosProfessor($_GET['atpid']);
	extract($arrDadosProfessor);

	if($arrDadosProfessor['sbaid']) {
		$sbarenovacao = $db->pegaLinha("SELECT sbarenovacao, sbaanoinicio, sbaanofim  FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = " . $arrDadosProfessor['sbaid']);
		if ($sbarenovacao['sbarenovacao'] == 't') {
			//$preidDescricao = $db->pegaUm("SELECT 'Renovação - ' || peremesinicioref || '/' || pereanoinicioref || ' à ' || peremesfimref || '/' || pereanofimref as descricao FROM fiesabatimento.parametrosrenovacao WHERE preid = ".$preid);
			$preidDescricao = "Renovação - {$sbarenovacao['sbaanoinicio']} a {$sbarenovacao['sbaanofim']}";
		} else {
			$preidDescricao = "Solicitação - {$sbarenovacao['sbaanoinicio']} a {$sbarenovacao['sbaanofim']}";
		}
	}
}

$arrMeses = recuperaMesesFIES();

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
	<?php if($_SESSION['fiesabatimento_var']['alert']): ?>
	jQuery(function() {
		alert('<?php echo $_SESSION['fiesabatimento_var']['alert'] ?>');
		<?php unset($_SESSION['fiesabatimento_var']['alert']) ?>
	});
	<?php endif; ?>
	
	function confirmarMesAno(obj,cod_inep,ano)
	{
		var td = obj.parentNode;
		if(obj.checked == true){
			td.bgColor = "#0E92D8";
			jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='todos']").attr("checked",false);
			jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='manual']").attr("checked",true);
		}else{
			td.bgColor = "#80BC44";
			jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='todos']").attr("checked",false);
			jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='manual']").attr("checked",true);
		}
	}
	
	function confirmarTodos(cod_inep,ano)
	{
		jQuery.each( jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano + "]']") , function(k, obj){
			 obj.parentNode.bgColor = "#0E92D8";
			 jQuery(obj).attr("checked",true);
		 });
	}
	
	function aprovarSolicitacao()
	{
		jQuery("#form_secretario").submit();
	}
	
	var totalProfessor = 0;
	var totalDiretor = 0;
</script>
<form name="form_secretario" id="form_secretario" action="" method="post" >
	<input type="hidden" name="requisicao" value="confirmarSolicitacaoSecretario" />
	<input type="hidden" name="atpid" value="<?php echo $_GET['atpid'] ?>" />
	<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Orientações :</td>
			<td>As informações do professor solicitante do abatimento (dados pessoais, atuação profissional e quantidade de meses trabalhados), confirmadas pelo Diretor de Escola, estão descritas abaixo. A Aprovação (total ou parcial), Reabertura ou Rejeição deverá ser realizada pelo Secretário ou Diretor de Escola Federal, conforme o caso.</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Dados pessoais</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF :</td>
			<td><?php echo mascaraglobal($idocpf,"###.###.###-##") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome :</td>
			<td><?php echo $idonome ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CEP :</td>
			<td><?php echo mascaraglobal($atpinep,"#####-###") ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Endereço :</td>
			<td><?php echo $idoendereco ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número :</td>
			<td><?php echo $idonumero ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento :</td>
			<td><?php echo $idocomplemento ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro :</td>
			<td><?php echo $idobairro ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF :</td>
			<td><?php echo $estuf ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Munícipio :</td>
			<td><?php echo $mundescricao ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone :</td>
			<td><?php echo $idotelefone ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Email :</td>
			<td><?php echo $idoeemail ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Atuação Profissional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo :</td>
			<td><?=$preidDescricao;?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Atuação Profissional :</td>
			<td><?// $arrDadosEscola = htmlAtuacaoUsuario($idocpf) ?>
				<? $arrDadosEscola = htmlAtuacaoSolicitacao($sbaid, true, false, $preid); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Total de meses trabalhados(initerruptos) :</td>
			<td>
				<p>Comprovados pelo Professor / solicitante: <span style="font-weight:bold" id="total_meses_comprovado_professor" ></span></p>
				<p>Comprovados pelo Diretor da Escola: <span style="font-weight:bold" id="total_meses_comprovado_diretor" ></span></p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Aprovação dos meses trabalhados pelo professor :</td>
			<td>
			<p>1) Observar os seguintes procedimentos para a aprovação da quantidade de meses/ano trabalhados pelo professor:</p>
			<p>a) Para aprovação total de determinado ano (aprovar todos os meses), selecionar o campo "aprovar todos os meses     " referente ao ano desejado;</p>
			<p>b) Para aprovação parcial de determinado ano (aprovar alguns meses), selecionar o campo "aprovação manual       " referente ao ano desejado;</p>
			<p>c) Para rejeição total de determinado ano (rejeitar todos os meses), selecionar o campo "aprovação manual       " referente ao ano desejado, não selecionar nenhum dos meses e acionar a opção "Aprovar" e em seguida confirmar rejeição total.</p>
			
			<?php 
			//dbg($arrDadosEscola);
			if($arrDadosEscola): ?>
					<?php foreach($arrDadosEscola as $escola): ?>
						<?php 
								//$arrConfirmacaoDiretor    = retornaConfirmacaoEscola($escola['co_inep']);
								$arrConfirmacaoDiretor    = retornaConfirmacaoDiretor($escola['atpid']); ?>
						<?php $arrConfirmacaoSecretario = retornaConfirmacaoSecretario($_GET['atpid']) ?>
						<fieldset style="margin-bottom:15px">
							<legend><?php echo $escola['no_entidade'] ?></legend>
							<table align="center" class="listagem" width="100%" cellpadding="1" cellspacing="1" >
								<tr bgcolor="#f9f9f9" >
									<td class="SubTituloCentro" rowspan="2" >Ano</td>
									<td class="SubTituloCentro" colspan="2" >Qtde. Meses</td>
									<td class="SubTituloCentro" colspan="2">Confirmação</td>
									<?php foreach($arrMeses as $mes): ?>
										<td width="60px"  class="SubTituloCentro" rowspan="2" ><?php echo $mes['descricao'] ?></td>
									<?php endforeach; ?>
								</tr>
								<tr>
									<td class="SubTituloCentro" >Declarados pelo Professor</td>
									<td class="SubTituloCentro" >Confirmados pelo Diretor da Escola</td>
									<td class="SubTituloCentro" >Confirmar todos os meses</td>
									<td class="SubTituloCentro" >Confirmação manual</td>
								</tr>
								<?php $arrAnosEscola = recupaAnosEscola($escola) ?>
								<?php if($arrAnosEscola): ?>
									<?php $n=0;foreach($arrAnosEscola as $ano): ?>
										<?php $cor_tr = $n%2 ==1 ? "#ffffff" : "" ?>
										<tr bgcolor="<?php echo $cor_tr ?>" >
											<td align="center" ><?php echo $ano ?></td>
											<td align="center" id="td_confirmados_professor_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>" >0</td>
											<td align="center" id="td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>" >0</td>
											<td align="center" ><input type="radio" onclick="confirmarTodos('<?php echo $escola['co_inep'] ?>','<?php echo $ano ?>')" name="rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]" value="todos" /></td>
											<td align="center" ><input type="radio" name="rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]" value="manual" /></td>
											<?php 
													if($preid){
														$DT_INICIO_PROGRAMA = '2013-01-01';
														$DT_FIM_PROGRAMA = '2013-12-31';
													}
													else{
														$DT_INICIO_PROGRAMA = '2010-01-01';
														$DT_FIM_PROGRAMA = '2012-12-31';
													}
													
													$testeDtInicio = explode('-', $DT_INICIO_PROGRAMA);
													$testeDtInicio = $testeDtInicio[0].$testeDtInicio[1];
													
													$testeDtFim = explode('-', $DT_FIM_PROGRAMA);
													$testeDtFim = $testeDtFim[0].$testeDtFim[1];
													
											?>
											<?php foreach($arrMeses as $mes): ?>
												<?php 
													//dbg($arrConfirmacaoDiretor[$ano][$mes['codigo']]);
													if($arrConfirmacaoDiretor[$ano][$mes['codigo']]){
														$cor = "#80BC44";
														$total_confirmados++;
														$verde = true;
													}else{
														$cor = "#FFC211";
														$verde = false;
													}
													
													if($arrConfirmacaoSecretario[$ano][$mes['codigo']]){
														$cor = "#0E92D8";
														$checked = "checked='checked'";
													}else{
														$checked = "";
													}
												
													$mescod = strlen($mes['codigo']) == 1 ? "0".$mes['codigo'] : $mes['codigo'];
													$dt_mescod = $ano.$mescod;
													
													$arrI = explode("-",$escola['dt_inicio']);
													$dt_ini = $arrI[0].$arrI[1];
													//$dt_ini = !$dt_ini ? date("Ym") : $dt_ini;
													$dt_ini = !$dt_ini ? $testeDtInicio : $dt_ini;
													
													$arrF = explode("-",$escola['dt_fim']);
													$dt_fim = $arrF[0].$arrF[1];
													$dt_fim = !$dt_fim ? $testeDtFim : ($dt_fim > 201212 ? $testeDtFim : $dt_fim);
													$permissaoCheckbox = ($dt_mescod >= $dt_ini && $dt_mescod <= $dt_fim) ? true : false; 
												?>
												<td align="center" <?php echo $permissaoCheckbox ? "bgcolor=\"$cor\"" : "" ?> >
													<?php if($permissaoCheckbox && $verde): ?>
														<input type="checkbox" <?php echo $checked ?> onclick="confirmarMesAno(this,'<?php echo $escola['co_inep'] ?>','<?php echo $ano ?>')" name="rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>][<?php echo $mes['codigo'] ?>]" value="manual" />
													<?php elseif($permissaoCheckbox): ?>
														<input type="hidden" <?php echo $checked ?> name="hdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>][<?php echo $mes['codigo'] ?>]" />
													<?php else: ?>
															-
													<?php endif; ?>
												</td>
											<?php endforeach; ?>
										</tr>
										<script>
											jQuery("#td_confirmados_professor_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html(jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length+jQuery("[name^='hdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length);
											totalProfessor+= jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length+jQuery("[name^='hdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length;
											jQuery("#td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html(jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length);
											totalDiretor+=jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length; 
											if( jQuery("#td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html() == jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']:checked").length ){
												jQuery("[name='rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]'][value='todos']").attr("checked",true);
											}else{
												jQuery("[name='rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]'][value='manual']").attr("checked",true);
											}
										</script>
									<?php $n++;endforeach; ?>
								<?php endif; ?>
							</table>
						</fieldset>
						<?php endforeach; ?>
						
						<p><b>Legenda:</b></p>
						<table border="0" cellpadding="1" >
						<tr>
							<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FFC211"></div></td>
							<td><b>Declarado pelo Professor</b></td>
						</tr>
						<tr>
							<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#80BC44"></div></td>
							<td><b>Confirmado pelo Diretor de Escola</b></td>
						</tr>
						<tr>
							<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#0E92D8"></div></td>
							<td><b>Aprovado pelo Secretário da Escola</b></td>
						</tr>
					</table>
				<?php endif; ?>
			
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Aprovação, Reabertura ou Rejeição da solicitação :</td>
			<td>
			<p>As condições abaixo são necessárias para solicitação da concessão do abatimento. Os registros na solicitação do abatimento indicam que todas as condições foram atendidas pelo professor. Selecione os itens abaixo para ratificar o atendimento (a não seleção de qualquer um dos itens, implicará na rejeição da solicitação do abatimento) e finalizar a aprovação da solicitação de abatimento, acionando a opção "Aprovar".</p>
			<p>
			<input type="checkbox"> Período do exercício da docência (igual ou superior a 12 meses ininterruptos)<br>
			<input type="checkbox"> Carga horária semanal igual ou superior a 20 horas semanais<br>
			<input type="checkbox"> Vinculo empregatício temporário ou efetivo<br>
			<input type="checkbox"> Função de Docente<br>
			<input type="checkbox"> Vínculo empregatício com esta Secretaria/Diretoria<br>
			</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="voltar" onclick="window.location.href='fiesabatimento.php?modulo=principal/listasolicitacaoabatimentoconcedida&acao=A'" value="Voltar">
				<input type="button" name="reabrir" onclick="rejeitarSolicitacao()" value="Reabrir">
				<input type="button" name="aprovar" onclick="aprovarSolicitacao()" value="Aprovar"></td>
		</tr>
	</table>
</form>
<script>
	//jQuery("#total_meses_comprovado_professor").html(totalProfessor)
	jQuery("#total_meses_comprovado_professor").html(<?=$sbaqmtsolicitado?>);
	jQuery("#total_meses_comprovado_diretor").html(totalDiretor);
</script>