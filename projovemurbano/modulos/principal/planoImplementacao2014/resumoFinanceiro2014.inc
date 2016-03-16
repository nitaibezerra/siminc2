<?
criaSessaoProfissionais();
$profissionais = $db->pegaLinha("SELECT * FROM projovemurbano.profissionais WHERE proid='".$_SESSION['projovemurbano']['proid']."'");
criaSessaoGeneroAlimenticios();
$generoalimenticio = $db->pegaLinha("SELECT * FROM projovemurbano.generoalimenticio WHERE galid='".$_SESSION['projovemurbano']['galid']."'");
$lancherefeicao = $db->pegaLinha("SELECT * FROM projovemurbano.lancherefeicao WHERE galid='".$_SESSION['projovemurbano']['galid']."'");
criaSessaoQualificacaoProfissional();
$qualificacaoprofissional = $db->pegaLinha("SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='".$_SESSION['projovemurbano']['qprid']."'");
criaSessaoFormacaoEducadores();
$sql = "SELECT * FROM projovemurbano.formacaoeducadores
          WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'
            AND tprid = {$_SESSION['projovemurbano']['tprid']}";
$formacaoeducadores = $db->pegaLinha($sql);
$coordenadorgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);

//if($_SESSION['projovemurbano']['muncod']) {
//	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
//	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."'");
//	if($sugestaoampliacao['suaverdade']=="t") {
//		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
//	}
//}
//if($_SESSION['projovemurbano']['estuf']) {
//	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
//	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'");
//	if($sugestaoampliacao['suaverdade']=="t") {
//		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
//	}
//}
$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);

$montante = calcularMontante($meta);

$coordgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);
$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$assistenteadministrativo_M = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'M', $tprid = $_SESSION['projovemurbano']['tprid']);

$assistenteadministrativo_AP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'AP', $tprid = $_SESSION['projovemurbano']['tprid']);
$assistenteadministrativo_PP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'PP', $tprid = $_SESSION['projovemurbano']['tprid']);

if($_SESSION['projovemurbano']['tprid'] == 2 ){
$casoprisionais1 = $assistenteadministrativo_AP['coaqtd']+$assistenteadministrativo_PP['coaqtd'];
$casoprisionais2 = $assistenteadministrativo_AP['coavlrtotal']+$assistenteadministrativo_PP['coavlrtotal'];
}

if($coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordenadorgeral['tprid'] != ''){
$assitente_A1 =  $assistenteadministrativo_A['coaqtd']+$assistenteadministrativo_P['coaqtd'];
$assitente_A2 = $assistenteadministrativo_A['coavlrtotal']+$assistenteadministrativo_P['coavlrtotal'];
}

$coordassistentes = array("coaqtd"		=> $assitente_A1+$assistenteadministrativo_M['coaqtd']+$casoprisionais1,
						  "coavlrtotal" => $assitente_A2+$assistenteadministrativo_M['coavlrtotal']+$casoprisionais2); 

$diretorpolo 	  = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');

if( $coordenadorgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordenadorgeral['tprid'] != ''){
$dirassistentes   = array("dasqtdefetivo40hr" 	 => $dirassistentes_A['dasqtdefetivo40hr']+$dirassistentes_P['dasqtdefetivo40hr'],
						  "dasqtdrecursoproprio" => $dirassistentes_A['dasqtdrecursoproprio']+$dirassistentes_P['dasqtdrecursoproprio'],
						  "creqtd" 				 => $dirassistentes_A['creqtd']+$dirassistentes_P['creqtd'],
						  "crevlrtotal" 		 => $dirassistentes_A['crevlrtotal']+$dirassistentes_P['crevlrtotal']);
}

