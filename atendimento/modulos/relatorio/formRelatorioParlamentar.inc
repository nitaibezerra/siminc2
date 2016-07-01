<?php
if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultRelatorioParlamentar.inc");
	exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';	

monta_titulo( 'Relatório de Atendimento ao Parlamentar', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

<script type="text/javascript">
	function gerarRelatorio(tipo){
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		if (formulario.elements['agrupador'][0] == null){
			alert('Selecione pelo menos um agrupador!');
			return false;
		}	

		if(formulario.datainicio.value != '' && formulario.datafim.value == ''){
			alert('Informe a data de fim!');
			formulario.datafim.focus();
			return false
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
				<?php #UF
					$sqlUF = "SELECT estuf as codigo, estdescricao as descricao
						     FROM territorios.estado est
							 ORDER BY estdescricao";
					mostrarComboPopup( 'Estado', 'estuf',  $sqlUF, '', 'Selecione o(s) Estado(s)' ); ?>
			<tr>
				<td class="subtitulodireita">Data Início</td>
				<td>
				<?= campo_data('datainicio','S',S,'','S','','',''); ?>&nbsp;&nbsp;
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Data Fim</td>
				<td>
				<?= campo_data('datafim','S',S,'','S','','',''); ?>&nbsp;&nbsp;
				</td>
			</tr>			
			<tr>
				<td class="subtitulodireita">Parlamentar</td>
				<td>
					<?php
						$sqlParlamentar = "SELECT DISTINCT par.pesid as codigo, pes.pesnome as descricao
  										  FROM atendimento.parlamentar par
  										  INNER JOIN atendimento.pessoa pes ON pes.pesid = par.pesid
  										  ORDER BY descricao";
						$db->monta_combo( "parlamentar", $sqlParlamentar, 'S', 'Selecione o Parlamentar', '', '', 'Selecione o Parlamentar!', '244', 'N', 'parlamentar' );		
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Partido</td>
				<td>
					<?php
						$sqlPartido = "SELECT DISTINCT parpartido as codigo, parpartido as descricao
						     		  FROM atendimento.parlamentar
							 		  ORDER BY descricao";
						$db->monta_combo( "parpartido", $sqlPartido, 'S', 'Selecione o Partido', '', '', 'Selecione o Partido!', '244', 'N', 'parpartido' );		
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
				'codigo'    => 'estuf',
				'descricao' => '01. Unidade Federativa'
			),
			array(
				'codigo'    => 'evtdataini',
				'descricao' => '02. Data'
			),
			array(
				'codigo'    => 'parlamentar',
				'descricao' => '03. Parlamentar'
			),
			array(
				'codigo'    => 'parpartido',
				'descricao' => '04. Partido'
			),
			array(
				'codigo'    => 'participante_externo',
				'descricao' => '05. Participante Externo'
			),
			array(
				'codigo'    => 'participante_mec',
				'descricao' => '06. Participantes MEC'
			),
			array(
				'codigo'    => 'evtpauta',
				'descricao' => '07. Pauta'
			),
			array(
				'codigo'    => 'evtencaminhamentos',
				'descricao' => '08. Encaminhamento'
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
				'data'    => 'evtdataini',
				'descricao' => '02. Data'
			),
			array(
				'codigo'    => 'parlamentar',
				'descricao' => '03. Parlamentar'
			),
			array(
				'codigo'    => 'parpartido',
				'descricao' => '04. Partido'
			)				
		);				
	}	
