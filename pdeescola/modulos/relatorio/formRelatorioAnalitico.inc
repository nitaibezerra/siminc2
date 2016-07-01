<?php
	if ($_POST['agrupador']){
		ini_set("memory_limit","256M");
		include("resultRelatorioAnalitico.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Analitico', '&nbsp;' );
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
						Select 	estuf as codigo,
								estdescricao as descricao
						From territorios.estado est
						Order by estdescricao
					";
					$stSqlCarregados = "";
					mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
					
					#Municípios
					$munSql = " 
						Select	muncod AS codigo,
								estuf ||' - '||mundescricao AS descricao
						From territorios.municipio
						Order by mundescricao
					";
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
				<td class="subtitulodireita">Ano de Seleção</td>
				<td>
					<?php
					
						$anoCorrente = date("Y");
						
						$arrayAno = array();
												
						for($i=$anoCorrente; $i>=2008; $i--){
							 array_push($arrayAno, array( "codigo" => "$i", "descricao" => "$i" ));
						}
					
						/*
						$anoCorrente = date("Y");
						
						$arrayAno = array(
							array( 'codigo' => 'T', 'descricao' => 'Universo corrente' )
						);
												
						for($i=2008; $i<=$anoCorrente; $i++){
							 array_push($arrayAno, array( "codigo" => "$i", "descricao" => "$i" ));
						}
						*/
						/*
						$arrayAno = array(
							array( 'codigo' => 'T', 'descricao' => 'Universo corrente' ),
							array( 'codigo' => '2011', 'descricao' => 'Somente 2011' ),
							array( 'codigo' => '2012', 'descricao' => 'Somente 2012' )
						);
						*/
						$memanoreferencia = $_POST['memanoreferencia'];
						$db->monta_combo( "memanoreferencia", $arrayAno, 'S', '', '', '', 'Selecione o item caso queira filtrar por Ano de referencia!', '244' );
					?>
				</td>
			</tr>
				
			<tr>
				<td class="subtitulodireita">Esfera</td>
				<td>
					<?php
						$arrayEsfera = array(
							array('codigo' => '1,3', 'descricao' => 'Todas'),
							array('codigo' => '3', 'descricao' => 'Municipal'),
							array('codigo' => '1', 'descricao' => 'Estadual')
						);
						$esferafiltro = $_POST['esfera'];
						$db->monta_combo( "esfera", $arrayEsfera, 'S', '', '', '', 'Selecione o item caso queira filtrar por esfera!', '244', 'N', 'esfera','','1,3' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Recursos FNDE</td>
				<td>
					<?php
						$arrayRecurso = array(
							array('codigo' => 'T', 'descricao' => 'Todos'),
							array('codigo' => 'N', 'descricao' => 'Não'),
							array('codigo' => 'S', 'descricao' => 'Sim')
						);
						$recursofiltro = $_POST['mempagofnde'];
						$db->monta_combo( "mempagofnde", $arrayRecurso, 'S', '', '', '', 'Selecione o item caso queira filtrar por Recurso FNDE!', '244', 'N', 'mempagofnde' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Projeto 2º Tempo</td>
				<td>
					<?php
						$array2Tempo = array(
							array('codigo' => 'T', 'descricao' => 'Todos'),
							array('codigo' => 'N', 'descricao' => 'Não'),
							array('codigo' => 'S', 'descricao' => 'Sim')
						);
						$tempofiltro = $_POST['memadesaopst'];
						$db->monta_combo( "memadesaopst", $array2Tempo, 'S', '', '', '', 'Selecione o item caso queira filtrar por projeto 2º tempo!', '244', 'N', 'memadesaopst' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Escola Aberta</td>
				<td>
					<?php
						$arrayAberta = array(
							array('codigo' => 'T', 'descricao' => 'Todos'),								
							array('codigo' => 'N', 'descricao' => 'Não'),
							array('codigo' => 'S', 'descricao' => 'Sim')
						);
						$abertafiltro = $_POST['mamescolaaberta'];
						$db->monta_combo( "mamescolaaberta", $arrayAberta, 'S', '', '', '', 'Selecione o item caso queira filtrar por Escola Aberta!', '244', 'N', 'mamescolaaberta' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Brasil sem miséria</td>
				<td>
					<?php
						$arrayMiseria = array(
							array('codigo' => 'T', 'descricao' => 'Todos'),
							array('codigo' => 'N', 'descricao' => 'Não'),
							array('codigo' => 'S', 'descricao' => 'Sim')
						);
						$miseriafiltro = $_POST['membrasilsemmiseria'];
						$db->monta_combo( "membrasilsemmiseria", $arrayMiseria, 'S', '', '', '', 'Selecione o item caso queira filtrar por Brasil sem miséria!', '244', 'N', 'membrasilsemmiseria' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Localização da escola</td>
				<td>
					<?php
						$arrayMiseria = array(
							array('codigo' => 'T', 'descricao' => 'Todas'),
							array('codigo' => 'R', 'descricao' => 'Rural'),
							array('codigo' => 'u', 'descricao' => 'Urbana')
						);
						$miseriafiltro = $_POST['memclassificacaoescola'];
						$db->monta_combo( "memclassificacaoescola", $arrayMiseria, 'S', '', '', '', 'Selecione o item caso queira filtrar por Localização da escola!', '244', 'N', 'memclassificacaoescola' );		
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
				'codigo'    => 'estuf',
				'descricao' => '01. Unidade Federativa'
			),
			array(
				'codigo'    => 'mundescricao',
				'descricao' => '02. Município'
			),
			array(
				'codigo'    => 'tpcdesc',
				'descricao' => '03. Esfera'
			),
			array(
				'codigo'    => 'entcodent',
				'descricao' => '04. Código INEP'
			),
			array(
				'codigo'    => 'cnpj',
				'descricao' => '05. CFP/CNPJ'
			),
			array(
				'codigo'    => 'entnome',
				'descricao' => '06. Escola'
			),
			array(
				'codigo'    => 'diretor',
				'descricao' => '07. Diretor'
			),
			array(
				'codigo'    => 'email',
				'descricao' => '08. E-mail'
			),
			array(
				'codigo'    => 'tpldesc',
				'descricao' => '09. Localização'
			),
			array(
				'codigo'    => 'endereco',
				'descricao' => '10. Endereço'
			),
			array(
				'codigo'    => 'telefone',
				'descricao' => '11. Telefone'
			),
			array(
				'codigo'    => 'mempagofnde',
				'descricao' => '12. Recursos FNDE'
			),
			array(
				'codigo'    => 'memvlrpago',
				'descricao' => '13. Recurso pago'
			),
			array(
				'codigo'    => 'memadesaopst',
				'descricao' => '14. Adesão ao 2º tempo'
			),
			array(
				'codigo'    => 'memdataadesaopst',
				'descricao' => '15. Data da adesão ao 2º tempo'
			),
			array(
				'codigo'    => 'mamescolaaberta',
				'descricao' => '16. Escola Aberta'
			),
			array(
				'codigo'    => 'membrasilsemmiseria',
				'descricao' => '17. Brasil sem miséria'
			),
			array(
				'codigo'    => 'alunadoparticipante',
				'descricao' => '18. Alunado Participante'
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
				'codigo'    => 'tpcdesc',
				'descricao' => '03. Esfera'
			),
			array(
				'codigo'    => 'entcodent',
				'descricao' => '04. Código INEP'
			),
			array(
				'codigo'    => 'entnome',
				'descricao' => '05. Escola'
			)				
		);				
	}	
?>