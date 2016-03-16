<?php
/************************* INCLUDES **********************************/
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ .'cte/classes/ProjetoSape.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');
include_once(APPRAIZ .'cte/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoConvItemComposicao.class.inc');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');


/************************* INSTANCIANDO OBJETOS *************************/
global $db;
$conveniosSape 		= new ProjetoSape();
$monitoramento 		= new HistoricoMonitoramentoConvenio();
$meses 				= new HistoricoMonitoramentoDadosBancarios();

/************************* FUNÇÕES AJAX **********************************/
//CARREGA MESES QUANDO CLICA NO +.
if($_REQUEST['requisicao'] == 'carregameses'){
	$conteudo 	= $meses->carregaMeses($_REQUEST['prsid'], $_SESSION['ano']);
	echo $conteudo;
	die();
}

//VALIDA SE PODE INICIAR O MONITORAMENTO
if($_REQUEST['requisicao'] == 'validaInicioMonitoramento'){
	$resposta 		= $meses->validaSePodeInserirMonitoramento($_REQUEST['hmdid']);
	echo $resposta;
	die();
}

//INICIA MONITORAMENTO
if($_REQUEST['requisicao'] == 'iniciaMonitoramento'){
	$resposta 			= $monitoramento->insereMonitoramento($_REQUEST['prsid'],$_REQUEST['hmdid']);
	echo $resposta;
	die();
}

// CRIPITOGRAFIA DE DADOS
/*
if($_REQUEST['requisicao'] == 'cripitografa'){
	$senhaP = "phpSimecPrsid";
	$senhaH = "phpSimecHmdid";
	$prsid 	= md5_encrypt($_REQUEST['prsid'],$senhaP);
	$hmdid 	= md5_encrypt($_REQUEST['hmdid'],$senhaH);
	echo $prsid."----".$hmdid;
	die();
}
*/
/************************* DECLARAÇÃO DE VARIAVEIS *************************/
$instrumentoUnidade = $_SESSION['inuid'];
$_SESSION['ano'] 	= $_REQUEST['anoreferencia'];

/************************* CARREGA CONVÊNIO ******************************/
if($instrumentoUnidade && $_SESSION['ano']){
	$convenios 		= $conveniosSape->carregaListaDeConvenio($_SESSION['ano'], $instrumentoUnidade); //carrega todos os Convênios do ano do inuid.
	if(!is_array($convenios)){
		unset($_SESSION['hmc']);
		unset($_SESSION['ano']);
		unset($conveniosSape);
		alert("Não existem convênios para este ano.");
	} elseif( $convenios[0]['prsnumconvsape'] == null ){
		unset($_SESSION['hmc']);
		unset($_SESSION['ano']);
		unset($conveniosSape);
		alert("Aguardando a confirmação da celebração do convênio.");
	}
}

/************************* DEIXA SELECIONADO O ANO DA SESSION NO COMBO DE ANO. **************/
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

