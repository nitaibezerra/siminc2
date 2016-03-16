<?php
//ver($unicod,d);
listagemLocalizadores();
function listagemLocalizadores($unicod) {
    $sql = "SELECT 
	    DISTINCT ssl.sslid AS codigo, 
			     ssl.unicod || '.' || ssl.prgcod || '.<b>' || ssl.acacod || '</b>.' || ssl.loccod AS programatica,
			     ssl.acacod || ' (' || ssl.loccod || ') - ' || aca.acadsc || ' - ' || loc.locdsc,
			     to_char(ssl.dataultimatualizacao,'DD/MM/YYYY') as ultimaatualizacao,
			     esd.esddsc as estado,
			     NULL as siop
		    FROM acomporc.snapshotlocalizador ssl
	   LEFT JOIN monitora.acao aca ON aca.acaid = ssl.acaid AND aca.loccod = ssl.loccod
	   LEFT JOIN public.localizador loc ON loc.loccod = ssl.loccod
	   LEFT JOIN acomporc.acompanhamentolocalizador acl ON acl.sslid = ssl.sslid
	   LEFT JOIN workflow.documento doc ON doc.docid = acl.docid
	   LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	   	   WHERE ssl.unicod = '{$unicod}' 
	    ORDER BY programatica";
//ver($sql);
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
	$listagem->setCabecalho(array('Programática', 'Localizador', 'Última Atualização', 'Status do Acomponhamento', 'Status no SIOP'));
	$listagem->addAcao('edit', 'atribuiResponsabilidade');
	$listagem->addAcao('view', array('func' => 'detalharLocalizadores', 'extra-paramns' => array('unicod')));
    $listagem->setQuery($sql);
	$listagem->render();
}
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . "includes/funcoesspo.php";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('acompanhamento/acao');

switch ($_REQUEST['requisicao']) 
{
	case 'detalharLocalizadores':
		listagemLocalizadores($_REQUEST['dados'][0]);
		die;
    case 'salvar':
        $modelDecisao->popularDadosObjeto();

        $sucesso = $modelDecisao->salvar();

        $modelDecisao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");
		die;
    case 'deletar':
        $modelDecisao->popularDadosObjeto();
        $modelDecisao->decstatus = 'I';
        $sucesso = $modelDecisao->salvar();

        $modelDecisao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");
		die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<style>
table.tabela-listagem tbody td {
	text-align: left;
}
</style>
<script type="text/javascript">
$(document).ready(function() {
	$('#prfid').on('change', function() {
		$("#form-search").submit();
	});
});

function detalharLocalizadores(id)
{
	if (id) {
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=detalharLocalizadores&unicod=" + id,
            async: false,
			success: function(resp) {
                return html(resp);
            }
        });
    }
}

function trocarResponsavel(id)
{
	$('#responsavel').modal('show');
}

function deletarDecisao(id)
{
    if (confirm('Deseja realmente excluir o registro?')) {
        window.location = 'demandasfies.php?modulo=principal/apoio/decisao&acao=A&action=deletar&decid=' + id;
    }
}
</script>
<div class="col-lg-12">
	<?php echo $fm->getMensagens(); ?>
	<div class="row">
		<div class="row col-md-12">
			<form id="form-search" method="post" class="form-horizontal">
				<div class="well">
					<fieldset>
						<div class="form-group">
							<label for="prfid" class="col-lg-2 control-label">Período de Referência:</label>
							<div class="col-lg-10">
							<?php
								$_SESSION['exercicio'] = 2014;
								$sql = "SELECT distinct prf.prfid as codigo, 
											   prf.prftitulo || ' : ' || to_char(prf.prfinicio,'DD/MM/YYYY')  || ' a ' || to_char(prf.prffim,'DD/MM/YYYY') as descricao
									 	  FROM acomporc.periodoreferencia prf
										 WHERE prf.prsano = '{$_SESSION['exercicio']}'";

								$options = array(
									'titulo' => 'Selecione ao menos um período de referência',
									'obrig' => 'S',
								);
								
								inputCombo('prfid', $sql, $_REQUEST['prfid'], 'prfid', $options);
							?>
							</div>
						</div>
				    </fieldset>
			    </div>	
			</form>
			<?php 
				if (isset($_REQUEST['prfid']) && $_REQUEST['prfid'])
				{
					$sql = "Identificação";
				ver($sql);
			        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
			        //$listagem->setCabecalho(array('Identificação'));
//			        $listagem->addAcao('user', 'trocarResponsavel');
			        $listagem->addAcao('plus', 'detalharLocalizadores');
			        $listagem->setQuery($sql);
			        $listagem->render();
				}
			?>
		</div>
	</div>
</div>

<div class="modal fade" id="responsavel" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title">Alterar Responsável</h4>
            </div>
            <div class="modal-body">
	            <form id="form-save" method="post" class="form-horizontal">
					<input name="requisicao" type="hidden" value="salvarResponsavel">
					<div class="well">
						<fieldset>
							<div class="form-group">
								<label for="prfid" class="col-lg-2 control-label"></label>
								<div class="col-lg-10">
									<span class="label label-info">Coordenador</span>
								</div>
							</div>
							<div class="form-group">
								<label for="prfid" class="col-lg-2 control-label">CPF:</label>
								<div class="col-lg-4">
									<?php echo campo_texto('usucpf', "S", "S", "CPF", 16, 14, "", "", '', '', 0, 'id="usucpf" class="form-control"'); ?>
								</div>
							</div>
							<div class="form-group">
								<label for="prfid" class="col-lg-2 control-label">Nome:</label>
								<div class="col-lg-10">
									<?php echo campo_texto('usunome', "S", "S", "Nome completo", null, null, "", "", '', '', 0, 'id="usunome" disabled="disabled" class="form-control"'); ?>
								</div>
							</div>
							<div class="form-group">
								<label for="prfid" class="col-lg-2 control-label">Telefone:</label>
								<div class="col-lg-1">
								</div>
								<div class="col-lg-4">
								</div>
							</div>
							<div class="form-group">
								<label for="prfid" class="col-lg-2 control-label">Email:</label>
								<div class="col-lg-10">
								</div>
							</div>
						</fieldset>
				    </div>	
				</form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar</button>
            	<button type="button" class="btn btn-success" data-dismiss="modal">Salvar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->