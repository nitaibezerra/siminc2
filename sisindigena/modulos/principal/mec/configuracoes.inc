<?
$sql = "SELECT uu.unisigla ||' - '|| uu.uninome as uninome, u.uncid, u.picid FROM sisindigena.nucleouniversidade u
		INNER JOIN workflow.documento d1 ON d1.docid = u.docid 
		INNER JOIN sisindigena.universidade uu ON uu.uniid = u.uniid 
		WHERE d1.esdid='".ESD_VALIDADO_COORDENADOR_IES."'";

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
		window.location='sisindigena.php?modulo=principal/mec/mec&acao=A&requisicao=carregarOrientadoresSISPorMunicipio&picid='+document.getElementById('picid').value;
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

$sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sisindigena.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer"; 
$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sisindigena.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";

unset($arrini,$arrfim);

// recuperando informações salvas
$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim  
						  FROM sisindigena.folhapagamentouniversidade fu 
						  INNER JOIN sisindigena.folhapagamento f ON f.fpbid = fu.fpbid 
						  WHERE fu.uncid='".$uc['uncid']."' AND fu.picid='".$uc['picid']."'");

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
	<td align="center"><? $db->monta_combo($hab.'mesini['.$uc['uncid'].']['.$uc['picid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesini_'.$uc['uncid'].'_'.$uc['picid'],'', $arrini[1]); ?> / <? $db->monta_combo($hab.'anoinicio['.$uc['uncid'].']['.$uc['picid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anoinicio','', $arrini[0]); ?></td>
	<td align="center"><? $db->monta_combo($hab.'mesfim['.$uc['uncid'].']['.$uc['picid'].']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesfim_'.$uc['uncid'].'_'.$uc['picid'],'', $arrfim[1]); ?> / <? $db->monta_combo($hab.'anofim['.$uc['uncid'].']['.$uc['picid'].']', $sql_ano, $hab, 'Selecione', '', '', '', '', 'N', 'anofim','', $arrfim[0]); ?></td>
</tr>
<? endforeach; ?>
<? endif; ?>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="submit" id="salvaconfiguracoes" value="Salvar">
	<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario';">
	</td>
</tr>
</table>
</form>
