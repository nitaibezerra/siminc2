<?php 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

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
 
 if($_POST['acao_form'] == 'cad'){
 
 		$dt_abert = explode('/', $_POST['turdtabr']);
 		$dt_fech  = explode('/', $_POST['turdtfec']);
 		
 		$dt_abert = $dt_abert[2].'-'.$dt_abert[1].'-'.$dt_abert[0];
 		$dt_fech = $dt_fech[2].'-'.$dt_fech[1].'-'.$dt_fech[0];
 		
 		$sql = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec) 
 				VALUES ('".$_POST['turnome']."','".$dt_abert."','".$dt_fech."')";
  		$db->executar($sql); 
  		$db->commit(); 
  		
  		
  		
	echo '<script type="text/javascript"> alert("Operação realizada com sucesso.")</script>';
 
 }
 
 if($_POST['acao_form'] == 'del'){
 	$sql = "DELETE FROM prova.turma WHERE turid = ".$_POST['turid']." ";
  		$db->executar($sql); 
  		$db->commit();
  		
  		echo '<script type="text/javascript"> alert("Operação realizada com sucesso.")</script>';
 }
 
 if($_POST['acao_form'] == 'edt'){
 
 	$dt_abert = explode('/', $_POST['turdtabr']);
 		$dt_fech  = explode('/', $_POST['turdtfec']);
 		
 		$dt_abert = $dt_abert[2].'-'.$dt_abert[1].'-'.$dt_abert[0];
 		$dt_fech = $dt_fech[2].'-'.$dt_fech[1].'-'.$dt_fech[0];
 		
 	$sql = "UPDATE prova.turma SET turnome = '".$_POST['turnome']."', 
 								   turdtabr = '".$dt_abert."', 
 								   turdtfec = '".$dt_fech."'  
 			WHERE turid = ".$_POST['turid']." ";
  		$db->executar($sql); 
  		$db->commit();
 }

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	
	
	$( document ).ready(function() {
	
	$('#salvar').click(function(){
		
		var no_tur   = $('#turdsc').val();
		var dt_a_tur = $('#turabr').val();
		var dt_f_tur = $('#turfec').val();
		
		$('#acao_form').val('cad');
		
		if($.trim(no_tur) == '' || $.trim(dt_a_tur) == '' || $.trim(dt_f_tur) == ''){
			alert('Todos os campos devem ser preenchidos.');
		}else{
			$('#formulario').submit();
		}
		
		
	});
	
	
		alteraDado = function(id, turnome, tur_dt_abr, tur_dt_fec){
			//alert(id);
			
			$('#turid').val(id);
			$('#turdsc').val(turnome);
			$('#turabr').val(tur_dt_abr);
			$('#turfec').val(tur_dt_fec);
			
			$('#acao_form').val('edt');
			
			$('#turdsc').focus();
		
		};
		
		deletaDado = function(id){
			//alert(id);
			
			var resp = confirm('Tem certeza que deseja excluir a turma?');
			
			if(resp == true){
				$('#acao_form').val('del');
				$('#turid').val(id);
				$('#formulario').submit();
			}
			
			
		
		};
		
		 
	});
	
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td>
				<input type="text" name="turdtfec" id="turfec" size="50">
				<input type="hidden" name="acao_form" id="acao_form" value="0">
				<input type="hidden" name="turid" id="turid" value="0">
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
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" onclick=\"alteraDado('||turid||',\''||turnome||'\',\''||TO_CHAR(turdtabr,'dd/mm/YYYY')||'\',\''||TO_CHAR(turdtfec,'dd/mm/YYYY')||'\')\" />&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" onclick=\"deletaDado('||turid||')\" />' as acao, 
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