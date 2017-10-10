<?php 

set_time_limit(30000);
ini_set("memory_limit", "3000M");

if($_REQUEST['req']=='corrigeDiscrepancia'){
	
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
	
	include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario.class.inc';
	include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario_Itens.class.inc';
	echo "<h1>Corrige Formulários</h1>";
	$ob = new Ws_Respostas_Formulario();
	$ob->atualizaCodigoUniversidadeFormularios();
	$ob->atualizaNumeroCicloFormularios();
	$ob->inativaFormulariosCpfVazio();
	$ob->inativaFormulariosDataVisitaErrada();
	$ob->inativaFormulariosMuncodProfissionalVazio();
	if($_REQUEST['puxaformsws']=='true')
		$ob->importaFormulariosMaisMedicos();
		
// 	$obi = new Ws_Respostas_Formulario_Itens();
// 	$obi->importaRespostasFormularioMaisMedicos();
	
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo 'Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb';
	
	echo '<p><center><input type="button" value="Fechar" onclick="javascript:window.close();" /></center></p>';
	die;
}

if($_REQUEST['req']=='puxarRespostasFormulario'){
	if($_REQUEST['idpublicarformulario']>0){
		include_once APPRAIZ . 'maismedicos/classes/Ws_Respostas_Formulario_Itens.class.inc';
		echo "<center><h1>Respotas do formulário número ".$_REQUEST['idpublicarformulario'].'</h1></center>';
		$ob = new Ws_Respostas_Formulario_Itens();
		if($_REQUEST['puxarrespostassimec']=='true'){
			echo "<center><h3>SIMEC</h3></center>";
			echo "<p><center><b>Todas respostas</b></center></p>";
			$rs = $ob->recuperarRespostasPorIdUnasus($_REQUEST['idpublicarformulario']);
			if($rs){
				foreach($rs as $i => $dados){
					echo '<b>'.($i+1).'. '.$dados['dsdimensao'].' - '.$dados['dsgrupo'].' - '.$dados['dspergunta'].'</b><br/>';
					echo $dados['dsresposta'].'<br/><br/>';
					
				}
			}
			
		}else{
			echo "<center><h3>UNASUS</h3></center>";
			if($_REQUEST['todasrespostas']=='true'){
				echo "<p><center><b>Todas respostas</b></center></p>";
				$rs = $ob->getRespostasFormularioMaisMedicosTodas($_REQUEST['idpublicarformulario']);
			}else{
				echo "<p><center><b>Respostas padrão</b></center></p>";
				$rs = $ob->getRespostasFormularioMaisMedicos($_REQUEST['idpublicarformulario']);
			}
			if($rs->item->itemFormulario->item){
				foreach($rs->item->itemFormulario->item as $i => $dados){
					echo '<b>'.($i+1).'. '.$dados->dsDimensao.' - '.$dados->dsGrupo.' - '.$dados->dsPergunta.'</b><br/>';
					echo $dados->dsResposta.'<br/><br/>';
					
				}
			}
		}
// 		ver($rs->item->itemFormulario->item);
	}else{
		echo "<center><p>Id do formulário não encontrado.</p></center>";
	}
	echo '<center><p><input type="button" value="Fechar" onclick="javascript:window.close()" /></p></center>';
	die;
}

if($_REQUEST['req']=='importaUsuariosBolsistas'){
	
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
	
	include_once APPRAIZ . 'maismedicos/classes/Profissionais.class.inc';
	$ob = new Profissionais();
	$ob->importaUsuariosSupervisoresTutoresUnasus();
	
	if($_REQUEST['comitarusuario']=='true')
		$ob->commit();
	
	if($_REQUEST['enviaremail']=='true') 
		$ob->enviarEmailPrimeiroAcessoSimec();
	
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo 'Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb';
	
	echo '<p><center><input type="button" value="Fechar" onclick="javascript:window.close();" /></center></p>';
	die;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$linha1 = 'Atualiza Informações UNASUS'; 
$linha2 = '';
monta_titulo($linha1, $linha2);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('.btnRodarRotina').click(function(){
			
			mes = $('[name=mesreferencia]').val();
			ano = $('[name=anoreferencia]').val();
			idForm = $('[name=idpublicarformulario]').val();
			todasrespostas = $('[name=todasrespostas]').attr('checked');
			puxarrespostassimec = $('[name=puxarrespostassimec]').attr('checked');
			puxaformsws = $('[name=puxaformsws]').attr('checked');
			enviaremail = $('[name=enviaremail]').attr('checked');
			comitarusuario = $('[name=comitarusuario]').attr('checked');
			var url;

			
			
			url = this.id;

			if(mes && ano){
				mes = mes<10 ? '0'+mes : mes;
				url = url + '&mes='+mes+'/'+ano;
			}

			if(idForm>0){
				url = url + '&idpublicarformulario='+idForm;
			}
			
			if(todasrespostas){
				url = url + '&todasrespostas=true';
			}
			if(puxaformsws){
				url = url + '&puxaformsws=true';
			}
			if(puxarrespostassimec){
				url = url + '&puxarrespostassimec=true';
			}
			if(enviaremail){
				url = url + '&enviaremail=true';
			}
			if(comitarusuario){
				url = url + '&comitarusuario=true';
			}
			
			var popUp = window.open(url, 'popRotina', 'height=500,width=400,scrollbars=yes,top=50,left=200');
			popUp.focus();
		});
	});
</script>

