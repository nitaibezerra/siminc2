<?php
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if( $_REQUEST['requisicao'] == "editar" )
{
	$sqlCarregar = "
		SELECT 
			frmdsc,
			frmid  
		FROM
			par.formaexecucao
		WHERE
			frmid = {$_REQUEST['frmid']}	
	";
	$editFormaExecucao = $db->carregar($sqlCarregar);
	$editFormaExecucao = $editFormaExecucao[0];
	$descricao = $editFormaExecucao[frmdsc];
	$frmid = $editFormaExecucao[frmid];
	$editar = '- Editar';
}


if($_POST["submeter"] == 'salvar' ) 
{
	if( ! empty($_POST['frmid']) )
	{
		$sql = "UPDATE par.formaexecucao SET frmdsc = '{$_POST['frmdsc']}' WHERE frmid = ".$_POST['frmid'];
		$db->executar($sql);
		
		echo "<script>alert('Forma de Execução modificada com sucesso!')</script>";
		echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarFormaExecucao&acao=A'</script>";
	}
	else
	{
		$sql = "INSERT INTO par.formaexecucao
					( frmdsc ) 
				VALUES 
					('{$_POST['frmdsc']}')";
		$db->executar($sql);
		echo "<script>alert('Forma de Execução inserida com sucesso!')</script>";
		echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarFormaExecucao&acao=A'</script>";
	}
	$db->commit(); 
	
} 
elseif($_POST["submeter"] == 'excluir') 
{
	$sql = "UPDATE par.formaexecucao SET frmstatus = 'I' WHERE frmid = ".$_POST['frmid'];
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Forma de Execução destivada com sucesso!')</script>";
	echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarFormaExecucao&acao=A'</script>";
}

$titulo = 'Forma de Execução' . $editar;
monta_titulo($titulo , '');
?>
<form id="formulario" method="post" action="">
<input type="hidden" id="frmid" name="frmid" value="<?=$frmid; ?>" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<input type="hidden" id="submeter" name="submeter" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
		<td>
			
			<?php echo campo_texto('frmdsc', 'S', 'S', '', 100, 100, '', '','left','',0,'','',$descricao); ?>
			
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarObjeto();" />
			&nbsp;
			<input type="button" id="bt_cancel" value="Cancelar" onclick="javascript:cancelar();"/>
			<?php if ($frmid) 
			{?>
			&nbsp;
			<input type="button" id="bt_cancel" value="Novo" onclick="javascript:cancelar();"/>
			<?php }?>
		</td>
	</tr>
</table>
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td align="center" colspan="2"><b>Lista Formas de Execução</b></td>
	</tr>
</table>
<script>
	function salvarObjeto()
	{
		
		var desc = $("input[name='frmdsc']").val();

		if( desc == '' )
		{
			alert('O campo Descrição é obrigatório.');
			return false;
		}
		else
		{
			document.getElementById('submeter').value = 'salvar';	
			document.getElementById('formulario').submit();
		}
	}
	function alterarFormaExecucao( frmid )
	{
		document.getElementById('submeter').value = 'salvar';	
		document.getElementById('formulario').submit();
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarFormaExecucao&acao=A&frmid='+frmid+'&requisicao=editar';
		
	}
	function excluirTipoSubacao( frmid ){
		if(confirm('Deseja realmente inativar esta forma de Execução?'))
		{
			document.getElementById('frmid').value = frmid;
			document.getElementById('submeter').value = 'excluir';
			document.getElementById('formulario').submit();
		}
	}
	function cancelar()
	{
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarFormaExecucao&acao=A';
	}
</script>
<?

	$acao = "'<a href=\"#\" onclick=\"alterarFormaExecucao(' || frmid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
			||
			CASE WHEN 
				frmstatus = 'A' then 
					'<a href=\"#\" onclick=\"excluirTipoSubacao(' || frmid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
				ELSE
					''
			END	
					";
	
	 $sql = "SELECT '<center>'||".$acao."||'</center>' as acao,
	 			 frmid, 
	 			 frmdsc, 
	 			case when frmstatus = 'A' then 'Ativo'
	 			else 'Inativo' end as ativo
	 		FROM par.formaexecucao";
	 $cabecalho = array("ações", "Código", "Descrição", "Status");
	 $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center');
?>