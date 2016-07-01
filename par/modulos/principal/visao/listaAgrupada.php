<?php 
// FILTROS
// if($_REQUEST['enviaParaProximaFase']){
// 	$docids = $_REQUEST['docids'];
// 	foreach($docids as $docid){
// 		wf_alterarEstado( aedid, docid, esdid, acao );
// 	}
// }

// PERFIL
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if( in_array( PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) ){
	$tiposSubVisiveis = array(6, 13, 5); // id das formas de execução
}else if (in_array(PAR_PERFIL_EQUIPE_TECNICA, $perfil)){
	$tiposSubVisiveis = array(2, 4, 12); // id das formas de execução
}else if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){
	$tiposSubVisiveis = array(1,6,11,13,2,12,4,5); // id das formas de execução
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/external/jquery.bgiframe-2.1.1.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.core.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.widget.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.mouse.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.button.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.draggable.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.position.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.resizable.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.ui.dialog.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/development-bundle/ui/jquery.effects.core.js"></script>
<link rel="stylesheet" href="../par/css/listaAgrupada.css" type="text/css" media="all" />

<?php 
if($_REQUEST['visualizacaoListaAgrp'] == "fe" || !isset($_REQUEST['visualizacaoListaAgrp'])){ //FORMA DE EXECUÇÃO
	$sql = "SELECT frmid, frmdsc FROM par.formaexecucao ORDER BY frmdsc;";
	$formaExecucoes = $db->carregar($sql);
	
	$itrid = $_SESSION['par']['itrid'];
	
	$obSubacoes = new Dimensao();
	$select = " sbdparecer, ssuid, ";
	$innerInterno = "LEFT JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = date_part('year', now())";
	$listaSubacoes = $obSubacoes->lista('array',$itrid, null, null,$innerInterno);
	
	$arrSubacoesPAC 			= array();
	$arrSubacoesAssTec 			= array();
	$arrSubacoesExePeloMunic 	= array();
	$arrSubacoesFinancBNDES 	= array();
	$arrSubacoesAssFinanceira 	= array();
	$arrSubacoesAssFinancObras 	= array();
	$arrSubacoesExePeloEstado 	= array();
	$arrSubacoesAssFinancProgramada = array();
	$arrSubacoesAssFinancEmendasObras = array();
	$arrSubacoesAssFinanceiraEmendas = array();
	$arrSubacoesAssFinancBrasilPro = array();
	
	$contSubacoesPAC 			= 0;
	$contSubacoesAssTec 		= 0;
	$contSubacoesExePeloMunic 	= 0;
	$contSubacoesFinancBNDES 	= 0;
	$contSubacoesAssFinanceira 	= 0;
	$contSubacoesAssFinancObras = 0;
	$contSubacoesExePeloEstado 	= 0;
	$contSubacoesAssFinancProgramada = 0;
	$contSubacoesAssFinanceiraEmendas = 0;
	$contSubacoesAssFinancEmendasObras = 0;
	$contSubacoesAssFinancBrasilPro = 0;
	$contAnalisadas1 = 0;
	$contNaoAnalisadas1 = 0;
	$contAnalisadas2 = 0;
	$contNaoAnalisadas2 = 0;
	$contAnalisadas3 = 0;
	$contNaoAnalisadas3 = 0;
	$contAnalisadas4 = 0;
	$contNaoAnalisadas4 = 0;
	$contAnalisadas5 = 0;
	$contNaoAnalisadas5 = 0;
	$contAnalisadas6 = 0;
	$contNaoAnalisadas6 = 0;
	$contAnalisadas7 = 0;
	$contNaoAnalisadas7 = 0;
	$contAnalisadas8 = 0;
	$contNaoAnalisadas8 = 0;
	$contAnalisadas9 = 0;
	$contNaoAnalisadas9 = 0;
	$contAnalisadas10 = 0;
	$contNaoAnalisadas10 = 0;
	$contAnalisadas11 = 0;
	$contNaoAnalisadas11 = 0;
	
	if( is_array($listaSubacoes) ){
		foreach($listaSubacoes as $chave => $subacoes){
			
			switch ($subacoes['frmid']) {
			    case 1: //Ações do PAC 
			        $arrSubacoesPAC[$contSubacoesPAC]['acao'] 			= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesPAC[$contSubacoesPAC]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesPAC[$contSubacoesPAC]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
					if($subacoes['sbareformulacao'] == 't'){
				        $arrSubacoesPAC[$contSubacoesPAC]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_1 total" value="'.$subacoes['sbaid'].'"/>';
					}else{
				        $arrSubacoesPAC[$contSubacoesPAC]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_1 total" value="'.$subacoes['sbaid'].'"/>';
					}
			        $contSubacoesPAC++;
			        
					if($subacoes['sbdparecer'] != NULL && $subacoes['ssuid'] != NULL){
						$contAnalisadas1++;
					}else{
						$contNaoAnalisadas1++;
					}
			        
			        break;
			    case 2: //Assistência técnica do MEC
			        $arrSubacoesAssTec[$contSubacoesAssTec]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssTec[$contSubacoesAssTec]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssTec[$contSubacoesAssTec]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssTec[$contSubacoesAssTec]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_2 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesAssTec[$contSubacoesAssTec]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_2 total" value="'.$subacoes['sbaid'].'"/>';
			        	
			        }
			        $contSubacoesAssTec++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas2++;
					}else{
						$contNaoAnalisadas2++;
					}
			        
			        break;
			    case 4: //Executada pelo município
			        $arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			         if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_4 total" value="'.$subacoes['sbaid'].'"/>';
			         }else{
			        	$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_4 total" value="'.$subacoes['sbaid'].'"/>';
			         	
			         }
			        $contSubacoesExePeloMunic++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas3++;
					}else{
						$contNaoAnalisadas3++;
					}
			        
			        break;
			    case 5: //Financiamento BNDES
			        $arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['acao'] 			= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_5 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_5 total" value="'.$subacoes['sbaid'].'"/>';
			        	
			        }
			        $contSubacoesFinancBNDES++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas4++;
					}else{
						$contNaoAnalisadas4++;
					}
					
			        break;
			    case 6: //Assistência financeira do MEC
			        $arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['acao'] 		 = //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
				        $arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['estado'] 	 = 'Reformulação'.'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
				        $arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $contSubacoesAssFinanceira++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas5++;
					}else{
						$contNaoAnalisadas5++;
					}
			        
			        break;
			    case 14: //Assistência financeira do MEC - Emendas
			        $arrSubacoesAssFinanceiraEmendas[$contSubacoesAssFinanceiraEmendas]['acao'] 		 = //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinanceiraEmendas[$contSubacoesAssFinanceiraEmendas]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinanceiraEmendas[$contSubacoesAssFinanceiraEmendas]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
			         if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssFinanceiraEmendas[$contSubacoesAssFinanceiraEmendas]['estado'] 	 = 'Reformulação'.'<input type="hidden" class="totais_14 total" value="'.$subacoes['sbaid'].'"/>';
			         }else{
			        	$arrSubacoesAssFinanceiraEmendas[$contSubacoesAssFinanceiraEmendas]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_14 total" value="'.$subacoes['sbaid'].'"/>';
			         }
			        $contSubacoesAssFinanceiraEmendas++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas5++;
					}else{
						$contNaoAnalisadas5++;
					}
			        
			        break;
			    case 15: //Assistência financeira do MEC - Obras - Emendas
			        $arrSubacoesAssFinancEmendasObras[$contSubacoesAssFinancEmendasObras]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinancEmendasObras[$contSubacoesAssFinancEmendasObras]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinancEmendasObras[$contSubacoesAssFinancEmendasObras]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssFinancEmendasObras[$contSubacoesAssFinancEmendasObras]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesAssFinancEmendasObras[$contSubacoesAssFinancEmendasObras]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $contSubacoesAssFinancEmendasObras++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas9++;
					}else{
						$contNaoAnalisadas9++;
					}
			        
			        break;
			    case 11: //Assistência financeira do MEC - Obras
			        $arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $contSubacoesAssFinancObras++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas10++;
					}else{
						$contNaoAnalisadas10++;
					}
			        
			        break;
			    case 12: //Executada pelo estado
			        $arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['acao'] 			= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $contSubacoesExePeloEstado++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas7++;
					}else{
						$contNaoAnalisadas7++;
					}
			        
			        break;
			        
			     case 13: //Assistência financeira do MEC - PROGRAMADA
			        $arrSubacoesAssFinancProgramada[$contSubacoesAssFinancProgramada]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinancProgramada[$contSubacoesAssFinancProgramada]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinancProgramada[$contSubacoesAssFinancProgramada]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssFinancProgramada[$contSubacoesAssFinancProgramada]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_13 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesAssFinancProgramada[$contSubacoesAssFinancProgramada]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_13 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $arrSubacoesAssFinancProgramada++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas8++;
					}else{
						$contNaoAnalisadas8++;
					}
			        
			        break;
			        
			     case 16: //Assistência financeira do MEC - Brasil Profissionalizado
			        $arrSubacoesAssFinancBrasilPro[$contSubacoesAssFinancBrasilPro]['acao'] 		= //'<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> 
			        	'<img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAssFinancBrasilPro[$contSubacoesAssFinancBrasilPro]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAssFinancBrasilPro[$contSubacoesAssFinancBrasilPro]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        if($subacoes['sbareformulacao'] == 't'){
			        	$arrSubacoesAssFinancBrasilPro[$contSubacoesAssFinancBrasilPro]['estado'] 		= 'Reformulação'.'<input type="hidden" class="totais_13 total" value="'.$subacoes['sbaid'].'"/>';
			        }else{
			        	$arrSubacoesAssFinancBrasilPro[$contSubacoesAssFinancBrasilPro]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_13 total" value="'.$subacoes['sbaid'].'"/>';
			        }
			        $arrSubacoesAssFinancBrasilPro++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas11++;
					}else{
						$contNaoAnalisadas11++;
					}
			        
			        break;		  		        
			}
		}
	}
	
	$coresFundo 	= array('#C4C4C4', '#CDAF95', '#CDB38B', '#CDC9A5', '#CDC8B1', '#CDCDC1', '#C1CDC1', '#CDC1C5'  );
	$coresLetras 	= array('#333333', '#333333', '#333333', '#333333', '#333333', '#333333', '#333333', '#333333' );
	$ocultar['historico'] = true;
	
	
	function wf_desenhaBarraNavegacaoPar($frmid){
	?>
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
	<tr style="background-color: #c9c9c9; text-align: center;">
	<td style="font-size: 7pt; text-align: center;">
			<span title="estado atual"> <b>ações</b> </span>
		</td>
	</tr>
	<tr>
		<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;"
	onmouseover="this.style.backgroundColor='#ffffdd';"
	onmouseout="this.style.backgroundColor='';">
	<button id="btnEnviaWF<?php echo $frmid; ?>">Tramitar subações</button>
					
			</td>
		</tr>
	</table>
	
	<?php } ?>
	
	<div id="pai" class="pai">
			<?php 
	$script = '';
	
	if(is_array($formaExecucoes)){
		foreach($formaExecucoes as $chave=> $formaexecucao){
	
			$script .= " $('#{$formaexecucao['frmid']}').css('height', '');
						 $('#loader-container{$formaexecucao['frmid']}').hide();
						 
						 $( \"#btnEnviaWF{$formaexecucao['frmid']}\" )
							.button()
							.click(function() {
								wf_alterarEstadoPAR();
								$( \"#envioWF\" ).dialog( \"open\" );
							});
			";
			
			if($formaexecucao['frmid'] == 4 ||  $formaexecucao['frmid'] == 12){
				$titulo = "Subações ";
			}else{
				$titulo = "Subações de " ;
			}
			
			switch ($formaexecucao['frmid']) {
	    		case 1: //Ações do PAC 
			        $listaSubacoes = $arrSubacoesPAC;
			        $contAnalisadas = $contAnalisadas1;
			        $contNaoAnalisadas = $contNaoAnalisadas1;
			        break;
			    case 2: //Assistência técnica do MEC
			        $listaSubacoes = $arrSubacoesAssTec;
			        $contAnalisadas = $contAnalisadas2;
			        $contNaoAnalisadas = $contNaoAnalisadas2;
			        break;
			    case 4: //Executada pelo município
			        $listaSubacoes = $arrSubacoesExePeloMunic;
			        $contAnalisadas = $contAnalisadas3;
			        $contNaoAnalisadas = $contNaoAnalisadas3;
			        break;
			    case 5: //Financiamento BNDES
			        $listaSubacoes = $arrSubacoesFinancBNDES;
			        $contAnalisadas = $contAnalisadas4;
			        $contNaoAnalisadas = $contNaoAnalisadas4;
			        break;
			    case 6: //Assistência financeira do MEC
			        $listaSubacoes = $arrSubacoesAssFinanceira;
			        $contAnalisadas = $contAnalisadas5;
			        $contNaoAnalisadas = $contNaoAnalisadas5;
			        break;
			    case 11: //Assistência financeira do MEC - Obras
			        $listaSubacoes = $arrSubacoesAssFinancObras;
			        $contAnalisadas = $contAnalisadas6;
			        $contNaoAnalisadas = $contNaoAnalisadas6;
			        break;
			    case 12: //Executada pelo estado
			        $listaSubacoes = $arrSubacoesExePeloEstado;
			        $contAnalisadas = $contAnalisadas7;
			        $contNaoAnalisadas = $contNaoAnalisadas7;
			        break;
			    case 13: //Assistência financeira do MEC - Programada
			        $listaSubacoes = $arrSubacoesAssFinancProgramada;
			        $contAnalisadas = $contAnalisadas8;
			        $contNaoAnalisadas = $contNaoAnalisadas8;
			        break;
			   case 14: //Assistência financeira do MEC - Emendas
			        $listaSubacoes = $arrSubacoesAssFinanceiraEmendas;
			        $contAnalisadas = $contAnalisadas9;
			        $contNaoAnalisadas = $contNaoAnalisadas9;
			        break;
			   case 15: //Assistência financeira do MEC - Emendas Obras
			        $listaSubacoes = $arrSubacoesAssFinancEmendasObras;
			        $contAnalisadas = $contAnalisadas10;
			        $contNaoAnalisadas = $contNaoAnalisadas10;
			        break;
			   case 16: //Assistência financeira do MEC - Brasil Profissionalizado
			        $listaSubacoes = $arrSubacoesAssFinancBrasilPro;
			        $contAnalisadas = $contAnalisadas11;
			        $contNaoAnalisadas = $contNaoAnalisadas11;
			        break;
			}
						
		?>
	<div style="margin-top: 5px">
		<div class="tituloListagem" style="background-color: <?php echo $coresFundo[$chave]; ?>;  color: <?php echo $coresLetras[$chave]; ?>;   " >
			<table border="0" width="100%">
				<tr>
					<td style="padding-left: 20px; font-weight: bold; text-transform:capitalize;"><?php echo $titulo.$formaexecucao['frmdsc']; ?></td>
					<td style="text-align: right">Analisadas:<?php echo $contAnalisadas; ?> &nbsp; | &nbsp;  Pendentes: <?php echo $contNaoAnalisadas; ?></td>
				</tr>
			</table>
		</div>
	</div>
	<div id="<? echo $formaexecucao['frmid']; ?>" class="areaConteudo">
			<table class="Tabela">
				<tr>
					<td colspan="2">
						<table class="Tabela">
							<tr>
								<td class="SubTituloDireita labelFiltro">
								Filtros:
								</td>
								<td class="SubTituloEsquerda">
									<?php 
										if($formaexecucao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA){ // financeira  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA.'" src="../imagens/mais.gif" >&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									 <label>Gerado Convênio:	<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_CONVENIADA; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									 <label>Com itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_COMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Sem itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_SEMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_ASSITENCIA_TECNICA){ // técnica 
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_ASSITENCIA_TECNICA.'" src="../imagens/mais.gif">&nbsp;';
									?> 
									 <label>Analisadas:					<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Pendente de análise:		<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Gerado Termo de cooperação:	<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_GERADOTERMO; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS){ // obras  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO){ // executada pelo município  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Com itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_COMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Sem itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_SEMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_ACAO_PAC){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_ACAO_PAC.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == FORMA_EXECUCAO_FINANCIAMENTO_BNDES){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.FORMA_EXECUCAO_FINANCIAMENTO_BNDES.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									 <label>Gerado Convênio:	<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_CONVENIADA; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									 <label>Com itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_COMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
								 	 <label>Sem itens:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_SEMITENS; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ASSISTENCIA_FINANCEIRA_EMENDA.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
	
									<?php 
										}else if($formaexecucao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
									
									<?php 
										}else if($formaexecucao['frmid'] == ASSISTENCIA_FINANCEIRA_BRASIL_PRO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ASSISTENCIA_FINANCEIRA_BRASIL_PRO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $formaexecucao['frmid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $formaexecucao['frmid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $formaexecucao['frmid']; ?>"></label>
																																	
									<?php } ?>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr>
					<td id="lista<?php echo $formaexecucao['frmid']; ?>" class="tdlista">
						<div id="loader-container<? echo $formaexecucao['frmid']; ?>">
							<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
						</div>
						<div id="listaSubacoes<? echo $formaexecucao['frmid']; ?>" class="listas">
						<?php //$db->monta_lista_array($listaSubacoes,$cabecalho,10000,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); ?>
						<?php $db->monta_lista_simples($listaSubacoes,$cabecalho,10000,5,'N','95%', 'S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ); ?>
						
						</div>
 					</td> 
					<!--<td class="workflow" ><?php //wf_desenhaBarraNavegacaoPar( $formaexecucao['frmid'] );  ?></td>-->
				</tr>
			</table>
	</div>
	
	<?php } } //caixaDeTramitacao ?>
	</div>
	<div id="envioWF" title="Selecione as subações a serem enviadas.">
		<div id="listasubacoestramita">
		</div>
	</div>
	
	
	
	<script type="text/javascript">
	
	function listarSubacao(sbaid){
		var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
		janela(local,800,600,"Subação");
	}
	
	jQuery(document).ready(function($) {
			// Poupop
			function wf_alterarEstadoPAR()
			{	
				var sbaid 	= new Array();
				var cont 	= 0;
				var marcado = false;

				$("input[id*='subacoesCheck_']").each(function()
				{
				    if( $(this).is(':checked'))
				    {
				    	sbaid[cont] = $(this).val();
						cont = cont + 1;
						marcado = true;
	
				    }
				});
				
				if(marcado){
					$( "#envioWF" ).dialog({
						autoOpen: false,
						height: 600,
						width: 600,
						modal: true
					});
									
					 $.ajax({
					    type: "POST",
					   	url: "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada&visualizacaoListaAgrp=fe",
					   	async: false,
					   	data: "tramita=1&sbaid="+sbaid,
					   	success: function(msg){
					    	$('#listasubacoestramita').html(msg);
					   	}
					 }); 
					 
				 }else{
				 	alert("Não existe subações selecionadas para ser tramitadas.");
				    return false;	
				 }
			}
			
			// SCRIPT PARA DEIXAR OS QUADROS DINÁMICO
			$(function() {
				$( "#pai" ).accordion({
					collapsible: true,
					clearStyle: true,
					change: function(event, ui) { 
						$("input[id*='subacoesCheck_']").each(function()
						{
							if( $(this).is(':checked'))
				    		{
								$(this).attr("checked",false);
								
							}
								
						});
					 }
					
				});
			});
	
			<?php echo $script; ?>
		
		//FILTROS
	$('.checkboxFiltros').click(function()
	{ 	
		if( $(this).is(":checked") )
		{
			$('#listaSubacoes'+$(this).val()).hide();
			$('#loader-container'+$(this).val()).show();
			
			switch ($(this).attr('id')) {
				case $(this).val()+<?php echo SUBACOES_ANALISADAS;?>: // Analisada 
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_ANALISADAS;?>;	
				break;
				case $(this).val()+<?php echo SUBACOES_PENDENTESDEANALISE;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_PENDENTESDEANALISE;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_GERADOTERMO;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_GERADOTERMO;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_CONVENIADA;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_CONVENIADA;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_COMITENS;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_COMITENS;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_SEMITENS;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_SEMITENS;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_COM_OBRAS;?>:
					var parametros = "filtros=1&frmid="+$(this).val()+"&filtro="+<?php echo SUBACOES_COM_OBRAS;?>;		
				break;	
				}
			}else{
				var parametros = "filtros=1&frmid="+$(this).val()+"&filtro=";		
			}
			var formaexecucao = $(this).val();
			$.ajax({
			    type: "POST",
			   	url: "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada&visualizacaoListaAgrp=fe",
			   	async: false,
			   	data: parametros,
			   	success: function(resposta){
			   		$('#listaSubacoes'+formaexecucao).show();
			   		//$('#lista'+a).css('width', $('#listaSubacoes'+a).css('height'));
			    	$('#listaSubacoes'+formaexecucao).html(resposta);
			    	$('#loader-container'+formaexecucao).hide();
			   	}
			 }); // fim ajax
			
		}); // fim função
	
		$('.totaisAno').live('click',function(){
			var html;
			var frmid = $(this).attr('id');
			var larg = '8%';
			html = '<td width="'+larg+'" class="2011_'+frmid+' total_'+frmid+'" id="titulo2011" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2011</b></td>'+
				   '<td width="'+larg+'" class="2012_'+frmid+' total_'+frmid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2012</b></td>'+
				   '<td width="'+larg+'" class="2013_'+frmid+' total_'+frmid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2013</b></td>'+
				   '<td width="'+larg+'" class="2014_'+frmid+' total_'+frmid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2014</b></td>';
			
			if($(this).attr('src') == '../imagens/mais.gif'){
				if( $(this).parent().parent().parent().find('td').size() != 4 ){
					$('.total_'+frmid).show();
				}else{
					$(this).parent().after(html);
					var html2;
					$('.totais_'+frmid).each(function(){
						 $.ajax({
							    type: "POST",
							   	url: window.location,
							   	async: false,
							   	data: '&reqAjax=htmlValorSubacao&frmid='+frmid+'&sbaid='+$(this).val(),
							   	success: function(msg){
							 		html2 = msg;
							   	}
						}); 
						$(this).parent().after(html2);
					});
				}
				$(this).attr('src','../imagens/menos.gif');
			}else{
				$('.total_'+frmid).hide();
				$(this).attr('src','../imagens/mais.gif');
			}
		});
	});
	</script>

<?php } else { //FASE WORKFLOW
	
	$sql = "SELECT esdid, esddsc FROM workflow.estadodocumento WHERE tpdid = 62 ORDER BY esddsc";
	$fasesWorkflow = $db->carregar($sql);

	$itrid = $_SESSION['par']['itrid'];
	
	$obSubacoes = new Dimensao();
	$select = " sbdparecer, ssuid, ";
	$innerInterno = "LEFT JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = date_part('year', now())";
	$listaSubacoes = $obSubacoes->lista('array',$itrid, null, null,$innerInterno);
	
	$arrSubacoesElaboracao 			= array();
	$arrSubacoesAnalise 			= array();
	$arrSubacoesGerTermoCooperacao 	= array();
	$arrSubacoesDiligencia		 	= array();
	$arrSubacoesAgAnaliseGestor 	= array();
	$arrSubacoesEmpenho			 	= array();
	$arrSubacoesPagamento		 	= array();
	$arrSubacoesIndeferida			= array();
	$arrSubacoesEmAnaliseCom 		= array();
	
	$contSubacoesElaboracao = 0;
	$contSubacoesAnalise = 0;
	$contSubacoesGerTermoCooperacao = 0;
	$contSubacoesDiligencia = 0;
	$contSubacoesAgAnaliseGestor = 0;
	$contSubacoesEmpenho = 0;
	$contSubacoesPagamento = 0;
	$contSubacoesIndeferida = 0;
	$contSubacoesEmAnaliseCom 	= 0;	

	$contAnalisadas1 = 0;
	$contNaoAnalisadas1 = 0;
	$contAnalisadas2 = 0;
	$contNaoAnalisadas2 = 0;
	$contAnalisadas3 = 0;
	$contNaoAnalisadas3 = 0;
	$contAnalisadas4 = 0;
	$contNaoAnalisadas4 = 0;
	$contAnalisadas5 = 0;
	$contNaoAnalisadas5 = 0;
	$contAnalisadas6 = 0;
	$contNaoAnalisadas6 = 0;
	$contAnalisadas7 = 0;
	$contNaoAnalisadas7 = 0;
	$contAnalisadas8 = 0;
	$contNaoAnalisadas8 = 0;
	$contAnalisadas9 = 0;
	$contNaoAnalisadas9 = 0;
	
	if( is_array($listaSubacoes) ){
		foreach($listaSubacoes as $chave => $subacoes){
			switch ($subacoes['esdid']) {
			    case 451: //Elaboração
			        $arrSubacoesElaboracao[$contSubacoesElaboracao]['acao'] 		= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesElaboracao[$contSubacoesElaboracao]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesElaboracao[$contSubacoesElaboracao]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesElaboracao[$contSubacoesElaboracao]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_1 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesElaboracao++;
			        
					if($subacoes['sbdparecer'] != NULL && $subacoes['ssuid'] != NULL){
						$contAnalisadas1++;
					}else{
						$contNaoAnalisadas1++;
					}
			        
			        break;
			    case 452: //Análise
			        $arrSubacoesAnalise[$contSubacoesAnalise]['acao'] 			= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAnalise[$contSubacoesAnalise]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAnalise[$contSubacoesAnalise]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesAnalise[$contSubacoesAnalise]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_2 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesAnalise++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas2++;
					}else{
						$contNaoAnalisadas2++;
					}
			        
			        break;
			    case 453: //Geração do Termo de Cooperação Técnica
			        $arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['acao'] 		= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_4 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesGerTermoCooperacao++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas3++;
					}else{
						$contNaoAnalisadas3++;
					}
			        
			        break;
			    case 454: //Diligência
			        $arrSubacoesDiligencia[$contSubacoesDiligencia]['acao'] 			= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesDiligencia[$contSubacoesDiligencia]['localizacao'] 		= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesDiligencia[$contSubacoesDiligencia]['subacao'] 			= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesDiligencia[$contSubacoesDiligencia]['estado'] 			= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_5 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesDiligencia++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas4++;
					}else{
						$contNaoAnalisadas4++;
					}
					
			        break;
			    case 455: //Aguardando Análise do Gestor
			        $arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['acao'] 		 = '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesAgAnaliseGestor++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas5++;
					}else{
						$contNaoAnalisadas5++;
					}
			        
			        break;
			    case 456: //Empenho
			        $arrSubacoesEmpenho[$contSubacoesEmpenho]['acao'] 		 	= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesEmpenho[$contSubacoesEmpenho]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesEmpenho[$contSubacoesEmpenho]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesEmpenho[$contSubacoesEmpenho]['estado'] 	 	= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesEmpenho++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas6++;
					}else{
						$contNaoAnalisadas6++;
					}
			        
			        break;
			    case 457: //Pagamento
			        $arrSubacoesPagamento[$contSubacoesPagamento]['acao'] 			= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesPagamento[$contSubacoesPagamento]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesPagamento[$contSubacoesPagamento]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesPagamento[$contSubacoesPagamento]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesPagamento++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas7++;
					}else{
						$contNaoAnalisadas7++;
					}
			        
			        break;
			    case 458: //Indeferida
			        $arrSubacoesIndeferida[$contSubacoesIndeferida]['acao'] 		= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesIndeferida[$contSubacoesIndeferida]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesIndeferida[$contSubacoesIndeferida]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesIndeferida[$contSubacoesIndeferida]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesIndeferida++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas8++;
					}else{
						$contNaoAnalisadas8++;
					}
			        
			        break;
			    case 462: //Em análise da comissão
			        $arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['acao'] 		= '<input id="subacoesCheck_'.$subacoes['sbaid'].'" value="'.$subacoes['sbaid'].'" type="checkbox"> <img onclick="javascript:listarSubacao('.$subacoes['sbaid'].');" src="../imagens/alterar.gif" >';
			        $arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
			        $arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['subacao'] 		= delimitadorTexto($subacoes['sbadsc']);
			        $arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
			        $contSubacoesEmAnaliseCom++;
			        
					if($subacoes['sbdparecer'] != '' && $subacoes['ssuid'] != ''){
						$contAnalisadas9++;
					}else{
						$contNaoAnalisadas9++;
					}
			        
			        break;
			}
		}
	}
	
	$coresFundo 	= array('#C4C4C4', '#CDAF95', '#CDB38B', '#CDC9A5', '#CDC8B1', '#CDCDC1', '#C1CDC1', '#CDC1C5'  );
	$coresLetras 	= array('#333333', '#333333', '#333333', '#333333', '#333333', '#333333', '#333333', '#333333' );
	$ocultar['historico'] = true;

	
	function wf_desenhaBarraNavegacaoPar($esdid){?>
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
	<tr style="background-color: #c9c9c9; text-align: center;">
	<td style="font-size: 7pt; text-align: center;">
			<span title="estado atual"> <b>ações</b> </span>
		</td>
	</tr>
	<tr>
		<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;"
	onmouseover="this.style.backgroundColor='#ffffdd';"
	onmouseout="this.style.backgroundColor='';">
	<button id="btnEnviaWF<?php echo $esdid; ?>">Tramitar subações</button>
					
			</td>
		</tr>
	</table>
	
	<?php } ?>
	
	<div id="pai" class="pai">
			<?php 
	$script = '';
	
	if(is_array($fasesWorkflow)){
		foreach($fasesWorkflow as $chave=> $faseWorkflow){
	
			$script .= " $('#{$faseWorkflow['esdid']}').css('height', '');
						 $('#loader-container{$faseWorkflow['esdid']}').hide();
						 
						 $( \"#btnEnviaWF{$faseWorkflow['esdid']}\" )
							.button()
							.click(function() {
								wf_alterarEstadoPAR();
								$( \"#envioWF\" ).dialog( \"open\" );
							});";
			
			switch ($faseWorkflow['esdid']) {
	    		case 451: //Elaboração
			        $listaSubacoes = $arrSubacoesElaboracao;
			        $contAnalisadas = $contAnalisadas1;
			        $contNaoAnalisadas = $contNaoAnalisadas1;
			        break;
			    case 452: //Análise
			        $listaSubacoes = $arrSubacoesAnalise;
			        $contAnalisadas = $contAnalisadas2;
			        $contNaoAnalisadas = $contNaoAnalisadas2;
			        break;
			    case 453: //Geração do Termo de Cooperação Técnica
			        $listaSubacoes = $arrSubacoesGerTermoCooperacao;
			        $contAnalisadas = $contAnalisadas3;
			        $contNaoAnalisadas = $contNaoAnalisadas3;
			        break;
			    case 454: //Diligência
			        $listaSubacoes = $arrSubacoesDiligencia;
			        $contAnalisadas = $contAnalisadas4;
			        $contNaoAnalisadas = $contNaoAnalisadas4;
			        break;
			    case 455: //Aguardando Análise do Gestor
			        $listaSubacoes = $arrSubacoesAgAnaliseGestor;
			        $contAnalisadas = $contAnalisadas5;
			        $contNaoAnalisadas = $contNaoAnalisadas5;
			        break;
			    case 456: //Empenho
			        $listaSubacoes = $arrSubacoesEmpenho;
			        $contAnalisadas = $contAnalisadas6;
			        $contNaoAnalisadas = $contNaoAnalisadas6;
			        break;
			    case 457: //Pagamento
			        $listaSubacoes = $arrSubacoesPagamento;
			        $contAnalisadas = $contAnalisadas7;
			        $contNaoAnalisadas = $contNaoAnalisadas7;
			        break;
			    case 458: //Indeferida
			        $listaSubacoes = $arrSubacoesIndeferida;
			        $contAnalisadas = $contAnalisadas8;
			        $contNaoAnalisadas = $contNaoAnalisadas8;
			        break;
			   case 462: //Em análise da comissão
			        $listaSubacoes = $arrSubacoesEmAnaliseCom;
			        $contAnalisadas = $contAnalisadas9;
			        $contNaoAnalisadas = $contNaoAnalisadas9;
			        break;
			}
						
		?>
	<div style="margin-top: 5px">
		<div class="tituloListagem" style="background-color: <?php echo $coresFundo[$chave]; ?>;  color: <?php echo $coresLetras[$chave]; ?>;   " >
			<table border="0" width="100%">
				<tr>
					<td style="padding-left: 20px; font-weight: bold; text-transform:capitalize;"><?php echo $faseWorkflow['esddsc']; ?></td>
					<td style="text-align: right">Analisadas:<?php echo $contAnalisadas; ?> &nbsp; | &nbsp;  Pendentes: <?php echo $contNaoAnalisadas; ?></td>
				</tr>
			</table>
		</div>
	</div>
	<div id="<? echo $faseWorkflow['esdid']; ?>" class="areaConteudo">
			<table class="Tabela">
				<tr>
					<td colspan="2">
						<table class="Tabela">
							<tr>
								<td class="SubTituloDireita labelFiltro">
								Filtros:
								</td>
								<td class="SubTituloEsquerda">
									<?php 
										if($faseWorkflow['esdid'] == AGUARDANDO_ANALISE_GESTOR){ // financeira  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.AGUARDANDO_ANALISE_GESTOR.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == ANALISE){ // técnica 
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ANALISE.'" src="../imagens/mais.gif">&nbsp;';
									?> 
									 <label>Analisadas:					<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
								 	 <label>Pendente de análise:		<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == DILIGENCIA){ // obras  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.DILIGENCIA.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
								 	 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
								 	 
									<?php 
										}else if($faseWorkflow['esdid'] == ELABORACAO){ // executada pelo município  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.ELABORACAO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
								 	 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == EM_ANALISE_COMISSAO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.EM_ANALISE_COMISSAO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == EMPENHO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.EMPENHO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == GERACAO_TERMO_COOPERACAO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.GERACAO_TERMO_COOPERACAO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == INDEFERIDA){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.INDEFERIDA.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
									
									<?php 
										}else if($faseWorkflow['esdid'] == PAGAMENTO){  
										$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
										$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="'.PAGAMENTO.'" src="../imagens/mais.gif">&nbsp;';
									?>
									
									 <label>Analisadas:			<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_ANALISADAS; ?>" value="<?php echo $faseWorkflow['esdid']; ?>" ></label>
									 <label>Pendente de análise:<input class="checkboxFiltros" type="radio" name="radio" style="vertical-align: middle;" id="<?php echo $faseWorkflow['esdid'].SUBACOES_PENDENTESDEANALISE; ?>" value="<?php echo $faseWorkflow['esdid']; ?>"></label>
																																	
									<?php } ?>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr>
					<td id="lista<?php echo $faseWorkflow['esdid']; ?>" class="tdlista">
						<div id="loader-container<? echo $faseWorkflow['esdid']; ?>">
							<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
						</div>
						<div id="listaSubacoes<? echo $faseWorkflow['esdid']; ?>" class="listas">
						<?php $db->monta_lista_array($listaSubacoes,$cabecalho,10000,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); ?>
						</div>
					</td>
					<td class="workflow" ><?php wf_desenhaBarraNavegacaoPar( $faseWorkflow['esdid'] );  ?></td>
				</tr>
			</table>
	</div>
	
	<?php } } //caixaDeTramitacao ?>
	</div>
	<div id="envioWF" title="Selecione as subações a serem enviadas.">
		<div id="listasubacoestramita"></div>
	</div>
	
	<script type="text/javascript">
	function listarSubacao(sbaid){
		var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
		janela(local,800,600,"Subação");
	}
	
	jQuery(document).ready(function($) {
			// Poupop
			function wf_alterarEstadoPAR()
			{	
				var sbaid 	= new Array();
				var cont 	= 0;
				var marcado = false;
				
				$("input[id*='subacoesCheck_']").each(function()
				{
				    if( $(this).is(':checked'))
				    {
				    	sbaid[cont] = $(this).val();
						cont = cont + 1;
						marcado = true;
	
				    }
				});

				if(marcado){
					$( "#envioWF" ).dialog({
						autoOpen: false,
						height: 600,
						width: 600,
						modal: true
					});
									
					 $.ajax({
					    type: "POST",
					   	url: "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada&visualizacaoListaAgrp=wf",
					   	async: false,
					   	data: "tramita=1&sbaid="+sbaid,
					   	success: function(msg){
					    	$('#listasubacoestramita').html(msg);
					   	}
					 }); 
					 
				 }else{
				 	alert("Não existe subações selecionadas para ser tramitadas.");
				    return false;	
				 }
			}
			
			// SCRIPT PARA DEIXAR OS QUADROS DINÁMICO
			$(function() {
				$( "#pai" ).accordion({
					collapsible: true,
					clearStyle: true,
					change: function(event, ui) { 
						$("input[id*='subacoesCheck_']").each(function()
						{
							if( $(this).is(':checked'))
				    		{
								$(this).attr("checked",false);
								
							}
								
						});
					 }
					
				});
			});
	
			<?php echo $script; ?>
		
		//FILTROS
	$('.checkboxFiltros').click(function()
	{ 	
		if( $(this).is(":checked") )
		{
			$('#listaSubacoes'+$(this).val()).hide();
			$('#loader-container'+$(this).val()).show();
		
			switch ($(this).attr('id')) {
				case $(this).val()+<?php echo SUBACOES_ANALISADAS;?>: // Analisada 
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_ANALISADAS;?>;	
				break;
				case $(this).val()+<?php echo SUBACOES_PENDENTESDEANALISE;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_PENDENTESDEANALISE;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_GERADOTERMO;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_GERADOTERMO;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_CONVENIADA;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_CONVENIADA;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_COMITENS;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_COMITENS;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_SEMITENS;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_SEMITENS;?>;		
				break;
				case $(this).val()+<?php echo SUBACOES_COM_OBRAS;?>:
					var parametros = "filtros=1&esdid="+$(this).val()+"&filtro="+<?php echo SUBACOES_COM_OBRAS;?>;		
				break;	
				}
			}else{
				var parametros = "filtros=1&esdid="+$(this).val()+"&filtro=";		
			}
			var faseWorkflow = $(this).val();
			$.ajax({
			    type: "POST",
			   	url: "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada&visualizacaoListaAgrp=wf",
			   	async: false,
			   	data: parametros,
			   	success: function(resposta){
			   		$('#listaSubacoes'+faseWorkflow).show();
			   		//$('#lista'+a).css('width', $('#listaSubacoes'+a).css('height'));
			    	$('#listaSubacoes'+faseWorkflow).html(resposta);
			    	$('#loader-container'+faseWorkflow).hide();
			   	}
			 }); // fim ajax
			
		}); // fim função
	
		$('.totaisAno').live('click',function(){
			var html;
			var esdid = $(this).attr('id');
			var larg = '8%';
			html = '<td width="'+larg+'" class="2011_'+esdid+' total_'+esdid+'" id="titulo2011" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2011</b></td>'+
				   '<td width="'+larg+'" class="2012_'+esdid+' total_'+esdid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2012</b></td>'+
				   '<td width="'+larg+'" class="2013_'+esdid+' total_'+esdid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2013</b></td>'+
				   '<td width="'+larg+'" class="2014_'+esdid+' total_'+esdid+'" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" style="border-right: 1px solid #c0c0c0; '+ 
				   'border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><b>2014</b></td>';
			
			if($(this).attr('src') == '../imagens/mais.gif'){
				if( $(this).parent().parent().parent().find('td').size() != 4 ){
					$('.total_'+esdid).show();
				}else{
					$(this).parent().parent().after(html);
					var html2;
					$('.totais_'+esdid).each(function(){
						 $.ajax({
							    type: "POST",
							   	url: window.location,
							   	async: false,
							   	data: '&reqAjax=htmlValorSubacao&esdid='+esdid+'&sbaid='+$(this).val(),
							   	success: function(msg){
							 		html2 = msg;
							   	}
						}); 
						$(this).parent().after(html2);
					});
				}
				$(this).attr('src','../imagens/menos.gif');
			}else{
				$('.total_'+esdid).hide();
				$(this).attr('src','../imagens/mais.gif');
			}
		});
	});
	</script>
<?php } ?>