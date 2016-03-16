<?php

class DemandaPartesAcao extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "demandasfies.demandapartesacao";
	const TIPO_PESSOA_AUTOR = 'A';
	const TIPO_PESSOA_REU = 'R';
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("dpaid");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */

	protected $arAtributos = array('dpaid' => null, 'dmdid' => null, 'dpacpf' => null, 'dpacnpj' => null, 'dpatipo' => null, 'dpastatus' => null, 'usucpfinclusao' => null, 'dpadtinclusao' => null, 'usucpfalteracao' => null, 'dpadtalteracao' => null, 'usucpfinativacao' => null, 'dpadtinativacao' => null, 'dpanome' => null,);


	public function formularioAutor($podeEditar)
	{
		$dmdid = $this->dmdid ? $this->dmdid : 0;
		$sql = $this->getSqlPessoa(DemandaPartesAcao::TIPO_PESSOA_AUTOR, $this->dmdid);
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		?>
		<div class="well">
			<?php if ($podeEditar): ?>
			<form id="form-autor" method="post" class="form-horizontal">
				<input name="dmdid" type="hidden" value="<?= $this->dmdid; ?>"> <input name="dpatipo" type="hidden" value="<?= DemandaPartesAcao::TIPO_PESSOA_AUTOR; ?>">
				<input name="action" type="hidden" value="salvar_parte_acao"> <input name="cpfcnpj" id="cpfcnpj_autor" type="hidden" value="cpf">
				<?php endif; ?>
				<fieldset class="form-horizontal">
					<legend>Autor(es) <span style="color: red">(<?php echo count($dados); ?>)</span></legend>

					<div class="form-group">
						<label for="dmddsc" class="col-lg-4 col-md-4 control-label">CPF/CNPJ:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label id="btn_cpf_autor" class="btn btn-primary active"> <input type="radio" name="dpacpfcnpj_radio" value="1"> CPF </label>
								<label id="btn_cnpj_autor" class="btn btn-primary"> <input type="radio" name="dpacpfcnpj_radio" value="2"> CNPJ </label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label id="dpacpfcnpj_autor_label" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> CPF:</label>

						<div class="col-lg-8 col-md-8 ">

							<input id="dpacpf_autor" name="dpacpf" type="text" class="form-control cpf" placeholder=""
								   required="required"> <span id="cpf_autor" class="help-block"></span>

							<input id="dpacnpj_autor" name="dpacnpj" type="text" class="form-control cnpj"
								   placeholder=""
								   required="required" style="display:none"> <span id="cnpj_autor" class="help-block"></span>

						</div>
					</div>

					<div class="form-group">
						<label for="dmddsc" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Nome:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dpanome_autor" name="dpanome" type="text" class="form-control" placeholder=""
								   required="required"
								   value="<?php echo $this->dpanome; ?>">
						</div>
					</div>

				</fieldset>
				<?php if ($podeEditar): ?>
					<div>
						<button title="Salvar" id="btn-salvar-autor" class="btn btn-success" type="button"><span
								class="glyphicon glyphicon-thumbs-up"></span> Salvar
						</button>
					</div>
				<?php endif; ?>
				<?php if ($podeEditar): ?>
			</form>
		<?php endif; ?>

			<div style="margin-top: 20px; background: #ffffff !important;">
				<?php
				$listagem = new Simec_Listagem();
				$listagem->setCabecalho(array('Autor', 'CPF / CNPJ'));
				if ($podeEditar) {
					$listagem->addAcao('delete', 'inativarAutor');
					$listagem->setAcaoComoCondicional('delete', array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
				}
				$listagem->esconderColunas(array('usucpfinclusao'));

				$listagem->setQuery($this->getSqlPessoa(DemandaPartesAcao::TIPO_PESSOA_AUTOR, $this->dmdid));
				$listagem->addCallbackDeCampo(array('dpacpf'), 'adicionarMascaraCpfCnpj');
				$listagem->render();
				?>
			</div>

		</div>

		<script type="text/javascript">
			$(function () {
				$('.cnpj').mask('99.999.999/9999-99');
				$('.cpf').mask('999.999.999-99');

				$('#btn_cnpj_autor').click(function () {
					$('#cpfcnpj_autor').val('cnpj');
					if ($('#btn_cnpj_autor').hasClass('active')) {
						$('#btn_cnpj_autor').removeClass('active');
					} else {
						$('#btn_cnpj_autor').addClass('active');
					}

					if ($('#btn_cnpj_autor').hasClass('active')) {
						$('#dpacpf_autor').hide();
						$('#dpacpf_autor').val('');

						$('#cpf_autor').html('');
						$('#dpacnpj_autor').show();
						$('#dpacpfcnpj_autor_label').html('CNPJ');
					}

				});

				$('#btn_cpf_autor').click(function () {
					$('#cpfcnpj_autor').val('cpf');
					if ($('#btn_cpf_autor').hasClass('active')) {
						$('#btn_cpf_autor').removeClass('active');
					} else {
						$('#btn_cpf_autor').addClass('active');
					}

					if ($('#btn_cpf_autor').hasClass('active')) {
						$('#dpacpf_autor').show();

						$('#dpacnpj_autor').hide();
						$('#dpacnpj_autor').val('');

						$('#cnpj_autor').html('');
						$('#dpacpfcnpj_autor_label').html('<span style="color: red;">*</span> CPF');
					}
				});

				$('#btn-salvar-autor').click(function () {
					var dpacpf_autor = $('#dpacpf_autor').val();
					var selecionouCpf = $('#btn_cpf_autor').hasClass('active');
					var dpanome_autor = $('#dpanome_autor').val();

					if (empty(dpacpf_autor) && selecionouCpf) {
						alert('Favor preencher o campo de CPF.');
						return false
					}
					if ( empty(dpanome_autor) ) {
						alert('Favor preencher o campo de Nome.');
						return false
					}

					options = {
						success: function () {
							jQuery("#div_listagem_autor").load('/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&action=form_parte_acao_autor&dmdid=' + $('#dmdid').val());
						}
					}
					jQuery("#form-autor").ajaxForm(options).submit();
				});

				$('#dpacnpj_autor').on('change', function () {
					var cnpj = str_replace(['.', ',', '/', '-'], [''], $('#dpacnpj_autor').val());
					$.post(window.location.href, {action: 'getPessoaJuridica', cnpj: cnpj }, function (data) {
						$('#cnpj_autor').html(data);
						$('#dpanome_autor').val(data);
					});
				});

				$('#dpacpf_autor').on('change', function () {
					$.post(window.location.href, {action: 'getPessoaFisica', cpf: $('#dpacpf_autor').val() }, function (data) {
						if(data['msg']){
							alert(data['msg']);
						}else{
							alert('Não existe nenhuma demanda vinculada para este CPF.');
						}
						$('#cpf_autor').html(data['nome']);
						$('#dpanome_autor').val(data['nome']);
					}, 'json');
				});
			});

			function inativarAutor(dpaid) {
				if (confirm('Deseja realmente excluir o registro?')) {
					window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=inativar_autor&dpaid=' + dpaid + '&dmdid=' + '<?= $this->dmdid; ?>';
				}
			}
			function empty(data) {
				if (typeof(data) == 'number' || typeof(data) == 'boolean') {
					return false;
				}
				if (typeof(data) == 'undefined' || data === null) {
					return true;
				}
				if (typeof(data.length) != 'undefined') {
					return data.length == 0;
				}
				var count = 0;
				for (var i in data) {
					if (data.hasOwnProperty(i)) {
						count++;
					}
				}
				return count == 0;
			}
		</script>
	<?php
	}

	public function formularioReu($podeEditar = '')
	{
		$tipoReu = DemandaPartesAcao::TIPO_PESSOA_REU;
		$dados = $this->recuperarTodos('*', array("dmdid = {$this->dmdid}", " dpatipo = '{$tipoReu}' "));
		$this->popularDadosObjeto($dados[0]);
		?>
		<div class="well">
			<?php if ($podeEditar): ?>
			<form id="form-reu" method="post" class="form-horizontal">
				<input name="dmdid" type="hidden" value="<?= $this->dmdid; ?>"> <input name="dpatipo" type="hidden" value="<?= DemandaPartesAcao::TIPO_PESSOA_REU; ?>">
				<input name="action" type="hidden" value="salvar_parte_acao"> <input name="cpfcnpj" id="cpfcnpj_reu" type="hidden" value="cpf">
				<?php endif; ?>
				<fieldset class="form-horizontal">
					<legend>Réu</legend>

					<div class="form-group">
						<label for="dmddsc" class="col-lg-4 col-md-4 control-label">CPF/CNPJ:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<?php $dpacnpj = $this->dpacnpj; ?>

								<label id="btn_cpf_reu"
									   class="btn btn-primary <?= (empty($dpacnpj) ? 'active' : ''); ?>"> <input type="radio" name="dpacpfcnpj_radio" value="1"> CPF </label>
								<label id="btn_cnpj_reu"
									   class="btn btn-primary <?= (!empty($dpacnpj) ? 'active' : ''); ?>"> <input type="radio" name="dpacpfcnpj_radio" value="2"> CNPJ </label>

							</div>
						</div>
					</div>

					<div class="form-group">
						<label id="dpacpfcnpj_label"
							   class="col-lg-4 col-md-4 control-label"><?= (empty($dpacnpj) ? '<span style="color: red;">*</span> CPF' : 'CNPJ'); ?>
							:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dpacpf" name="dpacpf" type="text" class="form-control cpf" placeholder=""
								   required="required" <?= (!empty($dpacnpj) ? 'style="display:none"' : ''); ?>
								> <span id="cpf_reu" class="help-block"></span>

							<input id="dpacnpj" name="dpacnpj" type="text" class="form-control cnpj" placeholder=""
								   required="required" <?= (empty($dpacnpj) ? 'style="display:none"' : ''); ?>
								> <span id="cnpj_reu" class="help-block"></span>

						</div>
					</div>

					<div class="form-group">
						<label for="dmddsc" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Nome:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dpanome" name="dpanome" type="text" class="form-control" placeholder=""
								   required="required"
								>
						</div>
					</div>

				</fieldset>
				<?php if ($podeEditar): ?>
					<div>
						<button title="Salvar" id="btn-salvar-reu" class="btn btn-success" type="button"><span
								class="glyphicon glyphicon-thumbs-up"></span> Salvar
						</button>
					</div>
				<?php endif; ?>
				<?php if ($podeEditar): ?>
			</form>
		<?php endif; ?>

			<div style="margin-top: 20px; background: #ffffff !important;">
				<?php
				$listagem = new Simec_Listagem();
				$listagem->setCabecalho(array('Réu', 'CPF / CNPJ'));

				if ($podeEditar) {
					$listagem->addAcao('delete', 'inativarAutor');
					$listagem->setAcaoComoCondicional('delete', array(array('campo' => 'usucpfinclusao', 'operacao' => 'igual', 'valor' => $_SESSION['usucpf'])));
				}

				$listagem->esconderColunas(array('usucpfinclusao'));

				$listagem->setQuery($this->getSqlPessoa(DemandaPartesAcao::TIPO_PESSOA_REU, $this->dmdid));
				$listagem->addCallbackDeCampo(array('dpacpf'), 'adicionarMascaraCpfCnpj');
				$listagem->render();
				?>
			</div>

		</div>

		<script type="text/javascript">
			$(function () {
				$('.cnpj').mask('99.999.999/9999-99');
				$('.cpf').mask('999.999.999-99');

				$('#btn_cnpj_reu').click(function () {
					$('#cpfcnpj_reu').val('cnpj');
					if ($('#btn_cnpj_reu').hasClass('active')) {
						$('#btn_cnpj_reu').removeClass('active');
					} else {
						$('#btn_cnpj_reu').addClass('active');
					}

					if ($('#btn_cnpj_reu').hasClass('active')) {
						$('#dpacpf').hide();
						$('#cnpj_reu').html('');
						$('#dpacnpj').show();
						$('#cpf_reu').html('');
						$('#dpacpfcnpj_label').html('CNPJ');
					}
				});

				$('#btn_cpf_reu').click(function () {
					$('#cpfcnpj_reu').val('cpf');
					if ($('#btn_cpf_reu').hasClass('active')) {
						$('#btn_cpf_reu').removeClass('active');
					} else {
						$('#btn_cpf_reu').addClass('active');
					}

					if ($('#btn_cpf_reu').hasClass('active')) {
						$('#dpacpf').show();
						$('#dpacnpj').hide();
						$('#dpacnpj').html('');
						$('#cnpj_reu').html('');
						$('#dpacpfcnpj_label').html('<span style="color: red;">*</span> CPF');
					}
				});

				$('#btn-salvar-reu').click(function () {
					if (empty($('#dpacpf').val() ) && $('#btn_cpf_reu').hasClass('active')) {
						alert('Favor preencher o campo de CPF.');
						return false
					}

					if ( empty($('#dpanome').val()) ) {
						alert('Favor preencher o campo de Nome.');
						return false
					}
					options = {
						success: function () {
							jQuery("#div_listagem_reu").load('/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&action=form_parte_acao_reu&dmdid=' + $('#dmdid').val());
						}
					}
					jQuery("#form-reu").ajaxForm(options).submit();
				});

				$('#dpacnpj').on('change', function () {
					var cnpj = str_replace(['.', ',', '/', '-'], [''], $('#dpacnpj').val());
					$.post(window.location.href, {action: 'getPessoaJuridica', cnpj: cnpj }, function (data) {
						$('#cnpj_reu').html(data);
						$('#dpanome').val(data);
					});
				});

				$('#dpacpf').on('change', function () {
					$.post(window.location.href, {action: 'getPessoaFisica', cpf: $('#dpacpf').val() }, function (data) {
						$('#cpf_reu').html(data);
						$('#dpanome').val(data);
					});
				});
			});
		</script>
	<?php
	}

	public function getSqlPessoa($tipo, $dmdid = 0)
	{

		$sql = "SELECT dpaid, dpanome,
                        (coalesce((dpacpf),'') || coalesce((dpacnpj),'')) as dpacpf,
                        usucpfinclusao
                    FROM demandasfies.demandapartesacao
                    WHERE dmdid = {$dmdid}
                      AND dpastatus = 'A'
                      AND dpatipo = '{$tipo}'
                    ORDER BY dpaid DESC";
		return $sql;
	}

	public function possuiAutorReuArquivo($dmdid)
	{

		$sql = "
SELECT   COUNT(*) AS qtd
	FROM demandasfies.demandapartesacao as dr
	WHERE dr.dmdid = {$dmdid}  AND dr.dpastatus = 'A' AND dr.dpatipo = 'R'
UNION ALL

SELECT   COUNT(*) AS qtd
	FROM demandasfies.demandapartesacao  as da
	WHERE da.dmdid = {$dmdid} AND da.dpastatus = 'A'  AND da.dpatipo = 'A'
UNION ALL

SELECT   COUNT(*) AS qtd
	FROM demandasfies.demandaarquivo
	 WHERE dmdid = {$dmdid} AND dmastatus = 'A' AND dmatipo = 'GE'
";
		$dados = $this->carregar($sql);

		if($dados){
			return ( count($dados) >= 3 && $dados[0]['qtd'] > 0 && $dados[1]['qtd'] > 0 && $dados[2]['qtd'] > 0 );
		}
		return false;
	}

	public function getDemandasVinculadasCpf($cpf, $tipo = 'A')
	{
		$cpf = trim(str_replace( '.', '', str_replace( '-', '', $cpf ) ));
		$sql = "SELECT  dmdid FROM demandasfies.demandapartesacao WHERE dpacpf = '{$cpf}'  AND dpastatus = 'A' AND dpatipo = 'A' ";
		$dados = $this->carregar($sql);
		$dados = $dados ? $dados : array();
		$dmdids =  array();
		foreach ($dados as $valor){
			$dmdids[] = $valor['dmdid'];
		}
		return $dmdids;
	}

}

function adicionarMascaraCpfCnpj($valor = '')
{
	if (strlen($valor) > 11) {
		return formatar_cnpj($valor);
	} else {
		return formatar_cpf($valor);
	}
}