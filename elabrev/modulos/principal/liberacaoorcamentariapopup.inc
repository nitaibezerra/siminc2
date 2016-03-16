<?php
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script>
function ajaxatualizar(params,iddestinatario, pai) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}else{
						document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}	
				} 
			},
			onLoading: function(){
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}else{
						document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}
				}	
			}
		});
}

function gersubacao(obj, entid, sbaid) {

	window.opener.document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';

	if(obj.checked) {
		ajaxatualizar('requisicao=inserirSubacaoLiberacao&entid='+entid+'&sbaid='+sbaid, '');
	} else {
		ajaxatualizar('requisicao=removerSubacaoLiberacao&entid='+entid+'&sbaid='+sbaid, '');
	}
	ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid, true);
}

function gerpi(obj, entid, plicod) {

	window.opener.document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';

	if(obj.checked) {
		ajaxatualizar('requisicao=inserirPILiberacao&plicod='+plicod, '');
	} else {
		ajaxatualizar('requisicao=removerPILiberacao&plicod='+plicod, '');
	}
	ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid, true);
}

function gerptres(obj, entid, ptrid) {

	window.opener.document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';

	if(obj.checked) {
		ajaxatualizar('requisicao=inserirPtresLiberacao&ptrid='+ptrid, '');
	} else {
		ajaxatualizar('requisicao=removerPtresLiberacao&ptrid='+ptrid, '');
	}
	ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid, true);
}

function gerfontedet(obj, entid, foscod) {

	window.opener.document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';

	if(obj.checked) {
		ajaxatualizar('requisicao=inserirFontedetalhadaLiberacao&foscod='+foscod, '');
	} else {
		ajaxatualizar('requisicao=removerFontedetalhadaLiberacao&foscod='+foscod, '');
	}
	ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid, true);
}

function gernaturezades(obj, entid, ndpid) {

	window.opener.document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';
	
	if(obj.checked) {
		ajaxatualizar('requisicao=inserirNaturezadespesaLiberacao&ndpid='+ndpid, '');
	} else {
		ajaxatualizar('requisicao=removerNaturezadespesaLiberacao&ndpid='+ndpid, '');
	}
	ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid, true);
}

