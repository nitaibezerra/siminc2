<?
$percmaxprof = '75.5';
$percmaxform = '10';
$percmaxauxilio = '1';
$percmaxalimentos = '5';
$percmaxqualificaco = '7';
$percmaxtransporte = '1.5';

$meta = pegameta();

$montante = calcularMontante();

$planodeimplementacao = planodeimplementacao();

$sql = "SELECT
		SUM(qtdmeses*valor) as valortotal
		FROM
		projovemcampo.despesaqualificacaoplano dqp
		WHERE
		pimid = {$_SESSION['projovemcampo']['pimid']}";
$totalqualificacao = $db ->pegaUm($sql);

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
												$educadores_ET_rec['vlrtotal']);

$totalccm =  array(	"vlrtotal" 	 => 			$coordenadorgeralccm['vlrtotal']+ 
												$coordturmaccm['vlrtotal']+ 
												$educadores_EF_ccm['vlrtotal']+
												$educadores_EQ_ccm['vlrtotal']+
								 				$educadores_EP_ccm['vlrtotal']	);
	
$totalUtilizadoprof = $totalrec['vlrtotal']+$totalccm['vlrtotal'];

$percutilizadoprof = ($totalUtilizadoprof*100)/$montante;

$sql = "SELECT
			SUM(valor)
		FROM
			projovemcampo.tiposrecursoformacao tpf
		LEFT JOIN projovemcampo.tiporecursoplanoimplementacao trp ON trp.tpfid=tpf.tpfid
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']}";
$totalutilizadoform = $db->pegaUm($sql);

$sql = "SELECT
			qtdeduccontinuada,
			auxfinanceiroeduc
		FROM
			projovemcampo.planodeimplementacao
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']} ";

$auxiliofinanceiro = $db->pegaLinha($sql);

$sql = "SELECT
			gaqtdcriancas,
			gaqtdmeses,
			gavalormensal
		FROM
			projovemcampo.planodeimplementacao
		WHERE
			pimid = {$_SESSION['projovemcampo']['pimid']} ";

$generoalimenticio  = $db->pegaLinha($sql);

$retornarTotalMaximoDemaisAcoes=true;
$totalMax = validacaoCompletaPlanoImplementacao($retornarTotalMaximoDemaisAcoes);

$sql = "SELECT 
			SUM(valortotal) 
		FROM 
			projovemcampo.tipodemaisacoesplano tpa
		WHERE 
			tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
$totalutilizadodemais  = $db->pegaUm($sql);
$sql = "SELECT
			inl.inlnuminstlegal  as numero,
			inl.inldatainstlegal as data
		FROM projovemcampo.instrumentolegal inl
		INNER JOIN projovemcampo.anexogeral ang USING (angid)
		INNER JOIN public.arquivo arq USING (arqid)
		INNER JOIN projovemcampo.tipodocumentoinstrumentolegal tdi ON tdi.tpdid = inl.tpdid
		WHERE
		pimid = {$_SESSION['projovemcampo']['pimid']}
		AND arq.arqstatus = 'A'";
//  ver($sql,d);
$infoInstLegal = $db->carregar($sql);

