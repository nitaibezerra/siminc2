<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

?>
<br/>
<?

$db->cria_aba( $abacod_tela, $url, "&unicod=" . $_SESSION[unicod] );
$titulo_modulo='Monitoramento';
monta_titulo($titulo_modulo,'');
?>
<br/>
<?

$lan     = "lan2008";
$lanauto = "lan2008auto";
if($_REQUEST[lan]){
	$lan = (string) $_REQUEST[lan];
	$lanauto = $_REQUEST[lan]."auto";
}
$ano 	 =  substr($lan, 3, strlen($lan));
$unicod  = (string) $_REQUEST[unicod];

$menu[0] = array("descricao" => "2008", "link"=> "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=lan2008");
$menu[1] = array("descricao" => "2009", "link"=> "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=lan2009");
$menu[2] = array("descricao" => "2010", "link"=> "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=lan2010");
$menu[3] = array("descricao" => "2011", "link"=> "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=lan2011");
$menu[4] = array("descricao" => "2012", "link"=> "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=lan2012");

echo montarAbasArray($menu, "/reuni/reuni.php?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."&lan=".$lan."");

if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '') ){

	switch($_REQUEST[evento]){		
		
		case 'A':			
			if(isset($_REQUEST[$lanauto]) && ($_REQUEST[$lanauto] != '')){								
				foreach ($_REQUEST[$lanauto] as $lanid => $lanval) {
					
					if($lanval){
						//$lanval = str_replace(array(".",","), array("","."), $lanval);
						$sql_A = "
							UPDATE reuni.lancamentocargo SET $lanauto = '".$lanval."' where lanid = ".$lanid;						
						$db->executar($sql_A);
					}					
				}
				$db->commit();
				echo "<script>
					alert('Dados salvos com sucesso.');
					window.location = '?modulo=principal/monitoramentocargos&acao=A&unicod=".$unicod."';
				  </script>";
			}	
		exit;
		break;
	}
}
?>

<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="A">
<table  class="tabela" cellSpacing="1" cellPadding="3" align="center">
<!-- ********************* Docentes ********************** -->
<tr>
	<td colspan="5" class="SubTituloEsquerda">Docentes:</td>
</tr>
<tr background="#DCDCDC">
	<td class="SubTituloCentro" width="30%">Campus</td>
	<td class="SubTituloCentro">Projetado</td>
	<td class="SubTituloCentro">Autorizados</td>
	<td class="SubTituloCentro">Concursados</td>
	<td class="SubTituloCentro">Provimentos</td>	
</tr>
<?php 
$sql_docentes = "SELECT  	
					l.lanid,
					CASE  WHEN  l.".$lan." is null THEN '0'	
					ELSE l.".$lan."
					END as lancamento,
					CASE  WHEN  l.".$lanauto." is null THEN '0'	
					ELSE l.".$lanauto."					
					END as lancamentoauto,
					c.caudesc as campus,
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = l.lanid AND
						monano = '".$ano."' AND
						montipo = 'A' AND
						monstatus = 'A')
					 as totalautorizado,					
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = l.lanid AND
						monano = '".$ano."' AND
						montipo = 'E' AND
						monstatus = 'A')
					 as totalprovido,
					 (SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = l.lanid AND
						monano = '".$ano."' AND
						montipo = 'P' AND
						monstatus = 'A')
					 as totalconcrusados
				FROM reuni.lancamentocargo l
				INNER JOIN 
					reuni.campusuniversitario c  ON c.cauid = l.cauid
				WHERE 
					unicod = '".$unicod."' AND l.nlcid = 1
				ORDER BY c.caudesc
				";
				
$docentes = $db->carregar($sql_docentes);

$sql_totais = "SELECT
					sum(l.".$lan.") as lantotal, 
					sum(l.".$lanauto.") as lanautototal
				FROM reuni.lancamentocargo l
				INNER JOIN 
					reuni.campusuniversitario c  ON c.cauid = l.cauid
				WHERE 
					unicod = '".$unicod."'
					AND l.nlcid = 1";
$totais = $db->pegaLinha($sql_totais);

