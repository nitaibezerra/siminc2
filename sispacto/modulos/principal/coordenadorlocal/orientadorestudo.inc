<?
verificarCoordenadorLocalTermoCompromisso(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));

$consulta = verificaPermissao();

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$consulta = true;
}

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_LOCAL) {
	$consulta = true;
}

$ar = array("estuf" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf'],
			"muncod" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'],
			"dependencia" => $_SESSION['sispacto']['esfera']);

$totalalfabetizadores = carregarTotalAlfabetizadores($ar);

$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>
function inserirOrientadorEstudo() {
	window.open('sispacto.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A','Orientador','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function salvarJustificativas(goto) {
	var validajustificativa = true;
	<? if($orientadoresestudo) : ?>
	<? foreach($orientadoresestudo as $oe) : ?>
	if(jQuery('#tr_justificativa_troca_<?=$oe['iusd'] ?>').css('display')=='table-row') {
	
		var resp1_marcados = parseInt(jQuery("[name^='resp1[<?=$oe['iusd'] ?>]']:enabled:checked").length);
		if(resp1_marcados==0) {
			alert('Selecione motivo no qual o Orientador de Estudo esta sendo trocado');
			validajustificativa = false;
			return false;
		}
		
		jQuery("[name^='resp1[<?=$oe['iusd'] ?>]']:enabled:checked").each(function( key, inpu ) {
			if(inpu.value=='1') {
				if(jQuery("#arq1_<?=$oe['iusd'] ?>_1").val()=='') {
					alert('Anexe um documento assinado pelo Orientador de Estudo que será substituído informando as razões da desistência');
					validajustificativa = false;
					return false;
				}
			}
			
			if(inpu.value=='2') {
				var resp2_marcados = parseInt(jQuery("[name^='resp2[<?=$oe['iusd'] ?>]']:enabled:checked").length);
				if(resp2_marcados==0) {
					alert('Selecione o(s) requisito(s) que o Orientador de Estudo deixou de cumprir e anexe o documento comprobatório solicitado.');
					validajustificativa = false;
					return false;
				}
				
				jQuery("[name^='resp2[<?=$oe['iusd'] ?>]']:enabled:checked").each(function( key, inpu2 ) {
					if(inpu2.value=='1') {
						if(jQuery("#arq2_<?=$oe['iusd'] ?>_1").val()=='') {
							alert('Anexe o documento de exoneração');
							validajustificativa = false;
							return false;
						}
					}
					
					if(inpu2.value=='2') {
						if(jQuery("#arq2_<?=$oe['iusd'] ?>_2").val()=='') {
							alert('Anexe um documento da Secretaria que informe a formação divergente do magistério');
							validajustificativa = false;
							return false;
						}
					}

					if(inpu2.value=='3') {
						if(jQuery("#arq2_<?=$oe['iusd'] ?>_3").val()=='') {
							alert('Anexe um documento da Secretaria que informe a formação superior atual divergente da exigida');
							validajustificativa = false;
							return false;
						}
					}

					if(inpu2.value=='4') {
						if(jQuery("#arq2_<?=$oe['iusd'] ?>_4").val()=='') {
							alert('Anexe uma declaração assinada da Secretaria de Educação atestando este fato');
							validajustificativa = false;
							return false;
						}
					}

					if(inpu2.value=='5') {
						if(jQuery("#arq2_<?=$oe['iusd'] ?>_5").val()=='') {
							alert('Anexe um documento comprobatório');
							validajustificativa = false;
							return false;
						}
						if(jQuery("#jtooutrosdsc<?=$oe['iusd'] ?>").val()=='') {
							alert('Descreva os motivos');
							validajustificativa = false;
							return false;
						}
					}

				});

			}
		});
		
	}
	<? endforeach; ?>
	<? endif; ?>
	
	if(validajustificativa == false) {
		return false;
	}

	divCarregando();
	
	jQuery('#alteracaodados').val('0');
    jQuery('#goto').val(goto);
    jQuery('#requisicao').val('gravarJustificativasTroca');

	document.getElementById('formulario').submit();

}
function salvarOrientadoresEstudos(goto) {

	if(document.getElementById('alimuncodorigem')) {
		if(document.getElementById('alimuncodorigem').value=='') {
			alert('Selecione um município para incluir os professores');
			return false;
		}
	}
	
	var invalidaanexo=false;
	jQuery("[name^='anexoportaria[']").each(function() {
		if(this.value=='') {
			invalidaanexo=true;
		}
	});
	
	if(invalidaanexo) {
		alert('É obrigatório anexar a Portaria que designa os profissionais do magistério indicados como servidores efetivos da Secretaria.');
		return false;	
	}
	
	if(document.getElementById('picselecaopublica_TRUE').checked) {
		if((document.getElementById('tabelaorientadoresestudo').rows.length-1)==0) {
			alert('Selecione Orientadores de Estudo');
			return false;
		}
		
		if((document.getElementById('tabelaorientadoresestudo').rows.length-1)<jQuery('#total_a_serem_cadastrados').val()) {
			var conf = confirm("Não foram preenchidas todas as vagas dos orientadores de estudos. Deseja continuar assim mesmo?");
			if(!conf) {
				return false;
			}
		}
	}
	

	divCarregando();
	
	jQuery('#alteracaodados').val('0');
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function removerAnexoPortaria(ponid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=removerAnexoPortaria&ponid='+ponid;
	}
}


function removerOrientadorEstudo(iusd, obj) {
	var conf = confirm('Deseja realmente excluir este orientador?');
	
	if(conf) {
	
		divCarregando();
		
		var apagarLinha = true;
		
		if(iusd) {
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: 'requisicao=removerOrientadorEstudo&iusd='+iusd,
		   		async: false,
		   		success: function(html){
		   		
		   			if(html!='TRUE') {
		   			
		   				apagarLinha = false;
		   				alert(html);
		   				
		   			}
		   		}
			});
			
		}
		
		if(apagarLinha) {
		
			var tabela = obj.parentNode.parentNode.parentNode.parentNode;
			var linha = obj.parentNode.parentNode.parentNode;
			tabela.deleteRow(linha.rowIndex);
			
		}
		
		divCarregado();
	}
	

}

