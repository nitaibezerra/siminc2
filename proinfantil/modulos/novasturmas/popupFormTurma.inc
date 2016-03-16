<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include  "_funcoes_novasturmas.php";
$file = new FilesSimec();

//Validar Código INEP
if($_REQUEST['validar_inep']){
	$entidade = explode("-",$_REQUEST['inep']);
	$sql_inep = "SELECT 	entid
				FROM 	entidade.entidade 
				WHERE entcodent IS NOT NULL AND entstatus = 'A' AND entcodent = '".trim($entidade[0])."'";
	$rs_inep = $db->pegaLinha($sql_inep);
	$sql_prof = "SELECT resdsc
				 FROM questionario.resposta 
				 WHERE perid = 1589 AND trim(resdsc) = '".trim($entidade[0])."'";	
	$rs_prof = $db->pegaLinha($sql_prof);
	if(empty($rs_inep['entid'])){
		echo "1";
	} else {
		if(!empty($rs_prof['resdsc'])){
			echo "2";
		} else {
			echo "0";
		}
	}
	die();
}

//DOWNLOAD ARQUIVO
if($_REQUEST['requisicao'] == 'downloadArquivo'){	
	$file->getDownloadArquivo($_REQUEST['arqid']);	
}

//DELETAR ANEXO
if($_REQUEST['requisicao'] == 'deletaArquivo'){
	$sql  = "UPDATE proinfantil.turma SET arqid = null WHERE turid = {$_REQUEST['turid']};";
	$sql .= "DELETE FROM public.arquivo WHERE arqid = {$_REQUEST['arqid']};";
	$db->executar($sql);
	if($db->commit()){	
		$file->excluiArquivoFisico($_REQUEST['arqid']);
	}
	$db->sucesso('novasturmas/popupFormTurma&acao=A','&escolhaTurma='.$_REQUEST['escolhaTurma']);	
}

//PEQUISAR ESCOLAS
if($_REQUEST['requisicao'] == 'buscaEscolas'){
	ob_clean();
	//Nova Busca de escolas na tabela proinfantil.novasturmasescolas
	$sql = "SELECT 	entcodent as codigo,entcodent || ' - ' || ntenomeescola as descricao
			FROM proinfantil.novasturmasescolas
			WHERE ntestatus = 'A'
			AND (ntenomeescola ILIKE '%{$_GET["q"]}%' OR entcodent ILIKE '%{$_GET["q"]}%')
			AND nteano = '2012'
			AND muncod = '{$_REQUEST['muncod']}'
			ORDER BY ntenomeescola
			LIMIT 100";
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();

	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		echo "{$value['descricao']}|{$value['codigo']}\n";
	}
	die();
}

//VERIFICAR CEP
if($_REQUEST['requisicao'] == 'verificaCepMunicipio'){
	$cep = str_replace(array('.', '-'), '', $_REQUEST['cep']);
	$sql = "SELECT muncod FROM cep.v_endereco2 WHERE cep='{$cep}' AND muncod ='{$_SESSION['proinfantil']['muncod']}' ORDER BY cidade ASC";
	$rs = $db->pegaLinha($sql);
	echo simec_json_encode($rs);
	die();
}

if(!$_SESSION['proinfantil']['muncod']){
	$sql = "SELECT muncod 
			FROM proinfantil.usuarioresponsabilidade 
			WHERE usucpf = '{$_SESSION['usucpf']}' 
			AND pflcod = ".EQUIPE_MUNICIPAL;
	
	$muncod = $db->pegaUm($sql);
	$muncod = $muncod ? $muncod : $_REQUEST['muncod'];
	
	if($muncod){
		$_SESSION['proinfantil']['muncod'] = $muncod;
	}
}

if($_REQUEST['escolhaTurma']){
	$escolhaturma = $_REQUEST['escolhaTurma'];
	$arParams = explode('_', $_REQUEST['escolhaTurma']);
	if($arParams[0]){
		switch ($arParams[0]){
			case 'creche'		: $ttuid = 1; break;
			case 'preescola'	: $ttuid = 2; break;
			case 'unificada'	: $ttuid = 3; break;
		}

		switch ($arParams[1]){
			case 'publica'		: $tirid = 1; break;
			case 'conveniada'	: $tirid = 2; break;
		}
		$turtipoestabelecimento = $tirid == 2 ? 3 : 1;
		
		if($_REQUEST['turid']){
			$turma = "AND turid = {$_REQUEST['turid']}";
		} else {
			$turma =  "";
		}
	}
} else {
	echo "<script>
	alert('Nenhuma turma selecionada, tente novamente!');
	window.close();
	</script>";
	die();
}

//SALVAR TURMA
if($_REQUEST['requisicao'] == 'salvaDados'){
	$codinep 						= explode("-",$_REQUEST['codinep']);
	$muncod							= $_SESSION['proinfantil']['muncod'];
	$turdtinicio 					= $_REQUEST['turdtinicio'] ? formata_data_sql($_REQUEST['turdtinicio']) : '';
	$turdtinclusao 					= date('Y-m-d H:i:s');
	$usucpfinclusao 				= $_SESSION['usucpf'];
	$turcadeducacenso 				= $_REQUEST['turcadeducacenso'] == 'S' ? 't' : 'f';
	$turestabelecimentoautorizado 	= $_REQUEST['turestabelecimentoautorizado'] == 'S' ? 't' : 'f';
	$turnomeescola 					= $_REQUEST['turcadeducacenso'] == 'N' ? $_REQUEST['turnomeescola'] : '';
	$entcodent 						= trim($codinep[0]);
	$ttuid				= $_REQUEST['ttuid'] ? $_REQUEST['ttuid'] : 'null';
	$turenderecoescola				= $_REQUEST['turenderecoescola'];
	$turtipoestabelecimento 		= $_REQUEST['turtipoestabelecimento'] ? $_REQUEST['turtipoestabelecimento'] : 'null';
	$turlatitude					= ($_REQUEST['latitude']) ? implode(".", $_REQUEST['latitude']) : ''; 
	$turlongitude					= ($_REQUEST['longitude']) ? implode(".", $_REQUEST['longitude']) : '';
	$turcepescola					= str_replace(array('.', '-'), '', $_REQUEST['turcepescola']);
	$turenderecocodinep 			= $_REQUEST['turenderecocodinep'] ? "'".$_REQUEST['turenderecocodinep']."'" : 'null';
	$turemitidoconselho 			= $_REQUEST['turemitidoconselho'] ? "'".$_REQUEST['turemitidoconselho']."'" : 'null';
	$turtipoconselho				= $_REQUEST['turtipoconselho']; 
	$turnumeroato 					= $_REQUEST['turnumeroato'];
	$turdatapublicacaoato 			= $_REQUEST['turdatapublicacaoato'] ? "'".formata_data_sql($_REQUEST['turdatapublicacaoato'])."'" : 'null';
	$tirid 							= $_REQUEST['tirid'] ? $_REQUEST['tirid'] : 'null';
	$updateArquivo = '';
	$insertCampoArquivo = '';
	$insertValueArquivo = '';
	
	if(is_file($_FILES["arquivo"]["tmp_name"]) && $_REQUEST['turestabelecimentoautorizado'] == 'S'){
		$boSalvouArq = $file->setUpload($_FILES["arquivo"]["name"], "arquivo", false);
		if($boSalvouArq){
			$arquivoSalvo = $file->getIdArquivo();
			$updateArquivo = ",arqid={$arquivoSalvo}";
			$insertCampoArquivo = ", arqid";
			$insertValueArquivo = ", {$arquivoSalvo}";
		}		
	}
	$arquivoSalvo = $arquivoSalvo ? $arquivoSalvo : 'null';
	
	if($_REQUEST['turid']){
		$sql = "UPDATE proinfantil.turma SET 
					entcodent = '{$entcodent}', 
					turdsc = '{$_REQUEST['turdsc']}', 
					turdtinicio = '{$turdtinicio}',
					turcadeducacenso='{$turcadeducacenso}',
					turestabelecimentoautorizado='{$turestabelecimentoautorizado}',
					turnomeescola='{$turnomeescola}'
					{$updateArquivo}
					,ttuid={$ttuid}
					,turenderecoescola='{$turenderecoescola}'
					,turtipoestabelecimento={$turtipoestabelecimento}
					,turlatitude='{$turlatitude}'
					,turlongitude='{$turlongitude}'
					,turcepescola='{$turcepescola}'
					,turenderecocodinep={$turenderecocodinep}					
					,turnumeroato='{$turnumeroato}'
					,turdatapublicacaoato={$turdatapublicacaoato}
					,turtipoconselho='{$turtipoconselho}'
					,tirid={$tirid}
					,turano='{$_SESSION['exercicio']}'
				WHERE 
					turid = {$_REQUEST['turid']}";
		
		$db->executar($sql);
		$turid = $_REQUEST['turid'];
	} else {
		$sql = "INSERT INTO proinfantil.turma
					(muncod, entcodent, turdsc, turdtinicio, turdtinclusao, usucpfinclusao, turstatus, turcadeducacenso, turestabelecimentoautorizado, 
					turnomeescola {$insertCampoArquivo}, ttuid, turenderecoescola, turtipoestabelecimento, turlatitude, turlongitude,
					turcepescola, turenderecocodinep, turtipoconselho, turnumeroato, turdatapublicacaoato, tirid, turano)
				VALUES
					('{$muncod}', '{$entcodent}', '{$_REQUEST['turdsc']}', '{$turdtinicio}', '{$turdtinclusao}', '{$usucpfinclusao}', 'A', 
					'{$turcadeducacenso}', '{$turestabelecimentoautorizado}', '{$turnomeescola}' {$insertValueArquivo}, {$ttuid},
					'{$turenderecoescola}', {$turtipoestabelecimento}, '{$turlatitude}', '{$turlongitude}', '{$turcepescola}',
					{$turenderecocodinep}, '{$turtipoconselho}', '{$turnumeroato}', {$turdatapublicacaoato}, {$tirid}, '{$_SESSION['exercicio']}')
				returning turid;";
		$turid = $db->pegaUm($sql);
	}
	
	if($turid){
		if(!empty($_REQUEST['alaquantidade_creche'][0]) || !empty($_REQUEST['alaquantidade_creche'][1])){
			$sql = "DELETE FROM proinfantil.mdsalunoatendidopbf WHERE turid = {$turid} and timid = 3;";
			foreach($_REQUEST['alaquantidade_creche'] as $key=>$value){
				$alaquantidade_creche = $value && in_array($ttuid, array(1,3)) ? str_replace('.','',$value) : '0';
				$alaquantidadeprofessor_creche = $_REQUEST['alaquantidadeprofessor_creche'][$key] && in_array($ttuid, array(1,3)) ? str_replace('.','',$_REQUEST['alaquantidadeprofessor_creche'][$key]) : '0';
				
				if($_REQUEST['creche'][$key] == "S"){
				$sql .= "INSERT INTO proinfantil.mdsalunoatendidopbf 
							(alaquantidade, titid, turid, timid, alaquantidadeprofessor)
						 VALUES
						 	({$alaquantidade_creche}, '{$_REQUEST['titid'][$key]}', '{$turid}', '3', {$alaquantidadeprofessor_creche});";
				}			
			}
			$db->executar($sql);			
		}
		
		if(!empty($_REQUEST['alaquantidade_pre'][0]) || !empty($_REQUEST['alaquantidade_pre'][1])){
			$sql = "DELETE FROM proinfantil.mdsalunoatendidopbf WHERE turid = {$turid} AND timid = 1;";
			foreach($_REQUEST['alaquantidade_pre'] as $key=>$value){
				$alaquantidade_pre = $value && in_array($ttuid, array(2,3)) ? str_replace('.','',$value) : '0';
				$alaquantidadeprofessor_pre = $_REQUEST['alaquantidadeprofessor_pre'][$key] && in_array($ttuid, array(2,3)) ? str_replace('.','',$_REQUEST['alaquantidadeprofessor_pre'][$key]) : '0';
				
				if($_REQUEST['pre'][$key] == "S"){
				$sql .= "INSERT INTO proinfantil.mdsalunoatendidopbf 
							(alaquantidade, titid, turid, timid, alaquantidadeprofessor)
						 VALUES
						 	({$alaquantidade_pre}, '{$_REQUEST['titid'][$key]}', '{$turid}', '1', {$alaquantidadeprofessor_pre});";
				}
			}
			$db->executar($sql);			
		}		
	}
	
	$db->commit();
	$db->sucesso('novasturmas/popupFormTurma','&escolhaTurma='.$_REQUEST['escolhaTurma'].'&turid='.$_REQUEST['turid']);
	exit();
}

