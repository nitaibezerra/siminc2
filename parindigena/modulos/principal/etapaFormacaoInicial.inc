<?php 
if($_REQUEST['req'] == 'polos'){
	$id = $_REQUEST['id'];
	$fieid = $_REQUEST['fieid'] != '' ? $_REQUEST['fieid'] : '99'.$_REQUEST['linha'];
	
	$sql = "SELECT 
				locid as codigo,
				locnome as descricao
			FROM parindigena.localpolo ";
                    
	$sqlLocid = "SELECT 
					ep.locid as codigo,
					lp.locnome as descricao
				FROM parindigena.etapapolo ep
				INNER JOIN parindigena.localpolo lp ON lp.locid = ep.locid 
				WHERE etpidetapa = ".$fieid;
	if( $_REQUEST['friid'] != '' ){
		$$fieid    = $db->carregar($sqlLocid);
	}
                    
	echo combo_popup( $fieid , $sql, "Selecione o(s) Povo(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
	die();
}

if ($_REQUEST['friid']) 
{
	 $sql = "SELECT
                *  
            FROM
                parindigena.formacaoinicial AS fi
            INNER JOIN parindigena.itemmonitoramento AS m ON m.friid = fi.friid
            WHERE
                fi.friid = {$_REQUEST['friid']}
                and m.itmstatus = 'A'";               

    $rsDadosForm 	   = $db->carregar( $sql );
    $estuf 		 	   = $rsDadosForm[0]['estuf'];
    $_REQUEST['estuf'] = $rsDadosForm[0]['estuf'];  
    
	$sql = "	SELECT apmp.sbaid,  coalesce(pss.pssano,0) as pssano
				FROM parindigena.associativaplanodemetasprogramas apmp
				INNER JOIN cte.projetosapesubacao pss on apmp.sbaid = pss.sbaid 
				WHERE appid = ".$_REQUEST['friid']." and apptipo = 1 ";
	
	$sub = $db->carregar( $sql );
	
	if($sub){
		
		$valoreTotalExecutado = 0;
		
		foreach($sub as $dados){
			
			$sbaids[] 	= $dados['sbaid']; 
			$pssanos[] 	= $dados['pssano'];
			
			$sql = "select sum(coalesce(hic.hmsvalortotalpago,0)) as total 
			from cte.historicomonitoramentoconvenio  hmc
			inner join  cte.historicomonitoramentoconvsubac hms on hmc.hmcid = hms.hmcid  
			inner join cte.historicoconvitemcomposicao 	hic on   hic.hmsid = hms.hmsid
			where hms.sbaid = ".$dados['sbaid']." and hic.hmsano = ".$dados['pssano'];
			
			$valoreTotalExecutado += $db->pegaUm( $sql );
		}
	}
}

$sql = "select
		 * 
		from 
			parindigena.itemmonitoramento as im 
			inner join parindigena.formacaoinicial as f 
		on f.friid = im.friid 
		 where 
			f.friid= {$_REQUEST['friid']} and 
			im.itmstatus = 'A'";          

$rsDadosForm = $db->carregar( $sql );
    
$res = array( array (  "descricao" => "Formação inicial",
						    "id" 		=> "0",
						    "link" 		=> "/parindigena/parindigena.php?modulo=principal/popupCadFormacaoInicial&acao=I&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
				  		  )
			);	  
				
if ($_REQUEST['friid']){
	$perfis = arrayPerfil(); 		  
	if( in_array( PARIND_ADMINISTRADOR, $perfis ) || in_array( PARIND_SUPER_USUARIO, $perfis ) ){	
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
					  		   ),
					array ("descricao" => "Vinculação a subações do PAR Genêrico",
							    "id"		=> "4",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/associativaPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   ),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}else{
		array_push($res,
					array ("descricao" => "Etapas",
							    "id"        => "1",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
					array ("descricao" => "Restrição",
							    "id"        => "2",
							    "link" 		=> "/parindigena/parindigena.php?modulo=principal/restricao&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
							   ),
		   			array ("descricao" => "Documentos",
							    "id"		=> "3",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/documento&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}"
					  		   ),
					array ("descricao" => "Subações do PAR - Plano de Metas",
							    "id"		=> "5",
							    "link"		=> "/parindigena/parindigena.php?modulo=principal/subacoesPAR&acao=A&friid={$rsDadosForm[0]['friid']}&estuf={$_REQUEST['estuf']}"
					  		   )
			  );
	}

}

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/parindigena/parindigena.php?modulo=principal/etapaFormacaoInicial&acao=A&friid={$_REQUEST['friid']}&estuf={$_REQUEST['estuf']}");

