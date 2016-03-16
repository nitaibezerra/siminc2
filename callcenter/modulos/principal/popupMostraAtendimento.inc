<?php
include_once( APPRAIZ."callcenter/classes/Ligacao.class.inc");
$obLigacao = new Ligacao();

if( $_GET['ligid'] ){
	$arLigacao = $obLigacao->carregaDadosLigacao( $_GET['ligid'] );
	extract($arLigacao);
	
	$ligdataprovidenciamec = formata_data( $ligdataprovidenciamec );
	$ligdataprovidenciacontato = formata_data($ligdataprovidenciacontato);
	$ligdata = formata_data($ligdata);
}
?>
<head>
	<meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
    <title>Atendimento</title>    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body>
	<form name="formulario" id="formulario" method="post" action="" >
		<input type="hidden" name="requisicao" id="requisicao" value="salvar"  />
		<input type="hidden" name="temid" id="temid" value="<?=$_GET['temid']; ?>"  />
		<input type="hidden" name="proid" id="proid" value="<?=$_GET['proid']; ?>"  />
		<input type="hidden" name="programa" value="<?php echo $programaAtendimento; ?>"  />
		<input type="hidden" name="tlgid" id="tlgid" value="<?=$_GET['tlgid']; ?>" />
		<input type="hidden" name="ligid" id="ligid" value="<?=$_GET['ligid']; ?>" />
		<table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela">
			<tr>
				<td colspan="3" class="SubTituloCentro">Atendimento ao Cliente</td>
			</tr>			
			<tr>
				<td class="SubTituloDireita" style="width: 25%">Tipo de Ligação:</td>
				<td>
					<?php echo $tlgdescricao;  ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 25%">Telefone de Contato:</td>
				<td>
					<?php echo "(".trim($ligdddtelefone).") ". substr($lignumerotelefone,0 ,4).'-'.substr($lignumerotelefone,4 );  ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome do Contato:</td>
				<td colspan="2">
					<?php echo $lignomecontato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Cargo do Contato:</td>
				<td colspan="2">
					<?php echo $ligcargocontato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Dados da Conversa:</td>
				<td colspan="2">
					<?php echo $ligtexto; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Providência do Contato:</td>
				<td colspan="2">
					<?php echo $ligprovidenciacontato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Data da providência do Contato:</td>
				<td colspan="2">
					<?php echo $ligdataprovidenciacontato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Providência MEC:</td>
				<td colspan="2">
					<?php echo $ligprovidenciamec; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Data da providência do MEC:</td>
				<td colspan="2">
					<?php echo $ligdataprovidenciamec; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Usuário:</td>
				<td colspan="2">
					<?php echo $usunome; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Data:</td>
				<td colspan="2">
					<?php echo $ligdata; ?>
				</td>
			</tr>
			<tr>
				<td colspan="3" style="text-align: center; background-color: #E9E9E9;" >
					<input type="button" onclick="window.close();" value="Fechar" /> </td>
			</tr>
		</table>
	</form>
</body>