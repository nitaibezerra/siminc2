<div class="card" style="margin-top: 30px">
	<div class="card-body ">
		<div class="col-sm-3">
			<div class="form-group">
				<input type="text" name="orgcnpj" id="orgcnpj" class="form-control cnpj" data-inputmask="'mask': '99.999.999/9999-99', 'showMaskOnHover': false" value="<?php echo $organizacao->orgcnpj; ?>">
				<label for="orgcnpj" class="control-label">CNPJ</label>
			</div>
		</div>
		<div class="col-sm-9">
			<div class="form-group">
				<input type="text" name="orgrazaosocial" id="orgrazaosocial" class="form-control" value="<?php echo $organizacao->orgrazaosocial; ?>">
				<label for="orgrazaosocial" class="control-label"><span class="campo_obrigatorio">*</span> Razão social</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="orgnomefantasia" id="orgnomefantasia" class="form-control" value="<?php echo $organizacao->orgnomefantasia; ?>">
				<label for="orgnomefantasia" class="control-label">Nome fantasia</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="orgresponsavel" id="orgresponsavel" class="form-control" required value="<?php echo $organizacao->orgresponsavel; ?>">
				<label for="orgresponsavel" class="control-label">Responsável pela Organizaçao</label>
			</div>
		</div>
		<?php $sites = $site->lista(
				array('sw.sitid', 'sw.sitnome', 'osw.osidsc'), null, 
				array('left' => array('criatividadeeducacao.organizacaositeweb osw' => "osw.sitid = sw.sitid AND osw.orgid = {$organizacao->orgid}")), 
				array('order' => 'sitordem', 'alias' => 'sw')); ?>
		<?php foreach ($sites as $data) : ?>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="sitid[<?php echo $data['sitid']; ?>]" id="sitid[<?php echo $data['sitid']; ?>]" class="form-control" value="<?php echo $data['osidsc']; ?>">
				<label for="sitid[<?php echo $data['sitid']; ?>]" class="control-label"><?php echo $data['sitnome']; ?></label>
			</div>
		</div>
		<?php endforeach; ?>
	</div>
</div>

<div class="card" style="margin-top: 30px">
	<div class="card-body valida-cep">
		<div class="row " style="margin-bottom: 20px;">
			<div class="col-sm-12">
				<div class="form-group">
					<label class="checkbox-inline checkbox-styled checkbox-primary">
						<input type="checkbox" value="1" id="orgsemicep" name="orgsemicep" <?php echo $organizacao->orgsemicep == 't' ? 'checked' : null; ?> ><span>Organização não possui CEP</span>
					</label>
				</div>
			</div>
		</div>
		<div class="row endereco">
			<div class="col-sm-2">
				<div class="form-group">
					<input type="text" name="orgcep" id="orgcep" class="form-control cep" maxlength="10" data-inputmask="'mask': '99.999-999', 'showMaskOnHover': false" value="<?php echo $organizacao->orgcep; ?>">
					<label for="orgcep" class="control-label">CEP</label>
				</div>
			</div>
		</div>
		<div class="row">
			<div class="col-sm-12">
				<div class="form-group">
					<input type="text" name="orglogradouro" id="orglogradouro" class="form-control" data-rule-minlength="2" value="<?php echo $organizacao->orglogradouro; ?>" required>
					<label for="orglogradouro" class="control-label"><span class="campo_obrigatorio">*</span> Logradouro</label>
				</div>
			</div>
		</div>
		<div class="row">
			<div class="col-sm-2">
				<div class="form-group">
					<input type="text" name="orgnumeroendereco" id="orgnumeroendereco" class="form-control" value="<?php echo $organizacao->orgnumeroendereco; ?>" required>
					<label for="orgnumeroendereco" class="control-label"><span class="campo_obrigatorio">*</span> Número</label>
				</div>
			</div>
			<div class="col-sm-5">
				<div class="form-group">
					<input type="text" name="orgcompendereco" id="orgcompendereco" class="form-control" value="<?php echo $organizacao->orgcompendereco; ?>">
					<label for="orgcompendereco" class="control-label">Complemento</label>
				</div>
			</div>
			<div class="col-sm-5">
				<div class="form-group">
					<input type="text" name="orgbairro" id="orgbairro" class="form-control" value="<?php echo $organizacao->orgbairro; ?>" required>
					<label for="orgbairro" class="control-label"><span class="campo_obrigatorio">*</span> Bairro</label>
				</div>
			</div>
		</div>
		<div class="row ">
			<div class="col-sm-5">
				<div class="form-group">
					<select class="form-control select uf select2-list" name="estuf" id="estuf" required>
						<option value=""></option>
						<?php foreach ($estados as $estado) : ?>
						<?php $selected = $organizacao->estuf == $estado['regcod'] ? 'selected="selected"' : null; ?>
							<option <?php echo $selected; ?> value="<?php echo $estado['regcod']; ?>"><?php echo $estado['descricaouf']; ?></option>
						<?php endforeach; ?>
					</select>
					<label for="estuf" class="control-label"><span class="campo_obrigatorio">*</span> UF</label>
				</div>
			</div>
			<div class="col-sm-7">
				<div class="form-group">
					<select class="form-control select select2-list" name="muncod" id="muncod" required>
						<option value=""></option>
						<?php foreach ($municipios as $dados) : ?>
						<?php $selected = $organizacao->muncod == $dados['muncod'] ? 'selected="selected"' : null; ?>
							<option <?php echo $selected; ?> value="<?php echo $dados['muncod']; ?>"><?php echo ($dados['mundsc']); ?></option>
						<?php endforeach; ?>
					</select>
					<label for="muncod" class="control-label"><span class="campo_obrigatorio">*</span> Cidade</label>
				</div>
			</div>
		</div>
	</div>
