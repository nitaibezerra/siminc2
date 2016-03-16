<?php

include_once APPRAIZ . 'includes/workflow.php';

/*** Recupera o(s) perfil(is) do usuário ***/
$usuPerfil = arrayPerfil();

/**
 * Recupera o combo de municípios
 */
if( $_POST['ajaxestuf'] )
{
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '', '', 200, 'N', 'muncod' ));
}

/**
 *  Verifica se pode emitir o relatório para o lote de escolas
 *  Em caso afirmativo
 */
if( $_POST["ajax"] )
{
	/*** Inicializa o retorno do AJAX ***/
	$retorno = array();
	$retorno['valida'] = false;
	
	/*** Se for perfil Estadual ***/
	if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $usuPerfil) )
	{
		/*** Recupera a UF associada ***/
		$estuf = $db->pegaUm("SELECT estuf FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");
		/*** Se retornou algum registro ***/
		if( $estuf )
		{
			/*** Prepara a query ***/
			$sql = "SELECT num_escolas FROM
					(
						SELECT
							count(eac.eacid) as num_escolas
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 1
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.estuf = '".$estuf."'
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
							
						UNION ALL
						
						SELECT
							count(eac.eacid) as num_escolas
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 1
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.estuf = '".$estuf."'
						INNER JOIN
							workflow.documento doc ON doc.docid = eac.docid
												  AND doc.esdid = ".FINALIZADO_EA."
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
					) as foo";
			$numEscolas = $db->carregarColuna($sql);
			/*** Verifica se todas as escolas do estado(UF) estão no estado 'Finalizado' ***/
			if( (integer)$numEscolas[0] == (integer)$numEscolas[1] )
			{
				/*** Recupera todas as escolas finalizadas ***/
				$sql = "SELECT
							eac.eacid,
							eac.entid,
							eac.docid,
							ende.estuf
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 1
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.estuf = '".$estuf."'
						INNER JOIN
							workflow.documento doc ON doc.docid = eac.docid
												  AND doc.esdid = ".FINALIZADO_EA."
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."";
				$dadosEscolas = $db->carregar($sql);
				/*** Se existirem registros ***/
				if($dadosEscolas)
				{
					/*** Transação para evitar que usuários diferentes recuperem o mesmo código do lote ***/
					//pg_query( $db->link, 'begin transaction;');
					/*** Recupera o código do lote de escolas ***/
					$codLote = $db->pegaUm("SELECT coalesce(max(elicodigolote), 0) FROM pdeescola.eacloteimpressao");
					$codLote++;
					
					/*** Percorre o array de escolas ***/
					for($i=0; $i<count($dadosEscolas); $i++)
					{
						/*** Prepara e executa o INSERT ***/
						$sql = "INSERT INTO
									pdeescola.eacloteimpressao
									(
									 elidataimpressao,
									 entid,
									 estuf,
									 elicodigolote,
									 eacid
									)
								VALUES
									(
									 now(),
									 ".$dadosEscolas[$i]["entid"].",
									 '".$dadosEscolas[$i]["estuf"]."',
									 ".$codLote.",
									 ".$dadosEscolas[$i]["eacid"]."
									)";
						$db->executar($sql);
						
						/*** O 'aedid' da ação: finalizado -> relatório emitido. (produção) ***/
						$aedid = EA_AEDID_FINALIZADO_REL_EMITIDO;
						/*** Sem comentário ***/
						$comentario = NULL;
						/*** Sem dados no array, já que não há chamada de função após a ação ***/
						$dados = array();
						/*** Realiza a alteração do estado da escola ***/
						wf_alterarEstado( $dadosEscolas[$i]["docid"], $aedid, $comentario, $dados );
					}
					/*** Se der tudo certo com o commit, altera a variável de retorno do AJAX ***/
					if( $db->commit() )
					{
						$retorno['valida'] 	= true;
						$retorno['lote'] 	= $codLote;
					}
				}
			}
		}
	}
	/*** Se for perfil Municipal ***/
	if( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $usuPerfil) )
	{
		/*** Recupera o município associado ***/
		$muncod = $db->pegaUm("SELECT muncod FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");
		/*** Se retornou algum registro ***/
		if( $muncod )
		{
			/*** Prepara a query ***/
			$sql = "SELECT num_escolas FROM
					(
						SELECT
							count(eac.eacid) as num_escolas
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 3
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.muncod = '".$muncod."'
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
							
						UNION ALL
						
						SELECT
							count(eac.eacid) as num_escolas
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 3
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.muncod = '".$muncod."'
						INNER JOIN
							workflow.documento doc ON doc.docid = eac.docid
												  AND doc.esdid = ".FINALIZADO_EA."
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
					) as foo";
			$numEscolas = $db->carregarColuna($sql);
			/*** Verifica se todas as escolas do município estão no estado 'Finalizado' ***/
			if( (integer)$numEscolas[0] == (integer)$numEscolas[1] )
			{
				/*** Recupera todas as escolas finalizadas ***/
				$sql = "SELECT
							eac.eacid,
							eac.entid,
							eac.docid,
							ende.muncod
						FROM
							pdeescola.eacescolaacessivel eac
						INNER JOIN
							entidade.entidade ent ON ent.entid = eac.entid
												 AND ent.tpcid = 3
						INNER JOIN
							entidade.endereco ende ON ende.entid = eac.entid
												  AND ende.muncod = '".$muncod."'
						INNER JOIN
							workflow.documento doc ON doc.docid = eac.docid
												  AND doc.esdid = ".FINALIZADO_EA."
						WHERE
							eac.eacstatus = 'A'
							AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."";
				$dadosEscolas = $db->carregar($sql);
				/*** Se existirem registros ***/
				if($dadosEscolas)
				{
					/*** Transação para evitar que usuários diferentes recuperem o mesmo código do lote ***/
					//pg_query( $db->link, 'begin transaction;');
					/*** Recupera o código do lote de escolas ***/
					$codLote = $db->pegaUm("SELECT coalesce(max(elicodigolote), 0) FROM pdeescola.eacloteimpressao");
					$codLote++;
					
					/*** Percorre o array de escolas ***/
					for($i=0; $i<count($dadosEscolas); $i++)
					{
						/*** Prepara e executa o INSERT ***/
						$sql = "INSERT INTO
									pdeescola.eacloteimpressao
									(
									 elidataimpressao,
									 entid,
									 muncod,
									 elicodigolote,
									 eacid
									)
								VALUES
									(
									 now(),
									 ".$dadosEscolas[$i]["entid"].",
									 '".$dadosEscolas[$i]["muncod"]."',
									 ".$codLote.",
									 ".$dadosEscolas[$i]["eacid"]."
									)";
						$db->executar($sql);
						
						/*** O 'aedid' da ação: finalizado -> relatório emitido. (produção) ***/
						$aedid = EA_AEDID_FINALIZADO_REL_EMITIDO;
						/*** Sem comentário ***/
						$comentario = NULL;
						/*** Sem dados no array, já que não há chamada de função após a ação ***/
						$dados = array();
						/*** Realiza a alteração do estado da escola ***/
						wf_alterarEstado( $dadosEscolas[$i]["docid"], $aedid, $comentario, $dados );
					}
					/*** Se der tudo certo com o commit, altera a variável de retorno do AJAX ***/
					if( $db->commit() )
					{
						$retorno['valida'] 	= true;
						$retorno['lote'] 	= $codLote;
					}
				}
			}
		}
	}
	
	/*** Retorna o objeto JSON e finaliza o script ***/
	die( simec_json_encode($retorno) );
}