$sql_totaisprovido_docentes = "SELECT sum(monnumvagas)as qtd 
					FROM 
					reuni.lancamentocargo 	AS lc						
					INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
					INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
					LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
					LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
					WHERE
					cl.limctpreg  = 'C' AND
					cu.unicod     = '".$unicod."'AND
					cu.unitpocod  = 'U'AND
					cl.nlcid      = 1 AND
					dm.monano = '".$ano."' AND
					dm.montipo = 'E' AND
					dm.monstatus = 'A'";
$totaisprovido_docentes  = $db->pegaUm($sql_totaisprovido_docentes);
$sql_totaisconcrusados_docentes = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 1 AND
						dm.monano = '".$ano."' AND
						dm.montipo = 'P' AND
						dm.monstatus = 'A'";
$totaisconcrusados_docentes  = $db->pegaUm($sql_totaisconcrusados_docentes);

$sql_totaisautorizados_docentes = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 1 AND
						dm.monano = '".$ano."' AND
						dm.montipo = 'A' AND
						dm.monstatus = 'A'";
$totaisautorizados_docentes  = $db->pegaUm($sql_totaisautorizados_docentes);

$cor = "#F5F5F5";
foreach ($docentes as $docente) {
	$lancamento 		= $lan."_".$docente['lanid'];
	$lancamentoauto		= $lanauto."[".$docente['lanid']."]";
	${$lancamento} 		= $docente['lancamento'];
	${$lancamentoauto} 	= $docente['totalautorizado'];
	$totalprovido 		= $docente['totalprovido'];
	$totalconcrusados 	= $docente['totalconcrusados'];
	
	$totalautorizado 	 = "totalautorizado_".$docente['lanid'];
	${$totalautorizado}  = $docente['totalautorizado'];
	$totalprovido 		 = "totalprovido_".$docente['lanid'];
	${$totalprovido} 	 = $docente['totalprovido'];
	$totalconcrusados 	 = "totalconcrusados_".$docente['lanid'];
	${$totalconcrusados} = $docente['totalconcrusados'];
	$totais_autorizado 	 = "totais_autorizado_1";
		
	$onKeyUp 			 = "validaAutorizacao(".$docente['lancamento'].", this, ".$docente['lancamentoauto'].",".$totais_autorizado.");";
	$onblur 			 = "atualizaTotalAuto(".$docente['lancamento'].",this, ".$docente['lancamentoauto'].",'".$totais_autorizado."');";
	
	if($cor == "#E9E9E9") $cor = "#F5F5F5";	else $cor = "#E9E9E9";
	
	echo("<tr>
			<td style='background-color:".$cor."' align='left'>".$docente['campus']."</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($lancamento,'N','N','',20,100,'','')."</td>			
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalautorizado,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('A','".$lan."','".$docente['lanid']."');\" title=\"Autorizados\">
			</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalconcrusados,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('P','".$lan."','".$docente['lanid']."');\" title=\"Concursados\">
			</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalprovido,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('E','".$lan."','".$docente['lanid']."');\" title=\"Provimentos\">
			</td>
			
		</tr>");
}
?>
<tr>
	<td style='background-color: #F0F0F0' align="right"><b>Totais:</b></td>	
	<td style='background-color: #F0F0F0' align="center"><?=$totais['lantotal'];?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_autorizado_1"><?=$totaisautorizados_docentes?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_concrusados_1"><?=$totaisconcrusados_docentes?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_provido_1"><?=$totaisprovido_docentes?></td>
	
</tr>

<!-- ********************* TA Nível E ********************** -->
<tr>
	<td colspan="5" class="SubTituloEsquerda">TA Nível E:</td>
</tr>
<tr background="#DCDCDC">
	<td class="SubTituloCentro" width="30%">Campus</td>
	<td class="SubTituloCentro">Projetado</td>
	<td class="SubTituloCentro">Autorizados</td>
	<td class="SubTituloCentro">Concursados</td>	
	<td class="SubTituloCentro">Provimentos</td>
</tr>
<?php 
$sql_E = "SELECT  					
				
					lc.lanid,
					CASE  WHEN  lc.".$lan." is null THEN '0'	
					ELSE lc.".$lan."
					END as lancamento,
					CASE  WHEN  lc.".$lanauto." is null THEN '0'	
					ELSE lc.".$lanauto."					
					END as lancamentoauto,									
					cu.caudesc as campus,
					ca.cardesc as cargo,	
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'A' AND
						monstatus = 'A')
					 as totalautorizado,					
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'E' AND
						monstatus = 'A')
					 as totalprovido,
					 (SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'P' AND
						monstatus = 'A')
					 as totalconcrusados
			
				FROM 
					reuni.lancamentocargo 	AS lc						
					INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
					INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
					LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
				
				WHERE
					cl.limctpreg  = 'C' 	AND
					cu.unicod     = '".$unicod."' AND
					cu.unitpocod  = 'U' AND
					cl.nlcid      = 2
			
			ORDER BY
			cu.caudesc
			";

