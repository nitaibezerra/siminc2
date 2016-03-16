<?php

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirSetoresPlantao'=>true,
									 'atualizarSetoresPlantao'=>true,
									 'excluirSetoresPlantao'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */


if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

echo "<script language=\"JavaScript\" src=\"../includes/prototype.js\"></script>";
echo "<script language=\"JavaScript\" src=\"./js/rehuf.js\"></script>";

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
/* fim montando cabeçalho */

switch($_REQUEST['vis']) {
	
	case 'edicao':
		if($_REQUEST['setid']) {
			$sql = "SELECT * FROM rehuf.setorplantao WHERE setid='".$_REQUEST['setid']."'";
			$setor = $db->pegaLinha($sql);
			$setid  = $setor['carid'];
			$setnome  = $setor['setnome'];
			$requisicao = "atualizarSetoresPlantao";
			$sql = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM rehuf.setorhospitalplantao s 
					LEFT JOIN entidade.entidade ent ON ent.entid=s.entid 
			 		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
					WHERE setid='".$_REQUEST['setid']."' AND fen.funid='".HOSPITALUNIV."' GROUP BY ent.entid, ent.entnome, ena.entsig ORDER BY ena.entsig";
			
			$entid = $db->carregar($sql);
		} else {
			$requisicao = "inserirSetoresPlantao";
		}
?>
		<form method='post' onSubmit="selectAllOptions( document.getElementById('entid'));return true;">
		<input type='hidden' name='carid' value='<? echo $carid; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Cadastro de setores</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Setor:</td>
			<td><? echo campo_texto("setnome", "N", "S", "Nome do setor", 50, 240, "", "", '', '', 0, ""); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Hospitais:</td>
			<td>
			<?
			$sqlHospitais = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM entidade.entidade ent 
			 				 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
							 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
							 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
							 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) ORDER BY ena.entsig";
			combo_popup( "entid", $sqlHospitais, "Hospitais", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=plantao/cadastrosetor&acao=A';"></td>
		</tr>
		</table>
		</form>
<?		
		break;	

	default:
		$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=plantao/cadastrosetor&acao=A&vis=edicao&setid='||setid||'\';\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=plantao/cadastrosetor&acao=A&requisicao=excluirSetoresPlantao&setid='||setid||'\', \'Deseja realmente excluir este cargo?\');\"></center>', setnome FROM rehuf.setorplantao WHERE setstatus='A' ORDER BY setnome";
		$cabecalho = array("", "Setor");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<div style="float: left;">
				<input type='button' onclick="window.location='?modulo=plantao/cadastrosetor&acao=A&vis=edicao'" value='Inserir'>
				</div>
			</td>
		</tr>
		</table>
<?
}
?>