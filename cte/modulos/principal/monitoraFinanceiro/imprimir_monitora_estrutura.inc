<?
/************************* INCLUDES **********************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoConvItemComposicao.class.inc');

/************************* FUNÇÕES AJAX **********************************/

/************************* DECLARAÇÃO DE VARIAVEIS *************************/
global  $db;
$dataFormatada 	= new Data();
$inuid			= $_SESSION['inuid'];
$prsid			= $_REQUEST['prsid'];
$hmcid			= $_REQUEST['hmcid'];
$hmdid			= $_REQUEST['hmdid'];
$obconvenio		= new HistoricoMonitoramentoConvenio();

if(!$hmcid){ // Se iniciou o monitoramento carrega o id dele.
	$sql = "SELECT hmcid
			FROM cte.historicomonitoramentoconvenio hc
			INNER JOIN cte.historicomonitoramentodadosbancarios hm ON hm.hmdid = hc.hmdid
			WHERE hc.prsid =".$prsid." and hm.hmdid = ".$hmdid;
	$hmcid = $db->pegaUm($sql);
	$_REQUEST['hmcid'] = $hmcid;
}

/************************* CARREGA MONITORAMENTO DE ACORDO COM O MÊS. *************************/
$sql = "SELECT 	hm.hmdmes, 
				hm.hmdano,
				hm.hmdid, 
				hc.hmcstatus,
				ps.prsnumconvsape,
				CASE WHEN hc.hmcstatus = 'I' THEN 'Em andamento.' ELSE 'Finalizado.' END AS hmcstatusdescricao
		FROM cte.historicomonitoramentoconvenio hc 
		INNER JOIN cte.historicomonitoramentodadosbancarios hm ON hm.hmdid = hc.hmdid
		INNER JOIN cte.projetosape ps ON ps.prsid = hc.prsid
		WHERE hc.hmcid = ".$hmcid." AND hc.prsid = ".$prsid;
$monitoraMes 	= $db->pegaLinha($sql);
$mes 		 	= $monitoraMes['hmdmes'];
$ano 		 	= $monitoraMes['hmdano'];
$hmdid 		 	= $monitoraMes['hmdid'];
$status 	 	= $monitoraMes['hmcstatus'];
$numConvenio 	= $monitoraMes['prsnumconvsape'];
$descStatus  	= $monitoraMes['hmcstatusdescricao'];
$display 	 	= '';
$tudoMonitorado = false;

if($mes != 10 && $mes != 11 && $mes != 12){
	$mes = '0'.$mes;
}

if($status == 'I'){ // Caso iniciado o monitoramento carrega o Btn de Finalizar o monitoramento. 
	$style = '';
}else if($status == 'F'){
	$display = 'style="display:none;"';
}

$mesTexto = $dataFormatada->mesTextual($mes);

/************************* CABEÇALHO E TÍTULO ******************************/
$url = "?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia=".$_SESSION['ano'];
cteMontaTituloFinanceiro( 'Monitoramento Financeiro do PAR', '&nbsp;', $url );

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>
	<script type="text/javascript" src="../../includes/prototype.js"></script>