function fezSelecaoOrientadoresEstudo(obj) {
	if(obj.value=="FALSE") {
		var conf = confirm("Para indicar os Orientadores de Estudo é necessário realizar um processo de seleção interna segundo critérios técnicos e objetivos. Confirma que não foi feita seleção para a escolha dos Orientadores de Estudo? Está ação irá remover todos os orientadores selecionados.");
		if(conf) {
			ajaxatualizar('requisicao=atualizarSelecaoPublica&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>&picselecaopublica=FALSE','');
			window.location=window.location;		
		} else {
			if(jQuery('#tr_orientadoresestudo').css('display')=='table-row') {
				jQuery('#picselecaopublica_TRUE').attr('checked',true);
			}
			obj.checked=false;
		}
	} else if(obj.value=="TRUE") {
		ajaxatualizar('requisicao=atualizarSelecaoPublica&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>&picselecaopublica=TRUE','');
		jQuery('#tr_orientadoresestudo').css('display','');
	}
}

function desejaIncluirProfessoresFormacao(obj) {
	if(obj.value=="TRUE") {
		ajaxatualizar('requisicao=atualizarSelecaoPublica&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>&picselecaopublica=FALSE','');
		ajaxatualizar('requisicao=atualizarInclusaoProfessorRede&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>&picincluirprofessorrede=TRUE','');
		jQuery('#tr_picselecaopublica').css('display','none');
		jQuery('#tr_orientadoresestudo').css('display','none');
		jQuery('#tr_mm').css('display','');
	} else if(obj.value=="FALSE") {
		ajaxatualizar('requisicao=atualizarInclusaoProfessorRede&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>&picincluirprofessorrede=FALSE','');
		jQuery('#tr_picselecaopublica').css('display','');
		jQuery('#tr_mm').css('display','none');

	
	}
}

function verificarSituacaoAdesao(muncod) {
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A',
   		data: 'requisicao=verificarSituacaoAdesao&muncod='+muncod,
   		async: false,
   		success: function(msg){
   			if(msg!='FALSE') {
   				alert('A rede escolhida já concluiu a indicação dos seus Orientadores de Estudo. Escolha outro município.');
   				jQuery('#alimuncodorigem').val('');
   			}
   		}
	});
}

function verificaAlteracao() {
	if ( document.getElementById('alteracaodados').value == "1" ) {
			return 'Atenção. Existem dados do formulário que não foram guardados.';
	}
}

function situacaoIndicacao(situacao,aliid) {
	window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo&requisicao=alterarStatusIndicacao&status='+situacao+'&aliid='+aliid;
}

