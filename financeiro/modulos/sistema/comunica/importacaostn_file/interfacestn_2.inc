<?php

include APPRAIZ."includes/cabecalho.inc";
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

if (is_null($_FILES["uploadedfilestn"]["name"]))
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
					<td align = 'right' class="subtitulodireita" width="150px">Caminho do Arquivo:</td>
					<td>
					<input name="uploadedfilestn" type="file" />
					</td>
			 	</tr>
				<input type="hidden" name="sisfinalidade_selc" value=""/>
				<tr>
					<td align='right' class="subtitulodireita">Importação disponíveis:</td>
					<td>
					<p>(Selecione as importações que deseja executar)</p>
					<p><input type="checkbox" name="slcimport[]" value="CD" checked>Creditos Descentralizados</p>
					<p><input type="checkbox" name="slcimport[]" value="SC" checked>Saldo Contabil</p>
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
<?
}else
{

	require_once(APPRAIZ . "includes/PogProgressBar.php" );

	$objBar0 = new PogProgressBar( 'pb0' );
	//$objBar1 = new PogProgressBar( 'pb1' );

	// set themes
	$objBar0->setTheme( 'blue' );
	//$objBar1->setTheme( 'green' );

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
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Arquivo de Referência:</td>
					<td><?
					/*$tmp = str_replace("\\\\","?",$txtstncaminho);
					$tmp = str_replace("/","?",$tmp);
					$array_arquivo_prm =array_reverse(explode("?",$tmp));
					$arquivo_prm = $array_arquivo_prm[0];
					$caminho_prm = str_replace($arquivo_prm,"", $txtstncaminho);
					echo $arquivo_prm;*/
					echo $_FILES["uploadedfilestn"]["name"];
					?></td>
			 	</tr>
				<tr>
					<td align='right' class="subtitulodireita">Importação programadas:</td>
					<td>
					<?

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

				<?monta_titulo('','Status da Importação Atual');?>
    			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Total da Importação<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtquantidade" >
            	<? $objBar0->draw(); ?></td></tr>
            	<tr></tr>
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Status:</td><td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtstatus" size = "50"></td></tr>
        		</table>
        		</tr>
			</table>
		</form>
		<br/>
		</body>
</html>

<?
}

//Executa codigo

if (is_null($_FILES["uploadedfilestn"]["tmp_name"]))
{
	//imprime status sem importacões
}
else{
	$array_arquivo_prm =array_reverse(explode("/",$_FILES["uploadedfilestn"]["tmp_name"]));
	$arquivo_prm = $array_arquivo_prm[0];
	$caminho_prm = str_replace($array_arquivo_prm[0],"",$_FILES["uploadedfilestn"]["tmp_name"]);
	define ('STN_DIR_TXT_FILES',$caminho_prm);
	include ("importstn.inc");

	foreach($slcimport as $value)
	{
		$Layouts[$value]=$value;
	}



	//Dados do Log
	$arquivo_nome = $arquivo_prm;
	$data_atual = date("Y-m-d H:i:s");
	$matriz[$arquivo_nome]['logarquivonome']=$_FILES["uploadedfilestn"]["name"]; //armazena nomes dos arquivos na matriz
	//$matriz[$arquivo_nome]['logarquivodata'] ="'".date("Y-m-d H:i:s", filectime(STN_DIR_TXT_FILES.$arquivo_nome))."'";
	$matriz[$arquivo_nome]['logarquivodata'] ="'".date("Y-m-d H:i:s", $data_atual)."'";
	$matriz[$arquivo_nome]['logdatafim'] = "Null";
	$matriz[$arquivo_nome]['logdataini'] = "'".$data_atual."'";
	$matriz[$arquivo_nome]['logimporterros'] = "TRUE";
	$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
	$matriz[$arquivo_nome]['loginformacoes'] ="Null";
	$matriz[$arquivo_nome]['logarquivoano']= "Null";
	$matriz[$arquivo_nome]['logarquivotipo'] = "stn";
	$matriz[$arquivo_nome]['logatdregistros'] = 0;
	$matriz[$arquivo_nome]['caminho']="'".STN_DIR_TXT_FILES."'";
	$matriz[$arquivo_nome]['logdataimport']="'".$DataInicioImporta."'";
	$matriz[$arquivo_nome]['logdatainiscript'] ='Null';
	$matriz[$arquivo_nome]['logdatafimscript'] ='Null';

	$valor = processa_arquivo($_FILES["uploadedfilestn"]["tmp_name"],$_FILES["uploadedfilestn"]["name"],$_FILES["uploadedfilestn"]["size"],$Layouts);


	switch ($valor)
	{
		case $ErroImport[5]:
			$data_atual = date("Y-m-d H:i:s");
			$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
			$matriz[$arquivo_nome]['logerros'] = $ErroImport[5];
			gravarLog($matriz[$arquivo_nome]);
			break;

		case $ErroImport[8]:
			$data_atual = date("Y-m-d H:i:s");
			$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
			$matriz[$arquivo_nome]['logerros'] = $ErroImport[8];
			gravarLog($matriz[$arquivo_nome]);
			break;

		case $ErroImport[17]:
			$data_atual = date("Y-m-d H:i:s");
			$matriz[$arquivo_nome]['logdatafim'] = "'".$data_atual."'";
			$matriz[$arquivo_nome]['logerros'] = $ErroImport[17];
			gravarLog($matriz[$arquivo_nome]);
			break;

		case true:
			$matriz[$arquivo_nome]['logatdregistros'] = $valor;
			$matriz[$arquivo_nome]['logdatainiscript'] ="'".date("Y-m-d H:i:s")."'";
			$sufixo = str_replace("*.txt","",$_FILES["uploadedfilestn"]["name"]);
			ImprimeStatus("formul.txtstatus","Executando o script" ,Cor_Executando);
			if (RodarQuery(STN_DIR_TXT_FILES_DESTINO,$sufixo.'*.sql', "Roda_".$sufixo.".sql"))
			{
				$matriz[$arquivo_nome]['logdatafimscript'] ="'".date("Y-m-d H:i:s")."'";
				gravarLog($matriz[$arquivo_nome],4);
				gravarLog($matriz[$arquivo_nome],2);
				ImprimeStatus("formul.txtCD","Importação realizada com sucesso" ,Cor_Sucesso);
				ImprimeStatus("formul.txtstatus","Importação realizada com sucesso" ,Cor_Sucesso);
			}else
			{
				ImprimeStatus("formul.txtstatus",$ErroImport[12],Cor_Erro);
				ImprimeStatus("formul.txtCD",$ErroImport[12],Cor_Erro);
				gravarLog($matriz[$arquivo_nome],3);
			}
			break;

		default:
			break;

	}





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
			if ( document.formulario.uploadedfilestn.value == "" ) {
				mensagem += '\nSelecione o arquivo para ser importado.';
				validacao = false;
			}

			if ( !validacao ) {
				alert( mensagem );
			}
			return validacao;
		}

</script>