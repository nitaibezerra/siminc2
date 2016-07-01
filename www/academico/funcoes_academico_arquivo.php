<?php

function carregarMenuAcademico() {

	$menu[] = array("id" => 0, "descricao" => "Anexos de Editais e Portarias", "link" => "/academico/academico.php?modulo=sistema/public_arquivo/academico_arquivo&acao=A"); 
	return $menu;
	
}

function montaListaArquivosAcademico()
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
			
	monta_titulo( "Recuperação dos arquivos anexos de editais e portarias", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
			
	array_push($cabecalho, "Nº Portaria", "Ano","Tipo de Portaria","Programa","ID do Arquivo", "Nome do arquivo", "Descrição do Arquivo", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
			
	$sql = "SELECT distinct
				".($arrCampos ? implode(",",$arrCampos)."," : "" )." 
				por.prtnumero,
				por.prtano,
				tp.tprdsc as tipoportaria,
				pg.prgdsc as programa,
				a.arqid,
				a.arqnome||'.'||a.arqextensao,
				a.arqdescricao,
				a.arqtamanho,
				to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
				'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
			FROM 
				arquivo a
			INNER JOIN 
				academico.anexos anx ON anx.arqid = a.arqid
			INNER JOIN
				academico.portarias por ON por.prtid = anx.prtid
			INNER JOIN 
				academico.tipoportaria tp ON tp.tprid = por.tprid
			LEFT JOIN 
				academico.programa pg ON pg.prgid = por.prgid
			INNER JOIN
				seguranca.usuario u ON u.usucpf = a.usucpf 
			WHERE 
				anxstatus = 'A'::bpchar 
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "" );
	
	return array("sql" => $sql, "cabecalho" => $cabecalho);
}