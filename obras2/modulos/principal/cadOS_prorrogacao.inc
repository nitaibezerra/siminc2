<?php

if( !$_GET['sosid'] ){
	echo "<script>
			alert('Não foi possível encontrar as informações mínimas necessárias para acessar essa tela.');
			self.close();
		</script>";
	exit;
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

$habil 		  	= true;
$somenteLeitura = 'S';
if ( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC) ) == false
	|| true ){ // por enquanto somente bloqueia o que nao pode nunca
	$habil 		  	= false;
	$somenteLeitura = 'N';
}

switch ( $_POST['op'] ){
	case 'salvar':
		$os = new Supervisao_Os( $_POST['sosid'] );
		$docid = $os->docid;
		if( !$docid ){
			// descrição do documento
			$docdsc = "Fluxo de OS do módulo Obras II - sosid " . $sosid;
			// cria documento do WORKFLOW
			$docid = wf_cadastrarDocumento( TPDID_OS, $docdsc );
		}

		$arDado = array('usucpf' 	     => $_SESSION['usucpf'],
						'sgeid'  	     => $_POST['sgeid'] ? $_POST['sgeid'] : $_POST['sgeid_disable'],
						'semid'  	     => $_POST['semid'] ? $_POST['semid'] : $_POST['semid_disable'],
						'sosnum'  	     => $_POST['sosnum'],
						'docid'  	     => $docid,
						'sosvalortotal'  => $_POST['sosvalortotal'],
						'sosdiasexecucao'=> $_POST['sosdiasexecucao'],
						'sosemergencial' => ($_POST['sosemergencial'] == 't' ? 't' : 'f'),
						'sosdtemissao'   => formata_data_sql( $_POST['sosdtemissao'] ),
						'sosdtinicio'    => formata_data_sql( $_POST['sosdtinicio'] ),
						'sosobs'         => $_POST['sosobs'],
						'sosdttermino'   => ( ($_POST['nova_data'])?formata_data_sql($_POST['nova_data']):formata_data_sql($_POST['sosdttermino']) ) );

		//O número da OS ñ pode ser alterado, apenas criado!
		if($_POST['sosid']){
			unset($arDado['sosnum']);
		}
		$sosid = $os->popularDadosObjeto( $arDado )
					->salvar();

		// tratando dados da prorrogacao (registrando a mesma)
		$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOS();
		$dadosProrrogacao = array('prazo_anterior' 	=> ( ($_POST['nova_data'])?formata_data_sql($_POST['nova_data']):formata_data_sql($_POST['sosdttermino']) ),
								  'novo_prazo' 		=> formata_data_sql($_POST['nova_data']),
						 		  'justificativa' 	=> $_POST['justificativa'],
						 		  'usucpf' 			=> $_SESSION['usucpf'],
						 		  'data_prorrogacao'=>  date('Y-m-d H:m:s'),
						 		  'sosid' 			=> $_POST['sosid']);
		$prposid = $ProrrogacaoPrazoOs->popularDadosObjeto( $dadosProrrogacao )
						   			  ->salvar();

								 		  
		// $osObra = new Supervisao_Os_Obra();
		// $osObra->apagaPorSosid( $sosid );
		// foreach ( $_POST['empid'] as $empid ){
		// 	$arDado = array('sosid' => $sosid,
		// 					'empid' => $empid);
		// 	$osObra->popularDadosObjeto( $arDado )
		// 		   ->salvar();
		// 	$osObra->clearDados();	   
		// }
								 		  
	    $db->commit();
	    
	    $_SESSION['obras2']['sosid'] = $sosid;
	    
		die("<script>
				alert('Operação realizada com sucesso!'); 
				window.opener.location.reload();
				self.close();
			 </script>");
}


switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		carregaEmpresaAndListaObra();
		die;
	case 'carregaEmpenho':
		$sgeid = $_POST['sgeid'];
		if ( !empty($sgeid) ){
            $empenho = new Supervisao_Empenho();
            $dados = $empenho->listaCombo( array('sgeid' => $sgeid) );
        	$db->monta_combo("semid", $dados, 'S', "Selecione...", "", '', '', '', 'S', 'semid');
		}	
		die;
	case 'carregaEmpenhoPorEmpresa':
		$entid = $_POST['entid'];
		$sgrid = $_POST['sgrid'];
		if ( !empty($sgrid) && !empty($entid) ){
                    $empenho = new Supervisao_Empenho();
                    $dados = $empenho->listaComboPorEmpresa( array('sgrid' => $sgrid, 'entid'=>$entid) );
                    $db->monta_combo("semid", $dados, 'S', "Selecione...", "", '', '', '', 'S', 'semid');
		}	
		die;
          
}

