<?
criaSessaoProfissionais();
$profissionais = $db->pegaLinha("SELECT * FROM projovemurbano.profissionais WHERE proid='".$_SESSION['projovemurbano']['proid']."'");

$coordgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);
$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$coordassistentes = array("coaqtd"		=> $assistenteadministrativo_A['coaqtd']+$assistenteadministrativo_P['coaqtd'],
						  "coavlrtotal" => $assistenteadministrativo_A['coavlrtotal']+$assistenteadministrativo_P['coavlrtotal']);
$diretorpolo 	  = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');
$dirassistentes   = array("dasqtdefetivo40hr" 	 => $dirassistentes_A['dasqtdefetivo40hr']+$dirassistentes_P['dasqtdefetivo40hr'],
						  "dasqtdrecursoproprio" => $dirassistentes_A['dasqtdrecursoproprio']+$dirassistentes_P['dasqtdrecursoproprio'],
						  "creqtd" 				 => $dirassistentes_A['creqtd']+$dirassistentes_P['creqtd'],
						  "crevlrtotal" 		 => $dirassistentes_A['crevlrtotal']+$dirassistentes_P['crevlrtotal']);
		
$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');

if($_SESSION['projovemurbano']['muncod']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."' AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}

if($_SESSION['projovemurbano']['estuf']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'AND c.ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}

$montante = calcularMontante($meta);
$montante = $montante ? $montante :1;

?>
<form id="form" name="form" method="POST">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%">Orientações:</td>
		<td>
		<font color=blue>
		<p>Quantidade de profissionais e valores previstos para a remuneração, conforme as informações prestadas na aba de profissionais.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais do quadro efetivo designados para atuar no programa e/ou contratados com recursos do ente executor</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="50%">Número</td>
			</tr>

			<tr>
				<td class="SubTituloDireita">Coordenador Geral</td>
				<td align="right"><?=(($coordgeral['oppid']==4 || $coordgeral['oppid']==6)?number_format($coordgeral['cgeqtd'],0,",","."):'0') ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da coordenação geral</td>
				<td align="right"><?=number_format((($assistenteadministrativo_A['oppid']!=7)?$assistenteadministrativo_A['coaqtd']:'0')+(($assistenteadministrativo_P['oppid']!=10)?$assistenteadministrativo_P['coaqtd']:'0'),0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Diretor de polo</td>
				<td align="right"><?=number_format($diretorpolo['dipeqtdefetivo40hr']+$diretorpolo['dipqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da direção de polo</td>
				<td align="right"><?=number_format($dirassistentes['dasqtdefetivo40hr']+$dirassistentes['dasqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadores_F['eduefetivo30hr']+$educadores_F['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadores_Q['eduefetivo30hr']+$educadores_Q['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadores_P['eduefetivo30hr']+$educadores_P['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores para monitoramento de acolhimento às crianças filhas dos jovens atendidos no programa</td>
				<td align="right"><?=number_format($educadores_M['eduefetivo30hr']+$educadores_M['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tradutor e Intérprete de Libras</td>
				<td align="right"><?=number_format($educadores_T['eduefetivo30hr']+$educadores_T['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educador para atendimento educacional especializado</td>
				<td align="right"><?=number_format($educadores_E['eduefetivo30hr']+$educadores_E['eduqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais do quadro efetivo com complementação de carga horária</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="17%">Número</td>
				<td class="SubTituloCentro" width="17%">Valor das remunerações(R$)</td>
				<td class="SubTituloCentro" width="17%">Percentual Utilizado(%)</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Coordenador geral</td>
				<td align="right"><?=(($coordgeral['oppid']==5)?number_format($coordgeral['cgeqtd'],0,",","."):'0') ?></td>
				<td align="right"><?=number_format($coordgeral['cgevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordgeral['cgevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Diretor de polo</td>
				<td align="right"><?=number_format($diretorpolo['ccmqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($diretorpolo['ccmvlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($diretorpolo['ccmvlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadores_F['ccmqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_F['ccmvlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_F['ccmvlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadores_Q['ccmqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_Q['ccmvlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_Q['ccmvlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadores_P['ccmqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_P['ccmvlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_P['ccmvlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format((($coordgeral['oppid']==5)?$coordgeral['cgeqtd']:'0')+$diretorpolo['ccmqtd']+$educadores_F['ccmqtd']+$educadores_Q['ccmqtd']+$educadores_P['ccmqtd'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format($coordgeral['cgevlrtotal']+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format((($coordgeral['cgevlrtotal']+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal'])/$montante)*100,1,",",".") ?></b></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Profissionais contratados com recursos do programa</td>
		<td>
		<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloCentro" width="50%">Profissionais</td>
				<td class="SubTituloCentro" width="17%">Número</td>
				<td class="SubTituloCentro" width="17%">Valor das remunerações(R$)</td>
				<td class="SubTituloCentro" width="17%">Percentual Utilizado(%)</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da coordenação geral</td>
				<td align="right"><?=number_format((($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coaqtd']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coaqtd']:'0'),0,",",".") ?></td>
				<td align="right"><?=number_format((($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coavlrtotal']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coavlrtotal']:'0'),2,",",".") ?></td>
				<td align="right"><?=number_format((((($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coavlrtotal']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coavlrtotal']:'0'))/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da direção de polo</td>
				<td align="right"><?=number_format($dirassistentes['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($dirassistentes['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($dirassistentes['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadores_F['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_F['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_F['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadores_Q['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_Q['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_Q['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadores_P['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_P['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_P['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores para monitoramento de acolhimento às crianças filhas dos jovens atendidos no programa</td>
				<td align="right"><?=number_format($educadores_M['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_M['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_M['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tradutor e Intérprete de Libras</td>
				<td align="right"><?=number_format($educadores_T['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_T['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_T['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format((($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coaqtd']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coaqtd']:'0')+$dirassistentes['creqtd']+$educadores_F['creqtd']+$educadores_Q['creqtd']+$educadores_P['creqtd']+$educadores_M['creqtd']+$educadores_T['creqtd'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format((($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coavlrtotal']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coavlrtotal']:'0')+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format((($coordassistentes['coavlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'])/$montante)*100,1,",",".") ?></b></td>
			</tr>
		</table>
	
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Total Geral</td>
		<td>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="50%">Número de profissionais que atuarão no Programa e serão remunerados com recursos federais</td>
				<td align="right"><?=number_format((($coordgeral['oppid']==5)?$coordgeral['cgeqtd']:'0')+(($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coaqtd']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coaqtd']:'0')+$diretorpolo['ccmqtd']+$educadores_F['ccmqtd']+$educadores_Q['ccmqtd']+$educadores_P['ccmqtd']+$dirassistentes['creqtd']+$educadores_F['creqtd']+$educadores_Q['creqtd']+$educadores_P['creqtd']+$educadores_M['creqtd']+$educadores_T['creqtd'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Valor total das remunerações(pagas com recursos federais)</td>
				<td align="right"><?=number_format($coordgeral['cgevlrtotal']+(($assistenteadministrativo_A['oppid']==7)?$assistenteadministrativo_A['coavlrtotal']:'0')+(($assistenteadministrativo_P['oppid']==10)?$assistenteadministrativo_P['coavlrtotal']:'0')+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'],2,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">% Total Utilizado</td>
				<td align="right"><?=number_format((($coordgeral['cgevlrtotal']+$coordassistentes['coavlrtotal']+$diretorpolo['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal']+$educadores_E[''])/$montante)*100,1,",",".") ?></td>
			</tr>
		</table>
	
		</td>
	</tr>


	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro';"> <input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
