<?
/* Carregando profissionais */ 
criaSessaoProfissionais();
$profissionais = $db->pegaLinha("SELECT *
                                   FROM projovemurbano.profissionais
                                   WHERE proid='".$_SESSION['projovemurbano']['proid']."'");

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
if(!$montante){
	$montante = '1';
}
if($coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != ''){
	if($coordgeral['cgevlrtotal'] == 0){
		$coordgera1 = $coordgeral['cgeqtd'];
	}
	if($coordgeral['cgevlrtotal'] != 0){
		$coordgera2 = $coordgeral['cgeqtd'];
		$coordgera3 = $coordgeral['cgevlrtotal'];
	}
	if($coordgeral['crevlrtotal'] != 0){
		$coordgera4 = $coordgeral['cgeqtd'];
		$coordgera5 = $coordgeral['crevlrtotal'];
	}
	if($assistenteadministrativo_A['coavlrtotal'] == 0){
		$coordassA = $assistenteadministrativo_A['coaqtd'];
	}
	if($assistenteadministrativo_A['coavlrtotal'] != 0){
		$coordassA1 =  $assistenteadministrativo_A['coaqtd'];
		$coordassA2 =  $assistenteadministrativo_A['coavlrtotal'];
	}
	if($assistenteadministrativo_A['ccmvlrtotal'] != 0){
		$coordassA3 =  $assistenteadministrativo_A['ccmqtd'];
		$coordassA4 =  $assistenteadministrativo_A['ccmvlrtotal'];
	}
	if($assistenteadministrativo_P['coavlrtotal'] == 0){
		$coordassP = $assistenteadministrativo_P['coaqtd'];
	}
	if($assistenteadministrativo_P['coavlrtotal'] != 0){
		$coordassP1 = $assistenteadministrativo_P['coaqtd'];
		$coordassP2 = $assistenteadministrativo_P['coavlrtotal'];
		$coordassP3 = $assistenteadministrativo_P['ccmvlrtotal'];
	}
	if($assistenteadministrativo_P['ccmvlrtotal'] != 0){
		$coordassP3 = $assistenteadministrativo_P['ccmvqtd'];
		$coordassP4 = $assistenteadministrativo_P['ccmvlrtotal'];
	}
}
if($_SESSION['projovemurbano']['tprid'] == 2 ){
	if($assistenteadministrativo_AP['coavlrtotal']==0){
		$prisionalAP = $assistenteadministrativo_AP['coaqtd'];
	}
	if($assistenteadministrativo_AP['coavlrtotal'] !=0){
		$prisionalAP1 = $assistenteadministrativo_AP['coaqtd'];
		$prisionalAP2 = $assistenteadministrativo_AP['coavlrtotal'];
	}
	if($assistenteadministrativo_AP['ccmvlrtotal'] !=0){
		$prisionalAP3 = $assistenteadministrativo_AP['ccmqtd'];
		$prisionalAP4 = $assistenteadministrativo_AP['ccmvlrtotal'];
	}
	if($assistenteadministrativo_PP['coavlrtotal']==0){
		$prisionalPP = $assistenteadministrativo_PP['coaqtd'];
	}
	if($assistenteadministrativo_PP['coavlrtotal'] != 0){
		$prisionalPP1 = $assistenteadministrativo_PP['coaqtd'];
		$prisionalPP2 = $assistenteadministrativo_PP['coavlrtotal'];
	}
	if($assistenteadministrativo_PP['ccmvlrtotal'] != 0){
		$prisionalPP3 = $assistenteadministrativo_PP['ccmqtd'];
		$prisionalPP4 = $assistenteadministrativo_PP['ccmvlrtotal'];
	}
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
	if($dirassistentes_A['crevlrtotal'] == 0||($dirassistentes_A['dasqtdefetivo40hr']||$dirassistentes_A['dasqtdrecursoproprio'])||$dirassistentes_A['ccmvlrtotal'] != 0){
		$dirassistentesA40 = $dirassistentes_A['dasqtdefetivo40hr'];
		$dirassistentesArp = $dirassistentes_A['dasqtdrecursoproprio'];
	}
	if($dirassistentes_A['crevlrtotal'] != 0){
		$dirassistentesA1 = $dirassistentes_A['creqtd'];
		$dirassistentesA2 =	$dirassistentes_A['crevlrtotal'];
	}
	if($dirassistentes_A['ccmvlrtotal'] != 0){
		$dirassistentesA3 = $dirassistentes_A['ccmqtd'];
		$dirassistentesA4 =	$dirassistentes_A['ccmvlrtotal'];
	}
	if($dirassistentes_P['crevlrtotal'] == 0||($dirassistentes_P['dasqtdefetivo40hr']||$dirassistentes_P['dasqtdrecursoproprio'])||$dirassistentes_P['ccmvlrtotal'] != 0){
		$dirassistenteP40 = $dirassistentes_P['dasqtdefetivo40hr'];
		$dirassistentesPrp = $dirassistentes_P['dasqtdrecursoproprio'];
	}
	if($dirassistentes_P['crevlrtotal'] != 0){
		$dirassistentesP1 = $dirassistentes_P['creqtd'];
		$dirassistentesP2 =	$dirassistentes_P['crevlrtotal'];
	}
	if($dirassistentes_P['ccmvlrtotal'] != 0){
		$dirassistentesP3 = $dirassistentes_P['ccmqtd'];
		$dirassistentesP4 =	$dirassistentes_P['ccmvlrtotal'];
	}
	if($assistenteadministrativo_M['coavlrtotal'] == ''){
		$apoioMat = $assistenteadministrativo_M['coaqtd'];
	}
	if($assistenteadministrativo_M['coavlrtotal'] != ''){
		$apoioMat1 = $assistenteadministrativo_M['coaqtd'];
		$apoioMat2 = $assistenteadministrativo_M['coavlrtotal'];
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
						  "coavlrtotal" 	  =>$coordassA2+$coordassP2,
						  "ccmqtd"		  	  =>$coordassA3+$coordassP3,
						  "ccmvlrtotal"		  =>$coordassA4+$coordassP4,
						  "outros"            =>$coordassA+$coordassP);
$assistentesprisionais = array("coaqtd"			  =>$prisionalAP1+$prisionalPP1,
							   "coavlrtotal" 	  =>$prisionalAP2+$prisionalPP2 ,
							   "ccmqtd" 	  	  =>$prisionalAP3+$prisionalPP3,
							   "ccmvlrtotal" 	  =>$prisionalAP4+$prisionalPP4,
							   "outros"           =>$prisionalAP+$prisionalPP);
$dirassistente   = array("dasqtdefetivo40hr" 	 =>$dirassistentesA40+$dirassistenteP40,
						  "dasqtdrecursoproprio" =>$dirassistentesArp+$dirassistentesPrp,
						  "creqtd" 				 =>$dirassistentesA1+$dirassistentesP1 ,
						  "crevlrtotal" 		 =>$dirassistentesA2+$dirassistentesP2,
						  "ccmqtd" 				 =>$dirassistentesA3+$dirassistentesP3 ,
						  "ccmvlrtotal" 		 =>$dirassistentesA4+$dirassistentesP4);
$totalComplementacao = array("ccmqtd"			  =>$coordgera2+$coordassistentes['ccmqtd']+$assistentesprisionais['ccmqtd']+$dirassistente['ccmqtd']+$diretorpolo2+$educadoresFQtd1+$educadoresQqtd1+$educadoresPQtd1,
							 "valor" 	          =>$coordgera3+$coordassistentes['ccmvlrtotal']+$assistentesprisionais['ccmvlrtotal']+$dirassistente['ccmvlrtotal']+$diretorpolo3+$educadoresF2+$educadoresQ2+$educadoresP2,
							 "percent"            =>(($coordgera3+$coordassistentes['ccmvlrtotal']+$assistentesprisionais['ccmvlrtotal']+$dirassistente['ccmvlrtotal']+$diretorpolo3+$educadoresF2+$educadoresQ2+$educadoresP2)*100/$montante));

$totalRecursosProgra = array("creqtd"			  =>$coordgera4+$coordassistentes['coaqtd']+$assistentesprisionais['coaqtd']+$apoioMat1+$dirassistente['creqtd']+$educadoresFQtd2+$educadoresQqtd2+$educadoresPQtd2+$educadoresMQtd+$educadoresTQtd,
							 "valor" 	          =>$coordgera5+$coordassistentes['coavlrtotal']+$assistentesprisionais['coavlrtotal']+$apoioMat2+$dirassistente['crevlrtotal']+$educadoresF3+$educadoresQ3+$educadoresP3+$educadoresT2+$educadoresM2,
							 "percent"            =>(($coordgera5+$coordassistentes['coavlrtotal']+$assistentesprisionais['coavlrtotal']+$apoioMat2+$dirassistente['crevlrtotal']+$educadoresF3+$educadoresQ3+$educadoresP3+$educadoresT2+$educadoresM2)*100/$montante));
$totalGeral = array("qtd"		 =>$totalComplementacao['ccmqtd']+$totalRecursosProgra['creqtd'],
					"valor"      =>$totalComplementacao['valor']+$totalRecursosProgra['valor'],
					"percent"    =>$totalComplementacao['percent']+$totalRecursosProgra['percent']);


$contagemPolos = pegarNumeroPolos($agrupado = true);
$tenhoPolo = false;
$numeropolos = 0;

// -- Consulta os polos agrupados para ver se o polo foi definido para este público
if ($contagemPolos) {
    foreach ($contagemPolos as $poloTotal) {
        if ($_SESSION['projovemurbano']['tprid'] == $poloTotal['tprid']) {
            $tenhoPolo = true;
        }
        $numeropolos =+ (int)$poloTotal['count'];
    }
}
$numeronucleos = pegarNumeroNucleos($tenhoPolo);
/* Fim - Carregando profissionais */
if($_SESSION['projovemurbano']['muncod']) {
	$linkproximo = "Municipio";	
	$refesfera="M";
	if($_SESSION['projovemurbano']['tprid'] == 1){
		$isso = "juventude";
	}elseif($_SESSION['projovemurbano']['tprid'] == 2){
		$isso = "juventude";
	}elseif($_SESSION['projovemurbano']['tprid'] == 3){
		$isso = "geral";
	}
	
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT 
							cam.$isso 
						FROM 
							projovemurbano.cargameta cam
		                LEFT JOIN 
		                	territorios.municipio mun ON (cam.cmecodibge = mun.muncod::INT)
		                LEFT JOIN 
		                	projovemurbano.projovemurbano ppu ON cam.cmecodibge = ppu.muncod::INT
		                where 
		                	cam.cmecodibge = '{$cmecodibge}' 
		                	AND ppu.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
		                	AND cam.ppuid = {$_SESSION['projovemurbano']['ppuid']}
		                	AND pjuid = {$_SESSION['projovemurbano']['pjuid']}");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}
 
if($_SESSION['projovemurbano']['estuf']) {
	$linkproximo = "Estado";	
	$refesfera = "E";
	
	if($_SESSION['projovemurbano']['tprid'] == 1){
		$isso = "juventude";
	}elseif($_SESSION['projovemurbano']['tprid'] == 2){
		$isso = "juventude";
	}elseif($_SESSION['projovemurbano']['tprid'] == 3){
		$isso = "geral";
	} 
// 	ver($_SESSION['projovemurbano']['tprid'],d);
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT 
							cam.$isso 
						FROM 
							projovemurbano.cargameta cam
		                LEFT JOIN 
		                	territorios.estado est ON (cam.cmecodibge = est.estcod::INT)
		                LEFT JOIN 
		                		projovemurbano.projovemurbano ppu ON cam.cmecodibge = est.estcod::INT
		                where 
		                	cam.cmecodibge = '{$cmecodibge}' 
		                	AND ppu.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
		                	AND cam.ppuid = {$_SESSION['projovemurbano']['ppuid']}
		                	AND pjuid = {$_SESSION['projovemurbano']['pjuid']}
		                	");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}
				$sql = "SELECT 
					        arq.arqid, arq.arqnome, arq.arqextensao,
						 	inl.inlnuminstlegal, inl.inldatainstlegal,
							inl.tpdid
						FROM 
							projovemurbano.instrumentolegal inl
						INNER JOIN 
							projovemurbano.anexogeral ang USING (angid)
						INNER JOIN 
							public.arquivo arq USING (arqid)
						WHERE 
				        proid =  {$_SESSION['projovemurbano']['proid']}
						AND tprid =  {$_SESSION['projovemurbano']['tprid']}
						AND ang.ppuid =  {$_SESSION['projovemurbano']['ppuid']}";
				$infoInstLegal = $db->pegaLinha($sql); 

/* Carregando Formação Educadores */
criaSessaoFormacaoEducadores();

$sql = "SELECT * FROM projovemurbano.formacaoeducadores WHERE fedid='".$_SESSION['projovemurbano']['fedid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
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
/*Carregando Trnasporte de Material Didático*/
$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
$transportematerial = $db->pegaLinha($sql);
/*Fim Carregando Trnasporte de Material Didático*/
/* Carregando Demais Ações */
criaSessaoDemaisAcoes();
$sql = "SELECT propercutilizado FROM projovemurbano.profissionais WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$propercutilizado = $db->pegaUm($sql);
$sql = "SELECT fedperutilizado FROM projovemurbano.formacaoeducadores WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' --AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$fedperutilizado = $db->pegaUm($sql);
$sql = "SELECT galpercutilizado FROM projovemurbano.generoalimenticio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' --AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$galpercutilizado = $db->pegaUm($sql);
$sql = "SELECT qprpercutilizado FROM projovemurbano.qualificacaoprofissional WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' --AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$qprpercutilizado = $db->pegaUm($sql);
$sql = "SELECT tmaperutilizado FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$tmaperutilizado = $db->pegaUm($sql);
$sql = "SELECT deapercutilizado FROM projovemurbano.demaisacoes WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' --AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$deapercutilizado = $db->pegaUm($sql);
$sql = "SELECT aufpercutilizado FROM projovemurbano.auxiliofinanceiro aux 
								 INNER JOIN projovemurbano.formacaoeducadores fed ON fed.fedid = aux.fedid  
								 WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
$aufpercutilizado = $db->pegaUm($sql);

$deapercmax = 100-($propercutilizado+$fedperutilizado+$galpercutilizado+$qprpercutilizado+$tmaperutilizado+$aufpercutilizado);
$sugestaoampliacao = carregarSugestaoAmpliacao();
$meta = carregarMeta($sugestaoampliacao);
//ver($sugestaoampliacao);
$retornarTotalMaximoDemaisAcoes=true;
$totalMax = validacaoCompletaPlanoImplementacao2013();
//ver($totalMax);
$deaperx = (($totalMax*100)/calcularMontante($meta));
/* Fim - Carregando Demais Ações */
$sql = "SELECT	tprdesc FROM projovemurbano.tipoprograma WHERE 	tprid = {$_SESSION['projovemurbano']['tprid']}";
$publico =$db->pegaUm($sql);
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
				<td align="center"><? echo (($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax); ?>% - R$ 
									<span id="span_totalmaximo"><?=number_format(calcularMontante($meta)*(($profissionais['propercmax'])?$profissionais['propercmax']:$propercmax)/100,2,",",".") ?></span></td>
				<td align="center"><? echo (($totalGeral['percent'])?number_format($totalGeral['percent'],2,",","."):'0.0'); ?>% - R$ 
									<span id="span_totalutilizado"><?=number_format($totalGeral['valor'],2,",",".") ?></span></td>
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
		<? if($coordgeral) : ?>
			<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="SubTituloDireita" width="20%">Quantidade</td>
					<td><? echo (($coordgeral['cgeqtd'])?$coordgeral['cgeqtd']:'1'); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="20%">Opções</td>
					<td><? echo (($coordgeral['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND opptipo='C' AND oppid='".$coordgeral['oppid']."'"):"Não selecionado"); ?></td>
				</tr>
	
				<tr id="coordenadorgeral_complementacao" <?=(($coordgeral['oppid']==OPP_EFETIVO_COMPLEMENTACAO)?"":"style=\"display:none;\"") ?> >
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
								<td align="center"><? echo (($coordgeral['cgehorpagrec'])?$coordgeral['cgehorpagrec']:'&nbsp;'); ?></td>
								<td align="center"><? echo (($coordgeral['cgevlrhora'])?number_format($coordgeral['cgevlrhora'],2,",","."):'&nbsp;'); ?></td>
								<td align="center"><? echo (($coordgeral['cgevlrcomp'])?number_format($coordgeral['cgevlrcomp'],2,",","."):'&nbsp;'); ?></td>
								
								<td align="center"><? echo (($coordgeral['cgeqtdmeses'])?$coordgeral['cgeqtdmeses']:'22'); ?></td>						
								<td align="center"><? echo $coordgeral['cgeencargos']; ?></td>
								<td align="center"><? echo (($coordgeral['cgevlrtotal'])?number_format($coordgeral['cgevlrtotal'],2,",","."):'&nbsp;'); ?></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr id="coordenadorgeral_complementacao1" <?=(($coordgeral['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA)?"":"style=\"display:none;\"") ?>>
						<td colspan="2">
							<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
								<tr>
									<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
									<td class="SubTituloCentro">Meses</td>
									<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
									<td class="SubTituloCentro">Valor Total</td>
								</tr>
								<tr>
									<td align="center"><? echo (($coordgeral['crevlrbrutorem'])?number_format($coordgeral['crevlrbrutorem'],2,",","."):'&nbsp'); ?></td>
									<td align="center"><? echo (($coordgeral['creqtdmeses'])?$coordgeral['creqtdmeses']:'22'); ?></td>
									<td align="center"><? echo $coordgeral['creencargos']; ?></td>
									<td align="center"><? echo (($coordgeral['crevlrtotal'])?number_format($coordgeral['crevlrtotal'],2,",","."):'&nbsp'); ?></td>
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
			<tr id="assistente_A_complementacao2" <?=(($assistenteadministrativo_A['oppid']==OPP_EFETIVO_COMPLEMENTACAO_A)?"":"style=\"display:none;\"") ?> >
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
							<td align="center"><? echo (($assistenteadministrativo_A['ccmqtdhoras'])?number_format($assistenteadministrativo_A['ccmqtdhoras'],2,",","."):''); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_A['ccmvlrhora'])?number_format($assistenteadministrativo_A['ccmvlrhora'],2,",","."):''); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_A['ccmvlrcomp'])?number_format($assistenteadministrativo_A['ccmvlrcomp'],2,",","."):''); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_A['ccmqtdmeses'])?$assistenteadministrativo_A['ccmqtdmeses']:'22'); ?></td>						
							<td align="center"><? echo $assistenteadministrativo_A['ccmencargos']; ?></td>
							<td align="center"><? echo (($assistenteadministrativo_A['ccmvlrtotal'])?number_format($assistenteadministrativo_A['ccmvlrtotal'],2,",","."):''); ?></td>
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
			<tr id="assistente_P_complementacao2" <?=(($assistenteadministrativo_P['oppid']==OPP_EFETIVO_COMPLEMENTACAO_P)?"":"style=\"display:none;\"") ?> >
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
								<td align="center"><? echo (($assistenteadministrativo_P['ccmqtdhoras'])?number_format($assistenteadministrativo_P['ccmqtdhoras'],2,",","."):'&nbsp;'); ?></td>
								<td align="center"><? echo (($assistenteadministrativo_P['ccmvlrhora'])?number_format($assistenteadministrativo_P['ccmvlrhora'],2,",","."):'&nbsp;');?></td>
								<td align="center"><? echo (($assistenteadministrativo_P['ccmvlrcomp'])?number_format($assistenteadministrativo_P['ccmvlrcomp'],2,",","."):'&nbsp;'); ?></td>
								<td align="center"><? echo (($assistenteadministrativo_P['ccmqtdmeses'])?$assistenteadministrativo_P['ccmqtdmeses']:'22'); ?></td>						
								<td align="center"><? echo $assistenteadministrativo_P['ccmencargos']; ?></td>
								<td align="center"><? echo (($assistenteadministrativo_P['ccmvlrtotal'])?number_format($assistenteadministrativo_P['ccmvlrtotal'],2,",","."):''); ?></td>
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
		<td class="SubTituloDireita">Assistente Administrativo para Unidade Prisional</td>
		<td>
		<? if($assistenteadministrativo_AP && $_SESSION['projovemurbano']['tprid'] == 2 ) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistenteadministrativo_AP['coaqtd'])?$assistenteadministrativo_AP['coaqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistenteadministrativo_AP['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND oppid='".$assistenteadministrativo_AP['oppid']."'"):"Não selecionado");	?></td>
			</tr>
			<tr id="assistente_P_complementacao" <?=(($assistenteadministrativo_AP['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_A)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>
					</tr>
					<tr>
						<td align="center"><? echo (($assistenteadministrativo_AP['coavlrbrutorem'])?number_format($assistenteadministrativo_AP['coavlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($assistenteadministrativo_AP['coaqtdmeses'])?$assistenteadministrativo_AP['coaqtdmeses']:'22'); ?></td>
						<td align="center"><? echo ($assistenteadministrativo_AP['coaencargos']); ?></td>
						<td align="center"><? echo (($assistenteadministrativo_AP['coavlrtotal'])?number_format($assistenteadministrativo_AP['coavlrtotal'],2,",","."):''); ?></td>
					</tr>
				</table>
				</td>
			</tr>
			<tr id="assistente_AP_complementacao2" <?=(($assistenteadministrativo_AP['oppid']==OPP_EFETIVO_COMPLEMENTACAO_A)?"":"style=\"display:none;\"") ?> >
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
							<td align="center"><? echo (($assistenteadministrativo_AP['ccmqtdhoras'])?number_format($assistenteadministrativo_AP['ccmqtdhoras'],2,",","."):'&nbsp;'); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_AP['ccmvlrhora'])?number_format($assistenteadministrativo_AP['ccmvlrhora'],2,",","."):'&nbsp'); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_AP['ccmvlrcomp'])?number_format($assistenteadministrativo_AP['ccmvlrcomp'],2,",","."):'&nbsp'); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_AP['ccmqtdmeses'])?$assistenteadministrativo_AP['ccmqtdmeses']:'22'); ?></td>						
							<td align="center"><? echo $assistenteadministrativo_AP['ccmencargos']; ?></td>
							<td align="center"><? echo (($assistenteadministrativo_AP['ccmvlrtotal'])?number_format($assistenteadministrativo_AP['ccmvlrtotal'],2,",","."):'&nbsp'); ?></td>
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
		<td class="SubTituloDireita">Assistente Pedagógico para Unidade Prisional</td>
		<td>
		<? if($assistenteadministrativo_PP && $_SESSION['projovemurbano']['tprid'] == 2 ) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistenteadministrativo_PP['coaqtd'])?$assistenteadministrativo_PP['coaqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistenteadministrativo_PP['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND oppid='".$assistenteadministrativo_PP['oppid']."'"):"Não selecionado");	?></td>
			</tr>
			<tr id="assistente_P_complementacao" <?=(($assistenteadministrativo_PP['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_P)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total (R$)</td>
						</tr>
						<tr>
							<td align="center"><? echo (($assistenteadministrativo_PP['coavlrbrutorem'])?number_format($assistenteadministrativo_PP['coavlrbrutorem'],2,",","."):''); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['coaqtdmeses'])?$assistenteadministrativo_PP['coaqtdmeses']:'22'); ?></td>
							<td align="center"><? echo ($assistenteadministrativo_PP['coaencargos']); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['coavlrtotal'])?number_format($assistenteadministrativo_PP['coavlrtotal'],2,",","."):''); ?></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr id="assistente_PP_complementacao2" <?=(($assistenteadministrativo_PP['oppid']==OPP_EFETIVO_COMPLEMENTACAO_P)?"":"style=\"display:none;\"") ?> >
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
							<td align="center"><? echo (($assistenteadministrativo_PP['ccmqtdhoras'])?number_format($assistenteadministrativo_PP['ccmqtdhoras'],2,",","."):'&nbsp;'); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['ccmvlrhora'])?number_format($assistenteadministrativo_PP['ccmvlrhora'],2,",","."):'&nbsp');?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['ccmvlrcomp'])?number_format($assistenteadministrativo_PP['ccmvlrcomp'],2,",","."):'&nbsp'); ?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['ccmqtdmeses'])?$assistenteadministrativo_PP['ccmqtdmeses']:'22'); ?></td>						
							<td align="center"><? echo $assistenteadministrativo_PP['ccmencargos']; ?></td>
							<td align="center"><? echo (($assistenteadministrativo_PP['ccmvlrtotal'])?number_format($assistenteadministrativo_PP['ccmvlrtotal'],2,",","."):'&nbsp'); ?></td>
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
		<? if($assistenteadministrativo_M) : ?> 
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita" width="20%">Quantidade</td>
				<td><? echo (($assistenteadministrativo_M['coaqtd'])?$assistenteadministrativo_M['coaqtd']:'1'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Opções</td>
				<td><? echo (($assistenteadministrativo_M['oppid'])?$db->pegaUm("SELECT oppdesc FROM projovemurbano.opcoesprofissionais WHERE oppstatus='A' AND oppid='".$assistenteadministrativo_M['oppid']."'"):"Não selecionado");	?></td>
			</tr>
			<tr id="assistente_P_complementacao" <?=(($assistenteadministrativo_M['oppid']==OPP_CONTRATO_RECURSOS_PROGRAMA_A)?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">Valor bruto da remuneração/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>
					</tr>
					<tr>
						<td align="center"><? echo (($assistenteadministrativo_M['coavlrbrutorem'])?number_format($assistenteadministrativo_M['coavlrbrutorem'],2,",","."):''); ?></td>
						<td align="center"><? echo (($assistenteadministrativo_M['coaqtdmeses'])?$assistenteadministrativo_M['coaqtdmeses']:'22'); ?></td>
						<td align="center"><? echo $assistenteadministrativo_M['coaencargos']; ?></td>
						<td align="center"><? echo (($assistenteadministrativo_M['coavlrtotal'])?number_format($assistenteadministrativo_M['coavlrtotal'],2,",","."):''); ?></td>
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
						<td><? echo (($dirassistentes_A['dasqtdefetivo40hr'])?$$dirassistentes_A['dasqtdefetivo40hr']:'0'); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="30%">Contratado com recursos próprios</td>
						<td><? echo (($dirassistentes_A['dasqtdrecursoproprio'])?$dirassistentes_A['dasqtdrecursoproprio']:'0'); ?></td>
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
			<tr>
				<td colspan="2">
					<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
						<tr>
							<td class="SubTituloCentro">&nbsp;</td>
							<td class="SubTituloCentro">Número de assistentes</td>
							<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
							<td class="SubTituloCentro">Valor da hora (R$)</td>
							<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
							<td class="SubTituloCentro">Meses</td>
							<td class="SubTituloCentro">Encargos sociais<br> e/ou trabalhistas (%)</td>
							<td class="SubTituloCentro">Valor Total (R$)</td>
	
						</tr>
						<tr>
							<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
							<td align="center"><? echo $dirassistentes_A['ccmqtd']; ?></td>
							<td align="center"><? echo $dirassistentes_A['ccmqtdhoras']; ?></td>
							<td align="center"><? echo (($dirassistentes_A['ccmvlrhora'])?number_format($dirassistentes_A['ccmvlrhora'],2,",","."):''); ?></td>
							<td align="center"><? echo (($dirassistentes_A['ccmvlrcomp'])?number_format($dirassistentes_A['ccmvlrcomp'],2,",","."):''); ?></td>
							<td align="center"><? echo (($dirassistentes_A['ccmqtdmeses'])?$dirassistentes_A['ccmqtdmeses']:'18'); ?></td>
							<td align="center"><? echo $dirassistentes_A['ccmencargos']; ?></td>
							<td align="center"><? echo (($dirassistentes_A['ccmvlrtotal'])?number_format($dirassistentes_A['ccmvlrtotal'],2,",","."):''); ?></td>
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
			<tr>
				<td colspan="2">
				<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloCentro">&nbsp;</td>
						<td class="SubTituloCentro">Número de assistentes</td>
						<td class="SubTituloCentro">Número de horas pagas com recursos do programa/mês (Número de horas do total de educadores)</td>
						<td class="SubTituloCentro">Valor da hora (R$)</td>
						<td class="SubTituloCentro">Valor da complementação/mês (R$)</td>
						<td class="SubTituloCentro">Meses</td>
						<td class="SubTituloCentro">Encargos sociais<br> e/ou trabalhistas (%)</td>
						<td class="SubTituloCentro">Valor Total (R$)</td>

					</tr>
					<tr>
						<td class="SubTituloDireita">Efetivo com complementação da carga horária</td>
						<td align="center"><? echo $dirassistentes_P['ccmqtd']; ?></td>
						<td align="center"><? echo $dirassistentes_P['ccmqtdhoras']; ?></td>
						<td align="center"><? echo (($dirassistentes_P['ccmvlrhora'])?number_format($dirassistentes_P['ccmvlrhora'],2,",","."):''); ?></td>
						<td align="center"><? echo (($dirassistentes_P['ccmvlrcomp'])?number_format($dirassistentes_P['ccmvlrcomp'],2,",","."):''); ?></td>
						<td align="center"><? echo (($dirassistentes_P['ccmqtdmeses'])?$dirassistentes_P['ccmqtdmeses']:'18'); ?></td>
						<td align="center"><? echo $dirassistentes_P['ccmencargos']; ?></td>
						<td align="center"><? echo (($dirassistentes_P['ccmvlrtotal'])?number_format($dirassistentes_P['ccmvlrtotal'],2,",","."):''); ?></td>
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
				<td><? echo $infoInstLegal['inlnuminstlegal']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Data do instrumento :</b></td>
				<td><? echo formata_data($infoInstLegal['inldatainstlegal']); ?></td>
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
		
		$sql = "SELECT DISTINCT refdesc, 
					   CASE WHEN rec.rgavalor IS NULL THEN '' ELSE trim(to_char(rec.rgavalor,'999g999g999d99')) END as inp 
				FROM projovemurbano.referenciaformacao ref 
				LEFT JOIN projovemurbano.recursosgastos rec ON rec.refid=ref.refid AND rec.fedid='".$_SESSION['projovemurbano']['fedid']."' 
				WHERE refstatus='A' 
				AND refesfera='$refesfera'
				AND ppuid = {$_SESSION['projovemurbano']['ppuid']}";
//		ver($sql);
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
					<td align="center"><? echo (($qualificacaoprofissional['qprpercutilizado'])?number_format($qualificacaoprofissional['qprpercutilizado'],1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($db->pegaUm(
							"SELECT	SUM(vlr) FROM(SELECT DISTINCT SUM(DISTINCT COALESCE(pgaqtdmeses,0)*COALESCE(pgavlrmes,0)) as vlr, desid FROM projovemurbano.previsaogasto 
							WHERE pgastatus='A' AND qprid = '".$_SESSION['projovemurbano']['qprid']."' group by desid) as foo"),2,",",".") ?></span></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="25%">Tipo de oferta</td>
		<td>
		<?
		$sql = "SELECT desdesc,
					   COALESCE((SELECT DISTINCT CASE WHEN pgavlrmes IS NULL THEN '' ELSE trim(to_char(pgavlrmes,'999g999g999d99')) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."' LIMIT 1),'') as a, 
					   COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NULL THEN '10' ELSE trim(pgaqtdmeses::text) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."' LIMIT 1),'10') as b,
					   COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL THEN trim(to_char(pgaqtdmeses*pgavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."' LIMIT 1),'') as c
				FROM projovemurbano.despesas des 
				WHERE desstatus='A'";
		
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
 		?>
		</td>
	</tr>
</table>
<?if($_SESSION['projovemurbano']['estuf']){?>
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
						<td align="center"><? echo number_format($transportematerial['tmapercmax'],1); ?>% - R$ <span><?=number_format(calcularMontante($meta)*$transportematerial['tmapercmax']/100,2,",",".") ?></span></td>
						<td align="center"><? echo (($transportematerial['tmaperutilizado'])?number_format($transportematerial['tmaperutilizado'],1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($transportematerial['tmarecursoutilizado'],2,",",".") ?></span></td>
					</tr>
				</table>
			</td>
		</tr>
<!-- 		<tr> -->
<!-- 			<td class="SubTituloDireita" width="25%">Tipo de oferta</td> -->
<!-- 			<td> -->
			<?
// 			$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
			
// 			$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
			
// 			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
// 	 		?>
<!-- 			</td> -->
<!-- 		</tr> -->
	</table>
<?}?>
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
			<td align="center"><? echo number_format($deaperx,1); ?>% - R$ <span><?=number_format((calcularMontante($meta)*$deaperx)/100,2,",",".") ?></span></td>
			<td align="center"><? echo (($deapercutilizado)?number_format($deapercutilizado,1):'0.0'); ?>% - R$ <span id="span_utilizado"><?=number_format($db->pegaUm("SELECT SUM(idaqtdmeses*idavlrmes)FROM projovemurbano.itemdemaisacoes WHERE deaid='".$_SESSION['projovemurbano']['deaid']."' AND idastatus='A'"),2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Demais ações</td>
		<td>
		<?
		$sql = "SELECT
				*
				FROM(
				SELECT acodesc,
					   COALESCE((SELECT CASE WHEN idavlrmes IS NULL THEN '' ELSE trim(to_char(idavlrmes,'999g999g999d99')) END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'') as inp1, 
					   COALESCE((SELECT CASE WHEN idaqtdmeses IS NULL THEN '' ELSE trim(idaqtdmeses::text) END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'18') as inp2,
					   --COALESCE(acomaxporcent::text,'') as inp3,
					   COALESCE((SELECT CASE WHEN idaqtdmeses IS NOT NULL AND idavlrmes IS NOT NULL THEN trim(to_char(idaqtdmeses*idavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.itemdemaisacoes WHERE acoid=aca.acoid and deaid='".$_SESSION['projovemurbano']['deaid']."'),'') as inp4
				FROM projovemurbano.acoes aca 
				WHERE acostatus='A') AS A 
				WHERE
				inp1 != ''";
		$cabecalho=array("Demais Ações","Valor/mês(R$)","Meses(18)"/*,"Percentual Máximo previstos(%)"*/,"Valor total(R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
<?
$esdid = $db->pegaUm("SELECT d.esdid FROM projovemurbano.projovemurbano p
					  INNER JOIN workflow.documento d ON d.docid=p.docid 
					  WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'
                      --AND tprid = '{$_SESSION['projovemurbano']['tprid']}'");

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