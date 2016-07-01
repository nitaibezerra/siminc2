<?php 

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Programas do MEC', 'Escolha um filtro');

if($_POST){
	extract($_POST);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script>

	$(function(){
		
		$('#btnPesquisar').click(function(){
			$('#formulario').submit();
		});
		
		$('#btnLimpar').click(function(){
			document.location.href = 'par.php?modulo=sistema/tabelasapoio/feirao_programas/listaProgramas&acao=A';
		});
		
	});
	
	function editarPrograma(prgid)
	{		
		document.location.href = 'par.php?modulo=sistema/tabelasapoio/feirao_programas/formPrograma&acao=A&prgid='+prgid;
	}
		
</script>
<form name="formulario" id="formulario" method="post" action="">
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Nome do Programa</td>
			<td>
				<?php echo campo_texto('prgdsc', 'N', 'S', '', 30, 255, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar"/>
				<input type="button" value="Limpar" id="btnLimpar"/>
			</td>
		</tr>
	</table>
</form>

<?php



if($_REQUEST['prgdsc']){
    $prgdscTmp = removeAcentos(str_replace("-"," ", $_REQUEST['prgdsc']));
	$arWhere[] = "UPPER(public.removeacento(prgdsc)) ilike '%{$prgdscTmp}%'";
}
$sql = "select
			'
			<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" id=\"' || prg.prgid || '\" onclick=\"editarPrograma(' || prg.prgid || ')\" />
			' as acao,			
			prgdsc,
			to_char(pfadatainicial, 'DD/MM/YYYY') as pfadatainicial,
			to_char(pfadatafinal, 'DD/MM/YYYY') as pfadatafinal,
			pfaano,
			case when pfaesfera = 'E' then 'Estadual'
				 when pfaesfera = 'M' then 'Municipal'
			else 'Todos' end as esfera
		from
			par.programa prg
		left join 
			par.pfadesao pfa on pfa.prgid = prg.prgid
		".( is_array($arWhere) ? ' where '.implode(' and ', $arWhere) : '' )."
		order by
			prgdsc";

$cabecalho = array('Ação','Descrição', 'Data Inicial', 'Data Final', 'Ano de Referência', 'Esfera');
$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', '', '');

?>