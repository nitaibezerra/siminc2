<?php
if (!$_GET[ardid])
	die("<script>window.close();</script>");

$ardid = $_POST[ardid] ? $_POST[ardid] : $_GET[ardid];	
$indid = $_POST[indid] ? $_POST[indid] : $_GET[indid];

if ($ardid && $_POST[inddsc] && $_POST[indcod] && $_POST[indqtdporescola]):
/*
 indicador.indtipo
 2    => Municipal 
 1    => Estadual 
 0    => Ambos
 null => Indefinido
*/
	
	if ($indid):
		$sql = sprintf("UPDATE
							cte.indicador
						SET
							indcod  		= %d,
							inddsc  		= '%s',
							indqtdporescola = '%s'									
						WHERE
							indid = %d",$_POST[indcod], $_POST[inddsc], $_POST[indqtdporescola], $indid);	
		$db->executar($sql);
		
		# Deletar Registros cte.indicadorunidademedida
		$sql_1 = sprintf("DELETE FROM
							cte.indicadorunidademedida
						  WHERE
						  	indid = %d",$indid);
		$db->executar($sql_1);
		
		# Deletar Registros cte.diretrizindicador
		$sql_2 = sprintf("DELETE FROM
							cte.diretrizindicador
						  WHERE
						  	indid = %d",$indid);
		$db->executar($sql_2);
		
		unset($sql, $sql_1, $sql_2);		
	else:
		$sql = sprintf("INSERT INTO 
							cte.indicador (
							ardid,
							inddsc,
							indcod,
							indtipo,
							indqtdporescola
						) VALUES (
							%d,
							'%s',
							%d,
							'%d',
							'%s'
						) returning indid",$ardid, $_POST[inddsc], $_POST[indcod], 2, $_POST[indqtdporescola]);
		$indid = $db->pegaUm($sql);
	endif;	
	
	if ($indid):
		if (is_array($_POST[undid])):
			foreach ($_POST[undid] AS $undid):
				$sql_3 .= sprintf("INSERT INTO
									cte.indicadorunidademedida (
									indid,
									undid
								  ) VALUES (
									%d,
									%d
								  );", $indid, $undid);
			endforeach;
			$db->executar($sql_3);
		endif;
		
		if (is_array($_POST[dinpeso])):
			foreach ($_POST[dinpeso] AS $dirid => $dinpeso):
				$sql_2 .= sprintf("INSERT INTO
									cte.diretrizindicador (
									indid,
									dirid,
									dinpeso
								  ) VALUES (
									%d,
									%d,
									%d
								  );", $indid, $dirid, $dinpeso);
			endforeach;	
		    $db->executar($sql_2);				
		endif;
	endif;
	$db->commit();
	die ("<script>
			alert('Operação executada com sucesso!');
			window.location.replace(window.location);
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");   
endif;

if ($indid):
	$sql = sprintf("SELECT
						dimdsc,
						arddsc,
						indcod,
						inddsc,
						CASE
							WHEN indqtdporescola = 'f' THEN 'FALSE'
							WHEN indqtdporescola = 't' THEN 'TRUE'
						END as indqtdporescola
					FROM
						cte.dimensao d,
						cte.areadimensao ad,
						cte.indicador ind						
					WHERE
						ind.ardid = ad.ardid AND
						ad.dimid = d.dimid AND
						ad.ardid = %d AND
						ind.indid = %d", $ardid, $indid);
else:
	$sql = "SELECT
				dimdsc,
				arddsc,
				MAX(indcod) as indcod			
			FROM
				cte.areadimensao a JOIN cte.dimensao d ON a.dimid = d.dimid
				LEFT JOIN cte.indicador ind ON ind.ardid = a.ardid
			WHERE
				a.ardid = {$ardid}
			GROUP BY
			 	dimdsc,
			 	arddsc";	
endif;
$dados = $db->carregar($sql);

foreach ($dados[0] as $k => $val) {
	${$k} = $k == 'indcod' ?  $val+1 : $val;				
}
?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
function montaDiretriz(sql) {
	var d = document;
	var f = d.formInd;
	var loc1, loc2, elemento = '';
	
	for (i=0; i < f.elements.length; i++) {
		if (f.elements[i].name.search(/dinpeso/) >= 0 && f.elements[i].checked) {
			loc1 = Number(f.elements[i].name.indexOf("["));
			loc2 = Number(f.elements[i].name.indexOf("]"));
			elemento += f.elements[i].name.substr((loc1+1),(loc2-(loc1+1)))+'|'+f.elements[i].value+';';
		}		
	}
	w = window.open('cte.php?modulo=principal/configuracao/popupDiretriz&acao=A&selected='+elemento,'Janela_2','scrollbars=yes,location=no,toolbar=no,menubar=no,width=450,height=400,top=100,left=400'); w.focus(); 
}

function manipularLista(values) {

	d = document;
	var tRows='';
	var item = ''//, asdf=''; 
	
	table = d.getElementById('listaDiretriz');
	
	inputPeso = d.getElementById('mopvalor');
	inputPeso = inputPeso.value.split(';');
	//table = d.getElementsByTagName('TABLE')[2];			
    //rowTot = table.rows.length;
    //alert(values);
    var linhas =  values.split(" && ");

	if (linhas.length < 2 ) {
		tRows = "<table width=\"99%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\"><tr><td align=\"center\" style=\"color:#cc0000;\">No foram encontrados Registros.</td></tr></table>";
		table.innerHTML = '';
		table.innerHTML = tRows;
		return true;
	}
    
    /*
    Cabeçalho
    */
	tRows += "<table width=\"99%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\">";
	tRows += "<thead><tr>\n";
	tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">&nbsp</td>\n";
	tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"><strong>Diretriz Vinculada</strong></td>\n";
	//tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"><strong>Peso de cada diretriz (1 a 5)</strong></td>\n";

	tRows += "	<td class=\"title\" valign=\"top\" style=\"border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\"><strong>Peso de cada diretriz <br/>&nbsp;&nbsp;";
	for (a=0; a < (inputPeso.length -1); a++) {
		tRows += inputPeso[a]+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	}
	tRows += "</strong></td>\n";	

	tRows += "</tr></thead>\n";

	for (i=0; i < (linhas.length-1); i++) {
	
		item = linhas[i].split(" || ")
		if (!item[2])
			item[2]=1;
			
		tRows += "<tbody><tr bgcolor='' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor=''\">\n";
		tRows += "	<td title=\"&nbsp\"><img src='/imagens/excluir.gif' title='Excluir a diretriz' onclick=\"this.parentNode.parentNode.parentNode.parentNode.deleteRow(this.parentNode.parentNode.rowIndex);\" /></td>\n";
		tRows += "	<td title=\"<strong>Diretriz Vinculada</strong>\">"+ item[1] +"</td>\n";
		
		tRows += "	<td title=\"<strong>Peso de cada diretriz</strong>\" nowrap>";
		
		for (a=0; a < (inputPeso.length -1); a++) {
            //asdf += item[2]+" == "+inputPeso[a]+"\n";
			if (item[2] == inputPeso[a])
				tRows += "<input type=\"radio\" checked=\"checked\" value=\""+ inputPeso[a] +"\" name=\"dinpeso["+ item[0] +"]\"/>\n";
			else
				tRows += "<input type=\"radio\" value=\""+ inputPeso[a] +"\" name=\"dinpeso["+ item[0] +"]\"/>\n";
				
		}
		
		//asdf='';
		tRows += "	</td>\n";

		//tRows += "	<td title=\"<strong>Peso de cada diretriz</strong>\"><input name=\"dinpeso["+ item[0] +"]\" type=\"text\" value=\""+ item[2] +"\" size=\"10\" class=\"normal\" maxlength=\"1\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"javascript:return  validaPeso(this)\"> <img src=\'/imagens/obrig.gif\'></td>\n";
		tRows += "</tr></tbody>\n";
	} 
	tRows += "</table>";
	table.innerHTML="";

	table.innerHTML = tRows;
	return true;

}

function validaPeso (obj) {
	if (!(obj.value >= 1 && obj.value <= 5) && obj.value != '') {
		obj.value = '1';
		obj.select();
		alert('Somente valores entre 1 e 5!')
	}	
}		

function validar() {
	d = document.formInd;
	
	for (i=0; i<d.length; i++) {
		if (d.elements[i].name == 'inddsc' && d.elements[i].value == '') {
			d.elements[i].focus();
			alert('O campo Indicador é obrigatório!');
			return false;
		}else if (d.elements[i].name == 'indcod' && d.elements[i].value == ''){
			d.elements[i].focus();
			alert('O campo Ordem é obrigatório!');
			return false;
		}else if (d.elements[i].name == 'undid[]' && d.elements[i][0].value == ''){
			d.elements[i].focus();
			alert('O campo Unidade de medida é obrigatório!');
			return false;
		}else if (d.elements[i].name.search(/dinpeso/) >= 0 && !(d.elements[i].value >= 1 && d.elements[i].value <= 5)){
			d.elements[i].focus();
			alert('O campo Peso da Diretriz é obrigatório!\nSomente valores entre 1 e 5');
			return false;
		}    
	}
	selectAllOptions( document.getElementById( 'undid' ));
	d.submit();
}

/*
	function manipularLista(obj) {
		d = document;
		var tRows=''; 
		select = d.getElementById( obj );
		table = select.parentNode.parentNode.parentNode.getElementsByTagName('TABLE')[0];
		
		rowTot = table.rows.length;
		
		if (select.options.length < 1 ) {
			tRows = "<tr><td align=\"center\" style=\"color:#cc0000;\">No foram encontrados Registros.</td></tr>";
			table.innerHTML = '';
			table.innerHTML = tRows;
			return true;
		}
		tRows += "<thead><tr>\n";
		tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\">&nbsp</td>\n";
		tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"><strong>Diretriz Vinculada</strong></td>\n";
		tRows += "	<td valign=\"top\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\"><strong>Peso de cada diretriz</strong></td>\n";
		tRows += "</tr></thead>\n";
		
		for (i=0; i < select.options.length; i++) {
			tRows += "<tbody><tr bgcolor='' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor=''\">\n";
			tRows += "	<td title=\"&nbsp\"><img src='/imagens/excluir.gif' title='Excluir a diretriz' onclick=\"this.parentNode.parentNode.style.display = 'none';\" /></td>\n";
			tRows += "	<td title=\"<strong>Diretriz Vinculada</strong>\">"+ select.options[i].innerHTML +"</td>\n";
			tRows += "	<td title=\"<strong>Peso de cada diretriz</strong>\"><input name=\"dinpeso["+ select.options[i].value +"][]\" type=\"text\" size=\"10\"></td>\n";
			tRows += "</tr></tbody>\n";
		} 
		table.innerHTML = '';

		table.innerHTML = tRows;
		return true;

			}
*/			
</script>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formInd">
<div id="sp_acoes">
<?php
monta_titulo( 'Manutenção do Indicador', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Dimensão:</td>
		<td><?= campo_texto( 'dimdsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right">Área:</td>
		<td><?= campo_texto( 'arddsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Indicador:</td>
		<td><?= campo_texto( 'inddsc', 'S', 'S', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Ordem:</td>
		<td><?= campo_texto( 'indcod', 'S', 'S', '', 10, 75, '', '' ); ?></td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right" nowrap="nowrap">Unidade de Medida:</td>
		<td>
		<?php 
		//setando as variaveis para a combo_popup()
			$sqlUniMed = "SELECT
							undid as codigo,
							unddsc as descricao
						  FROM
						  	 cte.unidademedida
						  WHERE
						  	undtipo = 'M'
						  ORDER BY
						  	unddsc ASC	
						  ";
		
			$nome = 'undid';
			if($indid){
				$sql = sprintf("SELECT
									iud.undid as codigo,
									unddsc as descricao
								FROM
									cte.unidademedida ud,
									cte.indicadorunidademedida iud
								WHERE
									ud.undid = iud.undid AND
									iud.indid = %d", $indid);

				$$nome = $db->carregar( $sql ); 
			}else{
				$$nome = array();
			}
						
			combo_popup( $nome, $sqlUniMed, 'Selecione a(s) Unidade(s) de Medida(s)', '360x460' );	
			
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Quantitativo por Escola:</td>
		<td>
		<?php
			if (!$indqtdporescola)
				$indqtdporescola = 'FALSE';

			$itens = array(array("codigo" => "TRUE",
							     "descricao" => "Sim"),
						   array("codigo" => "FALSE",
						   		 "descricao" => "Não"));
			
			$db->monta_combo( "indqtdporescola", $itens, 'S', '', '', '', '', '', 'S' );  
	    ?>
	   </td>
	</tr>
<!-- 
	<tr>
		<td class="SubTituloDireita" align="right">Diretriz:</td>
		<td>
		<?php 
/*		
		//setando as variaveis para a combo_popup()
				
			$sqlDiretriz = "SELECT
							dirid as codigo,
							dirdsc as descricao
						  FROM
						  	 cte.diretriz
						  ORDER BY
						  	dirdsc ASC	
						  ";
			
			$nome = 'diretrizindicador[' . $sisid . ']';
			
			#$evento = 'pre_alterar_acao';
			if($evento == 'pre_alterar_acao'){
				
				$sql = "SELECT	
							dirid as codigo,
							dirdsc as descricao 
						FROM 
							cte.diretriz
						ORDER BY
							dirdsc ASC
							";
				$$nome = $db->carregar( $sql ); 
			}else{
				$$nome = array();
			}
						
			combo_popup( $nome, $sqlDiretriz, 'Selecione a(s) Unidade(s) de Medida(s)', '360x460', 0,
						array(), '', 'S', false, false, 10, 400 , "manipularLista('diretrizindicador[]')", "manipularLista('diretrizindicador[]')")
*/			
		?>
		</td>
	</tr>
-->			
	<tr>
		<td colspan="2">
		<?php 		
		$sqlMontPeso = sprintf("SELECT
									CASE 
										WHEN mopvalor = dinpeso THEN '<input name=\"dinpeso[' || dirid || ']\" type=\"radio\" value=\"' || mopvalor || '\" checked=\"checked\">'
										ELSE '<input name=\"dinpeso['  || dirid || ']\" type=\"radio\" value=\"' || mopvalor || '\">'
									END as radio,
									mopvalor,
									dirid	
								FROM
									cte.monitoramentopeso CROSS JOIN cte.diretrizindicador
								WHERE
									indaplicacao = 'I' AND
									indid = %d
								ORDER BY
									mopvalor
								", $indid);
		//echo $sqlMontPeso;
		$radio = $db->carregar($sqlMontPeso);		

		if (!$radio):
			unset($sqlMontPeso);
			$sqlMontPeso = sprintf("SELECT
										mopvalor	
									FROM
										cte.monitoramentopeso
									WHERE
										indaplicacao = 'I'
									ORDER BY
										mopvalor
									");
			
			$valor = $db->carregar($sqlMontPeso);
			foreach ($valor as $val)
				$mopvalor .= $val[mopvalor].";";
		endif;
	
		$sqlDiretriz = sprintf("SELECT
									'<img src=\'/imagens/excluir.gif\' title=\'Excluir a diretriz\' onclick=\"this.parentNode.parentNode.parentNode.parentNode.deleteRow(this.parentNode.parentNode.rowIndex); \" />',	
										
									dirdsc as descricao,
									di.dirid
									--'<input maxlength=\"1\" name=\"dinpeso[' || d.dirid || ']\" value=\"' || di.dinpeso || '\" type=\"text\" size=\"10\" value=\"\" class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"javascript:return  validaPeso(this)\" > <img src=\'/imagens/obrig.gif\'>' as campo
								FROM 
									cte.diretriz d,
									cte.diretrizindicador di
								WHERE
									d.dirid = di.dirid AND
									di.indid = %d
								ORDER BY
									dirdsc ASC", $indid);
		$dadList = $db->carregar($sqlDiretriz);  
		
		if (is_array($dadList)):
			for ($i=0; $i < count($dadList); $i++):
				for ($a=0; $a < count($radio); $a++):
					if (end($dadList[$i]) == end($radio[$a])) {
						$unionRadio .= $radio[$a][radio];
						$mopvalor   .= $i == 0 ? $radio[$a][mopvalor].";" : '';
					}	
				endfor;
				array_pop($dadList[$i]);
				array_push($dadList[$i], $unionRadio);
				unset($unionRadio);
			endfor;
			$sqlDiretriz = $dadList;
			unset($dadList,$radio);
		endif;
		
		$cabecalho = array("&nbsp","<strong>Diretriz Vinculada</strong>", "<strong>Peso de cada diretriz<BR>&nbsp;&nbsp;".str_replace(";","&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",$mopvalor)."</strong>");		
		
		echo "<div id='listaDiretriz'>";
		$db->monta_lista_simples($sqlDiretriz, $cabecalho, 100, 20, '', '100%' ,'' );
		echo "</div>";
		?>
		</td>
	</tr>									
	<tr bgcolor="#C0C0C0">
		<td><input type='button' class='botao' value='Diretriz' id='btDiretriz' name='btDiretriz' onclick="montaDiretriz()" /></td>
		<?
			if($aedid){
				echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"validar()\" />");
			}else{
				echo("<td><input type='button' class='botao' name='consultar' value='Salvar' onclick=\"validar()\" />");
			}
		?>
			<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();' />
			<input type='hidden' value='<?php echo $indid;?>' id='indid' name='indid'/>
			<input type='hidden' value='<?php echo $ardid;?>' id='ardid' name='ardid'/>
			<input type='hidden' value='<?php echo $mopvalor; ?>' id='mopvalor' name='mopvalor'>
		</td>			
	</tr>
</table>
</form>
</body>
</html>
