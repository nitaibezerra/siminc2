<script src="../includes/calendario.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>

function mostrarInsercaoAtividade() {
	
	jQuery('#gmcid').val('');
	jQuery('#gmcatividade').val('');
   			
	jQuery('#tr_outra_atividade').css('display','none');
   			
	jQuery('#gmcatividadeoutra').val('');
	jQuery('#gmccargahoraria').val('');
	jQuery('#gmcqtdparticipantes').val('');
	jQuery('#gmcinicio').val('');
	jQuery('#gmcfim').val('');
	jQuery('#anexo1').html('');
	jQuery('#anexo2').html('');
   			
	jQuery('#tr_outros_publicoalvo').css('display','none');
	jQuery('#gmcpublicoalvooutros').val('');
   			
	jQuery("[name^='gmcpublicoalvo[]']").attr('checked',false);

	jQuery("#modalGerenciarAtividade").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function mostrarEdicaoAtividade(gmcid) {
	
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A',
   		data: 'requisicao=pegarAtividadeCoordenadorLocal&gmcid='+gmcid,
   		async: false,
   		success: function(json){
   		
   			obj = JSON.parse(json);
   			
   			jQuery('#gmcid').val(obj.gmcid);
   			jQuery('#gmcatividade').val(obj.gmcatividade);
   			
   			if(obj.gmcatividade=='O') {
   				jQuery('#tr_outra_atividade').css('display','');
   			}
   			
   			jQuery('#gmcatividadeoutra').val(obj.gmcatividadeoutra);
   			jQuery('#gmccargahoraria').val(obj.gmccargahoraria);
   			jQuery('#gmcqtdparticipantes').val(obj.gmcqtdparticipantes);
   			jQuery('#gmcinicio').val(obj.gmcinicio);
   			jQuery('#gmcfim').val(obj.gmcfim);
   			jQuery('#anexo1').html(obj.anexo1);
   			jQuery('#anexo2').html(obj.anexo2);
   			
   			
			jQuery("[name^='gmcpublicoalvo[]']").each(function() {
			
				var f = obj.gmcpublicoalvo.indexOf(jQuery(this).val()+";");
				
				if(f >= 0) {
				
					jQuery(this).attr('checked', true);
					
					if(jQuery(this).val()=='O') {
					
						jQuery('#tr_outros_publicoalvo').css('display','');
						jQuery('#gmcpublicoalvooutros').val(obj.gmcpublicoalvooutros);
						
					}
					
				}
		
			});
   			
   		}
	});


	jQuery("#modalGerenciarAtividade").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function excluirAtividadeCoordenadorLocal(gmcid) {
	var conf = confirm('Deseja realmente excluir a atividade?');
	
	if(conf) {
		window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&requisicao=excluirAtividadeCoordenadorLocal&gmcid='+gmcid;
	}

}