if( $_POST['submetido']){
	if (  $_REQUEST['fieid']){
//		dbg($_POST);
//		die();
            	 
		for( $k = 1; $k <= count( $_REQUEST['fieid'] ); $k++){

			$i = $k-1;
                	
			$arrDataI = explode( "/", $_REQUEST['fiedatainicial'.$k][0]);
			$formataDataI = $arrDataI[2].'-'.$arrDataI[1].'-'.$arrDataI[0];
                    
			$arrDataF = explode( "/", $_REQUEST['fiedatafinal'.$k][0]);
			$formataDataF = $arrDataF[2].'-'.$arrDataF[1].'-'.$arrDataF[0]; 
			if ($_REQUEST['fieid'][$i] == '' &&  $formataDataI != '--' &&  $formataDataF != '--' ){ 
				if ($_REQUEST['friid'] != '' ){
					$friid = $_REQUEST['friid'];
				} 
				$sql = " INSERT INTO
							parindigena.formacaoinicialetapa (
							fiedescricao,
							fiedatainicial,
							fiedatafinal,
							friid,
							locid,
							fiecustolicitado,
							fiecontrapartidaestado)
						VALUES ( 
							'{$_REQUEST['fiedescricao'.$k][0]}',       
							'{$formataDataI}',       
							'{$formataDataF}', 
							'$friid',
							NULL,
							'" . str_replace('.','', $_REQUEST['fiecustolicitado'.$k][0]). "',
							'" . str_replace('.','', $_REQUEST['fiecontrapartidaestado'.$k][0]). "')
						RETURNING fieid";
				
				$fieid = '99'.$k;
			}else{       
				if (  $formataDataI != '--' &&  $formataDataF != '--' ){
                                                        
					$sql = "UPDATE 
								parindigena.formacaoinicialetapa 
  							SET
								fiedescricao           = '{$_REQUEST['fiedescricao'.$k][0]}',
								fiedatainicial         = '{$formataDataI}',
								fiedatafinal           = '{$formataDataF}', 
								fiecustolicitado       = '" . str_replace('R$','', $_REQUEST['fiecustolicitado'.$k][0]). "',
								fiecontrapartidaestado = '" . str_replace('R$','', $_REQUEST['fiecontrapartidaestado'.$k][0]). "'
							WHERE
								fieid     =  '{$_REQUEST['fieid'][$i]}' 
							RETURNING fieid";
					
					$sqlLimaEtapa = "DELETE FROM parindigena.etapapolo WHERE etpidetapa = ".$_REQUEST['fieid'][$i];
					$db->executar($sqlLimaEtapa);

					$fieid = $_REQUEST['fieid'][$i];
				}
			}
			$sqlStatus = " UPDATE parindigena.formacaoinicial 
							SET
	                            fristatus       = 'A'
							WHERE                       
								friid            =  '{$_REQUEST['friid']}' "; 
			
			$fieidInsere = $db->pegaUm($sql);
			
			for( $p = 0; $p < count( $_REQUEST[$fieid] ); $p++){
				$sqlEtapas = "INSERT INTO parindigena.etapapolo (etpidetapa, locid, etptipo)
								VALUES (".$fieidInsere.",
										".$_REQUEST[$fieid][$p].",1)";
//				dbg($sqlEtapas);
				$db->executar( $sqlEtapas );
			}
    
			$db->executar( $sqlStatus );    
			$db->commit();
			 
			$dadosSalvos = true; 
		} 
//		die();
	}
	echo "<script type=\"text/javascript\">
			    alert('Operação realizada com sucesso!');
			    window.location.href = '?modulo=principal/etapaFormacaoInicial&acao=A&friid=".$_REQUEST['friid']."&estuf=".$_REQUEST['estuf']."'; 
		</script>"; 
}
if (array_key_exists('excluir', $_REQUEST)) 
{
    $sql = "DELETE FROM parindigena.formacaoinicialetapa WHERE fieid = {$_REQUEST['excluir']}";
    $sqlEtapas = "DELETE FROM parindigena.etapapolo WHERE etpidetapa =  {$_REQUEST['excluir']}";
    
    $db->executar($sqlEtapas);
    $deleteItemForm = $db->executar( $sql );
    $db->commit(); 
}

