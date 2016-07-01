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

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	
	
</script>
<? 
echo '</br>';

if( $_POST['requisicao'] == 'Salvar' ){
	$data = explode('/',$_POST['turdtabr']);
	$data2 = explode('/',$_POST['turdtfec']);
	
	
	$turdtabr = $data['2'].'-'.$data['1'].'-'.$data['0'];
	$turdtfec = $data2['2'].'-'.$data2['1'].'-'.$data2['0'];
	
	
	$sql = "INSERT INTO prova.turma 
					(turnome, turdtabr, turdtfec) 
				VALUES ('".$_POST['turnome']."','".$turdtabr."','".$turdtfec."')";
		$db->executar($sql); 
		$result = $db->commit(); 
		
	 if($result == true){
	 	echo '<script>alert("Operação realizada com sucesso.");</script>';
	 }
	 
}

if( $_POST['requisicao'] == 'excluir' ){
	$sql = "DELETE FROM prova.turma 
				 WHERE turnome = '".$_POST['turnome']."'";
		$db->executar($sql); 
		$result = $db->commit();
// 	if($result == true){
//	 	echo '<script>alert("Operação realizada com sucesso.");</script>';
//	 }

	
}

if( $_POST['requisicao'] == 'editar' ){
//	$sql = "UPDATE prova.turma SET
//	 				turnome = '$nomeTur'
//	 				turdtabr ='$turDtAbr'
//	 				turdtfec ='$turDtFec' 
//				WHERE turnome = ".$_POST['turnome']."";die('altera');
//	 	$db->executar($sql); 
//		$result = $db->commit(); 
// 	if($result == true){
//	 	echo '<script>alert("Operação realizada com sucesso.");</script>';
	 //}
}

	?>
<form name="formulario" id="formulario" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita" >Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turnome" size="50" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turdtabr" size="50"  ></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turdtfec" size="50">
			<input type="hidden" id="requisicao" name="requisicao" value=""></td>
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
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" onclick=\"editar()\"  /></a> <img title=\"Excluir\" src=\"../imagens/excluir.gif\"  onclick=\"excluir()\" />' as acao, 
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
					<input value="Salvar" type="button" onclick="salvar()" id="salvar">
					
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>


<script>
	
		
		function salvar(){
			var  nomeTur =   $("input[name=turnome]").val();
			var  turDtAbr =   $("input[name=turdtabr]").val();
			var  turDtFec =   $("input[name=turdtfec]").val();
			
				if(nomeTur == ''){
					alert('O senhor de digitar uma turma');		
				}else if (turDtAbr == ''){
					alert('Digite a data de abertura da turma');	
				}else if (turDtFec == ''){
					alert('Digite a data de fechamento da turma');
				}else{
					document.getElementById('requisicao').value = 'Salvar';
					document.forms["formulario"].submit();
					
				}
				
		}
		
		function excluir(){
			
				if (confirm('Tem certeza que deseja excluir a turma?')){
					document.getElementById('requisicao').value = 'excluir';
					document.forms["formulario"].submit();	
					
				}
				
		}
		function editar(){
				
				
			document.getElementById('requisicao').value = 'editar';
			document.forms["formulario"].submit();
			
		}		
		
	

</script>

