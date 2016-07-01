<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<?

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$permissoes = verificaPerfilRehuf();

$sql = "SELECT ent.entid, UPPER(ent.entnome)||' ('||ena.entsig||')' as entnome FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid=ent.entid
		LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
		LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid
		LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid=ena.entid 
		WHERE fen.funid='".HOSPITALUNIV."' AND fen2.funid='12' AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') ".(($_REQUEST['entid'][0])?"AND ent.entid IN('".implode("','", $_REQUEST['entid'])."')":"");
$hospitais = $db->carregar($sql);

$sql = "SELECT to_char(ppldata, 'YYYY-mm') as ppldata, to_char(ppldata, 'mm/YYYY') as ppldata2 FROM rehuf.periodoplantao WHERE ".(($_REQUEST['pplid'][0])?" pplid IN('".implode("','", $_REQUEST['pplid'])."') AND pplstatus='A'":"pplstatus='A'")." ORDER BY ppldata";
$periodos = $db->carregar($sql);

$sql = "SELECT * FROM rehuf.cargoplantao";
$cargos = $db->carregar($sql);

if($hospitais[0]) {
	// Legenda estatica, implementação no futuro (montar dinamica)
	if($_REQUEST['somentetotalizadores'] != "sim") {
		echo "<fieldset>
			  <legend>Instruções</legend>
			  <table class=\"tabela\" style=\"width:100%\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\">
			  		<tr><td class=SubTituloEsquerda colspan=4><font size=1>Instruções para impressão: 1 - Acesse o menu Arquivo / Configurar página, 2 - Clique no formato paisagem e na opção \"Imprimir cores e imagens do plano de fundo\", 3 - Configure as margens para 0mm, 4 - Retire as configurações de rodapé e cabeçalho, 5 - Clique no botão OK</font></td></tr>
					<tr><td class=SubTituloCentro colspan=2>Nível Superior</td><td class=SubTituloCentro colspan=2>Nível Médio</td></tr>
					<tr><td class=SubTituloCentro colspan=4>Tipo plantão</td></tr>
					<tr><td class=SubTituloDireita><font size=1>PN</font></td><td><font size=1>Plantão normal</font></td><td class=SubTituloDireita><font size=1>PN</font></td><td><font size=1>Plantão normal</font></td></tr>
					<tr><td class=SubTituloDireita><font size=1>PD</font></td><td><font size=1>Presencial dias úteis</font></td><td class=SubTituloDireita><font size=1>PD</font></b></td><td><font size=1>Presencial dias úteis</font></td></tr>
					<tr><td class=SubTituloDireita><font size=1>PF</font></td><td><font size=1>Presencial final de semana e feriados</font></td><td class=SubTituloDireita><font size=1>PF</font></b></td><td><font size=1>Presencial final de semana e feriados</font></td></tr>
					<tr><td class=SubTituloDireita><font size=1>SD</font></td><td><font size=1>Sobreaviso dias úteis</font></td><td colspan=2>&nbsp;</td></tr>
					<tr><td class=SubTituloDireita><font size=1>SF</font></td><td><font size=1>Sobreaviso final de semana e feriados</font></td><td colspan=2>&nbsp;</td></tr>
					<tr><td class=SubTituloCentro colspan=4>Cargos</td></tr>
					<tr><td class=SubTituloDireita><font size=1>S2</font></td><td><font size=1>Enfermeiro</font></td><td class=SubTituloDireita><font size=1>M1</font></td><td><font size=1>Auxiliar de Enfermagem</font></td></tr>
					<tr><td class=SubTituloDireita><font size=1>S3</font></td><td><font size=1>Médico</font></td><td class=SubTituloDireita><font size=1>M4</font></td><td><font size=1>Técnico em Enfermagem</font></td></tr>
			  </table>
			  </fieldset>
			  <hr style='width:100%'>";
	}
	
	
	foreach($hospitais as $hos) {
		
		$sql = "SELECT s.setid, UPPER(s.setnome) as setnome FROM rehuf.setorplantao s 
				INNER JOIN rehuf.setorhospitalplantao sp ON sp.setid=s.setid 
				WHERE sp.entid='".$hos['entid']."' ".(($_REQUEST['setid'][0])?"AND s.setid IN('".implode("','", $_REQUEST['setid'])."')":"");
		$setores = $db->carregar($sql);
		
		if($setores[0]) {
			foreach($setores as $set) {
				
				if($periodos[0]) {
					foreach($periodos as $per) {
						echo "<table class=\"tabela\" style=\"width:100%\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\">
								<tr>
									<td class=\"SubTituloCentro\" style=\"width:33%\"><b>HOSPITAL :</b> ".$hos['entnome']."</td>
									<td class=\"SubTituloCentro\" style=\"width:33%\"><b>SETOR :</b> ".$set['setnome']."</td>
									<td class=\"SubTituloCentro\" style=\"width:33%\"><b>PERÍODO :</b> ".$per['ppldata2']."</td>
								</tr>
							  </table>";
						
						$peri = explode("-", $per['ppldata']);
						$dadosgrid['ano'] = $peri[0];
						$dadosgrid['mes'] = $peri[1];
						$dadosgrid['setid'] = $set['setid'];
						$dadosgrid['entid'] = $hos['entid'];
						$dadosgrid['somentetotalizadores'] = $_REQUEST['somentetotalizadores'];
						echo gridPlantaoRelatorio($dadosgrid);
						echo "<hr style='width:100%'>";
						
						if($_TOTALPER) {
							foreach($_TOTALPER as $ni => $arrr) {
								foreach($arrr as $ca => $arr) {
									foreach($arr as $ar) {
										$_TOTALHOSP += $ar;
									}
								}
							}
						}
						unset($_TOTALPER);
					}
				}
			}
		}
		echo "<table class=\"tabela\" style=\"width:100%\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\" align=\"center\">
					<tr><td class=SubTituloDireita><b>".$hos['entnome']." TOTAL:</b></td><td>".$_TOTALHOSP."</td></tr>
			  </table>";
		unset($_TOTALHOSP);
	}
}


?>