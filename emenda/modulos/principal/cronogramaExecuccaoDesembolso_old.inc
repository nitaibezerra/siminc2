<?php

if ($_REQUEST['insere']) {
	header('content-type: text/html; charset=ISO-8859-1');
	insereCronogramaExecucaoDesembolso($_POST);
	exit;
}

include APPRAIZ ."includes/workflow.php";
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( "Cronograma de Execução e Desembolso", "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);
echo "<br />";

$ptrid = $_SESSION["emenda"]["ptrid"];

$sql = "SELECT 
			    CASE WHEN sum(ptae.ptequantidade * ptae.ptevalorunitario) is null
			 THEN '-'
			 ELSE trim(to_char(sum(ptae.ptequantidade * ptae.ptevalorunitario), '999G999G999D99'))
		    END as valor_total,
		    CASE WHEN sum(ptae.ptevalorproponente) is null
			 THEN null
			 ELSE sum(ptae.ptevalorproponente)
		    END as valor_proponente,
		    CASE WHEN (sum(ptae.ptequantidade * ptae.ptevalorunitario)-sum(ptae.ptevalorproponente)) is null
			 THEN null
			 ELSE (sum(ptae.ptequantidade * ptae.ptevalorunitario)-sum(ptae.ptevalorproponente))
		    END as valor_concedente
		FROM emenda.ptacaoespecificacao ptae INNER JOIN emenda.ptacao pta
			ON (ptae.ptaid = pta.ptaid) INNER JOIN emenda.planotrabalho pt
		    ON (pta.ptrid = pt.ptrid)
		WHERE ptae.ptestatus = 'A' 
			AND pt.ptrstatus = 'A' 
		    AND pt.ptrid = $ptrid ";

	$valores = $db->pegalinha($sql);
	
	$totalEspecificacao = $valores['valor_total'];
	$totalProponente    = str_replace('.',',', $valores['valor_proponente']);
	$totalConcedente    = str_replace('.',',', $valores['valor_concedente']);

$sql_data = "SELECT 
			    to_char(min(ptedatainicio), 'MM/YYYY') as min_dataini, 
			    to_char(max(ptedatafim), 'MM/YYYY') as max_datafim 
			FROM emenda.ptacaoespecificacao ptae INNER JOIN emenda.ptacao pta
			    ON (ptae.ptaid = pta.ptaid) INNER JOIN emenda.planotrabalho pt
			    ON (pta.ptrid = pt.ptrid)
			WHERE ptae.ptestatus = 'A' 
			    AND pt.ptrstatus = 'A'
				AND pt.ptrid = $ptrid";
	
	$arData = $db->pegaLinha($sql_data);
	
	$mesIni = explode('/', $arData["min_dataini"]); 
	$mesFim = explode('/', $arData["max_datafim"]); 

	$DataInicio	= retornaMes( $mesIni[0] ) . '/' . $mesIni[1];
	$DataFim	= retornaMes( $mesFim[0] ) . '/' . $mesFim[1];
	
	$sql = "SELECT 
			  ptcid,
			  ptrid,
			  ptctipo,
			  ptcdata,
			  trim(to_char(ptcvalor, '999G999G999D99')) as ptcvalor
			FROM 
			  emenda.ptcronogramadesembolso
			WHERE ptrid = $ptrid
			order by ptcdata ";
	
	$arDados = $db->carregar($sql);

	$quantAno = $mesFim[1] - $mesIni[1];

	
	$sql = "SELECT 
			  ptctipo as tipo,
			  --to_char(sum(ptcvalor), '999G999G999D99') as valor
			  sum(ptcvalor) as valor
			FROM 
			  emenda.ptcronogramadesembolso
			WHERE ptrid = $ptrid
			GROUP BY ptctipo
			ORDER BY ptctipo";

	$arTotal = $db->carregar($sql);
?>

<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script	src="/includes/calendario.js"></script>
<body>


<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >

<input type="hidden" name="totalproponente" id="totalproponente" value="<?=$totalProponente; ?>">
<input type="hidden" name="totalconcedente" id="totalconcedente" value="<?=$totalConcedente; ?>">
<input type="hidden" name="informadoConc" id="informadoConc" value="0">
<input type="hidden" name="informadoProp" id="informadoProp" value="0">
<input type="hidden" name="anoIni" id="anoIni" value="<?=$mesIni[1]; ?>">
<input type="hidden" name="anoFim" id="anoFim" value="<?=$mesFim[1]; ?>">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th style="text-align: left;">Cronograma de Execução</th>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style=""/>
	</tr>
</table>

<table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:20%;"><b>Mês/Ano Inicial:</b></td>
		<td><?=$DataInicio; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Mês/Ano Final:</b></td>
		<td><?=$DataFim; ?></td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
</table>

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th style="text-align: left;">Cronograma de Desembolso</th>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style=""/>
	</tr>
</table>

<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="6">DESEMBOLSO DO CONCEDENTE</th>
	</tr>
</table>
<?php
	mostraCronogramaExecucaoConcedente($ptrid, $totalConcedente);
?>

<br>
<br>
<table id="tblform" class="listagem" width="60%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="6">DESEMBOLSO DO PROPONENTE</th>
	</tr>
</table>
<?php
	mostraCronogramaExecucaoProponente($ptrid, $totalProponente);
?>

</form>
<div id="erro"></div>
</body>
<script type="text/javascript">
	function incluirConcedente(){
		alert($('ptcmes').value);
		alert($('ptcano').value);
		alert($('ptcvalor').value);
	}
	
	function incluirProponente(){
	
	}
	
	function calculavalorConcedente(valor){
		var v = valor.value;
		var informado = 0;
		var naoinformado = 0;
		var valorC = 0;
		var concedente = $('totalconcedente').value;
		
		//if(v){
			for(i=0; i<formulario.length; i++){
				if(formulario.elements[i].type == "text"){
					if(formulario.elements[i].id.indexOf("ptcvalorC") == 0){
						if( (formulario.elements[i].value != 0) && (formulario.elements[i].value != "") ){
							valorC = parseFloat(valorC) + parseFloat( retiraPontos(formulario.elements[i].value) );
						}
					}
				}
			}

			if(valorC > 0)
				naoinformadoC = parseFloat( retiraPontos( concedente ) ) - parseFloat(valorC);

			if(naoinformadoC < 0){
				alert('A soma dos valores informados ultrapassa o valor total do plano de trabalho para o Concedente');
				valor.value = '';
				calculavalorConcedente('0,00');
				return false;	
			}else{
					$('NinformadoC').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', naoinformadoC.toFixed(2));
					$('informadoC').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valorC.toFixed(2));
					$('informadoConc').value = valorC.toFixed(2);
			}
		//}
	}
	
	function calculavalorProponente(valor){
		var v = valor.value;
		var naoinformado = 0;
		var valorP = 0;
		var proponente = $('totalproponente').value;

		//if(v){
			for(i=0; i<formulario.length; i++){
				if(formulario.elements[i].type == "text"){
				
					if(formulario.elements[i].id.indexOf("ptcvalorP") == 0){
						if( (formulario.elements[i].value != 0) && (formulario.elements[i].value != "") ){
							valorP = parseFloat(valorP) + parseFloat( retiraPontos(formulario.elements[i].value) );
						}
					}
				}
			}
			if(valorP > 0)
				naoinformado = parseFloat( retiraPontos( $('totalproponente').value) ) - parseFloat(valorP);
			
			if(naoinformado < 0){
				alert('A soma dos valores informados ultrapassa o valor total do plano de trabalho para o Proponente');
				valor.value = '';
				calculavalorProponente('0,00');
				return false;
			}else{
				$('NinformadoP').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', naoinformado.toFixed(2));
				$('informadoP').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valorP.toFixed(2));
				$('informadoProp').value = valorP.toFixed(2);
			}
		//}
	}
	
	function retiraPontos(valor){
		valor = valor.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
		
		return valor;
	}
	
	function validaDados(){
		var valorC = parseFloat(0);
		var valorP = parseFloat(0);
		var arDadosP = "";
		var arDadosC = "";
		var arDadosIdP = "";
		var arDadosIdC = "";
		var proponente = $('totalproponente').value;
		var concedente = $('totalconcedente').value;
		
		retiraPontos(proponente);
		retiraPontos(concedente);
		
		//monta os id dos campos
		for(i=0; i<formulario.length; i++){
			//if(formulario.elements[i].type == "hidden"){
				if(formulario.elements[i].id.indexOf("ptcidC") == 0){
					if(arDadosIdC == ""){
						arDadosIdC = formulario.elements[i].value;	
					}else{
						arDadosIdC = arDadosIdC + '|' + retiraPontos(formulario.elements[i].value);						
					}
				}
				if(formulario.elements[i].id.indexOf("ptcidP") == 0){
					if(arDadosIdP == ""){
						arDadosIdP = formulario.elements[i].value;	
					}else{
						arDadosIdP = arDadosIdP + '|' + retiraPontos(formulario.elements[i].value);						
					}
				}
				//alert(arDadosIdC);
			//}
			//monta os valores dos campos
			if(formulario.elements[i].type == "text"){
				
				if(formulario.elements[i].id.indexOf("ptcvalorC") == 0){

					if(arDadosC == ""){
						if(formulario.elements[i].value == ""){
							arDadosC = " ";
						}else{
							arDadosC = retiraPontos(formulario.elements[i].value);
						}	
					}else{
						arDadosC = arDadosC + '|' + retiraPontos(formulario.elements[i].value);						
					}
				}
				
				if(formulario.elements[i].id.indexOf("ptcvalorP") == 0){
					
					if(arDadosP == ""){
						if(formulario.elements[i].value == ""){
							arDadosP = " ";
						}else{
							arDadosP = retiraPontos(formulario.elements[i].value);
						}	
					}else{
						arDadosP = arDadosP + '|' + retiraPontos(formulario.elements[i].value);						
					}
				}
			}
		}

		var myajax = new Ajax.Request('emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A', {
		        method:     'post',
		        parameters: '&insere=true&'+$('formulario').serialize() /*arDadosIdP+'&dadosP='+arDadosP+'&dadosIdC='+arDadosIdC+'&dadosC='+arDadosC*/,
		        asynchronous: false,
		        onComplete: function (res){
					
					//$('erro').innerHTML = res.responseText; 
					if( Number(res.responseText) == 1 ){
						alert('Registro cadastrado com sucesso!');
						window.location.href = "emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A";
					}else{
						//alert('Registro não cadastrado');
					}
		        }
		  });
	}
</script>
</html>