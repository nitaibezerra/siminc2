<?php
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('inserirsubitemindicador'=>true,
									 'atualizarsubitemindicador'=>true,
									 'carregarindicadorvalor'=>true
									 );
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

if($_REQUEST['sinid']) {
	$subitem = $db->pegaLinha("SELECT * FROM rehuf.indicadorsubitem WHERE sinid = '". $_REQUEST['sinid'] ."'");
	if($subitem) {
		$sindsc = $subitem['sindsc']; 
		$sinformula = $subitem['sinformula'];
		if($subitem['sincores']) {
			$sincores = explode(";",$subitem['sincores']);
		}
		$requisicao = 'atualizarsubitemindicador';
	} else {
		echo "<script>
				alert('Subitem não encontrado');
				window.close();
			  </script>";
		exit;
	}
} else {
	$sinformula = "CASE WHEN [Denominador Fórmula]= 0 THEN 0 ELSE [Fórmula] END";
	$requisicao = 'inserirsubitemindicador';
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script>
function validarFormularioSubItem(frm) {
	if(frm.elements['sindsc'].value == "") {
		alert("'Descrição' é obrigatório");
		return false;
	}
	if(frm.elements['sinformula'].value == "") {
		alert("'Fórmula' é obrigatório");
		return false;
	}
	// Validando a última linha da faixa (não é validada)
	var tabela = document.getElementById('faixa');
	if(tabela.rows.length-2!=0) {
		if(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].value && document.formulario.elements['faixa['+(tabela.rows.length-3)+'][cor]'].value) {
			if(parseInt(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].value) <= parseInt(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][min]'].value)) {
				alert("Valor máximo deve ser maior que o valor mínimo");
				return false;
			}
		} else {
			alert("'Valor máximo' e 'Cor' são obrigatórios");
			return false;
		}
	}
	
	return true;
}

function simular() {
	if(document.formulario.elements['entid'].value == "") {
		alert('Selecione um hospital');
		return false;
	}
	if(document.formulario.elements['ano'].value == "") {
		alert('Selecione um ano');
		return false;
	}
	document.getElementById('indicadorvalor').innerHTML = "Carregando...";
	ajaxatualizar('requisicao=carregarindicadorvalor&entid='+document.formulario.elements['entid'].value+'&ano='+document.formulario.elements['ano'].value,'indicadorvalor');
}

function inserircor() {
	var tabela = document.getElementById('faixa');
	// se for a primeira linha
	if(tabela.rows.length-2==0) {
		var valormin = 0;
	} else {
		if(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].value && document.formulario.elements['faixa['+(tabela.rows.length-3)+'][cor]'].value) {
			if(parseInt(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].value) > parseInt(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][min]'].value)) {
				document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].className="disabled";
				document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].readOnly="readonly";
				tabela.rows[tabela.rows.length-2].cells[3].innerHTML="&nbsp;";
				var valormin = parseInt(document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].value)+1;
			} else {
				alert("Valor máximo deve ser maior que o valor mínimo");
				return false;
			}
		} else {
			alert("'Valor máximo' e 'Cor' são obrigatórios");
			return false;
		}
	}
	var linha = tabela.insertRow(tabela.rows.length-1);
	var coluna0 = linha.insertCell(0);
	coluna0.innerHTML = "<select name='faixa["+(tabela.rows.length-3)+"][cor]' class='CampoEstilo' onchange='mostrarcor(this);' style='width: auto'><option value=''>Selecione</option><option value='#00FF00'>Verde</option><option value='#FFFF00'>Amarelo</option><option value='#FF0000'>Vermelho</option></select>";
	var coluna1 = linha.insertCell(1);
	coluna1.innerHTML = "<input type='text' name='faixa["+(tabela.rows.length-3)+"][min]' class='disabled' size='10' readonly='readonly' value='"+valormin+"'>";
	var coluna2 = linha.insertCell(2);
	coluna2.innerHTML = "<input type='text' name='faixa["+(tabela.rows.length-3)+"][max]' class='normal' size='10' onKeyUp=\"this.value=mascaraglobal('##########',this.value);\">";
	var coluna3 = linha.insertCell(3);
	coluna3.align = "right";
	coluna3.innerHTML = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerfaixa(this);\">";

}
function mostrarcor(obj) {
	var corcell = obj.parentNode;
	corcell.style.backgroundColor = obj.value;
}

function removerfaixa(obj) {
	var nlinha = obj.parentNode.parentNode.rowIndex;
	var tabela = document.getElementById('faixa');
	tabela.deleteRow(nlinha);
	if(tabela.rows.length-2>0) {
		document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].className="normal";
		document.formulario.elements['faixa['+(tabela.rows.length-3)+'][max]'].readOnly="";
		tabela.rows[tabela.rows.length-2].cells[3].innerHTML = "<img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerfaixa(this);\">";		
	}
}
</script>

