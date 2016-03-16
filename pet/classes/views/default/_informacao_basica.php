<div>
	<p><strong>IES:</strong> <?= $this->grupoInfos['instuicaoEnsinoSuperior'] ?> </p>

	<p><strong>Caracterização da Abrangência do Grupo:</strong> <?= $this->grupoInfos['abrangencia'] ?></p>

	<p><strong>CPF/Tutor:</strong> <?= $this->grupoInfos['cpftutor'] ?> - <?= $this->grupoInfos['nometutor'] ?></p>

	<p><strong>Início da Tutoria:</strong> <?= $this->grupoInfos['datainiciotutoria'] ?> </p>

	<p>
		<strong>Curso(s) que é (são) atendido(s) pelo grupo PET objeto da avaliação:</strong>
	<ul>
		<?php
		if (!empty($this->grupoInfos['nomecurso'])):
			foreach ($this->grupoInfos['nomecurso'] as $curso): ?>
				<li><?= $curso ?></li>
			<?php
			endforeach;
		endif; ?>
	</ul>
	</p>

	<div class="well">
		<div class="text-center">
			<h5> Lista de Estudantes que integram ou integraram o grupo PET no período de Avaliação </h5>
		</div>

		<?= $this->discente->getListaDiscentes($this->view->idGrupo); ?>
	</div>

</div>