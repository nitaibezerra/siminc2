<?php

function carregarMenuConjur() {

	$menu[] = array("id" => 0, "descricao" => "Anexos de Processos", "link" => "/conjur/conjur.php?modulo=sistema/public_arquivo/conjur_arquivo&acao=A"); 
	return $menu;
	
}

function montaListaArquivosConjur()
{
	global $db;
	
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";
	$arrWhere[] = "a.arqid/1000 between 647 and 725";
	
	$cabecalho = array();
	
	if($db->testa_superuser()) {
		$cabecalho = array("CPF", "Nome");
		$arrCampos[] = "a.usucpf";
		$arrCampos[] = "u.usunome";
	}else{
		$arrWhere[] = "a.usucpf = '{$_SESSION['usucpf']}'";
	}
			
	monta_titulo( "Recuperação dos arquivos anexos aos processos", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
	array_push($cabecalho, "Nº do Processo SIDOC", "Interessado","Prioridade","Coordenação", "Situação CONJUR","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
	$sql = "SELECT distinct
				".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
				prc.prcnumsidoc,
				prc.prcnomeinteressado,
				tpr.tipdsc as prioridade,
				coo.coodsc,
				esd.esddsc,
				a.arqid,
				a.arqnome||'.'||a.arqextensao,
				a.arqdescricao,
				a.arqtamanho,
				to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
				'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
			FROM 
				arquivo a
			INNER JOIN 
				conjur.anexos anx ON anx.arqid = a.arqid
			INNER JOIN
				conjur.processoconjur prc ON prc.prcid = anx.prcid
			LEFT JOIN 
				conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
	    	LEFT JOIN 
	    		conjur.coordenacao coo ON coo.coonid = prc.cooid
	    	LEFT JOIN 
	    		workflow.documento doc ON doc.docid = esp.docid 
	    	LEFT JOIN 
	    		(SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
	    	LEFT JOIN 
	    		workflow.estadodocumento esd ON esd.esdid = doc.esdid
	    	LEFT JOIN 
	    		conjur.expressaochave exp ON exp.prcid = prc.prcid  
	    	LEFT JOIN 
	    		conjur.procedencia pro ON pro.proid = prc.proid
	        LEFT JOIN 
	        	conjur.tipoprioridade tpr ON tpr.tipid = prc.tipid
			INNER JOIN
				seguranca.usuario u ON u.usucpf = a.usucpf 
			WHERE 
				anxstatus = 'A'::bpchar 
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" );

	return array("sql" => $sql, "cabecalho" => $cabecalho);
}