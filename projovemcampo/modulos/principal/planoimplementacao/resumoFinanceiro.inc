<?
$efetivorecprog = 3;
$efeticocomp = 2;

$montante = calcularMontante();
/*carregar dados tela*/
	$coordenadorgeralrec = pegarprofissionais('1',$efetivorecprog);
	$coordenadorgeralccm = pegarprofissionais('1',$efeticocomp);
	
	$coordturmarec = pegarprofissionais('2',$efetivorecprog);
	$coordturmaccm = pegarprofissionais('2',$efeticocomp);
	
	$assistenterec = pegarprofissionais('3',$efetivorecprog);

	$educadores_EF_rec = pegarprofissionais('4',$efetivorecprog);
	$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);
	
	$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprog);
	$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);
	
	$educadores_EP_rec = pegarprofissionais('6',$efetivorecprog);
	$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);
	
	$educadores_ET_rec = pegarprofissionais('7',$efetivorecprog);
	
	
	$totalrec   = array(	"vlrtotal" 	 => 		$coordenadorgeralrec['vlrtotal']+ 
													$coordturmarec['vlrtotal']+ 
													$assistenterec['vlrtotal']+
													$educadores_EF_rec['vlrtotal']+ 
													$educadores_EQ_rec['vlrtotal']+
													$educadores_EP_rec['vlrtotal']+ 
													$educadores_ET_rec['vlrtotal']);
	
	$totalccm =  array(	"vlrtotal" 	 => 			$coordenadorgeralccm['vlrtotal']+ 
													$coordturmaccm['vlrtotal']+ 
													$educadores_EF_ccm['vlrtotal']+
													$educadores_EQ_ccm['vlrtotal']+
									 				$educadores_EP_ccm['vlrtotal']	);	
					
	$percutilizado = ($totalUtilizado*100)/$montante;

	$planodeimplementacao = planodeimplementacao();
	
	$sql = "SELECT
				SUM(valor)
			FROM
				projovemcampo.tiposrecursoformacao tpf
			LEFT JOIN projovemcampo.tiporecursoplanoimplementacao trp ON trp.tpfid=tpf.tpfid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$totalutilizado_formacao = $db->pegaUm($sql);
	
	$totalutilizado_auxiliofinanceiro = $planodeimplementacao['qtdeduccontinuada']*$planodeimplementacao['auxfinanceiroeduc'];

	$totalutilizado_generoalimenticio = ($planodeimplementacao['gaqtdcriancas']+$meta)*$planodeimplementacao['gaqtdmeses']*$planodeimplementacao['gavalormensal'];
	
	$sql = "SELECT
				qtdmeses*valor as valortotal
			FROM
				projovemcampo.qualificacaoprofissionaldespesa qpd
			LEFT JOIN projovemcampo.despesaqualificacaoplano dqp ON dqp.qpdid = qpd.qpdid
			WHERE
				pimid = {$_SESSION['projovemcampo']['pimid']}";
	$qualificacaoprofissional = $db->Carregar($sql);
	
	$totalutilizado_qualificacaoprofissional = $qualificacaoprofissional['0']['valortotal']+$qualificacaoprofissional['1']['valortotal']+$qualificacaoprofissional['2']['valortotal'];
	
	$valormaterialdidatico = $planodeimplementacao['valormaterialdidatico'];
		
	$totalutilizado_transportematerial = $valormaterialdidatico;
	
	$retornarTotalMaximoDemaisAcoes=true;
	$totalMax = validacaoCompletaPlanoImplementacao($retornarTotalMaximoDemaisAcoes);
	
	$sql = "SELECT
				*
			FROM
				projovemcampo.tipodemaisacoesplano tpa
			INNER JOIN projovemcampo.tiposdemaisacoes tda ON tda.tdaid = tpa.tdaid
			WHERE
				tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
	$acoes = $db->carregar($sql);
	$sql = "SELECT
				SUM(valortotal)
			FROM
				projovemcampo.tipodemaisacoesplano tpa
			WHERE
				tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
	$total = $db->pegaUm($sql);
