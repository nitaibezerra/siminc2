<?php
require_once APPRAIZ . "www/spo/_funcoes.php";
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . "spo/ws/sof/Quantitativo.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/quantitativo/consultarProposta');

// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao')) 
{
    switch ($_REQUEST['requisicao']) 
    {
        case 'carregarAcoes':
            print carregarAcoes($_SESSION['exercicio'], $_POST['dados']['unidadeOrcamentaria'], true);
            die;
            break;
        case 'carregarLocalizador':
            print carregarLocalizadores($_SESSION['exercicio'], $_POST['dados']['acoes'], true);
            die;
            break;
        case 'consultarProposta':
          	$proposta = consultarProposta($_POST, $fm);
          	break;            
    }
}

require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
?>
<script type="text/javascript">
	/**
	 * Carrega as opções do combo de tipo de credito, com base no momento escolhido.
	 */
	function carregarAcoes()
	{
	    // -- requisicao de dados do tipo de crédito, por momento
	    $.post(window.location, {'requisicao':'carregarAcoes', 'dados[unidadeOrcamentaria]':$('#unidadeOrcamentaria').val()}, function(data) {
            var _data = JSON.parse(data);

            // -- limpando o select
            $('#acoes').find('option').remove().end().append($('<option>', {value: '', text:'Selecione uma ação orçamentaria'}));
            if (_data) {
                $.each(_data, function(index, value){
                    $('#acoes').append($('<option>', {value:value.codigo, text:value.descricao}));
                });
            }
            $('#acoes.chosen-select').trigger('chosen:updated');
        }, 'text');
	};

	/**
	 * Carrega as opções do combo de tipo de credito, com base no momento escolhido.
	 */
	function carregarLocalizador()
	{
	    // -- requisicao de dados do tipo de crédito, por momento
	    $.post(window.location, {'requisicao':'carregarLocalizador', 'dados[acoes]':$('#acoes').val()}, function(data) {
            var _data = JSON.parse(data);

            // -- limpando o select
            $('#localizador').find('option').remove().end().append($('<option>', {value: '', text:'Selecione um localizador'}));
            if (_data) {
                $.each(_data, function(index, value){
                    $('#localizador').append($('<option>', {value:value.codigo, text:value.descricao}));
                });
            }
            $('#localizador.chosen-select').trigger('chosen:updated');
        }, 'text');
	};
</script>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>Integração</li>
            <li>Sof Qualitativo</li>
            <li class="active">Consultar Proposta</li>
        </ol>
    </div>
    <div class="well">
    	<fieldset>
            <form id="frmConsultar" name="frmConsultar" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            	<input type="hidden" name="requisicao" value="consultarProposta" />
				<div class="form-group">
					<label for="momento" class="col-lg-2 control-label">
						Unidade Orçamentaria
					</label>
					<div class="col-lg-10">
					<?php
				        $sql = " SELECT uni.codigoorgao AS codigo,
				                		uni.codigoorgao || ' - ' || descricao AS descricao 
				           		   FROM	wssof.ws_unidadesorcamentariasdto uni
				            	  WHERE uni.snativo = '1'
				                    AND uni.exercicio = '%d'
				               ORDER BY uni.descricao";

			        	$sql = sprintf($sql, '2013');
			        	//$sql = sprintf($sql, $_SESSION['exercicio']);                
			        
						$options = array(
							'titulo' => 'Selecione ao menos uma unidade orçamentaria',
							'acao' => 'carregarAcoes',
							'obrig' => 'S'
						);
						
						inputCombo('unidadeOrcamentaria', $sql, $_REQUEST['unidadeOrcamentaria'], 'unidadeOrcamentaria', $options);
					?>
					</div>
				</div>

				<div class="form-group">
					<label for="acoes" class="col-lg-2 control-label">
						Ações
					</label>
					<div class="col-lg-10">
					<?php
						$options = array(
							'titulo' => 'Selecione ao menos uma ação',
							'acao' => 'carregarLocalizador',
							'obrig' => 'S'
						);
						
						inputCombo('acoes', array(), $_REQUEST['acoes'], 'acoes', $options);
					?>
					</div>
				</div>

		        <div class="form-group">
					<label for="localizador" class="col-lg-2 control-label">
						Localizador
					</label>
					<div class="col-lg-10">
					<?php
						$options = array(
							'titulo' => 'Selecione ao menos um localizador',
							'obrig' => 'S'
						);
						
						inputCombo('localizador', array(), $_REQUEST['localizador'], 'localizador', $options);
					?>
					</div>
				</div>  
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                    	<input type="hidden" name="importar" value="1" />
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnImportar" type="submit"><i class="glyphicon glyphicon-search"></i> Consultar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
    <?php echo $fm->getMensagens(); ?>
	<div class="well">
		<fieldset>
			<form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal" enctype="multipart/form-data">
			    <div class="form-group">
			        <label class="col-lg-2 control-label">Ação:</label>
			        <div class="col-lg-10">
			        	<?php echo $proposta->acao->identificadorunico .  ' - ' . $proposta->acao->titulo; ?>
			        </div>
			    </div>
	           	<div class="form-group">
					<label class="col-lg-2 control-label">Base Legal:</label>
					<div class="col-lg-10">
						<?php echo $proposta->acao->baselegal; ?>
					</div>
				</div>			    
				<div class="form-group">
			        <label class="col-lg-2 control-label">Descrição:</label>
			        <div class="col-lg-10">
	                    <textarea class="form-control" rows="3" maxlength="500" ><?php echo $proposta->acao->descricao; ?></textarea>
	    			</div>
			    </div>
			    <div class="form-group">
			        <label class="col-lg-2 control-label">Localizador:</label>
			        <div class="col-lg-10">
	                    <?php echo $proposta->localizador->descricao; ?>
	    			</div>
			    </div>
			    <div class="form-group">
			        <label class="col-lg-2 control-label">Justificativa:</label>
			        <div class="col-lg-10">
	                    <textarea class="form-control" rows="3" maxlength="500" ><?php echo $proposta->justificativa; ?></textarea>
	    			</div>
			    </div>
			    <div class="form-group">
			        <label class="col-lg-2 control-label">Exercicio:</label>
			        <div class="col-lg-10">
			        	<?php echo $proposta->acao->exercicio; ?>
	    			</div>
			    </div>   
			</form>
		</fieldset>
	</div>
</div>