$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
$rs = $db->carregar( $sql );
$convenioAno = $rs[0]['covnumero']; 
$dadosConvenio = explode('/', $convenioAno);
$numConvenio = $dadosConvenio[0];
$ano = $dadosConvenio[1];
$_REQUEST['covcod'] = $numConvenio;

$sql = "SELECT	 
			si.sbaid,
			si.sbaporescola
		FROM parindigena.associativaplanodemetasprogramas app
		inner join cte.subacaoindicador 	si ON si.sbaid = app.sbaid
		WHERE app.appid = '{$_REQUEST['friid']}'
		";
		
$arSubacoes = $db->carregar($sql);		

if(is_array($arSubacoes)){
	
	$totalValorProgramado = 0;
	foreach($arSubacoes as $dados){
		$arSbaids[] = $dados['sbaid'];
		$arValores = calculoValorProgramado($dados['sbaid'], $ano, $dados['sbaporescola']);
		$totalValorProgramado += $arValores['cronograma']; 	
	}
}

$totalValorProgramado = $totalValorProgramado ? $totalValorProgramado : 0;

?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>
    
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript">
    
function getElementsByName_iefix(tag, name){ 
	
	var elem = document.getElementsByTagName(tag);  
	var arr = new Array();  
	for(i = 0,iarr = 0; i < elem.length; i++) {  
		att = elem[i].getAttribute("name");  
		if(att == name){  
			arr[iarr] = elem[i];  
			iarr++;  
		}  
	}  
	return arr;  
}
	
function atualizarTotalGeral( name, id ){	
	
	var tabela = document.getElementById( "itensEtapasCurso" );
	var nrLinha = tabela.rows.length-1;							
	var elementoTotal = document.getElementById( id );
	total = 0;		
	
	for( i=1; i<=nrLinha; i++ ){	

		campo = document.getElementById( name+i )
		valorTT = campo.value.replace(",","");			
		total += parseFloat( valorTT );						
	}
			
	mascaraTotal = mascaraglobal( '###.###.###,##', total );		
	elementoTotal.value = mascaraTotal;		
}
	
function pegaTotal(){
		
	/* Nome da tabela onde contem as linhas adicionadas */
	var tabela = document.getElementById( "itensEtapasCurso" );
	var totalLicitado = document.getElementById( "totlicitado" );
	var totalContra = document.getElementById( "totcontrapartida" );
				
	/* Número de linhas na tabele menos o cabeçalho */
	var nrLinha = tabela.rows.length;							
	
	for (i=1;i<nrLinha;i++){					      	
      	valorLT = 0;
      	valorCT = 0;
      	
      	for (j=1;j<nrLinha;j++){		      		  
			campoLT = document.getElementById( "fielicitado_"+j );
      		  
			if(campoLT.id == 'fielicitado_'+j){	              		              		              	
				campoLT.value = campoLT.value ? campoLT.value : 0;
				campoLTr = campoLT.value.replace(",","");
				valorLT += parseFloat( campoLTr );
//				mascaraValorLT = mascaraglobal( '###.###.###,##', valorLT );
//				totalLicitado.value = mascaraValorLT;		              
			}              
	              	      		
			campoCT = document.getElementById( "fiecontrapartida_"+j );
	              
			if(campoCT.id == 'fiecontrapartida_'+j){
					              		              		              	
				campoCT.value = campoCT.value ? campoCT.value : 0;
				campoCTr = campoCT.value.replace(",","");
				valorCT += parseFloat( campoCTr );
//				mascaraValorCT = mascaraglobal( '###.###.###,##', valorCT );						
//				totalContra.value = mascaraValorCT;													              	
			}		                            		            
		}			      
	}
	
	mascaraValorLT = mascaraglobal( '###.###.###,##', valorLT );
	totalLicitado.value = mascaraValorLT;
	mascaraValorCT = mascaraglobal( '###.###.###,##', valorCT );						
	totalContra.value = mascaraValorCT;						
}   
	
</script>    
</head>
<body onload="pegaTotal()">
<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">

<!-- Tabela 1 -->

<table  align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
	<tr style="background-color: #cccccc;">
		<td>
			<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastro de Formação Inicial</div>
			<div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
