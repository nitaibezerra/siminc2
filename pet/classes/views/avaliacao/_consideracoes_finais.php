<?php
$respConsideracaoFinal = array();
if ($this->view->identificacaoGrupo->getAttributeValue('idgid')) {
	$respConsideracaoFinal = $this->view->consideracoesfinais->getByIdgidQueid($this->view->identificacaoGrupo->getAttributeValue('idgid'), $eixo['queid']);
}
?>

<form role="form" class="form-horizontal" method="post" id="form-ConsideracaoFinal">

	<input name="idgid" type="hidden" value="<?= $this->identificacaoGrupo->getAttributeValue('idgid'); ?>">
	<input name="queid" id="queid" type="hidden" value="<?= $eixo['queid']; ?>">
	<input name="idGrupo" type="hidden" value="<?= $this->view->idGrupo; ?>">

	<div class="well">
		<div class="form-group">
			<label class="control-label" for="consideracoes">Considerações Finais da Comissão de Avaliadores</label>
			<?php if($this->somenteLeitura): ?>
				<td class="text-center"> <br><br> <?= (!empty( $respConsideracaoFinal['consideracoes'] ) ? $respConsideracaoFinal['consideracoes'] : 'Não preenchido') ?></td>
			<?php else: ?>
				<textarea id="consideracoes" class="form-control" name="consideracoes"><?= $respConsideracaoFinal['consideracoes']; ?></textarea>
			<?php endif; ?>
		</div>
	</div>
	
	<div class="form-group">
		<div>
				<td class="text-center"> <b>Média dos eixos:</b>  <span class="mediadivtot" id ="mediaTotal">  </span> </td>
		</div>
	</div>
	
	<?php
	if (!$_SESSION['finalizado'] && !$this->somenteLeitura): ?>
		<div class="form-group">
			<div class="col-lg-offset-2 col-lg-10 text-right">
				<button type="button" class="btn btn-primary" id="btFinalizarAvaliacao">
					<span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Finalizar Avaliação
				</button>
			</div>
		</div>
	<?php endif; ?>
</form>

<?php if(!$this->somenteLeitura): ?>
	<script type="text/javascript">

		$(function () {
			$('#btFinalizarAvaliacao').on('click', function () {
				confirm('Você tem certeza de que quer finalizar?\n Ao finalizar o questinário não poderá ser editado.');
				$('#form-ConsideracaoFinal').saveAjaxRetorno({controller: 'resposta', action: 'salvarConsideracaoFinal', clearForm: false, functionSucsess:'retornoSucessoConsFinais', retorno: true, displayErrorsInput: true,
					funcaoRetornoInvalido: 'retornoSucessoConsideracaoFinal' });
			});

		});
		function retornoSucessoConsideracaoFinal(retorno) {
			var html = '';
			if (retorno.result) {
				$('.has-error').removeClass('has-error');

				var form = $('#form-ConsideracaoFinal');
				form.find('.erro_input').remove();

				$(retorno['result']).each(function () {
					element = form.find('.' + this.name);

					label = form.find('label[for=' + this.name + ']').eq(0).text();
					if (label) {
						html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
					} else {
						html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
					}
					element.closest('.form-group').addClass('has-error');
				});
				if (html === '') {
					html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + result['msg'] + '</div></div>'
				}
				$('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
			}
		}

		function retornoSucessoConsFinais(retorno){
			atualializaGrupo(retorno.idGrupo);
			atualializaListaGrupo(retorno.idGrupo);
		}
	</script>
<?php endif; ?>
<script type="text/javascript">
	$(function () {
		mediaTotal();
	});
	function mediaTotal(){
		var soma = 0;
		var x =0;
		var media = 0;

		$('.mediadiv').each(function() {
			var qtd = 0;
			if('<?=!$this->somenteLeitura?>'){
				if( $(this).val()!=0 ){
					qtd = $(this).val();
					x = x+1;
				}
			}else{
				if( $(this).attr('value')!=0 ){
					qtd = $(this).attr('value');
					x = x+1;
				}
			}
			soma = parseFloat(soma) + parseFloat(qtd);
		});
		media = (soma/x).toFixed(2);

		$('#mediaTotal').html(media);
	}
</script>