/*** Inclue o cabeçalho do sistema ***/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
/*** Monta o título da página ***/
$titulo		= 'Relatório Escola Acessível';
$subtitulo	= 'Plano de Atendimento Geral Consolidado - ' . $_SESSION["exercicio"];
monta_titulo( $titulo, $subtitulo );


if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) )
{
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
		array_push($arrWhere, "eli.estuf = '".$_REQUEST["estuf"]."'");
	}
	//busca so municipal
	if(isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "") {
		array_push($arrWhere, "eli.muncod = '".$_REQUEST["muncod"]."'");
	}
	
	// Se existir algum filtro na consulta, monta o 'WHERE' a ser incluído.
	if(count($arrWhere) > 0) {
		//$strWhere .= " WHERE ".implode(' OR ', $arrWhere); 
		$strWhere .= " WHERE ".implode(' AND ', $arrWhere);
	}
	
	$sql = "SELECT
				to_char(eli.elidataimpressao, 'DD/MM/YYYY') as elidataimpressao,
				eli.entid,
				eli.elicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao
			FROM
				pdeescola.eacloteimpressao eli
			INNER JOIN
				pdeescola.eacescolaacessivel eac ON eac.eacid = eli.eacid
												AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
												AND eac.eacstatus = 'A'
			INNER JOIN
				entidade.entidade ent ON ent.entid = eli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = eli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = eli.estuf 
			".$strWhere." 
			ORDER BY
				eli.elicodigolote";
}
else if( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $usuPerfil) )
{
	/*** Recupera o município associado ao usuário ***/
	$muncod = $db->pegaUm("SELECT muncod FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");

	$sql = "SELECT
				to_char(eli.elidataimpressao, 'DD/MM/YYYY') as elidataimpressao,
				eli.entid,
				eli.elicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao
			FROM
				pdeescola.eacloteimpressao eli
			INNER JOIN
				pdeescola.eacescolaacessivel eac ON eac.eacid = eli.eacid
												AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
												AND eac.eacstatus = 'A'
			INNER JOIN
				entidade.entidade ent ON ent.entid = eli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = eli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = eli.estuf 
			WHERE
				eli.muncod = '".$muncod."'
			ORDER BY
				eli.elicodigolote";
}
else if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $usuPerfil)) {
	/*** Recupera o estado associado ao usuário ***/
	$estuf = $db->pegaUm("SELECT estuf FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");
	
	$sql = "SELECT
				to_char(eli.elidataimpressao, 'DD/MM/YYYY') as elidataimpressao,
				eli.entid,
				eli.elicodigolote,
				ent.entnome,
				mun.muncod,
				mun.mundescricao,
				est.estuf,
				est.estdescricao
			FROM
				pdeescola.eacloteimpressao eli
			INNER JOIN
				pdeescola.eacescolaacessivel eac ON eac.eacid = eli.eacid
												AND eac.eacanoreferencia = ".$_SESSION["exercicio"]."
												AND eac.eacstatus = 'A'
			INNER JOIN
				entidade.entidade ent ON ent.entid = eli.entid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = eli.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = eli.estuf 
			WHERE
				eli.estuf = '".$estuf."'
			ORDER BY
				eli.elicodigolote";
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			history.back(-1);
		  </script>";
	exit;
}

