<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];
validaSessao( $facid, 'principal/dadosPessoais' );

if( $_POST['requisicao'] == 'adicionar' ){
	extract( $_POST );

	if( $exdtema || $exdeventorealizado || $exdperiodoini ||$exdperiodofim || $exdcargahoraria || $exdinstituicao ){
		$exdcargahoraria = $exdcargahoraria ? $exdcargahoraria : 0;
		if( empty($exdid) ){
			$sql = "INSERT INTO siscap.experienciadidatica( facid, exdtema, exdeventorealizado, exdperiodoini, exdperiodofim, exdcargahoraria, exdinstituicao)
					VALUES ('$facid', '$exdtema', '$exdeventorealizado', '$exdperiodoini', '$exdperiodofim', $exdcargahoraria, '$exdinstituicao' )";

			$db->executar($sql);
		} else {
			$sql = "UPDATE siscap.experienciadidatica SET
					  facid = '$facid',
					  exdtema = '$exdtema',
					  exdeventorealizado = '$exdeventorealizado',
					  exdperiodoini = '$exdperiodoini',
					  exdperiodofim = '$exdperiodofim',
					  exdcargahoraria = $exdcargahoraria,
					  exdinstituicao = '$exdinstituicao'
					WHERE
					  exdid = $exdid";

			$db->executar($sql);
		}
		if($db->commit()){
//			$db->sucesso( 'principal/experienciaProfissional' );
            echo "<script>
                alert('Operação realizada com sucesso.');
             </script>";
		} else {
			echo "<script>
					alert('Falha na Operação');
					window.location.href = 'siscap.php?modulo=principal/experienciaDidatica&acao=A';
				 </script>";
			exit();
		}
	} else {
		echo "<script>
					alert('Registro em branco não pode ser gravado.');
					window.location.href = 'siscap.php?modulo=principal/experienciaDidatica&acao=A';
				 </script>";
	}
}

