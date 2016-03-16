<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_POST['requisicao'] == 'salvar' ){
	
	if($_FILES["arquivo"] && !$_POST['arqid']){	
		if( $_POST['tipo'] == 'par' ){
			$campos	= array("prpid"  => $_POST['codigo'],
							"prddescricao" => "'{$_POST['prddescricao']}'",
							"usucpf" => "'{$_SESSION['usucpf']}'"
							);
		}		
		if( $_POST['tipo'] == 'obras' ){
			$campos	= array("proid_par"  => $_POST['codigo'],
							"prddescricao" => "'{$_POST['prddescricao']}'",
							"usucpf" => "'{$_SESSION['usucpf']}'"
							);
		}
		if( $_POST['tipo'] == 'pac' ){
			$campos	= array("proid_pac"  => $_POST['codigo'],
							"prddescricao" => "'{$_POST['prddescricao']}'",
							"usucpf" => "'{$_SESSION['usucpf']}'"
							);
		}
						
		$file = new FilesSimec("processodocumento", $campos ,"par");
		$arquivoSalvo = $file->setUpload();
		if($arquivoSalvo){
			$db->commit();
			$db->sucesso('principal/popupProcessoDocumento', '&&codigo='.$_POST['codigo'].'&tipo='.$_POST['tipo']);
			die;	
		}
	} elseif($_POST['arqid']) {
		
	    $sql = "UPDATE par.processodocumento SET prddescricao = '{$_POST['prddescricao']}' where arqid=".$_POST['arqid'];
	    $db->executar($sql);
	    $db->commit();
	    $db->sucesso('principal/popupProcessoDocumento', '&&codigo='.$_POST['codigo'].'&tipo='.$_POST['tipo']);
	    die;
	}
}

if( $_POST['requisicao'] == 'alterar' ){
	
	$sql = "SELECT
				d.arqid,
			    d.prddescricao,
			    a.arqnome||'.'||a.arqextensao as arquivo
			FROM
				par.processodocumento d
				inner join public.arquivo a on a.arqid = d.arqid
			WHERE
				d.arqid = {$_POST['arqid']}";
	$arrDados = $db->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();
	extract($arrDados);
}
if( $_POST['requisicao'] == 'excluir' ){
	$sql = "UPDATE par.processodocumento set prdstatus = 'I' where arqid=".$_POST['arqid'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_POST['arqid'];
    $db->executar($sql);
	
    $db->commit();
	$db->sucesso('principal/popupProcessoDocumento', '&&codigo='.$_POST['codigo'].'&tipo='.$_POST['tipo']);
    die;
}
if( $_POST['requisicao'] == 'download' ){
	$file = new FilesSimec();
	$arqid = $_POST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    exit;
}
/*
 * obras
 * par
 * pac
 */
?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
	<body>
		<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input type="hidden" name="codigo" value="<?=$_REQUEST['codigo'] ?>">
			<input type="hidden" name="tipo" value="<?=$_REQUEST['tipo'] ?>">
			<input type="hidden" name="arqid" value="<?=$_REQUEST['arqid'] ?>">
		
		<table align="center" border="0" width="100%" cellpadding="3" cellspacing="2">
			<tr>
				<th colspan="2">Documentos Diversos</th>
			</tr>
			<tr>
				<td class="subtitulodireita" width="25%">Descrição:</td>
				<td><?=campo_texto('prddescricao', 'N', 'S', '', 80, 200, '', '', '', '', 0, '', '', $prddescricao ); ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Documento:</td>
				<td>
				<?if( $arquivo ){ ?>
					<a href="" style="cursor: pointer;" onclick="donwloadAnexo( <?=$arqid; ?> );"><?=$arquivo; ?></a>
				<?} else { ?>
					<input type="file" name="arquivo">
				<?} ?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="2" align="center">
					<input type="button" onclick="javascript: salvar();" value="Salvar">
					<input type="button" onclick="javascript: window.close();" value="Fechar">
				</td>
			</tr>
		</table>
	</form>
	<table align="center" border="0" width="100%" cellpadding="3" cellspacing="2">
		<tr>
			<th colspan="2">Lista de Documentos Diversos</th>
		</tr>
	<?
	if( $_REQUEST['tipo'] == 'par' ){
		$inner = " inner join par.processopar p on p.prpid = d.prpid and p.prpstatus = 'A'";
		$where = " and d.prpid = {$_REQUEST['codigo']} ";
	}
	if( $_REQUEST['tipo'] == 'obras' ){
		$inner = " inner join par.processoobraspar p on p.proid = d.proid_par and p.prostatus = 'A'";
		$where = " and d.proid_par = {$_REQUEST['codigo']} ";
	}
	if( $_REQUEST['tipo'] == 'pac' ){
		$inner = " inner join par.processoobra p on p.proid = d.proid_pac  and p.prostatus = 'A' ";
		$where = " and d.proid_pac = {$_REQUEST['codigo']} ";
	}
	
	$sql = "SELECT
				'<center><img src=\"../imagens/alterar.gif\" onclick=\"alterarAnexo('||d.arqid||');\" style=\"border:0; cursor:pointer;\" title=\"Alterar Descrição Anexo\">&nbsp;
    			 <img src=\"../imagens/excluir.gif\" onclick=\"excluirAnexo('||d.arqid||');\" style=\"border:0; cursor:pointer;\" title=\"Excluir Documento Anexo\">&nbsp;
    			 <img src=\"../imagens/anexo.gif\" onclick=\"donwloadAnexo('||d.arqid||');\" style=\"border:0; cursor:pointer;\" title=\"Donwload do Anexo\"></center>' as acoes,
			    d.prddescricao,
			    to_char(d.prddata, 'DD/MM/YYYY HH24:MI:SS') as data,
			    a.arqnome||'.'||a.arqextensao as arquivo,
			    a.arqtipo,
			    u.usunome
			FROM
				par.processodocumento d
			    inner join public.arquivo a on a.arqid = d.arqid
			    inner join seguranca.usuario u on u.usucpf = d.usucpf
			    $inner
			WHERE
				prdstatus = 'A'
				$where";
	$cabecalho = array("Ação", "Descrição", "Data", "Arquivo", "Tipo", "Usuário");
	$db->monta_lista_simples($sql, $cabecalho, 200, 4, 'N', '100%', 'S', '', '', '', true);
	?>
	</table>
</body>
<script type="text/javascript">

function salvar(){
	if( $('[name=prddescricao]').val() == '' ){
		alert('O campo Descrição é obrigatório!');
		$('[name=prddescricao]').focus();
		return false;
	} else
	if( $('[name=arquivo]').val() == '' ){
		alert('O campo Documento é obrigatório!');
		$('[name=arquivo]').focus();
		return false;
	} else {
		$('[name=requisicao]').val('salvar');
		$('[name=formulario]').submit();
	}
}

function alterarAnexo( arqid ){
	$('[name=requisicao]').val('alterar');
	$('[name=arqid]').val(arqid);
	$('[name=formulario]').submit();
}

function excluirAnexo( arqid ){
	$('[name=requisicao]').val('excluir');
	$('[name=arqid]').val(arqid);
	$('[name=formulario]').submit();
}

function donwloadAnexo( arqid ){
	$('[name=requisicao]').val('download');
	$('[name=arqid]').val(arqid);
	$('[name=formulario]').submit();
}
</script>
</html>