?>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Profissionais</label></td></tr></tbody></table>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela"><?echo $publico;?></label></td></tr></tbody></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Percentual da ação</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
				<td class="SubTituloCentro">Percentual Utilizado(%)</td>
			</tr>
			<tr>
				<td align="center"><? echo ($percmaxprof); ?>% - R$ 
									<span id="span_totalmaximo"><?=number_format($montante*($percmaxprof)/100,2,",",".") ?></span></td>
				<td align="center"><? echo (($percutilizadoprof)?number_format($percutilizadoprof,2,",","."):'0.0'); ?>% - R$ 
									<span id="span_totalutilizado"><?=number_format($totalUtilizadoprof,2,",",".") ?></span></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Coordenação Local</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Coordenador Geral</td>
		<td>
		<? if($coordenadorgeral) : ?>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo (($coordenadorgeral['qtdcontratado'])?$coordenadorgeral['qtdcontratado']:'1'); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? echo (($coordenadorgeral['ocpid'])?$db->pegaUm("SELECT nome FROM projovemcampo.opcaocontratoprofissional WHERE ocpid='".$coordenadorgeral['ocpid']."'"):"Não selecionado"); ?></td>
				</tr>
	
				<tr <?=(($coordenadorgeral['ocpid']==$efeticocomp)?"":"style=\"display:none;\"") ?> >
					<td colspan="2">
						<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
							<tr>
								<td class="SubTituloCentro">Número de horas pagas<br> com recursos do programa/mês</td>
								<td class="SubTituloCentro">Valor da hora (R$)</td>
								<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
								<td class="SubTituloCentro">Meses</td>
								<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
								<td class="SubTituloCentro">Valor Total (R$)</td>
							</tr>
							<tr>
								<td align="center"><? echo (($coordenadorgeralccm['numerohoraspagas'])?$coordenadorgeralccm['numerohoraspagas']:'&nbsp;'); ?></td>
								<td align="center"><? echo (($coordenadorgeralccm['valorhora'])?number_format($coordenadorgeralccm['valorhora'],2,",","."):'&nbsp;'); ?></td>
								<td align="center"><? echo (($coordenadorgeralccm['valorbrutoremuneracao'])?number_format($coordenadorgeralccm['valorbrutoremuneracao'],2,",","."):'&nbsp;'); ?></td>
								
								<td align="center"><? echo (($coordenadorgeralccm['qtdmeses'])?$coordenadorgeralccm['qtdmeses']:'28'); ?></td>						
								<td align="center"><? echo $coordenadorgeralccm['encargossociais']; ?></td>
								<td align="center"><? echo (($coordenadorgeralccm['vlrtotal'])?number_format($coordenadorgeralccm['vlrtotal'],2,",","."):'&nbsp;'); ?></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr <?=(($coordenadorgeral['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?>>
						<td colspan="2">
							<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
								<tr>
									<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
									<td class="SubTituloCentro">Meses</td>
									<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
									<td class="SubTituloCentro">Valor Total</td>
								</tr>
								<tr>
									<td align="center"><? echo (($coordenadorgeralrec['valorbrutoremuneracao'])?number_format($coordenadorgeralrec['valorbrutoremuneracao'],2,",","."):'&nbsp'); ?></td>
									<td align="center"><? echo (($coordenadorgeralrec['qtdmeses'])?$coordenadorgeralrec['qtdmeses']:'28'); ?></td>
									<td align="center"><? echo $coordenadorgeralrec['encargossociais']; ?></td>
									<td align="center"><? echo (($coordenadorgeralrec['vlrtotal'])?number_format($coordenadorgeralrec['vlrtotal'],2,",","."):'&nbsp'); ?></td>
								</tr>
							</table>
						</td>
					</tr>
			</table>
		<? else : ?>
		<b>Não cadastrado</b>
		<? endif; ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Coordenador de Turma</td>
		<td>
		<? if($coordturma) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($coordturma['qtdcontratado'])?$coordturma['qtdcontratado']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($coordturma['ocpid'])?$db->pegaUm("SELECT nome FROM projovemcampo.opcaocontratoprofissional WHERE ocpid='".$coordturma['ocpid']."'"):"Não selecionado"); ?></td>
			</tr>
			<tr  <?=(($coordturma['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?>>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total</td>
					</tr>
					<tr>
						<td align="center"><? echo (($coordturmarec['valorbrutoremuneracao'])?number_format($coordturmarec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($coordturmarec['qtdmeses'])?$coordturmarec['qtdmeses']:'3'); ?></td>
						<td align="center"><? echo $coordturmarec['encargossociais']; ?></td>
						<td align="center"><? echo (($coordturmarec['vlrtotal'])?number_format($coordturmarec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr <?=(($coordturma['ocpid']==$efeticocomp)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">Número de horas pagas<br> com recursos do programa/mês</td>
							<td class="SubTituloCentro">Valor da hora (R$)</td>
							<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total (R$)</td>
						</tr>
						<tr>
							<td align="center"><? echo (($coordturmaccm['numerohoraspagas'])?number_format($coordturmaccm['numerohoraspagas'],2,",","."):''); ?></td>
							<td align="center"><? echo (($coordturmaccm['valorhora'])?number_format($coordturmaccm['valorhora'],2,",","."):''); ?></td>
							<td align="center"><? echo (($coordturmaccm['valorbrutoremuneracao'])?number_format($coordturmaccm['valorbrutoremuneracao'],2,",","."):''); ?></td>
							<td align="center"><? echo (($coordturmaccm['qtdmeses'])?$coordturmaccm['qtdmeses']:'3'); ?></td>						
							<td align="center"><? echo $coordturmaccm['encargossociais']; ?></td>
							<td align="center"><? echo (($coordturmaccm['vlrtotal'])?number_format($coordturmaccm['vlrtotal'],2,",","."):''); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<b>Não cadastrado</b>
		<? endif; ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Pessoal de Apoio para a Etapa de Matrícula</td>
		<td>
		<? if($assistente) : ?> 
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistente['qtdcontratado'])?$assistente['qtdcontratado']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistente['ocpid'])?$db->pegaUm("SELECT nome FROM projovemcampo.opcaocontratoprofissional WHERE ocpid='".$assistente['ocpid']."'"):"Não selecionado");	?></td>
			</tr>
			<tr id="assistente_P_complementacao" <?=(($assistente['ocpid']==$efetivorecprog)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>
					</tr>
					<tr>
						<td align="center"><? echo (($assistenterec['valorbrutoremuneracao'])?number_format($assistenterec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($assistenterec['qtdmeses'])?$assistenterec['qtdmeses']:'28'); ?></td>
						<td align="center"><? echo $assistenterec['encargossociais']; ?></td>
						<td align="center"><? echo (($assistenterec['vlrtotal'])?number_format($assistenterec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<b>Não cadastrado</b>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Educadores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores/Professores de educação básica</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo ($numeroturmas*3); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_EF_eft['qtdcontratado'])?$educadores_EF_eft['qtdcontratado']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_EF_recprop['qtdcontratado'])?$educadores_EF_recprop['qtdcontratado']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $educadores_EF_rec['qtdcontratado']; ?></td>
						<td align="center"><? echo (($educadores_EF_rec['valorbrutoremuneracao'])?number_format($educadores_EF_rec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EF_rec['qtdmeses'])?$educadores_EF_rec['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_EF_rec['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_EF_rec['vlrtotal'])?number_format($educadores_EF_rec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_EF_ccm['vlrtotal']) : ?>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais<br> e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo $educadores_EF_ccm['qtdcontratado']; ?></td>
						<td align="center"><? echo $educadores_EF_ccm['numerohoraspagas']; ?></td>
						<td align="center"><? echo (($educadores_EF_ccm['valorhora'])?number_format($educadores_EF_ccm['valorhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EF_ccm['valorbrutoremuneracao'])?number_format($educadores_EF_ccm['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EF_ccm['qtdmeses'])?$educadores_EF_ccm['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_EF_ccm['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_EF_ccm['vlrtotal'])?number_format($educadores_EF_ccm['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? endif; ?>
			
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores/Professores do Qualificação Social e Profissional</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo ($numeroturmas); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_EQ_eft['qtdcontratado'])?$educadores_EQ_eft['qtdcontratado']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_EQ_recprop['qtdcontratado'])?$educadores_EQ_recprop['qtdcontratado']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $educadores_EQ_rec['qtdcontratado']; ?></td>
						<td align="center"><? echo (($educadores_EQ_rec['valorbrutoremuneracao'])?number_format($educadores_EQ_rec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EQ_rec['qtdmeses'])?$educadores_EQ_rec['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_EQ_rec['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_EQ_rec['vlrtotal'])?number_format($educadores_EQ_rec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_EQ_ccm ['vlrtotal']) : ?>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo $educadores_EQ_ccm['qtdcontratado']; ?></td>
						<td align="center"><? echo $educadores_EQ_ccm['numerohoraspagas']; ?></td>
						<td align="center"><? echo (($educadores_EQ_ccm['valorhora'])?number_format($educadores_EQ_ccm['valorhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EQ_ccm['valorbrutoremuneracao'])?number_format($educadores_EQ_ccm['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EQ_ccm['qtdmeses'])?$educadores_EQ_ccm['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_EQ_ccm['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_EQ_ccm['vlrtotal'])?number_format($educadores_EQ_ccm['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? endif; ?>
			
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores para acolhimento de crianças de zero a oito anos, filhas de beneficiários do programa</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo ($numeroturmas); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 20 horas</td>
						<td><? echo (($educadores_EP_eft['qtdcontratado'])?$educadores_EP_eft['qtdcontratado']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_EP_recprop['qtdcontratado'])?$educadores_EP_recprop['qtdcontratado']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $educadores_EP_rec['qtdcontratado']; ?></td>
						<td align="center"><? echo (($educadores_EP_rec['valorbrutoremuneracao'])?number_format($educadores_EP_rec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_EP_rec['qtdmeses'])?$educadores_EP_rec['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_EP_rec['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_EP_rec['vlrtotal'])?number_format($educadores_EP_rec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_EP_ccm  ['vlrtotal']) : ?>
				<tr>
					<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">&nbsp;</td>
							<td class="SubTituloCentro">Número de educadores</td>
							<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
							<td class="SubTituloCentro">Valor da hora (R$)</td>
							<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total (R$)</td>
		
						</tr>
						<tr>
							<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
							<td align="center"><? echo $educadores_EP_ccm['qtdcontratado']; ?></td>
							<td align="center"><? echo $educadores_EP_ccm['numerohoraspagas']; ?></td>
							<td align="center"><? echo (($educadores_EP_ccm['valorhora'])?number_format($educadores_EP_ccm['valorhora'],2,",","."):''); ?></td>
							<td align="center"><? echo (($educadores_EP_ccm['valorbrutoremuneracao'])?number_format($educadores_EP_ccm['valorbrutoremuneracao'],2,",","."):''); ?></td>
							<td align="center"><? echo (($educadores_EP_ccm['qtdmeses'])?$educadores_EP_ccm['qtdmeses']:'18'); ?></td>
							<td align="center"><? echo $educadores_EP_ccm['encargossociais']; ?></td>
							<td align="center"><? echo (($educadores_EP_ccm['vlrtotal'])?number_format($educadores_EP_ccm['vlrtotal'],2,",","."):''); ?></td>
						</tr>
					</table>
					</td>
				</tr>
			<? endif; ?>
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo ($numeroturmas); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_ET_eft['qtdcontratado'])?$educadores_ET_eft['qtdcontratado']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_ET_recprop['qtdcontratado'])?$educadores_ET_recprop['qtdcontratado']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de educadores</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $educadores_ET_rec['qtdcontratado']; ?></td>
						<td align="center"><? echo (($educadores_ET_rec['valorbrutoremuneracao'])?number_format($educadores_ET_rec['valorbrutoremuneracao'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_ET_rec['qtdmeses'])?$educadores_ET_rec['qtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_ET_rec['encargossociais']; ?></td>
						<td align="center"><? echo (($educadores_ET_rec['vlrtotal'])?number_format($educadores_ET_rec['vlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educador/Professor para atendimento educacional especializado</td>
		<td>
		<? if($numeroturmas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_E )?$educadores_E :''); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo (($educadores_EE_eft['qtdcontratado'])?$educadores_EE_eft['qtdcontratado']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_EE_recprop['qtdcontratado'])?$educadores_EE_recprop['qtdcontratado']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		
		<? else : ?>
		<p><b>Não existem turmas cadastrados</b></p>
		<? endif; ?>
	
		</td>
	</tr>
	<?if($infoInstLegal){?>
		<tr>
			<td class="SubTituloDireita">Instrumento legal</td>
			<td>
			<table cellspacing="3" cellpadding="3">
			<?foreach ($infoInstLegal as $instrumento){?>
				<tr>
					<td class="SubTituloDireita"><b>Número do instrumento :</b></td>
					<td><? echo $instrumento['numero']; ?></td>
					<td class="SubTituloDireita"><b>Data do instrumento :</b></td>
					<td><? echo formata_data($instrumento['data']); ?></td>
				</tr>
			<?}?>
			</table>
			</td>
		</tr>
	<?}else{?>
		<p><b>Não existem turmas cadastrados</b></p>
	<?}?>
</table>

<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Formação de educadores</label></td></tr></tbody></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

	<tr>
		<td class="SubTituloDireita" width="25%">Percentual da Ação</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
			<td class="SubTituloCentro">Percentual utilizado(%)</td>
		</tr>
		<tr>
			<td align="center"><? echo ($percmaxform); ?>% - R$ <span id="span_totalmaximo"><?=number_format($montante*($percmaxform)/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($totalutilizadoform)?number_format(($totalutilizadoform/$montante)*100,2,",","."):'0.0'); ?>% - R$ <span id="span_totalutilizado"><?=number_format($totalutilizadoform,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos gastos com a formação</td>
		<td>
		<?
		
		if(!$_SESSION['projovemcampo']['estuf']){
			$AND =" WHERE tpf.tpfid != 6" ;
		}
		$sql1 = "SELECT 
					CASE WHEN valor IS NULL THEN '0' ELSE valor END as inp 
				FROM 
					projovemcampo.tiporecursoplanoimplementacao trp
				WHERE
					trp.tpfid = tpf.tpfid 
				AND	pimid = {$_SESSION['projovemcampo']['pimid']}"; 
		$sql = "SELECT 
					nome, 
					'<input type=\"text\" class=\"disabled\" title=\"Valor\" id=\"tpfid_'||tpf.tpfid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal1();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calculaPorcentTotal1();\" value=\"'||COALESCE(to_char(($sql1),'999g999g999d99'),'')||'\" maxlength=\"14\" size=\"16\" name=\"valor['||tpf.tpfid||']\" style=\"text-align:;\">' as inp 
				FROM 
					projovemcampo.tiposrecursoformacao tpf
					$AND
                    ";
		$cabecalho=array("&nbsp;","Valor(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual da Ação</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
			<td class="SubTituloCentro">Percentual utilizado(%)</td>
		</tr>
		<tr>
			<td align="center"><? echo ($percmaxauxilio); ?>% - R$ <span id="span_totalmaximo2"><?=number_format($montante*($percmaxauxilio)/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($auxiliofinanceiro['qtdeduccontinuada'])?number_format(($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc']*100)/$montante,2,",","."):'0.0'); ?>% - R$ <span id="span_totalutilizado2"><?=number_format($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc'],2,",",".") ?></span></td>
		</tr>
		</table>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação continuada de educadores de ensino fundamental, qualificação profissional e participação cidadã</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Número de educadores previstos para primeira etapa de formação continuada</td>
			<td class="SubTituloCentro">Auxílio financeiro a ser pago(R$)</td>
			<td class="SubTituloCentro">Valor total Auxílio(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo (($auxiliofinanceiro['qtdeduccontinuada'])?$auxiliofinanceiro['qtdeduccontinuada']:''); ?></td>
			<td align="center"><? echo (($auxiliofinanceiro['auxfinanceiroeduc'])?number_format($auxiliofinanceiro['auxfinanceiroeduc'],2,",","."):''); ?></td>
			<td align="center"><? echo number_format(($auxiliofinanceiro['qtdeduccontinuada']*$auxiliofinanceiro['auxfinanceiroeduc']),2,",","."); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
</table>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Gêneros Alimenticios</label></td></tr></tbody></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Percentual da Ação</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
				<tr>
					<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
					<td class="SubTituloCentro">Percentual utilizado(%)</td>
				</tr>
				<tr>
					<td align="center"><? echo ($percmaxalimentos); ?>% - R$ <span><?=number_format($montante*($percmaxalimentos)/100,2,",",".") ?></span></td>
					<td align="center"><? echo (number_format(((($generoalimenticio['gaqtdcriancas']+$meta)*$generoalimenticio['gavalormensal']*$generoalimenticio['gaqtdmeses'])*100)/$montante,2,",",".")); ?>% - R$ <span id="span_totalutilizado"><?=number_format((($generoalimenticio['gaqtdcriancas']+$meta)*$generoalimenticio['gavalormensal']*$generoalimenticio['gaqtdmeses']),2,",","."); ?></span></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do Lanche ou Refeição</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Meta</td>
			<td class="SubTituloCentro">Crianças filhas de estudantes(10% da meta)</td>
			<td class="SubTituloCentro">Meses</td>
			<td class="SubTituloCentro">Valor mensal do lanche ou Refeição por pessoa(R$)</td>
			<td class="SubTituloCentro">Valor total(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo (($meta)?$meta:''); ?></td>
			<td align="center"><? echo $generoalimenticio ['gaqtdcriancas']; ?></td>
			<td align="center"><? echo (($generoalimenticio ['gaqtdmeses'])?$generoalimenticio ['gaqtdmeses']:'24'); ?></td>
			<td align="center"><? echo (($generoalimenticio ['gavalormensal'])?number_format($generoalimenticio ['gavalormensal'],2,",","."):''); ?></td>
			<td align="center"><? echo (($generoalimenticio ['gavalormensal'])?number_format((($generoalimenticio['gaqtdcriancas']+$meta)*$generoalimenticio['gavalormensal']*$generoalimenticio['gaqtdmeses']),2,",","."):''); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
</table>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Qualificação Profissional</label></td></tr></tbody></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Percentual da Ação</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
				<tr>
					<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
					<td class="SubTituloCentro">Percentual utilizado(%)</td>
				</tr>
				<tr>
					<td align="center"><? echo ($percmaxqualificaco) ?>% - R$ <span><?=number_format($montante*$percmaxqualificaco/100,2,",",".") ?></span></td>
					<td align="center"><? echo (($totalqualificacao)?number_format(($totalqualificacao/$montante)*100,1):'0.0'); ?>% - R$ 
 						<span id="span_utilizado"> 	
 					<?=number_format($totalqualificacao,2,",",".") ?></span></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Tipo de oferta</td>
		<td>
		<?
		$sql1="SELECT
					valor
				FROM
					projovemcampo.despesaqualificacaoplano dqp
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql2="SELECT
					qtdmeses
				FROM
					projovemcampo.despesaqualificacaoplano dqp 
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql3 = "SELECT
					qtdmeses*valor as valortotal
				FROM
					projovemcampo.despesaqualificacaoplano dqp 
				WHERE
					dqp.qpdid = qpd.qpdid
				AND 	pimid = {$_SESSION['projovemcampo']['pimid']}";
		$sql = "SELECT
					nome,
					'<input type=\"text\" class=\"disabled\" title=\"Valor\" id=\"valor_'||qpd.qpdid||'\" onblur=\"MouseBlur(this);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" value=\"'||COALESCE(to_char(($sql1),'999g999g999d99'),'')||'\" maxlength=\"14\" size=\"16\" name=\"valor['||qpd.qpdid||']\" style=\"text-align:;\">' as inp1,
					'<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"qtdmeses_'||qpd.qpdid||'\" onblur=\"MouseBlur(this);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);calcularValorTotal(\''||qpd.qpdid||'\');calculaPorcentTotal();\" value=\"'||COALESCE(($sql2),24)||'\" maxlength=\"7\" size=\"8\" name=\"qtdmeses['||qpd.qpdid||']\" style=\"text-align:;\">' as inp2,
					'<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"valortotal_'||qpd.qpdid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" value=\"'||COALESCE(($sql3),'0')||'\" maxlength=\"14\" size=\"16\" style=\"text-align:;\">' as inp3 
				FROM
					projovemcampo.qualificacaoprofissionaldespesa qpd
				LEFT JOIN projovemcampo.despesaqualificacaoplano dqp ON dqp.qpdid = qpd.qpdid
				";
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
 		?>
		</td>
	</tr>
</table>
 <?//if($_SESSION['projovemcampo']['estuf']){?>
	<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Transporte de Material Didático</label></td></tr></tbody></table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="25%">Percentual da Ação</td>
			<td>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
					<tr>
						<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
						<td class="SubTituloCentro">Percentual utilizado(%)</td>
					</tr>
					<tr>
						<td align="center"><? echo number_format($percmaxtransporte,2); ?>% - R$ <span><?=number_format($montante*$percmaxtransporte/100,2,",",".") ?></span></td>
						<td align="center"><? echo (($planodeimplementacao['valormaterialdidatico'])?number_format(($planodeimplementacao['valormaterialdidatico']*100)/$montante,1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($planodeimplementacao['valormaterialdidatico'],2,",",".") ?></span></td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
 <?//}?>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Demais ações</label></td></tr></tbody></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Percentual da Ação</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
			<td class="SubTituloCentro">Percentual utilizado(%)</td>
		</tr>
		<tr>
			<td align="center"><? echo number_format(($totalMax*100)/$montante,2); ?>% - R$ <span><?=number_format($totalMax,2,",",".") ?></span></td>
			<td align="center"><? echo (($totalutilizadodemais)?number_format(($totalutilizadodemais*100)/$montante,2):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($totalutilizadodemais,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Demais ações</td>
		<td>
		<?
			$sql1 = "SELECT 
						CASE WHEN valorpormes IS NULL 
						THEN '' 
						ELSE trim(to_char(valorpormes,'999g999g999d99')) END 
					FROM 
						projovemcampo.tipodemaisacoesplano tpa
					WHERE 
						tpa.tdaid=tda.tdaid 
					AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
			$sql2 = "SELECT 
						CASE WHEN qtdmeses IS NULL 
						THEN '' 
						ELSE trim(to_char(qtdmeses,'999')) END 
					FROM 
						projovemcampo.tipodemaisacoesplano tpa
					WHERE 
						tpa.tdaid=tda.tdaid 
					AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
			$sql3 = "SELECT 
						CASE WHEN qtdmeses IS NULL 
						THEN '' 
						ELSE trim(to_char(valortotal,'999g999g999d99')) END 
					FROM 
						projovemcampo.tipodemaisacoesplano tpa
					WHERE 
						tpa.tdaid=tda.tdaid 
					AND	tpa.pimid = {$_SESSION['projovemcampo']['pimid']}";
			
			$sql = "SELECT 
						nome,
				   		'<input type=\"text\" class=\"disabled\" title=\"Valor\" id=\"valorpormes_'||tda.tdaid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal();\" 
				   			onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
				   			onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||tda.tdaid||'\');calculaPorcentTotal();\" 
				   			value=\"'||COALESCE(($sql1),'')||'\" maxlength=\"14\" size=\"16\" name=\"valorpormes['||tdaid||']\" 
							style=\"text-align:;\">' as inp1, 
				   		'<input type=\"text\" class=\"disabled\" title=\"Valor\" id=\"qtdmeses_'||tda.tdaid||'\" onblur=\"MouseBlur(this);calculaPorcentTotal();\" 
				   			onmouseout=\"MouseOut(this);\"onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
				   			onkeyup=\"this.value=mascaraglobal(\'########\',this.value);calcularValorTotal(\''||tda.tdaid||'\');calculaPorcentTotal();\"
							value=\"'||COALESCE(($sql2),'24')||'\" maxlength=\"7\" size=\"8\" name=\"qtdmeses['||tdaid||']\" 
							style=\"text-align:;\">' as inp2,
				   		'<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"valortotal_'||tda.tdaid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" 
							onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" 
							value=\"'||COALESCE(($sql3),'')||'\" maxlength=\"14\" size=\"16\" 
							name=\"valortotal['||tdaid||']\" style=\"text-align:;\">' as inp4
					FROM projovemcampo.tiposdemaisacoes tda 
					--WHERE 
						--ppuid = {$_SESSION['projovemcampo']['ppuid']}
					ORDER BY tda.tdaid";
		$cabecalho=array("Demais Ações","Valor/mês(R$)","Meses(24)","Valor total(R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
<?
$sql = "
		SELECT 
			d.esdid 
		FROM projovemcampo.adesaoprojovemcampo p
		INNER JOIN workflow.documento d ON d.docid=p.docid 
		WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'";

$esdid = $db->pegaUm($sql);

if($esdid==ESD_VALIDADOMEC) :
?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		ASSINATURA DO SECRETÁRIO
		<br><br><br>
		<p>________________, _____ de ________________ de _______</p>
		<br><br><br>
		<? if($_SESSION['projovemcampo']['estuf']) : ?>
		<p>_______________________________________________________________<br>
		Secretário(a) Estadual de Educação</p>
		<? endif; ?>
		
		
		<? if($_SESSION['projovemcampo']['muncod']) : ?>
		<p>_______________________________________________________________<br>
		Secretário(a) Municipal de Educação</p>
		<? endif; ?>
		
		</td>
	</tr>
<? endif; ?>
</table>
<?registarUltimoAcesso();?>