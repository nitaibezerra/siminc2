<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 

<script>

function detalhar(sotid) {

	ajaxatualizar('requisicao=exibirDetalhesSubstituicao&iusd_homologar=<?=$iusd_homologar ?>&pflabrev=<?=$pflabrev ?>&sotid='+sotid,'modalDetalhamento');

	jQuery("#modalDetalhamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function atualizarSolicitacaoTroca(sotid, situacao) {
	if(jQuery('#sotdescricao<?=$pflabrev ?>').val()=='') {
		alert('Preencha seu parecer');
		return false;
	}

	jQuery('#situacao').val(situacao);

	jQuery('#formulario_justificativa').submit();
}

</script>

<div id="modalDetalhamento" style="display:none;"></div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><?=$titulo ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td><? echo carregarOrientacao($abaativa); ?></td>
</tr>
<tr>
	<td colspan="2"><?
	
	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"detalhar('||s.sotid||')\">' as img, 
					replace(to_char(i1.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i1.iusnome||' ( '||p1.pfldsc||' )' as sub1, 
					replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i2.iusnome||' ( '||p2.pfldsc||' )' as sub2,
					uni.unisigla||' - '||uni.uninome as universidade
					 
			FROM sismedio.solicitacaotroca s 
			INNER JOIN sismedio.identificacaousuario i1 ON i1.iusd = s.iusdantigo 
			INNER JOIN sismedio.tipoperfil t1 ON t1.iusd = i1.iusd 
			INNER JOIN seguranca.perfil p1 ON p1.pflcod = t1.pflcod 
			INNER JOIN sismedio.identificacaousuario i2 ON i2.iusd = s.iusdnovo 
			INNER JOIN sismedio.tipoperfil t2 ON t2.iusd = i2.iusd 
			INNER JOIN seguranca.perfil p2 ON p2.pflcod = t2.pflcod
			INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = s.uncid 
			INNER JOIN sismedio.universidade uni ON uni.uniid = unc.uniid 
			WHERE sotstatus='A' {$wh}";
	
	$cabecalho = array("&nbsp;","Substituído (Sai)","Substituto (Entra)","Universidade");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2,true,false,false,true);
	
	?></td>
</tr>

</table>