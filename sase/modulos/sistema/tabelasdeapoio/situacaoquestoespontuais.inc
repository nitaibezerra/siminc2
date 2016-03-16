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
$SituacaoQuestoesPontuaisPar = new SituacaoQuestoesPontuaisPar();


// tratamento de filtros -------------------------

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroStadsc':
				$filtros['sqpdsc'] = $valor;
				break;
			case 'filtroSqpstatus':
				$filtros['sqpstatus'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $SituacaoQuestoesPontuaisPar->excluirSituacaoQuestoesPontuais( $_GET['sqpcod'] ) ){
		echo "
			<script>
				alert('Situação de Questões Pontuais excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabeladeapoio/situacaoquestoespontuais&acao=A';
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
				<label for="filtroStadsc" class="col-sm-3 control-label">Situação de Questões Pontuais:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroStadsc" id="filtroStadsc" value="<?=($_POST['filtroStadsc'])?$_POST['filtroStadsc']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroSqpstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroSqpstatus" id="filtroSqptatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroSqpstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroSqpstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuais&acao=A" class="btn btn-primary">Listar Todos</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A" class="btn btn-primary">Nova Situação de Questões Pontuais</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Situação", "Cor","Status");
$alinhamento = array('left','left','left','left');
$larguras = array('15%','30%', '25%','30%');
$db->monta_lista($SituacaoQuestoesPontuaisPar->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaSituacaoQuestoesPontuaisPar',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarSituacaoQuestoesPontuais( sqpcod ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuaisedicao&acao=A&sqpcod=' + sqpcod;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirSituacaoQuestoesPontuais( sqpcod ){
	 	if( confirm("Deseja realmente apagar essa Situação de Questões Pontuais?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/situacaoquestoespontuais&acao=A&excluir=1&sqpcod=' + sqpcod;
	 	}
	}
</script>
<!-- script js / -->