<?
$sql = "SELECT uu.unisigla ||' - '|| uu.uninome as uninome, u.uncid FROM sispacto.universidadecadastro u
		INNER JOIN workflow.documento d1 ON d1.docid = u.docid 
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
		INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid 
		WHERE d1.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."'";

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
		window.location='sispacto.php?modulo=principal/mec/mec&acao=A&requisicao=carregarOrientadoresSISPorMunicipio&picid='+document.getElementById('picid').value;
	}
}
</script>
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

$sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sispacto.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer"; 
$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sispacto.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";

unset($arrini,$arrfim);

// recuperando informações salvas
$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim  
						  FROM sispacto.folhapagamentouniversidade fu 
						  INNER JOIN sispacto.folhapagamento f ON f.fpbid = fu.fpbid 
						  WHERE fu.uncid='".$uc['uncid']."'");

$arrini = explode("-",$arr['mesanoini']);
$arrfim = explode("-",$arr['mesanofim']);

if($arrini[0] || $arrfim[0]) $hab = 'N';
else $hab = 'S';

if(in_array(PFL_SUPERUSUARIO,$perfis)) {
	$hab = 'S';
}

?>
<tr>
	<td class="SubTituloDireita"><?=$uc['uninome'] ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesini['.$uc['uncid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesini_'.$uc['uncid'],'', $arrini[1]); ?> / <? $db->monta_combo($hab.'anoinicio['.$uc['uncid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anoinicio','', $arrini[0]); ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesfim['.$uc['uncid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesfim_'.$uc['uncid'],'', $arrfim[1]); ?> / <? $db->monta_combo($hab.'anofim['.$uc['uncid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anofim','', $arrfim[0]); ?></td>
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
			FROM sispacto.pactoidadecerta p 
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
	<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario';">
	</td>
</tr>
</table>
</form>
