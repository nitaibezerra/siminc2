<?php 
if ( $_REQUEST['tipoPesquisa'] == 'xls' ){
	include "exportarExcel.inc";
	exit;
}
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_funcoes.php";
require_once APPRAIZ . "monitora/www/planotrabalhoUN/_constantes.php";
include APPRAIZ. 'includes/classes/relatorio.class.inc';

boUnidadesObrigatorias();

if($_REQUEST['requisicao'] == 'vincular'){
	
	require_once APPRAIZ . "monitora/classes/Pi_Subacao.class.inc";
	require_once APPRAIZ . "monitora/classes/Pi_SubacaoHistorico.class.inc";

	extract($_POST);
	
	$arSbaId = explode(",",$arSbaId);
	$retorno = false;
	$arSbaIdSemPtres = array();
	foreach($arSbaId as $sbaid){
		$sql = "SELECT ptres FROM monitora.pi_subacaodotacao sd
							INNER JOIN monitora.ptres p on sd.ptrid = p.ptrid
						WHERE sd.sbaid = $sbaid";
				
		$ptres = $db->pegaUm($sql);
		if($ptres){
			$pi_SubacaoHistorico = new Pi_SubacaoHistorico();
			$pi_SubacaoHistorico->sbaid = $sbaid;
			$pi_SubacaoHistorico->usucpf = $_SESSION['usucpf'];
			$pi_SubacaoHistorico->sahsituacao = $situacao;
			$pi_SubacaoHistorico->salvar();
			
			$pi_Subacao = new Pi_Subacao($sbaid);
			$pi_Subacao->sbasituacao = $situacao;
			$pi_Subacao->salvar();
			
			if($pi_SubacaoHistorico->commit()){
				$retorno = true;
			}
		} else {
			array_push($arSbaIdSemPtres, $sbaid);
		}
		
		unset($pi_Subacao);
		unset($pi_SubacaoHistorico);
	}
	
	if($arSbaIdSemPtres && $arSbaIdSemPtres[0]){
		$ptres = implode(', ',$arSbaIdSemPtres);
		
		$sql = "SELECT sbacod ||' - '||sbatitulo as subacao FROM monitora.pi_subacao WHERE sbaid in ($ptres)";
		$arSubacao = $db->carregar($sql);
		$count = 0;
		foreach($arSubacao as $subacao){
			$arSubacao[$count]['subacao'] = utf8_encode($subacao['subacao']);
			$count++;
		}
		echo simec_json_encode($arSubacao);
		die;		
	}	
	
	echo $retorno;
	die;
	
}

if ( $_REQUEST['submetido'] == 1 ){
	$arFiltro = array();
	
	if( $_REQUEST["sbacod"] ){
		array_push($arFiltro, " UPPER(sa.sbacod) ilike '%" . strtoupper($_REQUEST['sbacod']) . "%' ");
	}
	
	if( $_REQUEST["sbatitulo"] ){
		array_push($arFiltro, " UPPER(sa.sbatitulo) ilike '%" . strtoupper($_REQUEST['sbatitulo']) . "%' ");
	}
	
	if( $_REQUEST["sbadsc"] ){
		array_push($arFiltro, " UPPER(sa.sbadsc) ilike '%" . strtoupper($_REQUEST['sbadsc']) . "%' ");
	}
	
	if( $_REQUEST["sbasituacao"] ){
		array_push($arFiltro, " sa.sbasituacao ilike '%" . $_REQUEST['sbasituacao'] . "%' ");
	}
	
	if( $_REQUEST["unicod"] && $_REQUEST["unicod"][0] ){
		$unicod = "'" . implode("', '",$_REQUEST["unicod"]) . "'";
		array_push($arFiltro, " suu.unicod in ( " . $unicod . " )");
		$unicod = "";		
	}
} 

//faz o controle do registro atual para a paginação
$numero = !$_REQUEST['numero'] ? 1 : intval( $_REQUEST['numero'] );
//Registros por página
$perpage = 100;
//Qtde de Paginas que serão mostradas na barrinha
$pages = 50;

include APPRAIZ."includes/cabecalho.inc";

echo '<br/>';

// Cria o título da tela
$titulo_modulo = "Gerenciar Subação";
if($db->testa_superuser()){
	$unidade = "";
} else {
	$unidade = $db->pegaUm("SELECT unicod || ' - ' || unidsc FROM public.unidade WHERE unicod = '".$_SESSION['monitora_var']['unicod']."'");
}
monta_titulo( $titulo_modulo, $unidade);


