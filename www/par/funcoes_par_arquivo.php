<?php

function carregarMenuPar() {
	global $db;

	if($db->testa_superuser()){
		$menu[] = array("id" => 2, "descricao" => "Documentos", "link" => "/par/par.php?modulo=sistema/public_arquivo/par_arquivo&acao=A", "tabela" => "preobrasanexo");
		$menu[] = array("id" => 1, "descricao" => "Vistorias", "link" => "/par/par.php?modulo=sistema/public_arquivo/par_arquivo&acao=A&tbl=preobrasfotos" , "tabela" => "preobrasfotos"); 
	}else{
		$flag_menu = false;
		if(verificaArquivoUsuarioTabela("obras","preobraanexo")){
			$menu[] = array("id" => 2, "descricao" => "Documentos", "link" => "/par/par.php?modulo=sistema/public_arquivo/par_arquivo&acao=A", "tabela" => "preobrasanexo" );
			$flag_menu = true;
		}
		if(verificaArquivoUsuarioTabela("obras","preobrafotos")){
			$menu[] = array("id" => 1, "descricao" => "Vistorias", "link" => "/par/par.php?modulo=sistema/public_arquivo/par_arquivo&acao=A".($flag_menu == false ? "" : "&tbl=preobrasfotos"), "tabela" => "preobrasfotos");
			$flag_menu = true;
		}
		if($flag_menu == false){
			$menu = array();
		}
	}
	return $menu;
	
}

function montaListaArquivosPar($tabela = null)
{
	global $db;
	
	$tabela = $_REQUEST['tbl'] ? $_REQUEST['tbl'] : $tabela;
	
	$tabela = !$tabela ? "preobrasanexo" : $tabela;

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
		
		case "preobrasanexo":
			
			monta_titulo( "Recuperação dos arquivos anexos da obra", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
			array_push($cabecalho, "Nome da Obra", "Tipo da Obra", "Situação da Obra", "Grupo da Obra", "Município/UF","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
			$sql = "SELECT distinct
						".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
						p.predescricao,
						t.ptodescricao,
						ed.esddsc,
						tm.tpmdsc,
						m.mundescricao || ' / ' || m.estuf as municipio,
						a.arqid,
						a.arqnome||'.'||a.arqextensao,
						a.arqdescricao,
						a.arqtamanho,
						to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
						'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
					FROM 
						arquivo a
					JOIN 
						obras.preobraanexo b ON b.arqid = a.arqid
					JOIN 
						obras.preobra p ON p.preid = b.preid
					JOIN 
						obras.pretipoobra t ON t.ptoid = p.ptoid
					JOIN
						workflow.documento d ON d.docid = p.docid
					JOIN 
						workflow.estadodocumento ed ON ed.esdid = d.esdid
					JOIN 
						territorios.municipio m ON m.muncod = p.muncod
					JOIN 
						territorios.muntipomunicipio mtm ON mtm.muncod = p.muncod
					JOIN 
						territorios.tipomunicipio tm ON tm.tpmid = mtm.tpmid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = a.usucpf 
					WHERE 
						a.sisid = 23 
					AND 
						p.prestatus = 'A'::bpchar 
					AND 
						a.arqstatus = 'A'::bpchar 
					AND 
						t.ptostatus = 'A'::bpchar
					AND 
						tm.gtmid = 7
					AND 
						tm.tpmstatus = 'A'::bpchar 
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" ).
				" ORDER BY
					p.predescricao,
					t.ptodescricao,
					ed.esddsc,
					tm.tpmdsc";
		
		break;
		
		case "preobrasfotos":

			monta_titulo( "Recuperação dos arquivos de vistoria da obra", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
			array_push($cabecalho, "Nome da Obra", "Tipo da Obra", "Situação da Obra", "Grupo da Obra", "Município/UF","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
			$sql = "SELECT distinct
						".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
						p.predescricao,
						t.ptodescricao,
						ed.esddsc,
						tm.tpmdsc,
						m.mundescricao || ' / ' || m.estuf as municipio,
						a.arqid,
						a.arqnome||'.'||a.arqextensao,
						a.arqdescricao,
						a.arqtamanho,
						to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
						'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
					FROM arquivo a
              		JOIN 
              			obras.preobrafotos b ON b.arqid = a.arqid
					JOIN 
						obras.preobra p ON p.preid = b.preid
					JOIN 
						obras.pretipoobra t ON t.ptoid = p.ptoid
					JOIN 
						workflow.documento d ON d.docid = p.docid
					JOIN 
						workflow.estadodocumento ed ON ed.esdid = d.esdid
					JOIN 
						territorios.municipio m ON m.muncod = p.muncod
					JOIN 
						territorios.muntipomunicipio mtm ON mtm.muncod = p.muncod
					JOIN 
						territorios.tipomunicipio tm ON tm.tpmid = mtm.tpmid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = a.usucpf
					WHERE 
						a.sisid = 23  
					AND 
						p.prestatus = 'A'::bpchar
					AND 
						a.arqstatus = 'A'::bpchar
					AND 
						t.ptostatus = 'A'::bpchar
					AND 
						tm.gtmid = 7
					AND 
						tm.tpmstatus = 'A'::bpchar 
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" ).
				" ORDER BY
					p.predescricao,
					t.ptodescricao,
					ed.esddsc,
					tm.tpmdsc";
			
		break;
	}
	
	$db->monta_lista($sql,$cabecalho,10,10,"N","center","","form_arquivo");
}