if( $_POST['requisicao'] == 'excluir' && !empty($_POST['exdid']) ){

	$sql = "DELETE FROM siscap.experienciadidatica WHERE exdid = ".$_POST['exdid'];
	$db->executar( $sql );

	if($db->commit()){
		$db->sucesso( 'principal/experienciaDidatica' );
	} else {
		echo "<script>
				alert('Falha na exclução da formação');
				window.location.href = 'siscap.php?modulo=principal/experienciaDidatica&acao=A';
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
monta_titulo( 'Experiência Didática', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

if( !empty($_GET['exdid']) ){
	$arDados = $db->pegaLinha( "SELECT exdid, facid, exdtema, exdeventorealizado, exdperiodoini, exdperiodofim, exdcargahoraria, exdinstituicao FROM siscap.experienciadidatica WHERE exdid = ".$_GET['exdid'] );
	$arDados = $arDados ? $arDados : array();
	extract( $arDados );

	$exdperiodoini = trim($exdperiodoini);
	$exdperiodofim = trim($exdperiodofim);
}
montaCabecalhoFacilitador($facid);
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="exdid" id="exdid" value="<?=$exdid; ?>">

	<input type="hidden" name="dataAtual" id="dataAtual" value="<?=date( 'd/m/Y' ); ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>Experiência didática como Instrutor(a)/Professor(a)</b></td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Tema:</td>
			<td>
			<? echo campo_texto('exdtema', 'N', $habilita, 'Tema', 50, 20, '', '', 'left', '', 0, 'id=exdtema' ); ?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Instituição:</td>
			<td>
			<? echo campo_texto('exdinstituicao', 'N', $habilita, 'Instituição', 50, 100, '', '', 'left', '', 0, 'id=exdinstituicao' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Eventos Realizados:</td>
			<td>
				<?
				echo campo_textarea('exdeventorealizado', 'N', $habilita, '', 80, 5, 500, '', '', '', '', 'Eventos Realizados');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Período (MM/AAAA):</td>
			<td>
			<? echo campo_texto('exdperiodoini', 'N', $habilita, 'Periodo Inicial', 10, 7, '', '', 'left', '', 0, 'id=exdperiodoini', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ).' - '.
					campo_texto('exdperiodofim', 'N', $habilita, 'Periodo Final', 10, 7, '', '', 'left', '', 0, 'id=exdperiodofim', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ); ?>
			</td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Carga Horária:</td>
			<td>
			<? echo campo_texto('exdcargahoraria', 'N', $habilita, 'Carga Horária', 5, 3, '[#]', '', 'left', '', 0, 'id=exdcargahoraria' ); ?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
            <td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarDidatica();" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A';" />
			</td>
		</tr>
	</table>
</form>
<?
if( $habilita != 'N' ){
	$acoes = "'<center><a href=\"siscap.php?modulo=principal/experienciaDidatica&acao=A&exdid='|| exdid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
    				'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirFormacao('|| exdid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
} else {
	$acoes = "'<center><img src=\"/imagens/alterar_01.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
    				'<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
}
$sql = "SELECT $acoes as acao,
			exdtema, exdinstituicao, exdeventorealizado, exdperiodoini||' - '||exdperiodofim as periodo
		FROM  siscap.experienciadidatica WHERE facid = '$facid'";

$cabecalho = array("Ações", "Tema", "Instituição", "Eventos Realizados", "Período Inicial/Final");
$db->monta_lista($sql, $cabecalho, 10, 4, 'N','Center','','form', $tamanho, $alinhamento);
?>
<script type="text/javascript">
function excluirFormacao( exdid ){
	if(confirm("Deseja realmente excluir está Experiência Didática?")) {
		$('requisicao').value = 'excluir';
		$('exdid').value = exdid;
		$('formulario').submit();
	}
}

function salvarDidatica(){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

//	campos = "#exdtema#exdeventorealizado#exdperiodoini#exdperiodofim#exdcargahoraria";
//	tiposDeCampos 	= "#texto#textarea#texto#texto#texto";

//	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		var obDt  = new Data();
		var exdperiodoini = $('exdperiodoini').value.split('/');
		var exdperiodofim = $('exdperiodofim').value.split('/');
		var dataAtual = $('dataAtual').value;
		var periodoini = '01/'+$('exdperiodoini').value;
		var periodofim = '01/'+$('exdperiodofim').value;

		if( exdperiodoini != '' && exdperiodofim != '' ){

			if(exdperiodoini[0] > 12 ){
				alert( 'O mês do periodo inicial não pode ser maior que 12' );
				$('exdperiodoini').focus();
				return false;
			}
			if(exdperiodofim[0] > 12 ){
				alert( 'O mês do periodo final não pode ser maior que 12' );
				$('exdperiodofim').focus();
				return false;
			}
			if( (exdperiodoini[0] > exdperiodofim[0]) && (exdperiodoini[1] == exdperiodofim[1]) ){
				alert( 'O mês do periodo inicial não pode ser maior que o mês do periodo final' );
				$('exdperiodoini').focus();
				return false;
			}
			if( exdperiodoini[1] > exdperiodofim[1] ){
				alert( 'O ano do periodo inicial não pode ser maior que o ano do periodo final' );
				$('exdperiodofim').focus();
				return false;
			}

			if(exdperiodoini[1].length < 4){
				alert( 'O ano do periodo inicial está no formato incorreto' );
				$('exdperiodoini').focus();
				return false;
			}
			if(exdperiodofim[1].length < 4){
				alert( 'O ano do periodo final está no formato incorreto' );
				$('exdperiodofim').focus();
				return false;
			}

			if( obDt.comparaData( periodoini, dataAtual, '>' ) ){
				alert('O Período Inicial/Final não pode ser maior que a data atual!');
				$('exdperiodoini').focus();
				return false;
			}
			if( obDt.comparaData( periodofim, dataAtual, '>' ) ){
				alert('O Período Inicial/Final não pode ser maior que a data atual!');
				$('exdperiodofim').focus();
				return false;
			}
		}
		$('requisicao').value = 'adicionar';
		$('formulario').submit();
//	}
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