<?php
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];

validaSessao( $facid, 'principal/dadosPessoais' );

if( $_POST['requisicao'] == 'salvar' ){

		$concep 			= str_replace(array(".", "-"), "", $_POST['endcep1']);
		$concidade 			= $_POST['mundescricao'];
		$conestado 			= $_POST['estuf'];
		$conbairro 			= $_POST['endbai'];
		$conlogradouro 		= $_POST['endlog'];
		$concomplemento 	= $_POST['endcomp'];
		$contelecomercial 	= $_POST['condddcomercial'].str_replace("-", "", $_POST['contelecomercial']);
		$conteleresidencial = $_POST['condddresidencial'].str_replace("-", "", $_POST['conteleresidencial']);
		$contelecelular		= $_POST['condddcelular'].str_replace("-", "", $_POST['contelecelular']);

	if( $_POST['conid'] ){
		$conid = $_POST['conid'];
		$sql = "UPDATE siscap.contato
				   SET concep='".$concep."', concidade='".$concidade."', conestado='".$conestado."', conbairro='".$conbairro."', conlogradouro='".$conlogradouro."',
				       concomplemento='".$concomplemento."', contelecomercial = '$contelecomercial', conteleresidencial = '$conteleresidencial', contelecelular = '$contelecelular'
				 WHERE conid = ".$_POST['conid'];

		$db->executar( $sql );
	} else {
		$sql = "INSERT INTO siscap.contato(
			            concep, concidade, conestado, conbairro, conlogradouro, concomplemento, facid, contelecomercial, conteleresidencial, contelecelular)
			    VALUES ('".$concep."', '".$concidade."', '".$conestado."', '".$conbairro."', '".$conlogradouro."', '".$concomplemento."', '".$facid."', '$contelecomercial', '$conteleresidencial', '$contelecelular')";

		$db->executar( $sql );
	}
	if($db->commit()){
//		$db->sucesso( 'principal/formacao' );
        echo "<script>
            alert('Operação realizada com sucesso.');
         </script>";
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/contato&acao=A';
			 </script>";
		exit();
	}

}

$sql = "SELECT conid, concep, concidade, conestado, conbairro, conlogradouro, concomplemento, facid, contelecomercial, conteleresidencial, contelecelular FROM siscap.contato WHERE facid = '$facid'";
$con = $db->pegaLinha( $sql );

$con['concep'] = trim($con['concep']);
if( !empty( $con['concep'] ) )
	$con['concep'] = substr( $con['concep'], 0, 5).'-'.substr( $con['concep'], 5);

$con['contelecomercial'] = trim($con['contelecomercial']);
if( !empty($con['contelecomercial']) ){
	$condddcomercial = substr( $con['contelecomercial'], 0, 2);
	$contelecomercial = substr( $con['contelecomercial'], 2, 4).'-'.substr( $con['contelecomercial'], 6);
}
$con['conteleresidencial'] = trim($con['conteleresidencial']);
if( !empty($con['conteleresidencial']) ){
	$condddresidencial = substr( $con['conteleresidencial'], 0, 2);
	$conteleresidencial = substr( $con['conteleresidencial'], 2, 4).'-'.substr( $con['conteleresidencial'], 6);
}
if( !empty($con['contelecelular']) ){
	$condddcelular = substr( $con['contelecelular'], 0, 2);
	$contelecelular = substr( $con['contelecelular'], 2, 4).'-'.substr( $con['contelecelular'], 6);
}

