<?
$sql = "SELECT uu.unisigla ||' - '|| uu.uninome as uninome, u.uncid FROM sismedio.universidadecadastro u
		INNER JOIN workflow.documento d1 ON d1.docid = u.docid 
		INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
		WHERE d1.esdid='".ESD_VALIDADO_COORDENADOR_IES."' ORDER BY 1";

$universidadecadastro = $db->carregar($sql);

$perfis = pegaperfilGeral();

?>
<script>

<? if(in_array(PFL_CONSULTAMEC,$perfis)) : ?>
jQuery(document).ready(function() {
	jQuery('#picid').attr('disabled','disabled');
	jQuery('#carregarOrientadoresSIS').css('display','none');
	jQuery('#salvaconfiguracoes').css('display','none');
});
<? endif; ?>


function carregarOrientadoresSIS() {
	if(document.getElementById('picid').value=='') {
		alert('Selecione um município/estado');
		return false;
	}
	
	var conf = confirm('Deseja realmente carregar este Município/Estado com Orientadores de Estudo SIS?');
	if(conf) {
		window.location='sismedio.php?modulo=principal/mec/mec&acao=A&requisicao=carregarOrientadoresSISPorMunicipio&picid='+document.getElementById('picid').value;
	}
}

function inserirTipoAvaliacao(uncid,pflcod,fpbid,obj) {
	if(obj.checked) {
		ajaxatualizar('requisicao=inserirTipoAvaliacao&uncid='+uncid+'&pflcod='+pflcod+'&fpbid='+fpbid+'&tipo=inserir','');
	} else {
		ajaxatualizar('requisicao=inserirTipoAvaliacao&uncid='+uncid+'&pflcod='+pflcod+'&fpbid='+fpbid+'&tipo=remover','');
	}
}

function exibirPeriodosReferencia(uncid,pflcod) {
	ajaxatualizar('requisicao=exibirPeriodosReferenciaUniversidade&uncid='+uncid+'&pflcod='+pflcod,'modalPeriodosReferencia');
	
	jQuery("#modalPeriodosReferencia").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function atualizarFolhaPagamento(rfuid, obj) {
	var linha = obj.parentNode.parentNode;
	
	if(obj.checked) {
		linha.cells[4].innerHTML = 'Inativo';
		ajaxatualizar('requisicao=atualizarPeriodosReferenciaUniversidade&rfuid='+rfuid+'&status=I','');
		
	} else {
		linha.cells[4].innerHTML = 'Ativo';
		ajaxatualizar('requisicao=atualizarPeriodosReferenciaUniversidade&rfuid='+rfuid+'&status=A','');
	}
	
}

function carregaDetalhesPeriodoReferencia(uncid, obj) {
	if(obj.checked) {
		jQuery('#tr_periodo_'+uncid).css('display','');
		ajaxatualizar('requisicao=carregaDetalhesPeriodoReferencia&uncid='+uncid,'td_periodo_'+uncid);
	} else {
		jQuery('#tr_periodo_'+uncid).css('display','none');
		jQuery('#td_periodo_'+uncid).html('');
	}
	
}

</script>

<div id="modalPeriodosReferencia" style="display:none;"></div>

<form method=post name="periodoreferencia" id="periodoreferencia">
<input type="hidden" name="requisicao" value="cadastrarPeriodoReferencia">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Orientações</td>
	<td colspan="2">
	Definindo períodos de referência de cada universidade, esta definição ocorrerá quando a universidade for validada e tiver o Registro de Frequência fechado.
	</td>
</tr>
<tr>
	<td class="SubTituloCentro">Universidade</td>
	<td class="SubTituloCentro">Período de referência(Início)</td>
	<td class="SubTituloCentro">Período de referência(Término)</td>
</tr>
<? if($universidadecadastro[0]) : ?>
<? foreach($universidadecadastro as $uc) : 

$sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sismedio.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer";
$arrMES = $db->carregar($sql_mes); 
$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sismedio.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";
$arrANO = $db->carregar($sql_ano);

unset($arrini,$arrfim);

// recuperando informações salvas
$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim  
						  FROM sismedio.folhapagamentouniversidade fu 
						  INNER JOIN sismedio.folhapagamento f ON f.fpbid = fu.fpbid 
						  WHERE fu.uncid='".$uc['uncid']."' AND fu.pflcod IS NULL");

$arrini = explode("-",$arr['mesanoini']);
$arrfim = explode("-",$arr['mesanofim']);

$hab = 'S';

?>
<tr>
	<td class="SubTituloDireita"><input type="checkbox" name="uncid_atualizar[]" value="<?=$uc['uncid'] ?>" <?=((!$arrini[0] || !$arrfim[0])?"disabled":"") ?> onclick="carregaDetalhesPeriodoReferencia('<?=$uc['uncid'] ?>', this);"><?=$uc['uninome'] ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesini['.$uc['uncid'].']', $arrMES, $hab, 'Selecione', '', '', '', '', 'N', 'mesini_'.$uc['uncid'],'', $arrini[1]); ?> / <? $db->monta_combo($hab.'anoinicio['.$uc['uncid'].']', $arrANO, $hab, 'Selecione', '', '', '', '', 'N', 'anoinicio','', $arrini[0]); ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesfim['.$uc['uncid'].']', $arrMES, $hab, 'Selecione', '', '', '', '', 'N', 'mesfim_'.$uc['uncid'],'', $arrfim[1]); ?> / <? $db->monta_combo($hab.'anofim['.$uc['uncid'].']', $arrANO, $hab, 'Selecione', '', '', '', '', 'N', 'anofim','', $arrfim[0]); ?></td>
</tr>
<tr id="tr_periodo_<?=$uc['uncid'] ?>" style="display:none">
	<td align="center" id="td_periodo_<?=$uc['uncid'] ?>" colspan="3"></td>
</tr>
<? endforeach; ?>
<? endif; ?>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Carregando com Orientadores de Estudo SIS (Robôs)</td>
</tr>
<tr>
	<td class="SubTituloDireita">Selecione Município/Estado:</td>
	<td colspan="2">
	<? 
	$sql = "SELECT p.picid as codigo, 
				   CASE WHEN p.muncod IS NOT NULL THEN 'MUNICIPAL : ' || m.estuf || ' / ' || m.mundescricao
				   		WHEN p.estuf IS NOT NULL THEN 'ESTADUAL : ' || e.estuf || ' / ' || e.estdescricao 
				   	END as descricao 
			FROM sismedio.pactoidadecerta p 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_LOCAL."'
			ORDER BY 2";
	
	$db->monta_combo('picid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'picid','');
	?> 
	<input type="button" name="carregar" id="carregarOrientadoresSIS" value="Carregar" onclick="carregarOrientadoresSIS();">
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="submit" id="salvaconfiguracoes" value="Salvar">
	<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario';">
	</td>
</tr>
</table>
</form>
