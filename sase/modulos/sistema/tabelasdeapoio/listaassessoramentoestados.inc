<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/AssessoramentoEstado.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// Situação assessoramento
$sql = "select * from sase.situacaoassessoramento order by stacod ASC";
$situacao = $db->carregar($sql);
// situação assessoramento /

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Assessoramento Estado';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$AssessoramentoEstados = new AssessoramentoEstado();


// tratamento de filtros -------------------------

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroEstuf':
				$filtros['estuf'] = $valor;
				break;
			case 'filtroSituacao':
				$filtros['sa.stadsc'] = $valor;
				break;
			case 'filtroAsestatus':
				$filtros['asestatus'] = $valor;
				break;
				
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $AssessoramentoEstados->excluirAssessoramentoEstado( $_GET['aseid'] ) ){
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
				<label for="filtroEstuf" class="col-sm-3 control-label">Estado:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroEstuf" id="filtroEstuf">
						<option value="">Filtrar estado</option>
						<?php 
						foreach( $estados as $chave => $estado ){ ?>
							<option <?=($estado['estuf']==$_POST['filtroEstuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
						<?php } ?>
					</select>
				</div>
			</div>			
			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroSituacao" class="col-sm-3 control-label">Situação:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroSituacao" id="filtroSituacao">
						<option value="">Filtrar Situação</option>
						<?php 
						foreach( $situacao as $chave => $sit ){ ?>
							<option <?=($sit['stacod']==$_POST['filtroSituacao'])?'selected':''?> value="<?=$sit['stadsc']?>"><?=$sit['stadsc']?></option>
						<?php } ?>
					</select>
				</div>
			</div>			

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroAsestatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroAsestatus" id="filtroAsestatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroAsestatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroAsestatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>
			
			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formPesquisaLista]').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listaassessoramentoestados&acao=A" class="btn btn-primary">Listar Todas</a>
					&nbsp;&nbsp;&nbsp;
					<!-- <a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A" class="btn btn-primary">Novo Assessoramento Estado</a> -->
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação", "Estado","Situação Assessoramento","Status");
$alinhamento = array('left','left','left','left');
$larguras = array('5%','5%','85%', '5%');
// ver($AssessoramentoEstados->montaListaQuery( $filtros ),d);
$db->monta_lista($AssessoramentoEstados->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMacroCategorias',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarAssessoramentoEstado( aseid ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A&aseid=' + aseid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirAssessoramentoEstado( aseid ){
	 	if( confirm("Deseja realmente apagar esse Assessoramento Estado ?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/listaassessoramentoestados&acao=A&excluir=1&aseid=' + aseid;
	 	}
	}
</script>
<!-- script js / -->