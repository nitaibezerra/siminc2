<?php $dadosQuestaoMultiplaEscolha = $this->questaoMultiplaEscolha->getQuestaoByIdEixo($eixo['ideixo']); ?>
<div class="well">
	<h3><?= $eixo['nome']; ?></h3>

	<p><?= $eixo['descricao']; ?></p>
</div>

<form role="form" class="form-horizontal" method="post" id="form-RespostaMultiplaEscolha<?= $eixo['ideixo']; ?>">
	
	<input name="idgid" type="hidden" value="<?= $this->identificacaoGrupo->getAttributeValue('idgid'); ?>"> <input name="ideixo" id="idEixo<?= $eixo['ideixo']; ?>" type="hidden" value="<?= $eixo['ideixo']; ?>">
	<input name="queid" type="hidden" value="<?= $eixo['queid']; ?>"> <input name="numeroeixo" type="hidden" value="<?= $eixo['numeroeixo']; ?>">
	<?php foreach ($dadosQuestaoMultiplaEscolha as $key => $questaoMultiplaEscolha): ?>
		<?php
		$this->questaoMultiplaEscolha->populateEntity($questaoMultiplaEscolha);
		$conceitos = $this->view->conceito->getAllByValues(array('qmeid ' => $questaoMultiplaEscolha['qmeid']));
		?>
		<input name="qmeid[]" type="hidden" value="<?= $this->questaoMultiplaEscolha->getAttributeValue('qmeid'); ?>">

		<div class="well">

			<h4><?= $this->questaoMultiplaEscolha->getAttributeValue('numeroquestao'); ?>: <?= $questaoMultiplaEscolha['titulo']; ?></h4>

			<p><?= $this->questaoMultiplaEscolha->getAttributeValue('descricao'); ?></p>

			<div class="form-group">
				<table class="table table-striped table-condensed table-bordered">
					<tr>
						<th><label class="control-label" for="opcaoescolhida">Conceito</label></th>
						<th>Critério de Análise</th>
					</tr>

					<?php
					if (is_array($conceitos) && !empty($conceitos)):
						
						foreach ($conceitos as $conceito): ?>

							<?php
							$arrayConidEscolhidos = $this->questaoMultiplaEscolha->getOpcoesEscolhidas($this->respostas, $conceito['conid'], $this->questaoMultiplaEscolha->getAttributeValue('qmeid'));
							$checked = array_key_exists($conceito['conid'], $arrayConidEscolhidos);
							if( $checked ){

								$x++;
								if($y==''){
									$y = $conceito['ordem'];
								}else{
									$y = $y + $conceito['ordem'];
								}

								$justificativa = $arrayConidEscolhidos[$conceito['conid']]['justificativa'] ;
								$rmpid = $arrayConidEscolhidos[$conceito['conid']]['rmpid'] ;
							}
							?>

							<tr>
								<?php if ($this->somenteLeitura): ?>
									<td class="text-center"><?= ($checked ? $conceito['ordem'] : '') ?></td>
								<?php else: ?>

									<td class="text-center">
										<input class="opcaoescolhida" data-ideixo="<?= $eixo['ideixo']; ?>" type="radio" name="opcaoescolhida<?= $key; ?>" value="<?= $conceito['conid'] ?>" ordem="<?=$conceito['ordem']?>" <?= ($checked ? 'checked' : '') ?> >
										<?= $conceito['ordem'] ?>
									</td>
								<?php endif; ?>
								<td><?= $conceito['texto'] ?></td>
							</tr>

						<?php endforeach; ?>


					<?php endif;?>
		
				</table>
			</div>
			
			<div class="form-group">
				<?php if ($this->somenteLeitura): ?>
					<td class="text-center"><b>Justificativa:</b> <?= $justificativa; ?></td>
				<?php else: ?>
					<input type="hidden" name="rmpid[]" value="<?php echo $rmpid; ?>">
					<label class="control-label" for="justificativa">Justificativa</label>
					<textarea id="text_justificativa" class="form-control justificativa" name="justificativa[]"><?= $justificativa; ?></textarea>
				<?php endif; ?>
			</div>
			<?php $rmpid = $justificativa = $arrayConidEscolhidos = null; ?>
			
		</div>

	<?php endforeach; ?>
	
		<div class="form-group">
			<?php if (!$this->somenteLeitura): ?>
				<div>
						<td class="text-center"><b>Média:</b> <span class="mediadiv" id = "media<?=$eixo['ideixo'];?>" value=""></span></td>
				</div>
			<?php else: ?>
				<div>
					<td class="text-center"><b>Média:</b> <span class="mediadiv" id = "media<?=$eixo['ideixo'];?>" value="<? echo $y?$x ? $y/$x: '':''; ?>"> <? echo $y?$x ? $y/$x: '':''; ?></span></td>
				</div>
			<?php endif; ?>
		</div>
	<?php if (!$_SESSION['finalizado'] && !$this->somenteLeitura): ?>
		<div class="form-group">
			<div class="col-lg-offset-2 col-lg-10 text-right">
				<button type="button" class="btn btn-primary btGravarRespostaMultiplaEscolha">
					<span class="glyphicon glyphicon-floppy-disk" aria-hidden="true"></span> Salvar Eixo <?= $eixo['numeroeixo']; ?>
				</button>
			</div>
		</div>
	<?php endif; ?>
</form>
