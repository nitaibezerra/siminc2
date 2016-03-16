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


if( $_GET['orgid'] || $_GET['orgid'] === '0' )
	$Orgao->carregarPorId( $_GET['orgid'] );


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['orgid']) ){

		$Orgao->populaOrgao( $_POST );
		$retorno = $Orgao->atualizarOrgao();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A&orgid='+".$_GET['orgid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A&orgid='+".$_GET['orgid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A&orgid='+".$_GET['orgid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['orgid']);
		$Orgao->populaOrgao( $_POST );
		$retorno = $Orgao->cadastrarOrgao();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.history.back();
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso!');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/orgaoedicao&acao=A';
					</script>
				";

			exit;
		}

	}
}
// submit /


// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /


?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoOrgao" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="orgid" value="<?=($_GET['orgid'])?$_GET['orgid']:''?>" type="hidden"/>

			<div class="form-group" id="divOrgao">
				<label for="orgdsc" class="col-sm-3 control-label">Orgão:</label>
				<div class="col-lg-8 inputarea">
					<input style="width:400px;" type="text" maxlength="98" name="orgdsc" id="orgdsc" value="<?=trim($Orgao->orgdsc)?>"/>
				</div>
			</div>

            <div class="form-group" id=divOrgcod>
                <label for="orgcod" class="col-sm-3 control-label">Código:</label>
                <div class="col-lg-8 inputarea">
                    <input style="width:400px;" type="text" maxlength="98" name="orgcod" id="orgcod" value="<?=trim($Orgao->orgcod)?>"/>
                </div>
            </div>

<!--			<div class="form-group" id="divEstado">-->
<!--				<label for="estuf" class="col-sm-3 control-label">Estado:</label>-->
<!--				<div class="col-sm-8 inputarea">-->
<!--					<select name="estuf" id="estuf">-->
<!--						<option value="">Selecionar estado</option>-->
<!--						--><?php //
//						foreach( $estados as $chave => $estado ){ ?>
<!--							<option --><?//=($estado['estuf']==$Orgao->estuf)?'selected':''?><!-- value="--><?//=$estado['estuf']?><!--">--><?//=$estado['estdescricao']?><!--</option>-->
<!--						--><?php //} ?>
<!--					</select>-->
<!--				</div>-->
<!--			</div>-->

			<div class="form-group" id="divStatus">
				<label for="orgstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-lg-8 inputarea">
					<select name="orgstatus" id="orgstatus">
						<option value="">Status</option>
						<option <?=($Orgao->orgstatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($Orgao->orgstatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoOrgao]').submit()" 
						value="<?=($_GET['orgid']||$_GET['orgid']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/orgao&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->