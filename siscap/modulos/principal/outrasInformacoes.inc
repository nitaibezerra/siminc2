<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];

validaSessao( $facid, 'principal/dadosPessoais' );

if( $_POST['requisicao'] == 'salvar' ){
	extract( $_POST );

	$sql = "UPDATE siscap.facilitador SET
			  facmincurriculo = '$facmincurriculo',
			  facconhecimento = '$facconhecimento',
			  facareaatuacao = '$facareaatuacao',
			  facpublicacoes = '$facpublicacoes',
			  faccurextcurriculares = '$faccurextcurriculares'
			WHERE
			  facid = $facid";

	$db->executar( $sql );

	if( $db->commit() ){
//		echo "<script>
//				alert('Cadastro Concluído');
//				window.location.href='siscap.php?modulo=principal/listaFacilitador&acao=A';
//			</script>";
//		exit();
        echo "<script>
                alert('Operação realizada com sucesso.');
             </script>";
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/experienciaProfissional&acao=A';
			 </script>";
		exit();
	}
}
$visualiza = $_SESSION['siscap']['visualizar'];
$habilita = $visualiza == 1 ? 'N' : 'S';
$habil = $habilita == 'N'  ? 'disabled="disabled"' : '';

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Outras Informações', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$sql = "SELECT
			facmincurriculo, facconhecimento, facareaatuacao, facpublicacoes, faccurextcurriculares
		FROM siscap.facilitador WHERE facid = ".$facid;

$arDados = $db->pegaLinha( $sql );
$arDados = $arDados ? $arDados : array();
extract($arDados);

montaCabecalhoFacilitador($facid);
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>Outras Informações</b></td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Cursos Extra Curriculares:</td>
			<td>
				<?
				echo campo_textarea('faccurextcurriculares', 'N', $habilita, '', 85, 5, 500, '', '', '', '', 'Cursos Extra Curriculares');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Publicações:</td>
			<td>
				<?
				echo campo_textarea('facpublicacoes', 'N', $habilita, '', 85, 5, 500, '', '', '', '', 'Publicações');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área de preferência para atuação:</td>
			<td>
				<?
				echo campo_textarea("facareaatuacao", 'N', $habilita, '', 85, 5, 800, '', '', '', '', "Área de preferência para atuação");
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Mini-Currículo:</td>
			<td>
				<?
				echo campo_textarea("facmincurriculo", 'S', $habilita, '', 85, 5, 800, '', '', '', '', "Mini-Currículo");
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Conhecimentos:</td>
			<td>
				<?
				echo campo_textarea("facconhecimento", 'N', $habilita, '', 85, 5, 500, '', '', '', '', "Conhecimentos");
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
            <td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarInformacoes();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

function salvarInformacoes(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	var faccurextcurriculares 	= $('faccurextcurriculares').value;
	var facpublicacoes 			= $('facpublicacoes').value;
	var facareaatuacao 			= $('facareaatuacao').value;
	var facmincurriculo 		= $('facmincurriculo').value;
	var facconhecimento 		= $('facconhecimento').value;

	if( facmincurriculo == '' ){
		alert('É obrigatório o preenchimento do campo Mini-Currículo.');
		$('facmincurriculo').focus();
		return false;
	}
	campos = "#faccurextcurriculares#facpublicacoes#facareaatuacao#facmincurriculo#facconhecimento";
	tiposDeCampos 	= "#textarea#textarea#textarea#textarea#textarea";

	//if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		var fieldName = "#faccurextcurriculares#facpublicacoes#facareaatuacao#facmincurriculo#facconhecimento";
		var labelCampo = "#Cursos Extra Curriculares#Publicações#Área de preferência para atuação#Mini-Currículo#Conhecimentos";

		if(naoSomenteNumeros('formulario', fieldName, labelCampo)){
			$('requisicao').value = 'salvar';
			$('formulario').submit();
		}
	//}
}
</script>