$niveis_E = $db->carregar($sql_E);

$sql_totais_E = "SELECT
					sum(l.".$lan.") as lantotal, 
					sum(l.".$lanauto.") as lanautototal
				FROM reuni.lancamentocargo l
				INNER JOIN 
					reuni.campusuniversitario c  ON c.cauid = l.cauid
				WHERE 
					unicod = '".$unicod."' AND l.nlcid = 2";

$totais_E = $db->pegaLinha($sql_totais_E);

$sql_totaisprovido_E = "SELECT sum(monnumvagas)as qtd 
					FROM 
					reuni.lancamentocargo 	AS lc						
					INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
					INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
					LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
					LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
					WHERE
					cl.limctpreg  = 'C' AND
					cu.unicod     = '".$unicod."'AND
					cu.unitpocod  = 'U'AND
					cl.nlcid      = 2 AND
					dm.monano = '".$ano."' AND
					dm.montipo = 'E' AND
					dm.monstatus = 'A'";
$totaisprovido_E = $db->pegaUm($sql_totaisprovido_E);
$sql_totaisconcrusados_E = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 2 AND
						dm.monano = '".$ano."' AND
						dm.montipo = 'P' AND
						dm.monstatus = 'A'";
$totaisconcrusados_E = $db->pegaUm($sql_totaisconcrusados_E);

$sql_totaisautorizados_E = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 2 AND
						dm.monano 	  = '".$ano."' AND
						dm.montipo    = 'A' AND
						dm.monstatus  = 'A'";
$totaisautorizados_E  = $db->pegaUm($sql_totaisautorizados_E);

