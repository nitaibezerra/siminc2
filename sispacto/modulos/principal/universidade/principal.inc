<script>
function selecionarPeriodoReferencia(fpbid) {

	if(fpbid!='') {
		ajaxatualizar('requisicao=exibirSituacaoMensario&fpbid='+fpbid+'&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','dv_situacao_mensario');
		ajaxatualizar('requisicao=exibirPorcentagemPagamento&fpbid='+fpbid+'&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','dv_porcentagem_pagamento');
	}
	
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	
	<? carregarInformes(array('pflcoddestino' => PFL_COORDENADORIES)); ?>
	
	</td>
</tr>
	<tr>
		<td colspan="2">
			<table width="100%">
			<tr>
				<td width="50%" valign="top">
				<p align="center">Resumo acesso dos usuários ao SIMEC</p>
				<?
				exibirAcessoUsuarioSimec(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				<p align="center">Materiais</p>
				<? 
				exibirMateriais(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'])); 
				?>
				</td>
				<td width="50%" valign="top">
				<p align="center">Municípios que não fecharam as turmas</p>
				<div style="height:150px;overflow:auto;">
				<?
				exibirMunicipiosNaoFechados(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				</div>
				<hr/>
				<?
				$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sispacto.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");
				carregarPeriodoReferencia(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'],'fpbid'=>$fpbid_padrao));
				?>
				<hr/>
				<p align="center">Resumo situações dos Mensários</p>
				<div style="height:150px;overflow:auto;" id="dv_situacao_mensario">
				<?
				exibirSituacaoMensario(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'],'fpbid'=>$fpbid_padrao));
				?>
				</div>
				<p align="center">Porcentagem geral de pagamentos</p>
				
				<div id="dv_porcentagem_pagamento">
				<?
				exibirPorcentagemPagamento(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'],'fpbid'=>$fpbid_padrao));
				?>
				</div>
				</td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=dados';">
		</td>
	</tr>
</table>