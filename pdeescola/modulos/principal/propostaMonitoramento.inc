<?php
//verificaEscolaPagaWs();
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

if (!$_SESSION['entid']){
	die('<script>
			history.go(-1);
		 </script>');
}
// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
  echo "<script language=\javascript\" type=\"text/javascript\">
				alert('Nenhum dado para este ano Exercício Base de Comparação.');
			window.history.go(-1);
		</script>";
	exit;	
}

//dbg($_SESSION,1);

# Salvar ou alterar monitoramento
//dbg(count($_POST['dpaid']),1);
if ($_POST['pseid']){
	$tamanho = count($_POST['pseid']);
	for($x=0;$x<$tamanho;$x++){    //criando um for para cada plano
		$pseid = $_POST['pseid'][$x];
		
  		$paametaalcancada = $_POST['paametaalcancada'][$pseid] ? "'".pg_escape_string($_POST['paametaalcancada'][$pseid])."'" : "null";
  		$paametaalcancadasn = $_POST['paametaalcancadasn'][$pseid] ? "'".$_POST['paametaalcancadasn'][$pseid]."'" : "null";
  		$paacomentario = $_POST['paacomentario'][$pseid] ? "'".pg_escape_string($_POST['paacomentario'][$pseid])."'" : "null";
  		
  		$sqlDelete = "delete from pdeescola.planoacaoavaliacao where pseid = {$pseid}";
		$db->executar($sqlDelete);
  		
		$sql = "SELECT pseid FROM pdeescola.planoacaoavaliacao WHERE pseid = ".$pseid;
		$testapseid = $db->pegaUm( $sql );
		
		if( !$testapseid ){	

			if($_POST['paametaalcancada'][$pseid] || $_POST['paacomentario'][$pseid]){
				$sqlSalva = "INSERT INTO 
							      pdeescola.planoacaoavaliacao 
							    ( pseid,
					              paametaalcancada,
					              paametaalcancadasn,
					              paacomentario
					            ) VALUES (
					            	".$pseid.",
					            	".$paametaalcancada.",
					            	".$paametaalcancadasn.",
					            	".$paacomentario.")";
				$db->executar($sqlSalva);
			}
		} 
			
	}

	$db->commit();

	echo '<script type="text/javascript"> alert("Operação realizada com sucesso!");</script>';
	echo"<script>window.location.href = 'pdeescola.php?modulo=principal/propostaMonitoramento&acao=A';</script>";
	die;
	
}
			
			

$entid = $_SESSION['entid'];

//checagem de documentos
include_once APPRAIZ . 'includes/workflow.php';
			
//criarDocumento( $entid );
$docid 			   = pegarDocid($entid);
$_SESSION['pdeid'] = pegarPdeid($entid);

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

//criando e exibindo Aba
echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A");
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Autoavaliação';

//Carrega total geral	
$sql = "SELECT
	 SUM(ea.eavvalor::integer) AS eavid
	FROM
	 pdeescola.questaoavaliacaoplano qap
	 INNER JOIN pdeescola.questaoavaliacaoplano qap1 ON qap1.qapidpai = qap.qapid
	 INNER JOIN pdeescola.questaoavaliacaoplano qap2 ON qap2.qapidpai = qap1.qapid
	 INNER JOIN pdeescola.avaliacaoplano ap ON ap.qapid = qap2.qapid
	 INNER JOIN pdeescola.escalaavaliacao ea ON ea.eavid = ap.eavid
	WHERE
	 qap.qapano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
