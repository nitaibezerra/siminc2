<?
$enbid = $_SESSION["emenda"]["enbid"];
$ptrid = $_SESSION["emenda"]["ptrid"];

if( $_REQUEST['requisicao'] == 'salvar' ){
	
	$_POST['enbnumprocesso'] = str_replace( array('.', '/', '-'), '', $_POST['enbnumprocesso'] );
	
	$sql = "UPDATE emenda.entidadebeneficiada SET 
			  enbnumprocesso = '{$_POST['enbnumprocesso']}',
			  enbvaldataini = '".formata_data_sql($_POST['enbvaldataini'])."',
			  enbvaldatafim = '".formata_data_sql($_POST['enbvaldatafim'])."',
			  enbdatacertificado = ".(!empty($_POST['enbdatacertificado']) ? "'".formata_data_sql($_POST['enbdatacertificado'])."'" : 'null').",
			  enbcertificado = '{$_POST['enbcertificado']}',
			  enbcertificadovalido = '{$_POST['enbcertificadovalido']}', 
			  enborgaocertificado = '{$_POST['enborgaocertificado']}' 
			WHERE 
			  enbid = ".$_POST['enbid'];

	$db->executar( $sql );
	if( $db->commit() ){
		echo "<script>
				alert('Operação Realizada com Sucesso.');
				window.opener.location.href = window.opener.location;
				window.close();
			</script>";
		exit();
	} else {
		echo "<script>
				alert('Falha na Operação.');
				window.close();
			</script>";
	}
}

