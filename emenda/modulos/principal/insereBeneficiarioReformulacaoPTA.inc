<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

verificaExistePTA( $_SESSION["emenda"]["ptridReformulacao"] );
$entid = $_SESSION["emenda"]["entid"];
validaSessionPTA( $_SESSION["emenda"]["ptiidReformulacao"] );

// Grava beneficiários
if($_POST){	
	$obPTA->manterBeneficiariosPTA( $_POST["ptiid"], $_POST, true );	
}

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Detalhamento da Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
monta_titulo( "Iniciativa", '');

if($_SESSION["emenda"]["iniidReformulacao"]) {	
	$acao = $obPTA->carregaIniciativaPTA( $_SESSION["emenda"]["iniidReformulacao"] );
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/funcoes.js"></script>
	  
<form method="POST" name="formulariobeneficiario" id="formulariobeneficiario">

<input type="hidden" name="iniid" id="iniid" value="<?=$acao[0]['iniid'];?>">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$_SESSION["emenda"]["ptridReformulacao"];?>">
<input type="hidden" name="ptiid" id="ptiid" value="<?=$_SESSION["emenda"]["ptiidReformulacao"];?>">

<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
	<td><?=$acao[0]['ininome']; ?></td>
</tr>
<tr>
	<td class="SubTitulocentro" colspan="2">Beneficiários da Iniciativa</td>
</tr>
<?php

	$quant = $obPTA->carregaBeneficiarioIniciativaPTA( $_SESSION["emenda"]["ptiidReformulacao"] );
?>
<tr>
	<td align="center"><?=$obPTA->mostraAcaoBeneficiarios($_SESSION["emenda"]["ptiidReformulacao"], $acao[0]['iniid']); ?></td>
</tr>

</table>
<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr bgcolor="#cccccc">
	<td colspan="2" style="text-align: center" >
	     <input type="button" class="" name="btgravar" value="Salvar" onclick="submeter()" />
	</td>
</tr>
</table>
</form>


<script type="text/javascript">

var form = document.getElementById('formulariobeneficiario');

function CalculaBeneficiario(id){
	x1 = Number(eval("document.formulariobeneficiario.rural_"+id).value);
	x2 = Number(eval("document.formulariobeneficiario.urbana_"+id).value);
	somando = (x1 + x2);
	if( Number(somando) ){
		document.getElementById("total_"+id).innerHTML = "<b>"+somando+"</b>";
	} else {
		document.getElementById("total_"+id).innerHTML = "";
	}
	calculaGeral();
}
	
function calculaGeral(){
	somaRural = parseInt(0);
	somaUrbana = parseInt(0);
	nrobj = <?=$quant+3;?>;
	for (i=0;i<=nrobj;i++){
      if(form.elements[i].type == "text"){
      	campo = form.elements[i].id;
      	for (ii=0;ii<=nrobj;ii++){
              if(campo == 'rural_'+ii){
              	vrRural = Number(eval("document.formulariobeneficiario."+campo).value);
                somaRural = (somaRural + vrRural);
                document.getElementById("total_rural").innerHTML = "<b>"+somaRural+"</b>";
              }
              else if(campo == 'urbana_'+ii){
              	vrUrbana = Number(eval("document.formulariobeneficiario."+campo).value);
                somaUrbana = (somaUrbana + vrUrbana);
                document.getElementById("total_urbana").innerHTML = "<b>"+somaUrbana+"</b>";
              }
         }
       }
       document.getElementById("total_geral").innerHTML = "<b>"+(somaUrbana+somaRural)+"</b>";
	}
}

function submeter(){
	form.submit();
}
</script>
