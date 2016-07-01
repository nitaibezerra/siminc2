<?php

global $db;
$Assessoramento = new AssessoramentoEstado();

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

if($_REQUEST['aseid']){
    $Assessoramento->carregarPorId($_GET['aseid']);
}


if($Assessoramento->arAtributos['aseleipne'] != null){
    $arquivo = $Assessoramento->arquivo->getDadosArquivo($Assessoramento->arAtributos['aseleipne']);
}

if( !in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) && !in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) && !$_GET['aseid'] ){
	echo '<script>
			alert("Por favor, Selecione um Estado para acessar está Aba.");
			location.href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista";
		  </script>';
	die;
}

// cadastro
if( $_POST['formulario'] ){

	// ver($_POST,d);
	// edicao
	if( !empty($_POST['aseid']) ){

		$Assessoramento->populaAssessoramento( $_POST );
		//ver($Assessoramento, d);
		$retorno = $Assessoramento->atualizarAssessoramentoEstado();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid='+".$_GET['aseid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid='+".$_GET['aseid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid='+".$_GET['aseid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['formulario']);
		unset($_POST['aseid']);
		// ver($_POST,d);
		$novoAssessoramento = array(
			'estuf' => $_POST['estuf'],
			'stacod' => $_POST['stacod'],
			'asestatus' => 'A' );
		$inserido = $Assessoramento->insereAssessoramento( $novoAssessoramento )->inserido;
		// ver($inserido,d);
		if( is_array($inserido) ){

			if( $inserido['info'] == 'duplicidade' ){
				echo "<script>if(confirm('Município já Cadastrado! Utilizar o Workflow para Alteração!')){window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&aseid=".$inserido['aseid']."';}else{window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao';}</script>";
			}else if( $inserido['info'] == 'camposvazios' ){
				echo "<script>alert('Campo ".$inserido['dado']." é Obrigatório.');window.history.back()</script>";
			}
			exit;

		}else if( $inserido ){
			echo "<script>alert('Assessoramento inserido com sucesso!');window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&aseid=".$inserido."';</script>";
			exit;
		}else{
			echo "<script>alert('Houve um erro ao inserir assessoramento.');window.history.back()</script>";
			exit;
		}

	}
}
// --


// carrega objeto

if( $_GET['aseid'] )
	$Assessoramento->carregaAssessoramento( $_GET['aseid'] );

// carrega objeto /

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /


// situacao
$sql = "select * from sase.situacaoassessoramento where stastatus = 'A' order by stacod ASC ";
$situacoes = $db->carregar( $sql );
// situacao /

?>

