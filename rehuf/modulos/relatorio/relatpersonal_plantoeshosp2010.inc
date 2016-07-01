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
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

if($_REQUEST['carid'][0]) {
	$filtro .= " AND car.carid IN(".implode(',',$_REQUEST['carid']).")";
	$carid = $db->carregar("SELECT carid as codigo, carnome as descricao FROM rehuf.cargoplantao WHERE carid IN(".implode(',',$_REQUEST['carid']).")");
}

if($_REQUEST['setid'][0]) {
	$filtro .= " AND esp.setid IN(".implode(',',$_REQUEST['setid']).")";
	$setid = $db->carregar("SELECT setid as codigo, setnome as descricao FROM rehuf.setorplantao WHERE setid IN(".implode(',',$_REQUEST['setid']).")");
}


$sql = array(
			 // Presencial dias úteis (nivel superior)
			 0 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid 
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='PD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro} 
				   GROUP BY esp.entid",
			 // Presencial dias úteis (nivel medio)
			 1 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='M' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='PD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro} 
				   GROUP BY esp.entid",
			 // Presencial final de semana e feriados (nivel superior)
			 2 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='PF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro}
				   GROUP BY esp.entid",
			 // Presencial final de semana e feriados (nivel medio)
			 3 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='M' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='PF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro}
				   GROUP BY esp.entid",
			 // Sobreaviso dias úteis (nivel superior)
			 4 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='SD' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro}
				   GROUP BY esp.entid",
			 // PSobreaviso final de semana e feriados (nivel superior)
			 5 => "SELECT COUNT(*), esp.entid FROM rehuf.escalaplantao esp 
			 	   INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=esp.setid  AND sp.entid=esp.entid
				   LEFT JOIN rehuf.funcionarioplantao func ON func.fcoid = esp.fcoid 
				   LEFT JOIN rehuf.cargoplantao car ON car.carid = func.carid 
				   WHERE carnivel='S' AND func.fcostatus='A' AND car.carstatus='A' AND epltipo='SF' AND to_char(epldata, 'YYYY-mm')= '{perid}' AND esp.entid IN({entid}) {filtro}
				   GROUP BY esp.entid"
			 
			);
			  
			  
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ena.entid 
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");


echo "<h1>Relatório de plantões hospitalares 2010</h1>";


echo "<script>
		function exibirRelatorio() {
			var formulario = document.formulario;
			selectAllOptions( document.getElementById( 'carid' ) );
			selectAllOptions( document.getElementById( 'setid' ) );
			formulario.submit();
		}
	  </script>
	  <form name=formulario method=post action='rehuf.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_plantoeshosp2010.inc'>
	  <fieldset>
	  <legend>Filtros</legend>
	  <table width='100%'>
	  <tr>
	  <td class=SubTituloDireita valign=top>Setores :</td>
	  <td>";

$sqlSetores = "SELECT setid as codigo, setnome as descricao FROM rehuf.setorplantao";
combo_popup( "setid", $sqlSetores, "Setores", "192x400", 0, array(), "", "S", false, false, 5, 400 );				

echo "</td>
	  <td class=SubTituloDireita valign=top>Cargos :</td><td>";

$sqlCargos = "SELECT carid as codigo, carnome as descricao FROM rehuf.cargoplantao WHERE carstatus='A'";
combo_popup( "carid", $sqlCargos, "Cargos", "192x400", 0, array(), "", "S", false, false, 5, 400 );				

echo "</td>
	  </tr>
	  <tr>
	  <td colspan=4><input type=button value=Ok onclick=exibirRelatorio();></td>
	  </tr>
	  </table>
	  </fieldset>
	  </form>";


echo "<fieldset>
		<legend>Legenda</legend>
		<table width='100%'>
		<tr><td class='SubTituloDireita'><b>NM</b></td><td>Nível Médio</td><td class='SubTituloDireita'><b>NS</b></td><td>Nível Superior</td></tr>
		<tr><td class='SubTituloDireita'><b>PD</b></td><td>Presencial dias úteis</td><td class='SubTituloDireita'><b>PF</b></td><td>Presencial final de semana e feriados</td></tr>
		<tr><td class='SubTituloDireita'><b>SD</b></td><td>Sobreaviso dias úteis</td><td class='SubTituloDireita'><b>SF</b></td><td>Sobreaviso final de semana e feriados</td></tr>
		</table>
	  </fieldset>";

