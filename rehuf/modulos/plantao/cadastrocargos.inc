<?php
/*
 * TELA LISTA DE TABELAS (lista de tabelas para serem preenchidas pelos hospitais)
 * As tabelas são criadas pelo administrador. Nesta tela é criado o documento
 * referente ao workflow. Esta passa por validação das permissões.
 * Parâmetro obrigatório : $_REQUEST['entid']
 */

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirCargosPlantao'=>true,
									 'atualizarCargosPlantao'=>true,
									 'excluirCargosPlantao'=>true);
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
		if($_REQUEST['carid']) {
			$sql = "SELECT * FROM rehuf.cargoplantao WHERE carid='".$_REQUEST['carid']."'";
			$cargo = $db->pegaLinha($sql);
			$carid  = $cargo['carid'];
			$carnome  = $cargo['carnome'];
			$carnivel = $cargo['carnivel'];
			$sql = "SELECT ent.entid as codigo, ent.entnome||' - '||coalesce(ena.entsig, '') as descricao FROM rehuf.cargohospitalplantao s 
					LEFT JOIN entidade.entidade ent ON ent.entid=s.entid 
			 		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
					WHERE carid='".$_REQUEST['carid']."' ORDER BY ena.entsig";
			$entid = $db->carregar($sql);
			$requisicao = "atualizarCargosPlantao"; 
		} else {
			$requisicao = "inserirCargosPlantao";
		}
?>
		<form method='post' onSubmit="selectAllOptions( document.getElementById('entid'));return true;">
		<input type='hidden' name='carid' value='<? echo $carid; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Cadastro de cargos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cargo:</td>
			<td><? echo campo_texto("carnome", "N", "S", "Nome do cargo", 50, 240, "", "", '', '', 0, ""); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nível:</td>
			<td><input type="radio" name="carnivel" value="S" <? echo (($carnivel=='S' || !$carnivel)?"checked":""); ?>> Superior <input type="radio" name="carnivel" value="M" <? echo (($carnivel=='M')?"checked":""); ?>> Médio</td>
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
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=plantao/cadastrocargos&acao=A';"></td>
		</tr>
		</table>
		</form>
<?		
		break;

	default:
		$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=plantao/cadastrocargos&acao=A&vis=edicao&carid='||carid||'\';\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=plantao/cadastrocargos&acao=A&requisicao=excluirCargosPlantao&carid='||carid||'\', \'Deseja realmente excluir este cargo?\');\"></center>', carnome, CASE WHEN carnivel='S' THEN 'Superior' ELSE 'Médio' END as nivel FROM rehuf.cargoplantao WHERE carstatus='A' ORDER BY carnome";
		$cabecalho = array("", "Cargo", "Nível");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<div style="float: left;">
				<input type='button' onclick="window.location='?modulo=plantao/cadastrocargos&acao=A&vis=edicao'" value='Inserir'>
				</div>
			</td>
		</tr>
		</table>
<?
}
?>
