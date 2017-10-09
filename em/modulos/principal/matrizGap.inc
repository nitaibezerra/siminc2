<?php

if(!$_SESSION['emiid']){
	echo "<script>
			alert('Erro de variável, refaça o procedimento!');
			window.close();
		  </script>";
	exit;
}

if($_REQUEST['requisicao'] == 'excluirItem'){
	if($_REQUEST['mdoid']){
		$sql = "delete from em.emimatrizdistribuicaoorcamentargap where mdoid = ".$_REQUEST['mdoid'];
		$db->executar($sql);
		if($db->commit()){
			die('true');
		}
	}
	die('false');
}

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid']   );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}

if(empty($_REQUEST['papid']) || !isset($_REQUEST['papid']))
	$_REQUEST['papid'] = $_SESSION['papid'];
else
	$_SESSION['papid'] = $_REQUEST['papid'];

// Editar item GAP
if($_REQUEST["requisicao"] == 'alteraritemGap'){
	
	$sql = "SELECT
					mdoid,
					itfid, 
					unddid,
					trim(mdoespecificacao) as mdoespecificacao, 
					mdoqtd, 
					mdovalorunitario, 
					mdototal
				FROM
					em.emimatrizdistribuicaoorcamentargap
				WHERE
					mdoid = {$_REQUEST['mdoid']}";
		
		$dados = $db->pegaLinha( $sql );
		
		$dados["mdoespecificacao"] = iconv("ISO-8859-1", "UTF-8", $dados["mdoespecificacao"]);
		$dados["mdovalorunitario"] = number_format( $dados["mdovalorunitario"], 2, ",", "." );
		$dados["mdototal"]		   = number_format( $dados["mdototal"], 2, ",", "." );
		
		echo simec_json_encode($dados);
		die;
}
	
// Salva itens	
if($_REQUEST["requisicao"] == 'salvaDados'){
	
	$dados = $_REQUEST;
	
	$mdoespecificacao = "'".substr($dados["mdoespecificacao"],0,1000)."'";
	
	/*
	if( substr( $mdoespecificacao, -1 ) != "'" ){
		$mdoespecificacao = "'" . $mdoespecificacao . "'";	
	}
	*/
	
	$undid	 		  = $dados["undid"] ? $dados["undid"] : 'null';
	$mdoqtd 		  = $dados["mdoqtd"] && $dados["mdoqtd"] != "''" ? $dados["mdoqtd"] : 'null';
	$mdovalorunitario = $dados["mdovalorunitario"] ? str_replace( ".", "",  $dados["mdovalorunitario"] ) : 'null';
	$mdovalorunitario = $mdovalorunitario && $mdovalorunitario != "''" ? str_replace( ",", ".", $mdovalorunitario ) : 'null';
	$mdototal 		  = $dados["mdototal"] && $dados["mdototal"] != "''" ? str_replace( ",", ".", str_replace( ".", "",  $dados["mdototal"] )) : 'null';	
	$mdoespecificacao = $mdoespecificacao && $mdoespecificacao != "''" ? $mdoespecificacao : 'null';
	$itfid			  = $dados["itfid"] ? $dados["itfid"] : 'null';
	
	$docid = pegaDocid( $_SESSION['entid'], $_SESSION['emiid']);		
	
	$sql = "SELECT 
				count(1) as count 
			FROM workflow.historicodocumento 
			WHERE aedid = '".WF_AEDID_RETURN_PREENCHIMENTO."' 
			AND docid = '".$docid."' ";
	
	$boCorrecao = $db->pegaUm($sql);
	
	$papflagalterado = ($boCorrecao) ? 'true':'false';
	
	if ( !$dados["mdoid"] || $dados["mdoid"] == "''" ){
		
		$sql = "INSERT INTO em.emimatrizdistribuicaoorcamentargap 
					( papid, unddid, itfid, mdoespecificacao, mdoqtd, mdovalorunitario, mdototal, mdostatus, mdoflagalterado )
				VALUES 
					( {$dados["papid"]}, {$undid}, {$itfid}, {$mdoespecificacao}, {$mdoqtd}, {$mdovalorunitario}, {$mdototal}, 'A', {$papflagalterado} )";			
																 
	}else{
		
		$sql = "UPDATE
					em.emimatrizdistribuicaoorcamentargap
				SET
					itfid = {$itfid}, 
					unddid = {$undid}, 
					mdoespecificacao = {$mdoespecificacao}, 
					mdoqtd = {$mdoqtd}, 
					mdovalorunitario = {$mdovalorunitario}, 
					mdototal = {$mdototal},
					mdoflagalterado = {$papflagalterado}
				WHERE
					mdoid = {$dados["mdoid"]}";
		
	}

	$db->executar( $sql );		
	$db->commit();
	$db->sucesso( "principal/matrizGap" );
	
}


