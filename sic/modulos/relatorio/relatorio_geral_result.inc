<?php 

ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
// ver($sql);
$dados = $db->carregar($sql);

// Gerar arquivo xls
if( $_POST['req'] == 'geraxls' ){
	
	$arCabecalho = array();
	$colXls = array();
	
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'secretaria'){
			array_push($arCabecalho, 'Secretaria');
			array_push($colXls, 'secretaria');
		}
		if($agrup == 'unidade'){
			array_push($arCabecalho, 'Unidade');
			array_push($colXls, 'unidade');
		}
		if($agrup == 'requerente'){
			array_push($arCabecalho, 'Requerente');
			array_push($colXls, 'requerente');
		}
		if($agrup == 'data'){
			array_push($arCabecalho, 'Data de inclusão');
			array_push($colXls, 'data');
		}
		if($agrup == 'situacao'){
			array_push($arCabecalho, 'Situação');
			array_push($colXls, 'situacao');
		}
		if($agrup == 'pergunta'){
			array_push($arCabecalho, 'Pergunta');
			array_push($colXls, 'pergunta');
		}
		if($agrup == 'prazo'){
			array_push($arCabecalho, 'Prazo');
			array_push($colXls, 'prazo');
		}					
	}
	
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $campo == 'slcnumsic' ? (string) '-'.trim($registro[$campo]).'-' : $registro[$campo];			
		}
	}
	
// 	$arquivo = 'SIMEC_SIC_RelatorioGeral_'.date("dmYHis").'.xls';
	
// 	header ("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
// 	header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
// 	header ("Cache-Control: no-cache, must-revalidate");
// 	header ("Pragma: no-cache");
// 	header ("Content-type: application/x-msexcel");
// 	header ("Content-Disposition: attachment; filename=\"{$arquivo}\"" );
// 	header ("Content-Description: PHP Generated Data" );
	
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%','');
	die;
}

