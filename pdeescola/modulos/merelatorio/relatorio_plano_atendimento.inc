<?

ini_set( "memory_limit", "512M" );
set_time_limit(0);

if ($_POST['ajaxestuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '' ));
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório Mais Educação', 'Plano de Atendimento Geral Consolidado - ' . $_SESSION["exercicio"] );

// Recupera o perfil do usuário no Mais Educação.
$usuPerfil = arrayPerfil();

if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
	$arrWhere = array();
	$strWhere = "";
	
	if(isset($_REQUEST["entcodent"]) && $_REQUEST["entcodent"] != "") {
		array_push($arrWhere, "ent.entcodent in ('".$_REQUEST["entcodent"]."')");
	}
	if(isset($_REQUEST["entnome"]) && $_REQUEST["entnome"] != "") {
		array_push($arrWhere, "ent.entnome ilike '%".$_REQUEST["entnome"]."%'");
	}
	//busca so estadual
	if(isset($_REQUEST["estuf"]) && $_REQUEST["estuf"] != "" && !$_REQUEST["muncod"]) {
		array_push($arrWhere, "mli.estuf = '".$_REQUEST["estuf"]."'");
	}
	//busca so municipal
	if(isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "") {
		array_push($arrWhere, "mli.muncod = '".$_REQUEST["muncod"]."'");
	}
	
	
	
	// Se existir algum filtro na consulta, monta o 'WHERE' a ser incluído.
	if(count($arrWhere) > 0) {
		//$strWhere .= " WHERE ".implode(' OR ', $arrWhere); 
		$strWhere .= " WHERE ".implode(' AND ', $arrWhere);
	}
	
	$sql = "SELECT
				to_char(mli.mlidataimpressao, 'DD/MM/YYYY') as mlidataimpressao,
				mli.entid,
				mli.mlicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao,
				esd.esdid
			FROM
				pdeescola.meloteimpressao mli
			INNER JOIN
				pdeescola.memaiseducacao mme ON mme.memid = mli.memid AND
												mme.entid = mli.entid AND
												mme.memanoreferencia = " . $_SESSION["exercicio"] . " AND
												mme.memstatus = 'A'
			INNER JOIN
				workflow.documento doc ON doc.docid = mme.docid
			INNER JOIN
				workflow.estadodocumento esd ON esd.esdid = doc.esdid
			INNER JOIN
				entidade.entidade ent ON ent.entid = mli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = mli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = mli.estuf 
			".$strWhere." 
			ORDER BY
				mli.mlicodigolote";
}
else if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);

	$sql = "SELECT
				to_char(mli.mlidataimpressao, 'DD/MM/YYYY') as mlidataimpressao,
				mli.entid,
				mli.mlicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao
			FROM
				pdeescola.meloteimpressao mli
			INNER JOIN
				pdeescola.memaiseducacao mme ON mme.memid = mli.memid AND
												mme.entid = mli.entid AND
												mme.memanoreferencia = " . $_SESSION["exercicio"] . " AND
												mme.memstatus = 'A'
			INNER JOIN
				entidade.entidade ent ON ent.entid = mli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = mli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = mli.estuf 
			WHERE
				mli.muncod = '".$pflcod[0]["muncod"]."'
			ORDER BY
				mli.mlicodigolote";
}
else if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);
	
	$sql = "SELECT
				to_char(mli.mlidataimpressao, 'DD/MM/YYYY') as mlidataimpressao,
				mli.entid,
				mli.mlicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao
			FROM
				pdeescola.meloteimpressao mli
			INNER JOIN
				pdeescola.memaiseducacao mme ON mme.memid = mli.memid AND
												mme.entid = mli.entid AND
												mme.memanoreferencia = " . $_SESSION["exercicio"] . " AND
												mme.memstatus = 'A'
			INNER JOIN
				entidade.entidade ent ON ent.entid = mli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = mli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = mli.estuf 
			WHERE
				mli.estuf = '".$pflcod[0]["estuf"]."'
			ORDER BY
				mli.mlicodigolote";
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			history.back(-1);
		  </script>";
	exit;
}
// ver($sql);
//($_REQUEST["entcodent"] || $_REQUEST["entnome"] || $_REQUEST["estuf"] || $_REQUEST["muncod"])
if( $_REQUEST["submetido"] || (in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) ) {
	$meLoteImpressao = $db->carregar($sql);
}
?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formFiltroRelatorio" method="post" action="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3"	align="center">
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td align="center"><b>Visualizar todas as escolas finalizadas?</b></td>
	</tr>
	<tr>
		<td align="center">
			<input type="radio" name="visualizar_escolas" value="sim" checked="ckecked" <?= (((!$meLoteImpressao) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) ? "disabled=\"disabled\"" : "") ?> /> Sim
			<input type="radio" name="visualizar_escolas" value="nao" <?= (((!$meLoteImpressao) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) ? "disabled=\"disabled\"" : "") ?> /> Não
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td align="center">
			<input type="button" id="bt_gerar_relatorio" onclick="abrePopupRelatorio();" value="Gerar Relatório" <? if(!verificaPodeGerarRelConsolidado()) { echo "disabled=\"disabled\""; } ?> />
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<? if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) { ?>
<input type="hidden" name="submetido" value="1" />
<tr>
	<td valign="bottom" align="center">
	<div style="background-color:#f0f0f0; width:50%; border: 1px solid #a0a0a0;">
	
		<b>Filtros</b>
		<br /><br />
		<table width="80%" align="center" border="0" cellspacing="2" cellpadding="4" class="listagem">
			<tr>
				<td>Código da Escola:</td>
				<td>
				<?
					$entcodent = $_REQUEST['entcodent'];
					echo campo_texto( 'entcodent', 'N', 'S', '', 50, 200, '', '' );
				?>
				</td>
			</tr>
			<tr>
				<td>Escola:</td>
				<td>
				<?
					$entnome = simec_htmlentities( $_REQUEST['entnome'] );
					echo campo_texto( 'entnome', 'N', 'S', '', 50, 200, '', '' );
				?>
				</td>
			</tr>
			<tr>
				<td>Estado:</td>
				<td>
				<?php
				$estuf = $_REQUEST['estuf'];
				$sql = "SELECT
							e.estuf AS codigo, 
							e.estdescricao AS descricao 
						FROM
							territorios.estado e 
						ORDER BY
							e.estdescricao ASC";
				$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', 200 );
				?>
				</td>
			</tr>
			<tr>
				<td>Município:</td>
				<td id="tdMunicipio">
				<?		
				$muncod = $_REQUEST['muncod'];
				if(!$estuf) $estufx = 'X';
				else $estufx = $estuf;
				$sql = "SELECT
							muncod AS codigo, 
							mundescricao AS descricao 
						FROM
							territorios.municipio
						where
							 estuf = '$estufx' 
						ORDER BY
							mundescricao ASC";
				$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', 200 );
				?>
				</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:center;">
					<input type="button" id="btFiltroRelatorio" value="Filtrar" onclick="submeteFiltro();" />
				</td>
			</tr>
		</table>
	
	</div>
	</td>
</tr>
<tr>
	<td align="center">
		<?
			if($meLoteImpressao) {
				echo '<font color="red">* As escolas em vermelho ainda não receberam o pagamento.</font>';
			} else {
				echo '&nbsp;';
			}
		?>
	</td>
</tr>
<? } ?>
<tr>	
	<td>
	
		<?
		//($_REQUEST["entcodent"] || $_REQUEST["entnome"] || $_REQUEST["estuf"] || $_REQUEST["muncod"])
		if( $_REQUEST["submetido"] || (in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) ) {?>
		
			<table id="tabela_etapas" width="80%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr id="cabecalho">
				<? if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) { 
					$colspan = 7;
				?>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>
					<td width="40%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Entidades Envolvidas</strong></td>
					<td width="15%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
					<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Estado</strong></td>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Enviar/Visualizar Pagamento</strong></td>
					<!--<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Pagamento Enviado</strong></td>-->
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Visualizar o Relatório</strong></td>
				<? } else { 
					$colspan = 5;
				?>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>
					<td width="40%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Entidades Envolvidas</strong></td>
					<td width="20%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
					<td width="20%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Estado</strong></td>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Visualizar o Relatório</strong></td>
				<? } ?>
				</tr>
			</thead>
			<tbody>
			<?
			if($meLoteImpressao) {
				$vEntidAux = array();
				$vEntidAuxTodos = array();
				$vEntidNomeAux = array();
				$cont = 0;
				$cor = "#f4f4f4";
				
				for($i=0; $i<count($meLoteImpressao); $i++) {
					//array_push($vEntidAux, $meLoteImpressao[$i]["entid"]);
					//array_push($vEntidAuxTodos, $meLoteImpressao[$i]["entid"]);	
									
					if( !in_array($meLoteImpressao[$i]["mlicodigolote"],$vEntidAux) ) array_push($vEntidAux, $meLoteImpressao[$i]["mlicodigolote"]);
					if( !in_array($meLoteImpressao[$i]["mlicodigolote"],$vEntidAuxTodos) ) array_push($vEntidAuxTodos, $meLoteImpressao[$i]["mlicodigolote"]);
					
					//$pagamentoEnviado = ($meLoteImpressao[$i]["esdid"] == 40) ? "Sim" : "Não";
					$pagamentoEnviado = ((integer)$meLoteImpressao[$i]["esdid"] == 40) ? true : false;
					
					if($pagamentoEnviado)
						$vNome = "<img src=\"../imagens/arrow_simples.gif\" /> ".$meLoteImpressao[$i]["entnome"];
					else
						$vNome = "<img src=\"../imagens/arrow_simples.gif\" /> <font color=\"red\">".$meLoteImpressao[$i]["entnome"]."</font>";
					
					array_push($vEntidNomeAux, $vNome);
					
					// último registro
					if($i == (count($meLoteImpressao)-1)) {
						echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\" >
								<td align=\"center\">".$meLoteImpressao[$i]["mlidataimpressao"]."</td>
								<td align=\"left\">".implode("<br />", $vEntidNomeAux)."</td>
								<td align=\"center\">".(($meLoteImpressao[$i]["mundescricao"]) ? $meLoteImpressao[$i]["mundescricao"] : "-")."</td>
								<td align=\"center\">".(($meLoteImpressao[$i]["estdescricao"]) ? $meLoteImpressao[$i]["estdescricao"] : "-")."</td>";
						
						if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
							echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"realizaPagamento('".implode(",", $vEntidAux)."');\"><img border=\"0\" src=\"../imagens/money.gif\" /></a></td>";
							//echo "<td align=\"center\">".$pagamentoEnviado."</td>";
						}
	
						echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"visualizaRelatorio('".implode(",", $vEntidAux)."', '".$meLoteImpressao[$i]["muncod"]."', '".$meLoteImpressao[$i]["estuf"]."');\"><img border=\"0\" src=\"../imagens/consultar.gif\" /></a></td>
							</tr>";
						
						$cont++;
					} else {
						if($meLoteImpressao[($i+1)]["mlicodigolote"] != $meLoteImpressao[$i]["mlicodigolote"]) {
							echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
									<td align=\"center\">".$meLoteImpressao[$i]["mlidataimpressao"]."</td>
									<td align=\"left\">".implode("<br />", $vEntidNomeAux)."</td>
									<td align=\"center\">".(($meLoteImpressao[$i]["mundescricao"]) ? $meLoteImpressao[$i]["mundescricao"] : "-")."</td>
									<td align=\"center\">".(($meLoteImpressao[$i]["estdescricao"]) ? $meLoteImpressao[$i]["estdescricao"] : "-")."</td>";
							
							if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
								echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"realizaPagamento('".implode(",", $vEntidAux)."');\"><img border=\"0\" src=\"../imagens/money.gif\" /></a></td>";
								//echo "<td align=\"center\">".$pagamentoEnviado."</td>";
							}
									
							echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"visualizaRelatorio('".implode(",", $vEntidAux)."', '".$meLoteImpressao[$i]["muncod"]."', '".$meLoteImpressao[$i]["estuf"]."');\"><img border=\"0\" src=\"../imagens/consultar.gif\" /></a></td>
								</tr>";
							
							$cont++;
							$vEntidAux = array();
							$vEntidNomeAux = array();
							$cor = ($cor == "#f4f4f4") ? "#e0e0e0" : "#f4f4f4";
						}
					}
				}
				
				if(!in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) && !in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
					if($cont > 1) {
						echo "<tr bgcolor=\"c0c0c0\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='c0c0c0';\" >
								<td align=\"right\" colspan=\"4\"><b>Consolidação dos relatórios gerados:</td>
								<td align=\"center\">
									<a href=\"javascript:void(0);\" onclick=\"visualizaRelatorio('".implode(",", $vEntidAuxTodos)."', '', '');\"><img border=\"0\" src=\"../imagens/consultar.gif\" /></a>
									&nbsp;&nbsp;
									<a href=\"javascript:void(0);\" onclick=\"visualizaRelatorioPDF('".implode(",", $vEntidAuxTodos)."');\"><img border=\"0\" style=\"width:16px;height:16px;\" src=\"../imagens/pdf.gif\" /></a>
								</td>
							  </tr>";
					}
				}
			} else {
				echo "<tr bgcolor=\"#f4f4f4\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#f4f4f4';\">
						<td colspan=\"".$colspan."\" align=\"center\">
							<font color=\"red\">Sem registros. Não foram gerados relatórios até o momento.</font>
						</td>
					  </tr>";
			}
			?>
			</tbody>
			</table>
		<?}?>
	</td>
