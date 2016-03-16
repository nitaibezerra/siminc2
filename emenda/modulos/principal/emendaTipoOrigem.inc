<?php
if( $_REQUEST['requisicao'] == 'alterarTiposEmenda' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$db->executar("UPDATE emenda.emenda SET etoid = {$_POST['tipo']} WHERE emecod = '{$_POST['emecod']}' and emeano = '{$_SESSION['exercicio']}'");
	$db->commit();
	exit();
}
include  APPRAIZ."includes/cabecalho.inc";
$db->cria_aba( $abacod_tela, $url, '' );
?> 
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value=""/>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Código:</td>
			<td>
				<?php 
					$emecod = $_REQUEST["emecod"];
					echo campo_texto( 'emecod', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
			<td class="subtitulodireita">Responsável Pelo Recurso:</td>
			<td>
				<?php 
					$resid = $_REQUEST["resid"];
					echo comboResponsavel($_SESSION['usucpf'], $resid);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Unidade Orçamentária:</td>
				<td>
				<?php 
					
					$unicod = $_REQUEST["unicod"];
					$sql = array();
					$sql = "SELECT DISTINCT 
								u.unicod as codigo,
								u.unidsc as descricao
							FROM
								emenda.emenda e
								INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
								INNER JOIN public.unidade u ON u.unicod = f.unicod
							ORDER BY u.unidsc";
					
					$db->monta_combo("unicod",$sql,'S',"Todas",'','','','300','N','unicod');
				?>
			</td>
			<td class="subtitulodireita">Ação Orçamentária:</td>
			<td>
				<?php 
					
					$acacod = $_REQUEST["acacod"];
					
					$sql = array();
					$sql ="SELECT DISTINCT 
								f.acacod as codigo,
								f.acacod || ' - ' || f.acadsc as descricao
							FROM
								emenda.emenda e
								INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							ORDER BY f.acacod";
					
					$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '300', 'N','acacod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Autor do Recurso:</td>
			<td colspan="3">
				<?php 
					
					$autid = $_REQUEST["autid"];
					
					$sql = array();
					$sql ="SELECT DISTINCT 
							ea.autid as codigo,
							ea.autnome as descricao
						FROM
							emenda.autor ea
							INNER JOIN emenda.emenda ee ON ea.autid = ee.autid and ea.autstatus = 'A'
						ORDER BY
							ea.autnome";
					
					$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '365', 'N','autid');
					
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="4" align="center">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaEmendaOrigem();" style="cursor: pointer;"/>
				<input type="button" value="Todos" id="btnTodos" name="btnTodos" onclick="window.location.href = window.location;" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> 
	<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
	
	<script type="text/javascript">
		function pesquisaEmendaOrigem(){
			$('#requisicao').val('pesquisa');
			$('#formulario').submit();
		}
		
		function alterarTiposEmenda(emecod, tipo){
			$.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/emendaTipoOrigem&acao=A",
			data: "requisicao=alterarTiposEmenda&emecod="+emecod+"&tipo="+tipo,
			async: false,
			success: function(msg){
					$('.labOrigem').css('color', '');
					$('.img_span').html('');
					$('#etoid_'+emecod+'_'+tipo).css('color', 'blue');
					$('#etoid_'+emecod+'_'+tipo).css('font-weight', 'bold');
					$('#span_'+emecod+'_'+tipo).html('<img src="../imagens/check.jpg" border="0">');
				}
			});
		}
	</script>
	<style>
		.divLabel{  
			width: 180px;
		}
	</style>
</form>
<?
$filtro = '';
if( $_POST['requisicao'] == 'pesquisa' ){
	if( $_POST['emecod'] ) $filtro .= " and ee.emecod = '".$_POST['emecod']."'";
	if( $_POST['resid'] ) $filtro .= " and ee.resid = ".$_POST['resid'];
	if( $_POST['autid'] ) $filtro .= " and ea.autid = ".$_POST['autid'];
	if( $_POST['unicod'] ) $filtro .= " and ef.unicod = '".$_POST['unicod']."'";
	if( $_POST['acacod'] ) $filtro .= " and ef.acacod = '".$_POST['acacod']."'";
}

/**
 * '<center><input type=\"radio\" name=\"etoid_'||ee.emecod||'\" value=\"'||ee.emecod||'\"></center>' as par,
		    '<center><input type=\"radio\" name=\"etoid_'||ee.emecod||'\" value=\"'||ee.emecod||'\"></center>' as acao1,
		    '<center><input type=\"radio\" name=\"etoid_'||ee.emecod||'\" value=\"'||ee.emecod||'\"></center>' as acao2,
		    '<center><input type=\"radio\" name=\"etoid_'||ee.emecod||'\" value=\"'||ee.emecod||'\"></center>' as acao3,
 */

