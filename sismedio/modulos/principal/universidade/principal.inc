<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($abaativa); ?></td>
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
				<!-- 
				<p align="center">Resumo acesso dos usuários ao SIMEC</p>
				<?
				//exibirAcessoUsuarioSimec(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				<p align="center">Materiais</p>
				<? 
				//exibirMateriais(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid'])); 
				?>
				</td>
				<td width="50%" valign="top">
				<p align="center">Resumo situações dos Mensários</p>
				<div style="height:150px;overflow:auto;">
				<?
				//exibirSituacaoMensario(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				</div>
				<p align="center">Porcentagem geral de pagamentos</p>
				<?
				//exibirPorcentagemPagamento(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				<p align="center">Municípios que não fecharam as turmas</p>
				<div style="height:150px;overflow:auto;">
				<?
				//exibirMunicipiosNaoFechados(array('uncid'=>$_SESSION['sispacto']['universidade']['uncid']));
				?>
				</div>
				 -->
				</td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=dados';">
		</td>
	</tr>
</table>