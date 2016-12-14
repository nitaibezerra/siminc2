<?php
//error_reporting(E_ALL);
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once APPRAIZ."elabrev/classes/AlteracaoOrcamentariaDAO.class.inc";

function montaTabelaRetornoAlteracao( $arrResponse = array(), $funcao = '' ){
	global $db;
	$obAlteracaoDAO = new AlteracaoOrcamentariaDAO();
	if( $funcao == 'obterPedidoAlteracao' || $funcao == 'excluirPedidoAlteracao' ){
		$caption = 'Identificador';
	} else if( $funcao == 'obterPerguntaJustificativa' ){
		$caption = 'Pergunta';
	} else {
		$caption = 'CódigoUO';	
	}
	
	$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
				<tr>
					<th colspan="4">Retorno WSAlteração</th>
				</tr>
				<tr>
					<th width="10%">Momento de Crédito</th>
					<th width="10%">Tipo Alteração</th>
					<th width="10%">'.$caption.'</th>
					<th width="70%">Mensagem Retorno</th>
				</tr>';
	
	foreach ($arrResponse as $key => $retorno) {
		/*$arrPedido = $obAlteracaoDAO->carregarPedido( $mcrid, $tipoalteracao, $codigouo );
		
		foreach ($arrPedido as $pedido) {*/
		if( $retorno['retorno'] != 'erro' ){
			if(!$retorno['retorno']->return->sucesso){
				if( is_array($retorno['retorno']->return->mensagensErro) ){
					$strMensagem = implode('<br>', $retorno['retorno']->return->mensagensErro);
				} else {
					$strMensagem = $retorno['retorno']->return->mensagensErro;
				}
				$mensagem = '<b>Detalhe do erro:</b> <br><br>'.$strMensagem;
				$corTD = 'red';
			} else {
				if( $funcao == 'cadastrarPedidoAlteracao' ){
					$mensagem = 'Crédito Orçamentário cadastrada com sucesso!';					
					$obAlteracaoDAO->gravaRetornoCadastrarPedido( $retorno );
					
				} else if( $funcao == 'obterPedidoAlteracao' ){
					$mensagem = 'Consulta realizada com sucesso!';
					$retornoGravado = $obAlteracaoDAO->gravaRetornoObterPedidosAlteracao( $retorno['retorno'] );
					$mensagem = $mensagem . '<br>Segue abaixo o total de registro inserido:<br>'.implode( '<br>', $retornoGravado );
					
				}else if( $funcao == 'obterPedidosAlteracao' ){
					$mensagem = 'Consulta realizada com sucesso!';
					$retornoGravado = $obAlteracaoDAO->gravaRetornoObterPedidosAlteracao( $retorno['retorno'] );
					$mensagem = $mensagem . '<br>Segue abaixo o total de registro inserido:<br>'.implode( '<br>', $retornoGravado );
					
				} else if( $funcao == 'excluirPedidoAlteracao' ){
					$mensagem = 'Pedido(s) excluido(s) com sucesso!';
					
					$sql = "UPDATE elabrev.despesaacao SET dpaidentificadorunico = null WHERE dpaidentificadorunico in(".$retorno['codigouo'].")";
					$db->executar( $sql );
				} else if( $funcao == 'obterPerguntaJustificativa' ){
					$mensagem = 'Consulta realizada com sucesso!';
					
					$obAlteracaoDAO->gravaPerguntaResposta( $retorno['retorno']->return->registros );
					
					$codigoPergunta = $retorno['retorno']->return->registros->codigoPergunta;
					$pergunta = $retorno['retorno']->return->registros->pergunta;
					$msgpergunta = ' - '.$pergunta;
					
				} else if( $funcao == 'obterPerguntasJustificativa' ){
					$mensagem = 'Consulta realizada com sucesso!';
					
					$obAlteracaoDAO->gravaPerguntaResposta( $retorno['retorno']->return->registros );					
				} else if( $funcao == 'obterSaldosAcoesPAC' ){
					$mensagem = 'Consulta realizada com sucesso!';
					
					//$obAlteracaoDAO->gravaPerguntaResposta( $retorno['retorno']->return->registros );
				} else if( $funcao == 'obterTabelasApoioAlteracoesOrcamentarias' ){
					$mensagem = 'Consulta realizada com sucesso!';
					$retornoGravado = $obAlteracaoDAO->gravaRetornoApoioAlteracoesOrcamentarias( $retorno['retorno']->return );
					$mensagem = $mensagem . '<br>Segue abaixo o total de registro inserido:<br>'.implode( '<br>', $retornoGravado );					
				}				
				else {
					/*$mensagem = 'Proposta excluida com sucesso!';
					$sql = "UPDATE elabrev.ppaacao_orcamento SET acapropostaenviada = false, acadtenviosiop = now() WHERE acaid = {$retorno['acaid']}";
					$db->executar( $sql );
					$db->commit();*/
				}
				$corTD = 'blue';
			}
		} else {
			$mensagem = 'Não foi encontrado registro.';
			$corTD = 'red';
		}
		
		 			
			$arrRegistro = $arrRegistro['retorno']->return->registros;
			$arrFisico = $arrRegistro->fisicosPedidoAlteracao;
			if( is_object($arrFisico ) ){
				
				$strFuncional = '<br /><br />
								Ação: '.$arrFisico->codigoAcao .' <br />
								Programa: '.$arrFisico->codigoPrograma.' <br />
								Localizador: '.$arrFisico->codigoLocalizador .' <br />';
			}
		
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td>'.$retorno['mcrid'].'</td>
						<td>'.$retorno['tipoalteracao'].'</td>
						<td>'.$retorno['codigouo'].$msgpergunta.'</td>
						<td style="color: '.$corTD.';">'.$mensagem.' 
						 '.$strFuncional.'
						</td>
					</tr>';
		//}
	}
	$html.= '</table>';
	return $html;
}