if( !empty($ptrid) ) {
	$enbid = $db->pegaUm("SELECT enbid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
	$estadoAtual = pegarEstadoAtual( $ptrid );
}
if( $_REQUEST['requisicao'] == 'check' || !empty($_REQUEST['enbcertificadovalido']) ){
	$check = !empty($_REQUEST['radio']) ? $_REQUEST['radio'] : $_REQUEST['enbcertificadovalido'];
	$sql = "SELECT enbcnpj, enbnome FROM emenda.entidadebeneficiada WHERE enbid = $enbid";
	$arDados = $db->pegaLinha( $sql );
	
	$sql = "SELECT enbnumprocesso, enbvaldataini, enbvaldatafim, enbdatacertificado, enbcertificado, enbcertificadovalido, enborgaocertificado 
			FROM emenda.entidadebeneficiada WHERE enbid = $enbid and enbcertificadovalido = '".$_REQUEST['enbcertificadovalido']."'";
	$arRegistro = $db->pegaLinha( $sql );
	$arRegistro = $arRegistro ? $arRegistro : array();
	extract($arRegistro);
	
	$enbvaldataini = !empty($enbvaldataini) ? formata_data( $enbvaldataini ) : '';
	$enbvaldatafim = !empty($enbvaldatafim) ? formata_data( $enbvaldatafim ) : '';
	$enbdatacertificado = !empty($enbdatacertificado) ? formata_data( $enbdatacertificado ) : '';
	if( !empty($enbnumprocesso) ) $enbnumprocesso = substr($enbnumprocesso, 0,5).'.'.substr($enbnumprocesso, 5,6).'/'.substr($enbnumprocesso, 11,4).'-'.substr($enbnumprocesso, 15);
}
$arOrgao = array(
				array("codigo" => 'MEC', "descricao" => 'Ministério da Educação'),
				array("codigo" => 'MS', "descricao" => 'Ministério da Saúde'),
				array("codigo" => 'MDS', "descricao" => 'Ministério do Desenvolvimento Social e Combate à Fome '),
				);
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
<input type="hidden" id="radio" name="radio" value="">
<input type="hidden" id="enbcertificado" name="enbcertificado" value="<?=$_REQUEST['enbcertificado']; ?>">
<input type="hidden" id="enbid" name="enbid" value="<?=$enbid; ?>">
<input type="hidden" id="requisicao" name="requisicao" value="">
<table id="tblform" class="tabela" width="95%" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:40%;"><b>Certificado está válido?</b></td>
		<td><input type="radio" onclick="varificaCertificadoValido('S');" <?=($check == 'S' ? 'checked="checked"' : '') ?> name="enbcertificadovalido" id="enbcertificadovalido_sim" value="S">Sim
			<input type="radio" onclick="varificaCertificadoValido('N');" <?=($check == 'N' ? 'checked="checked"' : '') ?> name="enbcertificadovalido" id="enbcertificadovalido_nao" value="N">Não</td>
	</tr>
	<?if( !empty( $check ) ){ ?>
	<tr>
		<td style="text-align: center;" colspan="2"><div style="width: 100%; font:12px Courier New; color=black; text-align: justify;"><br><br><div style="text-align: center;">DECLARAÇÃO</div><br><br><br>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, para fins de prova junto ao FNDE quanto à isenção de contrapartida, conforme disposto na Resolução nº 15, de 23 de maio de 2011, 
		sob as penas da lei, que a <?=$arDados['enbnome']; ?>, CNPJ
		nº <?=formatar_cnpj($arDados['enbcnpj']); ?>, em conformidade com os requisitos retratados
		em legislação específica possui:<br><br>
		<?if( $check == 'S' ){ ?>
		* Certificado de entidade beneficente de assistência social, por meio do 
			processo de nº <?=campo_texto( 'enbnumprocesso', 'S', 'S', 'Nº Processo', 30, 20, '#####.######/####-##', '','','','','id="enbnumprocesso"'); ?>, emitido <br>pelo 
			<?=$db->monta_combo('enborgaocertificado', $arOrgao, 'S', 'Selecione...', '', '', '', 350, 'S', 'enborgaocertificado' ) ?>.<br>
			Validade: <?=campo_data2('enbvaldataini', 'S','S','Data Inicial','','','') ?> a <?=campo_data2('enbvaldatafim', 'S','S','Data Final','','','')  ?><br>
		<?} else { ?>
		* Protocolo de pedido de renovação tempestivo em processo de análise,
		recebido na data <?=campo_data2('enbdatacertificado', 'S','S','Data Certificado','','','') ?>, pelo 
			<?=$db->monta_combo('enborgaocertificado', $arOrgao, 'S', 'Selecione...', '', '', '', 350, 'S', 'enborgaocertificado' ) ?>,<br> por meio do
			processo de nº <?=campo_texto( 'enbnumprocesso', 'S', 'S', 'Nº Processo', 30, 20, '#####.######/####-##', '','','','','id="enbnumprocesso"'); ?>.
			Validade do certificado anterior: <br><?=campo_data2('enbvaldataini', 'S','S','Data Inicial','','','') ?> a <?=campo_data2('enbvaldatafim', 'S','S','Data Final','','','')  ?>
		<?} ?>
		<br><br>
		</div></td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: center">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarCertificado('<?=$check; ?>');" <?=verificaPermissaoPerfil('pta', '', $estadoAtual); ?>>
			<input type="button" value="Fechar" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
	<tr>
		<td colspan="2" style="font-size: 12px; color: red; font-weight: bold; text-align: center;">Anexar os documentos referentes ao certificado na “Aba Documentos"</td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
	<?} ?>
	<br>
</table>
</form>
<script type="text/javascript">
function varificaCertificadoValido(check){
	document.getElementById('requisicao').value = 'check';
	document.getElementById('radio').value = check;
	document.getElementById('formulario').submit();
}

function salvarCertificado( check ){
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		if( !validaDataMaior( document.getElementById('enbvaldataini'), document.getElementById('enbvaldatafim') ) ){
			alert("A data inicial não pode ser maior que data final.");
			$('enbvaldataini').focus();
			return false;
		}
		document.getElementById('radio').value = check;
		document.getElementById('requisicao').value = 'salvar';
		$("#formulario").submit();
	}
}
</script>