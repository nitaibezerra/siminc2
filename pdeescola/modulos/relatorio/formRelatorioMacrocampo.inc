<?php
	function comboMacrocampo($ano = null) {
		global $db;
		$ano = !$ano ? $_POST['memanoreferencia'] : $ano;
		
		if($ano != "2012"){
			$sqlMacrocampo = "SELECT mtmid AS codigo, mtmdescricao AS descricao 
							 FROM pdeescola.metipomacrocampo 
							 WHERE mtmsituacao = 'A' AND mtmanoreferencia = {$ano} ORDER BY 2";
			$db->monta_combo("macrocampo", $sqlMacrocampo,'S', 'Todos', '', '','Selecione o item caso queira filtrar por Macrocampo!', '244');
		} else {
			$sqlMacrocampo = "SELECT mtmid AS codigo, mtmdescricao AS descricao 
							 FROM pdeescola.metipomacrocampo 
							 WHERE mtmsituacao = 'A' AND mtmanoreferencia = {$ano} ORDER BY 2";
			$db->monta_combo("macrocampo", $sqlMacrocampo,'S', 'Todos', '', '','Selecione o item caso queira filtrar por Macrocampo!', '244');
		}
	}

	if($_REQUEST['req'] == 'carregarAtividade'){
		$ano = $_REQUEST['ano'];
		$stSql = "SELECT
					mtaid AS codigo,
					mtadescricao || ' - ' || mtaanoreferencia AS descricao
				FROM pdeescola.metipoatividade
				WHERE mtasituacao = 'A'
					--AND mtaatividadepst = true
					AND mtaanoreferencia = ".$ano."
				ORDER BY descricao ASC";
		
		mostrarComboPopup( 'Atividades', 'atividade',  $stSql, '', 'Selecione a(s) Atividades(s)' );
		die;
	}

	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}

	if ($_POST['agrupador']){
		/* configurações */
		ini_set("memory_limit", "2048M");
		set_time_limit(0);
		/* FIM configurações */
		
		include("resultRelatorioMacrocampo.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Macrocampo Atividade', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">

	function comboMacrocampo(memanoreferencia){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=comboMacrocampo&memanoreferencia=" + memanoreferencia,
			success: function(retorno){
				jQuery('#id_macrocampo').html(retorno);
			}
		});

		jQuery.ajax({
			url		: window.location,
			data	: 'req=carregarAtividade&ano='+memanoreferencia,
			type	: 'post',
			success	: function(e){
				jQuery('#tr_atividade').remove();
				jQuery('#tr_estuf').after(e);
			}
		});
	}


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
		selectAllOptions( formulario.atividade );

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
<?php
$ano = $_POST['memanoreferencia'] ? $_POST['memanoreferencia'] : $_SESSION['exercicio']; 
?>
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
					
					$stSql = "SELECT
							mtaid AS codigo,
							mtadescricao || ' - ' || mtaanoreferencia AS descricao
						FROM pdeescola.metipoatividade
						WHERE mtasituacao = 'A'
							--AND mtaatividadepst = true
							AND mtaanoreferencia = ".$ano."
						ORDER BY descricao ASC";
					
					mostrarComboPopup( 'Atividades', 'atividade',  $stSql, '', 'Selecione a(s) Atividades(s)' );
						
					
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
						$arrayAno = array(
							array( 'codigo' => '2012', 'descricao' => '2012' ),
							array( 'codigo' => '2008', 'descricao' => '2008' ),
							array( 'codigo' => '2009', 'descricao' => '2009' ),
							array( 'codigo' => '2010', 'descricao' => '2010' ),
							array( 'codigo' => '2011', 'descricao' => '2011' )
						);
						*/
						$memanoreferencia = $_POST['memanoreferencia'];
						$db->monta_combo( "memanoreferencia", $arrayAno, 'S', '', 'comboMacrocampo', '', 'Selecione o item caso queira filtrar por Ano de referencia!', '244');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Macrocampo</td>
				<td id="id_macrocampo">
					<?php
						if($ano)
							comboMacrocampo($ano);
						else
							comboMacrocampo(2012);												
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
							array('codigo' => '', 'descricao' => 'Todos'),
							array('codigo' => 't', 'descricao' => 'Sim'),
							array('codigo' => 'f', 'descricao' => 'Não')
						);
						$recursofiltro = $_POST['mempagofnde'];
						$db->monta_combo( "mempagofnde", $arrayRecurso, 'S', '', '', '', 'Selecione o item caso queira filtrar por Recurso FNDE!', '244', 'N', 'mempagofnde' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Escola Aberta</td>
				<td>
					<?php
						$arrayAberta = array(
							array('codigo' => '', 'descricao' => 'Todos'),
							array('codigo' => 't', 'descricao' => 'Sim'),
							array('codigo' => 'f', 'descricao' => 'Não')
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
							array('codigo' => '', 'descricao' => 'Todos'),
							array('codigo' => 't', 'descricao' => 'Sim'),
							array('codigo' => 'f', 'descricao' => 'Não')
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
				<td align="center">
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
				'codigo'    => 'tpcid',
				'descricao' => '01. Esfera'
			),
			array(
				'codigo'    => 'estuf',
				'descricao' => '02. Unidade Federativa'
			),
			array(
				'codigo'    => 'mundescricao',
				'descricao' => '03. Município'
			),
			array(
				'codigo'    => 'entcodent',
				'descricao' => '04. Código INEP'
			),
			array(
				'codigo'    => 'entnome',
				'descricao' => '05. Escola'
			),			
			array(
				'codigo'    => 'endereco',
				'descricao' => '06. Endereço'
			),
			array(
				'codigo'    => 'endcep',
				'descricao' => '07. CEP'
			),
			array(
				'codigo'    => 'entemail',
				'descricao' => '08. E-mail'
			),
			array(
				'codigo'    => 'telefone',
				'descricao' => '09. Telefone'
			),
			array(
				'codigo'    => 'mtmdescricao',
				'descricao' => '10. Macrocampo'
			),
			array(
				'codigo'    => 'mtadescricao',
				'descricao' => '11. Atividade'
			),
			array(
				'codigo'    => 'diretor',
				'descricao' => '12. Diretor'
			),
			array(
				'codigo'    => 'cpf_diretor',
				'descricao' => '13. CPF Diretor'
			),
			array(
				'codigo'    => 'email_diretor',
				'descricao' => '14. E-mail Diretor'
			),
			array(
				'codigo'    => 'telefone_diretor',
				'descricao' => '15. Telefone Diretor'
			),		
			array(
				'codigo'    => 'celular_diretor',
				'descricao' => '16. Celular Diretor'
			),
			array(
				'codigo'    => 'coordenador',
				'descricao' => '17. Coordenador'
			),
			array(
				'codigo'    => 'cpf_coordenador',
				'descricao' => '18. CPF Coordenador'
			),
			array(
				'codigo'    => 'email_coordenador',
				'descricao' => '19. E-mail Coordenador'
			),
			array(
				'codigo'    => 'telefone_coordenador',
				'descricao' => '20. Telefone Coordenador'
			),		
			array(
				'codigo'    => 'celular_coordenador',
				'descricao' => '21. Celular Coordenador'
			),
			array(
				'codigo'    => 'mempagofnde',
				'descricao' => '22. Recursos FNDE'
			),			
			array(
				'codigo'    => 'memvlrpago',
				'descricao' => '23. Recurso pago'
			),	
			array(
				'codigo'    => 'mamescolaaberta',
				'descricao' => '24. Escola Aberta'
			),
			array(
				'codigo'    => 'membrasilsemmiseria',
				'descricao' => '25. Brasil sem miséria'
			),
			array(
				'codigo'    => 'total_aluno',
				'descricao' => '26. Total de Alunos'
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
			),				
			array(
				'codigo'    => 'mtmdescricao',
				'descricao' => '06. Macrocampo'
			),
			array(
				'codigo'    => 'mtadescricao',
				'descricao' => '07. Atividade'
			)
		);				
	}	
?>