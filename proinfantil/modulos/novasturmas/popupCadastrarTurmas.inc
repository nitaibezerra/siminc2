<?php
header('content-type: text/html; charset=ISO-8859-1');

if ($_REQUEST['exibirfoto']) {
	echo '<img src="/slideshow/slideshow/verimagem.php?&arqid=' . $_REQUEST['arqid'] . '" width="800" height="600"/>';
	die;
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$file = new FilesSimec();

include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once "_funcoes_novasturmas.php";
include_once APPRAIZ . "includes/workflow.php";

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 

$obNovasTurmas = new NovasTurmas( $_REQUEST );

$turid = $_REQUEST['turid'];

$arrSituacao = array(WF_NOVASTURMAS_EM_CADASTRAMENTO, WF_NOVASTURMAS_EM_DILIGENCIA);

$perfis = pegaPerfil($_SESSION['usucpf']);

if( empty($turid) ){
	echo "<script>
			alert('Turma não selecionada!'); 
		 	window.opener.location.href = window.opener.location;
		 	window.close();
		 </script>";
	exit();
}

if(!$_SESSION['proinfantil']['muncod']){
	$sql = "SELECT muncod FROM proinfantil.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND pflcod = ".EQUIPE_MUNICIPAL;
	
	$muncod = $db->pegaUm($sql);
	$muncod = $muncod ? $muncod : $_REQUEST['muncod'];
	
	if($muncod){
		$_SESSION['proinfantil']['muncod'] = $muncod;
	}
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	$codinep 	= explode("-",$_POST['codinep']);
	$entcodent 	= trim($codinep[0]);
	
	$msg = '';	
	if( empty($_POST['turdtinicio']) ){
		$msg .= '\nA data de início é obrigatório!';
	} 
	if( empty($entcodent) ){
		$msg .= '\nO campo Código INEP é obrigatório!';
	} 
	if( strlen($entcodent) < 8 ){
		$msg .= '\nCódigo INEP não pode ser menor que o 8 digitos.';
	} elseif( $_POST['turcadeducacenso'] == 'N' ){
		if( empty($_POST['turnomeescola']) ){
			$msg .= '\nO campo Nome da escola é obrigatório!';
		}
		if( empty($_POST['turcepescola']) ){
			$msg .= '\nO campo CEP da escola é obrigatório!';
		}
		if( empty($_POST['turenderecoescola']) ){
			$msg .= '\nO campo endereço da escola é obrigatório!';
		}
	} elseif( $_POST['turenderecocodinep'] == 'f' ){
		if( empty($_POST['turnometurma']) ){
			$msg .= '\nO campo Nome da turma é obrigatório!';
		}
		if( empty($_POST['turcepturma']) ){
			$msg .= '\nO campo CEP da turma é obrigatório!';
		}
		if( empty($_POST['turenderecoturma']) ){
			$msg .= '\nO campo endereço da turma é obrigatório!';
		}
	}
	if(!is_file($_FILES["arquivo"]["tmp_name"]) && empty($_POST['arqid']) ){
		$msg .= '\nO campo anexo é obrigatório!';
	}
	
	if( empty($msg) ){
		$obNovasTurmas->salvarDadosTurma($_REQUEST);
		exit();
	} else {
		echo "<script>alert('Campos obrigatório: $msg');</script>";
	}
}

if( $_REQUEST['requisicaoAjax'] == 'removerFotoSalaNovasTurmas' ){
	$obNovasTurmas->removerFotoSalaNovasTurmas();
	exit();
}

if( $_REQUEST['requisicao'] == 'salvarFotosSalaNovasTurmas' ){
	$obNovasTurmas->salvarFotosSalaNovasTurmas();
	exit();
}

if($_REQUEST['validar_inep_municipio']){
	$codigo = $db->pegaUm("select distinct substring(muncod,1,2) as codigo from territorios.municipio where muncod = '{$_SESSION['proinfantil']['muncod']}'");
	
	if( $codigo == substr($_POST['inep'], 0,2) ){
		echo 'ok';
	} else {
		echo $codigo;
	}
	
	exit();
}


//Validar Código INEP
if($_REQUEST['validar_inep']){
	$entidade = explode("-",$_REQUEST['inep']);
	$sql_inep = "SELECT entid FROM entidade.entidade WHERE entcodent IS NOT NULL AND entstatus = 'A' AND entcodent = '".trim($entidade[0])."'";
	$rs_inep = $db->pegaLinha($sql_inep);
	
	$sql_prof = "SELECT resdsc FROM questionario.resposta WHERE perid = 1589 AND trim(resdsc) = '".trim($entidade[0])."'";	
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
	$db->sucesso('novasturmas/popupCadastrarTurmas&acao=A','');	
}

//PEQUISAR ESCOLAS
if($_REQUEST['requisicao'] == 'buscaEscolas'){
	ob_clean();
	//Nova Busca de escolas na tabela proinfantil.novasturmasescolas
	$sql = "SELECT 	entcodent as codigo,entcodent || ' - ' || ntenomeescola as descricao
			FROM proinfantil.novasturmasescolas
			WHERE ntestatus = 'A'
			AND (ntenomeescola ILIKE '%{$_GET["q"]}%' OR entcodent ILIKE '%{$_GET["q"]}%')
			AND nteano = '{$_SESSION['exercicio']}'
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
	$sql = "select 
				endcep as cep,
			    endlog as logradouro,
			    estuf as estado,
			    endbai as bairro,
			    medlatitude as latitude,
			    medlongitude as logitude,
			    endzoom as zoom,
			    muncod
			from entidade.endereco where muncod = '{$_SESSION['proinfantil']['muncod']}' and endcep = '{$cep}' and endstatus = 'A'";
	$rs = $db->pegaLinha($sql);
	$rs['logradouro'] = utf8_encode($rs['logradouro']);
	echo simec_json_encode($rs);
	die();
}

$docidAtual = $obNovasTurmas->criaDocumentoNovasTurmas();
$esdid = $obNovasTurmas->pegaEstadoAtualNovasTurmas($docidAtual);

$arrTurma = $obNovasTurmas->carregarTurma();
extract($arrTurma);

$turtipoestabelecimento = $tirid == 2 ? 3 : 1;

$tirid = $tirid ? $tirid : 0;
$ttuid = $ttuid ? $ttuid : 0;

$titulo1 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Cadastro de novas turmas', $titulo1);

cabecalhoTurma($turid);

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$rsTurmasMatriculas = $obNovasTurmas->carregaDadosMatriculaMes( $arrTurma['turmes'] );
$rsTurmasMatriculas = $rsTurmasMatriculas ? $rsTurmasMatriculas : array();

$totGeralInformado = ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialpublica'] 			+ (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralpublica'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialpublica'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralpublica'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralconveniada'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralconveniada']);
						
