<?php
function temPerfilEstadual(){
	global $db; 
	$sql = "select estuf from pdeescola.usuarioresponsabilidade 
	where pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO." 
	and usucpf = '".$_SESSION['usucpf']."'
	and rpustatus = 'A'"; 
	$estuf = $db->pegaUm( $sql ); 
	if( $estuf )
		return $estuf;
	else
		return false;
}
 

function temPerfilMunicipal(){
	global $db; 
	$sql = "select muncod from pdeescola.usuarioresponsabilidade 
	where pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO." 
	and usucpf = '".$_SESSION['usucpf']."'
	and rpustatus = 'A'"; 
	 
	$muncod = $db->pegaUm( $sql ); 
	if( $muncod )
		return $muncod;
	else
		return false;
}
 
if(isset($_POST['ajaxestuf'])) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_POST['ajaxestuf'] != "")
		$where = "where estuf = '".$_POST['ajaxestuf']."'";
	else
		$where = "";
	
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			".$where."
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '', '', 215 ));
}

if(isset($_POST["ajaxregcod"])) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_POST['ajaxregcod'] != "")
		$where = "where regcod = '".$_POST['ajaxregcod']."'";
	else
		$where = "";
	
	$sql = "select
			 estuf as codigo, estdescricao as descricao 
			from
			 territorios.estado
			".$where."
			order by
			 estdescricao asc";
	die($db->monta_combo( "estuf", $sql, 'S', 'Selecione um Estado', 'recuperaMunicipio', '', '', 215 ));
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Mais Educação', 'Estimativa da Educação Integral' );
 
?>