$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');;
$arrTotal = array("vlrprofrecurso" => $coordassistentes['coavlrtotal']+$dirassistentes['crevlrtotal']+$educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+$educadores_M['crevlrtotal']+$educadores_T['crevlrtotal'],
			  	  "vlrprofcomplem" => $coordgeral['cgevlrtotal']+$diretorpolo['ccmvlrtotal']+$dirassistentes['ccmvlrtotal']+$educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+$diretorpolo['ccmvlrtotal'],
			      "qtdeducrecursoFQP" => $educadores_F['creqtd']+$educadores_Q['creqtd']+$educadores_P['creqtd'],
			      "vlreducrecursoFQP" => $educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']);


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
		Assistentes da Coordenação Local, Assistentes de Direção de Polo,
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
				<td align="center"><?=number_format($arrTotal['vlrprofrecurso'],2,",",".") ?></td>
				<td align="center"><?=round(($arrTotal['vlrprofrecurso']/$montante)*100,1) ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Remuneração para complementação de carga
		horária:<br>
		<br>
		Educadores, Coordenador Geral e Diretor de Polo</td>
		<td valign="top">
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<tr>
				<td align="center"><?=number_format($arrTotal['vlrprofcomplem'],2,",",".") ?></td>
				<td align="center"><?=round(($arrTotal['vlrprofcomplem']/$montante)*100,1) ?></td>
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
				<td align="center"><?=number_format(($montante*($profissionais['propercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$profissionais['propercmax'] ?></td>
				<td align="center"><?=number_format(($arrTotal['vlrprofrecurso']+$arrTotal['vlrprofcomplem']),2,",",".") ?></td>
				<td align="center"><?=round(((($arrTotal['vlrprofrecurso']+$arrTotal['vlrprofcomplem'])/$montante)*100),1) ?></td>
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
			<? $auxiliofinanceiro = $db->pegaLinha("SELECT *, (aufqtdeducador*aufavlrauxilio) as total FROM projovemurbano.auxiliofinanceiro WHERE fedid='".$_SESSION['projovemurbano']['fedid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*($auxiliofinanceiro['aufpercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$auxiliofinanceiro['aufpercmax'] ?></td>
				<td align="center"><?=number_format(($auxiliofinanceiro['total']),2,",",".") ?></td>
				<td align="center"><?=round((($auxiliofinanceiro['total'])/$montante)*100,1) ?></td>
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
			<? $recursosgastos = $db->pegaLinha("SELECT SUM(rgavalor) as total FROM projovemurbano.recursosgastos rec WHERE rec.fedid='".$_SESSION['projovemurbano']['fedid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*($formacaoeducadores['fedpercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$formacaoeducadores['fedpercmax'] ?></td>
				<td align="center"><?=number_format(($recursosgastos['total']),2,",",".") ?></td>
				<td align="center"><?=round((($recursosgastos['total'])/$montante)*100,1) ?></td>
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
		$sql = "SELECT * FROM projovemurbano.lancherefeicao WHERE galid='".$_SESSION['projovemurbano']['galid']."'";
		$lancherefeicao = $db->pegaLinha($sql);
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
				<td align="center"><?=number_format(($montante*($generoalimenticio['galpercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$generoalimenticio['galpercmax'] ?></td>
				<td align="center"><?=number_format($lancherefeicao['lrevlrtotal'],2,",",".") ?></td>
				<td align="center"><?=$generoalimenticio['galpercutilizado'] ?></td>
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
		$sql = "SELECT SUM(pgavlrmes*pgaqtdmeses) as total, nucid FROM projovemurbano.previsaogasto WHERE  qprid='".$_SESSION['projovemurbano']['qprid']."' GROUP BY nucid"  ;
		
		$previsaogasto = $db->pegaUM($sql);
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
				<td align="center"><?=number_format(($montante*($qualificacaoprofissional['qprpercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$qualificacaoprofissional['qprpercmax'] ?></td>
				<td align="center"><?=number_format($previsaogasto,2,",",".") ?></td>
				<td align="center"><?=$qualificacaoprofissional['qprpercutilizado'] ?></td>

			</tr>
		</table>
		</td>
	</tr>
	<? if($_SESSION['projovemurbano']['estuf']) : ?>
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
			<? $transportematerial = $db->pegaLinha("SELECT * FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = '".$_SESSION['projovemurbano']['tprid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*($transportematerial['tmapercmax']/100)),2,",",".") ?></td>
				<td align="center"><?=$transportematerial['tmapercmax'] ?></td>
				<td align="center"><?=number_format($transportematerial['tmarecursoutilizado'],2,",",".") ?></td>
				<td align="center"><?=$transportematerial['tmaperutilizado'] ?></td>
			</tr>
		</table>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">Demais Ações</td>
	</tr>
	<?
	$demaisacoes = $db->pegaLinha("SELECT DISTINCT * FROM projovemurbano.demaisacoes WHERE --deaid='".$_SESSION['projovemurbano']['deaid']."' AND 
	pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}");
	
	$sql = "SELECT DISTINCT * FROM projovemurbano.acoes WHERE acostatus='A' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
	$acoes = $db->carregar($sql);
		
	if($acoes[0] && $demaisacoes) :
	foreach($acoes as $acao) :
	?>
	<tr>
		<td class="SubTituloDireita"><?=$acao['acodesc'] ?></td>
		<td>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloCentro">Valor da ação(R$)</td>
				<td class="SubTituloCentro">Valor Utilizado(R$)</td>
				<td class="SubTituloCentro">(%) Utilizado</td>
			</tr>
			<? $itemdemaisacoes = $db->pegaLinha("SELECT (idavlrmes*idaqtdmeses) as total FROM projovemurbano.itemdemaisacoes WHERE acoid='".$acao['acoid']."' AND deaid='".$demaisacoes['deaid']."'"); ?>
			<tr>
				<td align="center"><?=number_format(($montante*($acao['acomaxporcent']/100)),2,",",".") ?></td>
				<td align="center"><?=number_format($itemdemaisacoes['total'],2,",",".") ?></td>
				<td align="center"><?=round(($itemdemaisacoes['total']/$montante)*100,1) ?></td>
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
<? registarUltimoAcesso(); ?>
