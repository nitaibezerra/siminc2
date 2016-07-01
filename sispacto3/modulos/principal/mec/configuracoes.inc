<?
$sql = "SELECT uu.unisigla ||' - '|| uu.uninome as uninome, u.uncid FROM sispacto3.universidadecadastro u 
		INNER JOIN sispacto3.universidade uu ON uu.uniid = u.uniid 
		INNER JOIN workflow.documento d ON d.docid = u.docidorcamento
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidestruturaformacao
		INNER JOIN workflow.documento d3 ON d3.docid = u.docidequipeies 
		INNER JOIN workflow.documento d4 ON d4.docid = u.dociddadosprojeto 
		INNER JOIN workflow.documento d5 ON d5.docid = u.docidturmas
		WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
			  d2.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
			  d3.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
			  d4.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
			  d5.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND
			  u.usucpfparecer IS NOT NULL
		ORDER BY 1";

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

function gravarPeriodoReferencia() {
	jQuery('#periodoreferencia').submit();
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


</script>


<div id="modalPeriodosReferencia" style="display:none;"></div>

<form method=post name="periodoreferencia" id="periodoreferencia">
<input type="hidden" name="requisicao" value="cadastrarPeriodoReferencia">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Orientações</td>
	<td colspan="2">
	<? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/mec/mec&acao=A&aba=configuracoes"); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro">Universidade</td>
	<td class="SubTituloCentro">Período de referência(Início)</td>
	<td class="SubTituloCentro">Período de referência(Término)</td>
</tr>
<? if($universidadecadastro[0]) : ?>
<? foreach($universidadecadastro as $uc) : 

$sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sispacto3.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer"; 
$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sispacto3.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";

unset($arrini,$arrfim);

// recuperando informações salvas
$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim  
						  FROM sispacto3.folhapagamentouniversidade fu 
						  INNER JOIN sispacto3.folhapagamento f ON f.fpbid = fu.fpbid 
						  WHERE fu.uncid='".$uc['uncid']."' AND fu.pflcod IS NULL");

$arrini = explode("-",$arr['mesanoini']);
$arrfim = explode("-",$arr['mesanofim']);

$hab = 'S';

?>
<tr>
	<td class="SubTituloDireita"><?=$uc['uninome'] ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesini['.$uc['uncid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesini_'.$uc['uncid'],'', $arrini[1]); ?> / <? $db->monta_combo($hab.'anoinicio['.$uc['uncid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anoinicio','', $arrini[0]); ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesfim['.$uc['uncid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesfim_'.$uc['uncid'],'', $arrfim[1]); ?> / <? $db->monta_combo($hab.'anofim['.$uc['uncid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anofim','', $arrfim[0]); ?></td>
</tr>
<?
 
if($arrini[0] && $arrfim[0]) { 

	$perfis = $db->carregar("SELECT * FROM seguranca.perfil p 
							 INNER JOIN sispacto3.pagamentoperfil pp ON pp.pflcod = p.pflcod 
							 ORDER BY p.pflnivel");
	
	foreach($perfis as $pfl) {

		$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoinip, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofimp
								  FROM sispacto3.folhapagamentouniversidade fu
								  INNER JOIN sispacto3.folhapagamento f ON f.fpbid = fu.fpbid
								  WHERE fu.uncid='".$uc['uncid']."' AND fu.pflcod='".$pfl['pflcod']."'");
		
		$arrini = explode("-",$arr['mesanoinip']);
		$arrfim = explode("-",$arr['mesanofimp']);



		echo '<tr>';
		echo '<td align=right><img src=../imagens/report.gif style=cursor:pointer; onclick="exibirPeriodosReferencia('.$uc['uncid'].','.$pfl['pflcod'].');"> <img src=../imagens/seta_filho.gif> '.$pfl['pfldsc'].'</td>';
		echo '<td align="center">';
		$db->monta_combo($hab.'mesinip['.$uc['uncid'].']['.$pfl['pflcod'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesinip_'.$uc['uncid'].'_'.$pfl['pflcod'],'', $arrini[1]); 
		echo ' / ';
		$db->monta_combo($hab.'anoiniciop['.$uc['uncid'].']['.$pfl['pflcod'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anoiniciop','', $arrini[0]);
		echo '</td>';
		echo '<td align="center">';
		$db->monta_combo($hab.'mesfimp['.$uc['uncid'].']['.$pfl['pflcod'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesfimp_'.$uc['uncid'].'_'.$pfl['pflcod'],'', $arrfim[1]); 
		echo ' / ';
		$db->monta_combo($hab.'anofimp['.$uc['uncid'].']['.$pfl['pflcod'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anofimp','', $arrfim[0]);
		echo '</td>';
		echo '</tr>';
		
	}

}

?>

<? endforeach; ?>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<?=criarBotoesNavegacao(array('url' => "/sispacto3/sispacto3.php?modulo=principal/mec/mec&acao=A&aba=configuracoes",'funcao' => 'gravarPeriodoReferencia')) ?>
	</td>
</tr>
</table>
</form>