</div>

<div class="card" style="margin-top: 30px">
	<div class="card-body ">
		<div class="col-sm-12">
			<div class="form-group">
				<?php $grupos = $grupo->lista(array('gruid', 'grudsc'), null, null, array('order' => 'gruordem')); ?>
				<select name="gruid" id="gruid" class="form-control tipo-organizacao select select2-list" required>
					<option value=""></option>
					<?php foreach ($grupos as $data) : ?>
						<?php $marcado = $data['gruid'] == $organizacao->gruid ? 'selected="selected"' : null; ?>
						<option <?php echo $marcado; ?> value="<?php echo $data['gruid']; ?>"><?php echo $data['grudsc']; ?></option>
					<?php endforeach; ?>
				</select>
				<label for="gruid" class="control-label"><span class="campo_obrigatorio">*</span> Grupo de Organização</label>
			</div>
		</div>
		<div class="col-sm-12 campos_grupo_outro">
            <div class="form-group">
                <input type="text" name="ortdscoutro" id="ortdscoutro" class="form-control" value="<?php echo $organizacao->ortdscoutro; ?>">
                <label for="orgcompendereco" class="control-label"><span class="campo_obrigatorio">*</span> Especifique: </label>
            </div>
        </div>
        <div class="col-sm-12 campos_grupo">
            <div class="form-group">
                <select class="form-control select2-list" name="ortid" id="ortid" required>
                    <option value=""></option>
                    <?php $tipos = $organizacaoTipo->getByGrupo($organizacao->gruid);
                    foreach ($tipos as $dados) : ?>
                        <?php $selected = $organizacao->ortid == $dados['ortid'] ? 'selected="selected"' : null; ?>
                        <option <?php echo $selected; ?> value="<?php echo $dados['ortid']; ?>"><?php echo ($dados['ortdsc']); ?></option>
                    <?php endforeach; ?>
                </select>
                <label for="ortid" class="control-label"><span class="campo_obrigatorio">*</span> Tipo de Organização</label>
            </div>
		</div>
        <div class="col-sm-12 campos_grupo_publico">
            <div class="form-group">
                <?php $esferas = $organizacao->getEsferas(); ?>
                <select name="orgesfera" id="orgesfera" class="form-control tipo-organizacao select2-list" required>
                    <option value=""></option>
                    <?php foreach ($esferas as $codigo => $descricao) : ?>
                        <?php $marcado = $codigo == $organizacao->orgesfera ? 'selected="selected"' : null; ?>
                        <option <?php echo $marcado; ?> value="<?php echo $codigo; ?>"><?php echo $descricao; ?></option>
                    <?php endforeach; ?>
                </select>
                <label for="orgesfera" class="control-label"><span class="campo_obrigatorio">*</span> Esfera</label>
            </div>
        </div>
        <div class="col-sm-12 campos_grupo_publico">
            <div class="form-group">
                <?php $poderes = $organizacao->getPoderes(); ?>
                <select name="orgpoder" id="orgpoder" class="form-control tipo-organizacao select2-list" required>
                    <option value=""></option>
                    <?php foreach ($poderes as $codigo => $descricao) : ?>
                        <?php $marcado = $codigo == $organizacao->orgpoder ? 'selected="selected"' : null; ?>
                        <option <?php echo $marcado; ?> value="<?php echo $codigo; ?>"><?php echo $descricao; ?></option>
                    <?php endforeach; ?>
                </select>
                <label for="orgpoder" class="control-label"><span class="campo_obrigatorio">*</span> Poderes</label>
            </div>
        </div>
		<div class="col-sm-4">
			<div>
				<label class="control-label"><span class="campo_obrigatorio">*</span> Área de atuação na Educação</label>
			</div>
			<?php $atuacoes = $atuacao->lista(array('araid', 'aradsc'), null, null, array('order' => 'aradsc')); ?>
			<?php $marcados = $organizacaoAtuacao->lista(array('araid'), array("orgid = {$organizacao->orgid}")); ?>
			<?php foreach ($atuacoes as $data) : ?>
			<div class="checkbox checkbox-styled">
				<label>
					<?php $marcado = simec_multi_in_array($data['araid'], (array) $marcados) ? 'checked' : null; ?>
					<input class="check_area_atuacao" <?php echo $marcado; ?> name="araid[]" type="checkbox" value="<?php echo $data['araid']; ?>"  id="check_area_atuacao_<?php echo $data['araid']; ?>" required>
					<span><?php echo $data['aradsc']; ?></span>
				</label>
			</div>
			<?php endforeach; ?>
		</div>
		<div class="col-sm-4 campos_area_escolar">
            <div>
                <label class="control-label"><span class="campo_obrigatorio">*</span> Nível de ensino</label>
            </div>
            <?php $niveis = $nivel->lista(array('nieid', 'niedsc'), null, null, array('order' => 'niedsc')); ?>
            <?php $marcados = $organizacaoNivel->lista(array('nieid'), array("orgid = {$organizacao->orgid}")); ?>
            <?php foreach ($niveis as $data) : ?>
            <div class="checkbox checkbox-styled">
                <label>
                    <?php $marcado = simec_multi_in_array($data['nieid'], (array) $marcados) ? 'checked' : null; ?>
                    <input class="check_area_escolar" <?php echo $marcado; ?> name="nieid[]" type="checkbox" value="<?php echo $data['nieid']; ?>">
                    <span><?php echo $data['niedsc']; ?></span>
                </label>
            </div>
            <?php endforeach; ?>
        </div>
        <div class="col-sm-4 campos_area_escolar">
            <div>
                <label class="control-label"><span class="campo_obrigatorio">*</span> Rede</label>
            </div>
            <?php $redes = $rede->lista(array('redid', 'reddsc'), null, null, array('order' => 'reddsc')); ?>
            <?php $marcados = $organizacaoRede->lista(array('redid'), array("orgid = {$organizacao->orgid}")); ?>
            <?php foreach ($redes as $data) : ?>
            <div class="checkbox checkbox-styled">
                <label>
                    <?php $marcado = simec_multi_in_array($data['redid'], (array) $marcados) ? 'checked' : null; ?>
                    <input class="area_escolar" <?php echo $marcado; ?> name="redid[]" type="checkbox" value="<?php echo $data['redid']; ?>">
                    <span><?php echo $data['reddsc']; ?></span>
                </label>
            </div>
            <?php endforeach; ?>
        </div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="orgqtdfuncionarios" id="orgqtdfuncionarios" data-inputmask="'mask': '[999999]', 'showMaskOnHover': false" class="form-control" maxlength="5" value="<?php echo $organizacao->orgqtdfuncionarios; ?>" required>
				<label for="orgqtdfuncionarios" class="control-label"><span class="campo_obrigatorio">*</span> Número de funcionários/colaboradores</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<input type="text" name="orgqtdestudantes" id="orgqtdestudantes" data-inputmask="'mask': '[999999]', 'showMaskOnHover': false" class="form-control" maxlength="5" value="<?php echo $organizacao->orgqtdestudantes; ?>" required>
				<label for="orgqtdestudantes" class="control-label"><span class="campo_obrigatorio">*</span> Número de estudantes (aproximadamente)</label>
			</div>
		</div>
		<div class="col-sm-12">
			<label class="control-label"><span class="campo_obrigatorio">*</span> Idade dos estudantes/participantes</label>
		</div>
		<div class="col-sm-12">
			<?php $idades = $faixa->lista(array('faeid', 'faedsc'), null, null, array('order' => 'faeid')); ?>
			<?php $marcados = $organizacaoFaixa->lista(array('faeid'), array("orgid = {$organizacao->orgid}")); ?>
			<?php foreach ($idades as $data) : ?>
			<div class="checkbox checkbox-styled">
				<label>
					<?php $marcado = simec_multi_in_array($data['faeid'], (array) $marcados) ? 'checked' : null; ?>
					<input <?php echo $marcado; ?> name="faeid[]" type="checkbox" value="<?php echo $data['faeid']; ?>" required>
					<span><?php echo $data['faedsc']; ?></span>
				</label>
			</div>
			<?php endforeach; ?>
		</div>
	</div>
</div>

<div class="card">
	<div class="card-head card-head-xs style-danger">
		<header><i class="fa fa-youtube"></i> Video no youtube</header>
	</div>
	<div class="card-body">
		<div class="col-sm-12">
			<div class="form-group">
				<div class="input-group">
					<div class="input-group-content">
						<input type="url" name="orglinkvideo" id="orglinkvideo" data-rule-url="true" class="form-control youtube" value="<?php echo $organizacao->orglinkvideo; ?>">
						<label for="orglinkvideo" class="control-label">Link do youtube</label>
						<p class="help-block" style="right: 0 !important; left: none;">Por favor, envie via <b>youtube</b> um vídeo de até 5 (cinco) minutos mostrando o funcionamento da sua organização</p>
					</div>
					<span class="input-group-addon youtube-time"></span>
				</div>
			</div>
		</div>
	</div>
</div>

<script>setTimeout(function() {$('.valida-cep').trigger('change')}, 100);</script>

<?php if ($organizacao->orglinkvideo) : ?>
	<script>setTimeout(function() {$('#orglinkvideo').trigger('change')}, 500);</script>
<?php endif; ?>