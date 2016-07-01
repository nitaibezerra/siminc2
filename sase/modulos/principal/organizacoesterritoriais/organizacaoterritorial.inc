<?php

// --------------- Dependências
include_once '../../sase/classes/Territorio.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


global $db;
$Territorio = new Territorio();


if( $_GET['terid'] ) {
    $Territorio->carregarTerritorio($_GET['terid']);
} else {
    echo "
				<script>
					alert('Selecione uma organização territorial.');
					window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=lista';
				</script>
			";
    exit();
}


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['terid']) ){

//        ver($_POST['terid'], $_POST['mesid'], d);
        $Territorio->carregarPorId($_POST['terid']);

        $Territorio->mesid = $_POST['mesid'];

		// ver($Territorio,d);
		$retorno = $Territorio->atualizarTerritorio();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&terid='+".$_GET['terid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&terid='+".$_GET['terid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&terid='+".$_GET['terid'].";
					</script>
				";
			exit;
		}

	}
}
// submit /

// mesoregiao
$sql = <<<DML
    select
        mes.mesid,
        mes.oesid,
        mes.mescod,
        mes.mesdsc,
        mes.mescor,
        mes.messtatus
    from sase.orgaoestado oes
    inner join sase.mesoregiao mes on oes.oesid = mes.oesid
    where oes.estuf = '{$Territorio->estuf}'
    order by mesdsc asc
DML;
$mesoregioes = $db->carregar( $sql );
// mesoregiao /

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// municipio
$sql = "select * from territorios.municipio";
if( $_GET['filtroEstado'] || $Territorio->estuf ) $sql .= " where estuf = '" . (($_GET['filtroEstado'])?$_GET['filtroEstado']:$Territorio->estuf) . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);
if( $_GET['filtroAjax'] ){ 
	ob_clean(); ?>
	<select name="muncod" id="muncod">
		<option value="">Município:</option>
		<?php 
		foreach( $municipios as $chave => $municipio ){ ?>
			<option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
		<?php }?>
	</select>
<?php die();}
// municipio /

?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoTerritorio" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="terid" value="<?=($_GET['terid'])?$_GET['terid']:''?>" type="hidden"/>

            <div class="form-group" id="divEstado">
                <label for="estuf" class="col-sm-3 control-label">Estado:</label>
                <div class="col-sm-8 inputarea">
                    <select name="estuf" id="estuf" disabled onchange="filtraMunicipios(this.value)">
                        <option value="">Selecionar estado</option>
                        <?php
                        foreach( $estados as $chave => $estado ){ ?>
                            <option <?=($estado['estuf']==$Territorio->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>

			<div class="form-group" id="divFiltroTerritorio">
				<label for="mesid" class="col-sm-3 control-label">Mesorregião:</label>
				<div class="col-sm-8 inputarea">
					<select name="mesid" id="mesid">
						<option value="">Filtrar Mesorregião</option>
						<?php 
						foreach( $mesoregioes as $chave => $mesoregiao ){ ?>
							<option <?=($mesoregiao['mesid']==$Territorio->mesid)?'selected':''?> value="<?=$mesoregiao['mesid']?>"><?=$mesoregiao['mesdsc']?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divFiltroMunicipio">
				<label for="muncod" class="col-sm-3 control-label">Município:</label>
				<div class="col-sm-8 inputarea">
					<?php if(!$Territorio->muncod){ echo "Selecione um Estado"; }else{ ?>
					<select name="muncod" id="muncod" disabled>
						<option value="">Selecione um Município</option>
						<?php 
						foreach( $municipios as $chave => $municipio ){ ?>
							<option <?=($municipio['muncod']==$Territorio->muncod)?'selected':'';?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
						<?php }?>
					</select>
					<?php } ?>
				</div>
			</div>

			<div class="form-group" id="divStatus">
				<label for="terstatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="terstatus" id="terstatus" disabled>
						<option value="">Status</option>
						<option <?=($Territorio->terstatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=($Territorio->terstatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="salvar()"
						value="Salvar" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=lista" class="btn btn-primary">Voltar</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->

<!-- js -->
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

    function salvar(){
        jQuery('[name=formEdicaoTerritorio]').submit();
    }
</script>
<!-- /js -->
