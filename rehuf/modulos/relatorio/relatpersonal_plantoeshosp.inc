<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center; 
}
.stylelinhas {
	background-color: #dcdcdc;
	text-align: right; 
}
.styleconteudo {
	text-align: left;
}
.titulounidade {
	background-color:#CDCDCD;
	text-align:center;
	font-weight: bold;
}
</style>
<?
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


$sql = array(
			 // Presencial dias úteis (nivel superior)
			 0 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND epltipo='PD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})",
			 // Presencial dias úteis (nivel medio)
			 1 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='M' AND epltipo='PD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})",
			 // Presencial final de semana e feriados (nivel superior)
			 2 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND epltipo='PF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})",
			 // Presencial final de semana e feriados (nivel medio)
			 3 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='M' AND epltipo='PF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})",
			 // Sobreaviso dias úteis (nivel superior)
			 4 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND epltipo='SD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})",
			 // PSobreaviso final de semana e feriados (nivel superior)
			 5 => "SELECT COUNT(*) FROM rehuf.escalaplantao esp 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND epltipo='SF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid})"
			 
			);
			  
			  
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ena.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");

echo "<h1>Relatório de plantões hospitalares 2009</h1>";
echo "<fieldset>
		<legend>Legenda</legend>
		<table width='100%'>
		<tr><td class='SubTituloDireita'><b>NM</b></td><td>Nível Médio</td></tr>
		<tr><td class='SubTituloDireita'><b>NS</b></td><td>Nível Superior</td></tr>
		<tr><td class='SubTituloDireita'><b>PD</b></td><td>Presencial dias úteis</td></tr>
		<tr><td class='SubTituloDireita'><b>PF</b></td><td>Presencial final de semana e feriados</td></tr>
		<tr><td class='SubTituloDireita'><b>SD</b></td><td>Sobreaviso dias úteis</td></tr>
		<tr><td class='SubTituloDireita'><b>SF</b></td><td>Sobreaviso final de semana e feriados</td></tr>
		</table>
	  </fieldset>";

echo "<table width='100%'>";
echo "<tr>";
echo "<td class='SubTituloCentro' rowspan=3>Hospital</td>";
echo "<td class='SubTituloCentro' rowspan=3>IFES</td>";
echo "<td class='SubTituloCentro' colspan=6>Outubro</td>";
echo "<td class='SubTituloCentro' colspan=6>Novembro</td>";
echo "<td class='SubTituloCentro' colspan=6>Dezembro</td>";
echo "<td class='SubTituloCentro' rowspan=3>Total</td>";
echo "</tr>";

echo "<tr>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "</tr>";

echo "<tr>";
echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";

echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";
echo "<td class='SubTituloCentro'>SD</td>";
echo "<td class='SubTituloCentro'>SF</td>";

echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";

echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";
echo "<td class='SubTituloCentro'>SD</td>";
echo "<td class='SubTituloCentro'>SF</td>";

echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";

echo "<td class='SubTituloCentro'>PD</td>";
echo "<td class='SubTituloCentro'>PF</td>";
echo "<td class='SubTituloCentro'>SD</td>";
echo "<td class='SubTituloCentro'>SF</td>";

