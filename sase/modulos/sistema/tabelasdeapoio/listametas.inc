<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Metas.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Metas';

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


// tratamento de filtros -------------------------

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroMettitulo':
				$filtros['mettitulo'] = $valor;
				break;
			case 'filtroMetstatus':
				$filtros['metstatus'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $Metas->excluirMetas( $_GET['metid'] ) ){
		echo "
			<script>
				alert('Orgão excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabelasdeapoio/listametas&acao=A';
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

			<div class="form-group" id="divFiltroMetas">
				<label for="filtroMettitulo" class="col-sm-3 control-label">Meta:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroMettitulo" id="filtroMettitulo" value="<?=($_POST['filtroMettitulo'])?$_POST['filtroMettitulo']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroMetstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroMetstatus" id="filtroSqptatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroMetstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroMetstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formPesquisaLista').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listametas&acao=A" class="btn btn-primary">Listar Todas</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A" class="btn btn-primary">Nova Meta</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação", "Meta", "Status");
$alinhamento = array('left','left','left');
$larguras = array('15%','70%', '15%');
// ver($Metas->montaListaQuery( $filtros ),d);
$db->monta_lista($Metas->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMetas',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarMetas( metid ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/metas&acao=A&metid=' + metid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirMetas( metid ){
	 	if( confirm("Deseja realmente apagar essa Meta?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/listametas&acao=A&excluir=1&metid=' + metid;
	 	}
	}
</script>
<!-- script js / -->