$sql = "SELECT DISTINCT
		    '
		    <table border=1 width=100%>
		    	<tr>
		    		<td style=\"background-color: #DCDCDC; text-align: center; font-weight: bold \">Selecione o Tipo de Emenda</td>
	    		</tr>
		    	<tr>
		    		<td>
		    			<div>
		    				<label id=\"etoid_'||ee.emecod||'\" style=\"cursor: pointer;\">
		    					<div id=\"par_'||ee.emecod||'\" class=\"divLabel\">
		    						<div id=\"etoid_'||ee.emecod||'_1\" class=\"labOrigem\">
		    							<input type=\"radio\" '||(case when ee.etoid = 1 then 'checked' else '' end)||' name=\"etoid_'||ee.emecod||'\" value=\"1\" style=\"cursor: pointer;\" onclick=\"alterarTiposEmenda('||ee.emecod||', 1);\"> 
	    								PAR<span id=\"span_'||ee.emecod||'_1\" class=\"img_span\"></span>
    								</div>
		    					</div>
	    					</label>
		    				<label id=\"etoid_'||ee.emecod||'\" style=\"cursor: pointer;\">
		    					<div id=\"des_'||ee.emecod||'\" class=\"divLabel\">
			    					<div id=\"etoid_'||ee.emecod||'_2\" class=\"labOrigem\">	
		    							<input type=\"radio\" '||(case when ee.etoid = 2 then 'checked' else '' end)||' name=\"etoid_'||ee.emecod||'\" value=\"2\" style=\"cursor: pointer;\" onclick=\"alterarTiposEmenda('||ee.emecod||', 2);\">
				    					Descentralização<span id=\"span_'||ee.emecod||'_2\" class=\"img_span\"></span>
			    					</div>
		    					</div>
	    					</label>
		    				<br>
		    				<label id=\"etoid_'||ee.emecod||'\" style=\"cursor: pointer;\">
		    					<div id=\"con_'||ee.emecod||'\" class=\"divLabel\">
		    						<div id=\"etoid_'||ee.emecod||'_3\" class=\"labOrigem\">
				    					<input type=\"radio\" '||(case when ee.etoid = 3 then 'checked' else '' end)||' name=\"etoid_'||ee.emecod||'\" value=\"3\" style=\"cursor: pointer;\" onclick=\"alterarTiposEmenda('||ee.emecod||', 3);\"> 
				    					Convênios<span id=\"span_'||ee.emecod||'_3\" class=\"img_span\"></span>
			    					</div>
		    					</div>
		    				</label>
		   					<label id=\"etoid_'||ee.emecod||'\" style=\"cursor: pointer;\">
		    					<div id=\"uni_'||ee.emecod||'\" class=\"divLabel\">
		    						<div id=\"etoid_'||ee.emecod||'_4\" class=\"labOrigem\">
			    						<input type=\"radio\" '||(case when ee.etoid = 4 then 'checked' else '' end)||' name=\"etoid_'||ee.emecod||'\" value=\"4\" style=\"cursor: pointer;\" onclick=\"alterarTiposEmenda('||ee.emecod||', 4);\"> 
			    						Universidades e Institutos<span id=\"span_'||ee.emecod||'_4\" class=\"img_span\"></span>
		    						</div>
		    					</div>
		    				</label>
	    				</div>
    				</td>
    			</tr>
			</table>' as acao3,		     
		    ee.emecod as numero,
		    case when ee.emerelator = 'S' then ea.autnome||' - Relator Geral' else ea.autnome end as autor,
		    pu.unidsc as unidade,
		    ef.fupfuncionalprogramatica as funcional,
		    ef.fupdsc as subtitulo,
		    CASE WHEN ee.resid is not null THEN er.resdsc ELSE 'Não Informado' END as responsavel,
		    ee.emeano				
		FROM
		    emenda.emenda ee
		    inner join emenda.autor ea on ea.autid = ee.autid 
		    inner join emenda.v_funcionalprogramatica ef on ef.acaid = ee.acaid 
		    inner join public.unidade pu on pu.unicod = ef.unicod			
		    left join emenda.responsavel er on er.resid = ee.resid
		    left join emenda.emendatipoorigem et on et.etoid = ee.etoid and et.etostatus = 'A'
		WHERE 
		    ee.emeano = '{$_SESSION['exercicio']}' 
		    and ee.emetipo = 'E'
		    $filtro
		ORDER BY ee.emecod";

$tamanho   = array( '390px', '5%', '', '', '', '', '', '', '', '10%');
		    
$cabecalho = array('', "Código", "Autor", "UO", "Funcional Prg", "Subtítulo", "Responsável", "Ano" );
$db->monta_lista( $sql, $cabecalho, 1000000, 10, 'N', 'center', '', '', $tamanho);

?>