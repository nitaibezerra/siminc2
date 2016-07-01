<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" >
		<form action="" method="post" name="formDir">
		<table border="1" width="100%" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td width="1%"><input type="checkbox" value="" onclick="javascript:allSelected(this,'true')" /></td>
				<td style="font-weight:bold;">Vincule a(s) diretriz(es)</td>
			</tr>
			<tr bgcolor="#dcdcdc">
				<td width="1%">&nbsp</td>
				<td style="text-align: center; color:blue;">Descrição</td>
			</tr>
		<?php
		$sql = "SELECT
					'<td><input name=\"itens[]\" type=\"checkbox\"  value=\"' || dirid || '\" onclick=\"javascript:enviarCheck(this, \'' || dirid || '\', \'' || dirdsc || '\')\" /></td><td><div id=\'' || dirid || '\'>' || dirdsc || '</div></td>'
				FROM
					cte.diretriz
				ORDER BY
					dirdsc ASC				
				--LIMIT 100; 
				";
		$dados = $db->carregar($sql);
		
		for ($i=0; $i < count($dados); $i++):
			if (is_array($dados[$i])):
				foreach ($dados[$i] as $val):
					is_int($a/2) ? $color = '#f4f4f4' : $color = '#e0e0e0';	
					echo "<tr bgcolor='$color'>$val</tr>";
				 	$a++;
				endforeach;
			endif;
		endfor;
		?>	
		<tr bgcolor="#cdcdcd">
			<td width="1%"><input type="checkbox" value="" onclick="javascript:allSelected(this,'true')" /></td>
			<td><strong>Vincule a(s) diretriz(es)</strong>&nbsp<input type="button" value="ok" onclick="window.close();" /></td>
		</tr>
		</table>	
		</form>
		<script type="text/javascript">
		var d = document;
		var fdir = d.formDir;
		var tFdir = fdir.elements.length;
		
		
		function enviarCheck () {
		var elemento = '';
			for (i=0; i < tFdir; i++) {
				if (fdir.elements[i].type == 'checkbox' && fdir.elements[i].name.search(/itens/) >= 0 && fdir.elements[i].checked === true) {
					if (fdir.elements[i].value.search(";") < 0)
						fdir.elements[i].value += ';';
					//alert(fdir.elements[i].value);						
					idVal = fdir.elements[i].value.split(";");	
					elemento += idVal[0]+' || '+d.getElementById(idVal[0]).innerHTML+' || '+idVal[1]+' && \n';
				}			
			}

			window.opener.manipularLista( elemento );
			return true;
		}
		
		function allSelected (obj,comand) {
			comand ? param=false : param=true;
		
			for (i=0; i < tFdir; i++) {
				if (fdir.elements[i].type == 'checkbox') {
					fdir.elements[i].checked = comand;			
				}	
		
			}
			
			enviarCheck();
			
			obj.onclick=function () {allSelected(this, param)};
			return true;
		}
		
		function selecionados (selectCheck) {
			var item = selectCheck.split(";");
			//alert(item);
			for (i=0; i < tFdir; i++) {										
				if (fdir.elements[i].type == 'checkbox') {
					for (a=0; a < (item.length -1); a++) {
						idVal = item[a].split("|");
						if (fdir.elements[i].value === idVal[0]) {
						    fdir.elements[i].value += ';'+idVal[1]
							fdir.elements[i].checked = true;
							//alert(fdir.elements[i].value);
							//alert(fdir.elements[i].value);
						}	
					}			
				}	
		
			}	
		}
		
		selecionados('<?php echo $_GET[selected]; ?>')

		</script>				
	</body>
</html>	
