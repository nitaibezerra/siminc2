<?php
include APPRAIZ . "par/modulos/execucaoprocessosfinanceiros/classes/processos.class.inc";

if($_REQUEST['processo']){
	$processo = $_REQUEST['processo'];
	$dadosProcesso = new Processos();
	$dadosProcesso->carregarPorId($processo);
	$probanco = $dadosProcesso->probanco;
	$proagencia = $dadosProcesso->proagencia;
	$proconta = $dadosProcesso->proconta;
	$prosequencialcontasigef = $dadosProcesso->prosequencialcontasigef;
	$prodocumenta = $dadosProcesso->prodocumenta;
	$procnpj = $dadosProcesso->procnpj;	
}else{
	alert('Processo não encontrado!');
	die();
}

$dadosCabecalho = $dadosProcesso->carregaDadosProcessoPorMuncodUF($dadosProcesso->muncod);

monta_titulo( $titulo_modulo, 'Tela de edição dos dados do processo.' );

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<form name="form" id="form" method="post" action="" >
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<thead>
					<tr>
						<td width="30%"></td>
						<td width="70%"></td>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td class="subtituloDireita" style="font-weight: bold;">Número do Processo:</td>
						<td><?php echo mascaraglobal($dadosProcesso->pronumeroprocesso, "#####.######/####-##"); ?> </td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="font-weight: bold;">Estado:</td>
						<td><?php echo $dadosCabecalho['estuf']; ?></td>
					</tr>
					<tr>
						<td class="subtituloDireita" style="font-weight: bold;">Município:</td>
						<td><?php echo $dadosCabecalho['mundescricao']; ?></td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Esfera:</td>
						<td><?php echo $dadosCabecalho['esfera']; ?></td>
					</tr>
					<tr>
						<td colspan="2" class="subtituloCentro"  style="font-weight: bold; background-color: #dcdcdc;">Informações do Processo  <?php echo mascaraglobal($dadosProcesso->pronumeroprocesso, "#####.######/####-##"); ?> :</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Banco:</td>
						<td>
							<? echo campo_texto('probanco', 'N', 'S', '', 15, 3, '####', '', 'right', '', 0, 'id="probanco" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Agência:</td>
						<td>
							<? echo campo_texto('proagencia', 'N', 'S', '', 15, 4, '', '', 'right', '', 0, 'id="proagencia" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Conta:</td>
						<td>
							<? echo campo_texto('proconta', 'N', 'S', '', 20, 30, '', '', 'right', '', 0, 'id="proconta" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Sequencial da Conta:</td>
						<td>
							<? echo campo_texto('prosequencialcontasigef', 'N', 'S', '', 20, 25, '', '', 'right', '', 0, 'id="prosequencialcontasigef" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">Nº do Processos no sistema de Documenta:</td>
						<td>
							<? echo campo_texto('prodocumenta', 'N', 'S', '', 20, 25, '', '', 'right', '', 0, 'id="prodocumenta" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita"  style="font-weight: bold;">CNPJ da Entidade:</td>
						<td>
							<? echo campo_texto('procnpj', 'N', 'S', '', 20, 25, '', '', 'right', '', 0, 'id="procnpj" style="text-align:right;" '); ?>
						</td>
					</tr>
					<tr>
						<td colspan="2" class="subtituloCentro"  style="font-weight: bold; background-color: #dcdcdc;">
							<input type="button" name="salvar" value="Salvar" onclick="valida( this.form )">&nbsp;
							<input type="button" name="limpar" value="Limpar" onclick="limpa()">
						</td>
					</tr>
				</tbody>
			</table>
		</form>
	</body>
</html>