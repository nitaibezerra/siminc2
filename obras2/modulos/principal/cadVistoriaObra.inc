<?php
// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

$empid = $_SESSION['obras2']['empid'];

$somenteLeitura = 'S';
$habilitado     = true;

if ( $_GET['acao'] == 'A' ){
	$emsid = '';
	$_SESSION['obras2']['emsid'] = $emsid;
}else{
	$emsid = ($_REQUEST['emsid'] ? $_REQUEST['emsid'] : $_SESSION['obras2']['emsid']);
	$_SESSION['obras2']['emsid'] = $emsid;
	
	if ( empty($emsid) ){
		die("<script>
				alert('Faltam parametros para acessar a tela!'); 
				window.location = '?modulo=principal/listaVistoriaObra&acao=A';
			 </script>");
	}
	
	$empreendimentoSupervisao = new EmpreendimentoSupervisao( $emsid );
	$ultEmsid 				  = $empreendimentoSupervisao->pegaUltEmsidByEmpreendimento( $empid );
	
	extract( $empreendimentoSupervisao->getDados() );
	
	if ( $ultEmsid != $emsid && $emsid ){
		$somenteLeitura = 'N';
		$habilitado     = false;
	}
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER) ) ){
	$somenteLeitura = 'N';
	$habilitado     = false;
}

