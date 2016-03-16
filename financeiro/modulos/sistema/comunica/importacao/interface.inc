<?php
/*
 * Interfece principal do sistema de importação dos Dados do Siafi via Grande Porte
 *  
 */
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
include ("funcoes.inc");

$pathimport = SIAF_DIR_TXT_FILES;
$slcano = $_REQUEST['slcano'];
$slcimport = $_REQUEST['slcimport'];


if (is_null($slcano))
{
	

//Formulario de Parametros da Importação
?>
	
 <html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">

		<br>
		<form method="POST" name="formulario">
			
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacao/importacaosiafi"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
			
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Ano:</td>
					<td>
					<select name="slcano">
					<option value="" selected>selecione um ano</option>
					 <?php 

					  $dirs = glob($pathimport."*");
					  
					  foreach($dirs as $dir){
					  	$dirano = substr($dir,strlen($dir)-4);
     					if(is_dir($dir) and is_numeric($dirano)){
     						print '<option value="'.$dirano.'">'.$dirano.'</option>';
     					}
     				  }
					  ?>
					  </select>
					</td>
			 	</tr>
				<input type="hidden" name="sisfinalidade_selc" value=""/>
				<tr>
					<td align='right' class="subtitulodireita">Importação disponíveis:</td>
					<td>
					<p>(Selecione as importações que deseja executar)</p>
					<p><input type="checkbox" name="slcimport[]" value="Nota de Empenho" checked>Nota de Empenho</p>
					<p><input type="checkbox" name="slcimport[]" value="PTRES" checked>PTRES</p>
					<p><input type="checkbox" name="slcimport[]" value="Saldo Contabil" checked>Saldo Contabil</p>
					</td>
					</tr>
					<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>
						<input type="button" name="btnimportar" value="Importar" onclick="enviar_formulario()"/>
					</td>
				</tr>
			</table>
		</form>
		<br/>
 </body>
 </html>		

<?php
}else
{

?>

 <html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">

		<br/>
		<form method="POST" name="formul">
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacao/importacaosiafi"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Ano de Referência:</td>
					<td><?php print $slcano; ?></td>
			 	</tr>
				<tr>
					 <td align='right' class="subtitulodireita">Importação programadas:</td>
					 <td>
					<?php

						if (is_null($slcimport))
						{
							echo "Nenhuma importação programada";
						}else
						{
							foreach ($slcimport as $lista_import)
							{
								
								echo $lista_import.' - '.'<input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;color:#0000FF;" name="txt'.str_replace(' ','_',$lista_import).'" size = "50" value = "aguardando inicio.">'.'<br><br>';

							}
						}
					?>
					 </td>
				</tr>
				<tr>
				<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr><td align = 'right' class="subtitulodireita" width="150px">Importando:<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txttipo" >
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Total da Importação<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtquantidade" >
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Importação do Arquivo Atual<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtarquivo" >
            	<tr></tr>
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Status:</td><td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtstatus" size = "50"></td></tr>
        		</table>
        		</tr>
			</table>
		</form>
		<br/>
 </body>
 </html>		


<?php


$ano_referencia = $slcano;
 
 foreach ($slcimport as $lista_import)
 {
	switch ($lista_import){
		case 'Nota de Empenho':
			ImprimeStatus('formul.txtNota_de_Empenho',"Executando importação" ,Cor_Executando);
			include('importa_noe.inc');
			$resultado = RodaNOE();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtNota_de_Empenho',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtNota_de_Empenho',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'PTRES':
			ImprimeStatus('formul.txtPTRES',"Executando importação" ,Cor_Executando);
			include('importa_ptres.inc');
			$resultado = RodaPTRES();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtPTRES',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtPTRES',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'Saldo Contabil':
			ImprimeStatus('formul.txtSaldo_Contabil',"Executando importação" ,Cor_Executando);
			include('importa_saldo.inc');
			$resultado = RodaSALDO();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtSaldo_Contabil',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtSaldo_Contabil',$sucessoImport,Cor_Sucesso);
			}
			break;
		break;		
	}
 }


}
?>

 <script language="javascript">


	function enviar_formulario() {

		if ( validar_formulario() ) {
			document.formulario.importar.value = '1';
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( document.formulario.slcano.value == "" ) {
			mensagem += '\nSelecione o ano da importação.';
			validacao = false;
		}

		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

 </script>
