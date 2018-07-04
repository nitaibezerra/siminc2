   <div class="clearfix" style="margin-top: 10px;">&nbsp;</div>

				<div class="footer">
		            <div class="pull-right">
		            	<strong>Data:</strong> <?= date("d/m/Y - H:i:s") ?> | 
		            	<strong>Último acesso</strong> <?= formata_data( $_SESSION['usuacesso'] ); ?>
		            </div>
		            <div class="pull-left footer-actions">
		                <a href="javascript:abrirRegras(<?=$_SESSION['mnuid']; ?>);"><strong><i class="fa fa-book"></i> Visualizar Regras</strong></a>
		            	 <!--| <a href="javascript:abrirUsuariosOnline();"><strong><i class="fa fa-user"></i> <?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']];?> Usuários On-Line</strong></a>-->
		            </div>
		        </div>
			</div>
		</div>
		
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
							<div class="col-md-3 col-lg-3 text-center">
								<img style="width: 95%; margin-bottom: 10px; height: 110px;" class="img-circle profile-pic" src="/seguranca/imagemperfil.php" alt="<?php echo ucwords(strtolower($usuarioNome)); ?>">
								<a style="padding-left: 5px;" href="#" class="profile-picture">Editar foto</a>
							</div>
							<div class="col-md-9 col-lg-9 hidden-xs hidden-sm">
								<table class="table" id="user-info-table">
									<tbody>
									<tr>
										<td colspan="2"><strong><?php echo ucwords(strtolower($usuarioNome)); ?></strong></td>
									</tr>
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
							<div class="row">
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
									<legend>Pesquisar usuário neste módulo</legend>
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
			$('#bt-carregar-usuarios-sistema-simular').click(function(e) {
				e.preventDefault();
				$('#formulario-simular-usuario').ajaxSubmit(function(html) {
					$('#container-lista-simular').hide().html(html).fadeIn();
				});
				return false;
			});
		</script>
		
		<?php $dt_inicio = $_REQUEST['dt_inicio'] ? $_REQUEST['dt_inicio'] : '01/01/' . date('Y'); ?>
		<?php $dt_fim = $_REQUEST['dt_fim'] ? $_REQUEST['dt_fim'] : date('d/m/Y'); ?>
		
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

