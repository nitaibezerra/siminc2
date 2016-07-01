<?php
ob_start ();

// --------------- DependÃªncias
include_once '../../sase/classes/AssessoramentoEstado.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar ( $sql );
// estado /

// Situação assessoramento
$sql = "select * from sase.situacaoassessoramento order by stacod ASC";
$situacao = $db->carregar ( $sql );
// situação assessoramento /

// --------------- CabeÃ§alho
$cabecalhoSistema [] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema [] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Assessoramento Estado';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css"
	media='screen' />

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center>
		<h3><?=$titulo?></h3>
	</center>
</div>
<?php
// --

global $db;
$AssessoramentoEstado = new AssessoramentoEstado ();

if ($_GET ['aseid'] || $_GET ['aseid'] === '0')
	$AssessoramentoEstado->carregarPorId ( $_GET ['aseid'] );
	
	// submit
if ($_POST ['formulario']) {
	unset ( $_POST ['formulario'] );
	
	// edicao
	if (! empty ( $_POST ['aseid'] )) {
		
		$AssessoramentoEstado->populaAssessoramentoEstado ( $_POST );
		$retorno = $AssessoramentoEstado->atualizarAssessoramentoEstado ();
		if (! is_bool ( $retorno )) {
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A&aseid='+" . $_GET ['aseid'] . ";
				</script>
			";
			exit ();
		} else {
			if ($retorno)
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A&aseid='+" . $_GET ['aseid'] . ";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A&aseid='+" . $_GET ['aseid'] . ";
					</script>
				";
			exit ();
		}
	} else { // cadastro
		
		unset ( $_POST ['aseid'] );
		$AssessoramentoEstado->populaAssessoramentoEstado ( $_POST );
		$retorno = $AssessoramentoEstado->cadastrarAssessoramentoEstado ();
		if (! is_bool ( $retorno )) {
			echo "
				<script>
					alert('Existem campos vazios.');
					window.history.back();
				</script>
			";
			exit ();
		} else {
			if ($retorno)
				echo "
					<script>
						alert('Solicitação realizada com sucesso!');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A&aseid=" . $AssessoramentoEstado->aseid . "';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/assessoramentoestado&acao=A';
					</script>
				";
			
			exit ();
		}
	}
}
// submit /

?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoassessoramentoestados"
			role="form" method="POST">
			<input name="formulario" value="1" type="hidden" /> <input
				name="aseid" value="<?=($_GET['aseid'])?$_GET['aseid']:''?>"
				type="hidden" />

			<div class="col-sm-8">

				<div class="form-group" id="divMetas">
					<label for="estuf" class="col-sm-3 control-label">Estado:</label>
					<div class="col-sm-8 inputarea">
						<select name="estuf" id="estuf" disabled>
							<option value="">Selecionar estado</option>
						<?php
						foreach ( $estados as $chave => $estado ) {
							?>
							<option value="<?=$estado['estuf']?>" <?php echo $AssessoramentoEstado->estuf == $estado['estuf']?'selected':'' ?> ><?=$estado['estdescricao']?></option>
						<?php } ?>
					</select>
					</div>
				</div>

				<div class="form-group" id="divOrgcod">
					<label for="stacod" class="col-sm-3 control-label">Situação:</label>
					<div class="col-sm-8 inputarea">
						<select name="stacod" id="stacod" disabled>
							<option value="">Selecionar Situação</option>						
						<?php
						foreach ( $situacao as $chave => $sit ) {
							?>							
							<option  value="<?=$sit['stacod']?>" <?php echo $AssessoramentoEstado->stacod == $sit['stacod']?'selected':'' ?>><?=$sit['stadsc']?></option>
						<?php } ?>
					</select>
					</div>
				</div>

				<div class="form-group" id="divStatus">
					<label for="asestatus" class="col-sm-3 control-label">Status:</label>
					<div class="col-sm-8 inputarea">
						<select name="asestatus" id="asestatus">
							<option value="">Status</option>
							<option
								<?=($AssessoramentoEstado->asestatus=='A')?'selected':''?>
								value="A">Ativo</option>
							<option
								<?=($AssessoramentoEstado->asestatus=='I')?'selected':''?>
								value="I">Inativo</option>
						</select>
					</div>
				</div>

				<div class="form-group">
					<label class="col-sm-3 control-label">&nbsp;</label>
					<div class="col-sm-8">
						<input type="button" class="btn btn-primary"
							onclick="jQuery('[name=formEdicaoassessoramentoestados]').submit()"
							value="<?=($_GET['aseid']||$_GET['aseid']==='0')?'Modificar':'Cadastrar'?>" />
						&nbsp;&nbsp; <a
							href="/sase/sase.php?modulo=sistema/tabelasdeapoio/listaassessoramentoestados&acao=A"
							class="btn btn-primary">Voltar para Lista</a>
					</div>
				</div>
			</div>
			<div class="col-sm-3">
				<?php
								
				if( !$AssessoramentoEstado->docid && $_GET['aseid'] ){
					$docid = wf_cadastrarDocumento( TPDID_SASE_ASSESSORAMENTO, 'Documento Assistência Técnica' );
					$AssessoramentoEstado->docid = $docid;
					// ver($Assessoramento,d);
					$AssessoramentoEstado->alterar();
					$AssessoramentoEstado->commit();
				}
				
				if($_GET['aseid'] && $AssessoramentoEstado->docid) wf_desenhaBarraNavegacao( $AssessoramentoEstado->docid, array('docid'=>$AssessoramentoEstado->docid) ); ?>
				
			</div>

		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->