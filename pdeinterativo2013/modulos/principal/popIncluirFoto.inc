<?php

if( $_POST['submetido'] )
{
	if(!$_REQUEST['aefid'] || !$_REQUEST['pdeid'] || $_FILES['arquivo']['error']!=0) { 
		die("<script>alert(\"Foto não foi enviada corretamente. Tente novamente!\");window.opener.location.href = window.opener.location.href;self.close();</script>");
	}
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array('aefid' => $_REQUEST['aefid'], 'pdeid' => $_REQUEST['pdeid'], 'gfodescricao' => (($_REQUEST['gfodescricao'])?"'".$_REQUEST['gfodescricao']."'":"NULL"));
	$file = new FilesSimec("galeriafoto", $campos, "pdeinterativo2013");
	
	$arqdescricao = 'foto_galeria_pdeinterativo2013';
	
	if( $file->setUpload($arqdescricao, "arquivo", true) )
	{
		$sql = "SELECT 
			coalesce(aefqtdfotoobrigatoria, 0) AS minimo
		FROM 
			pdeinterativo2013.ambienteescolafoto 
		WHERE 
			aefstatus = 'A'";
		(int)$minimo_fotos = $db->pegaUm($sql);
		$sql = "select count(*) from pdeinterativo2013.galeriafoto where pdeid = {$_REQUEST['pdeid']} and aefid = {$_REQUEST['aefid']} and gfostatus = 'A'";
		(int)$total_fotos = $db->pegaUm($sql);
		if($total_fotos >= $minimo_fotos){
			salvarAbaResposta("identificacao_galeria");
		}else{
			removerAbaResposta("identificacao_galeria");
		}
		
		
		$sql = "UPDATE PDEINTERATIVO2013.LISTAPDEINTERATIVO SET FOTO = (SELECT CASE WHEN (SELECT count(1) FROM pdeinterativo2013.galeriafoto gal WHERE gal.pdeid = {$_REQUEST['pdeid']} and gal.gfostatus = 'A') > 0
		THEN '<img src=\"../imagens/cam_foto.gif\" onclick=\"galeriaPDEInterativo({$_REQUEST['pdeid']})\" class=\"link\" >'
		ELSE '' END)
		WHERE PDEID = {$_REQUEST['pdeid']}";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	echo '<script>
			alert("Arquivo incluído com sucesso.");
			window.opener.location.href = window.opener.location.href;
			self.close();
		  </script>';
	die;
}

extract($_GET);

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<script type="text/javascript">
		$(document).ready(function()
		{
			$('#btSalvar').click(function()
			{
				if( $('#arquivo').val() == '' )
				{
					alert("O campo 'Arquivo' deve ser preenchido.");
					$('#arquivo').focus();
					return;
				}
				if( $('#gfodescricao').val() == '' )
				{
					alert("O campo 'Descrição' deve ser preenchido.");
					$('#gfodescricao').focus();
					return;
				}

				$('#formFoto').submit();
			});
		});
		</script>
	</head>
	<body>
		<form id="formFoto" method="post" enctype="multipart/form-data" action="">
		<input type="hidden" id="submetido" name="submetido" value="1" />
		<input type="hidden" id="pdeid" name="pdeid" value="<?=$pdeid?>" />
		<input type="hidden" id="aefid" name="aefid" value="<?=$aefid?>" />
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
			<tr>
				<td bgcolor="#c4c4c4" align="center" colspan="2">
					<b>Inserir Fotos</b>
				</td>
			</tr>
			<tr>
				<td bgcolor="" align="center" colspan="2">
					<img border="0" src="/imagens/obrig.gif" /> Indica campo obrigatório
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width:100px;">Arquivo</td>
				<td>
					<input type="file" id="arquivo" name="arquivo" />
					<img border="0" src="/imagens/obrig.gif" />
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Descrição</td>
				<td>
					<?=campo_textarea( 'gfodescricao', 'S', 'S', '', '55', '6', '1500', '' , 0, '')?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"></td>
				<td align="left">
					<input type="button" value="Salvar" id="btSalvar" />
					<input type="button" value="Fechar" onclick="self.close();" />
				</td>
			</tr>
		</table>
		</form>
	</body>
</html>