<form name='formulario' action='?modulo=indicadores/gerenciarsubitem&acao=A' method='post' onsubmit='return validarFormularioSubItem(this);'>
<input type='hidden' name='requisicao' value='<? echo $requisicao; ?>'>
<input type='hidden' name='indid' value='<? echo $_REQUEST['indid']; ?>'>
<?
if($_REQUEST['sinid']) {
	echo "<input type='hidden' name='sinid' value='".$_REQUEST['sinid']."'>";
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class='SubTituloCentro' colspan="2">Gerenciar subitem do indicador</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Descrição :</td>
		<td><? echo campo_texto('sindsc', "S", "S", "Descrição", 62, 200, "", "", '', '', 0, '' ); ?></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Fórmula :</td>
		<td><? echo campo_textarea( 'sinformula', 'S', 'S', '', '65', '5', '1000'); ?></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Faixa de valores :</td>
		<td>
		<table cellSpacing="1" cellPadding="3" width="100%" id="faixa">
		<tr>
		<td class='SubTituloCentro'>Cor</td>
		<td class='SubTituloCentro'>Mínimo</td>
		<td class='SubTituloCentro'>Máximo</td>
		<td class='SubTituloCentro'>&nbsp;</td>
		</tr>
		<?
		if($sincores) {
			foreach($sincores as $key => $dadoscor) {
				if($dadoscor) {
					$line = explode(",",$dadoscor);
					echo "<tr>";
					echo "<td bgcolor='".$line[2]."'><select name='faixa[".$key."][cor]' class='CampoEstilo' onchange='mostrarcor(this);' style='width: auto'>
								<option value=''>Selecione</option>
								<option value='#00FF00' ".(($line[2]=="#00FF00")?"selected":"").">Verde</option>
								<option value='#FFFF00' ".(($line[2]=="#FFFF00")?"selected":"").">Amarelo</option>
								<option value='#FF0000' ".(($line[2]=="#FF0000")?"selected":"").">Vermelho</option>
						  	  </select></td>";
					echo "<td><input type='text' name='faixa[".$key."][min]' class='disabled' size='10' readonly='readonly' value='".$line[0]."'></td>";
					echo "<td><input type='text' name='faixa[".$key."][max]' ".(($key==(count($sincores)-2))?"class='normal'":"class='disabled' readonly='readonly'")." size='10' onKeyUp=\"this.value=mascaraglobal('##########',this.value);\" value='".$line[1]."'></td>";
					echo "<td align='right'>".(($key==(count($sincores)-2))?"<img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"removerfaixa(this);\">":"&nbsp;")."</td>";
					echo "</tr>";
				}
			}
		}
		?>
		<tr>
		<td colspan="4" align="right"><img src="../imagens/gif_inclui.gif" onclick="inserircor();"></td>
		</tr>
		
		</table>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center"><input type='submit' value='Gravar'> <input type='button' value='Fechar' onclick="window.close();"></td>
	</tr>
	<tr>
		<td colspan='2'>
		<?
		$sql = "SELECT ent.entid as codigo, upper(ena.entsig) ||' - '|| ent.entnome as descricao
				FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
				LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid  
				LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid
				WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) GROUP BY ent.entid, ent.entnome, ena.entsig 
				ORDER BY ena.entsig";
		$db->monta_combo('entid', $sql, 'S', 'Selecione um hospital', '', '', '', '200', 'N', 'entid'); ?> | 
		<? 
		$ano = array(0 => array("codigo"=>"2004","descricao"=>"2004"),
					 1 => array("codigo"=>"2005","descricao"=>"2005"),
					 2 => array("codigo"=>"2006","descricao"=>"2006"),
					 3 => array("codigo"=>"2007","descricao"=>"2007"),
					 4 => array("codigo"=>"2008","descricao"=>"2008"),
					 5 => array("codigo"=>"2009","descricao"=>"2009"),
					 6 => array("codigo"=>"2010","descricao"=>"2010"));
		
		$db->monta_combo('ano', $ano, 'S', 'Selecione um ano', '', '', '', '', 'N', 'ano'); ?>
		<input type='button' name='simula' value='Simular' onclick="simular();">
		</td>
	</tr> 
	<tr>
		<td colspan='2'><div style='width:480px;height:285px;overflow:auto;vertical-align:top' id='indicadorvalor'></div></td>
	</tr>
</table>
</form>
<script>
ajaxatualizar('requisicao=carregarindicadorvalor','indicadorvalor');
</script>
