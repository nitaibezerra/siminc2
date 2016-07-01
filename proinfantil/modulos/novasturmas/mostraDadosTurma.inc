<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once APPRAIZ . 'includes/workflow.php';
include  "_funcoes_novasturmas.php";

if ($_REQUEST['exibirfoto']) {
	echo '<img src="/slideshow/slideshow/verimagem.php?&arqid=' . $_REQUEST['arqid'] . '" width="800" height="600"/>';
	die;
}

$file = new FilesSimec();

//DOWNLOAD ARQUIVO
if($_REQUEST['requisicao'] == 'downloadArquivo'){
	if(!$file->getDownloadArquivo($_REQUEST['arqid'])){
		echo "<script>
				window.close(); 
			</script>";
	}
	exit();
}

$perfis = pegaPerfil($_SESSION['usucpf']);

$_REQUEST['muncod'] = ( $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['proinfantil']['muncod'] ); 
$_REQUEST['turid'] = $_REQUEST['turma'];

$obNovasTurmas = new NovasTurmas( $_REQUEST );

$turid = $_REQUEST['turid'];

$docid = $obNovasTurmas->criaDocumentoNovasTurmas();
$esdid = $obNovasTurmas->pegaEstadoAtualNovasTurmas($docid);

//Recupera Turmas do Município
$sql = "SELECT
				    tre.tirdescricao, 
				    tur.ttuid,
				    CASE WHEN tur.ttuid = 1 THEN 'Exclusivo Creche'
						 WHEN tur.ttuid = 2 THEN 'Exclusivo Pré-escola'
						 WHEN tur.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
				    END AS tipodependencia,  
				    CASE WHEN tur.turtipoestabelecimento = 1 THEN 'Municipal ou Distrital'
						 WHEN tur.turtipoestabelecimento = 3 THEN 'Conveniado (Comunitário, confessional ou filantrópico)'
				    END AS tipoestabelecimento,  				    
				    tur.turdsc,
				    to_char(tur.turdtinicio,'DD/MM/YYYY') AS turdtinicio,
				    CASE WHEN tur.turcadeducacenso = 't' THEN 'Sim'
						 WHEN tur.turcadeducacenso = 'f' THEN 'Não'
				    END AS educacenso,  				    
				    CASE WHEN tur.turestabelecimentoautorizado = 't' THEN 'Sim'
						 WHEN tur.turestabelecimentoautorizado = 'f' THEN 'Não'
				    END AS estabelecimentoautorizado,  					    
					ent.entcodent || ' - ' || ent.entnome as codinep,
			    	CASE WHEN tur.turtipoconselho = 'E' THEN 'Conselho Estadual de Educação'
						 WHEN tur.turtipoconselho = 'M' THEN 'Conselho Municipal de Educação'
				    END AS tipoconselho,  								
				    tur.turid,
					tur.entcodent,
				    CASE WHEN tur.turenderecocodinep = 't' THEN 'Sim'
						 WHEN tur.turenderecocodinep = 'f' THEN 'Não'
					 END AS enderecocodinep, 					
					tur.turnomeescola,
					tur.arqid,
					tur.turenderecoescola,
					tur.turtipoestabelecimento,
					tur.turlatitude, 
					tur.turlongitude,
					tur.turcepescola,					
					tur.turnumeroato,
					tur.turdatapublicacaoato,
					arq.arqdescricao,	
					tur.turnometurma,
					tur.turcepturma,
					tur.turenderecoturma,
					tur.turlatitudeturma,
					tur.turlongitudeturma,
					turmes,
					turano,
					muncod
					
		FROM 		proinfantil.turma tur
		INNER JOIN 	proinfantil.tiporede tre ON tre.tirid = tur.tirid
		left JOIN  public.arquivo arq ON arq.arqid = tur.arqid
		LEFT JOIN   entidade.entidade ent ON trim(ent.entcodent) = trim(tur.entcodent)
		WHERE muncod = '{$_SESSION['proinfantil']['muncod']}'
		AND turstatus = 'A'
		AND turid = {$_REQUEST['turma']}";

$rs_turma = $db->pegaLinha($sql);
$rs_turma = $rs_turma ? $rs_turma : array();
$nome_turma = "Turma: ".$rs_turma['turdsc'];