$cor = "#F5F5F5";
foreach ($niveis_E as $nivel_E) {
	$lancamento 		= $lan."_".$nivel_E['lanid'];	
	$lancamentoauto 	= $lanauto."[".$nivel_E['lanid']."]";
	${$lancamento} 		= $nivel_E['lancamento'];
	${$lancamentoauto} 	= $nivel_E['totalautorizado'];	
	
	$totalautorizado 	 = "totalautorizado_".$nivel_E['lanid'];
	${$totalautorizado}  = $nivel_E['totalautorizado'];
	$totalprovido 		 = "totalprovido_".$nivel_E['lanid'];
	${$totalprovido} 	 = $nivel_E['totalprovido'];
	$totalconcrusados 	 = "totalconcrusados_".$nivel_E['lanid'];
	${$totalconcrusados} = $nivel_E['totalconcrusados'];
	$totais_autorizado 	 = "totais_autorizado_2";	
	
	$onkeyup 			 = "validaAutorizacao(".$nivel_E['lancamento'].",this, ".$nivel_E['lancamentoauto'].", ".$totais_autorizado.");";
	$onblur 			 = "atualizaTotalAuto(".$nivel_E['lancamento'].",".$nivel_E['lancamento'].",this, ".$nivel_E['lancamentoauto'].",'".$totais_autorizado."');";
	
	if($cor == "#E9E9E9") $cor = "#F5F5F5";	else $cor = "#E9E9E9";
	
	echo("<tr>
			<td style='background-color:".$cor."' align='left'>".$nivel_E['campus']." - ".$nivel_E['cargo']."</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($lancamento,'N','N','',20,100,'','')."</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalautorizado,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('A','".$lan."','".$nivel_E['lanid']."');\" title=\"Autorizados\">
			</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalconcrusados,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('P', '".$lan."','".$nivel_E['lanid']."');\" title=\"Concursados\">
			</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalprovido,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('E', '".$lan."','".$nivel_E['lanid']."');\" title=\"Provimentos\">
			</td>
		</tr>");
}
?>
<tr>
	<td style='background-color: #F0F0F0' align="right"><b>Totais:</b></td>	
	<td style='background-color: #F0F0F0' align="center"><?=$totais_E['lantotal'];?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_autorizado_2"><?=$totaisautorizados_E?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_provido_2"><?=$totaisprovido_E?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_concrusados_2"><?=$totaisconcrusados_E?></td>
</tr>

<!-- ********************* TA Nível B, C e D ********************** -->
<tr>
	<td colspan="5" class="SubTituloEsquerda">TA Nível B, C e D:</td>
</tr>
<tr background="#DCDCDC">
	<td class="SubTituloCentro" width="30%">Campus</td>
	<td class="SubTituloCentro">Projetado</td>
	<td class="SubTituloCentro">Autorizados</td>
	<td class="SubTituloCentro">Concursados</td>
	<td class="SubTituloCentro">Provimentos</td>
</tr>
<?php 
$sql_niveis_BCD = "SELECT  	
					lc.lanid,
					CASE  WHEN  lc.".$lan." is null THEN '0'	
					ELSE lc.".$lan."
					END as lancamento,
					CASE  WHEN  lc.".$lanauto." is null THEN '0'	
					ELSE lc.".$lanauto."					
					END as lancamentoauto,					
					cu.caudesc as campus,
					ca.cardesc as cargo,	
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'A' AND
						monstatus = 'A')
					 as totalautorizado,					
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'E' AND
						monstatus = 'A')
					 as totalprovido,
					(SELECT sum(monnumvagas)as qtd 
						FROM reuni.dadosmonitoramento 
						WHERE lanid = lc.lanid AND
						monano = '".$ano."' AND
						montipo = 'P' AND
						monstatus = 'A')
					 as totalconcrusados
			
				FROM 
					reuni.lancamentocargo 	AS lc						
					INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
					INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
					LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
				
				WHERE
					cl.limctpreg  = 'C' 	AND
					cu.unicod     = '".$unicod."' AND
					cu.unitpocod  = 'U' AND
					cl.nlcid      = 3
			
				ORDER BY
				cu.caudesc";
$niveis_BCD = $db->carregar($sql_niveis_BCD);

$sql_totais_BCD = "SELECT
					sum(l.".$lan.") as lantotal, 
					sum(l.".$lanauto.") as lanautototal
				FROM reuni.lancamentocargo l
				INNER JOIN 
					reuni.campusuniversitario c  ON c.cauid = l.cauid
				WHERE 
					unicod = '".$unicod."' AND l.nlcid = 3";

$totais_BCD = $db->pegaLinha($sql_totais_BCD);
$sql_totaisprovido_BCD = "SELECT sum(monnumvagas)as qtd 
					FROM 
					reuni.lancamentocargo 	AS lc						
					INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
					INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
					LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
					LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
					WHERE
					cl.limctpreg  = 'C' AND
					cu.unicod     = '".$unicod."'AND
					cu.unitpocod  = 'U'AND
					cl.nlcid      = 3 AND
					dm.monano = '".$ano."' AND
					dm.montipo = 'E' AND
					dm.monstatus = 'A'";
$totaisprovido_BCD = $db->pegaUm($sql_totaisprovido_BCD);
$sql_totaisconcrusados_BCD = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 3 AND
						dm.monano = '".$ano."' AND
						dm.montipo = 'P' AND
						dm.monstatus = 'A'";
$totaisconcrusados_BCD = $db->pegaUm($sql_totaisconcrusados_BCD);

$sql_totaisautorizados_BCD = "SELECT sum(monnumvagas)as qtd 
						FROM 
						reuni.lancamentocargo 	AS lc						
						INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
						INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
						LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
						LEFT JOIN reuni.dadosmonitoramento   AS dm ON dm.lanid = lc.lanid
						WHERE
						cl.limctpreg  = 'C' AND
						cu.unicod     = '".$unicod."'AND
						cu.unitpocod  = 'U'AND
						cl.nlcid      = 3 AND
						dm.monano = '".$ano."' AND
						dm.montipo = 'A' AND
						dm.monstatus = 'A'";
