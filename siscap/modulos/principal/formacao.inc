<?php
$_SESSION['siscap']['facid'] =  empty($_SESSION['siscap']['facid']) ? $_REQUEST['facid'] : $_SESSION['siscap']['facid'];
$facid = $_SESSION['siscap']['facid'];

validaSessao( $facid, 'principal/dadosPessoais' );

if( $_POST['requisicao'] == 'salvar' ){
	extract( $_POST );

	/*$forperiodoini = '01/'.$forperiodoini;
	$forperiodofim = '01/'.$forperiodofim;

	$forperiodoini = "'".formata_data_sql($forperiodoini)."'";
	$forperiodofim = "'".formata_data_sql($forperiodofim)."'";*/

	if( empty($forid) ){
		$sql = "INSERT INTO siscap.formacao( facid, forcurso, fornivel, forinstituicao, forperiodoini, forperiodofim)
				VALUES ('$facid', '$forcurso', '$fornivel', '$forinstituicao', '$forperiodoini', '$forperiodofim')";
		$db->executar($sql);
	} else {
		$sql = "UPDATE siscap.formacao SET
				  facid = '$facid',
				  forcurso = '$forcurso',
				  fornivel = '$fornivel',
				  forinstituicao = '$forinstituicao',
				  forperiodoini = '$forperiodoini',
				  forperiodofim = '$forperiodofim'
				WHERE
				  forid = $forid";

		$db->executar($sql);
	}
	if($db->commit()){
		if( $_POST['tipo'] == 'salvar' ){
//			$db->sucesso( 'principal/experienciaDidatica' );
            echo "<script>
                alert('Operação realizada com sucesso.');
             </script>";
		} else {
			$db->sucesso( 'principal/formacao' );
		}
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/formacao&acao=A';
			 </script>";
		exit();
	}
}