if(!$_REQUEST["papid"] && !$_SESSION["emi"]["papid"]){
	print "<script>
				  alert('O PRC selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
}

if ( $_REQUEST["papid"] || $_SESSION["emi"]["papid"] ){

	$_SESSION["emi"]["papid"] = !empty($_REQUEST["papid"]) ? $_REQUEST["papid"] : $_SESSION["emi"]["papid"];
	$papid = $_SESSION["emi"]["papid"];
	
	$sql = "SELECT papid FROM em.emigap WHERE papid = {$papid}";	
	$verificaGap = $db->pegaUm( $sql );
	
	if ( !$verificaGap ){
		
		unset( $_SESSION["emi"]["papid"] );
		
		print "<script>
				  alert('O PRC selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
	}
	
}


monta_titulo("Matriz de Distribuição Orçamentária / Ação-Atividade", "PRC 2012");
echo montaCabecalhoPRC($_SESSION['emiid'], $_SESSION['mcpid'], $_REQUEST['papid'], false);
echo "<br/>";

$abacod_tela = 57532;
$url 		 = 'em.php?modulo=principal/matrizGap&acao=A';
$parametros  = null;
$arMnuid 	 = $esdid == WF_ESDID_EM_ANALISE_SEDUC ? array() : array(11189);
$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>	    
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script>
	    
	    	jQuery.noConflict();
	    	
			function salvarBeneficiarios()
			{
				document.escola_Form.submit();
			}
			function insereDadosMatriz()
			{
				var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
				var validacao = true;
				
				var itfid 			 = document.getElementById('itfid');
				var mdoespecificacao = document.getElementById('mdoespecificacao');
				var undid 			 = document.getElementById('undid');
				var mdoqtd 			 = document.getElementById('mdoqtd');
				var mdovalorunitario = document.getElementById('mdovalorunitario');
				var mdototal		 = document.getElementById('mdototal');
				
				var emrcusteio	 	 = document.getElementById('emrcusteio');
				var emrcapital	 	 = document.getElementById('emrcapital');
				
				var saldocusteio	 = replaceAll(document.getElementById('td_saldo_custeio').innerHTML,'.','').replace(',','.');
				var saldocapital	 = replaceAll(document.getElementById('td_saldo_capital').innerHTML,'.','').replace(',','.');
				var saldoconsultoria = parseFloat(document.getElementById('emrcusteio').value)*0.1;
				
				var totalItem = replaceAll(mdototal.value, '.','').replace(',','.');
				var saldo = 0;
				
				var arCusteio  = [8,5,3,2,7,1,4,6,13];
			    var arCapital  = [9,11,10,12];
				
				if(inArray(itfid.value, arCusteio)){
					saldo = parseFloat(saldocusteio)-parseFloat(totalItem);
					if(itfid.value == 7){						
						if(totalItem > saldoconsultoria){
							alert('Valor não pode ultrapassar 10% do saldo do custeio. (R$ '+MascaraMonetario(parseFloat(saldoconsultoria).toFixed(2))+')');
							return false;
						}
					}
					if(saldo < 0){
						alert('Valor não pode ultrapassar o saldo do custeio. (R$ '+MascaraMonetario(parseFloat(saldocusteio).toFixed(2))+')');
						return false;
					}
					calculo = parseFloat(saldocapital)+parseFloat(totalItem);
					document.getElementById('td_saldo_custeio').innerHTML = MascaraMonetario(parseFloat(calculo).toFixed(2));
				}else{
					saldo = parseFloat(saldocapital)-parseFloat(totalItem);				
					if(saldo < 0){
						alert('Valor não pode ultrapassar o saldo do capital. (R$ '+MascaraMonetario(parseFloat(saldocapital).toFixed(2))+')');
						return false;
					}
					calculo = parseFloat(saldocusteio)+parseFloat(totalItem);
					document.getElementById('td_saldo_capital').innerHTML = MascaraMonetario(parseFloat(calculo).toFixed(2));
				}
			
				if( itfid.value == '' ){
					mensagem += 'Itens Financiáveis \n';
					validacao = false;
				}
				
				if( mdoespecificacao.value == '' ){
					mensagem += 'Especificação \n';
					validacao = false;
				}
				
				if( undid.value == '' ){
					mensagem += 'Unidade \n';
					validacao = false;
				}
				
				if( mdoqtd.value == '' ){
					mensagem += 'Quantidade \n';
					validacao = false;
				}
				
				if( mdovalorunitario.value == '' ){
					mensagem += 'Valor Unitário \n';
					validacao = false;
				}
				
				if( mdototal.value == '' ){
					mensagem += 'Total';
					validacao = false;
				}
			
				if( !validacao ){
					alert(mensagem);
				}else{
					document.getElementById('requisicao').value = "salvaDados";
					document.getElementById('escola_Form').submit();
				}
			
			}
			function preencheTotal(){
			
				var qtd   = document.getElementById( 'mdoqtd' ).value;
				var mdovalorunitario  = document.getElementById( 'mdovalorunitario' ).value;
				var total = 0;
				
				if( mdovalorunitario == '' ) mdovalorunitario = '1';
				
				mdovalorunitario = mdovalorunitario.replace(".", "");
				mdovalorunitario = mdovalorunitario.replace(".", "");
				mdovalorunitario = mdovalorunitario.replace(".", "");
				mdovalorunitario = mdovalorunitario.replace(",", ".");
			
				total = Number( qtd * mdovalorunitario );
			
				total = total.toFixed(2);
			
				total = mascaraglobal('###.###.###.###,##', total);
				
				document.getElementById( 'mdototal' ).value = total;
				
			
			}
			function alterarItemMatrizGap ( mdoid, acao ){
			
				var url = 'em.php?modulo=principal/matrizGap&acao=A';
				var parametros = '&requisicao=alteraritemGap&mdoid=' + mdoid;
			
				var myAjax = new Ajax.Request(
					url,
					{
						method: 'post',
						parameters: parametros,
						asynchronous: false,
						onComplete: function(resp) {
							
							var json = resp.responseText.evalJSON();
							
							var saldocusteio	 = replaceAll(document.getElementById('td_saldo_custeio').innerHTML,'.','').replace(',','.');
							var saldocapital	 = replaceAll(document.getElementById('td_saldo_capital').innerHTML,'.','').replace(',','.');
						
							var totalitem = replaceAll(json.mdototal,'.','').replace(',','.');				
											
						    arCusteio  = [8,5,3,2,7,1,4,6,13];
						    arCapital  = [9,11,10,12];
			
							if(inArray(json.itfid, arCusteio)){					
								calculo = parseFloat(saldocusteio)+parseFloat(totalitem);
								document.getElementById('td_saldo_custeio').innerHTML = MascaraMonetario(parseFloat(calculo).toFixed(2));					
							}else{
								calculo = parseFloat(saldocapital)+parseFloat(totalitem);
								document.getElementById('td_saldo_capital').innerHTML = MascaraMonetario(parseFloat(calculo).toFixed(2));
							}
							
							$('mdoid').value			= json.mdoid;
							$('itfid').value 			= json.itfid;
							$('undid').value 			= json.unddid;
							$('mdoespecificacao').value = json.mdoespecificacao;
							$('mdoqtd').value 			= json.mdoqtd;
							$('mdovalorunitario').value = json.mdovalorunitario;
							$('mdototal').value 		= json.mdototal;
							
						}
					}
				);
				
			}
			function excluiItemMatrizGap( mdoid, acao ){
				if( confirm("Deseja realmente excluir este item?") ){
					window.location.href = "em.php?modulo=principal/matrizGap&acao=" + acao + "&requisicao=excluiritemGap&mdoid=" + mdoid;
				}
			}
			
			function inArray(needle, haystack) {
			    var length = haystack.length;
			    for(var i = 0; i < length; i++) {
			        if(haystack[i] == needle) return true;
			    }
			    return false;
			}
			
			function excluirItemFinanciavel(mdoid)
			{	
				if(confirm('Deseja excluir o registro?')){			
					jQuery.ajax({
						url: 'em.php?modulo=principal/matrizGap&acao=A',
						type: 'post',
						data: 'requisicao=excluirItem&mdoid='+mdoid,
						success: function(e){
							if(e == 'true'){	
								alert('Registro excluído com sucesso.');						
								document.location.href = 'em.php?modulo=principal/matrizGap&acao=A';		
							}
						}
					});
				}
			}
			function listarItens()
			{
				var janela = window.open("?modulo=principal/listaItensFinanciaveis&acao=A", "prcEscolaItens", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");
				janela.focus();
			}
		</script>
	</head>
	<body>
		<form method="post" name="escola_Form" id="escola_Form" action="">
			<input type="hidden" id="requisicao" name="requisicao" value="salvaBeneficiarios"/>
			<input type="hidden" id="papid" name="papid" value="<?php echo $papid?>"/>
			<table bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="1" align="center" >				
				<tr>
					<td class="subtitulocentro" rowspan="2">Itens Financiáveis</td>
					<td class="subtitulocentro" rowspan="2">Especificação</td>
					<td class="subtitulocentro" rowspan="2">Unidade</td>
					<td class="subtitulocentro" rowspan="1" colspan="3">Estimativa Orçamentária</td>
					<td class="subtitulocentro" rowspan="2">Ação</td>
				</tr>
				<tr>
					<td class="subtitulocentro">Quantidade <br/> A </td>
					<td class="subtitulocentro">Valor Unitário (R$) <br/> B </td>
					<td class="subtitulocentro">Total (R$) <br/> A x B </td>
				</tr>
				<tr>
					<td align="center">
						<?php 
							
							$sql = "SELECT
										itfid as codigo, 
										itfdsc as descricao
									FROM
										em.emiitemfinanciavel
									ORDER BY
										descricao";
							
							$db->monta_combo("itfid", $sql, $ativo, "Selecione...", '', '', '', 120, 'N', 'itfid');
							
						?>
					</td>
					<td>
						<?php echo campo_textarea( 'mdoespecificacao', 'N', $ativo, '', 40, 4, 1000, '', 0, ''); ?>
					</td>
					<td>
						<?php 
							
							$sqlUnidadeMedida = "SELECT
													unddid as codigo,
													undddsc as descricao
												 FROM
												 	cte.unidademedidadetalhamento 
												 WHERE 
												 	unddid NOT IN (
													 	205,208,23,251,284,8,219,21,285,276,213,2,241,11,197,51,17,280,52,57,303,61,
														204,297,307,263,10,191,244,271,187,239,289,25,301,203,245,13,3,70,183,189,
														73,164,240,171,9,82,291,300,277,200,198,275,265,111,238,201,237,227,192,209,
														15,234,233,169,124,216,24,202,144,132,19,20,14,309,248,6,146,254,229,1,22,128,
														190,282,256,268,269,136,27,177,223,137,138,139,184,196,246,232,293,162,273,299,
														272,217,143,247,287,298,294,18,279,149,148,193,230,306,226,152,153,42,186,225,
														302,158,185,159
												 	)
												 ORDER BY
												 	undddsc";
								
							$db->monta_combo("undid", $sqlUnidadeMedida, $ativo, "Selecione...", '', '', '', 120, 'N', "undid" );
						?>
					</td>
					<td align='center'>
						<?php echo campo_texto( 'mdoqtd', 'N', $ativo, '', 8, 8, '########', '', 'left', '', 0, "id='mdoqtd'","", "", "preencheTotal();" ); ?>
					</td>
					<td align='center'>
						<?php echo campo_texto( 'mdovalorunitario', 'N', $ativo, '', 12, 14, '###.###.###,##', '', 'left', '', 0, "id='mdovalorunitario'", "", "", "preencheTotal();"); ?>
					</td>
					<td align='center'>
						<?php echo campo_texto( 'mdototal', 'N', 'N', '', 14, 14, '###.###.###,##', '', 'left', '', 0, "id='mdototal'" ); ?>
					</td>
					<td align="center">
							<input type="hidden" value="" name="mdoid" id="mdoid"/>
						<?php if( $ativo == 'S' ){ ?>
							<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereDadosMatriz();">							
						<?php }else{ ?>
							<img src="/imagens/gif_inclui_d.gif">							
						<?php } ?>
					</td> 
				</tr>
				
				<?php
		
				// inicializa a variável
				$totalGeralAxB = 0;
				
				// Verifica se a opção de cadastro de crítica deve ser exibida.		
				$retorno = false;
			
				// Testa se é 'Cadastrador' ou 'Analista COEM' e verifica o estado do documento para mostrar botão de cadastro de crítica.
				if( checkPerfil(array(PERFIL_CADASTRADOR, EMI_PERFIL_ANALISTACOEM, PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR)) ) {
					
					$docid = pegaDocid( $_SESSION['entid'], $_SESSION['emiid'] );
					
					// O teste com o aedid é para verificar se o documento já foi enviado para preenchimento ou para correção.
					// Enviado para preenchimento: aedid(245) = esdidorigem(62) => esdiddestino(63)
					// Enviado para correção: 	   aedid(246) = esdidorigem(63) => esdiddestino(62)
					if($docid) {
						if( checkPerfil(PERFIL_CADASTRADOR) )
							$aedid = "aedid = 246";
						if( checkPerfil(EMI_PERFIL_ANALISTACOEM) )
							$aedid = "aedid = 245";
						if( checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR)))
							$aedid = "aedid in (245,246)";
						
						$sql = "SELECT
									count(*)
								FROM
									workflow.historicodocumento
								WHERE
									{$aedid} AND 
									docid = {$docid}";
						
						if( (integer)$db->pegaUm($sql) > 0 ) $retorno = true;
					}
					
					// se tiver observação, cadastrador vê mesmo assim a crítica
					if( checkPerfil(PERFIL_CADASTRADOR) && !$retorno ) {
						if( (string) verificaValidacaoCritica( $_SESSION['emiid'] ) == 'observacao' )
							$retorno = true;
					}
				}
				
				//$retorno; verificar retorno
				//fim
				
				$sql = "SELECT
							mdoid, 
							undddsc, 
							itfdsc, 
							mdoespecificacao, 														 
							mdoqtd, 
							mdovalorunitario, 
							mdototal,
							mdoflagalterado
						FROM 
							em.emimatrizdistribuicaoorcamentargap em
						INNER JOIN
							cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
						INNER JOIN
							em.emiitemfinanciavel ei ON ei.itfid = em.itfid 
						WHERE 
							papid = {$papid} AND mdostatus = 'A'
						ORDER BY
							itfdsc";		
				
				$dados = $db->carregar( $sql );
				
				if( $dados ){
					
					for( $i = 0; $i < count( $dados ); $i++ ){
						
						$img = ($ativo == 'S')
											  ? "<img src='/imagens/alterar.gif' style='cursor: pointer;' title='Editar' onclick='alterarItemMatrizGap({$dados[$i]["mdoid"]}, \"{$acao}\");'>
												 <img src='/imagens/excluir.gif' style='cursor: pointer;' title='Excluir' onclick='excluirItemFinanciavel({$dados[$i]["mdoid"]}, \"{$acao}\");'>
												 ".(($mostraCritica) ? "<img src='/imagens/editar_nome.gif' style='cursor: pointer;' title='Crítica' onclick='preencheCriticaMatrizGap({$dados[$i]["mdoid"]});'>" : "")
											  : "<img src='/imagens/alterar_01.gif'/>
											  	 <img src='/imagens/excluir_01.gif'/>";
						
						$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
						
						// Recupera se a crítica realizada validou ou não o item da matriz.
						$crmvalidado 	= $db->pegaUm("SELECT crmvalidado FROM em.emicritricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
						$crmobs 		= $db->pegaUm("SELECT crmobs FROM em.emicritricamatriz WHERE mdoid = ".$dados[$i]["mdoid"]);
						
						
						// calcula o total geral do campo 'Total (R$) A x B'
						$totalGeralAxB += $dados[$i]["mdototal"];
						
						// Mostra o texto em vermelho se não tiver sido validado.
						if($dados[$i]['mdoflagalterado'] == "t"){
							$item 			= "<font color='blue'>".$dados[$i]["itfdsc"]."</font>" 			;
							$especificacao 	= "<font color='blue'>".$dados[$i]["mdoespecificacao"]."</font>" ;
							$unidade 		= "<font color='blue'>".$dados[$i]["undddsc"]."</font>" 			;
							$quantidade 	= "<font color='blue'>".$dados[$i]["mdoqtd"]."</font>" 			;
							$valorunitario 	= "<font color='blue'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" ;
							$total 			= "<font color='blue'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		;				
						} else {
							$item 			= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["itfdsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["itfdsc"]."</font>" : $dados[$i]["itfdsc"]);
							$especificacao 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoespecificacao"]."</font>" : (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoespecificacao"]."</font>" : $dados[$i]["mdoespecificacao"]);
							$unidade 		= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["undddsc"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["undddsc"]."</font>" : $dados[$i]["undddsc"]);
							$quantidade 	= ($crmvalidado == "f") ? "<font color='red'>".$dados[$i]["mdoqtd"]."</font>" 			: (($crmobs == "t") ? "<font color='orange'>".$dados[$i]["mdoqtd"]."</font>" : $dados[$i]["mdoqtd"]);
							
							$valorunitario 	= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdovalorunitario"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdovalorunitario"], 2, ",", "."));
							$total 			= ($crmvalidado == "f") ? "<font color='red'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" 		: (($crmobs == "t") ? "<font color='orange'>".number_format($dados[$i]["mdototal"], 2, ",", ".")."</font>" : number_format($dados[$i]["mdototal"], 2, ",", "."));
						}
						
						print "<tr bgColor='{$cor}'>
								   <td align='center' width='10%'>{$item}</td>
								   <td align='justify' width='30%'>".wordwrap($especificacao, 40, "<br />\n", true)."</td>
								   <td align='center' width='10%'>{$unidade}</td>
								   <td align='right' width='10%'>{$quantidade}</td>
								   <td align='right' width='15%'>" . $valorunitario . "</td>
								   <td align='right' width='15%'>" . $total . "</td>
								   <td align='center' width='10%'>
								       {$img}
								   </td>
							   </tr>";
					}
				}
				?>
				<tr bgcolor="#C0C0C0">
					<?php
					$sql = "SELECT 
								coalesce(sum(emcquantidadealunos),0) as total
							FROM 
								em.emicenso 
							WHERE 
								entid = '".$_SESSION['entid']."'";
					$totalAluno = $db->pegaUm($sql);
					
					$sql = "SELECT 
								coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
							FROM 
								em.emimatrizdistribuicaoorcamentargap m
							INNER JOIN 
								em.emigap p ON p.papid = m.papid 
							WHERE 
								emiid = {$_SESSION['emiid']}  
							AND 
								itfid in (8,5,3,2,7,1,4,6,13)";
					$totalCusteio = $db->pegaUm($sql);
					//echo $totalCusteio;
					
					$sql = "SELECT 
								coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
							FROM 
								em.emimatrizdistribuicaoorcamentargap m
							INNER JOIN 
								em.emigap p ON p.papid = m.papid
							WHERE 
								emiid = {$_SESSION['emiid']} 
							AND 
								itfid in (9,11,10,12)";
					$totalCapital = $db->pegaUm($sql);
					
					$sql = "SELECT 
								etjjornada, entcodent 
							FROM 
								em.emiensinomedioinovador ei
							INNER JOIN 
								em.emitipojornada tj ON tj.etjid = ei.etjid
					 		WHERE 
					 			ei.emiid = ".$_SESSION['emiid'];
					$dadosEMI = $db->pegaLinha($sql);
					$entcodent = $dadosEMI['entcodent'];
					$jornada = trim($dadosEMI['etjjornada']);
					$totalAluno = trim($totalAluno);
					
					if(empty($jornada) || empty($totalAluno)){
						echo "<script>
								alert('Problemas com variáveis. Refaça o procedimento.');
								window.close();
							  </script>";	
						exit;
					}					
					
					$sql = "SELECT * FROM em.emireccusteiocapital WHERE emrjornada = {$jornada} AND {$totalAluno} BETWEEN emrqtdinialunos AND emrqtdfinalunos;";
					$reccusteiocapital = $db->pegaLinha($sql);
	
					/*$saldoCusteio = $reccusteiocapital['emrcusteio']-$totalCusteio;				
					$saldoCapital = $reccusteiocapital['emrcapital']-$totalCapital;*/
					$saldoCusteio = $totalCusteio;
					$saldoCapital = $totalCapital;
					
					$sql = "
						SELECT emvlrcusteio,
						emvlrcapital,
						emvlrpago
						FROM em.emipagtofnde
						WHERE entcodent = '".$entcodent."'
					";
					$dadosMEC = $db->pegaLinha($sql);
					
					$capitalDisponivel = $dadosMEC['emvlrcapital'] - $saldoCapital;
					$custeioDisponivel = $dadosMEC['emvlrcusteio'] - $saldoCusteio;
					//$totalDisponivel   = $dadosMEC['emvlrpago']    - $totalGeralAxB;
					$totalDisponivel   = $dadosMEC['emvlrpago']    - ($saldoCapital + $saldoCusteio);
					
					?>					
					<td  colspan="8" align="center">
						<input type="hidden" name="emrcusteio" id="emrcusteio" value="<?php echo $reccusteiocapital['emrcusteio'] ?>" />
						<input type="hidden" name="emrcapital" id="emrcapital" value="<?php echo $reccusteiocapital['emrcapital'] ?>" />				
						<input type="hidden" name="saldo_custeio" id="saldo_custeio" value="<?php echo $capitalDisponivel ?>" />
						<input type="hidden" name="saldo_capital" id="saldo_capital" value="<?php echo $custeioDisponivel ?>" />											
						<table width="100%">
							<tr>
								<td align="center" style="border-right: 1px solid white;">
									<table>
										<tr>
											<td align="center" colspan="6"><strong>Recursos Financeiros transferidos FNDE</strong></td>
										</tr>
										<tr>
											<td width="" align="right"><b>Capital:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <?php echo number_format($dadosMEC['emvlrcapital'], 2, ",", "."); ?></b>&nbsp;
											</td>
											<td width="" align="right"><b>Custeio:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <?php echo number_format($dadosMEC['emvlrcusteio'], 2, ",", ".")?></b>&nbsp;
											</td>
											<td width="" align="right"><b>Total:</b></td>
											<td style="border-left: 0px solid white;">
												<b>R$ <?php echo number_format($dadosMEC['emvlrpago'], 2, ",", ".")?></b>
											</td>
										</tr>
									</table>
								</td>
								<td align="center" style="border-right: 1px solid white;">
									<table>
										<tr>
											<td align="center" colspan="6"><strong>Recursos Financeiros utilizados PRC</strong></td>
										</tr>
										<tr>
											<td width="" align="right"><b>Capital:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <?php echo number_format($saldoCapital, 2, ",", ".")?></b>&nbsp;
											</td>
											<td width="" align="right"><b>Custeio:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <?php echo number_format($saldoCusteio, 2, ",", ".")?></b>&nbsp;
											</td>
											<td width="" align="right"><b>Total:</b></td>
											<td style="border-left: 0px solid white;">
												<b>R$ 
												<?php 
													//echo number_format($totalGeralAxB, 2, ",", ".");
													echo number_format($saldoCapital + $saldoCusteio, 2, ",", ".")
												?></b>
											</td>
										</tr>
									</table>
								</td>
								<td align="center">
									<table>
										<tr>
											<td align="center" colspan="6"><strong>Saldo disponível</strong></td>
										</tr>
										<tr>
											<td width="" align="right"><b>Capital:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <span id="td_saldo_capital"><?php echo number_format($capitalDisponivel, 2, ",", ".")?></span></b>&nbsp;
											</td>
											<td width="" align="right"><b>Custeio:</b>&nbsp;</td>
											<td style="border-left: 0px solid white;">
												&nbsp;<b>R$ <span id="td_saldo_custeio"><?php echo number_format($custeioDisponivel, 2, ",", ".")?></span></b>&nbsp;
											</td>
											<td width="" align="right"><b>Total:</b></td>
											<td style="border-left: 0px solid white;">
												<b>R$ <?php echo number_format($totalDisponivel, 2, ",", ".")?></b>
											</td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td colspan="7">
						<input type="button" value="Voltar" onclick="document.location.href='em.php?modulo=principal/popupAcaoGap&acao=A'" style="cursor: pointer;"/>
						<input type="button" value="Listar Itens" onclick="listarItens()" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>