<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'emenda/classes/MinutaConvenio.class.inc';

verificaPermissao();

$ptrid = $_REQUEST['ptrid'];

/*$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

if ($_REQUEST[ptrid]) $_SESSION["emenda"]["ptridAnalise"] = $_REQUEST[ptrid];

validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

$ptridAnalise = $_SESSION["emenda"]["ptridAnalise"];*/

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Confirmar Solicitação de Reformulação', '');

echo cabecalhoPlanoTrabalho($ptrid);

echo '<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr bgcolor="#D0D0D0">
				<td>
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarPTA( '. $ptrid .' );">
					<a style="cursor:pointer;" onclick="visualizarPTA( '. $ptrid .' );"> Visualizar PTA </a>
				</td>
				<td align="right">
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $ptrid .' );">
					<a style="cursor:pointer;" onclick="visualizarHistoricoPTA( '. $ptrid .' );"> Visualizar Histórico PTA </a>
				</td>
			</tr>
		  </table>';
monta_titulo( '', '<b>Solicitar Reformulação</b>');
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<form id="formReformulacao" name="formReformulacao" method="post" action="emenda.php?modulo=principal/reformulacao&acao=A">
<table width='100%'>
<td valign='top'>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
    <tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Confirmar Solicitação de Reformulação:</b>				
        </td>
		<td>
		<input type='button' id='s' value='Sim' style='width:120px;' onclick="wf_alterarEstado( document.getElementById('aedid').value, document.getElementById('docid').value, 121, 'solicitar a reformulação deste projeto')">
		<input type='button' id='n' value='Não' style='width:120px;' onclick='history.back(-1)'>	
        </td>
    </tr>
	</table>
	
</td>
<td width='100'>
<?
	$sql = "SELECT 
			  refid
			FROM 
			  emenda.ptminreformulacao
			WHERE
				ptrid = $ptrid
				and refstatus = 'A'
				and refsituacao = 'A'";
	
	$refidWF = $db->pegaUm( $sql );
	
	$docid = criarDocumento( $ptrid );
	wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'refid' => $refidWF ) );	
	
	$sql = "SELECT 
			  esdid
			FROM 
			  workflow.documento
			WHERE
				docid = $docid";
	$esdid = $db->pegaUm( $sql );

	if ($esdid == 123) // EM_REFORMULACAO_PROCESSO
	{
			echo "<script>
					window.location = emenda.php?modulo=principal/alteraDefinirRecursoPTA&acao=A&ptrid=$ptrid&emeid=0';
					return false;
				</script>";
	}
	elseif ($esdid == 121 || $refidWF ) // EM_SOLICITACAO_REFORMULACAO
	{
			echo "<script>
					window.location = 'emenda.php?modulo=principal/reformulacao&acao=C&ptrid=$ptrid';
					return false;
				</script>";
			exit();
	}
	else
	{
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = 121";
		$aedid = $db->pegaUm( $sql );	
	
		if ($aedid == "")
		{
			echo "<script>\n";
			echo "document.getElementById('s').disabled = true;alert('Neste estágio não é possível solicitar a reformulação.');\n";
			echo "</script>\n";
		}
			
		echo "<input type='hidden' id='docid' value='$docid'>\n";
		echo "<input type='hidden' id='esdid' value='$esdid'>\n";
		echo "<input type='hidden' id='aedid' value='$aedid'>\n";
	}
	
?>
</td>
</table>
    
</form>