function inserirAtividadeCoordenadorLocal() {

	if(jQuery('#gmcatividade').val()=='') {
		alert('Selecione a Atividade');
		return false;
	}
	
	if(jQuery('#gmcatividade').val()=='O') {
		if(jQuery('#gmcatividadeoutra').val()=='') {
			alert('Descreva a outra atividade');
			return false;
		}
	}
	
	var chk_numPublicoAlvo = parseInt(jQuery("[name^='gmcpublicoalvo[']:enabled:checked").length);
	
	if(chk_numPublicoAlvo==0) {
		alert('Marque o público-alvo');
		return false;
	}
	
	if(jQuery('#gmcpublicoalvo_O').attr('checked')==true) {
		if(jQuery('#gmcpublicoalvooutros').val()=='') {
			alert('Descreva outros públicos-alvos');
			return false;
		}
	}
	
	if(jQuery('#gmccargahoraria').val()=='') {
		alert('Preencha a carga horária');
		return false;
	}
	
	if(jQuery('#gmcqtdparticipantes').val()=='') {
		alert('Preencha a quantidade de participantes');
		return false;
	}
	
	if(jQuery('#gmcinicio').val()=='') {
		alert('Preencha Início');
		return false;
	}
	
	if(jQuery('#gmcfim').val()=='') {
		alert('Preencha Fim');
		return false;
	}
	
	if(!validaData(document.getElementById('gmcinicio'))) {
		alert('Data Início inválida. Formato correto : dd/mm/YYYY');
		return false;
	}
	
	if(!validaData(document.getElementById('gmcfim'))) {
		alert('Data Fim inválida. Formato correto : dd/mm/YYYY');
		return false;
	}
	
	if(parseInt(document.getElementById('gmcinicio').value.split("/")[2].toString()) < 2013) {
		alert('Ano de inicío não pode ser inferior a 2013');
		return false;
	}
	
	if(parseInt(document.getElementById('gmcfim').value.split("/")[2].toString()) < 2013) {
		alert('Ano de fim não pode ser inferior a 2013');
		return false;
	}
	
	var data1 = parseInt(document.getElementById('gmcinicio').value.split("/")[2].toString() + document.getElementById('gmcinicio').value.split("/")[1].toString() + document.getElementById('gmcinicio').value.split("/")[0].toString());
	var data2 = parseInt(document.getElementById('gmcfim').value.split("/")[2].toString() + document.getElementById('gmcfim').value.split("/")[1].toString() + document.getElementById('gmcfim').value.split("/")[0].toString());

	if (data2 < data1) {
		alert('Data Término não pode ser inferior à data de início');
		return false;
	}
	
	var dias=0;
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A',
   		data: 'requisicao=calcularNumeroDias&inicio='+jQuery('#gmcinicio').val()+'&fim='+jQuery('#gmcfim').val(),
   		async: false,
   		success: function(ndias){
   			dias = parseInt(ndias);
   		}
	});
	
	if(parseInt(jQuery('#gmccargahoraria').val()) > (dias*8)) {
		alert('A carga horária é superior ao somatório das horas dos dias informados (considerando 8 horas úteis por dia).');
		return false;
	}
	
	jQuery('#formulario').submit();

}

function gravarPerguntasCoordenadorLocal() {

	var natividades=0;
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A',
   		data: 'requisicao=verificarNumeroAtividades',
   		async: false,
   		success: function(natvs){
   			if(natvs!='') {
   				natividades = parseInt(natvs);
   			}
   		}
	});
	if(natividades==0 && jQuery('#gmpnaoatividades').attr('checked')==false) {
		alert('Não existem atividades cadastradas. Marque a opção "Não foram realizadas atividades de Gestão e Mobilização este semestre"');
		return false;
	}


	var chk_numpergunta1 = parseInt(jQuery("[name^='gmppergunta1']:enabled:checked").length);
	
	if(chk_numpergunta1==0) {
		alert('Marque alguma opção para pergunta 1');
		return false;
	}
	
	if(jQuery('#gmppergunta1_comentario').val()=='') {
		alert('Comente a resposta da pergunta 1');
		return false;
	}

	var chk_numpergunta2 = parseInt(jQuery("[name^='gmppergunta2[']:enabled:checked").length);
	
	if(chk_numpergunta2==0) {
		alert('Marque alguma opção para pergunta 2');
		return false;
	}
	
	if(jQuery('#gmppergunta2_outro').attr('checked')==true) {
		if(jQuery('#gmppergunta2_outros').val()=='') {
			alert('Preencha qual a outra dificuldade');
			return false;
		}
	}
	
	var chk_numpergunta3 = parseInt(jQuery("[name^='gmppergunta3']:enabled:checked").length);
	
	if(chk_numpergunta3==0) {
		alert('Marque alguma opção para pergunta 3');
		return false;
	}

	jQuery('#formulario2').submit();
}


function analisarAtividade(gmcatividade) {
	if(gmcatividade=='O') {
		jQuery('#tr_outra_atividade').css('display','');
	} else {
		jQuery('#tr_outra_atividade').css('display','none');
		jQuery('#gmcatividadeoutra').val('');
	}
}