$totGeralCenso = ((int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica'] 			+ (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica'] +
					(int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica'] 		+ (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica'] +  
					(int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada'] 		+ (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada'] +  
					(int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada']);
//ver("SELECT sum(ntaquantidade) FROM proinfantil.novasturmasalunoatendido 
//							where turid in (select turid from proinfantil.turma where muncod = '$muncod' and turano = $turano and turmes = $turmes and turid not in ($turid))",$totGeralInformado, (int)$totGeralCenso);
$totalGeralMatricula = ((int)$totGeralInformado - (int)$totGeralCenso);

$totAluno = $db->pegaUm("SELECT sum(ntaquantidade) FROM proinfantil.novasturmasalunoatendido 
							where turid in (select turid from proinfantil.turma where turstatus = 'A' and muncod = '$muncod' and turano = $turano and turmes = $turmes and turid not in ($turid))");
$totalGeralMatricula = ((int)$totalGeralMatricula - (int)$totAluno);

//if( $ttuid == 3 ){ #Unificada
	#pega os alunos ja inseridos em creche e preescola
	$sql = "SELECT coalesce(SUM(jainseridacrecheintegral), 0) AS jainseridacrecheintegral, 
			      coalesce(SUM(jainseridacrecheparcial), 0)  AS jainseridacrecheparcial , 
			      coalesce(SUM(jainseridapreescolaintegral), 0) AS jainseridapreescolaintegral, 
			      coalesce(SUM(jainseridapreescolaparcial), 0) AS jainseridapreescolaparcial
			FROM (
			                SELECT
			                               -- QUANTIDADE EM CRECHE INTEGRAL JÁ INSERIDA
			                               CASE WHEN n.tatid = 1 AND n.timid = 6  
			                               THEN  SUM(n.ntaquantidade) END AS jainseridacrecheintegral,
			                               -- QUANTIDADE EM CRECHE PARCIAL JÁ INSERIDA
			                               CASE WHEN n.tatid = 2 AND n.timid = 6
			                               THEN  SUM(n.ntaquantidade) END AS jainseridacrecheparcial,
			                               -- QUANTIDADE EM PRE-ESCOLA INTEGRAL JÁ INSERIDA
			                               CASE WHEN n.tatid = 1 AND n.timid = 7
			                               THEN  SUM(n.ntaquantidade) END AS jainseridapreescolaintegral,
			                               -- QUANTIDADE EM PRE-ESCOLA PARCIAL JÁ INSERIDA
			                               CASE WHEN n.tatid = 2 AND n.timid = 7
			                               THEN  SUM(n.ntaquantidade) END AS jainseridapreescolaparcial
			                FROM proinfantil.turma t
			                INNER JOIN proinfantil.novasturmasalunoatendido n ON n.turid = t.turid
			                WHERE muncod = '$muncod'
			                	and t.turano = '{$turano}'
                                                and turstatus = 'A'
	                            and t.turmes = '{$turmes}'
	                            and t.turid not in ($turid)
			GROUP BY n.tatid, n.timid
			) AS foo
			";
                                  //  ver($sql,d);
	$arrAlunoAten = $db->pegaLinha($sql);
//}

$qtdCrecheIntegral = (((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralpublica'] + (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralconveniada']) - 
						((int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica'] + (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada'] )) - (int)$arrAlunoAten['jainseridacrecheintegral'];
//ver($qtdCrecheIntegral,d);						
$qtdCrecheParcial = (((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialpublica'] + (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialconveniada']) - 
						((int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica'] + (int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada'] )) - (int)$arrAlunoAten['jainseridacrecheparcial'];
						
$qtdPreEscolaIntegral = (((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralpublica'] + (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralconveniada']) - 
						((int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica'] + (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada'] )) - (int)$arrAlunoAten['jainseridapreescolaintegral'];
						
$qtdPreEscolaParcial = (((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialpublica'] + (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialconveniada']) - 
						((int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica'] + (int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada'] )) - (int)$arrAlunoAten['jainseridapreescolaparcial'];
					

$qtdCreche_Integral_Publica 		= ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralpublica'] 		- (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica']);
$qtdCreche_Integral_Conveniada 		= ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralconveniada'] 	- (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada']);

$qtdCreche_Parcial_Publica 			= ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialpublica'] 		- (int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica']);
$qtdCreche_Parcial_Conveniada 		= ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialconveniada'] 		- (int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada']);

$qtdPreescola_Parcial_Publica 		= ((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialpublica'] 		- (int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica']);
$qtdPreescola_Parcial_Conveniada 	= ((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialconveniada']	- (int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada']);

$qtdPreescola_Integral_Publica 		= ((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralpublica'] 	- (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica']);
$qtdPreescola_Integral_Conveniada 	= ((int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralconveniada'] 	- (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada']);

$qtdCreche_Parcial_Publica = ( ((int)$qtdCreche_Parcial_Publica < 0) ? 0 : $qtdCreche_Parcial_Publica);
$qtdCreche_Integral_Publica = ( ((int)$qtdCreche_Integral_Publica < 0) ? 0 : $qtdCreche_Integral_Publica);
$qtdPreescola_Parcial_Publica = ( ((int)$qtdPreescola_Parcial_Publica < 0) ? 0 : $qtdPreescola_Parcial_Publica);
$qtdPreescola_Integral_Publica = ( ((int)$qtdPreescola_Integral_Publica < 0) ? 0 : $qtdPreescola_Integral_Publica);
$qtdCreche_Parcial_Conveniada = ( ((int)$qtdCreche_Parcial_Conveniada < 0) ? 0 : $qtdCreche_Parcial_Conveniada);
$qtdCreche_Integral_Conveniada = ( ((int)$qtdCreche_Integral_Conveniada < 0) ? 0 : $qtdCreche_Integral_Conveniada);
$qtdPreescola_Parcial_Conveniada = ( ((int)$qtdPreescola_Parcial_Conveniada < 0) ? 0 : $qtdPreescola_Parcial_Conveniada);
$qtdPreescola_Integral_Conveniada = ( ((int)$qtdPreescola_Integral_Conveniada < 0) ? 0 : $qtdPreescola_Integral_Conveniada);

if( (int)$qtdCrecheIntegral < 0 || (int)$qtdCrecheParcial < 0){
	if( (int)$qtdCrecheIntegral > 0 ){
		$qtdCrecheIntegral = (int)$qtdCrecheIntegral + (int)$qtdCrecheParcial; //Coloquei + em vez de - porque o valor vem negativo exemplo -3
	} else {
		$qtdCrecheParcial = (int)$qtdCrecheParcial + (int)$qtdCrecheIntegral; //Coloquei + em vez de - porque o valor vem negativo exemplo -3
	}
}

if( (int)$qtdPreEscolaIntegral < 0 || (int)$qtdPreEscolaParcial < 0){
	if( (int)$qtdPreEscolaIntegral > 0 ){
		$qtdPreEscolaIntegral = (int)$qtdPreEscolaIntegral + (int)$qtdPreEscolaParcial; //Coloquei + em vez de - porque o valor vem negativo exemplo -3
	} else {
		$qtdPreEscolaParcial = (int)$qtdPreEscolaParcial + (int)$qtdPreEscolaIntegral; //Coloquei + em vez de - porque o valor vem negativo exemplo -3
	}
}

$qtdCrecheIntegral = ((int)$qtdCrecheIntegral < 0? 0 : (int)$qtdCrecheIntegral);
$qtdCrecheParcial = ((int)$qtdCrecheParcial < 0? 0 : (int)$qtdCrecheParcial);
$qtdPreEscolaIntegral = ((int)$qtdPreEscolaIntegral < 0? 0 : (int)$qtdPreEscolaIntegral);
$qtdPreEscolaParcial = ((int)$qtdPreEscolaParcial < 0? 0 : (int)$qtdPreEscolaParcial);

/*
echo '<pre>';
echo 'totGeralInformado: '.$totGeralInformado.'<br>';
echo 'totGeralCenso: '.$totGeralCenso.'<br>';
echo '$qtdIntegral: '.$qtdIntegral.'<br>';
echo '$$qtdParcial: '.$qtdParcial.'<br>';
echo 'totalGeralMatricula: '.$totalGeralMatricula.'<br>';
echo '$qtdCrecheIntegral: '.$qtdCrecheIntegral.'<br>';
echo '$qtdCrecheParcial: '.$qtdCrecheParcial.'<br>';
echo '$qtdPreEscolaIntegral: '.$qtdPreEscolaIntegral.'<br>';
echo '$qtdPreEscolaParcial: '.$qtdPreEscolaParcial.'<br>';
echo '<br>';*/
/*
echo 'qtdCreche_Parcial_Publica: '.$qtdCreche_Parcial_Publica.'<br>';
echo 'qtdCreche_Parcial_Conveniada: '.$qtdCreche_Parcial_Conveniada.'<br><br>';

echo 'qtdCreche_Integral_Publica: '.$qtdCreche_Integral_Publica.'<br>';
echo 'qtdCreche_Integral_Conveniada: '.$qtdCreche_Integral_Conveniada.'<br><br>';

echo 'qtdPreescola_Parcial_Publica: '.$qtdPreescola_Parcial_Publica.'<br>';
echo 'qtdPreescola_Parcial_Conveniada: '.$qtdPreescola_Parcial_Conveniada.'<br><br>';

echo 'qtdPreescola_Integral_Publica: '.$qtdPreescola_Integral_Publica.'<br>';
echo 'qtdPreescola_Integral_Conveniada: '.$qtdPreescola_Integral_Conveniada.'<br>';*/
//echo '</pre>';

