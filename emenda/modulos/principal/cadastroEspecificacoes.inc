<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if( isset( $_POST['submetido'] ) ) {
	$espkit = ($_REQUEST["espkit"]) ? 't' : 'f';

	if($_REQUEST["espid"] && $_REQUEST["espid"] != "") {
		// UPDATE
		$sql = "UPDATE emenda.especificacao SET  espnome = '".pg_escape_string($_REQUEST["espnome"])."',
														espunidademedida = '".pg_escape_string($_REQUEST["espunidademedida"])."',
														espkit = '".$espkit."',
														ssiid = ".(($_REQUEST["ssiid"])?"'".$_REQUEST["ssiid"]."'":"NULL").",
														espvalorunitariopadrao = ".($_REQUEST["espvalorunitariopadrao"] ? retiraPontos($_REQUEST["espvalorunitariopadrao"]) : 'null').",
														edpcod = '".$_REQUEST["edpcod"]."'
												   WHERE
												   		espid = ".$_REQUEST["espid"];
		$db->executar($sql);
		
		if( is_array($_REQUEST["prsano"]) && $_REQUEST["prsano"][0] ){
			$sql = "DELETE FROM emenda.especificacao_programacaoexercicio WHERE espid = ".$_REQUEST["espid"];
			$db->executar($sql);
			foreach ($_REQUEST["prsano"] as $prsano) {
				$sql = "INSERT INTO emenda.especificacao_programacaoexercicio(espid, prsano) 
						VALUES (".$_REQUEST["espid"].", '".$prsano."')";
				$db->executar($sql);	
			}
		}
		
		// recupera os responsável associados a iniciativa no banco
		$especificacaoResponsavel = $db->carregarColuna("SELECT resid FROM emenda.especificacaoresponsavel WHERE espid = ".$_REQUEST["espid"]." and erestatus = 'A'");
		// insere, se houver, os novos
		for($i=0; $i<count($_REQUEST["resid"]); $i++) { 			
			if( !in_array($_REQUEST["resid"][$i], $especificacaoResponsavel) ){
				$db->executar("INSERT INTO emenda.especificacaoresponsavel(espid, resid, erestatus) VALUES (".$_REQUEST["espid"].", ".$_REQUEST["resid"][$i].", 'A')");
			}			
		}		
		// exclui, se houver necessidade
		if ( $_REQUEST["resid"] ){   
			for($i=0; $i<count($especificacaoResponsavel); $i++) {
				if( ! in_array($especificacaoResponsavel[$i], $_REQUEST["resid"]) )
					$db->executar("UPDATE emenda.especificacaoresponsavel SET erestatus = 'I' WHERE espid = ".$_REQUEST["espid"]." AND resid = ".$especificacaoResponsavel[$i]." and erestatus = 'A'");
			}
		}
		$espid = $_REQUEST["espid"];
	}
	else {
		// INSERT
		$espid = $db->pegaUm("INSERT INTO emenda.especificacao(espnome,
													    --resid,
														espunidademedida,
														espkit,
														espvalorunitariopadrao,
														edpcod,
														espstatus,
														ssiid) 
					   							VALUES( '".pg_escape_string($_REQUEST["espnome"])."',
					   									'".pg_escape_string($_REQUEST["espunidademedida"])."',
					   									'".$espkit."',
					   									".($_REQUEST["espvalorunitariopadrao"] ? retiraPontos($_REQUEST["espvalorunitariopadrao"]) : 'null') .",
					   									'".$_REQUEST["edpcod"]."',
					   									'A',
					   									".(($_REQUEST['ssiid'])?"'".$_REQUEST['ssiid']."'":"NULL").") RETURNING espid");
		if( is_array($_REQUEST["prsano"]) && $_REQUEST["prsano"][0] ){
			$sql = "DELETE FROM emenda.especificacao_programacaoexercicio WHERE espid = $espid";
			$db->executar($sql);
			foreach ($_REQUEST["prsano"] as $prsano) {
				$sql = "INSERT INTO emenda.especificacao_programacaoexercicio(espid, prsano) 
						VALUES ($espid, '".$prsano."')";	
				$db->executar($sql);	
			}
		}
		
		// insere responsável
		for($i=0; $i<count($_REQUEST["resid"]); $i++) {
			$db->executar("INSERT INTO emenda.especificacaoresponsavel(espid, resid, erestatus) VALUES(".$espid.",".$_REQUEST["resid"][$i].",'A')");
		}
	}
	if( is_array($_REQUEST["ipaid"]) && $_REQUEST["ipaid"][0] ){
		$sql = "select ipaid as codigo, tipo, descricao from (
					SELECT DISTINCT g.ipaid, 'P' as tipo, p.picdescricao as descricao FROM emenda.itempar_especificacao ipe
						inner join emenda.ptitemkit ptk on ptk.iteid = ipe.iteid
					    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
					    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid --and ie.icestatus = 'A'
                        inner join emenda.itempar g on ipe.ipaid = g.ipaid
						inner join par.propostaitemcomposicao p ON g.picid = p.picid
					WHERE 
						ipe.espid = $espid
					    and ie.espid = $espid
					    and ipe.ipaid is not null
					union
					SELECT DISTINCT ipaid, 'O' as tipo, po.predescricao as descricao FROM emenda.itempar_especificacao ipe
						inner join emenda.ptitemkit ptk on ptk.itoid = ipe.itoid
					    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
					    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid --and ie.icestatus = 'A'
                        inner join emenda.itemobras o on ipe.itoid = o.itoid
                        inner join obras.preobra po on po.ptoid = o.ptoid
					WHERE 
						ipe.espid = $espid
					    and ie.espid = $espid
					    and ipe.ipaid is not null
				) as foo";
		$arrItem = $db->carregarColuna( $sql );
		$arrItem = $arrItem ? $arrItem : array();
		
		$sql = "DELETE FROM emenda.itempar_especificacao WHERE espid = $espid ".($arrItem ? " and (ipaid not in (".implode(',', $arrItem).") or itoid not in (".implode(',', $arrItem).") )" : '' );
		$db->executar($sql);
		$arrItemPost = array();
		foreach ($_REQUEST["ipaid"] as $ipaid) {
			$arIpad = explode('_', $ipaid);
			array_push($arrItemPost, $arIpad[1]);
			if( $arIpad[0] == 'P' ){
				$item = $arIpad[1];
				$obras = 'null';

				if( !in_array( $item, $arrItem ) ){
					$sql = "INSERT INTO emenda.itempar_especificacao(ipaid, itoid, espid) 
							VALUES ($item, $obras, $espid)";
					$db->executar($sql);
				}
			} else {
				$item = 'null';
				$obras = $arIpad[1];
				
				if( !in_array( $obras, $arrItem ) ){
					$sql = "INSERT INTO emenda.itempar_especificacao(ipaid, itoid, espid) 
							VALUES ($item, $obras, $espid)";
					$db->executar($sql);
				}
			}
		}
		#Item que não serão desvinculado.
		$sql = "select descricao from (
					SELECT DISTINCT g.ipaid, 'P' as tipo, p.picdescricao as descricao FROM emenda.itempar_especificacao ipe
						inner join emenda.ptitemkit ptk on ptk.iteid = ipe.iteid
					    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
					    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid --and ie.icestatus = 'A'
                        inner join emenda.itempar g on ipe.ipaid = g.ipaid
						inner join par.propostaitemcomposicao p ON g.picid = p.picid
					WHERE 
						ipe.espid = $espid
					    and ie.espid = $espid
                        and ipe.iteid in (select iteid from emenda.itempar_especificacao 
											where espid = $espid and (ipaid not in (".implode(',', $arrItemPost).") or itoid not in (".implode(',', $arrItemPost).")))
					    and ipe.ipaid is not null
					union
					SELECT DISTINCT ipaid, 'O' as tipo, po.predescricao as descricao FROM emenda.itempar_especificacao ipe
						inner join emenda.ptitemkit ptk on ptk.itoid = ipe.itoid
					    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
					    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid --and ie.icestatus = 'A'
                        inner join emenda.itemobras o on ipe.itoid = o.itoid
                        inner join obras.preobra po on po.ptoid = o.ptoid
					WHERE 
						ipe.espid = $espid
					    and ie.espid = $espid
                        and ipe.iteid in (select iteid from emenda.itempar_especificacao 
											where espid = $espid and (ipaid not in (".implode(',', $arrItemPost).") or itoid not in (".implode(',', $arrItemPost).")))
					    and ipe.ipaid is not null
				) as foo";
		$arrItemNot = $db->carregarColuna( $sql );
		$arrItemNot = $arrItemNot ? $arrItemNot : array();
		if( $arrItemNot ){
			$msg = 'Algum(ns) item(ns) não foi(ram) desvinculado(s) da especificação por estar(em) vinculado(s) ao plano de trabalho.\nSegue(m) abaixo o(s) item(ns):\n*'.implode('\n*',$arrItemNot);		
			$html = "<html><head><script>alert('".$msg."');";
			$html.='</script></head><body></body></html>';
			echo $html;
		}
	}		
	$db->commit();
	if( !isset($_REQUEST['popup']) ){
		$db->sucesso('principal/cadastroEspecificacoes', '&espid='.(!empty($_REQUEST["espid"]) ? $_REQUEST["espid"] : $espid));
	} else {
		echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload(); 
				window.close();
			</script>";
	}
	exit();
}

