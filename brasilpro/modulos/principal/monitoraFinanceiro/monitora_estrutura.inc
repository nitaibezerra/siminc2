<?
/************************* INCLUDES **********************************/
include_once(APPRAIZ.'www/brasilpro/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'brasilpro/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ .'brasilpro/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'brasilpro/classes/HistoricoMonitoramentoConvSubac.class.inc');
global $db;
/************************* FUNÇÕES AJAX **********************************/
//INICIA MONITORAMENTO
if($_REQUEST['requisicao'] == 'iniciaMonitoramento'){
	$resposta = insereMonitoramento($_REQUEST['prsid']);
	echo $resposta;
	die();
}

//ATUALIZA LISTA DE MONITORAMENTOS
if($_REQUEST['requisicao'] == 'atualizaListaHistoricoMonitoramento'){
	echo atualizaListaHistoricoMonitoramento($_REQUEST['prsid']);
	die();
}

//CARREGA SUBAÇÕES
if($_REQUEST['requisicao'] == 'carregasubacao'){
	$conteudo = carregaSubacoes($_REQUEST['prsid'], $_SESSION['ano'], $_REQUEST['ssuid']);
	echo $conteudo;
	die();
}

//SALVA STATUS DA SUBAÇÃO
if($_REQUEST['requisicao'] == 'salvarStatusSub'){
	$resposta = insereStatusSubacao($_REQUEST['ssuid'], $_REQUEST['sbaid'],$_REQUEST['prsid']);
	echo($resposta);
	die();
}

// CARREGA ITENS COMPOSIÇÃO
if($_REQUEST['requisicao'] == "carregaitens"){
	$conteudo = carregaItensComposicao($_REQUEST['sbaid'], $_REQUEST['ssuid'], $_REQUEST['prsid'], $_REQUEST['hmsid'],$_REQUEST['ano'] );
	echo $conteudo;
	die();
}

//FINALIZAR MONITORAMENTO
if($_REQUEST['requisicao'] == "finalizarmonitoramento" ){
	$resposta = finalizarMonitoramento($_REQUEST['prsid']);
	echo $resposta;
	die();
	
}
/************************* FIM FUNÇÕES AJAX ********************************/

/************************* DECLARAÇÃO DE VARIAVEIS *************************/
$instrumentoUnidade = $_SESSION['inuid'];
$prsid				= $_REQUEST['prsid'];
$hmcid				= $_REQUEST['id'];

/************************* CARREGA CONVÊNIOS *******************************/
if($instrumentoUnidade && $_SESSION['ano']){
	//unset($_SESSION['hmc']);
	$convenios = carregaConvenio($_SESSION['ano'], $instrumentoUnidade, $prsid); 
	if(!is_array($convenios)){
		unset($_SESSION['hmc']);
		alert("Não existe convênios para este ano.");
	}
}

// RECUPERA O MÊS
if($_REQUEST['mes']){
	if($_REQUEST['mes'] != 10 && $_REQUEST['mes'] != 11 && $_REQUEST['mes'] != 12){
		$mesBusca = '0'.$_REQUEST['mes'];
		$_SESSION['mes'] = $mesBusca;
	}else{
		
		$mesBusca = $_REQUEST['mes'];
		$_SESSION['mes'] = $mesBusca;
	}
}

$sql = "SELECT DISTINCT 
		to_char(hmcdatamonitoramento, 'MM') as mes 
FROM cte.historicomonitoramentoconvenio h
INNER JOIN cte.projetosape ps ON h.prsid = ps.prsid
LEFT join cte.historicomonitoramentoconvsubac  hs ON hs.hmcid = h.hmcid
LEFT JOIN  cte.historicoconvitemcomposicao hi ON hi.hmsid = hs.hmsid AND (hi.scsid = 1 OR hi.scsid = 2)
LEFT join cte.composicaosubacao c on c.cosid = hi.cosid 
WHERE ps.inuid= ".$instrumentoUnidade." AND ps.prsano = ".$_SESSION['ano']." AND ps.prsid = ".$prsid." AND to_char(hmcdatamonitoramento, 'MM') = '".$mesBusca."'";

$mesTela =  $db->pegaUm($sql);
$dataFormatada = new Data();
$mesTela = $dataFormatada->mesTextual($mesTela);

/************************* DEIXA SELECIONADO O ANO DA SESSION, E MOSTRA MÊS TEXTUAL NA MSG DE ENCERRAR O MONITORAMENTO **************/

if($_SESSION['ano'] == "2007"){
	$selected07 = "selected";
}else if($_SESSION['ano'] == "2008"){
	$selected08 = "selected";
}else if($_SESSION['ano'] == "2009"){
	$selected09 = "selected";
}else if($_SESSION['ano'] == "2010"){
	$selected10 = "selected";
}else if($_SESSION['ano'] == "2011"){
	$selected11 = "selected";
}

$obdata = new Data();
$mesTextualMonitora = $obdata->mesTextual($_REQUEST['mes']);

/************************* CABEÇALHO E TÍTULO ******************************/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
$url = "?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia=".$_SESSION['ano'];
cte_montaTitulo( $titulo_modulo, '&nbsp;', $url );
?>
<html>
	<head>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/prototype.js"></script>
		<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
		<link rel="stylesheet" type="text/css" href="../../brasilpro/monitoraFinanceiro/includes/monitora_financeiro.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/LyteBox/lytebox.css"/>
		<!--[if IE]>
			<link rel="stylesheet" type="text/css" href="../../brasilpro/monitoraFinanceiro/includes/monitora_financeiro_ie.css"/>
		<![endif]-->
	</head>
	<body>
		<!-- CARREGANDO DADOS AJAX -->
		<div id="loader-container" style="display:none;">
	    	<div id="loader">
	    		<img src="../../imagens/wait.gif" border="0" align="middle">
	    		<span>Aguarde! Carregando Dados...</span>
	    	</div>
	    </div>
		<!-- FORMULÁRIO DOS CONVÊNIOS -->
		<form name="formulario" id="formulario" action="" method="post">	
		<?php if(is_array($convenios)){ //Se existe convênio. Só carrega dados se existir convênios
				foreach($convenios as $convenio){
					$dadosConvenios = recuperaDadosDoMonitoramento($convenio['prsid'], $_SESSION['mes']);
					if(is_array($dadosConvenios)){
						foreach($dadosConvenios as $dados){
							$convenio['hmcstatus'] = $dados['hmcstatus'];
							if($dados['hmcstatus'] == 'I'){//Se o status do monitoramento for Iniciado carrega para Session o id do Monitoramento
								carregaMonitoramentosParaSession($dados['prsid'],$dados['hmcid']);	
							}else{
								retiraMonitoramentosDaSession($dados['prsid'],$dados['hmcid']);	
								continue;
							}
						}
					}
		?>
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1" width="95%">
			<tr>
				<td  class="tdTopo" style="text-align: left;" >Monitoramento do mês: <?=$mesTela; ?> </td>
				<td width="170px;" class="tdTopo" style="text-align: left;" >Status deste monitoramento: </td>
			</tr>
			<tr>
				<th class="textoEsquerda">
				<div class="div_imagens" id="img_convenios_<?=$convenio['prsid'];?>">	
					<img src="../../imagens/mais.gif" id="mais_<?=$convenio['prsid'];?>" name="mais_<?=$convenio['prsid'];?>" onclick="carregaSubacoes(<?=$convenio['prsid'];?>, 0);"/> 
					<img src="../../imagens/menos.gif" style="display:none;" id="menos_<?=$convenio['prsid'];?>" name="menos_<?=$convenio['prsid'];?>" onclick="disableSubacoes(<?=$convenio['prsid'];?>);"/>
				</div>
				Convênio nº: <?=$convenio['prsnumconvsape'];?>
				</th>
				<th id="tdIniciaMonitoramento_<?=$convenio['prsid'];?>" class="td_btnsTopo">
				<?php 
					if($convenio['hmcstatus'] == 'I'){ // Se monitoramento não iniciado carrega Btn de iniciar nomnitoramento. 
				?>
					Em andamento.
				<? }else if($convenio['hmcstatus'] == 'F'){ ?>
					Finalizado.
				<? } ?>
				</th>
			</tr>
			<tr>
				<td colspan="2">
					<div style="display:none;" id="linha_<?=$convenio['prsid'];?>" class="linha">
						<img  src="../../includes/dtree/img/line.gif"/><br>
						<img  src="../../includes/dtree/img/joinbottom.gif"/>
					</div>
					<!-- DIV ONDE CARREGA A LISTA DE SUBAÇÕES -->
					<div class="listaSubAcoes" id="listaAcoes_<?=$convenio['prsid'];?>"></div>
				</td>
			</tr>
			
			<?php 
				$style = 'style="display:none;"';
				$styleVoltar = 'style="display:none;"';
				if($convenio['hmcstatus'] == 'I'){ // Caso iniciado o monitoramento carrega o Btn de Finalizar o monitoramento. 
					$style = '';
				}else if($convenio['hmcstatus'] == 'F'){
					$styleVoltar = '';
				}
			?>
			<!-- BTN FINALIZAR MONITORAMENTO -->
			
			<tr id="trfinalizarMonitora_<?=$convenio['prsid'];?>" <?=$style; ?> >
				<td colspan="2" class="td_btns">
					<input type="button" name="voltar" id="voltar" value="Voltar"  onclick="voltarListaMeses();" />
					<input type="button" name="finalizarMonitora_<?=$convenio['prsid'];?>" id="finalizarMonitora_<?=$convenio['prsid'];?>" value="Finalizar Monitoramento"  onclick="finalizarmonitoramento(<?=$convenio['prsid'];?>);" />
				</td>
			</tr>
			<tr id="trVoltar" <?=$styleVoltar; ?> >
				<td   colspan="2" class="td_btns">
					<input type="button" name="voltar" id="voltar" value="Voltar"  onclick="voltarListaMeses();" />
				</td>
			</tr>
		</table>
		<?php
				}
		 ?>
		
		<br>
		<!-- LISTA DE HISTÓRICO DE MONITORAMENTO -->
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<th>Histórico de Monitoramentos</th>
			</tr>
		</table>
		<table align="center" width="100%" >
			<tr>
				<td id="listaMonitoramentos">
				<?php 	listaHistoricoMonitoramento($instrumentoUnidade,$_SESSION['ano'], $prsid);  ?>
				</td>
			</tr>
		</table>
		<?php		
			} // Fim se existe convênio
		?>
		
		</form>
		<script type="text/javascript">
		/****************** VARIAVEIS GLOBAIS ***********************/
			var convenio 	= <?=$convenios['prsid'] = $convenios['prsid']== NULL ? 0 : $convenios['prsid'];?>;
			var anoConvenio = <?=$_SESSION['ano'] = $_SESSION['ano']== NULL ? 0 : $_SESSION['ano'];?>;
			
		/****************** FUNÇÕES ***********************/
			
			function voltarListaMeses(){
				var url 	= '?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia='+anoConvenio;
				window.location = url;
			}
			
			/**
			 * function abreTelaCadastroValoresConvenio();
			 * desccrição 	: Abre tela para cadastra os dados financeiro do Convênio. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function abreTelaCadastroValoresConvenio(prsid){
				url 	= 'brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_convenio&acao=A&prsid='+prsid;
				title 	= 'Informativos de Valores de Convênio';
				width 	= '800';
				height 	= '500';
				inicialytebox(url,title,width,height);
				return true;
			}
			
			/**
			 * function carregarConvenios();
			 * desccrição 	: Recupera a lista de convênios de acordo com o ano de referencia. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function carregarConvenios(){
			
				var ano     = $('anoreferencia').value;
				var formAno = $('formAno');
				if(!ano){
					alert("Selecione um ano de referência.");
					return false;
				}else{
					formAno.submit();
				}	
			}
			
			/**
			 * function iniciaMonitoramento();
			 * desccrição 	: Inicia um novo monitoramento Financeiro do convênio selecionado. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function iniciaMonitoramento(convenioSelecionado){
				if(confirm('Deseja criar um novo monitoramento?')){
					$('loader-container').show();
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=iniciaMonitoramento&prsid='+convenioSelecionado,
						asynchronous: false,
						onComplete: function(resposta)
						{	
							alert(resposta.responseText);
							if(resposta.responseText == "Monitoramento iniciado com sucesso."){
								atualizaListaHistoricoMonitoramento(convenioSelecionado);
								carregaSubacoes(convenioSelecionado,0);
								$('tdIniciaMonitoramento_'+convenioSelecionado).innerHTML = "Em Andamento.";
								$('trfinalizarMonitora_'+convenioSelecionado).style.display = '';
								$('trVoltar').style.display = 'none';
							}
							$('loader-container').hide();
						}
					});
				}
			}
		
			/**
			 * function finalizarmonitoramento(fimConvenio);
			 * parametros	: id do convênio.
			 * desccrição 	: finaliza o convênnio. 
			 * author 		: Thiago Tasca Barbosa
			*/
			function finalizarmonitoramento(fimConvenio){
				var cpf = <?=$_SESSION["usucpf"];?>; 
				var nome = "<?=$_SESSION["usunome"];?>"; 
				var ano = "<?=$_SESSION['ano'];?>";
				var mes = "<?=$mesTextualMonitora;?>";
				
				if(confirm('Eu, '+nome+', CPF '+cpf+', desejo finalizar o monitoramento do mês de '+mes+' de '+ano+'? \n Estou ciente de que, após a finalização, os dados não podem ser alterados e de que as  informações inseridas no sistema são de inteira responsabilidade do convenente.  Certifico a veracidade das informações inseridas.')){
					$('loader-container').show();
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=finalizarmonitoramento&prsid='+fimConvenio,
						onComplete: function(resposta)
						{	
							alert(resposta.responseText);
							if( resposta.responseText == "Monitoramento encerrado com sucesso."){
								$('trfinalizarMonitora_'+fimConvenio).style.display = 'none';
								$('trVoltar').style.display = '';
								$('tdIniciaMonitoramento_'+fimConvenio).innerHTML = 'Finalizado.' //'<input type="button" name="salvar" value="Iniciar Monitoramento"  onclick="iniciaMonitoramento('+fimConvenio+');" />';
								carregaSubacoes(fimConvenio, 0);
								atualizaListaHistoricoMonitoramento(fimConvenio);
							}
							$('loader-container').hide();	
						}
					});
				}
			}
			
			/**
			 * function atualizaListaHistoricoMonitoramento(prsid);
			 * parametros	: id do convênio.
			 * desccrição 	: Atualiza a lista de Monitoramento quando um monitoramento e iniciado e finalizado. 
			 * author 		: Thiago Tasca Barbosa
			*/
			function atualizaListaHistoricoMonitoramento(prsid){
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=atualizaListaHistoricoMonitoramento&prsid='+prsid,
					onComplete: function(resposta)
					{				
						$('listaMonitoramentos').innerHTML = resposta.responseText;
						$('loader-container').hide();	
					}
				});
			}

			/**
			 * function carregaSubacoes(convenio, ssuid);
			 * descscricao 	: Carrega lista de subações quando clicado no btn + do convênio. 
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 * parametros  	: ssuid (id do status da subação)
			 */
			function carregaSubacoes(convenio, ssuid){
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=carregasubacao&prsid='+convenio+'&ssuid='+ssuid,
					asynchronous: false,
					onComplete: function(resposta)
					{	
						$('img_convenios_'+convenio).innerHTML = '<img id="mais_'+convenio+'" onclick="enableSubacoes('+convenio+');" name="mais_'+convenio+'" src="../../imagens/mais.gif" style="display: none;"/><img id="menos_'+convenio+'" onclick="disableSubacoes('+convenio+');" name="menos_'+convenio+'" style="" src="../../imagens/menos.gif"/>'
						$('listaAcoes_'+convenio).style.display = '';
						$('linha_'+convenio).style.display = '';
						$('listaAcoes_'+convenio).innerHTML = resposta.responseText;
						$('menos_'+convenio).style.display = '';
						$('mais_'+convenio).style.display = 'none';
						$('loader-container').hide();	
					}
				});
			}
			
			/**
			 * function enableSubacoes(convenio);
			 * descscricao 	: Habilita subações. Função que troca a função do botão + por - e mostra a lista de subações. 
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas habilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function enableSubacoes(convenio){
				$('menos_'+convenio).style.display = '';
				$('linha_'+convenio).style.display = '';
				$('mais_'+convenio).style.display = 'none';
				$('listaAcoes_'+convenio).style.display = '';
			}
			
			/**
			 * function disableSubacoes(convenio);
			 * descscricao 	: Desabilita subações. Função que troca o o botão - por + e esconde a lista de subações.
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas Desabilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function disableSubacoes(convenio){
				$('menos_'+convenio).style.display = 'none';
				$('linha_'+convenio).style.display = 'none';
				$('mais_'+convenio).style.display = '';
				$('listaAcoes_'+convenio).style.display = 'none';
				
			}
			
			function carregaTelaStatusSubAcao(sbaid, ssuid, prsid,hmsid, ano){
				inicialytebox('brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_SubAcao&acao=A&requisicao=carregar&sbaid='+sbaid+'&ssuid='+ssuid+'&prsid='+prsid+'&hmsid='+hmsid+'&ano='+ano,'Monitora Status Subação','800','250');
				return true;
			}
			
			
			
			/**
			 * function insereStatusSubacao(ssuid, sbaid, prsid, historiocoSub);
			 * descscricao 	: Grava o status da subação no banco de dados.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: sbaid (Id da subação) 
			 * parametros 	: ssuid (status da subação)
			 * parametros 	: prsid (id do convênio)
			 * parametros 	: hmcid (id do Historico do Monitoramento)
			 * parametros 	: historiocoSub (id do monitoramento da subação)
			 */
			function insereStatusSubacao(ssuid, sbaid, prsid, historiocoSub){
				var hmsid = historiocoSub;
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=salvarStatusSub&ssuid='+ssuid+'&sbaid='+sbaid+'&prsid='+prsid,
					onComplete: function(resposta)
					{	
						if(hmsid == 0){
							hmsid = resposta.responseText;
						}
						//alert(hmsid);
						//if( ssuid == <?=EXECUTADA; ?> || ssuid == <?=EMEXECUCAO; ?> ){ // Se o status da subação for em execução ou executada mostra os itens de composição.

							carregaItensComposicao(sbaid, anoConvenio, ssuid, prsid, hmsid);
							return hmsid;
						//}
						$('loader-container').hide();	
					}
				});
			}
			
			/**
			 * function carregaItensComposicao(sbaid, ano, ssuidmonitora, prsid, hmsid);
			 * descscricao 	: Carrega lista de itens de composição por subação.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: sbaid (id da subação)
			 * parametros 	: ano (ano referencia)
			 * parametros 	: ssuidmonitora (id do status da subação )
			 * parametros 	: prsid (id do convênio )
			 * parametros 	: hmsid (id do monitoramento da Subação )
			 * parametros 	: ssuidmonitora (id do status da subação )
			 */
			function carregaItensComposicao(sbaid, ano, ssuidmonitora, prsid, hmsid){
				sbaidPrsid = sbaid.toString()+prsid.toString()+ano.toString();
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=carregaitens&sbaid='+sbaid+'&ssuid='+ssuidmonitora+'&prsid='+prsid+'&hmsid='+hmsid+'&ano='+ano,
					onComplete: function(resposta)
					{	
						$('img_subacoes_'+sbaidPrsid).innerHTML = '<img id="mais_'+sbaidPrsid+'" onclick="enableItensComposicao('+sbaidPrsid+');" name="mais_'+sbaidPrsid+'" src="../../imagens/mais.gif" style="display: none;"/><img id="menos_'+sbaidPrsid+'" onclick="disableItensComposicao('+sbaidPrsid+');" name="menos_'+sbaidPrsid+'" style="" src="../../imagens/menos.gif"/>'; // troca os onclick dos botões + e -.
						$('listaItensComposicao_'+sbaidPrsid).style.display = ''; 	// mostra a lista de itens de composição
						$('linha_'+sbaidPrsid).style.display = ''; 				    // mostra a lista de subações
						$('listaItensComposicao_'+sbaidPrsid).innerHTML = resposta.responseText;
						$('menos_'+sbaidPrsid).style.display = ''; 				    // mostra o botão -
						$('mais_'+sbaidPrsid).style.display = 'none'; 				// oculta o botão +
						$('loader-container').hide();	
						
					}
				});
			}
			
			/**
			 * function enableItensComposicao(sbaid);
			 * descscricao 	: habilita itens composição. Função que troca o o botão + por - e mostra a lista de itens com css. 
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: sbaid (id da subação)
			 */
			function enableItensComposicao(sbaidPrsid){
				$('menos_'+sbaidPrsid).style.display = '';
				$('linha_'+sbaidPrsid).style.display = '';
				$('mais_'+sbaidPrsid).style.display = 'none';
				$('listaItensComposicao_'+sbaidPrsid).style.display = '';
			}
			
			/**
			 * function disableItensComposicao(sbaid);
			 * descscricao 	: habilita itens composição. Função que troca o o botão - por + e esconde a lista de itens com css. 
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: sbaid (id da subação)
			 */
			function disableItensComposicao(sbaidPrsid){
				if($('menos_'+sbaidPrsid)){
					if($('menos_'+sbaidPrsid).style.display != 'none'){
						$('menos_'+sbaidPrsid).style.display = 'none';
					}
				}
					if($('linha_'+sbaidPrsid).style.display != 'none'){
						$('linha_'+sbaidPrsid).style.display = 'none';
					}
					if($('mais_'+sbaidPrsid).style.display != ''){
						$('mais_'+sbaidPrsid).style.display = '';
					}
					if($('listaItensComposicao_'+sbaidPrsid).style.display != 'none'){
						$('listaItensComposicao_'+sbaidPrsid).style.display = 'none';
					}
			}
			
			
			/**
			 * function inicialytebox(url, title );
			 * descscricao 	: Função que faz o lytebox rodar com o ajax. carrega as telas de formulario.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: url (caminho)
			 * parametros 	: title (titulo do campo)
			 */
			function inicialytebox(url,title,width,height  ) {
			      var anchor = this.document.createElement('a'); // cria um elemento de link
			      anchor.setAttribute('rev', 'width: '+width+'px; height: '+height+'px; scrolling: auto;');
			      anchor.setAttribute('title', '');
			      anchor.setAttribute('href', url);
			      anchor.setAttribute('rel', 'lyteframe');
			      // paramentros para do lytebox //
			      myLytebox.maxOpacity = 50;
			      myLytebox.filter = 10;
			      myLytebox.outerBorder = true;
			      myLytebox.doAnimations = false;
			      myLytebox.start(anchor, false, true);
			      document.getElementById("lbIframe").frameBorder='0';
			      //tooltipWindow
			      return false;
			}
			
			/**
			 * function erro(codigo);
			 * descscricao 	: Mostra erros para o usuário (Regras de Negócio).
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: codigo 
			 */
			function erro(codigo){
				if(codigo == <?=ERRONAOINICIADA; ?>){ // Monitoramento não iniciado.
					alert("O monitoramento desta subação não foi iniciada. Cadastre o status da subação.");
				}
				if(codigo == <?=ERRONAOEXECUTADAOUEMEXECUCAO; ?>){ // Subação não aberta para monitoramento.
					alert("Item já monitorado.");
				}
				if(codigo == <?=MONITORAMENTONAOINICIADA; ?>){ // Subação não aberta para monitoramento.
					alert("Inicie o monitoramento para poder monitorar as sub-ações e os itens de composição.");
				}
				if(codigo == 4){ // Subação não aberta para monitoramento.
					alert("O item só pode ser monitorado se a sub-ação estiver com o status de monitorada .");
				}
				if(codigo == 5){ // monitoramento do mes encerrado.
					alert("Monitorado deste mês finalizado.");
				}
			}
			
			/*
			* Burlando monta_combo simec
			*/
			function dummies( valor ){
				return false;
			}
			
			/**
			 * function closeLytebox(codigo);
			 * descscricao 	: Fecha a tela de edição do item de composição.
			 * author 		: Thiago Tasca Barbosa
			 */
			function closeLytebox(item, sbaid, hmsid, hciid, quant, valor,quantPaga ,valorUnitario, valorTotalPago){
				// formatar valores
				valorUnitario = valorUnitario+'00'; // Para poder mostrar o valor com ,00 quando fechar a tela de itens
				valorTotalPago = valorTotalPago+'00';
				valorUnitario = mascaraglobal( "[###.]###,##", valorUnitario );
				valorTotalPago = mascaraglobal( "[###.]###,##", valorTotalPago );
			
				$('td_'+item).innerHTML = '<img title="Item monitorado" id="'+item+'" align="absmiddle"  src="../../imagens/check_p.gif" width="18" height="18">';
				$('tdPago1_'+item).innerHTML = quantPaga;
				$('tdPago2_'+item).innerHTML = 'R$ '+valorUnitario;
				$('tdPago3_'+item).innerHTML = 'R$ '+valorTotalPago;
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					$('Editar_'+item).setAttribute( "onclick", 'inicialytebox(\'brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&cosid='+item+'&sbaid='+sbaid+'&hmsid='+hmsid+'&hciid='+hciid+'&valor='+valor+'&quant='+quant+'\',\'Monitora Itens Composição\',\'800\',\'550\')' );
				} else { // IE
					$('Editar_'+item).attachEvent( "onclick", 'inicialytebox(\'brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&cosid='+item+'&sbaid='+sbaid+'&hmsid='+hmsid+'&hciid='+hciid+'&valor='+valor+'&quant='+quant+'\',\'Monitora Itens Composição\',\'800\',\'550\')' );
				}
				
				myLytebox.end(); 
				return true;
			}
			
			function closeLyteboxBancarios(){
				myLytebox.end(); 
				return true;
			}

			function closeLyteboxSubAcao(sbaidPrsid,sbaid, ssuid,prsid, hmsid,sucesso, descStatusSub, ano){
				if(sucesso == "ok"){
					$('subStatus_'+sbaidPrsid).innerHTML ='<img title="Sub-ação monitorada" id="imgsubStatus_'+sbaidPrsid+'" align="absmiddle"  src="../../imagens/check_p.gif" width="18" height="18">';
					$('subStatusDesc_'+sbaidPrsid).innerHTML = descStatusSub;
					$('img_subacoes_'+sbaidPrsid).innerHTML = '<img id="mais_'+sbaidPrsid+'" onclick="carregaItensComposicao('+sbaid+', '+<?=$_SESSION['ano'] ?>+','+ssuid+','+prsid+','+hmsid+' );" name="mais_'+sbaidPrsid+'" src="../../imagens/mais.gif"/>';
					carregaItensComposicao(sbaid, ano, ssuid, prsid, hmsid);
					if(window.addEventListener){ // Mozilla, Netscape, Firefox
						$('editarSub_'+sbaidPrsid).setAttribute( "onclick", 'carregaTelaStatusSubAcao('+sbaid+','+ssuid+','+prsid+','+hmsid+','+ano+')' );
					} else { // IE
						$('editarSub_'+sbaidPrsid).attachEvent( "onclick", 'carregaTelaStatusSubAcao('+sbaid+','+ssuid+','+prsid+','+hmsid+','+ano+')' );
					}
				}
				myLytebox.end(); 
				return true;
			}
			
			function visualizaRelatorioHistoricoMonitoramento(prsid,hmcid){
				return windowOpen('brasilpro.php?modulo=principal/monitoraFinanceiro/relatorio_historico_convenio&acao=A&prsid='+prsid+'&hmcid='+hmcid,
                          'Histórico de monitoramento de convênio',
                          'height=700,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
			}
		</script>
	</body>
</html>