if($_REQUEST['turid']){
	$turid = $_REQUEST['turid'];
}

//Chamada de programa
include_once APPRAIZ . "includes/workflow.php";

$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);
$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

if( $_SESSION['exercicio'] == date('Y') && $esdid != WF_NOVASTURMAS_EM_DILIGENCIA ){
	$acesso['cadastro'] = 'N';
}

$tirid = $tirid ? $tirid : 0;
$ttuid = $ttuid ? $ttuid : 0;

//Recupera turmas do município	
$sql = "SELECT
			tur.turid,
			tur.entcodent,
			tur.turdsc,
			tur.turdtinicio,
			ent.entcodent || ' - ' || ent.entnome as codinep,
			turcadeducacenso,
			turestabelecimentoautorizado,
			turnomeescola,
			arqid,
			ttuid,
			turenderecoescola,
			turtipoestabelecimento,
			turlatitude, 
			turlongitude,
			turcepescola,
			turenderecocodinep,
			turtipoconselho,
			turnumeroato,
			turdatapublicacaoato
		FROM 
			proinfantil.turma tur
		LEFT JOIN 
			entidade.entidade ent on trim(ent.entcodent) = trim(tur.entcodent)
		WHERE 
			muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND turstatus = 'A'
			AND turano <= '2012'
			AND ttuid = {$ttuid}
			AND tirid = {$tirid}
		$turma";
		
$rs = $db->carregar($sql);
$keyTurma = $arParams[2]-1;

if(!empty($rs)){
	if($rs[$keyTurma]){	
		extract($rs[$keyTurma]);
	} 
	
	if($rs['0'] && !empty($_REQUEST['turid'])){	
		extract($rs['0']);
	} 
}

$turid = $turid ? $turid : 0;

$titulo1 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Cadastro de novas turmas', $titulo1);

$rsTurmasMatriculas = recuperarDadosMatriculasTurmas();