?>
<form id="form" name="form" method="POST"><input type="hidden"
	name="requisicao" value="gravarQualificacaoProfissional">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td><font color=blue> <?=$arrOrientacoes[11] ?> </font></td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Pagamento de profissionais:<br>
		<br>
		Assistentes da Coordenação Local,
		Educadores de Ensino Fundamental, de qualificação profissional e de
		participação cidadã, Educadores para monitoramento do acolhimento de
		crianças de zero a oito anos e Tradutores e interpreté de libras</td>
		<td valign="top">
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format($totalrec['vlrtotal'],2,",",".") ?></td>
				<td align="center"><?=round(($totalrec['vlrtotal']/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Remuneração para complementação de carga
		horária:<br>
		<br>
		Educadores, Coordenador Geral e Coordenador de turma</td>
		<td valign="top">
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format($totalccm['vlrtotal'],2,",",".") ?></td>
				<td align="center"><?=round(($totalccm['vlrtotal']/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Total Geral da Ação</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format(($montante*('75,0'/100)),2,",",".") ?></td>
				<td align="center"><?='75,0' ?></td>
				<td align="center"><?=number_format(($totalccm['vlrtotal']+$totalrec['vlrtotal']),2,",",".") ?></td>
				<td align="center"><?=round(((($totalccm['vlrtotal']+$totalrec['vlrtotal'])/$montante)*100),1) ?></td>
			</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Formação de educadores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pagamento de ajuda de custo para a
		primeira etapa de formação continuada de educadores de ensino
		fundamental, qualificação profissional e participação cidadã</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format(($montante*('1,0'/100)),2,",",".") ?></td>
				<td align="center"><?='1,0'?></td>
				<td align="center"><?=number_format(($totalutilizado_auxiliofinanceiro),2,",",".") ?></td>
				<td align="center"><?=round((($totalutilizado_auxiliofinanceiro)/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Custeio da Formação</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<? //$recursosgastos = $db->pegaLinha("SELECT SUM(rgavalor) as total FROM projovemurbano.recursosgastos rec WHERE rec.fedid='".$_SESSION['projovemurbano']['fedid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*('10,0'/100)),2,",",".") ?></td>
				<td align="center"><?='10,0' ?></td>
				<td align="center"><?=number_format(($totalutilizado_formacao),2,",",".") ?></td>
				<td align="center"><?=round((($totalutilizado_formacao)/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Aquisição de gêneros
		alimentícios</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Aquisição de gêneros alimenticios
		destinados ao fornecimento de lanches ou refeição para os alunos
		matriculados e frequentes no ambito do programa, bem como para
		crianças de zero a cinco anos, filhas de alunos do programa atendidas
		em salas de acolhimento</td>
		<td><? 
// 		$sql = "SELECT * FROM projovemurbano.lancherefeicao WHERE galid='".$_SESSION['projovemurbano']['galid']."'";
// 		$lancherefeicao = $db->pegaLinha($sql);
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format(($montante*('5,0'/100)),2,",",".") ?></td>
				<td align="center"><?='5,0' ?></td>
				<td align="center"><?=number_format($totalutilizado_generoalimenticio,2,",",".") ?></td>
				<td align="center"><?=round((($totalutilizado_generoalimenticio)/$montante)*100,1)?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Qualificação Profissional</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Locação de espaços e equipamentos,
		aquisição de material de consumo, bem como o pagamento de monitores
		para atividades de qualificação profissional</td>
		<td><?
// 		$sql = "SELECT SUM(pgavlrmes*pgaqtdmeses) as total, nucid FROM projovemurbano.previsaogasto WHERE  qprid='".$_SESSION['projovemurbano']['qprid']."' GROUP BY nucid"  ;
		
// 		$previsaogasto = $db->pegaUM($sql);
		//		ver($sql);
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format(($montante*('7,0'/100)),2,",",".") ?></td>
				<td align="center"><?='7,0'?></td>
				<td align="center"><?=number_format($totalutilizado_qualificacaoprofissional,2,",",".") ?></td>
				<td align="center"><?=round((($totalutilizado_qualificacaoprofissional)/$montante)*100,1) ?></td>

			</tr>
		</table>
		</td>
	</tr>
	<? if($_SESSION['projovemcampo']['estuf']) : ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Transporte do Material
		Didático</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pagamento do Transporte do Material
		didático-pedagógico do Projovem Urbano</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<?// $transportematerial = $db->pegaLinha("SELECT * FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = '".$_SESSION['projovemurbano']['tprid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*('1,5'/100)),2,",",".") ?></td>
				<td align="center"><?='1,5' ?></td>
				<td align="center"><?=number_format($totalutilizado_transportematerial,2,",",".") ?></td>
				<td align="center"><?=round((($totalutilizado_transportematerial)/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Demais Ações</td>
	</tr>
		<tr>
		<td class="SubTituloDireita">Total Geral da Ação</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">(%) Máximo permitido</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format($totalMax,2,",",".") ?></td>
				<td align="center"><?=round((($totalMax)/$montante)*100,1) ?></td>
				<td align="center"><?=number_format($total,2,",",".") ?></td>
				<td align="center"><?=round((($total)/$montante)*100,1)?></td>
			</tr>
		</table>

		</td>
	</tr>

	<?
	if($acoes[0]) :
		foreach($acoes as $acao) :
	?>
			<tr>
				<td class="SubTituloDireita"><?=$acao['nome'] ?></td>
				<td>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
					cellPadding="3" align="center" width="100%">
					<tr>
						<td class="SubTituloCentro">Valor Utilizado(R$)</td>
						<td class="SubTituloCentro">(%) Utilizado</td>
					</tr>
					<tr>
						<td align="center"><?=number_format($acao['valortotal'],2,",",".") ?></td>
						<td align="center"><?=round(($acao['valortotal']/$montante)*100,1) ?></td>
					</tr>
				</table>
				</td>
			</tr>
	<?
		endforeach;
	endif;
	?>

</table>
</form>
<?// registarUltimoAcesso(); ?>