qap.qapidpai IS NULL AND
ap.pdeid = {$_SESSION['pdeid']}";
$total_geral = $db->pegaUm($sql);
?>
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<script>function verificaPendenciaMonitoramento(){return windowOpen('pdeescola.php?modulo=principal/instrumento3/exibePendenciasMonitoramento&acao=A', 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');}</script>
<?=cabecalhoPDE($orientacoes = 'Lembre-se de convidar o Grupo de Sistematização para responder à Avaliação, exercendo a gestão democrática e participativa.'); ?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<?=montaTreePropostaMonitoramento();?>
				
				<?if($total_geral){echo '<B>Total Geral:  </B>'.$total_geral;}?>
				
				<br><br>
				
				<fieldset style="width: 97%; background: #fff;"  >
						<legend>PARTE III - Avaliação de Resultados</legend>
						<br>
						&nbsp;&nbsp;&nbsp;&nbsp;
						&nbsp;&nbsp;&nbsp;&nbsp;
						<b>Observação:&nbsp;&nbsp;O preenchimento da Parte III não é obrigatório para habilitar a Parcela Complementar</b>
						<br><br>
						<!-- Inicio monitoramento -->
						<?php
						
							$sqlDados = "SELECT   
											pa.plaindicadormeta,
											ps.psedescricao,
											pa.pseid,
									     	pav.paametaalcancada,
									     	pav.paametaalcancadasn,
									     	pav.paacomentario
										 FROM
												pdeescola.planosuporteestrategico ps
										 INNER JOIN 
												pdeescola.planoacao pa ON pa.pseid = ps.pseid
										 LEFT JOIN
										    pdeescola.planoacaoavaliacao pav on pav.pseid = ps.pseid
								 		 WHERE
												ps.pdeid = ".$_SESSION["pdeid"];
							
						
							//ver( $sqlDados );
						    //if($plaid != '' ){			
								$dados = $db->carregar( $sqlDados );
								$dados = ($dados) ? $dados : array();
							//}
							
						?>
						<form name=formulario id=formulario method=post>
								<?=subCabecalho('Meta prevista');?>
						
									<table width="95%" align="center" border="3" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
										<thead>
										<tr>
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												Nº
											</td> 
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												Meta prevista 
											</td>
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												Indicador da meta
											</td>
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												A meta foi alcançada?
											</td>
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												Meta alcançada
											</td>
											<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
												Fatores de sucesso e/ou dificuldades para alcançar a meta
											</td>
										</tr> 
										</thead>
										
										<tbody>
											<?php
											if(count($dados) == 0){
												echo "<tr><td colspan='5' align='center'>
														<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' style='color:333333;' class='listagem'>
															<tr>
																<td style='color:red;' align='center';>
																	Não foram encontrados Registros.
																</td>
															</tr>
													  	</table>
													  </td>
													  </tr>";
											}else{
												//loop preenche dados do Desdobramento das Metas em Planos de Ação
												for( $k = 0; $k < count( $dados ); $k++ ){
													
													//if( $dados[$k]['moaid'] != '' ){
													//	$boHabilitado = false;
													//}
													?>
												
													<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Número">				
															<b><?=$k+1 ?></b>
															<input type='hidden' id='pseid' name='pseid[]' value='<?=$dados[$k]['pseid']?>'>
														</td> 
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Descrição">
															<?=$dados[$k]['psedescricao']?>
														</td>
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Descrição">
															<?=$dados[$k]['plaindicadormeta']?>
														</td>
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Justificativa">
															<?$paametaalcancadasn =  $dados[$k]['paametaalcancadasn'];?>
															<input type="radio" name="paametaalcancadasn[<?=$dados[$k]['pseid']?>]" value="t" <?if($paametaalcancadasn == 't') echo 'checked';?>> Sim
															<br>
															<input type="radio" name="paametaalcancadasn[<?=$dados[$k]['pseid']?>]" value="f" <?if($paametaalcancadasn == 'f') echo 'checked';?>> Não
														</td>
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Justificativa">
															<?$paametaalcancada =  trim(stripslashes($dados[$k]['paametaalcancada']));?>
															<?=campo_textarea('paametaalcancada['. $dados[$k]['pseid'] .']', 'N', 'S', '', 25, 5,'','','','','','',$paametaalcancada);?>
														</td>
														<td style="border-color: #c0c0c0; border-right: 1px solid #c0c0c0;" title="Justificativa">
															<?$paacomentario =  trim(stripslashes($dados[$k]['paacomentario']));?>
															<?=campo_textarea('paacomentario['. $dados[$k]['pseid'] .']', 'N', 'S', '', 45, 5,'','','','','','',$paacomentario);?>
														</td>
													</tr>
												<?}
											}?>
										</tbody>
									</table>
									
									<br>
									
									<div align="center">
										<input type="button" name="botao" value="Salvar" onclick="validaForm()">
									</div>									
							</form>
														
				</fieldset>
			</td>
			<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
			<?php
				// Barra de estado atual e ações e Historico
				wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );			
			?>
			</br>	
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="Cadastramento de parecer">
					<strong>Preenchimento</strong>
				</span>
			</td>
		</tr>
		<tr style="text-align:center;">
			<td style="font-size:7pt; text-align:center;">
              <a id="verificaPendenciaMonitoramento" title="Verificar pendencias" onclick="verificaPendenciaMonitoramento();">Verificar pendências Autoavaliação</a>
			</td>
		</tr>
	</table>
				
			</td>
		</tr>
		
	</tbody>
</table>	

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script>
function validaForm(){
	/*
	var arDpaid = eval('<?=simec_json_encode($arDpaid)?>');
	var campoVazio = false;	
	for (i=0; i <  arDpaid.length; i++){
		var dpaid = arDpaid[i];

		var justif    = document.getElementsByName('moajustificativa[' + dpaid + ']')[0];
		
		if (justif.value.length > 1000){
			alert('Você excedeu o nº máximo de 1000 caracteres no campo Justificativa!"');
			justif.focus();
			return false;
		}
		
	}
	*/

	document.formulario.submit();
}
</script>

