<?

include APPRAIZ."includes/cabecalho.inc";
print '<br>';
//echo "teste";
//include "config.inc";
//include APPRAIZ . "includes/classes_simec.inc";
//include APPRAIZ . "includes/funcoes.inc";
//include "includes/cabecalho.inc";
$pathimport = APPRAIZ . 'financeiro/arquivos/siafi/';
$slcano = $_REQUEST['slcano'];
$slcimport = $_REQUEST['slcimport'];

define('Cor_Aguardando',"#0000FF");
define('Cor_Executando',"#FF9900");
define('Cor_Erro',"#FF0000");
define('Cor_Sucesso',"#336633");



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
		<br/>
		<form method="POST" name="formulario">
			<? monta_titulo('Comunicação com o SIAFI','Selecione os parâmetros da importação');?>
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacao/importacaosiafi"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Ano:</td>
					<td>
					<select name="slcano">
					<option value="" selected>selecione um ano</option>
					  <?

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
					<p><input type="checkbox" name="slcimport[]" value="Cambio">Cambio</p>
					<p><input type="checkbox" name="slcimport[]" value="Fonte de Recurso">Fonte de Recurso</p>
					<p><input type="checkbox" name="slcimport[]" value="Nota de Empenho">Nota de Empenho</p>
					<p><input type="checkbox" name="slcimport[]" value="Plano de Contas">Plano de Contas</p>
					<p><input type="checkbox" name="slcimport[]" value="PTRES">PTRES</p>
					<p><input type="checkbox" name="slcimport[]" value="Saldo Contabil">Saldo Contabil</p>
					<p><input type="checkbox" name="slcimport[]" value="Unidade Gestora">Unidade Gestora</p>
					<p><input type="checkbox" name="slcimport[]" value="Vinculacao Pagamento">Vinculação Pagamento</p>
					<p><input type="checkbox" name="slcimport[]" value="Gestao">Gestão</p>

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
		<form method="POST" name="formul">
			<? monta_titulo('Comunicação com o SIAFI','Executando a(s) Importação(ões)');?>
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="sistema/comunica/importacao/importacaosiafi"/>
			<input type=hidden name="importar" value="0"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Ano de Referência:</td>
					<td><?echo $slcano;?></td>
			 	</tr>
				<input type="hidden" name="sisfinalidade_selc" value=""/>
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
            	<tr><td align = 'right' class="subtitulodireita" width="150px">Importação do Arquivo Atual<td><input type="Text" align="middle" readonly style="border: none; background-color:#f5f5f5;" name="txtarquivo" >
            	<? $objBar1->draw(); ?></td></tr>
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
	$ano_referencia = $slcano;
	include "importacao.inc";


	foreach ($slcimport as $lista_import)
	{
		$objBar0->setProgress(0);

		switch ($lista_import){
		case 'Cambio':

			ImprimeStatus('formul.txtCambio',"Executando importação" ,Cor_Executando);
			include('importcambio.inc');
			$resultado = RodaCambio();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtCambio',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtCambio',$sucessoImport,Cor_Sucesso);
			}

			break;
		case 'Fonte de Recurso':
			ImprimeStatus('formul.txtFonte_de_Recurso',"Executando importação" ,Cor_Executando);
			include('importfr.inc');
				$resultado = RodaFR();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtFonte_de_Recurso',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtFonte_de_Recurso',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'Nota de Empenho':
			ImprimeStatus('formul.txtNota_de_Empenho',"Executando importação" ,Cor_Executando);
			include('importnoe.inc');
				$resultado = RodaNOE();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtNota_de_Empenho',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtNota_de_Empenho',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'Plano de Contas':
			ImprimeStatus('formul.txtPlano_de_Contas',"Executando importação" ,Cor_Executando);
			include('importpc.inc');
			$resultado = RodaPC();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtPlano_de_Contas',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtPlano_de_Contas',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'PTRES':
			ImprimeStatus('formul.txtPTRES',"Executando importação" ,Cor_Executando);
			include('importptres.inc');
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
			include('importsaldo.inc');
			$resultado = RodaSALDO();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtSaldo_Contabil',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtSaldo_Contabil',$sucessoImport,Cor_Sucesso);
			}
			break;
		case 'Unidade Gestora':
			ImprimeStatus('formul.txtUnidade_Gestora',"Executando importação" ,Cor_Executando);
			include('importug.inc');
			$resultado = RodaUG();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtUnidade_Gestora',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtUnidade_Gestora',$sucessoImport,Cor_Sucesso);
			}
			break;

		case 'Vinculacao Pagamento':
			ImprimeStatus('formul.txtvinculacao_pagamento',"Executando importação" ,Cor_Executando);
			include('importvp.inc');
			$resultado = RodaVP();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtVinculacao_Pagamento',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtVinculacao_Pagamento',$sucessoImport,Cor_Sucesso);
			}
			break;

		case 'Gestao':
			ImprimeStatus('formul.txtGestao',"Executando importação" ,Cor_Executando);
			include('importgst.inc');
			$resultado = RodaGST();
			if ($resultado != $sucessoImport)
			{
				ImprimeStatus('formul.txtGestao',$resultado,Cor_Erro);
			}else
			{
				ImprimeStatus('formul.txtGestao',$sucessoImport,Cor_Sucesso);
			}
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