<?php
$perfilEmenda = pegaPerfil( $_SESSION["usucpf"] );
unset($_POST['acao']);

include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatórios Geral - Emendas', '<b>Filtros de Pesquisa</b>');

function agrupador(){
	return array(
				array('codigo' => 'estuf',
					  'descricao' => 'Estado'),
				array('codigo' => 'parid',
					  'descricao' => 'Partido'),				
				array('codigo' => 'autid',
					  'descricao' => 'Autor'),					
				array('codigo' => 'resid',
					  'descricao' => 'Responsavel'),				
				array('codigo' => 'acacod',
					  'descricao' => 'Ação'),
				array('codigo' => 'unicod',
					  'descricao' => 'Unidade'),
				array('codigo' => 'emecod',
					  'descricao' => 'Emenda'),
				array('codigo' => 'entid',
					  'descricao' => 'Entidade'),
				array('codigo' => 'gndcod',
					  'descricao' => 'GND'),
				array('codigo' => 'tpaid',
					  'descricao' => 'Tipo de Autor')				
				);
}
?>

<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script type="text/javascript" src="/includes/prototype.js"></script>
</head>
<body>
<form action="emenda.php?modulo=relatorio/popUpRelatorioGeral&acao=A" method="post" target="page" name="formulario" id="formulario">

<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Agrupadores</b></td>
		<td>
			<?php
			
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, array(			
																	));
			$campoAgrupador->exibir();
			
			?>
			
		</td>
	</tr>
	<tr>
	<?php 
		//Responsavel		
		$resid = $_REQUEST["resid"];
		
		$sql = "SELECT DISTINCT 
					er.resid as codigo,
					er.resdsc as descricao
				FROM
					emenda.responsavel er
				INNER JOIN
					emenda.emenda ee ON er.resid = ee.resid";
		
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Responsável Pela Emenda</b>', 'responsavel',  $sql, $stSqlCarregados, 'Selecione o(s) Responsável(s)' ); 
		
		
		//Partido		
		$parid = $_REQUEST["parid"];
		
		$sql = "SELECT 
				  parid as codigo,
				  parnome as descricao
				FROM 
				  emenda.partido
				WHERE parstatus = 'A'
				order by parnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Partido</b>', 'parid',  $sql, $stSqlCarregados, 'Selecione o(s) Partido(s)' ); 
		
		//Tipo de Autor		
		$tpaid = $_REQUEST["tpaid"];
		
		$sql = "SELECT 
				  tpaid as codigo,
				  tpanome as descricao
				FROM 
				  emenda.tipoautor
				WHERE tpastatus = 'A'
				order by tpanome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Tipo de Autor</b>', 'tpaid',  $sql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Autor(es)' ); 				

		//Estado
		$estuf = $_REQUEST["estuf"];		
		$sql = "SELECT 
				  estuf as codigo,
				  estuf||' - '||estdescricao as descricao
				FROM 
				  territorios.estado 
				ORDER BY estdescricao";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Estado</b>', 'estuf',  $sql, $stSqlCarregados, 'Selecione o(s) Estados(s)' ); 				
	?>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Número da Emenda:</b></td>
		<td>
			<?php 
				$emecod = $_REQUEST["emecod"];
				echo campo_texto( 'emecod', 'N', 'S', '', 12, 8, '[#]', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
	<?php 
		//Entidade
		$entid = $_REQUEST["entid"];
		
		$sql = "SELECT DISTINCT 
					eb.entid as codigo,
					ee.entnome as descricao
				FROM
					emenda.entidadebeneficiada eb
				INNER JOIN
					entidade.entidade ee ON eb.entid = ee.entid
				ORDER BY
					ee.entnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Entidade Beneficiada</b>', 'entid',  $sql, $stSqlCarregados, 'Selecione a(s) Entidade Beneficiada(s)' ); 
		
		//Unidade Orçamentária
		$unicod = $_REQUEST["unicod"];
		
		$sql = "SELECT DISTINCT 
					u.unicod as codigo,
					u.unidsc as descricao
				FROM
					emenda.emenda e
				INNER JOIN
					emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
				INNER JOIN
					public.unidade u ON u.unicod = f.unicod
				order by u.unidsc";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Unidade Orçamentária</b>', 'unicod',  $sql, $stSqlCarregados, 'Selecione a(s) Unidade Orçamentária(s)' ); 				
		
		//Ação Orçamentária
		$acacod = $_REQUEST["acacod"];
		
		$sql = "SELECT DISTINCT 
					f.acacod as codigo,
					f.acacod || ' - ' || f.acadsc as descricao
				FROM
					emenda.emenda e
				INNER JOIN
					emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
				ORDER BY f.acacod";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Ação Orçamentária</b>', 'acacod',  $sql, $stSqlCarregados, 'Selecione a(s) Ação(ões) Orçamentária(s)' );
		
		//GND
		$gndcod = $_REQUEST["gndcod"];
	
		$sql = "SELECT 
				  gndcod as codigo,
				  gndcod ||' - '||gnddsc as descricao						  
				FROM 
				  public.gnd";
		
		$stSqlCarregados = "";
		mostrarComboPopup( '<b>GND</b>', 'gndcod',  $sql, $stSqlCarregados, 'Selecione o(s) GND(s)' ); 
		
		//Autor da Emenda
		$autid = $_REQUEST["autid"];
		
		$sql = "SELECT DISTINCT 
					ea.autid as codigo,
					ea.autnome as descricao
				FROM
					emenda.autor ea
				INNER JOIN
					emenda.emenda ee ON ea.autid = ee.autid
				ORDER BY
					ea.autnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Autor da Emenda</b>', 'autid',  $sql, $stSqlCarregados, 'Selecione o(s) Autor(es) da Emenda' );
		
		$sql = "SELECT 
					esp.espid as codigo,
					esp.espnome as descricao
				FROM
					emenda.especificacao esp
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE espstatus = 'A'				
				ORDER BY
					espnome";

		$stSqlCarregados = "";
		mostrarComboPopup( '<b>Especificações</b>', 'espid',  $sql, $stSqlCarregados, 'Selecione a(s) Especificações(s)' );
	?>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
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
	function geraPopRelatorio(){
		var form = $('formulario');
		
		if (form.elements['agrupador'][0] == null){
			alert('Selecione pelo menos um agrupador!');
			return false;
		}
		if($('responsavel_campo_flag').value == "1"){
			selectAllOptions( form.responsavel );
		}
		if($('entid_campo_flag').value == "1"){
			selectAllOptions( form.entid );
		}
		if($('unicod_campo_flag').value == "1"){
			selectAllOptions( form.unicod );
		}
		if($('acacod_campo_flag').value == "1"){
			selectAllOptions( form.acacod );
		}
		if($('gndcod_campo_flag').value == "1"){
			selectAllOptions( form.gndcod );
		}
		if($('autid_campo_flag').value == "1"){
			selectAllOptions( form.autid );
		}
		if($('parid_campo_flag').value == "1"){
			selectAllOptions( form.parid );
		}
		if($('tpaid_campo_flag').value == "1"){
			selectAllOptions( form.tpaid );
		}
		if($('estuf_campo_flag').value == "1"){
			selectAllOptions( form.estuf );
		}
		if($('espid_campo_flag').value == "1"){
			selectAllOptions( form.espid );
		}

		selectAllOptions( form.agrupador );
		var janela = window.open('emenda.php?modulo=relatorio/popUpRelatorioGeral&acao=A','page','toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no, fullscreen');
		janela.focus();
		form.submit();
	}
</script>
</html>