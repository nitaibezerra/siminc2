<?php

include APPRAIZ."includes/cabecalho.inc";
include "importstn.inc";
print '<br>';
//echo "teste";
//include "config.inc";
//include APPRAIZ . "includes/classes_simec.inc";
//include APPRAIZ . "includes/funcoes.inc";
//include "includes/cabecalho.inc";

$slcimport = $_REQUEST['slcimport'];
//$txtstncaminho = $_REQUEST['uploadedfilestn'];
define('Cor_Aguardando',"#0000FF");
define('Cor_Executando',"#FF9900");
define('Cor_Erro',"#FF0000");
define('Cor_Sucesso',"#336633");

if (is_null($_REQUEST['importar_stn']))
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
		<br/>
		<form method="POST" name="formulario" enctype="multipart/form-data">
			<? monta_titulo('Comunicação com o SIAFI atraves dos Arquivos do STN','Selecione os parâmetros da importação');?>
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacaostn/interfacestn"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td>
					<?
					  $pathimport = STN_DIR_TXT_FILES;
					  $dirs = glob($pathimport."*.txt");
					  $arquivos_import = '';
					  $encontrado = false;
					  $visivel = false;
					  foreach($dirs as $dir){
					  	$array_files = array_reverse(explode('/',$dir));
					  	$dirfile = $array_files[0];

     					if(is_file($dir))
     					{
     						$encontrado = true;

     						if (!verificaLog($dirfile))
							{

								$arquivos_import=true;
     						}
     					}
     				  }

     				  if ($encontrado)
     				  {
     				  	if (is_null($arquivos_import))
     				  	{
     				  		echo "Arquivos já foram processados.";

     				  	}else
     				  	{
     				  		echo "Este processo realizar a importação dos dados, clique em 'importar' para continuar.";
     				  		$visivel = true;
     				  	}
     				  }else
     				  {
     				  	echo "Nenhum Arquivo a ser Atualizado.";
     				  }


					  ?>
					</td>
					</tr>
					<input type=hidden name="importar_stn" value="<?echo $arquivos_import?>"/>
				<tr bgcolor="#C0C0C0">
				<? if ($visivel) {?>
					<td>
						<input type="button" name="btnimportar" value="Importar" onclick="enviar_formulario()"/>
					</td>
				<?}else
				{?>
				<td>&nbsp;</td>
				<?}?>
				</tr>
			</table>
		</form>
		<br/>
		</body>
</html>
<?
}else
{

	$pathimport = STN_DIR_TXT_FILES;
	$arquivos = glob($pathimport."*.txt");

	require_once(APPRAIZ . "includes/PogProgressBar.php" );

	$objBar0 = new PogProgressBar( 'pb0' );
	$objBar1 = new PogProgressBar( 'pb1' );

	// set themes
	$objBar0->setTheme( 'blue' );
	$objBar1->setTheme( 'green' );

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
		<form method="POST" name="formul" enctype="multipart/form-data">
			<? monta_titulo('Comunicação com o SIAFI atraves dos Arquivos do STN','Executando a(s) Importação(ões)');?>
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacaostn/interfacestn"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr>
					<td align='right' class="subtitulodireita">Importação programadas:</td>
					<td>
					<?
					foreach($arquivos  as $arquivo){
					  	$array_files = array_reverse(explode('/',$arquivo));
					  	$file = $array_files[0];
						echo $file.' - '.'<input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;color:#0000FF;" name="txt'.str_replace(".","_",$file).'" size = "50" value = "aguardando inicio.">'.'<br><br>';
					}
					?>
					</td>
					</tr>
				<tr>

				<?monta_titulo('','Progresso da Importação');?>
    			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Arquivo Atual<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtquantidade" >
            	<? $objBar0->draw(); ?></td></tr>
            	<tr></tr>
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Total da Importação<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtquantidade" >
            	<? $objBar1->draw(); ?></td></tr>
            	<tr></tr>
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Status atual:</td><td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtstatus" size = "50"></td></tr>
        		</table>
        		</tr>
			</table>
		</form>
		<br/>
		</body>
</html>

<?

	//dbg(sizeof($arquivos));

	//Executa codigo
	//Processa_Array_Arquivos(explode(';',$_REQUEST['importar_stn']));
	Processa_Array_Arquivos($arquivos);
}
?>
	<script language="javascript">


		function enviar_formulario()
		{

			if ( validar_formulario() ) {
				document.formulario.importar.value = '1';
				document.formulario.submit();
			}
		}

		function validar_formulario() {
			var validacao = true;
			var mensagem = '';

			if ( !validacao ) {
				alert( mensagem );
			}
			return validacao;
		}

</script>