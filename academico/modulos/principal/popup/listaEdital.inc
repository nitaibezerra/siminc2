<?php
if ( empty($_GET['entidcampus']) && empty($_GET['entidunidade']) ){
	die("<script type=\"text/javascript\">alert('Faltam parametros!'); window.close();</script>");	
}

include APPRAIZ. 'includes/classes/relatorio.class.inc';


// objeto da classe do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
?>

<html>
	<head>
		<META http-equiv="Pragma" content="no-cache">
		<title>Detalhamento de Editais</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>	
		<script type="text/javascript">
		function direcionaEdital(param){
			window.opener.location = param;
		}
		
		// Seta-se no atributo "notclose" o obj instanciado "this", a fim de que não se feche a popup
		this.opener.notclose = this;
		</script>
	</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

<?
// Monta cabeçalho
echo $autoriazacaoconcursos->cabecalho_entidade($_GET['entidcampus'] ? $_GET['entidcampus'] : $_GET['entidunidade']);

$sql   = monta_sql();
$dados = $db->carregar($sql);
$dados = tratar_dados($dados);
$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir(true);
//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
<?
function monta_sql(){
	
	$where = array();
	
	if ($_GET['entidunidade']){
		$where[] = "idunidade = " . $_GET['entidunidade'];
	}

	if ($_GET['entidcampus']){
		$where[] = "idcampus = " . $_GET['entidcampus'];
	}
	
	
	if ($_GET['idcampus']){
		$_GET['idcampus'] = !is_array($_GET['idcampus']) ? explode(',', $_GET['idcampus']) : $_GET['idcampus'];
		$where[] = "idcampus " . $_GET['campusexcludente'] . "(" . implode( ',', $_GET['idcampus'] ) . ") ";
	}
		
	if ($_GET['idclasse']){
		$where[] = "idclasse = " . $_GET['idclasse'];
	}

	if ($_SESSION['academico']['orgid']){
		$where[] = "orgid = " . $_SESSION['academico']['orgid'];
	}
	
	if ($_GET['ano']){
		$_GET['ano'] = !is_array($_GET['ano']) ? explode(',', $_GET['ano']) : $_GET['ano'];
		$where[] = "ano " . $_GET['anoexcludente'] ."(" . implode( ',', $_GET['ano'] ) . ")";
	}
	
	if ($_GET['idprograma']){
		$_GET['idprograma'] = !is_array($_GET['idprograma']) ? explode(',', $_GET['idprograma']) : $_GET['idprograma'];
		$where[] = "idprograma " . $_GET['programaexcludente'] . "(" . implode( ',', $_GET['idprograma'] ) . ")";
	}
	
			
	$sql = "SELECT
				COALESCE('<a href=\"#\" onclick=\"direcionaEdital(\'academico.php?modulo=principal/cadedital&acao=C&edpid=' || edpidpub || '&prtid=' || portaria || '&entidcampus=' || idcampus || '&prgid=' || idprograma || '&evento=A\'); \"><b>(EP)</b>&nbsp;' || edital_publicado || '</a>', 'Não cadastrado') AS edital_publicado,
				COALESCE('<a href=\"#\" onclick=\"direcionaEdital(\'academico.php?modulo=principal/cadedital&acao=H&edpid=' || edpidhom || '&prtid=' || portaria || '&entidcampus=' || idcampus || '&prgid=' || idprograma || '&evento=A\'); \"><b>(EH)</b>&nbsp;' || edital_homologado || '</a>', '<b>(EH)</b> Não cadastrado') AS edital_homologado,
				COALESCE('<a href=\"#\" onclick=\"direcionaEdital(\'academico.php?modulo=principal/cadedital&acao=N&edpid=' || edpidefe || '&prtid=' || portaria || '&entidcampus=' || idcampus || '&prgid=' || idprograma || '&evento=A\'); \"><b>(PE)</b>&nbsp;' || edital_efetivado || '</a>', '<b>(PE)</b> Não cadastrado') AS edital_efetivado,
			--	edpidpub,
			--	edpidhom,
			--	edpidefe,				
				COALESCE(publicado) as publicado, 
				COALESCE(SUM( homologado ),0) as homologado,
				COALESCE(SUM(lepvlrprovefetivados),0) as efetivado,
				COALESCE(unidade , 'Não informado') as unidade , 
				idunidade as idunidade , 
				COALESCE(campus, 'Não informado') as campus , 
				idcampus as idcampus , 
				COALESCE(classe, 'Não informado') as classe , 
				idclasse as idclasse , 
				tipoensino as tipoensino , 
				programa as programa , 
				idprograma as idprograma , 
				ano , 
				'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria
			FROM
			(
				
				(


					-- Lançamento Publicado

					SELECT
						CASE 
							WHEN TRIM(ep.edpnumero) != '' THEN  'Nº controle: ' || ep.edpid || ' / Nº edital: ' || ep.edpnumero
							ELSE COALESCE('Nº edital: ' || ep.edpid::varchar, 'Não cadastrado')
						END AS edital_publicado,
						CASE 
							WHEN TRIM(ep1.edpnumero) != '' THEN  'Nº controle: ' || ep1.edpid || ' / Nº edital: ' || ep1.edpnumero
							ELSE COALESCE('Nº edital: ' || ep1.edpid::varchar, 'Não cadastrado')
						END AS edital_homologado,
						CASE 
							WHEN TRIM(ep2.edpnumero) != '' THEN  'Nº controle: ' || ep2.edpid || ' / Nº edital: ' || ep2.edpnumero
							ELSE COALESCE('Nº edital: ' || ep2.edpid::varchar, 'Não cadastrado')
						END AS edital_efetivado,
						ep.edpid AS edpidpub,
						ep1.edpid AS edpidhom,
						ep2.edpid AS edpidefe,
						p.prtid as portaria,
						p.prtnumero as numeroportaria,
						p.prtano as ano, 
						SUM( lep.lepvlrpublicacao) as publicado,
						0 as homologado,
						0 as lepvlrprovefetivados,
						COALESCE(ent.entnome , 'Não informado') as unidade ,
						ent.entid as idunidade, 
						COALESCE(cp.entnome, 'Não informado') as campus ,
						cp.entid as idcampus, 
						COALESCE(cl.clsdsc, 'Não informado') as classe ,
						cl.clsid as idclasse, 
						ao.orgdesc as tipoensino , 
						ao.orgid,
						pp.prgdsc as programa,
						p.prgid as idprograma 
					FROM 
						academico.portarias p 
						
					-- publicado --
					INNER JOIN 
						academico.editalportaria ep ON ep.prtid = p.prtid 
									       AND ep.edpstatus = 'A'
									       AND ep.tpeid = 2
					INNER JOIN 
						academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
											  AND lep.lepstatus = 'A'		
						
					-- homologado --
					LEFT JOIN 
						academico.editalportaria ep1 ON ep1.edpidhomo = ep.edpid 
										AND ep1.edpstatus = 'A'	
										AND ep1.tpeid = 3
				
					-- efetivado --
					LEFT JOIN 
						academico.editalportaria ep2 ON ep2.edpideditalhomologacao = ep1.edpid 
										AND ep2.edpstatus = 'A'
										AND ep2.tpeid = 5
														
					INNER JOIN 
						entidade.entidade cp ON cp.entid = ep.entidcampus 
					INNER JOIN 
						entidade.entidade ent ON ent.entid = ep.entidentidade
					INNER JOIN 
						academico.cargos c ON c.crgid = lep.crgid
					INNER JOIN 
						academico.classes cl ON cl.clsid = c.clsid
					INNER JOIN 
						academico.orgao ao ON ao.orgid = p.orgid 
					INNER JOIN 
						academico.programa pp ON p.prgid = pp.prgid
					WHERE 
						p.tprid = 1 
						AND p.prtstatus = 'A'
					GROUP BY 
						edital_publicado,
						edital_homologado,
						edital_efetivado,
						ep.edpid,
						ep1.edpid,
						ep2.edpid,
						p.prtid ,
						p.prtnumero,
						p.prtano, 
						ent.entnome,
						ent.entid, 
						cp.entnome,
						cp.entid, 
						cl.clsdsc,
						cl.clsid, 
						ao.orgdesc, 
						ao.orgid,
						pp.prgdsc,
						p.prgid 
						
				) UNION ALL (

					-- Lançamento Homologado	

					SELECT
						CASE 
							WHEN TRIM(ep.edpnumero) != '' THEN  'Nº controle: ' || ep.edpid || ' / Nº edital: ' || ep.edpnumero
							ELSE COALESCE('Nº edital: ' || ep.edpid::varchar, 'Não cadastrado')
						END AS edital_publicado,
						CASE 
							WHEN TRIM(ep1.edpnumero) != '' THEN  'Nº controle: ' || ep1.edpid || ' / Nº edital: ' || ep1.edpnumero
							ELSE COALESCE('Nº edital: ' || ep1.edpid::varchar, 'Não cadastrado')
						END AS edital_homologado,
						CASE 
							WHEN TRIM(ep2.edpnumero) != '' THEN  'Nº controle: ' || ep2.edpid || ' / Nº edital: ' || ep2.edpnumero
							ELSE COALESCE('Nº edital: ' || ep2.edpid::varchar, 'Não cadastrado')
						END AS edital_efetivado,
						ep.edpid AS edpidpub,
						ep1.edpid AS edpidhom,
						ep2.edpid AS edpidefe,
						p.prtid as portaria,
						p.prtnumero as numeroportaria,
						p.prtano as ano, 
						0 as publicado,
						SUM( lep1.lepvlrhomologado ) as homologado,
						0 as lepvlrprovefetivados,
						COALESCE(ent.entnome , 'Não informado') as unidade ,
						ent.entid as idunidade, 
						COALESCE(cp.entnome, 'Não informado') as campus ,
						cp.entid as idcampus, 
						COALESCE(cl.clsdsc, 'Não informado') as classe ,
						cl.clsid as idclasse, 
						ao.orgdesc as tipoensino , 
						ao.orgid,
						pp.prgdsc as programa,
						p.prgid as idprograma 
					FROM 
						academico.portarias p 
						
					-- publicado --
					INNER JOIN 
						academico.editalportaria ep ON ep.prtid = p.prtid 
													   AND ep.edpstatus = 'A'
													   AND ep.tpeid = 2
				
					-- homologado --
					INNER JOIN 
						academico.editalportaria ep1 ON ep1.edpidhomo = ep.edpid 
														AND ep1.edpstatus = 'A'	
														AND ep1.tpeid = 3
					INNER JOIN 
						academico.lancamentoeditalportaria lep1 ON lep1.edpid = ep1.edpid 
															   AND lep1.lepstatus = 'A'		
				
					-- efetivado --
					LEFT JOIN 
						academico.editalportaria ep2 ON ep2.edpideditalhomologacao = ep1.edpid 
														AND ep2.edpstatus = 'A'
														AND ep2.tpeid = 5
				
					INNER JOIN 
						entidade.entidade cp ON cp.entid = ep.entidcampus 
					INNER JOIN 
						entidade.entidade ent ON ent.entid = ep.entidentidade
					INNER JOIN 
						academico.cargos c ON c.crgid = lep1.crgid
					INNER JOIN 
						academico.classes cl ON cl.clsid = c.clsid
					INNER JOIN 
						academico.orgao ao ON ao.orgid = p.orgid 
					INNER JOIN 
						academico.programa pp ON p.prgid = pp.prgid
					WHERE 
						p.tprid = 1 
						AND p.prtstatus = 'A' 
					GROUP BY 
						edital_publicado,
						edital_homologado,
						edital_efetivado,
						ep.edpid,
						ep1.edpid,
						ep2.edpid,
						p.prtid ,
						p.prtnumero,
						p.prtano, 
						ent.entnome,
						ent.entid, 
						cp.entnome,
						cp.entid, 
						cl.clsdsc,
						cl.clsid, 
						ao.orgdesc, 
						ao.orgid,
						pp.prgdsc,
						p.prgid 
				) UNION ALL (

					-- Lançamento Efetivado
					SELECT
						CASE 
							WHEN TRIM(ep.edpnumero) != '' THEN  'Nº controle: ' || ep.edpid || ' / Nº edital: ' || ep.edpnumero
							ELSE COALESCE('Nº edital: ' || ep.edpid::varchar, 'Não cadastrado')
						END AS edital_publicado,
						CASE 
							WHEN TRIM(ep1.edpnumero) != '' THEN  'Nº controle: ' || ep1.edpid || ' / Nº edital: ' || ep1.edpnumero
							ELSE COALESCE('Nº edital: ' || ep1.edpid::varchar, 'Não cadastrado')
						END AS edital_homologado,
						CASE 
							WHEN TRIM(ep2.edpnumero) != '' THEN  'Nº controle: ' || ep2.edpid || ' / Nº edital: ' || ep2.edpnumero
							ELSE COALESCE('Nº edital: ' || ep2.edpid::varchar, 'Não cadastrado')
						END AS edital_efetivado,
						ep.edpid AS edpidpub,
						ep1.edpid AS edpidhom,
						ep2.edpid AS edpidefe,
						p.prtid as portaria,
						p.prtnumero as numeroportaria,
						p.prtano as ano, 
						0 as publicado,
						0 as homologado,
						SUM( lep2.lepvlrprovefetivados) as lepvlrprovefetivados,
						COALESCE(ent.entnome , 'Não informado') as unidade ,
						ent.entid as idunidade, 
						COALESCE(cp.entnome, 'Não informado') as campus ,
						cp.entid as idcampus, 
						COALESCE(cl.clsdsc, 'Não informado') as classe ,
						cl.clsid as idclasse, 
						ao.orgdesc as tipoensino , 
						ao.orgid,
						pp.prgdsc as programa,
						p.prgid as idprograma 
					FROM 
						academico.portarias p 
						
					-- publicado --
					INNER JOIN 
						academico.editalportaria ep ON ep.prtid = p.prtid 
													   AND ep.edpstatus = 'A'
													   AND ep.tpeid = 2
				
					-- homologado --
					INNER JOIN 
						academico.editalportaria ep1 ON ep1.edpidhomo = ep.edpid 
														AND ep1.edpstatus = 'A'	
														AND ep1.tpeid = 3
				
					-- efetivado --
					INNER JOIN 
						academico.editalportaria ep2 ON ep2.edpideditalhomologacao = ep1.edpid 
														AND ep2.edpstatus = 'A'
														AND ep2.tpeid = 5
					INNER JOIN 
						academico.lancamentoeditalportaria lep2 ON lep2.edpid = ep2.edpid 
														       AND lep2.lepstatus = 'A'		
				
					INNER JOIN 
						entidade.entidade cp ON cp.entid = ep.entidcampus 
					INNER JOIN 
						entidade.entidade ent ON ent.entid = ep.entidentidade
					INNER JOIN 
						academico.cargos c ON c.crgid = lep2.crgid
					INNER JOIN 
						academico.classes cl ON cl.clsid = c.clsid
					INNER JOIN 
						academico.orgao ao ON ao.orgid = p.orgid 
					INNER JOIN 
						academico.programa pp ON p.prgid = pp.prgid
					WHERE 
						p.tprid = 1 
						AND p.prtstatus = 'A' 
					GROUP BY 
						edital_publicado,
						edital_homologado,
						edital_efetivado,
						ep.edpid,
						ep1.edpid,
						ep2.edpid,
						p.prtid ,
						p.prtnumero,
						p.prtano, 
						ent.entnome,
						ent.entid, 
						cp.entnome,
						cp.entid, 
						cl.clsdsc,
						cl.clsid, 
						ao.orgdesc, 
						ao.orgid,
						pp.prgdsc,
						p.prgid 
				)

			) AS foo 
			WHERE  
			" . (count($where) > 0 ? implode(' AND ', $where) : ' ' ) . "
			GROUP BY 
				edital_publicado,
				edital_homologado,
				edital_efetivado,
				edpidpub,
				edpidhom,
				edpidefe,
				portaria,
				numeroportaria,
				unidade,
				idunidade,
				campus,
				idcampus,
				classe,
				idclasse,
				tipoensino,
				orgid,
				programa,
				idprograma,
				ano, 
				publicado
			ORDER BY 
				ano, programa, classe";
//	dbg($sql, 1);
	
	return $sql;
}