if( $esdid != WF_NOVASTURMAS_EM_DILIGENCIA ){
	verificaPermissaoAcesso();
	$urlBlq = substr($_SESSION['favurl'], 0, strpos($_SESSION['favurl'], '&'));
	$arrAcesso = $_SESSION['proinfantil']['acesso'][$urlBlq];
	$arrAcesso = ($arrAcesso ? $arrAcesso : array());
	$arBloqueio = array();
	foreach ($arrAcesso as $key => $value) {
		if($value == 'N'){
			array_push($arBloqueio, $key);
		}
	}
	$arBloqueio = '"'.implode('", "',$arBloqueio).'"';
}

$arAnatipo = $db->pegaLinha("select a.anatipo, anaparecer from proinfantil.analisenovasturmasaprovacao a where turid = $turid and anastatus = 'A'");
?>
<html>
<head>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>


<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script type="text/javascript">
	$jq	 = $.noConflict(); 
</script>


<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<!-- Add Thumbnail helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

<!-- Add Media helper (this is optional) -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>



<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
	
	.tabela{
		width: 100%
	}
	#calendarDiv{
		z-index: 1 !important;
	}
</style>

</head>
<body>

<form method="post" name="formulario" id="formulario" action="" enctype="multipart/form-data">
<table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
		<tr>
			<td>	
		<input type="hidden" id="qtdCreche_Parcial_Publica" 		name="qtdCreche_Parcial_Publica" 		value="<?php echo $qtdCreche_Parcial_Publica; ?>" />
		<input type="hidden" id="qtdCreche_Integral_Publica" 		name="qtdCreche_Integral_Publica" 		value="<?php echo $qtdCreche_Integral_Publica; ?>" />
		<input type="hidden" id="qtdPreescola_Parcial_Publica" 		name="qtdPreescola_Parcial_Publica" 	value="<?php echo $qtdPreescola_Parcial_Publica; ?>" />
		<input type="hidden" id="qtdPreescola_Integral_Publica" 	name="qtdPreescola_Integral_Publica" 	value="<?php echo $qtdPreescola_Integral_Publica; ?>" />
		<input type="hidden" id="qtdCreche_Parcial_Conveniada" 		name="qtdCreche_Parcial_Conveniada" 	value="<?php echo $qtdCreche_Parcial_Conveniada; ?>" />
		<input type="hidden" id="qtdCreche_Integral_Conveniada" 	name="qtdCreche_Integral_Conveniada" 	value="<?php echo $qtdCreche_Integral_Conveniada; ?>" />
		<input type="hidden" id="qtdPreescola_Parcial_Conveniada" 	name="qtdPreescola_Parcial_Conveniada" 	value="<?php echo $qtdPreescola_Parcial_Conveniada; ?>" />
		<input type="hidden" id="qtdPreescola_Integral_Conveniada" 	name="qtdPreescola_Integral_Conveniada" value="<?php echo $qtdPreescola_Integral_Conveniada; ?>" />
		<input type="hidden" id="totalGeralMatricula" 				name="totalGeralMatricula" 				value="<?php echo $totalGeralMatricula; ?>" />
		
		<input type="hidden" id="qtdCrecheIntegral" 				name="qtdCrecheIntegral" 				value="<?php echo $qtdCrecheIntegral; ?>" />
		<input type="hidden" id="qtdCrecheParcial" 					name="qtdCrecheParcial" 				value="<?php echo $qtdCrecheParcial; ?>" />
		<input type="hidden" id="qtdPreEscolaIntegral" 				name="qtdPreEscolaIntegral" 			value="<?php echo $qtdPreEscolaIntegral; ?>" />
		<input type="hidden" id="qtdPreEscolaParcial" 				name="qtdPreEscolaParcial" 				value="<?php echo $qtdPreEscolaParcial; ?>" />
	
		<input type="hidden" name="ntmano" 		value="<?php echo $_SESSION['exercicio']; ?>" />
		<input type="hidden" name="requisicao" 	value="" />
		<input type="hidden" name="turid" 		value="<?php echo $_REQUEST['turid'];?>" />
		<input type="hidden" name="entcodent" 	value="<?php echo $entcodent ? $entcodent : ''?>" />
		<input type="hidden" name="muncod" 	id="muncod" value="<?php echo $_REQUEST['muncod']; ?>" />
		<input type="hidden" name="esdid" 	id="esdid" value="<?php echo $esdid; ?>" />
		<input type="hidden" name="turmes" 	id="turmes" value="<?php echo $turmes; ?>" />
		<input type="hidden" name="docid" 	id="docid" value="<?php echo $docidAtual; ?>" />
		<input type="hidden" name="anatipo" id="anatipo" value="<?php echo $arAnatipo['anatipo']; ?>" />
			
		<input type="hidden" name="turdtinclusao" id="turdtinclusao" value="<?php echo formata_data($turdtinclusao); ?>" />	
	
		
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
				<?				
				if( $esdid == WF_NOVASTURMAS_EM_DILIGENCIA || $arAnatipo['anatipo'] == 'D' ){
					$sql = "select 
								ana.anaid, 
							    ana.anaparecer,
							    dil.diltexto
							from proinfantil.analisenovasturmasaprovacao ana
								left join proinfantil.novasturmasdiligencia dil on dil.anaid = ana.anaid and dilstatus = 'A'
							where ana.turid = {$turid}
								and ana.anastatus = 'A'
							order by dil.dildata desc";
					
					$arrParecer = $db->pegaLinha($sql);
					$diltexto = $arrParecer['diltexto'];
					?>
					<tr>
						<th colspan="2" style="font-size: 14px; color: blue; text-align: justify;">Após salvar a resposta da diligência é necessário retornar a aba "Cadastrar Turmas" e clicar no botão "retornar turmas de diligência para análise" selecionando as turmas que deverão ser analisadas novamente.</th>
					</tr>
					<tr>
						<td width="20%" class="subtitulodireita"><b>Parecer de Diligência:</b></td>
						<td style="color: red; "><?php echo $arrParecer['anaparecer']; ?>
							<input type="hidden" name="anaid" id="anaid" value="<?=$arrParecer['anaid'] ?>">
						</td>
					</tr>
					<tr>
						<td width="20%" class="subtitulodireita"><b>Resposta da Diligência:</b></td>
						<td><?=campo_textarea('diltexto', 'S', 'S', 'Resposta da Diligência', 110, 8, 4000, '', '', '', '', 'Resposta da Diligência');?></td>
					</tr>
				<?} else if($arAnatipo['anatipo'] == 'A'){ 
					$sql = "select 
								ntuapareceraprovacao,
								to_char(ntuadata, 'DD/MM/YYYY HH24:MI:SS') as data
							from proinfantil.novasturmasanaliseaprovacao
							where turid = {$turid}
							order by ntuadata desc";
					
					$arrParecer = $db->pegaLinha($sql);
					?>
					<tr>
						<th width="70%"><b>Parecer de Aprovação:</b></th>
						<th><b>Data</b></th>
					</tr>
					<tr>
						<td><?php echo $arrParecer['ntuapareceraprovacao']; ?></td>
						<td><?=$arrParecer['data']; ?></td>
					</tr>
				<?} else if( $esdid == WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO || $arAnatipo['anatipo'] == 'I' ){?>
					<tr>
						<th><b>Parecer de Indeferido:</b></th>
					</tr>
					<tr>
						<td style="color: red; ">
							<?php							
							echo $arAnatipo['anaparecer'];
							?>
						</td>
					</tr>
				<?} ?>
				</table>
			</td>
		</tr>
		<tr>
			<td>
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
				<tr>
					<th colspan="2">Informações sobre a turma</th>
				</tr>
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
						$sql = "SELECT
									ttuid as codigo,
								  	ttudsc as descricao
								FROM 
								  	proinfantil.tipoturma WHERE ttustatus = 'A'";
						
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
                        <?php $acesso['cadastro'] = isset($acesso['cadastro']) ? $acesso['cadastro'] : ''; ?>
						<?php echo campo_texto('turdsc', 'S', $acesso['cadastro'], '', 70, 255, '', ''); ?>
					</td>
				</tr>		
				<tr>
					<td class="subtituloDireita">Data início do atendimento as crianças</td>
					<td>
						<?php
						if( $esdid == WF_NOVASTURMAS_EM_DILIGENCIA ) $habilita = 'S';
						echo campo_data2('turdtinicio', 'S', ($habilita ? $habilita : $acesso['cadastro']), '', 'S', '', '', $turdtinicio, '', '', 'turdtinicio'); 
						?>				
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Estabelecimento está cadastrado no Educacenso <?=((int)$_SESSION['exercicio'] - 1)?></td>
					<td>
						<input type="radio" name="turcadeducacenso" id="turcadeducacenso_s" value="S" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turcadeducacenso == 't' ? 'checked="checked"' : '' ?>/>&nbsp;Sim&nbsp;
						<input type="radio" name="turcadeducacenso" id="turcadeducacenso_n" value="N" <?php echo $acesso['cadastro'] == 'N' ? 'disabled="disabled"' : ''; ?> <?php echo $turcadeducacenso == 'f' ? 'checked="checked"' : '' ?> />&nbsp;Não&nbsp;
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Código INEP</td>
					<td>
						<?php 
						$codinep = $entcodent;
						echo campo_texto('codinep', 'S', $acesso['cadastro'], '', 70, '8', '[#]', ''); ?>				
					</td>
				</tr>
				<tr class="dadosescola">
					<td class="subtituloDireita">Dados da Escola:</td>
					<td>
						<table class="tabela" align="left" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width: 70%">
							<tr>
								<th colspan="2">Dados da Escola</th>
							</tr>
							<tr>
								<td class="subtituloDireita" width="10%"><b>Nome:</b></td>
								<td>
									<?php echo campo_texto('turnomeescola', 'S', $acesso['cadastro'], '', 65, '', '', ''); ?>				
								</td>
							</tr>
							
							<tr>
								<td class="subtituloDireita"><b>CEP:</b></td>
								<td>
									<?php $turcepescola = formata_cep($turcepescola); ?>
									<?php echo campo_texto('turcepescola', 'S', $acesso['cadastro'], '', 15, '', '##.###-###', ''); ?>				
								</td>
							</tr>
							<tr>
								<td class="subtituloDireita"><b>Endereço:</b></td>
								<td>
									<?php echo campo_texto('turenderecoescola', 'S', $acesso['cadastro'], '', 65, '', '', ''); ?>				
								</td>
							</tr>
							<?php 
							$latitude  = explode('.',$turlatitude);
							$longitude = explode('.',$turlongitude);
							?>
							<tr>
								<td class="SubTituloDireita"><b>Latitude:</b></td>
								<td>
									<input name="turlatitude" id="turlatitude" value="<? echo $turlatitude; ?>" class="normal" type="hidden"> 
										<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> - 
										<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
										<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
										<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita"><b>Longitude:</b></td>
								<td>
									<input name="turlongitude" id="turlongitude" maxlength="2" size="3" value="<? echo $turlongitude; ?>" type="hidden"> 
										<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	- 
										<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
										<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
										<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
									<input type="hidden" name="turendzoom" id="turendzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">&nbsp;</td>
								<td>
									<a href="#" id="btn_latlong">Visualizar / Buscar No Mapa</a> 
									<input name="endereco[1][endzoom]" id="endzoom1" value="" type="hidden">
								</td>
							</tr>
						</table>			
					</td>
				</tr>				
				<tr>
					<td class="subtituloDireita">Esta turma funciona no endereço do código INEP?</td>
					<td>				
						<input type="radio" name="turenderecocodinep" value="t" <?php echo ($turenderecocodinep == 't' || empty($turenderecocodinep) ) ? 'checked="checked"' : ''?>/>&nbsp;Sim&nbsp;
						<input type="radio" name="turenderecocodinep" value="f" <?php echo $turenderecocodinep == 'f' ? 'checked="checked"' : ''?>/>&nbsp;Não&nbsp;
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr class="dadosturma">
					<td class="subtituloDireita">Dados da Turma:</td>
					<td>
						<table class="tabela" align="left" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width: 70%">
							<tr>
								<th colspan="2">Dados da Turma</th>
							</tr>
							<tr>
								<td class="subtituloDireita" width="10%"><b>Nome:</b></td>
								<td>
									<?php echo campo_texto('turnometurma', 'S', $acesso['cadastro'], '', 65, '', '', ''); ?>				
								</td>
							</tr>
							
							<tr>
								<td class="subtituloDireita"><b>CEP:</b></td>
								<td>
									<?php $turcepturma = formata_cep($turcepturma); ?>
									<?php echo campo_texto('turcepturma', 'S', $acesso['cadastro'], '', 15, '', '##.###-###', ''); ?>				
								</td>
							</tr>
							<tr>
								<td class="subtituloDireita"><b>Endereço:</b></td>
								<td>
									<?php echo campo_texto('turenderecoturma', 'S', $acesso['cadastro'], '', 65, '', '', ''); ?>				
								</td>
							</tr>
							<?php 
							$latitude  = explode('.',$turlatitudeturma);
							$longitude = explode('.',$turlongitudeturma);
							?>
							<tr>
								<td class="SubTituloDireita"><b>Latitude:</b></td>
								<td>
									<input name="turlatitudeturma" id="turlatitudeturma" value="<? echo $turlatitudeturma; ?>" class="normal" type="hidden"> 
									<span id="_graulatitude1_turma"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> - 
									<span id="_minlatitude1_turma"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
									<span id="_seglatitude1_turma"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
									<span id="_pololatitude1_turma"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita"><b>Longitude:</b></td>
								<td>
									<input name="turlongitudeturma" id="turlongitudeturma" value="<? echo $turlongitudeturma; ?>" type="hidden"> 
										<span id="_graulongitude1_turma"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	- 
										<span id="_minlongitude1_turma"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  ' 
										<span id="_seglongitude1_turma"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> " 
										<span id="_pololongitude1_turma"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
										<input type="hidden" name="turendzoomturma" id="turendzoomturma" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">&nbsp;</td>
								<td>
									<a href="#" id="btn_latlong_turma">Visualizar / Buscar No Mapa</a> 
									<input name="endereco[1][endzoom]" id="endzoom1" value="" type="hidden">
								</td>
							</tr>
						</table>			
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
						<?php echo campo_data2('turdatapublicacaoato', 'N', $acesso['cadastro'], '', 'S', '', '', $turdatapublicacaoato); ?>	
					</td>
				</tr>
				<tr id="tr_anexo" style="display:none;">
					<td class="subtituloDireita">Anexo</td>
					<td>
						<input type="hidden" name="arqid" value="<?php echo $arqid?>" />
					<?
					$deletaArquivo = '';
					if( in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )  ){ ?>
						<input type="file" name="arquivo" />&nbsp;
						<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
						<?php 
						$deletaArquivo = '<a href="javascript:void(0)" class="deletarAnexo" id="'.$turid.','.$arqid.'">
											<img border="0" align="absmiddle" src="../imagens/exclui_p.gif" style="cursor:pointer;" />
										</a>';
					}
						if($arqid){
							$sql = "SELECT arqid, arqdescricao FROM public.arquivo WHERE arqid = {$arqid}";					
							$rsAnexo = $db->pegaLinha($sql);
		
							if($rsAnexo){
								echo '<br/>
									'.$deletaArquivo.'						
									<a href="proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='.$_REQUEST['turid'].'&arqid='.$arqid.'&requisicao=downloadArquivo">
										<img border="0" src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;"/>
									</a>													
									<a id="link_anexo" href="proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='.$_REQUEST['turid'].'&arqid='.$arqid.'&requisicao=downloadArquivo">
										'.$rsAnexo['arqdescricao'].'
									</a>';
							}
						}
						?>
					</td>
				</tr>
				<tr>
					<td colspan="2" height="100" class="quantidade_alunos" style="display:none;">
						<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:90%;">
							<tr>
								<td align="center" colspan="6" class="subtituloCentro">
									Quantidade Máxima de Alunos Permitidos para o Mês <?=mes_extenso($turmes); ?>/<?=$turano; ?>: <span style="color: blue;"><?php echo (int)$totalGeralMatricula;?></span><br>
								</td>
							</tr>					
							<tr>
								<th width="30%">Críticas</th>
								<th width="20%">Tipo turno</th>						
								<th width="10%" class="ntaquantidade_creche" style="display:none;">Qtd. Alunos Creche</th>
								<th width="10%" class="ntaquantidade_creche" style="display:none;">Qtd. Professores Creche</th>
								<th width="10%" class="ntaquantidade_pre" style="display:none;">Qtd. Alunos Pré-Escola</th>
								<th width="10%" class="ntaquantidade_pre" style="display:none;">Qtd. Professores Pré-Escola</th>
							</tr>
							<?php 
							$sql = "select t.tatid as codigo, t.tatdsc as descricao from
										proinfantil.tipoatendimentoturma t
									where
										t.tatstatus = 'A'
									ORDER BY t.tatdsc";
							
							$rsTurnos = $db->carregar($sql);
							$rsTurnos = $rsTurnos ? $rsTurnos : array();
							
							foreach($rsTurnos as $turnos){ ?>
							<?php
							$ntaquantidade = '';
							if($turid){						 
								$sql = "SELECT ntaid, ntaquantidade, ntaquantidadeprofessor FROM proinfantil.novasturmasalunoatendido 
										WHERE turid = {$turid}
										AND tatid = {$turnos['codigo']} AND timid = 7";
								
								$rs_pre = $db->pegaLinha($sql);
								
								$ntaquantidade_pre = $rs_pre['ntaquantidade'] ? $rs_pre['ntaquantidade'] : '0';
								$ntaquantidadeprofessor_pre = $rs_pre['ntaquantidadeprofessor'] ? $rs_pre['ntaquantidadeprofessor'] : '0';
								
								$sql = "SELECT ntaid, ntaquantidade, ntaquantidadeprofessor FROM proinfantil.novasturmasalunoatendido 
										WHERE turid = {$turid}
										AND tatid = {$turnos['codigo']} AND timid = 6";
								$rs_creche = $db->pegaLinha($sql);
								
								$ntaquantidade_creche = $rs_creche['ntaquantidade'] ? $rs_creche['ntaquantidade'] : '0';
								$ntaquantidadeprofessor_creche = $rs_creche['ntaquantidadeprofessor'] ? $rs_creche['ntaquantidadeprofessor'] : '0';
							} ?>
							<tr>
								<td>
									<font color="#FF0000">
									<?php 
									/*
									 * Quantidade máxima de alunos permitida em Creche Integral
									 * Quantidade máxima de alunos permitida em Creche Parcial
									 * Quantidade máxima de alunos permitida em Pré-escola Integral
									 * Quantidade máxima de alunos permitida em Pré-escola Parcial
									 */
									
									switch($ttuid){
										case 1 :
											echo ($turnos['codigo'] == 1) ? ((empty($qtdCrecheIntegral)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtdCrecheIntegral) : ((empty($qtdCrecheParcial)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtdCrecheParcial);
											($turnos['codigo'] == 1 && ($qtdCrecheIntegral <= 0 || $acesso['cadastro'] == 'N')) ? $habilC1 = "N":  $habilC1 = "S";
											($turnos['codigo'] == 2 && ($qtdCrecheParcial <= 0 || $acesso['cadastro'] == 'N')) ? $habilC2 = "N":  $habilC2 = "S";
											($turnos['codigo'] == 1) ? $habilC = $habilC1 : $habilC = $habilC2;										
											break;
										case 2 :
											echo ($turnos['codigo'] == 1) ? ((empty($qtdPreEscolaIntegral)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtdPreEscolaIntegral) : ((empty($qtdPreEscolaParcial)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtdPreEscolaParcial);
											($turnos['codigo'] == 1 && ($qtdPreEscolaIntegral <= 0 || $acesso['cadastro'] == 'N')) ? $habilP1 = "N":  $habilP1 = "S";
											($turnos['codigo'] == 2 && ($qtdPreEscolaParcial <= 0 || $acesso['cadastro'] == 'N')) ? $habilP2 = "N":  $habilP2 = "S";
											($turnos['codigo'] == 1) ? $habilP = $habilP1 : $habilP = $habilP2;
											break;
										case 3 :
											echo ($turnos['codigo'] == 1) ? ((empty($qtdCrecheIntegral)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtdCrecheIntegral) : ((empty($qtdCrecheParcial)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtdCrecheParcial);
											($turnos['codigo'] == 1 && ($qtdCrecheIntegral <= 0 || $acesso['cadastro'] == 'N')) ? $habilC1 = "N":  $habilC1 = "S";
											($turnos['codigo'] == 2 && ($qtdCrecheParcial <= 0 || $acesso['cadastro'] == 'N')) ? $habilC2 = "N":  $habilC2 = "S";
											($turnos['codigo'] == 1) ? $habilC = $habilC1 : $habilC = $habilC2;												
											echo "<br>";
											echo ($turnos['codigo'] == 1) ? ((empty($qtdPreEscolaIntegral)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtdPreEscolaIntegral) : ((empty($qtdPreEscolaParcial)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtdPreEscolaParcial);
											($turnos['codigo'] == 1 && ($qtdPreEscolaIntegral <= 0 || $acesso['cadastro'] == 'N')) ? $habilP1 = "N":  $habilP1 = "S";
											($turnos['codigo'] == 2 && ($qtdPreEscolaParcial <= 0 || $acesso['cadastro'] == 'N')) ? $habilP2 = "N":  $habilP2 = "S";
											($turnos['codigo'] == 1) ? $habilP = $habilP1 : $habilP = $habilP2;										
											break;
									}?>
									 </font>
									<input type="hidden" name="tatid[]" value="<?php echo $turnos['codigo']; ?>" />
								</td>
								<td width="25%">
									<?php echo 'Matrículas na nova turma - Tempo '.$turnos['descricao']; ?>
								</td>
		
								<td width="15%" class="ntaquantidade_creche" style="display:none;">
									<?php echo campo_texto('ntaquantidade_creche[]', 'S', $habilC, '', 20, 20, '###.###.###', '', '', '', '', 'id="ntaquantidade_creche_'.$turnos['codigo'].'"', '', $ntaquantidade_creche); ?>
									<input id="creche_<?php echo $turnos['codigo']; ?>"  type="hidden" name="creche[]" value="<?php echo $habilC; ?>">
								</td>
								<td width="15%" class="ntaquantidade_creche" style="display:none;">
									<?php echo campo_texto('ntaquantidadeprofessor_creche[]', 'S', $habilC, '', 20, 20, '###.###.###', '', '', '', '', 'id="ntaquantidadeprofessor_creche_'.$turnos['codigo'].'"', '', $ntaquantidadeprofessor_creche); ?>
								</td>
								<td width="15%" class="ntaquantidade_pre" style="display:none;">
									<?php echo campo_texto('ntaquantidade_pre[]', 'S', $habilP, '', 20, 20, '###.###.###', '', '', '', '', 'id="ntaquantidade_pre_'.$turnos['codigo'].'"', '', $ntaquantidade_pre); ?>
									<input id="pre_<?php echo $turnos['codigo']; ?>" type="hidden" name="pre[]" value="<?php echo $habilP; ?>">
								</td>
								<td width="15%" class="ntaquantidade_pre" style="display:none;">
									<?php echo campo_texto('ntaquantidadeprofessor_pre[]', 'S', $habilP, '', 20, 20, '###.###.###', '', '', '', '', 'id="ntaquantidadeprofessor_pre_'.$turnos['codigo'].'"', '', $ntaquantidadeprofessor_pre); ?>
								</td>
							</tr>
					<?php } ?>	
						</table>
					</td>			
				</tr>	
				<tr bgcolor="#D0D0D0">
					<td colspan="2" style="text-align: center">
				<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )) && in_array($esdid, $arrSituacao) && ($_SESSION['exercicio'] == date('Y') || $esdid == WF_NOVASTURMAS_EM_DILIGENCIA) ){ ?>
						<input type="button" value="Salvar" id="btn_salvar" />						
				<?php } ?>
						<input type="button" name="btnFechar" value="Fechar" onclick="javascript: fechar();">
					</td>
				</tr>
			</table>
	</td>
		<tr>
			<td>
	<?php 
	if($turid){
		//$_SESSION['proinfantil']['turid'] = $turid;
		$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");
		
		/*if($_REQUEST['requisicao']){
			$_REQUEST['requisicao']();
		}*/
	
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
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';
		print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Fotos</label></td></tr><tr>';
		print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$titulo2.'</td></tr></table>';
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
																arq.arqnome||'.'||arq.arqextensao as arquivo,
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
														<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )) && in_array($esdid, $arrSituacao) && ($_SESSION['exercicio'] == date('Y') || $esdid == WF_NOVASTURMAS_EM_DILIGENCIA)  ){ ?>
															<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
														<?php } else { ?>
															<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
														<?} 
														/*														
														<img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $foto['pinid'] ?>', '<?php echo $turid; ?>')" <?php echo $tamanho ?> 
															 onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho']/1024,2) : "N/A" ?>');" 
															 class="img_foto" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?=(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]) ?>" />
														*/
														
														$img = "../slideshow/slideshow/verimagem.php?arqid=".$foto['arqid']."&newwidth=100&newheight=85&_sisarquivo=".(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]);
														$link = "proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid=".$_REQUEST['turid']."&exibirfoto=1&arqid=".$foto['arqid'];
														?>													
														<a id="fancybox_img_<?php echo $foto['arqid'] ?>" class="fancybox_img fancybox.ajax" href="<?php echo $link; ?>" data-fancybox-group="gallery" title="<?php echo $foto['arquivo'] ?>">
															<img <?php echo $tamanho ?> class="img_foto" src="<?php echo $img; ?>" />
                                        				</a>
														
													</div>
												<?php endforeach; ?>
											<?php endif; ?>
										</td>
										<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )) && in_array($esdid, $arrSituacao) && ($_SESSION['exercicio'] == date('Y') || $esdid == WF_NOVASTURMAS_EM_DILIGENCIA)  ){ ?>
											<td valign="top" width="10"><img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Foto" onclick="addFoto('<?php echo $sala['salid'] ?>')"  /></td>
										<?php } else { ?>
											<td valign="top" width="10"><img class="img_add_foto" src="../imagens/gif_inclui_d.gif" title="Adicionar Foto" /></td>
										<?} ?>
									</tr>
								</table>
							</fieldset>
						</td>
						<?php echo $i%2 == 1 ? "</tr>" : "" ?>
						<?php $i++ ?>
					<?php endforeach; ?>
				<?php endif; ?>
			</table>
		</td>
		</tr>
	</table>
	</td>
	<td valign="top">
	<?
	if( $_SESSION['usucpforigem'] == '' ){
		$arrOcultar = array('acoes' => false);
	} else {
		$arrOcultar = array('acoes' => true);
	}
	/*if( ($esdid == WF_NOVASTURMAS_EM_VALIDACAO_DE_ANALISE && in_array( PERFIL_ADMINISTRADOR, $perfis ) ) 
			|| in_array( PERFIL_SUPER_USUARIO, $perfis ) ){
				$arrOcultar = array('acoes' => false);
	}*/
	wf_desenhaBarraNavegacao( $docidAtual , array( 'turid' => $_REQUEST['turid']), $arrOcultar );
	?>
	</td>
	</tr>
