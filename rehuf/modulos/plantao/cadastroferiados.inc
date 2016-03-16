<?php

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirFeriadosPlantao'=>true,
									 'atualizarFeriadosPlantao'=>true,
									 'excluirFeriadosPlantao'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

$permissoes = verificaPerfilRehuf();

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
/* fim montando cabeçalho */

switch($_REQUEST['vis']) {
	
	case 'edicao':
		if($_REQUEST['ferid']) {
			$sql = "SELECT * FROM rehuf.feriado WHERE ferid='".$_REQUEST['ferid']."'";
			$feriado = $db->pegaLinha($sql);
			$ferid  = $feriado['ferid'];
			$ferdata  = $feriado['ferdata'];
			$entid = $feriado['entid'];
			$requisicao = "atualizarFeriadosPlantao"; 
		} else {
			$requisicao = "inserirFeriadosPlantao";
		}
?>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<form method='post' name='formulario'>
		<input type='hidden' name='carid' value='<? echo $ferid; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Cadastro de Feriados</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Feriado:</td>
			<td><? echo campo_data2("ferdata","S","S","Data do Feriado","S","","", ""); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Classificação:</td>
			<td>
			<? 
			$sql = "SELECT upper(ent.entnome)||'('||ena.entsig||')' as descricao, ent.entid as codigo
				FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid
				LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid
				WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND 
					  ent.entid IN('".implode("','",$permissoes['verhospitais'])."') GROUP BY ent.entid, ent.entnome, ena.entsig 
				ORDER BY ena.entsig";
			$db->monta_combo('entid', $sql, 'S', 'FERIADO NACIONAL', '', '', '', '', 'S', 'entid');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=plantao/cadastroferiados&acao=A';"></td>
		</tr>
		</table>
		</form>
<?		
		break;

	default:
		$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=plantao/cadastroferiados&acao=A&vis=edicao&ferid='||ferid||'\';\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=plantao/cadastroferiados&acao=A&requisicao=excluirFeriadosPlantao&ferid='||ferid||'\', \'Deseja realmente excluir este feriado?\');\"></center>', to_char(ferdata , 'dd/mm/YYYY') as ferdata, CASE WHEN ent.entnome IS NULL THEN 'Feriado Nacional' ELSE 'Feriado Regional ('||ent.entnome||')' END as entnome 
				FROM rehuf.feriado fer 
				LEFT JOIN entidade.entidade ent ON ent.entid=fer.entid 
				ORDER BY fer.ferdata";
		$cabecalho = array("", "Data", "Classificação");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<div style="float: left;">
				<input type='button' onclick="window.location='?modulo=plantao/cadastroferiados&acao=A&vis=edicao'" value='Inserir'>
				</div>
			</td>
		</tr>
		</table>
<?
}
?>
