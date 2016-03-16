<?php 

$sql = "select muncod, estuf from snf.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}'";
$rsResponsabilidade = $db->pegaLinha($sql);

if(!$rsResponsabilidade){
	alert('Usuário sem responsabilidade!');
	header("Location: snf.php?modulo=principal/inicio&acao=A");
}

include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Síntese", "&nbsp;");

// Monta filtros
$arWhere = array();

if(checkPerfil(PERFIL_EQUIPE_MUNICIPAL_APROVACAO) && $rsResponsabilidade['muncod']){
	array_push($arWhere, "pp.muncod = '{$rsResponsabilidade['muncod']}'");
	array_push($arWhere, "pp.pdiesfera = 'Municipal'");
		
}elseif(checkPerfil(PERFIL_EQUIPE_ESTADUAL_APROVACAO) && $rsResponsabilidade['estuf']){
	array_push($arWhere, "pp.estuf = '{$rsResponsabilidade['estuf']}'");
	array_push($arWhere, "pp.pdiesfera = 'Estadual'");
}

// TOTAL DE ESCOLAS NA REDE
$sql = "SELECT COUNT(distinct d.no_escola)
		FROM educacenso_2010.tb_escola_inep_2010 d
		INNER JOIN pdeinterativo.pdinterativo pp
		ON d.pk_cod_entidade = cast(pp.pdicodinep AS Integer)
		".( count($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."";

//$rsEscolasRede = $db->pegaUm($sql);
$rsEscolasRede = $rsEscolasRede ? $rsEscolasRede : 0; 

// TOTAL de Professores na rede
$sql = "SELECT COUNT(distinct d.*) as total
		FROM educacenso_2010.tab_docente d 
		INNER JOIN educacenso_2010.tab_dado_docencia ee 
		ON d.pk_cod_docente = ee.fk_cod_docente 
		INNER JOIN pdeinterativo.pdinterativo pp
		ON ee.fk_cod_entidade = cast(pp.pdicodinep AS Integer)
		".( count($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."";

//$rsProfeRedeEnsino = $db->pegaUm($sql);
$rsProfeRedeEnsino = $rsProfeRedeEnsino ? $rsProfeRedeEnsino : 0;

// Verifica ano
if(date('Y') == '2012'){
	array_push($arWhere, "pp.pcfano IN ('2012','2013')");
} else {
	array_push($arWhere, "pp.pcfano = '".date('Y')."'");
}

// TOTAL Escolas com plano de Formação não atendidas
$sql = "SELECT COUNT(DISTINCT pp.pdeid) as total
		FROM snf.prioridadecursoescola pp
		WHERE privagasprevistas is null
		".( count($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."";

$rsEscolasRedeNaoAtendidas = $db->pegaUm($sql);
$rsEscolasRedeNaoAtendidas = $rsEscolasRedeNaoAtendidas ? $rsEscolasRedeNaoAtendidas : 0;

// TOTAL Escolas com plano de Formação ref 2012/13
$sql = "SELECT COUNT(DISTINCT pp.pdeid) as total
		FROM snf.prioridadecursoescola pp
		".( count($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."";

$rsEscolasPlanoFormacao201213 = $db->pegaUm($sql);
$rsEscolasPlanoFormacao201213 = $rsEscolasPlanoFormacao201213 ? $rsEscolasPlanoFormacao201213 : 0;

// TOTAL Professores solicitantes ref 2012/13 	
$sql = "SELECT SUM(privagassolicitadas) as total
		FROM snf.prioridadecursoescola pp
		".( count($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."";

$rsProfeSolicitantes = $db->pegaUm($sql);
$rsProfeSolicitantes = $rsProfeSolicitantes ? $rsProfeSolicitantes : 0; 

// TOTAL Professores atendidos ref 2012/13 	
$sql = "SELECT SUM(privagasprevistas) as total
		FROM snf.prioridadecursoescola pp
		".( count($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."";

$rsProfeAtendido = $db->pegaUm($sql);
$rsProfeAtendido = $rsProfeAtendido ? $rsProfeAtendido : 0;

// TOTAL Professores não atendidos
$rsProfeNaoAtendido = $rsProfeSolicitantes-$rsProfeAtendido;

// Demanda Social Atendida
$sql = "SELECT SUM(pridemvagasprevistas) as total
		FROM snf.prioridadecursoescola pp
		WHERE pp.pridemsoc = 'S'
		".( count($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."";

$rsDemandaAutorizada = $db->pegaUm($sql);
$rsDemandaAutorizada = $rsDemandaAutorizada ? $rsDemandaAutorizada : 0;

