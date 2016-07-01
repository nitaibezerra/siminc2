<?php
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if( $_REQUEST['requisicao'] == "editar" )
{
	$sqlCarregar = "
		SELECT 
			tpsdescricao,
			tpsid  
		FROM
			par.tiposubacao
		WHERE
			tpsid = {$_REQUEST['tpsid']}	
	";
	$editTipoSubacao = $db->carregar($sqlCarregar);
	$editTipoSubacao = $editTipoSubacao[0];
	$descricao = $editTipoSubacao[tpsdescricao];
	$tpsid = $editTipoSubacao[tpsid];
	$editar = '- Editar';
}


if($_POST["submeter"] == 'salvar' ) 
{
	if( ! empty($_POST['tpsid']) )
	{
		$sql = "UPDATE par.tiposubacao SET tpsdescricao = '{$_POST['tpsdescricao']}' WHERE tpsid = ".$_POST['tpsid'];
		$db->executar($sql);
		
		echo "<script>alert('Tipo de Subação modificado com sucesso!')</script>";
		echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarTipoSubacao&acao=A'</script>";
	}
	else
	{
		$sql = "INSERT INTO par.tiposubacao
					( tpsdescricao ) 
				VALUES 
					('{$_POST['tpsdescricao']}')";
		$db->executar($sql);
		echo "<script>alert('Tipo de Subação inserido com sucesso!')</script>";
		echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarTipoSubacao&acao=A'</script>";
	}
	$db->commit(); 
	
} 
elseif($_POST["submeter"] == 'excluir') 
{
	$sql = "UPDATE par.tiposubacao SET tpsstatus = 'I' WHERE tpsid = ".$_POST['tpsid'];
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Tipo de Subação desativado com sucesso!')</script>";
	echo "<script>window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarTipoSubacao&acao=A'</script>";
}

$titulo = 'Tipo Subação' . $editar;
monta_titulo($titulo , '');
?>
<form id="formulario" method="post" action="">
<input type="hidden" id="tpsid" name="tpsid" value="<?=$tpsid; ?>" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<input type="hidden" id="submeter" name="submeter" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Descrição:</b></td>
		<td>
			
			<?php echo campo_texto('tpsdescricao', 'S', 'S', '', 100, 100, '', '','left','',0,'','',$descricao); ?>
			
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarObjeto();" />
			&nbsp;
			<input type="button" id="bt_cancel" value="Cancelar" onclick="javascript:cancelar();"/>
			<?php if ($tpsid) 
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
		<td align="center" colspan="2"><b>Lista de Tipos de Subação</b></td>
	</tr>
</table>
<script>
	function salvarObjeto()
	{
		
		var desc = $("input[name='tpsdescricao']").val();

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
	function alterarTipoSubacao( tpsid )
	{
		document.getElementById('submeter').value = 'salvar';	
		document.getElementById('formulario').submit();
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarTipoSubacao&acao=A&tpsid='+tpsid+'&requisicao=editar';
		
	}
	function excluirTipoSubacao( tpsid ){
		if(confirm('Deseja realmente inativar este tipo de subação?'))
		{
			document.getElementById('tpsid').value = tpsid;
			document.getElementById('submeter').value = 'excluir';
			document.getElementById('formulario').submit();
		}
	}
	function cancelar(){
		window.location.href = 'par.php?modulo=sistema/tabelasapoio/configuracao_guia/configurarTipoSubacao&acao=A';
	}
</script>
<?

	$acao = "'<a href=\"#\" onclick=\"alterarTipoSubacao(' || tpsid || ');\" title=\"Editar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
			||
			CASE WHEN 
				tpsstatus = 'A' then 
					'<a href=\"#\" onclick=\"excluirTipoSubacao(' || tpsid || ');\" title=\"Status\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
				ELSE
					''
			END	
					";
			

	 $sql = "SELECT '<center>'||".$acao."||'</center>' as acao,
	 			 tpsid, 
	 			 tpsdescricao, 
	 			case when tpsstatus = 'A' then 'Ativo'
	 			else 'Inativo' end as ativo
	 		FROM par.tiposubacao";
	 $cabecalho = array("ações", "Código", "Descrição", "Status");
	 $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center');
?>