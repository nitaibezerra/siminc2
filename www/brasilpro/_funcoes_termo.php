<?php

function downloadanexotermo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array();
	$file = new FilesSimec("termocompromissopac",$arrCampos,"cte");
	$file->getDownloadArquivo($dados['arqid']);
}



function excluiranexotermo($dados) {
	global $db;
	
	$sql = "UPDATE cte.termocompromissopac SET arqidanexo=NULL WHERE terid='".$_REQUEST['terid']."'";
	$db->executar($sql);
	$db->commit();
	
	die("<script>
			alert('Anexo removido');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");
	
}

function inseriranexotermo($dados) {
	global $db;
	
	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("termocompromissopac",$arrCampos,"par");
		$file->setUpload("","arquivo",false);
		$arqid = $file->getIdArquivo();
	}

	$sql = "UPDATE cte.termocompromissopac SET arqidanexo='".$arqid."' WHERE terid='".$_POST['terid']."'";
	$db->executar($sql);
	$db->commit();
	
	
	die("<script>
			alert('Anexado com sucesso.');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");

}


function telaanexo($dados) {
	global $db;
	
	if($dados['terid']) {
		$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM cte.termocompromissopac t
				INNER JOIN public.arquivo a ON a.arqid = t.arqidanexo   
				WHERE terid='".$dados['terid']."'";
	} else {
		$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM cte.processoobraanexo p
				INNER JOIN public.arquivo a ON a.arqid = p.arqid   
				WHERE proid='".$dados['proid']."'";
	}
	
	$arquivos = $db->carregar($sql);
	
	echo "<form method=post name=formulario id=formularioanexo enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=".$dados['req'].">";
	echo "<input type=hidden name=urlgo value=".$dados['urlgo'].">";
	echo "<input type=hidden name=terid value=".$dados['terid'].">";	
	echo "<input type=hidden name=proid value=".$dados['proid'].">";
	echo "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Anexo:</td>";
	echo "<td><input type=file name=arquivo id=arquivo ".(($arquivos[0])?"disabled":"")."></td>";
	echo "</tr>";
	if($arquivos[0]) {
		foreach($arquivos as $arquivo) {
			echo "<tr>";
			echo "<td class=\"SubTituloDireita\">Nome do arquivo:</td>";
			echo "<td>".$arquivo['arquivo']." <input type=button name=download value=Download onclick=window.location='cte.php?modulo=principal/obras/gerarTermoObra&acao=A&arqid=".$arquivo['arqid']."&requisicao=downloadanexotermo';> <input type=button name=excluir value=Excluir onclick=excluirAnexoTermo(".$dados['terid'].");></td>";
			echo "</tr>";
		}
	}
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=2><input type=button name=salvar value=Salvar onclick=enviarAnexoTermo();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function download(){
	
	global $db;
	
	$sql = "SELECT
				arqid
			FROM
				cte.termocompromissopac
			WHERE
				terid = ".$_REQUEST['terid'];

	$arqid = $db->pegaUm($sql);
//	ver($arqid,d);
	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	$file 		= new FilesSimec('termocompromissopac', NULL, 'par');
	$file->getDownloadArquivo($arqid);
	die();
}

function cabecalhoSolicitacaoTermo() {
	global $db;

	$arrDados = $db->pegaLinha("SELECT 
									m.muncod,
									m.estuf,
									m.mundescricao,
									p.pronumeroprocesso,
									CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
									p.protipo
								FROM cte.processoobra p
							    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
							    WHERE 
							    	m.muncod = '".$_REQUEST['muncod']."' AND 
							    	p.proid = '".$_SESSION['par_var']['proid']."'");

	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Número do Processo:</td>";
	echo "<td>".$arrDados['pronumeroprocesso']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>UF:</td>";
	echo "<td>".$arrDados['estuf']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Município:</td>";
	echo "<td>".$arrDados['mundescricao']."</td>";
	echo "</tr>";
	echo "<tr>";
//	echo "<td class=SubTituloDireita>Nº processo:</td>";
//	echo "<td>".$arrDados['pronumeroprocesso']."</td>";
//	echo "</tr>";
//	echo "<tr>";
	echo "<td class=SubTituloDireita>Tipo obra:</td>";
	echo "<td>".$arrDados['tipoobra']."</td>";
	echo "</tr>";
	echo "</table>";
	
	return $arrDados['protipo'];
}

function excluirTermo() {
	
	global $db;
	
	$sql = "UPDATE cte.termocompromissopac
			SET
				terstatus = 'I'
			WHERE
				terid = ".$_REQUEST['terid'];
	
	$db->executar($sql);
	$db->commit();
}

function listaTermo($dados) {
	global $db;

	$sql = "SELECT DISTINCT
				'<img src=../imagens/print.gif     title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermo('||t.terid||');><img src=../imagens/reject.png     title=\"Enviar anexo\" style=cursor:pointer; onclick=enviarAnexo('||t.terid||');><img src=../imagens/excluir_2.gif title=\"Excluir\" width=\"15px\" style=\"cursor:pointer;\" onclick=excluirTermos('||t.terid||');>' as acao,
				en.entnumcpfcnpj, 
				en.entnome, 
				'PAC2'||to_char(t.terid,'00000')||'/'||to_char(t.terdatainclusao,'YYYY') as ternum, 
				u.usunome, 
				to_char(t.terdatainclusao,'DD/MM/YYYY') as terdata,
				'<a style=cursor:pointer onclick=\"window.location=\'cte.php?modulo=principal/obras/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo 
			FROM 
				cte.termocompromissopac t
			LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
			LEFT  JOIN entidade.endereco 	  ende ON ende.muncod = t.muncod
			LEFT  JOIN entidade.entidade 	    en ON en.entid    = ende.entid
			LEFT  JOIN entidade.funcaoentidade fun ON fun.entid   = en.entid
			LEFT  JOIN public.arquivo arq ON arq.arqid   = t.arqidanexo
			WHERE 
				ende.muncod = '".$dados['muncod']."'
				AND t.proid = '".$dados['proid']."'
				AND funid = 1
				AND ende.endstatus = 'A'
				AND t.terstatus = 'A'";

	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº Termo","Usuário criação","Data da Criação","Anexo");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}


function listaPreObrasTermo($dados) {
	global $db;
	
	if( $dados['tipoobra'] == 'P' ){
		$ptoids = "1,2,3,6,7";
	}else{
		$ptoids = "5";
	}
	
	$sql = "SELECT
				'<input type=hidden name=preids[] id='|| pre.preid ||' value='|| pre.preid ||'><input type=\"checkbox\" name=\"preids_\" checked disabled />' as check,
				pre.predescricao,
				pto.ptodescricao,
				prevalorobra
			FROM
				obras.preobra pre
			INNER JOIN cte.empenhoobra   emp ON emp.preid = pre.preid 
			INNER JOIN cte.empenho   emn ON emn.empid = emp.empid
			INNER JOIN cte.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
			WHERE
				pro.proid = '".$_SESSION['par_var']['proid']."' AND pre.muncod = '{$dados['muncod']}' AND pre.ptoid IN ({$ptoids})
			ORDER BY
				pto.ptodescricao,pre.predescricao";

	echo "<form id=\"formpreobras\">";
	$cabecalho = array("&nbsp;","Nome da obra","Tipo da Obra","Valor da obra");
	$celWidth  = array("5%","45%","25%","25%");
	$celAlign  = array("center","left","left","right");
	$db->monta_lista($sql,$cabecalho,500,5,'N','','S',"formpreobras",$celWidth,$celAlign);
	echo "</form>";

}




function assinarTermo($dados) {
	global $db;
	
	if($dados['terid']) {
		$sql = "UPDATE cte.termocompromissopac SET terassinado=".$dados['terassinado']." WHERE terid='".$dados['terid']."'";
		$db->executar($sql);
		$db->commit();
	}
}

function inseriranexotermoprocesso($dados) {
	global $db;
	
	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("processoobraanexo",$arrCampos,"par");
		$file->setUpload("","arquivo",false);
		$arqid = $file->getIdArquivo();
	}
	
	if($arqid) {
		
		$sql = "INSERT INTO cte.processoobraanexo(
	            usucpf, poadatainclusao, poastatus, arqid, proid)
	    		VALUES ('".$_SESSION['usucpf']."', NOW(), 'A', '".$arqid."', '".$_POST['proid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	
	die("<script>
			alert('Anexado com sucesso.');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");
}

?>