<?php
function wf_desenhaBarraNavegacaoBootstrap($docid, array $dados, $ocultar = null, $titulo = null) {
    /*
     * $ocultar - Define quais areas serão ocultadas. ex.: $ocultar['historico'] = true;
     *
     * --- Definidas ---
     * historico       : Oculta linha contendo informações obre o historico
     */

    global $db;
    $docid = (integer) $docid;

    $ocultar = is_array($ocultar) ? $ocultar : array();

    // captura dados gerais
    $documento = wf_pegarDocumento($docid);
    if (!$documento) {
        ?>
        <span class="badge" style="white-space: normal !important;">
            Documento inexistente.
        </span>
        <?php
        return;
    }

    $estadoAtual = wf_pegarEstadoAtual($docid);
    //$estados = wf_pegarProximosEstadosPossiveis( $docid, $dados );
    if (is_array($ocultar) && $ocultar['acoes']) {
        $estados = array();
    } else {
        $estados = wf_pegarProximosEstados($docid, $dados);
    }
    $modificacao = wf_pegarUltimaDataModificacao($docid);
    $usuario = wf_pegarUltimoUsuarioModificacao($docid);
    $comentario = trim(substr(wf_pegarComentarioEstadoAtual($docid), 0, 50)) . "...";

    $dadosHtml = serialize($dados);

    $sql = "SELECT sisdiretorio FROM seguranca.sistema WHERE sisid = {$_SESSION['sisid']}";
    $sisdiretorio = $db->pegaUm($sql);

    $temPreAcao = false;
    foreach ($estados as $estado) {
        if ($estado['aedpreacao'] != '') {
            $temPreAcao = true;
        }
    }
    if ($temPreAcao) {
        ?>
        <?php
    }
    ?>
    <script type="text/javascript">
    
    <?php if ($temPreAcao) { ?>
            jQuery.noConflict();

            jQuery(document).ready(function () {
                jQuery('.preacao').click(function () {
                    var dados = jQuery(this).attr('id').split(';');
                    var docid = dados[0];
                    var aedid = dados[1];
                    var aedpreacao = dados[2];
                    aedpreacao = aedpreacao.split('(');
                    aedpreacao = aedpreacao[0];
                    var esdid = dados[3];
                    var acao = dados[4];
                    // 				window.html 		= '';
                    var html = '';
                    jQuery.ajax({
                        type: "POST",
                        url: 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
                        data: 'req_ajax_workflow=form_' + aedpreacao + '&docid=' + docid + '&aedid=' + aedid + '&esdid=' + esdid + '&dados=<?= simec_json_encode($dados) ?>',
                        async: false,
                        success: function (msg) {
                            // 						window.html = '<center>'+
                            html = '<center>' +
                                    '<div id="aguardando" style="display:none; position: absolute; background-color: white; height:98%; width:95%; opacity:0.4; filter:alpha(opacity=40)" >' +
                                    '<div style="margin-top:250px; align:center;">' +
                                    '<img border="0" title="Aguardando" src="../imagens/carregando.gif">Carregando...</div>' +
                                    '</div>' +
                                    '</center>' +
                                    '<form method="POST" id="form' + aedpreacao + '" enctype="multipart/form-data" name="form' + aedpreacao + '">' +
                                    '<input type="hidden" name="docid" 				value="' + docid + '"/>' +
                                    '<input type="hidden" name="req_ajax_workflow" 	value="' + aedpreacao + '"/>' +
                                    msg +
                                    '</form>';
                            jQuery("#div_dialog_workflow").html(html);
                            jQuery('#div_dialog_workflow').show();
                            jQuery("#div_dialog_workflow").dialog({
                                resizable: true,
                                width: 700,
                                modal: true,
                                show: {effect: 'drop', direction: "up"},
                                buttons: {
                                    "Fechar": function () {
                                        jQuery(this).dialog("close");
                                    },
                                    "Tramitar": function () {
                                        jQuery('select[multiple="multiple"]').children().attr('selected', true);

                                        jQuery('select[multiple="multiple"],[class="required"]').each(function () {
                                            jQuery(this).find('[value=""]').remove();
                                        });

                                        jQuery("#form" + aedpreacao).validate();

                                        if (!jQuery("#form" + aedpreacao).valid()) {
                                            alert('Preencha os campos obrigatórios.');
                                            return false;
                                        }
                                        if (confirm('Deseja prosseguir com a tramitação?')) {
                                            jQuery('#aguardando').show();
                                            jQuery.ajax({
                                                type: "POST",
                                                url: 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php',
                                                data: 'req_ajax_workflow=' + aedpreacao + '&' + jQuery('#form' + aedpreacao).serialize(),
                                                async: false,
                                                dataType: 'json',
                                                success: function (msg) {

                                                    //var data = jQuery.parseJSON(msg);
                                                    var data = msg;

                                                    if (data.boo == true) {
                                                        if (data.msg != '') {
                                                            alert(data.msg);
                                                        }
                                                        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
                                                                '?aedid=' + aedid +
                                                                '&docid=' + docid +
                                                                '&esdid=' + esdid +
                                                                '&verificacao=<?php echo urlencode($dadosHtml); ?>';
                                                        var janela = window.open(
                                                                url,
                                                                'alterarEstado',
                                                                'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
                                                                );
                                                        janela.focus();
                                                    } else {
                                                        if (data.msg != '') {
                                                            alert(data.msg);
                                                        } else {
                                                            alert('Erro ao realisar a tramitação.');
                                                        }
                                                    }
                                                    jQuery(this).dialog("close");
                                                }
                                            });
                                        }
                                    }
                                }
                            });
                        }
                    });
                });
            });

            window.$ = jQuery.noConflict();
           

    <?php } ?>

        function wf_atualizarTela(mensagem, janela)
        {
            janela.close();
            alert(mensagem);
            window.location.reload();
        }

        function wf_alterarEstado(aedid, docid, esdid, acao)
        {
            if (acao) {
                var f = acao.charAt(0).toLowerCase();
                acao = f + acao.substr(1);
            }

            if (!confirm('Deseja realmente ' + acao + ' ?'))
            {
                return;
            }
            var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
                    '?aedid=' + aedid +
                    '&docid=' + docid +
                    '&esdid=' + esdid +
                    '&verificacao=<?php echo urlencode($dadosHtml); ?>';
            var janela = window.open(
                    url,
                    'alterarEstado',
                    'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
                    );
            janela.focus();
        }

        /**
         * Exibe informações sobre a tremitação
         */
        function wf_informacaoTramitacao(aedid) {
            $('#modal-alert .modal-title').html('Informações sobre esse trâmite');
            $('#modal-alert .modal-body').html('Não constam informações.');
            
             var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
                    '?modulo=principal/tramitacao' +
                    '&acao=C' +
                    '&requisicao=informacaoTramitacao' +
                    '&aedid=' + aedid;
            $.ajax({
                url: url,
                context: document.body
            }).done(function (result) {
                $('#modal-alert .modal-body').html(result);
            });
            $('#modal-alert').modal();
        }
        /**
         * Exibe informações sobre a tremitação
         */
        function wf_historicoBootstrap(docid) {
            $('#modal-alert .modal-title').html('Histórico de Tramitações');
            $('#modal-alert .modal-body').html('Não constam informações.');
            var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
                    '?modulo=principal/tramitacao' +
                    '&acao=C' +
                    '&requisicao=historicoBootstrap' +
                    '&docid=' + docid;
            $.ajax({
                url: url,
                context: document.body
            }).done(function (result) {
                $('#modal-alert .modal-body').html(result);
            });

            $('#modal-alert').modal();
            
        }

    </script>
    <div id="div_dialog_workflow" style="display:none" ></div>
    <div class="panel panel-default">
        <div class="panel-heading"><?= $titulo != null ? $titulo : 'Workflow' ?></div>
        <div class="panel-body">

            <?php if (count($estadoAtual)) : ?>
                <div style="margin-bottom: 5px; text-align: center">
                    Estado atual
                </div>    
                <div>
                    <span class="badge" style="white-space: normal !important; width: 100%">
                        <?php echo $estadoAtual['esddsc'] ?>
                    </span>
                </div>
            <?php endif; ?>
            <div style="margin-bottom: 5px; margin-top: 5px; text-align: center">
                Ações
            </div>
            <?php if (count($estados)) : ?>
                <?php $nenhumaacao = true; ?>
                <?php
                foreach ($estados as $estado) :
                    $action = wf_acaoPossivel2($docid, $estado['aedid'], $dados);
                    ?>
                    <?php if ($action === true) : ?>
                        <?php
                        $nenhumaacao = false;
                        switch ($estado['aedicone']) {
                            case '1.png':
                                $estado['aedicone'] = ' glyphicon-thumbs-up';
                                $tipo = "btn-success";
                                break;
                            case '2.png':
                                $estado['aedicone'] = ' glyphicon-thumbs-down';
                                $tipo = "btn-danger";
                                break;
                            case '3.png':
                                $estado['aedicone'] = ' glyphicon-share-alt';
                                $tipo = "btn-warning";
                                break;
                        }
                        ?>
                        <div style="white-space: nowrap; width: 90%">
                            <button type="button" style="white-space: normal !important; margin-bottom: 5px; width: 100%; font-size: 10px; padding: 5px !important;" class="btn <?php echo $tipo ?>"
                                    href="#"
                                    alt="<?php echo $estado['aeddscrealizar'] ?>"
                                    title="<?php echo $estado['aeddscrealizar'] ?>"
                                    <?php if ($estado['aedpreacao'] != '') { ?>
                                        class="preacao"
                                        id="<?= $docid ?>;<?= $estado['aedid'] ?>;<?= $estado['aedpreacao'] ?>;<?php echo $estado['esdid'] ?>;<?php echo $estado['aeddscrealizar'] ?>"
                                    <?php } else { ?>
                                        onclick="wf_alterarEstado('<?php echo $estado['aedid'] ?>', '<?php echo $docid ?>', '<?php echo $estado['esdid'] ?>', '<?php echo $estado['aeddscrealizar'] ?>');"
                                    <?php } ?>
                                    >
                                <span class="glyphicon <?php echo $estado['aedicone']; ?>" ></span>
                                <?php echo $estado['aeddscrealizar']; ?>
                            </button>
                            <span onclick="wf_informacaoTramitacao(<?php echo $estado['aedid'] ?>)" style="float:rigth; color:#428bca; cursor:pointer" class="glyphicon glyphicon-info-sign"></span>
                        </div>

                    <?php else : ?>

                        <?php if ($action === false) : ?>

                            <?
                            #Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida
                            if ($estado['aedcodicaonegativa'] == 't') :
                                ?>
                                <?php $nenhumaacao = false; ?>
                                <?php echo (($estado['aedicone']) ? "<img align=absmiddle src=../imagens/workflow/" . $estado['aedicone'] . " border=0><br/>" : ""); ?> <?php echo $estado['aeddscrealizar'] ?>

                            <? endif; ?>

                        <?php else: ?>
                            <?php $nenhumaacao = false; ?>
                            <?php echo (($estado['aedicone']) ? "<img align=absmiddle src=../imagens/workflow/" . $estado['aedicone'] . " border=0><br/>" : ""); ?> <?php echo $estado['aeddscrealizar'] ?>
                        <?php endif; ?>

                    <?php endif; ?>
                <?php endforeach; ?>
                <?php if ($nenhumaacao) : ?>
                    <span class="badge" style="white-space: normal !important; width: 100%">
                
                        Nenhuma ação disponível para o documento.
                    </span>
                    
                <?php endif; ?>
            <?php else: ?>
                    <span class="badge" style="white-space: normal !important; width: 100%">
                
                        Nenhuma ação disponível para o documento.
                    </span>
            <?php endif; ?>
            <?php if (is_array($ocultar)) : ?>
                <?php if (!$ocultar['historico']) : ?>
                    <div>
                        <button style="width: 100%; font-size: 10px; padding: 5px !important;" onclick="wf_historicoBootstrap('<?php echo $docid ?>');" type="button" class="btn btn-info" >
                            <span class="glyphicon glyphicon-time" aria-hidden="true"></span>
                            Histórico
                        </button>
                    </div>
                <?php endif; ?>
            <?php endif; ?>
            <br/><br/>
        </div>
    </div>
    <?php
}