if( $_POST['requisicao'] == 'excluir' && !empty($_POST['forid']) ){

	$sql = "DELETE FROM siscap.formacao WHERE forid = ".$_POST['forid'];
	$db->executar( $sql );

	if($db->commit()){
		$db->sucesso( 'principal/formacao' );
	} else {
		echo "<script>
				alert('Falha na exclução da formação');
				window.location.href = 'siscap.php?modulo=principal/formacao&acao=A';
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
monta_titulo( 'Formação', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

if( !empty($_GET['forid']) ){
	$arDados = $db->pegaLinha( "SELECT forid, facid, forcurso, fornivel, forinstituicao, forperiodoini, forperiodofim
								FROM siscap.formacao WHERE forid = ".$_GET['forid'] );
	$arDados = $arDados ? $arDados : array();
	extract( $arDados );

	$forperiodoini = trim( $forperiodoini );
	$forperiodofim = trim( $forperiodofim );
}
montaCabecalhoFacilitador($facid);
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="tipo" id="tipo" value="">
	<input type="hidden" name="forid" id="forid" value="<?=$forid; ?>">
	<input type="hidden" name="dataAtual" id="dataAtual" value="<?=date( 'd/m/Y' ); ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>Cursos Regulares</b></td>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">Curso:</td>
			<td>
			<? echo campo_texto('forcurso', 'S', $habilita, 'Curso', 50, 100, '', '', 'left', '', 0, 'id=forcurso' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nível:</td>
			<td>
				<?
				$arNivel = array(
								array("codigo" => 'ME', "descricao" => 'Médio' ),
								array("codigo" => 'SU', "descricao" => 'Superior' ),
								array("codigo" => 'PG', "descricao" => 'Pós Graduação' ),
								array("codigo" => 'MD', "descricao" => 'Mestrado' ),
								array("codigo" => 'DO', "descricao" => 'Doutorado' )
							);

				$db->monta_combo("fornivel", $arNivel, $habilita, 'Selecione...', '', '', '', '150', 'S', 'fornivel', '', '', 'Nível' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Instituição:</td>
			<td>
			<? echo campo_texto('forinstituicao', 'S', $habilita, 'Instituição', 50, 100, '', '', 'left', '', 0, 'id=forinstituicao' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Período Inicial/Final (MM/AAAA):</td>
			<td>
			<? echo campo_texto('forperiodoini', 'N', $habilita, 'Período Inicial', 10, 7, '', '', 'left', '', 0, 'id=forperiodoini', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ).' - '.
					campo_texto('forperiodofim', 'S', $habilita, 'Período Final', 10, 7, '', '', 'left', '', 0, 'id=forperiodofim', 'formataPeriodo(this, event)', '', 'formataPeriodo(this, event)' ); ?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
            <td style="text-align: left">
				<input type="button" id="btSalvar" value="Salvar" <?=$habil; ?> onclick="salvarFormacao('salvar');" />
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaFacilitador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<?
if( $habilita != 'N' ){
	$acoes = "'<center><a href=\"siscap.php?modulo=principal/formacao&acao=A&forid='|| forid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				'<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirFormacao('|| forid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
} else {
	$acoes = "'<center><img src=\"/imagens/alterar_01.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
				'<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
}
	$sql = "SELECT $acoes as acao,
				forcurso,
			    (case when fornivel = 'PG' then 'Pós Graduação'
			    	when fornivel = 'ME' then 'Médio'
			     	when fornivel = 'SU' then 'Superior'
			     	when fornivel = 'MD' then 'Mestrado'
			     	when fornivel = 'MO' then 'Doutorado'
			     end) as fornivel, forinstituicao,
  				forperiodoini||' - '||forperiodofim as periodo
			FROM siscap.formacao WHERE facid = '$facid'";

$cabecalho = array("Ações", "Curso", "Nível", "Instituição", "Período Inicial/Final");
$db->monta_lista($sql, $cabecalho, 10, 4, 'N','Center','','form', $tamanho, $alinhamento);
?>
<script type="text/javascript">
function excluirFormacao( forid ){
	if(confirm("Deseja realmente excluir está Formação?")) {
		$('requisicao').value = 'excluir';
		$('forid').value = forid;
		$('formulario').submit();
	}
}

function salvarFormacao(tipo){
	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	campos = "#forcurso#fornivel#forinstituicao#forperiodoini#forperiodofim";
	tiposDeCampos 	= "#texto#select#texto#texto#texto";

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
		var obDt  = new Data();

		var forperiodoini = $('forperiodoini').value.split('/');
		var forperiodofim = $('forperiodofim').value.split('/');
		var dataAtual = $('dataAtual').value;
		var periodoini = '01/'+$('forperiodoini').value;
		var periodofim = '01/'+$('forperiodofim').value;

		if(forperiodoini[0] > 12 ){
			alert( 'O mês do periodo inicial não pode ser maior que 12' );
			$('forperiodoini').focus();
			return false;
		}
		if(forperiodofim[0] > 12 ){
			alert( 'O mês do periodo final não pode ser maior que 12' );
			$('forperiodofim').focus();
			return false;
		}
		if( (forperiodoini[0] > forperiodofim[0]) && (forperiodoini[1] == forperiodofim[1]) ){
			alert( 'O mês do periodo inicial não pode ser maior que o mês do periodo final' );
			$('forperiodoini').focus();
			return false;
		}
		if( forperiodoini[1] > forperiodofim[1] ){
			alert( 'O ano do periodo inicial não pode ser maior que o ano do periodo final' );
			$('forperiodofim').focus();
			return false;
		}
		if(forperiodoini[1].length < 4){
			alert( 'O ano do periodo inicial está no formato incorreto' );
			$('forperiodoini').focus();
			return false;
		}
		if(forperiodofim[1].length < 4){
			alert( 'O ano do periodo final está no formato incorreto' );
			$('forperiodofim').focus();
			return false;
		}

		if( obDt.comparaData( periodoini, dataAtual, '>' ) ){
			alert('O Período Inicial/Final não pode ser maior que a data atual!');
			$('forperiodoini').focus();
			return false;
		}
		if( obDt.comparaData( periodofim, dataAtual, '>' ) ){
			alert('O Período Inicial/Final não pode ser maior que a data atual!');
			$('forperiodofim').focus();
			return false;
		}

		var fieldName = "#forcurso#forinstituicao";
		var labelCampo = "#Curso#Instituição";

		if(naoSomenteNumeros('formulario', fieldName, labelCampo)){
			$('requisicao').value = 'salvar';
			$('tipo').value = tipo;
			$('formulario').submit();
		}
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