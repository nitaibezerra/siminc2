<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center;
	font-weight: bold;
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
$sql = array(// Pegar linha
			 0 => "SELECT SUM(ctivalor) FROM rehuf.conteudoitem cdi 
				   LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid 
				   WHERE cdi.esuid IN('{esuid}') AND lin.linid IN('{linid}') AND cdi.colid='329' AND cdi.ctiexercicio='{ano}'"
			);
			  

$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao, esu.esutipo FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig");

$_ano = array('2008','2009');
$_esutipo = array("E" => "Especialidade", "G" => "Geral", "M" => "Maternidade");


foreach($_ano as $ano) {
	echo "<h1>Relatório de Procedimentos de FAEC (".$ano.")</h1>";
	echo "<table width='100%' class='tabela'>";
	$agrupadores = $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid='76' ORDER BY agpordem");
	if($agrupadores) {
		$lagp = "<tr>";
		$lagp .= "<td rowspan='2' class='SubTituloCentro'>IFES</td>";
		$lagp .= "<td rowspan='2' class='SubTituloCentro'>Hospital</td>";
		$llin = "<tr>";
		foreach($agrupadores as $agp) {
			$linhas[$agp['agpid']] = $db->carregar("SELECT * FROM rehuf.linha WHERE agpid='".$agp['agpid']."' ORDER BY linordem");
			if($linhas[$agp['agpid']][0]) {
				foreach($linhas[$agp['agpid']] as $lin) {
					$llin .= "<td nowrap class='stylecolunas'>".$lin['lindsc']."</td>";
				}
			} else {
				unset($linhas[$agp['agpid']]);
			}
			$lagp .= "<td colspan='".count($linhas[$agp['agpid']])."' class='SubTituloCentro'>".$agp['agpdsc']."</td>";
		}
		$llin .= "</tr>";
		$lagp .= "</tr>";
	}
	echo $lagp;
	echo $llin;
	
	foreach($esuids as $esuid) {
		echo "<tr>";
		if($agrupadores) {
			echo "<td>".$esuid['entsig']."</td>";
			echo "<td nowrap >".$esuid['entnome']."</td>";
			foreach($agrupadores as $agp) {
				if($linhas[$agp['agpid']][0]) {
					foreach($linhas[$agp['agpid']] as $lin) {
						echo "<td align='right'>".$db->pegaUm(str_replace(array("{esuid}","{ano}","{linid}"),array($esuid['esuid'],$ano,$lin['linid']),$sql[0]))."</td>";
					}
				} else {
					unset($linhas[$agp['agpid']]);
				}
			}
		}
		echo "</tr>";
	}
	echo "</table>";

}

?>