// Demanda Social Solicitada 
$sql = "SELECT SUM(pridemsocvagassolicitadas) as total
		FROM snf.prioridadecursoescola pp
		WHERE pp.pridemsoc = 'S'
		".( count($arWhere) ? ' AND '.implode(' AND ', $arWhere) : '')."";

$rsDemandaSolicitada = $db->pegaUm($sql);
$rsDemandaSolicitada = $rsDemandaSolicitada ? $rsDemandaSolicitada : 0;
?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="280" class="subtituloDireita">Total de Escolas na rede</td>
		<td><?php echo number_format($rsEscolasRede, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Escolas com plano de Formação ref 2012/13</td>
		<td><?php echo number_format($rsEscolasPlanoFormacao201213, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Escolas com plano de Formação não atendidas</td>
		<td><?php echo number_format($rsEscolasRedeNaoAtendidas, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Total de Professores na rede</td>
		<td><?php echo number_format($rsProfeRedeEnsino, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Professores solicitantes ref 2012/13</td>
		<td><?php echo number_format($rsProfeSolicitantes, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Professores atendidos ref 2012/13</td>
		<td><?php echo number_format($rsProfeAtendido, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Professores não atendidos</td>
		<td><?php echo number_format($rsProfeNaoAtendido, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Demanda Social Solicitada</td>
		<td><?php echo number_format($rsDemandaAutorizada, 0, ",", ".") ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Demanda Social Atendida</td>
		<td><?php echo number_format($rsDemandaSolicitada, 0, ",", ".") ?></td>
	</tr>
</table>

<link type="text/css" href="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" rel="stylesheet" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>

<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>

<script>
$(document).ready(function(){
	
	  var data = [
	    ['Professores atendidos ref 2012/13', <?php echo $rsProfeAtendido ?>],['Professores não atendidos', <?php echo $rsProfeNaoAtendido ?>]
	  ];
	  
	  var plot1 = jQuery.jqplot ('chartdiv1', [data],
	    {
	      seriesDefaults: {
	        // Make this a pie chart.
	        renderer: jQuery.jqplot.PieRenderer,
	        rendererOptions: {
	          // Put data labels on the pie slices.
	          // By default, labels show the percentage of the slice.
	          showDataLabels: true
	        }
	      },
	      legend: { show:true, location: 'e' }
	    }
	  );
	});
</script>
<center>
	<div id="chartdiv1" style="margin-top:10px;height:70%;width:60%; "></div>
</center>

<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.logAxisRenderer.js"></script>
<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.categoryAxisRenderer.min.js"></script>
<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.pointLabels.min.js"></script>

<script>
$(document).ready(function(){
	
	  var data = [
	    ['Professores não atendidos', <?php echo $rsProfeNaoAtendido?>],['Professores atendidos ref 2012/13', <?php echo $rsProfeAtendido ?>]
	    ,['Professors solicitantes ref 2012/13', <?php echo $rsProfeSolicitantes ?>]
	  ];
	  
	  var plot1 = jQuery.jqplot ('chartdiv2', [data],
	    {
	      seriesDefaults: {
	        // Make this a pie chart.
	        renderer: jQuery.jqplot.PieRenderer,
	        rendererOptions: {
	          // Put data labels on the pie slices.
	          // By default, labels show the percentage of the slice.
	          showDataLabels: true
	        }
	      },
	      legend: { show:true, location: 'e' }
	    }
	  );
	});
</script>
<center>
	<div id="chartdiv2" style="margin-top:10px;height:70%;width:60%; "></div>
</center>

<script>
$(document).ready(function(){
	
	  var data = [
	    ['Total de escolas na rede', <?php echo $rsEscolasRede ?>],['Escolas com plano de formação ref 2012/13', <?php echo $rsEscolasPlanoFormacao201213 ?>]
	    ,['Escolas com plano de formação não atendidas', <?php echo $rsEscolasRedeNaoAtendidas ?>]
	  ];
	  
	  var plot1 = jQuery.jqplot ('chartdiv3', [data],
	    {
	      seriesDefaults: {
	        // Make this a pie chart.
	        renderer: jQuery.jqplot.PieRenderer,
	        rendererOptions: {
	          // Put data labels on the pie slices.
	          // By default, labels show the percentage of the slice.
	          showDataLabels: true
	        }
	      },
	      legend: { show:true, location: 'e' }
	    }
	  );
	});
</script>
<center>
	<div id="chartdiv3" style="margin-top:10px;height:70%;width:60%; "></div>
</center>