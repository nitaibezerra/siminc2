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

function visualizarPeriodoTrava(tpeid) {

	ajaxatualizar('requisicao=visualizarPeriodoTrava&tpeid='+tpeid,'consultatd');
	
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

function atualizarPeriodoTrava(tpeid) {
	ajaxatualizar('requisicao=atualizarPeriodoTrava&tpeid='+tpeid+'&fpbidini='+jQuery('#fpbidini').val()+'&fpbidfim='+jQuery('#fpbidfim').val(),'consultatd');
	window.location=window.location;
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

function marcarTodos(obj) {
	jQuery("[name^='menid[]']:enabled").attr('checked',obj.checked);
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
	<td><? echo carregarOrientacao($abaativa); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione período de referência:</td>
	<td><? 
	carregarPeriodoReferencia(array('uncid'=>$_SESSION['sismedio'][$sis]['uncid'],'somentecombo'=>true,'fpbid'=>$_REQUEST['fpbid'],'pflcod_avaliador'=>$pflcod_avaliador))
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>

<? $sql_aux = exibirSituacaoMensario(array('uncid'=>$_SESSION['sismedio'][$sis]['uncid'],'retornarsql'=>true,'fpbid'=>$_REQUEST['fpbid'])); ?>
<? $sql_aux = "SELECT COALESCE(foo4.ap,0) FROM ({$sql_aux}) foo4 WHERE foo4.pflcod=p.pflcod"; ?>
<tr>
	<td class="SubTituloDireita">Selecione Perfil:</td>
	<td>
	<? 
    $sql = "SELECT p.pflcod as codigo, p.pfldsc || ' ( '||COALESCE(({$sql_aux}),0)||' )' as descricao FROM seguranca.perfil p WHERE p.pflcod IN('".PFL_FORMADORIES."',
																																				'".PFL_FORMADORREGIONAL."',
																																				'".PFL_SUPERVISORIES."',
																																				'".PFL_COORDENADORADJUNTOIES."',
																																				'".PFL_ORIENTADORESTUDO."',
																																				'".PFL_PROFESSORALFABETIZADOR."',
																																				'".PFL_COORDENADORPEDAGOGICO."',
																																				'".PFL_COORDENADORIES."') ORDER BY pfldsc";
    
    $db->monta_combo('pflcodaprovar', $sql, 'S', 'Selecione', 'selecionarFiltros', '', '', '', 'S', 'pflcodaprovar','', $_REQUEST['pflcodaprovar']); 
    ?> 
	</td>
</tr>
<? if($_REQUEST['pflcodaprovar']) : ?>
<? $consulta = verificaPermissao(); ?>
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
	<? if(!$consulta) : ?>
	<tr>
		<td colspan="2">
			<input type="checkbox" name="marcartodos" onclick="marcarTodos(this);"> <b>Marcar Todos</b>
		</td>
	</tr>
	<? endif; ?>
	</table>
	</form>
	<?
	
	
	$sql = "SELECT '<span style=white-space:nowrap;>".(($consulta)?"":"<input type=\"checkbox\" name=\"menid[]\" '||".criteriosAprovacao('restricao2')."||' value=\"'||foo.menid||'\">'||CASE WHEN foo.esdid=".ESD_ENVIADO_MENSARIO." THEN ' <img src=\"../imagens/valida3.gif\" style=\"cursor:pointer;\" onclick=\"mostrarJustificativa('||foo.docid||');\">' ELSE '' END ||' ")."<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verConsultar('||foo.menid||')\"></span>',
				  replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,																																												
				   foo.iusnome,
				   foo.pfldsc,
				   foo.rede,
				   CASE WHEN foo.mensarionota >= 7 THEN '<span style=color:blue;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>' 
				   		WHEN foo.mensarionota > 0 THEN '<span style=color:red;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>'
				   		ELSE '<span style=float:right;>Sem avaliação</span>' END  as mensarionota,
				   CASE WHEN foo.notacomplementar > 7 THEN '<span style=color:blue;float:right;>'||trim(to_char(foo.notacomplementar,'99d99'))||'</span>' 
				   	    WHEN foo.notacomplementar > 0 THEN '<span style=color:red;float:right;>'||trim(to_char(foo.notacomplementar,'99d99'))||'</span>'
				   		ELSE '<span style=float:right;>Sem avaliação complementar</span>' END as notacomplementar,
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
					cr.resposta,
				    t.tpeid,
					(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE pflcod=".PFL_FORMADORIES." AND d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO." AND p.uniid=unc.uniid) as numerobolsasformadoriespories,
					(SELECT COUNT(mapid) FROM sismedio.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
					(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia,
					(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
					COALESCE((SELECT AVG(mavtotal) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
					COALESCE((SELECT AVG(iccvalor) FROM sismedio.respostasavaliacaocomplementar r INNER JOIN sismedio.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid WHERE r.iusdavaliado=i.iusd AND r.fpbid = m.fpbid),0.00) as notacomplementar,
	                (SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
			    	(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
	                pp.plpmaximobolsas,
					m.fpbid,
					t.fpbidini,
					t.fpbidfim,
	                fu.rfuparcela,
					CASE WHEN mu.muncod IS NOT NULL THEN mu.estuf||' / '||mu.mundescricao 
					ELSE 'Equipe IES' END as rede
			FROM sismedio.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid  
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd 
			INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = i.uncid  
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.folhapagamentouniversidade fu ON fu.uncid = unc.uncid AND fu.fpbid = m.fpbid AND fu.pflcod = t.pflcod  
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			LEFT JOIN territorios.municipio mu ON mu.muncod = i.muncodatuacao 
			LEFT JOIN (
			SELECT COUNT(DISTINCT c.carid) as resposta, c.iusd FROM sismedio.cadernoatividadesrespostas c INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd WHERE caroeproposatividadecadernoformacao IS NOT NULL AND i.uncid='".$_SESSION['sismedio'][$sis]['uncid']."' AND c.fpbid='".$_REQUEST['fpbid']."' GROUP BY c.iusd
			) cr ON cr.iusd = m.iusd 
			WHERE d.esdid NOT IN('".ESD_APROVADO_MENSARIO."','".ESD_INVALIDADO_MENSARIO."') AND t.pflcod='".$_REQUEST['pflcodaprovar']."' AND i.uncid='".$_SESSION['sismedio'][$sis]['uncid']."' AND m.fpbid='".$_REQUEST['fpbid']."' ".(($_REQUEST['iusnome_f'])?"AND i.iusnome ILIKE '".$_REQUEST['iusnome_f']."%'":"")." ".(($_REQUEST['iuscpf_f'])?"AND i.iuscpf ILIKE '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf_f'])."%'":"")."  
			) foo
			ORDER BY 9, foo.mensarionota DESC, foo.iustermocompromisso, foo.notacomplementar DESC, foo.iusnome ASC";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Perfil","Rede","Nota avaliação","Nota avaliação complementar","Termo de Compromisso","Restrições");
	$db->monta_lista($sql,$cabecalho,150,5,'N','center','N','formulario','','',null,array('ordena'=>false));
 	?>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
	<? if(!$consulta) : ?>
	<input type="button" value="Aprovar" onclick="aprovarEquipe();">
	<? endif; ?>
	</td>
</tr>
<? endif; ?>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>

	</td>
</tr>

</table>