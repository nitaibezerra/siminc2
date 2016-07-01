<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/MacroCategoria.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
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


// tratamento de filtros -------------------------

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroMacdsc':
				$filtros['macdsc'] = $valor;
				break;
			case 'filtroMacstatus':
				$filtros['macstatus'] = $valor;
				break;
			case 'filtroMaccor':
				$filtros['maccor'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $MacroCategorias->excluirMacroCategorias( $_GET['maccod'] ) ){
		echo "
			<script>
				alert('Orgão excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabelasdeapoio/listamacrocategorias&acao=A';
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
				<label for="filtroMacdsc" class="col-sm-3 control-label">Descrição:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroMacdsc" id="filtroMacdsc" value="<?=($_POST['filtroMacdsc'])?$_POST['filtroMacdsc']:''?>"/>
				</div>
			</div>
			
			<div class="form-group" id="divFiltroMetas">
				<label for="filtroMaccor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroMaccor" id="filtroMaccor" value="<?=($_POST['filtroMaccor'])?$_POST['filtroMaccor']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroMacstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroMacstatus" id="filtroMacstatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroMacstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroMacstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>
			
			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formPesquisaLista]').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listamacrocategorias&acao=A" class="btn btn-primary">Listar Todas</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A" class="btn btn-primary">Nova Macro Categoria</a>
				</div>
			</div>
		</form>
	</div>


	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação", "Descrição","cor","Status");
$alinhamento = array('left','left','left','left');
$larguras = array('15%','50%','20%', '15%');
// ver($MacroCategorias->montaListaQuery( $filtros ),d);
$db->monta_lista($MacroCategorias->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMacroCategorias',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarMacroCategorias( maccod ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/macrocategoria&acao=A&maccod=' + maccod;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirMacroCategorias( maccod ){
	 	if( confirm("Deseja realmente apagar essa Macro Categoria?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/listamacrocategorias&acao=A&excluir=1&maccod=' + maccod;
	 	}
	}
</script>
<!-- script js / -->