// Monta agrupadores
function monta_agp()
{
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(												 
											"unidade",
											"secretaria",
											"requerente",
											"tipopessoa",
											"escolaridade",
											"profissao",
											"situacao",
											"slcnumsic",
											"data",											
											"prazo",
											"municipio",
											"estado",
											"pergunta",
											"comentario",
											"slcresposta",
											"prazo_final",
											"slcpergunta1instancia",
											"slcresposta1instancia",
											"slcpergunta2instancia",
											"slcresposta2instancia",
											"resposta1",
											"resposta2",
											"prodescricao",
											"sigilo",
											"slcpalavraschave",
											"slcclassificacao"										  )	  
					);
					
	foreach ($agrupador as $val){ 
		
		switch ($val) {
			
		    case 'unidade':
				array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade")										
									   				);				
		    	continue;
		        break;		    	
		    case 'secretaria':
				array_push($agp['agrupador'], array(
													"campo" => "secretaria",
											  		"label" => "Secretaria")										
									   				);				
		    	continue;
		        break;
		    case 'requerente':
				array_push($agp['agrupador'], array(
													"campo" => "requerente",
											 		"label" => "Requerente")										
									   				);					
		    	continue;			
		        break;
		    case 'situacao':
				array_push($agp['agrupador'], array(
												"campo" => "situacao",
												"label" => "Situação")										
										   		);	
				continue;
				break;
			case 'prazo':
				array_push($agp['agrupador'], array(
												"campo" => "prazo",
												"label" => "Prazo")										
										   		);	
				continue;
				break;	 
			case 'prazo_final':
				array_push($agp['agrupador'], array(
												"campo" => "prazo_final",
												"label" => "Prazo final")										
										   		);	
				continue;
				break;	 
			case 'data':
				array_push($agp['agrupador'], array(
												"campo" => "data",
												"label" => "Data")										
										   		);	
				continue;
				break;	 	 
		}
	}
	
	// Adiciona ultimo nivel, dados da solicitacao
	array_push($agp['agrupador'], array(
										"campo" => "solicitacao",
								  		"label" => "Solicitação")
						   				);	
	
	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	$agrupador = $_REQUEST['agrupador'];
	
	$coluna = array();
	if(!in_array('unidade', $agrupador)){
		$coluna[] = array(
						  "campo" => "unidade",
				   		  "label" => "Unidade"
					);
	}
	if(!in_array('secretaria', $agrupador)){
		$coluna[] = array(
						  "campo" => "secretaria",
				   		  "label" => "Secretaria"
					);
	}
	if(!in_array('municipio', $agrupador)){
		$coluna[] = array(
						  "campo" => "municipio",
				   		  "label" => "Município"
					);
	}
	if(!in_array('estado', $agrupador)){
		$coluna[] = array(
						  "campo" => "estado",
				   		  "label" => "UF"
					);
	}
	if(!in_array('requerente', $agrupador)){
		$coluna[] = array(
						  "campo" => "requerente",
				   		  "label" => "Requerente"
					);
	}
	if(!in_array('tipopessoa', $agrupador)){
		$coluna[] = array(
			"campo" => "tipopessoa",
			"label" => "Tipo Pessoa"
		);
	}
	if(!in_array('escolaridade', $agrupador)){
		$coluna[] = array(
			"campo" => "escolaridade",
			"label" => "Escolaridade"
		);
	}
	if(!in_array('profissao', $agrupador)){
		$coluna[] = array(
			"campo" => "profissao",
			"label" => "Profissão"
		);
	}
	if(!in_array('data', $agrupador)){
		$coluna[] = array(
						  "campo" => "data",
				   		  "label" => "Data de inclusão"	
					);
	}
	if(!in_array('prazo', $agrupador)){
		$coluna[] = array(
						  "campo" => "prazo",
				   		  "label" => "Prazo"
					);
	}
	if(!in_array('pergunta', $agrupador)){
		$coluna[] = array(
						  "campo" => "pergunta",
				   		  "label" => "Pergunta"
					);
	}
	if(!in_array('situacao', $agrupador)){
		$coluna[] = array(
						  "campo" => "situacao",
				   		  "label" => "Situação"
					);
	}
	if(!in_array('slcnumsic', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcnumsic",
				   		  "label" => "Nº do SIC",
						  "type"  => "string"
					);
	}
	if(!in_array('comentario', $agrupador)){
		$coluna[] = array(
						  "campo" => "comentario",
				   		  "label" => "Justificativa"
					);
	}
	if(!in_array('slcresposta', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcresposta",
				   		  "label" => "Resposta"
					);
	}
	if(!in_array('prazo_final', $agrupador)){
		$coluna[] = array(
						  "campo" => "prazo_final",
				   		  "label" => "Prazo final"
					);
	}
	if(!in_array('slcpergunta1instancia', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcpergunta1instancia",
				   		  "label" => "Pergunta 1º Instância"
					);
	}
	if(!in_array('slcresposta1instancia', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcresposta1instancia",
				   		  "label" => "Resposta 1º Instância"
					);
	}
	if(!in_array('slcpergunta2instancia', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcpergunta2instancia",
				   		  "label" => "Pergunta 2º Instância"
					);
	}
	if(!in_array('slcresposta2instancia', $agrupador)){
		$coluna[] = array(
						  "campo" => "slcresposta2instancia",
				   		  "label" => "Resposta 2º Instância"
					);
	}
	if(!in_array('resposta1', $agrupador)){
		$coluna[] = array(
			"campo" => "resposta1",
			"label" => "A informação já existe no portal do MEC?"
		);
	}
	if(!in_array('resposta2', $agrupador)){
		$coluna[] = array(
			"campo" => "resposta2",
			"label" => "Resposta Publicada?"
		);
	}
	if(!in_array('prodescricao', $agrupador)){
		$coluna[] = array(
			"campo" => "prodescricao",
			"label" => "Profissão?"
		);
	}
	if(!in_array('sigilo', $agrupador)){
		$coluna[] = array(
			"campo" => "sigilo",
			"label" => "Resposta Sigilosa?"
		);
	}
	if(!in_array('slcpalavraschave', $agrupador)){
		$coluna[] = array(
			"campo" => "slcpalavraschave",
			"label" => "Palavra Chave?"
		);
	}
	if(!in_array('slcclassificacao', $agrupador)){
		$coluna[] = array(
			"campo" => "slcclassificacao",
			"label" => "Classificação"
		);
	}
				
	return $coluna;	
}

