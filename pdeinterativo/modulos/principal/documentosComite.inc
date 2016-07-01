<?php

if($_REQUEST['arqid']) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("documentoscomite", array(), "pdeinterativo");
	$file->getDownloadArquivo($_REQUEST['arqid']);
}

if($_FILES['portaria']['name']) {
	
	/* configurações */
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	/* FIM configurações */
	
	foreach($_FILES['portaria']['name'] as $codigo => $arquivox) {
		if($arquivox) {
			$sql = "INSERT INTO public.arquivo (arqnome,
												arqextensao,
												arqdescricao,
												arqtipo,
												arqtamanho,
												arqdata,
												arqhora,
												usucpf,
												sisid,
												arqstatus)
					VALUES( '".current(explode(".", $arquivox))."',
							'".end(explode(".", $arquivox))."',
							'Portaria - PDE Interativo - ".$codigo."',
							'".$_FILES['portaria']['type'][$codigo]."',
							'".$_FILES['portaria']['size'][$codigo]."',
							'".date('Y-m-d')."',
							'".date('H:i:s')."',
							'".$_SESSION["usucpf"]."',
							".$_SESSION['sisid'].",
							'A') 
					RETURNING arqid;";
			
			$arqid = $db->pegaUm($sql);
			
			$caminhoEarquivo = APPRAIZ.'arquivos/pdeinterativo/'.floor($arqid/1000).'/'.$arqid;
			
			if(!is_dir(APPRAIZ."arquivos/pdeinterativo")) {
				mkdir(APPRAIZ."arquivos/pdeinterativo", 0777);
			}
			
			if(!is_dir(APPRAIZ."arquivos/pdeinterativo/".floor($arqid/1000))) {
				mkdir(APPRAIZ."arquivos/pdeinterativo/".floor($arqid/1000), 0777);
			}
			
			if ( !move_uploaded_file( $_FILES['portaria']['tmp_name'][$codigo], $caminhoEarquivo) ) {
				$erro[] = "Problemas no envio do arquivo.";
			}
			
			if(strlen($codigo)==2) $campo = "estuf";
			else  $campo = "muncod";
			
			$sql = "INSERT INTO pdeinterativo.documentoscomite(
            		{$campo}, arqid)
    				VALUES ('".$codigo."', '".$arqid."');";
						
			$db->executar($sql);
			$db->commit();
						
		}
	}
	
	if($erro) {
		
		echo "<pre>";
		print_r($erro);
		echo $sql;
		exit;
			
	} else {
	
		die("<script>
				alert('Arquivos gravados com sucesso');
				window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A".(($_REQUEST['pdiesfera'])?"&pdiesfera=".$_REQUEST['pdiesfera']:"").(($_REQUEST['estuf'])?"&estuf=".$_REQUEST['estuf']:"")."';
			 </script>");
	
	}
}

function abrirArquivosEstado($dados) {
	global $db;
	
	if($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC, $perfil)) $removerimg=true;
	
	$sql = "SELECT 
			'<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=donwload('||arq.arqid||');>".(($removerimg)?" <img src=../imagens/exclui_p.gif border=0 style=cursor:pointer onclick=\"excluirArquivoComite(\''||dc.dcoid||'\')\">":"")."</center>' as acao, arq.arqnome||'.'||arq.arqextensao as nomearq, arq.arqdescricao, to_char(arq.arqdata, 'dd/mm/YYYY')||' '||arq.arqhora as data
				FROM pdeinterativo.documentoscomite dc 
				INNER JOIN public.arquivo arq ON arq.arqid = dc.arqid 
				WHERE estuf='".$dados['estuf']."' AND dc.dcostatus='A'";
	
	$cabecalho = array("&nbsp;","Nome do arquivo", "Descrição", "Data/Hora");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
}

