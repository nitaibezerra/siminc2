<?php
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Classificação de Despesas - Exercício {$_SESSION['exercicio']}", "");

if ( empty($_REQUEST["estuf"]) && !$_REQUEST["escola"] ){
	$emi->listaEstados( "classificacaodespesas" ); 
}elseif($_REQUEST["estuf"]){
	if ( !emiVerificaUf( $_REQUEST["estuf"] ) ){
		
		print "<script>
				  alert('O estado selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
	}
	$emi->listaEscolasPorEstado( $_REQUEST["estuf"] ); 
}elseif($_REQUEST["escola"]){
	$sql = "select 
				gap.papid,
				em.emeid,
				em.emeidpai 
			from 
				emi.ementidade em
			inner join
				emi.emgap gap ON gap.emeid = em.emeid
			where 
				em.entid = {$_REQUEST["escola"]}
			and
				papexercicio = '{$_SESSION['exercicio']}'"; 
	$em = $db->pegaLinha($sql);
	if(!$em){
		print "<script>
				  alert('A escola seleciona ainda não possui GAP!');
				  history.back(-1);
			  </script>";
		die;
	}
	$emeid = $em['emeid'];
	$_SESSION["emi"]["emeidPai"] = $em['emeidpai'];
	$emi->montaCabecalho( $emeid, "escola" );
?>
<style>
	.titulo{font-weight:bold;text-align:center;background-color:#D5D5D5;color:#000000;font-size:11px}
	.number{color:blue;text-align:right}
	.bold{font-weight:bold}
	.center{text-align:center}
</style>
<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="2" cellpadding="3" align="center" border=0 >
	<tr>
		<td rowspan="3"  class="titulo" >Macrocampos</td>
		<td rowspan="3"  class="titulo" >Ações</td>
		<td colspan="11" class="titulo" >Número de Alunos Beneficiários</td>
		<td colspan="4" class="titulo" >Número de Profissionais Envolvidos</td>
		<?php $arrCusteio = pegaItensCusteio() ?>
		<td class="titulo" <?php echo count($arrCusteio) > 0 ? "colspan='".(count($arrCusteio)+1)."'" : "" ?> >Custeio</td>
		<?php $arrCapital = pegaItensCapital() ?>
		<td class="titulo" <?php echo count($arrCapital) > 0 ? "colspan='".(count($arrCapital)+1)."'" : "" ?> >Capital</td>
	</tr>
	<tr>
		<td colspan="3" class="titulo" >1º Ano</td>
		<td colspan="3" class="titulo" >2º Ano</td>
		<td colspan="3" class="titulo" >3º Ano</td>
		<td rowspan="2" class="titulo" >Total</td>
		<td rowspan="2" class="titulo" >Total</td>
		<td rowspan="2" class="titulo" >Profº</td>
		<td rowspan="2" class="titulo" >Equipe Direção</td>
		<td rowspan="2" class="titulo" >Outros Profissionais</td>
		<td rowspan="2" class="titulo" >Total</td>
		<?php foreach($arrCusteio as $custeio): ?>
			<td rowspan="2" class="titulo" ><?php echo $custeio['itfdsc'] ?></td>
		<?php endforeach; ?>
		<td rowspan="2" class="titulo" >Total</td>
		<?php foreach($arrCapital as $capital): ?>
			<td rowspan="2" class="titulo" ><?php echo $capital['itfdsc'] ?></td>
		<?php endforeach; ?>
		<td rowspan="2" class="titulo" >Total</td>
	</tr>
	<tr>
		<td class="titulo" >M</td>
		<td class="titulo" >V</td>
		<td class="titulo" >N</td>
		<td class="titulo" >M</td>
		<td class="titulo" >V</td>
		<td class="titulo" >N</td>
		<td class="titulo" >M</td>
		<td class="titulo" >V</td>
		<td class="titulo" >N</td>
	</tr>
	<?php $arrMacroCampos = pegaMacroCampos(); ?>
	<?php $n=0;foreach($arrMacroCampos as $mc): //Início LOOP do Macrocampo?>
		<?php $cor = $n%2 == 0 ? "#e9e9e9" : "#fcfcfc"; ?>
		<?php $arrAcoes = emiPegaGapsPorMacro($emeid,$mc['mcpid']);?>
		<tr bgcolor="<?php echo $cor ?>" >
			<td <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> ><?php echo $mc['mcpdsc'] ?></td>
			<?php $n2=0;foreach($arrAcoes as $ac): //Início LOOP do Ações ?>
				<?php if($n2 > 0): ?>
					<tr bgcolor="<?php echo $cor ?>" >
				<?php endif; ?>
					<td><?php echo $ac['papcaoatividade'] ?></td>
					<?php $sql = "select * from emi.beneficiario where benid = (select benid from emi.emgap where papid = {$ac['papid']})";
						  $arrBen =  $db->pegaLinha($sql);
					?>
					<td class="number" ><?php $total1Mat+=$arrBen['benqtd1anomat']; echo number_format($arrBen['benqtd1anomat'],'',2,'.') ?></td>
					<td class="number" ><?php $total1Vesp+=$arrBen['benqtd1anovesp']; echo number_format($arrBen['benqtd1anovesp'],'',2,'.') ?></td>
					<td class="number" ><?php $total1Not+=$arrBen['benqtd1anonot']; echo number_format($arrBen['benqtd1anonot'],'',2,'.') ?></td>
					<td class="number" ><?php $total2Mat+=$arrBen['benqtd2anomat']; echo number_format($arrBen['benqtd2anomat'],'',2,'.') ?></td>
					<td class="number" ><?php $total2Vesp+=$arrBen['benqtd2anovesp']; echo number_format($arrBen['benqtd2anovesp'],'',2,'.') ?></td>
					<td class="number" ><?php $total2Not+=$arrBen['benqtd2anonot']; echo number_format($arrBen['benqtd2anonot'],'',2,'.') ?></td>
					<td class="number" ><?php $total3Mat+=$arrBen['benqtd3anomat']; echo number_format($arrBen['benqtd3anomat'],'',2,'.') ?></td>
					<td class="number" ><?php $total3Vesp+=$arrBen['benqtd3anovesp']; echo number_format($arrBen['benqtd3anovesp'],'',2,'.') ?></td>
					<td class="number" ><?php $total3Not+=$arrBen['benqtd3anonot']; echo number_format($arrBen['benqtd3anonot'],'',2,'.') ?></td>
					<?php $total =  $arrBen['benqtd1anomat'] + $arrBen['benqtd1anovesp'] + $arrBen['benqtd1anonot'] + $arrBen['benqtd2anomat'] + $arrBen['benqtd2anovesp'] + $arrBen['benqtd2anovesp'] + $arrBen['benqtd3anomat'] + $arrBen['benqtd3anovesp'] + $arrBen['benqtd3anovesp']?>
					<?php $totalGeral+=$total ; $totalGeralAlunos+=$total ?>
					<td class="number" ><?php echo number_format($total,'',2,'.') ?></td>
					<?php if ($n2 == 0): ?>
						<td class="number bold" id="total_gera_beneficiarios_<?php echo $mc['mcpid'] ?>" <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> >0</td>
						<?php $arrProf = array() ?>
						<?php $arrProf = pegaProfissionaisGAP($mc['mcpid'],$emeid) ?>
						<td class="number" <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> ><?php $totalProf+=$arrProf['preqtdprofessor']; echo number_format($arrProf['preqtdprofessor'],'',2,'.') ?></td>
						<td class="number" <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> ><?php $totalDirecao+=$arrProf['preqtddirecao']; echo number_format($arrProf['preqtddirecao'],'',2,'.') ?></td>
						<td class="number" <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> ><?php $totalOutros+=$arrProf['preqtdoutros']; echo number_format($arrProf['preqtdoutros'],'',2,'.') ?></td>
						<td class="number bold" <?php echo count($arrAcoes) > 0 ? "rowspan='".count($arrAcoes)."'" : "" ?> ><?php $totalGeralProf+= ($arrProf['preqtdprofessor'] + $arrProf['preqtddirecao'] + $arrProf['preqtdoutros']); echo number_format($arrProf['preqtdprofessor'] + $arrProf['preqtddirecao'] + $arrProf['preqtdoutros'],'',2,'.') ?></td>
						
					<?php endif; ?>
					<?php if (($n2+1) == count($arrAcoes)): ?>
						<script>document.getElementById('total_gera_beneficiarios_<?php echo $mc['mcpid'] ?>').innerHTML =  mascaraglobal('[.###]', <?php echo $totalGeral ?>);</script>
						<?php $totalGeralFinal+=$totalGeral; $totalGeral = 0; ?>
					<?php endif; ?>
					<?php $totalItem = 0; ?>
					<?php foreach($arrCusteio as $custeio): ?>
						<?php 
							$sql = "select 
										sum(mdototal) 
									from 
										emi.emmatrizdistribuicaoorcamentargap 
									where 
										papid = {$ac['papid']} 
									and 
										itfid = {$custeio['itfid']} 
									and 
										mdostatus = 'A'";
							$valorItem = $db->pegaUm($sql);
							$totalItem += $valorItem;
						?>
						<td class="number" ><?php echo $valorItem ? number_format($valorItem,2,',','.') : 0 ?></td>
					<?php endforeach; ?>
					<td class="number bold" ><?php $valorGeralItemCusteio+=$totalItem; echo $totalItem ? number_format($totalItem,2,',','.') : 0 ?></td>
					<?php $totalItem = 0; ?>
					<?php foreach($arrCapital as $capital): ?>
						<?php 
							$sql = "select 
										sum(mdototal) 
									from 
										emi.emmatrizdistribuicaoorcamentargap 
									where 
										papid = {$ac['papid']} 
									and 
										itfid = {$capital['itfid']} 
									and 
										mdostatus = 'A'";
							$valorItem = $db->pegaUm($sql);
							$totalItem += $valorItem;
						?>
						<td class="number" ><?php echo $valorItem ? number_format($valorItem,2,',','.') : 0 ?></td>
					<?php endforeach; ?>
					<td class="number bold" ><?php $valorGeralItemCapital+=$totalItem; echo $totalItem ? number_format($totalItem,2,',','.') : 0 ?></td>
				</tr>
			<?php $n2++;endforeach; //Fim LOOP do Ações ?>
	<?php $n++;endforeach; //Fim LOOP do Macrocampo ?>
	<tr bgcolor="#d5d5d5" >
		<td class="center bold" colspan="2" >Total</td>
		<td class="number bold" ><?php echo number_format($total1Mat,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total1Vesp,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total1Not,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total2Mat,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total2Vesp,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total2Not,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total3Mat,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total3Vesp,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($total3Not,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalGeralAlunos,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalGeralFinal,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalProf,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalDirecao,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalOutros,'',2,'.') ?></td>
		<td class="number bold" ><?php echo number_format($totalGeralProf,'',2,'.') ?></td>
		<td <?php echo $arrCusteio ? "colspan='".(count($arrCusteio)+1)."'" : "" ?> class="number bold" ><?php echo number_format($valorGeralItemCusteio,2,',','.') ?></td>
		<td <?php echo $arrCapital ? "colspan='".(count($arrCapital)+1)."'" : "" ?> class="number bold" ><?php echo number_format($valorGeralItemCapital,2,',','.') ?></td>
	</tr>
	<tr bgcolor="#d5d5d5" >
		<td class="center bold" colspan="2" >Total Geral</td>
		<td class="number bold center" colspan="11" ><?php echo number_format($totalGeralFinal,'',2,'.') ?></td>
		<td class="number bold center" colspan="4" ><?php echo number_format($totalGeralProf,'',2,'.') ?></td>
		<td class="number bold center" <?php echo ($arrCapital || $arrCusteio) ? "colspan='".((count($arrCapital)+1)+(count($arrCusteio)+1))."'" : "" ?> ><?php echo number_format($valorGeralItemCusteio+$valorGeralItemCapital,2,',','.') ?></td>
	</tr>
</table>

<?php } ?>