if ( $_GET['acao'] == 'A' ){
	$_SESSION['obras2']['sosid'] = '';
}else{
	$sosid  = ($_GET['sosid'] ? $_GET['sosid'] : $_SESSION['obras2']['sosid']);
	$os 	= new Supervisao_Os( $sosid ); 
	extract( $os->getDados() );
	$grupoEmpresa = new Supervisao_Grupo_Empresa();
	if($sgeid){
		$sgrid = $grupoEmpresa->pegaGrupoIdPorSgeid( $sgeid );
	}
	
//	$docid = pegaDocidOs( $os->sosid );
}

$osObra  = new Supervisao_Os_Obra();
$arEmpid = $osObra->listaEmpidPorOs( $sosid );

// busca prorrogacao
$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOS();
if( $ProrrogacaoPrazoOs->verificaProrrogacao( $sosid ) )
	$prorrogacao = 1;

//Chamada de programa
// include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
// criaAbaOS();
monta_titulo( 'Cadastro de OS', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

//$somenteLeitura = 'S';
?>
<meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>.link{cursor:pointer}</style>
<form id="formulario_os" name="formulario_os" method="post" enctype="multipart/form-data" action="">
<input type="hidden" name="sosid" id="sosid" value="<?php echo $sosid ?>" />
<input type="hidden" name="op" id="op" value="salvar" />
<input type="hidden" name="hdn_mescod" id="hdn_mescod" value="<?php echo $arMescod ? implode(",",$arMescod) : "" ?>">
<input type="hidden" name="hdn_orgid" id="hdn_orgid" value="<?php echo $orgid ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
	<tr>
    	<td colspan="3">Dados da OS</td>
		<td rowspan="8" align="right" valign="top" width="1">
            <?php
            // Barra de estado WORKFLOW
//                wf_desenhaBarraNavegacao($docid, array('sosid' =>  $sosid));
            ?>
    	</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Tipo de Ensino</td>
		<td>
			<?php
                $orgao = new Orgao();
                $dados = $orgao->listaCombo();
                
            	$db->monta_combo("orgid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupoPorOrgid", '', '', '', 'N', 'orgid');
            ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" width="35%">Grupo</td>
		<td>
			<?php
                $grupo = new Supervisao_Grupo();
                $dados = $grupo->listaCombo();
                
            	$db->monta_combo("sgrid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupo", '', '', '', 'S', 'sgrid');
            ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Empresa</td>
		<td id="tdComboEmpresa">
			<?php
			if ( !empty($sgrid) ){
                $grupoEmpresa = new Supervisao_Grupo_Empresa();
                $dados = $grupoEmpresa->listaCombo( array('sgrid' => $sgrid) );
                
            	$db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaEmpresa", '', '', '', 'S', 'sgeid');
			}	
            ?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita">Nota de Empenho</td>
		<td id="tdEmpenho">
			<?php
			if ( !empty($sgeid) ){
                $empenho = new Supervisao_Empenho();
                $dados = $empenho->listaCombo( array('sgeid' => $sgeid) );
                
            	$db->monta_combo("semid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'semid');
			}	
            ?>
		</td>
	</tr>
    <tr>
    	<td class="SubTituloDireita">OS Nº</td>
        <td>
        	<?php $sosnum = ($sosnum) ? $sosnum : ($sosid && $sgrid && $sgrid ? $sosid.$sgrid.$sgeid.date("Y") : ""); ?>
			<?=campo_texto('sosnum', 'S', "N", '', 10, 10, '', '', 'right', '', 0, 'id="sosnum"', ''); ?>
        </td>
	</tr>
    <tr>
    	<td class="SubTituloDireita">Data Emissão</td>
        <td>
			<?php
            echo campo_data2( 'sosdtemissao', 'S', $somenteLeitura, '', 'S','','' );
            ?>
        </td>
	</tr>
    <tr>
    	<td class="SubTituloDireita" valign="top">Período de Execução do Serviço</td>
    	<td colspan="3">
    		<div>
	            <b>De</b>
				<?php
	            echo campo_data2( 'sosdtinicio', 'S', $somenteLeitura, '', 'S','', '', '','controlaPeriodoExecucao();' );
	            ?>
	            <div id="div_sosdttermino" style="display:<?php echo ($sosdttermino ? 'inline' : 'none') ?>;">
            	<b>até</b> <?php echo formata_data( $sosdttermino ); if($prorrogacao) echo " <a title='Ver Lista de Prorrogações' onclick='historicoProrrogacao(".$sosid.")' style='color:red;font-size:10px;'>(Visualizar Prorrogações)</a>"; ?>
	            </div>
	            <input type="hidden" name="sosdttermino" id="sosdttermino" value="<?=formata_data( $sosdttermino )?>">
	            <br>
	            <input type="radio" name="sosemergencial" id="" value="10" onclick="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 10  ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
	            <label for="sosemergencial_f">10 DIAS</label>
	            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	            <input type="radio" name="sosemergencial" id="" value="20" onclick="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 20 ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
	            <label for="sosemergencial_f">20 DIAS</label>
	            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	            <input type="radio" name="sosemergencial" id="" value="30" onclick="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 30 ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
	            <label for="sosemergencial_f">30 DIAS</label>
	            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	            <input type="radio" name="sosemergencial" id="sosemergencial_t" value="t" onclick="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 't' ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
	            <label for="sosemergencial_t">EMERGENCIAL</label>
	            <span id="span_sosdiasexecucao" style="display: <?php echo ($sosemergencial == 't' ? 'inline' : 'none') ?>;">
	            <?=campo_texto('sosdiasexecucao', 'S', $somenteLeitura, '', 10, 18, '[###]', '', 'right', '', 0, 'id="sosdiasexecucao"', 'addDiaPeriodoExecucao( this.value )'); ?>
	            DIAS
	            </span>
            </div>
        </td>
	</tr>	
	<tr>
        <td class="SubTituloDireita">
        	Valor Total dos Serviços (R$)
			<input type="hidden" name="sosvalortotal" id="sosvalortotal" value="<?php echo $sosvalortotal; ?>">
        </td>
		<td id="tdTotalServico" colspan="3">
		<span id="valorTotalObra">
		<?php echo ($sosvalortotal ? number_format($sosvalortotal, 2, ',', '.') : '-'); ?>
		</span>
		<span id="qtdTotalObra">
		<?php echo ( count($arEmpid) ? ' - ' . count($arEmpid) . ' Obra(s)'  :  '' ) ?>
		</span>
		</td>
	</tr>
    <tr>
		<td align='right' class="SubTituloDireita" >Observação:</td>
		<td><?= campo_textarea( 'sosobs', 'N', $somenteLeitura, '', 80, 4, 1000 ); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" >Nova Data de Término:</td>
		<td><?= campo_data2( 'nova_data', 'S', 'S', '', 'S','', '', '','' ) ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" >Justificativa para prorrogação:</td>
		<td><?= campo_textarea( 'justificativa', 'N', 'S', '', 80, 4, 1000 ); ?></td>
	</tr>
<?php
	if ( $habil ):
?>	
	<tr>
		<td colspan="2" ><span title="Incluir Obras" class="link" style="font-weight:bold" onclick="popUpAddObra()"><img style="vertical-align:middle" src="../imagens/gif_inclui.gif" /> Incluir Obras</span></td>	
	</tr>
<?php
	endif;
?>	
	<tr>
		<td class="SubtituloCentro" colspan="2">
		Obra(s) da OS
		</td>	
	</tr>
	<tr>
		<td colspan="4">
			<div id="divListaObra" style="height: 300px; overflow: auto;">
		<?php	
		if ( $sgeid ){
			$grupoEmpresa  = new Supervisao_Grupo_Empresa();
			$valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid( $sgeid );
            $valorUnitariob = $grupoEmpresa->pegaValorUnitariobPorSgeid($sgeid);
		?> 
			<input type="hidden" rel="<?= number_format($valorUnitario, 2, ',', '.'); ?>" name="sgevalorunitario" id="sgevalorunitario" value="<?php echo $valorUnitario; ?>">
            <input type="hidden" rel="<?= number_format($valorUnitariob, 2, ',', '.'); ?>" name="sgevalorunitariob" id="sgevalorunitariob" value="<?php echo $valorUnitariob; ?>">
		<?php	
		}
		if ( !empty( $sgrid ) ){
			$supMesoregiao = new Supervisao_Grupo_Mesoregiao();
			$arMescod 	   = $supMesoregiao->pegaMescodPorSgrid( $sgrid );
			if($arMescod){
				echo "<script>$(\"#hdn_mescod\").val('".implode(",",$arMescod)."')</script>";
			}
		}
		?>
		<table class="listagem" width="100%" bgcolor="#FFFFFF" id="lista_obra">
		<?php
		$vlrTotalServico = 0;
		if ( !empty( $sgrid ) ){
			$empreendimento = new Empreendimento();
                        $param = array('mescod' => $arMescod,
						   'orgid'	=> $orgid);

			$arDadoEmp = $empreendimento->listaDados( $param );
			
            foreach($empreendimento->listaDadosPorEmpid( $arEmpid ) as $emp){
                if( !in_array($emp['mescod'], $arMescod) )
                    $arDadoEmp[] = $emp;
            }
            
			if ( count($arDadoEmp) ){
		?>
				<thead>
                	<tr style="background-color: #CDCDCD;">
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		&nbsp;
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Nome da Obra
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		UF
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Mesorregião
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Microregião
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Município
                        </th>
                    	<th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		Valor Laudo
                    		<br>
                    		(R$)
                        </th>
					</tr>
				</thead>
		<?php	
//				$osObra  		 = new Supervisao_Os_Obra();
//				$arEmpid 		 = $osObra->listaEmpidPorOs( $sosid );
				$i				 = 0;
                    
				foreach ( $arDadoEmp as $dadoEmp ){
					$color = ($i%2 ? '#FFFFFF' : '#FFFFFF');
					$i++;
					if ( in_array($dadoEmp['empid'], $arEmpid) ){
                        if(count($arEmpid) > 10)
                            $vlrTotalServico += $valorUnitariob;	
                        else
                            $vlrTotalServico += $valorUnitario;	
					}

		?>
				
                	<tr style="background-color: <?=$color ?>;display:<?php echo in_array($dadoEmp['empid'], $arEmpid) ? "" : "none" ?>" id="tr_empid_<?php echo $dadoEmp['empid'] ?>">
                    	<td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<input type="checkbox" onclick="calculaTotalServico();" name="empid[]" id="empid_<?=$dadoEmp['empid'] ?>" value="<?=$dadoEmp['empid'] ?>" <?php echo (in_array($dadoEmp['empid'], $arEmpid) ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
                        </td>
                    	<td valign="middle" align="left" style="padding-left: 5px; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?='<label for="empid_' . $dadoEmp['empid'] . '">(' . $dadoEmp['obrid'] . ') ' . $dadoEmp['obrnome'] . '</label>' ?>
                        </td>
                    	<td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoEmp['estuf'] ?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoEmp['mesdsc'] ?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoEmp['micdsc'] ?>
                        </td>
                    	<td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    		<?=$dadoEmp['mundescricao'] ?>
                        </td>
                    	<td class="valorLaudo" valign="middle" align="right" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                    	<?php 
                            if(count($arEmpid) > 10)
                                echo ($valorUnitariob ? number_format($valorUnitariob, 2, ',', '.') : '-'); 
                            else
                                echo ($valorUnitario ? number_format($valorUnitario, 2, ',', '.') : '-'); 
                        ?>
                        </td>
					</tr>
				
		<?php			
				}
			}else{
		?>
				<tr style="color: red;">
					<td>
						Nenhuma obra encontrada.
					</td>
				</tr>
		<?php		
			}
		}
		?>
			</table>
			</div>
		</td>	
	</tr>
    <tr>
        <td bgcolor="#c0c0c0" colspan="4" align="center">
<?php
	// if ( $habil ):
?>        
			<input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario();">
<?php
	// endif;
?>        
            <!-- <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href='?modulo=principal/listOs&acao=A';"> -->
        </td>
    </tr>
	
</table>
</form>
<script>
$(function() {
    $('#sgrid,#sgeid').change(function(){
        atualizaNumOS();
    });
    
    $('input[name=sosemergencial]').change(function(){
        var val = $(this).val();
        if ( val == 't' ){
            $('#sosdiasexecucao').val('');
            $('#sosdttermino').val('');
            $('#div_sosdttermino').css('display', 'inline')
                                              .html('');
            $('#span_sosdiasexecucao').show();
        }else {
            $('#sosdttermino').val( '' );
            $('#sosdiasexecucao').val(val);
            $('#span_sosdiasexecucao').hide();
            addDiaPeriodoExecucao( parseInt(val) );
        }
    });

});

function calculaTotalServico(){
	var qtdEmpid = $('[name*=empid]:checked').size();
	var valUnit  = (qtdEmpid > 10) ? $('#sgevalorunitariob').val() : $('#sgevalorunitario').val();
    var valUnitExibicao = (qtdEmpid > 10) ? $('#sgevalorunitariob').attr('rel') : $('#sgevalorunitario').attr('rel');
    
	var calc 	 = new Calculo();
	var totVal   = calc.operacao(qtdEmpid, valUnit, '*', 2);
	
	$('#sosvalortotal').val( totVal );
	$('#valorTotalObra').html( mascaraglobal("###.###.###.###,##", totVal) );
	$('#qtdTotalObra').html( ' - ' + qtdEmpid + ' Obras(s)' );
    $('.valorLaudo').html(valUnitExibicao);
}

function enviaFormulario(){

	if( !confirm("Deseja realmente redefinir a data de término da Ordem de Serviço para " + jQuery('#nova_data').val() + "?") ){
		return false;
	}

	calculaTotalServico();
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	if ( $('#sosnum').val() == '' ){
		mensagem += 'OS Nº \n';
		validacao = false;
	}	
	
	if ( $('#sosdtemissao').val() == '' ){
		mensagem += 'Data Emissão \n';
		validacao = false;
	}	
	
	if ( $('#sgrid').val() == '' ){
		mensagem += 'Grupo \n';
		validacao = false;
	}	
	
	if ( $('#sgeid').val() == '' ){
		mensagem += 'Empresa \n';
		validacao = false;
	}	
	
	if ( $('#semid').val() == '' ){
		mensagem += 'Nota de Empenho \n';
		validacao = false;
	}	
	
	if ( $('[name*=empid]:checked').size() == 0 ){
		mensagem += 'Obra(s) \n';
		validacao = false;
	}	
	
	if ( $('#sosdtinicio').val() == '' || $('#sosdttermino').val() == '' ){
		mensagem += 'Período de Execução do Serviço \n';
		validacao = false;
	}	

        var obrMeso = '';
        $('#divListaObra input[type=checkbox]:checked').each(function(i){
            obrItemMeso = $(this).parents('tr:first').find('td:eq(3)').html();
             if (i == 0) 
                 obrMeso = $(this).parents('tr:first').find('td:eq(3)').html();
             
            if(obrMeso != obrItemMeso){
                mensagem += 'Não é permitido marcar Obras de diferentes mesorregiões \n';
		return validacao = false;
            }
        });
        
	if (!validacao){
		alert(mensagem);
	}else{
		$('#formulario_os').submit();
	}
	
}

function carregaDependenciaEmpresa( sgeid ){
	$('#sosvalortotal').html( '0,00' );
	$('#valorTotalObra').html( '0,00' );
	var sgrid = $('#sgrid').val();
	var sosid = $('#sosid').val();
	if ( sgeid ){
		$.post("?modulo=principal/cadOs&acao=A", {"sgeid":sgeid, "ajax":"carregaEmpenho"}, function(data){
			$('#tdEmpenho').html( data );
		});
		
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadOs&acao=A", {"sosid":sosid, "sgrid":sgrid, "sgeid":sgeid, "ajax":"carregaEmpresaAndListaObra", "not(comboGrupoEmpresa)":true}, function(data){
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
		});
	}else{
		$('#tdEmpenho').html('');
		if ( sgrid ){
			carregaDependenciaGrupo( sgrid );
		}
	}
	atualizaNumOS();
}

function carregaDependenciaGrupo( sgrid ){
	$('#sosvalortotal').html( '' );
	$('#valorTotalObra').html( '0,00' );
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$('#hdn_orgid').val(orgid);
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').html( comboEmpresa );
			
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
			
		});
	}else{
		$('#tdComboEmpresa').html('');
		$('#divListaObra').html('');
	}
	atualizaNumOS();
}