?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario1" name="formulario1" method="post" action="">
	<input type="hidden" name="tipoPesquisa" id="tipoPesquisa" value="" />
	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?php
					$sbatitulo = $_REQUEST['sbatitulo'];
					echo campo_texto( 'sbatitulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código</td>
			<td>
				<?php 
					$sbacod = $_REQUEST['sbacod'];
					echo campo_texto( 'sbacod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<!-- tr>
			<td class="SubTituloDireita">Unidade Orçamentária:</td>
			<td>
				<?php 
					//echo $db->pegaUm("SELECT unidsc FROM public.unidade WHERE unicod = '".$_SESSION['monitora_var']['unicod']."'");
				?>
			</td>
		</tr>
		<tr -->
			<td align='right' class="SubTituloDireita" valign="top">Unidade Orçamentária:</td>
		    <td>
				<?php
				$stWhere = "";
				if(!$db->testa_superuser()){
						$arUnicodTemp = perfil_unidade();
						$arUnicodTemp = ($arUnicodTemp) ? $arUnicodTemp : array();
						if($arUnicodTemp[0]){
							$arUnicod = array();
							foreach($arUnicodTemp as $uniid){
								if($uniid['unicod']){
									array_push($arUnicod,$uniid['unicod']);								
								}
							}
							if($arUnicod && $arUnicod[0]){
								$unicodTemp = "'" . implode("', '",$arUnicod) . "'";
								if( $unicodTemp ){
									$stWhere = " AND u.unicod in(" . $unicodTemp . ") ";
								}
							}
						}
				}
				
				$sql = "SELECT 
							u.unicod as codigo, 
							u.unicod || ' - ' || u.unidsc as descricao
						 FROM public.unidade u 
							--inner join monitora.pi_subacaounidade su on u.unicod = su.unicod
							WHERE u.orgcod = '". CODIGO_ORGAO_SISTEMA. "' and u.unistatus = 'A' $stWhere
						ORDER BY u.unicod 
						";
				//ver($sql);
				combo_popup( "unicod", $sql, "Unidade Orçamentária", "600x600", 0, array(), "", "S", false, false, 5, 400 );
				?>    	
		    </td>
		</tr>
		<tr>
	  	  	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
	        <td>
	        	<?php
	        	$sbadsc = $_REQUEST['sbadsc'];
	        	echo campo_textarea( 'sbadsc', 'N', 'S', 'Descrição ', 65 , 4, 1000); ?>
	        </td>
	    </tr>
	    <tr>
			<td class="SubTituloDireita">Situação</td>
			<td>
				<input type="radio" name="sbasituacao" id="situacao_qualquer" value="" <?if($_REQUEST['sbasituacao'] == '') echo 'checked'; ?>> Qualquer
				<input type="radio" name="sbasituacao" id="situacao_aprovado" value="A" <?if($_REQUEST['sbasituacao'] == 'A') echo 'checked'; ?>> Aprovado
				<input type="radio" name="sbasituacao" id="situacao_enviado" value="E" <?if($_REQUEST['sbasituacao'] == 'E') echo 'checked'; ?>> Enviado para Revisão
				<input type="radio" name="sbasituacao" id="situacao_pendente" value="P" <?if($_REQUEST['sbasituacao'] == 'P') echo 'checked'; ?>> Pendente de Aprovação
				<input type="radio" name="sbasituacao" id="situacao_revisado" value="R" <?if($_REQUEST['sbasituacao'] == 'R') echo 'checked'; ?>> Revisado
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<?if ( $_REQUEST['submetido'] == 1 ){ ?>
					<div id="mdtodos"><input type="checkbox" name="chktodos" id="chktodos" onclick="marcatodas()"><a href="#" onclick="marcatodas()">Marcar todos</a></div>
				<?}?>
			</td>
			<td>
				<div style="float: left;">
					<input type="button" value="Pesquisar" style="cursor: pointer;" onclick="submeter();">
					<input type="button" value="Ver Todos" style="cursor: pointer;" onclick="location.href='?modulo=principal/planointerno/gerenciarpi&acao=A&submetido=1';"/>
					<input type="button" name="btexcel" value="Visualizar XLS" onclick="visualizarXls();" class="botao">
				</div>
			</td>
		</tr>
	</table>
</form>


<?if ( $_REQUEST['submetido'] == 1 ){ ?>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="numero" value="1">
	
	<!-- Para manter os valores do formulario de pesquisa quando usar a paginação -->
	<input type=hidden name="submetido" value="1">
	<input type=hidden name="sbatitulo" value="<?=$_REQUEST["sbatitulo"]?>">
	<input type=hidden name="sbacod"    value="<?=$_REQUEST["sbacod"]?>">
	<input type=hidden name="sbadsc"	value="<?=$_REQUEST["sbadsc"]?>">
	<input type=hidden name="sbasituacao" value="<?=$_REQUEST["sbasituacao"]?>">
	
	<center>
		<div id="valida" style="overflow:auto; width:95%; height:230px; border-bottom:2px solid #cccccc;">
			<?php 
				/*
				 * REGRA : Não listar PIs sem subações
				 * Solicitado por Henrique Xavier (13/04/09)
				 */

				$sql = "SELECT distinct
							'<center><input class=\"normal\" type=\"checkbox\" id=\"sbacod[' || sa.sbaid || ']\"  name=\"sbacod\" value=\"' || sa.sbaid || '\" onclick=\"retorna(\'' || sa.sbaid || '\');\"></center>' as acao,
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostrahistoricosubacao(\'' || sa.sbaid || '\');\">' || sa.sbacod || '</a>' as sbacod,
							CASE WHEN trim(sa.sbatitulo) is not null THEN 
								sa.sbatitulo  || '<input type=\"hidden\" id=\"sbatitulo[' || sa.sbaid || ']\" value=\"' || sa.sbacod || ' - ' || sa.sbatitulo || '\">'
							ELSE 
								'Não Preenchido<input type=\"hidden\" id=\"sbatitulo[' || sa.sbacod || ']\" value=\" ' || sa.sbacod ||' - Não Preenchido\"/>' 
							END as titulo,
							sa.sbadsc,
							CASE WHEN sa.sbasituacao = 'P' THEN ' <font color=\"red\">Pendente de Aprovação</font> '
								 WHEN sa.sbasituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
								 WHEN sa.sbasituacao = 'R' THEN ' <font color=\"#3F85FF\">Revisado</font> '
								 WHEN sa.sbasituacao = 'E' THEN ' <font color=\"#EAC117\">Enviado para Revisão</font> ' 
							END as situacao
						FROM
							monitora.pi_subacao sa
						INNER JOIN monitora.pi_subacaounidade suu ON suu.sbaid = sa.sbaid 
						INNER JOIN public.unidade u ON suu.unicod = u.unicod 
						WHERE sa.sbastatus = 'A' " . ( !empty($arFiltro) ? " AND " . implode(" AND ", $arFiltro) : '' ) . " AND suu.unicod IS NOT NULL
						AND u.orgcod = '". CODIGO_ORGAO_SISTEMA. "' $stWhere
						GROUP BY sa.sbaid, sa.sbacod, sa.sbatitulo, sa.sbadsc, sa.sbasituacao, sa.sbastatus
						--ORDER BY sa.sbatitulo";
				//ver($sql);
				$cabecalho = array("Ação", "Código", "Título", "Descrição", "Situação");
				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
				
				$arDados = $db->carregar($sql);
				$arDados = ($arDados) ? $arDados : array();

				$count = 1;
				$situacao_anterior = "";
				$boPodeMarcarTodos = 1;
				foreach($arDados as $dados){
					if($dados['situacao'] != $situacao_anterior && $count != 1){
						$boPodeMarcarTodos = 2;
						break;
					}
					$situacao_anterior = $dados['situacao'];
					$count++;
				}
				$nlinhas = count($arDados);
				$nl = !$arDados ? 0 : $nlinhas;
				$reg_fim = (($numero + $perpage) > $nlinhas) ? $nlinhas : ($numero + $perpage -1);
				$total_reg = $nlinhas;
			?>
		</div>
	</center>
	<input type=hidden name="boPodeMarcarTodos" id="boPodeMarcarTodos" value="<?php echo $boPodeMarcarTodos;?>">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td colspan="2">
				<select multiple size="6" name="selectsbaid[]" id="selectsbaid" style="width:100%;" class="CampoEstilo" onchange="moveto(this);">
					<option value="">-- Selecione as Subações na Listagem Acima --</option>
				</select>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td align="left">
			<?php if($_REQUEST["sbasituacao"] == 'P' || $_REQUEST["sbasituacao"] == 'R'){ ?>
				<input type="button" value="Aprovar" onclick="vincular('A')" style="cursor: pointer;"/>
				<input type="button" value="Enviar para Revisão" onclick="vincular('E')" style="cursor: pointer;"/>
			<?php } ?>
			</td>
		</tr>
		<tr>
			<td colspan="3" align="right" bgcolor="#dcdcdc" style="padding:1px;">
				<?php include APPRAIZ . "includes/paginacao.inc"; ?>
			</td>
		</tr>
	</table>
</form>

<?}?>

<script>

	function visualizarXls()
	{
		$('tipoPesquisa').value = 'xls';
		var formulario = document.formulario1;
		selectAllOptions( document.formulario1.unicod );
	
	    formulario.target = 'contemplados';
		var janela = window.open( '?modulo=principal/planotrabalhoUN/gerenciarsubacaoUN&acao=A', 'contemplados', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}

	function submeter() {
		selectAllOptions( document.formulario1.unicod );
		$('tipoPesquisa').value = 'lista';
		$('formulario1').submit();
	}
	
	function retorna(objeto){
		var campoSelect = document.formulario.selectsbaid;
		var checado = document.getElementById('sbacod['+objeto+']');
		var titulo  = document.getElementById('sbatitulo['+objeto+']');
		
		tamanho = campoSelect.options.length;
		if ( campoSelect.options[0].value == ''){ tamanho--; }

		if ( checado.checked == true ){
			campoSelect.options[tamanho] = new Option( titulo.value, checado.value, false, false );
			sortSelect(campoSelect);
		}
		else {
			for( var i=0; i<=campoSelect.length-1; i++ ){
				if ( checado.value == campoSelect.options[i].value ){
					campoSelect.options[i] = null;
				}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('-- Selecione as Subações na Listagem Acima --', '', false, false);}
			sortSelect(campoSelect);
		}
	}
	
	function moveto(obj) {
		document.getElementById('sbacod[' + obj.value + ']').focus();
	}
	
	function marcatodas(){

		var boPodeMarcarTodos = $('boPodeMarcarTodos').value;

		if($('situacao_qualquer').checked == true || boPodeMarcarTodos == 2){
			alert('Favor filtrar por alguma situação');
			$('chktodos').checked = false;
			return false;
		}
		
		document.getElementById('mdtodos').innerHTML = '<input type="checkbox" name="chktodos" id="chktodos" onclick="desmarcatodas()"><a href="#" onclick="desmarcatodas()">Desmarcar todos</a>';
		document.getElementById('chktodos').checked = true;
		
		var sbacod = document.getElementsByName( 'sbacod' );

		for( var i=0; i < sbacod.length; i++ ){
			if ( sbacod[i].checked == false && sbacod[i].value != "" ){
				sbacod[i].checked = true;
				retorna(sbacod[i].value);
			}
		}
	}
	
	function desmarcatodas(){
		
		document.getElementById('mdtodos').innerHTML = '<input type="checkbox" name="chktodos" id="chktodos" onclick="marcatodas()"> <a href="#" onclick="marcatodas()">Marcar todos</a>';
		document.getElementById('chktodos').checked = false;
		
		var sbacod = document.getElementsByName( 'sbacod' );
		
		for( var i=0; i <= sbacod.length-1; i++ ){
			if ( sbacod[i].checked == true ){
				sbacod[i].checked = false;
				retorna(sbacod[i].value);
			}
		}
	}	

	function pagina( numero ){
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}

	function mostrahistoricosubacao( sbaid ){
		window.open('?modulo=principal/planotrabalhoUN/popuphistoricosubacaoUN&acao=A&sbaid='+sbaid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=500');
 	}
 	
	function vincular(situacao){
		var sbaid = document.getElementsByName( 'sbacod' );

		var arSbaId = new Array();
		for( var i=0; i <= sbaid.length-1; i++ ){
			if ( sbaid[i].checked == true ){
				arSbaId.push(sbaid[i].value);
			}
		}

	 	var url = window.location.href;
		var parametros = "requisicao=vincular&arSbaId="+arSbaId+'&situacao='+situacao ;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(r) {
					//alert(r.responseText);
					//$('dibDegug').update(r.responseText);
					//return;
					if(r.responseText == 1){
						alert('Dados gravados com Sucesso.');
						// feito isso por causa da presa.
						document.formulario1.submit();
					} else if(r.responseText){
						var arDados = eval('(' + r.responseText + ')');
						if(arDados.length){
							var subacao_sem_ptres = "";
							for (var j = 0; j < arDados.length; j++) {
								subacao_sem_ptres += arDados[j].subacao+'\n';
							}
							subacao_sem_ptres = "As seguintes subações não poderam ser aprovadas pois não exitem PTRES vinculados.\n\n"+subacao_sem_ptres;
							alert(subacao_sem_ptres);
							// feito isso por causa da presa.
							document.formulario1.submit();
						}
					}
				}
			}
		);
	 	
 	}
</script>
<div id="dibDegug"></div>