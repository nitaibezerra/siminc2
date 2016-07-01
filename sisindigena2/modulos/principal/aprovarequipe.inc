<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>
function selecionarFiltros(x) {
	var fpbid = jQuery('#fpbid').val();
	var pflcod = jQuery('#pflcodaprovar').val();
	if(fpbid && pflcod) {
		divCarregando();
		window.location=window.location+'&fpbid='+fpbid+'&pflcodaprovar='+pflcod;
	}
}

function aprovarEquipe() {
	var tchk = jQuery("[name^='menid[']:enabled:checked").length;
	
	if(tchk==0) {
		alert('Selecione os Usuários para aprovar');
		return false;
	}
	
	conf = confirm('TODOS OS USUÁRIOS MARCADOS serão aprovados, com isso entrarão para a fila de pagamento. Deseja realmente continuar?');
	
	if(conf) {
	
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "aprovarEquipe");
		document.getElementById("formulario").appendChild(input);

		document.getElementById('formulario').submit();	
	}

}

function verConsultar(menid) {

	ajaxatualizar('requisicao=consultarDetalhesAvaliacoes&menid='+menid,'consultatd');
	
	jQuery("#modalConsulta").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function mostrarJustificativa(docid) {

	jQuery('#docidmensario').val(docid);

	jQuery("#modalJustificativa").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function invalidarMensario() {

	if(jQuery('#cmddsc').val()=='') {
		alert('É necessário preencher a justificativa');
		return false;
	}
	
	var conf = confirm('Deseja realmente invalidar a avaliação? Esta ação descartará a avaliação e este usuário ficara permanentemente impossibilitado de receber a bolsa no período de referência selecionado. Não invalide se este usuário tem direito a esta bolsa.');
	
	if(conf) {
		document.getElementById('formulario2').submit();
	}
}

</script>

<div id="modalConsulta" style="display:none;">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td id="consultatd"></td>
</tr>
</table>
</div>

<div id="modalJustificativa" style="display:none;">
<form method="post" id="formulario2" name="formulario2">
<input type="hidden" name="docidmensario" id="docidmensario">
<input type="hidden" name="requisicao" value="invalidarMensario">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Você está invalidando uma avaliação. Neste procedimento a avaliação feita pelos perfis anteriores é considerada equivocada, e você, ao invalidar, exclui permanentemente a bolsa referente a o mês de referencia invalidado. Desta forma, para o bolsista em questão, este mês de pagamento será excluído, não podendo ser reencaminhado ou reavaliado em nenhum momento.</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Justificativa</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'cmddsc', 'S', 'S', '', '70', '4', '200'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Invalidar Avaliação" onclick="invalidarMensario();"></td>
</tr>
</table>
</form>
</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Aprovar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sisindigena2/sisindigena2.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=aprovarusuario"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione período de referência:</td>
	<td><? 
	carregarPeriodoReferencia(array('uncid'=>$_SESSION['sisindigena2'][$sis]['uncid'],'somentecombo'=>true,'fpbid'=>$_REQUEST['fpbid']))
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>

<? $sql_aux = exibirSituacaoMensario(array('uncid'=>$_SESSION['sisindigena2'][$sis]['uncid'],'picid'=>$_SESSION['sisindigena2'][$sis]['picid'],'retornarsql'=>true,'fpbid'=>$_REQUEST['fpbid'])); ?>
<? $sql_aux = "SELECT foo4.ap FROM ({$sql_aux}) foo4 WHERE foo4.pflcod=p.pflcod"; ?>
<tr>
	<td class="SubTituloDireita">Selecione Perfil:</td>
	<td>
	<? 
	if($sis=='coordenadoradjuntoies') {
		$in_pfl = array(PFL_FORMADORIES,PFL_CONTEUDISTA,PFL_PESQUISADOR,PFL_SUPERVISORIES,PFL_COORDENADORLOCAL,PFL_ORIENTADORESTUDO,PFL_PROFESSORALFABETIZADOR);
	} else {
		$in_pfl = array(PFL_FORMADORIES,PFL_CONTEUDISTA,PFL_PESQUISADOR,PFL_SUPERVISORIES,PFL_COORDENADORLOCAL,PFL_ORIENTADORESTUDO,PFL_PROFESSORALFABETIZADOR,PFL_COORDENADORADJUNTOIES,PFL_COORDENADORIES);
	}
		
		
    $sql = "SELECT p.pflcod as codigo, p.pfldsc || ' ( '||COALESCE(({$sql_aux}),0)||' )' as descricao FROM seguranca.perfil p WHERE p.pflcod IN('".implode("','",$in_pfl)."')";
    $db->monta_combo('pflcodaprovar', $sql, 'S', 'Selecione', 'selecionarFiltros', '', '', '', 'S', 'pflcodaprovar','', $_REQUEST['pflcodaprovar']); 
    ?> 
	</td>
</tr>
<? if($_REQUEST['pflcodaprovar']) : ?>
<tr>
	<td colspan="2">
	<form name="formulario_filtro" id="formulario_filtro" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	<td class="SubTituloDireita" width="25%">CPF:</td>
	<td><?=campo_texto('iuscpf_f', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_f"', '', $_REQUEST['iuscpf_f']); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita" width="25%">Nome:</td>
	<td><? echo campo_texto('iusnome_f', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome_f"', '', $_REQUEST['iusnome_f'] ); ?></td>
	</tr>
	<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" name="filtro" value="Buscar"> <input type="button" name="todos" value="Ver todos" onclick="document.getElementById('iusnome_f').value='';document.getElementById('iuscpf_f').value='';document.getElementById('formulario_filtro').submit();"></td>
	</tr>
	</table>
	</form>
	<?
	
	$sql = "SELECT '<input type=\"checkbox\" name=\"menid[]\" '||".criteriosAprovacao('restricao2')."||' value=\"'||foo.menid||'\"> <img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verConsultar('||foo.menid||')\">'||CASE WHEN foo.esdid=".ESD_ENVIADO_MENSARIO." THEN ' <img src=\"../imagens/valida3.gif\" style=\"cursor:pointer;\" onclick=\"mostrarJustificativa('||foo.docid||');\">' ELSE '' END,
				  replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,																																												
				   foo.iusnome,
				   foo.pfldsc,
				   CASE WHEN foo.mensarionota >= 7 THEN '<span style=color:blue;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>' 
				   		WHEN foo.mensarionota > 0 THEN '<span style=color:red;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>'
				   		ELSE '<span style=float:right;>Sem avaliação</span>' END  as mensarionota,
				   CASE WHEN foo.iustermocompromisso=true THEN '<center><span style=color:blue;>Sim</span></center>' ELSE '<center><span style=color:red;>Não</span></center>' END as termocompromisso,
				   ".criteriosAprovacao('restricao1')."
			FROM (
			SELECT  m.menid,
					m.iusd,
					i.iuscpf,
					i.iusnome,
					i.iustermocompromisso, 
					p.pfldsc,
					p.pflcod,
					e.esdid,
					i.iusdocumento,
					i.iustipoprofessor,
					i.iusnaodesejosubstituirbolsa,
					d.docid, 
					(SELECT COUNT(mavid) FROM sisindigena2.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
					(SELECT COUNT(pboid) FROM sisindigena2.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
					(SELECT COUNT(pboid) FROM sisindigena2.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
					COALESCE((SELECT AVG(mavtotal) FROM sisindigena2.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
					m.fpbid,
					t.fpbidini,
					t.fpbidfim,
					pp.plpmaximobolsas
			FROM sisindigena2.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid  
			INNER JOIN sisindigena2.identificacaousuario i ON i.iusd = m.iusd 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sisindigena2.pagamentoperfil pp ON pp.pflcod = t.pflcod 
			WHERE d.esdid NOT IN('".ESD_APROVADO_MENSARIO."') AND
				  t.pflcod='".$_REQUEST['pflcodaprovar']."' AND 
				  i.uncid='".$_SESSION['sisindigena2'][$sis]['uncid']."' AND 
				  m.fpbid='".$_REQUEST['fpbid']."' 
				  ".(($_REQUEST['iusnome_f'])?"AND i.iusnome ILIKE '".$_REQUEST['iusnome_f']."%'":"")." 
				  ".(($_REQUEST['iuscpf_f'])? "AND i.iuscpf ILIKE '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf_f'])."%'":"")." 
				  ".(($_SESSION['sisindigena2'][$sis]['picid'])?"AND i.picid='".$_SESSION['sisindigena2'][$sis]['picid']."'":"")."  
			) foo
			ORDER BY 1, foo.iusnome ASC 
			";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Perfil","Nota avaliação","Termo de Compromisso","Restrições");
	$db->monta_lista($sql,$cabecalho,150,5,'N','center','N','formulario','','',null,array('ordena'=>false));
 	?>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
	<input type="button" value="Aprovar" onclick="aprovarEquipe();">
	</td>
</tr>
<? endif; ?>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	</td>
</tr>
</table>