<?
/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//$_SESSION['fiesabatimento_var']['cpfusuario'] = '';

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$dados = pegarInfoUsuario($_SESSION['fiesabatimento_var']['cpfusuario']);
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
jQuery(document).ready(function(){
	jQuery('.historico').click(function(){
		var docid = $(this).attr('id');
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid='+docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	});
	jQuery('.abre').click(function(){
		var sbaid = jQuery(this).attr('id');
		if( jQuery('#tr_atuacoes_'+sbaid).css('display') == 'none' ){
			jQuery('#tr_atuacoes_'+sbaid).show();
			jQuery(this).attr('src','../imagens/menos.gif');
		}else{
			jQuery('#tr_atuacoes_'+sbaid).hide();
			jQuery(this).attr('src','../imagens/mais.gif');
		}
	});
});
</script>
<form name="formulario" method="post" id="formulario">
<input type="hidden" name="requisicao" value="confirmarSolicitacao">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="16%" ><b>Orientações :</b></td>
			<td >
				O quadro abaixo contempla síntese das solicitações e renovações de abatimento. Se desejar, o detalhamento e demais informações sobre as solicitações de abatimento poderão ser obtidas mediante<br>
				utilização da opção "detalhar <img border="0" title="Indica campo obrigatório." src="../imagens/consultar.gif">", referente aos registros selecionados.
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CPF:</td>
			<td><?=$_SESSION['fiesabatimento_var']['cpfusuario'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome Solicitante:</td>
			<td><?=$dados['no_pessoa'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico:</td>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr bgcolor="#DCDCDC">
						<td width="5%"><b>Detalhar</b></td>
						<td width="15%"><b>Solicitação</b></td>
						<td width="30%"><b>Situação atual</b></td>
						<td width="20%"><b>Data da última situação</b></td>
						<td width="15%"><b>Quantidade de meses solicitados</b></td>
						<td width="15%"><b>Quantidade de meses aprovados</b></td>
					</tr>
					<?php 
					$sql = "SELECT DISTINCT
								sba.sbaid,
								sba.docid,
								esd.esdid,
								esd.esddsc,
								to_char(hst.htddata,'DD/MM/YYYY') as dt,
								sba.sbaqmtsolicitado,
								coalesce(qtd.qtd, 0) as sbaqmtaprovado,
								(CASE WHEN sba.sbarenovacao = 'f' THEN
										'Solicitação (' || sba.sbaanoinicio || ' à ' || sba.sbaanofim || ')'
									  ELSE
									  	'Renovação (' || sba.sbaanoinicio || ' à ' || sba.sbaanofim || ')'
								 END) as tipo
							FROM
								fiesabatimento.solicitacaoabatimento sba 
							LEFT JOIN fiesabatimento.identificacaodocente 	ido ON ido.idoid = sba.idoid
							LEFT JOIN workflow.documento 					doc ON doc.docid = sba.docid --AND tpdid = ".TPDID_ANALISE_SITUACAO."
							LEFT JOIN workflow.historicodocumento 			hst ON hst.hstid = doc.hstid 
							LEFT JOIN workflow.acaoestadodoc				aed ON aed.aedid = hst.aedid
							LEFT JOIN workflow.estadodocumento 				esd ON esd.esdid = doc.esdid
							LEFT JOIN (	SELECT
											count(DISTINCT matano||matmes) as qtd,
											sbaid
								   		FROM 	
											fiesabatimento.mesesatuacao
									   	GROUP BY
											sbaid) as qtd ON qtd.sbaid = sba.sbaid
							LEFT  JOIN fiesabatimento.parametrosrenovacao 	par	ON par.preid = sba.preid
							WHERE
								sbastatus = 'A' and
								idocpf = '".$_SESSION['fiesabatimento_var']['cpfusuario']."'
							ORDER BY
								1";
					$dados = $db->carregar($sql);
					if( is_array($dados) ){
						$i = 0;
						$doc = 0;
						foreach( $dados as $dado ){
							if( $doc != $dado['docid'] ){
								$i = $i+1;
								$doc = $dado['docid'];
							}
					?>
					<tr style="background-color:white;">
						<td align="center">
							<img border="0" title="Indica campo obrigatório." src="../imagens/consultar.gif" 
								 class="historico" id="<?=$dado['docid'] ?>" style="cursor:pointer">
							<img border="0" title="Detalhe das Atuações Profissionais." src="../imagens/mais.gif" 
								 class="abre" id="<?=$dado['sbaid'] ?>" style="cursor:pointer">
						</td>
						<td nowrap="nowrap"><?=$dado['tipo'] ?> - n° <?=$i ?></td>
						<td><?=$dado['esddsc'] ?> 
							<?php 
							if( $dado['esdid'] == WF_FIES1_REJEITADO ){
								
								echo " por: <br><br> Não possui 12 meses ininterruptos com no minimo<br> 20 horas de carga horária semanal.";
							}
							?>
						</td>
						<td><?=$dado['dt'] ?></td>
						<td><?=$dado['sbaqmtsolicitado'] ?></td>
						<td><?=$dado['sbaqmtaprovado'] ?></td>
					</tr>
					<tr id="tr_atuacoes_<?=$dado['sbaid'] ?>" style="display:none">
						<td></td>
						<td colspan="5">
							<?php
							 listaAtuacoesAtivas( $dado['sbaid'] );
							 listaAtuacoesCanceladas( $dado['sbaid'] );
							?>
						</td>
					</tr>
					<?php 
						}
					}
					?>
				</table>
			</td>
		</tr>
	</table>
</form>