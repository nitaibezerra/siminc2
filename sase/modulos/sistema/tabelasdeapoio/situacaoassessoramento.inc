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


// tratamento de filtros -------------------------

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroStadsc':
				$filtros['stadsc'] = $valor;
				break;
			case 'filtroStastatus':
				$filtros['stastatus'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $SituacaoAssessoramento->excluirSituacaoAssessoramento( $_GET['stacod'] ) ){
		echo "
			<script>
				alert('Situação de Assistência Técnica excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabelasdeapoio/situacaoassessoramento&acao=A';
			</script>";
		exit;
	}
// para exclusao /

// tratamento de filtros ------------------------- /

?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formPesquisaLista" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="filtroStadsc" class="col-sm-3 control-label">Situação de Assistência Técnica:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroStadsc" id="filtroStadsc" value="<?=($_POST['filtroStadsc'])?$_POST['filtroStadsc']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroStastatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroStastatus" id="filtroStastatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroStastatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroStastatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramento&acao=A" class="btn btn-primary">Listar Todos</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A" class="btn btn-primary">Nova Situação de Assistência Técnica</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Situação", "Cor","Status");
$alinhamento = array('left','left','left','left');
$larguras = array('15%','30%', '25%','30%');
$db->monta_lista($SituacaoAssessoramento->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaSituacaoAssessoramento',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de assessoramento
	 */
	function editarSituacaoAssessoramento( stacod ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramentoedicao&acao=A&stacod=' + stacod;
	}

	/**
	 * Excluir logicamente assessoramento
	 */
	 function excluirSituacaoAssessoramento( stacod ){
	 	if( confirm("Deseja realmente apagar essa Situação de Assistência Técnica?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoassessoramento&acao=A&excluir=1&stacod=' + stacod;
	 	}
	}
</script>
<!-- script js / -->