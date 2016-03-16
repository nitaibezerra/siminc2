<?php

if( $_REQUEST['tpoid'] )
{
	$tpoid = $_REQUEST['tpoid'];
	
	$db->executar("DELETE FROM obras.tipologiamodalidade WHERE tpoid = ".$tpoid);
	$db->executar("DELETE FROM obras.tipologiatipoobra WHERE tpoid = ".$tpoid);
	$db->executar("DELETE FROM obras.tipologiaorigem WHERE tpoid = ".$tpoid);
	$db->executar("DELETE FROM obras.tipologiaprograma WHERE tpoid = ".$tpoid);
	
	$sql = "DELETE FROM obras.tipologiaobra WHERE tpoid = ".$tpoid;
	$db->executar($sql);
	$db->commit();
	
	echo '<script>
			alert("Dados gravados com sucesso.");
		  </script>';
}

header('Content-type: text/html; charset="iso-8859-1"',true);
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Lista de Tipologia da Obra' );

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	
});

function alterarTipologia(tpoid)
{
	window.location.href = 'obras.php?modulo=sistema/geral/cadastroTipologiaObra&acao=A&tpoid='+tpoid;
}

function excluirTipologia(tpoid)
{
	if( confirm("Deseja realmente excluir esta Tipologia?") )
	{
		$('#tpoid').val(tpoid);
		$('#formLista').submit();
	}
}

</script>

<form id="formLista" method="post" action="">
	<input type="hidden" value="" name="tpoid" id="tpoid" />
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<span style="margin-left:30px;"><a href="obras.php?modulo=sistema/geral/cadastroTipologiaObra&acao=A"><img border="0" style="vertical-align:middle;" src="/imagens/gif_inclui.gif" /> Cadastrar nova Tipologia da Obra</a></span>
			<br /><br />
			<?php 
			$sql = "SELECT
						'<center>
						 <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"alterarTipologia(' || tpoid || ')\" />
						 <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"excluirTipologia(' || tpoid || ')\" />
						 </center>' as acao,
						tpodsc
					FROM
						obras.tipologiaobra
					WHERE
						tpostatus = 'A'";
			
			$celWidth  = array('10%', '90%');
			$cabecalho = array("Ação", "Descrição");
			$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', $celWidth);
			?>
			<br />
		</td>
	</tr>
</table>