//NOVA REGRA PARA NÃO BURLAR QUANTIDADE DE ALUNOS
if($turtipoestabelecimento == 1){
	//Creche Integral
	$creche_int = ($rsTurmasMatriculas['ntmqtdmatriculacrecheintegralpublica']) - ($rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica']); 
	//Creche Parcial
	$creche_par = ($rsTurmasMatriculas['ntmqtdmatriculacrecheparcialpublica']) - ($rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica']);
	//PréEscola Integral
	$pre_int = ($rsTurmasMatriculas['ntmqtdmatriculapreescolaintegralpublica']) - ($rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica']);
	//PréEscola Parcial
	$pre_par = ($rsTurmasMatriculas['ntmqtdmatriculapreescolaparcialpublica']) - ($rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica']);
} else{
	//Creche Integral
	$creche_int = ($rsTurmasMatriculas['ntmqtdmatriculacrecheintegralconveniada']) - ($rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada']); 
	//Creche Parcial
	$creche_par = ($rsTurmasMatriculas['ntmqtdmatriculacrecheparcialconveniada']) - ($rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada']);
	//PréEscola Integral
	$pre_int = ($rsTurmasMatriculas['ntmqtdmatriculapreescolaintegralconveniada']) - ($rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada']);
	//PréEscola Parcial
	$pre_par = ($rsTurmasMatriculas['ntmqtdmatriculapreescolaparcialconveniada']) - ($rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada']);
}

//Creche - Municipal ou Distrital - Integral
$colunaA = $rsTurmasMatriculas['ntmqtdmatriculacrecheintegralpublica']-$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica'];
//Creche - Conveniado - Integral
$colunaB = $rsTurmasMatriculas['ntmqtdmatriculacrecheintegralconveniada']-$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada'];
//Creche  - Municipal ou Distrital - Parcial
$colunaC = $rsTurmasMatriculas['ntmqtdmatriculacrecheparcialpublica']-$rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica'];
//Creche  - Conveniado - Parcial
$colunaD = $rsTurmasMatriculas['ntmqtdmatriculacrecheparcialconveniada']-$rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada'];
//PréEscola - Municipal ou Distrital - Integral
$colunaE = $rsTurmasMatriculas['ntmqtdmatriculapreescolaintegralpublica']-$rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica'];
//PréEscola - Conveniado - Integral
$colunaF = $rsTurmasMatriculas['ntmqtdmatriculapreescolaintegralconveniada']-$rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada'];
//PréEscola - Municipal ou Distrital - Parcial
$colunaG = $rsTurmasMatriculas['ntmqtdmatriculapreescolaparcialpublica']-$rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica'];
//PréEscola - Conveniado - Parcial
$colunaH = $rsTurmasMatriculas['ntmqtdmatriculapreescolaparcialconveniada']-$rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada'];

//Constantes 
$tempoIntegral = 5;
$tempoParcial = 6;
$tipoCreche = 3;
$tipoPreescola = 1;
$depenCreche = 1;
$depenPreescola	= 2;
$depenCrechePreescola = 3;
$estabMunicipalDistrital = 1;
$estabConveniado = 3;
$redePublica = 1;
$redeConveniada = 2;

//Variavéis Soma de Matriculas
$colunamatA = 0;
$colunamatB = 0;
$colunamatC = 0;
$colunamatD = 0;
$colunamatE = 0;
$colunamatF = 0;
$colunamatG = 0;
$colunamatH = 0;

$rsMatrCad = recuperarDadosMatriculasCadastradas($turid);

if(!empty($rsMatrCad)){
	foreach($rsMatrCad as $MatrCad){
		//Creche - Municipal ou Distrital - Integral
		if($MatrCad['ttuid'] == $depenCreche && $MatrCad['turtipoestabelecimento'] == $estabMunicipalDistrital && $MatrCad['titid'] == $tempoIntegral && $MatrCad['tirid'] == $redePublica){
			$colunamatA = $MatrCad['qtdalunos'];
		}
	
		//Creche - Conveniado - Integral
		if($MatrCad['ttuid'] == $depenCreche && $MatrCad['turtipoestabelecimento'] == $estabConveniado && $MatrCad['titid'] == $tempoIntegral && $MatrCad['tirid'] == $redeConveniada){
			//dbg($MatrCad);
			$colunamatB = $MatrCad['qtdalunos'];
		}	

		//Creche  - Municipal ou Distrital - Parcial
		if($MatrCad['ttuid'] == $depenCreche && $MatrCad['turtipoestabelecimento'] == $estabMunicipalDistrital && $MatrCad['titid'] == $tempoParcial && $MatrCad['tirid'] == $redePublica){
			$colunamatC = $MatrCad['qtdalunos'];
		} 
	
		//Creche - Conveniado - Parcial
		if($MatrCad['ttuid'] == $depenCreche && $MatrCad['turtipoestabelecimento'] == $estabConveniado && $MatrCad['titid'] == $tempoParcial && $MatrCad['tirid'] == $redeConveniada){
     		$colunamatD = $MatrCad['qtdalunos'];
    	}	

		//PréEscola - Municipal ou Distrital - Integral
		if($MatrCad['ttuid'] == $depenPreescola && $MatrCad['turtipoestabelecimento'] == $estabMunicipalDistrital && $MatrCad['titid'] == $tempoIntegral && $MatrCad['tirid'] == $redePublica){
			$colunamatE = $MatrCad['qtdalunos'];
		}
	
    	//PréEscola - Conveniado - Integral
    	if($MatrCad['ttuid'] == $depenPreescola && $MatrCad['turtipoestabelecimento'] == $estabConveniado && $MatrCad['titid'] == $tempoIntegral && $MatrCad['tirid'] == $redeConveniada){
      		$colunamatF = $MatrCad['qtdalunos'];
    	}	

		//PréEscola - Municipal ou Distrital - Parcial
		if($MatrCad['ttuid'] == $depenPreescola && $MatrCad['turtipoestabelecimento'] == $estabMunicipalDistrital && $MatrCad['titid'] == $tempoParcial && $MatrCad['tirid'] == $redePublica){
			$colunamatG = $MatrCad['qtdalunos'];
		}
	
    	//PréEscola - Conveniado - Parcial
    	if($MatrCad['ttuid'] == $depenPreescola && $MatrCad['turtipoestabelecimento'] == $estabConveniado && $MatrCad['titid'] == $tempoParcial && $MatrCad['tirid'] == $redeConveniada){
        	$colunamatH = $MatrCad['qtdalunos'];
    	}	
	}
}	

$qtd_creche5 = 0;
$qtd_creche6 = 0;
$qtd_pre5 = 0;
$qtd_pre6 = 0;
$qtd_crechepre5 = 0;
$qtd_crechepre6 = 0;

//Creche  - Municipal ou Distrital - Integral/Parcial
if($ttuid == $depenCreche && $turtipoestabelecimento == $estabMunicipalDistrital && $tirid == $redePublica){
	$qtd_a 			= $colunaA-$colunamatA;
	$qtd_creche5 	= $qtd_a;
	$qtd_c 			= $colunaC-$colunamatC;
   	$qtd_creche6 	= $qtd_c;
}

//Creche - Conveniado - Integral/Parcial
if($ttuid == $depenCreche && $turtipoestabelecimento == $estabConveniado && $tirid == $redeConveniada){
	$qtd_b 			= $colunaB-$colunamatB;
	$qtd_creche5  	= $qtd_b;
	$qtd_d 			= $colunaD-$colunamatD;
	$qtd_creche6 	= $qtd_d;
}	

//PréEscola - Municipal ou Distrital - Integral/Parcial
if($ttuid == $depenPreescola && $turtipoestabelecimento == $estabMunicipalDistrital && $tirid == $redePublica){
	$qtd_e 			= $colunaE-$colunamatE;
	$qtd_pre5 		= $qtd_e;	
	$qtd_g 			= $colunaG-$colunamatG;
    $qtd_pre6 		= $qtd_g;	
}

//PréEscola - Conveniado - Integral/Parcial
if($ttuid == $depenPreescola && $turtipoestabelecimento == $estabConveniado && $tirid == $redeConveniada){
	$qtd_f 			= $colunaF-$colunamatF;
	$qtd_pre5 		= $qtd_f;	
 	$qtd_h 			= $colunaH-$colunamatH;
   	$qtd_pre6 		= $qtd_h;	
}     

//CrechePréEscola - Municipal ou Distrital - Integral/Parcial
if($ttuid == $depenCrechePreescola && $turtipoestabelecimento == $estabMunicipalDistrital && $tirid == $redePublica){
	$qtd_a 			= $colunaA-$colunamatA;
	$qtd_creche5 	= $qtd_a;
	$qtd_c 			= $colunaC-$colunamatC;
   	$qtd_creche6 	= $qtd_c;
	$qtd_e 			= $colunaE-$colunamatE;
	$qtd_pre5 		= $qtd_e;	
	$qtd_g 			= $colunaG-$colunamatG;
    $qtd_pre6 		= $qtd_g;	
}

//CrechePréEscola - Conveniado - Integral/Parcial
if($ttuid == $depenCrechePreescola && $turtipoestabelecimento == $estabConveniado && $tirid == $redeConveniada){
	$qtd_b 			= $colunaB-$colunamatB;
	$qtd_creche5  	= $qtd_b;
	$qtd_d 			= $colunaD-$colunamatD;
	$qtd_creche6 	= $qtd_d;	
	$qtd_f 			= $colunaF-$colunamatF;
	$qtd_pre5 		= $qtd_f;	
 	$qtd_h 			= $colunaH-$colunamatH;
   	$qtd_pre6 		= $qtd_h;		
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" src="../includes/calendario.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
</style>

<script type="text/javascript" language="javascript">
$(function(){
	$('[name=codinep]').autocomplete("proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&requisicao=buscaEscolas&muncod="+$('[name=muncod]').val(), {
	  	cacheLength:10,
		width: 440,		
		scrollHeight: 220,
		selectFirst: true,
		autoFill: true	    		
	}).result(function(event, data, formatted) {
		$('[name=entcodent]').val(data[1]);
	});

	$('[name=codinep]').change(function(){		
		if(this.value == ''){
			$('[name=entcodent]').val('');
		}
	});

	$('#btn_salvar').click(function(){
		var erro = false;
		var nome = $('[name=turdsc]').val();
		var data = $('[name=turdtinicio]').val();
		var entcodent = $('[name=entcodent]').val();
		var anexo = $('#link_anexo').html() ? $('#link_anexo').html() : $('[name=arquivo]').val();

		if($('[name=turenderecocodinep]:checked').val() == 'f'){
			if($('[name=turenderecoescola]').val() == ''){
				alert('O campo endereço da escola é obrigatório!');
				$('[name=turenderecoescola]').focus();
				return false;
			}
		}
		
		if($('[name=turestabelecimentoautorizado]:checked').val() == 'S'){
			var boolValid = false;
			$('[name=turtipoconselho]').each(function() {
				if ( $(this).is(':checked') ) {
					boolValid = true;
				}
			});
			
			if(!boolValid){
				alert('O campo "Emitido pelo" é obrigatório!');
				$('[name=turtipoconselho]').focus();
				return false;
			}
		}

		var total1 = 0;
		var valor1 = 0;
		$.each($('[name=alaquantidade_creche[]]'), function(i,v){
			valor1 = v.value ? v.value : 0;
			total1 = parseInt(valor1)+parseInt(total1);
		});

		var total2 = 0;
		var valor2 = 0;		
		$.each($('[name=alaquantidade_pre[]]'), function(i,v){
			valor2 = v.value ? v.value : 0;
			total2 = parseInt(valor2)+parseInt(total2);
		});
		
		var totalGeral = parseInt(total1)+parseInt(total2);

		if(!$('[name=turestabelecimentoautorizado]:checked').val()){
			alert('A pergunta "O estabelecimento tem ato autorizativo do respectivo sistema de ensino?" é obrigatória!');
			$('[name=turestabelecimentoautorizado]').focus();
			return false;
		}

		if(!$('[name=turcadeducacenso]:checked').val()){
			alert('A pergunta "Estabelecimento esté cadastrado no Educacenso?" é obrigatória!');
			$('[name=turcadeducacenso]').focus();
			return false;
		}

		if($('[name=turcadeducacenso]:checked').val() == 'S'){
			if($('[name=codinep]').val() == ''){
				alert('A entidade tem que informar o código do INEP!');
				$('[name=codinep]').focus();
				return false;
			}

			if($('[name=codinep]').val() != ''){
				var inep = $('[name=codinep]').val();
				var turma = "<?php echo $turid; ?>";
				$.ajax({
					type: "POST",
					url: 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&turid='+turma,
					data: { validar_inep: "true", inep: inep},
					async: false,
					success: function(retornoajax) {
					if(retornoajax == 1){
						erro = true;
						alert("O código do INEP informado não foi encontrado na Base do sistema. \n Entre em contato pelo endereço de e-mail: planodemetas@mec.gov.br");
					} else if(retornoajax == 2){
						erro = true;
						alert("Cadastro não permitido. Escola atendida pelo Unidades do Proinfância.");
					} else {
						erro = false;
					}	
  				}});
			} 			
		} else if($('[name=turcadeducacenso]:checked').val() == 'N'){			
			if($('[name=codinep]').val() == ''){
				alert('A entidade tem que informar o código do INEP!');
				$('[name=codinep]').focus();
				return false;
			}		
		}
	
		if($('[name=ttuid]').val() == 1){ // Exclusivo creche
			var totalCrecheI = 0;
			var totalCrecheP = 0;
			$.each($('[name=alaquantidade_creche[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalCrecheI = parseInt(totalCrecheI)+parseInt(v.value);
					} else {
						totalCrecheP = parseInt(totalCrecheP)+parseInt(v.value);
					} 
				}
			});

			var totalCrecheProfessorI = 0;
			var totalCrecheProfessorP = 0;
			$.each($('[name=alaquantidadeprofessor_creche[]]'), function(i, v){
				if(v.value)
					if(i==0){
						totalCrecheProfessorI = parseInt(totalCrecheProfessorI)+parseInt(v.value);
					} else {
						totalCrecheProfessorP = parseInt(totalCrecheProfessorP)+parseInt(v.value);
					} 
			});
			
			if( (parseInt(totalCrecheI) != 0 && parseInt(totalCrecheProfessorI) == 0) || (parseInt(totalCrecheP) != 0 && parseInt(totalCrecheProfessorP) == 0)){
				alert('O campo Qtd. Professores Creche é obrigatório!');
				$('[name=alaquantidadeprofessor_creche[]]').focus();
				return false;
			} else if((parseInt(totalCrecheI) == 0 && parseInt(totalCrecheProfessorI) != 0) || (parseInt(totalCrecheP) == 0 && parseInt(totalCrecheProfessorP) != 0)){
				alert('O campo Qtd. Alunos Creche é obrigatório!');
				$('[name=alaquantidade_creche[]]').focus();
				return false;
			}

			if(parseInt(totalCrecheI) == 0 && parseInt(totalCrecheProfessorI) == 0 && parseInt(totalCrecheP) == 0 && parseInt(totalCrecheProfessorP) == 0){
				alert('O campo creche é obrigatório!');
				$('[name=alaquantidade_creche[]]').focus();
				return false;
			}			

			if((totalCrecheI < totalCrecheProfessorI) || (totalCrecheP < totalCrecheProfessorP)){
				alert('Número de Professores maior que Qtd. Alunos!');
				$('[name=alaquantidade_creche[]]').focus();
				return false;
			}
			
		}else
		if($('[name=ttuid]').val() == 2){ // Exclusivo pré-escola
			var totalPreI = 0;
			var totalPreP = 0;
			$.each($('[name=alaquantidade_pre[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalPreI = parseInt(totalPreI)+parseInt(v.value);
					} else {
						totalPreP = parseInt(totalPreP)+parseInt(v.value);
					} 
				}
			});

			var totalPreProfessorI = 0;
			var totalPreProfessorP = 0;
			$.each($('[name=alaquantidadeprofessor_pre[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalPreProfessorI = parseInt(totalPreProfessorI)+parseInt(v.value);
					} else {
						totalPreProfessorP = parseInt(totalPreProfessorP)+parseInt(v.value);
					} 
				}
			});

			if( (parseInt(totalPreI) != 0 && parseInt(totalPreProfessorI) == 0) || (parseInt(totalPreP) != 0 && parseInt(totalPreProfessorP) == 0)){
				alert('O campo Qtd. Professores Pré-escola é obrigatório!');
				$('[name=alaquantidadeprofessor_pre[]]').focus();
				return false;
			} else if((parseInt(totalPreI) == 0 && parseInt(totalPreProfessorI) != 0) || (parseInt(totalPreP) == 0 && parseInt(totalPreProfessorP) != 0)){
				alert('O campo Qtd. Alunos Pré-escola é obrigatório!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}

			if(parseInt(totalPreI) == 0 && parseInt(totalPreProfessorI) == 0 && parseInt(totalPreP) == 0 && parseInt(totalPreProfessorP) == 0){
				alert('O campo pré-escola é obrigatório!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}			

			if((totalPreI < totalPreProfessorI) || (totalPreP < totalPreProfessorP)){
				alert('Número de Professores maior que Qtd. Alunos!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}			
			
		}else
		if($('[name=ttuid]').val() == 3){ // Creche e pré-escola
			var totalCrecheI = 0;
			var totalCrecheP = 0;
			$.each($('[name=alaquantidade_creche[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalCrecheI = parseInt(totalCrecheI)+parseInt(v.value);
					} else {
						totalCrecheP = parseInt(totalCrecheP)+parseInt(v.value);
					} 
				}
			});

			var totalCrecheProfessorI = 0;
			var totalCrecheProfessorP = 0;
			$.each($('[name=alaquantidadeprofessor_creche[]]'), function(i, v){
				if(v.value)
					if(i==0){
						totalCrecheProfessorI = parseInt(totalCrecheProfessorI)+parseInt(v.value);
					} else {
						totalCrecheProfessorP = parseInt(totalCrecheProfessorP)+parseInt(v.value);
					} 
			});
			
			var totalPreI = 0;
			var totalPreP = 0;
			$.each($('[name=alaquantidade_pre[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalPreI = parseInt(totalPreI)+parseInt(v.value);
					} else {
						totalPreP = parseInt(totalPreP)+parseInt(v.value);
					} 
				}
			});

			var totalPreProfessorI = 0;
			var totalPreProfessorP = 0;
			$.each($('[name=alaquantidadeprofessor_pre[]]'), function(i, v){
				if(v.value){
					if(i==0){
						totalPreProfessorI = parseInt(totalPreProfessorI)+parseInt(v.value);
					} else {
						totalPreProfessorP = parseInt(totalPreProfessorP)+parseInt(v.value);
					} 
				}
			});

			if( (parseInt(totalCrecheI) != 0 && parseInt(totalCrecheProfessorI) == 0) || (parseInt(totalCrecheP) != 0 && parseInt(totalCrecheProfessorP) == 0)){
				alert('O campo Qtd. Professores Creche é obrigatório!');
				$('[name=alaquantidadeprofessor_creche[]]').focus();
				return false;
			} else if((parseInt(totalCrecheI) == 0 && parseInt(totalCrecheProfessorI) != 0) || (parseInt(totalCrecheP) == 0 && parseInt(totalCrecheProfessorP) != 0)){
				alert('O campo Qtd. Alunos Creche é obrigatório!');
				$('[name=alaquantidade_creche[]]').focus();
				return false;
			}

			if( (parseInt(totalPreI) != 0 && parseInt(totalPreProfessorI) == 0) || (parseInt(totalPreP) != 0 && parseInt(totalPreProfessorP) == 0)){
				alert('O campo Qtd. Professores Pré-escola é obrigatório!');
				$('[name=alaquantidadeprofessor_pre[]]').focus();
				return false;
			} else if((parseInt(totalPreI) == 0 && parseInt(totalPreProfessorI) != 0) || (parseInt(totalPreP) == 0 && parseInt(totalPreProfessorP) != 0)){
				alert('O campo Qtd. Alunos Pré-escola é obrigatório!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}

			if(parseInt(totalCrecheI) == 0 && parseInt(totalCrecheProfessorI) == 0 && parseInt(totalCrecheP) == 0 && parseInt(totalCrecheProfessorP) == 0){
				alert('O campo creche é obrigatório!');
				$('[name=alaquantidade_creche[]]').focus();
				return false;
			}		
			
			if(parseInt(totalPreI) == 0 && parseInt(totalPreProfessorI) == 0 && parseInt(totalPreP) == 0 && parseInt(totalPreProfessorP) == 0){
				alert('O campo pré-escola é obrigatório!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}	

			if((totalCrecheI < totalCrecheProfessorI) || (totalCrecheP < totalCrecheProfessorP) || (totalPreI < totalPreProfessorI) || (totalPreP < totalPreProfessorP)){
				alert('Número de Professores maior que Qtd. Alunos!');
				$('[name=alaquantidade_pre[]]').focus();
				return false;
			}		
		}
	
		if(nome == ''){
			alert('O campo nome é obrigatório!');
			$('[name=turnome]').focus();
			return false;
		}

		if(data == ''){
			alert('A data de início é obrigatório!');
			$('[name=slcdtinicio]').focus();
			return false;
		}

		if(anexo == ''){
			alert('O campo anexo é obrigatório!');
			$('[name=arquivo]').focus();
			return false;
		}	

		if($('[name=ttuid]').val() == ''){
			alert('O campo tipo de atendimento é obrigatório!');
			$('[name=ttuid]').focus();
			return false;
		}

		var tipoCreche = 3;
		var tipoPreescola = 1;
		var depenCreche = 1;
		var depenPreescola	= 2;
		var depenCrechePreescola = 3;
		var estabMunicipalDistrital = 1;
		var estabConveniado = 3;
		var redePublica = 1;
		var redeConveniada = 2;
		
		//Creche  - Municipal ou Distrital - Pública
		if($('[name=ttuid]').val() == depenCreche && $('[name=turtipoestabelecimento]').val() == estabMunicipalDistrital && $('[name=tirid]').val() == redePublica){
			var vaa = Number($('#colunamat_a').val()) + Number($('#alaquantidade_creche_5').val()); 
			var vqa = $('#coluna_a').val();
			var cap_a = (Number($('#coluna_a').val())-Number($('#colunamat_a').val()));
			var soma_creche_int = Number($('#colunamat_a').val()) + Number($('#alaquantidade_creche_5').val());

			if(cap_a >= 0){
				if(vaa > vqa){
					alert("A quantidade de alunos em Creche Integral é maior que a permitida! \nCapacidade permitida "+cap_a+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				} 
			}

			if($('#creche_int').val() >= 0){
				if(soma_creche_int > $('#creche_int').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Integral: "+$('#creche_int').val()+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}	
				
			var vac = Number($('#colunamat_c').val()) + Number($('#alaquantidade_creche_6').val()); 
			var vqc = $('#coluna_c').val();
			var cap_c = (Number($('#coluna_c').val())-Number($('#colunamat_c').val()));
			var soma_creche_par = Number($('#colunamat_c').val()) + Number($('#alaquantidade_creche_6').val());

			if(cap_c >= 0){
				if(vac > vqc){
					alert("A quantidade de alunos em Creche Parcial é maior que a permitida! \nCapacidade permitida "+cap_c+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}
			
			if($('#creche_par').val() >= 0){
				if(soma_creche_par > $('#creche_par').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Parcial: "+$('#creche_par').val()+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}
		}
	
		//Creche - Conveniado - Privada
		if($('[name=ttuid]').val() == depenCreche && $('[name=turtipoestabelecimento]').val() == estabConveniado && $('[name=tirid]').val() == redeConveniada){
			var vab = Number($('#colunamat_b').val()) + Number($('#alaquantidade_creche_5').val()); 
			var vqb = $('#coluna_b').val();
			var cap_b = (Number($('#coluna_b').val())-Number($('#colunamat_b').val()));
			var soma_creche_int = Number($('#colunamat_b').val()) + Number($('#alaquantidade_creche_5').val());
			
			if(cap_b >= 0){
				if(vab > vqb){
					alert("A quantidade de alunos em Creche Integral é maior que a permitida! \nCapacidade permitida "+cap_b+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}

			if($('#creche_int').val() >= 0){
				if(soma_creche_int > $('#creche_int').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Integral: "+$('#creche_int').val()+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}

			var vad = Number($('#colunamat_d').val()) + Number($('#alaquantidade_creche_6').val()); 
			var vqd = $('#coluna_d').val();
			var cap_d = (Number($('#coluna_d').val())-Number($('#colunamat_d').val()));
			var soma_creche_par = Number($('#colunamat_d').val()) + Number($('#alaquantidade_creche_6').val());
			
			if(cap_d >= 0){
				if(vad > vqd){
					alert("A quantidade de alunos em Creche Parcial é maior que a permitida! \nCapacidade permitida "+cap_d+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}

			if($('#creche_par').val() >= 0){
				if(soma_creche_par > $('#creche_par').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Parcial: "+$('#creche_par').val()+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}			
			}
		}	
		
		//PréEscola - Municipal ou Distrital - Pública
		if($('[name=ttuid]').val() == depenPreescola && $('[name=turtipoestabelecimento]').val() == estabMunicipalDistrital && $('[name=tirid]').val() == redePublica){
			var vae = Number($('#colunamat_e').val()) + Number($('#alaquantidade_pre_5').val()); 
			var vqe = $('#coluna_e').val();
			var cap_e = (Number($('#coluna_e').val())-Number($('#colunamat_e').val()));
			var soma_pre_int = Number($('#colunamat_e').val()) + Number($('#alaquantidade_pre_5').val());

			if(cap_e >= 0){
				if(vae > vqe){
					alert("A quantidade de alunos em Pré Escola Integral é maior que a permitida! \nCapacidade permitida "+cap_e+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}

			if($('#pre_int').val() >= 0){
				if(soma_pre_int > $('#pre_int').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Integral: "+$('#pre_int').val()+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}

			var vag = Number($('#colunamat_g').val()) + Number($('#alaquantidade_pre_6').val()); 
			var vqg = $('#coluna_g').val();
			var cap_g = (Number($('#coluna_g').val())-Number($('#colunamat_g').val()));	
			var soma_pre_par = Number($('#colunamat_g').val()) + Number($('#alaquantidade_pre_6').val());		

			if(cap_g >= 0){			
				if(vag > vqg){
					alert("A quantidade de alunos em Pré Escola Parcial é maior que a permitida! \nCapacidade permitida "+cap_g+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}		
			}

			if($('#pre_par').val() >= 0){
				if(soma_pre_par > $('#pre_par').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Parcial: "+$('#pre_par').val()+" alunos!");
					alert("S");
					alert($('#pre_par').val());
					$('#alaquantidade_pre_6').focus();
					return false;
				}	
			}
		}
	
    	//PréEscola - Conveniado - Privada
    	if($('[name=ttuid]').val() == depenPreescola && $('[name=turtipoestabelecimento]').val() == estabConveniado && $('[name=tirid]').val() == redeConveniada){
			var vaf = Number($('#colunamat_f').val()) + Number($('#alaquantidade_pre_5').val()); 
			var vqf = $('#coluna_f').val();
			var cap_f = (Number($('#coluna_f').val())-Number($('#colunamat_f').val()));
			var soma_pre_int = Number($('#colunamat_f').val()) + Number($('#alaquantidade_pre_5').val());

			if(cap_f >= 0){	
	    		if(vaf > vqf){
					alert("A quantidade de alunos em Pré Escola Integral é maior que a permitida! \nCapacidade permitida "+cap_f+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}

			if($('#pre_int').val() >= 0){
				if(soma_pre_int > $('#pre_int').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Integral: "+$('#pre_int').val()+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}	
			}
    		
			var vah = Number($('#colunamat_h').val()) + Number($('#alaquantidade_pre_6').val()); 
			var vqh = $('#coluna_h').val();
			var cap_h = (Number($('#coluna_h').val())-Number($('#colunamat_h').val()));    		
			var soma_pre_par = Number($('#colunamat_h').val()) + Number($('#alaquantidade_pre_6').val());	

			if(cap_h >= 0){	
				if(vah > vqh){
					alert("A quantidade de alunos em Pré Escola Parcial é maior que a permitida! \nCapacidade permitida "+cap_h+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}	
			}

			if($('#pre_par').val() >= 0){
				if(soma_pre_par > $('#pre_par').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Parcial: "+$('#pre_par').val()+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}	
			}			
    	}	

		//CrechePréEscola - Municipal ou Distrital - Pública
		if($('[name=ttuid]').val() == depenCrechePreescola && $('[name=turtipoestabelecimento]').val() == estabMunicipalDistrital && $('[name=tirid]').val() == redePublica){
			//Valida Creche Integral
			var vaa = Number($('#colunamat_a').val()) + Number($('#alaquantidade_creche_5').val()); 
			var vqa = $('#coluna_a').val();
			var cap_a = (Number($('#coluna_a').val())-Number($('#colunamat_a').val()));
			var soma_creche_int = Number($('#colunamat_a').val()) + Number($('#alaquantidade_creche_5').val());

			if(cap_a >= 0){	
				if(vaa > vqa){
					alert("A quantidade de alunos em Creche Integral é maior que a permitida! \nCapacidade permitida "+cap_a+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}
				
			if(soma_creche_int > $('#creche_int').val()){
				alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Integral: "+$('#creche_int').val()+" alunos!");
				$('#alaquantidade_creche_5').focus();
				return false;
			}
			
			//Valida Creche Parcial	
			var vac = Number($('#colunamat_c').val()) + Number($('#alaquantidade_creche_6').val()); 
			var vqc = $('#coluna_c').val();
			var cap_c = (Number($('#coluna_c').val())-Number($('#colunamat_c').val()));	
			var soma_creche_par = Number($('#colunamat_c').val()) + Number($('#alaquantidade_creche_6').val());		

			if(cap_c >= 0){	
				if(vac > vqc){
					alert("A quantidade de alunos em Creche Parcial é maior que a permitida! \nCapacidade permitida "+cap_c+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}

			if($('#creche_par').val() >= 0){	
				if(soma_creche_par > $('#creche_par').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Parcial: "+$('#creche_par').val()+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}

			//Valida Pré-escola Integral
			var vae = Number($('#colunamat_e').val()) + Number($('#alaquantidade_pre_5').val()); 
			var vqe = $('#coluna_e').val();
			var cap_e = (Number($('#coluna_e').val())-Number($('#colunamat_e').val()));
			var soma_pre_int = Number($('#colunamat_e').val()) + Number($('#alaquantidade_pre_5').val());

			if(cap_e >= 0){	
				if(vae > vqe){
					alert("A quantidade de alunos em Pré Escola Integral é maior que a permitida! \nCapacidade permitida "+cap_e+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}

			if($('#pre_int').val() >= 0){	
				if(soma_pre_int > $('#pre_int').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Integral: "+$('#pre_int').val()+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}
			
			//Valida Pré-escola Parcial
			var vag = Number($('#colunamat_g').val()) + Number($('#alaquantidade_pre_6').val()); 
			var vqg = $('#coluna_g').val();
			var cap_g = (Number($('#coluna_g').val())-Number($('#colunamat_g').val()));		
			var soma_pre_par = Number($('#colunamat_g').val()) + Number($('#alaquantidade_pre_6').val());	

			if(cap_g >= 0){	
				if(vag > vqg){
					alert("A quantidade de alunos em Pré Escola Parcial é maior que a permitida! \nCapacidade permitida "+cap_g+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}	
			}

			if($('#pre_par').val() >= 0){	
				if(soma_pre_par > $('#pre_par').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Parcial: "+$('#pre_par').val()+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}		
			}			
		}

   		//CrechePréEscola - Conveniado - Privada
    	if($('[name=ttuid]').val() == depenCrechePreescola && $('[name=turtipoestabelecimento]').val() == estabConveniado && $('[name=tirid]').val() == redeConveniada){
			//Valida Creche Integral
			var vab = Number($('#colunamat_b').val()) + Number($('#alaquantidade_creche_5').val()); 
			var vqb = $('#coluna_b').val();
			var cap_b = (Number($('#coluna_b').val())-Number($('#colunamat_b').val()));
			var soma_creche_int = Number($('#colunamat_b').val()) + Number($('#alaquantidade_creche_5').val());
			
			if(cap_b >= 0){	
				if(vab > vqb){
					alert("A quantidade de alunos em Creche Integral é maior que a permitida! \nCapacidade permitida "+cap_b+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}

			if($('#creche_int').val() >= 0){
				if(soma_creche_int > $('#creche_int').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Integral: "+$('#creche_int').val()+" alunos!");
					$('#alaquantidade_creche_5').focus();
					return false;
				}
			}
			
			//Valida Creche Parcial
			var vad = Number($('#colunamat_d').val()) + Number($('#alaquantidade_creche_6').val()); 
			var vqd = $('#coluna_d').val();
			var cap_d = (Number($('#coluna_d').val())-Number($('#colunamat_d').val()));
			var soma_creche_par = Number($('#colunamat_d').val()) + Number($('#alaquantidade_creche_6').val());

			if(cap_d >= 0){	
				if(vad > vqd){
					alert("A quantidade de alunos em Creche Parcial é maior que a permitida! \nCapacidade permitida "+cap_d+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}

			if($('#creche_par').val() >= 0){
				if(soma_creche_par > $('#creche_par').val()){
					alert("Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Parcial: "+$('#creche_par').val()+" alunos!");
					$('#alaquantidade_creche_6').focus();
					return false;
				}
			}
			
			//Valida Pré-escola Integral
			var vaf = Number($('#colunamat_f').val()) + Number($('#alaquantidade_pre_5').val()); 
			var vqf = $('#coluna_f').val();
			var cap_f = (Number($('#coluna_f').val())-Number($('#colunamat_f').val()));
			var soma_pre_int = Number($('#colunamat_f').val()) + Number($('#alaquantidade_pre_5').val());

			if(cap_f >= 0){	
	    		if(vaf > vqf){
					alert("A quantidade de alunos em Pré Escola Integral é maior que a permitida! \nCapacidade permitida "+cap_f+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}

			if($('#pre_int').val() >= 0){
				if(soma_pre_int > $('#pre_int').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Integral: "+$('#pre_int').val()+" alunos!");
					$('#alaquantidade_pre_5').focus();
					return false;
				}
			}
			
    		//Valida Pré-escola Parcial
			var vah = Number($('#colunamat_h').val()) + Number($('#alaquantidade_pre_6').val()); 
			var vqh = $('#coluna_h').val();
			var cap_h = (Number($('#coluna_h').val())-Number($('#colunamat_h').val()));  
			var soma_pre_par = Number($('#colunamat_h').val()) + Number($('#alaquantidade_pre_6').val());  		

			if(cap_h >= 0){	
				if(vah > vqh){
					alert("A quantidade de alunos em Pré Escola Parcial é maior que a permitida! \nCapacidade permitida "+cap_h+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}
			}    

			if($('#pre_par').val() >= 0){
				if(soma_pre_par > $('#pre_par').val()){
					alert("Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Parcial: "+$('#pre_par').val()+" alunos!");
					$('#alaquantidade_pre_6').focus();
					return false;
				}	
			}
    	}

		var ano_exercicio = <?php echo $_SESSION['exercicio']; ?>;
		var ano = $('#turdtinicio').val();
    	if(ano.substr(6,12) != ano_exercicio){
			alert('Somente pode ser cadastrado turmas abertas no ano 2012!');
			$('#turdtinicio').focus();
			return false;
    	}
		
		$('[name=requisicao]').val('salvaDados');

		$('input, select, textarea').attr('disabled', false);
		
		if(erro == false){
			$('#formulario').submit();
		}
	});

	$('.cadastro_educacenso').click(function(){
		if(this.value == 'S'){
			$('#tr_cod_inep, #tr_existe_end').show();
			$('#tr_nome_escola, #tr_endereco_escola, .tr_coordenadas').hide(); 
		}else{
			$('#tr_cod_inep, #tr_existe_end').hide();
			$('#tr_cod_inep').show(); 
		}
	});

	$('.estabelecimento_autorizativo').click(function(){
		if(this.value == 'S'){
			$('#btn_salvar').attr('disabled', false);
			$('#tr_anexo').show();
		}else{
			$('#btn_salvar').attr('disabled', true);
			$('#tr_anexo').hide();
			alert('Somente escolas com ato autorizativo podem ser beneficiadas do recurso!');
		}
	});

	$('.deletarAnexo').click(function(){
		if(confirm('Deseja deletar o anexo?')){
			var arParams = this.id.split(',');
			document.location.href = 'proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&turid='+arParams[0]+'&arqid='+arParams[1]+'&escolhaTurma='+arParams[2]+'&requisicao=deletaArquivo';
		}
	});

	$('[name=ttuid]').change(function(){
		if(this.value == 1){
			$('.quantidade_alunos, .alaquantidade_creche').show();
			$('.alaquantidade_pre').hide();
		}else
		if(this.value == 2){
			$('.quantidade_alunos, .alaquantidade_pre').show();
			$('.alaquantidade_creche').hide();			
		}else
		if(this.value == 3){
			$('.quantidade_alunos, .alaquantidade_creche, .alaquantidade_pre').show();
		}else{
			$('.quantidade_alunos, .alaquantidade_creche, .alaquantidade_pre').hide();
		}
	});

	$('[name=turcepescola]').blur(function(){		
		$.getJSON('proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&requisicao=verificaCepMunicipio&cep='+this.value, function(data) {
			if(data.muncod){
				$('#endcep1').val(data.cep);
				$('#estuf1').val(data.estado);
				$('#endbai1').val(data.bairro);
				$('#mundescricao1').val(data.cidade);
				$('#muncod1').val(data.muncod);
				$('[name=turenderecoescola]').val(data.logradouro);		
			}else{
				alert('CEP não encontrado!');
				$('#endcep1').val('');
				$('#estuf1').val('');
				$('#endbai1').val('');
				$('#mundescricao1').val('');
				$('#muncod1').val('');
			}
		}); 
    });

    $('#btn_latlong').click(function(){
		if($('[name=turcepescola]').val() == ''){
			alert('Preencha o CEP primeiro!');
			$('[name=turcepescola]').focus();
			return false;
		}else{
			 abreMapaEntidade('1');
		}
    });

    $('[name=turenderecocodinep]').click(function(){
		if(this.value == 'f'){
			$('#tr_endereco_escola, .tr_coordenadas').show();			
		}else{
			$('#tr_endereco_escola, .tr_coordenadas').hide();
		}
     });

    $('[name=turtipoconselho]').click(function(){        
		if(this.value == 't'){			
			$('.campos_ato').show();
		}else{
			$('.campos_ato').hide();
		}
    });

    $('[name=turestabelecimentoautorizado]').click(function(){
		if(this.value == 'S'){
			$('#tr_emitido_conselho').show();
		}else{
			$('#tr_emitido_conselho').hide();
		}
    });
});

function abreMapaEntidade(tipoendereco){
	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var entid = null;
	var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}

</script>
<body onunload="opener.location.reload();">
<?php cabecalhoTurma(); ?>
<form method="post" name="formulario" id="formulario" action="" enctype="multipart/form-data">
	<input id="coluna_a" type="hidden" name="coluna_a" value="<?php echo $colunaA; ?>" />
	<input id="coluna_b" type="hidden" name="coluna_b" value="<?php echo $colunaB; ?>" />
	<input id="coluna_c" type="hidden" name="coluna_c" value="<?php echo $colunaC; ?>" />
	<input id="coluna_d" type="hidden" name="coluna_d" value="<?php echo $colunaD; ?>" />
	<input id="coluna_e" type="hidden" name="coluna_e" value="<?php echo $colunaE; ?>" />
	<input id="coluna_f" type="hidden" name="coluna_f" value="<?php echo $colunaF; ?>" />
	<input id="coluna_g" type="hidden" name="coluna_g" value="<?php echo $colunaG; ?>" />
	<input id="coluna_h" type="hidden" name="coluna_h" value="<?php echo $colunaH; ?>" />

	<input id="colunamat_a" type="hidden" name="colunamat_a" value="<?php echo $colunamatA; ?>" />
	<input id="colunamat_b" type="hidden" name="colunamat_b" value="<?php echo $colunamatB; ?>" />
	<input id="colunamat_c" type="hidden" name="colunamat_c" value="<?php echo $colunamatC; ?>" />
	<input id="colunamat_d" type="hidden" name="colunamat_d" value="<?php echo $colunamatD; ?>" />
	<input id="colunamat_e" type="hidden" name="colunamat_e" value="<?php echo $colunamatE; ?>" />
	<input id="colunamat_f" type="hidden" name="colunamat_f" value="<?php echo $colunamatF; ?>" />
	<input id="colunamat_g" type="hidden" name="colunamat_g" value="<?php echo $colunamatG; ?>" />
	<input id="colunamat_h" type="hidden" name="colunamat_h" value="<?php echo $colunamatH; ?>" />
	
	<input id="creche_int" type="hidden" name="creche_int" value="<?php echo $creche_int; ?>" />
	<input id="creche_par" type="hidden" name="creche_par" value="<?php echo $creche_par; ?>" />
	<input id="pre_int" type="hidden" name="pre_int" value="<?php echo $pre_int; ?>" />
	<input id="pre_par" type="hidden" name="pre_par" value="<?php echo $pre_par; ?>" />	

	<input type="hidden" name="ntmano" value="<?php echo (date('Y')-1); ?>" />
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="turid" value="<?php echo $turid;?>" />
	<input type="hidden" name="entcodent" value="<?php echo $entcodent ? $entcodent : ''?>" />
	<input type="hidden" name="muncod" id="muncod" value="<?php echo $_SESSION['proinfantil']['muncod']; ?>" />
	<input type="hidden" name="escolhaTurma" value="<?php echo $_REQUEST['escolhaTurma']; ?>" />
	
	<?php 
	$sql = "SELECT * FROM territorios.municipio WHERE muncod = '{$_SESSION['proinfantil']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	?>
	
	<input type="hidden" name="endcep1" id="endcep1" value="" />
	<input type="hidden" name="estuf1" id="estuf1" value="" />
	<input type="hidden" name="endbai1" id="endbai1" value="" />
	<input type="hidden" name="mundescricao1" id="mundescricao1" value="" />
	<input type="hidden" name="muncod1" id="muncod1" value="" />
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
	
		<tr>
			<td class="subtituloDireita">Tipo de Rede</td>
			<td>
				<?php
				$sql = "SELECT 		tirid as codigo, tirdescricao as descricao
						FROM		proinfantil.tiporede
						ORDER BY	tirid";
				
				$db->monta_combo('tirid', $sql, 'N', 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
	
		<tr>
			<td class="subtituloDireita">Tipo de atendimento da nova turma</td>
			<td>
				<?php 
				$arTipoDeoendencia = array(
					array('codigo' => 1, 'descricao' => 'Exclusivo Creche'),
					array('codigo' => 2, 'descricao' => 'Exclusivo Pré-escola'),
					array('codigo' => 3, 'descricao' => 'Creche e Pré-escola (Misto)'),
				);
				$sql = $arTipoDeoendencia;
				
				$db->monta_combo('ttuid', $sql, 'N', 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tipo do estabelecimento</td>
			<td>			
				<?php 
				$arTipoEstabelecimento = array(
					array('codigo' => 1, 'descricao' => 'Municipal ou Distrital'),
					array('codigo' => 3, 'descricao' => 'Conveniado (Comunitário, confessional ou filantrópico)'),
				);
				$sql = $arTipoEstabelecimento;
				
				$db->monta_combo('turtipoestabelecimento', $sql, 'N', 'Selecione...', '', '', '', '', 'S');
				?>		
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Nome da nova turma</td>
			<td>
				<?php echo campo_texto('turdsc', 'S', $acesso['cadastro'], '', 70, 255, '', ''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Data início do atendimento as crianças</td>
			<td>
				<?php
				echo campo_data('turdtinicio', 'S', $acesso['cadastro'], '', 'S', '', '', $turdtinicio); 
				?>				
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Estabelecimento está cadastrado no Educacenso 2011?</td>
			<td>
				<input type="radio" name="turcadeducacenso" value="S" class="cadastro_educacenso" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turcadeducacenso == 't' ? 'checked="checked"' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="turcadeducacenso" value="N" class="cadastro_educacenso" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turcadeducacenso == 'f' ? 'checked="checked"' : '' ?> />&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="tr_cod_inep" style="display:none;">
			<td class="subtituloDireita">Código INEP</td>
			<td>
				<?php echo campo_texto('codinep', 'N', $acesso['cadastro'], '', 70, '', '', ''); ?>				
			</td>
		</tr>
		<tr id="tr_existe_end" style="display:none;">
			<td class="subtituloDireita">Esta turma funciona no endereço do código INEP?</td>
			<td>				
				<input type="radio" name="turenderecocodinep" value="t" <?php echo $turenderecocodinep == 't' ? 'checked="checked"' : ''?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="turenderecocodinep" value="f" <?php echo $turenderecocodinep == 'f' ? 'checked="checked"' : ''?>/>&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr id="tr_nome_escola" style="display:none;">
			<td class="subtituloDireita">Nome da escola</td>
			<td>
				<?php echo campo_texto('turnomeescola', 'S', $acesso['cadastro'], '', 70, '', '', ''); ?>				
			</td>
		</tr>
		
		<tr class="tr_coordenadas" style="display:none;">
			<td class="subtituloDireita">CEP</td>
			<td>
				<?php $turcepescola = formata_cep($turcepescola); ?>
				<?php echo campo_texto('turcepescola', 'S', $acesso['cadastro'], '', 15, '', '##.###-###', ''); ?>				
			</td>
		</tr>
		<tr id="tr_endereco_escola" style="display:none;">
			<td class="subtituloDireita">Endereço da escola</td>
			<td>
				<?php echo campo_texto('turenderecoescola', 'S', $acesso['cadastro'], '', 70, '', '', ''); ?>				
			</td>
		</tr>
		<?php 
		$latitude  = explode('.',$turlatitude);
		$longitude = explode('.',$turlongitude);
		?>
		<tr class="tr_coordenadas" style="display:none;">
			<td class="SubTituloDireita">Latitude :</td>
			<td>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> 
					<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> - 
				<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> 
					<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
				<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> 
					<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
				<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
					<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
			</td>
		</tr>
		<tr class="tr_coordenadas" style="display:none;">
			<td class="SubTituloDireita">Longitude :</td>
			<td>
				<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> 
					<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	- 
				<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> 
					<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
				<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> 
					<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
				<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> 
					<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr class="tr_coordenadas" style="display:none;">
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<a href="#" id="btn_latlong">Visualizar / Buscar No Mapa</a> 
				<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">O estabelecimento tem ato autorizativo do respectivo sistema de ensino?</td>
			<td>
				<input type="radio" name="turestabelecimentoautorizado" value="S" class="estabelecimento_autorizativo" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turestabelecimentoautorizado == 't' ? 'checked="checked"' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="turestabelecimentoautorizado" value="N" class="estabelecimento_autorizativo" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turestabelecimentoautorizado == 'f' ? 'checked="checked"' : '' ?> />&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>		
		<tr id="tr_emitido_conselho" style="display:none;">
			<td class="subtituloDireita">
				Emitido pelo				  
			</td>
			<td>
				<input type="radio" name="turtipoconselho" value="E" <?php echo $turtipoconselho == 'E' ? 'checked="checked"' : ''?>/>&nbsp;Conselho Estadual de Educação&nbsp;
				<input type="radio" name="turtipoconselho" value="M" <?php echo $turtipoconselho == 'M' ? 'checked="checked"' : ''?>/>&nbsp;Conselho Municipal de Educação&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr class="campos_ato" style="display:none;">
			<td class="subtituloDireita">
				Nº do Ato				  
			</td>
			<td>	
				<?php echo campo_texto('turnumeroato', 'N', $acesso['cadastro'], '', 15, '', '', ''); ?>			
			</td>
		</tr>
		<tr class="campos_ato" style="display:none;">
			<td class="subtituloDireita">
				Data de Publicação				  
			</td>
			<td>	
				<?php echo campo_data('turdatapublicacaoato', 'N', $acesso['cadastro'], '', 'S', '', '', $turdatapublicacaoato); ?>	
			</td>
		</tr>
		<tr id="tr_anexo" style="display:none;">
			<td class="subtituloDireita">Anexo</td>
			<td>
				<input type="file" name="arquivo" />&nbsp;
				<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				<?php 
				if($arqid){
					$sql = "SELECT arqid, arqdescricao 
							FROM public.arquivo 
							WHERE arqid = {$arqid}";
					
					$rsAnexo = $db->pegaLinha($sql);

					if($rsAnexo){
						echo '<br/>
							<a href="javascript:void(0)" class="deletarAnexo" id="'.$turid.','.$arqid.','.$escolhaturma.'">
								<img border="0" align="absmiddle" src="../imagens/exclui_p.gif" style="cursor:pointer;" />
							</a>							
							<a href="proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&turid='.$_REQUEST['turid'].'&arqid='.$arqid.'&requisicao=downloadArquivo">
								<img border="0" src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;"/>
							</a>													
							<a id="link_anexo" href="proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&turid='.$_REQUEST['turid'].'&arqid='.$arqid.'&requisicao=downloadArquivo">
								'.$rsAnexo['arqdescricao'].'
							</a>';
					}
				}
				?>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2" class="subtituloCentro">
			<?php if($ttuid == 1 || $ttuid == 3){ ?>
			Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Integral: <?php echo $creche_int;?><br>
			Quantidade Máxima de Alunos para Creche Conveniada e Pública em Tempo Parcial: <?php echo $creche_par;?>
			<?php } ?>
			<?php if($ttuid == 2 || $ttuid == 3){ ?>
			Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Integral: <?php echo $pre_int;?><br>
			Quantidade Máxima de Alunos para Pré-Escola Conveniada e Pública em Tempo Parcial:	<?php echo $pre_par;?>		
			<?php } ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" height="100" class="quantidade_alunos" style="display:none;">
				<?php 
				$sql = "SELECT titid as codigo, titdescricao as descricao 
						FROM proinfantil.tipoturno
						WHERE titstatus = 'A' AND modid = 2
						ORDER BY titdescricao";
				
				$rsTurnos = $db->carregar($sql);
				$rsTurnos = $rsTurnos ? $rsTurnos : array();
				?>
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:85%;">					
					<tr class="subtituloEsquerda">
						<td>Críticas</td>
						<td>Tipo turno</td>						
						<td width="180" class="alaquantidade_creche" style="display:none;">Qtd. Alunos Creche</td>
						<td width="180" class="alaquantidade_creche" style="display:none;">Qtd. Professores Creche</td>
						<td width="180" class="alaquantidade_pre" style="display:none;">Qtd. Alunos Pré-Escola</td>
						<td width="180" class="alaquantidade_pre" style="display:none;">Qtd. Professores Pré-Escola</td>
					</tr>
					<?php foreach($rsTurnos as $turnos): ?>
						<?php
						$alaquantidade = '';
						if($turid){
							 
							$sql = "SELECT alaid ,alaquantidade ,alaquantidadeprofessor
									FROM proinfantil.mdsalunoatendidopbf 
									WHERE turid = {$turid} AND titid = {$turnos['codigo']} AND timid = 1";
							
							$rs_pre = $db->pegaLinha($sql);
							$alaquantidade_pre = $rs_pre['alaquantidade'];
							$alaquantidadeprofessor_pre = $rs_pre['alaquantidadeprofessor'];
							$alaid_pre = $rs_pre['alaid'];
							
							$sql = "SELECT alaid, alaquantidade, alaquantidadeprofessor
									FROM proinfantil.mdsalunoatendidopbf 
									WHERE turid = {$turid} AND titid = {$turnos['codigo']} AND timid = 3";
							
							$rs_creche = $db->pegaLinha($sql);
							$alaquantidade_creche = $rs_creche['alaquantidade'];
							$alaquantidadeprofessor_creche = $rs_creche['alaquantidadeprofessor']; 
							$alaid_creche = $rs_creche['alaid']; 
						} ?>
						<tr>
							<td>
								<font color="#FF0000">
								<?php 
								switch($ttuid){
									case 1 :
										echo ($turnos['codigo'] == 5) ? ((empty($qtd_creche5)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtd_creche5) : ((empty($qtd_creche6)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtd_creche6);
										($turnos['codigo'] == 5 && ($qtd_creche5 <= 0 || $acesso['cadastro'] == 'N')) ? $habilC5 = "N":  $habilC5 = "S";
										($turnos['codigo'] == 6 && ($qtd_creche6 <= 0 || $acesso['cadastro'] == 'N')) ? $habilC6 = "N":  $habilC6 = "S";
										($turnos['codigo'] == 5) ? $habilC = $habilC5 : $habilC = $habilC6;										
										break;
									case 2 :
										echo ($turnos['codigo'] == 5) ? ((empty($qtd_pre5)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtd_pre5) : ((empty($qtd_pre6)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtd_pre6);
										($turnos['codigo'] == 5 && ($qtd_pre5 <= 0 || $acesso['cadastro'] == 'N')) ? $habilP5 = "N":  $habilP5 = "S";
										($turnos['codigo'] == 6 && ($qtd_pre6 <= 0 || $acesso['cadastro'] == 'N')) ? $habilP6 = "N":  $habilP6 = "S";
										($turnos['codigo'] == 5) ? $habilP = $habilP5 : $habilP = $habilP6;
										break;
									case 3 :
										echo ($turnos['codigo'] == 5) ? ((empty($qtd_creche5)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtd_creche5) : ((empty($qtd_creche6)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtd_creche6);
										($turnos['codigo'] == 5 && ($qtd_creche5 <= 0 || $acesso['cadastro'] == 'N')) ? $habilC5 = "N":  $habilC5 = "S";
										($turnos['codigo'] == 6 && ($qtd_creche6 <= 0 || $acesso['cadastro'] == 'N')) ? $habilC6 = "N":  $habilC6 = "S";
										($turnos['codigo'] == 5) ? $habilC = $habilC5 : $habilC = $habilC6;												
										echo "<br>";
										echo ($turnos['codigo'] == 5) ? ((empty($qtd_pre5)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtd_pre5) : ((empty($qtd_pre6)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtd_pre6);
										($turnos['codigo'] == 5 && ($qtd_pre5 <= 0 || $acesso['cadastro'] == 'N')) ? $habilP5 = "N":  $habilP5 = "S";
										($turnos['codigo'] == 6 && ($qtd_pre6 <= 0 || $acesso['cadastro'] == 'N')) ? $habilP6 = "N":  $habilP6 = "S";
										($turnos['codigo'] == 5) ? $habilP = $habilP5 : $habilP = $habilP6;										
										break;
								}?>
								 </font>
								<input type="hidden" name="titid[]" value="<?php echo $turnos['codigo']; ?>" />
							</td>
							<td width="25%">
								<?php echo $turnos['descricao']; ?>
							</td>

							<td width="15%" class="alaquantidade_creche" style="display:none;">
								<?php echo campo_texto('alaquantidade_creche[]', 'N', $habilC, '', 20, 20, '###.###.###', '', '', '', '', 'id="alaquantidade_creche_'.$turnos['codigo'].'"', '', $alaquantidade_creche); ?>
								<input id="creche_<?php echo $turnos['codigo']; ?>"  type="hidden" name="creche[]" value="<?php echo $habilC; ?>">
							</td>
							<td width="15%" class="alaquantidade_creche" style="display:none;">
								<?php echo campo_texto('alaquantidadeprofessor_creche[]', 'N', $habilC, '', 20, 20, '###.###.###', '', '', '', '', 'id="alaquantidadeprofessor_creche_'.$turnos['codigo'].'"', '', $alaquantidadeprofessor_creche); ?>
							</td>
							<td width="15%" class="alaquantidade_pre" style="display:none;">
								<?php echo campo_texto('alaquantidade_pre[]', 'N', $habilP, '', 20, 20, '###.###.###', '', '', '', '', 'id="alaquantidade_pre_'.$turnos['codigo'].'"', '', $alaquantidade_pre); ?>
								<input id="pre_<?php echo $turnos['codigo']; ?>" type="hidden" name="pre[]" value="<?php echo $habilP; ?>">
							</td>
							<td width="15%" class="alaquantidade_pre" style="display:none;">
								<?php echo campo_texto('alaquantidadeprofessor_pre[]', 'N', $habilP, '', 20, 20, '###.###.###', '', '', '', '', 'id="alaquantidadeprofessor_pre_'.$turnos['codigo'].'"', '', $alaquantidadeprofessor_pre); ?>
							</td>
						</tr>
					<?php endforeach; ?>	
				</table>
			</td>			
		</tr>	
		<?php 
		if($acesso['cadastro'] == 'S'): ?>
			<tr>
				<td class="subtituloCentro" colspan="2">
					<input type="button" value="Salvar" id="btn_salvar" disabled="disabled"/>
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>

<?php 
if($turid){
	$_SESSION['proinfantil']['turid'] = $turid;
	$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");
	
	if($_REQUEST['requisicao']){
		$_REQUEST['requisicao']();
	}

	$habilitado = 'N';
	if($esdid == WF_NOVASTURMAS_EM_CADASTRAMENTO){
		$habilitado = 'S';
	}
	
	$sql = "SELECT 		DISTINCT sal.salid, tis.tisdescricao
			FROM		proinfantil.tiposala tis
			INNER JOIN	proinfantil.sala sal ON sal.tisid = tis.tisid
			WHERE		sal.modid = 2
			ORDER BY	tis.tisdescricao";
	
	$arrSalas = $db->carregar($sql);
	
	$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> É obrigatório incluir uma foto de cada tipo para a nova turma cadastrada.';
	monta_titulo('Fotos', $titulo2);
	?>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">				
		<?php if($arrSalas): ?>
			<?php $i=0 ?>
			<?php foreach($arrSalas as $sala): ?>
				<?php echo $i%2 == 0 ? "<tr>" : "" ?>
				<td width="50%" valign="top" id="td_sala_<?php echo $sala['salid'] ?>">
					<fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
						<legend>Fotos - <?php echo $sala['tisdescricao'] ?></legend>
						<table width="100%">
							<tr>
								<td valign="top">
									<?php 
	
									$sql = "SELECT		arq.arqid,
														arq.arqnome||'.'||arq.arqextensao,
														arq.arqdescricao,
														arq.arqtamanho,
														fot.pinid
											FROM 		proinfantil.fotos fot
											INNER JOIN	public.arquivo arq ON arq.arqid = fot.arqid
											WHERE		fot.salid = {$sala['salid']}
											AND			fot.turid = {$turid}
											AND			fot.fotstatus = 'A'";
									
									$arrFotos = $db->carregar($sql); ?>
									<?php if($arrFotos): ?>
										<?php foreach($arrFotos as $foto): ?>
											<?php $pagina = floor($n/16); ?>
											<?php
													$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
											
													if(is_file($imgend)){
														$img_max_dimX = 100;
														$img_max_dimY = 85;
														
														$imginfo = getimagesize($imgend);
														
														$width = $imginfo[0];
														$height = $imginfo[1];
													
														if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
															if ($width > $height){
															  	$w = $width * 0.9;
																  while ($w > $img_max_dimX){
																	  $w = $w * 0.9;
																  }
																  $w = round($w);
																  $h = ($w * $height)/$width;
															  }else{
																  $h = $height * 0.9;
																  while ($h > $img_max_dimY){
																	  $h = $h * 0.9;
																  }
																  $h = round($h);
																  $w = ($h * $width)/$height;
															  }
														}else{
															  $w = $width;
															  $h = $height;
														}
														
														$tamanho = " width=\"$w\" height=\"$h\" ";
													}else{
														$tamanho = "";
													}
											?>
											<div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">
												<?php if($acesso['cadastro'] == 'S'){ ?>
													<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
												<?php } ?>
												<img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $foto['pinid'] ?>', '<?php echo $_SESSION['proinfantil']['turid']; ?>')" <?php echo $tamanho ?> onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho']/1024,2) : "N/A" ?>');" class="img_foto" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?=(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]) ?>" />
											</div>
										<?php endforeach; ?>
									<?php endif; ?>
								</td>
								<?php if($acesso['cadastro'] == 'S' && !empty($rs) ){ ?>
									<td valign="top" width="10"><img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Foto" onclick="addFoto('<?php echo $sala['salid'] ?>')"  /></td>
								<?php } else { ?>
									<td valign="top" width="10"><img class="img_add_foto" src="../imagens/gif_inclui_d.gif" title="Adicionar Foto" /></td>
								<?	} ?>
							</tr>
						</table>
					</fieldset>
				</td>
				<?php echo $i%2 == 1 ? "</tr>" : "" ?>
				<?php $i++ ?>
			<?php endforeach; ?>
		<?php endif; ?>
	</table>
</body>
	<?php 
	
	$html = '<form id="form_upload" name="form_upload" method=post enctype="multipart/form-data" >
				<input type="hidden" name="salid" id="salid" value="" />
				<input type="hidden" name="foto" id="foto" value="turma" />
				<input type="hidden" name="escolhaTurma" id="escolhaTurma" value="'.$escolhaturma.'" />
				<input type="hidden" name="turid" id="turid" value="'.$_SESSION['proinfantil']['turid'].'" />
				<input type="hidden" name="requisicao" value="salvarFotosSalaNovasTurmas" />
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td width="30%" class="SubtituloDireita" >Foto:</td>
						<td><input type="file" name="arquivo" /></td>
					</tr>
					<tr>
						<td class="SubtituloDireita" >Descrição:</td>
						<td>'.campo_texto("arqdescricao","S","S","",40,60,"","","","","","","").'</td>
					</tr>
					<tr>
						<td class="SubtituloDireita" ></td>
						<td class="SubtituloEsquerda" >
							<input type="button" name="btn_upload" value="Salvar" />
							<input type="button" name="btn_cancelar" value="Cancelar" />
						</td>
					</tr>
				</table>
		   </form>'; 
				   
	popupAlertaGeral($html,"400px","120px","div_sub_foto","hidden");
	$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");	

}
?>

<script>
$(function() {
	 $('[name=btn_upload]').click(function() {
	 	var erro = 0;
	 	if(!$('#form_upload [name=arquivo]').val()){
	 		alert('Favor selecionar a foto!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(!$('#form_upload [name=arqdescricao]').val()){
	 		alert('Favor informar uma descrição!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(erro == 0){
	 		$('[name=btn_upload]').val("Carregando...");
	 		$('[name=btn_cancelar]').val("Carregando...");
	 		$('[name=btn_upload]').attr("disabled","disabled");
	 		$('[name=btn_cancelar]').attr("disabled","disabled");
	 		$('[name=form_upload]').submit();
	 	}
	 });
	 
	 $('[name=btn_cancelar]').click(function() {
	 	$('[id=div_sub_foto]').hide();
	 });

	 <?php if($_SESSION['proinfantil']['mgs']): ?>
	 	alert('<?php echo $_SESSION['proinfantil']['mgs'] ?>');
	 	<?php unset($_SESSION['proinfantil']['mgs']) ?>
	 <?php endif; ?>
});

function addFoto(salid){
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function removerFotoSala(arqid){
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicao=removerFotoSalaNovasTurmas&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
	    	}
		});
	}
}

function abrirGaleria(arqid,pagina,salid,pinid, turid){	
	window.open("../slideshow/slideshow/index.php?getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&turid="+turid,"imagem","width=850,height=600,resizable=yes");
}

</script>

<?php if($turcadeducacenso == 't'): ?>
	<script>
		$(function(){
			$('#tr_cod_inep, #tr_existe_end').show();
		});
	</script>
<?php endif; ?>
<?php if($turcadeducacenso == 'f'): ?>
	<script>
		$(function(){
			$('#tr_cod_inep').show(); 
		});
	</script>
<?php endif; ?>
<?php if($turestabelecimentoautorizado == 't'): ?>
	<script>
		$(function(){
			$('#tr_anexo, #tr_emitido_conselho').show();
			$('#btn_salvar').attr('disabled', false);
		});
	</script>
<?php endif; ?>
<?php if($ttuid): ?>
	<script>
		$(function(){
			var ttuid = <?php echo $ttuid; ?>;
			if(ttuid == 1){
				$('.quantidade_alunos, .alaquantidade_creche').show();
				$('.alaquantidade_pre').hide();
			}else
			if(ttuid == 2){
				$('.quantidade_alunos, .alaquantidade_pre').show();
				$('.alaquantidade_creche').hide();			
			}else
			if(ttuid == 3){
				$('.quantidade_alunos, .alaquantidade_creche, .alaquantidade_pre').show();
			}else{
				$('.quantidade_alunos, .alaquantidade_creche, .alaquantidade_pre').hide();
			}
		});
	</script>
<?php endif; ?>

<?php if($turenderecocodinep == 'f'): ?>
	<script>
		$(function(){			
			$('#tr_endereco_escola, .tr_coordenadas').show();
		});
	</script>
<?php endif; ?>

<?php if($turemitidoconselho == 't'): ?>
	<script>
		$(function(){
			$('.campos_ato').show();
		});
	</script>
<?php endif; ?>