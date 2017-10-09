<?php
if($_REQUEST["requisicao"]){
	$emi->$_REQUEST["requisicao"]( $_REQUEST );
}

if ( $_REQUEST["tppid"] || $_SESSION["emi"]["tppid"] ){
	
	$_SESSION["emi"]["tppid"] = !empty($_REQUEST["tppid"]) ? $_REQUEST["tppid"] : $_SESSION["emi"]["tppid"];
	$tppid = $_SESSION["emi"]["tppid"];
	
}

$emeid = $tppid == EMI_TIPO_ENTIDADE_ESCOLA ? $_SESSION["emi"]["emeidEscola"] : $_SESSION["emi"]["emeidPai"];

monta_titulo("Ação / Atividade e Meta do GAP", "");

if ( $_REQUEST["mcpid"] ){
	$emi->montaCabecalho( $_REQUEST["mcpid"], "gap" );
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
			<input type="hidden" value="cadastraGapEscola" name="requisicao"/>
			<input type="hidden" value="<?=$_REQUEST['mcpid'];?>" name="mcpid"/>
			<input type="hidden" value="<?=$_REQUEST['emeid'];?>" name="emeid"/>
			<input type="hidden" value="<?=$_REQUEST['papid'];?>" name="papid" id="papid">
			<input type="hidden" value="<?=$tppid;?>" name="tppid" >
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
						<input type="button" value="Salvar" onclick="salvaDadosGap();" style="cursor: pointer;"/>
					</td>
				</tr>
				<tr><td colspan="2" class="subtitulocentro">Lista de Ações/Atividades e Metas</td></tr>
			</table>
		</form>
		<?php 
		
			$sql = "SELECT
						'<center>
							<span style=\'white-space:nowrap;\' >
							<img src=\"/imagens/check_p.gif\" onclick=\"alterarGAP(' || papid || ');\" style=\"cursor:pointer;\" title=\"Alterar\">
							<img src=\"/imagens/exclui_p.gif\" onclick=\"excluirGAP(' || papid || ');\" style=\"cursor:pointer;\" title=\"Excluir\">
							</span>
						</center>' as acao,
						trim(papcaoatividade) as atividade,
						trim(papmeta) as meta
					FROM
						emi.emgap
					WHERE
						mcpid = {$_REQUEST['mcpid']} AND
						emeid = {$_REQUEST['emeid']} AND
						papstatus = 'A'
					ORDER BY
						papid";
			
			$cabecalho = array( "Ação", "Ação/Atividade", "Meta" );
			$db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );
			
		?>
	</body>
</html>