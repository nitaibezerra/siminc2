<?php

global $db;
$QuestoesPontuaisPar = new QuestoesPontuaisPar();

// carrega objeto
if( $_GET['qptid'] )
	$QuestoesPontuaisPar->carregaQuestoesPontuaisPar( $_GET['qptid'] );
// carrega objeto /


// SUBMIT DO FORM
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	if( !empty($_POST['qptid']) ){ // UPDATE

		$qtpid = $_POST['qptid'];
		unset($_POST['qptid']);
		$QuestoesPontuaisPar->populaQuestoesPontuaisPar( $_POST );
		$retorno = $QuestoesPontuaisPar->atualizarQuestoesPontuaisPar();
		// ver($retorno,d);
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao&qptid='+".$_GET['qptid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao&qptid='+".$_GET['qptid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao&qptid='+".$_GET['qptid'].";
					</script>
				";
			exit;
		}

	}else{ // INSERT

		unset($_POST['qptid']);
		$QuestoesPontuaisPar->populaQuestoesPontuaisPar( $_POST );
		$retorno = $QuestoesPontuaisPar->insereQuestoesPontuaisPar( $_POST );
		// dbg($retorno,d);
		if( is_bool($retorno) ){
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao';
					</script>
				";
			exit;
		}else{
			echo "
				<script>
					alert('Solicitação realizada com sucesso.');
					window.location.href = '/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao&qptid='+".$retorno->qptid.";
				</script>
			";
			exit;
		}

	}

}
// SUBMIT DO FORM /


// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// municipio
$sql = "select * from territorios.municipio";
if ( $_GET['estuf'] ) $sql .= " where estuf = '" . $_GET['estuf'] . "' ";
elseif ( $QuestoesPontuaisPar->estuf ) $sql .= " where estuf = '" . $QuestoesPontuaisPar->estuf . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);
if( $_GET['filtroAjax'] ){ 
	ob_clean(); ?>
	<select class="select100" name="muncod" id="muncod">
		<option value="">Município:</option>
		<?php 
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

?>

<!-- html -->
<div id="container">

	<h4>Cadastro de Questoes Pontuais Par</h4>

	<div id="formularioDeCadastro">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formCadastroLista" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="qptid" value="<?=@$_GET['qptid']?>" type="hidden"/>
			
			<div class="col-sm-8">

				<div class="form-group" id="divFiltroEstado">
					<label for="estuf" class="col-sm-2 control-label">Estado:</label>
					<div class="col-sm-7 inputarea">
						<select class="select100" name="estuf" id="estuf" onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}" <?=(!$_GET['qptid'])?'':'disabled'?>>
							<option value="">Filtrar estado</option>
							<?php 
							foreach( $estados as $chave => $estado ){ ?>
								<option <?=($estado['estuf']==$QuestoesPontuaisPar->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
							<?php }?>
						</select>
					</div>
				</div>

				<div class="form-group" id="divFiltroMunicipio">
					<label for="muncod" class="col-sm-2 control-label">Município:</label>
					<div class="col-sm-7 inputarea">
						<?php if(!$QuestoesPontuaisPar->estuf){ echo "Selecione um Estado"; }else{ ?>
						<select class="select100" name="muncod" id="muncod" <?=(!$_GET['qptid'])?'':'disabled'?>>
							<option value="">Filtrar município</option>
							<?php 
							foreach( $municipios as $chave => $municipio ){ ?>
								<option <?=($municipio['muncod']==$QuestoesPontuaisPar->muncod)?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
							<?php }?>
						</select>
						<?php } ?>
					</div>
				</div>

				<div class="form-group" id="divFiltroSituacao">
					<label for="sqpcod" class="col-sm-2 control-label">Situação:</label>
					<div class="col-sm-7 inputarea">
						<select class="select100" name="sqpcod" id="sqpcod">
							<option value="">Filtrar situação</option>
							<?php 
							foreach( $situacoes as $chave => $situacao ){ ?>
								<option <?=($situacao['sqpcod']==$QuestoesPontuaisPar->sqpcod)?'selected':''?> value="<?=$situacao['sqpcod']?>"><?=$situacao['sqpdsc']?></option>
							<?php }?>
						</select>
					</div>
				</div>

				<div class="form-group" id="divQptstatus">
					<label for="qptstatus" class="col-sm-2 control-label">Status:</label>
					<div class="col-sm-7 inputarea">
						<select name="qptstatus" id="qptstatus">
							<option value="">Status</option>
							<option <?=($QuestoesPontuaisPar->qptstatus=='A')?'selected':''?> value="A">Ativo</option>
							<option <?=($QuestoesPontuaisPar->qptstatus=='I')?'selected':''?> value="I">Inativo</option>
						</select>
					</div>
				</div>

				<div class="form-group">
					<label class="col-sm-2 control-label">&nbsp;</label>
					<div class="col-sm-7">
						<input type="button" class="btn btn-primary" onclick="jQuery('[name=formCadastroLista]').submit()" 
							value="<?=($_GET['qptid'])?'Modificar':'Cadastrar'?>"/>
						&nbsp;&nbsp;&nbsp;
						<a href="/sase/sase.php?modulo=principal/questoespontuais&acao=A&aba=lista" class="btn btn-primary">Voltar</a>
					</div>
				</div>

			</div>
			
			<div class="col-sm-3">
				<?php 
				if( !$QuestoesPontuaisPar->docid && $_GET['qptid']){
					$docid = wf_cadastrarDocumento( TPDID_SASE_QUESTOESPONTUAIS, 'Documento Questões Pontuais' );
					$QuestoesPontuaisPar->docid = $docid;
					// ver($QuestoesPontuaisPar,d);
					$QuestoesPontuaisPar->alterar();
					$QuestoesPontuaisPar->commit();
				}

				if($_GET['qptid'] && $QuestoesPontuaisPar->docid) wf_desenhaBarraNavegacao( $QuestoesPontuaisPar->docid, array() ); ?>
			</div>

		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

	</div>

</div>
<!-- html / -->

<!-- script js -->
<script>
	/**
	 * Filtro de municipios com a mudanca do estado
	 */
	function filtraMunicipios( estuf ){
		jQuery.ajax({
			url:'',
			type:'GET',
			data:{filtroAjax:true,estuf:estuf},
			success: function( resposta ){
				jQuery('#divFiltroMunicipio .inputarea').html( resposta );
			}
		});
	}

	/**
	 * Manda para edicao de assessoramento
	 */
	function editarQuestoesPontuaisPar( qptid ){
		window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&qptid=' + qptid;
	}

	/**
	 * Excluir logicamente assessoramento
	 */
	 function excluirQuestoesPontuaisPar( qptid ){
	 	if( confirm("Deseja realmente apagar esse QuestoesPontuaisPar?") ){
	 		window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=lista&excluir=1&qptid=' + qptid;
	 	}
	}

</script>
<!-- script js / -->