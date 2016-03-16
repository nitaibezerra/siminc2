<?php

global $db;
$QuestoesPontuaisPar = new QuestoesPontuaisPar();


// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// municipio
$sql = "select * from territorios.municipio";
if( $_REQUEST['filtroEstado'] ) $sql .= " where estuf = '" . $_REQUEST['filtroEstado'] . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);
if( $_GET['filtroAjax'] ){ 
	ob_clean(); ?>
	<select name="filtroMunicipio" id="filtroMunicipio">
		<option value="">Filtrar município:</option>
		<?php 
		if( $municipios )
		foreach( $municipios as $chave => $municipio ){ ?>
			<option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
		<?php }?>
	</select>
<?php die();}
// municipio /

// situacao
$sql = "select * from sase.situacaoquestoespontuais where sqpstatus = 'A' order by sqpdsc ASC ";
$situacoes = $db->carregar( $sql );
// situacao /

// tramitacao
$sql = "select * from workflow.estadodocumento where tpdid = {$QuestoesPontuaisPar->tpdid} order by esddsc ASC ";
$tramitacoes = $db->carregar( $sql );
// tramitacao /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
		switch ($chave) {
			case 'filtroMunicipio':
				$filtros['a.muncod'] = $valor;
				break;
			case 'filtroEstado':
				$filtros['a.estuf'] = $valor;
				break;
			case 'filtroSituacao':
				$filtros['a.sqpcod'] = $valor;
				break;
			case 'filtroTramitacao':
				$filtros['ed.esdid'] = $valor;
				break;
		}
// ver($filtros);
// para lista /

// para exclusao
if( $_GET['excluir'] )
	if( $QuestoesPontuaisPar->excluirQuestoesPontuaisPar( $_GET['qptid'] ) ){
		echo "
			<script>
				alert('QuestoesPontuaisPar excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=lista';
			</script>";
		exit;
	}
// para exclusao /

// tratamento de filtros ------------------------- /

?>

<!-- ------------------------------- FORM ------------------------------- -->

<div id="formularioFiltros">
	<form class="form-horizontal" name="formPesquisaLista" role="form" method="POST">
		<input name="formulario" value="1" type="hidden"/>
		
		<div class="form-group" id="divFiltroEstado">
			<label for="filtroEstado" class="col-sm-1 control-label">Estado:</label>
			<div class="col-sm-10 inputarea">
				<select name="filtroEstado" id="filtroEstado" onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}">
					<option value="">Filtrar estado</option>
					<?php 
					foreach( $estados as $chave => $estado ){ ?>
						<option <?=($estado['estuf']==$_POST['filtroEstado'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
					<?php }?>
				</select>
			</div>
		</div>

		<div class="form-group" id="divFiltroMunicipio">
			<label for="filtroMunicipio" class="col-sm-1 control-label">Município:</label>
			<div class="col-sm-10 inputarea">
				<?php if(!$_POST['filtroEstado']){ echo "Selecione um Estado"; }else{ ?>
				<select name="filtroMunicipio" id="filtroMunicipio">
					<option value="">Filtrar município</option>
					<?php 
					foreach( $municipios as $chave => $municipio ){ ?>
						<option <?=($municipio['muncod']==$_POST['filtroMunicipio'])?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
					<?php }?>
				</select>
				<?php } ?>
			</div>
		</div>

		<div class="form-group" id="divFiltroSituacao">
			<label for="filtroSituacao" class="col-sm-1 control-label">Situação:</label>
			<div class="col-sm-10 inputarea">
				<select name="filtroSituacao" id="filtroSituacao">
					<option value="">Filtrar situação</option>
					<?php 
					foreach( $situacoes as $chave => $situacao ){ ?>
						<option <?=($situacao['sqpcod']==$_POST['filtroSituacao'])?'selected':''?> value="<?=$situacao['sqpcod']?>"><?=$situacao['sqpdsc']?></option>
					<?php }?>
				</select>
			</div>
		</div>

		<div class="form-group" id="divFiltroTramitacao">
			<label for="filtroTramitacao" class="col-sm-1 control-label">Tramitação:</label>
			<div class="col-sm-10 inputarea">
				<select name="filtroTramitacao" id="filtroTramitacao">
					<option value="">Filtrar tramitação</option>
					<?php 
					foreach( $tramitacoes as $chave => $tramicacao ){ ?>
						<option <?=($tramicacao['esdid']==$_POST['filtroTramitacao'])?'selected':''?> value="<?=$tramicacao['esdid']?>"><?=$tramicacao['esddsc']?></option>
					<?php }?>
				</select>
			</div>
		</div>

		<div class="form-group">
			<label class="col-sm-1 control-label">&nbsp;</label>
			<div class="col-sm-10">
				<input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
				&nbsp;&nbsp;&nbsp;
				<input type="button" class="btn btn-primary" onclick="window.location.reload()" value="Listar todos" />
			</div>
		</div>
	</form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Estado","Município","Situação","Tramitação","Usuário","Status");
$alinhamento = array('left','left','left','left','left','left','left');
$larguras = array('10%','15%','15%','15%','15%','20%','10%');
// ver($QuestoesPontuaisPar->montaListaQuery( $filtros ),d);
$db->monta_lista($QuestoesPontuaisPar->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaQuestoesPontuaisPar',$larguras,$alinhamento); ?>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
	/**
	 * Filtro de municipios com a mudanca do estado
	 */
	function filtraMunicipios( estuf ){
		jQuery.ajax({
			url:'',
			type:'GET',
			data:{filtroAjax:true,filtroEstado:estuf},
			success: function( resposta ){
				jQuery('#divFiltroMunicipio .inputarea').html( resposta );
			}
		});
	}

	/**
	 * Manda para edicao de questoespontuais
	 */
	function editarQuestoesPontuaisPar( qptid ){
		window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao&qptid=' + qptid;
	}

	/**
	 * Excluir logicamente questoespontuais
	 */
	 function excluirQuestoesPontuaisPar( qptid ){
	 	if( confirm("Deseja realmente inativar esse QuestoesPontuaisPar?") ){
	 		window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=lista&excluir=1&qptid=' + qptid;
	 	}
	}
</script>
<!-- script js / -->