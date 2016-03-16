<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}



function buscaDemanda($dados){
	global $db;
	
	//print_r($dados);
	$dmdid = $dados['dmdid'];
	
	if($dmdid){
		$sql = "SELECT dmdtitulo FROM demandas.demanda WHERE dmdid=$dmdid";
		$titulo = $db->pegaUm($sql);
	}
		
	if($titulo){
		echo "<b>Assunto:</b> $titulo";
	}else{
		echo "N";
	}
    exit;
}


/*
 * Executa a função que salva 
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_POST['dmdpai'] && $_POST['dmdfilhas']){
	
	$filhasArray = explode("," , $_POST['dmdfilhas']);
	
	//verifica se é numerico
	foreach($filhasArray as $v){
		if(!is_numeric($v)){

			die("<script>
					alert('Existem Demandas Filhas que não são numéricos!');
					history.back();
				 </script>");
			
		}
	}

	foreach($filhasArray as $f){
		
		if(is_numeric($f)){
			
			//a pai existe pai?
			$sql = "select dmdidorigem from demandas.demanda where dmdid = {$f}";
			$paidaPai = $db->pegaUm($sql);	
			
			/*
			//a pai existe filhas?
			$sql = "select dmdid from demandas.demanda where dmdidorigem = {$f}";
			$filhasdaPai = $db->carregarColuna($sql);
			*/
			
			if($paidaPai){
			    $sql = "UPDATE demandas.demanda SET dmdidorigem = {$paidaPai} WHERE dmdid in (select dmdid from demandas.demanda where dmdidorigem = {$f})";
				$db->executar($sql);
			}
			else{
				$sql = "UPDATE demandas.demanda SET dmdidorigem = NULL WHERE dmdid in (select dmdid from demandas.demanda where dmdidorigem = {$f})";
				$db->executar($sql);
			}
			
		}
		
	}
	
	//REMANEJA AS DEMANDAS
	$filhasArray = implode("," , $filhasArray);
    $sql = "UPDATE demandas.demanda SET dmdidorigem = {$_POST['dmdpai']} WHERE dmdid in ({$filhasArray})";
    $db->executar($sql);
	     
	$db->commit();
 	
	die("<script>
			alert('Operação realizada com sucesso!');
			location.href='demandas.php?modulo=principal/lista&acao=A&dmdid={$_POST['dmdpai']}'
		 </script>");
    	
}

	

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Remanejar Demandas', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );


?>
<html>

<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<form name="formulario" action="" method="post">

<table width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Demanda Pai:</td>
		<td>
		<?=campo_texto( 'dmdpai', 'S', 'S', '', 10, 10, '##########', '','','','','','',$titulo,'buscaDem()')?>
		<div id="divDemanda" style="display:none"></div>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Demandas Filhas:<br>(Separadas por vírgulas)</td>
		<td>
			<?=campo_texto( 'dmdfilhas', 'S', 'S', '', 75, 200, '', '','','','','','',$titulo)?>
			<br>
			(Ex: 56321,56422,76321,86321)
		</td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btalterar" id="btalterar" value="Executar" onclick="validaForm();">
		</td>
	</tr>	
</table>
</form>
</body>
</html>

<script type="text/javascript">
d = document.formulario;


function buscaDem()
{
	document.getElementById('divDemanda').style.display = "";
	document.getElementById('divDemanda').innerHTML = "Carregando...";

	$.ajax({
   		type: "POST",
   		url: "demandas.php?modulo=principal/remanejaDemanda&acao=A",
   		data: "requisicao=buscaDemanda&dmdid="+d.dmdpai.value,
   		success: function(msg){

   			if(msg == 'N'){
   				document.getElementById('divDemanda').innerHTML = '<font color=red>Demanda não encontrada.</font>';
   				d.btalterar.disabled = true;
   			}
   			else{
   				document.getElementById('divDemanda').innerHTML = msg;
   				d.btalterar.disabled = false;
   			}
   			
   		}
 		});
		
}


/*
* Function destinada a validação do formulário
*/
function validaForm(){
	
	if (d.dmdpai.value == ''){
		alert('O campo Demanda Pai é obrigatório!');
		d.dmdpai.focus();
		return false;
	}
	if (d.dmdfilhas.value == ''){
		alert('O campo Demandas Filhas é obrigatório!');
		d.dmdfilhas.focus();
		return false;
	}

	if(confirm('Deseja realmente executar esta operação?')){
		d.btalterar.disabled = true;
		d.submit();
	}
}
</script>