<!-- Tabela 2 -->
			<table  id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">           	
				<tr>            	
					<td colspan = "2" align="center">
						<input type="hidden" name="submetido" id="submetido" value=1 >
<!-- Tabela 3 -->						
							<table width = "100%" class="tabela listagem">
								<tr>
				                	<td colspan = "2" align="center">
<!-- Tabela 4 -->			                	
					                    <table width = "100%" class="tabela listagem">
					                    	<tr bgcolor="#ffffcc">
						                    	<td>
							                    	<? 
							                    	if( $_REQUEST['friid']){
							                    		echo verificaEtapa( $_REQUEST['friid']);
							                    	}
							                    	?>	
												</td>
											</tr>
											<tr>
												<td>
													<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
												</td>
											</tr>
											<tr>
												<td>                                
													<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Formação Inicial</span>
												</td>
											</tr>                        
											<?php                       
											if ( $_REQUEST['covcod'] != '' )
											{	                             
												$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$_REQUEST['covcod']}'";
												$rs = $db->carregar( $sql );
												$convenio = $rs[0]['covnumero'];	                        
											?>
											<tr>
												<td>
													<span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
												</td>
											</tr>
											<?php
											}                        	                        	                             
											$sql = "SELECT covnumero FROM financeiro.convenio WHERE covcod = '{$rsDadosForm[0]["covcod"]}'";
											$rs = $db->carregar( $sql );
											$convenio = $rs[0]['covnumero'];                        
											?>
											<tr>
												<td>
													<span style="display: block;font-size:100%;color:black;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> Convênio(<?php  echo $convenio; ?>)</span>
												</td>
											</tr>
											<tr>
												<td>
													<span style="display: block;font-size:100%;color:black; font-weight:bold;"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?php  echo $rsDadosForm[0]["itmnome"]; ?></span>
												</td>
											</tr>
<!-- Fim  da Tabela 4 -->											                        
										</table>
						</td>
					</tr>
					<tr>
		            <td>
<!-- Tabela 5 -->                 	
						<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
			            	<tr style="background-color: #cccccc;">
				                <td colspan="2">
									<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Etapas do Curso</div>
									<div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
<!-- Tabela 6 -->				                    
					                    <table id="itensEtapasCurso" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
					                    <!-- 766px largura -->
					                      <colgroup>
					                        <col width="20px" />
					                        <col width="22px" />
					                        <col width="110px" />
					                        <col width="110px" />
					                        <col width="110px" />
					                        <col /> 
					                        <col width="130px" /> 
					                        <col width="130px" />
					                      </colgroup>       
					                      <tr style="text-align: center">
					                        <td style="padding: 2px; font-weight: bold;">Excluir</td>
					                        <td style="padding: 2px; font-weight: bold;">Etapa </td>
					                        <td style="padding: 2px; font-weight: bold;">Descrição</td>
					                        <td style="padding: 2px; font-weight: bold;">Data de Início</td>
					                        <td style="padding: 2px; font-weight: bold;">Data Fim</td>
					                        <td style="padding: 2px; font-weight: bold;">Local / Pólo &nbsp;&nbsp;&nbsp;&nbsp;<span style="cursor:pointer; text-align: center;" id="linkInserirItem" onclick="return abrirCadLocalPolo();"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none "  /> Cadastrar</span></td>
					                        <td style="padding: 2px; font-weight: bold;">Custo da Etapa (R$)</td>
					                        <td style="padding: 2px; font-weight: bold;">Contrapartida do Estado (R$)</td>
					                      </tr>
<!-- Fim  da Tabela 6 -->				                      
					                    </table>                   
									</div>
<!-- Tabela 7 --> 									
									<table width="100%">
										<thead>
											<tr style="border: 0px solid black; padding: 0px; margin: 0px;">												
												<td style="border: 0px;" rowspan="2"><b>Total R$:</b></td>
												<td width="125px" align="center" style="border: 0px;"><b>Custo das Etapas (R$)</b></td>
												<td width="125px" align="center" style="border: 0px;"><b>Contrapartida do Estado (R$)</b></td>
												<td width="8px" style="border: 0px;" rowspan="2">&nbsp;</td>												
											</tr>
											<tr>																																		
												<td align="center" style="border: 0px;"><?= campo_texto('totlicitado', 'N', 'N', '', 16, 50, '', '', 'left', '',  0, 'id="totlicitado" onblur="MouseBlur(this)" onClick="MascaraMonetario( this )"'); ?></td>
												<td align="center" style="border: 0px;"><?= campo_texto('totcontrapartida', 'N', 'N', '', 16, 50, '', '', 'left', '',  0, 'id="totcontrapartida" onblur="MouseBlur(this);"' ); ?></td>																																			
											</tr>
										</thead>
									</table>								
									
