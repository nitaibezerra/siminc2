<?
criaSessaoProfissionais();
$profissionais = $db->pegaLinha("SELECT * FROM projovemurbano.profissionais WHERE proid='".$_SESSION['projovemurbano']['proid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}");

$coordgeral = pegarCoordenadorGeral($_SESSION['projovemurbano']['proid']);
$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'A');
$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'P');
$assistenteadministrativo_M = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'M', $tprid = $_SESSION['projovemurbano']['tprid']);

$assistenteadministrativo_AP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'AP', $tprid = $_SESSION['projovemurbano']['tprid']);
$assistenteadministrativo_PP = pegarAssistentesCoordenadorGeral($_SESSION['projovemurbano']['proid'], 'PP', $tprid = $_SESSION['projovemurbano']['tprid']);

$diretorpolo 	  = pegarDiretorPolo($_SESSION['projovemurbano']['proid']);
$dirassistentes_A = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'A');
$dirassistentes_P = pegarAssistentesDiretorPolo($_SESSION['projovemurbano']['proid'], 'P');

$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');

if($_SESSION['projovemurbano']['muncod']) {
$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."'");
if($sugestaoampliacao['suaverdade']=="t") {
if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
}
}

if($_SESSION['projovemurbano']['estuf']) {
$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
if( $_SESSION['projovemurbano']['ppuid'] == '2' ){
$sql = "SELECT
					mtpvalor as valor,
					mtp.tpmid as tipo
				FROM
					projovemurbano.metasdoprograma mtp
				INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
				WHERE
					pjuid = {$_SESSION['projovemurbano']['pjuid']}
					AND tprid = {$_SESSION['projovemurbano']['tprid']}
				ORDER BY
					tipo DESC ";
$meta = $db->pegaUm($sql);
}else{
$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c
							INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge 
							WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'");		
if($sugestaoampliacao['suaverdade']=="t") {
if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
}
}
}
$montante = calcularMontante($meta);


if($coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != ''){
if($coordgeral['cgevlrtotal'] == 0){
$coordgera1 = $coordgeral['cgeqtd'];
}
if($coordgeral['cgevlrtotal'] != 0){
$coordgera2 = $coordgeral['cgeqtd'];
$coordgera3 = $coordgeral['cgevlrtotal'];
}
if($assistenteadministrativo_A['coavlrtotal'] == 0){
$coordassA = $assistenteadministrativo_A['coaqtd'];
}
if($assistenteadministrativo_A['coavlrtotal'] != 0){
$coordassA1 =  $assistenteadministrativo_A['coaqtd'];
$coordassA2 =  $assistenteadministrativo_A['coavlrtotal'];
}
if($assistenteadministrativo_P['coavlrtotal'] == 0){
$coordassP = $assistenteadministrativo_P['coaqtd'];
}
if($assistenteadministrativo_P['coavlrtotal'] != 0){
$coordassP1 = $assistenteadministrativo_P['coaqtd'];
$coordassP2 = $assistenteadministrativo_P['coavlrtotal'];
}
}
if($_SESSION['projovemurbano']['tprid'] == 2 ){
if($assistenteadministrativo_AP['coavlrtotal']==0){
$prisionalAP = $assistenteadministrativo_AP['coaqtd'];
}
if($prisionalAP2 = $assistenteadministrativo_AP['coavlrtotal'] !=0){
$prisionalAP1 = $assistenteadministrativo_AP['coaqtd'];
$prisionalAP2 = $assistenteadministrativo_AP['coavlrtotal'];
}
if($assistenteadministrativo_PP['coavlrtotal']==0){
$prisionalPP = $assistenteadministrativo_PP['coaqtd'];
}
if($assistenteadministrativo_PP['coavlrtotal'] != 0){
$prisionalPP1 = $assistenteadministrativo_PP['coaqtd'];
$prisionalPP2 = $assistenteadministrativo_PP['coavlrtotal'];
}
}

if($assistenteadministrativo_M['coavlrtotal'] == ''){
$apoioMat = $assistenteadministrativo_M['coaqtd'];
}
if($assistenteadministrativo_M['coavlrtotal'] != ''){
$apoioMat1 = $assistenteadministrativo_M['coaqtd'];
$apoioMat2 = $assistenteadministrativo_M['coavlrtotal'];
}