function monta_coluna(){
	
//	$arrPerfil = array(
//						PERFIL_SUPERUSUARIO,
//						PERFIL_ADMINISTRADOR
//					  );
//	
//	$permissao = academico_possui_perfil($arrPerfil);
	
	$coluna    = array(
						array(
								  "campo" => "publicado",
						   		  "label" => "Concursos <br/>Publicados <br/>(C)",
								  "type"  => "numeric"
							  )
					);
					
	array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
							 )
				  );			
	
	array_push($coluna, array(
								  "campo" => "efetivado",
						   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
								  "type"  => "numeric"
							  )				  
			  );			
	

	return $coluna;			  	
}

function monta_agp(){
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(

 									   		"publicado",
 									   		"homologado",
											"efetivado"
										  )	  
				);
	
	array_push($agp['agrupador'], array(
									"campo" => "ano",
									"label" => "Ano")										
							   		);	
				
	array_push($agp['agrupador'], array(
									"campo" => "programa",
									"label" => "Programa")										
							   		);	

	array_push($agp['agrupador'], array(
										"campo" => "portaria",
								  		"label" => "Portaria")										
						   				);					
	if ( !empty($_GET['entidunidade']) ){
		array_push($agp['agrupador'], array(
											"campo" => "campus",
									 		"label" => "Campus")										
							   				);					
	}
	array_push($agp['agrupador'], array(
									"campo" => "classe",
									"label" => "Classe")										
							   		);	

	array_push($agp['agrupador'], array(
										"campo" => "edital_publicado",
								  		"label" => "Edital Publicado (EP)")										
						   				);

	array_push($agp['agrupador'], array(
										"campo" => "edital_homologado",
								  		"label" => "Edital Homologado (EH)")										
						   				);	

	array_push($agp['agrupador'], array(
										"campo" => "edital_efetivado",
								  		"label" => "Portaria de Efetivação (PE)")										
						   				);				
	
	return $agp;
}

