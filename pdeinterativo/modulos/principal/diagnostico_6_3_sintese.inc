<?
monta_titulo( "Síntese da Dimensão 6 - Infraestrutura", "&nbsp;");
?>
<script type="text/javascript">

	function salvarSinteseInfraestrutura(local)
	{
	
		if(jQuery("[name^='exibeprojeto']:checked").length == 0) {
			alert('Clique se a escola desenvolve algum PROJETO');
			return false;
		}
		
		if(jQuery("[name^='exibeprograma']:checked").length == 0) {
			alert('Clique se a escola desenvolve algum PROGRAMA');
			return false;
		}
		
		if(document.getElementById('exibeprojeto_S').checked) {
			if(document.getElementById('projeto_label').childNodes[1].rows[0].cells.length == 1) {
				alert('Insira um projeto');
				return false;
			}
		}
		
		if(document.getElementById('exibeprograma_S').checked) {
			if(document.getElementById('programa_label').childNodes[1].rows[0].cells.length == 1) {
				alert('Insira um programa');
				return false;
			}
		}

	
		if(jQuery("[name^='chk_problemas[']:checked").length > 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("#form_distorcao_infraestrutura_equipamentos").submit();
		}else{
			alert('Selecione pelo menos 1 problema como crítico!');
		}
	}
	
	function exibePrograma(obj)
	{
		if(obj.value == "Sim"){
			jQuery("#programa_label").show();
		}else{
			jQuery("#programa_label").hide();
		}
	}
	
	function exibeProjeto(obj)
	{
		if(obj.value == "Sim"){
			jQuery("#projeto_label").show();
		}else{
			jQuery("#projeto_label").hide();
		}
	}
	
	function addProjeto()
	{
		janela('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProjetos&sprmodulo=D',500,300,'Projetos');
	}
	
	function addPrograma()
	{
		janela('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProgramas&sprmodulo=D',500,300,'Projetos');
	}
	
	function calculaNumProblemasCriticos()
	{
		var numTotal = jQuery("[name^='chk_problemas[']").length;
		if(numTotal > 0){
			var num = numTotal*0.3;
			num = num.toFixed(1);
			num +='';
			if(num.search(".") >= 0){
				var NewNum = num.split(".");
				if( ((NewNum[1])*1) >= 5 ){
					num = (NewNum[0]*1);
					num = num + 1;
				}else{
					num = (NewNum[0]*1);
				}
			}
			if(num == 0){
				num = 1;
			}
			jQuery("#num_problemas_possiveis").html(num);
		}
	}
	
	function verificaCheckBox(obj)
	{
		
		if(jQuery("[name='" + obj.name + "']").attr("checked") == true)
		{
			var num = jQuery("#num_problemas_possiveis").html();
			var numMarcados = jQuery("[name^='chk_problemas[']:checked").length;
			num = num*1;
			numMarcados = numMarcados*1;
			if(numMarcados > num)
			{
				jQuery("[name='" + obj.name + "']").attr("checked","");
				alert('Selecione no máximo ' + num + ' problema(s) como crítico(s)!');
			}
		}
	}
	
	jQuery(function(){
		calculaNumProblemasCriticos();
		
		<? $existePrograma = existePrograma(array('rppmodulo'=>'F')); ?>
		<? if($existePrograma=="t") : ?>
		document.getElementById('programa_label').style.display='';
		carregarProgramas('F');
		document.getElementById('exibeprograma_S').checked=true;
		<? elseif($existePrograma=="f"): ?>
		document.getElementById('exibeprograma_N').checked=true;
		<? endif; ?>
		
		<? $existeProjeto = existeProjeto(array('rppmodulo'=>'F')); ?>
		<? if($existeProjeto=="t") : ?>
		document.getElementById('projeto_label').style.display='';
		carregarProjetos('F');
		document.getElementById('exibeprojeto_S').checked=true;
		<? elseif($existeProjeto=="f"): ?>
		document.getElementById('exibeprojeto_N').checked=true;
		<? endif; ?>
		
	});	
	
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0"	align="center">
<tr>
	<td class="blue center" width="20%" >Orientações</td>
	<td class="blue" >
		<p>Chegamos à Síntese da Dimensão 6. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
		<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
	</td>