function analisarPublicoAlvo(obj) {
	if(obj.value=='O') {
		if(obj.checked) {
			jQuery('#tr_outros_publicoalvo').css('display','');		
		} else {
			jQuery('#tr_outros_publicoalvo').css('display','none');
			jQuery('#gmcpublicoalvooutros').val('');
		}
	} else {
		jQuery('#tr_outros_publicoalvo').css('display','none');
		jQuery('#gmcpublicoalvooutros').val('');
	}
}

function marcarNaoAtividades(obj) {
	
	var natividades=0;
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A',
   		data: 'requisicao=verificarNumeroAtividades',
   		async: false,
   		success: function(natvs){
   			if(natvs!='') {
   				natividades = parseInt(natvs);
   			}
   		}
	});
	
	if(natividades > 0) {
		alert('Não podem ter atividades cadastradas');
		obj.checked=false;
		return false;
	}


	if(obj.checked) {
		jQuery('#inseriratividade').attr('disabled','disabled');
	} else {
		jQuery('#inseriratividade').attr('disabled','');
	}

}

jQuery(document).ready(function() {
});


</script>

<div id="modalGerenciarAtividade" style="display:none;">
<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="salvarAtividadeCoordenadorLocal">
<input type="hidden" name="gmcid" id="gmcid">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Registro de Atividades</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	O MEC considera que a carga horária mínima que o(a) Coordenador(a) local deve dedicar ao eixo de Gestão e Mobilização Social é de 20 horas mensais de atividades presenciais - ou 120 horas por semestre, dividido entre os diferentes públicos, conforme parâmetros abaixo:<br/>
	<br/>
	a.	Universidade – mínimo 12 horas por semestre (2 horas por mês);<br/>
	b.	Orientadores de Estudo – mínimo 40 horas por semestre (um encontro de 4 horas por mês);<br/>
	c.	Professores Alfabetizadores – mínimo 40 horas por semestre (um encontro de 4 horas por mês);<br/>
	d.	Diretores de Escolas – mínimo 6 horas por semestre (1 hora por mês);<br/>
	e.	Coordenadores Pedagógicos – mínimo 6 horas por semestre (1 hora por mês);<br/>
	f.	Conselhos Escolares – mínimo 4 horas por semestre (1 evento por semestre);<br/>
	g.	Conselho de Educação – mínimo 4 horas por semestre (1 evento por semestre);<br/>
	h.	Equipe da Secretaria – mínimo 8 horas (2 reuniões por semestre);<br/>
	i.	Outros – mínimo 1 hora.<br/>
	<br/>
	Caso o total de horas registradas ou a carga horária para cada tipo de público não alcance o mínimo desejável, o sistema exibirá um aviso, este fato não impedirá a conclusão do registro nem o recebimento da bolsa de estudo.	
	</td>
</tr>
 

<tr>
	<td class="SubTituloDireita">Atividade:</td>
	<td>
	<?
	
	$gestaomobilizacaoatividades = array(array('codigo' => 'R','descricao' => 'Reunião'),
										 array('codigo' => 'E','descricao' => 'Encontro'),
										 array('codigo' => 'S','descricao' => 'Seminário'),
										 array('codigo' => 'D','descricao' => 'Debate'),
										 array('codigo' => 'M','descricao' => 'Mesa-redonda'),
										 array('codigo' => 'C','descricao' => 'Conversa'),
										 array('codigo' => 'F','descricao' => 'Conferência'),
										 array('codigo' => 'P','descricao' => 'Painel'),
										 array('codigo' => 'X','descricao' => 'Exposição'),
										 array('codigo' => 'W','descricao' => 'Workshop'),
										 array('codigo' => 'I','descricao' => 'Simpósio'),
										 array('codigo' => 'G','descricao' => 'Congresso'),
										 array('codigo' => 'V','descricao' => 'Visita'),
										 array('codigo' => 'O','descricao' => 'Outra'));
										 
	$db->monta_combo('gmcatividade', $gestaomobilizacaoatividades, 'S', 'Selecione', 'analisarAtividade', '', '', '', 'S', 'gmcatividade', ''); 
	?>
	</td>
