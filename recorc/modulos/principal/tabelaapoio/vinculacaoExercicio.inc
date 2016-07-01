<?php
/**
 * Sistema Receita Orçamentaria
 * @package simec
 */
require_once APPRAIZ . 'recorc/modulos/principal/topo.inc';

/**
 * UO | Fonte | Natureza
 * *
 */
$execucao = $_REQUEST ['execucao'];
$vieid = $_REQUEST ['vieid'];

if($execucao == 'excluir') {
	deletarVinculacaoExercico( $vieid );
}

if($execucao == 'listaEdicao') {
	listaEdicaoVinculacaoExercicio( $vieid );
}

if($execucao == 'mostrarAlterar') {
	$unicod = $_REQUEST ['unicod'];
	$foncod = $_REQUEST ['foncod'];
	$nrccod = $_REQUEST ['nrccod'];
	$prfid = $_REQUEST ['prfid'];
	$vieid = $_REQUEST ['vieid'];
}

// if($_POST ['unicod'] &&($_POST['vieid'] == '' || $_POST['vieid'] == null)) {
if('inserir' == $_POST ['execucao']) {
	$unicod = $_POST ['unicod'];
	$foncod = $_POST ['foncod'];
	$nrccod = $_POST ['nrccod'];
	$prsano = $_SESSION ['exercicio'];
	$prfid = $_REQUEST ['prfid'];
	inserirVinculacaoExercicio( $unicod, $foncod, $nrccod, $prsano, $prfid );
}

// if($_POST ['unicod'] &&($_POST['vieid'] != '' || $_POST['vieid'] != null)) {
if('atualizar' == $_POST ['execucao']) {
	$unicod = $_POST ['unicod'];
	$foncod = $_POST ['foncod'];
	$nrccod = $_POST ['nrccod'];
	$prsano = $_SESSION ['exercicio'];
	$prfid = $_POST ['prfid'];
	$vieid = $_POST ['vieid'];
	editarVinculacaoExercicio( $unicod, $foncod, $nrccod, $prsano, $prfid, $vieid );
}
?>

<script type="text/javascript" language="javascript">
    jQuery(document).ready(function() {
//    inicializarFormulario();
        // -- Ação do botão buscar
        $('#buscar').click(function() {
            window.location = "recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A"
                	+ '&prfid=' + $('#prfid').val()
                    + '&unicod=' + $('#unicod').val()
                    + '&foncod=' + $('#foncod').val()
                    + '&nrccod=' + $('#nrccod').val()
                    + '&execucao=pesquisa'
        });
    });

        //$("#reset").on("click", function() {
        //	var select = jQuery('#unicod');

        //	select.val(jQuery('options:first', select).val());
        //});


</script>
<div class="col-md-12">
<!--	<div class="page-header">
		<h4>Vínculos de previsão por período</h4>
	</div>
	<Br />-->
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>Parâmetros SPO</li>
            <li class="active">Vínculos de previsão por período</li>
        </ol>
	<div class="well">
		<fieldset>
			<form id="formPrincipal" name="formPrincipal" method="POST"
				class="form-horizontal">
				<input type="hidden" name="vieid" id="vieid"
					value="<?php echo $vieid ?>" /> <input type="hidden"
					name="execucao" id="execucao" />
				<div class="form-group">
					<label for="inputExercicio" class="col-lg-2 control-label">Período</label>
					<div class="col-lg-10">
                        <?php
                        $sqlPeriodo = "SELECT
   prfid AS codigo,
   prfdsc AS descricao
FROM
   recorc.periodoreferencia
WHERE
    exercicio = '{$_SESSION['exercicio']}'