if( $coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != ''){
	if($diretorpolo['ccmvlrtotal'] == 0 ||$diretorpolo['dipeqtdefetivo40hr'] != 0){
		$diretorpolo0 = $diretorpolo['dipeqtdefetivo40hr'];
	}
	if($diretorpolo['ccmvlrtotal'] == 0||$diretorpolo['dipqtdrecursoproprio']){
		$diretorpolo1 =$diretorpolo['dipqtdrecursoproprio'];
	}
	if($diretorpolo['ccmvlrtotal'] != 0){
		$diretorpolo2 = $diretorpolo['ccmqtd'];
		$diretorpolo3 =	$diretorpolo['ccmvlrtotal'];
	}
	if($dirassistentes_A['crevlrtotal'] == 0||($dirassistentes_A['dasqtdefetivo40hr']||$dirassistentes_A['dasqtdrecursoproprio'])){
		$dirassistentesA40 = $dirassistentes_A['dasqtdefetivo40hr'];
		$dirassistentesArp = $dirassistentes_A['dasqtdrecursoproprio'];
	}
	if($dirassistentes_A['crevlrtotal'] != 0){
		$dirassistentesA1 = $dirassistentes_A['creqtd'];
		$dirassistentesA2 =	$dirassistentes_A['crevlrtotal'];
	}
	if($dirassistentes_P['crevlrtotal'] == 0||($dirassistentes_P['dasqtdefetivo40hr']||$dirassistentes_P['dasqtdrecursoproprio'])){
		$dirassistenteP40 = $dirassistentes_P['dasqtdefetivo40hr'];
		$dirassistentesPrp = $dirassistentes_P['dasqtdrecursoproprio'];
	}
	if($dirassistentes_P['crevlrtotal'] != 0){
		$dirassistentesP1 = $dirassistentes_P['creqtd'];
		$dirassistentesP2 =	$dirassistentes_P['crevlrtotal'];
	}
}

if($educadores_F['ccmvlrtotal'] == 0||$educadores_F['eduefetivo30hr'] != 0){
$educadoresF = $educadores_F['eduefetivo30hr'];
}
if($educadores_F['crevlrtotal'] == 0||$educadores_F['eduqtdrecursoproprio'] != 0){
$educadoresF1 = $educadores_F['eduqtdrecursoproprio'];
}
if($educadores_F['ccmvlrtotal'] != 0){
$educadoresF2 = $educadores_F['ccmvlrtotal'];
}
if($educadores_F['crevlrtotal'] != 0){
$educadoresF3 = $educadores_F['crevlrtotal'];
}
if($educadores_F['ccmvlrtotal'] != 0){
$educadoresFQtd1 = $educadores_F['ccmqtd'];
}
if($educadores_F['crevlrtotal'] != 0){
$educadoresFQtd2 = $educadores_F['creqtd'];
}

if($educadores_Q['ccmvlrtotal'] == 0||$educadores_Q['eduefetivo30hr'] != 0 ){
$educadoresQ = $educadores_Q['eduefetivo30hr'];
}
if($educadores_Q['crevlrtotal'] == 0||$educadores_Q['eduqtdrecursoproprio'] != 0 ){
$educadoresQ1 = $educadores_Q['eduqtdrecursoproprio'];
}
if($educadores_Q['ccmvlrtotal'] != 0){
$educadoresQ2 = $educadores_Q['ccmvlrtotal'];
}
if($educadores_Q['crevlrtotal'] != 0){
$educadoresQ3 = $educadores_Q['crevlrtotal'];
}
if($educadores_Q['ccmvlrtotal'] != 0){
$educadoresQqtd1 = $educadores_Q['ccmqtd'];
}
if($educadores_Q['crevlrtotal'] != 0){
$educadoresQqtd2 = $educadores_Q['creqtd'];
}

if($educadores_P['ccmvlrtotal'] == 0||$educadores_P['eduefetivo30hr']!= 0 ){
$educadoresP = $educadores_P['eduefetivo30hr'];
}
if($educadores_P['crevlrtotal'] == 0||$educadores_P['eduqtdrecursoproprio'] != 0 ){
$educadoresP1 = $educadores_P['eduqtdrecursoproprio'];
}
if($educadores_P['ccmvlrtotal'] != 0){
$educadoresP2 = $educadores_P['ccmvlrtotal'];
}
if($educadores_P['crevlrtotal'] != 0){
$educadoresP3 = $educadores_P['crevlrtotal'];
}
if($educadores_P['ccmvlrtotal'] != 0){
$educadoresPQtd1 = $educadores_P['ccmqtd'];
}
if($educadores_P['crevlrtotal'] != 0){
$educadoresPQtd2 = $educadores_P['creqtd'];
}

if($educadores_M['ccmvlrtotal'] == 0||$educadores_M['eduefetivo30hr']!= 0 ){
$educadoresM = $educadores_M['eduefetivo30hr'];
}
if($educadores_M['crevlrtotal'] == 0||$educadores_M['eduqtdrecursoproprio'] != 0 ){
$educadoresM1 = $educadores_M['eduqtdrecursoproprio'];
}
if($educadores_M['crevlrtotal'] != 0){
$educadoresM2 = $educadores_M['crevlrtotal'];
}
if($educadores_M['crevlrtotal'] != 0){
$educadoresMQtd = $educadores_M['creqtd'];
}