</table>
</form>
</body>
	<?php 
	
	$html = '<form id="form_upload" name="form_upload" method=post enctype="multipart/form-data" >
				<input type="hidden" name="salid" id="salid" value="" />
				<input type="hidden" name="foto" id="foto" value="turma" />
				<input type="hidden" name="turid" id="turid" value="'.$turid.'" />
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
<script type="text/javascript">
this._closeWindows = false;
var _modificado    = false;

var btnSalvar = false;

function addFoto(salid){
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function removerFotoSala(arqid){
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicaoAjax=removerFotoSalaNovasTurmas&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
	    	}
		});
	}
}

function abrirGaleria(arqid,pagina,salid,pinid, turid){	
	window.open("../slideshow/slideshow/index.php?getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&turid="+turid,"imagem","width=850,height=600,resizable=yes");
}

function fechar(){
	window.opener.location.href = window.opener.location;
	window.close();
}

function validaDadosAlunos(tipo, label){
	
	if( $('.ntaquantidade_'+tipo).css('display') != 'none'  ){
		if( $('#ntaquantidade_'+tipo+'_1').val() == '0' && $('#ntaquantidade_'+tipo+'_2').val() == '0' ){
			if( $('#ntaquantidade_'+tipo+'_1').attr('readonly') == false ){
				alert('O campo Qtd. Alunos '+label+' Integral é obrigatório!');
				$('#ntaquantidade_'+tipo+'_1').focus();
				return false;
			} else {
				alert('O campo Qtd. Alunos '+label+' Parcial é obrigatório!');
				$('#ntaquantidade_'+tipo+'_2').focus();
				return false;
			}
		}
		
		if( $('#ntaquantidade_'+tipo+'_1').attr('readonly') == false ){
			if( parseInt($('#ntaquantidade_'+tipo+'_1').val()) > 0 && $('#ntaquantidadeprofessor_'+tipo+'_1').val() == '0' ){
				alert('O campo Qtd. Professores '+label+' Integral é obrigatório!');
				$('#ntaquantidadeprofessor_'+tipo+'_1').focus();
				return false;
			}
			
			if( parseInt($('#ntaquantidade_'+tipo+'_1').val()) < parseInt($('#ntaquantidadeprofessor_'+tipo+'_1').val()) ){
				alert('Número de Professores '+label+' Integral não pode ser maior que Qtd. Alunos '+label+' Integral!');
				$('#ntaquantidadeprofessor_'+tipo+'_1').focus();
				return false;
			}
			
			if( label == 'Pré-escola' ) var label1 = label.replace('Pré-escola', 'PreEscola');
			else label1 = label; 

			if( parseInt( $('#ntaquantidade_'+tipo+'_1').val() ) > parseInt( $('#qtd'+label1+'Integral').val() ) ){
				alert("A quantidade de alunos em "+label+" Integral é maior que a permitida! \nCapacidade permitida "+$('#qtd'+label1+'Integral').val()+" alunos!");
				$('#ntaquantidade_'+tipo+'_1').focus();
				return false;
			}
			if( parseInt( $('#ntaquantidade_'+tipo+'_1').val() ) > parseInt($('#totalGeralMatricula').val()) ){
				alert("A quantidade de alunos em "+label+" Integral é maior que a permitida! \nQuantidade Máxima de Alunos Permitidos para o Mês MARÇO/2015: "+$('#totalGeralMatricula').val()+" alunos!");
				$('#ntaquantidade_'+tipo+'_1').focus();
				return false;
			}
		}
		
		if( $('#ntaquantidade_'+tipo+'_2').attr('readonly') == false ){
			if( parseInt($('#ntaquantidade_'+tipo+'_2').val()) > 0 && $('#ntaquantidadeprofessor_'+tipo+'_2').val() == '0' ){
				alert('O campo Qtd. Professores '+label+' Parcial é obrigatório!');
				$('#ntaquantidadeprofessor_'+tipo+'_2').focus();
				return false;
			}
			
			if( parseInt($('#ntaquantidade_'+tipo+'_2').val()) < parseInt($('#ntaquantidadeprofessor_'+tipo+'_2').val()) ){
				alert('Número de Professores '+label+' Parcial não pode ser maior que Qtd. Alunos '+label+' Parcial!');
				$('#ntaquantidadeprofessor_'+tipo+'_2').focus();
				return false;
			}
			
			if( label == 'Pré-escola' ) var label2 = label.replace('Pré-escola', 'PreEscola');
			else label2 = label; 

			if( parseInt( $('#ntaquantidade_'+tipo+'_2').val() ) > parseInt( $('#qtd'+label2+'Parcial').val() ) ){
				alert("A quantidade de alunos em "+label+" Parcial é maior que a permitida! \nCapacidade permitida "+$('#qtd'+label2+'Parcial').val()+" alunos!");
				$('#ntaquantidade_'+tipo+'_2').focus();
				return false;
			}
			if( parseInt( $('#ntaquantidade_'+tipo+'_2').val() ) > parseInt($('#totalGeralMatricula').val()) ){
				alert("A quantidade de alunos em "+label+" Parcial é maior que a permitida! \nQuantidade Máxima de Alunos Permitidos para o Mês MARÇO/2015: "+$('#totalGeralMatricula').val()+" alunos!");
				$('#ntaquantidade_'+tipo+'_2').focus();
				return false;
			}
		}
	}
	return true;
}

