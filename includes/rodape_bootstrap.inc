		<!-- Modal -->
        </div>
        <div class="row">
            <div class="col-md-12">

                <div class="modal fade" id="modal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                </div><!-- /.modal -->
                <div class="modal fade" id="modal-user" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                    <div class="modal-dialog">
                        <div class="panel panel-primary">
                            <div class="panel-heading">
                                <h3 class="panel-title">Informações de usuário</h3>
                            </div>
                            <div class="panel-body">
                                <div class="row">
                                    <?php
                                    	$usuarioNome = $_SESSION['usunome'];
                                    	$usuarioEmail = $_SESSION['usuemail'];
                                    	$usuarioCpf = $_SESSION['usucpf'];
                                    	$usuarioCpfOrigem = $_SESSION['usucpforigem'];
                                    ?>
									<div class="col-md-3 col-lg-3 hidden-xs hidden-sm">
                                    	<img class="img-circle" src="/photo100.png" alt="User Pic">
                                    </div>
                                    <div class="col-xs-2 col-sm-2 hidden-md hidden-lg">
                                    	<img class="img-circle" src="/photo50.png" alt="User Pic">
									</div>
                                    <div class=" col-md-9 col-lg-9 hidden-xs hidden-sm">
                                            <strong><?php echo ucwords(strtolower($usuarioNome)); ?></strong><br>
                                        <table class="table ">
                                            <tbody>
                                            <tr>
                                                <td>CPF:</td>
                                                <td><?php echo formatar_cpf($usuarioCpf); ?></td>
                                            </tr>
                                            <tr style="border: solid 0px; ">
                                                <td>E-mail:</td>
                                                <td><?php echo $usuarioEmail ?></td>
                                            </tr>
                                            <tr style="border: solid 0px; ">
                                                <td>Perfil:</td>
                                                <td>
						<?php $perfisUsuarioOrigem = pegaPerfilPorUsuario($usuarioCpf, $_SESSION['sisid']); ?>
                                                    <?php 
                                                    if(is_array($perfisUsuarioOrigem)):
                                                    foreach($perfisUsuarioOrigem as $perfilUsuarioOrigem): ?>
                                                        <?php if( $perfilUsuarioOrigem['pfldsc'] == 'Super Usuário'): ?>
                                                            <span class="label label-success"><?php echo $perfilUsuarioOrigem['pfldsc'] ?></span>
                                                        <?php else: ?>
                                                            <span class="label label-primary"><?php echo $perfilUsuarioOrigem['pfldsc'] ?></span>
                                                        <?php endif; ?>
                                                        <?php endforeach; ?>
                                                    <?php endif; ?>        
                                                </td>
                                            </tr>
                                            </tbody>
                                        </table>
                                    </div>
                                    <div class="row" style="height: 300px; overflow:auto; border: 1px solid white" >
                                        <br/>
                                        <?php
                                        $sql = "
                                            SELECT
                                            sis.sisabrev ,
                                            avb.avbmsg   ,
                                            avb.avburl   ,
                                            to_char(avb.dataultimaalteracao, 'dd/mm/yyyy às hh:mi') as datahora,
                                            avb.avbstatus
                                           FROM
                                            public.avisousuario avb
                                           JOIN
                                            seguranca.sistema sis
                                           USING
                                            ( sisid )
                                            WHERE
                                             usucpf = '{$_SESSION['usucpf']}' ORDER BY avb.dataultimaalteracao DESC LIMIT 30";
                                             $avisosBootstrap = $db->carregar($sql);
                                             #ver($sql, $avisosBootstrap);
                                             if(is_array($avisosBootstrap)){
                                                 foreach($avisosBootstrap as $valor){
                                                     $nova = "";
                                                     if($valor['avbstatus']=='A'){$nova = " background-color:#D3D3D3 !important; ";}
                                                     $saida = "<div style=\"font-size: 12px; padding: 4px; {$nova}\">  Em <b>{$valor['datahora']}</b> no sistema <b>{$valor['sisabrev']}</b> você recebeu a mensagem: ";
                                                     $saida .= "{$valor['avbmsg']}";
                                                     $saida .= "&nbsp; <a class=\"confirmarAviso\" title=\"Ir para a referência.\" href = \"{$valor['avburl']}\" target=\"_top\" ><span class=\"glyphicon glyphicon-screenshot\" aria-hidden=\"true\"></span></a>";
                                                     $saida .= "</div> <hr style=\"padding:0px;  margin: 3px;\">";
                                                     echo $saida;
                                                 }
                                             }
                                        ?>
                                    </div> 
                                </div>
                            </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                            </div>
                        </div><!-- /.modal-content -->
                        <!-- /.modal-content -->
                    </div><!-- /.modal-dialog -->
                </div><!-- /.modal -->
                <!-- Modal -->
            </div>
            <div class="row">
            <div class="col-md-12">
            <div class="modal fade" id="modal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                <!--deleteByemail('asdiaosdas')-->
            </div><!-- /.modal -->
            <div class="modal fade" id="modal-user-search" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog modal-lg">
                    <div id="container-search-users" class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title">Simular usuário</h4>
                        </div>
                        <div class="modal-body">
                            <div class="well">
                                <form class="form-horizontal" id="formulario-simular-usuario" method="post">
                                    <input type="hidden" name="carregarUsuariosSistema" value="true">
                                    <fieldset>
                                        <legend>Pesquisar usuário neste módutlo</legend>
                                        <div class="form-group">
                                            <label for="nomeUsuario" class="col-lg-2 control-label">Usuário</label>
                                            <div class="col-lg-10">
                                                <input type="text" class="form-control" id="nomeUsuario" name="nomeSimularUsuario" placeholder="Nome, CPF ou E-mail">
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Perfil</label>
                                            <div class="col-lg-10">
                                                <?php $perfis = carregarPerfils($_SESSION['sisid']); ?>
                                                <select id="perfisSimularUsuario" name="perfisSimularUsuario[]" multiple class="form-control" data-placeholder="Selecione">
                                                    <?php foreach ($perfis as $perfil): ?>
                                                        <option value="<?php echo $perfil['codigo'] ?>"><?php echo $perfil['descricao'] ?></option>
                                                    <?php endforeach; ?>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <div class="col-lg-12">
                                                <button class="btn btn-primary" id="bt-carregar-usuarios-sistema-simular" ">Procurar</button>
                                            </div>
                                        </div>
                                    </fieldset>
                                </form>
                            </div>
                            <div id="container-lista-simular">

                            </div>
                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                    <!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
                <script>
                    $('#btSimularUsuario').click(function(){
                        $('#modal-user-search').modal();
                        setTimeout(function(){
                            $('#perfisSimularUsuario').chosen({});
                        } , 300);
                    });
                    $('#bt-carregar-usuarios-sistema-simular').click(function(){
                        $('#formulario-simular-usuario').ajaxSubmit(function(html , teste2 ,teste3) {
                            $('#container-lista-simular').hide().html(html).fadeIn();
                        });
                        return false;
                    });
                </script>
            <?php
                $dt_inicio = $_REQUEST['dt_inicio'] ? $_REQUEST['dt_inicio'] : '01/01/' . date('Y');
                $dt_fim = $_REQUEST['dt_fim'] ? $_REQUEST['dt_fim'] : date('d/m/Y');
            ?>
            <!-- MODAL - PAINEL WORKFLOW -->
            <div class="modal fade" id="modal-painel-workflow" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog-large">
                    <div id="container-search-users" class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title">Painel</h4>
                        </div>
                        <div class="modal-body">
                            <div class="well">
                                <form class="form-horizontal" id="form-painel-workflow" method="post">
                                    <input type="hidden" name="carregarUsuariosSistema" value="true">
                                    <fieldset>
                                        <div class="form-group">
                                            <label for="nomeUsuario" class="col-lg-2 control-label">Fluxo</label>
                                            <div class="col-lg-10">
                                                <?php $tipoDocumentos = carregarTipoDocumentos($_SESSION['sisid']); ?>
                                                <select id="fluxo-painel-workflow" name="tpdid" class="form-control" data-placeholder="Selecione...">
                                                    <option value=""></option>
                                                    <?php foreach ($tipoDocumentos as $tipoDocumento): ?>
                                                        <option value="<?php echo $tipoDocumento['codigo'] ?>"><?php echo $tipoDocumento['descricao'] ?></option>
                                                    <?php endforeach; ?>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Situação</label>
                                            <div id="situacao-painel-workflow" class="col-lg-10">
                                                <?php $tipoDocumentos = carregarTipoDocumentosPainel($_SESSION['sisid']); ?>
                                                <select id="estado-documento-situacao-workflow" name="esdid[]" class="form-control" data-placeholder="Selecione...">
                                                    <option value=""></option>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Período</label>
                                            <div class="col-lg-12">
                                                <div class="col-lg-2">
                                                    <input name="dt_inicio" type="text" class="form-control" id="dt-inicio-painel-workflow" placeholder="00/00/0000" maxlength="10" value="<?php echo $dt_inicio ?>" data-format="dd/MM/yyyy hh:mm:ss">
                                                </div>
                                                <div class="col-lg-2">
                                                    <input name="dt_fim" type="text" class="form-control" id="dt-fim-painel-workflow" placeholder="00/00/0000" maxlength="10" value="<?php echo $dt_fim ?>" data-format="dd/MM/yyyy hh:mm:ss">
                                                </div>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Exibição</label>
                                            <div class="col-lg-10">
                                                <div class="col-lg-10 text-left">
                                                    <input type="radio" name="tipo_exibicao" id="" value="D" style="margin-left: 10px;">
                                                    Diário
                                                    <input type="radio" name="tipo_exibicao" id="" value="M" checked="checked" style="margin-left: 10px;">
                                                    Mensal
                                                    <input type="radio" name="tipo_exibicao" id="" value="A" style="margin-left: 10px;">
                                                    Anual
                                                </div>
                                            </div>
                                        </div>
                                    </fieldset>
                                </form>
                            </div>
                            <div id="container-grafico-painel-workflow">
                            </div>
                        </div>
                        <div class="modal-footer">
                            <button class="btn btn-primary" id="bt-procurar-painel-workflow" ">Procurar</button>
                            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                    <!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
            <script type="text/javascript">
                var config = {allow_single_deselect: true};

                $('#btPainelWorkflow').click(function(){
                    $('#modal-painel-workflow').modal();
                        setTimeout(function(){
                            $( '#fluxo-painel-workflow' ).chosen(config);
                            $( '#estado-documento-situacao-workflow' ).chosen(config);
                        } , 450);
                });

                $("#dt-fim-painel-workflow").datepicker({
                    showWeek: false
                    , 'showAnim':'drop'
                });

                $("#dt-inicio-painel-workflow").datepicker({
                    showWeek: false
                    , onClose: function(selectedDate) {
                        $( "#dt-fim-painel-workflow").datepicker("option", "minDate", selectedDate );
                    }
                    , 'showAnim':'drop'
                });

                $('#fluxo-painel-workflow').change(function(){
                    url = window.location.href;
                    dataForm = $('#form-painel-workflow').serialize();
                    data = dataForm + '&carregarSituacaoPainel=true';
                    $.post(
                        url
                        , data
                        , function(html){
                            $( '#situacao-painel-workflow' ).hide().html(html).fadeIn('slow');
                            $( '#estado-documento-situacao-workflow' ).attr('multiple' , 'multiple');
                            $( '#estado-documento-situacao-workflow').attr('data-placeholder' , 'Selecione...');
                            $( '#estado-documento-situacao-workflow').find('option').attr('selected', 'selected');
                            $( '#estado-documento-situacao-workflow').chosen(config);
                        }
                    );
                });

                $('#bt-procurar-painel-workflow').click(function(){
                    $('.has-error').removeClass('has-error');
                    isValid = true;
                    html = '';
                    if($('#fluxo-painel-workflow').val() == ''){
                        element = $('#fluxo-painel-workflow');
                        label = element.parent().parent('.form-group').children('label').text();
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
                        $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
                        element.parent().parent('.form-group').addClass('has-error');
                        $('#fluxo-painel-workflow').focus();
                        isValid = false;
                        return false;
                    }
                    if($('#dt-inicio-painel-workflow').val() == ''){
                        element = $('#dt-inicio-painel-workflow');
                        label = element.parent().parent().parent('.form-group').children('label').text();
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>Período (Data de início):</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
                        $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
                        element.parent().parent().parent('.form-group').addClass('has-error');
                        $('#fluxo-painel-workflow').focus();
                        isValid = false;
                        return false;
                    }
                    if($('#dt-fim-painel-workflow').val() == ''){
                        element = $('#dt-fim-painel-workflow');
                        label = element.parent().parent().parent().parent('.form-group').children('label').text();
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>Período (Data fim):</strong> não pode ser vazio!.<a class="alert-link" href="#"></a></div></div>';
                        $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
                        element.parent().parent().parent('.form-group').addClass('has-error');
                        $('#fluxo-painel-workflow').focus();
                        isValid = false;
                        return false;
                    }

                    if(isValid){
                        $('.has-error').removeClass('has-error');
                        url = window.location.href;
                        dataForm = $('#form-painel-workflow').serialize();
                        data = dataForm + '&gerarGraficoPainel=true';
                        $.post(
                            url
                            , data
                            , function(html){
                                $('#container-grafico-painel-workflow').hide().html(html).fadeIn('slow');
                            }
                        );
                    }
                    return false;
                });
            </script>
            <div class="modal fade" id="modal-alert" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog">
                    <div class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title" style="text-align:left">Aviso!</h4>
                        </div>
                        <div class="modal-body">

                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-danger" data-dismiss="modal">Ok</button>
                        </div>
                    </div><!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
                <div class="modal fade" id="modal-confirm">
                    <div class="modal-dialog">
                      <div class="modal-content">
                        <div class="modal-header">
                          <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                          <h4 class="modal-title">Aviso!</h4>
                        </div>
                        <div class="modal-body">
                          <p></p>
                        </div>
                        <div class="modal-footer">
                          <button type="button" class="btn btn-default" data-dismiss="modal">Não</button>
                          <button type="button" class="btn btn-primary">Sim</button>
                        </div>
                      </div><!-- /.modal-content -->
                    </div><!-- /.modal-dialog -->
                  </div><!-- /.modal -->
            </div>
        </div>
        <div class="row cont rodape notprint" style="min-height: 45px; color: #667">
            <br />
                <div class="col-md-5">
                    <ul>
                        <li>
                            Data: <?= date("d/m/Y - H:i:s") ?> / Último acesso (<?= formata_data( $_SESSION['usuacesso'] ); ?>) - 
                            <a style="color: #707070; font-style: normal" href="javascript:abrirUsuariosOnline();" >
                            <span id="rdpUsuariosOnLine"><?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']];?></span>Usuários On-Line</a>
                        </li>
                    </ul>
                </div>
                <div class="col-md-2 text-center">
                    <ul>
                        <li>
                            <a style="color: #707070; font-style: normal" href="javascript: abrirRegras( <?=$_SESSION['mnuid']; ?> );">Visualizar Regras</a>
                        </li>
                    </ul>
                </div>
                <div class="col-md-4 text-right">
                    <address>
                        <?php echo SIGLA_SISTEMA; ?> - Fale Conosco Manual	| Tx.: 0,2015s / 0,34
                    </address>
                </div>
                <br style="clear:both" />
                <?php if (1 == $_SESSION['superuser']): ?>
                <div class="col-md-12 text-right">
                    <?php //require_once APPRAIZ . '/svnversion.php'; ?>
                    <br style="clear:both" />
                    <br />
                </div>
                <?php endif; ?>
        </div>
        <script type="text/javascript">
            $.datepicker.regional[ 'pt-BR' ];
            var config = {
                '.chosen-select': {allow_single_deselect: true},
                '.chosen-select-deselect': {allow_single_deselect: true},
                '.chosen-select-no-single': {disable_search_threshold: 10},
                '.chosen-select-no-results': {no_results_text: 'Oops, nothing found!'},
                '.chosen-select-width': {width: "95%"}
            }
            for (var selector in config) {
                $(selector).chosen(config[selector]);
            }
        </script>

        <?php // include_once APPRAIZ . "includes/estouvivo.php"; ?>

        <script type="text/javascript" language="javascript">
            function abrirUsuariosOnline()
            {
                window.open(
                    '../geral/usuarios_online.php',
                    'usuariosonline',
                    'height=500,width=600,scrollbars=yes,top=50,left=200'
                );
            }
        </script>

        <!-- The blueimp Gallery widget -->
        <div id="blueimp-gallery" class="blueimp-gallery blueimp-gallery-controls" data-filter=":even" style="display: none;">
            <div class="slides"></div>
            <h3 class="title"></h3>
            <a class="prev" style="font-size: 30px;" ><span class="glyphicon glyphicon-chevron-left"></span></a>
            <a class="next" style="font-size: 30px;" ><span class="glyphicon glyphicon-chevron-right"></span></a>
            <a class="close">×</a>
            <a class="play-pause"></a>
            <ol class="indicator"></ol>
        </div>
    </body>
