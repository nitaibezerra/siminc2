<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mesoregiao.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Região';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>
<?php
// --


global $db;
$Mesoregiao = new Mesoregiao();


// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// orgao
$sql = "select orgid, orgdsc from sase.orgao order by orgdsc ASC";
$orgaos = $db->carregar($sql);
// orgao /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroMesdsc':
				$filtros['m.mesdsc'] = $valor;
				break;
			case 'filtroMesstatus':
				$filtros['m.messtatus'] = $valor;
				break;
			case 'filtroOrgao':
				$filtros['o.orgid'] = $valor;
				break;
			case 'filtroEstado':
				$filtros['oes.estuf'] = $valor;
				break;
			case 'filtroMescor':
				$filtros['m.mescor'] = $valor;
				break;
		}
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $Mesoregiao->excluirMesoregiao( $_GET['mesid'] ) ){
		echo "
			<script>
				alert('Região excluída com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/tabelasdeapoio/mesoregiao&acao=A';
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

			<div class="form-group" id="divFiltroMesoregiao">
				<label for="filtroMesdsc" class="col-sm-3 control-label">Região:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroMesdsc" id="filtroMesdsc" value="<?=($_POST['filtroMesdsc'])?$_POST['filtroMesdsc']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroEstado">
				<label for="filtroEstado" class="col-sm-3 control-label">Estado:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroEstado" id="filtroEstado">
						<option value="">Filtrar estado</option>
						<?php 
						foreach( $estados as $chave => $estado ){ ?>
							<option <?=($estado['estuf']==$_POST['filtroEstado'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divFiltroOrgao">
				<label for="filtroOrgao" class="col-sm-3 control-label">Orgão:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroOrgao" id="filtroOrgao">
						<option value="">Filtrar orgão</option>
						<?php 
						foreach( $orgaos as $chave => $orgao ){ ?>
							<option <?=($orgao['orgid']==$_POST['filtroOrgao'])?'selected':''?> value="<?=$orgao['orgid']?>"><?=$orgao['orgdsc']?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divFiltroMesoregiao">
				<label for="filtroMescor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="filtroMescor" id="filtroMescor" value="<?=($_POST['filtroMescor'])?$_POST['filtroMescor']:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divFiltroSituacao">
				<label for="filtroMesstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="filtroMesstatus" id="filtroMesstatus">
						<option value="">Status</option>
						<option <?=($_POST['filtroMesstatus']=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($_POST['filtroMesstatus']=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formPesquisaLista]').submit()" value="Pesquisar" />
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiao&acao=A" class="btn btn-primary">Listar Todos</a>
					&nbsp;&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A" class="btn btn-primary">Novo Mesoregiao</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

<?php 

$cabecalho = array("Ação", "Estado", "Região", "Cor", "Orgão", "Status");
$alinhamento = array('left','left','left','left','left','left');
$larguras = array('15%','15%', '20%','15%','25%','10%');
$db->monta_lista($Mesoregiao->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaMesoregiao',$larguras,$alinhamento); ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarMesoregiao( mesid ){
		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A&mesid=' + mesid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirMesoregiao( mesid ){
	 	if( confirm("Deseja realmente apagar essa Situação de Questões Pontuais?") ){
	 		window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiao&acao=A&excluir=1&mesid=' + mesid;
	 	}
	}
</script>
<!-- script js / -->