</tr>
<tr id="tr_outra_atividade" style="display:none;">
	<td class="SubTituloDireita">Outra:</td>
	<td><?=campo_texto('gmcatividadeoutra', "S", "S", "Outra", 40, 30, "", "", '', '', 0, 'id="gmcatividadeoutra"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Público-alvo</td>
	<td>
	
	<table width="100%">
		<tr>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="E"> Orientadores de Estudo</td>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="P"> Professores Alfabetizadores</td>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="D"> Diretores de Escolas</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="C"> Coordenadores Pedagógicos</td>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="M"> Conselhos Escolares</td>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="N"> Conselho de Educação</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="S"> Equipe da SEDUC</td>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="R"> Pais ou responsáveis</td>			
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="U"> Universidade</td>
		</tr>
		<tr>
			<td><input type="checkbox" name="gmcpublicoalvo[]" value="O" id="gmcpublicoalvo_O" onclick="analisarPublicoAlvo(this);"> Outros</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
	</table>
 
	</td>
</tr>
<tr id="tr_outros_publicoalvo" style="display:none;">
	<td class="SubTituloDireita">Outros:</td>
	<td><?=campo_texto('gmcpublicoalvooutros', "S", "S", "Outros", 40, 30, "", "", '', '', 0, 'id="gmcpublicoalvooutros"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Carga horária da atividade (em horas):</td>
	<td><?=campo_texto('gmccargahoraria', "S", "S", "Carga horária da atividade", 6, 3, "###", "", '', '', 0, 'id="gmccargahoraria"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Quantidade de participantes:</td>
	<td><?=campo_texto('gmcqtdparticipantes', "S", "S", "Quantidade de participantes", 6, 4, "####", "", '', '', 0, 'id="gmcqtdparticipantes"'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Início:</td>
	<td>
	<?=campo_data('gmcinicio', 'S', 'S', '', 'S'); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Fim:</td>
	<td>
	<?=campo_data('gmcfim', 'S', 'S', '', 'S'); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Anexe uma foto:</td>
	<td><input type="file" name="arquivo1"> <div id="anexo1"></div></td>
</tr>
<tr>
	<td class="SubTituloDireita">Anexe um documento:</td>
	<td><input type="file" name="arquivo2"> <div id="anexo2"></div></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar Atividade" onclick="inserirAtividadeCoordenadorLocal();"></td>
</tr>
</table>
</form>
</div>


<? $gestaomobilizacaoperguntas = $db->pegaLinha("SELECT * FROM sispacto.gestaomobilizacaoperguntas WHERE iusd='".$_SESSION['sispacto']['coordenadorlocal']['iusd']."'"); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Gestão e Mobilização</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2">
			<form method="post" id="formulario2" enctype="multipart/form-data">
			<input type="hidden" name="requisicao" value="gerenciarPerguntasCoordenadorLocal">
		
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita" width="25%">UF / Município:</td>
				<td><?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['descricao']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Universidade responsável pela formação:</td>
				<td><?=$db->pegaUm("SELECT unisigla ||' - '|| uninome FROM sispacto.universidadecadastro uu INNER JOIN sispacto.universidade u ON u.uniid = uu.uniid WHERE uu.uncid='".$_SESSION['sispacto']['coordenadorlocal']['uncid']."'") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Número de Orientadores de Estudo previsto:</td>
				<td>
				<?  
				$ar = array("estuf" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf'],
							"muncod" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'],
							"dependencia" => $_SESSION['sispacto']['esfera']);
				
				$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
				
				echo $totalalfabetizadores['total_orientadores_a_serem_cadastrados'];
				
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Número de Orientadores de Estudo ativos no SisPacto:</td>
				<td><?=$db->pegaUm("SELECT COUNT(*) as t FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."' AND iusstatus='A'") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Número de Professores Alfabetizadores previsto:</td>
				<td><? echo $totalalfabetizadores['total']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Número de Professores Alfabetizadores ativos no SisPacto:</td>
				<td><?=$db->pegaUm("SELECT COUNT(*) as t FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."' AND iusstatus='A'") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Número de escolas envolvidas:</td>
				<td><?=$db->pegaUm("SELECT COUNT(DISTINCT tpacodigoescola) as t FROM sispacto.turmasprofessoresalfabetizadores tf INNER JOIN sispacto.identificacaousuario i ON i.iusd = tf.iusd INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."' AND iusstatus='A'") ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<fieldset>
				<legend>Cadastro de Atividades</legend>
				<p>
				<input type="button" name="inseriratividade" id="inseriratividade" value="Inserir Atividade" onclick="mostrarInsercaoAtividade();" <?=(($gestaomobilizacaoperguntas['gmpnaoatividades']=='t')?'disabled':'') ?>>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="gmpnaoatividades" id="gmpnaoatividades" value="1"  <?=(($gestaomobilizacaoperguntas['gmpnaoatividades']=='t')?'checked':'') ?> onclick="marcarNaoAtividades(this);"> Não foram realizadas atividades de Gestão e Mobilização este semestre
				</p>
				<div id="listaatividades">
				<?
				
				if($gestaomobilizacaoatividades) {
					$case_atividades = "CASE ";
					foreach($gestaomobilizacaoatividades as $gma) {
						$case_atividades .= "WHEN gmcatividade='".$gma['codigo']."' THEN '".$gma['descricao']."'";
					}
					$case_atividades .= "END as gmcatividade";
				}
				
				$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"mostrarEdicaoAtividade('||gmcid||');\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirAtividadeCoordenadorLocal('||gmcid||');\">' as acao,
							    {$case_atividades},
							    REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(gmcpublicoalvo,'E;','<font size=1> - Orientadores de Estudo</font><br>'),'P;','<font size=1> - Professores Alfabetizadores</font><br>'),'D;','<font size=1> - Diretores de Escolas</font><br>'),'C;','<font size=1> - Coordenadores Pedagógicos</font><br>'),'M;','<font size=1> - Conselhos Escolares</font><br>'),'N;','<font size=1> - Conselho de Educação</font><br>'),'S;','<font size=1> - Equipe da SEDUC</font><br>'),'O;','<font size=1> - Outros</font><br>'),'U;','<font size=1> - Universidade</font><br>'),'R;','<font size=1> - Pais ou responsáveis</font><br>') as  gmcpublicoalvo,
							    gmccargahoraria,
							    gmcqtdparticipantes,
							    to_char(gmcinicio,'dd/mm/YYYY') as gmcinicio,
							    to_char(gmcfim,'dd/mm/YYYY')    as gmcfim 
						FROM sispacto.gestaomobilizacaocoordenadorlocal WHERE iusd='".$_SESSION['sispacto']['coordenadorlocal']['iusd']."' AND gmcstatus='A'";
							    
				$cabecalho = array("&nbsp;","Atividade","Público-Alvo","Carga Horária","Quant. Participantes","Inicío","Fim");
				$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, true);
				
				?>
				</div>
				</fieldset>

		</td>
	</tr>
	
	<tr>
		<td colspan="2">
		<p><b>[PERGUNTAS]</b></p>
		
		<p>1. A Secretaria de Educação está assegurando as condições de deslocamento, alimentação e hospedagem para participação nos encontros presenciais dos Orientadores de Estudo e dos Professores Alfabetizadores, quando necessário?</p>
		<p>
		<input type="radio" name="gmppergunta1" value="S" <?=(($gestaomobilizacaoperguntas['gmppergunta1']=='S')?'checked':'') ?>> Sempre<br>
		<input type="radio" name="gmppergunta1" value="A" <?=(($gestaomobilizacaoperguntas['gmppergunta1']=='A')?'checked':'') ?>> Às vezes<br>
		<input type="radio" name="gmppergunta1" value="N" <?=(($gestaomobilizacaoperguntas['gmppergunta1']=='N')?'checked':'') ?>> Nunca<br>
		</p>
		<p>
		<b>Comente</b><br>
		<? $gmppergunta1_comentario = $gestaomobilizacaoperguntas['gmppergunta1_comentario']; ?>
		<?=campo_textarea('gmppergunta1_comentario', 'N', 'S', '', '70', '4', '200'); ?>
		</p>

		<p>2. Quais as principais dificuldades encontradas na execução do Pacto no seu município ou estado?</p>
		<p>
		<? 
		$gmppergunta2 = explode(";",$gestaomobilizacaoperguntas['gmppergunta2']); 
		?>
		<input type="checkbox" name="gmppergunta2[]" value="1" <?=((in_array('1',$gmppergunta2))?'checked':'') ?>> O acesso à Internet é precário;<br>
		<input type="checkbox" name="gmppergunta2[]" value="2" <?=((in_array('2',$gmppergunta2))?'checked':'') ?>> O material da formação chegou após o início da formação;<br>
		<input type="checkbox" name="gmppergunta2[]" value="3" <?=((in_array('3',$gmppergunta2))?'checked':'') ?>> O material complementar chegou após o início da formação;<br>
		<input type="checkbox" name="gmppergunta2[]" value="4" <?=((in_array('4',$gmppergunta2))?'checked':'') ?>> O gestor local não conhece bem o Pacto;<br>
		<input type="checkbox" name="gmppergunta2[]" value="5" <?=((in_array('5',$gmppergunta2))?'checked':'') ?>> Os Orientadores de Estudos e/ou Professores Alfabetizadores estão pouco motivados;<br>
		<input type="checkbox" name="gmppergunta2[]" value="6" <?=((in_array('6',$gmppergunta2))?'checked':'') ?>> Ocorreram greves/ paralisações de professores da rede e/ou da universidade formadora;<br>
		<input type="checkbox" name="gmppergunta2[]" value="7" <?=((in_array('7',$gmppergunta2))?'checked':'') ?>> A estrutura física do município/ estado é inadequada para as atividades de formação, gestão e mobilização;<br>
		<input type="checkbox" name="gmppergunta2[]" value="8" <?=((in_array('8',$gmppergunta2))?'checked':'') ?>> Há dificuldade de obter informações sobre o programa;<br>
		<input type="checkbox" name="gmppergunta2[]" value="9" <?=((in_array('9',$gmppergunta2))?'checked':'') ?>> O contato com a equipe da Universidade formadora é raro e/ou inexistente;<br>
		<input type="checkbox" name="gmppergunta2[]" value="10" <?=((in_array('10',$gmppergunta2))?'checked':'') ?>> O município/ estado não dispõe de recursos financeiros para custear as atividades formativas;<br>
		<input type="checkbox" name="gmppergunta2[]" value="11" id="gmppergunta2_outro" <?=((in_array('11',$gmppergunta2))?'checked':'') ?> onclick="if(this.checked==false){document.getElementById('gmppergunta2_outros').value='';}"> Outros. Qual? <?=campo_texto('gmppergunta2_outros', "S", "S", "Outros", 30, 100, "", "", '', '', 0, 'id="gmppergunta2_outros"', '', $gestaomobilizacaoperguntas['gmppergunta2_outros'] ); ?><br>
		
		</p>
		
		<p>3. A Secretaria de Educação participou de alguma reunião convocada pela Coordenação Institucional do Pacto no seu estado?</p>
		<p>
		<input type="radio" name="gmppergunta3" value="1" <?=(($gestaomobilizacaoperguntas['gmppergunta3']=='1')?'checked':'') ?>> Sim, participamos de mais de uma reunião;<br>
		<input type="radio" name="gmppergunta3" value="2" <?=(($gestaomobilizacaoperguntas['gmppergunta3']=='2')?'checked':'') ?>> Sim, participamos de uma reunião;<br>
		<input type="radio" name="gmppergunta3" value="3" <?=(($gestaomobilizacaoperguntas['gmppergunta3']=='3')?'checked':'') ?>> Não participamos de nenhuma reunião.<br>
		</p>
		
			
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=acompanhamentoavaliacaobolsas';"> 
			<input type="button" id="salvar" value="Salvar" onclick="gravarPerguntasCoordenadorLocal();">
		</td>
	</tr>
</table>
