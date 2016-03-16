<?
function mostrar_profissional($filtroSQL = false) {
	global $db;
	$sql =  "SELECT edc.estuf, 
					mun.mundescricao, 
					mun.muncod, 
					ent.entnome, 
					uor.entsig, 
				 	cam.cmpexistencia, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_OFERTATUAL_PROF."') AS matatual,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREVISTA_PROF."' AND cmi.cpiano = '2010') AS ano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREVISTA_PROF."' ORDER BY cmi.cpiano DESC LIMIT 1) AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 cam.cmpdataimplantacao,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."') AS insprevisto,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."') AS insrealizado
		  FROM academico.campus cam 
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
		  LEFT JOIN entidade.endereco edc ON ent.entid = edc.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY edc.estuf, uor.entsig, ent.entnome";
	$dadoscsv = $db->carregar($sql);
	if($dadoscsv[0]) {
		$_cmpexistencia = array("N"=>"Novo","P"=>"Pré-Existente");
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
		
		$xls = new GeraExcel();
		$xls->MontaConteudoString(0, 0, "UF");
		$xls->MontaConteudoString(0, 1, "Município");
		$xls->MontaConteudoString(0, 2, "Muncod");
		$xls->MontaConteudoString(0, 3, "Campus/Uned");
		$xls->MontaConteudoString(0, 4, "Vínculo");
		$xls->MontaConteudoString(0, 5, "Código Existência do campus/uned");
		$xls->MontaConteudoString(0, 6, "Existência do campus/uned");
		$xls->MontaConteudoString(0, 7, "Código Situação do campus/uned");
		$xls->MontaConteudoString(0, 8, "Situação do campus/uned");
		$xls->MontaConteudoString(0, 9, "Código Instalações");
		$xls->MontaConteudoString(0, 10, "Instalações");
		$xls->MontaConteudoString(0, 11, "Código Situação sobre obras no campus/uned");
		$xls->MontaConteudoString(0, 12, "Situação sobre obras no campus/uned");
		$xls->MontaConteudoString(0, 13, "Matrículas ofertadas - Ano Atual");
		$xls->MontaConteudoString(0, 14, "Matrículas ofertadas - Ano 2010");
		$xls->MontaConteudoString(0, 15, "Matrículas ofertadas - Previsão final");
		$xls->MontaConteudoString(0, 16, "Data de inauguração");
		$xls->MontaConteudoString(0, 17, "Início de aulas");
		$xls->MontaConteudoString(0, 18, "Investimento atual");
		$xls->MontaConteudoString(0, 19, "Investimento previsto");
		/* Gerar cabeçalho CSV */
		$csv .= "UF;Município;Muncod;Campus/Uned;Vínculo;Existência do campus/uned;Situação do campus/uned;Instalações;Situação sobre obras no campus/uned;Matrículas ofertadas - Ano Atual;Matrículas ofertadas - Ano 2010;Matrículas ofertadas - Previsão final;Início de aulas;Implantação;Investimento atual;Investimento previsto\n";
		
		/* Gerar cabeçalho HTML */
		$html = "<script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'>
					<tr bgcolor=\"#C0C0C0\">
						<td colspan='16' align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>
					<tr>
						<td rowspan='2' class='SubTituloCentro'>UF</td>
						<td rowspan='2' class='SubTituloCentro'>Município</td>
						<td rowspan='2' class='SubTituloCentro'>Muncod</td>
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Vínculo</td>
						<td rowspan='2' class='SubTituloCentro'>Existência</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<td rowspan='2' class='SubTituloCentro'>Data de inauguração</td>
						<td rowspan='2' class='SubTituloCentro'>Início de aulas</td>
						<td rowspan='2' class='SubTituloCentro'>Investimento atual</td>
						<td rowspan='2' class='SubTituloCentro'>Investimento previsto</td>
					</tr>
					<tr>
						<td class='SubTituloCentro'>Atual</td>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>Final</td>
					</tr>";
		$i=1;
		foreach($dadoscsv as $registro) {
			/* Gerar linha XLS */
			$xls->MontaConteudoString($i, 0, $registro['estuf']);
			$xls->MontaConteudoString($i, 1, $registro['mundescricao']);
			$xls->MontaConteudoString($i, 2, $registro['muncod']);
			$xls->MontaConteudoString($i, 3, $registro['entnome']);
			$xls->MontaConteudoString($i, 4, $registro['entsig']);
			$xls->MontaConteudoString($i, 5, $registro['cmpexistencia']);
			$xls->MontaConteudoString($i, 6, $_cmpexistencia[$registro['cmpexistencia']]);
			$xls->MontaConteudoString($i, 7, $registro['cmpsituacao']);
			$xls->MontaConteudoString($i, 8, $_cmpsituacao[$registro['cmpsituacao']]);
			$xls->MontaConteudoString($i, 9, $registro['cmpinstalacao']);
			$xls->MontaConteudoString($i, 10, $_cmpinstalacao[$registro['cmpinstalacao']]);
			$xls->MontaConteudoString($i, 11, $registro['cmpsituacaoobra']);
			$xls->MontaConteudoString($i, 12, $_cmpsituacaoobra[$registro['cmpsituacaoobra']]);
			$xls->MontaConteudoString($i, 13, $registro['matatual']);
			$xls->MontaConteudoString($i, 14, $registro['ano2010']);
			$xls->MontaConteudoString($i, 15, $registro['ano2012']);
			$xls->MontaConteudoString($i, 16, $registro['cmpdatainauguracao']);
			$xls->MontaConteudoString($i, 17, ((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):""));
			$xls->MontaConteudoString($i, 18, number_format($registro['insrealizado'], 2, ',', '.'));
			$xls->MontaConteudoString($i, 19, number_format($registro['insprevisto'], 2, ',', '.'));
			$i++;
			
			/* Gerar linha CSV */
			$csv .= $registro['estuf'].";".
					$registro['mundescricao'].";".
					$registro['muncod'].";".
					$registro['entnome'].";".
					$registro['entsig'].";".
					$_cmpexistencia[$registro['cmpexistencia']].";".
					$_cmpsituacao[$registro['cmpsituacao']].";".
					$_cmpinstalacao[$registro['cmpinstalacao']].";".
					$_cmpsituacaoobra[$registro['cmpsituacaoobra']].";".
					$registro['matatual'].";".
					$registro['ano2010'].";".
					$registro['ano2012'].";".
					$registro['cmpdatainauguracao'].";".
					((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"").";".
					number_format($registro['insrealizado'], 2, ',', '.').";".
					number_format($registro['insprevisto'], 2, ',', '.')."\n";
					
			/* Gerar linha HTML */
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$html .= "<tr bgcolor='".$marcado."'>
						<td>".$registro['estuf']."</td>
					 	<td>".$registro['mundescricao']."</td>
					 	<td>".$registro['muncod']."</td>
					 	<td>".$registro['entnome']."</td>
					 	<td>".$registro['entsig']."</td>
					 	<td>".$_cmpexistencia[$registro['cmpexistencia']]."</td>
					 	<td>".$_cmpsituacao[$registro['cmpsituacao']]."</td>
					 	<td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td>
					 	<td>".$_cmpsituacaoobra[$registro['cmpsituacaoobra']]."</td>
					 	<td>".$registro['matatual']."</td>
					 	<td>".$registro['ano2010']."</td>
					 	<td>".$registro['ano2012']."</td>
					 	<td>".$registro['cmpdatainauguracao']."</td>
					 	<td>".((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"")."</td>
					 	<td align='right'>".number_format($registro['insrealizado'], 2, ',', '.')."</td>
					 	<td align='right'>".number_format($registro['insprevisto'], 2, ',', '.')."</td>
					 </tr>";
		}
		$html .= "</table>";
	}
	
	switch(substr($_POST['exibir'],10,3)) {
		case 'csv':
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"my-data.csv\"");
			echo $csv;
			break;
		case 'xls':
			$xls->GeraArquivo();
			break;
		case 'htm':
			echo $html;
			break;
			
	}
	exit;
}
function mostrar_superior($filtroSQL = false) {
	global $db;
	$sql =  "SELECT edc.estuf, 
					mun.mundescricao, 
					mun.muncod, 
					ent.entnome, 
					uor.entsig, 
				 	cam.cmpexistencia, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '".date("Y")."') AS ano".date("Y").",
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2010') AS ano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2012') AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 cam.cmpdataimplantacao,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_SUP."') AS insvtot
		  FROM academico.campus cam 
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 
		  LEFT JOIN entidade.endereco edc ON ent.entid = edc.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY edc.estuf, uor.entsig, ent.entnome";
	$dadoscsv = $db->carregar($sql);
	if($dadoscsv[0]) {
		$_cmpexistencia = array("N"=>"Novo","P"=>"Pré-Existente");
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
		
		$xls = new GeraExcel();
		$xls->MontaConteudoString(0, 0, "UF");
		$xls->MontaConteudoString(0, 1, "Município");
		$xls->MontaConteudoString(0, 2, "Muncod");
		$xls->MontaConteudoString(0, 3, "Campus/Uned");
		$xls->MontaConteudoString(0, 4, "Vínculo");
		$xls->MontaConteudoString(0, 5, "Existência do campus/uned");
		$xls->MontaConteudoString(0, 6, "Situação do campus/uned");
		$xls->MontaConteudoString(0, 7, "Instalações");
		$xls->MontaConteudoString(0, 8, "Situação sobre obras no campus/uned");
		$xls->MontaConteudoString(0, 9, "Vagas ofertadas - Ano Atual");
		$xls->MontaConteudoString(0, 10, "Vagas ofertadas - Ano 2010");
		$xls->MontaConteudoString(0, 11, "Vagas ofertadas - Previsão final");
		$xls->MontaConteudoString(0, 12, "Data de inauguração");
		$xls->MontaConteudoString(0, 13, "Início de aulas");
		$xls->MontaConteudoString(0, 14, "Investimento total");
		/* Gerar cabeçalho CSV */
		$csv .= "UF;Município;Muncod;Campus/Uned;Vínculo;Existência do campus/uned;Situação do campus/uned;Instalações;Situação sobre obras no campus/uned;Vagas ofertadas - Ano Atual;Vagas ofertadas - Ano 2010;Vagas ofertadas - Previsão final;Início de aulas;Implantação;Investimento total\n";
		/* Gerar cabeçalho HTML */
		$html = "<script language=\"JavaScript\" src=\"../../includes/funcoes.js\"></script>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				 <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/listagem.css\"/>
				 <table class='tabela'>
					<tr bgcolor=\"#C0C0C0\">
						<td colspan='16' align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Superior (SESU)<br>
						<br>
						Expansão da Educação Superior
						</td>
					</tr>
					<tr>
						<td rowspan='2' class='SubTituloCentro'>UF</td>
						<td rowspan='2' class='SubTituloCentro'>Município</td>
						<td rowspan='2' class='SubTituloCentro'>Muncod</td>
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Vínculo</td>
						<td rowspan='2' class='SubTituloCentro'>Existência</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<td rowspan='2' class='SubTituloCentro'>Início de aulas</td>
						<td rowspan='2' class='SubTituloCentro'>Implantação</td>
						<td rowspan='2' class='SubTituloCentro'>Investimento total</td>
					</tr>
					<tr>
						<td class='SubTituloCentro'>Atual</td>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>Final</td>
					</tr>";
		
		$i=1;
		foreach($dadoscsv as $registro) {
			/* Gerar linha XLS */
			$xls->MontaConteudoString($i, 0, $registro['estuf']);
			$xls->MontaConteudoString($i, 1, $registro['mundescricao']);
			$xls->MontaConteudoString($i, 2, $registro['muncod']);
			$xls->MontaConteudoString($i, 3, $registro['entnome']);
			$xls->MontaConteudoString($i, 4, $registro['entsig']);
			$xls->MontaConteudoString($i, 5, $_cmpexistencia[$registro['cmpexistencia']]);
			$xls->MontaConteudoString($i, 6, $_cmpsituacao[$registro['cmpsituacao']]);
			$xls->MontaConteudoString($i, 7, $_cmpinstalacao[$registro['cmpinstalacao']]);
			$xls->MontaConteudoString($i, 8, $_cmpsituacaoobra[$registro['cmpsituacaoobra']]);
			$xls->MontaConteudoString($i, 9, $registro['ano'.date("Y")]);
			$xls->MontaConteudoString($i, 10, $registro['ano2010']);
			$xls->MontaConteudoString($i, 11, $registro['ano2012']);
			$xls->MontaConteudoString($i, 12, $registro['cmpdatainauguracao']);
			$xls->MontaConteudoString($i, 13, ((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):""));
			$xls->MontaConteudoString($i, 14, number_format($registro['insvtot'], 2, ',', '.'));
			$i++;
			
			/* Gerar linha CSV */
			$csv .= $registro['estuf'].";".
					$registro['mundescricao'].";".
					$registro['muncod'].";".
					$registro['entnome'].";".
					$registro['entsig'].";".
					$_cmpexistencia[$registro['cmpexistencia']].";".
					$_cmpsituacao[$registro['cmpsituacao']].";".
					$_cmpinstalacao[$registro['cmpinstalacao']].";".
					$_cmpsituacaoobra[$registro['cmpsituacaoobra']].";".
					$registro['ano'.date("Y")].";".
					$registro['ano2010'].";".
					$registro['ano2012'].";".
					$registro['cmpdatainauguracao'].";".
					((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"").";".
					number_format($registro['insvtot'], 2, ',', '.')."\n";
					
					
			/* Gerar linha HTML */
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$html .= "<tr bgcolor='".$marcado."'>
						<td>".$registro['estuf']."</td>
					 	<td>".$registro['mundescricao']."</td>
					 	<td>".$registro['muncod']."</td>
					 	<td>".$registro['entnome']."</td>
					 	<td>".$registro['entsig']."</td>
					 	<td>".$_cmpexistencia[$registro['cmpexistencia']]."</td>
					 	<td>".$_cmpsituacao[$registro['cmpsituacao']]."</td>
					 	<td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td>
					 	<td>".$_cmpsituacaoobra[$registro['cmpsituacaoobra']]."</td>
					 	<td>".$registro['ano'.date("Y")]."</td>
					 	<td>".$registro['ano2010']."</td>
					 	<td>".$registro['ano2012']."</td>
					 	<td>".$registro['cmpdatainauguracao']."</td>
					 	<td>".((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"")."</td>
					 	<td align='right'>".number_format($registro['insvtot'], 2, ',', '.')."</td>
					 </tr>";
					
		}
		$html .= "</table>";
	}
	switch(substr($_POST['exibir'],10,3)) {
		case 'csv':
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"my-data.csv\"");
			echo $csv;
			break;
		case 'xls':
			$xls->GeraArquivo();
			break;
		case 'htm':
			echo $html;
			break;
			
	}
	exit;
}


