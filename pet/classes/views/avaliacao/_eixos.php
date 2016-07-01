<?php $dados = $this->eixo->getDadosComQuestoes($this->questionario->getAttributeValue('queid')); ?>
<?php foreach ($dados as $key => $eixo): ?>
			
	<?php if ($active == false and $key == 0) : $active = 'active';
	else: $active = ''; endif; ?>

	<div id="tabEixo<?= $eixo['numeroeixo']; ?>" class='tab-pane fade <?= $active; ?> in tabeixo' data-ideixo="<?= $eixo['numeroeixo']; ?>"> 
		<?php
		if ($eixo['tipo'] == 'Binárias'):
			require(APPRAIZ_VIEW . 'questoesbinarias/questao.php');

		elseif ($eixo['tipo'] == 'Multipla Escolha'):
		
			require(APPRAIZ_VIEW . 'questoesmultiplaescolha/questao.php');
		endif;
		?>
	</div>

<?php endforeach; ?>
<script>
	function media(ideixo){
		var soma = 0;
		var x =0;
		var media = 0;
		$('#form-RespostaMultiplaEscolha'+ideixo).find('.opcaoescolhida:checked').each(function() {
			var qtd = 0;
			if( $(this).attr('ordem')!=0 ){
				qtd = $(this).attr('ordem');
				x = x+1
			}
			soma = parseFloat(soma) + parseFloat(qtd)
		});

		media = (soma/x).toFixed(2);

		$('#media'+ideixo).html(media);
		$('#media'+ideixo).val(media);
	}
</script>
<?php if(!$this->somenteLeitura): ?>
	<script type="text/javascript">
	
		var requestSent = false;
		$(function () {
			
			// gravar multipla escolha
			$('.btGravarRespostaMultiplaEscolha').on('click', function () {
				var form = $(this).closest('form');
				$('#' + form.attr('id')).saveAjaxRetorno({controller: 'resposta', action: 'salvarMultiplaEscolha', clearForm: false, retorno: true, displayErrorsInput: true, funcaoRetornoInvalido: 'retornoSucessoQuestoes' });
			});

			// gravar binario
			$('.btGravarRespostaBinario').on('click', function () {
				var form = $(this).closest('form');
				$('#' + form.attr('id')).saveAjaxRetorno({controller: 'resposta', action: 'salvarBinario', clearForm: false, retorno: true, displayErrorsInput: true, funcaoRetornoInvalido: 'retornoSucessoQuestoes'});
			});
			$('.tabeixo').each(function() {
				media( $(this).data('ideixo'));
			});
			$('.opcaoescolhida').click(function(){
				media( $(this).data('ideixo'));
				mediaTotal();
			});
		});
	</script>
<?php endif; ?>
