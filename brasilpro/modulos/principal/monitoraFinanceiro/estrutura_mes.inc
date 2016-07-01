<?php
/************************* INCLUDES **********************************/
include_once(APPRAIZ .'brasilpro/classes/ProjetoSape.class.inc');
include_once(APPRAIZ .'brasilpro/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ .'brasilpro/classes/HistoricoMonitoramentoDadosBancarios.class.inc');

include_once(APPRAIZ.'www/brasilpro/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'includes/classes/dateTime.inc');
include_once(APPRAIZ.'brasilpro/modulos/principal/monitoraFinanceiro/funcoes.inc');

include_once(APPRAIZ .'brasilpro/classes/HistoricoMonitoramentoConvSubac.class.inc');
include_once(APPRAIZ.'brasilpro/classes/HistoricoConvItemComposicao.class.inc');
global $db;
/************************* FUNÇÕES AJAX **********************************/
//VALIDA INICIO DE MONITORAMENTO
if($_REQUEST['requisicao'] == 'validaInicioMonitoramento'){
	//$regras 		= new HistoricoMonitoramentoConvenio();
	//$resposta 		= $regras->validaSePodeInserirMonitoramento($_REQUEST['prsid'], $_REQUEST['mes'], $_REQUEST['ano']);
	$resposta 		= validaSePodeInserirMonitoramento($_REQUEST['prsid'], $_REQUEST['mes'], $_REQUEST['ano']);
	echo $resposta;
	die();
}

//INICIA MONITORAMENTO
if($_REQUEST['requisicao'] == 'iniciaMonitoramento'){
	$resposta 			= insereMonitoramento($_REQUEST['prsid'], $_REQUEST['mes'], $_REQUEST['ano'], $_REQUEST['id']);
	echo $resposta;
	die();
}

//ATUALIZA LISTA DE MONITORAMENTOS
if($_REQUEST['requisicao'] == 'atualizaListaHistoricoMonitoramento'){
	echo atualizaListaHistoricoMonitoramento($_REQUEST['prsid']);
	die();
}

//CARREGA MESES
if($_REQUEST['requisicao'] == 'carregameses'){
	//$meses 		= new HistoricoMonitoramentoDadosBancarios();
	//$conteudo 	= $meses->carregaMeses($_REQUEST['prsid'], $_SESSION['ano']);
	$conteudo 	= carregaMeses($_REQUEST['prsid'], $_SESSION['ano']);
	echo $conteudo;
	die();
}



/************************* DECLARAÇÃO DE VARIAVEIS *************************/
$instrumentoUnidade = $_SESSION['inuid'];
$_SESSION['ano'] 	= $_REQUEST['anoreferencia'];

/************************* CARREGA LISTA DE CONVÊNIOS *******************************/
if($instrumentoUnidade && $_SESSION['ano']){
	//$conveniosPAR 	= new ProjetoSape();
	//$convenios 		= $conveniosPAR->carregaListaDeConvenio($_SESSION['ano'], $instrumentoUnidade); //carrega todos os Convênios do ano para o inuid.
	$convenios 		= carregaListaDeConvenio($_SESSION['ano'], $instrumentoUnidade); //carrega todos os Convênios do ano para o inuid.
	if(!is_array($convenios)){
		unset($_SESSION['hmc']);
		alert("Não existem convênios para este ano.");
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
		<script type="text/javascript" src="../../includes/prototype.js"></script>
		<script type="text/javascript" src="../../includes/LyteBox/lytebox.js"></script>
		<script type="text/javascript" src="../../includes/remedial.js"></script>
		<script type="text/javascript" src="../../includes/superTitle.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
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
				</td>
			</tr>
		</table>
		</form>
		<!-- FORMULÁRIO DOS CONVÊNIOS -->
		<form name="formulario" id="formulario" action="" method="post">	
		<?php if(is_array($convenios)){ //Se existe convênio. Só carrega dados se existir convênios
				foreach($convenios as $convenio){
					//$monitoramento 	= new HistoricoMonitoramentoConvenio();
					//$dadosConvenios = $monitoramento->recuperaDadosDoMonitoramento($convenio['prsid']);
					$dadosConvenios = recuperaDadosDoMonitoramento($convenio['prsid']);
					if(is_array($dadosConvenios)){
						foreach($dadosConvenios as $dados){
							if($dados['hmcstatus'] == 'I'){//Se o status do monitoramento for Iniciado carrega para Session o id do Monitoramento
								//$monitoramento->carregaMonitoramentosParaSession($dados['prsid'],$dados['hmcid']);
								carregaMonitoramentosParaSession($dados['prsid'],$dados['hmcid']);
								$convenio['hmcstatus'] = $dados['hmcstatus'];
							}else{
								continue;
							}
						}
					}
		?>
		<table align="center" border="0" class="tabela tabelaMonitoramento" cellpadding="3" cellspacing="1" width="95%">
			<tr>
				<th  class="textoEsquerda">
				<div class="div_imagens" id="img_convenios_<?=$convenio['prsid'];?>">	
					<img src="../../imagens/mais.gif" id="mais_<?=$convenio['prsid'];?>" name="mais_<?=$convenio['prsid'];?>" onclick="carregaMeses(<?=$convenio['prsid'];?>);"/> 
					<img src="../../imagens/menos.gif" style="display:none;" id="menos_<?=$convenio['prsid'];?>" name="menos_<?=$convenio['prsid'];?>" onclick="disableMeses(<?=$convenio['prsid'];?>);"/>
				</div>
				<?php 
				$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA);
				$possuiPerfil = cte_possuiPerfil($perfis);
				if($possuiPerfil){ ?>
				<a style="cursor:pointer;" onclick="abreTelaCadastroValoresConvenio(<?=$convenio['prsid'];?>, <?=$convenio['prsano']; ?>)">Convênio nº: <?=$convenio['prsnumconvsape'];?></a>
				<?php }else{ //Convênio nº: $convenio['prsnumconvsape']; ?>
				
				<a style="cursor:pointer;" onclick="abreTelaCadastroValoresConvenio(<?=$convenio['prsid'];?>, <?=$convenio['prsano']; ?>)">Convênio nº: <?=$convenio['prsnumconvsape'];?></a>
				<?php } ?>
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
		</table>
		<?php 
				}
			} // Fim se existe convênio
		 ?>
		</form>
		<script type="text/javascript">
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
			function iniciaMonitoramento(convenioSelecionado, mes, ano, idmes){
				if(confirm('Deseja criar um novo monitoramento?')){
					$('loader-container').show();
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=iniciaMonitoramento&prsid='+convenioSelecionado+"&mes="+mes+"&ano="+ano+"&id="+idmes,
						asynchronous: false,
						onComplete: function(resposta)
						{	
							
							alert(resposta.responseText);
							if(resposta.responseText == "Monitoramento iniciado com sucesso."){
								window.location = "?modulo=principal/monitoraFinanceiro/monitora_estrutura&acao=A&prsid="+convenioSelecionado;
							}
							$('loader-container').hide();
						}
					});
				}
			}
			
			function validaSePodeInserirMonitoramento(convenioSelecionado, mes,ano, idmes){
					return new Ajax.Request(window.location.href,{	
						method: 'post',
						parameters: '&requisicao=validaInicioMonitoramento&prsid='+convenioSelecionado+"&mes="+mes+"&ano="+ano,
						asynchronous: false,
						onComplete: function(resposta)
						{	
							if(resposta.responseText != true){
								alert(resposta.responseText);
							}else{
								iniciaMonitoramento(convenioSelecionado, mes, ano, idmes);
							}
						}
					});
			}
			
			function carregaMonitoramento(convenio, mes){
				//if(confirm('Deseja criar um novo monitoramento?')){
					window.location = "?modulo=principal/monitoraFinanceiro/monitora_estrutura&acao=A&prsid="+convenio+"&mes="+mes;
					return true;
				//}
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
			
			function closeLyteboxBancarios(){
				myLytebox.end();
				carregaMeses(<?=$convenio['prsid'];?>);
				return true;
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
			 * function abreTelaCadastroValoresConvenio();
			 * desccrição 	: Abre tela para cadastra os dados financeiro do Convênio. 
			 * author 		: Thiago Tasca Barbosa
			 */
			function abreTelaCadastroValoresConvenio(prsid, ano){
				url 	= 'brasilpro.php?modulo=principal/monitoraFinanceiro/monitora_convenio&acao=A&prsid='+prsid+'&ano='+ano;
				title 	= 'Informativos de Valores de Convênio';
				width 	= '800';
				height 	= '500';
				inicialytebox(url,title,width,height);
				return true;
			}
			
		
		</script>
</body>
</html>