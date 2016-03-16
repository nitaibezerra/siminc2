<? verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sisindigena']['universidade']['uncid'])); ?>
<script>
function selecionarEixoPedagogico(obj) {
	if(obj.checked) {
		jQuery('#tr_eprobs_'+obj.value).css('display','');
	} else {
		jQuery('#tr_eprobs_'+obj.value).css('display','none');
		jQuery('#eprobs_'+obj.value).val('');
	}
}

function salvarProjetoPedagogico(goto) {
	divCarregando();
	jQuery('#goto').val(goto);
	jQuery('#formulario').submit();
}
</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="gravarProjetoPedagogico">
<input type="hidden" name="uncid" value="<?=$_SESSION['sisindigena']['universidade']['uncid'] ?>">
<input type="hidden" name="goto" id="goto" value="">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Projeto Pedagógico</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Eixos Pedagógicos</td>
		<td>
		<? 
 		$sql = "SELECT * FROM sisindigena.eixospedagogicos WHERE expstatus='A'";
 		$eixospedagogicos = $db->carregar($sql);
		?>
		
		<? if($eixospedagogicos[0]) : ?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<? foreach($eixospedagogicos as $ep) : ?>
		<? 
		$eixospedagogicosuniversidade = $db->pegaLinha("SELECT epuid, eprobs FROM sisindigena.eixospedagogicosuniversidade WHERE expid='".$ep['expid']."' AND uncid='".$_SESSION['sisindigena']['universidade']['uncid']."'");
		$obs = 'eprobs_'.$ep['expid'];
		$$obs = $eixospedagogicosuniversidade['eprobs'];
		?>
		<tr>
			<td width="5"><input type="checkbox" name="expid[]" value="<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"checked":"") ?> onclick="selecionarEixoPedagogico(this);"></td>
			<td><?=$ep['expdsc'] ?></td>
		</tr>
		<tr id="tr_eprobs_<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"":"style=\"display:none;\"") ?> >
			<td colspan="2"><? echo campo_textarea( 'eprobs_'.$ep['expid'], 'S', 'S', '', '70', '4', '1000'); ?></td>
		</tr>
		<? endforeach; ?>
		</table>
		<? else : ?>
		Não existem eixos pedagógicos
		<? endif; ?>
		</td>
	</tr>	

	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=dados';">
			<? if(!$consulta) : ?> 
				<input type="button" name="salvar" value="Salvar" onclick="salvarProjetoPedagogico('sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=projetopedagogico');"> 
				<input type="button" value="Salvar e Continuar" onclick="salvarProjetoPedagogico('sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=orcamento');"> 
			<? endif; ?>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso';">
		</td>
	</tr>
</table>
</form>