</html>
<?php

    // -- Caso a página requisitada seja uma página existente, realiza o log de estatísticas - Verificação necessária pois
    // -- ainda não foi possível reproduzir o erro no sistema financeiro que faz com que todas as imagens do tema do sistema
    // -- sejam requeridas pelo browers como um módulo. Esta mesma verificação é feita no controleAcesso no momento de
    // -- incluir os arquivos.
    if (file_exists(realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc"))) {
        global $db;
        global $Tinicio;

		if (isset($_SESSION['mnuid']) && !empty($_SESSION['mnuid'])) {
            simec_gravar_estatistica();
		}
    }
?>
<script language="JavaScript">
    /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
    $(document).ajaxSend(function (e, jqxhr, settings) {
    	 jQuery("#loading").fadeIn();

    }).ajaxStop(function(){
    	 jQuery("#loading").fadeOut();
	});

    /** Mensagem de carregando quando houver requisições em ajax */
    $.ajaxSetup({
        timeout: 0,
        error: function(xhr, status, error) {
            console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
            jQuery("#loading").fadeOut();
            jQuery("#loading").fadeIn();

            setTimeout(function(){ jQuery("#loading").fadeOut();}, 1300);
        }
    });

    // -- Substituíndo o alert do browser.
    window.alert = function(texto)
    {
        jQuery('#modal-alert .modal-body').html(texto);
        jQuery('#modal-alert').modal();
    };
</script>
<!-- The template to display files available for upload -->
<script id="template-upload" type="text/x-tmpl">
{% for (var i=0, file; file=o.files[i]; i++) { %}
    <tr class="template-upload fade">
        <td>
            <span class="preview"></span>
        </td>
        <td>
            <p class="name">{%=file.name%}</p>
            <strong class="error text-danger"></strong>
        </td>
        <td>
            <p class="size">Processando...</p>
            <div class="progress progress-striped active" role="progressbar" aria-valuemin="0" aria-valuemax="100" aria-valuenow="0"><div class="progress-bar progress-bar-success" style="width:0%;"></div></div>
        </td>
        <td>
            {% if (!i && !o.options.autoUpload) { %}
                <button class="btn btn-primary start" disabled>
                    <i class="glyphicon glyphicon-upload"></i>
                    <span>Iniciar</span>
                </button>
            {% } %}
            {% if (!i) { %}
                <button class="btn btn-warning cancel">
                    <i class="glyphicon glyphicon-ban-circle"></i>
                    <span>Cancelar</span>
                </button>
            {% } %}
        </td>
    </tr>
{% } %}
</script>
<!-- The template to display files available for download -->
<script id="template-download" type="text/x-tmpl">

{% for (var i=0, file; file=o.files[i]; i++) { %}
    <tr class="template-download fade">
        <td>
            <span class="preview">
                {% if (file.thumbnailUrl) { %}
                    <a href="{%=file.url%}" title="{%=file.name%}" download="{%=file.name%}" data-gallery><img src="{%=file.thumbnailUrl%}"></a>
                {% } %}
            </span>
        </td>
        <td>
            <p class="name">
                {% if (file.url) { %}
                    <a href="{%=file.url%}" title="{%=file.name%}" download="{%=file.name%}" {%=file.thumbnailUrl?'data-gallery':''%}>{%=file.name%}</a>
                {% } else { %}
                    <span>{%=file.name%}</span>
                {% } %}
            </p>
            {% if (file.error) { %}
                <div><span class="label label-danger">Erro</span> {%=file.error%}</div>
            {% } %}
        </td>
        <td>
            <span class="size">{%=o.formatFileSize(file.size)%}</span>
        </td>
        <td>
            {% if (file.deleteUrl) { %}
                <button class="btn btn-danger delete" data-type="{%=file.deleteType%}" data-url="{%=file.deleteUrl%}"{% if (file.deleteWithCredentials) { %} data-xhr-fields='{"withCredentials":true}'{% } %}>
                    <i class="glyphicon glyphicon-trash"></i>
                    <span>Remover</span>
                </button>
                <input type="checkbox" name="delete" value="1" class="toggle">
            {% } else { %}
                <button class="btn btn-warning cancel">
                    <i class="glyphicon glyphicon-ban-circle"></i>
                    <span>Cancelar</span>
                </button>
            {% } %}
        </td>
    </tr>
{% } %}

</script>
<!-- The jQuery UI widget factory, can be omitted if jQuery UI is already included -->
<script src="../library/bootstrap-file-upload-9.5.1/js/vendor/jquery.ui.widget.js"></script>
<!-- The Templates plugin is included to render the upload/download listings -->
<script src="../library/bootstrap-file-upload-9.5.1/blueimp/js/tmpl.min.js"></script>
<!-- The Load Image plugin is included for the preview images and image resizing functionality -->
<script src="../library/bootstrap-file-upload-9.5.1/blueimp/js/load-image.min.js"></script>
<!-- The Canvas to Blob plugin is included for image resizing functionality -->
<script src="../library/bootstrap-file-upload-9.5.1/blueimp/js/canvas-to-blob.min.js"></script>
<!-- blueimp Gallery script -->
<script src="../library/bootstrap-file-upload-9.5.1/blueimp/js/jquery.blueimp-gallery.min.js"></script>
<!-- The Iframe Transport is required for browsers without support for XHR file uploads -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.iframe-transport.js"></script>
<!-- The basic File Upload plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload.js"></script>
<!-- The File Upload processing plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-process.js"></script>
<!-- The File Upload image preview & resize plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-image.js"></script>
<!-- The File Upload audio preview plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-audio.js"></script>
<!-- The File Upload video preview plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-video.js"></script>
<!-- The File Upload validation plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-validate.js"></script>
<!-- The File Upload user interface plugin -->
<script src="../library/bootstrap-file-upload-9.5.1/js/jquery.fileupload-ui.js"></script>
<!-- The main application script -->
<!--<script src="../library/bootstrap-file-upload-9.5.1/js/main.js"></script>-->
<!-- The XDomainRequest Transport is included for cross-domain file deletion for IE 8 and IE 9 -->
<!--[if (gte IE 8)&(lt IE 10)]>
<script src="../library/bootstrap-file-upload-9.5.1/js/cors/jquery.xdr-transport.js"></script>
<![endif]-->