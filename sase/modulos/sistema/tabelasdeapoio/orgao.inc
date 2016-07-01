<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Orgao.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Orgão';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$Orgao = new Orgao();


// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroOrgdsc':
				$filtros['orgdsc'] = $valor;
				break;
			case 'filtroOrgcod':
				$filtros['orgcod'] = $valor;
				break;
			case 'filtroOrgstatus':
				$filtros['orgstatus'] = $valor;
				break;
//			case 'filtroEstado':
//				$filtros['estuf'] = $valor;
//				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $Orgao->excluirOrgao( $_GET['orgid'] ) ){
		echo "
			<script>
				alert('Orgão excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabelasdeapoio/orgao&acao=A';
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

			<div class="form-group" id="divFiltroOrgao">
				<label for="filtroOrgdsc" class="col-sm-3 control-label">Orgão:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroOrgdsc" id="filtroOrgdsc" value="<?=($_POST['filtroOrgdsc'])?$_POST['filtroOrgdsc']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroOrgcod">
				<label for="filtroOrgcod" class="col-sm-3 control-label">Código do Orgão:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroOrgcod" id="filtroOrgcod" value="<?=($_POST['filtroOrgcod'])?$_POST['filtroOrgcod']:''?>"/>
				</div>
			</div>

<!--			<div class="form-group" id="divFiltroEstado">-->
<!--				<label for="filtroEstado" class="col-sm-3 control-label">Estado</label>-->
<!--				<div class="col-sm-8 inputarea">-->
<!--					<select name="filtroEstado" id="filtroEstado">-->
<!--						<option value="">Filtrar estado</option>-->
<!--						--><?php //
//						foreach( $estados as $chave => $estado ){ ?>
<!--							<option --><?//=($estado['estuf']==$_POST['filtroEstado'])?'selected':''?><!-- value="--><?//=$estado['estuf']?><!--">--><?//=$estado['estdescricao']?><!--</option>-->
<!--						--><?php //} ?>
<!--					</select>-->
<!--				</div>-->
<!--			</div>-->

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroOrgstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroOrgstatus" id="filtroSqptatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroOrgstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroOrgstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/orgao&acao=A" class="btn btn-primary">Listar Todos</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A" class="btn btn-primary">Novo Orgao</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação", "Código do Orgão", "Orgão","Status");
$alinhamento = array('left','left','left','left');
$larguras = array('15%', '20%','45%','20%');
$db->monta_lista($Orgao->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaOrgao',$larguras,$alinhamento); 
?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarOrgao( orgid ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A&orgid=' + orgid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirOrgao( orgid ){
	 	if( confirm("Deseja realmente apagar essa Situação de Questões Pontuais?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgao&acao=A&excluir=1&orgid=' + orgid;
	 	}
	}
</script>
<!-- script js / -->