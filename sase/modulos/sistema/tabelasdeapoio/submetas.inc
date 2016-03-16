<?php

ob_start();

// --------------- DependÃªncias
include_once '../../sase/classes/Submetas.class.inc';
//include_once '../../sase/classes/EvalMath.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- CabeÃ§alho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Submeta';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$Submetas = new Submetas();


if( $_GET['subid'] || $_GET['subid'] === '0' )
	$Submetas->carregarPorId( $_GET['subid'] );


// submit
if( $_POST['formulario'] ){
	
	validaDados();

	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['subid']) ){

		$Submetas->populaSubmetas( $_POST );
		$retorno = $Submetas->atualizarSubmetas();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A&subid='+".$_GET['subid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A&subid='+".$_GET['subid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A&subid='+".$_GET['subid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['subid']);
		$Submetas->populaSubmetas( $_POST );
		$retorno = $Submetas->cadastrarSubmetas();
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
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A&subid=" . $Submetas->subid . "';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A';
					</script>
				";

			exit;
		}

	}
}
function validaDados(){
	if( !is_numeric($_POST['subtaxa']) || !is_numeric($_POST['submetanacional']) || !is_numeric($_POST['subpatamarminimo']) ){
		echo "
			<script>
				alert('Existem campos númericos incorretamente preenchidos.');
				window.history.back();
			</script>
		";
		exit;
	}	
}
// submit /


// meta
$sql = "select metid, mettitulo from sase.meta order by metid ASC";
$metas = $db->carregar($sql);
// meta /


?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoSubmetas" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="subid" value="<?=($_GET['subid'])?$_GET['subid']:''?>" type="hidden"/>
			<input name="subformulatcn" value="<?=$Submetas->subformulatcn?>" type="hidden"/>
			<input name="subformulatpc" value="<?=$Submetas->subformulatpc?>" type="hidden"/>
			<input name="subformulatxc" value="<?=$Submetas->subformulatxc?>" type="hidden"/>


			<div class="form-group" id="divSubmetas">
				<label for="subtitulo" class="col-sm-3 control-label">Submeta:</label>
				<div class="col-sm-8 inputarea">
					<textarea style="width:400px;height:100px;" name="subtitulo" id="subtitulo" ><?=trim($Submetas->subtitulo)?></textarea>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="metid" class="col-sm-3 control-label">Meta:</label>
				<div class="col-sm-8 inputarea">
					<select name="metid" id="metid">
						<option value="">Meta:</option>
						<?php 
						foreach( $metas as $chave => $meta ){ ?>
							<option <?=($meta['metid']==$Submetas->metid)?'selected':''?> value="<?=$meta['metid']?>"><?=substr($meta['mettitulo'], 0, 40) . '...'?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="subtaxa" class="col-sm-3 control-label">Taxa Atual:</label>
				<div class="col-sm-8 inputarea">
					<input name="subtaxa" id="subtaxa" value="<?=($Submetas->subtaxa)?$Submetas->subtaxa:''?>" type="text"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="subtaxacrescnacional" class="col-sm-3 control-label">Taxa de Crescimento Nacional:</label>
				<div class="col-sm-8 inputarea">
					<input name="subtaxacrescnacional" id="subtaxacrescnacional" value="<?=($Submetas->subtaxacrescnacional)?$Submetas->subtaxacrescnacional:''?>" type="text" disabled/>
				</div>
			</div>


			<div class="form-group" id="divFiltroEstado">
				<label for="submetanacional" class="col-sm-3 control-label">Meta Nacional:</label>
				<div class="col-sm-8 inputarea">
					<input name="submetanacional" id="submetanacional" value="<?=($Submetas->submetanacional)?$Submetas->submetanacional:''?>" type="text"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="subtempoconvergencia" class="col-sm-3 control-label">Tempo de Convergência:</label>
				<div class="col-sm-8 inputarea">
					<input name="subtempoconvergencia" id="subtempoconvergencia" value="<?=($Submetas->subtempoconvergencia)?$Submetas->subtempoconvergencia:''?>" type="text" disabled/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="subtaxaconvergencia" class="col-sm-3 control-label">Taxa de Convergência:</label>
				<div class="col-sm-8 inputarea">
					<input name="subtaxaconvergencia" id="subtaxaconvergencia" value="<?=($Submetas->subtaxaconvergencia)?$Submetas->subtaxaconvergencia:''?>" type="text" disabled/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="subpatamarminimo" class="col-sm-3 control-label">Patamar Mínimo:</label>
				<div class="col-sm-8 inputarea">
					<input name="subpatamarminimo" id="subpatamarminimo" value="<?=($Submetas->subpatamarminimo)?$Submetas->subpatamarminimo:''?>" type="text"/>
				</div>
			</div>
			
			<div class="form-group" id="divStatus">
				<label for="substatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="substatus" id="substatus">
						<option value="">Status</option>
						<option <?=($Submetas->substatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($Submetas->substatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoSubmetas]').submit()" 
						value="<?=($_GET['subid']||$_GET['subid']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listasubmetas&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->