function exibirInformacoes() {
	jQuery("#modalInformacoes").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

window.onbeforeunload = verificaAlteracao;


function abrirJustificativaTroca(iusd) {

	if(jQuery('#img_troca_'+iusd).attr('title')=='mais') {
		jQuery('#tr_justificativa_troca_'+iusd).css('display','');
		jQuery('#img_troca_'+iusd).attr('src','../imagens/refresh2_01.gif');
		jQuery('#img_troca_'+iusd).attr('title','menos');
	} else {
		if(jQuery('#img_setafilho_'+iusd).attr('title')) {
			var conf = confirm('Deseja realmente remover a justificativa? Estas informações serão removidas.');
			if(conf) {
				window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=removerJustificativaTroca&iusd='+iusd+'&picid=<?=$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'] ?>';
			}
		} else {
			jQuery('#tr_justificativa_troca_'+iusd).css('display','none');
			jQuery('#img_troca_'+iusd).attr('src','../imagens/refresh2.gif');
			jQuery('#img_troca_'+iusd).attr('title','mais');
		}
	}
}

function abrirBlocoJustificativa(id,obj) {
	if(obj.checked) {
		jQuery('#'+id).show();
	} else {
		jQuery('#'+id).hide();
	}
}

function trocarOrientadorEstudoMunicipio(iusd) {
	if(document.getElementById('fioid_'+iusd).value=='') {
		alert('Selecione um usuário. Este usuário deve ter sido cadastrado pelo Coordenador da IES como Ouvinte na aba Formação Inicial');
		return false;
	}

	var conf = confirm('Deseja realmente efetuar a troca do Orientador de Estudo Selecionado?');
	
	if(conf) {
		window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=trocarOrientadorEstudoMunicipio&iusd_antigo='+iusd+'&fioid='+document.getElementById('fioid_'+iusd).value;
	}
}


<? if($estado['esdid'] == ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL) : ?>
jQuery(document).ready(function() {
jQuery("[name^='resp1[']").attr('disabled','disabled');
jQuery("[name^='resp2[']").attr('disabled','disabled');
jQuery("[name^='jtooutrosdsc']").attr('disabled','disabled');
});
<? endif; ?>

</script>

<div id="modalInformacoes" style="display:none;" >
<p>O número de professores alfabetizadores de cada rede foi definido de acordo com o número de turmas do ciclo de alfabetização informado no Censo Escolar 2012. Por exemplo: se o município possui 40 turmas do ciclo de alfabetização e 38 professores atuando nessas turmas (considerando que dois respondem por mais de uma turma), foi utilizado como referência o total de 40 turmas. Se, ao contrário, o município possui 42 professores porque algumas turmas possuem mais de um professor em sala de aula, também foi considerado o total de turmas, ou seja, os mesmos 40.</p>
<p>Logo, é possível que haja pequenas divergências, mas dificilmente esta variação afetará o número final de Orientadores de Estudo do seu estado ou município.</p>
</div>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="inserirOrientadoresEstudo">
<input type="hidden" name="goto" id="goto" value="">
<input type="hidden" name="alteracaodados" id="alteracaodados" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Orientadores de Estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?>
	<? if($estado['esdid'] == ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL) : ?>
	<font style=color:red;>
	<p>ATENÇÃO: Para efetuar a troca dos Orientadores de Estudo será necessário assinalar quem será substituído e responder um questionário para cada um, informando as justificativas da mudança e anexando os documentos comprobatórios. Após assinalar as justificativas e comprova-las, clique no botão “Salvar Justificativas”. Em seguida, retorne para a tela “Resumo Orientadores de Estudo”, clique em “Enviar para análise da Substituição do(s) Orientador(es) de Estudo pelo MEC” e aguarde a resposta, acompanhando a situação no próprio SisPacto. Somente se o MEC aprovar a troca de nomes é que o Coordenador local poderá incluir os novos Orientadores de Estudo.</p>
	<p>Caso desista de dar continuidade à substituição, retorne à aba "Resumo Orientadores de Estudo" e clique em "Devolver para análise da IES".</p>
	</font>
	<? endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Número de Professores Alfabetizadores da Rede (conforme número de turmas indicadas do Censo Escolar 2012)</td>
	<td><?=campo_texto('totalalfabetizadores', "N", "N", "Número de professores Alfabetizadores da Rede (CENSO-2012)", 8, 7, "#######", "", '', '', 0, 'id="totalalfabetizadores"', '', $totalalfabetizadores['total']); ?> <span style="cursor:pointer" onclick="exibirInformacoes();"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
</tr>
<? if($_SESSION['sispacto']['esfera']=='estadual') : ?>
<tr>
	<td class="SubTituloDireita" width="25%">Municípios que desejam incluir professores na sua rede</td>
	<td>
	<?
	$sql = "SELECT m.mundescricao, 
			CASE WHEN alistatus='A' THEN 'Ativo' WHEN alistatus='P' THEN 'Pendente' WHEN alistatus='I' THEN 'Inativo' END as alistatus, 
			a.aliquantidade, 
			CASE WHEN alistatus='P' THEN '<center>".((!$consulta)?"<input type=button value=Aprovar onclick=\"situacaoIndicacao(\'A\', '||a.aliid||');\"> <input type=button name=alfabetizadoresindicados value=Reprovar onclick=\"situacaoIndicacao(\'I\', '||a.aliid||');\">":"")."</center>'
				 WHEN alistatus='I' THEN '<center>".((!$consulta)?"<input type=button value=Aprovar onclick=\"situacaoIndicacao(\'A\', '||a.aliid||');\">":"")."</center>' 
				 WHEN alistatus='A' THEN '<center>".((!$consulta)?"<input type=button value=Reprovar onclick=\"situacaoIndicacao(\'I\', '||a.aliid||');\">":"")."</center>'
				 END as acao,
			e.esddsc
			FROM sispacto.alfabetizadoresindicados a
	        INNER JOIN territorios.municipio m ON m.muncod = a.alimuncodorigem 
	        INNER JOIN sispacto.pactoidadecerta p ON p.muncod = a.alimuncodorigem 
	        INNER JOIN workflow.documento d ON d.docid = p.docid 
	        INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			WHERE a.aliestufdestino='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']."'";
	
	$cabecalho = array("Município solicitante","Situação","Quantidade de professores","&nbsp;","&nbsp;");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="25%">Número de orientadores de estudos a serem cadastrados</td>
	<td><?=campo_texto('total_a_serem_cadastrados', "N", "N", "Número de orientadores de estudos a serem cadastrados", 8, 7, "#######", "", '', '', 0, 'id="total_a_serem_cadastrados"', '', $totalalfabetizadores['total_orientadores_a_serem_cadastrados']); ?></td>