function tratar_dados($dados){
	
	if (empty($dados)){
		return;
	}
	
	$arrPublicado  = array();
	$arrHomologado = array();
	$arrEfetivado  = array();

	$i = 0;
	foreach ($dados as $d){
		$arrPublicado[$d['edital_publicado']] = isset($arrPublicado[$d['edital_publicado']]) ? $arrPublicado[$d['edital_publicado']] : array();
		if ( !in_array($d['idclasse'], $arrPublicado[$d['edital_publicado']]) && $dados[$i]['publicado'] > 0 ){
			array_push($arrPublicado[$d['edital_publicado']], $d['idclasse']);			
		}else{
			$dados[$i]['publicado'] = 0;
		}	
		
		$arrHomologado[$d['edital_homologado']] = isset($arrHomologado[$d['edital_homologado']]) ? $arrHomologado[$d['edital_homologado']] : array();
		if ( !in_array($d['idclasse'], $arrHomologado[$d['edital_homologado']]) && $dados[$i]['homologado'] > 0){
			array_push($arrHomologado[$d['edital_homologado']], $d['idclasse']);			
		}else{
			$dados[$i]['homologado'] = 0;
		}
//		$dados[$i]['provimentosnaoefetivados'] = $dados[$i]['provimento'] - $dados[$i]['lepvlrprovefetivados']; 
//		$dados[$i]['provimentopendencia']	   = $dados[$i]['homologado'] - $dados[$i]['provimento'];	
		$i++;
	}
	
	return $dados;
}

function tratarClausula($dadoParam, $quebra, $separacao=null){
	$dado = explode($quebra, $dadoParam);
	if (is_array($dado)){
		$dado = $separacao . implode($separacao, $dado) . $separacao;
	}
	
	return $dado;
}
?>