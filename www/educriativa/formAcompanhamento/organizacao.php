<div class="card" style="margin-top: 30px">
	<div class="card-body ">
		<div class="col-sm-3">
			<div class="form-group">
				<?php echo $organizacao->orgcnpj; ?>
				<label for="orgcnpj" class="control-label">CNPJ</label>
			</div>
		</div>
		<div class="col-sm-9">
			<div class="form-group">
				<?php echo $organizacao->orgrazaosocial; ?>
				<label for="orgrazaosocial" class="control-label">Razão social</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $organizacao->orgnomefantasia; ?>
				<label for="orgnomefantasia" class="control-label">Nome fantasia</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $organizacao->orgresponsavel; ?>
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
				<?php echo $data['osidsc']; ?>
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
					<?php echo $organizacao->orgcep; ?>
					<label for="orgcep" class="control-label">CEP</label>
				</div>
			</div>
		</div>
		<div class="row">
			<div class="col-sm-12">
				<div class="form-group">
					<?php echo $organizacao->orglogradouro; ?>
					<label for="orglogradouro" class="control-label"><span class="campo_obrigatorio">*</span> Logradouro</label>
				</div>
			</div>
		</div>
		<div class="row">
			<div class="col-sm-2">
				<div class="form-group">
					<?php echo $organizacao->orgnumeroendereco; ?>
					<label for="orgnumeroendereco" class="control-label"><span class="campo_obrigatorio">*</span> Número</label>
				</div>
			</div>
			<div class="col-sm-5">
				<div class="form-group">
					<?php echo $organizacao->orgcompendereco; ?>
					<label for="orgcompendereco" class="control-label">Complemento</label>
				</div>
			</div>
			<div class="col-sm-5">
				<div class="form-group">
					<?php echo $organizacao->orgbairro; ?>
					<label for="orgbairro" class="control-label"><span class="campo_obrigatorio">*</span> Bairro</label>
				</div>
			</div>
		</div>
		<div class="row ">
			<div class="col-sm-5">
				<div class="form-group">
                    <?php $sql = "select o.estuf, e.estdescricao, o.muncod, m.mundescricao
                                  from criatividadeeducacao.organizacao o
                                      left join territorios.estado e on e.estuf = o.estuf
                                      left join territorios.municipio m on m.muncod = o.muncod
                                  where orgid = {$organizacao->orgid} ";

                        $dados = $organizacao->pegaLinha($sql);
                        $dados = $dados ? $dados : array();
                    ?>
                    <?php echo $dados['estuf'] . ' - ' . $dados['estdescricao']; ?>
					<label for="estuf" class="control-label"><span class="campo_obrigatorio">*</span> UF</label>
				</div>
			</div>
			<div class="col-sm-7">
				<div class="form-group">
                    <?php echo $dados['mundescricao']; ?>
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
                <?php $sql = "  select grudsc, ortdsc, o.gruid,
                                        case
                                            when orgesfera = 'F' then 'Federal'
                                            when orgesfera = 'E' then 'Estadual'
                                            when orgesfera = 'M' then 'Municipal'
                                            else orgesfera
                                        end as orgesfera,
                                        case
                                            when orgpoder = 'L' then 'Legislativo'
                                            when orgpoder = 'E' then 'Executivo'
                                            when orgpoder = 'J' then 'Judiciário'
                                            else orgpoder
                                        end as orgpoder,
                                        ortdscoutro
                                from criatividadeeducacao.organizacao o
                                    left join criatividadeeducacao.grupo g on g.gruid = o.gruid
                                    left join criatividadeeducacao.organizacao_tipo t on t.ortid = o.ortid
                                where orgid = {$organizacao->orgid} ";

                $dados = $organizacao->pegaLinha($sql);
                $dados = $dados ? $dados : array();
                ?>
				<?php echo $dados['gruid'] == 5 ? $dados['ortdscoutro'] : $dados['grudsc']; ?>
				<label for="gruid" class="control-label"><span class="campo_obrigatorio">*</span> Grupo de Organização</label>
			</div>
		</div>
        <div class="col-sm-12 ">
            <div class="form-group">
                <?php echo $dados['ortdsc']; ?>
                <label for="ortid" class="control-label"><span class="campo_obrigatorio">*</span> Tipo de Organização</label>
            </div>
		</div>
        <div class="col-sm-12">
            <div class="form-group">
                <?php echo $dados['orgesfera']; ?>
                <label for="orgesfera" class="control-label"><span class="campo_obrigatorio">*</span> Esfera</label>
            </div>
        </div>
        <div class="col-sm-12">
            <div class="form-group">
                <?php echo $dados['orgpoder']; ?>
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
                <?php $marcado = simec_multi_in_array($data['araid'], (array) $marcados) ? 'checked' : null;
                if(!$marcado) continue;
                ?>
                <div class="checkbox checkbox-styled">
                    <label>
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
                <?php $marcado = simec_multi_in_array($data['nieid'], (array) $marcados) ? 'checked' : null;
                if(!$marcado) continue;
                ?>
                <div class="checkbox checkbox-styled">
                    <label>
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
                <?php $marcado = simec_multi_in_array($data['redid'], (array) $marcados) ? 'checked' : null;
                if(!$marcado) continue;
                ?>
                <div class="checkbox checkbox-styled">
                    <label>
                        <input class="area_escolar" <?php echo $marcado; ?> name="redid[]" type="checkbox" value="<?php echo $data['redid']; ?>">
                        <span><?php echo $data['reddsc']; ?></span>
                    </label>
                </div>
            <?php endforeach; ?>
        </div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $organizacao->orgqtdfuncionarios; ?>
				<label for="orgqtdfuncionarios" class="control-label"><span class="campo_obrigatorio">*</span> Número de funcionários/colaboradores</label>
			</div>
		</div>
		<div class="col-sm-12">
			<div class="form-group">
				<?php echo $organizacao->orgqtdestudantes; ?>
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
                <?php $marcado = simec_multi_in_array($data['faeid'], (array) $marcados) ? 'checked' : null;
                if(!$marcado) continue;
                ?>
                <div class="checkbox checkbox-styled">
                    <label>
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
                <?php echo $organizacao->orglinkvideo; ?>
                <label for="orglinkvideo" class="control-label"><span class="campo_obrigatorio">*</span> Link do youtube</label>
            </div>
        </div>
	</div>
</div>

<script>setTimeout(function() {$('.valida-cep').trigger('change')}, 100);</script>

<?php if ($organizacao->orglinkvideo) : ?>
	<script>setTimeout(function() {$('#orglinkvideo').trigger('change')}, 500);</script>
<?php endif; ?>