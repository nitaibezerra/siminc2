<?php
require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/listadadosqualitativo');
?>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">SIOP (SOF)</li>
            <li class="active">Relatório</li>
            <li class="active">Listar dados do qualitativo</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <fieldset>
            <form id="frmVinculacao" name="frmPesquisa" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
                <div class="form-group">
                    <label for="nomeTabela" class="col-lg-2 control-label">
                        Tabela de qualitativo:
                    </label>
                    <div class="col-lg-10">
                    <?php
                        $nomesTabelas = array();
                        $nomesTabelas[] = array('codigo' => 'ws_acoesdto', 'descricao' => 'Ações (ws_acoesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_agendasamdto', 'descricao' => 'Agendamentos (ws_agendasamdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_basesgeograficasdto', 'descricao' => 'Bases geograficas (ws_basesgeograficasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_esferasdto', 'descricao' => 'Esferas (ws_esferasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_funcoesdto', 'descricao' => 'Funções (ws_funcoesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_indicadoresdto', 'descricao' => 'Indicadores (ws_indicadoresdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_iniciativasdto', 'descricao' => 'Iniciativas (ws_iniciativasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_localizadoresdto', 'descricao' => 'Localizadores (ws_localizadoresdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_macrodesafiosdto', 'descricao' => 'Macro desafios (ws_macrodesafiosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_medidasinstitucionaisnormativasdto', 'descricao' => 'Medidas institucionais normativas (ws_medidasinstitucionaisnormativasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_metasdto', 'descricao' => 'Metas (ws_metasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_momentosdto', 'descricao' => 'Momentos (ws_momentosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_objetivosdto', 'descricao' => 'Objetivos (ws_objetivosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_orgaosdto', 'descricao' => 'Órgãos (ws_orgaosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_perfisdto', 'descricao' => 'Perfis (ws_perfisdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_periodicidadesdto', 'descricao' => 'Periodicidades (ws_periodicidadesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_planosorcamentariosdto', 'descricao' => 'Planos orçamentarios (ws_planosorcamentariosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_produtosdto', 'descricao' => 'Produtos (ws_produtosdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_programasdto', 'descricao' => 'Programas (ws_programasdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_regioesdto', 'descricao' => 'Regiões (ws_regioesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_regionalizacoesdto', 'descricao' => 'Regionalizações (ws_regionalizacoesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_subfuncoesdto', 'descricao' => 'Subfunções (ws_subfuncoesdto)');
                        $nomesTabelas[] = array('codigo' => 'ws_tiposacaodto', 'descricao' => 'Tipos ação (ws_tiposacaodto)');
                        $nomesTabelas[] = array('codigo' => 'ws_tiposinclusaodto', 'descricao' => 'Tipos inclusão (ws_tiposinclusaodto)');
                        $nomesTabelas[] = array('codigo' => 'ws_tiposprogramadto', 'descricao' => 'Tipos programas (ws_tiposprogramadto)');
                        $nomesTabelas[] = array('codigo' => 'ws_unidadesmedidadto', 'descricao' => 'Unidades medida (ws_unidadesmedidadto)');
                        $nomesTabelas[] = array('codigo' => 'ws_unidadesmedidaindicadordto', 'descricao' => 'Unidades medida indicador (ws_unidadesmedidaindicadordto)');
                        $nomesTabelas[] = array('codigo' => 'ws_unidadesorcamentariasdto', 'descricao' => 'Unidades orçamentarias (ws_unidadesorcamentariasdto)');

                        $options = array(
                            'titulo' => 'Selecione ao menos uma tabela'
                        );
                        
                        sort($nomesTabelas);
                        
                        inputCombo('nomeTabela', $nomesTabelas, $_REQUEST['nometabela'], 'cols-qualit', $options);
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnListar" type="submit"><i class="glyphicon glyphicon-search"></i> Exibir dados da tabela</button>
                    </div>
                </div>
            </form>
           
        </fieldset>
    </div>
	<?php
		if (isset($_REQUEST['nometabela']) && $_REQUEST['nometabela'] != '')
		{
	    	$sql = "SELECT *, 
					to_char(dataultimaatualizacao,'DD/MM/YYYY HH24:MI:SS') as dataultimaatualizacao 
					FROM wssof.{$_REQUEST['nometabela']} ORDER BY 1";
            
			$listagem = new Simec_Listagem();
	        $listagem->setQuery($sql);
	        $listagem->turnOnPesquisator();
	        $listagem->setCabecalho('auto');
	        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
		}
	?>
</div>