<?php

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$_POST['enbcnpj'] = str_ireplace(array(".","/","-"), "", $_POST['enbcnpj']);
	$_POST['enbcep']  = str_ireplace("-", "", $_POST['enbcep']);
		
	if(!$_POST['enbid']) {
		
		$sql = "INSERT INTO emenda.entidadebeneficiada(
		            		enbstatus, enbano, enbdataalteracao, 
		            		enbnome, enbcnpj, muncod, estuf, enblog, enbnum, enbbai, enbcep, 
				            enbnumdddcomercial, enbnumcomercial, enbnumfax, enbemail)
		    				VALUES ('A', '".$_SESSION['exercicio']."', NOW(), 
		            		'".$_POST['enbnome']."', '".$_POST['enbcnpj']."', 
		            		'".(integer)$_POST['muncod']."', '".$_POST['estuf']."',
		            		'".$_POST['enblog']."','".$_POST['enbnum']."','".$_POST['enbbai']."','".$_POST['enbcep']."',
		            		'".$_POST['enbnumdddcomercial']."','".$_POST['enbnumcomercial']."','".$_POST['enbnumfax']."', '".$_POST['enbemail']."') RETURNING enbid;";
		
		$enbid = $db->pegaUm($sql);
			
	} else {		
		$sql = "UPDATE emenda.entidadebeneficiada SET enbstatus = 'A', muncod = '".$_POST['muncod']."',
					  estuf = '".$_POST['estuf']."',
					  enblog = '".$_POST['enblog']."',
					  enbnum = '".$_POST['enbnum']."',
					  enbbai = '".$_POST['enbbai']."',
					  enbcep = '".$_POST['enbcep']."',
					  enbnumdddcomercial = '".$_POST['enbnumdddcomercial']."',
					  enbnumcomercial = '".$_POST['enbnumcomercial']."',
					  enbnumfax = '".$_POST['enbnumfax']."',
					  enbemail = '".$_POST['enbemail']."',
					  enbnome = '".$_POST['enbnome']."' 
				WHERE 
				  	enbid = ".$_POST['enbid'];
		$db->executar( $sql );		
		$enbid = $_POST['entid'];		
	}

	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='emenda.php?modulo=principal/cadastrarEntidadeBeneficiada&acao=A&enbid={$enbid}';
			window.opener.pesquisar();
			window.close();
		  </script>";
	exit;
}

