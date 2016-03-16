<?php
include APPRAIZ . 'includes/cabecalho.inc';

print "<br/>";

// Monta as Abas da Página
$menu[0] = array("descricao" => "Itens de auditoria", "link"=> "fabrica.php?modulo=sistema/geral/itens-auditoria/listar&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=sistema/geral/itens-auditoria/listar&acao=A");

//Monta o Titulo da página abaixo da Aba
monta_titulo("Consultar itens de auditoria", "<b>Filtros</b>");

$itemAuditoriaRepositorio = new ItemAuditoriaRepositorio();

$grid = new Grid(
	array(
		'Ação'=>null,
		'Item'=>'itemnome',
		'Descrição'=>'itemdsc',
		'Situação'=>'itemsituacao'
	),
	'geral/itens-auditoria/itensAuditoria.php',
	'ItensAuditoria',
	$itemAuditoriaRepositorio->recupereQtdeRegistros()
);
	
//$itensAuditoria = $itemAuditoriaRepositorio->recupereTodos($orderBy, $ordem, $limit);

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="./js/additional-methods.min.js"></script>

<form id="form-listarItensAuditoria" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Item: </td>
			<td>
				<? echo campo_texto('itemAuditoria', 'N',  'S', "", 50, 30, '',
                    '', '', '', 0, 'id="itemAuditoria"', '', "", '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Situação:</td>
			<td>
				<input id="itensAuditoriaSituacao" type="checkbox" name="situacaoItemAuditoria" value="A" checked="checked"/>Ativo
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%"></td>
			<td>
				<input id="botaoPesquisarItensAuditoria" type="submit" value="Consultar"/>
				<input type="button" value="Novo" onclick="parent.location='?modulo=sistema/geral/itens-auditoria/formulario&acao=A'"/>
			</td>
		</tr>
	</table>
</form>

<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
	<tbody>
		<tr>
			<td align="center" class="SubTituloCentro" colspan="2">Lista de itens de auditoria</td>
		</tr>
	</tbody>
</table>

<?php 
	print $grid->montaGrid();
?>

<script type="text/javascript" src="./js/itens-auditoria.js"></script>