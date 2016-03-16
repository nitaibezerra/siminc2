<?php

global $db;
$Assessoramento = new Assessoramento();

$pfls = arrayPerfil();

if($_REQUEST['assid']){
    $Assessoramento->carregarPorId($_GET['assid']);
}

if($Assessoramento->arAtributos['assleipne'] != null){
    $arquivo = $Assessoramento->arquivo->getDadosArquivo($Assessoramento->arAtributos['assleipne']);
}


if( !in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) && !in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) && !$_GET['assid'] ){
	echo '<script>
			alert("Por favor, Selecione um Município para acessar está Aba.");
			location.href="sase.php?modulo=principal/assessoramento&acao=A&aba=lista";
		  </script>';
	die;
}

// cadastro
if( $_POST['formulario'] ){

	// ver($_POST,d);
	// edicao
	if( !empty($_POST['assid']) ){

		$Assessoramento->populaAssessoramento( $_POST );
		// ver($Assessoramento,d);
		$retorno = $Assessoramento->atualizarAssessoramento();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid='+".$_GET['assid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid='+".$_GET['assid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid='+".$_GET['assid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['formulario']);
		unset($_POST['assid']);
		// ver($_POST,d);
		$novoAssessoramento = array(
			'estuf' => $_POST['estuf'],
			'muncod' => $_POST['muncod'],
			'stacod' => $_POST['stacod'],
			'assstatus' => 'A' );
		$inserido = $Assessoramento->insereAssessoramento( $novoAssessoramento )->inserido;
		// ver($inserido,d);
		if( is_array($inserido) ){

			if( $inserido['info'] == 'duplicidade' ){
				echo "<script>if(confirm('Município já Cadastrado! Utilizar o Workflow para Alteração!')){window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid=".$inserido['assid']."';}else{window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao';}</script>";
			}else if( $inserido['info'] == 'camposvazios' ){
				echo "<script>alert('Campo ".$inserido['dado']." é Obrigatório.');window.history.back()</script>";
			}
			exit;

		}else if( $inserido ){
			echo "<script>alert('Assessoramento inserido com sucesso!');window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid=".$inserido."';</script>";
			exit;
		}else{
			echo "<script>alert('Houve um erro ao inserir assessoramento.');window.history.back()</script>";
			exit;
		}

	}
}
// --


// carrega objeto
if( $_GET['assid'] )
	$Assessoramento->carregaAssessoramento( $_GET['assid'] );
// carrega objeto /

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// municipio
$sql = "select * from territorios.municipio";
if ( $_GET['estuf'] ) $sql .= " where estuf = '" . $_GET['estuf'] . "' ";
elseif ( $Assessoramento->estuf ) $sql .= " where estuf = '" . $Assessoramento->estuf . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);
if( $_GET['filtroAjax'] ){ 
	ob_clean(); ?>
	<select class="select100" name="muncod" id="muncod" <?=($_GET['assid'])?'disabled':''?>>
		<option value="">Município:</option>
		<?php  
		foreach( $municipios as $chave => $municipio ){ ?>
			<option <?=($municipio['muncod']==$_GET['muncod'])?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
		<?php }?>
	</select>
<?php die();}
// municipio /

// situacao
$sql = "select * from sase.situacaoassessoramento where stastatus = 'A' order by stacod ASC ";
$situacoes = $db->carregar( $sql );
// situacao /

?>

