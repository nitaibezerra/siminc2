<?
/* Carregando profissionais */ 
criaSessaoProfissionais(); 
$profissionais = $db->pegaLinha("SELECT * FROM projovemurbano.profissionais WHERE proid='".$_SESSION['projovemurbano']['proid']."'");

$coordenadorgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);
$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$diretorpolo = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');

$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');
		
$numeropolos = pegarNumeroPolos();

if($numeropolos) {
	$numeronucleos = pegarNumeroNucleos(true);
} else {
	$numeronucleos = pegarNumeroNucleos(false);
}
/* Fim - Carregando profissionais */

/* Carregando Formação Educadores */
criaSessaoFormacaoEducadores();
$sql = "SELECT * FROM projovemurbano.formacaoeducadores WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'";
$formacaoeducadores = $db->pegaLinha($sql);

$auxiliofinanceiro = $db->pegaLinha("SELECT * FROM projovemurbano.auxiliofinanceiro WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'");

$fedqtd = $educadores_F['eduqtd']+$educadores_Q['eduqtd']+$educadores_P['eduqtd'];

$arrT = array("vlrprofrecurso" => $coordassistentes['coavlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'], 
			  "vlrprofcomplem" => $coordgeral['cgevlrtotal']+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal'],
			  "qtdeducrecursoFQP" => $educadores_F['creqtd']+$educadores_Q['creqtd']+$educadores_P['creqtd']+$educadores_F['eduefetivo30hr']+$educadores_Q['eduefetivo30hr']+$educadores_P['eduefetivo30hr'],
			  "vlreducrecursoFQP" => $educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']);
/* Fim - Carregando Formação Educadores */

/* Carregando Genero Alimenticios */
criaSessaoGeneroAlimenticios();
$generoalimenticio = $db->pegaLinha("SELECT * FROM projovemurbano.generoalimenticio WHERE galid='".$_SESSION['projovemurbano']['galid']."'");
$lancherefeicao = $db->pegaLinha("SELECT * FROM projovemurbano.lancherefeicao WHERE galid='".$_SESSION['projovemurbano']['galid']."'");
/* Fim - Carregando Genero Alimenticios */

/* Carregando Qualificação Profissional */
criaSessaoQualificacaoProfissional();
$qualificacaoprofissional = $db->pegaLinha("SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='".$_SESSION['projovemurbano']['qprid']."'");
/* Fim - Carregando Qualificação Profissional */

/* Carregando Demais Ações */
criaSessaoDemaisAcoes();
$sql = "SELECT propercutilizado FROM projovemurbano.profissionais WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$propercutilizado = $db->pegaUm($sql);
$sql = "SELECT fedperutilizado FROM projovemurbano.formacaoeducadores WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$fedperutilizado = $db->pegaUm($sql);
$sql = "SELECT galpercutilizado FROM projovemurbano.generoalimenticio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$galpercutilizado = $db->pegaUm($sql);
$sql = "SELECT qprpercutilizado FROM projovemurbano.qualificacaoprofissional WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$qprpercutilizado = $db->pegaUm($sql);
$sql = "SELECT tmaperutilizado FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$tmaperutilizado = $db->pegaUm($sql);
$sql = "SELECT deapercutilizado FROM projovemurbano.demaisacoes WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$deapercutilizado = $db->pegaUm($sql);
$sql = "SELECT aufpercutilizado FROM projovemurbano.auxiliofinanceiro aux 
								 INNER JOIN projovemurbano.formacaoeducadores fed ON fed.fedid = aux.fedid  
								 WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$aufpercutilizado = $db->pegaUm($sql);

$deapercmax = 100-($propercutilizado+$fedperutilizado+$galpercutilizado+$qprpercutilizado+$tmaperutilizado+$aufpercutilizado);
/* Fim - Carregando Demais Ações */


if($_SESSION['projovemurbano']['muncod']) {
	$linkproximo = "Municipio";	
	$refesfera="M";
	
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}
 
if($_SESSION['projovemurbano']['estuf']) {
	$linkproximo = "Estado";	
	$refesfera = "E";
	
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}
?>
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela"><tbody><tr><td width="100%" align="center"><label style="color:#000000;" class="TituloTela">Profissionais</label></td></tr></tbody></table>
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
				<td align="center"><? echo (($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax); ?>% - R$ <span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*(($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax)/100,2,",",".") ?></span></td>
				<td align="center"><? echo (($profissionais['propercutilizado'])?$profissionais['propercutilizado']:'0.0'); ?>% - R$ <span id="span_totalutilizado"><?=number_format($coordenadorgeral['cgevlrtotal']+$assistenteadministrativo_A['coavlrtotal']+$assistenteadministrativo_P['coavlrtotal']+$diretorpolo['ccmvlrtotal']+$dirassistentes_A['crevlrtotal']+$dirassistentes_P['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+$educadores_M['ccmvlrtotal']+$educadores_T['ccmvlrtotal'],2,",",".") ?></span></td>
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
				<td><? echo (($coordenadorgeral['cgeqtd'])?$coordenadorgeral['cgeqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($coordenadorgeral['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='C' AND oppid='".$coordenadorgeral['oppid']."'"):"Não selecionado"); ?></td>
			</tr>

			<tr id="coordenadorgeral_complementacao" <?=(($coordenadorgeral['oppid']==OPP_EFETIVO_COMPLEMENTACAO)?"":"style=\"display:none;\"") ?> >
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
						<td align="center"><? echo (($coordenadorgeral['cgehorpagrec'])?$coordenadorgeral['cgehorpagrec']:'&nbsp;'); ?></td>
						<td align="center"><? echo (($coordenadorgeral['cgevlrhora'])?number_format($coordenadorgeral['cgevlrhora'],2,",","."):'&nbsp;'); ?></td>
						<td align="center"><? echo (($coordenadorgeral['cgevlrcomp'])?number_format($coordenadorgeral['cgevlrcomp'],2,",","."):'&nbsp;'); ?></td>
						
						<td align="center"><? echo (($coordenadorgeral['cgeqtdmeses'])?$coordenadorgeral['cgeqtdmeses']:'22'); ?></td>						
						<td align="center"><? echo $coordenadorgeral['cgeencargos']; ?></td>
						<td align="center"><? echo (($coordenadorgeral['cgevlrtotal'])?number_format($coordenadorgeral['cgevlrtotal'],2,",","."):'&nbsp;'); ?></td>
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
		<td class="SubTituloDireita">Assistente Administrativo</td>
		<td>
		<? if($assistenteadministrativo_A) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistenteadministrativo_A['coaqtd'])?$assistenteadministrativo_A['coaqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistenteadministrativo_A['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='A' AND oppid='".$assistenteadministrativo_A['oppid']."'"):"Não selecionado"); ?></td>
			</tr>


			<tr id="assistente_A_complementacao" <?=(($assistenteadministrativo_A['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_A)?"":"style=\"display:none;\"") ?>>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total</td>
					</tr>
					<tr>
						<td align="center"><? echo (($assistenteadministrativo_A['coavlrbrutorem'])?number_format($assistenteadministrativo_A['coavlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($assistenteadministrativo_A['coaqtdmeses'])?$assistenteadministrativo_A['coaqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $assistenteadministrativo_A['coaencargos']; ?></td>
						<td align="center"><? echo (($assistenteadministrativo_A['coavlrtotal'])?number_format($assistenteadministrativo_A['coavlrtotal'],2,",","."):''); ?></td>
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
		<td class="SubTituloDireita">Assistente Pedagógico</td>
		<td>
		<? if($assistenteadministrativo_P) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistenteadministrativo_P['coaqtd'])?$assistenteadministrativo_P['coaqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistenteadministrativo_P['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='P' AND oppid='".$assistenteadministrativo_P['oppid']."'"):"Não selecionado");	?></td>
			</tr>


			<tr id="assistente_P_complementacao" <?=(($assistenteadministrativo_P['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_P)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>
					</tr>
					<tr>
						<td align="center"><? echo (($assistenteadministrativo_P['coavlrbrutorem'])?number_format($assistenteadministrativo_P['coavlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($assistenteadministrativo_P['coaqtdmeses'])?$assistenteadministrativo_P['coaqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $assistenteadministrativo_P['coaencargos']; ?></td>
						<td align="center"><? echo (($assistenteadministrativo_P['coavlrtotal'])?number_format($assistenteadministrativo_P['coavlrtotal'],2,",","."):''); ?></td>
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
		<td class="SubTituloCentro" colspan="2">Direção de polo</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Diretor de polo</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($diretorpolo['dipqtd'])?$diretorpolo['dipqtd']:$numeropolos); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo (($diretorpolo['dipeqtdefetivo40hr'])?$diretorpolo['dipeqtdefetivo40hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($diretorpolo['dipqtdrecursoproprio'])?$diretorpolo['dipqtdrecursoproprio']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de diretor de polo</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo $diretorpolo['ccmqtd']; ?></td>
						<td align="center"><? echo $diretorpolo['ccmqtdhoras']; ?></td>
						<td align="center"><? echo (($diretorpolo['ccmvlrhora'])?number_format($diretorpolo['ccmvlrhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($diretorpolo['ccmvlrcomp'])?number_format($diretorpolo['ccmvlrcomp'],2,",","."):''); ?></td>
						<td align="center"><? echo (($diretorpolo['ccmqtdmeses'])?$diretorpolo['ccmqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $diretorpolo['ccmencargos']; ?></td>
						<td align="center"><? echo (($diretorpolo['ccmvlrtotal'])?number_format($diretorpolo['ccmvlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Administrativo</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($dirassistentes_A['dasqtd'])?$dirassistentes_A['dasqtd']:$numeropolos); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo $dirassistentes_A['dasqtdefetivo40hr']; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo $dirassistentes_A['dasqtdrecursoproprio']; ?></td>
					</tr>
				</table>
				</td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de assistentes</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $dirassistentes_A['creqtd']; ?></td>
						<td align="center"><? echo (($dirassistentes_A['crevlrbrutorem'])?number_format($dirassistentes_A['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($dirassistentes_A['creqtdmeses'])?$dirassistentes_A['creqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $dirassistentes_A['creencargos']; ?></td>
						<td align="center"><? echo (($dirassistentes_A['crevlrtotal'])?number_format($dirassistentes_A['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Assistente Pedagógico</td>
		<td>
		<? if($numeropolos>0) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($dirassistentes_P['dasqtd'])?$dirassistentes_P['dasqtd']:$numeropolos); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 40 horas</td>
						<td><? echo (($dirassistentes_P['dasqtdefetivo40hr'])?$dirassistentes_P['dasqtdefetivo40hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($dirassistentes_P['dasqtdrecursoproprio'])?$dirassistentes_P['dasqtdrecursoproprio']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de assistentes</td>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Contrato com recursos do programa</td>
						<td align="center"><? echo $dirassistentes_P['creqtd']; ?></td>
						<td align="center"><? echo (($dirassistentes_P['crevlrbrutorem'])?number_format($dirassistentes_P['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($dirassistentes_P['creqtdmeses'])?$dirassistentes_P['creqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $dirassistentes_P['creencargos']; ?></td>
						<td align="center"><? echo (($dirassistentes_P['crevlrtotal'])?number_format($dirassistentes_P['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem polos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Educadores</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores do Ensino Fundamental</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_F['eduqtd'])?$educadores_F['eduqtd']:($numeronucleos*5)); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_F['eduefetivo30hr'])?$educadores_F['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_F['eduqtdrecursoproprio'])?$educadores_F['eduqtdrecursoproprio']:'0'); ?></td>
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
						<td align="center"><? echo $educadores_F['creqtd']; ?></td>
						<td align="center"><? echo (($educadores_F['crevlrbrutorem'])?number_format($educadores_F['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_F['creqtdmeses'])?$educadores_F['creqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_F['creencargos']; ?></td>
						<td align="center"><? echo (($educadores_F['crevlrtotal'])?number_format($educadores_F['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_F['ccmvlrtotal']) : ?>
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
						<td align="center"><? echo $educadores_F['ccmqtd']; ?></td>
						<td align="center"><? echo $educadores_F['ccmqtdhoras']; ?></td>
						<td align="center"><? echo (($educadores_F['ccmvlrhora'])?number_format($educadores_F['ccmvlrhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_F['ccmvlrcomp'])?number_format($educadores_F['ccmvlrcomp'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_F['ccmqtdmeses'])?$educadores_F['ccmqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_F['ccmencargos']; ?></td>
						<td align="center"><? echo (($educadores_F['ccmvlrtotal'])?number_format($educadores_F['ccmvlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? endif; ?>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores do Qualificação Profissional</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_Q['eduqtd'])?$educadores_Q['eduqtd']:$numeronucleos); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_Q['eduefetivo30hr'])?$educadores_Q['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_Q['eduqtdrecursoproprio'])?$educadores_Q['eduqtdrecursoproprio']:'0'); ?></td>
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
						<td align="center"><? echo $educadores_Q['creqtd']; ?></td>
						<td align="center"><? echo (($educadores_Q['crevlrbrutorem'])?number_format($educadores_Q['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_Q['creqtdmeses'])?$educadores_Q['creqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_Q['creencargos']; ?></td>
						<td align="center"><? echo (($educadores_Q['crevlrtotal'])?number_format($educadores_Q['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_Q['ccmvlrtotal']) : ?>
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
						<td align="center"><? echo $educadores_Q['ccmqtd']; ?></td>
						<td align="center"><? echo $educadores_Q['ccmqtdhoras']; ?></td>
						<td align="center"><? echo (($educadores_Q['ccmvlrhora'])?number_format($educadores_Q['ccmvlrhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_Q['ccmvlrcomp'])?number_format($educadores_Q['ccmvlrcomp'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_Q['ccmqtdmeses'])?$educadores_Q['ccmqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_Q['ccmencargos']; ?></td>
						<td align="center"><? echo (($educadores_Q['ccmvlrtotal'])?number_format($educadores_Q['ccmvlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? endif; ?>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_P['eduqtd'])?$educadores_P['eduqtd']:$numeronucleos); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_P['eduefetivo30hr'])?$educadores_P['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_P['eduqtdrecursoproprio'])?$educadores_P['eduqtdrecursoproprio']:'0'); ?></td>
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
						<td align="center"><? echo $educadores_P['creqtd']; ?></td>
						<td align="center"><? echo (($educadores_P['crevlrbrutorem'])?number_format($educadores_P['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_P['creqtdmeses'])?$educadores_P['creqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_P['creencargos']; ?></td>
						<td align="center"><? echo (($educadores_P['crevlrtotal'])?number_format($educadores_P['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? if($educadores_P['ccmvlrtotal']) : ?>
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
						<td align="center"><? echo $educadores_P['ccmqtd']; ?></td>
						<td align="center"><? echo $educadores_P['ccmqtdhoras']; ?></td>
						<td align="center"><? echo (($educadores_P['ccmvlrhora'])?number_format($educadores_P['ccmvlrhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_P['ccmvlrcomp'])?number_format($educadores_P['ccmvlrcomp'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_P['ccmqtdmeses'])?$educadores_P['ccmqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_P['ccmencargos']; ?></td>
						<td align="center"><? echo (($educadores_P['ccmvlrtotal'])?number_format($educadores_P['ccmvlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<? endif; ?>
			
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educadores para monitoramento do acolhimento ás crianças de 0 a 8 anos, filhas do jovens atendidos pelo programa</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_M['eduqtd'])?$educadores_M['eduqtd']:''); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 20 horas</td>
						<td><? echo (($educadores_M['eduefetivo30hr'])?$educadores_M['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_M['eduqtdrecursoproprio'])?$educadores_M['eduqtdrecursoproprio']:'0'); ?></td>
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
						<td align="center"><? echo $educadores_M['creqtd']; ?></td>
						<td align="center"><? echo (($educadores_M['crevlrbrutorem'])?number_format($educadores_M['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_M['creqtdmeses'])?$educadores_M['creqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_M['creencargos']; ?></td>
						<td align="center"><? echo (($educadores_M['crevlrtotal'])?number_format($educadores_M['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_T['eduqtd'])?$educadores_T['eduqtd']:$numeronucleos); ?></td>
			</tr>
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo com 30 horas</td>
						<td><? echo (($educadores_T['eduefetivo30hr'])?$educadores_T['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_T['eduqtdrecursoproprio'])?$educadores_T['eduqtdrecursoproprio']:'0'); ?></td>
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
						<td align="center"><? echo $educadores_T['creqtd']; ?></td>
						<td align="center"><? echo (($educadores_T['crevlrbrutorem'])?number_format($educadores_T['crevlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($educadores_T['creqtdmeses'])?$educadores_T['creqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $educadores_T['creencargos']; ?></td>
						<td align="center"><? echo (($educadores_T['crevlrtotal'])?number_format($educadores_T['crevlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Educador para atendimento educacional especializado</td>
		<td>
		<? if($numeronucleos) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($educadores_E['eduqtd'])?$educadores_E['eduqtd']:''); ?></td>
			</tr>

			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita" width="30%">Efetivo</td>
						<td><? echo (($educadores_E['eduefetivo30hr'])?$educadores_E['eduefetivo30hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($educadores_E['eduqtdrecursoproprio'])?$educadores_E['eduqtdrecursoproprio']:'0'); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			
		</table>
		
		<? else : ?>
		<p><b>Não existem núcleos cadastrados</b></p>
		<? endif; ?>
	
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Instrumento legal</td>
		<td>
		<table cellspacing="1" cellpadding="1">
			<tr>
				<td class="SubTituloDireita"><b>Número do instrumento :</b></td>
				<td><? echo $profissionais['pronuminstlegal']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Data do instrumento :</b></td>
				<td><? echo formata_data($profissionais['prodatainstlegal']); ?></td>
			</tr>
		</table>
		</td>
	</tr>
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
			<td align="center"><? echo (($formacaoeducadores['fedpercmax'])?$formacaoeducadores['fedpercmax']:'5.0'); ?>% - R$ <span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*(($formacaoeducadores['fedpercmax'])?$formacaoeducadores['fedpercmax']:'5.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($formacaoeducadores['fedperutilizado'])?$formacaoeducadores['fedperutilizado']:'0.0'); ?>% - R$ <span id="span_totalutilizado"><?=number_format($db->pegaUm("SELECT SUM(rgavalor) FROM projovemurbano.recursosgastos WHERE fedid='".$_SESSION['projovemurbano']['fedid']."' AND rgastatus='A'"),2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos gastos com a formação</td>
		<td>
		<p><b>Quantidade:</b> <? echo $fedqtd; ?></p>
		<?
		
		$sql = "SELECT refdesc, 
					   CASE WHEN rec.rgavalor IS NULL THEN '' ELSE trim(to_char(rec.rgavalor,'999g999g999d99')) END as inp 
				FROM projovemurbano.referenciaformacao ref 
				LEFT JOIN projovemurbano.recursosgastos rec ON rec.refid=ref.refid AND rec.fedid='".$_SESSION['projovemurbano']['fedid']."' 
				WHERE refstatus='A' AND refesfera='$refesfera'";
		
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
			<td align="center"><? echo (($auxiliofinanceiro['aufpercmax'])?$auxiliofinanceiro['aufpercmax']:'1.5'); ?>% - R$ <span id="span_totalmaximo2"><?=number_format(calcularMontante($meta)*(($auxiliofinanceiro['aufpercmax'])?$auxiliofinanceiro['aufpercmax']:'1.5')/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($auxiliofinanceiro['aufpercutilizado'])?$auxiliofinanceiro['aufpercutilizado']:'0.0'); ?>% - R$ <span id="span_totalutilizado2"><?=number_format($auxiliofinanceiro['aufqtdeducador']*$auxiliofinanceiro['aufavlrauxilio'],2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação continuada de educadores de ensino fundamental, qualificação profissional e participação cidadã</td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Número de educadores previstos para primeira etapa de formação continuada</td>
			<td class="SubTituloCentro">Auxílio financeiro a ser pago(R$)</td>
		</tr>
		<tr>
			<td align="center"><? echo (($auxiliofinanceiro['aufqtdeducador'])?$auxiliofinanceiro['aufqtdeducador']:''); ?></td>
			<td align="center"><? echo (($auxiliofinanceiro['aufavlrauxilio'])?number_format($auxiliofinanceiro['aufavlrauxilio'],2,",","."):''); ?></td>
			<td align="center"><? echo number_format(($auxiliofinanceiro['aufqtdeducador']*$auxiliofinanceiro['aufavlrauxilio']),2,",","."); ?></td>
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
			<td align="center"><? echo (($generoalimenticio['galpercmax'])?$generoalimenticio['galpercmax']:'10.0'); ?>% - R$ <span><?=number_format(calcularMontante($meta)*(($generoalimenticio['galpercmax'])?$generoalimenticio['galpercmax']:'10.0')/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($generoalimenticio['galpercutilizado'])?$generoalimenticio['galpercutilizado']:'0.0'); ?>% - R$ <span id="span_totalutilizado"><?=number_format($lancherefeicao['lrevlrtotal'],2,",",".") ?></span></td>
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
			<td align="center"><? echo (($lancherefeicao['lremeta'])?$lancherefeicao['lremeta']:$meta); ?></td>
			<td align="center"><? echo $lancherefeicao['lreqtdcrianca']; ?></td>
			<td align="center"><? echo (($lancherefeicao['lreqtdmeses'])?$lancherefeicao['lreqtdmeses']:'18'); ?></td>
			<td align="center"><? echo (($lancherefeicao['lrevlrreflanche'])?number_format($lancherefeicao['lrevlrreflanche'],2,",","."):''); ?></td>
			<td align="center"><? echo (($lancherefeicao['lrevlrtotal'])?number_format($lancherefeicao['lrevlrtotal'],2,",","."):''); ?></td>
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
			<td align="center"><? echo number_format($qualificacaoprofissional['qprpercmax'],1); ?>% - R$ <span><?=number_format(calcularMontante($meta)*$qualificacaoprofissional['qprpercmax']/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($qualificacaoprofissional['qprpercutilizado'])?number_format($qualificacaoprofissional['qprpercutilizado'],1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($db->pegaUm("SELECT SUM(COALESCE(pgaqtdmeses,0)*COALESCE(pgavlrmes,0)) FROM projovemurbano.previsaogasto WHERE pgastatus='A' AND qprid='".$_SESSION['projovemurbano']['qprid']."'"),2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Tipo de oferta</td>
		<td>
		<?
		$sql = "SELECT desdesc,
					   COALESCE((SELECT DISTINCT CASE WHEN pgavlrmes IS NULL THEN '' ELSE trim(to_char(pgavlrmes,'999g999g999d99')) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'') as a, 
					   COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NULL THEN '10' ELSE trim(pgaqtdmeses::text) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'10') as b,
					   COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL THEN trim(to_char(pgaqtdmeses*pgavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'') as c
				FROM projovemurbano.despesas des 
				WHERE desstatus='A'";
		
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
 		?>
		</td>
	</tr>
</table>
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
			<td align="center"><? echo number_format($deapercmax,1); ?>% - R$ <span><?=number_format(calcularMontante($meta)*$deapercmax/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($deapercutilizado)?number_format($deapercutilizado,1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($db->pegaUm("SELECT SUM(idaqtdmeses*idavlrmes)FROM projovemurbano.itemdemaisacoes WHERE deaid='".$_SESSION['projovemurbano']['deaid']."' AND idastatus='A'"),2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Demais ações</td>
		<td>
		<?
		$sql = "SELECT acodesc,
					   COALESCE((SELECT CASE WHEN idavlrmes IS NULL THEN '' ELSE trim(to_char(idavlrmes,'999g999g999d99')) END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'') as inp1, 
					   COALESCE((SELECT CASE WHEN idaqtdmeses IS NULL THEN '' ELSE trim(idaqtdmeses::text) END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'18') as inp2,
					   COALESCE(acomaxporcent::text,'') as inp3,
					   COALESCE((SELECT CASE WHEN idaqtdmeses IS NOT NULL AND idavlrmes IS NOT NULL THEN trim(to_char(idaqtdmeses*idavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'') as inp4
				FROM projovemurbano.acoes aca 
				WHERE acostatus='A'";
		$cabecalho=array("Demais Ações","Valor/mês(R$)","Meses(18)","Percentual Máximo previstos(%)","Valor total(R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
<?
$esdid = $db->pegaUm("SELECT d.esdid FROM projovemurbano.projovemurbano p
					  INNER JOIN workflow.documento d ON d.docid=p.docid 
					  WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");

if($esdid==ESD_VALIDADOMEC) :
?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		ASSINATURA DO SECRETÁRIO
		<br><br><br>
		<p>________________, _____ de ________________ de _______</p>
		<br><br><br>
		<? if($_SESSION['projovemurbano']['estuf']) : ?>
		<p>_______________________________________________________________<br>
		Secretário(a) Estadual de Educação</p>
		<? endif; ?>
		
		
		<? if($_SESSION['projovemurbano']['muncod']) : ?>
		<p>_______________________________________________________________<br>
		Secretário(a) Municipal de Educação</p>
		<? endif; ?>
		
		</td>
	</tr>
<? endif; ?>
</table>