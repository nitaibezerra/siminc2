<?
include ("../../includes/open_flash_chart/open-flash-chart.php");
include ("../../includes/open_flash_chart/ofc_sugar.php");

include APPRAIZ . 'includes/cabecalho.inc';
include "resultEscolasAtendidasGraficoBack.inc";
 
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/json/json2.js"></script>
<style>
	#grafico {
		font-weight: bold;
		align: center;
	}
</style>

<?
	if( $_REQUEST['ano'] == '2008' ){
		$port = "Portaria 2.931 de 4 de dezembro de 2008";
		$ck8 = "selected";
	} else if( $_REQUEST['ano'] == '2009' ){
		$port = "Portaria 1.537 de 15 de junho de 2010";
		$ck9 = "selected";
	} else if( $_REQUEST['ano'] == '10' ){
		$ck1 = "selected";
	}
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" width="195" valign="top">
			Ano da portaria
		</td>
		<td>
			<select name="portaria" id="portaria" onchange="javascript:carregaGrafico(this.value);" >
				<option value="99">Selecione</option>
				<option value="10" <? echo $ck1; ?>>Todos</option>
				<option value="2008" <? echo $ck8; ?>>Portaria 2.931 de 4 de dezembro de 2008</option>
				<option value="2009" <? echo $ck9; ?>>Portaria 1.537 de 15 de junho de 2010</option>
			</select>
		</td>
	</tr>	
</table>

<?php 

if( $_REQUEST['ano'] ){
	$monta = $_REQUEST['ano'];

//	$anoEspecifico = $_REQUEST['acao']=='A' ? "2008" : "2009";
	$sqlGrafico = obterSqlGrafico( $monta );
	$registrosParaGrafico = $db->carregar( $sqlGrafico );
	
	$js = ' var data_values = '. retornaGrafico( $title , $registrosParaGrafico  ).'; ';
	
	echo "<br><center><div id=\"grafico\" align=\"center\"></div></center>";
	$js.= 'swfobject.embedSWF(';
	$js.= '"/includes/open_flash_chart/open-flash-chart.swf", "grafico",';
	$js.= '"600", "350", "9.0.0" ,';
	$js.= '{"loading":"Carregando gráfico..."} );';
	$js.= 'function open_flash_chart_data(){return JSONOFC.stringify( data_values );}';

}
?>

<script type="text/javascript">
function mudaGrafico(data,titulo){
	tmp = findSWF("grafico");
	x = tmp.load( JSON.stringify(data) );

	var elementosTabelas = document.getElementsByTagName('div'); 
	for (var i=0; i<elementosTabelas.length; i++){
  		var elemento = elementosTabelas[i];
	  	if (elemento.className == 'tabela') {
	    	elemento.style.display = "none";
  		}
	}
	document.getElementById("tabela_data_" + titulo ).style.display = "block";
}

function findSWF(movieName) {
	  if (navigator.appName.indexOf("Microsoft")!= -1) {
	    return window[movieName];
	  } else {
	    return document[movieName];
	  }
	}

function carregaGrafico(ano) {
	if( ano == 99 ){
		return false;
	}
	window.location.href='/pse/pse.php?modulo=relatorio/formEscolasAtendidasGrafico&acao=A&ano='+ano;
}	
<?php  if( $monta ){ echo $js; } ?>
</script>

<? if( $monta ){

	if( $monta == 10 ){
		$tit = "Escolas atendidas";
	} else {
		$tit = "Escolas atendidas pela $port";
	}
?>

<br />
<h2 align="center"><?= $tit ?></h2>
<?php
	// Devido às tags Html <center>, o somatório automático não está funcionando. 
	// Solucionado com soma no interador abaixo: 
	$arrayParaMontaLista = array();

	$totalEscolasExistentes = 0;
	$totalEscolasAtendidas = 0;

	foreach( $registrosParaGrafico as $dados ){
		$percentualAtendidas = formata_valor(($dados['escolas_atendidas'] / $dados['total']) * 100) . "%";
		$arrayParaMontaLista[] = array(
									"uf" 				   => "<center>".$dados['ufb']."</center>"				  ,
									"total"  			   => "<center>".$dados['total']."</center>" 			  ,
									"escolas_atendidas"    => "<center>".$dados['escolas_atendidas']."</center>"  , 
									"percentual_atendidos" => "<center>".$percentualAtendidas."</center>"
								 );
								 
		$totalEscolasExistentes += $dados['total'];
		$totalEscolasAtendidas  += $dados['escolas_atendidas'];								 
	}		
		
	$arrayParaMontaLista[] = array(
								"uf" 				   => "<P><center><b>Totais</b></center>"					     ,        
								"total"  			   => "<P><center><b>".$totalEscolasExistentes."</b></center>" 				 ,
								"escolas_atendidas"    => "<P><center><b>".$totalEscolasAtendidas."</b></center>"  , 
								"percentual_atendidos" => "&nbsp;" 
							 );		
	
	$cabecalho = array( 'UF','Total de escolas existentes', 'Escolas atendidas', 'Percentual de atendidas');
	$db->monta_lista_simples( $arrayParaMontaLista, $cabecalho, 500000, 10, 'N', '60%', 'N' );
}
?>
<br><br>