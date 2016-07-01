<?php
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregarsubitensindicadores'=>true,
									 'atualizardadosindicadores'=>true,
									 'removersubitemindicador'=>true,
									 'inserirdadosindicadores'=>true,
									 'removerdadosindicadores'=>true,
									 'ordenarsubitemindicador'=>true
									 );
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

$titulo_modulo = "REHUF - Reestruturação dos Hospitais Universitários Federais";

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<?

switch($_REQUEST['pagina']) {
	
	case 'editar':
		if($_REQUEST['indid']) {
			$indicador = $db->pegaLinha("SELECT * FROM rehuf.indicador WHERE indid = '". $_REQUEST['indid'] ."'");
			if($indicador) {
				$inddsc = $indicador['inddsc']; 
				$indformula = trim($indicador['indformula']);
				$indanoini = $indicador['indanoini'];
				$indanofim = $indicador['indanofim'];
				$dimid = $indicador['dimid'];
				$indid = $indicador['indid'];
				$requisicao = 'atualizardadosindicadores';
				$dscacao = 'Editar indicador';
			} else {
				echo "<script>
						alert('Indicador não encontrado');
						window.location='?modulo=indicadores/gerenciarestruturaindicadores&acao=A';
					  </script>";
				exit;
			}
		} else {
			$requisicao = 'inserirdadosindicadores';
			$dscacao = 'Inserir indicador';
		}
		
		foreach($_ANOS as $a) {
			$dadosano[] = array('codigo' => $a, 'descricao' => $a); 
		}
		
		monta_titulo( $titulo_modulo, $dscacao );
?>
<script>
			function validarFormularioIndicadores(frm) {
				if(frm.elements['inddsc'].value == "") {
					alert("'Nome do indicador' é obrigatório");
					return false;
				}
				if(frm.elements['indanoini'].value == "") {
					alert("'Ano inicial' é obrigatório");
					return false;
				}
				if(frm.elements['indanofim'].value == "") {
					alert("'Ano final' é obrigatório");
					return false;
				}
				if(frm.elements['indanofim'].value < frm.elements['indanoini'].value) {
					alert("'Ano final' não pode ser menor do que 'Ano inicial'");
					return false;
				}
				if(frm.elements['dimid'].value == "") {
					alert("'Dimensão' é obrigatório");
					return false;
				}
				return true;
			}
		</script>
		<form name='formulario' action='?modulo=indicadores/gerenciarestruturaindicadores&acao=A' method='post' onsubmit='return validarFormularioIndicadores(this);'>
		<?
		if($_REQUEST['indid']) {
			echo "<input type='hidden' name='indid' value='". $_REQUEST['indid'] ."'>";
		}
		?>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita">Nome do indicador :</td>
				<td><? echo campo_texto('inddsc', "S", "S", "Nome do indicador", 67, 150, "", "", '', '', 0, '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descrição geral da formula :</td>
				<td><? echo campo_textarea('indformula', 'S', 'S', '', '65', '5', '500'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Ano inicial :</td>
				<td><?
				$sql = "SELECT ano AS codigo, ano AS descricao FROM public.anos";
				$db->monta_combo('indanoini', $dadosano, (($desabilitarmodificarformato)?'N':'S'), 'Selecione', 'detalheslinha', '', '', '', 'S', 'tpgidlinha');
				?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Ano final :</td>
				<td><?
				$sql = "SELECT ano AS codigo, ano AS descricao FROM public.anos";
				$db->monta_combo('indanofim', $dadosano, (($desabilitarmodificarformato)?'N':'S'), 'Selecione', 'detalheslinha', '', '', '', 'S', 'tpgidlinha');
				?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Dimensão :</td>
				<td><?
				$sql = "SELECT dimid AS codigo, dimdsc AS descricao FROM rehuf.dimensao";
				$db->monta_combo('dimid', $sql, (($desabilitarmodificarformato)?'N':'S'), 'Selecione', 'detalheslinha', '', '', '', 'S', 'tpgidlinha');
				?></td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td colspan='2' align='center'>
					<table cellSpacing="0" cellPadding="0" align="center" width="100%">
					<tr>
					<td align="center">
					<input type='submit' value="Gravar" onclick="document.formulario.submit();"> 
					<input type='button' value="Voltar" onclick="window.location='?modulo=indicadores/gerenciarestruturaindicadores&acao=A';">
					</td>
					</tr>
					</table>
				</td>
			</tr>
			<? if($_REQUEST['indid']) { ?>
			<tr>
				<td colspan="2" id="listasubitens"></td>
			</tr>
			<script>
			ajaxatualizar('requisicao=carregarsubitensindicadores&indid=<? echo $_REQUEST['indid']; ?>','listasubitens');
			</script>
			<tr bgcolor="#C0C0C0">
				<td colspan='2' align='center'>
					<table cellSpacing="0" cellPadding="0" align="center" width="100%">
					<tr>
					<td align="center">
					<input type='button' value="Inserir subitem" onclick="window.open('?modulo=indicadores/gerenciarsubitem&acao=A&indid=<? echo $_REQUEST['indid']; ?>','Gerenciar','scrollbars=no,height=600,width=500,status=no,toolbar=no,menubar=no,location=no');">
					</td>
					</tr>
					</table>
				</td>
			</tr>
			<? } ?>
		</table>
		</form>
<?		
	break;
	default:
		$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&pagina=editar&indid='|| ind.indid||'\'\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=indicadores/gerenciarestruturaindicadores&acao=A&requisicao=removerdadosindicadores&indid='||ind.indid||'\',\'Deseja realmente excluir o indicador?\');\">' as opcoes, 
					   ind.inddsc, 
					   dim.dimdsc 
				FROM rehuf.indicador ind 
				LEFT JOIN rehuf.dimensao dim ON dim.dimid = ind.dimid ORDER BY dim.dimid";
		
		$cabecalho = array("Ações", "Indicador", "Dimensão");
		$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2);
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">
				<tr bgcolor=\"#C0C0C0\">
				<td colspan='2' align='center'>
					<table cellSpacing=\"0\" cellPadding=\"0\" align=\"center\" width=\"100%\">
					<tr>
					<td align=\"right\">
					<input type='button' value=\"Inserir indicador\" onclick=\"window.location='?modulo=indicadores/gerenciarestruturaindicadores&acao=A&pagina=editar';\">
					</td>
					</tr>
					</table>
				</td>
				</tr>
			</table>";
		
?>

<?
}
?>