$arrPost = array(
				'muncod' => $rs_turma['muncod'],
				'ntmmmes' => $rs_turma['turmes'],
				'turid' => $rs_turma['turid'],
			);

$obNovasTurmas = new NovasTurmas( $arrPost );
extract($rs_turma);
$muncod = $_SESSION['proinfantil']['muncod'];

$rsTurmasMatriculas = $obNovasTurmas->carregaDadosMatriculaMes( $rs_turma['turmes'] );
$rsTurmasMatriculas = $rsTurmasMatriculas ? $rsTurmasMatriculas : array();

$totGeralInformado = ((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialpublica'] 			+ (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralpublica'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialpublica'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralpublica'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralconveniada'] +
						(int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntmmqtdmatriculapreescolaintegralconveniada']);
						
$totGeralCenso = ((int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialpublica'] 			+ (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica'] +
					(int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialpublica'] 		+ (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralpublica'] +  
					(int)$rsTurmasMatriculas['ntcqtdalunocrecheparcialconveniada'] 		+ (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada'] +  
					(int)$rsTurmasMatriculas['ntcqtdalunopreescolaparcialconveniada'] 	+ (int)$rsTurmasMatriculas['ntcqtdalunopreescolaintegralconveniada']);
					
$totalGeralMatricula = ((int)$totGeralInformado - (int)$totGeralCenso);

$totAluno = $db->pegaUm("SELECT sum(ntaquantidade) FROM proinfantil.novasturmasalunoatendido 
							where turid in (select turid from proinfantil.turma 
												where muncod = '{$_SESSION['proinfantil']['muncod']}' 
													and turano = {$rs_turma['turano']} 
													and turmes = {$rs_turma['turmes']} 
													and turid not in ({$rs_turma['turid']})
											)");
//ver($totGeralInformado, $totGeralCenso, $totalGeralMatricula, $totAluno);
$totalGeralMatricula = ((int)$totalGeralMatricula - (int)$totAluno);

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
	                            and t.turmes = '{$turmes}'
	                            and t.turid not in ($turid)
			GROUP BY n.tatid, n.timid
			) AS foo
			";
	$arrAlunoAten = $db->pegaLinha($sql);
//}

$qtdCrecheIntegral = (((int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralpublica'] + (int)$rsTurmasMatriculas['ntmmqtdmatriculacrecheintegralconveniada']) - 
						((int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralpublica'] + (int)$rsTurmasMatriculas['ntcqtdalunocrecheintegralconveniada'] )) - (int)$arrAlunoAten['jainseridacrecheintegral'];
						
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

monta_titulo($nome_turma, '');

$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");
?>
<html>
<head>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

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

<script type="text/javascript">
$(document).ready(function(){
	/*$('.fancybox_img').each(function(){
		var id = $(this).attr('id');*/
		
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
	//});
	
	$('.dadosescola').hide();
	$('.dadosturma').hide();
	
	if( $('[name="educacenso"]').val() == 'Não' ){
		$('.dadosescola').show();
	}
	
	if( $('[name="enderecocodinep"]').val() == 'Não' ){
		$('.dadosturma').show();
	}
	
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
    
    var ttuid = $('[name="ttuid"]').val();
			
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

function abrirGaleria(arqid, pagina, salid, turid){	
	window.open("../slideshow/slideshow/index.php?getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&turid="+turid,"imagem","width=850,height=600,resizable=yes");
}
</script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>


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
<?php 

$arAnatipo = $db->pegaLinha("select a.anatipo, anaparecer from proinfantil.analisenovasturmasaprovacao a where turid = $turid");

cabecalhoTurma($turid); ?>
<form id="formulario" name="formulario">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
 <?php
 if( $esdid == WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO_SISTEMA ){ ?>
	<tr>
            <th><b>Mensagem</b></th>
	</tr>
	<tr>
            <td style="color: red; text-align: center; font-weight: bold "><?php
			$sql = "select distinct
					    cd.cmddsc
					from workflow.historicodocumento hd
					    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
					    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
					    inner join workflow.comentariodocumento cd on cd.hstid = hd.hstid
					    inner join proinfantil.novasturmasworkflowturma nw on nw.docid = hd.docid
					where
					    nw.turid = $turid
					    and ac.esdiddestino = ".WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO_SISTEMA;
			
			$Historico = $db->pegaUm($sql);
			if( empty($Historico) ){
				echo 'Prazo de diligência expirado. Turma encaminhada para Indeferido automaticamente via sistema';
			} else {
				echo $Historico;
			}
            ?></td>
	</tr>
<?}else if( $esdid == WF_NOVASTURMAS_EM_DILIGENCIA || $arAnatipo['anatipo'] == 'D' ){ ?>
	<tr>
		<th width="50%"><b>Parecer de Diligência</b></th>
		<th width="50%"><b>Resposta da Diligência</b></th>
	</tr>
	<tr>
		<td style="color: red; ">
			<?php
			$sql = "select 
						ana.anaid, 
					    ana.anaparecer,
					    dil.diltexto
					from proinfantil.analisenovasturmasaprovacao ana
						left join proinfantil.novasturmasdiligencia dil on dil.anaid = ana.anaid and dilstatus = 'A'
					where ana.turid = {$turid}
					order by ana.anaid desc";
			
			$arrParecer = $db->pegaLinha($sql);
			
			echo $arrParecer['anaparecer'];
			?>
		</td>
		<td style="color: blue; "><? echo ($arrParecer['diltexto'] ? $arrParecer['diltexto'] : '-'); ?></td>
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
		<th width="70%"><b>Parecer de Aprovação</b></th>
		<th><b>Data</b></th>
	</tr>
	<tr>
		<td><?php echo $arrParecer['ntuapareceraprovacao']; ?></td>
		<td><?=$arrParecer['data']; ?></td>
	</tr>
<?} else if( $esdid == WF_NOVASTURMAS_EM_INDEFERIDO_ARQUIVADO || $arAnatipo['anatipo'] == 'I' ){?>
	<tr>
		<th><b>Parecer de Indeferido</b></th>
	</tr>
	<tr>
		<td style="color: red; "><?php echo $arAnatipo['anaparecer'];?></td>
	</tr>
<?} ?>
</table>
<table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td>
	<table border="0" class="tabela" style="width: 100%" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="30%">Tipo de Rede</td>
			<td><?php echo $rs_turma['tirdescricao']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tipo de atendimento da nova turma</td>
			<td><?php echo $rs_turma['tipodependencia']; ?>
				<input type="hidden" name="ttuid" id="ttuid" value="<?=$rs_turma['ttuid'] ?>">
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tipo do estabelecimento</td>
			<td><?php echo $rs_turma['tipoestabelecimento']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">Nome da nova turma</td>
			<td><?php echo $rs_turma['turdsc']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data início do atendimento as crianças</td>
			<td><?php echo $rs_turma['turdtinicio']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Estabelecimento está cadastrado no Educacenso?</td>
			<td><?php echo $rs_turma['educacenso']; ?>
				<input type="hidden" name="educacenso" id="educacenso" value="<?=$rs_turma['educacenso']; ?>">
				</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Código INEP</td>
			<td><?php echo $rs_turma['codinep'] ? $rs_turma['codinep'] : $rs_turma['entcodent']; ?></td>
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
							<?php echo $rs_turma['turnomeescola']; ?>				
						</td>
					</tr>
					
					<tr>
						<td class="subtituloDireita"><b>CEP:</b></td>
						<td>
							<?php $turcepescola = formata_cep($rs_turma['turcepescola']); ?>
							<?php echo $turcepescola; ?>				
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"><b>Endereço:</b></td>
						<td>
							<?php echo $rs_turma['turenderecoescola']; ?>				
						</td>
					</tr>
					<?php 
					$latitude  = explode('.',$rs_turma['turlatitude']);
					$longitude = explode('.',$rs_turma['turlongitude']);
					?>
					<tr>
						<td class="SubTituloDireita"><b>Latitude:</b></td>
						<td>
							<input name="turlatitude" id="turlatitude" value="<? echo $rs_turma['turlatitude']; ?>" class="normal" type="hidden"> 
								<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> - 
								<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
								<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
								<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Longitude:</b></td>
						<td>
							<input name="turlongitude" id="turlongitude" maxlength="2" size="3" value="<? echo $rs_turma['turlongitude']; ?>" type="hidden"> 
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
			<td><?php echo $rs_turma['enderecocodinep']; ?>
				<input type="hidden" name="enderecocodinep" id="enderecocodinep" value="<?=$rs_turma['enderecocodinep']; ?>">
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
							<?php echo $rs_turma['turnometurma']; ?>				
						</td>
					</tr>
					
					<tr>
						<td class="subtituloDireita"><b>CEP:</b></td>
						<td>
							<?php $turcepturma = formata_cep($rs_turma['turcepturma']); ?>
							<?php echo $turcepturma; ?>				
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"><b>Endereço:</b></td>
						<td>
							<?php echo $rs_turma['turenderecoturma']; ?>				
						</td>
					</tr>
					<?php 
					$latitude  = explode('.',$rs_turma['turlatitudeturma']);
					$longitude = explode('.',$rs_turma['turlongitudeturma']);
					?>
					<tr>
						<td class="SubTituloDireita"><b>Latitude:</b></td>
						<td>
							<input name="turlatitudeturma" id="turlatitudeturma" value="<? echo $rs_turma['turlatitudeturma']; ?>" class="normal" type="hidden"> 
							<span id="_graulatitude1_turma"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> - 
							<span id="_minlatitude1_turma"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  ' 
							<span id="_seglatitude1_turma"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
							<span id="_pololatitude1_turma"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"><b>Longitude:</b></td>
						<td>
							<input name="turlongitudeturma" id="turlongitudeturma" value="<? echo $rs_turma['turlongitudeturma']; ?>" type="hidden"> 
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
			<td class="subtituloDireita">O estabelecimento tem ato autorizativo do
				respectivo sistema de ensino?</td>
			<td><?php echo $rs_turma['estabelecimentoautorizado']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Emitido pelo</td>
			<td><?php echo $rs_turma['tipoconselho']; ?></td>
		</tr>
		<tr class="campos_ato" style="display: none;">
			<td class="subtituloDireita">Nº do Ato</td>
			<td><?php echo $rs_turma['turnumeroato']; ?></td>
		</tr>
		<tr class="campos_ato" style="display: none;">
			<td class="subtituloDireita">Data de Publicação</td>
			<td><?php echo $rs_turma['turdatapublicacaoato']; ?></td>
		</tr>
		<?if( $rs_turma['arqid'] ){ ?>
		<tr>
			<td class="subtituloDireita">Anexo</td>
			<td><a
				href="proinfantil.php?modulo=novasturmas/mostraDadosTurma&acao=A&turid=<?php echo $rs_turma['arqid'];?>&arqid=<?php echo $rs_turma['arqid'];?>&requisicao=downloadArquivo">
					<img border="0" src="../imagens/anexo.gif" style="cursor: pointer;" />&nbsp;<?php echo $rs_turma['arqdescricao']; ?>
			</a>
			</td>
		</tr>
		<?} ?>
		<tr>
			<td colspan="2" height="100" class="quantidade_alunos">
				<table class="tabela" style="width: 100%" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
						<tr>
							<td align="center" colspan="6" class="subtituloCentro">
								Quantidade Máxima de Alunos Permitidos para o Mês <?=mes_extenso($rs_turma['turmes']); ?>/<?=$rs_turma['turano']; ?>: <span style="color: blue;"><?php echo (int)$totalGeralMatricula;?></span><br>
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
								switch($ttuid){
									case 1 :
										echo ($turnos['codigo'] == 1) ? ((empty($qtdCrecheIntegral)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtdCrecheIntegral) : ((empty($qtdCrecheParcial)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtdCrecheParcial);
										break;
									case 2 :
										echo ($turnos['codigo'] == 1) ? ((empty($qtdPreEscolaIntegral)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtdPreEscolaIntegral) : ((empty($qtdPreEscolaParcial)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtdPreEscolaParcial);
										break;
									case 3 :
										echo ($turnos['codigo'] == 1) ? ((empty($qtdCrecheIntegral)) ? "Quantidade máxima de alunos permitida em Creche Integral:0" : "Quantidade máxima de alunos permitida em Creche Integral:".$qtdCrecheIntegral) : ((empty($qtdCrecheParcial)) ? "Quantidade máxima de alunos permitida em Creche Parcial:0" : "Quantidade máxima de alunos permitida em Creche Parcial:".$qtdCrecheParcial);
										echo "<br>";
										echo ($turnos['codigo'] == 1) ? ((empty($qtdPreEscolaIntegral)) ? "Quantidade máxima de alunos permitida em Pré-escola Integral:0" : "Quantidade máxima de alunos permitida em Pré-escola Integral:".$qtdPreEscolaIntegral) : ((empty($qtdPreEscolaParcial)) ? "Quantidade máxima de alunos permitida em Pré-escola Parcial:0" : "Quantidade máxima de alunos permitida em Pré-escola Parcial:".$qtdPreEscolaParcial);										
										break;
								}?>
								 </font>
								<input type="hidden" name="tatid[]" value="<?php echo $turnos['codigo']; ?>" />
							</td>
							<td width="25%">
								<?php echo 'Matrículas na nova turma - Tempo '.$turnos['descricao'];?>
							</td>
	
							<td width="15%" class="ntaquantidade_creche" style="display:none;"><?php echo $ntaquantidade_creche; ?></td>
							<td width="15%" class="ntaquantidade_creche" style="display:none;"><?php echo $ntaquantidadeprofessor_creche; ?></td>
							<td width="15%" class="ntaquantidade_pre" style="display:none;"><?php echo $ntaquantidade_pre; ?></td>
							<td width="15%" class="ntaquantidade_pre" style="display:none;"><?php echo $ntaquantidadeprofessor_pre; ?></td>
						</tr>
				<?php } ?>	
					</table>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center"><input type="button" name="btnFechar" value="Fechar" onclick="javascript: window.close();"></td>
		</tr>
	</table>
	
	<?php 
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Fotos</label></td></tr><tr>';
	print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	
	$sql = "SELECT 			distinct sal.salid,tis.tisdescricao
			FROM			proinfantil.tiposala tis
			INNER JOIN		proinfantil.sala sal ON sal.tisid = tis.tisid
			WHERE			sal.modid = 2
			ORDER BY		tis.tisdescricao";
	
	$arySalas = $db->carregar($sql);
	?>
	<table class="tabela" style="width: 100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">				
		<?php if($arySalas){ ?>
			<?php $i=0; ?>
			<?php foreach($arySalas as $sala){ ?>
				<?php echo $i%2 == 0 ? "<tr>" : "" ?>
				<td width="50%" valign="top" id="td_sala_<?php echo $sala['salid']; ?>">
					<fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
						<legend>Fotos - <?php echo $sala['tisdescricao'] ?></legend>
						<table width="100%">
							<tr>
								<td valign="top">
									<?php 
									$sql = "SELECT 			arq.arqid,	arq.arqnome||'.'||arq.arqextensao,	arq.arqdescricao, arq.arqtamanho
											FROM 			proinfantil.fotos fot
											LEFT JOIN		public.arquivo arq ON arq.arqid = fot.arqid
											WHERE			fot.salid = {$sala['salid']}
											AND				fot.turid = {$_REQUEST['turma']}
											AND				fot.fotstatus = 'A'";
									
									$arrFotos = $db->carregar($sql); ?>
									<?php if($arrFotos){ ?>
										<?php foreach($arrFotos as $foto){ ?>
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
													$img = "../slideshow/slideshow/verimagem.php?arqid=".$foto['arqid']."&newwidth=100&newheight=85&_sisarquivo=".(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]);
													$link = "proinfantil.php?modulo=novasturmas/mostraDadosTurma&acao=A&turid=".$_REQUEST['turma']."&exibirfoto=1&arqid=".$foto['arqid'];
													?>
													<div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">
														<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
														<a id="fancybox_img_<?php echo $foto['arqid'] ?>" class="fancybox_img fancybox.ajax" href="<?php echo $link; ?>" data-fancybox-group="gallery" title="<?php echo $foto['arquivo'] ?>">
															<img <?php echo $tamanho ?> class="img_foto" src="<?php echo $img; ?>" />
														</a>
													</div>
										<?php } ?>
									<?php } ?>
								</td>
							</tr>
						</table>
					</fieldset>
				</td>
				<?php echo $i%2 == 1 ? "</tr>" : "" ?>
				<?php $i++ ?>
			<?php } ?>
		<?php } ?>
	</table>
	</td>
	<td valign="top">
		<?
		$arrOcultar = array('acoes' => true);	
		if( in_array( PERFIL_SUPER_USUARIO, $perfis ) ) $arrOcultar = array('acoes' => false);
		wf_desenhaBarraNavegacao( $docid, array( 'turid' => $_REQUEST['turma'], 'muncod' => $_REQUEST['muncod']), $arrOcultar );
		?>
	</td>
</tr>
</table>
</form>
</body>
</html>