<!-- html -->
<div id="container">

	<!-- h4><?=($_GET['aseid'])?'':'Nova&nbsp;'?>Assistência Técnica</h4>  -->
	<h4><?=($_GET['aseid'])?'':'Nova&nbsp;'?>Assessoria Estado</h4>


	<div id="formularioDeCadastro">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formCadastroLista" role="form" method="POST" enctype="multipart/form-data">
            <input type="hidden" name="arqid" id="arqid" >
            <input type="hidden" name="acao" id="acao"/>
			<input name="formulario" value="1" type="hidden"/>
            <input type="hidden" name="acao" id="acao" value=""/>
			<input name="aseid" value="<?=@$_GET['aseid']?>" type="hidden"/>

			<div class="col-md-11">

				<div class="form-group" id="divFiltroEstado">
					<label for="estuf" class="col-md-1 control-label">Estado:</label>
					<div class="col-md-3 inputarea">
						<select class="select100" name="estuf" id="estuf" <?=($_GET['aseid'])?'disabled':''?>>
							<option value="">Filtrar estado</option>
							<?php

							foreach( $estados as $chave => $estado ){ ?>
								<option <?=($estado['estuf']==$Assessoramento->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
							<?php }?>
						</select>
					</div>
				</div>


				<div class="form-group" id="divFiltroSituacao">
					<label for="stacod" class="col-md-1 control-label">Situação:</label>
					<div class="col-md-3 inputarea">
						<select class="select100" name="stacod" id="stacod" <?=($_GET['aseid'])?'disabled':''?>>
							<option value="">Filtrar situação</option>
							<?php
							foreach( $situacoes as $chave => $situacao ){ ?>
								<option <?=($situacao['stacod']==$Assessoramento->stacod)?'selected':''?> value="<?=$situacao['stacod']?>"><?=$situacao['stadsc']?></option>
							<?php }?>
						</select>
					</div>
				</div>

                <div class="form-group">
                    <label for="aiddsc" class="col-lg-1 col-md-1 control-label">Lei PNE:</label>

                    <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) || in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) || in_array(PFLCOD_SASE_EXECUTIVO, $perfis)) { ?>
                    <div class="col-md-3 inputarea">
                        <input type="hidden" name="arqid" id="arqid" >
                        <input type="file" class="btn btn-primary start" accept=".pdf" name="file" id="file" title="Selecionar arquivo" />
                    </div>
                    <div class="col-md-6" style="padding-top: 10px;">
                        <?php if ($arquivo['arqid']){ ?>
                            <a onclick="downloadArquivo(<?= $arquivo['arqid'] ?>)" style="margin-left: 15px; cursor: pointer;"><?= $arquivo['arqdescricao'] ?></a>
                            <a onclick="apagaArquivo(<?= $arquivo['arqid'] ?>)" style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                        <?php } ?>
                    </div>
                    <?php } else { ?>
                        <div class="col-md-9" style="padding-top: 10px;">
                            <?php if ($arquivo['arqid']) { ?>
                                <a onclick="downloadArquivo(<?= $arquivo['arqid'] ?>)"
                                   style="margin-left: 15px; cursor: pointer;"><?= $arquivo['arqdescricao'] ?></a>
                            <?php } ?>
                        </div>
                    <?php } ?>
                </div>

<!--				<div class="form-group" id="divFiltroStatus">-->
<!--					<label for="asestatus" class="col-sm-2 control-label">Status:</label>-->
<!--					<div class="col-sm-7 inputarea">-->
<!--						<select name="asestatus" id="asestatus">-->
<!--							<option value="">Status</option>-->
<!--							<option --><?//=($Assessoramento->asestatus=='A')?'selected':''?><!-- value="A">Ativo</option>-->
<!--							<option --><?//=($Assessoramento->asestatus=='I')?'selected':''?><!-- value="I">Inativo</option>-->
<!--						</select>-->
<!--					</div>-->
<!--				</div>-->

				<div class="form-group">
					<label class="col-md-1 control-label">&nbsp;</label>
					<div class="col-md-3">
<!--						--><?//if( in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) == true
//							  || in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) == true ){ ?>
<!---->
<!--							<input type="button" class="btn btn-primary" onclick="enviaFormulario()"-->
<!--								value="--><?//=($_GET['aseid'])?'Modificar':'Cadastrar'?><!--" />-->
<!--							&nbsp;&nbsp;-->
<!---->
<!--						--><?//}?>
                        <input type="button" name="btnSalvar" id="btnSalvar" onclick="enviaFormulario()" value="Salvar" class="btn btn-primary"/>
						<input onclick="    javascript:window.location.href='sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista'"
							value="Voltar" type="button" class="btn btn-primary"/>
					</div>
				</div>

			</div>

			<div class="col-md-1">
				<?php

				if($_GET['aseid'] && $Assessoramento->docid) wf_desenhaBarraNavegacao( $Assessoramento->docid, array('docid'=>$Assessoramento->docid, 'aseid'=>$_GET['aseid']) ); ?>
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
	 * Envia Formulário
	 */
	 function enviaFormulario(){
        jQuery('[name=acao]').val('salva_arquivo');
        jQuery('[name=formulario]').val('');
	 	jQuery('[name=formCadastroLista]').submit();
	 }

    function apagaArquivo(arqid){
        if (confirm('Deseja deletar este arquivo?')) {
            jQuery('[name=acao]').val('apaga_arquivo');
            jQuery('[name=arqid]').val(arqid);
            jQuery('[name=formCadastroLista]').submit();
        }
    }

    function downloadArquivo(arqid){
        jQuery('[name=acao]').val('download_arquivo');
        jQuery('[name=arqid]').val(arqid);
        jQuery('[name=formCadastroLista]').submit();
    }

</script>
<!-- script js / -->