if($educadores_T['ccmvlrtotal'] == 0||$educadores_T['eduefetivo30hr'] != 0 ){
$educadoresT = $educadores_T['eduefetivo30hr'];
}
if($educadores_T['crevlrtotal'] == 0||$educadores_T['eduqtdrecursoproprio'] != 0 ){
$educadoresT1 = $educadores_T['eduqtdrecursoproprio'];
}
if($educadores_T['crevlrtotal'] != 0){
$educadoresT2 = $educadores_T['crevlrtotal'];
}
if($educadores_T['crevlrtotal'] != 0){
$educadoresTQtd = $educadores_T['creqtd'];
}


if($educadores_E['eduefetivo30hr'] != 0){
$educadoresE = $educadores_E['eduefetivo30hr'];
}
if($educadores_E['eduqtdrecursoproprio'] != 0){
$educadoresE1 = $educadores_E['eduqtdrecursoproprio'];
}

$coordassistentes = array("coaqtd"			  =>$coordassA1+$coordassP1,
						  "coavlrtotal" 	  =>$coordassA2+$coordassP2 ,
						  "outros"            =>$coordassA+$coordassP);

$assistentesprisionais = array("coaqtd"			  =>$prisionalAP1+$prisionalPP1,
							   "coavlrtotal" 	  =>$prisionalAP2+$prisionalPP2 ,
							   "outros"           =>$prisionalAP+$prisionalPP);

$dirassistente   = array("dasqtdefetivo40hr" 	 =>$dirassistentesA40+$dirassistenteP40,
						  "dasqtdrecursoproprio" =>$dirassistentesArp+$dirassistentesPrp,
						  "creqtd" 				 =>$dirassistentesA1+$dirassistentesP1 ,
						  "crevlrtotal" 		 =>$dirassistentesA2+$dirassistentesP2);

$totalComplementacao = array("ccmqtd"			  =>$coordgera2+$diretorpolo2+$educadoresFQtd1+$educadoresQqtd1+$educadoresPQtd1,
							 "valor" 	          =>$coordgera3+$diretorpolo3+$educadoresF2+$educadoresQ2+$educadoresP2,
							 "percent"            =>(($coordgera3+$diretorpolo3+$educadoresF2+$educadoresQ2+$educadoresP2)/$montante)*100);

$totalRecursosProgra = array("creqtd"			  =>$coordassistentes['coaqtd']+$assistentesprisionais['coaqtd']+$apoioMat1+$dirassistente['creqtd']+$educadoresFQtd2+$educadoresQqtd2+$educadoresPQtd2+$educadoresMQtd+$educadoresTQtd,
							 "valor" 	          =>$coordassistentes['coavlrtotal']+$assistentesprisionais['coavlrtotal']+$apoioMat2+$dirassistente['crevlrtotal']+$educadoresF3+$educadoresQ3+$educadoresP3+$educadoresT2+$educadoresM2,
							 "percent"            =>(($coordassistentes['coavlrtotal']+$assistentesprisionais['coavlrtotal']+$apoioMat2+$dirassistente['crevlrtotal']+$educadoresF3+$educadoresQ3+$educadoresP3+$educadoresM2+$educadoresT2)/$montante)*100);

