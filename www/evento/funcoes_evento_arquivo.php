<?php
function carregarMenuEvento() {
	global $db;

	if($db->testa_superuser()) {
		//$menu[0] = array("id" => 0, "descricao" => "Compras", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=compras");
		$menu[0] = array("id" => 1, "descricao" => "Contratos", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=contratos");
		$menu[1] = array("id" => 2, "descricao" => "Eventos", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=eventos");
	}else{
		
		$arrTblCompras = tabelasSubmodulo("compras");
		if(is_array($arrTblCompras)){
			foreach($arrTblCompras as $tbl){
				if(verificaArquivoUsuarioTabela("evento",$tbl)){
					$menu[] = array("id" => 0, "descricao" => "Compras", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=compras");
					break;
				}
			}
		}
		$arrTblCcontratos = tabelasSubmodulo("contratos");
		if(is_array($arrTblCcontratos)){
			foreach($arrTblCcontratos as $tbl){
				if(verificaArquivoUsuarioTabela("evento",$tbl)){
					$menu[] = array("id" => 1, "descricao" => "Contratos", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=contratos");
					break;
				}
			}
		}
		$arrTblEventos = tabelasSubmodulo("eventos");
		if(is_array($arrTblEventos)){
			foreach($arrTblEventos as $tbl){
				if(verificaArquivoUsuarioTabela("evento",$tbl)){
					$menu[] = array("id" => 2, "descricao" => "Eventos", "link" => "/evento/evento.php?modulo=sistema/public_arquivo/evento_arquivo&acao=A&submodulo=eventos");
					break;
				}
			}
		}
		
	}
	
	if(!$menu){
		return array();
	}else{
		return $menu;	
	}
	
}

function tabelasSubmodulo($submodulo)
{
	switch($submodulo){
		case "compras":
			$arrTbl = false;
		break;
		case "contratos":
			$arrTbl = array("ctanexo");
		break;
		case "eventos":
			$arrTbl = array("anexoevento");
		break;
		default:
			$arrTbl = false;
		break;
	}
	return $arrTbl;
}

function dadosTabelasArquivosSubmodulo($submodulo) {
	
	global $db;
	
	$arrTabelas = tabelasSubmodulo($submodulo);
	
	if($db->testa_superuser()) {
		$cabecalho = array("CPF", "Nome");
		$arrCampos[] = "a.usucpf";
		$arrCampos[] = "u.usunome";
		$arrJoin[] = "inner join seguranca.usuario u ON u.usucpf = a.usucpf";
	}else{
		$arrWhere[] = "a.usucpf = '{$_SESSION['usucpf']}'";
		$cabecalho = array();
	}
	
	$arrWhere[] = "(a.arqid / 1000) between 647 and 725";
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";
	
	if(is_array($arrTabelas)){
		
		foreach($arrTabelas as $tbl){
			
			switch($tbl){
				
				case "ctanexo":
					$arrTbl["ctanexo"]['nome'] = "ctanexo";
					$arrTbl["ctanexo"]['descricao'] = "Anexo do Contrato";
					$arrTbl["ctanexo"]['cabecalho'] = $cabecalho;
					array_push($arrTbl["ctanexo"]['cabecalho'],"Tipo do Contrato","Número","Ano","Modalidade","Situação","Objeto","Contratada","Modalidade","Nome do Arquivo","Descrição","Tamanho (bytes)","Data","Upload");
					$arrTbl["ctanexo"]['sql'] = "	select distinct 
														".($arrCampos ? implode(",",$arrCampos)."," : "")."
														tpc.tpcdsc,
														ctr.ctrnum,
														ctr.ctrano as ano,
			   	   										ctr.ctrnummod as numeromodalidade,
			   	   										sit.sitdsc as situacao,
			   	   										ctr.ctrobj,
			   	   										ent.entnome as contratada, 
			       										mod.moddsc as modalidade,
			       										a.arqnome||'.'||a.arqextensao,
														a.arqdescricao,
														a.arqtamanho,
														to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
														'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(' || a.arqid || ')\" src=\"../imagens/excluir.gif\" /></span>' as upload
													from
														public.arquivo a
													inner join
														evento.ctanexo anx ON anx.arqid = a.arqid
													inner join
														evento.ctcontrato AS ctr ON ctr.ctrid = anx.ctrid
											       	LEFT JOIN
											       		entidade.entidade AS ent ON ent.entid = ctr.entidcontratada
											       	LEFT JOIN 
											       		evento.cttipocontrato AS tpc ON tpc.tpcid = ctr.tpcid
											       	LEFT JOIN 
											       		evento.ctanexo anc on anc.ctrid = ctr.ctrid
											       	LEFT JOIN 
											       		evento.ctmodalidadecontrato mod on ctr.modid = mod.modid
											       	LEFT JOIN 
											       		evento.ctsituacaocontrato sit on ctr.sitid = sit.sitid
											       	".($arrJoin ? implode(" ",$arrJoin) : "")."
											    	where 
											    		ctr.ctrstatus = 'A'
											    	".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
				break;
				
				case "anexoevento":
					$arrTbl["anexoevento"]['nome'] = "anexoevento";
					$arrTbl["anexoevento"]['descricao'] = "Anexo do Evento";
					$arrTbl["anexoevento"]['cabecalho'] = $cabecalho;
					array_push($arrTbl["anexoevento"]['cabecalho'],"Cód","Evento","Unidade Gestora","Início do Evento","Fim do Evento","Local","Custo","Situação","Nome do Arquivo","Descrição","Tamanho (bytes)","Data","Upload");
					$arrTbl["anexoevento"]['sql'] = "	select distinct
														".($arrCampos ? implode(",",$arrCampos)."," : "")."
														ev.eveid,
														ev.evetitulo,
			   	   										uni.ungdsc,
			   	   										to_char( ev.evedatainicio, 'dd/mm/YYYY' ) as datainicio,
														to_char( ev.evedatafim, 'dd/mm/YYYY' ) as datafim,
			   	   										(m.mundescricao||' - '||ev.estuf)as local,
			   	   										ev.evecustoprevisto,
														est.esddsc,
			       										a.arqnome||'.'||a.arqextensao,
														a.arqdescricao,
														a.arqtamanho,
														to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
														'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(' || a.arqid || ')\" src=\"../imagens/excluir.gif\" /></span>' as upload
													from
														public.arquivo a
													inner join
														evento.anexoevento anx ON anx.arqid = a.arqid 
													inner join
														evento.evento ev ON ev.eveid = anx.eveid
													LEFT JOIN 
														territorios.municipio m ON m.muncod = ev.muncod 
													LEFT JOIN 
														seguranca.usuario AS u ON u.usucpf = ev.usucpf 
												    LEFT JOIN 
												    	evento.anexoevento AS aev ON aev.eveid = ev.eveid
												    LEFT JOIN 
												    	workflow.documento d ON d.docid = ev.docid
												    LEFT JOIN 
												    	public.unidadegestora AS uni ON ev.ungcod = uni.ungcod
												    LEFT JOIN 
												    	workflow.estadodocumento est ON est.esdid = d.esdid 
												    LEFT JOIN 
												    	evento.avaliacaoevento aval ON aval.eveid = ev.eveid
												    LEFT JOIN 
												    	( select docid, max(htddata) as htddata from workflow.historicodocumento group by docid ) hd ON hd.docid = d.docid
											    	where 
											    		ev.evestatus = 'A'
											    	".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
				break;
				
			}
			
		}
		
	}
	
	return $arrTbl;

}
?>