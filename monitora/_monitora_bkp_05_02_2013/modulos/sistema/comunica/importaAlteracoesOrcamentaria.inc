<?php
//error_reporting(E_ALL);
set_time_limit(30000);
ini_set("memory_limit", "3000M");

/*include(APPRAIZ."monitora/classes/AlteracaoOrcamentariaDAO.class.inc");
$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();*/

if($_POST['requisicao'] == 'enviasolicitacao') {
	include(APPRAIZ."monitora/classes/WSAlteracoesOrcamentaria.class.inc");
	include(APPRAIZ."monitora/classes/MontaObjetoAlteracao.class.inc");
	
	//Endereço do wsdl do serviço
	/*$wsdl = WEB_SERVICE_SIOP_URL. "WSAlteracoesOrcamentarias?wsdl";
	
	if( $_SESSION['baselogin'] == 'simec_espelho_producao' ){
		$certificado =  APPRAIZ. "monitora/modulos/sistema/comunica/mec.pem";
		$senha_certificado = "mec2011";
		//$senha_certificado = "";
	} else {
		$certificado =  "/etc/ssl/cert_simec_2012.pem";
		$senha_certificado = "12345678";
	}*/
	$documento = $_POST['documento'];
	
	$arrParam = array('usuario' => $_POST['wsusuario'],
					  'senha' 	=> $_POST['wssenha'],
					  'post' 	=> $_POST
					);
	
//	$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
	
	/*$wsAlteracao = new WSAlteracoesOrcamentarias($wsdl, array(
			'local_cert'	=> $certificado, 
			'passphrase ' 	=> $senha_certificado,
			'exceptions'	=> true,
	        'trace'			=> true,
			'encoding'		=> 'ISO-8859-1' ));*/
	
	//monta a credencial
	/*$credencial = new credencialDTO();
	$credencial->perfil = 32;
	$credencial->usuario = $_POST['wsusuario'];
	$credencial->senha = md5($_POST['wssenha']);*/
	
	if($documento == 'obterPedidoAlteracao') {
		
		$obterPedidoAlteracao = new obterPedidoAlteracao();
		//atribui parâmetros
		$obterPedidoAlteracao->credencial = $credencial;
		$obterPedidoAlteracao->exercicio = 2012;
		$obterPedidoAlteracao->identificadorUnicoPedido = 11733;
		//$obterPedidoAlteracao->codigoMomento = 1;
		
		$obterPedidoAlteracaoResponse = $wsAlteracao->obterPedidoAlteracao($obterPedidoAlteracao);
		ver($obterPedidoAlteracaoResponse,d);
		$retorno = $obAlteracaoDAO->manterObterPeditoAlteracaoResponse( $obterPedidoAlteracaoResponse );
		
		if( $retorno == '1' ){
			$db->sucesso('modulo=sistema/comunica/importaAlteracoesOrcamentaria');
		} else {
			echo "<script>
					alert('Falha na operação');
				</script>";
		}
		
	}elseif( $documento == 'cadastrarPedidoAlteracao' || $documento == 'XMLcadastrarPedidoAlteracao'){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->cadastrarPedidoAlteracao();
		//$arrPedido = $obAlteracaoDAO->carregarPedidoAlteracao();
		/*$arrPedido = $obAlteracaoDAO->carregarPedido( $_POST['limit'] );
		//ver($arrPedido,d);
		
		$strTotalProcessado 	= 0;
		$strTotalnaoProcessado 	= 0;
		
		if( $documento == 'XMLcadastrarPedidoAlteracao' ){
			$strChaveFisico = 'fisicosPedidoAlteracao_';
			$strChaveFinanceiro = 'listaFinanceiroPedidoAlteracaoDTO_';
		} else {
			$strChaveFisico = '';
			$strChaveFinanceiro = '';
		}*/
		
		/*$arrFisicoPedido = array();
		$arrFinanceiro 	 = array();
		$strTotal = 0;
		foreach ($arrPedido as $pedido) {			
			$arrFisico = $obAlteracaoDAO->carregarFisico($pedido['codigouo'], $pedido['codigotipoalteracao']);
			$strTotal++;
			$obFisico = array();
			$arrRespostaDTO = array();
			foreach ($arrFisico as $chave => $fisico) {
				
				$fisico['codigotipoinclusaolocalizador'] = $obAlteracaoDAO->getTipoInclusaoLocalizador($pedido, $fisico);
				
				$arrFinanceiro = $obAlteracaoDAO->carregarFinanceiro($fisico['codigouo'], $fisico['codigotipoalteracao'], $fisico['codigoacao'], $fisico['codigolocalizador']);
				
				$obFinaceiro = array();
				$totalAcrescicmo = 0;
				$totalReducao = 0;				
				foreach ($arrFinanceiro as $key => $financeiro) {
					if( $financeiro['codigotipofonterecurso'] == '2' || $financeiro['codigotipofonterecurso'] == '1' ){
							
							if( $financeiro['codigotipofonterecurso'] == '1' ){
								$financeiro['valorcancelamento'] = $financeiro['valordespesa'];
								$financeiro['valorsuplementacao'] = '0';
								$totalReducao++;
							}else{
								$financeiro['valorcancelamento'] = '0';
								$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
								$totalAcrescicmo++;
							}				
							$financeiro['codigotipofonterecurso'] = '1';
							
					}
					if( $financeiro['codigotipofonterecurso'] == '3' ){
						$financeiro['codigotipofonterecurso'] = '2';
						$financeiro['valorcancelamento'] = '0';
						$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
						$totalAcrescicmo++;
					}
					if( $financeiro['codigotipofonterecurso'] == '4' ){
						$financeiro['codigotipofonterecurso'] = '3';
						$financeiro['valorcancelamento'] = '0';
						$financeiro['valorsuplementacao'] = $financeiro['valordespesa'];
						$totalAcrescicmo++;
					}
					$$key = new financeiroPedidoAlteracaoDTO();
					$$key->codigoFonte = $financeiro['codigofonte'];
					$$key->codigoIdOC = $financeiro['codigoidoc'];
					$$key->codigoIdUso = $financeiro['codigoiduso'];
					$$key->codigoNatureza = $financeiro['codigonatureza'];
					$$key->codigoRP = $financeiro['codigorp'];
					$$key->codigoRPLei = $financeiro['codigorp'];
					$$key->codigoTipoFonteRecurso = $financeiro['codigotipofonterecurso'];
					$$key->valorCancelamento = str_replace( '-', '', $financeiro['valorcancelamento'] );
					$$key->valorSuplementacao = $financeiro['valorsuplementacao'];
					
					$obFinaceiro[$strChaveFinanceiro.$key] = $$key;
				}
				$fisico['quantidadeacrescimo'] 	= $totalAcrescicmo;
				$fisico['quantidadereducao'] 	= $totalReducao;
				
				$$chave = new fisicoPedidoAlteracaoDTO();
				$$chave->codigoAcao = $fisico['codigoacao'];
				$$chave->codigoEsfera = $fisico['codigoesfera'];
				$$chave->codigoFuncao = $fisico['codigofuncao'];
				$$chave->codigoLocalizador = $fisico['codigolocalizador'];
				$$chave->codigoPrograma = $fisico['codigoprograma'];
				$$chave->codigoSubFuncao = $fisico['codigosubfuncao'];
				$$chave->codigoTipoInclusaoLocalizador = $fisico['codigotipoinclusaolocalizador'];
				$$chave->codigoUO = $fisico['codigouo'];
				$$chave->exercicio = $fisico['exercicio'];
				$$chave->listaFinanceiroPedidoAlteracaoDTO = $obFinaceiro;
				$$chave->quantidadeAcrescimo = 0; //$fisico['quantidadeacrescimo'];
				$$chave->quantidadeReducao = 0; //$fisico['quantidadereducao'];
				$obFisico[$strChaveFisico.$chave] = $$chave;
			}
			$arrResposta = $obAlteracaoDAO->carregarPerguntaJustificativa( $pedido );
			
			$obrespostaJustificativaDTO = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO->codigoPergunta = '114';
			$obrespostaJustificativaDTO->resposta 		= ($arrResposta['jsccaracterizacao'] ? $arrResposta['jsccaracterizacao'] : '-');			
			$arrRespostaDTO[] = $obrespostaJustificativaDTO;
			
			$obrespostaJustificativaDTO1 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO1->codigoPergunta = '116';
			$obrespostaJustificativaDTO1->resposta 		 = ($arrResposta['jscconsequencia'] ? $arrResposta['jscconsequencia'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO1;
			
			$obrespostaJustificativaDTO2 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO2->codigoPergunta = '115';
			$obrespostaJustificativaDTO2->resposta 		 = ($arrResposta['jscreflexo'] ? $arrResposta['jscreflexo'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO2;
			
			$obrespostaJustificativaDTO3 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO3->codigoPergunta = '117';
			$obrespostaJustificativaDTO3->resposta 		 = ($arrResposta['jscrepercussao'] ? $arrResposta['jscrepercussao'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO3;
			
			$obrespostaJustificativaDTO4 = new respostaJustificativaDTO();
			$obrespostaJustificativaDTO4->codigoPergunta = '118';
			$obrespostaJustificativaDTO4->resposta 		 = ($arrResposta['jscmemoria'] ? $arrResposta['jscmemoria'] : '-');
			$arrRespostaDTO[] = $obrespostaJustificativaDTO4;

			$obPedidoAlteracaoDTO = new pedidoAlteracaoDTO( $pedido, $obFisico, $arrRespostaDTO );
			
			$obCadastrarPedidoAlteracao = new cadastrarPedidoAlteracao();
			$obCadastrarPedidoAlteracao->credencial = $credencial;
			$obCadastrarPedidoAlteracao->pedidoAlteracaoDTO = $obPedidoAlteracaoDTO;	
			//ver($obCadastrarPedidoAlteracao,d);
			
			if( $documento == 'XMLcadastrarPedidoAlteracao' ){
				$arrXML = $obAlteracaoDAO->arr2xml( $obCadastrarPedidoAlteracao );
				//ver( htmlentities($arrXML),d );
				header('content-type: text/html; charset=ISO-8859-1');
				//$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
			    header( 'Content-type: text/xml');
		        header( 'Content-Disposition: attachment; filename=teste.xml');
		        print_r( ($arrXML) );
		        exit();	
			} else {			
				$cadastrarPedidoAlteracaoResponse = $wsAlteracao->cadastrarPedidoAlteracao( $obCadastrarPedidoAlteracao );
				//ver($cadastrarPedidoAlteracaoResponse,d);
				$retorno = $obAlteracaoDAO->manterCadastrarPedidoAlteracaoResponse( $cadastrarPedidoAlteracaoResponse, $documento, $obCadastrarPedidoAlteracao, $strTotal );
			}
			if( $retorno == 'ok' ){
				$strTotalProcessado++;
			}else{
				$strTotalnaoProcessado++;
			}
				//ver($retorno,d);
			//}
		}*/
		$id_total = 'cpa';
	}elseif( $documento == 'obterPerguntasJustificativa' ){
		$obterPerguntasJustificativa = new obterPerguntasJustificativa();
		$obterPerguntasJustificativa->credencial = $credencial;
		$obterPerguntasJustificativaResponse = $wsAlteracao->obterPerguntasJustificativa( $obterPerguntasJustificativa );
		ver($obterPerguntasJustificativaResponse,d);
	}else {
		$operacoes = $wsAlteracao->__getFunctions();
		$tipos = $wsAlteracao->__getTypes();
		
		ver($operacoes);
		ver($tipos);
	}
	echo "<script>
			alert('Operação realizada com sucesso!');
		 </script>";
	//$db->sucesso('sistema/comunica/importaAlteracoesOrcamentaria');
	//ver($cadastrarPedidoAlteracaoResponse,d);
	//exit();
}

// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Alterações Orçamentárias');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none
	}
</style>
<style>
	label { cursor: pointer; }
</style>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao">
	<input type="hidden" name="mcrid" id="mcrid">
	<input type="hidden" name="id_div" value="<?=$id; ?>">
	<input type="hidden" name="id_div_total" id="id_div_total" value="<?=$id_total; ?>">
<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita" width="19%">Momento de Credito:</td>
		<td><?
		$sql = "SELECT m.mcrid as codigo,
					m.mcrdsc as descricao 
				FROM elabrev.momentocredito m 
				WHERE m.mcrano = '{$_SESSION['exercicio']}' and m.mcrstatus = 'A'";
		$mcrid = $_POST['mcrid'];
		$db->monta_combo("mcrid", $sql, 'S','-- Selecione um mês --','carregaPedido', '', '',150,'S','mcrid', '', '', 'Especificação da Ação');
		?></td>
	</tr>
	<?if( $_POST['mcrid'] ){ ?>
	<tr>
		<td class="subtitulodireita">Funções:</td>
		<td style="padding: 0 20px 20px 20px;">
			<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_cadastra" value="cadastrarPedidoAlteracao" onclick="mostraCombo('cadastra')"/>
				Cadastrar Pedido Alteracao:
					<div id="cadastra" style="display: 'none';">
				<?php 
				$sql = "SELECT 
							codigoTipoAlteracao||'_'||codigouo as codigo, codigoTipoAlteracao||' - '||descricao as descricao
						FROM
						( 
						SELECT 
						    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d 
						    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
						    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
						    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
						    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
						    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
		                    left join public.unidade 			un on un.unicod = a.unicod
						WHERE 
							d.dpavalor > 0 
						    and d.mcrid = $mcrid
							and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
						    
						union all 
						 
						SELECT
							d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma, 
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricao,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d
							INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod like '26%' and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
						    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
						    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
						    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
						    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
		                    left join public.unidade 			un on un.unicod = a.unicod
						WHERE 
							d.dpavalor < 0 
						    and d.mcrid = $mcrid
						    and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
						) as foo
						where 
							identificadorunico is null
						group by 
							codigoTipoAlteracao, descricao, codigouo
		                order by codigouo";
		                
				mostrarComboPopup('<b>Pedidos de Crédito</b>', 'pedido_cadastra',  $sql, $stSqlCarregados, 'Selecione a(s) Pedido de Crédito(s)' );
				?></div>
			</label>
			</td></tr>
		</table>
		</td>
	</tr>
	<?} ?>
	<!-- <tr>
		<td class="subtitulodireita">Funções:</td>
		<td style="padding: 0 20px 20px 20px;">
			<label><input type="radio" name="documento" value="cadastrarPedidoAlteracao" checked/>Cadastrar Pedido Alteracao</label><br/>
			<label><input type="radio" name="documento" value="XMLcadastrarPedidoAlteracao"/>Gerar XML Cadastrar Pedido Alteracao</label><br/>
			<label><input type="radio" name="documento" value="obterPedidoAlteracao"/>Obter Pedido Alteracao</label><br/>
			<label><input type="radio" name="documento" value="obterPerguntasJustificativa"/>Obter Perguntas Justificativa</label><br/>
			&nbsp;<br/>
			Limit "SQL":<input type="text" name="limit" id="limit" value="">
		</td>
	</tr>-->
	<tr>
		<td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" id="btnenviar" value="Enviar Solicitação"></td>
	</tr>