<script language="JavaScript">
	/** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
	$(document).ajaxSend(function (e, jqxhr, settings) {
		 jQuery("#loading").fadeIn();
	}).ajaxStop(function(){
		 jQuery("#loading").fadeOut();
		 attachInit();
		 attachEvents();
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

<?php
$mensagemCarregar = $_SESSION['mensagem_carregar'];
unset($_SESSION['mensagem_carregar']);
?>
<!-- Custom and plugin javascript -->
<script src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/slimscroll/jquery.slimscroll.min.js"></script>

<!-- Chosen -->
<script src="/zimec/public/temas/simec/js/plugins/chosen/chosen.jquery.js"></script>

<!-- JSKnob -->
<script src="/zimec/public/temas/simec/js/plugins/jsKnob/jquery.knob.js"></script>

<!-- Switcher -->
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-switch/bootstrap-switch.js"></script>

<!-- NanoScroller -->
<script src="/zimec/public/temas/simec/js/plugins/nanoscroll/jquery.nanoscroller.min.js"></script>

<!-- FullCalendar -->
<script src="/zimec/public/temas/simec/js/plugins/fullcalendar/fullcalendar.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/fullcalendar/moment.min.js"></script>

<!-- Switchery -->
<script src="/zimec/public/temas/simec/js/plugins/switchery/switchery.js"></script>

<!-- JsTree -->
<script src="/zimec/public/temas/simec/js/plugins/jstree/jstree.min.js"></script>

<!-- iCheck -->
<script src="/zimec/public/temas/simec/js/plugins/iCheck/icheck.min.js"></script>

<!-- File Input -->
<script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput_locale_pt-BR.js"></script>

<!-- Summernote -->
<script src="/zimec/public/temas/simec/js/plugins/summernote/summernote.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/summernote/summernote-pt-BR.js"></script>

<!-- Flot -->
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.tooltip.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.spline.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.resize.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.pie.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.symbol.js"></script>

<!-- Dropzone -->
<script src="/zimec/public/temas/simec/js/plugins/dropzone/dropzone.js"></script>

<!-- Masked Input -->
<script src="/zimec/public/temas/simec/js/plugins/maskedinput/jquery.maskedinput.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/jquery.inputmask.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.numeric.extensions.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.regex.extensions.min.js"></script>

<!-- Menu -->
<script src="/zimec/public/temas/simec/js/plugins/metisMenu/jquery.metisMenu.js"></script>

<!-- Datatables -->
<script src="/zimec/public/temas/simec/js/plugins/dataTables/jquery.dataTables.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/dataTables/dataTables.bootstrap.js"></script>

<!-- Bootstrap Tour -->
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-tour/bootstrap-tour.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/bootstrap-tour/bootstrap-tour-standalone.min.js"></script>

<!-- Gritter -->
<script src="/zimec/public/temas/simec/js/plugins/gritter/jquery.gritter.min.js"></script>

<!-- Bootbox -->
<script src="/zimec/public/temas/simec/js/plugins/bootbox/bootbox.min.js"></script>

<!-- Ion Range Slider -->
<script src="/zimec/public/temas/simec/js/plugins/ionRangeSlider/ion.rangeSlider.min.js"></script>

<!-- Validate -->
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.min.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.extend.js"></script>
<script src="/zimec/public/temas/simec/js/plugins/validate/jquery.form.min.js"></script>

<!-- Sweet Aler -->
<script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>


<!-- Typehead -->
<script src="/zimec/public/temas/simec/js/plugins/typehead/bootstrap3-typeahead.min.js"></script>

<!-- Toastr -->
<script src="/zimec/public/temas/simec/js/plugins/toastr/toastr.min.js"></script>

<!-- SIMEC -->
<script src="/zimec/public/temas/simec/js/theme.js"></script>
<script src="/zimec/public/temas/simec/js/simec.js"></script>
<script src="/zimec/public/temas/simec/js/simec-eventos.js"></script>
<script src="/zimec/public/temas/simec/js/simec-mascaras.js"></script>

<script>
$(document).ready(function() {
    var $input = $(".typeahead");
    $input.typeahead({
        source: <?php echo $_SESSION['acesso-rapido'][$_SESSION['sisid']] ?>,
        autoSelect: true
    });
    $input.change(function() {
        var current = $input.typeahead("getActive");
        if (current) {
            if (current.mnulink) {
                window.location.href = current.mnulink;
            }
        }
    });

	$('#voltar_usuario_origem').click(function() {
		simularUsuariosSistemas('<?php echo $_SESSION['usucpforigem']?>');
	});

    <?php if (!empty($mensagemCarregar) && isset($mensagemCarregar['text'])) { ?>
        swal({title: "", text: "<?php echo str_replace(array("\n", "\r"), '', $mensagemCarregar['text']); ?>", type: "<?php echo $mensagemCarregar['type']; ?>", html: true});
    <?php } ?>
        
    Dropzone.options.formularioAnexo = {
        init: function() {

            this.on("success", function(file, response){
                var jsonResponse = $.parseJSON(response);
                listarAnexosModulo();
            });

            this.on("queuecomplete", function(file){

                // Armazena o objeto Dropzone para chamar métodos
                objFormularioAnexo = this;
                // Chama mensagem de sucesso
                swal({
                  title: "",
                  text: "Arquivos salvos com sucesso!",
                  timer: 2000,
                  showConfirmButton: false,
                  type: "success"
                }, function(){
                    // Fecha o swal alert
                    swal.close();
                    // limpa campo de upload
                    objFormularioAnexo.removeAllFiles();
                    // fecha modal após a seleção
                    $('#modal_upload').modal('hide');
                });
            });
        }
    };        
});

function listarAnexosModulo(){
    $('#listaArquivosModulo').load('?modulo=principal/monitorar-pnc&acao=A&requisicao_upload=listar_arquivos_modulo');
}
</script>
<div class="row">
    <div class="col-md-12">
        <div id="modal_upload_rodape" class="modal fade">
            <div class="modal_upload_rodape">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Anexar arquivos(Documentos)</h4>
                    </div>
                    <div class="modal-body">
                        <form class="dropzone" method="POST" enctype="multipart/form-data" action="?modulo=principal/monitorar-pnc&acao=A&acoid=<?php echo $mAcompanhamento->acoid; ?>&aba=1&requisicao_upload=upload_arquivo" id="formularioAnexo" name="formularioAnexo">
                            <input type="hidden" id="arqmdid" name="arqmdid">
                            <div class="form-group">
                                <label for="arqdescricao" class="col-lg-2 control-label">Descrição:</label>
                                <div class="col-lg-10">
                                    <input type="text" value="" class="CampoEstilo normal form-control" placeholder="Insira a descrição do arquivo." title="Descrição" id="arqdescricao" name="arqdescricao" maxlength="255" size="2">
                                </div>
                            </div>
                            <div class="fallback">
                                <input name="file" type="file" multiple />
                            </div>
                        </form>
                        <div id="listaArquivosModulo">
                            <?php
                            $arquivoModulo = new Public_Model_ArquivoModulo();
                            $listaArquivos = $arquivoModulo->recuperaArquivosPorModulo();
                            include_once APPRAIZ. "public/lista_arquivos_modulo.inc";
                            ?>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>   