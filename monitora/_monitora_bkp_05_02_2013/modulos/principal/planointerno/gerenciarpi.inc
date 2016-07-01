<?php
/*
 * Verifica qual página sera redirecionado
 */
include "planotrabalho/_constantes.php";
include "planotrabalho/_funcoes.php";

redirecionaUnidadePerfil('gerenciarPi');

exit;


include "planointerno/_constantes.php";
include "planointerno/_funcoes.php";



if(!$db->testa_superuser())
	$regra_0001 = aplicarregra_0001();
	
	

if ( $_REQUEST['ajax'] == 1 ){
	$piarray = explode(",",$_REQUEST['pi']);
	$sql = "UPDATE monitora.planointerno SET plisituacao = '".$_REQUEST['situacao']."' WHERE plicod in ('".implode("','",$piarray)."')";
	if($db->executar($sql)){
		for($i=0; $i<count($piarray);$i++){
			$sqlx = "INSERT INTO monitora.planointernohistorico( plicod, pihobs, pihsituacao, pihdata, usucpf ) VALUES ( '{$piarray[$i]}', '', '{$_REQUEST['situacao']}', now(), '{$_SESSION['usucpf']}' )";
			$db->executar($sqlx);
			enviaEmailStatusPi($piarray[$i]);	
		}
		$db->commit();
		echo 'Operação realizada com sucesso!';	
	}else{
		$db->rollback();
		echo 'Ocorreram erros na transação!';
	}
	die;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");


define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);

define("EXEC_FNDE",42);
define("EXEC_CAPES",39);