</table>
<div id="debug"></div>

<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php
				$wsusuario = $usuario; 
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'", '', WEB_SERVICE_SIOP_USUARIO) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?php echo WEB_SERVICE_SIOP_SENHA; ?>" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" id="btn_enviar" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<script type="text/javascript">

document.getElementById('tr_pedido_cadastra').style.display = 'none';
/*document.getElementById('tr_acaid_consulta').style.display = 'none';
document.getElementById('tr_acaid_exclui').style.display = 'none';*/

var form = document.getElementById('formulario');

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function mostraCombo(tipo){
	
	for( i=0; i<form.length; i++ ){
		if( form.elements[i].type == 'select-multiple' ){
			if( document.getElementById('documento_'+tipo).checked == true ){
				document.getElementById('tr_pedido_'+tipo).style.display = '';
				document.getElementById( 'pedido_'+tipo+'_campo_on' ).style.display = '';
				document.getElementById( 'pedido_'+tipo+'_campo_off' ).style.display = 'none';
				document.getElementById( 'pedido_'+tipo+'_campo_flag' ).value = '1';
			}
			
			var arObjeto = form.elements[i].id.split('_');
			if( tipo != arObjeto[1] ){
				document.getElementById('tr_pedido_'+arObjeto[1]).style.display = 'none';
				document.getElementById( 'pedido_'+arObjeto[1]+'_campo_on' ).style.display = 'none';
				document.getElementById( 'pedido_'+arObjeto[1]+'_campo_off' ).style.display = 'none';
				document.getElementById( 'pedido_'+arObjeto[1]+'_campo_flag' ).value = '0';	
			}
		}
	}
}