<table class="listagem" cellpadding="3" cellspacing="1" width="95%" align="center">	
	<tr>
		<td width="180" class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_cadastro.php?acao=A" /></center></td>
		<td>
			<b>Cadastro de bolsistas</b><br/>
			Atualiza informações dos tutores e supervisores do cadastro UNASUS.
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_importa_profissionais.php?acao=A" /></center></td>
		<td>
			<b>Cadastro de profissionais</b><br/>
			Atualiza informações dos médicos do cadastro de profissionais do UNASUS.
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_importa_instituicoes.php?acao=A" /></center></td>
		<td>
			<b>Cadastro de instituições</b><br/>
			Atualiza os dados das intituições supervisoras e suas resposabilidades territoriais.		
		</td>
	</tr>
	<tr>
		<td width="180" class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_importa_planotrabalho.php?acao=A" /></center></td>
		<td>
			<b>Plano de Trabalho</b><br/>
			Atualiza planos de trabalho do UNASUS.
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_importa_formulario.php?acao=A" /></center></td>
		<td>
			<b>Cadastro de formulários</b><br/>
			Atualiza as informações dos relatórios de supervisão de Primeiras impressões, Primeira Visita de Supervisão, Supervisão Prática, 
			Primeiras impressões - DSEI, Primeira Visita de Supervisão "IN LOCO" – DSEI e Supervisão Prática – DSEI.<br/>
			<font color="red">*Puxa os relatórios do mês atual e do mês anterior.</font> 
			Para baixar todos os formularios novamente <a href="javascript:void(0)" class="btnRodarRotina" id="/seguranca/scripts_exec/ws_mais_medicos_importa_formulario.php?puxtaTudo=true">clique aqui</a>.  </font>
			<br/>
			<table cellpadding="0" cellspacing="0" border="0">
				<tr>
					<td>Para baixar de um mês:</td>
					<td>
					<?php
					$a=0;
					for($x=1;$x<13;$x++){
						$meses_periodo[$a]['codigo'] = $x;
						$meses_periodo[$a]['descricao'] = mes_extenso($x);
						$a++;
					}
		// 			$mesreferencia = date('m');
					$db->monta_combo('mesreferencia',$meses_periodo,'S','Selecione...',null,null,null,null,'N','mesreferencia');
					
					$b=0;
					for($y=2013;$y<2018;$y++){
						$anos_periodo[$b]['codigo'] = $y;
						$anos_periodo[$b]['descricao'] = $y;
						$b++;
					}
		// 			$anoreferencia = date('Y');
					$db->monta_combo('anoreferencia',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anoreferencia');
					?>
					</td>
				</tr>
			</table>
			
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/seguranca/scripts_exec/ws_mais_medicos_importa_formulario_respostas.php?acao=A" /></center></td>
		<td>
			<b>Cadastro de respotas dos formulários</b><br/>
			Atualiza as respotas dos relatórios de supervisão do cadastro de formulários do UNASUS.<br/>
			<font color="red">*Antes de rodar essa rotina execute a atualização dos relatórios de supervisão no Cadastro de formulários 
			logo acima ou <a href="javascript:void(0)" class="btnRodarRotina" id="/seguranca/scripts_exec/ws_mais_medicos_importa_formulario.php?acao=A">clique aqui.</a></font>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/maismedicos/maismedicos.php?modulo=principal/atualizaDadosUnasus&acao=A&req=corrigeDiscrepancia" /></center></td>
		<td>
			<b>Corrigir respostas dos formulários com discrepância</b></br>
			Corrige as respotas dos relatórios de supervisão com datas de visita errada ou vazia, cpf do médico vazio, código da universidade vazia ou 
			ciclo e etapa vazios.<br/>
			<font color="red">*Após rodar essa rotina execute a atualização das respostas dos relatórios de supervisão no Cadastro de respostas dos formulários 
			ou <a href="javascript:void(0)" class="btnRodarRotina" id="/seguranca/scripts_exec/ws_mais_medicos_importa_formulario_respostas.php?acao=A">clique aqui</a>.</font>
			<br/><input type="checkbox" name="puxaformsws" value="true" />&nbsp;Puxa todos formulários novamente
		</td>
	</tr>
	<?php if($db->testa_superuser()): ?>
		<tr>
			<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/maismedicos/maismedicos.php?modulo=principal/atualizaDadosUnasus&acao=A&req=puxarRespostasFormulario" /></center></td>
			<td>
				<b>Puxa respostas de um formulário específico via Webservice do UNASUS</b><br/>
				Puxa as respostas de um formulário específico via WS do UNASUS, informar o códico do formulário no campo abaixo.<br/>
				<input type="text" name="idpublicarformulario" size="10"/>
				<input type="checkbox" name="todasrespostas" value="true"/>&nbsp;Todas respostas UNASUS&nbsp;
				<input type="checkbox" name="puxarrespostassimec" value="true"/>&nbsp;Todas respostas <?php echo SIGLA_SISTEMA; ?>
			</td>
		</tr>
	<?php endif; ?>
	<?php if($db->testa_superuser()): ?>
	<tr>
		<td class="subtitulodireita" align="center" valign="top"><center><input type="button" class="btnRodarRotina" value="Executar" id="/maismedicos/maismedicos.php?modulo=principal/atualizaDadosUnasus&acao=A&req=importaUsuariosBolsistas" /></center></td>
		<td>
			<b>Importa usuários tutores e supervisores</b><br/>
			Importa os bolsistas (tutores e supervisores) do UNASUS e insere os usuários e responsabilidades no <?php echo SIGLA_SISTEMA; ?>.<br/>
			<input type="checkbox" name="enviaremail" value="true"/>&nbsp;Envia e-mail do cadastro&nbsp;
			<input type="checkbox" name="comitarusuario" value="true"/>&nbsp;Comitar&nbsp;
		</td>
	</tr>
	<?php endif; ?>
</table>