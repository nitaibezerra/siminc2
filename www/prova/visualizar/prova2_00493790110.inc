<?php
include  APPRAIZ."includes/cabecalho.inc";


if(isset($_SESSION['message'])) { echo "<div style='padding:10px; color:#990000; text-align:center'><h3>" . $_SESSION['message'] . "</h3></div>"; unset($_SESSION['message']); }

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

 if($_POST && !isset($_GET['alterar'])) {
 		$sql = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec) VALUES ('" . $_POST['turnome'] . "','" . formataDataBanco($_POST['turdtabr']) . "','" . formataDataBanco($_POST['turdtfec']) . "')";
  		$db->executar($sql); 
  		$db->commit();
  		$_SESSION['message'] = 'Operação efetuada com sucesso';
  		header( "Location: " . $_SERVER['REQUEST_URI'] );
 }
 
 if(isset($_GET['remover'])) {
 		$sql = "DELETE FROM prova.turma WHERE turid = " . $_GET['turid'];
  		$db->executar($sql); 
  		$db->commit();
  		$_SESSION['message'] = 'Operação efetuada com sucesso';
  		header( "Location: ?modulo=principal/prova2&acao=A");
 }
 
 if($_POST && isset($_GET['alterar'])) {
 		$sql = "UPDATE prova.turma  SET turnome = '" . $_POST['turnome'] . "', turdtabr = '" . formataDataBanco($_POST['turdtabr']) . "', turdtfec = '" . formataDataBanco($_POST['turdtfec']) . "' WHERE turid = " . $_GET['turid'];
  		$db->executar($sql); 
  		$db->commit();
  		$_SESSION['message'] = 'Operação efetuada com sucesso';
  		header( "Location: ?modulo=principal/prova2&acao=A");
 }
 
if(!$_POST && isset($_GET['alterar'])) {
	
 		$sql = "SELECT t.turnome,
			TO_CHAR(turdtabr,'dd/mm/YYYY') as turdtabr,
			TO_CHAR(turdtfec,'dd/mm/YYYY') as turdtfec
			FROM prova.turma t WHERE t.turid = " . $_GET['turid'];
 		
		$arDadosTurma = $db->carregar($sql);
		
 }

 function formataDataBanco($data) {
 	$data = explode("/", $data);
 	return $data[2] . "-" . $data[1] . "-" . $data[0];
 }

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>
<form name="formulario" id="formulario" method="post" action="" >

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50" required value="<?php echo isset($arDadosTurma[0]['turnome']) ? $arDadosTurma[0]['turnome'] : '' ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50" required value="<?php echo isset($arDadosTurma[0]['turdtabr']) ? $arDadosTurma[0]['turdtabr'] : '' ?>" ></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" size="50" required value="<?php echo isset($arDadosTurma[0]['turdtfec']) ? $arDadosTurma[0]['turdtfec'] : '' ?>" ></td>
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
			'<a href=\"prova.php?modulo=principal/prova2&acao=A&alterar=true&turid=' || turid || '\"  id=\"editar\"><img title=\"Alterar\" src=\"../imagens/alterar.gif\" /></a>&nbsp;<a href=\"prova.php?modulo=principal/prova2&acao=A&remover=true&turid=' || turid || '\" id=\"excluir\"><img title=\"Excluir\" src=\"../imagens/excluir.gif\" /></a>' as acao,
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
					<input value="Salvar" type="button" id="salvar">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>

<script>
	$(function() {
		$('#salvar').click(function(){
			var errors = 0;
			$('#formulario').find('input').each(function(){
				if($(this).val() == '') {
					$(this).after('<em style="color:#990000;"> Campo de preenchimento obrigatório</em>');
					errors++;
				}
			});
			
			if(errors == 0)
				$('#formulario').submit();
		});
		
		$('#excluir').click(function(){
			if(confirm('Tem certeza que deseja excluir a turma?')) {
				return true;
			};
			return false;
		});
		
	});

</script>