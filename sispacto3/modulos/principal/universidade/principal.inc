<? 
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */
?>
<script>
function selecionarPeriodoReferencia(fpbid) {

	if(fpbid!='') {
		ajaxatualizar('requisicao=exibirSituacaoMensario&fpbid='+fpbid+'&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','dv_situacao_mensario');
		ajaxatualizar('requisicao=exibirPorcentagemPagamento&fpbid='+fpbid+'&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','dv_porcentagem_pagamento');
	}
	
}

function carregarQtdTurmas_<?=PFL_FORMADORIES ?>(filtroconsulta) {
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_FORMADORIES ?>&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>&filtroconsulta='+filtroconsulta,'mediaTurmasPerfilFormador');
}
function carregarQtdTurmas_<?=PFL_ORIENTADORESTUDO ?>(filtroconsulta) {
	ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_ORIENTADORESTUDO ?>&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>&filtroconsulta='+filtroconsulta,'mediaTurmasPerfilOrientadoresEstudo');
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
<? if($_REQUEST['modulo'] == 'principal/universidade/universidadeexecucao' && false) : ?>
</tr>
	<tr>
		<td colspan="2">
			<table width="100%">
			<tr>
				<td width="50%" valign="top">
				<script>
				function exibirSituacaoMensarioPadrao() {
					jQuery('#dv_situacao_mensario').html('<img src="../imagens/carregando.gif" align="absmiddle"> Carregando...');
					ajaxatualizarAsync('requisicao=exibirSituacaoMensario&fpbid=<?=$fpbid_padrao ?>&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','dv_situacao_mensario');
				}
				
				jQuery(document).ready(function() {
					ajaxatualizarAsync('requisicao=exibirAcessoUsuarioSimec&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','acessousuario');
					ajaxatualizarAsync('requisicao=exibirPorcentagemPagamento&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','dv_porcentagem_pagamento');
					ajaxatualizarAsync('requisicao=ausenciaOEIES&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','ausenciaOEIES');
					ajaxatualizarAsync('requisicao=ausenciaPAMunicipio&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','ausenciaPAMunicipio');
					ajaxatualizarAsync('requisicao=bolsistasSISPACTODirigentes&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','bolsistasSISPACTODirigentes');

					ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_FORMADORIES ?>&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','mediaTurmasPerfilFormador');
					ajaxatualizarAsync('requisicao=mediaTurmasPerfil&pflcod=<?=PFL_ORIENTADORESTUDO ?>&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','mediaTurmasPerfilOrientadoresEstudo');
									
					
				});
				</script>
				<p align="center">Resumo acesso dos usuários ao <?php echo SIGLA_SISTEMA; ?></p>
				<div id="acessousuario"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				<hr>
				<p align="center">Municípios que não fecharam as turmas</p>
				
				<div style="height:100px;overflow:auto;">
				<?
				exibirMunicipiosNaoFechados(array('uncid'=>$_SESSION['sispacto2']['universidade']['uncid']));
				?>
				</div>
				
				<div id="ausenciaOEIES"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				<br>
				<div id="ausenciaPAMunicipio"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				<br>
				<div id="bolsistasSISPACTODirigentes"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				
				
				</td>
				<td width="50%" valign="top">
				<?
				$fpbid_padrao = $db->pegaUm("SELECT fpbid FROM sispacto2.folhapagamento WHERE fpbanoreferencia='".date("Y")."' AND fpbmesreferencia='".date("m")."'");
				carregarPeriodoReferencia(array('uncid'=>$_SESSION['sispacto2']['universidade']['uncid'],'fpbid'=>$fpbid_padrao));
				?>
				<hr/>
				<p align="center">Resumo situações dos Mensários</p>
				<div style="height:150px;overflow:auto;" id="dv_situacao_mensario">A situação de <b>Aptos/Não Aptos/Aprovados</b> demora cerca de 2 minutos para ser processada. Caso queira visualizar essa informação <a style="cursor:pointer;" onclick="exibirSituacaoMensarioPadrao();"><b>CLIQUE AQUI</b></a></div>
				<br>
				<p align="center">Porcentagem geral de pagamentos</p>
				
				<div id="dv_porcentagem_pagamento"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				
			 <p>Turmas de formadores</p>
			<p align="center">
			<select name="qtd[]" class="CampoEstilo" style="width: auto;font-size:x-small;" onchange="carregarQtdTurmas_<?=PFL_FORMADORIES ?>(this.value)">'
			<option value="BETWEEN 1 AND 5">Até 5 cursistas</option>
			<option value="BETWEEN 6 AND 10">De 6 a 10 cursistas</option>
			<option value="BETWEEN 11 AND 25">De 11 a 25 cursistas</option>
			<option value="BETWEEN 26 AND 34">De 26 a 34 cursistas</option>
			<option value="BETWEEN 35 AND 50">De 35 a 50 cursistas</option>
			<option value="BETWEEN 51 AND 75">De 51 a 75 cursistas</option>
			<option value="BETWEEN 76 AND 100">De 76 a 100 cursistas</option>
			<option value=">100">Mais de 100 cursistas</option>
			</select>
			</p>
			 
			 <div id="mediaTurmasPerfilFormador"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
			 <p>Turmas de Orientadores de Estudo</p>
			<p align="center">
			<select name="qtd[]" class="CampoEstilo" style="width: auto;font-size:x-small;" onchange="carregarQtdTurmas_<?=PFL_ORIENTADORESTUDO ?>(this.value)">'
			<option value="BETWEEN 1 AND 5">Até 5 cursistas</option>
			<option value="BETWEEN 6 AND 10">De 6 a 10 cursistas</option>
			<option value="BETWEEN 11 AND 25">De 11 a 25 cursistas</option>
			<option value="BETWEEN 26 AND 34">De 26 a 34 cursistas</option>
			<option value="BETWEEN 35 AND 50">De 35 a 50 cursistas</option>
			<option value="BETWEEN 51 AND 75">De 51 a 75 cursistas</option>
			<option value="BETWEEN 76 AND 100">De 76 a 100 cursistas</option>
			<option value=">100">Mais de 100 cursistas</option>
			</select>
			</p>
			 
			 <div id="mediaTurmasPerfilOrientadoresEstudo"><img src="../imagens/carregando.gif" align="absmiddle"> Carregando...</div>
				
				</td>
			</tr>
			</table>
		</td>
	</tr>
<? endif; ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
		</td>
	</tr>
</table>