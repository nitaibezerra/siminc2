<?php
include  APPRAIZ."includes/cabecalho.inc";
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
	class date{

		/**
		 * Converte data.
		 */
		public function dateConvert(&$date)
		{
			$date = explode('/', $date);
			$date = "{$date[2]}-{$date[1]}-{$date[0]}";
		}
	}
	
	$valueForm = array('turid' => '' ,'turnome' => '','turdtabr' => '','turdtfec' => '');

	if($_POST){
		if(
			isset($_POST['turnome'])  && !is_null($_POST['turnome']) &&
			isset($_POST['turdtabr']) && !is_null($_POST['turdtabr']) &&
			isset($_POST['turdtfec']) && !is_null($_POST['turdtfec'])
		){

			# Convertendo datas para formato do banco
			$clsDate = new date();
			$clsDate->dateConvert($_POST['turdtabr']);
			$clsDate->dateConvert($_POST['turdtfec']);

			# Se tiver id edita se nao insere
			if($_POST['turid'])
				$sql = "UPDATE prova.turma 
						SET turnome = '{$_POST['turnome']}', turdtabr = '{$_POST['turdtabr']}', turdtfec = '{$_POST['turdtfec']}'
						WHERE turid = {$_POST['turid']}";
			else 
				$sql = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec) 
						VALUES ('{$_POST['turnome']}','{$_POST['turdtabr']}','{$_POST['turdtfec']}')";

			$db->executar($sql); 
	 		$db->commit(); 
			
			echo '<script type="text/javascript">alert("Operação realizada com sucesso!");</script>';
		} else{
			echo '<script type="text/javascript">alert("Não foi possivel realizar peração!");</script>';;
		}
	} elseif(isset($_GET['editar']) && !is_null($_GET['editar'])){
		$sql = "SELECT *
				, TO_CHAR(turdtabr,'dd/mm/YYYY') as turdtabr
				, TO_CHAR(turdtfec,'dd/mm/YYYY') as turdtfec 
				FROM prova.turma
				WHERE turid = {$_GET['editar']}
			";
 		$valueForm = $db->carregar($sql);
		$valueForm = $valueForm[0];
		
	} elseif(isset($_GET['excluir']) && !is_null($_GET['excluir'])){
		$sql = "DELETE FROM prova.turma WHERE turid = {$_GET['excluir']}";
		$db->executar($sql); 
 		$db->commit(); 

		echo '<script type="text/javascript">alert("Operação realizada com sucesso!");</script>';;
	}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	
	function salvar(teste)
	{
		var turnome  = $("#turnome");
		var turdtabr = $("#turdtabr");
		var turdtfec = $("#turdtfec");
		
		if(turnome.val() == ''){
			alert('O campo "Nome da Turma" não pode ser vazio!');
			turnome.focus();
			return false;
		}
		if(turdtabr.val() == ''){
			alert('O campo "Data de abertura da Turma" não pode ser vazio!');
			turdtabr.focus();
			return false;
		}
		if(turdtfec.val() == ''){
			alert('O campo "Data de fechamento da Turma" não pode ser vazio!');
			turdtfec.focus();
			return false;
		}
		
		$("#formulario").submit();
	}
	
	function excluir(id)
	{
		if(confirm('Tem certeza que deseja excluir a turma?')){
			window.location = "http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&excluir=" + id;
		}
	}
	
	function editar(id){
		window.location = "http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&editar=" + id;
	}
	
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="turid" id="turid" size="50" value="<?php echo $valueForm['turid'] ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turnome" size="50" value="<?php echo $valueForm['turnome'] ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turdtabr" size="50" value="<?php echo $valueForm['turdtabr'] ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turdtfec" size="50" value="<?php echo $valueForm['turdtfec'] ?>"></td>
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
			'
				<img title=\"Alterar\" src=\"../imagens/alterar.gif\" onclick=\"javascript:editar(' || turid || ');\"/>&nbsp;
				<img title=\"Excluir\" src=\"../imagens/excluir.gif\" onclick=\"javascript:excluir(' || turid || ');\"/>
			' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";
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
					<input value="Salvar" type="button" id="salvar" onclick="javascript:salvar('teste');">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>