ORDER BY
   prfid DESC";
                        $prfid = $_REQUEST['prfid'];
                        $db->monta_combo( 'prfid', $sqlPeriodo, 'S', 'Selecione um período', null, null, null, null, 'N', 'prfid', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required"' );
                        ?>
                    </div>
				</div>
				<div class="form-group">
					<label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária(UO)</label>
					<div class="col-lg-10">
                        <?php
                        $sql = "SELECT
    uni.unicod                    AS codigo,
    uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unicod IN(SELECT DISTINCT unicod
                         FROM recorc.vinculacaoexercicio
                         WHERE exercicio = '{$_SESSION['exercicio']}')
AND uni.unistatus = 'A'
ORDER BY
    uni.unicod";
                        $unicod = $_REQUEST['unicod'];
                        $db->monta_combo( 'unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required"', null,(isset( $unicod ) ? $unicod : null) );
                        ?>
                    </div>
				</div>
				<div class="form-group">
					<label for="inputNatureza" class="col-lg-2 control-label">Natureza da Receita</label>
					<div class="col-lg-10">
                        <?php
                        $sqlNatureza = "SELECT "
                                        . " nrccod as codigo, nrccod || ' - ' || nrcdsc AS descricao "
                                        . " FROM public.naturezareceita"
                                        . " WHERE nrcano = '{$_SESSION['exercicio']}' order by nrccod";
                        $nrccod = $_REQUEST['nrccod'];
                        $db->monta_combo( 'nrccod', $sqlNatureza, 'S', 'Selecione uma Natureza', null, null, null, null, 'N', 'nrccod', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required""', null,(isset( $nrccod ) ? $nrccod : null) );
                        ?> </div>
				</div>
				<div class="form-group">
					<label for="inputSubFonte" class="col-lg-2 control-label">Fonte</label>
					<div class="col-lg-10">

                        <?php
                        $sqFont = " SELECT  foncod as codigo, foncod || ' - ' || fondsc as descricao
                                         FROM public.fonterecurso ORDER BY foncod";
                        $foncod = $_REQUEST['foncod'];
                        $db->monta_combo( 'foncod', $sqFont, 'S', 'Selecione uma Fonte', null, null, null, null, 'N', 'foncod', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required""', null,(isset( $foncod ) ? $foncod : null) );
                        ?>
                    </div>
				</div>
				<div class="form-group">
					<div class="col-lg-10 col-lg-offset-2">
                        <?php if(isset($vieid) & !empty($vieid)): ?>
                            <button class="btn btn-danger" id="voltar"  type="reset"
							onclick="voltarMenu()">Cancelar</button>
						<button class="btn btn-info" id="atualizar"
							onclick="atuaVinculacaoExercicio()">Atualizar</button>
                        <?php else: ?>
                            <button class="btn btn-warning" type="reset" id="reset" onclick="voltarMenu()">Limpar</button>
						<button class="btn btn-success" id="inserir"
							onclick="insertVinculacaoExercicio()">Inserir</button>
						<button class="btn btn-primary" id="buscar" >Buscar</button>
                        <?php endif; ?>
                    </div>
				</div>

			</form>
		</fieldset>
	</div>
    <?php
    echo getFlashMessage();
    require(dirname( __FILE__ ) . "/listagemVinculacaoExercicio.inc");
    ?>
    <script lang="javascript">

        function deleteVinculacaoExercico(id) {
            if(!confirm("Excluir Vinculação?"))
                return false;
            else
                window.location = "recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A&execucao=excluir&vieid=" + id;
            return true;
        }
        setTimeout(function() {
            for(var selector in config) {
                $(selector).chosen(config[selector]);
            }
        }, 300);

        function editVinculacaoExercicio(id) {
            window.location = "recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A&execucao=listaEdicao&vieid=" + id;
        }

        function voltarMenu() {
            window.location = "recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A";
        }

        function insertVinculacaoExercicio() {


//             console.info($('select[required]'));
        	var enviar = true;
            // -- Limpando todos os erros anteriores
            $('#formPrincipal .has-error').removeClass('has-error');
            var enviar = true;
            var msg = '<div class="alert alert-danger text-center">';
            $('select[required]').each(function(){
                if('' == $(this).val()) {
                    enviar = false;
                    var label = $(this).parent().prev().text();
                    $(this).closest('.form-group').addClass('has-error');
                    msg += '<p>O campo <strong>' + label.replace(':', '') + '</strong> é obrigatório e não pode ser deixado em branco.</p>';
                }
            });
            msg += '</div>';
            if(enviar) {
                $('#execucao').attr('value', 'inserir');
                $('#formPrincipal').submit();
            } else {
                $('.modal-body').html(msg);
                $('#modal-alert').modal();
                return false;
            }


//          	document.getElementById("formPrincipal").submit();
//          	$('#execucao').val('inserir');
        }

        function atuaVinculacaoExercicio() {
            document.getElementById("formPrincipal").submit();
            $('#execucao').val('atualizar');
        }
    </script>
</div>