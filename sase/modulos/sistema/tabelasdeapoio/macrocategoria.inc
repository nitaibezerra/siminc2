<?php

ob_start();

// --------------- DependÃªncias
include_once '../../sase/classes/MacroCategoria.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- CabeÃ§alho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Macro Categoria';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$MacroCategorias = new MacroCategoria();


if( $_GET['maccod'] || $_GET['maccod'] === '0' )
	$MacroCategorias->carregarPorId( $_GET['maccod'] );


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['maccod']) ){

		$MacroCategorias->populaMacroCategorias( $_POST );
		$retorno = $MacroCategorias->atualizarMacroCategorias();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A&maccod='+".$_GET['maccod'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A&maccod='+".$_GET['maccod'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A&maccod='+".$_GET['maccod'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['maccod']);
		$MacroCategorias->populaMacroCategorias( $_POST );
		$retorno = $MacroCategorias->cadastrarMacroCategorias();
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
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A&maccod=" . $MacroCategorias->maccod . "';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A';
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
		<form class="form-horizontal" name="formEdicaoMacroCategorias" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="maccod" value="<?=($_GET['maccod'])?$_GET['maccod']:''?>" type="hidden"/>

			<div class="form-group" id="divMetas">
				<label for="macdsc" class="col-sm-3 control-label">Descrição:</label>
				<div class="col-sm-8 inputarea">
					<input name="macdsc" id="macdsc" value="<?=trim($MacroCategorias->macdsc)?>"/>					
				</div>
			</div>

			<div class="form-group" id="divOrgcod">
				<label for="maccor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="maccor" id="maccor" value="<?=trim($MacroCategorias->maccor)?>"/>					
				</div>
			</div>

			<div class="form-group" id="divStatus">
				<label for="macstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="macstatus" id="macstatus">
						<option value="">Status</option>
						<option <?=($MacroCategorias->macstatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($MacroCategorias->macstatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoMacroCategorias]').submit()" 
						value="<?=($_GET['maccod']||$_GET['maccod']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listamacrocategorias&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->