function validaDataMaiorHoje(data)  {
	hoje = new Date();
	dia = hoje.getDate();
	if (dia < 10) dia = "0"+dia;
	
	mes = hoje.getMonth()+1;
	if (mes < 10) mes = "0"+mes;
	
	ano=hoje.getFullYear();
	diadehoje = ano + '' + mes +''+ dia;
	data = data.value.substr(6,4) + data.value.substr(3,2) + data.value.substr(0,2);
	if (data > diadehoje)
		return(true);
	else return(false);
}

$(document).ready(function() {
	$jq('.fancybox_img').fancybox({
		openEffect	: 'elastic',
    	closeEffect	: 'elastic',
    	helpers : {
    		title : {
    			type : 'inside'
    		},
			thumbs	: {
				width	: 100,
				height	: 50
			}
    	}
	});

	window.onload = function( event ){
		$(window).focus();
	}

	window.onbeforeunload = function( event ){
		// captura o botao clicado para fechar
		if( event.explicitOriginalTarget.URL != 'undefined' ){
			window.opener.location.href = window.opener.location;
		}
	}
	
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
	 
	$('[name=codinep]').autocomplete("proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&requisicao=buscaEscolas&muncod="+$('[name=muncod]').val()+'&turid='+$('[name=muncod]').val(), {
	  	cacheLength:10,
		width: 440,		
		scrollHeight: 220,
		selectFirst: true,
		autoFill: true,
		zIndex: 1
	}).result(function(event, data, formatted) {
		$('[name=entcodent]').val(data[1]);
	});

	$('[name=codinep]').change(function(){		
		if(this.value == ''){
			$('[name=entcodent]').val('');
		}
	});
	
	$('[name=codinep]').blur(function(){		
		$.ajax({
			type: "POST",
			url: 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='+$('#turid').val(),
			data: { validar_inep_municipio: "true", inep: $('[name=codinep]').val()},
			async: false,
			success: function(retornoajax) {
				
				if(retornoajax != 'ok'){
					alert("O código do INEP inválido para o Município.");
					$('[name=codinep]').val('');
				} 
  			}
  		});
	});
	
	$('[name="ntaquantidade_creche[]"], [name="ntaquantidadeprofessor_creche[]"], [name="ntaquantidade_pre[]"], [name="ntaquantidadeprofessor_pre[]"]').focusin(function(){
		if( parseInt( $(this).val() ) == '0' ){
			$(this).val('');
		}
	});
	
	$('[name="ntaquantidade_creche[]"], [name="ntaquantidadeprofessor_creche[]"], [name="ntaquantidade_pre[]"], [name="ntaquantidadeprofessor_pre[]"]').focusout(function(){
		if( $(this).val() == '' ){
			$(this).val('0');
		}
	});

	$('#btn_salvar').click(function(){
		var erro = false;
		var nome = $('[name=turdsc]').val();
		var dataInicio = $('[name=turdtinicio]').val();
		var entcodent = $('[name=entcodent]').val();
		var anexo = $('#link_anexo').html() ? $('#link_anexo').html() : $('[name=arquivo]').val();
		
		/*var retornoC = validaDadosAlunos('creche', 'Creche');
		if( !retornoC ){
			return false;
		}
		
		var retornoP = validaDadosAlunos('pre', 'Pré-escola');		
		if( !retornoP ){
			return false;
		}*/
		if($('[name=anatipo]').val() == 'D'){
			if($('[name=diltexto]').val() == ''){
				alert('O campo Resposta da Diligência é obrigatório!');
				$('[name=anatipo]').focus();
				return false;
			}
		}
		
		if(nome == ''){
			alert('O campo nome é obrigatório!');
			$('[name=turnome]').focus();
			return false;
		}

		if(dataInicio == ''){
			alert('A data de início é obrigatório!');
			$('[name=turdtinicio]').focus();
			return false;
		}	

		if($('[name=ttuid]').val() == ''){
			alert('O campo tipo de atendimento é obrigatório!');
			$('[name=ttuid]').focus();
			return false;
		}		
		
		if( validaDataMaiorHoje( document.getElementById('turdtinicio') ) ){
			alert('Não pode iniciar turmas com data maior que a data atual!');
			$('#turdtinicio').focus();
			return false;
    	}
    	
    	if( !validaDataMaior( document.getElementById('turdtinicio'), document.getElementById('turdtinclusao') ) ){
			alert("A data de início do atendimento não pode ser maior que data de cadastro da turma.");
			$('#turdtinicio').focus();
			return false;
		}    	
		
		if($('[name=codinep]').val() == ''){
			alert('O campo Código INEP é obrigatório!');
			$('[name=codinep]').focus();
			return false;
		}
		if( $('[name=codinep]').val().length < 8 ){
			alert('Código INEP não pode ser menor que o 8 digitos.');
			$('[name=codinep]').focus();
			return false;
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

			if($('[name=codinep]').val() != ''){
				var inep = $('[name=codinep]').val();
				var turma = "<?php echo $turid; ?>";
				$.ajax({
					type: "POST",
					url: 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='+turma,
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
		}
		
		if(erro == true){
			return false;
		}
		
		if($('[name=turcadeducacenso]:checked').val() == 'N'){
			
			if($('[name=turnomeescola]').val() == ''){
				alert('O campo Nome da escola é obrigatório!');
				$('[name=turnomeescola]').focus();
				return false;
			}
			if($('[name=turcepescola]').val() == ''){
				alert('O campo CEP da escola é obrigatório!');
				$('[name=turcepescola]').focus();
				return false;
			}
			if($('[name=turenderecoescola]').val() == ''){
				alert('O campo endereço da escola é obrigatório!');
				$('[name=turenderecoescola]').focus();
				return false;
			}
		}
		
		if($('[name=turenderecocodinep]:checked').val() == 'f'){
			if($('[name=turnometurma]').val() == ''){
				alert('O campo Nome da turma é obrigatório!');
				$('[name=turnometurma]').focus();
				return false;
			}
			if($('[name=turcepturma]').val() == ''){
				alert('O campo CEP da turma é obrigatório!');
				$('[name=turcepturma]').focus();
				return false;
			}
			if($('[name=turenderecoturma]').val() == ''){
				alert('O campo endereço da turma é obrigatório!');
				$('[name=turenderecoturma]').focus();
				return false;
			}
		}
		
		if(anexo == ''){
			alert('O campo anexo é obrigatório!');
			$('[name=arquivo]').focus();
			return false;
		}
		
		var retornoC = validaDadosAlunos('creche', 'Creche');
		var retornoP = validaDadosAlunos('pre', 'Pré-escola');
		//console.log(retornoC+' - '+retornoP);
		if( !retornoC ){
			return false;
		}
		if( !retornoP ){
			return false;
		}
		
		$('[name=requisicao]').val('salvaDados');

		$('input, select, textarea').attr('disabled', false);
		
		$('#formulario').submit();
	});

	$('[name=turcadeducacenso]').click(function(){
		if( $(this).val() == 'S' ){
			$('.dadosescola').hide();
		} else {
			$('.dadosescola').show();
		}
	});
	
	if($('[name=turcadeducacenso]:checked').val() == 'S'){
		$('.dadosescola').hide();
	} else {
		$('.dadosescola').show();		
	}
	
	if($('[name=turenderecocodinep]:checked').val() == 't'){
		$('.dadosturma').hide();
	} else {
		$('.dadosturma').show();		
	}

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
			document.location.href = 'proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&turid='+arParams[0]+'&arqid='+arParams[1]+'&requisicao=deletaArquivo';
		}
	});

	$('[name=ttuid]').change(function(){
		if(this.value == 1){
			$('.quantidade_alunos, .ntaquantidade_creche').show();
			$('.ntaquantidade_pre').hide();
		}else
		if(this.value == 2){
			$('.quantidade_alunos, .ntaquantidade_pre').show();
			$('.ntaquantidade_creche').hide();			
		}else
		if(this.value == 3){
			$('.quantidade_alunos, .ntaquantidade_creche, .ntaquantidade_pre').show();
		}else{
			$('.quantidade_alunos, .ntaquantidade_creche, .ntaquantidade_pre').hide();
		}
	});

	$('[name=turcepescola]').blur(function(){
		if( $('[name=turcepescola]').val() != '' ){
			$.getJSON('proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&requisicao=verificaCepMunicipio&cep='+this.value+'&turid='+$('#turid').val(), function(data) {
				
				if(data.muncod){
					$('[name=turenderecoescola]').val(data.logradouro);	
					var latitude = data.latitude.split('.');
					$('#_graulatitude1').html(latitude[0]);
					$('#_minlatitude1').html(latitude[1]);
					$('#_seglatitude1').html(latitude[2]);
					$('#_pololatitude1').html(latitude[3]);
					
					var logitude = data.logitude.split('.');
					$('#_graulongitude1').html(logitude[0]);
					$('#_minlongitude1').html(logitude[1]);
					$('#_seglongitude1').html(logitude[2]);
					$('#_pololongitude1').html(logitude[3]);
						
					$('#turlongitude').val(data.logitude);
					$('#turlatitude').val(data.latitude);
					
					//turendzoomturma
				}else{
					alert('CEP não foi encontrado no município da turma!');
					$('[name=turcepescola]').val('');
					$('[name=turenderecoturma]').val('');	
					
					$('#_graulatitude1').html('XX');
					$('#_minlatitude1').html('XX');
					$('#_seglatitude1').html('XX');
					$('#_pololatitude1').html('X');
					
					$('#_graulongitude1').html('XX');
					$('#_minlongitude1').html('XX');
					$('#_seglongitude1').html('XX');
					$('#_pololongitude1').html('X');
						
					$('#turlongitude').val('');
					$('#turlatitude').val('');
				}
			});
		}
    });
    
    $('[name=turcepturma]').blur(function(){
		if( $('[name=turcepturma]').val() != '' ){
			$.getJSON('proinfantil.php?modulo=novasturmas/popupCadastrarTurmas&acao=A&requisicao=verificaCepMunicipio&cep='+this.value+'&turid='+$('#turid').val(), function(data) {
				
				if(data.muncod){
					$('[name=turenderecoturma]').val(data.logradouro);	
					var latitude = data.latitude.split('.');
					$('#_graulatitude1_turma').html(latitude[0]);
					$('#_minlatitude1_turma').html(latitude[1]);
					$('#_seglatitude1_turma').html(latitude[2]);
					$('#_pololatitude1_turma').html(latitude[3]);
					
					var logitude = data.logitude.split('.');
					$('#_graulongitude1_turma').html(logitude[0]);
					$('#_minlongitude1_turma').html(logitude[1]);
					$('#_seglongitude1_turma').html(logitude[2]);
					$('#_pololongitude1_turma').html(logitude[3]);
						
					$('#turlongitudeturma').val(data.logitude);
					$('#turlatitudeturma').val(data.latitude);
					
					//turendzoomturma
				}else{
					alert('CEP não foi encontrado no município da turma!');
					$('[name=turcepturma]').val('');
					$('[name=turenderecoturma]').val('');	
					
					$('#_graulatitude1_turma').html('XX');
					$('#_minlatitude1_turma').html('XX');
					$('#_seglatitude1_turma').html('XX');
					$('#_pololatitude1_turma').html('X');
					
					$('#_graulongitude1_turma').html('XX');
					$('#_minlongitude1_turma').html('XX');
					$('#_seglongitude1_turma').html('XX');
					$('#_pololongitude1_turma').html('X');
						
					$('#turlongitudeturma').val('');
					$('#turlatitudeturma').val('');
				}
			});
		}
    });

    $('#btn_latlong').click(function(){
		if($('[name=turcepescola]').val() == ''){
			alert('Preencha o CEP primeiro!');
			$('[name=turcepescola]').focus();
			return false;
		}else{
			 abreMapaEntidade('1', 'escola');
		}
    });
    
    $('#btn_latlong_turma').click(function(){
		if($('[name=turcepturma]').val() == ''){
			alert('Preencha o CEP primeiro!');
			$('[name=turcepturma]').focus();
			return false;
		}else{
			 abreMapaEntidade('1', 'turma');
		}
    });

    $('[name=turenderecocodinep]').click(function(){
		if(this.value == 'f'){
			$('.dadosturma').show();
			//$('.dadosescola').show();			
		}else{
			//$('.dadosescola').hide();
			$('.dadosturma').hide();
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
    
    var arBloqueio = new Array(<?=$arBloqueio ?>);
	
	for(var i=0; i<arBloqueio.length; i++){
		switch (arBloqueio[i]) {
			case ('select'):
				$('#formulario').find('select').attr('disabled', 'disabled');
				break;
			case ('textarea'):
				$('#formulario').find('textarea').attr('disabled', 'disabled');
				break;
			default:
				if( arBloqueio[i] == 'button' ){
					btnSalvar = true;
				}
				$('#formulario').find('input[type='+arBloqueio[i]+']').attr('disabled', 'disabled');
				break;
		}
	}
});

function abreMapaEntidade(tipoendereco, tipo){
	if( tipo == 'escola' ){
		var turlatitude = $('[name=turlatitude]').val();
		var arLatitude = turlatitude.split('.');
		
		var turlongitude = $('[name=turlongitude]').val();
		var arLongitude = turlongitude.split('.');
	} else {
		var turlatitudeturma = $('[name=turlatitudeturma]').val();
		var arLatitude = turlatitudeturma.split('.');
		
		var turlongitudeturma = $('[name=turlongitudeturma]').val();
		var arLongitude = turlongitudeturma.split('.');
	}
	
	var graulatitude = arLatitude[0];
	var minlatitude  = arLatitude[1];
	var seglatitude  = arLatitude[2];
	var pololatitude = arLatitude[3];
	
	var graulongitude = arLongitude[0];
	var minlongitude  = arLongitude[1];
	var seglongitude  = arLongitude[2];
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var entid = null;
	var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}
	
</script>


<?php if($ttuid): ?>
	<script>
	$(document).ready(function() {
			var ttuid = <?php echo $ttuid; ?>;
			
			if(ttuid == 1){
				$('.quantidade_alunos, .ntaquantidade_creche').show();
				$('.ntaquantidade_pre').hide();
			}else
			if(ttuid == 2){
				$('.quantidade_alunos, .ntaquantidade_pre').show();
				$('.ntaquantidade_creche').hide();			
			}else
			if(ttuid == 3){
				$('.quantidade_alunos, .ntaquantidade_creche, .ntaquantidade_pre').show();
			}else{
				$('.quantidade_alunos, .ntaquantidade_creche, .ntaquantidade_pre').hide();
			}
		});
	</script>
<?php endif; ?>

<?php if($turestabelecimentoautorizado == 't'): ?>
	<script>
	$(document).ready(function() {
			$('#tr_anexo, #tr_emitido_conselho').show();
			if( !btnSalvar )
			 	$('#btn_salvar').attr('disabled', false);
		});
	</script>
<?php endif; ?>

<?php if($turemitidoconselho == 't'): ?>
	<script>
	$(document).ready(function() {
			$('.campos_ato').show();
		});
	</script>
<?php endif; ?>
</html>