<!-- Fim  da Tabela 7 -->
									<div style="padding: 5px 0px 0px 5px"></div>
									<span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarEtapa('','','','', '', '');"><img src="/imagens/gif_inclui.gif" align="middle" style="border: none "  /> Inserir Etapas</span>          
			          				<br />
								</td>
							</tr>
							<?php 
						
						if($sbaids){
							
							$sql = "select distinct
										app.sbaid,
										count(*) as total
									from parindigena.associativaplanodemetasprogramas app
									inner join cte.subacaoindicador si on si.sbaid = app.sbaid
									inner join cte.acaoindicador ai on ai.aciid = si.aciid
									inner join cte.pontuacao pt on pt.ptoid = ai.ptoid
									inner join cte.instrumentounidade iu on pt.inuid = iu.inuid
									where app.apptipo = '1'
									and iu.itrid = '1'
									and iu.estuf = '{$estuf}'									
									group by app.sbaid";									
									
							$nrSubacoes = $db->carregar($sql);							
							
							foreach($nrSubacoes as $dados){
								if($dados['total'] > 1){
									foreach($sbaids as $sbaid){
										if($sbaid == $dados['sbaid']){
											$mostraAlerta = 'true';
										}
									}
								}
							}
							
						}
						
						$mostraAlerta = $mostraAlerta ? $mostraAlerta : false;
						
						if($mostraAlerta){ 
						?>	
							<tr>
								<td  colspan="2" class="SubTituloDireita" >
								<span style="color:red;">
								<b>
								Os valores apresentados correspondem ao somatório de todas as subações do Material Didático.
								</b>
								</span>
								</td>										
							</tr>
							<?php } ?>
							<tr>
								<td  class="SubTituloDireita" >Valor total programado:</td>
								<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$totalValorProgramado),2,',','.');?> </td>
							</tr>
							<tr>
								<td  class="SubTituloDireita" >Valor total executado:</td>
								<td class="SubTituloEsquerda">R$ <?=number_format(str_replace(',','',$valoreTotalExecutado),2,',','.');?> </td>
							</tr>
							<tr style="background-color: #cccccc;">
								<td colspan="2" class="SubTituloEsquerda"  align="left">
									<input type="button" name="btnGravar" value="Gravar" onclick="validaForm()"/>            
									<!-- <input type="submit" name="btnGravar" value="Gravar"/> -->
									<input type="button" name="btnCancel" value="Fechar" onclick="fecharPopup();"/>
				         		</td>
				         	</tr>
<!-- Fim  da Tabela 5 -->			         		
					</table>
<!-- Fim  da Tabela 3 -->				
				</table>
<!-- Fim  da Tabela 2 -->			
			</table>
			</div>			
		</td>		
	</tr>	