echo "<style>
		.borda_plantao
		{
			BORDER-RIGHT: #000000 1px solid;
			BORDER-TOP: #000000 1px solid;
			BORDER-LEFT: #000000 1px solid;
			BORDER-BOTTOM: #000000 1px solid;
		}
	  </style>";

echo "<table width='100%'>";
echo "<tr>";
echo "<td class='SubTituloCentro' rowspan=3>Hospital</td>";
echo "<td class='SubTituloCentro' rowspan=3>IFES</td>";
echo "<td class='SubTituloCentro' colspan=6>Janeiro</td>";
echo "<td class='SubTituloCentro' colspan=6>Fevereiro</td>";
echo "<td class='SubTituloCentro' colspan=6>Março</td>";
echo "<td class='SubTituloCentro' colspan=6>Abril</td>";
echo "<td class='SubTituloCentro' colspan=6>Maio</td>";
echo "<td class='SubTituloCentro' colspan=6>Junho</td>";
echo "<td class='SubTituloCentro' colspan=6>Julho</td>";
echo "<td class='SubTituloCentro' colspan=6>Agosto</td>";
echo "<td class='SubTituloCentro' colspan=6>Setembro</td>";
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
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
echo "<td class='SubTituloCentro' colspan=2>NM</td>";
echo "<td class='SubTituloCentro' colspan=4>NS</td>";
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

$sqlp = "SELECT to_char(ppldata, 'YYYY-mm') as data FROM rehuf.periodoplantao WHERE pplstatus='A'";
$periodos = $db->carregar($sqlp);

if($periodos[0]) {
	foreach($periodos as $peri) {
		$_PERIODOSEX[$peri['data']] = true;
		$_PERIODOSPL[] = $peri['data'];
	}
}

if($esuids[0]) {
	foreach($esuids as $esu) {
		$es[] = $esu['entid'];
	}
}

for($i=1;$i<=12;$i++) {
	
	if($_PERIODOSEX['2010-'.sprintf("%02d", $i)]) {
		$PD_M_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[1]));
		$dados[$v['entid']]['PD_M_'.$i]='0';
		if($PD_M_arr[0]) {
			foreach($PD_M_arr as $v) {
				$dados[$v['entid']]['PD_M_'.$i] = $v['count'];
			}
		}
		$PF_M_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[3]));
		$dados[$v['entid']]['PF_M_'.$i]='0';
		if($PF_M_arr[0]) {
			foreach($PF_M_arr as $v) {
				$dados[$v['entid']]['PF_M_'.$i] = $v['count'];
			}
		}	
		
		$PD_S_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[0]));
		$dados[$v['entid']]['PD_S_'.$i]='0';
		if($PD_S_arr[0]) {
			foreach($PD_S_arr as $v) {
				$dados[$v['entid']]['PD_S_'.$i] = $v['count'];
			}
		}	
		
		$PF_S_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[2]));
		$dados[$v['entid']]['PF_S_'.$i]='0';
		if($PF_S_arr[0]) {
			foreach($PF_S_arr as $v) {
				$dados[$v['entid']]['PF_S_'.$i] = $v['count'];
			}
		}		
		
		$SD_S_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[4]));
		$dados[$v['entid']]['SD_S_'.$i]='0';
		if($SD_S_arr[0]) {
			foreach($SD_S_arr as $v) {
				$dados[$v['entid']]['SD_S_'.$i] = $v['count'];
			}
		}		
				
		$SF_S_arr = $db->carregar(str_replace(array("{entid}","{perid}","{filtro}"),array(implode(",",$es),'2010-'.sprintf("%02d", $i),$filtro),$sql[5]));
		$dados[$v['entid']]['SF_S_'.$i]='0';
		if($SF_S_arr[0]) {
			foreach($SF_S_arr as $v) {
				$dados[$v['entid']]['SF_S_'.$i] = $v['count'];
			}
		}		
		
	}

}
	
