<?php

if($_REQUEST['prpnumeroprocesso'] == 0){
	echo "	<script>
			alert('Selecione um programa para poder inserir o Plano Interno.');
			window.close();
		</script>";
	die();
}

if($_REQUEST['salvar']){

	$sql = "INSERT INTO par.empenhoprocessoplanointerno (
				prpnumeroprocesso, 
				plicod) 
			VALUES (
				'".$_REQUEST['prpnumeroprocesso']."', 
				'".$_REQUEST['plicod']."')";

	if($db->executar($sql)){
		$db->commit();
		echo "	<script>
					alert('Operação realizada com sucesso.');
					location.href='par.php?modulo=principal/cadPlanoInterno&acao=A&prpnumeroprocesso='+".$prpnumeroprocesso.";
					//window.close();
				</script>";
	}
}


$sqlPlanos = "  SELECT DISTINCT 
					plicod as codigo, 
					plicod as descricao
				FROM 
					monitora.pi_planointerno pi
				INNER JOIN monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
				INNER JOIN monitora.ptres pt ON pt.ptrid = pip.ptrid
				WHERE 
					pi.plistatus = 'A' 
					AND prgcod::integer IN (1061, 1060, 1062, 1448, 1374, 1377, 2030)
					AND plicod NOT IN ( SELECT plicod FROM par.empenhoprocessoplanointerno WHERE prpnumeroprocesso = '".$_REQUEST['prpnumeroprocesso']."' )  
				ORDER BY 
					plicod ";
//inserir ano no Sql de planos
// and pliano = '2009'

/************************* MONTA TÍTULO *************************/
monta_titulo($titulo_modulo,'(Plano Interno)');
?>
<html>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body>
<form method="POST"  name="formulario">
<input type=hidden name="salvar" id="salvar" value="0" />
<input type=hidden name="modo" id="modo" value="<?php echo($modo); ?>" />
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita" >Processo: </td>
        <td ><?=$_REQUEST['prpnumeroprocesso']; ?></td>
      </tr>
		<tr>
        	<td align='right' class="SubTituloDireita" >Plano Interno: </td>
        	<td ><?php
       			$db->monta_combo( "plicod", $sqlPlanos , 'S', 'Selecione', 'filtraPTRES', '', '','','S', 'plicod', false, $plicod, 'Plano Interno' ); 
       			?>
       		</td>
		</tr>
      <tr>
      	<td class="SubTituloDireita"></td>
      	<td class="SubTituloEsquerda" align="center">
      		<input type="button" name="salvar" value="salvar"  onclick="salvarDados();" />
      	</td>
      </tr>
</table>
</form>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th  colspan="3" >Lista de Planos Internos</th>
	</tr>
	<tr>
		<td>
		<?php 
			$cabecalho 			= array("Processo","Plano Interno");
			$sqlPlanoInternos 	= "SELECT prpnumeroprocesso, plicod FROM par.empenhoprocessoplanointerno WHERE prpnumeroprocesso = '".$_REQUEST['prpnumeroprocesso']."'";
			$db->monta_lista_simples($sqlPlanoInternos,$cabecalho,20, 10, 'N', '100%', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan="3"><input id="btnfechar" type="button" name="btnfechar" value="Fechar" onclick="window.close();" /></td>
	</tr>
</table>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function salvarDados(){
	var nomeform 		= 'formulario';
	campos 				= new Array("plicod");
	tiposDeCampos 		= new Array("select");

	if(validaForm(nomeform, campos, tiposDeCampos, false )){
		document.getElementById("salvar").value = 1;
		document.formulario.submit();
	}
}
</script>
</body>
</html>