function abrirArquivosMunicipio($dados) {
	global $db;
	
	$perfil = pegaPerfilGeral();
	
	if($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC, $perfil)) $removerimg=true;
	
	$sql = "SELECT 
			'<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=donwload('||arq.arqid||');>".(($removerimg)?" <img src=../imagens/exclui_p.gif border=0 style=cursor:pointer onclick=\"excluirArquivoComite(\''||dc.dcoid||'\')\">":"")."</center>' as acao, arq.arqnome||'.'||arq.arqextensao as nomearq, arq.arqdescricao, to_char(arq.arqdata, 'dd/mm/YYYY')||' '||arq.arqhora as data
				FROM pdeinterativo.documentoscomite dc 
				INNER JOIN public.arquivo arq ON arq.arqid = dc.arqid 
				WHERE muncod='".$dados['muncod']."' AND dc.dcostatus='A'";
	
	$cabecalho = array("&nbsp;","Nome do arquivo", "Descrição", "Data/Hora");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function excluirArquivoComite($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo.documentoscomite SET dcostatus='I' WHERE dcoid='".$dados['dcoid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Arquivo excluido com sucesso');
			window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&pdiesfera=".$dados['pdiesfera'].(($dados['estuf'])?"&estuf=".$dados['estuf']:"")."';
		  </script>";
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}




include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	
function gravarDocumentoComite() {
	jQuery('#formulario').submit();
}

function donwload(arqid) {
	window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&arqid='+arqid;
}

function filtrarPorUF(estuf) {
	window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&pdiesfera=<?=$_REQUEST['pdiesfera'] ?>&estuf='+estuf;
}

function selecionarEsfera(esfera) {
	window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&pdiesfera='+esfera;
}

function abrirArquivosMunicipio(muncod, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;

	if(obj.title=='mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		col0 = nlinha.insertCell(0);
		col0.colSpan=5;
		col0.id="coluna_"+muncod;
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/documentosComite&acao=A",
	   		data: "requisicao=abrirArquivosMunicipio&muncod="+muncod,
	   		async: false,
	   		success: function(msg){document.getElementById("coluna_"+muncod).innerHTML = msg;}
	 		});
 	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		
		tabela.deleteRow(linha.rowIndex);
 	
 	}

}

function abrirArquivosEstado(estuf, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;

	if(obj.title=='mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		col0 = nlinha.insertCell(0);
		col0.colSpan=5;
		col0.id="coluna_"+estuf;
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/documentosComite&acao=A",
	   		data: "requisicao=abrirArquivosEstado&estuf="+estuf,
	   		async: false,
	   		success: function(msg){document.getElementById("coluna_"+estuf).innerHTML = msg;}
	 		});
 	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		
		tabela.deleteRow(linha.rowIndex);
 	
 	}

}

function excluirArquivoComite(dcoid) {
	var conf = confirm('Deseja realmente excluir o arquivo?');
	if(conf) {
		window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&requisicao=excluirArquivoComite<?=(($_REQUEST['estuf'])?"&estuf=".$_REQUEST['estuf']:"") ?>&pdiesfera=<?=$_REQUEST['pdiesfera'] ?>&dcoid='+dcoid;
	}
}

function donwload(arqid) {
	window.location='pdeinterativo.php?modulo=principal/documentosComite&acao=A&arqid='+arqid;
}

	
</script>
<?
$pdiesfera = $_GET['pdiesfera'];

$arrPerfil = pegaPerfilGeral();