$('#btnenviar').click(function(){
	var docCadastra = document.getElementById('documento_cadastra').checked;
	
	if( docCadastra == false ){
		alert('Selecione uma função');
		return false;
	} else {
		if( docCadastra == true ){
			var pedido_cadastra = document.getElementById('pedido_cadastra').options;
			if( pedido_cadastra[0].value == '' ){
				alert('Selecione pelo menos um pedido de crédito!');
				return false;	
			}
		}
		$('#div_auth').css('display', 'block');
	}
});

$(document).ready(function(){
	$('#btn_enviar').click(function(){
			
		var usuario = $('#wsusuario').val();
		var senha = $('#wssenha').val();
		var id_div = $("[name='id_div']").val();
		
		//jQuery("[name='aggid']")
		
		if(!usuario) {
			alert('Favor informar o usuário!');
			return false;
		}
		
		if(!senha) {
			alert('Favor informar a senha!');
			return false;
		}
		
		selectAllOptions( document.getElementById('pedido_cadastra') );
		$('#requisicao').val('enviasolicitacao');
		$("#"+id_div).css('display', 'none');
		$('#formulario').submit();
	});
	
	/*var id_div_funcao = $('#id_div_total').val();
	if( id_div_funcao != '' ){
		$('#'+id_div_funcao).css('display', '');
	}*/
});

function carregaPedido(mcrid){
	$('#mcrid').val(mcrid);
	$('#requisicao').val('');
	$('#formulario').submit();
}

function mostraErroWS(tipo){
	if( tipo == 'cpa' ){
		window.open('monitora.php?modulo=sistema/comunica/mostraErroWS&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
	}
} 
</script>
</form>
<div id="cpa" style="display: ''">
<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td width="30%" class="SubtituloDireita">Total de registro para processar:</td>
		<td><?//=$obAlteracaoDAO->carregarTotalCredito( $_POST['limit'] ); ?></td>
	</tr>
	<tr>
		<td width="30%" class="SubtituloDireita">Total de registro processado:</td>
		<td><?=$strTotalProcessado; ?></td>
	</tr>
	<tr>
		<td width="30%" class="SubtituloDireita">Total de registro não processado:</td>
		<td><?=$strTotalnaoProcessado; ?></td>
	</tr>
	<tr>
		<td width="30%" class="SubtituloDireita">Tela de Erro:</td>
		<td><a href="#" onclick="mostraErroWS('cpa');">Clique Aqui</a></td>
	</tr>
</table>
</div>