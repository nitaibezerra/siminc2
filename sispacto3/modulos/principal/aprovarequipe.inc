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
	<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':$sis)."&acao=A&aba=aprovarusuario"); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione período de referência:</td>
	<td><? 
	carregarPeriodoReferencia(array('uncid'=>$_SESSION['sispacto3'][$sis]['uncid'],'somentecombo'=>true,'fpbid'=>$_REQUEST['fpbid']))
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>

<? $sql_aux = exibirSituacaoMensario(array('uncid'=>$_SESSION['sispacto3'][$sis]['uncid'],'retornarsql'=>true,'fpbid'=>$_REQUEST['fpbid'])); ?>
<? $sql_aux = "SELECT foo4.ap FROM ({$sql_aux}) foo4 WHERE foo4.pflcod=p.pflcod"; ?>
<tr>
	<td class="SubTituloDireita">Selecione Perfil:</td>
	<td>
	<?
	 
    $sql = "SELECT p.pflcod as codigo, 
    			   p.pfldsc || ' ( '||COALESCE(({$sql_aux}),0)||' )' as descricao 
    		FROM seguranca.perfil p 
    		WHERE p.pflcod IN('".PFL_FORMADORIES."',
							  '".PFL_SUPERVISORIES."',
							  '".PFL_COORDENADORADJUNTOIES."',
							  '".PFL_COORDENADORLOCAL."',
							  '".PFL_ORIENTADORESTUDO."',
							  '".PFL_PROFESSORALFABETIZADOR."',
							  '".PFL_COORDENADORIES."')";
    
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
    	<td class="SubTituloDireita">Esfera</td>
    	<td>
    	<? 
		$arrEsfera = array(0 => array("codigo"=>"M","descricao"=>"Municipal"),
	    				   1 => array("codigo"=>"E","descricao"=>"Estadual")
	    				  );
	    $db->monta_combo('esfera_f', $arrEsfera, 'S', 'TODOS', '', '', '', '200', 'S', 'esfera_f','', $_REQUEST['esfera_f']); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF</td>
		<td><?
		$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
		$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '', $_REQUEST['estuf_endereco']); 
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município</td>
		<td id="td_municipio3">
		<? 
		
		if($_REQUEST['estuf_endereco']) :
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['estuf_endereco']."' ORDER BY mundescricao";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
		else :
		echo "Selecione uma UF";
		endif;

		?>
		</td>
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
				   '<span style=font-size:x-small;>'||replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</span>' as iuscpf,																																												
				   '<span style=font-size:x-small;>'||foo.iusnome||'</span>' as iusnome,
				   '<span style=font-size:x-small;>'||foo.pfldsc||'</span>' as pfldsc,
				   '<span style=font-size:x-small;>'||foo.rede||'</span>' as rede,
				   CASE WHEN foo.mensarionota >= 7 THEN '<span style=font-size:x-small;color:blue;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>' 
				   		WHEN foo.mensarionota > 0 THEN '<span style=font-size:x-small;color:red;float:right;>'||trim(to_char(foo.mensarionota,'99d99'))||'</span>'
				   		ELSE '<span style=float:right;>Sem avaliação</span>' END  as mensarionota,
				   CASE WHEN foo.notacomplementar > 7 THEN '<span style=font-size:x-small;color:blue;float:right;>'||trim(to_char(foo.notacomplementar,'99d99'))||'</span>' 
				   	    WHEN foo.notacomplementar > 0 THEN '<span style=font-size:x-small;color:red;float:right;>'||trim(to_char(foo.notacomplementar,'99d99'))||'</span>'
				   		ELSE '<span style=float:right;font-size:x-small;>Sem avaliação complementar</span>' END as notacomplementar,
				   CASE WHEN foo.iustermocompromisso=true THEN '<center><span style=color:blue;font-size:x-small;>Sim</span></center>' ELSE '<center><span style=color:red;font-size:x-small;>Não</span></center>' END as termocompromisso,
				   ".criteriosAprovacao('restricao1')."
			FROM (
			SELECT  m.menid,
					m.iusd,
					i.iuscpf,
					i.reuid,
					i.iusnome,
					i.iustermocompromisso, 
					p.pfldsc,
					p.pflcod,
					e.esdid,
					i.iusdocumento,
					i.iustipoprofessor,
					i.iusnaodesejosubstituirbolsa,
					d.docid, 
					--(SELECT COUNT(mapid) FROM sispacto3.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
					(SELECT COUNT(*) FROM sispacto3.turmasprofessoresalfabetizadores pa WHERE tpastatus='A' AND (coalesce(tpatotalmeninos,0)+coalesce(tpatotalmeninas,0))!=0 AND pa.iusd=m.iusd) as totalturmas,
					(SELECT COUNT(mavid) FROM sispacto3.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia,
					--(SELECT COUNT(*) FROM sispacto3.gestaomobilizacaoperguntas gm WHERE gm.iusd=m.iusd) as rcoordenadorlocal,
			
					(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
					 FROM sispacto3.aprendizagemconhecimentoturma a 
					 INNER JOIN sispacto3.aprendizagemconhecimento c ON c.catid = a.catid
					 INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
					 WHERE c.catstatus='A' AND t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat,
			
					--(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
					 --FROM sispacto3.aprendizagemconhecimentoturma2 a 
					 --INNER JOIN sispacto3.aprendizagemconhecimento c ON c.catid = a.catid
					 --INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
					 --WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat2,
			
					(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
					 FROM sispacto3.aprendizagemconhecimentoturma a 
					 INNER JOIN sispacto3.aprendizagemconhecimento c ON c.catid = a.catid
					 INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
					 WHERE c.catstatus='A' AND t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor,
			
					--(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
					 --FROM sispacto3.aprendizagemconhecimentoturma2 a 
					 --INNER JOIN sispacto3.aprendizagemconhecimento c ON c.catid = a.catid
					 --INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
					 --WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor2,
			
					--(SELECT count(*) as itens FROM sispacto3.usomateriaisdidaticos WHERE iusd=m.iusd) as aprendizagemUsoMateriaisDidaticos,
					--(SELECT count(*) as itens FROM sispacto3.relatoexperiencia WHERE iusd=m.iusd) as relatoexperiencia,
			
					--(SELECT count(*) as itens FROM sispacto3.impressoesana i INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON t.tpaid = i.tpaid AND i.iusd = t.iusd WHERE i.iusd=m.iusd) as impressoesana,
					--(SELECT count(*) as itens FROM sispacto3.questoesdiversasatv8 WHERE iusd=m.iusd) as questoesdiversasatv8,
					--(SELECT count(*) as itens FROM sispacto3.contribuicaopacto WHERE iusd=m.iusd) as contribuicaopacto,
			
					(SELECT COUNT(DISTINCT pflcodavaliador) FROM sispacto3.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
					(SELECT COUNT(DISTINCT fpbid) FROM sispacto3.pagamentobolsista pg WHERE pg.iusd=m.iusd) as qtduspagamento,
					(SELECT COUNT(DISTINCT fpbid) FROM sispacto3.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as qtdtppagamento,
					COALESCE((SELECT AVG(mavtotal) FROM sispacto3.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
					COALESCE((SELECT AVG(iccvalor) FROM sispacto3.respostasavaliacaocomplementar r INNER JOIN sispacto3.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid WHERE r.iusdavaliado=i.iusd AND r.fpbid = m.fpbid),0.00) as notacomplementar,
					m.fpbid,
					t.fpbidini,
					t.fpbidfim,
					pp.plpmaximobolsas,
					fpu.rfuparcela,
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||'/'||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||'/'||m2.mundescricao||' ( Estadual )' 
														END 
						 ELSE 'Equipe IES' END as rede,
					dorien.esdid as esdorien,
					dturpr.esdid as esdturpr
			FROM sispacto3.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid  
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = m.iusd 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND m.pflcod = t.pflcod 
			INNER JOIN sispacto3.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod AND fpu.fpbid = m.fpbid 			 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			LEFT JOIN sispacto3.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN workflow.documento dorien ON dorien.docid = pic.docid 
			LEFT JOIN workflow.documento dturpr ON dturpr.docid = pic.docidturma 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE d.esdid NOT IN('".ESD_APROVADO_MENSARIO."','".ESD_INVALIDADO_MENSARIO."') AND t.pflcod='".$_REQUEST['pflcodaprovar']."' AND i.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."' AND m.fpbid='".$_REQUEST['fpbid']."' 
				  ".(($_REQUEST['iusnome_f'])?"AND i.iusnome ILIKE '".$_REQUEST['iusnome_f']."%'":"")." 
				  ".(($_REQUEST['iuscpf_f'])?"AND i.iuscpf ILIKE '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf_f'])."%'":"")."
				  ".(($_REQUEST['esfera_f'])?"AND ".(($_REQUEST['esfera_f']=='M')?"pic.muncod IS NOT NULL":"pic.estuf IS NOT NULL"):"")."
				  ".(($_REQUEST['estuf_endereco'])?"AND (m1.estuf='".$_REQUEST['estuf_endereco']."' OR m2.estuf='".$_REQUEST['estuf_endereco']."')":"")." 
				  ".(($_REQUEST['muncod_endereco'])?"AND (m1.muncod='".$_REQUEST['muncod_endereco']."' OR m2.muncod='".$_REQUEST['muncod_endereco']."')":"")."
			) foo
			ORDER BY 9, foo.mensarionota DESC, foo.iustermocompromisso, foo.notacomplementar DESC, foo.iusnome ASC 
			";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>CPF</span>","<span style=font-size:x-small;>Nome</span>","<span style=font-size:x-small;>Perfil</span>","<span style=font-size:x-small;>Rede</span>","<span style=font-size:x-small;>Nota avaliação</span>","<span style=font-size:x-small;>Nota avaliação complementar</span>","<span style=font-size:x-small;>Termo de Compromisso</span>","<span style=font-size:x-small;>Restrições</span>");
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
		<?=criarBotoesNavegacao(array('url' => '/sispacto3/sispacto3.php?modulo=principal/'.$sis.'/'.(($sis=='universidade')?'universidadeexecucao':$sis).'&acao=A&aba=aprovarusuario')) ?>
		</td>
	</tr>

</table>