<?php 

if($_SESSION['projovemurbano']['muncod']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."' AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
} 

if($_SESSION['projovemurbano']['estuf']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."' AND c.ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}

$cmemeta = $meta;
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarQualificacaoProfissional() {

	jQuery('#form').submit();
}

function visualizarFormula()
{
	var janela = window.open( 'projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=popUpFormula', 'formula', 'width=800,height=400,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    janela.focus();
}

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarQualificacaoProfissional">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
		<font color="blue">
		<p>Este Município esclarece que tem ciência de que o Plano de Implementação apresentado é um instrumento de planejamento de ações e controle de gastos e que, para efeito de repasse da 1ª parcela de recursos, é considerado o número da meta de estudantes.</p>
		<p>Da mesma forma, este Município tem ciência de que as demais parcelas, serão efetuadas com base na fórmula descrita no Anexo V da Resolução CD/FNDE Nº 60, de 09 de novembro de 2011. Na mesma resolução determina-se que esses repasses terão como base de cálculo o número de estudantes efetivimante matriculados e frequentes, de acordo com o Sistema de Matrícula, Acompanhamento de Frequência e Certificação do Projovem Urbano.</p>
		<p>Por fim, este Município compromete-se a atualizar estas informações, constantemente, conforme prazos e por meio dos instrumentos estabelecidos pela SECADI/MEC.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Total previsto a ser recebido por este Município, caso a meta prevista seja mantida até o final da execução.</td>
		<td>
			<table class="tabela" bgcolor="#d5d5d5" cellSpacing="1" cellPadding="3"	align="center">
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Meta</td>
					<td style="font-weight:bold" >PER CAPTA (R$)</td>
					<td style="font-weight:bold" >Meses de Referência</td>
					<td style="font-weight:bold" >Valor Total</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td><?php echo number_format($cmemeta,0,',','.') ?></td>
					<?
					if($_SESSION['projovemurbano']['muncod']) { 
						echo "<td>R$ 165,00</td>";
						$vlrpercapita = 165;
					}
					
					if($_SESSION['projovemurbano']['estuf']) {
						echo "<td>R$ 170,00</td>";
						$vlrpercapita = 170;
					}
					?>
					<td>18</td>
					<td><?php echo number_format( ( $cmemeta*$vlrpercapita*18 ) ,2,',','.') ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor previsto para a 1ª parcela de recursos, com base no número de estudantes/meta.</td>
		<td>
			<table class="tabela" bgcolor="#d5d5d5" cellSpacing="1" cellPadding="3"	style="width:500px;" >
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Adicional - Provas</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td>
					<?php
					echo number_format($cmemeta*54,2,',','.');
					?>
					</td>
				</tr>
				<tr style="font-weight:bold;text-align:center" >
					<td style="font-weight:bold" >Valor 1ª Parcela</td>
				</tr>
				<tr  style="text-align:center" bgcolor="#f5f5f5" >
					<td>
					<?php
					 
					$m = 6; 
					if($_SESSION['projovemurbano']['muncod']) {
						$per_capta = '165.00';
						$percprof = '0.765';
						$pertrans = '0';
					}
					if($_SESSION['projovemurbano']['estuf']) {
						$per_capta = '170.00';
						$percprof = '0.75';
						$pertrans = '0.015';
					}

					$p1 = $percprof;
					$p2 = '0.05';
					$p3 = '0.015';
					$p4 = '0.10';
					$p5 = '0.07';
					$p6 = $pertrans;
					
					$valorTotal = ($cmemeta*( ($p1*$m*$per_capta) + ($p2*$m*$per_capta) + ($p3*18*$per_capta) + ($p4*$m*$per_capta) + ($p5*$m*$per_capta) + ($p6*18*$per_capta) )) + ($cmemeta*54);
					
					echo number_format($valorTotal,2,',','.');
					 
					?> (valor demostrativo) <input type="button" onclick="visualizarFormula()" name="btn_visualizar_formula" value="Visualizar Fórmula" />
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
		Enviar o plano de implementação
		</td>
		<td>
		<? 
		include APPRAIZ ."includes/workflow.php";
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $docid, array( 'entid' => $_SESSION['rehuf_var']['entid'] ) );			
		?>
		</td>
	</tr>
	<? 
	$cmddsc = $db->pegaUm("SELECT cmddsc FROM workflow.comentariodocumento WHERE docid='".$docid."' AND hstid IN(SELECT hstid FROM workflow.documento WHERE docid='".$docid."')");

	if($cmddsc) :
	?>
	<tr>
		<td class="SubTituloDireita" style="background-color:red">
		<b>Observações sobre Plano de Implementação</b>
		</td>
		<td>
		<?=$cmddsc ?>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro';">
			<!-- 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro';"></td>
			 -->
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>