if( !isset($_REQUEST['popup']) ){
	include  APPRAIZ."includes/cabecalho.inc";
	print "<br/>";
	$db->cria_aba( $abacod_tela, $url, '' );
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
monta_titulo( "Especificação (Metas)", "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

if($_REQUEST["espid"]) {
	$dados 					= $db->carregar("SELECT 
												  esp.espid,
												  esp.x_resid,
												  esp.espnome,
												  esp.espunidademedida,
												  esp.espkit,
												  esp.espvalorunitariopadrao,
												  esp.espstatus,
												  esp.edpcod,
												  esp.esppar,
												  epe.prsano,
												  esp.ssiid
												FROM 
												  emenda.especificacao esp
												  left join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid
												WHERE
													esp.espid = ".$_REQUEST["espid"]);
	
	$espnome 				= $dados[0]["espnome"];
	$resid	 				= $dados[0]["resid"];
	$espunidademedida 		= $dados[0]["espunidademedida"];
	$edpcod 				= $dados[0]["edpcod"];
	$espkit					= $dados[0]["espkit"];
	$prsano					= $dados[0]["prsano"];
	$ssiid					= $dados[0]["ssiid"];
	if($espkit == 't'){
		$readonly = 'disabled="disabled"';
	} else {
		$espvalorunitariopadrao	= number_format($dados[0]["espvalorunitariopadrao"], 2, ',', '.');
	}
}

?>

<form id="formCadastroEspecificacoes" name="formCadastroEspecificacoes" action="" method="post" enctype="multipart/form-data">
<input type="hidden" value="1" id="submetido" name="submetido" />
<input type="hidden" value="<?=$espkit; ?>" id="boespkit" name="boespkit" />
<input type="hidden" value="<?= ($_REQUEST["espid"]) ? $_REQUEST["espid"] : "" ?>" id="espid" name="espid" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;"><b>Especificação:</b></td>
		<td>
			<?=campo_texto( 'espnome', 'S', 'S', 'Especificação', 40, 250, '', '','','','','id="espnome"');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Subitem:</b></td>
		<td>
			<?
				$sql = "SELECT ssiid as codigo,ssicodigosubitem || ' - ' || ssidescricaosubitem as descricao FROM emenda.siconvsubitens ORDER BY ssidescricaosubitem";
				$db->monta_combo("ssiid", $sql, 'S', "Selecione...", '', '', '', '265', 'S', 'ssiid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Unidade de Medida:</b></td>
		<td>
			<?=campo_texto( 'espunidademedida', 'S', 'S', 'Unidade de Medida', 40, 100, '', '','','','','id="espunidademedida"');?>
			<br /><br />
			<input type="checkbox" value="S" onclick="checaKit(this);" name="espkit" id="espkit" <?= ($espkit=="t") ? "checked=\"checked\"" : "" ?>><b> Kit ou Módulo</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Valor Unitário Padrão:</b></td>
		<td>
			<?=campo_texto( 'espvalorunitariopadrao', 'N', 'S', 'Valor Unitário Padrão', 40, 10, '[###.]###,##', '','','','','id="espvalorunitariopadrao"; '.$readonly,'','',"this.value=mascaraglobal('[###.]###,##',this.value);");?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Elemento da despesa:</b></td>
		<td>
			<?
				$sql = "SELECT edpcod as codigo,edpcod || ' - ' || edpdsc as descricao FROM public.elementodespesa WHERE edpstatus = 'A' ORDER BY edpcod";
				$db->monta_combo("edpcod", $sql, 'S', "Selecione...", '', '', '', '265', 'S', 'edpcod');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Ano:</b></td>
		<td>
			<?
				$prsano = array();
				if($_REQUEST["espid"]) {
					$sql = "SELECT 
							  epe.prsano as codigo,
							  epe.prsano as descricao
							FROM 
							  emenda.especificacao esp
							  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid
							WHERE
								esp.espid = ".$_REQUEST["espid"];
					$prsano = $db->carregar( $sql );
					$prsano = $prsano ? $prsano : array();
				}
				$sql = "SELECT prsano as codigo, prsano as descricao FROM emenda.programacaoexercicio where prsstatus = 'A' order by prsano";
				//$db->monta_combo("prsano", $sql, 'S', "Selecione...", '', '', '', '265', 'S', 'prsano');
				combo_popup( 'prsano', $sql, '', '400x400', 0, array(), '', 'S', false, false, 05, 300 );
			?>
		</td>
	</tr>
	<tr id="tr_itens" style="display: none">
		<td class="SubTituloDireita"><b>Itens:</b></td>
		<td><?
				$ipaid = array();
				if($_REQUEST["espid"]) {
					$sql = "select tipo||'_'||codigo as codigo, descricao, tipo from (
								SELECT 
							        gip.ipaid as codigo,
							        pic.picdescricao as descricao,
							        'P' as tipo
							    FROM emenda.itempar gip
							        inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
							        inner join emenda.itempar_especificacao gie on gie.ipaid = gip.ipaid
							    WHERE
							        pic.picstatus = 'A'
							        and gie.espid = {$_REQUEST["espid"]}						
							UNION
								SELECT
							    	io.itoid as codigo,
							        pto.ptodescricao as descricao,
							        'O' as tipo
							    FROM
							    	emenda.itemobras io
							        inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
							        inner join emenda.itempar_especificacao gie on gie.itoid = io.itoid
							    WHERE
							    	pto.ptostatus = 'A'
							        and gie.espid = {$_REQUEST["espid"]}	) 
							as foo
							order by
								descricao
							 ";
					$ipaid = $db->carregar( $sql );
					$ipaid = $ipaid ? $ipaid : array();
					
					$sqlFixo = "select tipo||'_'||ipaid as codigo from (
									SELECT DISTINCT ipaid, 'P' as tipo FROM emenda.itempar_especificacao ipe
										inner join emenda.ptitemkit ptk on ptk.iteid = ipe.iteid
									    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
									    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid and ie.icestatus = 'A'
									WHERE 
										ipe.espid = {$_REQUEST["espid"]}
									    and ie.espid = {$_REQUEST["espid"]}
									    and ipe.ipaid is not null
									union
									SELECT DISTINCT ipaid, 'O' as tipo FROM emenda.itempar_especificacao ipe
										inner join emenda.ptitemkit ptk on ptk.itoid = ipe.itoid
									    inner join emenda.ptiniciativaespecificacao pte on pte.pteid = ptk.pteid
									    inner join emenda.iniciativaespecificacao ie on ie.iceid = pte.iceid and ie.icestatus = 'A'
									WHERE 
										ipe.espid = {$_REQUEST["espid"]}
									    and ie.espid = {$_REQUEST["espid"]}
									    and ipe.ipaid is not null
								) as foo";
					$codigos_fixos = $db->carregarColuna( $sqlFixo );					
					$mensagem_fixo = 'Este item ja está vinculado a especificação no PTA';
				}
				$sql = "select tipo||'_'||codigo as codigo, descricao, tipo from (
							SELECT 
									gip.ipaid as codigo,
								    pic.picdescricao as descricao,
		                            'P' as tipo
								FROM emenda.itempar gip
									inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
								WHERE
									pic.picstatus = 'A'						
							UNION
								SELECT
							    	io.itoid as codigo,
							        pto.ptodescricao as descricao,
							        'O' as tipo
							    FROM
							    	emenda.itemobras io
							        inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
							    WHERE
							    	pto.ptostatus = 'A') as foo
							order by
								descricao";			
				
				combo_popup('ipaid', $sql, '', '400x700', 0, $codigos_fixos, $mensagem_fixo, 'S', false, false, 10, 400 );
				echo obrigatorio();
			?></td>
	</tr>
	<!-- <tr>
		<td class="SubTituloDireita" valign="top">Responsável:</td>
		<td>
			<a href="#" onclick="inserirResponsavel();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Vincular Responsável à Especificação</a>
			<br />
			<table id="tb_responsavel" width="50%" border="0" cellspacing="0" cellpadding="2" class="listagem">
               <thead>
               <tr>
               		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Excluir</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Responsável</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Assunto</strong></td>
               </tr>
               </thead>
               <tbody>
               <?
			   	if($_REQUEST["espid"]) {
					$sql = "  SELECT 
								enr.ereid,
								res.resid,
								res.resdsc,
							    res.resassunto
							FROM 
							  emenda.responsavel res
							  inner join emenda.especificacaoresponsavel enr
							  	on res.resid = enr.resid
							    and enr.erestatus = 'A'
							WHERE
								enr.espid = ".$_REQUEST['espid']."
							ORDER BY
								res.resdsc ";
					
					$dados = $db->carregar($sql);
					
					if($dados) {
				   		for($i=0; $i<count($dados); $i++) {
				   			$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				   			
							echo "<tr bgcolor=\"".$cor."\" id=\"tr_responsavel_".$dados[$i]["resid"]."\">
		                               <td align=\"center\">
		                               		<input type=\"hidden\" name=\"resid[]\" value=\"".$dados[$i]["resid"]."\" />
		                               		<input type=\"hidden\" name=\"ereid[]\" value=\"".$dados[$i]["ereid"]."\" />
		                               		<a href=\"#\" onclick=\"excluirResponsavel(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>
		                               </td>
		                               <td align=\"center\">".$dados[$i]["resdsc"]."</td>
		                               <td align=\"center\">".$dados[$i]["resassunto"]."</td>
	                              </tr>";
						}
					}
				}
			   ?>
			   </tbody>
       		</table>
       		<br />
		</td>
	</tr>-->
	<?
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
		$retorno = '';
	} else {
		$retorno = 'disabled="disabled"';
	}
	?>
	<tr bgcolor="#c0c0c0">
		<td>&nbsp;</td>
		<td>
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarEspecif();" <?=$retorno;?> />
			<input type="button" value="Cancelar" name="btnVoltar" id="btnVoltar" onclick="voltarEspecif();">
		</td>
	</tr>
