<div class="card" style="margin-top: 30px">
	<div class="card-body ">
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="parnome" id="parnome" class="form-control" data-rule-minlength="2" value="<?php echo $particiante->parnome; ?>" disabled="disabled">
				<label for="parnome" class="control-label"><span class="campo_obrigatorio">*</span> Nome completo</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="parcargo" id="parcargo" class="form-control" data-rule-minlength="3" value="<?php echo $particiante->parcargo; ?>">
				<label for="parcargo" class="control-label">Cargo/Função</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<input type="text" name="partelefone" id="partelefone" class="form-control" data-inputmask="'mask': '(99) 9999-9999[9]', 'showMaskOnHover': false" maxlength="15" data-rule-minlength="14" value="<?php echo $particiante->partelefone; ?>">
				<label for="partelefone" class="control-label">Telefone</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<input type="text" name="parramal" id="parramal" class="form-control" data-rule-minlength="2" maxlength="5" value="<?php echo $particiante->parramal; ?>">
				<label for="parramal" class="control-label">Ramal</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<input type="text" name="parcelular" id="parcelular" class="form-control" maxlength="15" data-inputmask="'mask': '(99) 9999-9999[9]', 'showMaskOnHover': false" data-rule-minlength="14" value="<?php echo $particiante->parcelular; ?>">
				<label for="parcelular" class="control-label">Celular</label>
			</div>
		</div>
		<div class="col-sm-6">
			<div class="form-group">
				<input type="text" name="partelefoneoutro" id="partelefoneoutro" class="form-control" maxlength="15" data-inputmask="'mask': '(99) 9999-9999[9]', 'showMaskOnHover': false" data-rule-minlength="14" value="<?php echo $particiante->partelefoneoutro; ?>">
				<label for="partelefoneoutro" class="control-label">Outros</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="email" name="paremail" id="paremail" class="form-control" data-rule-email="true" value="<?php echo $particiante->paremail; ?>">
				<label for="paremail" class="control-label">Email</label>
			</div>
		</div>
	</div>
</div>