foreach($esuids as $esuid) {

	
	$Tothospital  = ($dados[$esuid['entid']]['PD_M_1'] + $dados[$esuid['entid']]['PF_M_1'] + $dados[$esuid['entid']]['PD_S_1'] + $dados[$esuid['entid']]['PF_S_1'] + $dados[$esuid['entid']]['SD_S_1'] + $dados[$esuid['entid']]['SF_S_1']) + ($dados[$esuid['entid']]['PD_M_2'] + $dados[$esuid['entid']]['PF_M_2'] + $dados[$esuid['entid']]['PD_S_2'] + $dados[$esuid['entid']]['PF_S_2'] + $dados[$esuid['entid']]['SD_S_2'] + $dados[$esuid['entid']]['SF_S_2']) + ($dados[$esuid['entid']]['PD_M_3'] + $dados[$esuid['entid']]['PF_M_3'] + $dados[$esuid['entid']]['PD_S_3'] + $dados[$esuid['entid']]['PF_S_3'] + $dados[$esuid['entid']]['SD_S_3'] + $dados[$esuid['entid']]['SF_S_3']);
	$Tothospital += ($dados[$esuid['entid']]['PD_M_4'] + $dados[$esuid['entid']]['PF_M_4'] + $dados[$esuid['entid']]['PD_S_4'] + $dados[$esuid['entid']]['PF_S_4'] + $dados[$esuid['entid']]['SD_S_4'] + $dados[$esuid['entid']]['SF_S_4']) + ($dados[$esuid['entid']]['PD_M_5'] + $dados[$esuid['entid']]['PF_M_5'] + $dados[$esuid['entid']]['PD_S_5'] + $dados[$esuid['entid']]['PF_S_5'] + $dados[$esuid['entid']]['SD_S_5'] + $dados[$esuid['entid']]['SF_S_5']) + ($dados[$esuid['entid']]['PD_M_6'] + $dados[$esuid['entid']]['PF_M_6'] + $dados[$esuid['entid']]['PD_S_6'] + $dados[$esuid['entid']]['PF_S_6'] + $dados[$esuid['entid']]['SD_S_6'] + $dados[$esuid['entid']]['SF_S_6']);
	$Tothospital += ($dados[$esuid['entid']]['PD_M_7'] + $dados[$esuid['entid']]['PF_M_7'] + $dados[$esuid['entid']]['PD_S_7'] + $dados[$esuid['entid']]['PF_S_7'] + $dados[$esuid['entid']]['SD_S_7'] + $dados[$esuid['entid']]['SF_S_7']) + ($dados[$esuid['entid']]['PD_M_8'] + $dados[$esuid['entid']]['PF_M_8'] + $dados[$esuid['entid']]['PD_S_8'] + $dados[$esuid['entid']]['PF_S_8'] + $dados[$esuid['entid']]['SD_S_8'] + $dados[$esuid['entid']]['SF_S_8']) + ($dados[$esuid['entid']]['PD_M_9'] + $dados[$esuid['entid']]['PF_M_9'] + $dados[$esuid['entid']]['PD_S_9'] + $dados[$esuid['entid']]['PF_S_9'] + $dados[$esuid['entid']]['SD_S_9'] + $dados[$esuid['entid']]['SF_S_9']);
	$Tothospital += ($dados[$esuid['entid']]['PD_M_10'] + $dados[$esuid['entid']]['PF_M_10'] + $dados[$esuid['entid']]['PD_S_10'] + $dados[$esuid['entid']]['PF_S_10'] + $dados[$esuid['entid']]['SD_S_10'] + $dados[$esuid['entid']]['SF_S_10']) + ($dados[$esuid['entid']]['PD_M_11'] + $dados[$esuid['entid']]['PF_M_11'] + $dados[$esuid['entid']]['PD_S_11'] + $dados[$esuid['entid']]['PF_S_11'] + $dados[$esuid['entid']]['SD_S_11'] + $dados[$esuid['entid']]['SF_S_11']) + ($dados[$esuid['entid']]['PD_M_12'] + $dados[$esuid['entid']]['PF_M_12'] + $dados[$esuid['entid']]['PD_S_12'] + $dados[$esuid['entid']]['PF_S_12'] + $dados[$esuid['entid']]['SD_S_12'] + $dados[$esuid['entid']]['SF_S_12']);
	
	$Tot_PD_M_1 += $dados[$esuid['entid']]['PD_M_1'];
	$Tot_PF_M_1 += $dados[$esuid['entid']]['PF_M_1'];
	$Tot_PD_S_1 += $dados[$esuid['entid']]['PD_S_1'];
	$Tot_PF_S_1 += $dados[$esuid['entid']]['PF_S_1'];
	$Tot_SD_S_1 += $dados[$esuid['entid']]['SD_S_1'];
	$Tot_SF_S_1 += $dados[$esuid['entid']]['SF_S_1'];
	
	$Tot_PD_M_2 += $dados[$esuid['entid']]['PD_M_2'];
	$Tot_PF_M_2 += $dados[$esuid['entid']]['PF_M_2'];
	$Tot_PD_S_2 += $dados[$esuid['entid']]['PD_S_2'];
	$Tot_PF_S_2 += $dados[$esuid['entid']]['PF_S_2'];
	$Tot_SD_S_2 += $dados[$esuid['entid']]['SD_S_2'];
	$Tot_SF_S_2 += $dados[$esuid['entid']]['SF_S_2'];
	
	$Tot_PD_M_3 += $dados[$esuid['entid']]['PD_M_3'];
	$Tot_PF_M_3 += $dados[$esuid['entid']]['PF_M_3'];
	$Tot_PD_S_3 += $dados[$esuid['entid']]['PD_S_3'];
	$Tot_PF_S_3 += $dados[$esuid['entid']]['PF_S_3'];
	$Tot_SD_S_3 += $dados[$esuid['entid']]['SD_S_3'];
	$Tot_SF_S_3 += $dados[$esuid['entid']]['SF_S_3'];
	
	$Tot_PD_M_4 += $dados[$esuid['entid']]['PD_M_4'];
	$Tot_PF_M_4 += $dados[$esuid['entid']]['PF_M_4'];
	$Tot_PD_S_4 += $dados[$esuid['entid']]['PD_S_4'];
	$Tot_PF_S_4 += $dados[$esuid['entid']]['PF_S_4'];
	$Tot_SD_S_4 += $dados[$esuid['entid']]['SD_S_4'];
	$Tot_SF_S_4 += $dados[$esuid['entid']]['SF_S_4'];
	
	$Tot_PD_M_5 += $dados[$esuid['entid']]['PD_M_5'];
	$Tot_PF_M_5 += $dados[$esuid['entid']]['PF_M_5'];
	$Tot_PD_S_5 += $dados[$esuid['entid']]['PD_S_5'];
	$Tot_PF_S_5 += $dados[$esuid['entid']]['PF_S_5'];
	$Tot_SD_S_5 += $dados[$esuid['entid']]['SD_S_5'];
	$Tot_SF_S_5 += $dados[$esuid['entid']]['SF_S_5'];
	
	$Tot_PD_M_6 += $dados[$esuid['entid']]['PD_M_6'];
	$Tot_PF_M_6 += $dados[$esuid['entid']]['PF_M_6'];
	$Tot_PD_S_6 += $dados[$esuid['entid']]['PD_S_6'];
	$Tot_PF_S_6 += $dados[$esuid['entid']]['PF_S_6'];
	$Tot_SD_S_6 += $dados[$esuid['entid']]['SD_S_6'];
	$Tot_SF_S_6 += $dados[$esuid['entid']]['SF_S_6'];

	$Tot_PD_M_7 += $dados[$esuid['entid']]['PD_M_7'];
	$Tot_PF_M_7 += $dados[$esuid['entid']]['PF_M_7'];
	$Tot_PD_S_7 += $dados[$esuid['entid']]['PD_S_7'];
	$Tot_PF_S_7 += $dados[$esuid['entid']]['PF_S_7'];
	$Tot_SD_S_7 += $dados[$esuid['entid']]['SD_S_7'];
	$Tot_SF_S_7 += $dados[$esuid['entid']]['SF_S_7'];
	
	$Tot_PD_M_8 += $dados[$esuid['entid']]['PD_M_8'];
	$Tot_PF_M_8 += $dados[$esuid['entid']]['PF_M_8'];
	$Tot_PD_S_8 += $dados[$esuid['entid']]['PD_S_8'];
	$Tot_PF_S_8 += $dados[$esuid['entid']]['PF_S_8'];
	$Tot_SD_S_8 += $dados[$esuid['entid']]['SD_S_8'];
	$Tot_SF_S_8 += $dados[$esuid['entid']]['SF_S_8'];
	
	$Tot_PD_M_9 += $dados[$esuid['entid']]['PD_M_9'];
	$Tot_PF_M_9 += $dados[$esuid['entid']]['PF_M_9'];
	$Tot_PD_S_9 += $dados[$esuid['entid']]['PD_S_9'];
	$Tot_PF_S_9 += $dados[$esuid['entid']]['PF_S_9'];
	$Tot_SD_S_9 += $dados[$esuid['entid']]['SD_S_9'];
	$Tot_SF_S_9 += $dados[$esuid['entid']]['SF_S_9'];
	
	$Tot_PD_M_10 += $dados[$esuid['entid']]['PD_M_10'];
	$Tot_PF_M_10 += $dados[$esuid['entid']]['PF_M_10'];
	$Tot_PD_S_10 += $dados[$esuid['entid']]['PD_S_10'];
	$Tot_PF_S_10 += $dados[$esuid['entid']]['PF_S_10'];
	$Tot_SD_S_10 += $dados[$esuid['entid']]['SD_S_10'];
	$Tot_SF_S_10 += $dados[$esuid['entid']]['SF_S_10'];
	
	$Tot_PD_M_11 += $dados[$esuid['entid']]['PD_M_11'];
	$Tot_PF_M_11 += $dados[$esuid['entid']]['PF_M_11'];
	$Tot_PD_S_11 += $dados[$esuid['entid']]['PD_S_11'];
	$Tot_PF_S_11 += $dados[$esuid['entid']]['PF_S_11'];
	$Tot_SD_S_11 += $dados[$esuid['entid']]['SD_S_11'];
	$Tot_SF_S_11 += $dados[$esuid['entid']]['SF_S_11'];
	
	$Tot_PD_M_12 += $dados[$esuid['entid']]['PD_M_12'];
	$Tot_PF_M_12 += $dados[$esuid['entid']]['PF_M_12'];
	$Tot_PD_S_12 += $dados[$esuid['entid']]['PD_S_12'];
	$Tot_PF_S_12 += $dados[$esuid['entid']]['PF_S_12'];
	$Tot_SD_S_12 += $dados[$esuid['entid']]['SD_S_12'];
	$Tot_SF_S_12 += $dados[$esuid['entid']]['SF_S_12'];
	
	$Tot_Tothospital += $Tothospital;
    
	echo "<tr>";
	echo "<td nowrap>".$esuid['entnome']."</td>";
	echo "<td>".$esuid['entsig']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_1']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_1']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_1']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_1']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_1']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_1']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_2']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_2']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_2']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_2']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_2']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_2']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_3']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_3']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_3']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_3']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_3']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_3']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_4']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_4']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_4']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_4']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_4']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_4']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_5']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_5']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_5']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_5']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_5']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_5']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_6']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_6']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_6']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_6']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_6']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_6']."</td>";
		
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_7']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_7']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_7']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_7']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_7']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_7']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_8']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_8']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_8']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_8']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_8']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_8']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_9']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_9']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_9']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_9']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_9']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_9']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_10']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_10']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_10']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_10']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_10']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_10']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_11']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_11']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_11']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_11']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_11']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_11']."</td>";
	
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_M_12']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_M_12']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PD_S_12']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['PF_S_12']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SD_S_12']."</td>";
	echo "<td align=right class=borda_plantao>".$dados[$esuid['entid']]['SF_S_12']."</td>";
	
	echo "<td align=right class=borda_plantao>".$Tothospital."</td>";
	unset($Tothospital);
	
	echo "</tr>";
}
echo "<tr>";
echo "<td colspan=2 align=right><b>Total:</b></td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_M_1."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_1."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_1."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_1."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_1."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_1."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_2."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_2."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_2."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_2."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_2."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_2."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_3."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_3."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_3."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_3."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_3."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_3."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_4."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_4."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_4."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_4."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_4."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_4."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_5."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_5."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_5."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_5."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_5."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_5."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_6."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_6."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_6."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_6."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_6."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_6."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_7."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_7."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_7."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_7."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_7."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_7."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_8."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_8."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_8."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_8."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_8."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_8."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_9."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_9."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_9."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_9."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_9."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_9."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_10."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_10."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_10."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_10."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_10."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_10."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_11."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_11."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_11."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_11."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_11."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_11."</td>";

echo "<td align=right class=borda_plantao>".$Tot_PD_M_12."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_M_12."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PD_S_12."</td>";
echo "<td align=right class=borda_plantao>".$Tot_PF_S_12."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SD_S_12."</td>";
echo "<td align=right class=borda_plantao>".$Tot_SF_S_12."</td>";

echo "<td align=right class=borda_plantao>".$Tot_Tothospital."</td>";

echo "</tr>";

echo "</table>";

$tx = number_format( ( getmicrotime() - $Tinicio ), 4, ',', '.' );
echo "<p>".$tx." segundos</p>";

?>