$visualiza = $_SESSION['siscap']['visualizar'];
$habilita = $visualiza == 1 ? 'N' : 'S';
$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Contato', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );
montaCabecalhoFacilitador($facid);
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="conid" id="conid" value="<?=$con['conid']; ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="15%" class="subtitulodireita">CEP:</td>
			<td >
				<input type="text" name="endcep1" title="CEP" <?=$habil; ?> onkeyup="this.value=mascaraglobal('##.###-###', this.value);" onblur="getEnderecoPeloCEP(this.value,'1'); " class="CampoEstilo" id="endcep1" value="<?=$con['concep'] ?>" size="13" maxlength="10" /><img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Cidade:</td>
			<td>
				<input type="text" title="Cidade" name="mundescricao" <?=$habil; ?> class="CampoEstilo" id="mundescricao1" value="<?=$con['concidade'] ?>" onkeypress="return somenteLetras( event )" size="20" maxlength="100" /><img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Estado:</td>
			<td>
				<input type="text" title="Estado" name="estuf" <?=$habil; ?> class="CampoEstilo" id="estuf1" maxlength="2" style="text-transform: uppercase;" onkeypress="return somenteLetras( event )" value="<?=$con['conestado'] ?>" size="10" /><img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Bairro:</td>
			<td>
				<input type="text" title="Bairro" name="endbai" <?=$habil; ?> class="CampoEstilo" id="endbai1" value="<?=$con['conbairro'] ?>" maxlength="100" /><img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Logradouro:</td>
			<td>
				<input type="text" title="Logradouro" name="endlog" <?=$habil; ?> class="CampoEstilo" id="endlog1" value="<?=$con['conlogradouro'] ?>" size="48" maxlength="100" /><img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Complemento:</td>
			<td>
				<input type="text" title="Complemento" name="endcomp" <?=$habil; ?> class="CampoEstilo" id="endcomp" value="<?=$con['concomplemento'] ?>" size="48" maxlength="20" />
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">DDD/Telefone Comercial:</td>
			<td>
				<?php
					echo campo_texto('condddcomercial', 'N', $habilita, 'DDD Comercial', 3, 2, '##', '', '', '', 0, 'id=condddcomercial', "pulaCampo('formulario', '2', 'condddcomercial', 'contelecomercial', event)" ).'&nbsp;&nbsp;&nbsp;&nbsp;';
					echo campo_texto('contelecomercial', 'N', $habilita, 'Número Comercial', 17, 9, '####-####', '', '', '', 0, 'id=contelecomercial', '', '', 'validaTelefone( this )' );
				 ?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">DDD/Telefone Residencial:</td>
			<td>
				<?php
					echo campo_texto('condddresidencial', 'N', $habilita, 'DDD Residencial', 3, 2, '##', '', '', '', 0, 'id=condddresidencial', "pulaCampo('formulario', '2', 'condddresidencial', 'conteleresidencial', event)" ).'&nbsp;&nbsp;&nbsp;&nbsp;';
					echo campo_texto('conteleresidencial', 'N', $habilita, 'Número Residencial', 17, 9, '####-####', '', '', '', 0, 'id=conteleresidencial', '', '', 'validaTelefone( this )' );
				 ?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">DDD/Telefone Celular:</td>
			<td>
				<?php
					echo campo_texto('condddcelular', 'S', $habilita, 'DDD Celular', 3, 2, '##', '', '', '', 0, 'id=condddcelular', "pulaCampo('formulario', '2', 'condddcelular', 'contelecelular', event)" );
					echo campo_texto('contelecelular', 'S', $habilita, 'Número Celular', 17, 9, '####-####', '', '', '', 0, 'id=contelecelular', '', '', 'validaTelefone( this )' );
				 ?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
            <td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarContato();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<script src="../includes/entidadesn.js"></script>
<script type="text/javascript">

form		=	$("formulario");
btSalvar	=	$("btSalvar");

function validaTelefone( dados ){
	tel = dados.value;
	id = dados.id;

	numero = tel.substring( 0, 4 );

	if( numero == '0000' || numero == '1111' || numero == '2222' || numero == '3333' || numero == '4444' || numero == '5555' || numero == '6666' || numero == '7777' || numero == '8888' || numero == '9999' ){
		alert( 'Telefone inválido.' );
		$(id).value = "";
	}
}

function salvarContato(){
	btSalvar.disabled = true;

	var camposObri 		= "#endcep1#mundescricao#estuf#endbai#endlog#condddcelular#contelecelular";
	var tiposCamposObri	= '#texto#texto#texto#texto#texto#texto#texto';


	if(!validaForm('formulario',camposObri,tiposCamposObri,false)){
		btSalvar.disabled  = false;
		return false;
	}

	var fieldName = "#mundescricao#estuf#endbai#endlog";
	var labelCampo = "#Cidade#Estado#Bairro#Logradouro";

	if(naoSomenteNumeros('formulario', fieldName, labelCampo)){
		document.getElementById('requisicao').value = 'salvar';
		form.submit();
	}
	btSalvar.disabled  = false;
	return false;
}
</script>