// Executa as funções da tela de acordo com suas ações
if ($_REQUEST["requisicao"] == 'salvar'){
	$empreendimentoSupervisao = new EmpreendimentoSupervisao( $emsid );
	$arDado = array('usucpf' 		 => $_SESSION['usucpf'],
					'empid'  		 => $empid,
					'entid'  		 => $_POST['entidvistoriador'],
					'staid'  		 => $_POST['staid'],
					'emsdtvistoria'  => formata_data_sql( $_POST['emsdtvistoria'] ),
					'emsdtconclusao' => formata_data_sql( $_POST['emsdtconclusao'] ));
	$emsid = $empreendimentoSupervisao->popularDadosObjeto( $arDado )
							 		  ->salvar();
	$_SESSION['obras2']['emsid'] = $emsid;
	
    $supervisao = new Supervisao();
    $percentExec = 0;
    foreach ( $_POST['obrid'] as $obrid ){
    	$numObr++;
    	if ( !empty( $_POST['supid'][$obrid] ) ){
    		$supervisao->carregarPorId( $_POST['supid'][$obrid] );	
    	}
    	$supervisao->emsid = $emsid;
    	$supervisao->obrid = $obrid;
    	$supid = $supervisao->salvar();
    	$supervisao->clearDados();
    	
    	$percentExec += $supervisao->percentExec;
    }
    
    $empreendimento = new Empreendimento( $empid );
	$empreendimento->empdtultvistoria = date('Y-m-d');
    
    $ultEmsid = $empreendimentoSupervisao->pegaUltEmsidByEmpreendimento( $empid );
    if ( $emsid == $ultEmsid ){
		$empreendimento->emppercentultvistoria = ($percentExec > 0 ? $percentExec / $numObr : 0);
    }
	$empreendimento->salvar();
	
    // QUESTIONÁRIO
    if ( is_array( $_POST['qstid'] ) ){
    	$questaoSupervisao = new QuestaoSupervisao();
    	foreach ( $_POST['qstid'] as $qstid => $resp ){
//    		$questaoSupervisao->apagaRespostaPorQuestao( $qstid );
			if ( $_POST["qtsid_{$qstid}"] ){
				$questaoSupervisao->carregarPorId( $_POST["qtsid_{$qstid}"] );
			}
    		$arDados = array(
    						'qstid' 	  => $qstid,
    						'emsid'		  => $emsid, 
    						'qtsresposta' => $resp,
    						'qtsobs'	  => $_POST['qtsobs'][$qstid]
    						 );
    		$qtsid = $questaoSupervisao->popularDadosObjeto( $arDados )
    						  		   ->salvar();
    		$questaoSupervisao->clearDados();				  

        	$fotogps = new FotoGps();
    		$totImg  = $_POST["totImg_{$qstid}"];
    		for ( $i=0; $i<=$totImg; $i++ ){
		    	$arquivo = $_FILES["img_{$qstid}_{$i}"];	
		    	
			    if ( $_FILES["img_{$qstid}_{$i}"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
					include_once APPRAIZ."includes/classes/fileSimec.class.inc";
					
					$campos	= array(
									"qtsid" => $qtsid,
									);	
					$file = new FilesSimec("arquivoquestaosupervisao", $campos ,"obras2");
			        $file->setPasta('obras2');
			        $file->setUpload(null, "img_{$qstid}_{$i}", true);
			        $arqid = $file->getIdArquivo();
			        
			    	$extension = explode(".", $arquivo['name']);
					$extension = "." . end( $extension );
					$validExtension = (($extension == ".jpg" || $extension == ".jpeg" || $extension == ".gif" || $extension == ".png"));
					
					if ( $validExtension ){
				        $pathImg = $file->getArquivo($arqid);
				        if ( $pathImg ){
				        	$dadoGps = $fotogps->extractInformacaoGps( $arqid, $pathImg );
				        	if ( $dadoGps != false ){
				        		$arDados = array('arqid' => $arqid,
				        						 'fgpzoom'  	=> 15,
				        						 'fgplatitude'  => $dadoGps['lat'],
				        						 'fgplongitude' => $dadoGps['long']);
				        		$fotogps->popularDadosObjeto( $arDados )
				        				->salvar();
				        				
				        		$fotogps->clearDados();		
				        	}
				        }
					}
			    }
    		}
    	}
    }
    
    $db->commit();
	die("<script>
			alert('Operação realizada com sucesso!'); 
			window.location = '?modulo=principal/cadVistoriaObra&acao=E'
		 </script>");
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if( $_SESSION['obras2']['emsid'] ){
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMP_EDICAO,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_CADASTRO_VISTORIA_EMP,$url,$parametros);
}

$empreendimento = new Empreendimento( $empid );
$empreendimento->montaCabecalho();

monta_titulo( 'Cadastro de Vistoria', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>
<!--
.divItemComposicao{
	width:100%;
	font-weight:bold;
}

.divDivisao{
	width:100%;
	margin-left:5px;
	font-weight:bold;
	padding:5px;
}

.divQuestao{
	width:100%;
	margin-left:10px;
	padding:5px;
}
.divQuestaoNivel1{
	width:100%;
}
.divQuestaoNivel2{
	width:100%;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel4{
	margin-top:3px;
}
.divQuestaoNivel4_none{
	margin-top:3px;
	display:none;
}
.divQuestaoNivel4_fieldset{
	width:98%
}
.divQuestaoNivel4_1{
	margin-top:3px;
}
.divQuestaoNivel4_2{
	margin-top:3px;
}
.divQuestaoNivel4_3{
	margin-top:3px;
	padding:2px 2px 2px 2px;
	border: 1px dashed #ccc;
	height: 70px;
}
.divQuestaoNivel3{
	margin-top:3px;
}
.divQuestaoNivel3_none{
	display:none;
	margin-top:3px;
}
.divQuestaoNivel3_1{
	font-weight:bold;
}
</style>

<form id="formulario_supervisao" name="formulario_supervisao" method="post" enctype="multipart/form-data" action="">

<input type="hidden" name="emsid"               id="emsid"         value="<?php echo $emsid ?>" />
<input type="hidden" name="requisicao"          id="requisicao"    value="salvar" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="3">Dados da Vistoria</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data da Vistoria</td>
            <td>
                <?php
//                $emsdtvistoria = $vistoriaEstrutura['emsdtvistoria'];
                echo campo_data2( 'emsdtvistoria', 'S', $somenteLeitura, '', 'S','','validaDataVistoria();' );
                ?>
            </td>
            <td rowspan="8" align="right" valign="top" width="1">
            <?php
            // Barra de estado WORKFLOW
//                wf_desenhaBarraNavegacao($docid, array('obrid' =>  $obrid, 'supid' => $_SESSION['obras2']["supid"]));
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data Prevista de Conclusão</td>
            <td>
                <?php
//                $emsdtconclusao = $vistoriaEstrutura['emsdtconclusao'];
                echo campo_data2( 'emsdtconclusao', 'S', $somenteLeitura, '', 'S','','' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome do Responsável</td>
            <td>
                <?php
//				$vistoriador 		= new Entidade($vistoriaEstrutura['supvistoriador']);
				$vistoriador 		= new Entidade( $entid );
                $entnomevistoriador = $vistoriador->entnome;
                $entidvistoriador   = $vistoriador->getPrimaryKey();
                ?>
                <span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
                <input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<? echo $entidvistoriador; ?>">
                <?php if($somenteLeitura == 'S' && $entnomevistoriador == ''){?>
                <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
                <?php }?>
                <?php print obrigatorio(); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação</td>
            <td>
                <?php
//                    $staid 		  = $vistoriaEstrutura['staid'];
                    $sitAtividade = new SituacaoAtividade();
                    $sql   		  = $sitAtividade->situacaoPossivelSql( $situacao_obra );
                    $db->monta_combo("staid", $sql, $somenteLeitura, "Selecione...", "validasituacao(this.value,'1".possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORUNIDADE))."');verificasituacao", '', '', '', 'S', 'staid');
                ?>
                <span id="msg_paralisacao" style="display:none; color:#ee0000;">Favor preencher a aba <b>Restrições e Providências</b> com as informações sobre a paralisação.</span>
            </td>
        </tr>
        <tr id="tr8" >
            <td colspan="3">Detalhamento de Supervisão e Acompanhamento</td>
        </tr>
        <?php 
        
		$param					= array();
		$param['not(obridpai)'] = true;
        $obras   				= new Obras();
        $arObrid 				= $obras->pegaIdObraPorEmpid( $empid, $param );
        
        foreach ( $arObrid as $obrid ):
        	$obra = new Obras( $obrid );
        	
        	$param			= array();
			$param['empid'] = $empid;
			$supervisao 	= new Supervisao();
			$supid			= $supervisao->pegaSupidByObraAndEmsid( $obrid, $emsid );
			$ultimoSupid 	= $supervisao->pegaUltSupidByObra( $obrid, $param );
			
			unset($total);
        ?>
        <tr id="tr9" >
            <td colspan="3">
                <table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_supervisao">
                    <thead>
                        <tr style="background-color: #CDCDCD;">
                            <td colspan="15" valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                            	<input type="hidden" name="obrid[]" id="obrid_<?php echo $obrid ?>" value="<?php echo $obrid ?>">
                            	<input type="hidden" name="supid[<?php echo $obrid ?>]" id="supid_<?php echo $obrid ?>" value="<?php echo $supid ?>">
                            	<b><?php echo "($obrid) {$obra->obrnome}" ?></b>
                            </td>
                        </tr>
                        <tr>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Descrição</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre o Objeto</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Unidade de Medida</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
                            <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
                            <td colspan="4" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
                            <td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Diferença para a Supervisão anterior</b></td>
                        </tr>
                        <tr>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado<br></b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre o Objeto após Supervisão</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Quantidade Executada<br></b></td>

                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor Executado</b></td>
                            <td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Executado<br></b></td>
                        </tr>
                    </thead>

<?php
                    $supervisaoItem = new SupervisaoItem();

                    $total['vlrProjeto']           = 0;
                    $total['vlrObra']              = 0;
                    $total['vlrExecSobreObra']     = 0;
                    $total['vlrExecSobreObraUlt']  = 0;
                    $total['percExecSobreObraUlt'] = 0;

					// Busca filhos da Edificação
					$dadoEtapa = $supervisaoItem->getItensByEtapa($obrid, ($supid ? $supid : $ultimoSupid));
					$habil = (count($dadoEtapa) ? 'N' : 'S');
                    
					foreach ($dadoEtapa as $etapa){
						
                        $total['vlrProjeto'] = $etapa['ocrvalorexecucao'];

                        $total['vlrObra']  += $etapa['icovlritem'];
                        $total['percObra']  = ($total['vlrObra'] / $etapa['ocrvalorexecucao']) * 100;

                        $total['vlrExecSobreObra']  += $etapa['spivlrfinanceiroinfsupervisor'];
                        $total['percExecSobreObra']  = ($total['vlrExecSobreObra'] / $etapa['ocrvalorexecucao']) * 100;

                        $total['vlrExecSobreObraUlt']  += $etapa['spivlrfinanceiroanterior'];
                        $total['percExecSobreObraUlt']  = ($total['vlrExecSobreObraUlt'] / $etapa['ocrvalorexecucao']) * 100;
//                        $total['percExecSobreObraUlt'] += $edificacao['supvlritemsobreobraexecanterior'];
						
                        
						$etapa['icodtinicioitem'] = formata_data( $etapa['icodtinicioitem'] );
					    $etapa['icodterminoitem'] = formata_data( $etapa['icodterminoitem'] );
					
					    $etapa['spipercsobreobrainfsupervisor'] = ($etapa['spivlrfinanceiroinfsupervisor'] / $etapa['ocrvalorexecucao']) * 100;
					    
					    // Busca Filhos da Etapa
					    $dadoDetalhe = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai IS NULL"));
					    $habil = (count($dadoDetalhe) ? 'N' : 'S');
?>
                            <tbody>
                                <tr bgcolor="#FDF8E7" id="<?="tr_item_etapa_" . $obrid . "_" . $etapa['icoid']?>">
                                    <td align="left">
                                    <input type="hidden" name="icoid[<?php echo $obrid?>][]" value="<?=$etapa['icoid']?>">
                                    <input type="hidden" name="ico_spiid[<?php echo $obrid?>][]" value="<?=$etapa['spiid'] ?>">
                                    &nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$etapa['itcdesc'] ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <?=number_format($etapa['icovlritem'], 2, ',', '.') ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <?=number_format($etapa['icopercsobreobra'], 2, ',', '.') ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    -
                                    </td>
                                    <td align="center">
                                    -
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodtinicioitem']  ?>
                                    </td>
                                    <td align="center">
                                    <?=$etapa['icodterminoitem']  ?>
                                    </td>
                                    <td align="center">
                                    <input type="hidden" name="ico_spivlrfinanceiroanterior[]" value="<?=number_format($etapa['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                    <?php echo campo_texto( 'ico_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemexecanterior'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($etapa['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlrinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .' )', number_format($etapa['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                    </td>
                                    <td align="center">
                                    <?php echo campo_texto( 'ico_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerEtapa( this, '. $obrid .', \'numerico\' )', number_format($etapa['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    <input type="hidden" name="ico_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                    <span>
                                    <?=number_format($etapa['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                    </span>
                                    </td>
                                    <td align="right" style="color:#336EFF">
                                    &nbsp;
                                    </td>
	                                <td align="right" style="color:#336EFF">
	                                	<input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="ico_spidiferencavlr[]">
										<span> <?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $etapa['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
	                                </td>
	                                <td align="right" style="color:#336EFF">
	                                	<input type="hidden" value="<?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?>" name="ico_spidiferencaporcentagem[]">
										<span> <?php echo number_format($etapa['spivlrinfsupervisor'] - $etapa['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
	                                </td>
                                </tr>
                            </tbody>
<?php
                            foreach ($dadoDetalhe as $detalhe){
                                $ditid = $detalhe['ditid'];
                                $detalhe['ditdatainicio']  = formata_data( $detalhe['ditdatainicio'] );
                                $detalhe['ditdatatermino'] = formata_data( $detalhe['ditdatatermino'] );
                                $detalhe['spipercsobreobrainfsupervisor'] = ($detalhe['spivlrfinanceiroinfsupervisor'] / $detalhe['ocrvalorexecucao']) * 100;

                                //adicionando os filhos
                                $dadoDetalheFilho = $supervisaoItem->getItensByDetalhamento($obrid, $etapa['icoid'], ($supid ? $supid : $ultimoSupid), array("di.ditidpai ={$ditid}"));
                                $habil = (count($dadoDetalheFilho) ? 'N' : 'S');
?>
                                <tbody>
                                    <tr bgcolor="#DBF6D5" id="<?="tr_item_detalhamento_" . $obrid . "_" . $detalhe['icoid'] . "_" . $detalhe['ditid']?>">
                                        <td align="left">
                                        <input type="hidden" name="ditid[<?php echo $obrid?>][]" value="<?=$detalhe['ditid']?>">
                                        <input type="hidden" name="dit_spiid[<?php echo $obrid?>][]" value="<?=$detalhe['spiid'] ?>">
                                        &nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalhe['ditdsc'] ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=number_format($detalhe['ditvalor'], 2, ',', '.') ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=number_format($detalhe['ditpercsobreobra'], 2, ',', '.') ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=($detalhe['ditmetafisica'] ? number_format($detalhe['ditmetafisica'], 2, ',', '.') : '-') ?>
                                        </td>
                                        <td align="center">
                                        <?=($detalhe['umcdsc'] ? $detalhe['umcdsc'] : '-') ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatainicio']  ?>
                                        </td>
                                        <td align="center">
                                        <?=$detalhe['ditdatatermino']  ?>
                                        </td>
                                        <td align="center">
                                        <input type="hidden" name="dit_spivlrfinanceiroanterior[]" value="<?=number_format($detalhe['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                        <?php echo campo_texto( 'dit_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemexecanterior'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalhe['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlrinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, '. $obrid .', '. $detalhe['icoid'] .' )', number_format($detalhe['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                        </td>
                                        <td align="center">
                                        <?php echo campo_texto( 'dit_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamento( this, '. $obrid .', '. $detalhe['icoid'] .', \'numerico\' )', number_format($detalhe['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <input type="hidden" name="dit_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                        <span>
                                        <?=number_format($detalhe['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                        </span>
                                        </td>
                                        <td align="right" style="color:#336EFF">
                                        <?=($detalhe['umcdsc'] ?
                                                        campo_texto( 'dit_spivlrmetafisicasupervisor[' . $detalhe['ditid'] . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalhe['ditmetafisica'] . ' )', number_format($detalhe['spivlrmetafisicasupervisor'], 2, ',', '.'))
                                                               :
                                                        '&nbsp;') ?>
                                        </td>

                                    <td align="right" style="color:#336EFF">
	                                	<input type="hidden" value="<?php echo number_format($etapa['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dit_spidiferencavlr[]">
										<span> <?php echo number_format($detalhe['spivlrfinanceiroinfsupervisor'] - $detalhe['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
	                                </td>
	                                <td align="right" style="color:#336EFF">
	                                	<input type="hidden" value="<?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dit_spidiferencaporcentagem[]">
										<span> <?php echo number_format($detalhe['spivlrinfsupervisor'] - $detalhe['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
	                                </td>
                                    </tr>
                                </tbody>
<?php
                                foreach ($dadoDetalheFilho as $detalheFilho){
                                    $detalheFilho['ditdatainicio']  = formata_data( $detalheFilho['ditdatainicio'] );
                                    $detalheFilho['ditdatatermino'] = formata_data( $detalheFilho['ditdatatermino'] );

                                    $detalheFilho['spipercsobreobrainfsupervisor'] = ($detalheFilho['spivlrfinanceiroinfsupervisor'] / $detalheFilho['ocrvalorexecucao']) * 100;
?>
                                    <tbody>
                                        <tr bgcolor="#D7EAF2" id="<?="tr_item_detalhamento_filho_" . $obrid . "_" . $detalheFilho['icoid'] . "_" . $detalheFilho['ditidpai'] . "_" . $detalheFilho['ditid']?>">
                                            <td align="left">
                                            <input type="hidden" name="ditid_filho[<?php echo $obrid?>][]" value="<?=$detalheFilho['ditid']?>">
                                            <input type="hidden" name="dtf_spiid[<?php echo $obrid?>][]" value="<?=$detalheFilho['spiid'] ?>">
                                            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/imagens/seta_filho.gif" border="0"><?=$detalheFilho['ditdsc'] ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=number_format($detalheFilho['ditvalor'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=number_format($detalheFilho['ditpercsobreobra'], 2, ',', '.') ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=($detalheFilho['ditmetafisica'] ? number_format($detalheFilho['ditmetafisica'], 2, ',', '.') : '-') ?>
                                            </td>
                                            <td align="center">
                                            <?=($detalheFilho['umcdsc'] ? $detalheFilho['umcdsc'] : '-') ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatainicio']  ?>
                                            </td>
                                            <td align="center">
                                            <?=$detalheFilho['ditdatatermino']  ?>
                                            </td>
                                            <td align="center">
                                            <input type="hidden" name="dtf_spivlrfinanceiroanterior[]" value="<?=number_format($detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.') ?>">
                                            <?php echo campo_texto( 'dtf_spivlritemexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemexecanterior'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlritemsobreobraexecanterior[' . $obrid . '][]', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($detalheFilho['spivlritemsobreobraexecanterior'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlrinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 7, 6, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, '. $obrid .', '. $detalheFilho['icoid'] .', '. $detalheFilho['ditidpai'] .' )', number_format($detalheFilho['spivlrinfsupervisor'], 2, ',', '.')); ?>


                                            </td>
                                            <td align="center">
                                            <?php echo campo_texto( 'dtf_spivlrfinanceiroinfsupervisor[' . $obrid . '][]', 'N', $somenteLeitura, '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerDetalhamentoFilho( this, '. $obrid .', '. $detalheFilho['icoid'] .', '. $detalheFilho['ditidpai'] .', \'numerico\'  )', number_format($detalheFilho['spivlrfinanceiroinfsupervisor'], 2, ',', '.')); ?>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <input type="hidden" name="dtf_spipercsobreobrainfsupervisor[<?php echo $obrid?>][]" value="<?=number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>">
                                            <span>
                                            <?=number_format($detalheFilho['spipercsobreobrainfsupervisor'], 2, ',', '.') ?>
                                            </span>
                                            </td>
                                            <td align="right" style="color:#336EFF">
                                            <?=($detalheFilho['umcdsc'] ?
                                                            campo_texto( 'dtf_spivlrmetafisicasupervisor[' . $obrid . '][' . $detalheFilho['ditid'] . '][]', 'N', 'S', '', 11, 15, '', '', 'right', '', 0, '', 'calculoSupervisao.managerMetaFisica( this, ' . $detalheFilho['ditmetafisica'] . ' )', number_format($detalheFilho['spivlrmetafisicasupervisor'], 2, ',', '.'))
                                                                   :
                                                            '&nbsp;') ?>
                                            </td>

		                                   <td align="right" style="color:#336EFF">
			                                	<input type="hidden" value="<?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencavlr[]">
												<span> <?php echo number_format($detalheFilho['spivlrfinanceiroinfsupervisor'] - $detalheFilho['spivlrfinanceiroanterior'], 2, ',', '.'); ?> </span>
			                                </td>
			                                <td align="right" style="color:#336EFF">
			                                	<input type="hidden" value="<?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?>" name="dtf_spidiferencaporcentagem[]">
												<span> <?php echo number_format($detalheFilho['spivlrinfsupervisor'] - $detalheFilho['spivlritemexecanterior'], 2, ',', '.'); ?> </span>
			                                </td>

                                        </tr>
                                    </tbody>
<?php
                                }
                            }
                        }
?>
                    <tbody>
                        <tr id="tr_total_supervisao_<?php echo $obrid ?>" bgcolor="#E3E3E3">
                            <td align="right">
                            <b>Total</b>
                            <input type="hidden" name="vlrProjeto_<?php echo $obrid ?>" id="vlrProjeto_<?php echo $obrid ?>" value="<?=$total['vlrProjeto'] ?>">
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['vlrObra'], 2, ',', '.') ?>
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['percObra'], 2, ',', '.') ?>
                            </td>
                            <td align="center">
                            &nbsp;
                            </td>
                            <td align="center">
                            &nbsp;
                            </td>
                            <td align="center">
                            &nbsp;
                            </td>
                            <td align="center">
                            &nbsp;
                            </td>
                            <td align="right">
                            &nbsp;
                            <?php // echo campo_texto( 'total_spivlritemexecanterior', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                            </td>
                            <td align="center">
                            <?php echo campo_texto( 'total_spivlritemsobreobraexecanterior', 'N', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($total['percExecSobreObraUlt'], 2, ',', '.')); ?>
                            </td>
                            <td align="right">
                            <?php // echo campo_texto( 'total_spivlrinfsupervisor', 'N', 'N', '', 7, 6, '', '', '', '', 0, '', '', '0,00'); ?>
                            </td>
                            <td align="right">
                            &nbsp;
                            </td>
                            <td align="right" style="color:#336EFF">
                            <?=number_format($total['percExecSobreObra'], 2, ',', '.') ?>
                            </td>
                            <td align="right">
                            &nbsp;
                            </td>
                            <td align="right">
                            &nbsp;
                            </td>
                            <td align="right">
                            &nbsp;
                            </td>
                        </tr>
                    </tbody>
                </table>
                <br>
            </td>
        </tr>    
        <?php
		endforeach;
        ?>
        
        
        
        
        
        
        
        
    <tr>
    	<td colspan="2">
	    <?php 
		$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras2","destino" => "obras2");
		$_SESSION['imgparams'] = array("filtro" => " 1=1 ", "tabela" => "obras2.arquivoquestaosupervisao");
	    
	    $questao  = new Questao();
	    $arFiltro = array("qstescopo" => "SOBR",
	    				  "orgid" 	  => $_SESSION['obras2']['orgid'], 
	    				  "emsid" => ($emsid ? $emsid : 0));
	    $arDados  = $questao->pegaTodaEstrutura( $arFiltro );
	    $etqidUlt = null;
	    $itcidUlt = null;
	    $qstidUlt = null;
	    
	    foreach ( $arDados as $dados ){
	    	$qstctrlobs = json_decode( $dados['qstctrlobs'] );
	    	switch ( $dados['qtsresposta'] ){
	    		case 't':
	    			$obsClass = ($qstctrlobs->S ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
	    			break;
	    		case 'f':
	    			$obsClass = ($qstctrlobs->N ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
	    			break;
	    		case 'n':
	    			$obsClass = ($qstctrlobs->NA ? 'divQuestaoNivel3' : 'divQuestaoNivel3_none');		
	    			break;
	    		default:
	    			$obsClass = 'divQuestaoNivel3_none';		
	    			break;
	    	}
	    	
	    	$qstctrlimg = json_decode( $dados['qstctrlimagem'] );
	    	switch ( $dados['qtsresposta'] ){
	    		case 't':
	    			$imgClass = ($qstctrlimg->S ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
	    			break;
	    		case 'f':
	    			$imgClass = ($qstctrlimg->N ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
	    			break;
	    		case 'n':
	    			$imgClass = ($qstctrlimg->NA ? 'divQuestaoNivel4' : 'divQuestaoNivel4_none');		
	    			break;
	    		default:
	    			$imgClass = 'divQuestaoNivel4_none';		
	    			break;
	    	}
	    	
	    	// ABRE Questionário
	    	if ( $etqidUlt != $dados['etqid'] ){
	    		if ( !is_null($etqidUlt) ){
					$html .= "</fieldset>";	    		
	    		}
    			$etqidUlt = $dados['etqid'];
				$html .= "<fieldset>
					      	<legend>{$dados['etqdsc']}</legend>";	    		
	    	}
	    		    	
	    	// ETAPA
	    	if ( $itcidUlt != $dados['itcid'] ){
	    		$itcidNum++;
				$html .= "<div class='divItemComposicao'>{$itcidNum} - {$dados['itcdesc']}</div>";	    		
	    		$itcidUlt = $dados['itcid'];
	    	}
	    	
	    	// DIVISÃO
	    	if ( $dvqidUlt != $dados['dvqid'] && !empty( $dados['dvqid'] ) ){
				$html .= "<div class='divDivisao'>{$dados['dvqnumero']} - {$dados['dvqdsc']}</div>";	    		
	    		$dvqidUlt = $dados['dvqid'];
	    	}
	    	
	    	// QUESTÃO
	    	if ( $qstidUlt != $dados['qstid'] ){
				$html .= "<div class='divQuestao'>
							<div class='divQuestaoNivel1'>{$dados['qstnumero']} - {$dados['qstdsc']}</div>
							<div class='divQuestaoNivel2'>
								<input type='hidden' name='qtsid_{$dados['qstid']}' id='qtsid_{$dados['qstid']}' value='{$dados['qtsid']}'>
								<input name='qstid[{$dados['qstid']}]' id='qstid_t_{$dados['qstid']}' value='t' type='radio' onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->S ? 'true' : 'false' ) . "); ctrlImg({$dados['qstid']}, " . ($qstctrlimg->S ? 'true' : 'false' ) . ");' " . ($dados['qtsresposta'] == 't' ? "checked='checked'" : "") . ">
								<label for='qstid_t_{$dados['qstid']}'>Sim</label>
								&nbsp;&nbsp;
								<input name='qstid[{$dados['qstid']}]' id='qstid_f_{$dados['qstid']}' value='f' type='radio' onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->N ? 'true' : 'false' ) . "); ctrlImg({$dados['qstid']}, " . ($qstctrlimg->N ? 'true' : 'false' ) . ");' " . ($dados['qtsresposta'] == 'f' ? "checked='checked'" : "") . ">
								<label for='qstid_f_{$dados['qstid']}'>Não</label>
								&nbsp;&nbsp;
								<input name='qstid[{$dados['qstid']}]' id='qstid_n_{$dados['qstid']}' value='n' type='radio' onclick='ctrlObs({$dados['qstid']}, " . ($qstctrlobs->NA ? 'true' : 'false' ) . "); ctrlImg({$dados['qstid']}, " . ($qstctrlimg->NA ? 'true' : 'false' ) . ");' " . ($dados['qtsresposta'] == 'n' ? "checked='checked'" : "") . ">
								<label for='qstid_n_{$dados['qstid']}'>Não se aplica</label>
							</div>
							<div class='" . $obsClass . "' id='div_obs_{$dados['qstid']}'>
								<div class='divQuestaoNivel3_1'>Observação:</div>
								" . campo_textarea('qtsobs[' . $dados['qstid'] . ']', 'S', 'S', '', 100, 4, 500, '', '', '', '', '', $dados['qtsobs'], array('id' => "qtsobs_{$dados['qstid']}")) . "
							</div>";
				 
//				if ( $dados['qstimagem'] == 't' ){ 
					$html .= "<div class='" . $imgClass . "' id='div_arq_{$dados['qstid']}'>
									<fieldset class='divQuestaoNivel4_fieldset'>
										<legend>Fotos:</legend>
										<div class='divQuestaoNivel4_1'>
											<input type='hidden' name='totImg_{$dados['qstid']}' id='totImg_{$dados['qstid']}'>
											<input type='file' name='img_{$dados['qstid']}_0'>
										</div>
										<div class='divQuestaoNivel4_2'>
											<input type='button' name='btn_novoarquivo' value='+ Foto' onclick='addArquivo({$dados['qstid']});'>
										</div>";
					
					$arqQuestSup = new ArquivoQuestaoSupervisao();	
					$arqDados 	 = ($dados['qtsid'] ? $arqQuestSup->listaPorRespQuestao( $dados['qtsid'] ) : array());
					
					if ( $arqDados ){
						$html .= "<div class='divQuestaoNivel4_3'>";
						
						foreach ( $arqDados as $arqDado ){
							
							$imgend = APPRAIZ.'arquivos/obras2/'. floor($arqDado['arqid']/1000) .'/'.$arqDado['arqid'];

							if(is_file($imgend)){
                            	$img_max_dimX = 100;
                                $img_max_dimY = 85;

                                $imginfo = getimagesize($imgend);

                                $width = $imginfo[0];
                                $height = $imginfo[1];

                                if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
                                	if ($width > $height){
                                    	$w = $width * 0.9;
                                        while ($w > $img_max_dimX){
                                        	$w = $w * 0.9;
                                        }
                                        $w = round($w);
                                        $h = ($w * $height)/$width;
									}else{
                                    	$h = $height * 0.9;
                                        while ($h > $img_max_dimY){
                                        	$h = $h * 0.9;
                                        }
                                        $h = round($h);
                                        $w = ($h * $width)/$height;
                                    }
								}else{
                                	$w = $width;
                                    $h = $height;
                                }

								$tamanho = " width=\"$w\" height=\"$h\" ";
							}else{
                            	$tamanho = "";
                            }	
							
                            $html .= "<div class=\"\" style=\"float:left; width:100px; height:80px; text-align:center;\" >
                            			<img title=\"".$arqDado["arqdescricao"]."\" 
                            				 border='1' 
                            				 id='".$arqDado["arqid"]."'
                            				 {$tamanho} 
											 src='../slideshow/slideshow/verimagem.php?&newwidth=100&newheight=85&arqid=".$arqDado["arqid"]."&_sisarquivo=obras2' 
											 hspace='0' 
											 vspace='0' 
											 style='cursor:pointer;' 
											 onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=obras2&arqid=\"+this.id+\"&qtsid=" . $dados['qtsid'] . "&getFiltro=true\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>
									  </div>";
						}
						
						$html .= "</div>";
					} 
					$html .= "	</fieldset>
							  </div>";
//				}	
				$html .= "</div>";	    		
	    		$qstidUlt = $dados['qstid'];
	    	}

	    	
//	    	// FECHA Questionário
//	    	if ( $etqidUlt != $dados['etqid'] && !is_null($etqidUlt) ){
//				$html .= "</fieldset>";	    		
//	    		$etqidUlt = $dados['etqid'];
//	    	}elseif ( is_null($etqidUlt) ){
//	    		$etqidUlt = $dados['etqid'];
//	    	}
	    	
	    	echo $html;
	    	$html = null;
	    }
	    ?>
    	</td>
    </tr>
        
        
        
        
        
        
        
        
        
        
	    <tr>
	        <td bgcolor="#c0c0c0" colspan="2" align="center">
	                <?php if($habilitado && $somenteLeitura == 'S'){ ?>
	                    <input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario();">
	                <?php } ?>
	                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listaVistoriaObra&acao=A';">
	        </td>
	    </tr>
</table>
</form>
<script type="text/javascript">
function ctrlObs(qstid, val){
	if ( val == true ){
		$('#div_obs_' + qstid).show();
	}else{
		$('#div_obs_' + qstid).hide()
							  .find('textarea')
							  .val('');
	}
}

function ctrlImg(qstid, val){
	if ( val == true ){
		$('#div_arq_' + qstid).show();
	}else{
		$('#div_arq_' + qstid).hide()
							  .find('input:file')
							  .val('');
	}
}

var totalArq = new Array();
function addArquivo(qstid){
	totalArq[qstid] = (totalArq[qstid] ? totalArq[qstid] + 1 : 1);
	
	$('<br>').appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	$('#div_arq_' + qstid).find('[type=file]:eq(0)')
						  .clone()
						  .val('')
						  .attr('name', 'img_'+ qstid + '_' + totalArq[qstid])
						  .appendTo('#div_arq_' + qstid + ' .divQuestaoNivel4_1');
	$('#totImg_' + qstid).val( totalArq[qstid] );

}

function calculoSupervisao(){
    var objClass     = this;
    var obCalc       = new Calculo();
    var percentTotal = new Number(0);
    var itemEdicao        = '';
    var vlrItemAnterior   = '';

    this.managerEdificacao = function (obj, tipo, obrid){
        obj  = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = $( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaTotalSupervisao( obrid );
    }

    this.managerEtapa = function (obj, obrid, tipo){
        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = $( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();
        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }

    this.managerDetalhamento = function (obj, obrid, icoid, tipo){

        obj = obj || '';
        tipo = tipo || 'percentual';

        var vlrItemObra = $( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);

        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }

        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }


    this.managerDetalhamentoFilho = function (obj, obrid, icoid, ditidPai, tipo){
        obj       = obj || '';
        tipo      = tipo || 'percentual';

        var vlrItemObra = $( obj ).parents('tr:eq(0)')
                              .find('td:eq(1)')
                              .html();

        var vlrObj = mascaraglobal("[###.]###,##", obj.value);
        if (tipo == 'percentual' && obCalc.comparar(new String(vlrObj), '100,00', '>') ){
            obj.value = '100,00';
        }else if (obCalc.comparar(new String(vlrObj), vlrItemObra, '>') ){
            obj.value = vlrItemObra;
        }
        this.formatarNumeroMonetario( obj );
        this.calculaPercentLinha( obj, tipo, obrid );

        calculaNivelDetalhamento( obrid, icoid, ditidPai );
//        calculaNivelEtapa( obrid, icoid );
//        calculaNivelEdificacao( obrid );
        calculaTotalSupervisao( obrid );
    }


    this.calculaPercentLinha = function ( obj, tipo, obrid ){
        var vlrItemObra = $( obj ).parents('tr:eq(0)')
                                  .find('td:eq(1)')
                                  .html();
        // CALCULA: "Valor Executado" a partir do "(%) Supervisão"
        if ( tipo == 'percentual' ){
            var calcVlrItemSup = obCalc.operacao(vlrItemObra, obj.value, '*', 20);
            calcVlrItemSup = obCalc.operacao(calcVlrItemSup, 100, '/', 20);
            $( obj ).parents('tr:eq(0)')
                    .find('td:eq(10) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcVlrItemSup ).toFixed(2)} ) );
        // CALCULA: "(%) Supervisão" a partir do "Valor Executado"
        }else{
            var calcPercItemSup = obCalc.operacao(obj.value, vlrItemObra, '/', 20);
            calcPercItemSup = obCalc.operacao(calcPercItemSup, 100, '*', 20);
            $( obj ).parents('tr:eq(0)')
                    .find('td:eq(9) input')
                    .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSup ).toFixed(2)} ) );
        }

        // CALCULA: "(%) do Item já Executado sobre a Obra após Supervisão" a partir do "Valor Executado"
        var vlrItemSup = $( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();
        var vlrProjeto = $('#vlrProjeto_' + obrid).val();
        var calcPercItemSupSobreObra = obCalc.operacao(vlrItemSup, vlrProjeto, '/', 20);
        calcPercItemSupSobreObra = obCalc.operacao(calcPercItemSupSobreObra, 100, '*', 20);
        $( obj ).parents('tr:eq(0)')
                .find('td:eq(11) span')
                .html( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );
                
        $( obj ).parents('tr:eq(0)')
                .find('td:eq(11) input')
                .val( this.formatarNumeroMonetario( {value : new Number( calcPercItemSupSobreObra ).toFixed(2)} ) );



        // CALCULA: "diferença Valor Executado
        var vlrItemSup  = $( obj ).parents('tr:eq(0)')
                                   .find('td:eq(10) input')
                                   .val();

        var vlrAnterior = $( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="hidden"]')
                                   .val();


       	var calcDiferencaValor = obCalc.operacao(vlrItemSup, vlrAnterior, '-', 20);

       	var sinal = "";
       	if( new Number( calcDiferencaValor ) < 0 ){
	       	sinal = "-";
       	}else{
	       	sinal = "";
        }

        $( obj ).parents('tr:eq(0)')
                .find('td:eq(13) span')
                .html( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );

        $( obj ).parents('tr:eq(0)')
                .find('td:eq(13) input')
                .val( sinal+""+this.formatarNumeroMonetario( {value : new Number( calcDiferencaValor ).toFixed(2)} ) );


        // CALCULA: "diferença % Executado
        var percItemSup  = $( obj ).parents('tr:eq(0)')
                                   .find('td:eq(9) input')
                                   .val();

        var percAnterior = $( obj ).parents('tr:eq(0)')
                                   .find('td:eq(7) input[type="text"]')
                                   .val();

        var calcDiferencaPerc = obCalc.operacao(percItemSup, percAnterior, '-', 20);

        $( obj ).parents('tr:eq(0)')
                .find('td:eq(14) span')
                .html(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );

        $( obj ).parents('tr:eq(0)')
                .find('td:eq(14) input')
                .val(  (new Number( calcDiferencaPerc ).toFixed(2)).replace(".",",") );


    }

    this.formatarNumeroMonetario = function ( obj ){
        var valor = new String( obCalc.retiraZeroEsquerda( new String(obj.value) ) );

        if ( valor.length == 2 ){
            valor = '0' + valor;
        }else if ( valor.length == 1 ){
            valor = '00' + valor;
        }else if ( valor.length == 0 ){
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }

    this.managerMetaFisica = function ( obj, limiteMetaFisica ){
        obj = obj || '';

        var vlrObj = this.formatarNumeroMonetario( {value : new String( obj.value )} );
        obj.value = vlrObj;
        if (obCalc.comparar(new String(vlrObj), limiteMetaFisica, '>') ){
            obj.value = this.formatarNumeroMonetario( {value : new Number( limiteMetaFisica ).toFixed(2) } );
        }

    }

    function calculaNivelEdificacao( obrid ){
        var idTrEdicacao   = 'tr_item_edificacao_';
        var idTrEtapa      = 'tr_item_etapa_';
        var vlrEtapaSupTot = 0;

        $('[id^=' + idTrEtapa + obrid + '_]').each(function (i, obj){
        	vlrEtapaSupTot = obCalc.operacao(vlrEtapaSupTot, $( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        $( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')
                                       .val( objClass.formatarNumeroMonetario( {value : new Number( vlrEtapaSupTot ).toFixed(2)} ) );
        objClass.managerEdificacao($( '#' + idTrEdicacao + obrid ).find('td:eq(10) input')[0] , 'numerico');
    }

    function calculaNivelEtapa( obrid, icoid ){
        var idTrEtapa             = 'tr_item_etapa_';
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var vlrDetalhamentoSupTot = 0;
        $('[id^=' + idTrDetalhamento + obrid + '_' + icoid + '_]').each(function (i, obj){
        	vlrDetalhamentoSupTot = obCalc.operacao(vlrDetalhamentoSupTot, $( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        $( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoSupTot ).toFixed(2)} ) );

        objClass.managerEtapa($( '#' + idTrEtapa + obrid + '_' + icoid ).find('td:eq(10) input')[0], obrid, 'numerico');
    }

    function calculaNivelDetalhamento( obrid, icoid, ditidPai ){
        var idTrDetalhamento      = 'tr_item_detalhamento_';
        var idTrDetalhamentoFilho = 'tr_item_detalhamento_filho_';

        var vlrDetalhamentoFilhoSupTot   = 0;

        $('[id^=' + idTrDetalhamentoFilho + obrid + '_' + icoid + '_' + ditidPai + '_]').each(function (i, obj){
        	vlrDetalhamentoFilhoSupTot = obCalc.operacao(vlrDetalhamentoFilhoSupTot, $( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        $( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')
                                                  .val( objClass.formatarNumeroMonetario( {value : new Number( vlrDetalhamentoFilhoSupTot ).toFixed(2)} ) );

        objClass.managerDetalhamento($( '#' + idTrDetalhamento + obrid + '_' + icoid + '_' + ditidPai ).find('td:eq(10) input')[0], obrid, icoid, 'numerico');
    }

    function calculaTotalSupervisao( obrid ){
//        var idTrEdificacao      = 'tr_item_edificacao_';
        var idTrEtapa      		= 'tr_item_etapa_' + obrid;
        var idTrTotal           = 'tr_total_supervisao_' + obrid;
        var vlrProjeto          = $('#vlrProjeto_' + obrid).val();
        var vlrEdificacaoSupTot = 0;
        var percSupTot          = 0;

//        $('[id^=' + idTrEdificacao + '_]').each(function (i, obj){
        $('[id^=' + idTrEtapa + '_]').each(function (i, obj){
          vlrEdificacaoSupTot = obCalc.operacao(vlrEdificacaoSupTot, $( obj ).find('td:eq(10) input').val(), '+', 20);
        });

        percSupTot = obCalc.operacao(vlrEdificacaoSupTot, vlrProjeto, '/', 20);
        percSupTot = obCalc.operacao(percSupTot, 100, '*', 20);

        $('#' + idTrTotal).find('td:eq(11)')
                        .html( objClass.formatarNumeroMonetario( {value : new Number( percSupTot ).toFixed(2)} ) );
    }

}

calculoSupervisao = new calculoSupervisao();

function inserirVistoriador(entid){
     var caminho_atual = '/obras2/obras2.php';

    if (entid){
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }else{
        return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
}

function enviaFormulario(){
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	if ( $('#emsdtvistoria').val() == '' ){
		mensagem += 'Data Prevista de Conclusão da Obra \n';
		validacao = false;
	}	
	
	if ( $('#emsdtconclusao').val() == '' ){
		mensagem += 'Data Prevista de Conclusão da Obra \n';
		validacao = false;
	}	
	
	if ( $('#entidvistoriador').val() == '' ){
		mensagem += 'Nome do Responsável \n';
		validacao = false;
	}	
	
	if ( $('#staid').val() == '' ){
		mensagem += 'Estágio \n';
		validacao = false;
	}	

	if (!validacao){
		alert(mensagem);
	}else{
		$('#formulario_supervisao').submit();
	}
	
}

</script>