if(!$db->testa_superuser()) {
	
	if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil)){
		$sql = "select mun.muncod
				from pdeinterativo.usuarioresponsabilidade ur
				inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL."
				order by rpuid desc";
		
		$muncod = $db->pegaUm($sql);
		$pdiesfera = "Municipal";
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
		$sql = "select mun.muncod
				from pdeinterativo.usuarioresponsabilidade ur
				inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL."
				order by rpuid desc";
		
		$muncod = $db->pegaUm($sql);
		$pdiesfera = "Municipal";
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil)){
		$sql = "select estuf
				from pdeinterativo.usuarioresponsabilidade ur
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL."
				order by rpuid desc";

		$estuf = $db->pegaUm($sql);
		$pdiesfera = "Estadual";
		
	}
	
	if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil)){
		$sql = "select estuf
				from pdeinterativo.usuarioresponsabilidade ur
				where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_ESTADUAL."
				order by rpuid desc";

		$estuf = $db->pegaUm($sql);
		$pdiesfera = "Estadual";
		
	}

}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Esfera:</td>
	<td>
	<?
	$esfera = array(0=>array("codigo"=>"Municipal","descricao"=>"Municipal"),1=>array("codigo"=>"Estadual","descricao"=>"Estadual"));
	$db->monta_combo('pdiesfera', $esfera, 'S', 'Selecione', 'selecionarEsfera', '', '', '200', 'N', 'pdiesfera', false, $pdiesfera);
	?>
	</td>
</tr>
</table>
<? if($pdiesfera=="Municipal") : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="25%">Estado:</td>
	<td>
	<?
	$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estdescricao";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'filtrarPorUF', '', '', '200', 'N', 'estuf', false, $_REQUEST['estuf']);
	?>
	</td>
</tr>
</table>
<?
$where[] = "pde.pdiesfera='{$pdiesfera}'";
$where[] = "pde.pdistatus='A'";

if($_REQUEST['estuf']) { $where[] = "mun.estuf='".$_REQUEST['estuf']."'";}

$sql = "SELECT '<img title=mais src=../imagens/mais.gif border=0 style=cursor:pointer; onclick=\"abrirArquivosMunicipio(\''||mun.muncod||'\', this);\">',
			   mun.estuf, 
			   mun.mundescricao, 
			   '<input type=\"file\" name=\"portaria['||mun.muncod||']\">' as portaria,
  			   (SELECT COUNT(*) as num FROM pdeinterativo.documentoscomite dc WHERE dc.muncod=mun.muncod AND dc.dcostatus='A' ) as numarq
 		FROM pdeinterativo.pdinterativo pde 
		INNER JOIN territorios.municipio mun ON mun.muncod = pde.muncod 
		".(($where)?"WHERE ".implode(" AND ",$where):"")." 
		GROUP BY mun.estuf, mun.mundescricao, mun.muncod 
		ORDER BY mun.estuf, mun.mundescricao";

$cabecalho = array("&nbsp;", "UF", "Município", "Enviar portaria", "Quantidade de arquivos");
$db->monta_lista($sql,$cabecalho,15,10,'N','center',$par2,"formulario");

elseif($pdiesfera=="Estadual") :

$sql = "SELECT '<img title=mais src=../imagens/mais.gif border=0 style=cursor:pointer; onclick=\"abrirArquivosEstado(\''||est.estuf||'\', this);\">',
			   est.estuf, 
			   est.estdescricao, 
			   '<input type=\"file\" name=\"portaria['||est.estuf||']\">' as portaria,
  			   (SELECT COUNT(*) as num FROM pdeinterativo.documentoscomite dc WHERE dc.estuf=est.estuf AND dc.dcostatus='A' ) as numarq
 		FROM territorios.estado est  
		".(($where)?"WHERE ".implode(" AND ",$where):"")." 
		GROUP BY est.estuf, est.estdescricao 
		ORDER BY est.estuf, est.estdescricao";

$cabecalho = array("&nbsp;", "UF", "Município", "Enviar portaria", "Quantidade de arquivos");
$db->monta_lista($sql,$cabecalho,15,10,'N','center',$par2,"formulario");

endif;

			   //'<center><input type=button name=download value=DOWNLOAD onclick=donwload('||arq.arqid||');></center>' as download
			   //	LEFT JOIN pdeinterativo.documentoscomite dco ON dco.muncod = mun.muncod 
		//LEFT JOIN public.arquivo arq ON arq.arqid = dco.arqid
			   
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloCentro'><input type="button" name="gravar" value="Gravar" onclick="gravarDocumentoComite();"></td>
</tr>
</table>
