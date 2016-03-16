<?php

include_once '_funcoes_escolaativa.php';
include_once APPRAIZ.'includes/workflow.php';
include_once APPRAIZ.'par/classes/modelo/EscolaAtiva.class.inc';
include_once APPRAIZ.'par/classes/modelo/EscolaAtivaEscolas.class.inc';
include_once APPRAIZ.'par/classes/modelo/EscolaAtivaPessoas.class.inc';

if($_POST['resetaEscolaAtiva'] == 'true'){
	
	$sql = "select esaid from par.escolaativa where inuid = '{$_SESSION['inuid']}' and esaano = '2012'";
	$esaid = $db->pegaUm($sql);
	if($esaid){	
		$sql = "delete from par.escolaativaescolas where esaid = '{$esaid}'";
		$db->executar($sql);
		$sql = "delete from par.escolaativa where esaid = '{$esaid}'";
		$db->executar($sql);
		$db->commit();
	}	
}

$obEscolaAtiva = new EscolaAtiva( "", $_SESSION['par']['inuid'] );

$docid = criaDocumentoEscolaAtiva($obEscolaAtiva->esaid);
$esdid = pegaEstadoAtualEscolaAtiva($docid);

if( $_REQUEST["gravarEscolaAtiva"] && $obEscolaAtiva->podeEditarEscolaAtiva() ){

	$arCampos = array( "esaid", "entid", "inuid", "esaano", "esaqtdprofessores" );
	$obEscolaAtiva->popularObjeto( $arCampos );

	$obEscolaAtiva->entid = $_REQUEST["entid"] ? $_REQUEST["entid"] : null;

	if( is_array( $_REQUEST["entid_escolas"] ) ){
		$obEscolaAtiva->esaqtdtecnicos =  ceil( count( $_REQUEST["entid_escolas"] )/25 );
	}

	if( $obEscolaAtiva->salvar() ){
		$obEscolaAtiva->commit();
	}
	else{
		$obEscolaAtiva->rollback();
	}

	if( is_array( $_REQUEST["eaeid"] ) ){
		
//		$sql = "DELETE FROM par.escolaativaescolas WHERE esaid = {$obEscolaAtiva->esaid}";
//		$db->executar($sql);
//		$db->commit();
		
		foreach( $_REQUEST["eaeid"] as $count => $eaeid ){
				
			( $eaeid ? $eaeid : $eaeid = null );
			
			$obEscolaAtivaEscolas = new EscolaAtivaEscolas( $eaeid );
				
			$obEscolaAtivaEscolas->eaeid = $eaeid;
			$obEscolaAtivaEscolas->esaid = $obEscolaAtiva->esaid;
			$obEscolaAtivaEscolas->entid = $_REQUEST["entid_escolas"][$count];
			$obEscolaAtivaEscolas->eaeqtdpredios = (int) $_REQUEST["eaeqtdpredios"][$count];
			$obEscolaAtivaEscolas->eaeqtdturmas  = (int) $_REQUEST["eaeqtdturmas" ][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos1 = (int) $_REQUEST["eaeqtdalunos1"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos2 = (int) $_REQUEST["eaeqtdalunos2"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos3 = (int) $_REQUEST["eaeqtdalunos3"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos4 = (int) $_REQUEST["eaeqtdalunos4"][$count];
			$obEscolaAtivaEscolas->eaeqtdalunos5 = (int) $_REQUEST["eaeqtdalunos5"][$count];
				
			$obEscolaAtivaEscolas->salvar();
			$obEscolaAtivaEscolas->commit();
		}
	}

	$obEscolaAtiva->sucesso( "principal/programas/escolaativa/listaEscolas" );

}

if($_REQUEST['req'] == 'removeescolas'){

	if( $obEscolaAtiva->esaid && $_REQUEST["entid"] ){
		$sql = " delete from par.escolaativaescolas where esaid = {$obEscolaAtiva->esaid} and entid = {$_REQUEST["entid"]} ";

		$obEscolaAtiva->executar( $sql );
		$obEscolaAtiva->commit();
	}
	die();
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

cabecalho();

if( date('Ymd') > PAR_ESCOLAATIVA_DATA_LIMITE_ADESAO ){
	
	echo 'O processo de preenchimento de Escola da Terra de 2012 está encerrado.';
	
}else{
	
	if( $obEscolaAtiva->esaid ){
		
		$boPodeEditar = "S";
		
		$muncod = recuperarMuncodPorInuid($_SESSION['par']['inuid']);
		$estuf = recuperarEstufPorInuid($_SESSION['par']['inuid']);
		
		if(!$_REQUEST['importarDados']){
			
			$coEscolaAtivaEscolas = $obEscolaAtiva->recuperarCoEscolas();
			
			$stNomeEntidade = $obEscolaAtiva->recuperarNomeEntidade();
			
		} else {
			
			$esaid = $db->pegaUm( "select esaid from par.escolaativa where inuid = {$_SESSION['par']['inuid']} and esaano = 2011" );
	
			if($esaid){
				
				$sql = "select eae.entid, e.entnome, e.entcodent, eaeqtdpredios, eaeqtdturmas, eaeqtdalunos1, eaeqtdalunos2, eaeqtdalunos3, eaeqtdalunos4, eaeqtdalunos5
						from par.escolaativaescolas eae
							inner join entidade.entidade e on e.entid = eae.entid
						where esaid = {$esaid} 
						order by e.entnome";				
				
				$coEscolaAtivaEscolas = $db->carregar( $sql );
			}
			
			$sql = "select esaid from par.escolaativa where inuid = {$_SESSION['par']['inuid']} and esaano = 2011";
			$rsEscolaAtiva = $db->pegaUm( $sql );
			
			if($rsEscolaAtiva){
				
				if($obEscolaAtiva->entid){
					$sql = "select entnome from entidade.entidade where entid = $obEscolaAtiva->entid ";
					$stNomeEntidade = $db->pegaUm( $sql );
				}
				
				$obEscolaAtiva->esaqtdprofessores = $rsEscolaAtiva['esaqtdprofessores'];
			}
			
		}
	?>
		
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript">
		function importarDados(){
		
			window.location.href = 'par.php?modulo=principal/programas/escolaativa/listaEscolas&acao=A&importarDados=true';
		
		}
		
		function popupSelecionarEscolas( muncod, estuf ){
		
			return window.open( 'par.php?modulo=principal/programas/escolaativa/popupEscola&acao=A&muncod=' + muncod + '&estuf=' + estuf,
								'PopupEscolas',
								'height=600, width=600, status=yes, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=yes' );
			
			return false;
		}
		
		function atualizarTotal( obj, indice ){
			
			var total  = document.getElementById( "totalAlunos_"+indice   );
			var aluno1 = document.getElementById( "eaeqtdalunos1_"+indice );
			var aluno2 = document.getElementById( "eaeqtdalunos2_"+indice );
			var aluno3 = document.getElementById( "eaeqtdalunos3_"+indice );
			var aluno4 = document.getElementById( "eaeqtdalunos4_"+indice );
			var aluno5 = document.getElementById( "eaeqtdalunos5_"+indice );
			
			aluno1.value = aluno1.value ? aluno1.value : 0;
			aluno2.value = aluno2.value ? aluno2.value : 0;
			aluno3.value = aluno3.value ? aluno3.value : 0;
			aluno4.value = aluno4.value ? aluno4.value : 0;
			aluno5.value = aluno5.value ? aluno5.value : 0;
			
			total.value = parseInt( aluno1.value ) + parseInt( aluno2.value ) + parseInt( aluno3.value ) + parseInt( aluno4.value ) + parseInt( aluno5.value );
		
			var arElemento = getElementsByName_iefix( "input", "totalAlunos[]" );
			var totalGeral = document.getElementById( "totalGeral" );
			
			valor = 0
			for( i=0; i<arElemento.length; i++ ){
				arElemento[i].value = arElemento[i].value ? arElemento[i].value : 0;
				valor += parseInt( arElemento[i].value );
			}
					
			totalGeral.value = parseInt( valor ); 		
		
		}
		
		function atualizarTotalGeral( name, id ){
			
			var arElemento = getElementsByName_iefix( "input", name );
			var elementoTotal = document.getElementById( id );
			total = 0;
			
			for( i=0; i<arElemento.length; i++ ){
				arElemento[i].value = arElemento[i].value ? arElemento[i].value : 0;
				total += parseInt( arElemento[i].value ); 
			}
					
			elementoTotal.value = parseInt( total ); 
		
		}
		
		function gerarSubacao ()
		{
			alert('Popup apenas p tecnicos Secad<br>Parecer padrão<br>Parecer aprovado');
		}
		
		function getElementsByName_iefix(tag, name) 
		{             
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
		
		function enviarForm ()
		{
			d = document.formulario;
			
			var tabela = document.getElementById( "tabelaEscolas" );		
	        var nrLinha = tabela.rows.length;        
	        
	        if( nrLinha < 1 ){
	            alert('Deve haver pelo menos uma escola');
	            exit;
	        }
			
			// Valida campo Qtd. Turmas >= Qtd. Predios			 
			var arQtdTurmas = getElementsByName_iefix( "input", "eaeqtdturmas[]" );
			var arQtdPredios = getElementsByName_iefix( "input", "eaeqtdpredios[]" );
	               
			for( i=0; i < arQtdTurmas.length; i++ ){
		               
				if( arQtdTurmas[i].value == '')
				{
					alert("O campo QTD. TURMAS é obrigatório!");
					arQtdTurmas[i].focus();
					return false;				
				}
				
				if( arQtdPredios[i].value == '')
				{
					alert("O campo PRÉDIOS / ANEXO é obrigatório!");
					arQtdPredios[i].focus();
					return false;				
				}
				
				if( parseInt(arQtdTurmas[i].value) < parseInt(arQtdPredios[i].value) )
				{
					alert("O campo QTD. TURMAS deve ser MAIOR ou IGUAL a QTD. PRÉDIOS!");
					arQtdTurmas[i].focus();
					return false;				
				}
				
			}
			
			// Valida campo totais de alunos > 0
			var arTotalAlunos = getElementsByName_iefix( "input", "totalAlunos[]" );
	               
			for( i=0; i<arTotalAlunos.length; i++ ){
		               
				if( !arTotalAlunos[i].value )
				{
					alert("O campo TOTAL DE ALUNOS deve ser maior que zero!");
					arTotalAlunos[i].focus();
					return false;
				}
			}			
					
			// Validar campo N.º DE PROFESSORES, obrigatório e maior que 0						
			if (d.esaqtdprofessores.value == '')
			{
				alert("O campo N.º DE PROFESSORES deve ser preenchido!");
				esaqtdprofessores.focus();
				return false;						
			}
			 
			if (d.esaqtdprofessores.value <= 0)
			{
				alert("O campo N.º DE PROFESSORES deve ser maior que zero!");
				esaqtdprofessores.focus();
				return false;
			}
			
			// Validar campo N.º DE TECNICOS, obrigatório e maior que 0						
			if (!d.esaqtdtecnicos.value)
			{
				alert("O campo N.º DE TÉCNICOS deve ser maior que zero!");
				esaqtdtecnicos.focus();
				return false;						
			}
			
			document.getElementById('formulario').submit();
			
			return true;		
		}
		
		function deletarEscola(entid){	
		
	    if (confirm("Deseja excluir a escola?"))
	    {
	    
		    return new Ajax.Request(window.location.href,
		                                {
		                                    method: 'post',
		                                    parameters: '&req=removeescolas&entid=' + entid,
		                                    asynchronous: false,
		                                    onComplete: function(res){
		                                    
		                                      		var tabela = document.getElementById( "tabelaEscolas" );
												 	var	linha =  document.getElementById( "linha_"+entid );
											 	tabela.deleteRow( linha.rowIndex );
		                                    }
		                                });
			
			}
		}	
		</script>
		<style>
		#divLabel {
			line-height: 25px;
		}
		#tabelaEscolasDiv {
			height: 210px;
			overflow: auto;
		}
	
		</style>
		
		<form action="par.php?modulo=principal/programas/escolaativa/listaEscolas&acao=A" id="formulario" name="formulario" method="post">
			
			<input type="hidden" name="esaid" id="esaid" value="<?php echo $obEscolaAtiva->esaid; ?>"> 
			<input type="hidden" name="entid" id="entid" value="<?php echo $obEscolaAtiva->entid; ?>"> 
			<input type="hidden" name="esaano" id="esaano" value="2012">
			 
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2" class="subtitulodireita" style="text-align: left;font-weight: bold;">
					<center><h3>Escola da Terra</h3></center>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Secretário(a) 1) estadual / municipal de educação:</td>
					<td>
						<?php
						$inuid = $_SESSION['par']['inuid']; 
	
						$sql = "select 
									u.usunome, 
									u.usufoneddd, 
									u.usufonenum, 
									u.usuemail 
								from par.escolaativa i 
								inner join seguranca.usuario u on i.usucpfescolaativa = u.usucpf  
								where i.inuid = $inuid
								and i.esaano = 2012";
						
						$usuAderiu = $db->pegaLinha( $sql );																				
						?>					
						<? echo campo_texto( "entnome", 'N', 'N', '', '50', '', '', '', '', '', '', 'id="entnome"', '', $stNomeEntidade ) ?>					
						<img src='../../imagens/alterar.gif' title="Inserir/editar secretário(a)" border='0' onclick="window.open('?modulo=principal/programas/escolaativa/popupContato&acao=A&entid=<?php echo $obEscolaAtiva->entid ?>','Dirigentes', 'scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');">&nbsp;<a onclick="window.open('?modulo=principal/programas/escolaativa/popupContato&acao=A&entid=<?php echo $obEscolaAtiva->entid ?>','Dirigentes', 'scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');" title="Inserir/editar secretário(a)">Inserir</a>&nbsp;|&nbsp; 
						<a onmouseover="return escape('<? echo "Nome: ".$usuAderiu['usunome']."<br>Telefone(s): (".$usuAderiu['usufoneddd'].") ".$usuAderiu['usufonenum']."<br>E-mail: ".$usuAderiu['usuemail']; ?>');">Usuário que aderiu</a>						
					</td>
				</tr>
				<tr>
					<td colspan="2">
						<table width="100%">
							<tr>
								<td valign="top">
									 <table class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%">
										<thead>		
											<tr align="center">
												<td width="4%" rowspan="2" >Excluir</td>
												<td width="30%" rowspan="2" >Nome das escolas<img src="../imagens/IconeAjuda.gif" onmouseover="return escape('Selecione apenas escolas multisseriadas que funcionam no campo e que desenvolverão o Programa Escola da Terra em 2012.');"></td>
												<td width="8%" rowspan="2" >Prédios/ Anexo<img src="../imagens/IconeAjuda.gif" onmouseover="return escape('O número de escolas e/ou prédios escolares anexos (escola mãe, filhas ...) implica na definição do quantitativo de Kit pedagógico que deverá ser encaminhado para o município, sendo que será destinado apenas um kit pedagógogico por prédio.');"></td>
												<td width="8%" rowspan="2" >Qtd. Turmas</td>
												<td width="30%" colspan="6" style="border-right: 0px;">Estudantes<img src="../imagens/IconeAjuda.gif" onmouseover="return escape('O número de alunos representa o número de cadernos de ensino aprendizagem (livros) necessários para atendimento da demanda do município por série/ano para 2010.');"></td>
												<td width="1%" rowspan="2" style="border-left: 0px;"></td>
																		
											</tr>
											<tr align="center">
												<td width="5%">1º ano</td>
												<td width="5%">2º ano</td>
												<td width="5%">3º ano</td>
												<td width="5%">4º ano</td>
												<td width="5%">5º ano</td>
												<td width="5%" style="border-right: 0px;">Total de <br>alunos</td>
												
											</tr>								
										</thead>
									</table> 
									<div id="tabelaEscolasDiv" style="overflow: scroll;">
										<table id="tabelaEscolas" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%" >
											<td width="13px"></td> 						
											<td></td> 
											<tbody id="tbodyTabela">
												<?php if( is_array( $coEscolaAtivaEscolas ) ){
										  			
										  			unset( $_SESSION["arEntidadesSelecionadas"] );
										  			$arEntidadesSelecionadas = array();
										  			$total = 0;
										  			$turmas = 0;
										  			$predios = 0;
										  			$alunos1 = 0;
										  			$alunos2 = 0;
										  			$alunos3 = 0;
										  			$alunos4 = 0;
										  			$alunos5 = 0;
										  			$totalGeral = 0;
													foreach( $coEscolaAtivaEscolas as $indice => $arEscolaAtivaEscolas ){
														$total = $arEscolaAtivaEscolas["eaeqtdalunos1"] + $arEscolaAtivaEscolas["eaeqtdalunos2"] + $arEscolaAtivaEscolas["eaeqtdalunos3"] + $arEscolaAtivaEscolas["eaeqtdalunos4"] + $arEscolaAtivaEscolas["eaeqtdalunos5"];
														$turmas += $arEscolaAtivaEscolas["eaeqtdturmas"];
														$predios += $arEscolaAtivaEscolas["eaeqtdpredios"];
														$alunos1 += $arEscolaAtivaEscolas["eaeqtdalunos1"];
														$alunos2 += $arEscolaAtivaEscolas["eaeqtdalunos2"];
														$alunos3 += $arEscolaAtivaEscolas["eaeqtdalunos3"];
														$alunos4 += $arEscolaAtivaEscolas["eaeqtdalunos4"];
														$alunos5 += $arEscolaAtivaEscolas["eaeqtdalunos5"];
														$totalGeral += $total; 
														?>
														
														<tr style="background: #f5f5f5;" id="linha_<?php echo $arEscolaAtivaEscolas["entid"]; ?>" >
															<td width="4%" align="center">										
															<?php if($boPodeEditar != "N"){ ?>
																<img onclick="deletarEscola(<?php echo $arEscolaAtivaEscolas["entid"]; ?>);" src="../imagens/excluir.gif" />
															<?php } else { ?>
																<img src="../imagens/excluir_01.gif"/>
															<?php } ?>
															</td>
															<td width="30%">										
																<input type="hidden" name="eaeid[]" value="<?php echo $arEscolaAtivaEscolas["eaeid"]; ?>" />
																<input type="hidden" name="entid_escolas[]" value="<?php echo $arEscolaAtivaEscolas["entid"]; ?>" />
																<?php echo $arEscolaAtivaEscolas["entnome"] ?>
															</td>
															<td width="8%" align="center"><?php echo campo_texto( "eaeqtdpredios[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdpredios_'.$indice.'" onchange="atualizarTotalGeral( \'eaeqtdpredios[]\', \'totalPredios\' );"', '', $arEscolaAtivaEscolas["eaeqtdpredios"] ); ?></td>
															<td width="8%" align="center"><?php echo campo_texto( "eaeqtdturmas[]",  'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdturmas_'.$indice.'" onchange="atualizarTotalGeral( \'eaeqtdturmas[]\', \'totalTurmas\' );"', '', $arEscolaAtivaEscolas["eaeqtdturmas"]  ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "eaeqtdalunos1[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdalunos1_'.$indice.'" onchange="atualizarTotal( this, '.$indice.' ); atualizarTotalGeral( \'eaeqtdalunos1[]\', \'totalAlunos1\' );"', '', $arEscolaAtivaEscolas["eaeqtdalunos1"] ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "eaeqtdalunos2[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdalunos2_'.$indice.'" onchange="atualizarTotal( this, '.$indice.' ); atualizarTotalGeral( \'eaeqtdalunos2[]\', \'totalAlunos2\' );"', '', $arEscolaAtivaEscolas["eaeqtdalunos2"] ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "eaeqtdalunos3[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdalunos3_'.$indice.'" onchange="atualizarTotal( this, '.$indice.' ); atualizarTotalGeral( \'eaeqtdalunos3[]\', \'totalAlunos3\' );"', '', $arEscolaAtivaEscolas["eaeqtdalunos3"] ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "eaeqtdalunos4[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdalunos4_'.$indice.'" onchange="atualizarTotal( this, '.$indice.' ); atualizarTotalGeral( \'eaeqtdalunos4[]\', \'totalAlunos4\' );"', '', $arEscolaAtivaEscolas["eaeqtdalunos4"] ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "eaeqtdalunos5[]", 'N', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="eaeqtdalunos5_'.$indice.'" onchange="atualizarTotal( this, '.$indice.' ); atualizarTotalGeral( \'eaeqtdalunos5[]\', \'totalAlunos5\' );"', '', $arEscolaAtivaEscolas["eaeqtdalunos5"] ); ?></td>
															<td width="5%" align="center"><?php echo campo_texto( "totalAlunos[]", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos_'.$indice.'";"', '', $total ); ?></td>
														</tr>
										  			<?php } 
										  			$_SESSION["arEntidadesSelecionadas"] = $arEntidadesSelecionadas;
										  			?>
										  		<?php } ?>					
											</tbody>
									</table>
									</div>
									<table class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%">
									<thead>
											<tr align="center">
												<td width="34%">Totais</td>
												<td width="8%"><?php echo campo_texto( "totalPredios", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalPredios"', '', $predios ); ?></td>
												<td width="8%"><?php echo campo_texto( "totalTurmas" , 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalTurmas"', '', $turmas ); ?></td>							
												<td width="5%"><?php echo campo_texto( "totalAlunos1", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos1"', '', $alunos1 ); ?></td>
												<td width="5%"><?php echo campo_texto( "totalAlunos2", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos2"', '', $alunos2 ); ?></td>
												<td width="5%"><?php echo campo_texto( "totalAlunos3", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos3"', '', $alunos3 ); ?></td>
												<td width="5%"><?php echo campo_texto( "totalAlunos4", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos4"', '', $alunos4 ); ?></td>
												<td width="5%"><?php echo campo_texto( "totalAlunos5", 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalAlunos5"', '', $alunos5 ); ?></td>
												<td width="6%"><?php echo campo_texto( "totalGeral"  , 'N', 'N', '', '4', '', '', '', '', '', '', 'id="totalGeral"', '', $totalGeral ); ?></td>
											</tr>
									</thead>
									</table>
								</td>
								<td width="85" valign="top" align="center">
									<?php
									wf_desenhaBarraNavegacao( $docid, array( 'inuid' => $_SESSION['par']['inuid'] ) );
									?>	
								</td>
							</tr>
						</table>
					</td>		
				</tr>
				<tr height="35px">
					<td colspan="2">
					<span id="addEntidadeExecucao" name="addEntidadeExecucao" style="cursor:pointer;display:block;float:left" onclick="popupSelecionarEscolas( <?php echo $muncod; ?>, '<?php echo $estuf; ?>' );">
						<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />&nbsp;&nbsp;&nbsp;Inserir Escolas
					</span>		
					</td>
				</tr>
				<tr>
					<td colspan="2" class="subtitulodireita" style="text-align: left;font-weight: bold;">
					<center><strong>Outras informações do município</strong></center>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">N.º de Escolas:</td>
					<td><? echo campo_texto( "esaqtdescolas", 'S', 'N', '', '4', '', '', '', '', '', '', 'id="esaqtdescolas"', '', ceil( count( $coEscolaAtivaEscolas )) ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">N.º de Coordenadores:</td>
					<td><? echo campo_texto( "esaqtdtecnicos", 'S', 'N', '', '4', '', '', '', '', '', '', 'id="esaqtdtecnicos"', '', ceil( count( $coEscolaAtivaEscolas ) / 25 ) ); ?><img src="../imagens/IconeAjuda.gif" onmouseover="return escape('O número de coordenadores municipais será definido a partir do número de professores que serão formados, devendo existir um coordenador para cada 10 professores cursistas. No caso dos estados, haverá apenas 01 coordenador.');"></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">N.º de Professores:</td>
					<td>
						<? echo campo_texto( "esaqtdprofessores", 'S', $boPodeEditar, '', '4', '', '', '', '', '', '', 'id="esaqtdprofessores"', '', $obEscolaAtiva->esaqtdprofessores ); ?><img src="../imagens/IconeAjuda.gif" onmouseover="return escape('O número de professores deverá representar o quantitativo dos professores que não tiveram formação para atuar nas classes multisseriadas.');">
					</td>
				</tr>
				<?php if( $boPodeEditar == 'S' ){ ?>
				<tr bgcolor="#C0C0C0">
					<td>		
					</td>
					<td>
						<input type="hidden" name="gravarEscolaAtiva" value="1" />
						<input type="button" value="Gravar" onclick="enviarForm()">
						<!--  <input type="button" value="Importar dados" onclick="importarDados()"> -->
					</td>
				</tr>
				<?php } ?>
			</table>
		</form>
		<script type="text/javascript">
	<!--
	
	var maximo = 20;
	var itens_selecionados = 0;
	function adiciona_item( cod, descricao, checkbox ){	
				
			
			var tabela = document.getElementById( "tabelaEscolas" ); //window.opener.
			var tbody = document.getElementById( "tbodyTabela" ); //window.opener.
			var nrLinha = tabela.rows.length;
			
			if ( checkbox == true ){
	
				itens_selecionados++;				
						
				linha = tabela.insertRow( tabela.rows.length );
				
				tbody.appendChild( linha );
				
				linha.setAttribute( "id", "linha_" + cod )
				linha.setAttribute( "style", "background: #f5f5f5;" )
				
				var eaeid = document.createElement( "input" );
				eaeid.setAttribute( "type", "hidden" );
				eaeid.setAttribute( "name", "eaeid[]" );
				//eaeid.setAttribute( "value", cod );
				
				var entid = document.createElement( "input" );
				entid.setAttribute( "type", "hidden" );
				entid.setAttribute( "name", "entid_escolas[]" );
				entid.setAttribute( "value", cod );
				
				var img1 = document.createElement( "img" );
				celula0 = linha.insertCell( 0 );
				img1.setAttribute("src","../imagens/excluir.gif");
				img1.setAttribute("align","center");
				img1.setAttribute("onclick","deletarEscola("+cod+")");				
				celula0.appendChild(img1);								
				celula0.style.textAlign = 'center';		
								
				celula1 = linha.insertCell( 1 );
				celula1.innerHTML = descricao;
				celula1.appendChild( eaeid );
				celula1.appendChild( entid );
				
				/* Inicio dos campos DATAGRID da Escola da Terra */
				
				var campo1 = document.createElement( "input" );
				celula2 = linha.insertCell ( 2 );				
				campo1.setAttribute( "type", "text" );
				campo1.setAttribute( 'name', 'eaeqtdpredios[]' );
				campo1.setAttribute( "size", "5" );
				campo1.setAttribute("class","normal");	
				campo1.setAttribute("onmouseover","MouseOver(this)");
				campo1.setAttribute("onfocus","MouseClick(this)");
				campo1.setAttribute("onmouseout","MouseOut(this)");						
				campo1.setAttribute("onblur","MouseBlur(this)");							
				campo1.setAttribute( "id", "eaeqtdpredios_"+nrLinha );
				celula2.appendChild( campo1 );		
				celula2.style.textAlign = 'center';		
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo1.setAttribute( "onchange", "atualizarTotalGeral( 'eaeqtdpredios[]', 'totalPredios' );" );				
				}
				else{ // IE
					campo1.attachEvent( "onchange", function() { atualizarTotalGeral( 'eaeqtdpredios[]', 'totalPredios' ); } );				
				}
				//onkeyup
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo1.setAttribute( "onkeyup", "this.value=mascaraglobal('#####', this.value);" );				
				}
				else{ // IE				
					//campo1.attachEvent( "onkeyup", function() { campo1.value = mascaraglobal('#####', campo1.value) } );
				}
				
				var campo2 = document.createElement( "input" );
				celula3 = linha.insertCell ( 3 );				
				campo2.setAttribute( "type", "text" );
				campo2.setAttribute( "name", "eaeqtdturmas[]" );
				campo2.setAttribute( "size", "5" );	
				campo2.setAttribute("class","normal");	
				campo2.setAttribute("onmouseover","MouseOver(this)");
				campo2.setAttribute("onfocus","MouseClick(this)");
				campo2.setAttribute("onmouseout","MouseOut(this)");						
				campo2.setAttribute("onblur","MouseBlur(this)");			
				campo2.setAttribute( "id", "eaeqtdturmas_"+nrLinha );
				celula3.appendChild( campo2 );
				celula3.style.textAlign = 'center';
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo2.setAttribute( "onchange", "atualizarTotalGeral( 'eaeqtdturmas[]', 'totalTurmas' );" );
				}
				else{ // IE
					campo2.attachEvent( "onchange", function() { atualizarTotalGeral( 'eaeqtdturmas[]', 'totalTurmas' ); } );
				}
				// onkeyup
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo2.setAttribute("onKeyUp","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo2.attachEvent( "onkeyup", function() { campo2.value = mascaraglobal('#####', campo2.value) } );
				}
				
				var campo3 = document.createElement( "input" );
				celula4 = linha.insertCell ( 4 );				
				campo3.setAttribute( "type", "text" );
				campo3.setAttribute( "name", "eaeqtdalunos1[]" );
				campo3.setAttribute( "size", "5" );
				campo3.setAttribute("class","normal");	
				campo3.setAttribute("onmouseover","MouseOver(this)");
				campo3.setAttribute("onfocus","MouseClick(this)");
				campo3.setAttribute("onmouseout","MouseOut(this)");						
				campo3.setAttribute("onblur","MouseBlur(this)");
				campo3.setAttribute( "id", "eaeqtdalunos1_"+nrLinha );
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo3.setAttribute( "onchange", "atualizarTotal( this, "+ nrLinha + " ); atualizarTotalGeral( 'eaeqtdalunos1[]', 'totalAlunos1' );" );
				}
				else{ // IE
					campo3.attachEvent( "onchange", function() { atualizarTotal( this, nrLinha ); atualizarTotalGeral( 'eaeqtdalunos1[]', 'totalAlunos1' ); } );				
				}
				// onkeyup
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo3.setAttribute("onKeyUp","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo3.attachEvent( "onkeyup", function() { campo3.value = mascaraglobal('#####', campo3.value) } );
				}
				
												
				celula4.appendChild( campo3 );
				celula4.style.textAlign = 'center';				
				
				
				
				var campo4 = document.createElement( "input" );
				celula5 = linha.insertCell ( 5 );				
				campo4.setAttribute( "type", "text" );
				campo4.setAttribute( "name", "eaeqtdalunos2[]" );
				campo4.setAttribute( "size", "5" );
				campo4.setAttribute("class","normal");	
				campo4.setAttribute("onmouseover","MouseOver(this)");
				campo4.setAttribute("onfocus","MouseClick(this)");
				campo4.setAttribute("onmouseout","MouseOut(this)");
				campo4.setAttribute("onblur","MouseBlur(this)");						
				campo4.setAttribute( "id", "eaeqtdalunos2_"+nrLinha );
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo4.setAttribute( "onchange", "atualizarTotal( this, "+ nrLinha + " ); atualizarTotalGeral( 'eaeqtdalunos2[]', 'totalAlunos2' );" );
				}
				else{ // IE
					campo4.attachEvent( "onchange", function() { atualizarTotal( this, nrLinha ); atualizarTotalGeral( 'eaeqtdalunos2[]', 'totalAlunos2' ); } );				
				}
				// onKeyUp
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo4.setAttribute("onkeyup","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo4.attachEvent( "onkeyup", function() { campo4.value = mascaraglobal('#####', campo4.value) } );
				}	
							
				celula5.appendChild( campo4 );
				celula5.style.textAlign = 'center';
				
				var campo5 = document.createElement( "input" );
				celula6 = linha.insertCell ( 6 );				
				campo5.setAttribute( "type", "text" );
				campo5.setAttribute( "name", "eaeqtdalunos3[]" );
				campo5.setAttribute( "size", "5" );
				campo5.setAttribute("class","normal");	
				campo5.setAttribute("onmouseover","MouseOver(this)");
				campo5.setAttribute("onfocus","MouseClick(this)");
				campo5.setAttribute("onmouseout","MouseOut(this)");			
				campo5.setAttribute("onblur","MouseBlur(this)");	
				campo5.setAttribute( "id", "eaeqtdalunos3_"+nrLinha );
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo5.setAttribute( "onchange", "atualizarTotal( this, "+ nrLinha + " ); atualizarTotalGeral( 'eaeqtdalunos3[]', 'totalAlunos3' );" );
				}
				else{ // IE
					campo5.attachEvent( "onchange", function() { atualizarTotal( this, nrLinha ); atualizarTotalGeral( 'eaeqtdalunos3[]', 'totalAlunos3' ); } );
				}
				// onkeyup
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo5.setAttribute("onkeyup","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo5.attachEvent( "onkeyup", function() { campo5.value = mascaraglobal('#####', campo5.value) } );
				}
					
				celula6.appendChild( campo5 );
				celula6.style.textAlign = 'center';
				
				var campo6 = document.createElement( "input" );
				celula7 = linha.insertCell ( 7 );				
				campo6.setAttribute( "type", "text" );
				campo6.setAttribute( "name", "eaeqtdalunos4[]" );
				campo6.setAttribute( "size", "5" );
				campo6.setAttribute("class","normal");	
				campo6.setAttribute("onmouseover","MouseOver(this)");
				campo6.setAttribute("onfocus","MouseClick(this)");
				campo6.setAttribute("onmouseout","MouseOut(this)");
				campo6.setAttribute("onblur","MouseBlur(this)");			
				campo6.setAttribute( "id", "eaeqtdalunos4_"+nrLinha );			
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo6.setAttribute( "onchange", "atualizarTotal( this, "+ nrLinha + " ); atualizarTotalGeral( 'eaeqtdalunos4[]', 'totalAlunos4' );" );
				}
				else{ // IE
					campo6.attachEvent( "onchange", function() { atualizarTotal( this, nrLinha ); atualizarTotalGeral( 'eaeqtdalunos4[]', 'totalAlunos4' ); } );
				}
				// onkeyup
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo6.setAttribute("onkeyup","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo6.attachEvent( "onkeyup", function() { campo6.value = mascaraglobal('#####', campo6.value) } );
				}
								
				celula7.appendChild( campo6 );
				celula7.style.textAlign = 'center';
				
				var campo7 = document.createElement( "input" );
				celula8 = linha.insertCell ( 8 );				
				campo7.setAttribute( "type", "text" );
				campo7.setAttribute( "name", 'eaeqtdalunos5[]' );
				campo7.setAttribute( "size", "5" );
				campo7.setAttribute("class","normal");
				campo7.setAttribute("onmouseover","MouseOver(this)");
				campo7.setAttribute("onfocus","MouseClick(this)");
				campo7.setAttribute("onmouseout","MouseOut(this)");
				campo7.setAttribute("onblur","MouseBlur(this)");			
				campo7.setAttribute( "id", "eaeqtdalunos5_"+nrLinha );
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					campo7.setAttribute( "onchange", "atualizarTotal( this, "+ nrLinha + " ); atualizarTotalGeral( 'eaeqtdalunos5[]', 'totalAlunos5' );" );				
				}
				else{ // IE
					campo7.attachEvent( "onchange", function() { atualizarTotal( this, nrLinha ); atualizarTotalGeral( 'eaeqtdalunos5[]', 'totalAlunos5' ); } );				
				}
				// onchange
				if(window.addEventListener){ // Mozilla, Netscape, Firefox				
					campo7.setAttribute("onkeyup","this.value=mascaraglobal('#####', this.value);");
				}
				else{ // IE				
					//campo7.attachEvent( "onkeyup", function() { campo7.value = mascaraglobal('#####', campo7.value) } );
				}
							
				celula8.appendChild( campo7 );
				celula8.style.textAlign = 'center';
				
				var campo8 = document.createElement( "input" );
				celula9 = linha.insertCell ( 9 );				
				campo8.setAttribute( "type", "text" );
				campo8.setAttribute( "name", "totalAlunos[]" );
				campo8.setAttribute( "id", "totalAlunos_" + nrLinha );
				campo8.setAttribute( "size", "4" );
				campo8.setAttribute( "disabled", "disabled" );
				campo8.setAttribute("class","normal");	
				campo8.setAttribute("onmouseover","MouseOver(this)");
				campo8.setAttribute("onfocus","MouseClick(this)");
				campo8.setAttribute("onmouseout","MouseOut(this)");
				campo8.setAttribute("onblur","MouseBlur(this)");				
				celula9.appendChild( campo8 );
				celula9.style.textAlign = 'center';
				
				/* Final dos campos */
			
				/*		
				var atingiu_maximo = false;
				// verifica se há limite de itens que podem ser selecionados
				// verifica se limite foi ultrapassado			
				if ( maximo != 0 && itens_selecionados >= maximo )
				{
					checkbox.checked = false;
					atingiu_maximo = true;
					alert( 'A quantidade máxima de itens que podem ser selecionados é <?php echo $maximo ?>.' );
				}
				
				if ( atingiu_maximo == false )
				{
									
				}
				*/
				
				document.getElementById( 'esaqtdtecnicos').value = Math.ceil( parseInt( nrLinha + 1 )/25 );
				document.getElementById( 'esaqtdescolas').value = Math.ceil( parseInt( nrLinha + 1 ) );
			}
			else{
				document.getElementById( 'esaqtdtecnicos').value = Math.ceil( parseInt( nrLinha - 1 )/25 );
				document.getElementById( 'esaqtdescolas').value = Math.ceil( parseInt( nrLinha + 1 ) );
				
				if ( itens_selecionados > 0 )
				{
					itens_selecionados--;
				}
				
				var linha = document.getElementById( "linha_" + cod );
				
				tabela.deleteRow( linha.rowIndex );
			}
			
			return true;	
			
		}
	
	//-->
	</script>
		
	<?php
	}
	else{
		echo "Não foi solicitada Adesão ao Programa Escola da Terra.";
	}
}
?>	