<!-- html -->
<div id="container">

	<h4><?=($_GET['assid'])?'':'Nova&nbsp;'?>Assistência Técnica</h4>


	<div id="formularioDeCadastro">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formCadastroLista" role="form" method="POST" enctype="multipart/form-data">
			<input name="formulario" value="" type="hidden"/>
            <input type="hidden" name="acao" id="acao" value=""/>
			<input name="assid" value="<?=@$_GET['assid']?>" type="hidden"/>
			
			<div class="col-md-11">

				<div class="form-group" id="divFiltroEstado">
					<label for="estuf" class="col-md-1 control-label">Estado:</label>
					<div class="col-md-3 inputarea">
						<select class="select100" name="estuf" id="estuf" onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}" <?=($_GET['assid'])?'disabled':''?>>
							<option value="">Filtrar estado</option>
							<?php 
							foreach( $estados as $chave => $estado ){ ?>
								<option <?=($estado['estuf']==$Assessoramento->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
							<?php }?>
						</select>
					</div>
				</div>

				<div class="form-group" id="divFiltroMunicipio">
					<label for="muncod" class="col-md-1 control-label">Município:</label>
					<div class="col-md-3 inputarea">
						<?php if(!$Assessoramento->estuf){ echo "Selecione um Estado"; }else{ ?>
						<select class="select100" name="muncod" id="muncod" <?=($_GET['assid'])?'disabled':''?>>
							<option value="">Filtrar município</option>
							<?php 
							foreach( $municipios as $chave => $municipio ){ ?>
								<option <?=($municipio['muncod']==$Assessoramento->muncod)?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
							<?php }?>
						</select>
						<?php } ?>
					</div>
				</div>

				<div class="form-group" id="divFiltroSituacao">
					<label for="stacod" class="col-md-1 control-label">Situação:</label>
					<div class="col-md-3 inputarea">
						<?php
						$estadoBuscado = '';
						
						if( $Assessoramento->docid )
							$estado = wf_pegarEstadoAtual( $Assessoramento->docid );

						if( $estado['esdid'] ){
							$sql = " select stacod from sase.situacaoassessoramento where esdid = '{$estado['esdid']}' ";
							$estadoBuscado = $db->carregar( $sql );
							$estadoBuscado = $estadoBuscado[0]['stacod'];
						}
						?>
						<select class="select100" name="stacod" id="stacod" <?=($_GET['assid'])?'disabled':''?>>
							<option value="">Filtrar situação</option>
							<?php 
							foreach( $situacoes as $chave => $situacao ){ ?>
								<option <?=($situacao['stacod']==$estadoBuscado)?'selected':''?> value="<?=$situacao['stacod']?>"><?=$situacao['stadsc']?></option>
							<?php }?>
						</select>
					</div>
				</div>

                <div class="form-group">
                    <label for="aiddsc" class="col-lg-1 col-md-1 control-label">Lei PNE:</label>

                    <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls) || in_array(PFLCOD_SASE_EXECUTIVO, $pfls)) { ?>
                        <div class="col-md-3 inputarea">
                            <input type="hidden" name="arqid" id="arqid" >
                            <input type="file" class="btn btn-primary start" accept=".pdf" name="file" id="file" title="Selecionar arquivo" />
                        </div>
                        <div class="col-md-6" style="padding-top: 10px;">
                            <?php if ($arquivo['arqid']) { ?>
                                <a onclick="downloadArquivo(<?= $arquivo['arqid'] ?>)"
                                   style="margin-left: 15px; cursor: pointer;"><?= $arquivo['arqdescricao'] ?></a>
                                <a onclick="apagaArquivo(<?= $arquivo['arqid'] ?>)"
                                   style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
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
<!--					<label for="assstatus" class="col-sm-2 control-label">Status:</label>-->
<!--					<div class="col-sm-7 inputarea">-->
<!--						<select name="assstatus" id="assstatus" --><?//=($_GET['assid'])?'disabled':''?><!-->
<!--							<option value="">Status</option>-->
<!--							<option --><?//=($Assessoramento->assstatus=='A')?'selected':''?><!-- value="A">Ativo</option>-->
<!--							<option --><?//=($Assessoramento->assstatus=='I')?'selected':''?><!-- value="I">Inativo</option>-->
<!--						</select>-->
<!--					</div>-->
<!--				</div>-->

				<div class="form-group">
					<label class="col-md-1 control-label">&nbsp;</label>
					<div class="col-md-3">
<!--						--><?//if( in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) == true ){ ?>
<!--							  -->
<!--							<input type="button" class="btn btn-primary" onclick="enviaFormulario()" -->
<!--								value="--><?//=($_GET['assid'])?'Modificar':'Cadastrar'?><!--" />-->
<!--							&nbsp;&nbsp;-->
<!--							-->
<!--						--><?//}?>
                        <input type="button" name="btnSalvar" id="btnSalvar" onclick="enviaFormulario()" value="Salvar" class="btn btn-primary"/>
						<input onclick="javascript:window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=lista'"
							value="Voltar" type="button" class="btn btn-primary"/>
					</div>
				</div>

			</div>
			
			<div class="col-md-1">
				<?php 

				if( !$Assessoramento->docid && $_GET['assid'] ){
					$docid = wf_cadastrarDocumento( TPDID_SASE_ASSESSORAMENTO, 'Documento Assistência Técnica' );
					$Assessoramento->docid = $docid;
					// ver($Assessoramento,d);
					$Assessoramento->alterar();
					$Assessoramento->commit();
				}

				if($_GET['assid'] && $Assessoramento->docid) wf_desenhaBarraNavegacao( $Assessoramento->docid, array('docid'=>$Assessoramento->docid) ); ?>
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

	/**
	 * Filtro de municipios com a mudanca do estado
	 */
	function filtraMunicipios( estuf ){
		jQuery.ajax({
			url:'',
			type:'GET',
			data:{filtroAjax:true,estuf:estuf,muncod:'<?=$Assessoramento->muncod?>'},
			success: function( resposta ){
				jQuery('#divFiltroMunicipio .inputarea').html( resposta );
			}
		});
	}

	jQuery('document').ready(function(){
		if( jQuery('#estuf').val() != '' ){
			filtraMunicipios( jQuery('#estuf').val() );
		}
	});
</script>
<!-- script js / -->