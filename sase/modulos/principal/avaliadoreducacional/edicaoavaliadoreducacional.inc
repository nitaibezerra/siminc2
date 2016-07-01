<?php

	$colTitulo = 'col-sm-4';
	$colCampo  = 'col-sm-4';
	
	
	switch ($_REQUEST['requisicao']){
		case 'salvar':
			
			$arAtributos = array(
					'aveid' => $_POST['aveid'],
					'dsamatricula' => $_POST['dsamatricula'],
					'usucpf' => $_POST['usucpf'],
					'estuf' => $_POST['estuf'],
					'taeid' => $_POST['taeid'],
					'orvid' => $_POST['orvid'],
					'lotid' => $_POST['lotid'],
					'avenumdocindicacao' => $_POST['avenumdocindicacao'],
					'avedataindicacao' => $_POST['avedataindicacao'],
					'avenome' => $_POST['avenome'],
					'aveapelido' => $_POST['aveapelido'],
					'avesexo' => $_POST['avesexo'],
					'avedatanascimento' => $_POST['avedatanascimento'],
					'avenumcpf' => str_replace('-','',str_replace('.','', $_POST['avenumcpf'])),
					'aveidentidade' => $_POST['aveidentidade'],
					'aveorgaoexpedidor' => $_POST['aveorgaoexpedidor'],
					'avedataexpedicao' => $_POST['avedataexpedicao'],
					'avelotacao' => $_POST['avelotacao'],
					'avedtnascimento' => $_POST['avedtnascimento'],
					'aveemail' => $_POST['aveemail'],
					'aveskype' => $_POST['aveskype'],
					'aveendereco' => $_POST['aveendereco'],
					'muncod' => $_POST['muncod'],
					'avecep' => str_replace('-', '', $_POST['avecep']),
					'avelogradouro' => $_POST['avelogradouro'],
					'avenumero' => $_POST['avenumero'],
					'avecomplemento' => $_POST['avecomplemento'],
					'avebairro' => $_POST['avebairro'],
					'avedddtelcomercial' => $_POST['avedddtelcomercial'],
					'avetelcomercial' => $_POST['avetelcomercial'],
					'avedddtelcelular' => $_POST['avedddtelcelular'],
					'avetelcelular' => $_POST['avetelcelular'],
					'avedddtelresidencial' => $_POST['avedddtelresidencial'],
					'avetelresidencial' => $_POST['avetelresidencial'],
					'avedeficiencia' => $_POST['avedeficiencia'],
					'avequaltipodeficiencia' => $_POST['avequaltipodeficiencia'],
					'avecargoqueaposentou' => $_POST['avecargoqueaposentou'],
					'avebanco' => $_POST['avebanco'],
					'aveagencia' => $_POST['aveagencia'],
					'aveconta' => $_POST['aveconta'],
					'avedtinclusao' => null,
					'avestatus' => 'A',
                    'avediretoria' => $_POST['avediretoria']);
							
				unset($_POST['aveid']);

                if (!validaData($arAtributos['avedatanascimento'])){
                    echo "
                                        <script>
                                            alert('Data de nascimento inválida.');
                                            window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
                                        </script>";
                    exit;
                }

                if (!validaData($arAtributos['avedataexpedicao'])){
                    echo "
                                        <script>
                                            alert('Data de expedição inválida.');
                                            window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
                                        </script>";
                    exit;
                }

			    if (!validaData($arAtributos['avedataindicacao'])){
                    echo "
									<script>
										alert('Data de indicação inválida.');
										window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
									</script>";
                    exit;
                }

				$avaliadoreducacional = $avaliadoreducacional->popula($arAtributos);
				
				if (validaCPF($_POST['avenumcpf']) == '0'){
					echo "
					<script>
						alert('CPF inválido.');
						//window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&acao=A&aba=cadastro';
					</script>";
					//exit;
				} else {
					$ret = $avaliadoreducacional->validaCpfAvaliador();
					//ver($ret, d);
					if ($ret['cod'] == 0){
						$res = $avaliadoreducacional->cadastrar();
		// 				/ver($res, d);
						if (!$res){
							echo "
									<script>
										alert('Fala ao cadastrar o avaliador.');
										window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
									</script>";
							exit;
						} else {
						//ver($res, d);
							echo "
								<script>
									alert('Avaliador Educacional cadastrado com sucesso.');
									window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
								</script>";
							exit;
						}
					} else {
						echo "
								<script>
									alert('{$ret['msg']}');
									window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro';
								</script>";
						exit();
					}
				}
			break;
			
		case 'alterar':
			if(!empty($_POST['aveid'])){
				$arAtributos = array(
						'aveid' => $_POST['aveid'],
						'dsamatricula' => $_POST['dsamatricula'],
						'usucpf' => $_POST['usucpf'],
						'estuf' => $_POST['estuf'],
						'taeid' => $_POST['taeid'],
						'orvid' => $_POST['orvid'],
						'lotid' => $_POST['lotid'],
						'avenumdocindicacao' => $_POST['avenumdocindicacao'],
						'avedataindicacao' => $_POST['avedataindicacao'],
						'avenome' => $_POST['avenome'],
						'aveapelido' => $_POST['aveapelido'],
						'avesexo' => $_POST['avesexo'],
						'avedatanascimento' => $_POST['avedatanascimento'],
						'avenumcpf' => str_replace('-','',str_replace('.','', $_POST['avenumcpf'])),
						'aveidentidade' => $_POST['aveidentidade'],
						'aveorgaoexpedidor' => $_POST['aveorgaoexpedidor'],
						'avedataexpedicao' => $_POST['avedataexpedicao'],
						'avelotacao' => $_POST['avelotacao'],
						'avedtnascimento' => $_POST['avedtnascimento'],
						'aveemail' => $_POST['aveemail'],
						'aveskype' => $_POST['aveskype'],
						'aveendereco' => $_POST['aveendereco'],
						'muncod' => $_POST['muncod'],
						'avecep' => str_replace('-', '', $_POST['avecep']),
						'avelogradouro' => $_POST['avelogradouro'],
						'avenumero' => $_POST['avenumero'],
						'avecomplemento' => $_POST['avecomplemento'],
						'avebairro' => $_POST['avebairro'],
						'avedddtelcomercial' => $_POST['avedddtelcomercial'],
						'avetelcomercial' => $_POST['avetelcomercial'],
						'avedddtelcelular' => $_POST['avedddtelcelular'],
						'avetelcelular' => $_POST['avetelcelular'],
						'avedddtelresidencial' => $_POST['avedddtelresidencial'],
						'avetelresidencial' => $_POST['avetelresidencial'],
						'avedeficiencia' => $_POST['avedeficiencia'],
						'avequaltipodeficiencia' => $_POST['avequaltipodeficiencia'],
						'avecargoqueaposentou' => $_POST['avecargoqueaposentou'],
						'avebanco' => $_POST['avebanco'],
						'aveagencia' => $_POST['aveagencia'],
						'aveconta' => $_POST['aveconta'],
						'avedtinclusao' => null,
						'avestatus' => 'A',
                        'avediretoria' => $_POST['avediretoria']);
								
					$avaliadoreducacional = $avaliadoreducacional->popula($arAtributos);

                    if (!validaData($arAtributos['avedatanascimento'])){
                        echo "
                                            <script>
                                                alert('Data de nascimento inválida.');
                                                window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
                                            </script>";
                        exit;
                    }

                    if (!validaData($arAtributos['avedataexpedicao'])){
                        echo "
                                            <script>
                                                alert('Data de expedição inválida.');
                                                window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
                                            </script>";
                        exit;
                    }

                    if (!validaData($arAtributos['avedataindicacao'])){
                        echo "
                                        <script>
                                            alert('Data de indicação inválida.');
                                            window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
                                        </script>";
                        exit;
                    }

					if (validaCPF($_POST['avenumcpf']) == '0'){
						echo "
							<script>
								alert('CPF inválido.');
							</script>";
					} else {
							
						$res = $avaliadoreducacional->atualizar();
						//ver($avaliadoreducacional->arAtributos, d);
						if (!$res){
							echo "
									<script>
										alert('Falha ao alterar os dados do avaliador.');
										window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
									</script>";
							exit;
						} else {
							//ver($res, d);
							echo "
								<script>
									alert('Avaliador Educacional alterado com sucesso.');
									window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
								</script>";
							exit;
						}
					}
				}
			break;
					
		case 'salvar_experiencia':
				$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
				$arAtributos = array(
						'eaeid' => null,
						'aveid' => $_GET['aveid'],
						'eaeorgaoquetrabalha' => $_POST['eaeorgaoquetrabalha'],
						'eaediretoriacoordenacaosetor' => $_POST['eaediretoriacoordenacaosetor'],
						'eaecargo' => $_POST['eaecargo'],
						'eaefuncao' => $_POST['eaefuncao'],
						'eaecargoqueaposentou' => $_POST['eaecargoqueaposentou']
				);
				
				$experienciaavaliadoreducacional = $experienciaavaliadoreducacional->popula($arAtributos);
				$res = $experienciaavaliadoreducacional->cadastrar();
				if ($res){
					echo "
						<script>
							alert('Experiência Profissional cadastrada com sucesso.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
				} else {
					echo "
						<script>
							alert('Falha ao cadastrar a Experiência Profissional.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
				}
				break;
			
		case 'alterar_experiencia':
				$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
				$arAtributos = array(
						'eaeid' => $_POST['eaeid'.$i],
						'aveid' => $_GET['aveid'],
						'eaeorgaoquetrabalha' => $_POST['eaeorgaoquetrabalha'],
						'eaediretoriacoordenacaosetor' => $_POST['eaediretoriacoordenacaosetor'],
						'eaecargo' => $_POST['eaecargo'],
						'eaefuncao' => $_POST['eaefuncao'],
						'eaecargoqueaposentou' => $_POST['eaecargoqueaposentou']
				);
												
				$experienciaavaliadoreducacional = $experienciaavaliadoreducacional->popula($arAtributos);
				$res = $experienciaavaliadoreducacional->atualizar();
				if ($res){
					echo "
						<script>
							alert('Experiência Profissional alterada com sucesso.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
				} else {
					echo "
						<script>
							alert('Falha ao alterar a Experiência Profissional.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
				}
			break;
			
		case 'apagar_experiencia':
			$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
			$res = $experienciaavaliadoreducacional->excluir($_POST['eaeid']);
				
			if ($res){
				echo "
									<script>
										alert('Experiência Profissional inativado com sucesso.');
									</script>";
			} else {
				echo "
									<script>
										alert('Falha ao inativar a Experiência Profissional.');
									</script>";
			}
			$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();
			$experienciaavaliadoreducacional->aveid = $_GET['aveid'];
			break;
			
		case 'salvar_graduacao':
										
			$res = $avaliadoreducacional->cadastraGraduacao($_POST['grdid']);
			
			if ($res) {
				echo "
									<script>
										alert('Graduação cadastrada com sucesso.');
									</script>";
				} else {
					echo "
									<script>
										alert('Falha ao cadastrar a Graduação.');
									</script>";
				}
			
			break;
			
		case 'apagar_graduacao':
			$res = $avaliadoreducacional->apagaGraduacao($_POST['grd']);
				
			if ($res) {
				echo "
									<script>
										alert('Graduação apagada com sucesso.');
									</script>";
			} else {
				echo "
									<script>
										alert('Falha ao apagar o Graduação.');
									</script>";
			}
				
			break;
			
		case 'salvar_curso':
			$dados = array(
					'sbcid' => $_POST['sbcid'],
					'icptipo' => $_POST['icptipo']
			);
										
			$res = $avaliadoreducacional->cadastraCurso($dados);
			
			if ($res){
				echo "
						<script>
							alert('Curso cadastrado com sucesso.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
			} else {
				echo "
						<script>
							alert('Falha ao cadastrar o Curso.');
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
			}
			break;
			
		case 'apagar_curso':
			$res = $avaliadoreducacional->apagaCurso($_POST['icpid']);
			
			if ($res) {
				echo "
									<script>
										alert('Curso apagado com sucesso.');
									</script>";
			} else {
				echo "
									<script>
										alert('Falha ao apagar o Curso.');
									</script>";
			}
				
			break;
			
		case 'salvar_arquivo':
			// -- Verifica se tem algum arquivo para salvar no servidor
			if ($_FILES['file']['size'] > 0) {
			
				$nomeArquivo = $_REQUEST['aiddsc'];
			
				$descricao = explode(".", $_FILES['file']['name']);
				$campos = array(
						"aveid" => "'" . $avaliadoreducacional->aveid . "'",
						"usucpf" => "'" . $_POST['usucpf'] . "'",
						"aiddsc" => "'" . $nomeArquivo . "'",
						"aiddtinclusao" => "'" . date('Y-m-d H:i:s') . "'",
						"aidstatus" => "'A'"
				);
				
				$file = new FilesSimec("arquivoindicacao", $campos, "sase");
				$arquivoSalvo = $file->setUpload($_FILES ['file']['name'], '', true);
				if ($arquivoSalvo) {
					echo "<script>alert('Arquivo cadastrado com sucesso!');</script>";
				}
			}
				
			break;
			
		case 'baixar_arquivo':
			$file = new FilesSimec();
			if ($_POST['arqid']) {
				ob_clean();
				$arquivo = $file->getDownloadArquivo($_POST['arqid']);
				ver($_POST['arqid'], d);
				echo "
						<script>
							window.location.href = 'sase.php?modulo=principal/avaliadoreducacional&avediretoria={$avediretoria}&acao=A&aba=cadastro&aveid='+".$avaliadoreducacional->aveid.";
						</script>";
			}
							
			break;
			
		case 'inativar_arquivo':
			$res = $avaliadoreducacional->inativarArquivo($_POST['aidid']);
			if ($res) {
				echo "
									<script>
										alert('Arquivo inativado com sucesso.');
									</script>";
			} else {
				echo "
									<script>
										alert('Falha ao inativar o arquivo.');
									</script>";
			}
							
			break;
	}
	
?>

<style>
	.titulo-grupo {
		font-weight: bold;
/* 		margin-bottom: 5px; */
	}
	
	.divSeparador {
		border-top: 1px solid #e5e5e5 !important;
	}
	
	div.form-group{
		margin-bottom: 15px !important;
	}
	
</style>

<script language="javascript" type="text/javascript">

	function carregaMunicipioPelaUf(seluf, id) {
	    divCarregando();
	    jQuery.ajax({
	        type: 'POST',
	        url: 'sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=@$avediretoria?>&acao=A',
	        data: {requisicao: 'carrega_municipio', seluf: seluf, id: id},
	        async: false,
	        success: function(data) {
		        console.log(1);
	            $('#divMunicipio').html(data);
	            console.log(data);
	            divCarregado();
	        }
	    });
	}

	function carregaSupArea(arcid) {
	    divCarregando();
	    jQuery.ajax({
	        type: 'POST',
	        url: 'sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=@$avediretoria?>&acao=A',
	        data: {requisicao: 'carrega_subarea', arcid: arcid},
	        async: false,
	        success: function(data) {
		        console.log(1);
	            $('#divSubArea').html(data);
	            console.log(data);
	            divCarregado();
	        }
	    });
	}
	
	function editaExperiencia(id){
	    divCarregando();
	    jQuery.ajax({
	        type: 'POST',
	        url: 'sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=@$avediretoria?>&acao=A',
	        data: {requisicao: 'carrega_experiencia', eaeid: id},
	        async: false,
	        success: function(data) {
		        var d = data.split("|");
		        console.log(data);
				$('#eaeid').val(d[0]);
				$('#acao').val(d[1]);
				$('#eaeorgaoquetrabalha').val(d[2]);
				$('#eaediretoriacoordenacaosetor').val(d[3]);
				$('#eaecargo').val(d[4]);
				$('#eaefuncao').val(d[5]);

				$('#labeleaecargoqueaposentou1').attr('class', 'btn btn-default');
				$('#labeleaecargoqueaposentou2').attr('class', 'btn btn-default');

				$('#labeleaecargoqueaposentou1').removeAttr('checked');
				$('#labeleaecargoqueaposentou2').removeAttr('checked');
												
				$('#'+d[7]).attr('class', 'btn btn-default active');
				$('#'+d[8]).attr('checked', 'checked');

				divCarregado();
			}
	    });
		//jQuery('[name=eaeid]').val(id);
		//jQuery('[name=requisicao]').val('carrega_experiencia');
		//jQuery('[name=formCadastroLista]').submit();
	}

	function apagaExperiencia(id){
		if (confirm('Deseja inativar a Experiência Profissional?')) {
			jQuery('[name=eaeid]').val(id);
			jQuery('[name=requisicao]').val('apagar_experiencia');
	 		jQuery('[name=formCadastroLista]').submit();
		}
	}
		
	function showTipoDeficiencia(v) {
		if ( v == 't'){
			$('#divTipoDeficiencia').removeAttr('style');
		} else {
			$('#divTipoDeficiencia').attr('style','display: none;');
		}
	}
	
</script>

<!-- html -->
<div id="container">

<div class="row">
<div class="col-md-12">


<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal" enctype="multipart/form-data">
<input type="hidden" id="requisicao" name="requisicao" value="" />
<input name="formulario" value="1" type="hidden"/>
<input name="aveid" id="aveid" value="<?=@$_GET['aveid']?>" type="hidden"/>
<input name="usucpf" value="<?=@$_SESSION['usucpf']?>" type="hidden"/>
<input name="hidCont" id="hidCont" value="<?php echo $qtdExperiencia ?>" type="hidden" />
    <input type="hidden" name="avediretoria" id="avediretoria" value="<?=@$avediretoria?>"/>
				
<div class="col-md-6">

<div class="well">
<fieldset class="form-horizontal">
<legend>Dados Pessoais</legend>

	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Nome:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenome" name="avenome" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['avenome'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Apelido:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveapelido" name="aveapelido" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $avaliadoreducacional->arAtributos['aveapelido'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdmandatoseguranca" class="col-lg-4 col-md-4 control-label obrigatorio">Sexo:</label>
	
	    <div class="col-lg-8 col-md-8">
	        <div class="btn-group " data-toggle="buttons">
	            <label class="btn btn-default <?= ($avaliadoreducacional->arAtributos['avesexo'] == 'M' ? 'active' : ''); ?>">
	                <input type="radio" name="avesexo" class="avesexo" id="avesexo1" value="M" <?= ($avaliadoreducacional->arAtributos['avesexo'] == 'M' ? 'checked="checked"' : ''); ?>> Masculino
	            </label>
	            <label class="btn btn-default <?= ($avaliadoreducacional->arAtributos['avesexo'] == 'F' ? 'active' : ''); ?>">
	                <input type="radio" name="avesexo" class="avesexo" id="avesexo2" value="F" <?= ($avaliadoreducacional->arAtributos['avesexo'] == 'F' ? 'checked="checked"' : ''); ?>> Feminino
	            </label>
	        </div>
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Matrícula SIAPE:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="dsamatricula" name="dsamatricula" type="text"
	               class="form-control" placeholder="" maxlength="8" value="<?php echo $avaliadoreducacional->arAtributos['dsamatricula'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Data de nascimento:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <?php $avedatanascimento = $avaliadoreducacional->arAtributos['avedatanascimento'] ? substr($avaliadoreducacional->arAtributos['avedatanascimento'], 8, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedatanascimento'], 5, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedatanascimento'], 0, 4) : ''; ?>
	        <input id="avedatanascimento" name="avedatanascimento" type="text" class="campoData form-control"
	               placeholder="" value="<?php echo $avedatanascimento ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">CPF:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenumcpf" name="avenumcpf" type="text"
	               class="campoCpf form-control" placeholder="" value="<?php echo $avaliadoreducacional->arAtributos['avenumcpf'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">RG:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveidentidade" name="aveidentidade" type="text"
	               class="form-control" placeholder="" maxlength="20" value="<?php echo $avaliadoreducacional->arAtributos['aveidentidade'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Órgão Expedidor:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveorgaoexpedidor" name="aveorgaoexpedidor" type="text"
	               class="form-control" placeholder="" maxlength="20" value="<?php echo $avaliadoreducacional->arAtributos['aveorgaoexpedidor'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Data de expedição:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <?php $avedataexpedicao = $avaliadoreducacional->arAtributos['avedataexpedicao'] ? substr($avaliadoreducacional->arAtributos['avedataexpedicao'], 8, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedataexpedicao'], 5, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedataexpedicao'], 0, 4) : ''; ?>
	        <input id="avedataexpedicao" name="avedataexpedicao" type="text" class="campoData form-control"
	               placeholder="" value="<?php echo $avedataexpedicao ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Endereço:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveendereco" name="aveendereco" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['aveendereco'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Logradouro:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avelogradouro" name="avelogradouro" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['avelogradouro'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Número:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenumero" name="avenumero" type="text"
	               class="form-control" placeholder="" maxlength="6" value="<?php echo $avaliadoreducacional->arAtributos['avenumero'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Complemento:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avecomplemento" name="avecomplemento" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['avecomplemento'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">UF:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getUfs('munuf', "carregaMunicipioPelaUf(this.value, 'muncod')", $estuf) ?>
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">Cidade:</label>
	
	    <div class="col-lg-8 col-md-8 " id="divMunicipio">
            <?php echo $avaliadoreducacional->getMunicipios('muncod') ?>
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Bairro:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avebairro" name="avebairro" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['avebairro'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">CEP:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avecep" name="avecep" type="text"
	               class="form-control campoCep" placeholder="" value="<?php echo $avaliadoreducacional->arAtributos['avecep'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Tel. Comercial:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avetelcomercial" name="avetelcomercial" type="text"
	               class="form-control campoTel" placeholder="" value="<?php echo $telcom ?> ">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Tel. Residencial:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avetelresidencial" name="avetelresidencial" type="text"
	               class="form-control campoTel" placeholder="" value="<?php echo $telres ?> ">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">Tel. Celular:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avetelcelular" name="avetelcelular" type="text"
	               class="form-control campoTel" placeholder="" value="<?php echo $telcel ?> ">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label obrigatorio">E-mail Pessoal:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveemail" name="aveemail" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['aveemail'] ?> ">
	    </div>
	</div>
	<div class="form-group">
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Skype:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveskype" name="aveskype" type="text"
	               class="form-control" placeholder="" maxlength="100" value="<?php echo $avaliadoreducacional->arAtributos['aveskype'] ?> ">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdmandatoseguranca" class="col-lg-4 col-md-4 control-label obrigatorio">Possui alguma deficiência?</label>
	
	    <div class="col-lg-8 col-md-8">
	        <div class="btn-group " data-toggle="buttons">
	            <label class="btn btn-default <?= ($avaliadoreducacional->arAtributos['avedeficiencia'] == 't' ? 'active' : ''); ?>" onclick="showTipoDeficiencia('t')">
	                <input type="radio" name="avedeficiencia" id="avedeficiencia1" value="t" <?= ($avaliadoreducacional->arAtributos['avedeficiencia'] == 't' ? 'checked="checked"' : ''); ?>> Sim
	            </label>
	            <label class="btn btn-default <?= ($avaliadoreducacional->arAtributos['avedeficiencia'] == 'f' ? 'active' : ''); ?>" onclick="showTipoDeficiencia('f')">
	                <input type="radio" name="avedeficiencia" id="avedeficiencia2" value="f" <?= ($avaliadoreducacional->arAtributos['avedeficiencia'] == 'f' ? 'checked="checked"' : ''); ?>> Não
	            </label>
	        </div>
	    </div>
	</div>
	<div class="form-group" id="divTipoDeficiencia" <?php echo $_GET['aveid'] == '' || $avaliadoreducacional->arAtributos['avedeficiencia'] == 'f' ? 'style="display: none;"' : '' ?>>
	    <label for="avenome" class="col-lg-4 col-md-4 control-label">Tipo de deficiência:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avequaltipodeficiencia" name="avequaltipodeficiencia" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $avaliadoreducacional->arAtributos['avequaltipodeficiencia'] ?> ">
	    </div>
	</div>
	
</fieldset>
</div>

<div class="well">
<fieldset class="form-horizontal">
<legend>Indicação</legend>

	<div class="form-group">
	    <label for="dmdvara" class="col-lg-4 col-md-4 control-label obrigatorio">Nº documento:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avenumdocindicacao" name="avenumdocindicacao" type="text"
	               class="form-control" placeholder="" value="<?php echo $avaliadoreducacional->arAtributos['avenumdocindicacao'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Data indicação:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <?php $avedataindicacao = $avaliadoreducacional->arAtributos['avedataindicacao'] ? substr($avaliadoreducacional->arAtributos['avedataindicacao'], 8, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedataindicacao'], 5, 2) . '/' . substr($avaliadoreducacional->arAtributos['avedataindicacao'], 0, 4) : ''; ?>
	        <input id="avedataindicacao" name="avedataindicacao" type="text" class="campoData form-control"
	               placeholder="" value="<?php echo $avedataindicacao ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">Órgão:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getOrgaosVinculacao() ?>
	    </div>
	</div>
	
</fieldset>
</div>

<?php if(!empty($_GET['aveid'])) { ?>
<div class="well">
<fieldset class="form-horizontal">
<legend>Documento(s) Anexo(s)</legend>

	<div class="form-group">
		<label for="aiddsc" class="col-lg-4 col-md-4 control-label">Arquivo:</label>
		
		<div class="col-lg-8 col-md-8 ">
			<input type="hidden" name="arqid" id="arqid" >
			<input type="hidden" name="aidid" id="aidid" >
			<input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
		</div>
	</div>
	<div class="form-group">
		<label for="aiddsc" class="col-lg-4 col-md-4 control-label">Descrição:</label>
		
		<div class="col-lg-8 col-md-8 ">
			<input id="aiddsc" name="aiddsc" type="text" class="form-control" placeholder="" maxlength="50" >
		</div>
	</div>
		
	<div class="text-left">
		<button title="Salvar" class="btn btn-success" type="button" id="btnSalvarArq" onclick="salvaArquivo()"><span
			class="glyphicon glyphicon-thumbs-up"></span> Salvar
		</button>
	</div>
	
	<div id="divDadosArquivos">
		<?php 
			if ($_GET['aveid'] != ''){
				echo $avaliadoreducacional->listarArquivo();
			}
		?>
	</div>
	
</fieldset>
</div>
<?php } ?>

</div>

<div class="col-md-6">
<div class="well">
<fieldset class="form-horizontal">
<legend><?=($_GET['aveid'])?'':'Novo&nbsp;'?>Avaliador Educacional</legend>

            
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">Perfil:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getTipos() ?>
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">UF:</label>
	
	    <div class="col-lg-8 col-md-8 ">
            <?php echo $avaliadoreducacional->getUfs('estuf') ?>
	    </div>
	</div>
	<div class="form-group">
	    <label for="lotid" class="col-lg-4 col-md-4 control-label obrigatorio">Unidade de Atuação:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	    	<?php echo $avaliadoreducacional->getLotacoes(); ?>
	    </div>
	</div>
	
</fieldset>
</div>

<div class="well">
<fieldset class="form-horizontal">
<legend>Dados Bancários</legend>

	<div class="form-group">
	    <label for="dmdvara" class="col-lg-4 col-md-4 control-label obrigatorio">Banco:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="avebanco" name="avebanco" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $avaliadoreducacional->arAtributos['avebanco'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdvara" class="col-lg-4 col-md-4 control-label obrigatorio">Agência:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveagencia" name="aveagencia" type="text"
	               class="form-control" placeholder="" maxlength="20"value="<?php echo $avaliadoreducacional->arAtributos['aveagencia'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="dmdvara" class="col-lg-4 col-md-4 control-label obrigatorio">CC:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="aveconta" name="aveconta" type="text"
	               class="form-control" placeholder="" maxlength="20" value="<?php echo $avaliadoreducacional->arAtributos['aveconta'] ?>">
	    </div>
	</div>
	
</fieldset>
</div>


<?php 
if (!empty($_GET['aveid'])){
?>

<div class="well">
<fieldset class="form-horizontal">
<legend>Experiências Profissionais</legend>

	<div><div id="divSeparador" class="divSeparador" style="display: none;"><br/></div></div>
	<div class="form-group">
		<input name="eaeid" id="eaeid" type="hidden" value="<?php echo $experienciaavaliadoreducacional->eaeid ?>"/>
		<input name="acao" id="acao" type="hidden" value=""/>
		<label for="eaeorgaoquetrabalha" class="col-lg-4 col-md-4 control-label">Órgão:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="eaeorgaoquetrabalha" name="eaeorgaoquetrabalha" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $experienciaavaliadoreducacional->arAtributos['eaeorgaoquetrabalha'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="eaediretoriacoordenacaosetor" class="col-lg-4 col-md-4 control-label">Diretoria / Coordenação / Setor:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="eaediretoriacoordenacaosetor" name="eaediretoriacoordenacaosetor" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $experienciaavaliadoreducacional->arAtributos['eaediretoriacoordenacaosetor'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="eaecargo" class="col-lg-4 col-md-4 control-label">Cargo:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="eaecargo" name="eaecargo" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $experienciaavaliadoreducacional->arAtributos['eaecargo'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="eaefuncao" class="col-lg-4 col-md-4 control-label">Função:</label>
	
	    <div class="col-lg-8 col-md-8 ">
	        <input id="eaefuncao" name="eaefuncao" type="text"
	               class="form-control" placeholder="" maxlength="50" value="<?php echo $experienciaavaliadoreducacional->arAtributos['eaefuncao'] ?>">
	    </div>
	</div>
	<div class="form-group">
	    <label for="eaecargoqueaposentou" class="col-lg-4 col-md-4 control-label">Foi o cargo que aposentou?</label>
	
	    <div class="col-lg-8 col-md-8">
	        <div class="btn-group " data-toggle="buttons">
	            <label class="btn btn-default <?= ($experienciaavaliadoreducacional->arAtributos['eaecargoqueaposentou'] == 't' ? 'active' : ''); ?>" id="labeleaecargoqueaposentou1">
	                <input type="radio" name="eaecargoqueaposentou" id="eaecargoqueaposentou1" value="t" <?= ($experienciaavaliadoreducacional->arAtributos['eaecargoqueaposentou'] == 't' ? 'checked="checked"' : ''); ?>> Sim
	            </label>
	            <label class="btn btn-default <?= ($experienciaavaliadoreducacional->arAtributos['eaecargoqueaposentou'] == 'f' ? 'active' : ''); ?>" id="labeleaecargoqueaposentou2">
	                <input type="radio" name="eaecargoqueaposentou" id="eaecargoqueaposentou2" value="f" <?= ($experienciaavaliadoreducacional->arAtributos['eaecargoqueaposentou'] == 'f' ? 'checked="checked"' : ''); ?>> Não
	            </label>
	        </div>
	    </div>
	</div>

	<div class="text-left">
		<button title="Salvar" class="btn btn-success" type="button" id="btnSalvar" onclick="salvaExperiencia()"><span
			class="glyphicon glyphicon-thumbs-up"></span> Salvar
		</button>
		<button title="Salvar" class="btn btn-info" type="button" id="btnSalvar" onclick="novaExperiencia()"><span
			class="glyphicon glyphicon-refresh"></span> Novo
		</button>
	</div>
	
	<div id="divDadosProfissionais">
		<?php 
			echo $experienciaavaliadoreducacional->getFormExperienciaPorAvaliador();
		?>
	</div>
	
</fieldset>
</div>

<div class="well">
<fieldset class="form-horizontal">
<legend>Graduações</legend>

	<div class="form-group">
		<input name="grd" id="grd" type="hidden" value=""/>
		<input name="acao" id="acao" type="hidden" value=""/>
	    <label for="grdid" class="col-lg-4 col-md-4 control-label">Curso:</label>
	
	    <div class="col-lg-8 col-md-8 ">
    		<?php echo $avaliadoreducacional->getGraduacoes(); ?>
	    </div>
	</div>

	<div class="text-left">
		<button title="Salvar" class="btn btn-success" type="button" id="btnSalvar" onclick="salvaGraduacao()"><span
			class="glyphicon glyphicon-thumbs-up"></span> Salvar
		</button>
		<button title="Salvar" class="btn btn-info" type="button" id="btnSalvar" onclick="novaGraduacao()"><span
			class="glyphicon glyphicon-refresh"></span> Novo
		</button>
	</div>
	
	<div id="divDadosGraduacoes">
		<?php 
			echo $avaliadoreducacional->listarGraduacao();
		?>
	</div>
	
</fieldset>
</div>

<div class="well">
<fieldset class="form-horizontal">
<legend>Especialização / Mestrado / Doutorado</legend>

	<div class="form-group">
		<input name="icpid" id="icpid" type="hidden" value=""/>
		<input name="acao" id="acao" type="hidden" value=""/>
	    <label for="grdid" class="col-lg-4 col-md-4 control-label">Área:</label>
	
	    <div class="col-lg-8 col-md-8 ">
    		<?php echo $avaliadoreducacional->getAreaCursos('carregaSupArea(this.value)'); ?>
	    </div>
	</div>
	<div class="form-group">
		<input name="acao" id="acao" type="hidden" value=""/>
	    <label for="grdid" class="col-lg-4 col-md-4 control-label">Sub-Aárea:</label>
	
	    <div class="col-lg-8 col-md-8 " id="divSubArea">
    		<?php echo $avaliadoreducacional->getSubAreaCursos(); ?>
	    </div>
	</div>
	<div class="form-group">
		<input name="acao" id="acao" type="hidden" value=""/>
	    <label for="grdid" class="col-lg-4 col-md-4 control-label">Tipo:</label>
	
	    <div class="col-lg-8 col-md-8 ">
    		<?php echo $avaliadoreducacional->getTipoCursos(); ?>
	    </div>
	</div>

	<div class="text-left">
		<button title="Salvar" class="btn btn-success" type="button" id="btnSalvarCurso" onclick="salvaCurso()"><span
			class="glyphicon glyphicon-thumbs-up"></span> Salvar
		</button>
		<button title="Novo" class="btn btn-info" type="button" id="btnNovoCurso" onclick="novoCurso()"><span
			class="glyphicon glyphicon-refresh"></span> Novo
		</button>
	</div>
		
	<div id="divDadosCursos">
		<?php 
			echo $avaliadoreducacional->listarCurso();
		?>
	</div>
		
</fieldset>
</div>
<?php } ?>

</div>

	<div class="text-right">
		<button title="Salvar" class="btn btn-success" type="button" id="btnSalvar" onclick="enviaFormulario()"><span
			class="glyphicon glyphicon-thumbs-up"></span> Salvar
		</button>
	</div>
	<br/>
</form>
</div>
</div>

</div>
<!-- html / -->

<!-- script js -->
<script>

	/**
	* 	Funções executadas quando a página é carregada
	*/
	$(document).ready(function(){
		$('#avenumdocindicacao').mask('999999999');
		$('.campoCpf').mask("999.999.999-99");
		$('.campoDdd').mask("99");
		$('.campoTel').mask("(99) 9999-99999");
		$('.campoCep').mask("99999-999");
		if ($('.campoTel').val() == '() '){ // Limpa os campos de telefone
			$('.campoTel').val('')
		}
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();

		$('.obrigatorio').append('<span style="color: red;">*</span>');
        
	});

	function novaExperiencia(){
		$('#eaeid').val('');
		$('#acao').val('');
		$('#eaeorgaoquetrabalha').val('');
		$('#eaediretoriacoordenacaosetor').val('');
		$('#eaecargo').val('');
		$('#eaefuncao').val('');
		
		$('#labeleaecargoqueaposentou1').attr('class', 'btn btn-default');
		$('#labeleaecargoqueaposentou2').attr('class', 'btn btn-default');

		$('#labeleaecargoqueaposentou1').removeAttr('checked');
		$('#labeleaecargoqueaposentou2').removeAttr('checked');
	}

	function baixarArquivo(aidid, arqid) {
		$('#arqid').val(arqid);
		jQuery('[name=requisicao]').val('baixar_arquivo');
		jQuery('[name=formCadastroLista]').submit();
	}

	function inativarArquivo(aidid) {
		if (confirm('Deseja inativar este arquivo?')) {
			$('#aidid').val(aidid);
			jQuery('[name=requisicao]').val('inativar_arquivo');
			jQuery('[name=formCadastroLista]').submit();
		}
	}
	
	function salvaArquivo() {
		jQuery('[name=requisicao]').val('salvar_arquivo');
		jQuery('[name=formCadastroLista]').submit();
	}
	
	function salvaExperiencia() {
		if ($('#eaeorgaoquetrabalha').val() == ''){
			alert('O campo \'Órgão\', do grupo \'Experiências Profissionais\', é obrigatório');
			$('#eaeorgaoquetrabalha').focus();
			return false;
		}

		if ($('#eaediretoriacoordenacaosetor').val() == ''){
			alert('O campo \'Diretoria / Coordenação / Setor\', do grupo \'Experiências Profissionais\', é obrigatório');
			$('#eaediretoriacoordenacaosetor').focus();
			return false;
		}

		if ($('#eaecargo').val() == ''){
			alert('O campo \'Cargo\', do grupo \'Experiências Profissionais\', é obrigatório');
			$('#eaecargo').focus();
			return false;
		}

		if ($('#eaefuncao').val() == ''){
			alert('O campo \'Função\', do grupo \'Experiências Profissionais\', é obrigatório');
			$('#eaefuncao').focus();
			return false;
		}
				
		if ($('#eaeid').val() != ''){
			jQuery('[name=requisicao]').val('alterar_experiencia');
		} else {
			jQuery('[name=requisicao]').val('salvar_experiencia');
		}
 		jQuery('[name=formCadastroLista]').submit();
	}

	function salvaGraduacao(){
		if ($('#grdid').val() == ''){
			alert('O campo \'Curso\', do grupo \'Graduações\', é obrigatório');
			$('#grdid').focus();
			return false;
		}
			
		jQuery('[name=requisicao]').val('salvar_graduacao');
		jQuery('[name=formCadastroLista]').submit();
	}

	function novaGraduacao() {
		$('#grdid').val('');
	}

	function apagaGraduacao(id){
		if (confirm('Deseja apagar este item de graduação?')) {
			$('#grd').val(id);
			jQuery('[name=requisicao]').val('apagar_graduacao');
			jQuery('[name=formCadastroLista]').submit();
		}
	}

	function salvaCurso() {
		if ($('#sbcid').val() == ''){
			alert('O campo \'Sub-Área\', do grupo \'Especialização / Mestrado / Doutorado\', é obrigatório');
			$('#sbcid').focus();
			return false;
		}

		if ($('#icptipo').val() == ''){
			alert('O campo \'Tipo\', do grupo \'Especialização / Mestrado / Doutorado\', é obrigatório');
			$('#icptipo').focus();
			return false;
		}
				
		jQuery('[name=requisicao]').val('salvar_curso');
		jQuery('[name=formCadastroLista]').submit();
	}
	
	function novoCurso() {
		$('#icpid').val('');
		$('#arcid').val('');
		$('#sbcid').val('');
		$('#icptipo').val('');
	}
	
	function apagaCurso(id) {
		if (confirm('Deseja apagar este item de curso?')) {
			$('#icpid').val(id);
			jQuery('[name=requisicao]').val('apagar_curso');
			jQuery('[name=formCadastroLista]').submit();
		}
	}
	
	/**
	*	Clona o formulário de dados profissionais
	*/	
	function clonaDadosProfissionais(){
		var con = $("#hidCont").val();
		con++;
		$("#divDados").clone().children().each(function() {//acao1
			$(this).find('#divSeparador').removeAttr("style");
			$(this).find('#eaeid1').attr("id", "eaeid" + con).attr("name", "eaeid" + con).removeAttr("value");
			$(this).find('#acao1').attr("id", "acao" + con).attr("name", "acao" + con).removeAttr("value");
			$(this).find('#eaeorgaoquetrabalha1').attr("id", "eaeorgaoquetrabalha" + con).attr("name", "eaeorgaoquetrabalha" + con).removeAttr("value");
			$(this).find('#eaediretoriacoordenacaosetor1').attr("id", "eaediretoriacoordenacaosetor" + con).attr("name", "eaediretoriacoordenacaosetor" + con).removeAttr("value");
			$(this).find('#eaecargo1').attr("id", "eaecargo" + con).attr("name", "eaecargo" + con).removeAttr("value");
			$(this).find('#eaefuncao1').attr("id", "eaefuncao" + con).attr("name", "eaefuncao" + con).removeAttr("value");
			$(this).find('#eaecargoqueaposentou1').attr("id", "eaecargoqueaposentou" + con).attr("name", "eaecargoqueaposentou" + con).removeAttr("value");
		}).appendTo('#divDadosProfissionais');
		$("#hidCont").val(con);
	}
	
	/**
	 * Envia Formulário
	 */
	 function enviaFormulario(){

		// DADOS PESSOAIS
			if ($('#avenome').val() == ''){
				alert('O campo \'Nome\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avenome').focus();
				return false;
			}

			if ($('#avedatanascimento').val() == ''){
				alert('O campo \'Data de nascimento\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avedatanascimento').focus();
				return false;
			}
						
			if ($('.avesexo:checked').length == 0){
				alert('O campo \'Sexo\', do grupo \'Dados Pessoais\', é obrigatório');
				return false;
			}
			
			if ($('#avenumcpf').val() == ''){
				alert('O campo \'CPF\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avenumcpf').focus();
				return false;
			}

			if ($('#aveidentidade').val() == ''){
				alert('O campo \'RG\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#aveidentidade').focus();
				return false;
			}

			if ($('#aveorgaoexpedidor').val() == ''){
				alert('O campo \'Órgão Expedidor\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#aveorgaoexpedidor').focus();
				return false;
			}

			if ($('#avedataexpedicao').val() == ''){
				alert('O campo \'Data de expedição\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avedataexpedicao').focus();
				return false;
			}
			
			if ($('#aveendereco').val() == ''){
				alert('O campo \'Endereço\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#aveendereco').focus();
				return false;
			}

			if ($('#muncod').val() == ''){
				alert('O campo \'Cidade\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#muncod').focus();
				return false;
			}

			if ($('#avebairro').val() == ''){
				alert('O campo \'Bairro\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avebairro').focus();
				return false;
			}
			
			if ($('#avecep').val() == ''){
				alert('O campo \'CEP\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avecep').focus();
				return false;
			}

			if ($('#avetelcomercial').val() == ''){
				alert('O campo \'Tel. Comercial\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avetelcomercial').focus();
				return false;
			}

			if ($('#avetelresidencial').val() == ''){
				alert('O campo \'Tel. Residencial\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avetelresidencial').focus();
				return false;
			}

			if ($('#avetelcelular').val() == ''){
				alert('O campo \'Tel. Celular\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#avetelcelular').focus();
				return false;
			}

			if ($('#aveemail').val() == '' || $('#aveemail').val() == ' '){
				alert('O campo \'E-Mail Pessoal\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#aveemail').focus();
				return false;
			} else {
				if ($('#aveemail').val().indexOf("@") == -1){
					alert('E-Mail inválido');
					$('#aveemail').focus();
					return false;
				}
			}

            // Validação comentada a pedido do usuário
			/*if ($('#aveskype').val() == '' || $('#aveskype').val() == ' '){
				alert('O campo \'Skype\', do grupo \'Dados Pessoais\', é obrigatório');
				$('#aveskype').focus();
				return false;
			}*/

			if (!$('#avedeficiencia1').is(':checked') && !$('#avedeficiencia2').is(':checked')){
				alert('O campo \'Deficiência Física\', do grupo \'Dados Pessoais\', é obrigatório');
				return false;
			}
						
		// Indicação
			if ($('#orvid').val() == ''){
				alert('O campo \'Órgão\', do grupo \'Indicação\', é obrigatório');
				$('#orvid').focus();
				return false;
			}

			if ($('#avenumdocindicacao').val() == ''){
				alert('O campo \'N° documento\', do grupo \'Indicação\', é obrigatório');
				$('#avenumdocindicacao').focus();
				return false;
			}

			if ($('#avedataindicacao').val() == ''){
				alert('O campo \'Data indicação\', do grupo \'Indicação\', é obrigatório');
				$('#avedataindicacao').focus();
				return false;
			}
						
		// Avaliador Educacional
			if ($('#taeid').val() == ''){
				alert('O campo \'Perfil\', do grupo \'<?php echo empty($_GET['aveid']) ? 'Novo ' : '' ?>Avaliador Educacional\', é obrigatório');
				$('#taeid').focus();
				return false;
			}

			if ($('#estuf').val() == ''){
				alert('O campo \'UF\', do grupo \'<?php echo empty($_GET['aveid']) ? 'Novo ' : '' ?>Avaliador Educacional\', é obrigatório');
				$('#estuf').focus();
				return false;
			}

			if ($('#lotid').val() == ''){
				alert('O campo \'Unidade de Atuação\', do grupo \'<?php echo empty($_GET['aveid']) ? 'Novo ' : '' ?>Avaliador Educacional\', é obrigatório');
				$('#lotid').focus();
				return false;
			}

		// Dados Bancários
			if ($('#avebanco').val() == ''){
				alert('O campo \'Banco\', do grupo \'Dados Bancários\', é obrigatório');
				$('#avebanco').focus();
				return false;
			}

			if ($('#aveagencia').val() == ''){
				alert('O campo \'Agência\', do grupo \'Dados Bancários\', é obrigatório');
				$('#aveagencia').focus();
				return false;
			}

			if ($('#aveconta').val() == ''){
				alert('O campo \'CC\', do grupo \'Dados Bancários\', é obrigatório');
				$('#aveconta').focus();
				return false;
			}
			
						
		if ($('#aveid').val() != ''){
			jQuery('[name=requisicao]').val('alterar');
		} else {
			jQuery('[name=requisicao]').val('salvar');
		}
 		jQuery('[name=formCadastroLista]').submit();
	 }


</script>
<!-- script js / -->