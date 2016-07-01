<?php
pegaSessoes(1,0,0);
$prtid 		= $_SESSION['academico']['prtid'];
?>
<html>
	<head>
		<title>Vincular Portaria de Provimento</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script type="text/javascript">
	var prtid_n;
	function vincularPortaria( prtid ){
		prtid_n= prtid.split("_",2); 
		//alert(prtid_n);
		window.opener.document.formulario.prtidautprovimento.value=prtid_n[0];
		window.opener.document.getElementById("DadosProvimento").innerHTML = "Nº da Portaria: "+prtid_n[1];
		window.close();
	}
	</script>
	</head>
	<body>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="60%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Portarias 
					</label>
				</td>
			</tr>
		</table>
		
		<br/>
				
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">		
			<tr bgcolor="#cdcdcd">
				<td  valign="top">
					<strong>Selecione a Portaria</strong>
				</td>				
			</tr>
			<?php
				montaListaPortariasProvimentos($prtid);
			?>
		</table>
		
	</body>
</html>