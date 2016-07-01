<?php if ($this->exibirAvaliacao): ?>

	<div class="panel with-nav-tabs panel-primary">
		<div class="panel-heading">
			<ul class="nav nav-tabs" id="tab_grupo">
				<?php
				$this->eixo->getMenuEixo( $this->questionario->getAttributeValue('queid') ); ?>
				<li><a data-toggle="tab" href="#tabConsideracoesFinais"><span class="glyphicon glyphicon-record" aria-hidden="true"></span> Considerações Finais</a></li>
			</ul>
		</div>

		<div class="panel-body">
			<div class="tab-content">

				<?php require_once(APPRAIZ_VIEW . 'avaliacao/_eixos.php'); ?>

				<div id="tabConsideracoesFinais" class="tab-pane fade">
					<?php require_once(APPRAIZ_VIEW . 'avaliacao/_consideracoes_finais.php'); ?>
				</div>
			</div>
		</div>
	</div>
<?php else: ?>
	<div>
		<div class="alert alert-danger" role="alert">
			<p>Cadastre as <strong>Descrições do Grupo</strong> para visualizar a Avaliação</p>
		</div>
	</div>
<?php endif; ?>
