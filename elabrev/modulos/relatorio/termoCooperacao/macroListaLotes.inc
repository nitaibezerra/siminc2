<?php 

require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo('Lista de Macros' , 'Lotes Gerados');
?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('.novoLote').click(function(){
			document.location = "elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A&aba=macroListaItensLote";
		});

		$('.visualizar').click(function(){
			document.location = 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A&aba=macroListaItensLote&lotid='+this.id;
		});

		$('.gerarExcel').click(function(){
			document.location = 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A&aba=macroGeraExcel&lotid='+this.id;
		});

		$('.pesquisar').click(function(){
			$('#formulario').submit();
		});

		$('.limpar').click(function(){
			document.location = 'elabrev.php?modulo=relatorio/termoCooperacao/macroTC&acao=A';
		});
	});
</script>
<?php if($_POST) extract($_POST); ?>
<form id="formulario" name="formulario" method="post" action="">
	<table border="0"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		<tr>
			<td class="subtituloDireita">Nº do Lote</td>
			<td>
				<?php
				$sql = "select lotid as codigo, lotid as descricao from elabrev.lotemacro order by lotid"; 
				$db->monta_combo('lotid', $sql, 'S', 'Selecione...', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nº do Termo</td>
			<td><?php echo campo_texto('lotdsc', 'N', 'S', '', 35, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data</td>
			<td>
			<?php 
				$lotdata = $lotdata ? formata_data($lotdata) : ''; 
				echo campo_data2('lotdata', 'N', 'S', '', '', '', '', ''); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">CPF</td>
			<td><?php echo campo_texto('usucpf', 'N', 'S', '', 20, '', '###.###.###-##', '')?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome</td>
			<td><?php echo campo_texto('usunome', 'N', 'S', '', 40, '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" class="pesquisar" />
				<input type="button" value="Limpar" class="limpar" />
				<input type="button" value="Novo Lote" class="novoLote" />			
			</td>
		</tr>
	</table>
</form>
<?php

if($_REQUEST['lotid']){
	$where[] = "lot.lotid = {$_REQUEST['lotid']}";
}

if($_REQUEST['lotdsc']){
	$where[] = "lot.lotdsc ilike '%{$_REQUEST['lotdsc']}%'";
}

if($_REQUEST['usucpf']){
	$_REQUEST['usucpf'] = str_replace(array('.', '-'), '', $_REQUEST['usucpf']);
	$where[] = "usu.usucpf = '{$_REQUEST['usucpf']}'";
}

if($_REQUEST['usunome']){
	$where[] = "usu.usunome ilike '%{$_REQUEST['usunome']}%'";
}

if($_REQUEST['lotdata']){
	$_REQUEST['lotdata'] = formata_data_sql($_REQUEST['lotdata']);
	$where[] = "lot.lotdata = '{$_REQUEST['lotdata']}'";
}

$sql = "select 
			'
			<img src=\"../imagens/consultar.gif\" border=\"0\" id=\"' || lot.lotid || '\" class=\"visualizar\" />
			<img src=\"../imagens/excel.gif\" border=\"0\" id=\"' || lot.lotid || '\" class=\"gerarExcel\" />
			' as acao,			
			lot.lotdsc,  
  			to_char(lot.lotdata, 'DD/MM/YYYY') as data,  			
  			lot.lotcpfresponsavel,  			
  			usu.usunome,
  			lot.lotid
		from elabrev.lotemacro lot
		left join seguranca.usuario usu on usu.usucpf = lot.lotcpfresponsavel
		".(is_array($where) ? ' WHERE '.implode(' AND ', $where) : '')."
		order by lotid desc";

$arCabecalho = array('Ação', 'Nº dos Termos', 'Data', 'CPF', 'Responsável', 'Nº do Lote');
$db->monta_lista($sql, $arCabecalho, 50, 10, '', '', '', '', '');

?>

