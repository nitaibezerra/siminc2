<?php

ob_start();

// --------------- DependÃªncias
include_once '../../sase/classes/Metas.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- CabeÃ§alho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Meta';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$Metas = new Metas();


if( $_GET['metid'] || $_GET['metid'] === '0' )
	$Metas->carregarPorId( $_GET['metid'] );


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['metid']) ){

		$Metas->populaMetas( $_POST );
		$retorno = $Metas->atualizarMetas();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A&metid='+".$_GET['metid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A&metid='+".$_GET['metid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A&metid='+".$_GET['metid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['metid']);
		$Metas->populaMetas( $_POST );
		$retorno = $Metas->cadastrarMetas();
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
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A&metid=" . $Metas->metid . "';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A';
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
		<form class="form-horizontal" name="formEdicaoMetas" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="metid" value="<?=($_GET['metid'])?$_GET['metid']:''?>" type="hidden"/>

			<div class="form-group" id="divMetas">
				<label for="mettitulo" class="col-sm-3 control-label">Meta:</label>
				<div class="col-sm-8 inputarea">
					<textarea style="width:400px;height:100px;" name="mettitulo" id="mettitulo" ><?=trim($Metas->mettitulo)?></textarea>
				</div>
			</div>

			<div class="form-group" id="divOrgcod">
				<label for="metfontemunicipio" class="col-sm-3 control-label">Fonte Município:</label>
				<div class="col-sm-8 inputarea">
					<textarea style="width:400px;height:100px;" name="metfontemunicipio" id="metfontemunicipio" ><?=trim($Metas->metfontemunicipio)?></textarea>
				</div>
			</div>

			<div class="form-group" id="divOrgcod">
				<label for="metfonteestado" class="col-sm-3 control-label">Fonte Estado:</label>
				<div class="col-sm-8 inputarea">
					<textarea style="width:400px;height:100px;" name="metfonteestado" id="metfonteestado" ><?=trim($Metas->metfonteestado)?></textarea>
				</div>
			</div>

			<div class="form-group" id="divStatus">
				<label for="metstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="metstatus" id="metstatus">
						<option value="">Status</option>
						<option <?=($Metas->metstatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($Metas->metstatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoMetas]').submit()" 
						value="<?=($_GET['metid']||$_GET['metid']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listametas&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->