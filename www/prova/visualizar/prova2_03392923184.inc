<?php
include  APPRAIZ."includes/cabecalho.inc";

if (isset($_GET['message']) && !empty($_GET['message'])) {
	echo "<div style='color:blue; font-weight: bold; font-size: 20pt; width:100%; text-align:center;'>Operação realizada com sucesso</div>";
}
echo'<br>';
monta_titulo( "Prova ". SIGLA_SISTEMA, 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

/*
 * DICAS:
 * 
 * Com a função "executar" você executa uma query, por exemplo para inseri-la no banco. Ex:
 * 
 * 		$sql = "INSERT INTO data (dia, mes, ano) VALUES ('24','03','1984')";
 * 		$db->executar($sql); 
 * 		$db->commit(); 
 * 
 */


if(!empty($_POST)) {
	$turma = $_POST['turnome'];
	$dtabr = $_POST['turdtabr'];
	$dtfec = $_POST['turdtfec'];
	$qtdalu = $_POST['qtdalu'];
	$proid = $_POST['proid'];
	
	
	//validar datas antes
	
	//converter $dtabr e $dtfec para timestamp
	$dtabr = date('Y-m-d H:i:s', strtotime($dtabr));
	$dtfec = date('Y-m-d H:i:s', strtotime($dtfec));
	
	//INSERIR
	if (empty($_POST['id'])) {
		$sql_insert = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec, qtdalu, proid)" 
					. " VALUES ('{$turma}', '{$dtabr}', '{$dtfec}', '{$qtdalu}', '{$proid}')";
		$db->executar($sql_insert);
		$db->commit();
		header('Location:http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&message=Operacao realizada com sucesso');
	//ALTERAR
	} else {
		$id = $_POST['id'];
		$sql_alterar = "UPDATE prova.turma" 
					. " SET turnome = '{$turma}', turdtabr = '{$dtabr}', turdtfec = '{$dtfec}', qtdalu = '{$qtdalu}', proid = '{$proid}'"
					. " WHERE turid = {$id}";
		$db->executar($sql_alterar);
		$db->commit();
		header('Location:http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&message=Operacao realizada com sucesso');
	}
	
}

if (isset($_GET['editar'])) {
	$popular = TRUE;
	$sql_editar = "SELECT turid, turnome, turdtabr, turdtfec, qtdalu, proid"
		. " FROM prova.turma"
		. " WHERE turid = " . $_GET['id'];
	$data_editar = $db->carregar($sql_editar);	
}


if (isset($_GET['excluir'])) {
	$sql_excluir = "DELETE FROM prova.turma WHERE turid = {$_GET['id']}";
	$db->executar($sql_excluir);
	$db->commit();
	header('Location:http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A');
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">		
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}	
</script>

<script language="Javascript">

	function confirmarExclusao(id) {
		
     	var resposta = confirm("Tem certeza que deseja excluir a turma?"); 
     	if (resposta == true) {
     		//alert('id = '+id);
			var url = "http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&excluir=true&id="+id;	
			window.location = url;
     	}
     	return false;
	}
	
	function valida(form) {	
		form = document.formulario;	
		for(i=0;i<form.length;i++) {
			if(form[i].value == "" && form[i].name != 'id') {
				alert("Todos os campos são obrigatórios");
				form[i].focus();
				return false;
			}
		}
		form.submit();
		return true;
	}
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<input type="hidden" name="id" value="<?php echo ($popular === TRUE) ? $data_editar[0]['turid'] : NULL;?>">
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td>								
				<input type="text" name="turnome" id="turdsc" size="50" value="<?php echo ($popular === TRUE) ? $data_editar[0]['turnome'] : NULL;?>">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td>
				<input type="text" name="turdtabr" id="turabr" size="50" value="<?php  echo ($popular === TRUE) ? date('d/m/Y',strtotime($data_editar[0]['turdtabr'])) : NULL;?>">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td>
				<input type="text" name="turdtfec" id="turfec" size="50" value="<?php  echo ($popular === TRUE) ? date('d/m/Y',strtotime($data_editar[0]['turdtfec'])) : NULL;?>">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Qtd Alunos:</td>
			<td>
				<input type="text" name="qtdalu" id="qtdalu" size="50" value="<?php  echo ($popular === TRUE) ? $data_editar[0]['qtdalu'] : NULL;?>">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Professor:</td>
			<td>
				<select name="proid">
					<option value="">--Selecione--</option>
					<?php
						$sqlCombo = "SELECT proid, pronome FROM prova.professor";
						$data = $db->carregar($sqlCombo);
						foreach ($data as $row):
							if(isset($data_editar)) {
								$selected = ($row['proid'] == $data_editar[0]['proid']) ? 'selected="selected"' : NULL; 
							}
					?>					
					<option value="<?php echo $row['proid'];?>" <?php echo $selected;?>><?php echo $row['pronome'];?></option>
					<?php endforeach; ?>
				</select>
			</td>
		</tr>
	</table>
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = "SELECT
			'<a href=\"?modulo=principal/prova2&acao=A&editar=true&id=' || turid || '\"><img title=\"Alterar\" src=\"../imagens/alterar.gif\" /></a>&nbsp;<a onclick=\"confirmarExclusao('|| turid ||')\"><img title=\"Excluir\" src=\"../imagens/excluir.gif\" /></a>' as acao,
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";
/*
$sql = "SELECT 
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" />&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";
*/
$cabecalho = array("Ações","Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');
?>

<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input value="Questão anterior" type="button" onclick="navega(1)" id="anterior">
				</div>
				<div style="float:left; width:30%; text-align: center">
					<input value="Salvar" type="button" id="salvar" onclick="valida();" >
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>