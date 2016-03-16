<?
/*
 * TELA utilizada na construção de estruturas de tabelas, utilizada para 
 * inserir, atualizar e remover itens da tabela (colunas, subcolunas, linhas, sublinhas e agrupamento)
 * Funciona no formato de popup
 */

switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('buscargrupoopcoes'=>true,
									 'atualizarsubcoluna'=>true,
									 'atualizarcoluna'=>true,
									 'inserirsubcoluna'=>true,
									 'inserircoluna'=>true,
									 'atualizarsublinha'=>true,
									 'atualizarlinha'=>true,
									 'inserirsublinha'=>true,
									 'inserirlinha'=>true,
									 'atualizaragrupamento'=>true,
									 'inseriragrupamento'=>true,
									 'removerlinha'=>true,
									 'removercoluna'=>true,
									 'removeragrupamento'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script>
function validarcoluna(frm) {
	if(frm.elements['coldsc'].value == "") {
		alert("'Coluna' é obrigatório");
		return false;
	}
	if(frm.elements['tpiid'].value == "") {
		alert("'Tipo de dados' é obrigatório");
		return false;
	}
	return true;
}
function validarlinha(frm) {
	if(frm.elements['lindsc'].value == "") {
		alert("'Linha' é obrigatório");
		return false;
	}
	if(frm.elements['gitid'].value == "") {
		alert("'Grupo do item' é obrigatório");
		return false;
	}
	return true;
}
function validaragrupamento(frm) {
	if(frm.elements['agpdsc'].value == "") {
		alert("'Agrupamento' é obrigatório");
		return false;
	}
	return true;
}
function verificatipodados(id) {
	switch(id) {
		case '<? echo TPIID_COMBO; ?>':
			ajaxatualizar('requisicao=buscargrupoopcoes&iscoluna=true','linhagrupoopcoes');
			document.getElementById('linhagrupoopcoes').style.display = '';
			break;
		default:
			document.getElementById('linhagrupoopcoes').style.display = 'none';
	}
}
</script>
</head>
<body>
<form name='formulario' method='post' onsubmit="return validar<? echo $_REQUEST['op']; ?>(this);">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?
/* 
 * Gerencia a atualização de colunas, linhas e agrupamentos. Cadas item possui caracteristicas diferente que 
 * são tratadas em seus determinados cases 
 */
switch($_REQUEST['op']) {
	case 'coluna':
		// Definindo se alterar/inserir
		if($_REQUEST['colid']) {
			$requisicao = (($_REQUEST['agpid'])?'atualizarsubcoluna':'atualizarcoluna');
			$dadoscoluna = $db->carregar("SELECT * FROM rehuf.coluna WHERE colid = '". $_REQUEST['colid'] ."'");
			if($dadoscoluna) {
				$dadoscoluna = current($dadoscoluna);
				$requisicao = (($_REQUEST['agpid'])?'atualizarsubcoluna':'atualizarcoluna');
			}
		} else {
			$requisicao = (($_REQUEST['agpid'])?'inserirsubcoluna':'inserircoluna');
		}
?>
		<input type='hidden' name='gitid' value='<? echo ($dadoscoluna['gitid'])?$dadoscoluna['gitid']:$_REQUEST['gitid']; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<tr>
			<td class="SubTituloDireita">Coluna :</td>
			<td><? $coldsc = $dadoscoluna['coldsc']; echo campo_texto('coldsc', "S", (($_REQUEST['is_porano'])?"N":"S"), "Descrição da coluna", 47, 100, "", "", '', '', 0, 'id="lindsc"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observações :</td>
			<td>
			<?
			$colobs = $dadoscoluna['colobs'];
			echo campo_textarea( 'colobs', 'S', 'S', '', '50', '4', '200');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Permitir observações :</td>
			<td><input type='radio' name='colpermiteobs' value='sim' <? echo (($dadoscoluna['colpermiteobs']=="t")?"checked":""); ?>> Sim <input type='radio' name='colpermiteobs' value='nao' <? echo (($dadoscoluna['colpermiteobs']=="f"  || !$dadoscoluna['colpermiteobs'])?"checked":""); ?>> Não</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de dados :</td>
			<td>
			<?
			$tpiid = $dadoscoluna['tpiid'];
			$sql = "SELECT tpiid AS codigo, tpidsc AS descricao FROM rehuf.tipoitem";
			$db->monta_combo('tpiid', $sql, 'S', 'Selecione', 'verificatipodados', '', '', '200', 'S', 'tpiid');
			?>
			</td>
		</tr>
		<tr id="linhagrupoopcoes" style="display:none">
		</tr>
		<script>
			verificatipodados(document.getElementById('tpiid').value);
		</script>

<?
	break;
	case 'linha':
		// Definindo se alterar/inserir
		if($_REQUEST['linid']) {
			$requisicao = (($_REQUEST['agpid'])?'atualizarsublinha':'atualizarlinha');
			$dadoslinha = $db->carregar("SELECT * FROM rehuf.linha WHERE linid = '". $_REQUEST['linid'] ."'");
			if($dadoslinha) {
				$dadoslinha = current($dadoslinha);
			}
		} else {
			$requisicao = (($_REQUEST['agpid'])?'inserirsublinha':'inserirlinha');
		}
?>
		<input type='hidden' name='gitid' value='<? echo ($dadoslinha['gitid'])?$dadoslinha['gitid']:$_REQUEST['gitid']; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<tr>
			<td class="SubTituloDireita">Linha :</td>
			<td><? $lindsc = $dadoslinha['lindsc']; echo campo_texto('lindsc', "S", "S", "Descrição da linha", 47, 100, "", "", '', '', 0, 'id="lindsc"' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Observações :</td>
			<td>
			<?
			$linobs = $dadoslinha['linobs'];
			echo campo_textarea( 'linobs', 'S', 'S', '', '50', '4', '200');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Permitir observações :</td>
			<td><input type='radio' name='linpermiteobs' value='sim' <? echo (($dadoslinha['linpermiteobs']=="t")?"checked":""); ?>> Sim <input type='radio' name='linpermiteobs' value='nao' <? echo (($dadoslinha['linpermiteobs']=="f"  || !$dadoslinha['linpermiteobs'])?"checked":""); ?>> Não</td>
		</tr>
<?
	break;
	case 'agrupamento':
		// Definindo se alterar/inserir
		if($_REQUEST['agpid']) {
			$dadosagrupamento = $db->carregar("SELECT * FROM rehuf.agrupamento WHERE agpid = '". $_REQUEST['agpid'] ."'");
			if($dadosagrupamento) {
				$dadosagrupamento = current($dadosagrupamento);
				$_REQUEST['islinha'] = (($dadosagrupamento['agplinha']=='t')?'sim':false);
			}	
		}
		if($_REQUEST['islinha']) {
			$agplinha = 'sim';
		} else {
			$agplinha = 'nao';
		}
?>

		<input type='hidden' name='agplinha' value='<? echo $agplinha; ?>'>
		<input type='hidden' name='requisicao' value='<? echo (($_REQUEST['agpid'])?'atualizaragrupamento':'inseriragrupamento'); ?>'>
<?
		if($_REQUEST['agpid']) {
			echo "<input type='hidden' name='agpid' value='". $_REQUEST['agpid'] ."'>";	
			echo "<input type='hidden' name='gitid' value='". $dadosagrupamento['gitid'] ."'>";
		}
?>
		<tr>
			<td class="SubTituloDireita">Agrupamento :</td>
			<td><?$agpdsc = $dadosagrupamento['agpdsc']; echo campo_texto('agpdsc', "S", "S", "Descrição do agrupamento", 47, 100, "", "", '', '', 0, 'id="lindsc"' ); ?></td>
		</tr>
		<? if(!$_REQUEST['islinha']) { ?>
		<tr>
			<td class="SubTituloDireita">Possui totalizador ?</td>
			<td><input type='radio' name='agppossuitotal' value='sim' <? echo (($dadosagrupamento['agppossuitotal']=='t')?'checked':''); ?> > Sim <input type='radio' name='agppossuitotal' value='nao' <? echo (($dadosagrupamento['agppossuitotal']=='f' || !$dadosagrupamento['agppossuitotal'])?'checked':''); ?>> Não</td>
		</tr>
		<? } ?>
<?
	break;
}
?>
<tr>
	<td colspan='2'>
	<input type='submit' value='Gravar'>
	<?
		if($_REQUEST['linid']){ 
			echo "<input type='button' value='Remover' onclick=\"Excluir('?modulo=principal/inserirdadostabela&acao=A&agpid=".$_REQUEST['agpid']."&requisicao=removerlinha&linid=".$_REQUEST['linid']."','Tem certeza que deseja remover esta linha?')\">";
		}elseif($_REQUEST['colid'] && !$_REQUEST['is_porano']) {
			echo "<input type='button' value='Remover' onclick=\"Excluir('?modulo=principal/inserirdadostabela&acao=A&agpid=".$_REQUEST['agpid']."&requisicao=removercoluna&colid=".$_REQUEST['colid']."','Tem certeza que deseja remover esta coluna?')\">";
		}elseif($_REQUEST['agpid'] && $_REQUEST['op'] == 'agrupamento') {
			echo "<input type='button' value='Remover' onclick=\"Excluir('?modulo=principal/inserirdadostabela&acao=A&requisicao=removeragrupamento&agpid=".$_REQUEST['agpid']."','Tem certeza que deseja remover este agrupamento?')\">";
		}
	?>
	
	</td>
</tr>
</table>
</form>
</body>
</html>