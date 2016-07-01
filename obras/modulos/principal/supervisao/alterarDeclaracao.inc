<?php 

if( $_REQUEST['submetido'] )
{
	$sql = "UPDATE
				obras.declaracao
			SET
				dclordembanc = ".$_REQUEST['dclordembanc']."
			WHERE
				dclid = ".$_REQUEST['dclid'];
	$db->executar($sql);
	
	if( $db->commit() )
	{
		echo "<script>
				alert('Dados gravados com sucesso.');
				window.opener.location.href = window.opener.location.href; 
				self.close();
			</script>";
	}
	else
	{
		echo "<script>
				alert('Ocorreu um erro ao gravar os dados.');
				self.close();
			</script>";
	}
	
	die;
}

$sql = "SELECT
			gpd.gpdid,
			dcl.dclid,
			dcl.orsid,
			to_char(dcl.dcldtemissao, 'DD/MM/YYYY') as dcldtemissao,
			ent.entnome,
			gpd.estuf,
			dcl.dclvalor,
			usu.usunome,
			dcl.dclordembanc
		FROM
			obras.declaracao dcl
		INNER JOIN
			obras.grupodistribuicao gpd ON gpd.gpdid = dcl.gpdid
		INNER JOIN
			obras.empresacontratada epc ON epc.epcid = gpd.epcid
		INNER JOIN
			entidade.entidade ent ON ent.entid = epc.entid
		INNER JOIN
			seguranca.usuario usu ON usu.usucpf = dcl.usucpf
		WHERE
			dcl.dclid = ".$_GET['dclid'];
$dados = $db->carregar($sql);

extract($dados[0]);
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
	<script type="text/javascript">

	function salvar()
	{
		if( $('[name=dclordembanc]').val() == '' )
		{
			alert('O campo "Ordem Bancária N°" deve ser preenchido.');
			$('[name=dclordembanc]').focus();
			return;
		}
		
		$('#formAlterarDeclaracao').submit();
	}
	
	</script>
</head>
<body>
	<form id="formAlterarDeclaracao" method="post">
		<input type="hidden" name="submetido" value="1" />
		<input type="hidden" name="dclid" value="<?=$_GET['dclid']?>" />
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#C0C0C0">
				<td colspan="2" style="font-size:14px;font-weight:bold;text-align:center;">Alterar Declaração</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">N° Grupo:</td>
				<td>
				<?=$gpdid?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">N° Declaracao Vigente:</td>
				<td>
				<?=$dclid?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Nº da OS:</td>
				<td>
				<?=$orsid?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Data Emissão:</td>
				<td>
				<?=$dcldtemissao?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Empresa:</td>
				<td>
				<?=$entnome?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">UF:</td>
				<td>
				<?=$estuf?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Valor:</td>
				<td>
				<?=number_format($dclvalor,2,',','.')?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Criado Por:</td>
				<td>
				<?=$usunome?>
			    </td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Ordem Bancária N°:</td>
				<td>
				<?=campo_texto('dclordembanc','S','S','',15,15,'','');?>
			    </td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td colspan="2" align="center">
					<input type="button" value="Salvar" style="cursor: pointer" onclick="salvar();" />
					<input type="button" value="Fechar" style="cursor: pointer" onclick="self.close();" />
				</td>
			</tr>
		</table>
	</form>
</body>
</html>