<?php
if(!empty($_REQUEST['tipo_relatorio'])){
	extract($_POST);
		
	if($plicod[0] && ( $plicod_campo_flag || $plicod_campo_flag == '1' )){	
		
		$where[0] = " AND plicod " . (( $plicod_campo_excludente == null || $plicod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $plicod ) . "') ";		
	}
		
	if($ptres[0] && ($ptres_campo_flag || $ptres_campo_flag == '1' )){
		$where[1] = " AND ptres " . (( $ptres_campo_excludente == null || $ptres_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ptres ) . "') ";		
	}
		
	$sql = "SELECT	
					plicod,
					sbacod,
					sbatitulo,
					plititulo,
					ptres,
					funcod||'.'||sfucod||'.'||prgcod||'.'||acacod||'.'||ptres.unicod||'.'||loccod as funcional,
					pipvalor as valor,
					plidsc,
					CASE plisituacao
						WHEN 'P' THEN 'Pendente de Aprovação'
						WHEN 'H' THEN 'Homologado'
						WHEN 'A' THEN 'Aprovado'
						WHEN 'E' THEN 'Enviado para Revisão'
						WHEN 'R' THEN 'Revisado'
						WHEN 'C' THEN 'Cadastrado no SIAFI'
						WHEN 'S' THEN 'Confirmado no SIAFI'
					END AS statusPI	
			FROM 		monitora.pi_planointerno pi
			INNER JOIN 	monitora.pi_subacao sa ON sa.sbaid = pi.sbaid
			INNER JOIN 	monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid
			INNER JOIN 	monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
			WHERE pi.plistatus = 'A' AND pliano = '2012' AND plicod IN ( SELECT TRIM(\"Plano Interno\") FROM carga.pi_26298 )
		--Código do PI
		".$where[0]."
		--Ptres
		".$where[1]."
		ORDER BY plicod";

		$cabecalho = array("Código PI", "Código Subação","Título Subação","Título PI", "Ptres", "Funcional Programática", "Valor", "Descrição PI", "Status PI");
		
		if($_REQUEST['tipo_relatorio'] == 'html'){
			header("Content-Type: text/html; charset=iso-8859-1");
			echo '
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td colspan="5" class="SubTituloCentro">Relatório Plano Interno</td>
					</tr>
					<tr>
						<td>';
			$db->monta_lista($sql, $cabecalho, 500, 10, 'N', 'rigth', 'N');
			echo '
						</td>
					</tr>
				</table>';
		} elseif($_REQUEST['tipo_relatorio'] == 'xls'){
			$dadosExcel = $db->carregar($sql);
			if(empty($dadosExcel)){
				echo '
					<script>
						alert("Nenhum registro encontrado!");
						window.close(); 
					</script>';
				exit;
			} else{
				ob_clean();
				header('content-type: text/html; charset=ISO-8859-1');
				$db->sql_to_excel($dadosExcel, 'Relatório_Plano_Interno', $cabecalho);
			}
		}
		exit();
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	echo "<br>";
	$titulo_modulo = "Relatório Plano Interno";
	monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	//jQuery.noConflict();
	
	function gerarRelatorio(tipo){
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		selectAllOptions( formulario.plicod );
		selectAllOptions( formulario.ptres );
		
		if(tipo_relatorio == 'html'){
			document.getElementById('tipo_relatorio').value = 'html';
		}else{
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		
		janela.focus();
	}	


	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		} else {
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}	
</script>

<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
	<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<?php #Código do PI
				$sql = "SELECT		DISTINCT plicod AS codigo, trim(plicod) AS descricao
						FROM 		monitora.pi_planointerno
						ORDER BY 	plicod";
				$stSqlCarregados = "";
				mostrarComboPopup('Código do PI','plicod',$sql, $stSqlCarregados, 'Selecione o(s) Código(s) do PI'); ?>
		</tr>
		<tr>
			<?php #PTRES
				$sql = "SELECT		DISTINCT ptres AS codigo, trim(ptres) AS descricao
						FROM 		monitora.ptres
						ORDER BY 	ptres";
				$stSqlCarregados = "";
				mostrarComboPopup('PTRES', 'ptres',  $sql, $stSqlCarregados, 'Selecione o(s) PTRE(s)'); ?>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" name="filtrar" value="Visualizar HTML" style="cursor: pointer;" onclick="javascript:gerarRelatorio('html');"/> 
				<input type="button" name="filtrar" value="Visualizar XLS"  style="cursor: pointer;" onclick="javascript:gerarRelatorio('xls');"/>			
			</td>
		</tr>		
	</table>
</form>