function mostrahistoricoplanointerno( pliid ){
	window.open('?modulo=principal/popuphistoricoplanointernoUN&acao=A&pliid='+pliid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
}
</script>
</head>
<body>
<?
$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

switch($_REQUEST['tipo']) {
	case 'subacao':
		if($_REQUEST['sbacod']) {
			$filtro[] = "sub.sbacod ilike '%".$_REQUEST['sbacod']."%'";
		}
		
		if($_REQUEST['sbadsc']) {
			$filtro[] = "sub.sbadsc ilike '%".$_REQUEST['sbadsc']."%'";
		}
		
		$sql = "SELECT '<input type=\"checkbox\" ".disabled($estadoAtual)." onclick=\"gersubacao(this, \''||sbu.entid||'\', \''||sub.sbaid||'\');\" '||
					   COALESCE((SELECT CASE 
					   					WHEN lbrid IS NULL 
					   					THEN '' 
					   					ELSE 'checked' 
					   					END as ck  
					   			FROM elabrev.liberacaosubacao l
								INNER JOIN elabrev.liberacaodetalhe ld ON ld.lbdid=l.lbdid 
				 				WHERE l.sbaid=sub.sbaid AND 
				 					  ld.lbrid=".$_SESSION['elabrev_var']['lbrid']." AND 
				 					  ld.entid=sbu.entid AND 
				 					  l.lbsstatus='A'), '')
				 	   ||'>' as chk, 
				 	   sub.sbacod, 
				 	   sub.sbatitulo 
				FROM monitora.subacao sub 
				INNER JOIN monitora.subacaounidade sbu ON sub.sbaid=sbu.sbaid 
				WHERE entid='".$_REQUEST['entid']."' ".(($filtro)?"AND ".implode(" AND ", $filtro):"")."
				GROUP BY sub.sbaid, sbu.entid, sub.sbacod, sub.sbatitulo";
		?>
		<form action="elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&entid=<? echo $_REQUEST['entid']; ?>" method="post">
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td><? echo campo_texto('sbacod', 'N', 'S', 'Código', 10, 4, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita">Descrição:</td>
			<td><? echo campo_texto('sbadsc', 'N', 'S', 'Descrição', 20, 150, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="window.location='elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&entid=<? echo $_REQUEST['entid']; ?>';"></td>
		</tr>
		</table>
		</form>
		<?
		$cabecalho = array("", "Código", "Descrição");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		break;
	case 'pi':
		
		if($_REQUEST['plicod']) {
			$filtro[] = "pli.plicod ilike '%".$_REQUEST['plicod']."%'";
		}
		
		if($_REQUEST['plititulo']) {
			$filtro[] = "pli.plititulo ilike '%".$_REQUEST['plititulo']."%'";
		}
		
		
		$sql = "SELECT '<input type=\"checkbox\" ".disabled($estadoAtual)." onclick=\"gerpi(this, \''||lid.entid||'\', \''||pli.plicod||'\');\"'||
						COALESCE((SELECT CASE 
										 WHEN lbpid IS NULL 
										 THEN '' 
										 ELSE 'checked' 
										 END as ck 
						 		  FROM elabrev.liberacaopi l
						 		  INNER JOIN elabrev.liberacaosubacao lds ON lds.lbsid=l.lbsid
						 		  INNER JOIN elabrev.liberacaodetalhe ld ON ld.lbdid=lds.lbdid 
						 		  WHERE lds.sbaid=sub.sbaid AND 
						 		  		ld.lbrid=".$_SESSION['elabrev_var']['lbrid']." AND 
						 		  		ld.entid=lid.entid AND
						 		  		l.lbsid='".$_REQUEST['lbsid']."' AND  
						 		  		l.plicod=pli.plicod AND 
						 		  		l.lbpstatus='A'), '') 
				 		|| '>' as chk, 
				 		'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrahistoricoplanointerno(\'' || pli.pliid || '\');\">' || pli.plicod || '</a>', 
				 		pli.plititulo 
				FROM monitora.planointerno pli
				INNER JOIN monitora.planointernoatividade pia ON pia.pliid=pli.pliid 
				INNER JOIN pde.atividade atv ON atv.atiid=pia.atiid 
				INNER JOIN elabrev.liberacaodetalhe lid ON lid.entid=atv.entid 
				INNER JOIN elabrev.liberacaosubacao sub ON sub.lbdid=lid.lbdid AND sub.sbaid=pli.sbaid
				WHERE sub.lbsid='".$_REQUEST['lbsid']."' AND pli.plisituacao='S' AND plistatus='A' ".(($filtro)?"AND ".implode(" AND ", $filtro):"")."
				GROUP BY sub.sbaid, lid.entid, pli.plicod, pli.plititulo, pli.pliid";
		
		$cabecalho = array("", "Código", "Descrição");
		?>
		<form action="elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbsid=<? echo $_REQUEST['lbsid']; ?>" method="post">
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td><? echo campo_texto('plicod', 'N', 'S', 'Código', 10, 4, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita">Título:</td>
			<td><? echo campo_texto('plititulo', 'N', 'S', 'Descrição', 20, 150, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="window.location='elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbsid=<? echo $_REQUEST['lbsid']; ?>';"></td>
		</tr>
		</table>
		</form>
		<?
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		break;
	case 'ptres':
		
		if($_REQUEST['ptres']) {
			$filtro[] = "ptr.ptres ilike '%".$_REQUEST['ptres']."%'";
		}
		
		$sql = "SELECT '<input type=\"checkbox\" ".disabled($estadoAtual)." onclick=\"gerptres(this, \''||lid.entid||'\', \''||ptr.ptrid||'\');\" '||
				COALESCE((SELECT CASE 
	   					  WHEN lptid IS NULL 
						  THEN '' 
						  ELSE 'checked' 
						  END as ck  
						  FROM elabrev.liberacaoptres l
						  INNER JOIN elabrev.liberacaopi ld ON ld.lbpid=l.lbpid 
						  WHERE l.ptrid=ptr.ptrid AND 
							    l.lbpid='".$_REQUEST['lbpid']."' AND 
								l.lptstatus='A'), '')
				||'>' as chk, ptr.ptres, ptr.unicod||'.'||ptr.prgcod||'.'||ptr.acacod||' - '||aca.acadsc as desc, to_char(ptr.ptrdotacao, '99g999g999g999d99') FROM monitora.ptres ptr
				INNER JOIN monitora.planointerno pli ON pli.pliptres=ptr.ptres  
				INNER JOIN monitora.acao aca ON aca.acaid=ptr.acaid
				INNER JOIN elabrev.liberacaopi lpi ON TRIM(lpi.plicod)=pli.plicod 
				INNER JOIN elabrev.liberacaosubacao sub ON sub.lbsid=lpi.lbsid 
				INNER JOIN elabrev.liberacaodetalhe lid ON lid.lbdid=sub.lbdid 
				WHERE lpi.lbpid='".$_REQUEST['lbpid']."' AND ptr.ptrano='2009' ".(($filtro)?"AND ".implode(" AND ", $filtro):"")." GROUP BY lid.entid, ptr.ptrid, ptr.ptres, ptr.unicod, ptr.prgcod, ptr.acacod, aca.acadsc, ptr.ptrdotacao";
		
		$cabecalho = array("", "Código","Ação","Dotação");
		?>
		<form action="elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbpid=<? echo $_REQUEST['lbpid']; ?>" method="post">
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">PTRES:</td>
			<td><? echo campo_texto('ptres', 'N', 'S', 'Código', 10, 4, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="window.location='elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbpid=<? echo $_REQUEST['lbpid']; ?>';"></td>
		</tr>
		</table>
		</form>
		<?
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		break;
	case 'fontedetalhada':
		
		if($_REQUEST['foscod']) {
			$filtro[] = "fsf.foscod ilike '%".$_REQUEST['foscod']."%'";
		}
		
		if($_REQUEST['fosdsc']) {
			$filtro[] = "fsf.fosdsc ilike '%".$_REQUEST['fosdsc']."%'";
		}

		$sql = "SELECT '<input type=\"checkbox\" ".disabled($estadoAtual)." onclick=\"gerfontedet(this, \''||lid.entid||'\', \''||fsf.foscod||'\');\" '||
				COALESCE((SELECT CASE 
	   					  WHEN lbfid IS NULL 
						  THEN '' 
						  ELSE 'checked' 
						  END as ck  
						  FROM elabrev.liberacaofonte l
						  INNER JOIN elabrev.liberacaoptres ld ON ld.lptid=l.lptid 
						  WHERE l.foscod=fsf.foscod AND 
							    l.lptid='".$_REQUEST['lptid']."' AND 
								l.lbfstatus='A'), '')
				||'>' as chk, fsf.foscod, fsf.fosdsc FROM financeiro.fontesiafi fsf
				INNER JOIN financeiro.unidadeptresfonte upf ON upf.fontesiafi=fsf.foscod 
				INNER JOIN monitora.ptres pes ON pes.ptres=upf.ptres 
				INNER JOIN elabrev.liberacaoptres res ON res.ptrid=pes.ptrid
				INNER JOIN elabrev.liberacaopi pi ON pi.lbpid=res.lbpid
				INNER JOIN elabrev.liberacaosubacao sub ON sub.lbsid=pi.lbsid
				INNER JOIN elabrev.liberacaodetalhe lid ON lid.lbdid=sub.lbdid
				WHERE res.lptid='".$_REQUEST['lptid']."' ".(($filtro)?"AND ".implode(" AND ", $filtro):"")." GROUP BY lid.entid, fsf.foscod, fsf.fosdsc";
		
		$cabecalho = array("", "Código", "Descrição");
		?>
		<form action="elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lptid=<? echo $_REQUEST['lptid']; ?>" method="post">
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td><? echo campo_texto('foscod', 'N', 'S', 'Código', 10, 30, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita">Descrição:</td>
			<td><? echo campo_texto('fosdsc', 'N', 'S', 'Código', 10, 100, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="window.location='elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lptid=<? echo $_REQUEST['lptid']; ?>';"></td>
		</tr>
		</table>
		</form>
		<?
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		break;
		
	case 'naturezadespesa':
		
		if($_REQUEST['ndpcod']) {
			$filtro[] = "nad.ndpcod ilike '%".$_REQUEST['ndpcod']."%'";
		}
		
		if($_REQUEST['ndpdsc']) {
			$filtro[] = "nad.ndpdsc ilike '%".$_REQUEST['ndpdsc']."%'";
		}

		$sql = "SELECT '<input type=\"checkbox\" ".disabled($estadoAtual)." onclick=\"gernaturezades(this, \''||
		(SELECT entid FROM elabrev.liberacaodetalhe lid 
				INNER JOIN elabrev.liberacaosubacao sub ON lid.lbdid=sub.lbdid 
				INNER JOIN elabrev.liberacaopi pi ON sub.lbsid=pi.lbsid 
				INNER JOIN elabrev.liberacaoptres res ON pi.lbpid=res.lbpid 
				INNER JOIN elabrev.liberacaofonte lbf ON lbf.lptid=res.lptid
				WHERE lbfid='".$_REQUEST['lbfid']."')||'\', \''||nad.ndpid||'\');\" '|| 
				COALESCE((SELECT CASE 
					   					WHEN lbnid IS NULL 
					   					THEN '' 
					   					ELSE 'checked' 
					   					END as ck  
					   			FROM elabrev.liberacaonatureza l
								INNER JOIN elabrev.liberacaofonte ld ON ld.lbfid=l.lbfid 
				 				WHERE l.ndpid=nad.ndpid AND 
				 					  l.lbfid='".$_REQUEST['lbfid']."' AND 
				 					  l.lbnstatus='A'), '')
				||'>' as chk, SUBSTR(nad.ndpcod,1,1)||'.'||SUBSTR(nad.ndpcod,2,1)||'.'||SUBSTR(nad.ndpcod,3,2)||'.'||SUBSTR(nad.ndpcod,5,2)||'.'||SUBSTR(nad.ndpcod,7,2) as ndpcod, nad.ndpdsc FROM public.naturezadespesa nad ".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")." ORDER BY nad.ndpcod";
		
		
		$cabecalho = array("", "Código", "Descrição");
		?>
		<form action="elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbfid=<? echo $_REQUEST['lbfid']; ?>" method="post">
		<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td><? echo campo_texto('ndpcod', 'N', 'S', 'Código', 10, 30, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita">Descrição:</td>
			<td><? echo campo_texto('ndpdsc', 'N', 'S', 'Descrição', 10, 100, "", "", '', '', 0, '' ); ?></td>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="window.location='elabrev.php?modulo=principal/liberacaoorcamentariapopup&acao=A&tipo=<? echo $_REQUEST['tipo']; ?>&lbfid=<? echo $_REQUEST['lbfid']; ?>';"></td>
		</tr>
		</table>
		</form>
		<?
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		break;
}
?>
<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td align="right"><input type="button" value="Ok" onclick="window.close();"></td>
</tr>
</table>
</body>
</html>