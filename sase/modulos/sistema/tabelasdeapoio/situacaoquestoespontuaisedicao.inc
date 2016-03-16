<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/SituacaoQuestoesPontuaisPar.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Situação de Questões Pontuais Par';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$SituacaoQuestoesPontuais = new SituacaoQuestoesPontuaisPar();


if( $_GET['sqpcod'] || $_GET['sqpcod'] === '0' )
	$SituacaoQuestoesPontuais->carregarPorId( $_GET['sqpcod'] );


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['sqpcod']) ){

		$SituacaoQuestoesPontuais->populaSituacaoQuestoesPontuais( $_POST );
		$retorno = $SituacaoQuestoesPontuais->atualizarSituacaoQuestoesPontuais();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A&sqpcod='+".$_GET['sqpcod'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A&sqpcod='+".$_GET['sqpcod'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A&sqpcod='+".$_GET['sqpcod'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['sqpcod']);
		$SituacaoQuestoesPontuais->populaSituacaoQuestoesPontuais( $_POST );
		$retorno = $SituacaoQuestoesPontuais->cadastrarSituacaoQuestoesPontuais();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.history.back();
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso!');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A';
					</script>
				";

			exit;
		}

	}
}
// submit /


?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoSituacaoQuestoesPontuais" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="sqpcod" value="<?=($_GET['sqpcod'])?$_GET['sqpcod']:''?>" type="hidden"/>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="stadsc" class="col-sm-3 control-label">Situação de Questões Pontuais Par:</label>
				<div class="col-sm-8 inputarea">
					<input name="sqpdsc" type="text" maxlength="48" id="sqpdsc" value="<?=trim($SituacaoQuestoesPontuais->sqpdsc)?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="stacor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="sqpcor" id="sqpcor" maxlength="7" value="<?=trim($SituacaoQuestoesPontuais->sqpcor)?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="sqpstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="sqpstatus" id="sqpstatus">
						<option value="">Status</option>
						<option <?=($SituacaoQuestoesPontuais->sqpstatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($SituacaoQuestoesPontuais->sqpstatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoSituacaoQuestoesPontuais]').submit()" 
						value="<?=($_GET['sqpcod']||$_GET['sqpcod']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuais&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->