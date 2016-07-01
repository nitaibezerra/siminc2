<?php
if ($_POST['colunas']){
	ini_set("memory_limit","256M");
	include("relatorioPagamentoAjudaDeCusto_resultado.inc");
	exit;
}
$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
if(!$db->testa_superuser()){
	
	$sql = "SELECT DISTINCT 
				p.ungcod,
				eug.empcod
			FROM 
				evento.usuarioresponsabilidade ur 
			INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
			INNER JOIN evento.empenho_unidadegestora eug ON eug.ungcod = ur.ungcod
			WHERE
				ur.rpustatus = 'A' and
				ur.usucpf = '".$_SESSION['usucpf']."' and
				ur.pflcod IN (".implode(',',$perfil).") and
				ur.prsano = '".$_SESSION['exercicio']."'";

	$notUngcod = $db->carregarColuna($sql);
	array_push($notUngcod, '00000000');
	$notEmpcod = $db->carregarColuna($sql,'empcod');
	array_push($notEmpcod, '00000000');
}

function comboEmpenho($dados){
	
	global $db;
	
	$sql = "SELECT 
				empcod as codigo, 
				empcod||' - '||empdsc as descricao
  			FROM 
  				evento.empenho_unidadegestora
  			WHERE
  				".($dados['ungcod'] ? 'ungcod in (\''.str_replace(Array('}{','{','}'),Array('\',\'','',''),$dados['ungcod']).'\')' : '1=1')."
  			ORDER BY 2";
	
	combo_popup('empcod', $sql, 'Empenho', '400x400', 0, array(), '', 'S', false, false, 5 );
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript">

jQuery.noConflict();
	
jQuery(document).ready(function() {

	jQuery('.visualizar').click(function(){

//		if (jQuery("#colunas option:first").val() == null){
//			alert('Selecione pelo menos uma coluna!');
//			return false;
//		}	
		
		var data1 = jQuery('#data1').val();
		var data2 = jQuery('#data2').val();

		if(jQuery('#data1').val() != '' && jQuery('#data2').val() != ''){
			
			if(!validaData(jQuery('#data1').val())){
				alert("Data Início Inválida.");
				jQuery('#data1').focus();
				return false;
			}		
			if(!validaData(jQuery('#data2').val())){
				alert("Data Fim Inválida.");
				jQuery('#data2').focus();
				return false;
			}		
		}
		
		//valida data
		if(data1 != '' && data2 != ''){
			var nova_data1 = parseInt(data1.split("/")[2].toString() + data1.split("/")[1].toString() + data1.split("/")[0].toString());
			var nova_data2 = parseInt(data2.split("/")[2].toString() + data2.split("/")[1].toString() + data2.split("/")[0].toString());
		}
		if(nova_data2 < nova_data1){
			alert('A data final não pode ser menor que a data inicial.');
			return false;
		}	

		jQuery("#colunas option").each(function(){ jQuery(this).attr('selected',true); });
		jQuery("#ungcod option").each(function(){ jQuery(this).attr('selected',true); });
		jQuery("#empcod option").each(function(){ jQuery(this).attr('selected',true); });
		jQuery("#esdid option").each(function(){ jQuery(this).attr('selected',true); });
		jQuery("#usucpf option").each(function(){ jQuery(this).attr('selected',true); });
		if( jQuery(this).attr('id')=='1' ){
			jQuery('#tiporel').val('1');
		}else{
			jQuery('#tiporel').val('2');
		}
		
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		jQuery('#filtro').attr('target','relatorio');	
		jQuery('#filtro').submit();
		
		janela.focus();
			
	});
	

	jQuery('#loader-container').hide();
	
	jQuery('#ungcod').blur(function(){
		var ungcod = '';
		jQuery('#ungcod option').each(function (){
			ungcod += '{'+jQuery(this).val()+'}';
		});

		jQuery.ajax({
			type: "POST",
			url: window.location,
			dataType: 'json',
			data: "req=comboEmpenho&ungcod="+ungcod,
			async: false,
			success: function(msg){
				jQuery('#tdEmp').html(msg);
			}
		});
	});
});
</script>
<form action="" method="post" name="filtro" id="filtro"> 
	<input type="hidden" id="tiporel" name="tiporel" value=""/>
	<div id="loader-container" style="position: absolute; margin-left: 600px; margin-top: 140px">
		<div id="loader">
			<img src="../imagens/wait.gif" width="30xp;" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</div>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<input type="hidden" name="colunas" value="true" />
		<!--<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'unidade' => array(
													'codigo'    => 'unidade',
													'descricao' => '01. Unidade Gestora'
						),
						'empenho' => array(
													'codigo'    => 'empenho',
													'descricao' => '02. Nota de Empenho'
						),
						'situacao' => array(
													'codigo'    => 'situacao',
													'descricao' => '03. Situacao'
						),
						'beneficiario' => array(
													'codigo'    => 'beneficiario',
													'descricao' => '04. Beneficiário'
						),
						'mes' => array(
													'codigo'    => 'mes',
													'descricao' => '05. Mês'
						),
						'dia' => array(
													'codigo'    => 'dia',
													'descricao' => '06. Data da Solicitação'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr> -->
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Unidade Gestora</td>
			<td>
			<?php 
				// Unidades Gestoras
				$sql = "SELECT 
							ungcod as codigo, 
							ungdsc as descricao
						FROM 
							public.unidadegestora
						WHERE
							".(count($notUngcod)>0&&(!$db->testa_superuser()) ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
							ungcod NOT IN ('152004','152005') AND
							ungstatus = 'A' 
						GROUP BY 
							ungcod, 
							ungdsc
						ORDER BY 
							ungdsc;";
				combo_popup('ungcod', $sql, 'Unidades Gestoras', '400x400', 0, array(), '', 'S', false, false, 5, 400,"jQuery('#ungcod').blur();","jQuery('#ungcod').blur();" );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Nota de Empenho</td>
			<td id="tdEmp">
			<?php 
				// Situacao
				$sql = "SELECT 
							empcod as codigo, 
							empcod||' - '||coalesce(empdsc,'Empenho') as descricao
			  			FROM 
			  				evento.empenho_unidadegestora
			  			WHERE
			  				".(count($notEmpcod)>0&&(!$db->testa_superuser()) ? "empcod IN ('".implode('\',\'', $notEmpcod)."') AND" : "")."
			  				1=1
			  			ORDER BY 2";
				combo_popup('empcod', $sql, 'Empenho', '400x400', 0, array(), '', 'S', false, false, 5, 400 );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Situacao</td>
			<td>
			<?php 
				// Situacao
				$sql = "SELECT
							esdid as codigo,
							esddsc as descricao
						FROM
							workflow.estadodocumento
						WHERE
							tpdid = 42
						ORDER BY 2";
				combo_popup('esdid', $sql, 'Situação', '400x400', 0, array(), '', 'S', false, false, 5, 400 );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Beneficiário</td>
			<td>
			<?php 
				// Beneficiário
				$sql = "SELECT 
							usucpf as codigo,
							solnome||' - '||replace(to_char(usucpf::bigint, '000:000:000-00'), ':', '.') as descricao
						FROM 
							evento.solicitacaodiaria
						WHERE
							".(count($notUngcod)>0&&(!$db->testa_superuser()) ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
							1=1
						ORDER BY 2";
				combo_popup('usucpf', $sql, 'Beneficiários', '400x400', 0, array(), '', 'S', false, false, 5, 400 );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Data da solicitação</td>
			<td>
			De:
			<?= campo_data2( 'data1', 'N', 'S', '', 'S', '', '', '', '' ); ?>
			Até:
			<?= campo_data2( 'data2', 'N', 'S', '', 'S', '', '', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
<!--				<input type="button" class="visualizar" id="1" value="Visualizar"          onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>-->
				<input type="button" class="visualizar" id="2" value="Gerar Relatório XLS" onclick="obras_exibeRelatorioGeral('exibir_xls');"/>
			</td>
		</tr>
	</table>
</form>