if($_POST['requisicao'] == 'enviasolicitacao') {
	include(APPRAIZ."elabrev/classes/WSAlteracoesOrcamentaria.class.inc");
	include(APPRAIZ."elabrev/classes/MontaObjetoAlteracao.class.inc");
	
	$documento = $_POST['documento'];
	
	$arrParam = array('usuario' => $_POST['wsusuario'],
					  'senha' 	=> $_POST['wssenha'],
					  'post' 	=> $_POST
					);
	
	if($documento == 'obterPedidoAlteracao') {
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterPedidoAlteracao();
		//ver($retorno,d);
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterPedidoAlteracao' );
		
	}else if($documento == 'obterPedidosAlteracao') {
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterPedidosAlteracao();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterPedidosAlteracao' );
		//ver($retorno);		
		
	}else if( $documento == 'cadastrarPedidoAlteracao'){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->cadastrarPedidoAlteracao();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'cadastrarPedidoAlteracao' );
		
	} else if( $documento == 'excluirPedidoAlteracao' ){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->excluirPedidoAlteracao();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'excluirPedidoAlteracao' );
		
	}elseif( $documento == 'obterPerguntaJustificativa' ){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterPerguntaJustificativa();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterPerguntaJustificativa' );
		
	}elseif( $documento == 'obterPerguntasJustificativa' ){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterPerguntasJustificativa();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterPerguntasJustificativa' );
		
	}elseif( $documento == 'obterSaldosAcoesPAC' ){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterSaldosAcoesPAC();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterSaldosAcoesPAC' );
	
	}elseif( $documento == 'obterTabelasApoioAlteracoesOrcamentarias' ){
		$obAlteracao = new MontaObjetoAlteracao( $arrParam ); 
		$retorno = $obAlteracao->obterTabelasApoioAlteracoesOrcamentarias();
		$htmlRetorno = montaTabelaRetornoAlteracao( $retorno, 'obterTabelasApoioAlteracoesOrcamentarias' );
		
	}
	else {
		/*$operacoes = $wsAlteracao->__getFunctions();
		$tipos = $wsAlteracao->__getTypes();
		
		ver($operacoes);
		ver($tipos);*/
	}
	$_SESSION['elabrev']['retornows'] = $htmlRetorno;
	$db->sucesso('sistema/comunica/importaAlteracoesOrcamentaria');
	exit();
}

// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Alterações Orçamentárias');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "270px";
$altura 	= "150px";
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
<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita" width="19%">Momento de Credito:</td>
		<td><?
		$sql = "SELECT m.mcrid as codigo,
					m.mcrdsc as descricao 
				FROM elabrev.momentocredito m 
				WHERE m.mcrano = '{$_SESSION['exercicio']}' and m.mcrstatus = 'A' order by 2";
		$mcrid = $_POST['mcrid'];
		$db->monta_combo("mcrid", $sql, 'S','-- Selecione um mês --','carregaPedido', '', '',150,'S','mcrid', '', '', 'Especificação da Ação');
		?></td>
	</tr>
	<?if( $_POST['mcrid'] ){ 
		
		$sql = "SELECT a.unicod  
				FROM elabrev.despesaacao d 
				INNER JOIN monitora.acao a on  a.acaid = d.acaidloa
				WHERE
					a.unicod like '26%' 
				    and a.prgano = '{$_SESSION['exercicio']}' 
				    and a.acasnrap = 'f' 
					and d.mcrid = {$_POST['mcrid']}";
		$arrUnicod = $db->carregarColuna( $sql );
		$arrUnicod = $arrUnicod ? $arrUnicod : array();
	?>
	<tr>
		<td class="subtitulodireita">Funções:</td>
		<td style="padding: 0 20px 20px 20px;">
			<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_cadastra" value="cadastrarPedidoAlteracao" onclick="mostraCombo('cadastra')"/>
				<b>Cadastrar Pedido Alteração</b>
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
						    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod in ('".implode("', '", $arrUnicod)."') 
						    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
						    INNER JOIN public.idoc 				 i  on i.idoid = d.idoid 
						    LEFT JOIN public.naturezadespesa 	 n  on n.ndpid = d.ndpid 
						    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid 
		                    left join public.unidade 			un on un.unicod = a.unicod
						WHERE 
							d.dpavalor > 0 
						    and d.mcrid = $mcrid
							--and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
						    
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
							INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod in ('".implode("', '", $arrUnicod)."')
						    INNER JOIN elabrev.tipocredito 		tc on tc.tcrid = d.tcrid
						    INNER JOIN public.idoc 				 i on i.idoid = d.idoid
						    LEFT JOIN public.naturezadespesa 	 n on n.ndpid = d.ndpid
						    LEFT JOIN public.naturezareceita 	nr on nr.nrcid = d.nrcid
		                    left join public.unidade 			un on un.unicod = a.unicod
						WHERE 
							d.dpavalor < 0 
						    and d.mcrid = $mcrid
						    --and ( trim(d.dpanumerodecontrole) is null or trim(d.dpanumerodecontrole) = '' )
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
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_exclui" value="excluirPedidoAlteracao" onclick="mostraCombo('exclui')"/>
				<b>Excluir Pedido Alteração</b>
					<div id="exclui" style="display: 'none';">
				<?php 
				$sql = "SELECT 
							codigoTipoAlteracao||'_'||identificadorunico as codigo, codigoTipoAlteracao||' - '||descricaoUO as descricao
						FROM
						( 
						SELECT 
						    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricaoUO,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d 
						    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricaoUO,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d
							INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
							identificadorunico is not null
						group by identificadorunico, descricaoUO, codigoTipoAlteracao
		                order by descricaoUO, codigoTipoAlteracao";
		               
				mostrarComboPopup('<b>Pedidos de Crédito</b>', 'pedido_exclui',  $sql, $stSqlCarregados, 'Selecione a(s) Pedido de Crédito(s)' );
				?></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterpedido" value="obterPedidoAlteracao" onclick="mostraCombo('obterpedido')"/>
				<b>Obter Pedido Alteração</b>
					<div id="obterpedido" style="display: 'none';">
				<?php 
				$sql = "SELECT 
							codigoTipoAlteracao||'_'||identificadorunico as codigo, codigoTipoAlteracao||' - '||descricaoUO as descricao
						FROM
						( 
						SELECT 
						    d.dpaidentificadorunico as identificadorunico, d.tcrid as idcredito, d.mcrid, a.unicod as codigouo, d.dpatipovalor as codigotipofonterecurso, case when tc.tcrcod in ('102', '103', '160', '600', '175') then tc.tcrcod||'a' else tc.tcrcod end as codigotipoalteracao, a.prgcod as codigoprograma,  
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricaoUO,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa, 
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza, 
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d 
						    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
						    a.acacod as codigoacao, a.loccod as codigolocalizador, d.foncod as codigofonte, un.unicod||' - '||un.unidsc as descricaoUO,
						    i.idocod as codigoidoc, d.iducod as codigoiduso, a.irpcod as codigorp, d.dpavalor as valordespesa,
						    CASE coalesce(trim(nr.nrccod), '') WHEN '' THEN n.ndpcod ELSE nr.nrccod END as codigonatureza,
						    d.dpaid as sequencial, a.esfcod as codigoesfera, a.funcod as codigofuncao, a.sfucod as codigosubfuncao,
		                    '0' as quantidadeacrescimo, '0' as quantidadereducao, '{$_SESSION['exercicio']}' as exercicio, '6' as codigotipoinclusaolocalizador,
		                     '". CODIGO_ORGAO_SISTEMA. "' AS  codigoorgao, '9200' as codigomomento, '1' as codigoclassificacaoalteracao
						FROM elabrev.despesaacao d
							INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
							identificadorunico is not null
						group by identificadorunico, descricaoUO, codigoTipoAlteracao
		                order by descricaoUO, codigoTipoAlteracao";
		               
				mostrarComboPopup('<b>Pedidos de Crédito</b>', 'pedido_obterpedido',  $sql, $stSqlCarregados, 'Selecione a(s) Pedido de Crédito(s)' );
				?></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterpedidos" value="obterPedidosAlteracao" onclick="mostraCombo('obterpedidos')"/>
				<b>Obter Pedidos Alteração</b>
					<div id="obterpedidos" style="display: 'none';">
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
						    INNER JOIN monitora.acao 			 a on  a.acaid = d.acaidloa and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
							INNER JOIN monitora.acao 			 a on a.acaid = d.acaidloa  and a.unicod in ('".implode("', '", $arrUnicod)."') and a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = 'f' 
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
							identificadorunico is not null
						group by 
							codigoTipoAlteracao, descricao, codigouo
		                order by codigouo";
		               
				mostrarComboPopup('<b>Pedidos de Crédito</b>', 'pedido_obterpedidos',  $sql, $stSqlCarregados, 'Selecione a(s) Pedido de Crédito(s)' );
				?></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterpergunta" value="obterPerguntaJustificativa" onclick="mostraTexto('obterpergunta')"/>
				<b>Obter Pergunta Justificativa</b>
					<div id="obterpergunta" style="display: 'none';">
				<?php 
				$sql = "";
		        echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		        		Codigo da Pergunta: ' . campo_texto( 'pedido_obterpergunta', 'N', "S", '', 10, 10, '', '', '', '', 0, 'id="pedido_obterpergunta"' );
				//mostrarComboPopup('<b>Pedidos de Crédito</b>', 'pedido_obterpergunta',  $sql, $stSqlCarregados, 'Selecione a(s) Pedido de Crédito(s)' );
				?></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterperguntas" value="obterPerguntasJustificativa" onclick="mostraTexto('obterperguntas')"/>
				<b>Obter Perguntas Justificativa</b>
					<div id="obterperguntas" style="display: 'none';"></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obtersaldopac" value="obterSaldosAcoesPAC" onclick="mostraTexto('obtersaldopac')"/>
				<b>Obter Saldos Acoes PAC</b>
					<div id="obtersaldopac" style="display: 'none';"></div>
			</label>
			</td></tr>
			<tr><td>
			<label>
				<input type="radio" name="documento" id="documento_obterstabelaapoio" value="obterTabelasApoioAlteracoesOrcamentarias" onclick="mostraTexto('obterstabelaapoio')"/>
				<b>Obter Tabelas Apoio Alteracoes Orcamentarias</b>
					<div id="obterstabelaapoio" style="display: 'none';"></div>
			</label>
			</td></tr>
		</table>
		</td>
	</tr>
	<?} ?>
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
<?
if( $_SESSION['elabrev']['retornows'] ){
	echo $_SESSION['elabrev']['retornows'];
}
unset($_SESSION['elabrev']['retornows']);
?>
<script type="text/javascript">

document.getElementById('tr_pedido_cadastra').style.display = 'none';
document.getElementById('tr_pedido_exclui').style.display = 'none';
document.getElementById('tr_pedido_obterpedido').style.display = 'none';
document.getElementById('tr_pedido_obterpedidos').style.display = 'none';

document.getElementById('obterpergunta').style.display = 'none';

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
	document.getElementById('obterpergunta').style.display = 'none';
}

function mostraTexto(tipo){
	if( tipo == 'obterperguntas' || tipo == 'obtersaldopac' || tipo == 'obterstabelaapoio' ){
		document.getElementById(tipo).style.display = 'none';
		document.getElementById('obterpergunta').style.display = 'none';
		document.getElementById('obtersaldopac').style.display = 'none';
		document.getElementById('obterstabelaapoio').style.display = 'none';
	} else {
		document.getElementById(tipo).style.display = '';
	}
	
	for( i=0; i<form.length; i++ ){
		if( form.elements[i].type == 'select-multiple' ){			
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
	var docExclui = document.getElementById('documento_exclui').checked;
	var docObterpedido = document.getElementById('documento_obterpedido').checked;
	var docObterpedidos = document.getElementById('documento_obterpedidos').checked;
	var docObterpergunta = document.getElementById('documento_obterpergunta').checked;
	var docObterperguntas = document.getElementById('documento_obterperguntas').checked;
	var docObtersaldopac = document.getElementById('documento_obtersaldopac').checked;
	var docObterstabelaapoio = document.getElementById('documento_obterstabelaapoio').checked;
	
	if( docCadastra == false && docExclui == false && docObterpedido == false && docObterpedidos == false  && docObterpergunta == false && docObterperguntas == false && docObtersaldopac == false && docObterstabelaapoio == false ){
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
		if( docExclui == true ){
			var pedido_exclui = document.getElementById('pedido_exclui').options;
			if( pedido_exclui[0].value == '' ){
				alert('Selecione pelo menos um pedido de crédito!');
				return false;	
			}
		}
		if( docObterpedido == true ){
			var pedido_obterpedido = document.getElementById('pedido_obterpedido').options;
			if( pedido_obterpedido[0].value == '' ){
				alert('Selecione pelo menos um pedido de crédito!');
				return false;	
			}
		}
		if( docObterpedidos == true ){
			var pedido_obterpedidos = document.getElementById('pedido_obterpedidos').options;
			if( pedido_obterpedidos[0].value == '' ){
				alert('Selecione pelo menos um pedido de crédito!');
				return false;	
			}
		}
		if( docObterpergunta == true ){
			var pedido_obterpergunta = document.getElementById('pedido_obterpergunta').value;
			if( pedido_obterpergunta == '' ){
				alert('informe o codigo da pergunta!');
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
		//var id_div = $("[name='id_div']").val();
		
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
		selectAllOptions( document.getElementById('pedido_exclui') );
		selectAllOptions( document.getElementById('pedido_obterpedido') );
		selectAllOptions( document.getElementById('pedido_obterpedidos') );
		//selectAllOptions( document.getElementById('pedido_obterpergunta') );
		$('#requisicao').val('enviasolicitacao');
		//$("#"+id_div).css('display', 'none');
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
		window.open('elabrev.php?modulo=sistema/comunica/mostraErroWS&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
	}
} 
</script>
</form>