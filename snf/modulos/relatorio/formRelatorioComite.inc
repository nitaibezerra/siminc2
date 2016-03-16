<?php
	if ($_POST['tipo_relatorio']){
		ini_set("memory_limit","256M");
		include("resultRelatorioComite.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Comitê Institucional', 'Filtros' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">

	function gerarRelatorio(tipo){
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		selectAllOptions( formulario.insid );
		selectAllOptions( formulario.papid );

		if(tipo_relatorio == 'visual'){
			document.getElementById('tipo_relatorio').value = 'visual';
		}else{
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		
		janela.focus();
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
	
	<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

			<?php 	
				#Instituições de ensino.
				$sql = " Select insid as codigo, insnome as descricao From snf.instituicaoensino Where insstatus = 'A' Order by 2 ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Instituição de Ensino', 'insid',  $sql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)' );
				
				#Papel
				$sql = " Select papid as codigo, papdescricao as descricao From snf.papelcomite Where papstatus = 'A' order by 2 ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Papel', 'papid',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)' );
			?>
					
			
			<tr>
				<td class="subtitulodireita" >&nbsp;</td>
				<td lign="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');"/>
					<input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');"/>
				</td>
			</tr>
	</table>
</form>
</body>
</html>

<?php
	function colunasOrigem(){	
		return array(
			array(
				'codigo'    => 'pdicodinep',
				'descricao' => '01. Código INEP'
			),		
			array(
				'codigo'    => 'pdenome',
				'descricao' => '02. Escola'
			),			
			array(
				'codigo'    => 'pdiesfera ',
				'descricao' => '03. Esfera'
			),			
			array(
				'codigo'    => 'estuf',
				'descricao' => '04. Unidade Federativa'
			),
			array(
				'codigo'    => 'mundescricao',
				'descricao' => '05. Município'
			),
			array(
				'codigo'    => 'pdilocalizacao',
				'descricao' => '06. Localização'
			),
			array(
				'codigo'    => 'atedesc',
				'descricao' => '07. Área Temática'
			),
			array(
				'codigo'    => 'curdesc',
				'descricao' => '08. Curso'
			),
			array(
				'codigo'    => 'ncudesc',
				'descricao' => '09. Nível' 
			),
			array(
				'codigo'    => 'moddesc',
				'descricao' => '10. Modalidade'
			),
			array(
				'codigo'    => 'pcfano',
				'descricao' => '11. Ano Referência'
			),
			array(
				'codigo'    => 'privagassolicitadas',
				'descricao' => '12. Vagas Solicitadas'
			),
			array(
				'codigo'    => 'privagasprevistas',
				'descricao' => '13. Vagas Autorizadas'
			)					
		);		
	}
	
	function AgrupadorOrigem(){
		return array(
			array(
				'codigo'    => 'estuf',
				'descricao' => '01. Unidade Federativa'
			),
			array(
				'codigo'    => 'mundescricao',
				'descricao' => '02. Município'
			),
			array(
				'codigo'    => 'pdiesfera',
				'descricao' => '03. Esfera'
			),
			array(
				'codigo'    => 'pdicodinep',
				'descricao' => '04. Código INEP'
			),
			array(
				'codigo'    => 'pdenome',
				'descricao' => '05. Escola'
			)			
		);				
	}	
?>