function carregaDependenciaGrupoPorOrgid( orgid ){
	var sgrid = $('#sgrid').val();
	if(orgid){
		$('#hdn_orgid').val(orgid);
	}
	if ( sgrid ){
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(comboGrupoEmpresa)":true}, function(data){
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
		});
	}
}

function controlaPeriodoExecucao( val ){
	val = val || $('[name=sosemergencial]:checked').val();
	val = val || '';
	
	if ( val == 'f' ){
		$('#sosdttermino').val( '' );
		$('#sosdiasexecucao').val('30');
		$('#span_sosdiasexecucao').hide();
		addDiaPeriodoExecucao( 30 );
	}else if( val == 't' ){
		$('#sosdiasexecucao').val('');
		$('#sosdttermino').val('');
		$('#div_sosdttermino').css('display', 'inline')
						  .html('');
		$('#span_sosdiasexecucao').show();
	}
}
function addDiaPeriodoExecucao( numDia ){
	var obData = new Data();
	
	var sosdtinicio  = $('#sosdtinicio').val();
	var sosdttermino = (sosdtinicio ? obData.dtAddDia(sosdtinicio, numDia) : '');
	$('#sosdttermino').val( sosdttermino );
	
	if ( sosdttermino ){
		$('#div_sosdttermino').css('display', 'inline')
						      .html('<b>até</b> ' + sosdttermino);
	}					  
}

