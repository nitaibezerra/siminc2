<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Situação de Assistência Técnica';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$SituacaoAssessoramento = new SituacaoAssessoramento();

if( $_GET['stacod'] || $_GET['stacod'] === '0' )
	$SituacaoAssessoramento->carregarPorId( $_GET['stacod'] );


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['stacod']) ){

		$SituacaoAssessoramento->populaSituacaoAssessoramento( $_POST );
		$retorno = $SituacaoAssessoramento->atualizarSituacaoassessoramento();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A&stacod='+".$_GET['stacod'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A&stacod='+".$_GET['stacod'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A&stacod='+".$_GET['stacod'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['stacod']);
		$SituacaoAssessoramento->populaSituacaoAssessoramento( $_POST );
		$retorno = $SituacaoAssessoramento->cadastrarSituacaoAssessoramento();
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
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A';
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
		<form class="form-horizontal" name="formEdicaoSituacaoAssessoramento" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="stacod" value="<?=($_GET['stacod'])?$_GET['stacod']:''?>" type="hidden"/>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="stadsc" class="col-sm-3 control-label">Situação de Assistência Técnica:</label>
				<div class="col-sm-8 inputarea">
					<input type="text" name="stadsc" maxlength="48" id="stadsc" value="<?=$SituacaoAssessoramento->stadsc?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="stacor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="stacor" id="stacor" maxlength="7" value="<?=$SituacaoAssessoramento->stacor?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="stastatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="stastatus" id="stastatus">
						<option value="">Status</option>
						<option <?=($SituacaoAssessoramento->stastatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($SituacaoAssessoramento->stastatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoSituacaoAssessoramento]').submit()" 
						value="<?=($_GET['stacod']||$_GET['stacod']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramento&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->