<?php

	if ($_POST['tipo_relatorio']){
		ini_set("memory_limit","256M");
		include_once ("result_relatorio_pnld.inc");
		exit;
	}
			
	include_once APPRAIZ ."includes/Agrupador.php";
	include_once APPRAIZ ."includes/cabecalho.inc";
	
	echo '<br>';
	
?>

	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	
	<script type="text/javascript">

		function gerarRelatorio(tipo){
			
			var formulario = document.formulario;
			var tipo_relatorio = tipo;
	
			selectAllOptions(formulario.editora);
	
			if(tipo_relatorio == 'html'){
				document.getElementById('tipo_relatorio').value = 'html';
			}else{
				document.getElementById('tipo_relatorio').value = 'xls';
			}
			
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			formulario.submit();
			
			janela.focus();
		}
	
		function onOffCampo(campo){
			var div_on = document.getElementById( campo + '_campo_on' );
			var div_off = document.getElementById( campo + '_campo_off' );
			var input = document.getElementById( campo + '_campo_flag' );
			if ( div_on.style.display == 'none' ){
				div_on.style.display = 'block';
				div_off.style.display = 'none';
				input.value = '1';
			}
			else{
				div_on.style.display = 'none';
				div_off.style.display = 'block';
				input.value = '0';
			}
		}
		
	</script>
	
	<?php monta_titulo('Relatório PNLD', ''); ?>
	
	<form name="formulario" id="formulario" action="" method="post">
		<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
		
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<?php
				#Filtros do relatório
				$stSql = "
					SELECT edt.edtid as codigo, 
					       edt.edtnome as descricao
					FROM livro.editora edt
					WHERE edt.edtstatus = 'A'
				";
				
				$stSqlCarregados = "";
		        
				if(is_array($_POST['editora']) && $_POST['editora'][0]){
					$stSqlCarregados = "
						SELECT edt.edtid as codigo,
						       edt.edtnome as descricao
						FROM livro.editora edt
						WHERE edt.edtstatus = 'A'
						AND   edt.edtid IN (".implode(", ", $_POST['editora']).")
					";
				}
				mostrarComboPopup( 'Editora', 'editora',  $stSql, $stSqlCarregados, 'Selecione a Editora' );
	
			?>
	
			<tr>		
				<td align="center" colspan="2">
					<input type="button" name="gerarhtml" onclick="gerarRelatorio('html');" value="Gerar Relatório" />
					<input type="button" name="gerarxls" onclick="gerarRelatorio('xls');" value="Gerar XLS" />
				</td>
			</tr>
		</table>
	</form>
	<br>

	<?php
	
	$sql = "
		SELECT 	DISTINCT rr.rreid,
				rr.rredescricao as Recursodescricao,
				arq.arqdata || ' - ' || arq.arqhora AS dataparecer,
				arq2.arqdata || ' - ' || arq2.arqhora AS datarecurso,
				arq3.arqdata || ' - ' || arq3.arqhora AS dataresposta,
				ed.edtcnpj,
				ed.edtnome
		FROM livro.resenharecurso AS rr
		INNER JOIN public.arquivo AS arq ON arq.arqid = rr.arqid
		LEFT JOIN public.arquivo AS arq2 ON arq2.arqid = rr.arqidrecurso
		LEFT JOIN public.arquivo AS arq3 ON arq3.arqid = rr.arqidresposta
		LEFT JOIN livro.livrodidatico lv ON lv.rreid = rr.rreid and lv.prsano = '".$_SESSION['exercicio']."'
		INNER JOIN livro.editora ed ON ed.edtid = lv.edtid
		ORDER BY rr.rreid
	";
		
		$tamanho	= array('10%', '95%');
		$alinhamento= array('center', 'left');
		
		$cabecalho 	= array('Id', 'Descrição do Recurso', 'Data do Parecer', 'Data do Recurso', 'Data da Resposta','cnpj','Nome Editora');
		
		$db->monta_lista ( $sql, $cabecalho, 50, 30, 'N', 'left', '', $tamanho, $alinhamento );
							
	?>

	