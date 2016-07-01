<?php
	if($_REQUEST['relatorio'] == 'relatorio'){ 
		
		extract($_POST);
		
		if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
			$where[0] = " and ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
		}
		
		if($municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
			$where[1] = " and ende.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
		}
		
		if($funid == 'T'){
			$where[2] = " and funid in (95,96) ";		
		}elseif($funid == 'E'){
			$where[2] = " and funid = 95 ";
		}elseif($funid == 'M'){
			$where[2] = " and funid = 96";
		}
		
		$sql = "
			Select	trim(e.entnome),
					Case When funid = 95 Then 'Estadual' Else 'Municípal' end as funid,
					ende.estuf,
					m.mundescricao,
		            e.entnumcpfcnpj,
		            lower(e.entemail) as entemail,
		            '('||e.entnumdddresidencial||')'||entnumresidencial As TelefoneResidencial,
		            '('||e.entnumdddcomercial||')'||entnumcomercial As TelefoneComercial,
		            '('||e.entnumdddcelular||')'||entnumcelular As TelefoneCelular
			From entidade.entidade e
			inner join entidade.funcaoentidade fe using(entid)
			inner join entidade.endereco ende on e.entid = ende.entid
			inner join territorios.municipio m using(muncod)
			Where e.entstatus = 'A' and fe.fuestatus = 'A'
			  /*and e.entid     = (Select Max(e.entid)
								   From entidade.entidade e
							 inner join entidade.funcaoentidade fe using(entid)
							 inner join entidade.endereco ende on e.entid = ende.entid
							 inner join territorios.municipio m using(muncod)
								  Where e.entstatus = 'A' and fe.fuestatus = 'A'
								--Estado
								".$where[0]."
								--Municipal
								".$where[1]."
								--Unidade
								".$where[2].")*/
			--Estado
			".$where[0]."
			--Municipal
			".$where[1]."
			--Unidade
			".$where[2]."
			order by 3,4,1,2 
		";

		$cabecalho = array("Nome", "Coordenador","UF", "Município", "CNPJ/CPF", "E-mail", "Telefone residencial", "Telefone comercial", "Telefone celular");
			
		if($_REQUEST['visualizar'] == 'html'){
			echo '
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td colspan="5" class="SubTituloCentro">Relatório : Relação Coordenadores Mais Educação</td>
					</tr>
					<tr>
						<td>
			';
			//$db->monta_lista_simples($sql, $cabecalho, 100, 10, 'N', '100%', 'N', true, true, true, true );
			//($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param=Array()) {
			$db->monta_lista($sql, $cabecalho, 1, 10, 'N', 'rigth', 'N');
			echo '
						</td>
					</tr>
				</table>
			';
		}
		elseif($_REQUEST['visualizar'] == 'xls'){
			$dadosExcel = $db->carregar($sql);
			if(empty($dadosExcel)){
				echo '
					<script>
						alert("Nenhum registro encontrado!");
						window.close(); 
					</script>
				';
				exit;
			}
			else{
				ob_clean();
				header('content-type: text/html; charset=ISO-8859-1');
				$db->sql_to_excel($dadosExcel, 'Relacao_Coordenadores_Mais_Educação', $cabecalho);
			}
		}
		exit;
	}

	
	include APPRAIZ . 'includes/cabecalho.inc';
	echo "<br>";
	$titulo_modulo = "Relatório : Relação Coordenadores Mais Educação";
	monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	//jQuery.noConflict();

	function exibirRelatorio(p){
		if(document.getElementById('relatorio').value!='') {
			var formulario = document.formulario;
			selectAllOptions( formulario.municipio );
			prepara_formulario();
			if(p=='0') {
				document.getElementById('visualizar').value='html';
				// submete formulario
				formulario.target = 'relacaocoordenacao';
				var janela = window.open('', 'relacaocoordenacao', 'width=960,height=450,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				formulario.submit();
				janela.focus();
			} else {
				document.getElementById('visualizar').value='xls';
				formulario.submit();
			}

		} else {
			alert("Selecione um relatório");
			return false;
		}
	}

	function onOffBloco( bloco ){
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
</script>

<form action="" method="post" name="formulario" id="filtro">
	<input type="hidden" id="relatorio" name="relatorio" value="relatorio"/>
	<input type="hidden" name="visualizar" id="visualizar" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Coordenadores da Esfera:</td>
			<td>
				<input type="radio" id="funid" name="funid" value="E" /> Estadual
				<input type="radio" id="funid" name="funid" value="M"/> Municípal
				<input type="radio" id="funid" name="funid" value="T" checked="checked"/> Todas
			</td>
		</tr>	
		<tr>
			<?php
				#UF - Unidade Federativa
				$sql = "
					Select	estuf AS codigo,
							estdescricao AS descricao
					From territorios.estado
					Order by estdescricao
				";
				$stSqlCarregados = "";
				mostrarComboPopup('UF - Unidade Federativa', 'estuf',  $sql, $stSqlCarregados, 'Selecione a(s) UF(s)');

				#Município				
				mostrarComboPopupMunicipios('municipio', 'Municípios', 'S', 'true', 'true', '400px', 'true');
			?>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" name="filtrar" value="Visualizar HTML" style="cursor: pointer;" onclick="exibirRelatorio(0);"/> 
				<input type="button" name="filtrar" value="Visualizar XLS"  style="cursor: pointer;" onclick="exibirRelatorio(1);"/>			
			</td>
		</tr>		
	</table>
</form>
