<?php
 
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";

function analiseEngenharia($preid, $preidpai = null)
{

	$preid = $preidpai ? $preidpai : $preid;
	
	$obPreObraControle = new PreObraControle();
	$arDadosAnalise = $obPreObraControle->recuperarNomeAnalistaObra($preid);
	
	$arQuestionarioIndeferimento = $obPreObraControle->verificaIndeferimentoQuestionario($preid);
	$arQuestionarioIndeferimento = $arQuestionarioIndeferimento ? $arQuestionarioIndeferimento : array();
	
	$consideracoesFinais = $obPreObraControle->recuperarConsideracoesFinais($preid);
	
	foreach($arQuestionarioIndeferimento as $dados){
		//a definir
		if($dados['itpid'] == QUEST_PAR_INDEFERIMENTO){
			$obIndeferidoQuestionario = true;
		}
	}
	
	//ver($arQuestionarioIndeferimento);
	
	$arDataHora = explode(" ", $arDadosAnalise['data']);
	$arData = explode("-", $arDataHora[0]);
	
	switch($arData[1]){
		case '1':
			$mes = "Janeiro";
			break;
		case '2':
			$mes = "Fevereiro";
			break;
		case '3':
			$mes = "Março";
			break;
		case '4':
			$mes = "Abril";
			break;
		case '5':
			$mes = "Maio";
			break;
		case '6':
			$mes = "Junho";
			break;
		case '7':
			$mes = "Julho";
			break;
		case '8':
			$mes = "Agosto";
			break;
		case '9':
			$mes = "Setembro";
			break;
		case '10':
			$mes = "Outubro";
			break;
		case '11':
			$mes = "Novembro";
			break;
		case '12':
			$mes = "Dezembro";
			break; 
	}
	
	//A definir
	if($arDadosAnalise['ptoid'] == 7 || $arDadosAnalise['ptoid'] == 2){
		$areaConstruida = "1211.92";
	}elseif($arDadosAnalise['ptoid'] == 3 || $arDadosAnalise['ptoid'] == 6){
		$areaConstruida = "564.50";
	}elseif($arDadosAnalise['ptoid'] == 5){
		$areaConstruida = "980.00";
	}
	
	//ver($arDadosAnalise['esdid'], WF_TIPO_OBRA_INDEFERIDA);
	?>
	<div style="margin-left:30px;">
		<center>
		<img width="100" alt="" src="http://simec.mec.gov.br<?php //echo $_SERVER['SERVER_NAME'] ?>/imagens/brasao.gif"><br/>
		MINISTÉRIO DA EDUCAÇÃO<br/>
		FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/> 
		SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF<br/>
		Telefone: (61) 2022-4350 - E-mail: cgaap@fnde.gov.br<br/><br/> 
		</center>
		<div style="font-family: arial, sans-serif;">
			<p align="left"><b>Análise Técnica de Engenharia - CGEST / DIRPE / FNDE</b></p><br/>
			<p align="right">Brasília, <?php echo $arData[2]  ?> de <?php echo $mes ?> de <?php echo $arData[0] ?> </p>
			<br/>
			
			<style>
			
			table.tabelaQuestionarioIdentificacao {
				border-left: 1px solid #000000;
				border-right: 1px solid #000000;
				border-top: 1px solid #000000;
				border-bottom: 1px solid #000000;
				/* background-color:#cccccc; */
				font-size: 14px;	
			}
			 
			</style>
			<table class="tabelaQuestionarioIdentificacao" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
				<tr>
					<td style="border-bottom:1px solid black;" colspan="2" align="center"><b>Identificação</b></td>		
				</tr>
				<tr>
					<td width="140" style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Número:</b></td>
					<td style="border-bottom:1px solid black;"><?php echo $arDadosAnalise['preid'] ?></td>
				</tr>
				<tr>
					<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Interessado:</b></td>
					<td style="border-bottom:1px solid black;">Prefeitura Municipal de <?php echo $arDadosAnalise['mundescricao']."-".$arDadosAnalise['estuf']; ?></td>
				</tr>
				<tr>
					<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Tipo de obra:</b></td>
					<td style="border-bottom:1px solid black;"><?php echo $arDadosAnalise['ptodescricao'] ?></td>
				</tr>
				<tr>
					<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;">
						<b>Objeto da Análise:</b>
					</td>
					<td style="border-bottom:1px solid black;">
						Análise Técnica de Engenharia dos documentos apresentados pelo interessado para construção de <?php echo $arDadosAnalise['ptodescricao'] ?> no bairro <?php echo $arDadosAnalise['prebairro'] ?> na cidade de <?php echo $arDadosAnalise['mundescricao'] ?> - <?php echo $arDadosAnalise['estuf'] ?>
					</td>
				</tr>
			</table>
		</div>
		<br/>
		<?php if($arDadosAnalise['poaindeferido'] == "S" && !$obIndeferidoQuestionario): ?>
			<?php
			$obImprime = new QImpressao( array('cabecalho' => 'N', 'qrpid' => $arDadosAnalise['qrpid']) );
			echo $obImprime->montaArvore();
			?>
			<br/>
			<div style="font-family: arial, sans-serif;">
				<table class="tabelaQuestionarioIdentificacao" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
					<tr>
						<td style="border-bottom:1px solid black;" colspan="2" align="center"><b>Análise de custo</b></td>		
					</tr>
					<?php 
					if($areaConstruida){	
						$calculoPorMetro = $arDadosAnalise['ppototal']/$areaConstruida;
					}
					?>
					<tr>
						<td width="140" style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Área construída:</b></td>
						<td style="border-bottom:1px solid black;"><?php echo formata_valor($areaConstruida) ?>m²</td>
					</tr>
					<tr>
						<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Custo:</b></td>
						<td style="border-bottom:1px solid black;">R$ <?php echo formata_valor($arDadosAnalise['ppototal']) ?></td>
					</tr>	
					<tr>
						<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;"><b>Custo/M²:</b></td>
						<td style="border-bottom:1px solid black;">R$ <?php echo formata_valor($calculoPorMetro) ?></td>
					</tr>
					
				</table>
				<br/>
				<?php if(!empty($consideracoesFinais['cmddsc'])): ?>
					<p><b>CONSIDERAÇÕES FINAIS</b></p>
					<p><?php echo $consideracoesFinais['cmddsc'] ?></p>
					<br/>				
				<?php endif; ?>
				<p><b>CONCLUSÃO</b></p>
				<p>Os documentos necessários para a aprovação do processo <?php echo ($arDadosAnalise['esdid'] == WF_TIPO_OBRA_APROVADA) ? "FORAM" : "NÃO FORAM" ?> apresentados adequadamente.</p>
				<p>Face às considerações, informamos que somos pela <?php echo ($arDadosAnalise['esdid'] == WF_TIPO_OBRA_APROVADA) ? "APROVAÇÃO" : "DILIGÊNCIA" ?> da proposta apresentada.</p>
				<br/>
				<p>À consideração superior,</p><br/><br/>			
				<center>
				<b><?php echo $arDadosAnalise['usunome'] ?><!-- Nome do Analista --></b><br/>
				Analista de Projetos - CGEST/FNDE<br/><br/><br/>
				</center>
				<p>De acordo, encaminhe-se para as providências cabíveis:</p><br/><br/>
				<center>
				<b>FABIO LÚCIO DE ALMEIDA CARDOSO</b><br/>
				Coordenador Geral de Infraestrutura Educacional - CGEST<br/>
				</center>
				<br/>
			<?php elseif($arDadosAnalise['poaindeferido'] == "N"): ?>
				<div style="font-family: arial, sans-serif;">
					<table class="tabelaQuestionarioIdentificacao" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
						<!-- tr>
							<td style="border-bottom:1px solid black;" align="center"><b>Justificativa</b></td>		
						</tr -->		
						<!-- tr>					
							<td style="border-bottom:1px solid black;border-right:1px solid black;background-color:#d1d1d1;">
								<b>Com base nos documentos apresentados, fotos, estudo de demanda, planta de localização, é justificável a construção da obra no local?</b>
								Não.
							</td>
						</tr -->						
						<tr>
							<td style="border-bottom:1px solid black;">
								<b>Justificativa</b>: <?php echo $arDadosAnalise['poajustificativa'] ?>
							</td>					
						</tr>	
					</table>				
					<br/>
					<?php if(!empty($consideracoesFinais['cmddsc'])): ?>
						<p><b>CONSIDERAÇÕES FINAIS</b></p>
						<p><?php echo $consideracoesFinais['cmddsc'] ?></p>
						<br/>				
					<?php endif; ?>
					<p><b>CONCLUSÃO</b></p>
					<p>Face às considerações, informamos que somos pelo INDEFERIMENTO da proposta apresentada.</p>
					<br/><br/>
					<p>À consideração superior,</p><br/><br/>				
					<center>
					<b><?php echo $arDadosAnalise['usunome'] ?><!-- Nome do Analista --></b><br/>
					Analista de Projetos - CGEST/FNDE<br/><br/><br/>
					</center>
					<p>De acordo, encaminhe-se para as providências cabíveis:</p><br/><br/>
					<center>
					<b>FABIO LÚCIO DE ALMEIDA CARDOSO</b><br/>
					Coordenador Geral de Infraestrutura Educacional - CGEST<br/>
					</center>
					<br/>
			<?php elseif($obIndeferidoQuestionario): ?>
				<style>
					table.tabelaQuestionario {
						border-left: 1px solid #000000;
						border-right: 1px solid #000000;
						border-top: 1px solid #000000;
						border-bottom: 1px solid #000000;
						/* background-color:#cccccc; */
						font-size: 14px;
						font-weight: bold;
					}
			
					table.tabelaGrupo {
						border-left: 1px solid #000000;
						border-right: 1px solid #000000;
						border-top: 1px solid #000000;
						border-bottom: 1px solid #000000;
						background-color:#d1d1d1;
						font-size: 14px;
					}
					
					td.bordaDireita {
						border-right: 1px solid #000000;
						border-left: 1px solid #000000;
						border-top: 1px solid #000000;
						border-bottom: 1px solid #000000;
						background-color:#ffffff;
						font-size: 14px;
						width:300px;
					}
				</style>
				
				<div style="font-family: arial, sans-serif;">
					<table class="tabelaQuestionario" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
							<tr>
								<td align='center' style="background:white;">
									<b>Análise de Engenharia</b>
									<table width='100%' class='tabelaGrupo'>
										<tr>
											<td>
												<b>1 - Relatório de Vistoria do Terreno</b>
											<?php foreach($arQuestionarioIndeferimento as $dados): ?>
												<tr>
													<td class='bordaDireita'>
														<b><?php echo $dados['pertitulo'];?>&nbsp;&nbsp;&nbsp;</b><?php echo $dados['itptitulo'] ?>
														<?php if( $dados['indeferida'] == 1 ){ ?>
															<tr>
																<td class='bordaDireita'>
																	<b>Observação:&nbsp;&nbsp;&nbsp;</b><?php echo $dados['justificativa'] ?>
																</td>
															</tr>
														<? } ?>
													</td>
												</tr>
											<?php endforeach; ?>	
											</td>
										</tr>
									</table>	
								</td>
							</tr>
					</table>				
					<br/>
					<?php if(!empty($consideracoesFinais['cmddsc'])): ?>
						<p><b>CONSIDERAÇÕES FINAIS</b></p>
						<p><?php echo $consideracoesFinais['cmddsc'] ?></p>
						<br/>				
					<?php endif; ?>
					<p><b>CONCLUSÃO</b></p>
					<p>Face às considerações, informamos que somos pelo INDEFERIMENTO da proposta apresentada.</p>
					<br/>
					<p>À consideração superior,</p>
					<br/><br/>				
					<center>
					<b><?php echo $arDadosAnalise['usunome'] ?><!-- Nome do Analista --></b><br/>
					Analista de Projetos - CGEST/FNDE<br/><br/><br/>
					</center>
					<p>De acordo, encaminhe-se para as providências cabíveis:</p><br/><br/>
					<center>
					<b>FABIO LÚCIO DE ALMEIDA CARDOSO</b><br/>
					Coordenador Geral de Infraestrutura Educacional - CGEST<br/>
					</center>
					<br/>
			<?php endif; ?>
		</div>
	</div>
	
<?php
}

$preid = $_SESSION['par']['preid'];

analiseEngenharia($preid);

$obPreObraControle = new PreObraControle();
$idReformulacao = $obPreObraControle->recuperarRefornulacao($preid);

if($idReformulacao){
	echo '<p>&nbsp;</p><center><h1>Cópia da Obra</h1></center>';
	analiseEngenharia($preid, $idReformulacao);
}

if($_REQUEST['requisicao'] == 'gerarPdf'){

	$html = ob_get_contents();
	ob_clean();
	
	$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));
	
	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=analise_engenharia_".$_REQUEST['preid']);
	echo $contents;
	exit;
	
}else{
	
	if(!isset($_REQUEST['copia'])){
		echo "<script>
				function gerarPdf()
				{
					document.location.href = document.location.href+'&requisicao=gerarPdf';
				}
			  </script>";
		echo '<center><input type="button" value="Gerar Pdf" onclick="gerarPdf()" /></center>';
	}
}

?>