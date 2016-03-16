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
$filtro = (($_REQUEST['entid'][0])?" AND ent.entid IN ('".implode("','",$_REQUEST['entid'])."')":"");
$esuids = $db->carregar("SELECT esu.esuid, ent.entnome, ent.entid, ena.entsig, ende.estuf, mundescricao FROM rehuf.estruturaunidade esu
						 LEFT JOIN entidade.entidade ent ON ent.entid=esu.entid 
						 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid  
			 			 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
						 LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
						 LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			 			 WHERE fen.funid = '". HOSPITALUNIV ."' AND 
			 			 (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)".$filtro);
echo "<table width='tabela'>";

// carregando os valores
$sql = "SELECT * FROM rehuf.indicadorvalor";
$indicadorvalores = $db->carregar($sql);
// Pegando dados da TABELA (pai de toda a estrutura)
$indicadores = $db->carregar("SELECT indid, inddsc, indarquivo, indformula, indanoini, indanofim FROM rehuf.indicador WHERE indid IN('". implode("','",$_REQUEST['agrupador']) ."') ORDER BY dimid, indid");

foreach($esuids as $esuid) {
	$_SESSION['rehuf_var']['esuid'] = $esuid['esuid']; 
	echo "<tr><td colspan='2'>";
	echo "<table bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' width='95%' style='border-top:2px solid #000000;border-bottom:2px solid #000000;'>
			<tr><td class='SubTituloDireita'>Hospital :</td><td><b>".$esuid['entnome']."</b></td></tr>
			<tr><td class='SubTituloDireita'>IFES :</td><td><b>".$esuid['entsig']."</b></td></tr>
			<tr><td class='SubTituloDireita'>UF / Munícipio :</td><td><b>".$esuid['estuf']." / ".$esuid['mundescricao']."</b></td></tr>
		  </table>";
	echo "</td></tr>";
	
	// 	Verifica se existe a Indicadores
	echo "<tr><td colspan='2'>";
	if($indicadores) {
		foreach($indicadores as $arquivo) {
			// pegar da tabela indicadores
			unset($_anos);
			for($i = $arquivo['indanoini'];$i<=$arquivo['indanofim'];$i++) {
				$_anos[] = $i;
			}
			// pegar o subitens do indicador
			$sql = "SELECT * FROM rehuf.indicadorsubitem WHERE indid='".$arquivo['indid']."' ORDER BY sinordem";
			$subitensindicadores = $db->carregar($sql);
			if($indicadorvalores) {
				foreach($indicadorvalores as $valores) {
					$indvalores["{valor".$valores['invid']."}"] = $valores['invselect'];
				}
			}
			
			echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
			if($_anos) {
				echo "<tr>
					  <td class=\"SubTituloCentro\" width=\"400\">".$arquivo['inddsc'];
				if($arquivo['indformula']) {
					echo "<br /><font size=1>Fórmula: ".$arquivo['indformula']."</font>";
				}
				echo "</td>";
				foreach($_anos as $ano) {
					echo "<td class=\"SubTituloCentro\">".$ano."</td>";
				}
				echo "</tr>"; 
			}
			
			if($subitensindicadores) {
				foreach($subitensindicadores as $subitensindicador) {
					echo "<tr>
						  <td class=\"SubTituloDireita\">".$subitensindicador['sindsc']."</td>";
					foreach($_anos as $ano) {
						$indices = array_keys($indvalores);
						$sql_formula = str_replace($indices, $indvalores, $subitensindicador['sinformula']);
						$sql_formula = str_replace(array("{esuid}","{ano}"), array($_SESSION['rehuf_var']['esuid'], $ano), $sql_formula);
						if($sql_formula) {
							$resultado = $db->pegaUm("SELECT (".$sql_formula.")");
						}
						echo "<td align='right'>".number_format($resultado, 2, ',', '')."</td>";
					}
					echo "</tr>";
				}
			}
			echo "</table>";
		}
	}
	echo "</td></tr>";
}

echo "</table>";

?>