if ( $_REQUEST['submetido'] == 1 ){
	
	$filtro = array();
		
	if( $_REQUEST["plicod"] ){
		array_push($filtro, " UPPER(pi.plicod) ilike '%" . strtoupper($_REQUEST['plicod']) . "%' ");
	}
	
	if( $_REQUEST["plititulo"] ){
		array_push($filtro, " UPPER(pi.plititulo) ilike '%" . strtoupper($_REQUEST['plititulo']) . "%' ");
	}
	
	/*
	 * Nos casos de GESTORES(112) E EQUIPE DE APOIO(113)
	 * filtrar os pis que estejam em subações ligadas as unidades definidas no perfil
	 * Solicitado pelo Henrique Xavier (08/06/2009)
	 */
	$sql = "SELECT trim(ur.unicod) as unicod
			FROM monitora.usuarioresponsabilidade ur
			WHERE ur.usucpf = '".$_SESSION["usucpf"]."' AND
				  (ur.pflcod = 112 OR ur.pflcod = 113) AND
				  ur.rpustatus = 'A'";
	
	$unidades = $db->carregar($sql);
	if($unidades) {
		foreach($unidades as $uni) {
			$unidadesperfil[] = $uni['unicod'];
		}
		$filtrounicod = " ent2.entunicod IN ('".implode("','",$unidadesperfil)."')";
	}
	
	
	if( $_REQUEST["entid"] ){
		array_push($filtro, " (e.entid = '" . $_REQUEST['entid'] . "'OR obr.entidunidade='".$_REQUEST['entid']."'".(($filtrounicod)?" OR".$filtrounicod:"").") ");
	} elseif($filtrounicod) {
		
		if($regra_0001['dadosentidades']) {
			
			
			foreach($regra_0001['dadosentidades'] as $d){
				$d2[] = $d['codigo'];
			}
			
			
			array_push($filtro, " (e.entid in (".implode(",",$d2).") OR obr.entidunidade in (".implode(",",$d2).") ) " );
		}
		else{
			array_push($filtro, $filtrounicod);
		}
		
	}
	
	if( $_REQUEST["plisituacao"] ){
		array_push($filtro, " pi.plisituacao ilike '%" . $_REQUEST['plisituacao'] . "%' ");
	}

	
	if( $_REQUEST["enquadramento"] ){
		array_push($filtro, " sa.eqdid = '" . $_REQUEST['enquadramento'] . "' ");
	}

	if( $_REQUEST["executor"] ){
		array_push($filtro, " sa.exeid = '" . $_REQUEST['executor'] . "' ");
	}
	
	if( $_REQUEST["gestor"] ){
		array_push($filtro, " sa.gstid = '" . $_REQUEST['gestor'] . "' ");
	}
	
	if( $_REQUEST["nivel"] ){
		array_push($filtro, " pi.dpiid = '" . $_REQUEST['nivel'] . "' ");
	}
	
	if( $_REQUEST["apropriacao"] ){
		array_push($filtro, " pi.cpiid = '" . $_REQUEST['apropriacao'] . "' ");
	}
	
	if( $_REQUEST["modalidade"] ){
		array_push($filtro, " pi.modid = '" . $_REQUEST['modalidade'] . "' ");
	}
	
	if( $_REQUEST['progunidade'] ) {
		array_push($filtro, " ptres.unicod = '" . $_REQUEST['progunidade'] . "' ");
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
$titulo_modulo = "Gerenciar Plano Interno";
monta_titulo( $titulo_modulo, "");



?>
<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="pesquisa" name="pesquisa" method="post" action="">
	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Código do PI</td>
			<td>
				<?php 
					$plicod = $_REQUEST['plicod'];
					echo campo_texto( 'plicod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?php
					$plititulo = $_REQUEST['plititulo'];
					echo campo_texto( 'plititulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
				<?php
				/*
				 * REGRA 0001
				 */
				if($regra_0001['dadosentidades']) {
					$entid = $_REQUEST['entid'];
					$arraysel = array( "codigo" => "", "descricao" => "Selecione...");
					array_unshift($regra_0001['dadosentidades'], $arraysel);
					$db->monta_combo("entid", $regra_0001['dadosentidades'], 'S', "", '', '', '', '340', 'N', 'unicod');
				} 
				/*
				 * FIM REGRA 0001
				 */
				else {
					$entid = $_REQUEST['entid'];
					$sql = "SELECT
						   ent.entid as codigo,
						   CASE fen.funid
						   	when 34 then 
						   		entungcod ||' - '|| entnome 
						   	else 
						   		entunicod ||' - '|| entnome 
						   	end as descricao
						   FROM entidade.entidade ent
						   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						   WHERE (entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."') OR fen.funid=12 OR fen.funid=11) and fen.funid <> 33 and ent.entid NOT IN ('391293','391294') 
						   ORDER BY entunicod";
					$db->monta_combo("entid", $sql, 'S', "Selecione...", '', '', '', '340', 'N', 'unicod');
				}
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
		    <td>
		    	
				<?
				$enquadramento = $_REQUEST['enquadramento'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 5";  
				$db->monta_combo('enquadramento', $sql, 'S',  'Selecione', '', '', '', '', 'N'); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
		    <td>
				<?
				$executor = $_REQUEST['executor'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 3";  
				$db->monta_combo('executor', $sql, 'S', 'Selecione', '', '', '', '100', 'N', '');
				?>    	
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
		    <td>
		    	
				<?
				$gestor = $_REQUEST['gestor'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = 4";  
				$db->monta_combo('gestor', $sql, 'S', 'Selecione', '', '', '', '100', 'N', ''); 
				?>    	
		    </td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
			<td>
				<?
				$nivel = $_REQUEST['nivel'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
					    FROM public.combodadostabela where ctbid=6 and cdtstatus='A' order by cdtcod";
				
				$db->monta_combo('nivel', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
				//'id="idCodificacao"','atualizarPrevisaoPI();'
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita'>Categoria de Apropriação:</td>
			<td>
				<?
				$apropriacao = $_REQUEST['apropriacao'];
				$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
					    FROM public.combodadostabela where ctbid=7 and cdtstatus='A' order by cdtcod";
				
				$db->monta_combo('apropriacao', $sql, 'S', 'Selecione', '', '', '', '340', 'N', '');
				
				?>
			</td>
		</tr>	
		<tr>
			<td align='right' class="SubTituloDireita">Modalidade de Ensino:</td>
		    <td>
				<?
					$modalidade = $_REQUEST['modalidade'];
					$sql = "SELECT cdtid as codigo, cdtcod ||' - '|| cdtdsc as descricao
						    FROM public.combodadostabela where ctbid=8 and cdtstatus='A' order by cdtcod";
					$modensino = $modid;
					$db->monta_combo('modalidade', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Programação da Unidade:</td>
		    <td>
				<?
					$progunidade = $_REQUEST['progunidade'];
					$sql = "SELECT unicod as codigo, unicod ||' - '|| unidsc as descricao
						    FROM public.unidade where orgcod ilike '26%' and unistatus='A' order by unicod";
					$db->monta_combo('progunidade', $sql, 'S', 'Selecione', '', '', '', '240', 'N', '');
		    	 ?>
		    </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação</td>
			<td>
				<input type="radio" name="plisituacao" id="plisituacao" value="" <?if($_REQUEST['plisituacao'] == '') echo 'checked'; ?>> Qualquer
				<input type="radio" name="plisituacao" id="plisituacao" value="C" <?if($_REQUEST['plisituacao'] == 'C') echo 'checked'; ?>> Aprovado
				<input type="radio" name="plisituacao" id="plisituacao" value="H" <?if($_REQUEST['plisituacao'] == 'H') echo 'checked'; ?>> Homologado
				<input type="radio" name="plisituacao" id="plisituacao" value="R" <?if($_REQUEST['plisituacao'] == 'R') echo 'checked'; ?>> Enviado para Revisão
				<input type="radio" name="plisituacao" id="plisituacao" value="P" <?if($_REQUEST['plisituacao'] == 'P') echo 'checked'; ?>> Pendente
				<input type="radio" name="plisituacao" id="plisituacao" value="S" <?if($_REQUEST['plisituacao'] == 'S') echo 'checked'; ?>> Cadastrado no SIAFI
				<input type="radio" name="plisituacao" id="plisituacao" value="V" <?if($_REQUEST['plisituacao'] == 'V') echo 'checked'; ?>> Revisado
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<?if ( $_REQUEST['submetido'] == 1 ){ ?>
					<div id="mdtodos"><a href="javascript:marcatodas();"><input type="checkbox" name="chktodos" id="chktodos"> Marcar todos</a></div>
				<?}?>
				<!-- <input type="button" value="Marcar Todos" onclick="marcatodas();" style="cursor: pointer;"/>  -->
			</td>
			<td>
				<div style="float: left;">
					<input type="submit" value="Pesquisar" style="cursor: pointer;"/>
					<input type="button" value="Ver Todos" style="cursor: pointer;" onclick="location.href='?modulo=principal/planointerno/gerenciarpi&acao=A&submetido=1';"/>
				</div>
			</td>
		</tr>
	</table>
</form>

<?if ( $_REQUEST['submetido'] == 1 ){ ?>
<form id="formulario" name="formulario" method="post" action="">
	<input type=hidden name="numero" value="1">
	
	<!-- Para manter os valores do formulario de pesquisa quando usar a paginação -->
	<input type=hidden name="submetido" 	value="1">
	<input type=hidden name="plicod" 		value="<?=$_REQUEST["plicod"]?>">
	<input type=hidden name="plititulo" 	value="<?=$_REQUEST["plititulo"]?>">
	<input type=hidden name="entid"			value="<?=$_REQUEST["entid"]?>">
	<input type=hidden name="enquadramento" value="<?=$_REQUEST["enquadramento"]?>">
	<input type=hidden name="executor" 		value="<?=$_REQUEST["executor"]?>">
	<input type=hidden name="gestor" 		value="<?=$_REQUEST["gestor"]?>">
	<input type=hidden name="nivel"			value="<?=$_REQUEST["nivel"]?>">
	<input type=hidden name="apropriacao" 	value="<?=$_REQUEST["apropriacao"]?>">
	<input type=hidden name="modalidade" 	value="<?=$_REQUEST["modalidade"]?>">
	<input type=hidden name="progunidade" 	value="<?=$_REQUEST["progunidade"]?>">
	<input type=hidden name="plisituacao" 	value="<?=$_REQUEST["plisituacao"]?>">
	
	<center>
		<div id="valida" style="overflow:auto; width:95%; height:230px; border-bottom:2px solid #cccccc;">
			<?php 
				/*
				 * REGRA : Não listar PIs sem subações
				 * Solicitado por Henrique Xavier (13/04/09)
				 */

				$sql = "SELECT distinct
							'<center><input class=\"normal\" type=\"checkbox\" id=\"plicod[' || pi.plicod || ']\"  name=\"plicod\" value=\"' || pi.plicod || '\" onclick=\"retorna(\'' || pi.plicod || '\');\"></center>' as acao,
							'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostradadospi(\'' || pi.plicod || '\',  \'cadastro\');\">' || pi.plicod || '</a>',
							CASE WHEN 
								trim(pi.plititulo) is not null 
							THEN 
								'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostradadospi(\'' || pi.plicod || '\', \'cadastro\');\">' || pi.plititulo  || '</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\"' || pi.plicod || ' - ' || pi.plititulo || '\">'
							ELSE 
								'<a title=\"Visualizar Dados\" style=\"cursor:pointer;\" onclick=\"mostradadospi(\'' || pi.plicod || '\',  \'cadastro\');\">Não Preenchido</a><input type=\"hidden\" id=\"plititulo[' || pi.plicod || ']\" value=\" ' || pi.plicod ||' - Não Preenchido\"/>' END as titulo,
							CASE WHEN pi.obrid IS NOT NULL THEN ent.entnome ELSE e.entnome END,
							to_char(pi.plidata, 'DD/MM/YYYY') as plidata,
							CASE WHEN 
								pi.plisituacao = 'P' 
								THEN 
									' <font color=\"red\">Pendente</font> '
								WHEN
									pi.plisituacao = 'C' 
								THEN 
									' <font color=\"green\">Aprovado</font> '
								WHEN
									pi.plisituacao = 'H' 
								THEN 
									' <font color=\"blue\">Homologado</font> '
								WHEN
									pi.plisituacao = 'V' 
								THEN 
									' <font color=\"#3F85FF\">Revisado</font> '
								WHEN
									pi.plisituacao = 'S' 
								THEN 
									' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
								WHEN
									pi.plisituacao = 'R' 
								THEN 
									' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
							to_char(max(pih.pihdata), 'DD/MM/YYYY') as plidata_alteracao	
						FROM
							monitora.planointerno pi
						INNER JOIN monitora.subacao sa ON sa.sbaid = pi.sbaid 
						LEFT JOIN obras.obrainfraestrutura obr ON obr.obrid = pi.obrid 
						LEFT JOIN entidade.entidade ent ON ent.entid = obr.entidunidade 
						LEFT JOIN monitora.planointernoatividade pia ON pia.pliid = pi.pliid
						LEFT JOIN pde.atividade ati ON ati.atiid = pia.atiid 
						LEFT JOIN entidade.entidade e ON e.entid = ati.entid 
						LEFT JOIN monitora.planointernohistorico pih ON pi.plicod = pih.plicod 
						LEFT JOIN monitora.subacaounidade sbu ON sbu.sbaid = sa.sbaid 
						LEFT JOIN entidade.entidade ent2 ON ent2.entid = sbu.entid  
						LEFT JOIN monitora.ptres ptres ON ptres.ptres = pi.pliptres
						WHERE pi.plistatus = 'A' " . ( !empty($filtro) ? "AND" . implode(" AND ", $filtro) : '' ) . "
						GROUP BY pi.plicod,pi.plititulo,e.entnome,pi.plidata,pi.plisituacao,pi.obrid,ent.entnome
						ORDER BY plidata";
				//dbg($sql,1);
				$cabecalho = array("Ação", "Código", "Título", "Unidade", "Data de Inclusão", "Situação", "Data de Alteração");
				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'N' );
				
				$RS = $db->carregar($sql);
				$nlinhas = count($RS);
				$nl = !$RS ? 0 : $nlinhas;
				$reg_fim = (($numero + $perpage) > $nlinhas) ? $nlinhas : ($numero + $perpage -1);
				$total_reg = $nlinhas;
			?>
		</div>
	</center>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
		<tr>
			<td colspan="2">
				<select multiple size="6" name="selectpliid[]" id="selectpliid" style="width:100%;" class="CampoEstilo" onchange="moveto(this);">
					<option value="">-- Selecione os Planos Internos na Listagem Acima --</option>
				</select>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td align="right">
			<?
			if($_REQUEST['plisituacao'] == "") {
				echo "<font style=\"color:red;\">Para alterar a situação em lote, use o filtro de \"Situação\".</font>";
			} else {
				// CPF's do Paulo Rocha, Wesley Washington e Leo Kessel.
				if($_SESSION['usucpf'] == '' || 
				   $_SESSION['usucpf'] == '' || 
				   $_SESSION['usucpf'] == '' || 
				   $_SESSION['usucpf'] == '' || 
				   $_SESSION['usucpf'] == '' ||
 				   $_SESSION['usucpf'] == '' ||
				   $_SESSION['usucpf'] == '' ||
				   $_SESSION['usucpf'] == '' ||
				   $_SESSION['usucpf'] == '' ||
				   $_SESSION['usucpf'] == '' ||
				   $_SESSION['usucpf'] == '') {
					echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera_situacao('H');\" style=\"cursor: pointer;\" />
						  <input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera_situacao('R');\" style=\"cursor: pointer;\" />
						  <input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera_situacao('C');\" style=\"cursor: pointer;\" />
						  <input type=\"button\" value=\"Cadastrado no SIAFI\" onclick=\"altera_situacao('S');\" style=\"cursor: pointer;\" />";
				} 
				else {
					switch($_REQUEST['plisituacao']) {
						case "P":
							echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera_situacao('H');\" style=\"cursor: pointer;\" />
								  <input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera_situacao('R');\" style=\"cursor: pointer;\" />";
							break;
						case "C":
							echo "<input type=\"button\" value=\"Cadastrado no SIAFI\" onclick=\"altera_situacao('S');\" style=\"cursor: pointer;\" />";
							break;
						case "H":
							if($_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '') {
								echo "<input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera_situacao('C');\" style=\"cursor: pointer;\" />";
							}
							/*
							 * REGRA 0001
							 * Checando no botão de Enviar para revisão
							 */
							if(!$regra_0001['btn_enviadorevisao_disabled'])
								echo "<input type=\"button\" value=\"Enviar para Revisão\" onclick=\"altera_situacao('R');\" style=\"cursor: pointer;\"/>";
								
							break;
						case "R":
							if($_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
							   $_SESSION['usucpf'] == '' || 
		   					   $_SESSION['usucpf'] == '' ||
							   $_SESSION['usucpf'] == '' ||
							   $_SESSION['usucpf'] == '' ||
							   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '' ||
					   		   $_SESSION['usucpf'] == '') {
								echo "<input type=\"button\" value=\"Alterar para Aprovado\" onclick=\"altera_situacao('C');\" style=\"cursor: pointer;\" />";
							}
							if(!$regra_0001['btn_alterahomologado_disabled'])
								echo "<input type=\"button\" value=\"Alterar para Homologado\" onclick=\"altera_situacao('H');\" style=\"cursor: pointer;\" />";
							break;
						case "V":
							echo "&nbsp;";
							break;
						case "S":
							echo "&nbsp;";
							break;
					}
				}
			}
			?>
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

	function altera_situacao( situacao ){
	
		var pliid = document.formulario.selectpliid;
		var arPis = new Array();
		
		for ( i = 0; i < pliid.options.length; i++  ){
			if ( pliid.options[i].value != "" ){
				arPis[i] = pliid.options[i].value;
			}
		}
		
		if ( arPis.length < 1 ){
			alert('Favor selecionar ao menos um Plano Interno!');
			return false;
		}
		
		if ( situacao == 'R' ){
		
			window.open('?modulo=principal/planointerno/obspirecusado&acao=A&pi=' + arPis,'Observação do PI','width=520, height=400, scrollbars=yes');
			
		}else{
		
			var url = '?modulo=principal/planointerno/gerenciarpi&acao=A&ajax=1';
			var parametros = "&situacao=" + situacao + "&pi=" + arPis;
			
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					parameters: parametros,
					asynchronous: false,
					onComplete: function(resp) {
						alert(resp.responseText);
						document.pesquisa.submit();
						//document.location.reload();
					}
				}
			);
		
		}
	}

	

	function retorna(objeto){
		var campoSelect = document.formulario.selectpliid;
	
		var checado = document.getElementById('plicod['+objeto+']');
		var titulo  = document.getElementById('plititulo['+objeto+']');
		
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
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('-- Selecione os Planos Internos na Listagem Acima --', '', false, false);}
			sortSelect(campoSelect);
		}
	}
	
	function moveto(obj) {
		document.getElementById('plicod[' + obj.value + ']').focus();
	}
	
	function marcatodas(){
		
		document.getElementById('mdtodos').innerHTML = '<a href="javascript:desmarcatodas();"><input type="checkbox" name="chktodos" id="chktodos"> Desmarcar todos</a>';
		document.getElementById('chktodos').checked = true;
		
		var plicod = document.getElementsByName( 'plicod' );
		
		for( var i=0; i <= plicod.length-1; i++ ){
			if ( plicod[i].checked == false && plicod[i].value != "" ){
				plicod[i].checked = true;
				retorna(plicod[i].value);
			}
		}
	}
	
	function desmarcatodas(){
		
		document.getElementById('mdtodos').innerHTML = '<a href="javascript:marcatodas();"><input type="checkbox" name="chktodos" id="chktodos"> Marcar todos</a>';
		document.getElementById('chktodos').checked = false;
		
		var plicod = document.getElementsByName( 'plicod' );
		
		for( var i=0; i <= plicod.length-1; i++ ){
			if ( plicod[i].checked == true ){
				plicod[i].checked = false;
				retorna(plicod[i].value);
			}
		}
	}	

	function pagina( numero ){
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}

</script>
