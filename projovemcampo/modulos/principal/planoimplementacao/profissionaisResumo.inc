<?
$percmax = '75,5';

$montante = calcularMontante();
$sql = "SELECT
			count(turid)
		FROM
			projovemcampo.turma tur
		INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.secaid = tur.secaid
		WHERE
			apcid ={$_SESSION['projovemcampo']['apcid']} ";
$numeroturmas = $db ->pegaUm($sql);

$sql="
		SELECT 
			count(foo)
		from 
		(SELECT DISTINCT
				entid
			FROM
				projovemcampo.turma tur
			INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.secaid = tur.secaid
			WHERE
				apcid ={$_SESSION['projovemcampo']['apcid']})
			as foo";
$numeroescolas = $db ->pegaUm($sql); 

/*Regra para quantidade de Coordenadore de Turma*/
if($numeroescolas>2||$numeroescolas==2){
	if($numeroturmas>10||$numeroturmas==10){
		$ct = round($numeroturmas/10);
	}else{
		$ct = 1 ;
	}
}elseif($numeroturmas>10||$numeroturmas==10){
	$ct = round($numeroturmas/10);
}
/*Fim regra quantidade de coordenadores de turma*/
/*carregar dados tela*/
	$efetivorecprog = 3;
	$efeticocomp = 2;
	$eftivo30 = 1;
	$efetivorecprop = 4;

	$coordenadorgeral = pegarocpid('1');
	$coordenadorgeralrec = pegarprofissionais('1',$efetivorecprog);
	$coordenadorgeralccm = pegarprofissionais('1',$efeticocomp);
	
	$coordturma = pegarocpid('2');
	$coordturmarec = pegarprofissionais('2',$efetivorecprog);
	$coordturmaccm = pegarprofissionais('2',$efeticocomp);
	
	$assistente = pegarocpid('3');
	$assistenterec = pegarprofissionais('3',$efetivorecprog);

	$educadores_EF_eft= pegarprofissionais('4',$eftivo30);
	$educadores_EF_recprop= pegarprofissionais('4',$efetivorecprop);
	$educadores_EF_rec = pegarprofissionais('4',$efetivorecprog);
	$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);
	
	$educadores_EQ_eft= pegarprofissionais('5',$eftivo30);
	$educadores_EQ_recprop= pegarprofissionais('5',$efetivorecprop);
	$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprog);
	$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);
	
	$educadores_EP_eft= pegarprofissionais('6',$eftivo30);
	$educadores_EP_recprop= pegarprofissionais('6',$efetivorecprop);
	$educadores_EP_rec = pegarprofissionais('6',$efetivorecprog);
	$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);
	
	$educadores_ET_eft= pegarprofissionais('7',$eftivo30);
	$educadores_ET_recprop= pegarprofissionais('7',$efetivorecprop);
	$educadores_ET_rec = pegarprofissionais('7',$efetivorecprog);
	
	$educadores_EE_eft= pegarprofissionais('8',$eftivo30);
	$educadores_EE_recprop= pegarprofissionais('8',$efetivorecprop);
	$educadores_E = $educadores_EE_eft['qtdcontratado'] + $educadores_EE_recprop['qtdcontratado'];
	
	$totalrec   = array(	"vlrtotal" 	 => 		$coordenadorgeralrec['vlrtotal']+ 
													$coordturmarec['vlrtotal']+ 
													$assistenterec['vlrtotal']+
													$educadores_EF_rec['vlrtotal']+ 
													$educadores_EQ_rec['vlrtotal']+
													$educadores_EP_rec['vlrtotal']+ 
													$educadores_ET_rec['vlrtotal'],
							"qtdcontratado" => 		$coordenadorgeralrec['qtdcontratado']+ 
													$coordturmarec['qtdcontratado']+ 
													$assistenterec['qtdcontratado']+
													$educadores_EF_rec['qtdcontratado']+ 
													$educadores_EQ_rec['qtdcontratado']+
													$educadores_EP_rec['qtdcontratado']+ 
													$educadores_ET_rec['qtdcontratado']);
	
	$totalccm =  array(	"vlrtotal" 	 => 			$coordenadorgeralccm['vlrtotal']+ 
													$coordturmaccm['vlrtotal']+ 
													$educadores_EF_ccm['vlrtotal']+
													$educadores_EQ_ccm['vlrtotal']+
									 				$educadores_EP_ccm['vlrtotal'],
						"qtdcontratado" => 			$coordenadorgeralccm['qtdcontratado']+ 
													$coordturmaccm['qtdcontratado']+ 
													$educadores_EF_ccm['qtdcontratado']+
													$educadores_EQ_ccm['qtdcontratado']+
									 				$educadores_EP_ccm['qtdcontratado']);	
					
	$percutilizado = ($totalUtilizado*100)/$montante;