<?php
	if(  !$_SESSION['bo_cadastrador_mais_escola'] ){
?>
	
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<form id="formRelatorioEstimativa" method="post" action="">
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<?php if( !($estuf = temPerfilEstadual()) && !( $muncod = temPerfilMunicipal())) {?>
			<tr>
				<td class="SubTituloDireita" valign="top">Região</td>
				<td>
					<?php
					$sql = "select
							 regcod as codigo, regdescricao as descricao 
							from
							 territorios.regiao
							order by
							 regdescricao asc";
					$db->monta_combo( "regcod", $sql, 'S', 'Selecione uma Região', 'recuperaEstado', '', '', "215", 'S' );
					?>
				</td>
			</tr>
		<?php } ?>
		<?php if(  !( $muncod = temPerfilMunicipal() ) ) { ?>
			<tr>
				<td class="SubTituloDireita" valign="top">Estado</td>
				<td id="estado">
					<?php
					$sql = "select
							 e.estuf as codigo, e.estdescricao as descricao 
							from
							 territorios.estado e 
							order by
							 e.estdescricao asc";
					if( $estuf = temPerfilEstadual())
						$disabled = "N";
					else
						$disabled = "S";
						 
					$db->monta_combo( "estuf", $sql, $disabled, 'Selecione um Estado', 'recuperaMunicipio', $disabled, '', "215" );
					?>
				</td>
			</tr>
		<?php }   ?>
			<tr>
				<td class="SubTituloDireita" valign="top">Município</td>
				<td id="municipio">
					<?
						$sql = "select
								 muncod as codigo, 
								 mundescricao as descricao 
								from
								 territorios.municipio
								order by
								 mundescricao asc";
						if( $muncod = temPerfilMunicipal())
							$disabled = "N";
						else
							$disabled = "S";
			 
						$db->monta_combo( "muncod", $sql, $disabled, 'Selecione um Município', '', '', '', "215" );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Tipo Classificação da Escola</td>
				<td>
					<?php 
						$arrRuralUrbana = array(
											//array('codigo' => 'T', 'descricao' => 'Todos'),
											array('codigo' => 'R', 'descricao' => 'Rural'),
											array('codigo' => 'U', 'descricao' => 'Urbana')
										);
						$db->monta_combo("memclassificacaoescola", $arrRuralUrbana, "S", "Todas", "", "", "", "215", "N", "memclassificacaoescola");
					?>
				</td>
			</tr>
					
			<tr>
				<td class="SubTituloDireita" valign="top">Tipo</td>
				<td>
					<?
						$habil = 'S';
						
						if( temPerfilEstadual() )
						{
							$tpcid = 1;
							$habil = 'N';
						}
						if( temPerfilMunicipal() )
						{
							$tpcid = 3;
							$habil = 'N';
						}
						
						$sql = "SELECT
									'1,3' AS codigo,
									'Todas' AS descricao
								UNION ALL
								SELECT
									'1' AS codigo,
									'Estadual' AS descricao									
								UNION ALL
								SELECT 
									'3' AS codigo,
									'Municipal' AS descricao";
						$db->monta_combo( "tpcid", $sql, $habil, 'Selecione o Tipo', '', '', '', "215", 'S' );
					?>
				</td>
			</tr>
		<?php if( !($estuf = temPerfilEstadual()) && !( $muncod = temPerfilMunicipal())) { ?> 
			<tr>
				<td class="SubTituloDireita" valign="top">Ano do Exercício</td>
				<td>
					<?
						// Recupera o perfil do usuário no Mais Educação.
						$usuPerfil = arrayPerfil();
							
						$sql = "SELECT
									prsano AS codigo,
									prsano AS descricao
								FROM
									pdeescola.programacaoexercicio
								WHERE
									prsstatus = 'A'
								ORDER BY
									prsano";
						$db->monta_combo( "ano", $sql, 'S', 'Selecione o Ano do Exercício', '', '', '', 215, 'S' );
					?>
				</td> 
			</tr>
		 
			<tr>
				<td class="SubTituloDireita" valign="top">Modalidade de Ensino</td>
				<td>
					<?
						$sql = "SELECT
									'F' AS codigo,
									'Ensino Fundamental' AS descricao
								UNION ALL
								SELECT
									'M' AS codigo,
									'Ensino Médio' AS descricao";
						$db->monta_combo( "modalidade", $sql, 'S', 'Selecione a Modalidade de Ensino', '', '', '', 215, 'S' );
					?>
				</td>
			</tr>
		 
			<tr>
				<td class="SubTituloDireita" valign="top">Situação</td>
				<td>
				<?
					$sql = "SELECT
							 esdid AS codigo,
							 esddsc AS descricao
							FROM
							 workflow.estadodocumento et
							INNER JOIN 
							 workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND sisid = ".$_SESSION['sisid']."
							WHERE 
							 et.tpdid = ".TPDID_MAIS_EDUCACAO." --AND
							 --esdid in (34,39,40,876)
							ORDER BY
							 esdordem;";
					$situacao = $db->carregar($sql);
					//$db->monta_combo( "esdid", $sql, 'S', 'Selecione a Situação', '', '', '', 215 );
					
					echo '<input type="checkbox" name="esdid[]" value="null"> Não Iniciado<br>';
					for($i=0; $i<count($situacao); $i++) {
						echo '<input type="checkbox" name="esdid[]" value="'.$situacao[$i]["codigo"].'" /> '.$situacao[$i]["descricao"].'<br />';
					}
				?>
				</td>
			</tr>
		 
			<tr>
				<td class="SubTituloDireita" valign="top">Valor</td>
				<td>
				<?
					echo campo_texto('valor', 'N', 'S', '', 19, 19, '###.###.###,##', '');
					echo "&nbsp;&nbsp;";
					echo "<select name='operador' class='CampoEstilo' style='width:99px;'>
							<option value='maior'>Maior (>)</option>
							<option value='menor'>Menor (<)</option>
						  </select>";
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">PST</td>
				<td>
					<input type="radio" name="pst" value="aderiu" /> Aderiu
					<input type="radio" name="pst" value="naoaderiu" /> Não Aderiu
					<input type="radio" name="pst" value="naoinformado" /> Não Informado
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Escolas que participaram no ano anterior?</td>
				<td>
					<input type="checkbox" name="anoanterior" />Sim
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">N° de Atividades calculadas</td>
				<td>
					<input type="radio" name="num_atividades_calculadas" value="5" />5&nbsp;
					<input type="radio" name="num_atividades_calculadas" value="6" checked />6&nbsp;
					<input type="radio" name="num_atividades_calculadas" value="7" />7&nbsp;
					<input type="radio" name="num_atividades_calculadas" value="8" />8&nbsp;
					<input type="radio" name="num_atividades_calculadas" value="9" />9&nbsp;
					<input type="radio" name="num_atividades_calculadas" value="10" />10
				</td>
			</tr>
		<?php } ?>
			<tr>
				<td class="SubTituloDireita" valign="top">Escola(s)</td>
				<td>
					<?
					$estuf = ($estuf) ? " AND estuf = '".$estuf."'" : "";
					$muncod = ($muncod) ? " AND muncod = '".$muncod."'" : "";
					
					$sql_combo = "SELECT
									ent.entid as codigo,
									ent.entnome as descricao
								FROM 
									entidade.entidade ent
								INNER JOIN
									pdeescola.memaiseducacao mme ON mme.entid = ent.entid 
																AND mme.memanoreferencia = " . $_SESSION["exercicio"] . "
																AND mme.memstatus = 'A'
								INNER JOIN
									entidade.endereco ende ON ende.entid = ent.entid
															".$estuf."
															".$muncod."
								ORDER BY
									ent.entnome";
					
					combo_popup( 'entid', $sql_combo, 'Selecione a(s) Escola(s) desejada(s)', '400x400', 0, array(), '', 'S', false, true, 10, 400);
					?>
				</td>
			</tr>
			<tr>
				<td align="center" colspan="2" bgcolor="#c0c0c0">
					<input type="button" id="relatorio" value="Gerar Relatório" onclick="javascript:geraRelatorio('html');" />
					&nbsp;
					<input type="button" id="planilha" value="Gerar Planilha" onclick="javascript:geraRelatorio('xls');" />
				</td>
			</tr>
		</table>
	</form>
<?
}
?>