echo "</tr>";
	
	
foreach($esuids as $esuid) {
	
	// _1_ representa o período de outubro de 2009
	$PD_M_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[1]));
	$PF_M_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[3]));
	
	$PD_S_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[0]));
	$PF_S_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[2]));
	$SD_S_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[4]));
	$SF_S_1 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-10'),$sql[5]));

	// _2_ representa o período de novembro de 2009
	$PD_M_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[1]));
	$PF_M_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[3]));
	
	$PD_S_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[0]));
	$PF_S_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[2]));
	$SD_S_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[4]));
	$SF_S_2 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-11'),$sql[5]));
	
	// _3_ representa o período de dezembro de 2009
	$PD_M_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[1]));
	$PF_M_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[3]));
	
	$PD_S_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[0]));
	$PF_S_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[2]));
	$SD_S_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[4]));
	$SF_S_3 = $db->pegaUm(str_replace(array("{entid}","{perid}"),array($esuid['entid'],'2009-12'),$sql[5]));
	
	$Tothospital = ($PD_M_1 + $PF_M_1 + $PD_S_1 + $PF_S_1 + $SD_S_1 + $SF_S_1) + ($PD_M_2 + $PF_M_2 + $PD_S_2 + $PF_S_2 + $SD_S_2 + $SF_S_2) + ($PD_M_3 + $PF_M_3 + $PD_S_3 + $PF_S_3 + $SD_S_3 + $SF_S_3);
	
	$Tot_PD_M_1 += $PD_M_1;
	$Tot_PF_M_1 += $PF_M_1;
	$Tot_PD_S_1 += $PD_S_1;
	$Tot_PF_S_1 += $PF_S_1;
	$Tot_SD_S_1 += $SD_S_1;
	$Tot_SF_S_1 += $SF_S_1;
	
	$Tot_PD_M_2 += $PD_M_2;
	$Tot_PF_M_2 += $PF_M_2;
	$Tot_PD_S_2 += $PD_S_2;
	$Tot_PF_S_2 += $PF_S_2;
	$Tot_SD_S_2 += $SD_S_2;
	$Tot_SF_S_2 += $SF_S_2;
	
	$Tot_PD_M_3 += $PD_M_3;
	$Tot_PF_M_3 += $PF_M_3;
	$Tot_PD_S_3 += $PD_S_3;
	$Tot_PF_S_3 += $PF_S_3;
	$Tot_SD_S_3 += $SD_S_3;
	$Tot_SF_S_3 += $SF_S_3;
	
	$Tot_Tothospital += $Tothospital;

	echo "<tr>";
	echo "<td>".$esuid['entnome']."</td>";
	echo "<td>".$esuid['entsig']."</td>";
	echo "<td align=right>".$PD_M_1."</td>";
	echo "<td align=right>".$PF_M_1."</td>";
	echo "<td align=right>".$PD_S_1."</td>";
	echo "<td align=right>".$PF_S_1."</td>";
	echo "<td align=right>".$SD_S_1."</td>";
	echo "<td align=right>".$SF_S_1."</td>";
	
	echo "<td align=right>".$PD_M_2."</td>";
	echo "<td align=right>".$PF_M_2."</td>";
	echo "<td align=right>".$PD_S_2."</td>";
	echo "<td align=right>".$PF_S_2."</td>";
	echo "<td align=right>".$SD_S_2."</td>";
	echo "<td align=right>".$SF_S_2."</td>";
	
	echo "<td align=right>".$PD_M_3."</td>";
	echo "<td align=right>".$PF_M_3."</td>";
	echo "<td align=right>".$PD_S_3."</td>";
	echo "<td align=right>".$PF_S_3."</td>";
	echo "<td align=right>".$SD_S_3."</td>";
	echo "<td align=right>".$SF_S_3."</td>";
	
	echo "<td align=right>".$Tothospital."</td>";
	unset($Tothospital);
	
	echo "</tr>";
}
echo "<tr>";
echo "<td colspan=2 align=right><b>Total:</b></td>";
echo "<td align=right>".$Tot_PD_M_1."</td>";
echo "<td align=right>".$Tot_PF_M_1."</td>";
echo "<td align=right>".$Tot_PD_S_1."</td>";
echo "<td align=right>".$Tot_PF_S_1."</td>";
echo "<td align=right>".$Tot_SD_S_1."</td>";
echo "<td align=right>".$Tot_SF_S_1."</td>";

echo "<td align=right>".$Tot_PD_M_2."</td>";
echo "<td align=right>".$Tot_PF_M_2."</td>";
echo "<td align=right>".$Tot_PD_S_2."</td>";
echo "<td align=right>".$Tot_PF_S_2."</td>";
echo "<td align=right>".$Tot_SD_S_2."</td>";
echo "<td align=right>".$Tot_SF_S_2."</td>";

echo "<td align=right>".$Tot_PD_M_3."</td>";
echo "<td align=right>".$Tot_PF_M_3."</td>";
echo "<td align=right>".$Tot_PD_S_3."</td>";
echo "<td align=right>".$Tot_PF_S_3."</td>";
echo "<td align=right>".$Tot_SD_S_3."</td>";
echo "<td align=right>".$Tot_SF_S_3."</td>";

echo "<td align=right>".$Tot_Tothospital."</td>";

echo "</tr>";

echo "</table>";

?>