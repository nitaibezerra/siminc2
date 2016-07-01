<div class="card" style="margin-top: 30px">
	<div class="card-body ">

        <div class="alert alert-warning">
            <p><i class="fa fa-bell-o"></i> Esta <b>chamada pública</b> busca encontrar organizações que desenvolvem ou pretendem desenvolver estratégias de articulação inovadoras e criativas. Sugerimos que preenchimento deste formulário seja feito como um processo de autorreflexão, envolvendo os diversos segmentos que compõem a organização.</p>
        </div>

		<?php $perguntas = $pergunta->lista(
				array('p.perid', 'p.pertexto', 'p.perresumo', 'r.restexto'), null,
				array('left' => array('criatividadeeducacao.resposta r' => "r.perid = p.perid AND r.queid = {$_SESSION['queid']}")),
				array('order' => 'p.perordem', 'alias' => 'p')); ?>
		<?php foreach ($perguntas as $data) : ?>
		<div class="col-sm-12">
			<label for="perid[<?php echo $data['perid']; ?>]" class="control-label" style="line-height: 18px">
				<span class="campo_obrigatorio">*</span> 
				<?php echo $data['pertexto']; ?>
			</label>
			<div class="form-group" style="padding-top: 0px;">
				<?php echo $data['restexto']; ?>
				<em class="help-block countdown" data-input="#perid_<?php echo $data['perid']; ?>" data-max-lenght="3000"></em>
			</div>
		</div>
		<?php endforeach; ?>
		<div class="col-sm-12">
		<div class="alert alert-callout alert-success">
	        <p>Estudante: A palavra refere-se a todas as pessoas que participam como público das organizações educativas, abrangendo termos como alunos, aprendizes, educandos, entre outros.</p>
	        <p>Agradecemos sua participação.</p>
	    </div>
	    </div>
	</div>
</div>