if($_REQUEST['entid']) {
	$sql = "SELECT enbid, enbemail, enbnumdddcomercial, enbnumcomercial, enbnumfax, enbnome, enbnum, enbbai, enblog, SUBSTR(enbcep,1,5)||'-'||SUBSTR(enbcep,6,3) as enbcep, enbemail, estuf, muncod, SUBSTR(enbcnpj,1,2)||'.'||SUBSTR(enbcnpj,3,3)||'.'||SUBSTR(enbcnpj,6,3)||'/'||SUBSTR(enbcnpj,9,4)||'-'||SUBSTR(enbcnpj,13,2) as enbcnpj FROM emenda.entidadebeneficiada WHERe enbid='".$_REQUEST['entid']."'";
	$entidadebeneficiada = $db->pegaLinha($sql);
	$enbid = $entidadebeneficiada['enbid'];
	$enbcnpj = $entidadebeneficiada['enbcnpj'];
	$enbnome = $entidadebeneficiada['enbnome'];
	$estuf = $entidadebeneficiada['estuf'];
	$muncod = $entidadebeneficiada['muncod'];
	$enbemail = $entidadebeneficiada['enbemail'];
	$enbcep = $entidadebeneficiada['enbcep'];
	$enbbai = $entidadebeneficiada['enbbai'];
	$enblog = $entidadebeneficiada['enblog'];
	$enbnum = $entidadebeneficiada['enbnum'];
	$enbemail = $entidadebeneficiada['enbemail'];
	$enbnumdddcomercial = $entidadebeneficiada['enbnumdddcomercial'];
	$enbnumcomercial = $entidadebeneficiada['enbnumcomercial'];
	$enbnumfax = $entidadebeneficiada['enbnumfax'];
	$habilita='N';
	$habilitaCampo = 'N';
	
	if( in_array( ADMINISTRADOR_INST, pegaPerfilArray( $_SESSION['usucpf'] ) ) || $db->testa_superuser() ){
		$habilitaCampo = 'S';
	}
}

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
	<script type="text/javascript" src="/includes/webservice/pj.js"></script>
	<script type="text/javascript" src="js/emenda.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
  </head>
  <script>
	function validarCNPJ( obj ){

		if( obj.value ){
			if( !validarCnpj( obj.value  ) ){
				alert( "CNPJ inválido!\nFavor informar um CNPJ válido!" );
				obj.value = "";	
				document.getElementById( "enbnome" ).value = "";
			}
		}	
		
	}
	  
  </script>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>
      <form action="emenda.php?modulo=principal/cadastrarEntidadeBeneficiada&acao=A&opt=salvarRegistro" method="post" id="formulario" name="formulario">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
		<td class="subtitulodireita">CNPJ:</td>
		<td>
			<span id="lupaCnpj" style="text-align: center; vertical-align: middle;"></span>
			<input type="hidden" name="enbid" id="enbid" value="<?=$enbid; ?>"/>
			<?php echo campo_texto( 'enbcnpj', 'S', $habilita, '', 22, 18, '##.###.###/####-##', '', 'left', '', 0, 'id="enbcnpj"', '', '', 'validarCNPJ(this);verificarEntidadeBeneficiada();');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Nome:</td>
		<td>
			<?php echo campo_texto( 'enbnome', 'S', $habilitaCampo, '', 60, 150, '', '', 'left', '', 0, 'id="enbnome"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">UF:</td>
		<td>
			<?php echo campo_texto( 'estuf', 'S', $habilitaCampo, '', 3, 2, '', '', 'left', '', 0, 'id="estuf"');  ?>
			<input type="hidden" name="muncod" id="muncod" value="<? echo $muncod; ?>">
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Email:</td>
		<td>
			<?php echo campo_texto( 'enbemail', 'S', $habilitaCampo, '', 60, 200, '', '', 'left', '', 0, 'id="enbemail"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">CEP:</td>
		<td>
			<?php echo campo_texto( 'enbcep', 'S', $habilitaCampo, '', 11, 10, '', '', 'left', '', 0, 'id="enbcep"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Bairro:</td>
		<td>
			<?php echo campo_texto( 'enbbai', 'S', $habilitaCampo, '', 60, 200, '', '', 'left', '', 0, 'id="enbbai"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Logradouro:</td>
		<td>
			<?php echo campo_texto( 'enblog', 'S', $habilitaCampo, '', 60, 200, '', '', 'left', '', 0, 'id="enblog"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Número:</td>
		<td>
			<?php echo campo_texto( 'enbnum', 'S', $habilitaCampo, '', 10, 10, '', '', 'left', '', 0, 'id="enbnum"');  ?>
		</td>
		</tr>
		<tr>
		<td class="subtitulodireita">Telefone comercial:</td>
		<td>
			<?php echo "( ".campo_texto( 'enbnumdddcomercial', 'N', 'S', '', 3, 2, '', '', 'left', '', 0, 'id="enbnumdddcomercial"')." ) ".campo_texto( 'enbnumcomercial', 'S', 'S', '', 10, 9, '', '', 'left', '', 0, 'id="enbnumcomercial"'); ?>
		</td>
		</tr>		
		<tr>
		<td class="subtitulodireita">Telefone fax:</td>
		<td>
			<?php echo campo_texto( 'enbnumfax', 'S', 'S', '', 10, 9, '', '', 'left', '', 0, 'id="enbnumfax"');  ?>
		</td>
		</tr>		

		<tr>
		<td colspan=2 class="subtitulodireita"><input type="button" name="submeter" value="Gravar" onclick="submeterEntidadeBeneficiada();"></td>
		</tr>

		</table>
	</form>

      
      
<script type="text/javascript">

function submeterEntidadeBeneficiada() {

	if(document.getElementById('enbcnpj').value == '') {
		alert('CNPJ é obrigatório');
		return false;	
	}
	
	document.formulario.submit();

}

function fechar(){
	window.close();
}

</script>
</div>
</body>
</html>