$totaisautorizados_BCD  = $db->pegaUm($sql_totaisautorizados_BCD);

$cor = "#F5F5F5";
foreach ($niveis_BCD as $nivel_BDC) {
	$lancamento 		= $lan."_".$nivel_BDC['lanid'];
	$lancamentoauto 	= $lanauto."[".$nivel_BDC['lanid']."]";
	${$lancamento} 		= $nivel_BDC['lancamento'];
	${$lancamentoauto} 	= $nivel_BDC['totalautorizado'];
	
	$totalautorizado 	 = "totalautorizado_".$nivel_E['lanid'];
	${$totalautorizado}  = $nivel_E['totalautorizado'];
	$totalprovido 		 = "totalprovido_".$nivel_BDC['lanid'];
	${$totalprovido} 	 = $nivel_BDC['totalprovido'];
	$totalconcrusados 	 = "totalconcrusados_".$nivel_BDC['lanid'];
	${$totalconcrusados} = $nivel_BDC['totalconcrusados'];
	$totais_autorizado 	 = "totais_autorizado_3";	
	
	$onkeyup = "validaAutorizacao(".$nivel_BCD['lancamento'].", this, ".$nivel_BDC['lancamentoauto'].");";
	$onblur = "atualizaTotalAuto(".$nivel_E['lancamento'].",this, ".$nivel_BDC['lancamentoauto'].",".$totais_autorizado.");";
	
	if($cor == "#E9E9E9") $cor = "#F5F5F5";else $cor = "#E9E9E9";	
	
	echo("<tr>
			<td style='background-color:".$cor."' align='left'>".$nivel_BDC['campus']." - ".$nivel_BDC['cargo']."</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($lancamento,'N','N','',20,100,'','')."</td>					
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalautorizado,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('A','".$lan."','".$nivel_BDC['lanid']."');\" title=\"Autorizados\">
			</td>			
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalconcrusados,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('P', '".$lan."','".$nivel_BDC['lanid']."');\" title=\"Concursados\">
			</td>
			<td style='background-color:".$cor."' align='center'>".campo_texto($totalprovido,'N','N','',20,100,'','')."
			<img style=\"cursor: pointer;\" src=\"/imagens/pop_p.gif \" border=0 onclick=\"abrirPopup('E', '".$lan."','".$nivel_BDC['lanid']."');\" title=\"Provimentos\">
			</td>			
		</tr>");
}
?>
<tr>
	<td style='background-color: #F0F0F0' align="right"><b>Totais:</b></td>	
	<td style='background-color: #F0F0F0' align="center"><?=$totais_BCD['lantotal'];?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_autorizado_3"><?=$totaisautorizados_BCD?></td>	
	<td style='background-color: #F0F0F0' align="center" id="totais_concrusados_3"><?=$totaisconcrusados_BCD?></td>
	<td style='background-color: #F0F0F0' align="center" id="totais_provido_3"><?=$totaisprovido_BCD?></td>
</tr>
<!--<tr bgcolor="#cccccc">
	      <td></td>
	      <td colspan="4"> 	  
	  	  	<input type="button" class="botao" name="btassociar" value="Salvar" onclick="submeter('A');">
	  	  </td>
 </tr>
--></table>

</form>
<script type="text/javascript"><!--

function submeter(evento){		
		document.getElementById('evento').value = evento;		
		document.formulario.submit();
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.submit();		
}

function abrirPopup(tipo, lan, id, autorizado){
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/listareditais&acao=A&tipo='+tipo+'&lan='+lan+'&lanid='+id,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	janela.focus();
}
function validaAutorizacao(projetado, autorizado, autoantigo){	
	
	if(projetado < Number(autorizado.value)){
		alert("Os valores Autorizados devem ser iguais ou menores que os valores Projetados.");
		autorizado.value = autoantigo;
		autorizado.focus;	
	}
}

function atualizaTotalAuto(projetado, autorizado, autoantigo, totais_autorizado){
	if(projetado > Number(autorizado.value)){
		var novo = (autoantigo - Number(autorizado.value));
		document.getElementById(totais_autorizado).innerHTML = Number(document.getElementById(totais_autorizado).innerHTML) - novo;
	}
}

--></script>