<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Submetas.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Submetas';

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


// tratamento de filtros -------------------------

// meta
$sql = "select metid, mettitulo from sase.meta order by metid ASC";
$metas = $db->carregar($sql);
// meta /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroSubtitulo':
				$filtros['subtitulo'] = $valor;
				break;
			case 'filtroSubstatus':
				$filtros['substatus'] = $valor;
				break;
			case 'filtroMeta':
				$filtros['metid'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $Submetas->excluirSubmetas( $_GET['metid'] ) ){
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

			<div class="form-group" id="divFiltroSubmetas">
				<label for="filtroSubtitulo" class="col-sm-3 control-label">Submeta:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroSubtitulo" id="filtroSubtitulo" value="<?=($_POST['filtroSubtitulo'])?$_POST['filtroSubtitulo']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="filtroMeta" class="col-sm-3 control-label">Meta:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroMeta" id="filtroMeta">
						<option value="">Filtrar meta:</option>
						<?php 
						foreach( $metas as $chave => $meta ){ ?>
							<option <?=($meta['metid']==$_POST['filtroMeta'])?'selected':''?> value="<?=$meta['metid']?>"><?=substr($meta['mettitulo'], 0, 40) . '...'?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroSubstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroSubstatus" id="filtroSqptatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroSubstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroSubstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formPesquisaLista').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listasubmetas&acao=A" class="btn btn-primary">Listar Todas</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A" class="btn btn-primary">Nova Submeta</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação", "Submeta", "Meta", "Taxa Atual", "Meta Nacional", "Patamar Mínimo", "Status");
$alinhamento = array('left','left', 'left', 'left', 'left', 'left', 'left');
$larguras = array('5%','30%', '45%', '5%', '5%', '5%', '5%');
// ver($Submetas->montaListaQuery( $filtros ),d);
$db->monta_lista($Submetas->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaSubmetas',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarSubmetas( subid ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/submetas&acao=A&subid=' + subid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirSubmetas( subid ){
	 	if( confirm("Deseja realmente apagar essa Meta?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/listasubmetas&acao=A&excluir=1&subid=' + subid;
	 	}
	}
</script>
<!-- script js / -->