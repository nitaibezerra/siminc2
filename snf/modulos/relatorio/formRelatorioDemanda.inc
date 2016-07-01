<?php
	if ($_POST['agrupador']){
		ini_set("memory_limit","256M");
		include("resultRelatorioDemanda.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Demanda', '&nbsp;' );
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

		if (formulario.elements['agrupador'][0] == null){
			alert('Selecione pelo menos um agrupador!');
			return false;
		}	
		selectAllOptions( formulario.colunas );
		selectAllOptions( formulario.agrupador );
		selectAllOptions( formulario.estuf );
		selectAllOptions( formulario.municipio );

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
		<tr>
			<td class="SubTituloDireita" width="10%">Colunas</td>
			<td>
				<?php				
					#Montar e exibe colunas
					$arrayColunas = colunasOrigem();
					$colunas = new Agrupador('formulario');
					$colunas->setOrigem('naoColunas', null, $arrayColunas);
					$colunas->setDestino('colunas', null);
					$colunas->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					#Montar e exibe agrupadores
					$arrayAgrupador = AgrupadorOrigem();
					$agrupador = new Agrupador('formulario');
					$agrupador->setOrigem('naoAgrupador', null, $arrayAgrupador);
					$agrupador->setDestino('agrupador', null);
					$agrupador->exibir();
				?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
				<?php 	
					#UF
					$ufSql = "
						SELECT 	estuf as codigo,
								estdescricao as descricao
						FROM territorios.estado est
						ORDER BY estdescricao";
					$stSqlCarregados = "";
					mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
					
					#Municípios
					$munSql = " 
						SELECT	muncod AS codigo,
								estuf ||' - '||mundescricao AS descricao
						FROM territorios.municipio
						ORDER BY mundescricao";
					$stSqlCarregados = "";
					$where = array(
								array(
									"codigo" 	=> "estuf",
									"descricao" => "<b style='size: 8px;'> Unidade Federativa</b>",
									"numeric"	=> false
							   	)
							 );
					mostrarComboPopup( 'Município', 'municipio',  $munSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where );
				?>
			<tr>
				<td class="subtitulodireita">Área Temática</td>
				<td>
					<?php
						$sqlArea = "SELECT ateid AS codigo, atedesc AS descricao FROM catalogocurso.areatematica WHERE atestatus = 'A' ORDER BY 2";
						$db->monta_combo("area", $sqlArea,'S', 'Selecione a Área Temática', '', '','Selecione o item caso queira filtrar por Área Temática!', '244');		
					?>
				</td>
			</tr>
				
			<tr>
				<td class="subtitulodireita">Curso</td>
				<td>
					<?php
						$sqlCurso = "SELECT curid AS codigo , curdesc AS descricao FROM catalogocurso.curso WHERE curstatus = 'A' ORDER BY 2";
						$db->monta_combo("curso", $sqlCurso,'S', 'Selecione o Curso', '', '','Selecione o item caso queira filtrar por Curso!', '244');		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Modalidade</td>
				<td>
					<?php
						$sqlModalidade = "SELECT modid AS codigo, moddesc AS descricao FROM catalogocurso.modalidadecurso WHERE modstatus = 'A' ORDER BY 2";
						$db->monta_combo("modalidade", $sqlModalidade,'S', 'Selecione a Modalidade', '', '','Selecione o item caso queira filtrar por Modalidade!', '244');		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Demanda Social</td>
				<td>
					<?php
						$arrayDemandaSocial = array(
							array('codigo' => 'S', 'descricao' => 'Sim'),
							array('codigo' => 'N', 'descricao' => 'Não')
						);
						$esferafiltro = $_POST['esfera'];
						$db->monta_combo( "demanda_social", $arrayDemandaSocial, 'S', 'Selecione a Demada Social', '', '', 'Selecione o item caso queira filtrar por Demanda Social!', '244');		
					?>
				</td>
			</tr>		
			
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
				'codigo'    => 'pdiesfera',
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