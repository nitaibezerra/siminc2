<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

$obPTA->verificaExistePTA( $_SESSION["emenda"]["ptrid"] );

$entid = $_SESSION["emenda"]["entid"];

$obPTA->validaSessionPTA( $_SESSION["emenda"]["ptiid"] );

// Grava beneficiários
if($_POST){
	
	$obPTA->manterBeneficiariosPTA( $_POST["ptiid"], $_POST );
	
}

include  APPRAIZ."includes/cabecalho.inc";

$estadoAtual = $obPTA->pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );
echo $obPTA->exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Detalhamento da Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

echo $obPTA->cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);
echo "<br />";

if($_REQUEST['ptaid'])
	echo montarAbasArray(carregaAbasPlanoTrabalho('insereBenecifiario'), "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A");
else
	echo montarAbasArray(carregaAbasPlanoTrabalho('insereBenecifiario'), "/emenda/emenda.php?modulo=principal/insereBeneficiario&acao=A");

monta_titulo( "Iniciativa", '');

if($_SESSION["emenda"]["iniid"]) {
	
	$acao = $obPTA->carregaIniciativaPTA( $_SESSION["emenda"]["iniid"] );
}
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	  
<form method="POST" name="formulario" id="formulario">

<input type="hidden" name="iniid" id="iniid" value="<?=$acao[0]['iniid'];?>">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$_SESSION["emenda"]["ptrid"];?>">
<input type="hidden" name="ptiid" id="ptiid" value="<?=$_SESSION["emenda"]["ptiid"];?>">

<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
	<td><?=$acao[0]['ininome']; ?></td>
</tr>
<tr>
	<td class="SubTitulocentro" colspan="2">Beneficiários da Iniciativa</td>
</tr>
<?php

	$quant = $obPTA->carregaBeneficiarioIniciativaPTA( $_SESSION["emenda"]["ptiid"] );
?>
<tr>
	<td align="center"><?=$obPTA->mostraAcaoBeneficiarios($_SESSION["emenda"]["ptiid"], $acao[0]['iniid'], $_SESSION["emenda"]["ptrid"]); ?></td>
</tr>

</table>
<table style="border-bottom:0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr bgcolor="#cccccc">
	<td colspan="2" style="text-align: center" >
	     <input type="button" class="" name="btgravar" value="Salvar" onclick="submeter()" <?=verificaPermissaoPerfil('pta', '', $estadoAtual, true); ?> />
	</td>
</tr>
</table>
</form>


<script type="text/javascript">


function CalculaBeneficiario(id){
	x1 = Number(eval("document.formulario.rural_"+id).value);
	x2 = Number(eval("document.formulario.urbana_"+id).value);
	somando = (x1 + x2);
	if( Number(somando) ){
		document.getElementById("total_"+id).innerHTML = "<b>"+somando+"</b>";
	} else {
		document.getElementById("total_"+id).innerHTML = "";
	}
	//calculaGeral();
}
	
function calculaGeral(){
	somaRural = parseInt(0);
	somaUrbana = parseInt(0);
	nrobj = <?=$quant+3;?>;
	for (i=0;i<=nrobj;i++){
      if(formulario.elements[i].type == "text"){
      	campo = document.formulario.elements[i].id;
      	for (ii=0;ii<=nrobj;ii++){
              if(campo == 'rural_'+ii){
              	vrRural = Number(eval("document.formulario."+campo).value);
                somaRural = (somaRural + vrRural);
                document.getElementById("total_rural").innerHTML = "<b>"+somaRural+"</b>";
              }
              else if(campo == 'urbana_'+ii){
              	vrUrbana = Number(eval("document.formulario."+campo).value);
                somaUrbana = (somaUrbana + vrUrbana);
                document.getElementById("total_urbana").innerHTML = "<b>"+somaUrbana+"</b>";
              }
         }
       }
       document.getElementById("total_geral").innerHTML = "<b>"+(somaUrbana+somaRural)+"</b>";
	}
}

function popupCadastoEscolas(tplid){
	window.open('emenda.php?modulo=principal/cadastrarEscolaBeneficiada&acao=A&tplid='+tplid,'escolas','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600');
}


function submeter(){
	var erro = 0;
	var id;
	var name;
	var tipo;
	$("[class~=obrigatorio]").each(function() { 
				id = this.id.split("_");
				name = id[1];
				tipo = id[0];
				if(tipo.search("rural") >= 0){
					if( (this.value == 0 || this.value == "0" || !this.value) && ( $("#urbana_" + name).val() == 0 || $("#urbana_" + name).val() == "0" || !$("#urbana_" + name).val() ) ){
						erro = 1;
						alert('Existem campos obrigatórios não preenchidos!');
						return false;
					}
				}				
		});
	if(erro == 0){
		document.formulario.submit();
	}
}
</script>
