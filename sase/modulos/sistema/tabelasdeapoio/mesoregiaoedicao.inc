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

if( $_GET['mesid'] || $_GET['mesid'] === '0' ) {
    $Mesoregiao->carregarPorId($_GET['mesid']);
    $sql = "select orgid, estuf from sase.orgaoestado where oesid = ".$Mesoregiao->oesid;
    $res = $db->pegaLinha($sql);
    $orgid = $res['orgid'];
    $estuf = $res['estuf'];
}


// submit
if( $_POST['formulario'] ){
	unset($_POST['formulario']);

	// edicao
	if( !empty($_POST['mesid']) ){

        $sql = "select oesid from sase.orgaoestado where orgid = {$_POST['orgid']} and estuf = '{$_POST['estuf']}'";
        $oesid = $db->pegaUm($sql);

        if(empty($oesid)){
            $sql = "insert into sase.orgaoestado (orgid, estuf) values ({$_POST['orgid']}, {$_POST['estuf']}) returning oesid";
            $oesid = $db->pegaUm($sql);
        }

        $dados = array(
            'mesid' => $_POST['mesid'],
            'oesid' => $oesid,
            'mescod' => $_POST['mescod'],
            'mesdsc' => $_POST['mesdsc'],
            'mescor' => $_POST['mescor'],
            'messtatus' => $_POST['messtatus']
        );

		$Mesoregiao->populaMesoregiao( $dados );
		$retorno = $Mesoregiao->atualizarMesoregiao();
		if( !is_bool($retorno) ){
			echo "
				<script>
					alert('Existem campos vazios.');
					window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A&mesid='+".$_GET['mesid'].";
				</script>
			";
			exit;
		}else{
			if( $retorno )
				echo "
					<script>
						alert('Solicitação realizada com sucesso.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A&mesid='+".$_GET['mesid'].";
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A&mesid='+".$_GET['mesid'].";
					</script>
				";
			exit;
		}

	}else{ // cadastro

		unset($_POST['mesid']);

        $sql = "select oesid from sase.orgaoestado where orgid = {$_POST['orgid']} and estuf = '{$_POST['estuf']}'";
        $oesid = $db->pegaUm($sql);

        if(empty($oesid)){
            $sql = "insert into sase.orgaoestado (orgid, estuf) values ({$_POST['orgid']}, '{$_POST['estuf']}') returning oesid";
            $oesid = $db->pegaUm($sql);
        }

        $dados = array(
            'oesid' => $oesid,
            'mescod' => $_POST['mescod'],
            'mesdsc' => $_POST['mesdsc'],
            'mescor' => $_POST['mescor'],
            'messtatus' => $_POST['messtatus']
        );

//        ver($dados, d);

		$Mesoregiao->populaMesoregiao( $dados );
		$retorno = $Mesoregiao->cadastrarMesoregiao();
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
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A';
					</script>
				";
			else
				echo "
					<script>
						alert('Houve um erro com a solicitação.');
						window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiaoedicao&acao=A';
					</script>
				";

			exit;
		}

	}
}
// submit /

// orgao
$sql = "select orgid, orgdsc from sase.orgao order by orgdsc ASC";
$orgaos = $db->carregar($sql);
// orgao /


// estado
$sql = "select estuf, estdescricao from territorios.estado order by estuf";
$estados = $db->carregar($sql);
// estado /

?>

<div id="container">

	<!-- ------------------------------- FORM ------------------------------- -->

	<div id="formularioFiltros">
		<form class="form-horizontal" name="formEdicaoMesoregiao" role="form" method="POST">
			<input name="formulario" value="1" type="hidden"/>
			<input name="mesid" value="<?=($_GET['mesid'])?$_GET['mesid']:''?>" type="hidden"/>

			<div class="form-group" id="divMesoregiao">
				<label for="mesdsc" class="col-sm-3 control-label">Região:</label>
				<div class="col-sm-8 inputarea">
					<input name="mesdsc" id="mesdsc" type="text" maxlength="146" value="<?=(@$Mesoregiao->mesdsc)?$Mesoregiao->mesdsc:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divMescod">
				<label for="mesdsc" class="col-sm-3 control-label">Código Região:</label>
				<div class="col-sm-8 inputarea">
					<input name="mescod" id="mescod" value="<?=(@$Mesoregiao->mescod)?$Mesoregiao->mescod:''?>"/>
				</div>
			</div>

            <div class="form-group" id="divEstado">
                <label for="estuf" class="col-sm-3 control-label">Estado:</label>
                <div class="col-sm-8 inputarea">
                    <select name="estuf" id="estuf">
                        <option value="">Filtrar estado</option>
                        <?php
                        foreach( $estados as $chave => $estado ){ ?>
                            <option <?=($estado['estuf']==@$estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>

			<div class="form-group" id="divOrgao">
				<label for="orgid" class="col-sm-3 control-label">Orgão:</label>
				<div class="col-sm-8 inputarea">
					<select name="orgid" id="orgid">
						<option value="">Filtrar orgão</option>
						<?php 
						foreach( $orgaos as $chave => $orgao ){ ?>
							<option <?=($orgao['orgid']==@$orgid)?'selected':''?> value="<?=$orgao['orgid']?>"><?=$orgao['orgdsc']?></option>
						<?php } ?>
					</select>
				</div>
			</div>

			<div class="form-group" id="divMescor">
				<label for="mescor" class="col-sm-3 control-label">Cor:</label>
				<div class="col-sm-8 inputarea">
					<input name="mescor" id="mescor" value="<?=($Mesoregiao->mescor)?$Mesoregiao->mescor:''?>"/>
				</div>
			</div>

			<div class="form-group" id="divMesstatus">
				<label for="messtatus" class="col-sm-3 control-label">Status:</label>
				<div class="col-sm-8 inputarea">
					<select name="messtatus" id="messtatus">
						<option value="">Status</option>
						<option <?=(@$Mesoregiao->messtatus=='A')?'selected':''?> value="A">Ativo</option>
						<option <?=(@$Mesoregiao->messtatus=='I')?'selected':''?> value="I">Inativo</option>
					</select>
				</div>
			</div>

			<div class="form-group">
				<label class="col-sm-3 control-label">&nbsp;</label>
				<div class="col-sm-8">
					<input type="button" class="btn btn-primary" onclick="jQuery('[name=formEdicaoMesoregiao]').submit()" 
						value="<?=($_GET['mesid']||$_GET['mesid']==='0')?'Modificar':'Cadastrar'?>" />
					&nbsp;&nbsp;
					<a href="/sase/sase.php?modulo=sistema/tabelasdeapoio/mesoregiao&acao=A" class="btn btn-primary">Voltar para Lista</a>
				</div>
			</div>
		</form>
	</div>

	<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<!-- /html -->