// Monta sql do relatorio
function monta_sql()
{
	
	extract($_REQUEST);
	
	$stDataResposta = "case when slcdtinclusao is not null and slcprorrogado = 't' 
						then						
						to_char((
							 	case when extract('dow' from slcdtinclusao+30) BETWEEN 1 AND 5 AND slcdtinclusao+30 not in (select feddata from public.feriados)
									then slcdtinclusao+30
								else
									case when extract('dow' from slcdtinclusao+31) BETWEEN 1 AND 5 AND slcdtinclusao+31 not in (select feddata from public.feriados)
										then slcdtinclusao+31
									else
										case when extract('dow' from slcdtinclusao+32) BETWEEN 1 AND 5 AND slcdtinclusao+32 not in (select feddata from public.feriados)
											then slcdtinclusao+32
										else
											case when extract('dow' from slcdtinclusao+33) BETWEEN 1 AND 5 AND slcdtinclusao+33 not in (select feddata from public.feriados)
												then slcdtinclusao+33
											else
												case when extract('dow' from slcdtinclusao+34) BETWEEN 1 AND 5 AND slcdtinclusao+34 not in (select feddata from public.feriados)
													then slcdtinclusao+34
												else
													case when extract('dow' from slcdtinclusao+35) BETWEEN 1 AND 5 AND slcdtinclusao+35 not in (select feddata from public.feriados)
														then slcdtinclusao+35
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')							
					 when slcdtinclusao is not null and (slcprorrogado = 'f' or slcprorrogado is null) 
					 	then 
					 		to_char((
							 	case when extract('dow' from slcdtinclusao+20) BETWEEN 1 AND 5 AND slcdtinclusao+20 not in (select feddata from public.feriados)
									then slcdtinclusao+20
								else
									case when extract('dow' from slcdtinclusao+21) BETWEEN 1 AND 5 AND slcdtinclusao+21 not in (select feddata from public.feriados)
										then slcdtinclusao+21
									else
										case when extract('dow' from slcdtinclusao+22) BETWEEN 1 AND 5 AND slcdtinclusao+22 not in (select feddata from public.feriados)
											then slcdtinclusao+22
										else
											case when extract('dow' from slcdtinclusao+23) BETWEEN 1 AND 5 AND slcdtinclusao+23 not in (select feddata from public.feriados)
												then slcdtinclusao+23
											else
												case when extract('dow' from slcdtinclusao+24) BETWEEN 1 AND 5 AND slcdtinclusao+24 not in (select feddata from public.feriados)
													then slcdtinclusao+24
												else
													case when extract('dow' from slcdtinclusao+25) BETWEEN 1 AND 5 AND slcdtinclusao+25 not in (select feddata from public.feriados)
														then slcdtinclusao+25
													end
												end
											end
										end
									end				
								end
							)::date, 'dd/MM/yyyy')
				else to_char(slcdtinclusao, 'dd/MM/yyyy') end";

	$arWhere = array();
	
	// secretaria
	if( $secretaria[0] && $secretaria_campo_flag ){
		$arWhere[] = " sec.secid ". (!$secretaria_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $secretaria )." ) ";
	}
	
	// requerente
	if( $requerente[0] && $requerente_campo_flag ){
		$arWhere[] = " sol.entid ". (!$requerente_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $requerente )." ) ";
	}
	
	// situacao
	if( $situacao[0] && $situacao_campo_flag ){
		$arWhere[] = " doc.esdid ". (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $situacao )." ) ";
	}
	
	// periodo
	if($dtinicio || $dtfim){		
		if($dtinicio && $dtfim){
			$arWhere[] = " slcdtinclusao between '".formata_data_sql($dtinicio)."' and '".formata_data_sql($dtfim)."' ";
		}else if($dtinicio){
			$arWhere[] = " slcdtinclusao = '".formata_data_sql($dtinicio)."' ";
		}else if($dtfim){
			$arWhere[] = " slcdtinclusao = '".formata_data_sql($dtfim)."' ";
		}
	}
	
	if($dtrespotainicio || $dtrespostafim){		
		if($dtrespotainicio && $dtrespostafim){
			$arWhere[] = " (".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") between '".formata_data_sql($dtrespotainicio)."' and '".formata_data_sql($dtrespostafim)."' ";
		}else if($dtrespotainicio){
			$arWhere[] = " (".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") = '".formata_data_sql($dtrespotainicio)."' ";
		}else if($dtrespostafim){
			$arWhere[] = " (".str_replace(array("to_char(", ", 'dd/MM/yyyy')"), '', $stDataResposta).") = '".formata_data_sql($dtrespostafim)."' ";
		}
	}
	
	if($prazo){
		$arWhere[] = " ( case when slcdtinclusao is not null then
				case when slcprorrogado = 't' then			
					case when (20-coalesce(CURRENT_DATE-slcdtinclusao,0))+10 < 0 then
						0
					else
						(20-coalesce(CURRENT_DATE-slcdtinclusao,0))+10 end
				else
					case when 20-coalesce(CURRENT_DATE-slcdtinclusao,0) < 0 then
						0
					else
						 20-coalesce(CURRENT_DATE-slcdtinclusao,0) end
				end
			else
				0 end ) = ".$prazo;
	}
	
	if($unidade[0]){
		$arWhere[] = "(case when sol.unicod is null then sec.unicod else sol.unicod end) in ('".implode("','", $unidade)."')";
	}
	
	if($recurso){
		switch ($recurso){
			case 'S':
				$arWhere[] = "(slcpergunta1instancia is not null)";
				break;
			case 'N':
				$arWhere[] = "(slcpergunta1instancia is null)";
				break;			
		}
	}
	
//	$slcnumsic = "' ' || slcnumsic || ' ' as slcnumsic,";
//	if($_POST['req'] == 'geraxls'){
//		$slcnumsic = "'.' || trim(slcnumsic) || '.' as slcnumsic,";
//	}
	
	$sql = "select 
				'Solicitação Nº ' || sol.slcid as solicitacao,
				ent.entnome as requerente,
				case when slctipopessoa = 'F' then
					'Física'
				     when slctipopessoa = 'J' then
					'Jurídica'
				end as tipopessoa,
				ese.escdsc as escolaridade,
				pro.prodescricao as profissao,
				u.unidsc as unidade,
				sec.secdsc as secretaria,
				esd.esddsc as situacao,
				slcnumsic,				
				mun.mundescricao as municipio,
				mun.estuf as estado,
				slcpergunta as pergunta,
				to_char(slcdtinclusao ,'dd/MM/yyyy') as data,
				case when slcdtinclusao is not null then
					case when slcprorrogado = 't' then			
						case when (20-coalesce(CURRENT_DATE-slcdtinclusao,0))+10 < 0 then
							0
						else
							(20-coalesce(CURRENT_DATE-slcdtinclusao,0))+10 end
					else
						case when 20-coalesce(CURRENT_DATE-slcdtinclusao,0) < 0 then
							0
						else
							 20-coalesce(CURRENT_DATE-slcdtinclusao,0) end
					end
				else
					0 end || ' ' as prazo,
				case when doc.esdid = 475 then (select 
													cmddsc 
												from 
													workflow.comentariodocumento cmt 
												inner join
													workflow.historicodocumento hst on hst.hstid = cmt.hstid
												inner join 
													workflow.acaoestadodoc aed on aed.aedid = hst.aedid
												where 
													cmt.docid = doc.docid
												and 
													aed.esdiddestino = 475 limit 1)
				else '' end as comentario,
				slcresposta,
				({$stDataResposta}) as prazo_final,
				slcpergunta1instancia,
				slcresposta1instancia,
				slcpergunta2instancia,
				slcresposta2instancia,
				case when slcexistenoportaldomec = 't' then
						'SIM'
				     when slcexistenoportaldomec = 'f' then
						'NÃO'
				end as resposta1,
				case when slcpublicar = 't' then
						'SIM'
				     when slcpublicar = 'f' then
						'NÃO'
				end as resposta2,
				prodescricao,
				case when classificacaoconteudo = 't' then 
					'SIM'
					else 
					'NÃO'
					end as sigilo,
				slcpalavraschave,				
				CASE
					WHEN slcclassificacao = 'verde'
					THEN 'verde'
					ELSE 
						CASE
							WHEN slcclassificacao = 'amarelo'
							THEN 'amarelo'
							ELSE 'vermelho'
						END
				END as slcclassificacao
			from 
				sic.solicitacao sol
			inner join 
				sic.secretaria sec on sol.secid = sec.secid
			inner join 
				entidade.entidade ent on ent.entid = sol.entid
			left join 
				unidade u ON u.unicod = sec.unicod
			left join 
				entidade.endereco ede on ent.entid = ede.entid
			left join
				territorios.municipio mun on mun.muncod = ede.muncod
			left join
				workflow.documento doc on doc.docid = sol.docid
			left join 
				workflow.estadodocumento esd on esd.esdid = doc.esdid
			left join
				sic.escolaridade ese on ese.escid = sol.escid
			left join
				sic.profissao pro on pro.proid = sol.proid
			where
				sol.slcstatus = 'A'
			".( !empty($arWhere) ? ' AND' . implode(' AND ', $arWhere) : '' ).
			"order by  ".( !empty($agrupador) ? implode(',', $agrupador) : '' );
"limit 10";
	
	return $sql;
}

?>

<html>

	<head>
	
		<title>Relatório Geral - SIC</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="javascript" type="text/javascript" src="../../library/jquery/jquery-1.11.1.min.js"></script>
		
	</head>
	
	<script>
	jQuery.noConflict();

	jQuery(document).ready(function(){
		jQuery('td:contains(verde)').attr('style','background:#008000');
		jQuery('td:contains(amarelo)').attr('style','background:#FFFF00');
		jQuery('td:contains(vermelho)').attr('style','background:#FF0000');
	});
	</script>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<?php
		
			$r = new montaRelatorio();
			$r->setAgrupador($agrup, $dados); 
			$r->setColuna($col);
			$r->setTotNivel(true);
			$r->setMonstrarTolizadorNivel(true);
			$r->setBrasao(true);
			$r->setEspandir( $_REQUEST['expandir'] );
			$r->setTotalizador(false);
			echo $r->getRelatorio();
			
		?>
		
	</body>
	
</html>