<?php

function inserirObs(){
	global $db;
	$sql = " INSERT INTO demandas.observacoes 
			(usucpf,dmdid,obsdsc,obsstatus,obsdata) VALUES
			('".$_SESSION['usucpf']."', '".$_SESSION['dmdid']."','".$_POST['obsdsc']."',
			'A',now())";
	$db->executar($sql);
	$db->commit();
}

if($_POST['obsdsc'] && $_SESSION['dmdid'] && $_SESSION['usucpf']){
	inserirObs();
	print "<script>alert('Inserido com Sucesso!');window.opener.location.reload();window.close();</script>";	
}

monta_titulo( 'Observação', '' );
?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript">
	function validaForm(){
		if(document.formObservacao.obsdsc.value == ''){
			alert ('O campo Observação deve ser preenchido.');
			document.formObservacao.obsdsc.focus();
			return false;
		}
	}	
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formObservacao" name="formObservacao" action="" method="post" onsubmit="return validaForm();">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita">Observação:</td>
		<td>
			<?=campo_textarea('obsdsc', 'N ', 'S', '', 80, 5, 4000); ?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='submit' class='botao' value='Salvar' name='cadastrar' />&nbsp;
	    	<input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	    	
		</td>			
	</tr>	
</table>
</form>
</body>
</html>



