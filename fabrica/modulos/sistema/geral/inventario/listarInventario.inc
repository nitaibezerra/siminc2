<?php
header( 'content-type: text/html; charset=iso-8859-1;' );
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

// Monta as Abas da Página
$menu[0] = array( "descricao" => "Inventário", "link" => "fabrica.php?modulo=sistema/geral/inventario/listar&acao=A" );
echo montarAbasArray( $menu, "fabrica.php?modulo=sistema/geral/inventario/listar&acao=A" );

//Monta o Titulo da página abaixo da Aba
monta_titulo( "INVENTÁRIO", "<b>Filtro</b>" );

$sql = "SELECT 
CASE WHEN dt_encerramento is null THEN '<a href=\"?modulo=sistema/geral/invenario/alterarInventario&acao=A\"><img style=\"border: none;\" src=\"/imagens/editar_nome.gif\" alt=\"Editar\" title=\"Editar\"/></a><img style=\"border: none;\" src=\"/imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\"/>' 
     WHEN usucpf is null THEN '<a href=\"?modulo=sistema/geral/invenario/alterarInventario&acao=A\"><img style=\"border: none;\" src=\"/imagens/editar_nome.gif\" alt=\"Editar\" title=\"Editar\"/></a><img style=\"border: none;\" src=\"/imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\"/>'
ELSE '<img style=\"border: none;\" src=\"/imagens/consultar.gif\" alt=\"Consultar\" title=\"Consultar\"/>'
END AS acoes,
sist.siddescricao, COALESCE (SUM (qtd_pf),0) AS Qtd_PF,
CASE WHEN dt_encerramento is null THEN 'Em Desenvolvimento' 
     WHEN usucpf is null THEN 'Em Desenvolvimento'
ELSE 'Finalizada'
END AS situacao
FROM inventario.tb_simec_inventario AS inv
INNER JOIN demandas.sistemadetalhe AS sist 
ON inv.sidid = sist.sidid
LEFT JOIN
inventario.tb_simec_funcionalidade as func
ON inv.co_inventario = func.co_inventario
GROUP BY sist.siddescricao, dt_encerramento, usucpf";

$cpf = $_SESSION['usucpf'];

$orderBy = $_POST['campo'] == null ? 'scsid' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

if ($_GET){
	if($_GET['redirecionamento'] && isset($_SESSION['form-listaInventario'])){
		extract($_SESSION['form-listaInventario']);
		$_POST = $_SESSION['form-listaInventario'];
	} else {
		unset($_SESSION['form-listaInventario']);
	}
}

?>

<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>

<form id="form-listaInventario" method="post" action="?modulo=sistema/geral/inventario/listar&acao=A">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Sistema:</td>
			<td>
				<select id="gc-sistemas" class="campoEstilo" name="sistemas" style="width: auto;">
					<option id="gc-fiscais-sistemas" value="">Selecione</option>
					<?php foreach ( $sistemasAtivos as $sa ) { ?>
						<option value="<?php echo $sa->getId(); ?>" 
							<?php echo $sa->getId() == $sistemas ? "selected=selected" : "" ;?>><?php echo $sa->getDescricao(); ?></option>
					<?php } ?>
				</select>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" align="center"><input id="submit-listaAuditoriaGc" type="submit" value="Consultar" /></td>
		</tr>
	</table>
</form>
<?php
	$cabecalho		= array('Ações','Sistema','QTD PF','Situação');
	$tamanho		= array("10%","50%","20%","20%");
	$alinhamento		= array("center","left","left","center");
	$db->monta_lista($sql,$cabecalho,20,5,'N','center',$par2,"",$tamanho,$alinhamento);
?>
<script type="text/javascript" src="./js/gerencia-configuracao.js"></script>

<table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" class="alignRight">
			<input id="submit-listaAuditoriaGc" type="submit" value="Inserir Novo" />
		</td>
	</tr>
</table>
