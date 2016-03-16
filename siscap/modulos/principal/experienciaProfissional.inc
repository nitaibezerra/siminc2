<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];
validaSessao( $facid, 'principal/dadosPessoais' );

if( $_POST['requisicao'] == 'adicionar' ){
	extract( $_POST );

	if( empty($expid) ){
		$sql = "INSERT INTO siscap.experienciaprofissional( facid, exporganizacao, expatividadeexercida, expperiodoini, expperiodofim)
				VALUES ('$facid', '$exporganizacao', '$expatividadeexercida', '$expperiodoini', '$expperiodofim')";

		$db->executar($sql);
	} else {
		$sql = "UPDATE siscap.experienciaprofissional SET
				  facid = '$facid',
				  exporganizacao = '$exporganizacao',
				  expatividadeexercida = '$expatividadeexercida',
				  expperiodoini = '$expperiodoini',
				  expperiodofim = '$expperiodofim'
				WHERE
				  expid = $expid";

		$db->executar($sql);
	}
	if($db->commit()){
//		$db->sucesso( 'principal/outrasInformacoes' );
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

if( $_POST['requisicao'] == 'excluir' && !empty($_POST['expid']) ){

	$sql = "DELETE FROM siscap.experienciaprofissional WHERE expid = ".$_POST['expid'];
	$db->executar( $sql );

	if($db->commit()){
		$db->sucesso( 'principal/experienciaProfissional' );
	} else {
		echo "<script>
				alert('Falha na exclução da formação');
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
monta_titulo( 'Experiência Profissional', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

if( !empty($_GET['expid']) ){
	$arDados = $db->pegaLinha( "SELECT expid, facid, exporganizacao, expatividadeexercida, expperiodoini, expperiodofim FROM siscap.experienciaprofissional WHERE expid = ".$_GET['expid'] );
	$arDados = $arDados ? $arDados : array();
	extract( $arDados );

	$expperiodoini = trim($expperiodoini);
	$expperiodofim = trim($expperiodofim);
}
montaCabecalhoFacilitador($facid);
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="expid" id="expid" value="<?=$expid; ?>">

	<input type="hidden" name="dataAtual" id="dataAtual" value="<?=date( 'd/m/Y' ); ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>Experiência Profissional</b></td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Organização:</td>
			<td>
			<? echo campo_texto('exporganizacao', 'S', $habilita, 'Organização', 50, 250, '', '', 'left', '', 0, 'id=exporganizacao' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Atividades Exercidas:</td>
			<td>
				<?
				echo campo_textarea('expatividadeexercida', 'S', $habilita, '', 80, 5, 500, '', '', '', '', 'Atividades Exercidas');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Período Inicial/Final (MM/AAAA):</td>
			<td>
			<? echo campo_texto('expperiodoini', 'N', $habilita, 'Período Inicial', 10, 7, '', '', 'left', '', 0, 'id=expperiodoini', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ).' - '.
					campo_texto('expperiodofim', 'S', $habilita, 'Período Final', 10, 7, '', '', 'left', '', 0, 'id=expperiodofim', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ); ?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
            <td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarExperiencia();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A';" />
			</td>
		</tr>
	</table>
</form>
<?
if( $habilita != 'N' ){
	$acoes = "'<center><a href=\"siscap.php?modulo=principal/experienciaProfissional&acao=A&expid='|| expid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirFormacao('|| expid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
} else {
	$acoes = "'<center><img src=\"/imagens/alterar_01.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				'<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
}
$sql = "SELECT $acoes as acao,
		  exporganizacao, expatividadeexercida, expperiodoini||' - '||expperiodofim as periodo
		FROM  siscap.experienciaprofissional WHERE facid = '$facid'";

$cabecalho = array("Ações", "Organização", "Atividades Exercidas", "Intervalo de Data");
$db->monta_lista($sql, $cabecalho, 10, 4, 'N','Center','','form', $tamanho, $alinhamento);
?>
<script type="text/javascript">
function excluirFormacao( expid ){
	if(confirm("Deseja realmente excluir está Experiência Profissional?")) {
		$('requisicao').value = 'excluir';
		$('expid').value = expid;
		$('formulario').submit();
	}
}

function salvarExperiencia(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos = "#exporganizacao#expatividadeexercida#expperiodoini#expperiodofim";
	tiposDeCampos 	= "#texto#textarea#texto#texto";

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		var obDt  = new Data();
		var dataAtual = $('dataAtual').value;
		var periodoini = '01/'+$('expperiodoini').value;
		var periodofim = '01/'+$('expperiodofim').value;

		var expperiodoini = $('expperiodoini').value.split('/');
		var expperiodofim = $('expperiodofim').value.split('/');
		if(expperiodoini[0] > 12 ){
			alert( 'O mês do período inicial não pode ser maior que 12' );
			$('expperiodoini').focus();
			return false;
		}
		/*if(expperiodofim[0] > 12 ){
			alert( 'O mês do periodo final não pode ser maior que 12' );
			$('expperiodofim').focus();
			return false;
		}*/
		if( (expperiodoini[0] > expperiodofim[0]) && (expperiodoini[1] == expperiodofim[1]) ){
			alert( 'O mês do período inicial não pode ser maior que o mês do período final' );
			$('expperiodoini').focus();
			return false;
		}
		if( expperiodoini[1] > expperiodofim[1] ){
			alert( 'O ano do período inicial não pode ser maior que o ano do periodo final' );
			$('expperiodofim').focus();
			return false;
		}

		if(expperiodoini[1].length < 4){
			alert( 'O ano do período inicial está no formato incorreto' );
			$('expperiodoini').focus();
			return false;
		}
		if(expperiodofim[1].length < 4){
			alert( 'O ano do período final está no formato incorreto' );
			$('expperiodofim').focus();
			return false;
		}
		if( obDt.comparaData( periodoini, dataAtual, '>' ) ){
			alert('O Período Inicial/Final não pode ser maior que a data atual!');
			$('expperiodoini').focus();
			return false;
		}
		if( obDt.comparaData( periodofim, dataAtual, '>' ) ){
			alert('O Período Inicial/Final não pode ser maior que a data atual!');
			$('expperiodofim').focus();
			return false;
		}

		$('requisicao').value = 'adicionar';
		$('formulario').submit();
	}
}

function formataPeriodo( id, event ){
	if (event.keyCode)
		var key = event.keyCode;
    else
	    var key = event.which;

	//8=backspace, 9=tab, 46=delete, 127=delete, 37-40=arrow keys
	if (key!=9 && key!=8 && key!=46 && (key<37 || key>40)){
		if(id.value.length == 2 ){
			if( Number(id.value) ){
				id.value = id.value + '/';
			} else {
				id.value = '';
			}
		} else {
			if( !Number(id.value) ){
				//id.value = '';
			}
		}
		return id.value;
	}
}
</script>