</tr>
</table>
<form name="form_distorcao_infraestrutura_equipamentos" id="form_distorcao_infraestrutura_equipamentos" method="post" >
	<input type="hidden" name="requisicao" value="salvarSinteseInfraestrutura" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita">Problemas Identificados</td>
		<td>
			<table width="100%" class="listagem" cellSpacing="1" cellPadding="3"  >
				<tr>
					<td class="SubTituloCentro bold" width="20%">Tema</td>
					<td class="SubTituloCentro bold">
						<table cellSpacing="1" cellPadding="3" width="100%" >
							<tr>
								<td  class="SubTituloCentro bold" >Problema(s) Identificado(s)</td>
								<td  class="SubTituloCentro bold" width="100" >Problema(s) Crítico(s)<br/>(Máximo <span id="num_problemas_possiveis">X</span>)</td>
							</tr>
						</table>	
					</td>
				</tr>
				<tr bgcolor="#FFFFFF" >
					<td class="bold center">Instalações</td>
					<td>
						<table cellSpacing="1" cellPadding="3" width="100%" >
							<?php $arrInstalacoesNecessarias = recuperaInstalacoesNecessarias() ?>
							<?php if($arrInstalacoesNecessarias): ?>
								<?php foreach($arrInstalacoesNecessarias as $infra): ?>
									<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td>A escola não possui <?php echo $infra['ifidesc'] ?>.</td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo $infra['rifcritico'] == "t" ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[instalacao][<?php echo $infra['ifiid'] ?>]" value="1" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php $arrInstalacoesInadequadas = recuperaInstalacoesInadequadas() ?>
							<?php if($arrInstalacoesInadequadas): ?>
								<?php foreach($arrInstalacoesInadequadas as $infra): ?>
									<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td>A escola considera que as instalações do(a) <?php echo $infra['ifidesc'] ?> estão inadequados(as).</td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo $infra['rifcritico'] == "t" ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[instalacao][<?php echo $infra['ifiid'] ?>]" value="1" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php $arrRespEscola = recuperaRespostasEscola(null,"I","I",null,array("(op.oppdesc like 'Nunca' or op.oppdesc like 'Raramente')")); ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrInstalacoesInadequadas && !$arrInstalacoesNecessarias && !$arrRespEscola): ?>
								<tr>
									<td>Não foram identificados problemas nas instalações.</td>
								</tr>
							<?php endif; ?>
						</table>
					</td>
				</tr>
				<tr>
					<td class="bold center" >Equipamentos</td>
					<td>
						<table cellSpacing="1" cellPadding="3" width="100%" >
							<?php $arrEquipamentosRuins = recuperaEquipamentosRuins() ?>
							<?php if($arrEquipamentosRuins): ?>
								<?php foreach($arrEquipamentosRuins as $equip): ?>
									<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td>A escola considera que o estado de conservação de <?php echo number_format($equip['rmeqtdruin'],"",2,".") ?> <?php echo $equip['tmedesc'] ?> é/são ruim(ns).</td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo $equip['remcritico'] == "t" ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[equipamento][<?php echo $equip['tmeid'] ?>]" value="1" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php $arrRespEscola = array() ?>
							<?php $arrRespEscola = recuperaRespostasEscola(null,"I","E",null,array("(op.oppdesc like 'Nunca' or op.oppdesc like 'Raramente')")); ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrEquipamentosRuins && !$arrRespEscola): ?>
								<tr>
									<td>Não foram identificados problemas nos equipamentos.</td>
								</tr>
							<?php endif; ?>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr id="tr_projetos_programas" style="display:<?php echo $display ?>" >
		<td class="SubTituloDireita">Projetos e Programas</td>
		<td>
		<p>A escola desenvolve algum PROJETO destinado a melhorar os resultados relacionados à Infraestrutura? <input type="radio" name="exibeprojeto" id="exibeprojeto_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('F','J',true);document.getElementById('projeto_label').style.display='';carregarProjetos('F');}else{document.getElementById('projeto_label').style.display='none';}divCarregado();"> Sim <input type="radio" id="exibeprojeto_N" name="exibeprojeto" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('F','J',false)){document.getElementById('projeto_label').style.display='none';}else{document.getElementById('exibeprojeto_S').checked=true;}}else{document.getElementById('projeto_label').style.display='';}divCarregado();"> Não </p>
		<div id="projeto_label" style="display:none"></div>
		
		<p>A escola participa ou gostaria de participar de algum PROGRAMA, do Governo Federal ou da secretaria, que auxilie na melhoria os resultados relacionados à Infraestrutura? <input type="radio" name="exibeprograma" id="exibeprograma_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('F','G',true);document.getElementById('programa_label').style.display='';carregarProgramas('F');}else{document.getElementById('programa_label').style.display='none';}divCarregado();"> Sim <input type="radio" id="exibeprograma_N" name="exibeprograma" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('F','G',false)){document.getElementById('programa_label').style.display='none';}else{document.getElementById('exibeprograma_S').checked=true;}}else{document.getElementById('programa_label').style.display='';}divCarregado();"> Não </p>
		<div id="programa_label" style="display:none"></div>
		</td>
	</tr>
	<tr id="tr_navegacao" >
		<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
		<td align="left">
			<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_6_infraestrutura") ?>
			<?php if(!$arrTelasPendentes): ?>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarSinteseInfraestrutura()">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarSinteseInfraestrutura('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese'" >
			<?php else: ?>
				<?php foreach($arrTelasPendentes as $tela): ?>
						<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
				<?php endforeach; ?>
				<input type="button" name="btn_anterior" value="Anterior" onclick="irTelaAnterior()" >
				<input type="button" name="btn_proximo" value="Próximo" onclick="irTelaProxima()" >
			<?php endif; ?>
		</td>
	</tr>
	</table>
</form>