</table>
</form>

<script type="text/javascript">
form 		= document.getElementById("formCadastroEspecificacoes");
btnSalvar 	= document.getElementById("btnSalvar");
btnVoltar 	= document.getElementById("btnVoltar");
verificaKit();

function verificaKit(){
	if( document.getElementById('boespkit').value == 't' ){
		document.getElementById('tr_itens').style.display = '';
	} else {
		document.getElementById('tr_itens').style.display = 'none';
	}
}

function checaKit(valor){
	if(valor.checked){
		document.getElementById("espvalorunitariopadrao").disabled = true;
		document.getElementById("espvalorunitariopadrao").value = "";
		document.getElementById("boespkit").value = "t";
	} else {
		document.getElementById("espvalorunitariopadrao").disabled = false;
		document.getElementById("boespkit").value = "f";
	}
	verificaKit();
}

function voltarEspecif() {
	btnSalvar.disabled = true;
	btnVoltar.disabled = true;

	window.location.href = "emenda.php?modulo=principal/listaEspecificacoes&acao=A";
}

function salvarEspecif() {
	var espnome 				= document.getElementById("espnome");
	var espunidademedida 		= document.getElementById("espunidademedida");
	var espvalorunitariopadrao 	= document.getElementById("espvalorunitariopadrao");
	//var resid					= document.getElementsByName("resid[]");
	var edpcod					= document.getElementById("edpcod");
	//var prsano					= document.getElementById("prsano");
	
	selectAllOptions( document.getElementById( 'prsano' ) );
	selectAllOptions( document.getElementById( 'ipaid' ) );
	
	btnSalvar.disabled = true;
	btnVoltar.disabled = true;

	if(espnome.value == "") {
		alert("O campo 'Especificação' deve ser informado.");
		btnSalvar.disabled = false;
		btnVoltar.disabled = false;
		espnome.focus();
		return;
	}
	if(espunidademedida.value == "") {
		alert("O campo 'Unidade de Medida' deve ser informado.");
		btnSalvar.disabled = false;
		btnVoltar.disabled = false;
		espunidademedida.focus();
		return;
	}
	/*if(espvalorunitariopadrao.value != "") {
		if(parseInt(espvalorunitariopadrao.value) <= "0") {
			alert("O campo 'Valor Unitário Padrão' não pode se menor ou igual a 0!");
			btnSalvar.disabled = false;
			btnVoltar.disabled = false;
			espvalorunitariopadrao.focus();
			return false;
		}
	}*/
	if(edpcod.value == "") {
		alert("O campo 'Elemento da despesa' deve ser informado.");
		btnSalvar.disabled = false;
		btnVoltar.disabled = false;
		edpcod.focus();
		return false;
	}
	var ano = prsano.options.length;
	
	if(ano == 0) {
		alert("O campo 'Ano' deve ser informado.");
		btnSalvar.disabled = false;
		btnVoltar.disabled = false;
		prsano.focus();
		return false;
	}
	
	/*if( document.getElementById('boespkit').value == 't' ){
		var valor = document.getElementById( 'ipaid' ).options[0].value;
		if( valor == '' ){
			alert('O campo "Itens" deve ser informado.');
			$('#ipaid').focus();
			return false;
		}
	}*/
	/*if(resid.length == 0) {
		alert("Deve ser inserido pelo menos um 'Responsável'.");
		btnSalvar.disabled 	= false;
		btnVoltar.disabled = false;
		return;
	}*/
	
	form.submit();
}

function inserirResponsavel() {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereResponsavel&acao=A&tipo=especificacao', 'responsavel', 'width=450,height=400,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
	janela.focus();
}
function excluirResponsavel(linha) {
	var tabela = document.getElementById("tb_responsavel");
	tabela.deleteRow(linha);
}
	
</script>