<?php

function carregarMenuBrasilPro() {
	
	global $db;
	
	if($db->testa_superuser()){
		$menu[] = array("id" => 0, "descricao" => "Anexos de Monitoramento", "link" => "/brasilpro/brasilpro.php?modulo=sistema/public_arquivo/brasilpro_arquivo&acao=A", "tabela" => "monitoramentoanexos"); 
		$menu[] = array("id" => 1, "descricao" => "Anexos de Atividades", "link" => "/brasilpro/brasilpro.php?modulo=sistema/public_arquivo/brasilpro_arquivo&acao=A&tbl=versaoanexomonitoramento", "tabela" => "versaoanexomonitoramento"); 
	}else{
		$flag_menu = false;
		if(verificaArquivoUsuarioTabela("cte","monitoramentoanexos")){
			$menu[] = array("id" => 0, "descricao" => "Anexos de Monitoramento", "link" => "/brasilpro/brasilpro.php?modulo=sistema/public_arquivo/brasilpro_arquivo&acao=A","tabela" => "monitoramentoanexos");
			$flag_menu = true;
		}
		if(verificaArquivoUsuarioTabela("cte","versaoanexomonitoramento")){
			$menu[] = array("id" => 1, "descricao" => "Anexos de Atividades", "link" => "/brasilpro/brasilpro.php?modulo=sistema/public_arquivo/brasilpro_arquivo&acao=A".($flag_menu == false ? "" : "&tbl=versaoanexomonitoramento"), "tabela" => "versaoanexomonitoramento");
			$flag_menu = true;
		}
		if($flag_menu == false){
			$menu = array();
		}
	}
	return $menu;
	
	
}

function montaListaArquivosBrasilPro($tabela = null)
{
	global $db;
	
	$tabela = $_REQUEST['tbl'] ? $_REQUEST['tbl'] : $tabela;
	
	$tabela = !$tabela ? "monitoramentoanexos" : $tabela;
	
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
			
	switch($tabela){
		
		case "monitoramentoanexos":
			
			monta_titulo( "Recuperação dos arquivos anexos ao monitoramento", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
			array_push($cabecalho, "Ação Monitorada","Incício","Fim","Ano","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
			$sql = "SELECT distinct
						".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
						mac.mosdsc,
						to_char(mosdatainicio,'dd/mm/YYYY') as mosdatainicio,
						to_char(mosdatafinal,'dd/mm/YYYY') as mosdatafinal,
					  	mosano,
						a.arqid,
						a.arqnome||'.'||a.arqextensao,
						a.arqdescricao,
						a.arqtamanho,
						to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
						'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
					FROM 
						arquivo a
					JOIN 
						cte.monitoramentoanexos anx ON a.arqid = anx.arqid
					JOIN 
						cte.monitoramentosubacoes mac ON mac.mosid = anx.mosid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = a.usucpf 
					WHERE 
						mac.mosstatus = 'A'::bpchar
					AND
						mnxstatus = 'A'::bpchar
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" );
		
		break;
		
		case "versaoanexomonitoramento":

			monta_titulo( "Recuperação dos arquivos anexados às Atividades das Subações", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
			array_push($cabecalho, "Anexo da Atividade", "Subação","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
			$sql = "SELECT distinct
						".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
						anx.anedsc,
						sba.sbadsc,
						a.arqid,
						a.arqnome||'.'||a.arqextensao,
						a.arqdescricao,
						a.arqtamanho,
						to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
						'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
					FROM 
						arquivo a
					JOIN 
						cte.versaoanexomonitoramento ver ON a.arqid = ver.arqid
					JOIN 
						cte.anexomonitoramento anx ON ver.aneid = anx.aneid
					JOIN
						cte.monitoramentosubacao mnt ON mnt.mntid = anx.mntid
					JOIN
						cte.subacaoindicador sba ON sba.sbaid = mnt.sbaid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = a.usucpf 
					WHERE 
						anx.anestatus = 'A'::bpchar
					AND
						mntstatus = 'A'::bpchar
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" );
			
		break;
	}
	
	$db->monta_lista($sql,$cabecalho,10,10,"N","center","","form_arquivo");
}