?>
<form id="form" name="form" method="POST">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Orientações:</td>
		<td><font color=blue>
		<p>Quantidade de profissionais e valores previstos para a remuneração,
		conforme as informações prestadas na aba de profissionais.</p>
		</font></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais do quadro efetivo
		designados para atuar no programa e/ou contratados com recursos do
		ente executor</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
			align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="50%">Número</td>
			</tr>

			<tr>
				<td class="SubTituloDireita">Coordenador Geral</td>
				<td align="right"><?=number_format((($coordenadorgeral['ocpid']!=2 &&$coordenadorgeral['ocpid']!=3 )?$coordenadorgeral['qtdcontratado']:0),0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Coordenador de Turma</td>
				<td align="right"><?=number_format((($coordturma['ocpid']!=2 &&$coordturma['ocpid']!=3 )?$coordturma['qtdcontratado']:0),0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Pessoal apoio etapa matrícula</td>
				<td align="right"><?=number_format((($assistente['ocpid']!=3 )?$assistente['qtdcontratado']:0),0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores/Professores de educação básica</td>
				<td align="right"><?=number_format(	$educadores_EF_eft['qtdcontratado'] + $educadores_EF_recprop['qtdcontratado'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores/Professores do Qualificação Social e Profissional</td>
				<td align="right"><?=number_format(	$educadores_EQ_eft['qtdcontratado'] + $educadores_EQ_recprop['qtdcontratado'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores para acolhimento de crianças de zaro a oito anos, filhas de beneficiários do programa</td>
				<td align="right"><?=number_format($educadores_EP_eft['qtdcontratado'] + $educadores_EP_recprop['qtdcontratado'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tradutor e Intérprete de Libras</td>
				<td align="right"><?=number_format($educadores_ET_eft['qtdcontratado'] + $educadores_ET_recprop['qtdcontratado'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educador/Professor para atendimento educacional especializado</td>
				<td align="right"><?=number_format($educadores_E,0,",",".") ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais do quadro efetivo com
		complementação de carga horária</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
			align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="17%">Número</td>
				<td class="SubTituloCentro" width="17%">Valor das remunerações(R$)</td>
				<td class="SubTituloCentro" width="17%">Percentual Utilizado(%)</td>
			</tr>
			<?if($coordenadorgeralccm['qtdcontratado']!= 0 && $coordenadorgeralccm['qtdcontratado']!= '') {?>
			<tr>
				<td class="SubTituloDireita">Coordenador geral</td>
				<td align="right"><?=number_format($coordenadorgeralccm['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($coordenadorgeralccm['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordenadorgeralccm['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if(($coordturmaccm['qtdcontratado'])!= 0 && $coordturmaccm['qtdcontratado']!= '') {?>
			<tr>
				<td class="SubTituloDireita">Coordenador de Turma</td>
				<td align="right"><?=number_format($coordturmaccm['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($coordturmaccm['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordturmaccm['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EF_ccm['qtdcontratado']!= 0 && $educadores_EF_ccm['qtdcontratado']){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadores_EF_ccm['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EF_ccm['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EF_ccm['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EQ_ccm['qtdcontratado']!= 0 && $educadores_EQ_ccm['qtdcontratado']){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadores_EQ_ccm['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EQ_ccm['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EQ_ccm['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EP_ccm['qtdcontratado']!= 0 && $educadores_EP_ccm['qtdcontratado']){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadores_EP_ccm['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EP_ccm['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EP_ccm['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format($totalccm['qtdcontratado'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalccm['vlrtotal'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format(($totalccm['vlrtotal']/$montante)*100,1,",",".") ?></b></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais contratados com recursos do
		programa</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
			align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="17%">Número</td>
				<td class="SubTituloCentro" width="17%">Valor das remunerações(R$)</td>
				<td class="SubTituloCentro" width="17%">Percentual Utilizado(%)</td>
			</tr>
			<?if($coordenadorgeralrec['qtdcontratado']!= 0 && $coordenadorgeralrec['qtdcontratado']!= '') {?>
			<tr>
				<td class="SubTituloDireita">Coordenador geral</td>
				<td align="right"><?=number_format($coordenadorgeralrec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($coordenadorgeralrec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordenadorgeralrec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if(($coordturmarec['qtdcontratado'])!= 0 && $coordturmarec['qtdcontratado']!= '') {?>
			<tr>
				<td class="SubTituloDireita">Coordenador de Turma</td>
				<td align="right"><?=number_format($coordturmarec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($coordturmarec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordturmarec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($assistenterec['qtdcontratado'] != 0 && $assistenterec['qtdcontratado'] != ''){ ?>
			<tr>
				<td class="SubTituloDireita">Assistentes da coordenação geral</td>
				<td align="right"><?=number_format($assistenterec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($assistenterec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($assistenterec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EF_rec['qtdcontratado'] != 0 ||$educadores_EF_rec['qtdcontratado']!= '' ){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadores_EF_rec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EF_rec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EF_rec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EQ_rec['qtdcontratado'] != 0 ||$educadores_EQ_rec['qtdcontratado']!= '' ){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadores_EQ_rec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EQ_rec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EQ_rec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_EP_rec['qtdcontratado'] != 0 ||$educadores_EP_rec['qtdcontratado']!= ''){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadores_EP_rec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_EP_rec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_EP_rec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_ET_rec['qtdcontratado'] != 0 ||$educadores_ET_rec['qtdcontratado']!= ''){ ?>
			<tr>
				<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
				<td align="right"><?=number_format($educadores_ET_rec['qtdcontratado'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_ET_rec['vlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_ET_rec['vlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format($totalrec['qtdcontratado'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalrec['vlrtotal'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format(($totalrec['vlrtotal']/$montante)*100,1,",",".")?></b></td>
			</tr>
		</table>
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Total Geral</td>
		<td>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" width="50%">Número de profissionais que
				atuarão no Programa e serão remunerados com recursos federais</td>
				<td align="right"><?=number_format($totalrec['qtdcontratado']+$totalccm['qtdcontratado'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Valor total das
				remunerações(pagas com recursos federais)</td>
				<td align="right"><?=number_format($totalrec['vlrtotal']+$totalccm['vlrtotal'],2,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">% Total Utilizado</td>
				<td align="right"><?=number_format((($totalrec['vlrtotal']+$totalccm['vlrtotal'])/$montante)*100,1,",",".") ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button"
			name="anterior" value="Anterior"
			onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro';">
		<input type="button" name="proximo" value="Próximo"
			onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores';">
		</td>
	</tr>
</table>
</form>
			<? registarUltimoAcesso(); ?>
