<?php 

if( $_POST['excluir'] )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array("alpstatus" => "'A'");
	$file = new FilesSimec("anexolinkprograma", $campos, "par");
	
	$arqid = $db->pegaUm("SELECT a.arqid FROM par.linkprograma l INNER JOIN par.anexolinkprograma a ON a.alpid = l.alpid WHERE l.lprid = ".$_POST['lprid']);
	
	$db->executar("UPDATE par.anexolinkprograma SET alpstatus = 'I' WHERE arqid = ".$arqid);
	$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid);
	$db->executar("DELETE FROM par.linkprogramaperfil WHERE lprid = ".$_POST['lprid']);
	$db->executar("DELETE FROM par.linkprograma WHERE lprid = ".$_POST['lprid']);
	$file->excluiArquivoFisico($arqid);
	
	$db->commit();
	echo "<script>
			alert('Dados gravados com sucesso.');
		  </script>";
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$menu = array(
0 => array("id" => 1, "descricao" => "Lista", "link" => "/par/par.php?modulo=sistema/listaLinksProgramas&acao=A"),
1 => array("id" => 2, "descricao" => "Cadastro", "link" => "/par/par.php?modulo=sistema/cadastroLinksProgramas&acao=A")
);

$abaAtiva = "/par/par.php?modulo=sistema/listaLinksProgramas&acao=A";

echo montarAbasArray($menu, $abaAtiva);

echo monta_titulo("PAR 2010", "Lista de Links de Programas");

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function()
{
	$('#btPesquisar').click(function()
	{
		$('#formPesquisar').submit();
	});
});

function alterarLink(lprid)
{
	window.location.href = "par.php?modulo=sistema/cadastroLinksProgramas&acao=A&lprid="+lprid;
}

function excluirLink(lprid)
{
	if( confirm('Deseja realmente excluir o Link do Programa?') )
	{
		document.getElementById('lprid').value = lprid;
		document.getElementById('formExcluir').submit();
	}
}

</script>

<form id="formExcluir" action="" method="post">
	<input type="hidden" value="1" name="excluir" />
	<input type="hidden" value="" id="lprid" name="lprid" />
</form>

<form id="formPesquisar" action="" method="post">
<input type="hidden" value="1" name="pesquisar" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="2" align="center">
	<tr>
		<td align="right" class="SubTituloDireita" width="350px">
			Nome
		</td>
		<td>
			<?php $lprnome = $_POST['lprnome']; ?>
			<?=campo_texto('lprnome', 'N', 'S', '', '40', '180', '', 'N', 'left', '', '', 'id="lprnome"', '', null, '' )?>
		</td>
	</tr>
	<tr>
		<td style="background-color:#c0c0c0;text-align:left;" width="350px">
			<a href="/par/par.php?modulo=sistema/cadastroLinksProgramas&acao=A">
				<img src="/imagens/gif_inclui.gif" border="0" style="vertical-align:middle;" /> Cadastrar novo Link
			</a> 
		</td>
		<td style="background-color:#c0c0c0;">
			<input type="button" value="Pesquisar" id="btPesquisar" />
		</td>
	</tr>
</table>
</form>

<br /><br />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="2" align="center">
	<tr>
		<td style="background-color:#c0c0c0;text-align:center;text-weight:bold">
			Lista dos Links Cadastrados
		</td>
	</tr>
</table>
<?php

if( $_POST['pesquisar'] )
{
	$where = array();
	
	if( $_POST['lprnome'] ){
        $lprnomeTmp = removeAcentos(str_replace("-","",$_POST['lprnome']));
        $where[] = "UPPER(public.removeacento(l.lprnome)) ILIKE '%".$lprnomeTmp."%'";
    }
	
	if( !empty($where) )
	{
		$where = " WHERE " . implode(" AND ", $where);
	}
	else
	{
		unset($where);
	}
}

$sql = "SELECT
			'<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarLink(' || l.lprid || ');\" />&nbsp;<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirLink(' || l.lprid || ');\" />' as acao,
			l.lprnome,
			l.lprlink,
			'<img src=\"../slideshow/slideshow/verimagem.php?arqid=' || ar.arqid || '\" />' as icone,
			CASE WHEN l.lprpopup = 'S' THEN 'Sim' ELSE 'Não' END as popup,
			CASE WHEN l.itrid is null THEN 'Todos' ELSE i.itrdsc END as instrumento,
			CASE WHEN l.lprdatainicio is null THEN '-' ELSE to_char(l.lprdatainicio, 'DD/MM/YYYY') END as datainicio,
			CASE WHEN l.lprdatafim is null THEN '-' ELSE to_char(l.lprdatafim, 'DD/MM/YYYY') END as datafim,
			CASE WHEN l.lprfuncao is null THEN '-' ELSE l.lprfuncao END as funcao,
			CASE WHEN l.lprstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END as status
		FROM
			par.linkprograma l
		LEFT JOIN
			par.instrumento i ON i.itrid = l.itrid
		INNER JOIN
			par.anexolinkprograma a ON a.alpid = l.alpid
		INNER JOIN
			public.arquivo ar ON ar.arqid = a.arqid
		".$where;

$cabecalho 		= array( "Ação", "Nome", "Link", "Ícone", "Usa Popup", "Instrumento", "Data Início do Período", "Data Fim do Período", "Função", "Status");
//$tamanho		= array( '10%', '90%' );															
$alinhamento	= array( 'center', 'left', 'left', 'center', 'center', 'center', 'center', 'center', 'left', 'center' );
	
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', $alinhamento);

?>