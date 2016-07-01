<?php
/**
 * Arquivo da aba resumo / tramite.
 * $Id: inicio.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

/**
 * Classe de criação de listagens.
 * @see Simec_Listagem
 */
if(!empty($_SESSION['altorc']['pedido']['dados']['paoid'])){
	$paoid = $_SESSION['altorc']['pedido']['dados']['paoid'];
}

// -- Removendo os dados do pedido da sessão
//limpaDadosDoPedido();
// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao')) {
    if ($_REQUEST['requisicao']) {
            enviarPedido($_REQUEST['dados']['paoid'], $_SESSION['exercicio']);
            header('Location: ' . substr($_SERVER['REQUEST_URI'], 0, strrpos($_SERVER['REQUEST_URI'], '&requisicao')));
            die();
    }
    // -- todo: fazer redirecionamento pra tirar mensagem de reenviar formulário
    die();
}

// -- Momento de crédito atual - para limitação de permissões
list($mcridAtual, $mcrtipocancelamento) = momentoDeCreditoAtual($_SESSION['exercicio'], array(), true);
?>
<div class="row col-md-11">
    <div class="col-md-12 panel-group">
        <div class="panel panel-default">
            <div class="panel-heading">
                <a data-toggle="collapse" data-parent="#accordion" href="#collapseDados">
                  Dados do pedido
                </a>
            </div>
            <div id="collapseDados" class="panel-collapse collapse">
                <div class="panel-body">
                    <?php require(dirname(__FILE__) . '/listarDadosPedido.inc'); ?>
                </div>
            </div>
        </div>
        <div class="panel panel-default">
            <div class="panel-heading">
                <a data-toggle="collapse" data-parent="#accordion" href="#collapseJustificativas">
                  Justificativas
                </a>
            </div>
            <div id="collapseJustificativas" class="panel-collapse collapse">
                <div class="panel-body">
                    <?php require(dirname(__FILE__) . '/listarJustificativas.inc'); ?>
                </div>
            </div>
        </div>

        <div class="panel panel-default">
            <div class="panel-heading">
                <a data-toggle="collapse" data-parent="#accordion" href="#collapseSIOP"><span class="glyphicon glyphicon-check"></span> <span class="glyphicon glyphicon-warning-sign"></span> Comunicação com o SIOP</a>
            </div>
            <div id="collapseSIOP" class="panel-collapse collapse">
                <div class="panel-body">
                    <?php require(dirname(__FILE__) . '/listarErros.inc'); ?>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row col-md-1 noprint">
<?php
if (!empty($dadospao['docid'])) {
    wf_desenhaBarraNavegacao(
        $dadospao['docid'],
        array(
            'paoid' => $dadospao['paoid'],
            'exercicio' => $_SESSION['exercicio'],
            'permissao' => ($mcridAtual == $dadospao['mcrid'])
        )
    );
}
?>
</div>
<br style="clear:both" />
<br />
<script type="text/javascript">
function enviarPedido(paoid)
{
    window.location = 'altorc.php?modulo=principal/pedido/inicio&acao=A&aba=resumo&requisicao=enviarPedido&dados[paoid]=' + paoid;
}
$('#print-resumo').click(function(){
    $('.panel-collapse').collapse('show');
    var ajax = false;
    $('[id^="arow"]').each(function(){
        if ($('span.glyphicon-plus', this)[0]) {
            eval($(this).attr('href'));
            ajax = true;
        }
    });
    if (ajax) {
        $(document).ajaxStop(function(){
            window.setTimeout(function(){
                window.print();
            }, 1000);
        });
    } else {
        window.setTimeout(function(){
            window.print();
        }, 1000);
    }
});
</script>
<?php
$fm = new Simec_Helper_FlashMessage("altorc/pedido");
echo $fm->getMensagens();
?>
<br style="clear:both" />