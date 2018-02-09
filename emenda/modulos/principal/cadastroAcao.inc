<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_SESSION["iniid"]) {
	$_REQUEST["iniid"] = $_SESSION["iniid"];
}

if($_REQUEST['carregaTipoEnsino']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT DISTINCT tpeid,tpedsc FROM emenda.tipoensino WHERE tpestatus = 'A' AND resid in ".$_REQUEST['resid']." ORDER BY tpedsc";
	$dados = $db->carregar($sql);
	
	$tpeid = explode( ',', $_REQUEST['tpeid'] );
	
	$retorno = "";
	if($dados) {
		for($i=0; $i<count($dados); $i++) {
			$checked = ( in_array( $dados[$i]["tpeid"], $tpeid ) ) ? 'checked="checked"' : '';
			$retorno .= '<input type="checkbox" name="tpeid[]" value="'.$dados[$i]["tpeid"].'" '.$checked.' /> '.$dados[$i]["tpedsc"].'<br />';
		}
	}
	echo $retorno;
	exit;
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if($_REQUEST["submetido"]) {
	
	if($_REQUEST["iniid"] && $_REQUEST["iniid"] != "") {
		// UPDATE
		// atualiza a iniciativa
		$db->executar("UPDATE 
							emenda.iniciativa 
					   SET 
					   		ininome 	= 	'".pg_escape_string($_REQUEST["ininome"])."',
					   		temid		=	".$_REQUEST["temid"].",
					   		gndcod		=	".$_REQUEST["gndcod"].",
					   		iniescolasbeneficiasdas = '".$_REQUEST["iniescolasbeneficiasdas"]."',
					   		iniresolucao = '".$_REQUEST["iniresolucao"]."'
					   WHERE 
					   		iniid 	= ".$_REQUEST["iniid"]);
		
		// recupera os beneficiários associados a iniciativa no banco
		$iniciativabeneficiarios = $db->carregarColuna("SELECT benid FROM emenda.iniciativabeneficiario WHERE iniid = ".$_REQUEST["iniid"]);
		
		// insere, se houver, os novos
		for($i=0; $i<count($_REQUEST["benid"]); $i++) {
			
			$_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]] = !empty($_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]]) ? $_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]] : 'f'; 
			
			if( !in_array($_REQUEST["benid"][$i], $iniciativabeneficiarios) ){
				$db->executar("INSERT INTO emenda.iniciativabeneficiario(iniid,benid,icbobrigatorio, icbstatus) VALUES(".$_REQUEST["iniid"].",".$_REQUEST["benid"][$i].", '{$_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]]}', 'A')");
			}else{
				
				$statusBen = $db->pegaUm( "SELECT icbstatus FROM emenda.iniciativabeneficiario WHERE iniid = ".$_REQUEST["iniid"]." AND benid = {$_REQUEST["benid"][$i]}" );
				
				if ( $statusBen == 'I' ){
					$db->executar("UPDATE emenda.iniciativabeneficiario SET icbobrigatorio = '{$_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]]}', icbstatus = 'A' WHERE iniid = ".$_REQUEST["iniid"]." AND benid = ".$_REQUEST["benid"][$i]."");		
				}else{
					$db->executar("UPDATE emenda.iniciativabeneficiario SET icbobrigatorio = '{$_REQUEST["icbobrigatorio"][$_REQUEST["benid"][$i]]}' WHERE iniid = ".$_REQUEST["iniid"]." AND benid = ".$_REQUEST["benid"][$i]."");
				}
				
			}
			
		}
		
		// exclui, se houver necessidade
		if ( $_REQUEST["benid"] ){   
			for($i=0; $i<count($iniciativabeneficiarios); $i++) {
				if( ! in_array($iniciativabeneficiarios[$i], $_REQUEST["benid"]) )
					$db->executar("UPDATE emenda.iniciativabeneficiario SET icbstatus = 'I' WHERE iniid = ".$_REQUEST["iniid"]." AND benid = ".$iniciativabeneficiarios[$i]);
			}
		}	
		
		// recupera os responsável associados a iniciativa no banco
		$iniciativaResponsavel = $db->carregarColuna("SELECT ireid FROM emenda.iniciativaresponsavel WHERE iniid = ".$_REQUEST["iniid"]);
		// insere, se houver, os novos
		for($i=0; $i<count($_REQUEST["resid"]); $i++) { 			
			if( !in_array($_REQUEST["resid"][$i], $iniciativaResponsavel) ){
				$db->executar("INSERT INTO emenda.iniciativaresponsavel(iniid, resid, irestatus) VALUES (".$_REQUEST["iniid"].", ".$_REQUEST["resid"][$i].", 'A')");
			}			
		}		
		// exclui, se houver necessidade
		if ( $_REQUEST["resid"] ){   
			for($i=0; $i<count($iniciativaResponsavel); $i++) {
				if( ! in_array($iniciativaResponsavel[$i], $_REQUEST["resid"]) )
					$db->executar("UPDATE emenda.iniciativaresponsavel SET irestatus = 'I' WHERE iniid = ".$_REQUEST["iniid"]." AND ireid = ".$iniciativaResponsavel[$i]);
			}
		}
		
		// recupera as especificações associadas a iniciativa no banco
		$iniciativaespecificacoes = $db->carregarColuna("SELECT espid FROM emenda.iniciativaespecificacao WHERE iniid = ".$_REQUEST["iniid"]." AND icestatus = 'A'");
		// insere, se houver, as novas
		for($i=0; $i<count($_REQUEST["espid"]); $i++) {
			if( ! in_array($_REQUEST["espid"][$i], $iniciativaespecificacoes) )
				$db->executar("INSERT INTO emenda.iniciativaespecificacao(iniid,espid,icestatus) VALUES(".$_REQUEST["iniid"].",".$_REQUEST["espid"][$i].",'A')");
		}
		// exclui, se houver necessidade   
		for($i=0; $i<count($iniciativaespecificacoes); $i++) {
			if( ! in_array($iniciativaespecificacoes[$i], $_REQUEST["espid"]) )
				$db->executar("UPDATE emenda.iniciativaespecificacao SET icestatus = 'I' WHERE iniid = ".$_REQUEST["iniid"]." AND espid = ".$iniciativaespecificacoes[$i]);
		}
		
		// recupera os tipos de ensino associados a iniciativa no banco com status A
		$iniciativatipoensino = $db->carregarColuna("SELECT tpeid FROM emenda.iniciativatipoensino WHERE iniid = ".$_REQUEST["iniid"]." AND itestatus = 'A'");
		// recupera os tipos de ensino associados a iniciativa no banco com status I
		$iniciativatipoensinoI = $db->carregarColuna("SELECT tpeid FROM emenda.iniciativatipoensino WHERE iniid = ".$_REQUEST["iniid"]." AND itestatus = 'I'");
		// insere, se houver, os novos
		for($i=0; $i<count($_REQUEST["tpeid"]); $i++) {
			if( in_array( $_REQUEST["tpeid"][$i],$iniciativatipoensinoI) ){
				$db->executar("UPDATE emenda.iniciativatipoensino SET itestatus = 'A' WHERE tpeid = ".$_REQUEST["tpeid"][$i]." and iniid = ".$_REQUEST["iniid"]." and itestatus = 'I'");
			} else {
			if( ! in_array($_REQUEST["tpeid"][$i], $iniciativatipoensino) )
				$db->executar("INSERT INTO emenda.iniciativatipoensino(iniid,tpeid,itestatus) VALUES(".$_REQUEST["iniid"].",".$_REQUEST["tpeid"][$i].",'A')");
			}
		}
		// exclui, se houver necessidade   
		for($i=0; $i<count($iniciativatipoensino); $i++) {
			if( ! in_array($iniciativatipoensino[$i], $_REQUEST["tpeid"]) )
				$db->executar("UPDATE emenda.iniciativatipoensino SET itestatus = 'I' WHERE iniid = ".$_REQUEST["iniid"]." AND tpeid = ".$iniciativatipoensino[$i]);
		}
		
		// Inativa o texto cadastrado e insere a nova Minuta de Convênio
		/*$imiid = $db->pegaUm("SELECT imiid FROM emenda.iniciativaminuta WHERE imistatus = 'A' AND iniid = ".$_REQUEST["iniid"]);
		if($imiid) {
			$db->executar("UPDATE 
								emenda.iniciativaminuta 
						   SET 
						   		imistatus = 'I'
						   WHERE 
						   		imiid = ".$imiid);
		}*/
		
		if( $_REQUEST["mdoid"][0] ){
			
			$db->executar( "UPDATE emenda.iniciativadocumentos SET idostatus ='I' WHERE iniid = {$_REQUEST["iniid"]}" );

			foreach( $_REQUEST["mdoid"] as $mdoid ){
				
				$db->executar( "INSERT INTO emenda.iniciativadocumentos ( iniid, 
																		  mdoid, 
																		  idostatus, 
																		  usucpf, 
																		  idodatainclusao )
																 VALUES ( {$_REQUEST["iniid"]},
																 		  {$mdoid},
																 		  'A',
																 		  '{$_SESSION["usucpf"]}',
																 		  'now()' )" );
					
			}
			
		}
		
		if($_REQUEST["imitexto"] && $_REQUEST["imitexto"] != "") {
			// Insere o texto da Minuta de Convênio
			$db->executar("INSERT INTO 
									emenda.iniciativaminuta(iniid,imitexto,imistatus,usucpf,imidatainclusao)
						   VALUES 
						   		(".$_REQUEST["iniid"].",'".simec_htmlspecialchars($_REQUEST["imitexto"], ENT_QUOTES)."','A','".$_SESSION["usucpf"]."',now())");
		}
		if( is_array($_POST['gdoid']) && $_POST['gdoid'][0] ){
			$db->executar( "DELETE FROM emenda.iniciativa_guiadocumento WHERE iniid = {$_REQUEST["iniid"]}" );
			foreach ($_POST['gdoid'] as $gdoid) {
				$sql = "INSERT INTO emenda.iniciativa_guiadocumento(gdoid, iniid) 
						VALUES ($gdoid, {$_REQUEST["iniid"]})";
				$db->executar( $sql );
			}
		}
	}
	else {
		// INSERT
		// insere a iniciativa 
		$iniid = $db->pegaUm("INSERT INTO 
								emenda.iniciativa(ininome,
													temid,
													gndcod,
													iniescolasbeneficiasdas,
													iniresolucao,
													inistatus) 
									 	VALUES('".pg_escape_string($_REQUEST["ininome"])."',
									 		   ".$_REQUEST["temid"].",
									 		   ".$_REQUEST["gndcod"].",
									 		   '".$_REQUEST["iniescolasbeneficiasdas"]."',
									 		   '".$_REQUEST["iniresolucao"]."',
									 		   'A') 
									 	RETURNING iniid");
		// insere responsável
		for($i=0; $i<count($_REQUEST["resid"]); $i++) {
			$db->executar("INSERT INTO emenda.iniciativaresponsavel(iniid, resid, irestatus) VALUES(".$iniid.",".$_REQUEST["resid"][$i].",'A')");
		}
		// insere beneficiários
		for($i=0; $i<count($_REQUEST["benid"]); $i++) {
			$db->executar("INSERT INTO emenda.iniciativabeneficiario(iniid,benid,icbstatus) VALUES(".$iniid.",".$_REQUEST["benid"][$i].",'A')");
		}
		// insere especificações
		for($i=0; $i<count($_REQUEST["espid"]); $i++) {
			$db->executar("INSERT INTO emenda.iniciativaespecificacao(iniid,espid,icestatus) VALUES(".$iniid.",".$_REQUEST["espid"][$i].",'A')");
		}
		// insere tipos de ensino
		for($i=0; $i<count($_REQUEST["tpeid"]); $i++) {
			$db->executar("INSERT INTO emenda.iniciativatipoensino(iniid,tpeid,itestatus) VALUES(".$iniid.",".$_REQUEST["tpeid"][$i].",'A')");
		}
		
		if($_REQUEST["imitexto"] && $_REQUEST["imitexto"] != "") {
			// Insere o texto da Minuta de Convênio
			$db->executar("INSERT INTO 
									emenda.iniciativaminuta(iniid,imitexto,imistatus,usucpf,imidatainclusao)
						   VALUES 
						   		(".$iniid.",'".simec_htmlspecialchars($_REQUEST["imitexto"], ENT_QUOTES)."','A','".$_SESSION["usucpf"]."',now())");
		}
		
		if( is_array($_POST['gdoid']) && $_POST['gdoid'][0] ){
			$db->executar( "DELETE FROM emenda.iniciativa_guiadocumento WHERE iniid = $iniid" );
			foreach ($_POST['gdoid'] as $gdoid) {
				$sql = "INSERT INTO emenda.iniciativa_guiadocumento(gdoid, iniid) 
						VALUES ($gdoid, $iniid)";
				$db->executar( $sql );
			}
		}
	}
	
	$db->commit();
	$db->sucesso( 'principal/cadastroAcao', '&iniid='.(!empty($_REQUEST["iniid"]) ? $_REQUEST["iniid"] : $iniid));
	die;
}

$db->cria_aba( $abacod_tela, $url, '' );

//echo montarAbasArray(carregaAbasEmendas('cadastroAcao'), "/emenda/emenda.php?modulo=principal/cadastroAcao&acao=A");
monta_titulo('Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

if($_REQUEST["iniid"]) {
	$_SESSION["iniid"] = $_REQUEST["iniid"];
	
	$sql = "SELECT
				ini.iniid,
				ini.ininome,
				ini.temid,
				ini.gndcod,
				ini.iniescolasbeneficiasdas,
				ini.iniresolucao,
				imi.imitexto
			FROM
				emenda.iniciativa ini
			LEFT JOIN
				emenda.iniciativaminuta imi ON imi.iniid = ini.iniid 
										   AND imi.imistatus = 'A'
			WHERE
				ini.inistatus = 'A' AND
				ini.iniid = " . $_REQUEST["iniid"];

	$dados = $db->carregar($sql);
	
	$iniid 		= $dados[0]["iniid"];
	$ininome 	= $dados[0]["ininome"];
	$temid 		= $dados[0]["temid"];
	$gndcod		= $dados[0]["gndcod"];
	$imitexto	= $dados[0]["imitexto"];
	$iniescolasbeneficiasdas = $dados[0]["iniescolasbeneficiasdas"];
	$iniresolucao = $dados[0]["iniresolucao"];
	
	$sql = "SELECT
				ite.tpeid
			FROM
				emenda.iniciativatipoensino ite
			WHERE
				ite.itestatus = 'A' AND
				ite.iniid = ".$_REQUEST["iniid"];
	$tpeid = $db->carregarColuna($sql);
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>	
<form id="formCadastroIniciativa" method="post" action="">
<input type="hidden" name="iniid" value="<?= ($_REQUEST["iniid"]) ? $_REQUEST["iniid"] : "" ?>" />
<input type="hidden" name="submetido" value="1" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Iniciativa:</td>
		<td>
		<?=campo_texto('ininome', 'S', 'S', 'Iniciativa a ser executada', 60, 500, '', '');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Tema:</td>
		<td>
		<?
			$sql = "SELECT temid as codigo,temnome as descricao FROM emenda.tema WHERE temstatus = 'A' ORDER BY temnome";
			$db->monta_combo("temid", $sql, 'S', "Selecione...", '', '', '', '265', 'S', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Grupo de Natureza de Despesa:</td>
		<td>
		<?
			$sql = "SELECT gndcod as codigo,gndcod || ' - ' || gnddsc as descricao FROM public.gnd WHERE gndstatus = 'A'";
			$db->monta_combo("gndcod", $sql, 'S', "Selecione...", '', '', '', '265', 'S', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Resolução:</td>
		<td>
			<?php
				echo campo_texto('iniresolucao', 'N', 'S', 'Resoluções', 100, 500, '', '');				 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">É necessário informar as escolas beneficiadas:</td>
		<td>
			<input type="radio" name="iniescolasbeneficiasdas" id="iniescolasbeneficiasdas" value="t" <? if($iniescolasbeneficiasdas == 't' || !$iniescolasbeneficiasdas) { echo "checked"; } ?>/> Sim
			<input type="radio" name="iniescolasbeneficiasdas" id="iniescolasbeneficiasdas" value="f" <? if($iniescolasbeneficiasdas == 'f' ) { echo "checked"; } ?>/> Não
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Documentos Vinculados:</td>
		<td>
			<?php
			 	if( $_SESSION["iniid"] ){
					$mdoid = $db->carregar("SELECT 
												em.mdoid as codigo,
												'(' || tpddsc || ') ' || mdonome as descricao
											FROM 
												emenda.iniciativadocumentos id
											INNER JOIN
												emenda.modelosdocumentos em ON em.mdoid = id.mdoid
											INNER JOIN
												public.tipodocumento tp ON tp.tpdcod = em.tpdcod 
											WHERE 
												iniid = {$_SESSION["iniid"]} AND 
												idostatus = 'A'" );
			 	} 
			
				$sql = "SELECT
							mdoid as codigo,
							'(' || tpddsc || ') ' || mdonome as descricao
						FROM
							emenda.modelosdocumentos em
						INNER JOIN
							public.tipodocumento tp ON tp.tpdcod = em.tpdcod
						WHERE
							mdostatus = 'A'
						ORDER BY
							mdonome";
			
				combo_popup( 'mdoid', $sql, '', '400x400', '', array(), '', 'S', false, false, 10, 400 );
				 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Responsável:</td>
		<td>
			<a href="#" onclick="inserirResponsavel();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Vincular Responsável à Iniciativa</a><?=obrigatorio(); ?>
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
			   	if($_REQUEST["iniid"]) {
					$sql = "SELECT 
								inr.ireid,
								res.resid,
								res.resdsc,
							    res.resassunto
							FROM 
							  emenda.responsavel res
							  inner join emenda.iniciativaresponsavel inr
							  	on res.resid = inr.resid
							    and inr.irestatus = 'A'
							WHERE
								inr.iniid = ".$_REQUEST['iniid']."
							ORDER BY
								res.resdsc";
					
					$dados = $db->carregar($sql);
					
					if($dados) {
				   		for($i=0; $i<count($dados); $i++) {
				   			$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				   			
							echo "<tr bgcolor=\"".$cor."\" id=\"tr_responsavel_".$dados[$i]["resid"]."\">
		                               <td align=\"center\">
		                               		<input type=\"hidden\" name=\"ireid[]\" value=\"".$dados[$i]["ireid"]."\" />
		                               		<input type=\"hidden\" name=\"resid[]\" value=\"".$dados[$i]["resid"]."\" />
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
	</tr>
	<tr id="tipo_ensino" style="display:<?= (($_REQUEST["iniid"]) ? "table-row" : "none") ?>;">
		<td class="SubTituloDireita" valign="top">Nível de Ensino:</td>
		<td><span id="conteudo_tipo_ensino">
		<?
		if($_REQUEST["iniid"]) {
			//$sql = "SELECT tpeid,tpedsc FROM emenda.tipoensino WHERE tpestatus = 'A' AND resid = ".$resid." ORDER BY tpedsc";
			/*$sql = "SELECT DISTINCT
						te.tpeid,
					    te.tpedsc 
					FROM 
						emenda.tipoensino te
					    inner join emenda.iniciativatipoensino ite	
					    	on ite.tpeid = te.tpeid
					WHERE 
						te.tpestatus = 'A' 
					    AND ite.iniid = ".$_REQUEST["iniid"]."
					ORDER BY te.tpedsc";*/
			$sql = "SELECT DISTINCT
						te.tpeid,
					    te.tpedsc 
					FROM 
						emenda.tipoensino te
					    inner join emenda.iniciativatipoensino ite	
					    	on ite.tpeid = te.tpeid
					WHERE 
						te.tpestatus = 'A' 
					    AND te.resid in (SELECT DISTINCT ir.resid FROM emenda.iniciativaresponsavel ir WHERE ir.irestatus = 'A' and ir.iniid = ".$_REQUEST["iniid"]." )
					ORDER BY te.tpedsc";
			
			$dados = $db->carregar($sql);
			
			if($dados) {
				for($i=0; $i<count($dados); $i++) {
					$checked = (in_array($dados[$i]["tpeid"], $tpeid)) ? 'checked="checked"' : '';
					
					echo '<input type="checkbox" name="tpeid[]" value="'.$dados[$i]["tpeid"].'" '.$checked.' /> '.$dados[$i]["tpedsc"].'<br />';
				}
			}
		}
		?>
		</span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Beneficiários:</td>
		<td>
			<a href="#" onclick="inserirBeneficiario();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Vincular Beneficiário à Iniciativa</a><?=obrigatorio(); ?>
			<br />
			<table id="tb_beneficiarios" width="50%" border="0" cellspacing="0" cellpadding="2" class="listagem">
               <thead>
               <tr>
               		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Excluir</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Beneficiários</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Obrigatório</strong></td>
               </tr>
               </thead>
               <tbody>
               <?
			   	if($_REQUEST["iniid"]) {
					$sql = "SELECT
								ben.benid,
								ben.bennome,
								icbobrigatorio
							FROM
								emenda.beneficiario ben
							INNER JOIN
								emenda.iniciativabeneficiario icb ON  icb.benid = ben.benid
																  AND icb.icbstatus = 'A'
																  AND icb.iniid = ".$_REQUEST["iniid"]."
							WHERE
								ben.benstatus = 'A'
							ORDER BY
								ben.benid";
					$dados = $db->carregar($sql);
					
					if($dados) {
				   		for($i=0; $i<count($dados); $i++) {
				   			$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				   			
							echo "<tr bgcolor=\"".$cor."\" id=\"tr_beneficiario_".$dados[$i]["benid"]."\">
		                               <td align=\"center\">
		                               		<input type=\"hidden\" name=\"benid[]\" value=\"".$dados[$i]["benid"]."\" />
		                               		<a href=\"#\" onclick=\"excluirBeneficiario(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>
		                               </td>
		                               <td align=\"center\">".$dados[$i]["bennome"]."</td>
		                               <td align=\"center\">
		                               		<input type=\"checkbox\" id=\"icbobrigatorio\" name=\"icbobrigatorio[{$dados[$i]["benid"]}]\" value=\"t\" ".($dados[$i]["icbobrigatorio"] == 't' ? 'checked' : '')."/>
		                               </td>
	                              </tr>";
						}
					}
				}
			   ?>
			   </tbody>
       		</table>
       		<br />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Especificações:</td>
		<td>
			<a href="#" onclick="inserirEspecificacao();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Vincular Especificação à Iniciativa</a>&nbsp;<?=obrigatorio(); ?>
			<br />
			<table id="tb_especificacoes" width="50%" border="0" cellspacing="0" cellpadding="2" class="listagem">
               <thead>
               <tr>
               		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Excluir</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Especificação</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade de Medida</strong></td>
               </tr>
               </thead>
               <tbody>
               <?
			   	if($_REQUEST["iniid"]) {
					$sql = "SELECT
								esp.espid,
								esp.espnome,
								CASE WHEN esp.espkit = true THEN esp.espunidademedida || ' (kit)' ELSE esp.espunidademedida END AS unidade   
							FROM
								emenda.especificacao esp
								INNER JOIN emenda.iniciativaespecificacao ice ON ice.espid = esp.espid AND ice.icestatus = 'A' AND ice.iniid = ".$_REQUEST["iniid"]."
								inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
							WHERE
								esp.espstatus = 'A'
							ORDER BY
								esp.espid";
					$dados = $db->carregar($sql);
					
					if($dados) {
						for($i=0; $i<count($dados); $i++) {
				   			$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				   			
							echo "<tr bgcolor=\"".$cor."\" id=\"tr_especificacao_".$dados[$i]["espid"]."\">
		                               <td align=\"center\">
		                               		<input type=\"hidden\" name=\"espid[]\" value=\"".$dados[$i]["espid"]."\" />
		                               		<a href=\"#\" onclick=\"excluirEspecificacao(this.parentNode.parentNode.rowIndex);\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>
		                               </td>
		                               <td align=\"center\">".$dados[$i]["espnome"]."</td>
		                               <td align=\"center\">".$dados[$i]["unidade"]."</td>
	                              </tr>";
						}
					}
				}
			   ?>
			   </tbody>
       		</table>
       		<br />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Documentos Obrigatórios:</td>
		<td><?
				$gdoid = array();
				if($_REQUEST["iniid"]) {
					$sql = "SELECT 
							  	gd.gdoid as codigo,
							  	gd.gdonome as descricao
							FROM 
							  	emenda.guiadocumento gd
							  	inner join emenda.iniciativa_guiadocumento igd on igd.gdoid = gd.gdoid
							WHERE
								igd.iniid = ".$_REQUEST["iniid"]."
							    and gd.gdostatus = 'A'";
					$gdoid = $db->carregar( $sql );
					$gdoid = $gdoid ? $gdoid : array();
				}
				$sql = "SELECT 
							gdoid as codigo,
  							gdonome as descricao
						FROM 
						  	emenda.guiadocumento 
						WHERE 
							gdostatus = 'A' order by gdonome";
				
				combo_popup('gdoid', $sql, '', '400x400', 0, array(), '', 'S', false, false, 05, 570 );
			?></td>
	</tr>
	<?
	$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
	 
	if( in_array( SUPER_USUARIO, $arPerfil ) || in_array( GESTOR_EMENDAS, $arPerfil ) ){
		$retorno = '';
		/*if(!possuiPermissao($categoria)) {
			$retorno = 'disabled="disabled"';
		}*/
	} else {
		$retorno = 'disabled="disabled"';
	}
	?>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarIniciativa();" <?=$retorno;?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="javascript:cancelarIniciativa();" />
		</td>
	</tr>
</table>
</form>

<script type="text/javascript">

<? 
if(!$_REQUEST["iniid"]) {
	echo "carregaTipoEnsino();";
}
?>

var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");

function elaborarMinuta() {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereMinutaConvenio&acao=A', 'minuta', 'fullscreen=1,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
	janela.focus();
}

function salvarIniciativa() {

	selectAllOptions( document.getElementById( 'mdoid' ) );
	selectAllOptions( document.getElementById( 'gdoid' ) );

	var form		= document.getElementById("formCadastroIniciativa");
	var ininome 	= document.getElementsByName("ininome")[0];
	var temid 		= document.getElementsByName("temid")[0];
	var gndcod		= document.getElementsByName("gndcod")[0];
	var espid		= document.getElementsByName("espid[]");
	var resid		= document.getElementsByName("resid[]");
	var tpeid		= document.getElementsByName("tpeid[]");
	var benid		= document.getElementsByName("benid[]");
	
	btSalvar.disabled 	= true;
	btCancelar.disabled = true;
	
	if(ininome.value == "") {
		alert("O campo 'Iniciativa' deve ser preenchido.");
		ininome.focus(); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	if(temid.value == "") {
		alert("O campo 'Tema' deve ser preenchido.");
		temid.focus(); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	if(gndcod.value == "") {
		alert("O campo 'Grupo de Natureza de Despesa' deve ser preenchido.");
		gndcod.focus(); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	if(resid.length == 0) {
		alert("Deve ser inserido pelo menos um 'Responsável'."); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	/*if(benid.length == 0) {
		alert("Deve ser inserido pelo menos um 'Beneficiário'."); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}*/
	if(espid.length == 0) {
		alert("Deve ser inserida pelo menos uma 'Especificação'.");
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}

	var flagTipoEnsino = false;
	for(var i=0; i<tpeid.length; i++) {
		if(tpeid[i].checked == true) flagTipoEnsino = true;
	}
	
	if(!flagTipoEnsino) {
		alert("Pelo menos uma das opções do campo 'Nível de Ensino' deve ser selecionada.");
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}
	
	form.submit();
}

function cancelarIniciativa() {
	btSalvar.disabled 	= true;
	btCancelar.disabled = true;
	window.location = "emenda.php?modulo=principal/listaAcao&acao=A";
}

function inserirBeneficiario() {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereBeneficiarios&acao=A', 'beneficiario', 'width=450,height=400,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
	janela.focus();
}
function excluirBeneficiario(linha) {
	var tabela = document.getElementById("tb_beneficiarios");
	tabela.deleteRow(linha);
}
function inserirResponsavel() {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereResponsavel&acao=A', 'responsavel', 'width=450,height=400,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
	janela.focus();
}
function excluirResponsavel(linha) {
	var trTipoEnsino		= document.getElementById("tipo_ensino");
	
	if(trTipoEnsino.style.display != 'none') {
		if(!confirm("Esta mudança vai excluir as associações com os 'Níveis de Ensino'\n selecionados. Deseja continuar?")) {
			return false;
		}
	}
	
	var tabela = document.getElementById("tb_responsavel");	
	tabela.deleteRow(linha);
	
	var count = document.getElementsByName("resid[]").length;
	var resid = '';
	
	if( count > 0 ){
		for(i=0; i<count; i++){
			var resp = document.getElementsByName("resid[]")[i].value
			if( resid == '' ){
				resid = resp;
			} else {
				resid = resid + ", " + resp ;
			}
		}		
		alteraTipoEnsino( resid );
	} else {
		alteraTipoEnsino( resid );
	}
}

function inserirEspecificacao() {
	//var count = document.getElementsByName("resid[]").length;
	var resp = "";
	var resid = "";
	
	/*if(count == 0) {
		alert("Deve ser inserido pelo menos um 'Responsável'."); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	} else {
		for(i=0; i<count; i++){
			resp = document.getElementsByName("resid[]")[i].value
			if( resid == '' ){
				resid = "(" + resp;
			} else {
				resid = resid + ", " + resp ;
			}
		}
		resid = resid + ')';*/
		
		var janela = window.open( '/emenda/emenda.php?modulo=principal/popInsereEspecificacoes&acao=A&resid=' + resid + '', 'especificacao', 'width=450,height=400,status=0,menubar=0,toolbar=0,scrollbars=yes,resizable=0' );
		janela.focus();
	//}
}
function excluirEspecificacao(linha) {
	var tabela = document.getElementById("tb_especificacoes");
	tabela.deleteRow(linha);
}

function alteraTipoEnsino(id_select) {
	var resid				= document.getElementsByName("resid[]");
	var trTipoEnsino		= document.getElementById("tipo_ensino");
	var residSelecionado	= id_select;//document.getElementById("resid_selecionado");
	var res = "";
	
	if(trTipoEnsino.style.display == 'none') {
		//residSelecionado.value = resid.value;
		carregaTipoEnsino();
	}
	else {
		//if(confirm("Esta mudança vai excluir as associações com as 'Especificações' e os 'Níveis de Ensino'\n selecionados. Deseja continuar?")) {
			/*var tbEspecificacoes = document.getElementById("tb_especificacoes");
			var tamanho 		 = tbEspecificacoes.rows.length;
			
			for(var k=1; k<tamanho; k++) {
				tbEspecificacoes.deleteRow(1);
			}
			
			residSelecionado = id_select;*/
			carregaTipoEnsino();
		//} 
		/*else {
			var vSelectedIndex;
			
			for(var i=0; i<resid.length; i++) {
				if(resid.options[i].value == residSelecionado)
					vSelectedIndex = i;
			}
			resid.selectedIndex = vSelectedIndex;
		}*/
	}
}

function carregaTipoEnsino() {
	var trTipoEnsino	= document.getElementById("tipo_ensino");
	var tpeid		= document.getElementsByName("tpeid[]");
	var spanTipoEnsino	= document.getElementById("conteudo_tipo_ensino");
	var resid			= "";
	var res = "";
	var count = document.getElementsByName("resid[]").length;

	if(count == 0) {
		trTipoEnsino.style.display 	= 'none';
		spanTipoEnsino.innerHTML	= '';
	}
	else {
		
		for(i=0; i<count; i++){
			res = document.getElementsByName("resid[]")[i].value
			if( resid == '' ){
				resid = "(" + res;
			} else {
				resid = resid + ", " + res ;
			}
		}
		resid = resid + ')';
		
		var tpeidCheck = '';
		for(var i=0; i<tpeid.length; i++) {
			if(tpeid[i].checked == true){
				if( tpeidCheck == '' )
				 	tpeidCheck = tpeid[i].value;
				else
					tpeidCheck = tpeidCheck + ',' + tpeid[i].value;
			} 
		}

		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: "carregaTipoEnsino=true&resid=" + resid + "&tpeid=" + tpeidCheck,
				asynchronous: false,
				onComplete: function(resp) {
					spanTipoEnsino.innerHTML	= resp.responseText;
					trTipoEnsino.style.display 	= 'table-row';
				}
			});
	}
}

</script>