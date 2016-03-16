<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 

<script>

function carregarListaCustos_s(alpid, sifid) {
	ajaxatualizar('requisicao=carregarListaCustos&sifid='+sifid+'&alpid='+alpid,'div_listacustos_s');
}

function autorizarAlteracao(alpid, alpautorizado, sifid) {

	switch(alpautorizado) {
		case '2':
			var conf = confirm('Deseja realmente autorizar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid='+sifid+'&alpautorizado=2&alpid='+alpid,'');
			
			break;

		case '3':
			var conf = confirm('Deseja realmente recusar a alteração do orçamento?');

			if(!conf) {
				return false;
			}

			ajaxatualizar('requisicao=confirmarSolicitacao&sifid='+sifid+'&alpautorizado=3&alpid='+alpid,'');
			
			break;
		
	}

	window.location=window.location;
	
	
}

function exibirSolicitacaoAlteracaoOrcamento(alpid, alpsituacao, sifid) {

	jQuery('#inserircustos_s').css('display','none');
	jQuery('#confirmarsolicitacao_orcamento').css('display','none');
	jQuery('#cancelarsolicitacao_orcamento').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=orcamento&sifid='+sifid+'&alpid='+alpid,'div_listacustos_anterior');
	jQuery('#tbl_situacaoanterior_orcamento').css('display','');
	
	carregarListaCustos_s(alpid, sifid);

	jQuery("#modalSolicitacaoOrcamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function exibirSolicitacaoAlteracaoBolsas(alpid, alpsituacao, sifid) {

	jQuery('#confirmarsolicitacao_bolsas').css('display','none');
	jQuery('#cancelarsolicitacao_bolsas').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=bolsas&sifid='+sifid+'&alpid='+alpid,'div_listabolsas_anterior');
	jQuery('#tbl_situacaoanterior_bolsas').css('display','');
	
	ajaxatualizar('requisicao=carregarQuantitativoPorPerfil&consulta=1&sifid='+sifid+'&alpid='+alpid,'td_carregarQuantitativoPorPerfil');

	jQuery("#modalSolicitacaoBolsas").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}

function exibirSolicitacaoAlteracaoMeta(alpid, alpsituacao, sifid) {

	jQuery('#confirmarsolicitacao_meta').css('display','none');
	jQuery('#cancelarsolicitacao_meta').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=meta&sifid='+sifid+'&alpid='+alpid,'div_listameta_anterior');
	jQuery('#tbl_situacaoanterior_meta').css('display','');
	
	ajaxatualizar('requisicao=carregarMeta_s&consulta=1&sifid='+sifid+'&alpid='+alpid,'td_meta');

	jQuery("#modalSolicitacaoMeta").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}


function exibirSolicitacaoAlteracaoVigencia(alpid, alpsituacao, sifid) {

	jQuery('#confirmarsolicitacao_vigencia').css('display','none');
	jQuery('#cancelarsolicitacao_vigencia').css('display','none');

	ajaxatualizar('requisicao=carregarHistoricoSolicitacao&tipo=vigencia&sifid='+sifid+'&alpid='+alpid,'div_listavigencia_anterior');
	jQuery('#tbl_situacaoanterior_vigencia').css('display','');
	
	ajaxatualizar('requisicao=carregarVigencia_s&consulta=1&sifid='+sifid+'&alpid='+alpid,'td_vigencia');

	jQuery("#modalSolicitacaoVigencia").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

	
}


</script>

<div id="modalSolicitacaoOrcamento" style="display:none;">

<input type="hidden" name="alpid" id="alpid" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos_s" onclick="inserirCustos_s('');"></p>
		<div id="div_listacustos_s"></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_orcamento" onclick="confirmarSolicitacao();"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_orcamento" onclick="jQuery('#modalSolicitacaoOrcamento').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_orcamento" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<div id="div_listacustos_anterior"></div>
		</td>
	</tr>

</table>

</div>

<div id="modalSolicitacaoBolsas" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>

	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td id="td_carregarQuantitativoPorPerfil"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_bolsas" onclick="gravarBolsas_s();confirmarSolicitacao();"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_bolsas" onclick="jQuery('#modalSolicitacaoBolsas').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_bolsas" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td>
		<div id="div_listabolsas_anterior"></div>
		</td>
	</tr>

</table>

</div>


<div id="modalSolicitacaoMeta" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td id="td_meta"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_meta" onclick="gravarMeta_s();confirmarSolicitacao();"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_meta" onclick="jQuery('#modalSolicitacaoMeta').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_meta" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td>
		<div id="div_listameta_anterior"></div>
		</td>
	</tr>

</table>

</div>


<div id="modalSolicitacaoVigencia" style="display:none;">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- NOVA SOLICITAÇÃO ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Vigência</td>
		<td id="td_vigencia"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" value="Confirmar solicitação" id="confirmarsolicitacao_vigencia" onclick="gravarVigencia_s();confirmarSolicitacao();"> <input type="button" value="Cancelar solicitação" id="cancelarsolicitacao_vigencia" onclick="jQuery('#modalSolicitacaoVigencia').dialog('close');"></td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbl_situacaoanterior_vigencia" style="display:none;">
	<tr>
		<td class="SubTituloCentro" colspan="2">---- SITUAÇÃO ANTERIOR ----</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Vigência</td>
		<td>
		<div id="div_listavigencia_anterior"></div>
		</td>
	</tr>

</table>

</div>

<p align="center"><b>Solicitações de alteração de orçamento</b></p>

<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Tipo de alteração</td>
	<td><?

	$tipo_alteracao = array(0 => array('codigo'=>'orcamento','descricao'=>'Orçamento do curso'),
							1 => array('codigo'=>'bolsas','descricao'=>'Qtd de bolsas por perfil'),
							2 => array('codigo'=>'meta','descricao'=>'Meta do curso'),
							3 => array('codigo'=>'vigencia','descricao'=>'Vigência do curso'),
							);

	$db->monta_combo('alptipo', $tipo_alteracao, 'S', 'Selecione', '', '', '', '200', 'N', 'alptipo', '', $_REQUEST['alptipo']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">IES</td>
	<td>
	<?

	$sql = "SELECT 	DISTINCT ent.entunicod as codigo, COALESCE(ent.entsig||' - ','')||ent.entnome as descricao
					FROM 		entidade.entidade ent
					INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11)
					INNER JOIN sisfor.sisfories si ON si.unicod = ent.entunicod
					WHERE ent.entstatus='A' AND ent.entunicod IS NOT NULL
					ORDER BY 2";
		
	$db->monta_combo('unicod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'unicod', '', $_REQUEST['unicod']);
	
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">Curso</td>
	<td>
	<? 
	$nomecurso = $_REQUEST['nomecurso'];
	echo campo_texto('nomecurso', 'N', 'S', 'Nome do curso', '50', '', '', '','','');
	?>
	</td>
</tr>
<tr>
	<td class="subtituloDireita">Secretaria:</td>
	<td>
	<?php 
	$sql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao WHERE coorano='".$_SESSION['exercicio']."' ORDER BY coordid";
	$db->monta_combo('coordid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'coordid', '', $_REQUEST['coordid']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location='sisfor.php?modulo=principal/mec/mec&acao=A&aba=alterarprojeto';"></td>
</tr>
</table>
</form>

<? 
$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
	$param = "<img align=absmiddle src=../imagens/valida1.gif style=cursor:pointer; onclick=\"autorizarAlteracao('||a.alpid||',\'2\','||a.sifid||');\"> <img align=absmiddle src=../imagens/valida3.gif style=cursor:pointer;  onclick=\"autorizarAlteracao('||a.alpid||',\'3\','||a.sifid||');\">";
}

if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
	$coordids = $db->carregarColuna("SELECT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod IN('".PFL_EQUIPE_MEC."','".PFL_COORDENADOR_MEC."','".PFL_DIRETOR_MEC."') ANd rpustatus='A'");

	if($coordids)
		$wh[] = "(cur.coordid IN(".implode(",",$coordids).") OR cur2.coordid IN(".implode(",",$coordids).") OR oc.coordid IN(".implode(",",$coordids).") OR cor4.coordid IN(".implode(",",$coordids)."))";

}

if($_REQUEST['alptipo']) {
	$wh[] = "a.alptipo='".$_REQUEST['alptipo']."'";
}

if($_REQUEST['unicod']) {
	$wh[] = "s.unicod='".$_REQUEST['unicod']."'";
}

if($_REQUEST['nomecurso']) {
	$wh[] = "(cur.curid||' '||cur.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR oc.ocunome ilike '%".$_REQUEST['nomecurso']."%' OR oat.oatnome ilike '%".$_REQUEST['nomecurso']."%')";
}

if($_REQUEST['coordid']) {
	$wh[] = "(cor.coordid='".$_REQUEST['coordid']."' OR cor2.coordid='".$_REQUEST['coordid']."' OR cor3.coordid='".$_REQUEST['coordid']."')";
}



$sql = "UPDATE sisfor.alterarprojeto SET alpjustificativa=NULL WHERE alpjustificativa='null'";
$db->executar($sql);
$db->commit();


$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"'||CASE WHEN a.alptipo='orcamento' THEN 'exibirSolicitacaoAlteracaoOrcamento' 
																						  WHEN a.alptipo='bolsas' THEN 'exibirSolicitacaoAlteracaoBolsas'
																						  WHEN a.alptipo='meta' THEN 'exibirSolicitacaoAlteracaoMeta' 
																						  WHEN a.alptipo='vigencia' THEN 'exibirSolicitacaoAlteracaoVigencia'
																					      END||'('||a.alpid||','||a.alpautorizado||','||a.sifid||')\">' as acao,
				'<span style=font-size:x-small><b>'||CASE WHEN a.alptipo='orcamento' THEN 'Orçamento do curso' 
					 WHEN a.alptipo='bolsas' THEN 'Qtd de bolsas por perfil'
					 WHEN a.alptipo='meta' THEN 'Meta do curso' 
					 WHEN a.alptipo='vigencia' THEN 'Vigência do curso'
					 END||'</b></span>' as tipo, 																							      
				'<span style=font-size:x-small>'||uu.uniabrev||' - '||uu.unidsc||'</span>' as universidade, 
				'<span style=font-size:x-small>'||case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
				     when s.ocuid is not null then oc.ocunome 
				     when s.oatid is not null then oatnome end||'</span>' as curso,
				'<span style=font-size:x-small>'||case when s.ieoid is not null then cor.coordsigla 
				     when s.cnvid is not null then cor2.coordsigla 
				     when s.ocuid is not null then cor3.coordsigla 
				     when s.oatid is not null then cor4.coordsigla end||'</span>' as secretaria,
			   '<span style=font-size:x-small>'||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )</span>' as solicitacao,
			   coalesce(alpjustificativa,'-') as justificativa,
			   '<span style=font-size:x-small>'||CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
					WHEN alpautorizado='2' THEN 'AUTORIZADO'
					WHEN alpautorizado='3' THEN 'RECUSADO' END||CASE WHEN alpdtautorizou IS NOT NULL THEN ' ( '||u2.usunome||' - '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )' ELSE '' END||'</span>' as situacao
		FROM sisfor.alterarprojeto a 
		INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
		LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
		LEFT JOIN sisfor.sisfor s ON s.sifid = a.sifid 
		LEFT JOIN public.unidade uu on uu.unicod = s.unicod
		left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
		left join catalogocurso2014.curso cur on cur.curid = ieo.curid
		left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
		left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
		left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
		left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
		left join seguranca.usuario usu on usu.usucpf = s.usucpf
		left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
		left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
		left join sisfor.outraatividade oat on oat.oatid = s.oatid 
		left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		
		WHERE alpautorizado in('1','2','3') ".(($wh)?"and ".implode(" AND ", $wh):"")." 
		ORDER BY a.alpdtsolicitou";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $key => $ar) {
		$arr[$key]['justificativa'] = '<textarea rows="3" cols="40" disabled>'.$ar['justificativa'].'</textarea>';
	}
}

if(!$arr) $arr = array();

$cabecalho = array("&nbsp;", "Tipo de alteração","IES", "Curso", "Secretaria", "Usuário que solicitou", "Justificativa", "Situação");
$db->monta_lista_simples($arr, $cabecalho, 100000, 5, 'N', '100%', 'N');


?>