if(substr($_POST['exibir'],0,9) == "relatorio") {
	$filtroSQL[] = "tuo.orgid IN('".$_POST['orgid']."')";
	$filtroSQL[] = "fen.funid IN (" . ($_POST['orgid'] == 2 ? 17 : 18) . ")"; 
	
	unset($vals);
	if($_POST['estuf'][0]) {
		foreach($_POST['estuf'] as $uf) {
			$vals[] = $uf;
		}
		$filtroSQL[] = "edc.estuf IN('".implode("','",$vals)."')";
	}
	unset($vals);
	if($_POST['unidades'][0]) {
		foreach($_POST['unidades'] as $uni) {
			$vals[] = $uni;
		}
		$filtroSQL[] = "uor.entid IN('".implode("','",$vals)."')";
	}
	if($_POST['cmpexistencia']) {
		$filtroSQL[] = "cam.cmpexistencia = '".$_POST['cmpexistencia']."'";
	}
	if($_POST['cmpsituacao']) {
		$filtroSQL[] = "cam.cmpsituacao = '".$_POST['cmpsituacao']."'";
	}
	if($_POST['cmpinstalacao']) {
		$filtroSQL[] = "cam.cmpinstalacao = '".$_POST['cmpinstalacao']."'";
	}
	if($_POST['cmpsituacaoobra']) {
		$filtroSQL[] = "cam.cmpsituacaoobra = '".$_POST['cmpsituacaoobra']."'";
	}
	/* Gerar cabeçalho XLS */
	$nomeDoArquivoXls = "my-data";
	switch($_REQUEST['orgid']) {
		case TPENSSUP:
			mostrar_superior($filtroSQL);
			break;
		case TPENSPROF:
			mostrar_profissional($filtroSQL);
			break;
	}

}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";