</head>
<body>
		<!-- CARREGANDO DADOS AJAX -->
		<div id="loader-container" style="display:none;">
	    	<div id="loader">
	    		<img src="../../imagens/wait.gif" border="0" align="middle">
	    		<span>Aguarde! Carregando Dados...</span>
	    	</div>
	    </div>
	    <!-- CARREGA DADOS DO MONITORAMENTO -->
	    <table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1" width="95%">
			<tr>
				<td  class="tdTopo" style="text-align: left;" >Monitoramento do mês: <?=$mesTexto; ?> / <?=$ano; ?> do convênio n°: <?=$numConvenio;?>  </td>
			</tr>
			<tr>
				<td width="170px;" class="tdTopo" style="text-align: left;" >Status deste monitoramento: <?=$descStatus;?></td>
			</tr>
		</table>
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
				<?php 	
					$obHistorico = new HistoricoMonitoramentoConvenio();
					$obHistorico->listaHistoricoMonitoramento($inuid,$_SESSION['ano'], $prsid, true);  
				?>
				</td>
			</tr>
			<tr>
				<td style="text-align: center;">
					<input type="button" value="imprimir" onClick="javascript:window.print()" id="botao" />
				</td>
			</tr>			
		</table>
		<script type="text/javascript">

		/**
		 * function erro(codigo);
		 * descscricao 	: Mostra erros para o usuário (Regras de Negócio).
		 * author 		: Thiago Tasca Barbosa
		 * parametros 	: codigo 
		 */
		function erro(codigo){
			var ERRONAOINICIADA = 1;
			var ERRONAOEXECUTADAOUEMEXECUCAO = 2;
			var MONITORAMENTONAOINICIADA = 3;
			
			if(codigo == ERRONAOINICIADA){ // Monitoramento não iniciado.
				alert("O monitoramento desta subação não foi iniciada. Cadastre o status da subação.");
			}
			if(codigo == ERRONAOEXECUTADAOUEMEXECUCAO){ // Subação não aberta para monitoramento.
				alert("Item já monitorado.");
			}
			if(codigo == MONITORAMENTONAOINICIADA){ // Subação não aberta para monitoramento.
				alert("Inicie o monitoramento para poder monitorar as sub-ações e os itens de composição.");
			}
			if(codigo == 4){ // Subação não aberta para monitoramento.
				alert("A subação está com status de Pendente. \n Monitore a subação para poder iniciar o monitoramento dos itens da própria subação.");
			}
			if(codigo == 5){ // monitoramento do mes encerrado.
				alert("Monitorado deste mês já foi finalizado. \n não é possivel editar a subação.");
			}
			if(codigo == 6){ // monitoramento do mes encerrado.
				alert("Monitorado deste mês já foi finalizado. \n não é possivel editar o item de composição.");
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
			 * function voltarListaMeses();
			 * descscricao 	: Volta para tela de meses do convênio. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function voltarListaMeses(){
				var url 	= '?modulo=principal/monitoraFinanceiro/estrutura_mes&acao=A&anoreferencia='+<?=$_SESSION['ano']; ?>;
				window.location = url;
			}
			
			/**
			 * function carregaSubacoes(convenio, hmcid);
			 * descscricao 	: Carrega lista de subações quando clicado no btn + do convênio. 
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 * parametros  	: ssuid (id do status da subação)
			 */
			function carregaSubacoes(convenio, hmcid){
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=carregasubacao&prsid='+convenio+'&hmcid='+hmcid,
					asynchronous: false,
					onComplete: function(resposta)
					{	
						$('img_convenios').innerHTML = '<img id="mais" onclick="enableSubacoes();" name="mais" src="../../imagens/mais.gif" style="display: none;"/><img id="menos" onclick="disableSubacoes();" name="menos" style="" src="../../imagens/menos.gif"/>'
						$('listaAcoes').style.display = '';
						$('linha').style.display = '';
						$('listaAcoes').innerHTML = resposta.responseText;
						$('menos').style.display = '';
						$('mais').style.display = 'none';
						$('loader-container').hide();	
					}
				});
			}
			
			/**
			 * function enableSubacoes();
			 * descscricao 	: Habilita subações. Função que troca a função do botão + por - e mostra a lista de subações. 
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas habilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function enableSubacoes(){
				$('menos').style.display = '';
				$('linha').style.display = '';
				$('mais').style.display = 'none';
				$('listaAcoes').style.display = '';
			}
			
			/**
			 * function disableSubacoes();
			 * descscricao 	: Desabilita subações. Função que troca o o botão - por + e esconde a lista de subações.
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas Desabilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function disableSubacoes(){
				$('menos').style.display = 'none';
				$('linha').style.display = 'none';
				$('mais').style.display = '';
				$('listaAcoes').style.display = 'none';
				
			}
			
			/**
			 * function carregaTelaStatusSubAcao();
			 * descscricao 	: Carrega tela de Subação
			 * author 		: Thiago Tasca Barbosa
			 */
			function carregaTelaStatusSubAcao(sbaid, ssuid, prsid,hmsid, ano, hmcid){
				inicialytebox('cte.php?modulo=principal/monitoraFinanceiro/monitora_SubAcao&acao=A&requisicao=carregar&sbaid='+sbaid+'&ssuid='+ssuid+'&prsid='+prsid+'&hmsid='+hmsid+'&ano='+ano+'&hmcid='+hmcid,'Monitora Status Subação','800','250');
				return true;
			}
			
			/**
			 * function closeLyteboxSubAcao();
			 * descscricao 	: Fecha tela de subação
			 * author 		: Thiago Tasca Barbosa
			 */
			function closeLyteboxSubAcao(sbaidPrsid,sbaid, ssuid,prsid, hmsid,sucesso, descStatusSub, ano,hmcid){
				if(sucesso == "ok"){
					$('subStatus_'+sbaidPrsid).innerHTML ='<img title="Sub-ação monitorada" id="imgsubStatus_'+sbaidPrsid+'" align="absmiddle"  src="../../imagens/check_p.gif" width="18" height="18">';
					$('subStatusDesc_'+sbaidPrsid).innerHTML = descStatusSub;
					$('img_subacoes_'+sbaidPrsid).innerHTML = '<img id="mais_'+sbaidPrsid+'" onclick="carregaItensComposicao('+sbaid+', '+<?=$_SESSION['ano'] ?>+','+ssuid+','+prsid+','+hmsid+','+hmcid+' );" name="mais_'+sbaidPrsid+'" src="../../imagens/mais.gif"/>';
					atualizaQtdItensPendentes(sbaid, ano, hmcid);
					carregaItensComposicao(sbaid, ano, ssuid, prsid, hmsid, hmcid);
					if(window.addEventListener){ // Mozilla, Netscape, Firefox
						$('editarSub_'+sbaidPrsid).setAttribute( "onclick", 'carregaTelaStatusSubAcao('+sbaid+','+ssuid+','+prsid+','+hmsid+','+ano+','+hmcid+')' );
					} else { // IE
						$('editarSub_'+sbaidPrsid).attachEvent( "onclick", 'carregaTelaStatusSubAcao('+sbaid+','+ssuid+','+prsid+','+hmsid+','+ano+','+hmcid+')' );
					}
				}
				myLytebox.end(); 
				return true;
			}
			
			function atualizaQtdItensPendentes(sbaid, ano, hmcid){
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=atualizaqtditenspendentes&sbaid='+sbaid+'&ano='+ano+'&hmcid='+hmcid,
					onComplete: function(resposta)
					{	
						$('itensPendentes_'+sbaid+ano).innerHTML = resposta.responseText;				
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
			function carregaItensComposicao(sbaid, ano, ssuidmonitora, prsid, hmsid, hmcid, indicePai){
				sbaidPrsid = sbaid.toString()+prsid.toString()+ano.toString();
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=carregaitens&sbaid='+sbaid+'&ssuid='+ssuidmonitora+'&prsid='+prsid+'&hmsid='+hmsid+'&ano='+ano+'&hmcid='+hmcid+'&indicePai='+indicePai,
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
			 * function closeLytebox(codigo);
			 * descscricao 	: Fecha a tela de edição do item de composição.
			 * author 		: Thiago Tasca Barbosa
			 */
			function closeItens(item, sbaid, hmsid, hciid, quant, valor,quantPaga ,valorUnitario, valorTotalPago, ano, hmcid, statusItem, arrTotalExecutado){
				// Atualiza os valores das colunas que referem-se ao grupo "Executados Acumulados"
				atualizaExecutadoAcumulado(item, arrTotalExecutado);

				// formatar valores
//				valorUnitario = valorUnitario+'00'; // Para poder mostrar o valor com ,00 quando fechar a tela de itens
//				valorTotalPago = valorTotalPago+'00';
				valorUnitario  = valorUnitario.toFixed(2); 
				valorTotalPago = valorTotalPago.toFixed(2);				
				valorUnitario  = mascaraglobal( "[###.]###,##", valorUnitario );
				valorTotalPago = mascaraglobal( "[###.]###,##", valorTotalPago );
				
				$('td_'+item).innerHTML = '<img title="Item monitorado" id="'+item+'" align="absmiddle"  src="../../imagens/check_p.gif" width="18" height="18">';
				$('tdPago1_'+item).innerHTML = quantPaga;
				$('tdPago2_'+item).innerHTML = 'R$ '+valorUnitario;
				$('tdPago3_'+item).innerHTML = 'R$ '+valorTotalPago;
				$('td_status_item_'+item).innerHTML = statusItem;
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					$('Editar_'+item).setAttribute( "onclick", 'inicialytebox(\'cte.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&cosid='+item+'&sbaid='+sbaid+'&hmsid='+hmsid+'&hciid='+hciid+'&valor='+valor+'&quant='+quant+'\',\'Monitora Itens Composição\',\'800\',\'550\')' );
				} else { // IE
					$('Editar_'+item).attachEvent( "onclick", 'inicialytebox(\'cte.php?modulo=principal/monitoraFinanceiro/monitora_itensComp&acao=A&cosid='+item+'&sbaid='+sbaid+'&hmsid='+hmsid+'&hciid='+hciid+'&valor='+valor+'&quant='+quant+'\',\'Monitora Itens Composição\',\'800\',\'550\')' );
				}
				if(sbaid && ano && hmcid){
					atualizaQtdItensPendentes(sbaid, ano, hmcid);
				}
				myLytebox.end(); 
				return true;
			}
			
			function atualizaExecutadoAcumulado(item, arrTotalExecutado){
				var vlrTot, 
					qtdTot,
					vlrNovo,
					qtdNovo,
					vlrAtual,
					qtdAtual;
				var d = document;
				var qtd = d.getElementById('td_qtd_executado_acumulado_' + item);			
				var vlr = d.getElementById('td_valor_executado_acumulado_' + item);	
				/**** 
				 *
				 * INÍCIO = Prepara valores vindos da página pai
				 *	
				 ****/
				vlrNovo  = arrTotalExecutado.valorExecutadoNovo;
				vlrNovo  = vlrNovo != '' ? parseFloat( vlrNovo ) : 0;
				
				qtdNovo  = arrTotalExecutado.qtdExecutadoNovo;
				qtdNovo  = qtdNovo != '' ? parseFloat( qtdNovo ) : 0;
				
				vlrAtual = arrTotalExecutado.valorExecutadoAtual;
				vlrAtual = vlrAtual != '' ? parseFloat( vlrAtual ) : 0;
				
				qtdAtual = arrTotalExecutado.qtdExecutadoAtual;
				qtdAtual = qtdAtual != '' ? parseFloat( qtdAtual ) : 0;
				/**** 
				 *
				 * FIM = Prepara valores vindos da página pai
				 *	
				 ****/
						
				// Prepara o valor total
				vlrTot = replaceAll( replaceAll( replaceAll( vlr.innerHTML, '.', ''), ',', '.'), ' ', '');
				vlrTot = vlrTot != '' ? parseFloat( vlrTot ) : 0;
				// Calcula o valor total
				vlrTot = parseFloat( (vlrTot - vlrAtual) + vlrNovo ).toFixed(2);
				
				// Prepara a quantidade total
				qtdTot = qtd.innerHTML;
				qtdTot = qtdTot != '' ? parseFloat( qtdTot ) : 0;
				// Calcula o quantidade total
				qtdTot = (qtdTot - qtdAtual) + qtdNovo; 
				
				vlr.innerHTML = mascaraglobal( '[###.]###,##', vlrTot );
				qtd.innerHTML = qtdTot;								
			}
			
			/**
			 * function finalizarmonitoramento(fimConvenio);
			 * parametros	: id do convênio.
			 * desccrição 	: finaliza o convênnio. 
			 * author 		: Thiago Tasca Barbosa
			*/
			function finalizarmonitoramento(fimConvenio, hmcid){
				var cpf = <?=$_SESSION["usucpf"];?>; 
				var nome = "<?=$_SESSION["usunome"];?>"; 
				//var ano = "<?=$_SESSION['ano'];?>";
				var ano = "<?=$monitoraMes['hmdano'];?>";				
				var mes = "<?=$mesTexto;?>";
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=podefinalizarmonitoramento&hmcid='+hmcid,
						onComplete: function(resposta)
						{	
							$('loader-container').hide();
							if(resposta.responseText == 1){
								if(confirm('O monitoramento do mês de '+mes+' de '+ano+' será finalizado agora. Deseja realmente prosseguir?')){
									if(confirm('Eu, '+nome+', CPF '+cpf+', desejo finalizar o monitoramento do mês de '+mes+' de '+ano+'? \n Estou ciente de que, após a finalização, os dados não podem ser alterados e de que as  informações inseridas no sistema são de inteira responsabilidade do convenente.  Certifico a veracidade das informações inseridas.')){
										$('loader-container').show();
										return new Ajax.Request(window.location.href,{	
											method: 'post',
											parameters: '&requisicao=finalizarmonitoramento&hmcid='+hmcid,
											onComplete: function(resposta)
											{	
												alert(resposta.responseText);
												if( resposta.responseText == "Monitoramento encerrado com sucesso."){
													$('finalizarMonitora_'+fimConvenio).style.display = 'none';
													$('tdStatusMonitoramento').innerHTML = 'Finalizado.'; 
													carregaSubacoes(fimConvenio, hmcid);
													atualizaListaHistoricoMonitoramento(fimConvenio);
												}
												$('loader-container').hide();	
											}
										});
									}
								}
							}else{
								alert(resposta.responseText);
							}
						}
					});
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

			 function visualizaRelatorioHistoricoMonitoramento(prsid,hmcid){
					return windowOpen('cte.php?modulo=principal/monitoraFinanceiro/relatorio_historico_convenio&acao=A&prsid='+prsid+'&hmcid='+hmcid,
	                          'Histórico de monitoramento de convênio',
	                          'height=700,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
				}
				
			/* Função para subustituir todos 
			 * descscricao 	: 
			 * author 		: Alexandre Dourado
			 * parametros 	: 
			 */
			function replaceAll(str, de, para){
				if ( str == ''){ return str; }
			    var pos = str.indexOf(de);
			    while (pos > -1){
					str = str.replace(de, para);
					pos = str.indexOf(de);
				}
			    return (str);
			}		

			function janelaImpressao(prsid, hmcid, hmdid) {
				return window.open('cte.php?modulo=principal/monitoraFinanceiro/imprimir_monitora_estrutura&acao=A&prsid='+prsid+'&hmcid='+hmcid+'&hmdid='+hmdid,'monitoraConvenio','height=500,width=800,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
			}
		</script>
	</body>
</html>
