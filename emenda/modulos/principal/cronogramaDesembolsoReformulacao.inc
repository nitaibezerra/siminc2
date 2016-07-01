<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');

$ptrid = $_REQUEST["ptrid"];
$_SESSION["emenda"]["ptridReformulacao"] = $ptrid;
validaSessionPTA( $ptrid );

$tipoChamada = $_REQUEST['chamada'];

$obPTA = new PTA();
$obCronograma = new CronogramaExecucao( $ptrid );

$obCronograma->ptrid = $ptrid;
$obCronograma->tipoChamada = $tipoChamada;

if($_REQUEST["submetido"]) {
	
	if($_REQUEST["parcela_excluir"] && $_REQUEST["tipo_parcela_excluir"]) {
		$obCronograma->excluiCronogramaDesembolso( $_REQUEST );
	} else {
		//update
		$obCronograma->manterCronogramaExecucao( $_POST, $obPTA, true );
	}
		
}

$cronogramaExecucao = $obCronograma->verificaIniciativaVinculadaEspecificacao();
	
monta_titulo( 'Cronograma de Execução e Desembolso', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
$prdminuta = 'P'; // plano de trabalho


$obCronograma->prdminuta = $prdminuta;

$verificaParcelaDesembolso = $obCronograma->carregaTipoParcelaDesembolso();

$existeParcelaConcedente = false;
$existeParcelaProponente = false;

foreach ($verificaParcelaDesembolso as $v) {
	if ($v["prdtipo"] == 'C')
		$existeParcelaConcedente = true;
	else if($v["prdtipo"] == 'P')
		$existeParcelaProponente = true;
}

$iniciativas = $obCronograma->carregaIniciativaCronograma();

if($existeParcelaConcedente == false){
	$arData = explode("/", $cronogramaExecucao[0]['inicial']);
	$data = $arData[1]."-".$arData[0]."-01";
	$obCronograma->criarParcelaDesembolso($ptrid, $data, 'C', $prdminuta, $iniciativas);
}

if($existeParcelaProponente == false){
	$data = explode("/", $cronogramaExecucao[0]['inicial']);
	$data = $data[1]."-".$data[0]."-01";
	$obCronograma->criarParcelaDesembolso($ptrid, $data, 'P', $prdminuta, $iniciativas);
}

//$perfil = pegaPerfil($_SESSION['usucpf']);

// Incluindo parcelas nas iniciativas que ainda nao as possuem
$obCronograma->incluirParcelasInexistentes();
					
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript">	
	var id_ptiid = new Array();
</script>
<form id="formCronograma" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="parcela_excluir" id="parcela_excluir" value="" />
<input type="hidden" name="tipo_parcela_excluir" id="tipo_parcela_excluir" value="" />
<input type="hidden" name="tipoChamada" id="tipoChamada" value="<?php echo $tipoChamada; ?>" />
<input type="hidden" name="prdminuta" id="prdminuta" value="<?=$prdminuta;?>">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
<input type="hidden" name="datainicial" id="datainicial" value="<?=$cronogramaExecucao[0]["inicial"];?>">
<input type="hidden" name="datafinal" id="datafinal" value="<?=$cronogramaExecucao[0]["final"];?>">
<?
$obRefor = new reformulacaoPTA( $db );
echo $obRefor->montaListaCronogramaDesembolso( $ptrid, $obCronograma );
?>
</form>

<script>

var form 		= document.getElementById("formCronograma");
var btSalvar 	= document.getElementById("bt_salvar");


function salvarCronograma() {
	btSalvar.disabled = true;
	//var tipochamada = document.getElementById("tipoChamada").value;
	var numParcelasConcedente 	= document.getElementById("num_parcelas_concedente").value;
	var numParcelasProponente 	= document.getElementById("num_parcelas_proponente").value;
		
	for(var k=0; k<id_ptiid.length; k++) {
	
		var totalConcedente	= Number(document.getElementById("total_C_" + id_ptiid[k]).value);
		var totalProponente	= Number(document.getElementById("total_P_" + id_ptiid[k]).value);
	
		var somaConcedente = 0;
		for(var i=0; i<numParcelasConcedente; i++) {
			var valor = document.getElementById(id_ptiid[k] + '_C_' + i).value;
			
			valor = valor.replace(/\./gi, "");
			valor = valor.replace(/,/gi, ".");
			
			somaConcedente += Number(valor);
		}
		
		if(somaConcedente > totalConcedente) {
			alert('A soma dos valores das parcelas da iniciativa não pode ser\n        superior ao valor da Iniciativa (concedente).');
			document.getElementById(id_ptiid[k] + '_C_0').focus();
			btSalvar.disabled = false;
			return;
		}
		
		var somaProponente = 0;
		for(var i=0; i<numParcelasProponente; i++) {
			var valor = document.getElementById(id_ptiid[k] + '_P_' + i).value;
		
			valor = valor.replace(/\./gi, "");
			valor = valor.replace(/,/gi, ".");
			
			somaProponente += Number(valor);
		}
		
		if(somaProponente > totalProponente) {
			alert('A soma dos valores das parcelas da iniciativa não pode ser\n        superior ao valor da Iniciativa (proponente).');
			document.getElementById(id_ptiid[k] + '_P_0').focus();
			btSalvar.disabled = false;
			return;
		}
	}
	form.submit();
}

</script>