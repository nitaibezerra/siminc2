<?php

switch ($_REQUEST["requisicao"]) {
	
	case "salvar":
		$emi->cadastraPapEscola( $_REQUEST );
	break;
	
	case "alterar":
		echo emiBuscaDadosPAP( $_REQUEST["papid"] );
		die;
	break;
	
	case "excluir":
		$emi->excluirPap( $_REQUEST["papid"] );
	break;
	
}

if ( $_REQUEST["tppid"] || $_SESSION["emi"]["tppid"] ){
	
	$_SESSION["emi"]["tppid"] = !empty($_REQUEST["tppid"]) ? $_REQUEST["tppid"] : $_SESSION["emi"]["tppid"];
	$tppid = $_SESSION["emi"]["tppid"];
	
}

$emeid = $tppid == EMI_TIPO_ENTIDADE_ESCOLA ? $_SESSION["emi"]["emeidEscola"] : $_SESSION["emi"]["emeidPai"];

if ( $_REQUEST["comid"] || $_SESSION["emi"]["comid"] ){
	
	$_SESSION["emi"]["comid"] = !empty($_REQUEST["comid"]) ? $_REQUEST["comid"] : $_SESSION["emi"]["comid"];
	$comid = $_SESSION["emi"]["comid"];
	
	if ( !emiVerificaComponente( $comid ) ){
		
		unset( $_SESSION["emi"]["comid"] );
		
		print "<script>
				  alert('O componente selecionado não existe ou está em branco!');
				  self.close();
			  </script>";
		die;
	}
	
}


if ( !$_SESSION["emi"]["comid"] || !$emeid ){
	print "<script>
			  alert('O Componente ou a Entidade selecionado(a) não existe ou está em branco!');
			  self.close();
		  </script>";
	die;
}

monta_titulo("Ação/ Atividade e Meta do PAP", "");

if ( $_SESSION["emi"]["comid"] ){
	$emi->montaCabecalho( $_SESSION["emi"]["comid"], "componente" );
}

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/emi/geral/js/emi.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form id="formulario" name="formulario" method="post" action="">
			<input type="hidden" value="salvar" name="requisicao"/>
			<input type="hidden" value="<?=$emeid;?>" name="emeid"/>
			<input type="hidden" value="<?=$comid;?>" name="comid"/>
			<input type="hidden" value="" name="papid" id="papid">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulodireita">Ação / Atividade</td>
					<td><?php echo campo_textarea( 'papcaoatividade', 'S', 'S', '', 80, 7, 1000, '', 0, '' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Meta</td>
					<td><?php echo campo_textarea( 'papmeta', 'S', 'S', '', 80, 7, 1000, '', 0, '' ); ?></td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td colspan="2">
						<input type="button" value="Salvar" onclick="salvaDadosPap('');" style="cursor: pointer;"/>
					</td>
				</tr>
				<tr><td colspan="2" class="subtitulocentro">Lista de Ações/Atividades e Metas</td></tr>
			</table>
		</form>
		<?php 
		
			$sql = "SELECT
						'<center>
							<img src=\"/imagens/check_p.gif\" onclick=\"alterarPAP(' || papid || ');\" style=\"cursor:pointer;\" title=\"Alterar\">
							<img src=\"/imagens/exclui_p.gif\" onclick=\"excluirPAP(' || papid || ');\" style=\"cursor:pointer;\" title=\"Excluir\">
						</center>' as acao,
						trim(papcaoatividade) as atividade,
						trim(papmeta) as meta
					FROM
						emi.empap
					WHERE
						comid = {$comid} AND
						emeid = {$emeid} AND
						papstatus = 'A'
					ORDER BY
						papid";
			
			$cabecalho = array( "Ação", "Ação/Atividade", "Meta" );
			$db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
			
		?>
	</body>
</html>