</table>
<!-- Fim  da Tabela 1 -->     
</form>
</body>
</html>
<script>
function abrirCadLocalPolo(){
	return windowOpen('parindigena.php?modulo=principal/popupLocalPolo&acao=I&tipocombo=prototype',
                  'abrirCadLocalPolo',
                  'height=360,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
} 
function fecharPopup(){
    window.close();
}
function removerEtapa(linha, fieid){
    var linha = $(linha);
    linha.style.backgroundColor = 'rgb(255,255,210)';
    linha.style.backgroundColor = '#DAE0D2';

    if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
        linha.style.backgroundColor = '#f0f0f0';
    } else { 
    	try {
        	if (fieid != '') {
            	var exc = new Ajax.Request(window.location.href,{
                                                   method: 'post',
                                                   parameters: 'excluir=' + fieid,
                                                   onComplete: function(r){
                                                       //if (r.responseText == 'null')
                                                   		linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                   		linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
             } else {
             	linha.parentNode.removeChild(linha);
             }
         } catch (e) {}
//         	$('loader-container').hide();
     }
     return false;
}

function verificaDataDaEtapa(data, tipo){
 	var datai = document.getElementById('itmdatainicio');
    var dataf = document.getElementById('itmdatafim');
	    
 	alert( datai.value );
	    
    if (!validaDataMaior(datai, data)){
        alert('A data da Etapa deve estar entre da data de início e a data final do curso');
        data.focus();
        data.value = ''; 
        return false;
    }
	    
    if (!validaDataMaior(data, dataf)){
        alert('A data da Etapa deve estar entre da data de início e a data final do curso');
        data.focus();
        data.value = '';
        return false;
    }	    
}
     
function adicionarEtapa(fieid, descricao, fiedatainicial,  fiedatafinal, locid, fiecustolicitado, fiecontrapartidaestado){

	top.verified = 10;
        
    var input_fiedatainicial;
    var input_locid;  
    var input_fiedatafinal;        
    var input_fiecustolicitado;
    var input_fiecontrapartidaestado;
    var input_fieid;
    var input_fiedescricao;
        
    var tabela = document.getElementById( "itensEtapasCurso" );		
	var nrLinha = tabela.rows.length - 1;
 
       
    var btnExcluirEtapa = document.createElement('img');
    btnExcluirEtapa.src     = '/imagens/excluir.gif';
    btnExcluirEtapa.onclick = function(){
            removerEtapa(this.parentNode.parentNode.getAttribute("id"), fieid);
    }
        
    //NUMERO DA ETAPA

    //Descrição
    input_fiedescricao    = document.createElement('input');
    input_fiedescricao.setAttribute('type', 'text');
    input_fiedescricao.setAttribute('class', 'normal');
    input_fiedescricao.setAttribute('name', 'fiedescricao'+tabela.rows.length+'[]');
    input_fiedescricao.setAttribute('maxlength', '150');
    input_fiedescricao.setAttribute('size', '30');
    input_fiedescricao.setAttribute('value', descricao);
        
    //Data inicial
    input_fiedatainicial    = document.createElement('input');
    input_fiedatainicial.setAttribute('type', 'text');
    input_fiedatainicial.setAttribute('class', 'normal');
    input_fiedatainicial.setAttribute('name', 'fiedatainicial'+tabela.rows.length+'[]');
    input_fiedatainicial.setAttribute('maxlength', '10');
    input_fiedatainicial.setAttribute('size', '12');
    input_fiedatainicial.setAttribute('value', mascaraglobal('##/##/####',fiedatainicial));
    input_fiedatainicial.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');       
    input_fiedatainicial.setAttribute('onBlur', 'VerificaData(this, this.value);');
        
    //hidden, id do material
    input_fieid     = document.createElement('input');
    input_fieid.setAttribute('type', 'hidden');
    input_fieid.setAttribute('name', 'fieid[]');
    input_fieid.setAttribute('value', fieid);

//    //combo dos locais
//    input_locid   = document.createElement('select');
//    input_locid.setAttribute('class', 'CampoEstilo');
//    input_locid.setAttribute('name', 'locid[]');
//    input_locid.setAttribute('style', 'width: 180px;');
//    input_locid.options[input_locid.options.length] = new Option('Selecione...' , '');
//    <?php
//        
//    //carregando a combo das etapas.
//    $res = (array) $db->carregar('select
//       								  locid, 
//       								  locnome 
//       								 from
//       								  parindigena.localpolo pl
//       								  INNER JOIN entidade.endereco e ON e.endid = pl.endid 
//       								  AND  e.estuf = \''.$_REQUEST['estuf'].'\'  
//       								 order by
//       								  locnome');
//        
//    if ($res[0]){
//        while (list(, $item) = each($res)) {
//            $item = array_map('addslashes', $item);
//            echo 'selected=locid==' , $item["locid"]
//                , ';input_locid.options[input_locid.options.length]=new Option(\'' , $item["locnome"] , '\',\'' , $item["locid"] , '\',selected,selected);';
//        }
//   	}	
//    ?>
    //Data Final
    input_fiedatafinal    = document.createElement('input');
    input_fiedatafinal.setAttribute('type', 'text');
    input_fiedatafinal.setAttribute('class', 'normal');
    input_fiedatafinal.setAttribute('name', 'fiedatafinal'+tabela.rows.length+'[]');
    input_fiedatafinal.setAttribute('maxlength', '10');
    input_fiedatafinal.setAttribute('size', '12');
    input_fiedatafinal.setAttribute('value', mascaraglobal('##/##/####',fiedatafinal));
    input_fiedatafinal.setAttribute('onKeyUp', 'this.value=mascaraglobal("##/##/####", this.value)');
    input_fiedatafinal.setAttribute('onBlur', 'VerificaData(this, this.value);');
              
    //Custo Licitado
    input_fiecustolicitado    = document.createElement('input');
    input_fiecustolicitado.setAttribute('type', 'text');
    input_fiecustolicitado.setAttribute('class', 'normal');
    input_fiecustolicitado.setAttribute('name', 'fiecustolicitado'+tabela.rows.length+'[]');
    input_fiecustolicitado.setAttribute('id', 'fielicitado_'+tabela.rows.length);
    input_fiecustolicitado.setAttribute('maxlength', '300');
    input_fiecustolicitado.setAttribute('size', '14');
    input_fiecustolicitado.setAttribute('value', mascaraglobal('#########,##',fiecustolicitado));
    input_fiecustolicitado.setAttribute('onKeyUp', '  this.value=mascaraglobal("#########,##", this.value); ');
    input_fiecustolicitado.setAttribute('onChange', '  atualizarTotalGeral( "fielicitado_", "totlicitado" ); ');        
              
    //Contrapartida do Estado
    input_fiecontrapartidaestado    = document.createElement('input');
    input_fiecontrapartidaestado.setAttribute('type', 'text');
    input_fiecontrapartidaestado.setAttribute('class', 'normal');
    input_fiecontrapartidaestado.setAttribute('name', 'fiecontrapartidaestado'+tabela.rows.length+'[]');
    input_fiecontrapartidaestado.setAttribute('id', 'fiecontrapartida_'+tabela.rows.length);
    input_fiecontrapartidaestado.setAttribute('maxlength', '300');
    input_fiecontrapartidaestado.setAttribute('size', '14');
    input_fiecontrapartidaestado.setAttribute('value', mascaraglobal('#########,##',fiecontrapartidaestado));
    input_fiecontrapartidaestado.setAttribute('onKeyUp', ' this.value=mascaraglobal("#########,##", this.value); ');
    input_fiecontrapartidaestado.setAttribute('onChange', '  atualizarTotalGeral( "fiecontrapartida_", "totcontrapartida" ); ');        
             
    var tabelaEtapasCurso = $('itensEtapasCurso');
    var novaLinha                    = tabelaEtapasCurso.insertRow(tabelaEtapasCurso.rows.length);
    novaLinha.id                     = 'linhaitensEtapasCurso' + (tabelaEtapasCurso.rows.length - 1);
    novaLinha.style.textAlign        = 'center';
    novaLinha.style.backgroundColor  = '#f0f0f0';
        
    var numRows = (Number(tabelaEtapasCurso.rows.length) - 1);
	
    input_num     = document.createElement('text');
    input_num.setAttribute('value',  numRows );
    input_num.setAttribute('name', 'numRows');
    input_num.setAttribute('style','font-family:verdana; color: blue;');
        
    input_num.innerHTML = numRows; 
       
    //Botao excluir        
    var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
    novaCelula0.appendChild(btnExcluirEtapa);

    //numero da etapa
    var novaCelulaNum = novaLinha.insertCell(novaLinha.cells.length);
    novaCelulaNum.appendChild(input_num); 
           
    //Descrição        
    var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
    input_fiedescricao.setAttribute('id', 'fiedescricao'+numRows);
    novaCelula2.appendChild(input_fiedescricao);
           
    //Data inicio        
    var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
    input_fiedatainicial.setAttribute('id', 'fiedatainicial'+numRows);
    novaCelula3.appendChild(input_fiedatainicial);
    novaCelula3.appendChild(input_fieid);
        
    //Data fim        
    var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);      
    input_fiedatafinal.setAttribute('id', 'fiedatafinal'+numRows);
    novaCelula4.appendChild(input_fiedatafinal);       
        
    //Local / Polo        
    var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);  
	var num = new Array();
    num[fieid] = numRows;
//    alert(fieid);
    novaCelula5.id = 'celulaPolos_'+num[fieid];

    new Ajax.Request(window.location.href,{
                method: 'post',
                parameters: '&req=polos&id=polos_' + num[fieid]+ '&fieid=' +fieid+ '&linha=' +num[fieid],
                onComplete: function(e){
//        			alert(e.responseText);
                    $('celulaPolos_'+num[fieid]).innerHTML = e.responseText;
                }
            });

        
         
    //Custo Licitado         
    var novaCelula6 = novaLinha.insertCell(novaLinha.cells.length);      
    novaCelula6.appendChild(input_fiecustolicitado);        
         
    //Contrapartida do Estado         
    var novaCelula7 = novaLinha.insertCell(novaLinha.cells.length);      
    novaCelula7.appendChild(input_fiecontrapartidaestado);      
        
	if (locid == null){
		input_locid.select();
	}

    return false;
}

function carregarEtapasCurso(){    
    <?php
        
    if( $_REQUEST['friid'] ){    
   
        $itens = $db->carregar('SELECT
                                    fi.fiedescricao,
        							fi.fieid,
                                    fi.locid,
                                    fi.fiedatainicial,
                                    fi.fiedatafinal,
                                    fi.fiecustolicitado,
                                    fi.fiecontrapartidaestado
                                FROM
                                    parindigena.formacaoinicialetapa fi
                                WHERE
                                    fi.friid  = ' . $_REQUEST['friid'] . '
                                ');
       
    }

    if (!is_array($itens)) {
        echo "return false;";
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            
            $arrexpI = explode( "-", $item['fiedatainicial']);
            $converteDataI = $arrexpI[2].'-'.$arrexpI[1].'-'.$arrexpI[0];
            
            $arrexpF = explode( "-", $item['fiedatafinal']);
            $converteDataF = $arrexpF[2].'-'.$arrexpF[1].'-'.$arrexpF[0];

            
            echo "adicionarEtapa(" , $item['fieid'] , ",'" , $item['fiedescricao'] , "','" , $converteDataI , "','" , $converteDataF , "'," , ($item['locid'] ? $item['locid'] : "''" ),"," , str_replace(array('.',',','R$'), '', $item['fiecustolicitado']),"," , str_replace(array('.',',','R$'), '', $item['fiecontrapartidaestado']),");";
        }
    }
	?>
}

carregarEtapasCurso();
	
function getElementsByName_iefix(tag, name){   
	
	var elem = document.getElementsByTagName(tag);  
	var arr = new Array();  
		for(i = 0,iarr = 0; i < elem.length; i++) {  
			att = elem[i].getAttribute("name");  
			if(att == name) 
			{  
				arr[iarr] = elem[i];  
				iarr++;  
			}  
		}  
		return arr;  
	}
	
function validaForm() {		

	var tabela = document.getElementById( "itensEtapasCurso" );		
	var nrLinha = tabela.rows.length;
		
	for( i=1; i<nrLinha; i++ ){

		var arDescricao = document.getElementById("fiedescricao"+i );
		var arDataInicial = document.getElementById("fiedatainicial"+i );
		var arDataFinal = document.getElementById("fiedatafinal"+i );
		var arLocId = document.getElementById("celulaPolos_"+i );
//		alert(arLocId.firstChild.children[0].value);
	               
		if( arDescricao.value == ''){
			alert("O campo DESCRIÇÃO é obrigatório!");
			arDescricao.focus();
			return false;				
		}
			
	               
		if( arDataInicial.value == ''){
			alert("O campo DATA INICIAL é obrigatório!");
			arDataInicial.focus();
			return false;				
		}
			
		if( arDataFinal.value == ''){
			alert("O campo DATA FINAL é obrigatório!");
			arDataFinal.focus();
			return false;				
		}
			
		dataInicial = arDataInicial.value;
		dataFinal = arDataFinal.value; 
		var dataFinalSepara = dataFinal.split("/");
		var dataFinalMonta = dataFinalSepara[2] + dataFinalSepara[1] + dataFinalSepara[0];
		var dataInicialSepara = dataInicial.split("/");
		var dataInicialMonta = dataInicialSepara[2] + dataInicialSepara[1] + dataInicialSepara[0]; 
			
		if( dataInicialMonta > dataFinalMonta ){
			alert("O campo DATA FINAL deve ser maior que a DATA INICIAL!");
			arDataInicial.focus();
			arDataInicial.value = "";
			arDataFinal.value = "";				
			return false;				
		}
						
//		alert(arLocId.firstChild.id);
		
		selectAllOptions(document.getElementById(arLocId.firstChild.id));
		
		if( arLocId.firstChild.children[0].value == ''){
			alert("O campo LOCAL / POLO é obrigatório!");
//			arLocId.focus();
			return false;				
		}
		
	}
	document.formulario.submit();		
}	
</script>
      