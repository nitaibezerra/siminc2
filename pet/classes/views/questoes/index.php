<?php
if ($this->eixo->getAttributeValue('tipo') == Model_Eixo::TIPO_BINARIO): ?>

	<div id="divQuestaoBinaria">
		<div class="panel-group" role="tablist">
			<?php require_once(APPRAIZ_VIEW . '/categoriabinaria/index.php'); ?>
			<div id="panelQuestaoBinaria"></div>
		</div>
	</div>

<?php elseif ($this->eixo->getAttributeValue('tipo') == Model_Eixo::TIPO_MULTIPLA_ESCOLHA): ?>
	<div id="divQuestaoMultiplaEscolha">
		<?php require_once(APPRAIZ_VIEW . '/questoesmultiplaescolha/index.php'); ?>
	</div>
<?php endif; ?>