</tr>
</table>

</form>


<script type="text/javascript">

function abrePopupRelatorio() {
 	var radioBt = document.getElementsByName("visualizar_escolas");
	var url;
	
	if(radioBt[0].checked) {
		url = 'pdeescola.php?modulo=merelatorio/plano_atendimento_geral_consolidado&acao=A&req=adesao';
// 		url = 'pdeescola.php?modulo=merelatorio/plano_atendimento_geral_consolidado&acao=A';
		params = 'width=780,height=560,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1';
	}
	else {
		url = 'pdeescola.php?modulo=merelatorio/lista_escolas_geral_consolidado&acao=A';
		params = 'width=780,height=460,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0';
	}
		
	var janela = window.open( url, 'relatorio', params );
	janela.focus();
}

function visualizaRelatorio(mlicodigolote, muncod, estuf) {
	var janela = window.open('pdeescola.php?modulo=merelatorio/plano_atendimento_geral_consolidado&acao=A&lista_escolas='+mlicodigolote+'&visualizar=1&muncod='+muncod+'&estuf='+estuf+'' , 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function visualizaRelatorioPDF(mlicodigolote)
{
	var janela = window.open('pdeescola.php?modulo=merelatorio/geral_consolidado_pdf&acao=A&lista_escolas='+mlicodigolote+'' , 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function realizaPagamento(mlicodigolote) {
	var janela = window.open('pdeescola.php?modulo=merelatorio/realizar_pagamento&acao=A&lista_escolas='+mlicodigolote+'' , 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
	janela.focus();
}

function submeteFiltro() {
	var form 	= document.getElementById("formFiltroRelatorio");
	var button	= document.getElementById("btFiltroRelatorio");
	
	button.disabled = true;

	/*if(form.entcodent.value=='' && form.entnome.value=='' && form.estuf.value==''){
		alert("Preencha pelo menos um dos campos para o filtro");
		button.disabled = false;
		return;
	}*/

	form.submit();
}

function filtraMunicipio(estuf) {

	
	td     = document.getElementById('tdMunicipio');	
	select = td.getElementsByTagName('select')[0];
	if (select)	select.disabled = true;

	if(estuf){
		var req = new Ajax.Request('pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A', {
						        method:     'post',
						        parameters: '&ajaxestuf=' + estuf,
						        onComplete: function (res)
						        {		
									td.innerHTML = res.responseText;
						        }
						  });
	}
	else{
		td.innerHTML = "<select name='muncod'><option value=''>Selecione...</option></select>";
	}
}

</script>