$totalGeral = array("qtd"		 =>$totalComplementacao['ccmqtd']+$totalRecursosProgra['creqtd'],
					"valor"      =>$totalComplementacao['valor']+$totalRecursosProgra['valor'],
					"percent"    =>$totalComplementacao['percent']+$totalRecursosProgra['percent']);

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
				<td align="right"><?=number_format($coordgera1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da coordenação geral</td>
				<td align="right"><?=number_format($coordassistentes['outros'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes para unidade prisional</td>
				<td align="right"><?=number_format($assistentesprisionais['outros'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Pessoal apoio etapa matrícula</td>
				<td align="right"><?=number_format($apoioMat,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Diretor de polo</td>
				<td align="right"><?=number_format($diretorpolo0+$diretorpolo1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Assistentes da direção de polo</td>
				<td align="right"><?=number_format($dirassistente['dasqtdefetivo40hr']+$dirassistente['dasqtdrecursoproprio'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadoresF+$educadoresF1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadoresQ+$educadoresQ1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadoresP+$educadoresP1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educadores para monitoramento de
				acolhimento às crianças filhas dos jovens atendidos no programa</td>
				<td align="right"><?=number_format($educadoresM+$educadoresM1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tradutor e Intérprete de Libras</td>
				<td align="right"><?=number_format($educadoresT+$educadoresT1,0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Educador para atendimento educacional
				especializado</td>
				<td align="right"><?=number_format($educadoresE+$educadoresE1,0,",",".") ?></td>
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
			<?if($coordgera3!= 0) {?>
			<tr>
				<td class="SubTituloDireita">Coordenador geral</td>
				<td align="right"><?=number_format($coordgera2,0,",",".") ?></td>
				<td align="right"><?=number_format($coordgera3,2,",",".") ?></td>
				<td align="right"><?=number_format(($coordgera3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($diretorpolo3!= 0) {?>
			<tr>
				<td class="SubTituloDireita">Diretor de polo</td>
				<td align="right"><?=number_format($diretorpolo2,0,",",".") ?></td>
				<td align="right"><?=number_format($diretorpolo3,2,",",".") ?></td>
				<td align="right"><?=number_format(($diretorpolo3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadoresF2!= 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadoresFQtd1,0,",",".") ?></td>
				<td align="right"><?=number_format($educadoresF2,2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresF2/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadoresQ2!= 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadoresQqtd1,0,",",".") ?></td>
				<td align="right"><?=number_format($educadoresQ2,2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresQ2/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadoresP2!= 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadoresPQtd1,0,",",".") ?></td>
				<td align="right"><?=number_format($educadoresP2,2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresP2/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format($totalComplementacao['ccmqtd'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalComplementacao['valor'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalComplementacao['percent'],1,",",".") ?></b></td>
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
			<?if($coordassistentes['coavlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Assistentes da coordenação geral</td>
				<td align="right"><?=number_format($coordassistentes['coaqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($coordassistentes['coavlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($coordassistentes['coavlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($_SESSION['projovemurbano']['tprid'] == 2 && ($assistentesprisionais['coavlrtotal'] != 0)){ ?>
			<tr>
				<td class="SubTituloDireita">Assistentes para unidade prisional</td>
				<td align="right"><?=number_format($assistentesprisionais['coaqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($assistentesprisionais['coavlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($assistentesprisionais['coavlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($assistenteadministrativo_M['coavlrtotal'] != 0 ||$assistenteadministrativo_M['coavlrtotal']!= '' ){ ?>
			</tr>
			<tr>
				<td class="SubTituloDireita">Pessoal apoio etapa matrícula</td>
				<td align="right"><?=number_format($apoioMat1,0,",",".") ?></td>
				<td align="right"><?=number_format($apoioMat2,2,",",".") ?></td>
				<td align="right"><?=number_format(($apoioMat2/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($dirassistente['crevlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Assistentes da direção de polo</td>
				<td align="right"><?=number_format($dirassistente['creqtd'],0,",",".") ?></td>
				<td align="right"><?=number_format($dirassistente['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($dirassistente['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_F['crevlrtotal'] != 0 ||$educadores_F['crevlrtotal']!= '' ){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Ensino Fundamental</td>
				<td align="right"><?=number_format($educadoresFQtd2,0,",",".") ?></td>
				<td align="right"><?=number_format($educadoresF3,2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresF3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_Q['crevlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Qualificação Profissional</td>
				<td align="right"><?=number_format($educadoresQqtd2,0,",",".") ?></td>
				<td align="right"><?=number_format($educadoresQ3,2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresQ3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_P['crevlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores de Participação Cidadã</td>
				<td align="right"><?=number_format($educadoresPQtd2,0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_P['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresP3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_M['crevlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Educadores para monitoramento de
				acolhimento às crianças filhas dos jovens atendidos no programa</td>
				<td align="right"><?=number_format($educadoresMQtd,0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_M['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadoresM3/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<?if($educadores_T['crevlrtotal'] != 0){ ?>
			<tr>
				<td class="SubTituloDireita">Tradutor de Intérprete de Libras</td>
				<td align="right"><?=number_format($educadoresTQtd,0,",",".") ?></td>
				<td align="right"><?=number_format($educadores_T['crevlrtotal'],2,",",".") ?></td>
				<td align="right"><?=number_format(($educadores_T['crevlrtotal']/$montante)*100,1,",",".") ?></td>
			</tr>
			<?} ?>
			<tr>
				<td class="SubTituloDireita"><b>Total</b></td>
				<td align="right"><b><?=number_format($totalRecursosProgra['creqtd'],0,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalRecursosProgra['valor'],2,",",".") ?></b></td>
				<td align="right"><b><?=number_format($totalRecursosProgra['percent'],1,",",".")?></b></td>
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
				<td align="right"><?=number_format($totalGeral['qtd'],0,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Valor total das
				remunerações(pagas com recursos federais)</td>
				<td align="right"><?=number_format($totalGeral['valor'],2,",",".") ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">% Total Utilizado</td>
				<td align="right"><?=number_format($totalGeral['percent'],1,",",".") ?></td>
			</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button"
			name="anterior" value="Anterior"
			onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=profissionais&aba3=profissionaisCadastro';">
		<input type="button" name="proximo" value="Próximo"
			onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=profissionais&aba3=resumoGeralEducadores';">
		</td>
	</tr>
</table>
</form>
			<? registarUltimoAcesso(); ?>