<script type="text/javascript">

function geraRelatorio(tipo) {
	var regcod		= document.getElementsByName("regcod");
	var tpcid		= document.getElementsByName("tpcid");
	var ano			= document.getElementsByName("ano");
	var modalidade	= document.getElementsByName("modalidade");
	var form 		= document.getElementById("formRelatorioEstimativa");
	
	<?php if( !($estuf = temPerfilEstadual()) && !( $muncod = temPerfilMunicipal())) { ?>

		if(regcod[0].value == "") {
			alert("Você deve selecionar a Região.");
			regcod[0].focus();
			return;
		}

		
		if(tpcid[0].value == "") {
			alert("Você deve selecionar o Tipo.");
			tpcid[0].focus();
			return;
		}
		if(ano[0].value == "") {
			alert("Você deve selecionar o Ano do Exercício.");
			ano[0].focus();
			return;
		}
		if(modalidade[0].value == "") {
			alert("Você deve selecionar a Modalidade de Ensino.");
			modalidade[0].focus();
			return;
		}
	<? } ?>	
	selectAllOptions( form.entid );
		
	form.action	= 'pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&tipo=' + tipo + '';
	var janela 	= window.open( '', 'relatorio', 'width=940,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	form.target = 'relatorio';
	form.submit();
}

function recuperaMunicipio(estuf) {
	var tdMunicipio	= document.getElementById('municipio');
	
	var req = new Ajax.Request('pdeescola.php?modulo=merelatorio/merelatorio_estimativa&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {
										tdMunicipio.innerHTML = res.responseText;
							        }
							  });
}

function recuperaEstado(regcod) {
	var tdEstado = document.getElementById('estado');
	
	var req = new Ajax.Request('pdeescola.php?modulo=merelatorio/merelatorio_estimativa&acao=A', {
							        method:     'post',
							        parameters: '&ajaxregcod=' + regcod,
							        onComplete: function (res)
							        {
										tdEstado.innerHTML = res.responseText;
							        }
							  });
							  
	recuperaMunicipio('');
}

</script>