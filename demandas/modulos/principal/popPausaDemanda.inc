<?php


if ($_REQUEST['pdmid']){
		
	$sql = "UPDATE demandas.pausademanda SET pdmdatafimpausa = '".date('Y/m/d H:i').":00' WHERE pdmid = ".$_REQUEST['pdmid'];
	$db->executar($sql);
	$db->commit();
	
	//envia email
	enviaEmailDespausarDemanda();
	
	die("<script>
			window.opener.location.reload();
			alert('Operação realizada com sucesso!');
			window.close();
		 </script>");	
}

/*
 * Executa a função que salva 
 * Retorna mensagem de sucesso e redireciona a página
 */
if ($_POST['tpaid'] && $_POST['just']){
 	

	$sql = sprintf("INSERT INTO demandas.pausademanda
					(
						dmdid,
						tpaid,
						pdmjustificativa,
						pdmstatus,
						pdmdatainiciopausa
					)VALUES (
						%d,
						%d,
						'%s',
						'A',
						'%s'
					) RETURNING pdmid",
					$_SESSION['dmdid'],
					$_POST['tpaid'],
					$_POST['just'],
					date('Y/m/d H:i').':00'
					);

	$pdmid = $db->pegaUm($sql);
	
	$sql = "select t.tpafimautomatico, t.tpatempopausa, t.tpaunidadetempopausa, p.pdmdatainiciopausa 
			from demandas.pausademanda p 
			inner join demandas.tipopausademanda t ON t.tpaid=p.tpaid
			where p.pdmid = $pdmid";
			
	$dados = $db->pegaLinha($sql);
	
	
	if($dados['tpafimautomatico'] == 'T'){
		 
		 $data 		= $dados['pdmdatainiciopausa'];
	     $ano 		= substr ( $data, 0, 4 );
	     $mes 		= substr ( $data, 5, 2 );
	     $dia 		= substr ( $data, 8, 2 );
	     $h 		= substr ( $data, 11, 2 );
	     $m 		= substr ( $data, 14, 2 );
	     //$s 		= substr ( $data, 17, 2 );
	     
	     $t = $dados['tpatempopausa'];
	     
	     if($dados['tpaunidadetempopausa'] == 'M'){ //minuto
	     	$dataFinal = mktime ( $h, $m+$t, 0, $mes, $dia, $ano );
	     }elseif($dados['tpaunidadetempopausa'] == 'D'){ //dia
	     	$dataFinal = mktime ( $h, $m, 0, $mes, $dia+$t, $ano );
	     }elseif($dados['tpaunidadetempopausa'] == 'H'){ //hora
	     	$dataFinal = mktime ( $h+$t, $m, 0, $mes, $dia, $ano );
	     }
		 
	     $dataFinalx = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);
	
	     $sql = "UPDATE demandas.pausademanda SET pdmdatafimpausa = '$dataFinalx' WHERE pdmid = ".$pdmid;
		 $db->executar($sql);
	     
	
	}
	
	$db->commit();

	//envia email
	enviaEmailPausarDemanda();
 	
	die("<script>
			window.opener.location.reload();
			alert('Operação realizada com sucesso!');
			window.close();
		 </script>");
}

	

//include APPRAIZ ."includes/cabecalho.inc";
global $db;
$db->cria_aba( $abacod_tela, $url, '' );

?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">


<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>

<form name="formulario" action="" method="post">

<table width="99%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
			<b>PAUSAR DEMANDA</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Estado atual:</td>
		<td>Em Atendimento</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Ação:</td>
		<td>Pausar Demanda</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Motivo:</td>
		<td>
		<?php
			$sql = "SELECT 
					 tpaid AS codigo,
					 tpadsc AS descricao
					FROM 
					 demandas.tipopausademanda
					";
			$lcadescricao = $db->PegaUm($sql);
			$db->monta_combo( 'tpaid', $sql, 'S', '-- Informe o Motivo --', '', '' );
			echo obrigatorio();
		?>	
		</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Justificativa:</td>
		<td><?= campo_textarea('just', 'S', 'S', '', 61, 17, 5000); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btalterar" id="btalterar" value="Tramitar" onclick="validaForm();">
		</td>
	</tr>	
</table>
</form>
</body>
</html>

<script type="text/javascript">
d = document.formulario;


/*
* Function destinada a validação do formulário
*/
function validaForm(){
	
	if (d.tpaid.value == ''){
		alert('O campo Motivo é obrigatório!');
		d.tpaid.focus();
		return false;
	}
	if (d.just.value == ''){
		alert('O campo Justificativa é obrigatório!');
		d.just.focus();
		return false;
	}

	d.btalterar.disabled = true;
	d.submit();
}
</script>
