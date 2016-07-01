<?php
/**
 * Tela de dados da prefeitura
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderEntidade        = new Par3_Controller_Entidade();
$controllerEquipeLocal = new Par3_Controller_EquipeLocal();
$modelEquipeLocal      = new Par3_Model_EquipeLocal();

$inuid = $_REQUEST['inuid'];

switch ($_REQUEST['req']) {
	case 'carregarCargos':
	    ob_clean();
	    $modelCargo = new Par3_Model_EquipeLocalCargo();
	    echo $modelCargo->carregarJSONCombo($_REQUEST);
	    die();
	    break;
	case 'novoEquipe':
	    $controllerEquipeLocal->formNovoEquipeLocal($_REQUEST);
	    break;
	case 'salvar':
	    $controllerEquipeLocal->salvar($_POST);
	    break;
	case 'inativar':
	    $controllerEquipeLocal->invativar($_REQUEST);
	    break;
	default:
	    $arrPost              = array();
	    $arrPost['inuid']     = $inuid;
	    $arrPost['elostatus'] = 'I';
	    $listaHistoricoEquipe = $modelEquipeLocal->carregaArrayEquipe($arrPost);
	    break;
}
?>
<style>
.esconde{
    cursor:pointer;
}
.esconde:hover{
    background-color: #F5F5DC;
}
</style>
<input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
<div class="ibox">
	<div class="ibox-title esconde" tipo="integrantes">
	    <h3>Equipe Local - Integrantes</h3>
	</div>
	<div class="ibox-content" id="integrantes">
		<?php
		$_REQUEST['elostatus'] = 'A';
		$controllerEquipeLocal->listaEquipe($_REQUEST);
		?>
	</div>
	<div class="ibox-footer">
		<button type="button" class="btn btn-success novo" <?php echo $disabled;?>><i class="fa fa-plus-square-o"></i> Inserir Integrante</button>
	</div>
</div>
<?php
if (count($listaHistoricoEquipe) > 0 && is_array($listaHistoricoEquipe)) { ?>
<div class="ibox">
	<div class="ibox-title">
	    <h3>Equipe Local - Histórico Modificações</h3>
	</div>
	<div class="ibox-content">
		<table class="table table-hover dataTable">
			<thead>
				<tr>
					<th>CPF</th>
					<th>Nome</th>
					<th>Email</th>
					<th>Data</th>
				</tr>
			</thead>
			<?php foreach ($listaHistoricoEquipe as $historico) : ?>
			<tr>
				<td><?php echo formatar_cpf($historico['entcpf']); ?></td>
				<td><?php echo $historico['entnome']; ?></td>
				<td><?php echo $historico['entemail']; ?></td>
				<td><?php echo $historico['entdtinativacao']; ?></td>
			</tr>
			<?php endforeach;?>
		</table>
	</div>
</div>
<?php }; ?>
<script>
$(document).ready(function()
{

    /**
     * @see .esconde
     *
     * Controla o atributo display hide/show de todos elementos com o id = ao tipo.
     *
     * @type class esconde
     * @param tipo idelemento(s)
     */
    $('.esconde').click(function()
	{
	    var id = $(this).attr('tipo');

	    if ($('#'+id).css('display') == 'none') {
	    	$('#'+id).show();
		} else {
	    	$('#'+id).hide();
		}

	});

    $('.novo').click(function(){
    	$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: '&req=novoEquipe&inuid='+$('#inuid').val(),
       		async: false,
       		success: function(resp){
    			$('#html_modal-form').html(resp);
    		    $('#modal-form').modal();
       		}
     	});
    });

    $('#html_modal-form').on('click', '.salvar', function(){
        if ($('#formEquipe').validate()) {
            if ($('#eseid').val() == '') {
                swal('Erro', 'Preencha o seguimento', 'error');
                return false;
            }
            if ($('#ecaid').val() == '') {
                swal('Erro', 'Preencha o cargo', 'error');
                return false;
            }
        	$('#formEquipe').submit();
        }
    });

});

function inativaIntegranteEquipe(id)
{
    var inuid  = $('#inuid').val();
    var url    = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid+'&menu=equipe';
    var action = '&req=inativar';
    window.location.href = url+action+'&eloid='+id;
}
</script>