if( $_REQUEST["submetido"] || (in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $usuPerfil)) ) 
{
	$eaLote = $db->carregar($sql);
}

?>

<!-- Inclue o JQuery -->
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.js"></script>
<!-- Inclue o arquivo JS específico da página -->
<script type="text/javascript" src="./js/js_relatorio_plano_atendimento.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3"	align="center">
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td align="center"><b>Gerar Rel.Consolidado para todas as escolas finalizadas:</b></td>
	</tr>
	<tr>
		<td align="center">
			<input type="button" id="bt_gerar_relatorio" value="Gerar Relatório" <? if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) ) { echo "disabled=\"disabled\""; } ?> />
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
</table>

<form id="formFiltroRelatorio" method="post" action="">
<input type="hidden" name="submetido" value="1" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

<? if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) ) { ?>

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
				$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', 200, 'N', 'estuf' );
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
				$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod' );
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
<? } ?>
<tr>	
	<td>
	<? if( $_REQUEST["submetido"] || (in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $usuPerfil)) ) { ?>
	<table id="tabela_etapas" width="80%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
	<thead>
		<tr id="cabecalho">
			<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>
			<td width="40%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Entidades Envolvidas</strong></td>
			<td width="20%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
			<td width="20%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Estado</strong></td>
			<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Visualizar o Relatório</strong></td>
		</tr>
	</thead>
	<tbody>
	<?php 
	if($eaLote)
	{		
		$vEntidNomeAux = array();
		$cor = "#f4f4f4";
		
		for($i=0; $i<count($eaLote); $i++) {
			$vNome = "<img src=\"../imagens/arrow_simples.gif\" /> ".$eaLote[$i]["entnome"];
			array_push($vEntidNomeAux, $vNome);
			
			// último registro
			if($i == ( count($eaLote) - 1 ) )
			{
				echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\" >
						<td align=\"center\">".$eaLote[$i]["elidataimpressao"]."</td>
						<td align=\"left\">".implode("<br />", $vEntidNomeAux)."</td>
						<td align=\"center\">".(($eaLote[$i]["mundescricao"]) ? $eaLote[$i]["mundescricao"] : "-")."</td>
						<td align=\"center\">".(($eaLote[$i]["estdescricao"]) ? $eaLote[$i]["estdescricao"] : "-")."</td>";

				echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"visualizaRelatorio(".$eaLote[$i]["elicodigolote"].", '".$eaLote[$i]["muncod"]."', '".$eaLote[$i]["estuf"]."');\"><img border=\"0\" src=\"../imagens/consultar.gif\" /></a></td>
					</tr>";
			}
			else
			{
				if($eaLote[($i+1)]["elicodigolote"] != $eaLote[$i]["elicodigolote"])
				{
					echo "<tr bgcolor=\"".$cor."\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".$cor."';\">
							<td align=\"center\">".$eaLote[$i]["elidataimpressao"]."</td>
							<td align=\"left\">".implode("<br />", $vEntidNomeAux)."</td>
							<td align=\"center\">".(($eaLote[$i]["mundescricao"]) ? $eaLote[$i]["mundescricao"] : "-")."</td>
							<td align=\"center\">".(($eaLote[$i]["estdescricao"]) ? $eaLote[$i]["estdescricao"] : "-")."</td>";
					
					echo "<td align=\"center\"><a href=\"javascript:void(0);\" onclick=\"visualizaRelatorio(".$eaLote[$i]["elicodigolote"].", '".$eaLote[$i]["muncod"]."', '".$eaLote[$i]["estuf"]."');\"><img border=\"0\" src=\"../imagens/consultar.gif\" /></a></td>
						</tr>";
					
					$vEntidNomeAux = array();
					$cor = ($cor == "#f4f4f4") ? "#e0e0e0" : "#f4f4f4";
				}
			}
		}
	}
	else
	{
		echo "<tr bgcolor=\"#f4f4f4\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#f4f4f4';\">
				<td colspan=\"5\" align=\"center\">
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