echo "<br/>";
$titulo_modulo = "Sistema de Informações Gerenciais";
monta_titulo( $titulo_modulo,'Relatório Nota Técnica');
?>

<script>
function abrirPopup(){
	window.open('', 'abrirPopup', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=800,height=600');	
}

function abrepopupUnidade(){
	for(i=0;i<document.formulario.orgid.length;i++) {
		if(document.formulario.orgid[i].checked) {
			var itemselecionado = document.formulario.orgid[i].value;
		}
	}
	janela = window.open('combo_unidades.php?tipoensino='+itemselecionado,'Unidades','width=400,height=400,toolbar=yes,status=yes,scrollbars=1,top=200,left=480');
	janela.focus();
}
function exibirRelatorio() {
	var formulario = document.formulario;
	//Filtros
	var radioselecionado = false;
	for(i=0;i<document.formulario.orgid.length;i++) {
		if(document.formulario.orgid[i].checked) {
			radioselecionado = true;
		}
	}
	if(!radioselecionado) {
		alert('"Tipo de ensino" é obrigatório.');
		return false;
	}
	selectAllOptions(document.formulario.estuf);
	selectAllOptions(document.formulario.unidades);
	
	return true;	
}

</script>

<form method="post" name="formulario" target="abrirPopup">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
	<tr><td colspan="2">Filtros</td></tr>
	<tr><td class="SubTituloDireita" valign="top">Tipo de ensino</td>
		<td>
		<?
		$sqlTipoEnsino = "SELECT * FROM academico.orgao ORDER BY orgdesc";
		$tipoen = $db->carregar($sqlTipoEnsino);
		if($tipoen[0]) {
			$prim = true;
			foreach($tipoen as $tpe) {
				echo "<input type='radio' name='orgid' value='".$tpe['orgid']."' ".(($prim)?"checked":"")."> ".$tpe['orgdesc']."&nbsp;&nbsp;";
				$prim = false;
			}
		}
		?>
		</td>
	</tr>

	<tr><td class="SubTituloDireita" valign="top">Unidades da Federação</td>
		<td>
		<?
		$sqlComboEstado = "select estuf as codigo, estdescricao as descricao
						   from territorios.estado
						   order by	estdescricao";
		$estuf = array(0 => array("codigo" => "", "descricao" => "Todas"));
		
		combo_popup("estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400);
		?>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Unidades</td>
        <td>
        <select multiple="multiple" size="5" name="unidades[]" id="unidades" ondblclick="abrepopupUnidade();" class="CampoEstilo" style="width:400px;">
        <option value="">Todas</option>
        </select>
        </td>
   </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Existência</td>
        <td><input type="radio" name="cmpexistencia" value="" checked> Todos <input type="radio" name="cmpexistencia" value="N"> Novo <input type="radio" name="cmpexistencia" value="P"> Pré-existente</td>
   </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Situação</td>
        <td><input type="radio" name="cmpsituacao" value="" checked> Todos <input type="radio" name="cmpsituacao" value="F"> Funcionando <input type="radio" name="cmpexistencia" value="N"> Não Funcionando</td>
   </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Instalações</td>
        <td><input type="radio" name="cmpinstalacao" value="" checked> Todos <input type="radio" name="cmpinstalacao" value="P"> Provisória <input type="radio" name="cmpinstalacao" value="D"> Definitiva</td>
   </tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Situação de Obras no Campus</td>
        <td><input type="radio" name="cmpsituacaoobra" value="" checked> Todos <input type="radio" name="cmpsituacaoobra" value="L"> Licitação da Obra <input type="radio" name="cmpsituacaoobra" value="A"> Obras em Andamento <input type="radio" name="cmpsituacaoobra" value="C">  Obras Concluídas</td>
   </tr>
   <tr bgcolor="#C0C0C0">
   		<td>&nbsp;</td>
		<td><input type="submit" value="Gerar CSV" onclick="document.getElementById('exibir').value='relatorio_csv';exibirRelatorio();"> <input type="submit" value="Gerar XLS" onclick="document.getElementById('exibir').value='relatorio_xls';exibirRelatorio();"> <input type="submit" value="Gerar HTML" onclick="document.getElementById('exibir').value='relatorio_html';exibirRelatorio();"><input type="hidden" id="exibir" name="exibir" value="relatorio"></td>
   </tr>
</table>
</form>