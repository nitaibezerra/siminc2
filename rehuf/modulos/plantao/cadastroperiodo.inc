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
		$_FUNCOESAUTORIZADAS = array('inserirPeriodosPlantao'=>true,
									 'atualizarPeriodosPlantao'=>true,
									 'excluirPeriodosPlantao'=>true);
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
		if($_REQUEST['pplid']) {
			$sql = "SELECT pplid, to_char(ppldata, 'mm') as mes, to_char(ppldata, 'YYYY') as ano FROM rehuf.periodoplantao WHERE pplid='".$_REQUEST['pplid']."'";
			$periodo = $db->pegaLinha($sql);
			$pplid  = $periodo['pplid'];
			$mes  = $periodo['mes'];
			$ano = $periodo['ano'];
			$requisicao = "atualizarPeriodosPlantao"; 
		} else {
			$requisicao = "inserirPeriodosPlantao";
		}
?>
		<script	src="/includes/calendario.js"></script>
		<script>
		function validarPeriodoPlantao() {
			
			if(document.getElementById('mes').value == '') {
				alert('Mês é obrigatório');
				return false;
			}
			
			if(document.getElementById('ano').value == '') {
				alert('Ano é obrigatório');
				return false;
			}
			<?
			if($requisicao == "atualizarPeriodosPlantao") {
			?>
			var form = document.formulario;
				
			for(var i=0;i<form.elements.length;i++) {
				if(form.elements[i].type == "text" && form.elements[i].id != "data_todos") {
					if(form.elements[i].value == "") {
						alert("Todas data devem ser preenchidas");
						form.elements[i].focus();
						return false;	
					}
				}
			}
			<?
			}
			?>
			
			return true;
			
		}
		
		function aplicarTodos(obj) {
			var tabela1 = document.getElementById('tabela_periodo').rows[5].cells[0].childNodes[1];
			for(i=1;i<tabela1.rows.length;i++) {
				var input = tabela1.rows[i].cells[document.getElementById('coluna').value].childNodes[0].childNodes[0];
				input.value=document.getElementById('data_todos').value;
			}
		}
		</script>
		<form method='post' name="formulario" onSubmit="return validarPeriodoPlantao();">
		<input type='hidden' name='carid' value='<? echo $carid; ?>'>
		<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" id="tabela_periodo">
		<tr>
			<td colspan="2" class="SubTituloCentro">Cadastro de Período</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mês:</td>
			<td>
			<?
			$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
			$db->monta_combo('mes', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'mes');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ano:</td>
			<td>
			<?
			foreach($_ANOS as $a) {
				$anoslista[] = array("codigo" => $a, "descricao" => $a);				
			}
			
			$db->monta_combo('ano', $anoslista, 'S', 'Selecione', '', '', '', '', 'S', 'ano');
			?>
			</td>
		</tr>
		<?
		if($requisicao == "inserirPeriodosPlantao") {
		?>
		<tr>
			<td class="SubTituloDireita">Data de preenchimento:</td>
			<td><?=campo_data('pplpreenchimentoinicial', 'N','S','','','','');?> até <?=campo_data('pplpreenchimentofinal', 'N','S','','','','');?></td>
		</tr>
		<?
		} elseif($requisicao == "atualizarPeriodosPlantao") {
		?>
		<tr>
			<td class="SubTituloCentro" colspan="2">Preenchimento</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Digite uma data:</td>
			<td>
			<input type="text" id="data_todos" name="data_todos" onblur="MouseBlur(this);this.value=mascaraglobal('##/##/####',this.value);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('##/##/####',this.value);" class="normal" maxlength="10" style="text-align: right;" size="12"> 
			aplicar esta data em todos os hospitais na coluna <? $db->monta_combo('coluna', $coluna = array(0 => array('codigo' => '2','descricao' => 'Data inicial'),1 => array('codigo' => '3','descricao' => 'Data final')), 'S', '', '', '', '', '', 'N', 'coluna'); ?> <input type="button" name="aplicar_todos" value="Aplicar" onclick="aplicarTodos(this);">
			</td>
		</tr>
		<tr>
			<td colspan="2">
			<?
			$sql = "SELECT e.entnome, ena.entsig, 
					'<center><input type=text id=pplpreenchimentoinicial_'|| e.entid ||' name=pplpreenchimentoinicial['|| e.entid ||'] value='|| to_char(p.pplpreenchimentoinicial,'dd/mm/YYYY') ||' onblur=MouseBlur(this);this.value=mascaraglobal(\"##/##/####\",this.value); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=this.value=mascaraglobal(\"##/##/####\",this.value); class=normal maxlength=10 style=text-align: right; size=12></center>' as inicio, 
					'<center><input type=text id=pplpreenchimentofinal_'|| e.entid ||' name=pplpreenchimentofinal['|| e.entid ||'] value='|| to_char(p.pplpreenchimentofinal, 'dd/mm/YYYY') ||' onblur=MouseBlur(this);this.value=mascaraglobal(\"##/##/####\",this.value); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=this.value=mascaraglobal(\"##/##/####\",this.value); class=normal maxlength=10 style=text-align: right; size=12></center>' as final 
					FROM rehuf.periodoplantaodata p 
					LEFT JOIN entidade.entidade e ON e.entid=p.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
					LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
					LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
					WHERE p.pplid='".$pplid."' AND fen.funid = '".HOSPITALUNIV."' ORDER BY ena.entsig, e.entnome";
			
			$cabecalho = array("Hospital","Unidade","Data inicial","Data final");
			
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
			
			?>
			</td>
		</tr>
		<?
		}
		?>
		<tr>
			<td class="SubTituloDireita" colspan="2"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=plantao/cadastroperiodo&acao=A';"></td>
		</tr>
		</table>
		</form>
<?		
		break;

	default:
		$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=plantao/cadastroperiodo&acao=A&vis=edicao&pplid='||pplid||'\';\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=plantao/cadastroperiodo&acao=A&requisicao=excluirPeriodosPlantao&pplid='||pplid||'\', \'Deseja realmente excluir este período?\');\"></center>', '<center>'||to_char(ppldata, 'mm/YYYY')||'</center>' as ppldata FROM rehuf.periodoplantao WHERE pplstatus='A' ORDER BY ppldata";
		$cabecalho = array("", "Período");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<div style="float: left;">
				<input type='button' onclick="window.location='?modulo=plantao/cadastroperiodo&acao=A&vis=edicao'" value='Inserir'>
				</div>
			</td>
		</tr>
		</table>
<?
}
?>