</tr>
<?
$pactoidadecerta = $db->pegaLinha("SELECT picselecaopublica, picincluirprofessorrede FROM sispacto.pactoidadecerta WHERE picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."'");
$picselecaopublica = $pactoidadecerta['picselecaopublica'];
$picincluirprofessorrede = $pactoidadecerta['picincluirprofessorrede'];

if($totalalfabetizadores['total'] <= 10) :

	$alfabetizadoresindicados = $db->pegaLinha("SELECT aliid, aliestufdestino, alistatus FROM sispacto.alfabetizadoresindicados WHERE alimuncodorigem='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod']."'");

?>
<tr>
	<td class="SubTituloDireita" width="25%">Deseja incluir professores em outra rede?</td>
	<td><input type="radio" <?=(($alfabetizadoresindicados['alistatus'] == 'A')?"disabled":"") ?> id="picincluirprofessorrede_TRUE" name="picincluirprofessorrede" <?=(($picincluirprofessorrede=="t")?"checked":"") ?> value="TRUE" onclick="if(this.checked){desejaIncluirProfessoresFormacao(this);}" <?=(($consulta)?"disabled":"") ?> > Sim <input type="radio" name="picincluirprofessorrede"  <?=(($alfabetizadoresindicados['alistatus'] == 'A')?"disabled":"") ?> id="picincluirprofessorrede_FALSE" value="FALSE" onclick="if(this.checked){desejaIncluirProfessoresFormacao(this);}" <?=(($picincluirprofessorrede=="f")?"checked":"") ?> <?=(($consulta)?"disabled":"") ?>> Não</td>
</tr>
<tr id="tr_mm" <?=(($picincluirprofessorrede=="t")?'':'style="display:none;"') ?>>
	<td class="SubTituloDireita" width="25%">Confirma o estado para incluir os professores</td>
	<td>
	<?
	$sql = "SELECT estuf as codigo, estuf||' - '||estdescricao as descricao FROM territorios.estado WHERE estuf='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']."'";
	$arrEst = $db->pegaLinha($sql);
	echo campo_texto('descricao', "N", "N", "Estado de destino", 67, 150, "", "", '', '', 0, 'id="descricao"', '', $arrEst['descricao'] );
	echo "<input type=\"hidden\" name=\"aliestufdestino\" value=\"".$arrEst['codigo']."\">";

	if($alfabetizadoresindicados['alistatus'] == 'P') {
		echo " <font size=1>* Aguardando aprovação do estado</font>";
	}
	if($alfabetizadoresindicados['alistatus'] == 'I') {
		echo " <font size=1>* Estado não aprovou a inclusão dos professores</font>";
	}
	if($alfabetizadoresindicados['alistatus'] == 'A') {
		echo " <font size=1>* Estado aprovou a inclusão dos professores</font>";
	}
	?>
	</td>
</tr>
<tr id="tr_picselecaopublica" <?=(($picincluirprofessorrede=="f")?'':'style="display:none;"') ?>>
	<td class="SubTituloDireita" width="25%">Foram adotados critérios técnicos e objetivos para a seleção dos Orientadores de Estudo?</td>
	<td><input type="radio" id="picselecaopublica_TRUE" name="picselecaopublica" <?=(($picselecaopublica=="t")?"checked":"") ?> value="TRUE" onclick="if(this.checked){fezSelecaoOrientadoresEstudo(this);}" <?=(($consulta)?"disabled":"") ?> > Sim <input type="radio" name="picselecaopublica" id="picselecaopublica_FALSE" value="FALSE" onclick="fezSelecaoOrientadoresEstudo(this);" <?=(($picselecaopublica=="f")?"checked":"") ?> <?=(($consulta)?"disabled":"") ?>> Não</td>
</tr>
<?
else :
?>
<tr>
	<td class="SubTituloDireita" width="25%">Foram adotados critérios técnicos e objetivos para a seleção dos Orientadores de Estudo?</td>
	<td><input type="radio" id="picselecaopublica_TRUE" name="picselecaopublica" <?=(($picselecaopublica=="t")?"checked":"") ?> value="TRUE" onclick="if(this.checked){fezSelecaoOrientadoresEstudo(this);}" <?=(($consulta)?"disabled":"") ?> > Sim <input type="radio" name="picselecaopublica" id="picselecaopublica_FALSE" value="FALSE" onclick="fezSelecaoOrientadoresEstudo(this);" <?=(($picselecaopublica=="f")?"checked":"") ?> <?=(($consulta)?"disabled":"") ?>> Não</td>
</tr>
<?
endif;
?>
<tr id="tr_orientadoresestudo" <?=(($picselecaopublica=="t")?"":"style='display:none'") ?> >
	<td colspan="2">
	<? if(!$consulta) : ?>
	<p><input type="button" name="inserirorientadorestudo" value="Inserir Orientador de Estudo" onclick="inserirOrientadorEstudo();"></p>
	<? endif; ?>
	
	<table class="listagem" id="tabelaorientadoresestudo" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="5%">&nbsp;</td>
		<td class="SubTituloCentro" width="10%">CPF</td>
		<td class="SubTituloCentro" width="15%">Nome</td>
		<td class="SubTituloCentro" width="15%">E-mail</td>
		<td class="SubTituloCentro" width="10%">UF/Município</td>
		<td class="SubTituloCentro" width="25%">Tipo</td>
		<td class="SubTituloCentro" width="20%">Documento comprobatório</td>		
	</tr>
	<? if($orientadoresestudo) : ?>
	<? foreach($orientadoresestudo as $oe) : ?>
	<tr>
		<td width="5%">
		<? $resp = carregarJustificativaTroca(array("iusd"=>$oe['iusd'],"picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));	?>
		<? if($estado['esdid'] == ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL) : ?>
		<center><img src="../imagens/<?=(($resp['cadastrojustificativa'])?"refresh2_01.gif":"refresh2.gif") ?>" border="0" id="img_troca_<?=$oe['iusd'] ?>" <?=(($resp['cadastrojustificativa'])?'title="menos"':'title="mais"') ?> style="cursor:pointer;" onclick="abrirJustificativaTroca('<?=$oe['iusd'] ?>');"></center>
		<? endif; ?>
		<? $aedid_ant = $db->pegaUm("SELECT h.aedid FROM workflow.documento d inner join workflow.historicodocumento h ON h.hstid = d.hstid WHERE d.docid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docid']."'"); ?>
		<? if(!$consulta && $aedid_ant!=AED_AUTORIZAR_TROCA_ORIENTADORES) : ?>
		<center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer;" id="img_<?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?>_<?=$oe['muncodatuacao'] ?>" onclick="removerOrientadorEstudo(<?=$oe['iusd'] ?>, this);"></center>
		<? endif; ?>
		<? if(($db->testa_superuser() || in_array(PFL_EQUIPEMEC,$perfis)) && $oe['iusformacaoinicialorientador']!='t') : ?>
		<center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer;" id="img_<?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?>_<?=$oe['muncodatuacao'] ?>" onclick="removerOrientadorEstudo(<?=$oe['iusd'] ?>, this);"></center>
		<? endif; ?>
		
		</td>
		<td width="10%"><?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?></td>
		<td width="15%">
		<? 
		unset($combo_troca);
		if($oe['uncid']) $esdidformacaoinicial = $db->pegaUm("SELECT d.esdid FROM sispacto.universidadecadastro u INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial WHERE u.uncid='".$oe['uncid']."'");
		
		if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_LOCAL && $esdidformacaoinicial == ESD_FECHADO_FORMACAOINICIAL) {
			if($oe['iusformacaoinicialorientador']=='t') {
				echo "<img src=\"../imagens/check_checklist.png\" width=\"15\" height=\"15\" onmouseover=\"return escape('Orientador de Estudo PRESENTE no curso de Formação Inicial');\"> ";
			}
			
			if($oe['iusformacaoinicialorientador']=='f') {
				echo "<img src=\"../imagens/exclamacao_checklist.png\" width=\"15\" height=\"15\" onmouseover=\"return escape('Orientador de Estudo AUSENTE no curso de Formação Inicial');\"> ";
			}
			
			if(substr($oe['iuscpf'],0,3)=='SIS') {
				echo "<img src=\"../imagens/erro_checklist.png\" align=\"absmiddle\" width=\"15\" height=\"15\"  onmouseover=\"return escape('Orientado de estudo CRIADO PELO SISTEMA deve ser substituído por Orientador de Estudo PRESENTE na Formação Inicial');\"> ";
			}
				
			$wh[] = "fiostatus='A'";
			if($_SESSION['sispacto']['esfera']=='municipal') $wh[] = "fioesfera='M' AND muncod='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod']."'";
			elseif($_SESSION['sispacto']['esfera']=='estadual') $wh[] = "fioesfera='E' AND estuf='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']."'";
			
			$sql = "SELECT fioid FROM sispacto.formacaoinicialouvintes WHERE iusd='".$oe['iusd']."'";
			$fioid_p = $db->pegaUm($sql);
			
			$sql = "SELECT fioid as codigo, fionome as descricao FROM sispacto.formacaoinicialouvintes 
					WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY fionome";
			
			$ddcombo = $db->carregar($sql);
			
			if($ddcombo[0]) $combo_troca = $db->monta_combo('fioid', $ddcombo, 'S', 'Selecione', '', '', '', '', 'N', 'fioid_'.$oe['iusd'], true, $fioid_p);
			
		}
		 
		echo $oe['iusnome']; 
		
		if($combo_troca) {
			echo "<br>".$combo_troca." <input type=button value=Atualizar onclick=\"trocarOrientadorEstudoMunicipio('".$oe['iusd']."');\">";
		}
		
		?>
		
		</td>
		<td width="15%" id="td_img_<?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?>_<?=$oe['muncodatuacao'] ?>"><?=$oe['iusemailprincipal'] ?></td>
		<td width="10%"><?=$oe['municipiodescricaoatuacao'] ?></td>
		<td width="25%"><?=$_TIPO_ORIENTADORES[$oe['iustipoorientador']] ?></td>
		<td width="20%"><?
		if($oe['iustipoorientador']=="profissionaismagisterio" && substr($oe['iuscpf'],0,3)!='SIS') {
			
			$sql = "SELECT arqnome||'.'||arqextensao as arquivo, a.arqid, p.ponid
					FROM public.arquivo a 
					INNER JOIN sispacto.portarianomeacao p ON a.arqid = p.arqid 
					WHERE iusd='".$oe['iusd']."'";
			
			$arquivo = $db->pegaLinha($sql);
			
			if($arquivo) {
				echo "<img src=\"../imagens/anexo.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=".$arquivo['arqid']."';\">".(($consulta || $aedid_ant==AED_AUTORIZAR_TROCA_ORIENTADORES)?"":" <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"removerAnexoPortaria('".$arquivo['ponid']."')\">")." ".$arquivo['arquivo'];
			} else {
				echo "<input type=\"file\" name=\"anexoportaria[".mascaraglobal($oe['iuscpf'],"###.###.###-##")."]\">";
			}
			
		} else {
			echo "&nbsp;";
		}
		?></td>
	</tr>
	<? if($estado['esdid'] == ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL || $estado['esdid'] == ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL) : ?>
	
	<tr <?=(($resp['cadastrojustificativa'])?'':'style="display:none;"') ?> id="tr_justificativa_troca_<?=$oe['iusd'] ?>">
		<td align="center"><img src="../imagens/seta_filho.gif" id="img_setafilho_<?=$oe['iusd'] ?>" title="<?=(($resp['cadastrojustificativa'])?'location':'') ?>"></td>
		<td colspan="6">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" class="SubTituloCentro">Pergunta: Por que este Orientador de Estudo será substituído?</td>
			</tr>
			<tr>
				<td><input type="checkbox" name="resp1[<?=$oe['iusd'] ?>][]" <?=((in_array('1',$resp['resp1']))?'checked':'') ?> value="1" onclick="abrirBlocoJustificativa('dv_blocojust<?=$oe['iusd'] ?>_1',this);"></td>
				<td>Porque o próprio Orientador desistiu da função.</td>
			</tr>
			<tr id="dv_blocojust<?=$oe['iusd'] ?>_1" <?=((in_array('1',$resp['resp1']))?'':'style="display:none"') ?>>
				<td colspan="2">
				
				<table cellSpacing="1" cellPadding="3">
					<tr>
						<td colspan="4" class="SubTituloEsquerda">Anexe um documento assinado pelo Orientador de Estudo que será substituído informando as razões da desistência.</td>
					</tr>
					<tr>
						<td colspan="4">
						<? if($resp['arq1']['1']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERe arqid='".$resp['arq1']['1']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq1']['1'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq1[<?=$oe['iusd'] ?>][1]" id="arq1_<?=$oe['iusd'] ?>_1" value="<?=$resp['arq1']['1'] ?>">
						<? else : ?>
						<input type="file" name="arq1[<?=$oe['iusd'] ?>][1]" id="arq1_<?=$oe['iusd'] ?>_1">
						<? endif; ?>
						</td>
					</tr>
				</table>
				
				</td>
			</tr>
			<tr>
				<td><input type="checkbox" name="resp1[<?=$oe['iusd'] ?>][]" <?=((in_array('2',$resp['resp1']))?'checked':'') ?> value="2" onclick="abrirBlocoJustificativa('dv_blocojust<?=$oe['iusd'] ?>_2',this);"></td>
				<td>Porque o Orientador de Estudo cadastrado não preenche um ou mais requisitos de seleção.</td>
			</tr>
			<tr id="dv_blocojust<?=$oe['iusd'] ?>_2" <?=((in_array('2',$resp['resp1']))?'':'style="display:none"') ?>>
				<td colspan="2">
				<table cellSpacing="1" cellPadding="3">
					<tr>
						<td colspan="4" class="SubTituloEsquerda">Assinale o(s) requisito(s) que o Orientador de Estudo selecionado não cumpre e anexe o(s) respectivo(s) documento(s) comprobatório(s).</td>
					</tr>

					<tr>
						<td><input type="checkbox" name="resp2[<?=$oe['iusd'] ?>][]" <?=((in_array('1',$resp['resp2']))?'checked':'') ?> value="1" onclick="abrirBlocoJustificativa('sp<?=$oe['iusd'] ?>_1',this);"></td>
						<td>
						O Orientador selecionado não é mais servidor efetivo da Secretaria de Educação<br/>
						<span <?=((in_array('1',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_1"><b>Anexe o documento de exoneração:</b>
						<? if($resp['arq2']['1']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['1']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['1'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq2[<?=$oe['iusd'] ?>][1]" id="arq2_<?=$oe['iusd'] ?>_1" value="<?=$resp['arq2']['1'] ?>">
						<? else : ?>
						<input type="file" name="arq2[<?=$oe['iusd'] ?>][1]" id="arq2_<?=$oe['iusd'] ?>_1">
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" name="resp2[<?=$oe['iusd'] ?>][]" <?=((in_array('2',$resp['resp2']))?'checked':'') ?> value="2" onclick="abrirBlocoJustificativa('sp<?=$oe['iusd'] ?>_2',this);"></td>
						<td>
						O Orientador selecionado não é profissional do magistério<br/>
						<span <?=((in_array('2',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_2"><b>Anexe um documento da Secretaria que declare a formação divergente do magistério, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['2']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['2']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['2'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq2[<?=$oe['iusd'] ?>][2]" id="arq2_<?=$oe['iusd'] ?>_2" value="<?=$resp['arq2']['2'] ?>">
						<? else : ?>
						<input type="file" name="arq2[<?=$oe['iusd'] ?>][2]" id="arq2_<?=$oe['iusd'] ?>_2">
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" name="resp2[<?=$oe['iusd'] ?>][]" <?=((in_array('3',$resp['resp2']))?'checked':'') ?> value="3" onclick="abrirBlocoJustificativa('sp<?=$oe['iusd'] ?>_3',this);"></td>
						<td>
						O Orientador selecionado não é formado em Pedagogia nem possui Licenciatura<br/>
						<span <?=((in_array('3',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_3"><b>Anexe uma declaração da Secretaria sobre a formação divergente da exigida, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['3']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['3']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['3'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq2[<?=$oe['iusd'] ?>][3]" id="arq2_<?=$oe['iusd'] ?>_3" value="<?=$resp['arq2']['3'] ?>">
						<? else : ?>
						<input type="file" name="arq2[<?=$oe['iusd'] ?>][3]" id="arq2_<?=$oe['iusd'] ?>_3">
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" name="resp2[<?=$oe['iusd'] ?>][]" <?=((in_array('4',$resp['resp2']))?'checked':'') ?> value="4" onclick="abrirBlocoJustificativa('sp<?=$oe['iusd'] ?>_4',this);"></td>
						<td>
						O Orientador selecionado não atuou por pelo menos 3 anos nos anos iniciais do Ensino Fundamental<br/>
						<span <?=((in_array('4',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_4"><b>Anexe uma declaração assinada da Secretaria de Educação atestando este fato, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['4']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['4']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['4'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq2[<?=$oe['iusd'] ?>][4]" id="arq2_<?=$oe['iusd'] ?>_4" value="<?=$resp['arq2']['4'] ?>">
						<? else : ?>
						<input type="file" name="arq2[<?=$oe['iusd'] ?>][4]" id="arq2_<?=$oe['iusd'] ?>_4">
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" name="resp2[<?=$oe['iusd'] ?>][]" <?=((in_array('5',$resp['resp2']))?'checked':'') ?> value="5" onclick="abrirBlocoJustificativa('sp<?=$oe['iusd'] ?>_5',this);"></td>
						<td>
						Outro(s) motivo(s) que descumpre(m) a Portaria 1.458, de 14/12/2012<br/>
						<span <?=((in_array('5',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_5">
						<b>Descreva os motivos:</b><br/>
						<? echo campo_textarea( 'jtooutrosdsc'.$oe['iusd'], 'S', 'S', '', '70', '4', '300', '', '', '', '', '', $resp['jtooutrosdsc']); ?>
						<br/>
						<b>Anexe um documento que comprove este fato:</b> 
						<? if($resp['arq2']['5']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['5']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['5'] ?>';"> <?=$arquivo ?>
						<input type="hidden" name="arq2[<?=$oe['iusd'] ?>][5]" id="arq2_<?=$oe['iusd'] ?>_5" value="<?=$resp['arq2']['5'] ?>">
						<? else : ?>
						<input type="file" name="arq2[<?=$oe['iusd'] ?>][5]" id="arq2_<?=$oe['iusd'] ?>_5">
						<? endif; ?>
						</span>
						</td>
					</tr>

				</table>
				</td>
			</tr>
		</table>
		
		</td>
	</tr>
	<? endif; ?>
	
	<? endforeach; ?>
	<? endif; ?>
	<tr>
		<td align="center"><img src="../imagens/seta_filho.gif" id="img_setafilho_<?=$oe['iusd'] ?>" title="<?=(($resp['cadastrojustificativa'])?'location':'') ?>"></td>
		<td colspan="6"><b>Total de Orientadores de Estudo : <?=count($orientadoresestudo) ?></b></td>
	</tr>
	
	</table>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=dados';">
		<? if(!$consulta || $estado['esdid'] == ESD_VALIDADO_COORDENADOR_LOCAL) : ?> 
		<input type="button" name="salvar" value="Salvar" onclick="salvarOrientadoresEstudos('sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo');"> <input type="button" value="Salvar e Continuar" onclick="salvarOrientadoresEstudos('sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=resumoorientadorestudo');">
		<? endif; ?> 
		<? if($estado['esdid'] == ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL) : ?>
		<input type="button" name="salvarjustificativa" value="Salvar Justificativas" onclick="salvarJustificativas('sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo');">
		<? endif; ?>
		<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=resumoorientadorestudo';"></td>
</tr>
</table>
</form>