function atualizaNumOS()
{
	var os_num = "<?php echo $sosid ? $sosid : $osObra->geraNovoNumOS() ?>";
	var grupo = $('#sgrid').val() ? $('#sgrid').val() : "";
	var empresa = $('#sgeid').val() ? $('#sgeid').val() : "";
	var ano = "<?php echo date("Y") ?>";
	$('#sosnum').val(os_num+grupo+empresa+ano);
}

function popUpAddObra()
{
	var sgrid = $('#sgrid').val();
	var orgid = $("#hdn_orgid").val();
	var mescod = $("#hdn_mescod").val();
	if(sgrid){
		return windowOpen( '?modulo=principal/cadOs&acao=E&classe=Empreendimento&requisicao=exibirListaObraPopUp&sosid=<?php echo $sosid ?>&orgid='+orgid+'&mescod='+mescod+'&sgrid='+sgrid,'blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		alert('Favor selecionar o Grupo.');
	}
}
function removeObraOS(empid)
{
	$("#tr_empid_" + empid).hide();
	$("#empid_" + empid).attr("checked",false);
	calculaTotalServico();
	
}
function addObraOS(empid)
{
	$("#tr_empid_" + empid).show();
	$("#empid_" + empid).attr("checked",true);
	calculaTotalServico();
}

function historicoProrrogacao( sosid ){
	window.location.href = 'obras2.php?modulo=principal/visualizacaoHistoricoProrrogacaoOS&acao=A&sosid='+sosid+'&l=1';
}
</script>