/************************* CABEÇALHO E TÍTULO ******************************/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
?>
<html>
	<head>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../../cte/monitoraFinanceiro/includes/funcoes.js"></script>
		<script type="text/javascript" src="../../includes/prototype.js"></script>
		<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
		<script type="text/javascript" src="../../includes/remedial.js"></script>
		<script type="text/javascript" src="../../includes/superTitle.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
		<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
		<link rel="stylesheet" type="text/css" href="../../includes/LyteBox/lytebox.css"/>
		<!--[if IE]>
			<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro_ie.css"/>
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
	    <!-- FORMULÁRIO DE CARREGA ANOS  -->
		<form name="formAno" id="formAno" action="" method="post">
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" >Ano:</td>
				<td>		
					<select name="anoreferencia" id="anoreferencia" title="Ao selecionar um ano, será carregado a lista de convênio deste ano. ">
						<option value="">selecione</option>
		  				<option <?= $selected07; ?> value="2007">2007</option>
		  				<option <?= $selected08; ?> value="2008">2008</option>
		  				<option <?= $selected09; ?> value="2009">2009</option>
		  				<option <?= $selected10; ?> value="2010">2010</option>
		  				<option <?= $selected11; ?> value="2011">2011</option>
					</select>
					<input type="button" name="Carregar" value="Carregar"  onclick="carregarConvenios();" />
					<img class="ajuda" src="../../imagens/ajuda.png" title="selecione o ano de celebração do convênio." >
				</td>
			</tr>
		</table>
		</form>
		<!-- FORMULÁRIO DOS CONVÊNIOS -->
		<form name="formulario" id="formulario" action="" method="post">
			<?php if(is_array($convenios)){ //Se existe convênio. Só carrega dados se existir convênios
					if( $convenios[0]['prsnumconvsape'] ){
						foreach($convenios as $convenio){
					
			?>
			<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1" width="95%">
			<?php if(cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR_FINANCEIRO, CTE_PERFIL_SUPER_USUARIO))){?>
				<tr>
					<td colspan="2" class="SubTituloEsquerda" ><a onclick="abreTelaDeGerencia(<?=$convenio['prsid'];?>, <?=$convenio['prsano']; ?>)" >Gerenciamento dos meses de monitoramento.</a></td>
				</tr>
			<?php } ?>
				<tr>
					<th  class="textoEsquerda">
					<div style="float:left;">
							<div class="div_imagens" id="img_convenios_<?=$convenio['prsid'];?>">	
								<img src="../../imagens/mais.gif" id="mais_<?=$convenio['prsid'];?>" name="mais_<?=$convenio['prsid'];?>" onclick="carregaMeses(<?=$convenio['prsid'];?>);"/> 
								<img src="../../imagens/menos.gif" style="display:none;" id="menos_<?=$convenio['prsid'];?>" name="menos_<?=$convenio['prsid'];?>" onclick="disableMeses(<?=$convenio['prsid'];?>);"/>
							</div>
							<a style="cursor:pointer;float:left;" onclick="abreTelaCadastroValoresConvenio(<?=$convenio['prsid'];?>, <?=$convenio['prsano']; ?>)">Convênio nº: <?=$convenio['prsnumconvsape'];?></a>
					</div>
					<div style="float:right;">
						<span style="float:left;color:#133368;" class="SubTituloEsquerda">
						Monitorado até o momento:
						</span>
						<span style="float:left;"> 
							<?php
							$conteudo = $meses->carregaPorcentagemConvenio($convenio['prsid'], $convenio['prsano']);
							echo $conteudo;
							?>
						</span>
					</div>
					</th>
				</tr>
				<tr>
					<td >
						<div  style="display:none;" id="linha_<?=$convenio['prsid'];?>" class="linha">
							<img src="../../includes/dtree/img/line.gif"/><br>
							<img src="../../includes/dtree/img/joinbottom.gif"/>
						</div>
						<!-- DIV ONDE CARREGA A LISTA DE Meses passivel de Monitoramento -->
						<div class="listaSubAcoes" id="listaMeses_<?=$convenio['prsid'];?>"></div>
					</td>
				</tr>
				<tr>
					<td  class="textoEsquerda"><a onclick="relatorioGeral(<?=$convenio['prsid'];?>);" >Relátorio dos Monitoramentos</a>
					</td>
				</tr>
			</table>
			<?php 
					} // FIM FOREACH -- foreach($convenios as $convenio)
				} // FIM do if verificando o $convenios['prsnumconvsape']
			} // Fim se existe convênio
		 ?>
		</form>
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
		
			function relatorioGeral(prsid){
				return windowOpen('cte.php?modulo=principal/monitoraFinanceiro/relatorio_geral_convenio&acao=A&prsid='+prsid,
                        'Histórico de monitoramento de convênio',
                        'height=700,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
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
			 * function carregaMeses(convenio);
			 * descscricao 	: Carrega lista de meses passivel de monitoramento quando clicado no btn + do convênio. 
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 * parametros  	: ssuid (id do status da subação)
			 */
			function carregaMeses(convenio){
				$('loader-container').show();
				return new Ajax.Request(window.location.href,{	
					method: 'post',
					parameters: '&requisicao=carregameses&prsid='+convenio,
					asynchronous: false,
					onComplete: function(resposta)
					{	
						$('img_convenios_'+convenio).innerHTML = '<img id="mais_'+convenio+'" onclick="enableMeses('+convenio+');" name="mais_'+convenio+'" src="../../imagens/mais.gif" style="display: none;"/><img id="menos_'+convenio+'" onclick="disableMeses('+convenio+');" name="menos_'+convenio+'" style="" src="../../imagens/menos.gif"/>';
						$('listaMeses_'+convenio).style.display = '';
						$('linha_'+convenio).style.display = '';
						$('listaMeses_'+convenio).innerHTML = resposta.responseText;
						$('menos_'+convenio).style.display = '';
						$('mais_'+convenio).style.display = 'none';
						$('loader-container').hide();	
					}
				});
			}

			 /**
			 * function abreTelaCadastroInformacoesConvenio();
			 * desccrição 	: Abre tela com informações do Convênio. 
			 * author 		: Eduardo Dunice Neto(Thiago Tasca Barbosa)
			 */
			function abreTelaCadastroInformacoesConvenio( prsid ){
				var url 	= 'cte.php?modulo=principal/monitoraFinanceiro/informacoes_convenio&acao=C&prsid='+prsid;
				var title 	= 'Informativos de Valores de Convênio';
				var width 	= '800';
				var height 	= '500';
				inicialytebox(url,title,width,height);
				return true;
			}
				
			
			/**
			 * function enableMeses(convenio);
			 * descscricao 	: Habilita subações. Função que troca a função do botão + por - e mostra a lista de subações. 
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas habilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function enableMeses(convenio){
				$('menos_'+convenio).style.display = '';
				$('linha_'+convenio).style.display = '';
				$('mais_'+convenio).style.display = 'none';
				$('listaMeses_'+convenio).style.display = '';
			}
			
			/**
			 * function disableMeses(convenio);
			 * descscricao 	: Desabilita subações. Função que troca o o botão - por + e esconde a lista de subações.
			 *				  Função utilizado para que os dados já carregado não seja carregado novamente evitanto outra requisição no banco, 
			 *				  apenas Desabilitar dados existentes com css.
			 * author 		: Thiago Tasca Barbosa
			 * parametros 	: convenio (id do convênio)
			 */
			function disableMeses(convenio){
				$('menos_'+convenio).style.display = 'none';
				$('linha_'+convenio).style.display = 'none';
				$('mais_'+convenio).style.display = '';
				$('listaMeses_'+convenio).style.display = 'none';
				
			}
			
			function validaSePodeInserirMonitoramento(hmdid, prsid){
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=validaInicioMonitoramento&hmdid='+hmdid,
						asynchronous: false,
						onComplete: function(resposta)
						{	
							if(resposta.responseText != true){
								alert(resposta.responseText);
							}else{
								iniciaMonitoramento(hmdid, prsid);
							}
						}
					});
			}
			
			/**
			 * function iniciaMonitoramento();
			 * desccrição 	: Inicia um novo monitoramento Financeiro do convênio selecionado. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function iniciaMonitoramento(hmdid, prsid){
				if(confirm('Deseja criar um novo monitoramento?')){
					$('loader-container').show();
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=iniciaMonitoramento&hmdid='+hmdid+'&prsid='+prsid,
						asynchronous: false,
						onComplete: function(resposta)
						{	
							alert(resposta.responseText);
							if(resposta.responseText == "Monitoramento iniciado com sucesso."){
								window.location = "?modulo=principal/monitoraFinanceiro/monitora_estrutura&acao=A&prsid="+prsid+"&hmdid="+hmdid;
							}
							$('loader-container').hide();
						}
					});
				}
			}
			
			function carregaMonitoramento(convenio, idMonitoramento, hmdid){
					window.location = "?modulo=principal/monitoraFinanceiro/monitora_estrutura&acao=A&prsid="+convenio+"&hmcid="+idMonitoramento+"&hmdid="+hmdid;
					return true;
			}

			/**
			 * function abreTelaDeGerencia();
			 * desccrição 	: Abre tela para cadastra os dados financeiro do Convênio. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function abreTelaDeGerencia(prsid, ano){
				var url 	= 'cte.php?modulo=principal/monitoraFinanceiro/gerencia_monitora&acao=A&prsid='+prsid+'&ano='+ano;
				var title 	= 'Informativos de Valores de Convênio';
				var width 	= '800';
				var height 	= '500';
				inicialytebox(url,title,width,height);
				return true;
			}
			
			/**
			 * function abreTelaCadastroValoresConvenio();
			 * desccrição 	: Abre tela para cadastra os dados financeiro do Convênio. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function abreTelaCadastroValoresConvenio(prsid, ano){
				var url 	= 'cte.php?modulo=principal/monitoraFinanceiro/monitora_convenio&acao=A&prsid='+prsid+'&ano='+ano;
				var title 	= 'Informativos de Valores de Convênio';
				var width 	= '800';
				var height 	= '500';
				inicialytebox(url,title,width,height);
				return true;
			}
			
			function closeLyteboxBancarios(){
				myLytebox.end();
				carregaMeses(<?=$convenio['prsid'];?>);
				return true;
			}

			function closeLyteboxGerencia(){
				myLytebox.end();
				carregaMeses(<?=$convenio['prsid'];?>);
				return true;
			}

			function janelaImpressao(prsid, ano) {
				return window.open('cte.php?modulo=principal/monitoraFinanceiro/imprimir_carregar_meses&acao=A&prsid='+prsid+'&ano='+ano,'monitoraMeses','height=500,width=900,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
			}
		</script>
	</body>
</html>