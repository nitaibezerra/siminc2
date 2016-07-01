<div class="card" style="margin-top: 30px">
	<div class="card-body ">
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $particiante->parnome; ?>
				<label for="parnome" class="control-label"><span class="campo_obrigatorio">*</span> Nome completo</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $particiante->parcargo; ?>
				<label for="parcargo" class="control-label">Cargo/Função</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<?php echo $particiante->partelefone; ?>
				<label for="partelefone" class="control-label">Telefone</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<?php echo $particiante->parramal; ?>
				<label for="parramal" class="control-label">Ramal</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<?php echo $particiante->parcelular; ?>
				<label for="parcelular" class="control-label">Celular</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<?php echo $particiante->partelefoneoutro; ?>
				<label for="partelefoneoutro" class="control-label">Outros</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $particiante->paremail; ?>
				<label for="paremail" class="control-label">Email</label>
			</div>
		</div>
	</div>
</div>