<?

if( !$_SESSION["exercicio"] )
{
	echo '<script>
			/*** Exibe o alerta de erro ***/
			alert("Ocorreu um erro interno.\n
			       O sistema irá redirecioná-lo à página inicial do módulo.");
			       
			/*** Redireciona o usuário ***/
			location.href = "pdeescola.php?modulo=inicio&acao=C";
		  </script>';
	die();
}

if( $_REQUEST['pesquisa'] )
{
	unset($_SESSION['maiseducacao']['filtro']);
	
	if($_POST['tpcid'])
	{
		if($_POST['tpcid'] == "1" && $_POST['estuf'])
		{
			$_SESSION['maiseducacao']['filtro']['tpcid'] = $_POST['tpcid'];
			$_SESSION['maiseducacao']['filtro']['estuf'] = $_POST['estuf'];
		}
		if($_POST['tpcid'] == "3" && $_POST['estuf'])
		{
			$_SESSION['maiseducacao']['filtro']['tpcid'] = $_POST['tpcid'];
			$_SESSION['maiseducacao']['filtro']['estuf'] = $_POST['estuf'];
			$_SESSION['maiseducacao']['filtro']['muncod'] = $_POST['muncod'];
		}
	}
}

if ($_POST['ajaxinativar']) {
	$sql = "UPDATE pdeescola.memaiseducacao SET memstatus = 'I' WHERE memid = ".$_POST['memid'];
	$db->executar($sql);
	$db->commit();
	die("Escola inativada com sucesso.");
}

if ($_POST['ajaxativar']) {
	$sql = "UPDATE pdeescola.memaiseducacao SET memstatus = 'A' WHERE memid = ".$_POST['memid'];
	$db->executar($sql);
	$db->commit();
	die("Escola ativada com sucesso.");
}

unset($_SESSION['memid']);
unset($_SESSION['meentid']);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script src="./js/meajax.js" type="text/javascript"></script>
<?php

if ( !pdeescola_possui_perfil(PDEESC_PERFIL_SUPER_USUARIO) && pdeescola_possui_perfil(PDEESC_PERFIL_CAD_MAIS_EDUCACAO) ){
	$_SESSION['bo_cadastrador_mais_escola'] = true;	
	$entidade = pdeescola_pegaescola($_SESSION['usucpf']);
?>
	<script>
		redirecionaME('meajax.php', 'tipo=redirecioname&entid='+<?php echo $entidade;?>);
	</script>							  	
<?php 
}



unset($_SESSION['entid']);
unset($_SESSION['pdeid']);

if ( selecionarEntidade($_GET['entid']) ){
	header( "Location: pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A" );
	exit();
}

if ($_POST['ajaxestuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '' ));
}

include APPRAIZ . 'includes/cabecalho.inc';
#include APPRAIZ . 'includes/Agrupador.php';

echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=melista&acao=E");

?>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/meajax.js" type="text/javascript"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="pesquisa" value="1" />
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						<table width="100%" border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Escola:
									<br/>
									<?php $escola = simec_htmlentities( $_REQUEST['escola'] ); ?>
									<?= campo_texto( 'escola', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Código Escola:
									<br/>
									<?php $entcodent = simec_htmlentities( $_REQUEST['entcodent'] ); ?>
									<?= campo_texto( 'entcodent', 'N', 'S', '', 30, 200, '', '' ); ?>
								</td>								
								<td>
									Tipo:
									<br>
									<?php
										if( $_POST['tpcid'] )
											$tpcid = $_POST['tpcid'];
										elseif( $_SESSION['maiseducacao']['filtro']['tpcid'] )
											$tpcid = $_SESSION['maiseducacao']['filtro']['tpcid'];
										
										$sql = sprintf("SELECT
															'1,3' AS codigo,
															'Todas' AS descricao
														UNION ALL
														SELECT
															'1' AS codigo,
															'Estadual' AS descricao									
														UNION ALL
														SELECT 
															'3' AS codigo,
															'Municipal' AS descricao");
										$db->monta_combo( "tpcid", $sql, 'S', 'Selecione...', '', '' );
									?>				
								</td>
								<?
									// Recupera o perfil e testa se pode visualizar relatório geral consolidado.
									$usuPerfil = arrayPerfil();
									
									if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil) || 
									   in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil) ||
									   in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || 
									   in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
									   	
									   	
									   	$mostraBotaoRel = true;
									   	
									   	if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
									   	
									   		$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO;
											$pflcodx = $db->carregar($sql);
											
											//verifica total de escolas do municipio 
											$sql1 = "SELECT DISTINCT
														count(maedu.memid) as total
													FROM
														entidade.entidade e
													INNER JOIN 
														entidade.endereco ee ON e.entid = ee.entid
													INNER JOIN 
														entidade.endereco endi ON endi.endid = ee.endid
													INNER JOIN 
														territorios.municipio m ON m.muncod = endi.muncod
													INNER JOIN 
														pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
																					  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
																					  AND maedu.memstatus = 'A'
													LEFT JOIN 
														workflow.documento d ON d.docid = maedu.docid
													LEFT JOIN 
														workflow.estadodocumento est ON est.esdid = d.esdid
													WHERE
														m.muncod = '".$pflcodx[0]["muncod"]."' AND 
														e.tpcid IN (3)";
											$total1 = $db->pegaUm($sql1);
												
											//verifica total de escolas do municipio - 34=finalizado , 39=Relatório consolidado emitido ou 40=Enviado ao FNDE 
											$sql2 = "SELECT DISTINCT
														count(maedu.memid) as total
													FROM
														entidade.entidade e
													INNER JOIN 
														entidade.endereco ee ON e.entid = ee.entid
													INNER JOIN 
														entidade.endereco endi ON endi.endid = ee.endid
													INNER JOIN 
														territorios.municipio m ON m.muncod = endi.muncod
													INNER JOIN 
														pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
																					  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
																					  AND maedu.memstatus = 'A'
													LEFT JOIN 
														workflow.documento d ON d.docid = maedu.docid
													LEFT JOIN 
														workflow.estadodocumento est ON est.esdid = d.esdid
													WHERE
														m.muncod = '".$pflcodx[0]["muncod"]."' AND 
														e.tpcid IN (3) AND
														est.esdid in (34,39,40,876)";
											$total2 = $db->pegaUm($sql2);
											
										   	if($total1 != $total2 || !$pflcodx[0]["muncod"]){
												$mostraBotaoRel = false;
											}
											
									   	}else
									   	
									   	
									   if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
									   	
									   		$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO;
											$pflcodx = $db->carregar($sql);
											
											//verifica total de escolas do municipio 
											$sql1 = "SELECT DISTINCT
														count(maedu.memid) as total
													FROM
														entidade.entidade e
													INNER JOIN 
														entidade.endereco ee ON e.entid = ee.entid
													INNER JOIN 
														entidade.endereco endi ON endi.endid = ee.endid
													INNER JOIN 
														territorios.municipio m ON m.muncod = endi.muncod
													INNER JOIN 
														pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
																					  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
																					  AND maedu.memstatus = 'A'
													LEFT JOIN 
														workflow.documento d ON d.docid = maedu.docid
													LEFT JOIN 
														workflow.estadodocumento est ON est.esdid = d.esdid
													WHERE
														m.estuf = '".$pflcodx[0]["estuf"]."' AND 
														e.tpcid IN (1)";
											$total1 = $db->pegaUm($sql1);
												
											//verifica total de escolas do municipio - 34=finalizado , 39=Relatório consolidado emitido ou 40=Enviado ao FNDE 
											$sql2 = "SELECT DISTINCT
														count(maedu.memid) as total
													FROM
														entidade.entidade e
													INNER JOIN 
														entidade.endereco ee ON e.entid = ee.entid
													INNER JOIN 
														entidade.endereco endi ON endi.endid = ee.endid
													INNER JOIN 
														territorios.municipio m ON m.muncod = endi.muncod
													INNER JOIN 
														pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
																					  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
																					  AND maedu.memstatus = 'A'
													LEFT JOIN 
														workflow.documento d ON d.docid = maedu.docid
													LEFT JOIN 
														workflow.estadodocumento est ON est.esdid = d.esdid
													WHERE
														m.estuf = '".$pflcodx[0]["estuf"]."' AND 
														e.tpcid IN (1) AND
														est.esdid in (34,39,40,876)";
											$total2 = $db->pegaUm($sql2);
											
										   	if($total1 != $total2 || !$pflcodx[0]["estuf"]){
												$mostraBotaoRel = false;
											}
											
									   	}
									   	
									?>
									<td rowspan="4" align="center" valign="center" width="1000px">
									<div style="background-color:#f4f4f4; width:50%; border: 1px solid #a0a0a0;">
										<br />
										<b><font color="#000000">Para imprimir o Plano Consolidado clique no botão abaixo:</font></b><br /><br />
										<?if($mostraBotaoRel){?>
											<input type="button" value="Relatório Geral Consolidado" onclick="abreRelGeralConsolidado();" />
										<?}else{?>
											<input type="button" value="Relatório Geral Consolidado" onclick="alert('É necessário que todas as escolas estejam com a situação Finalizado.');" />
										<?}?>
										<br /><br /><br />
									</div>
									</td>
								<?}?>
							</tr>
							<tr>
								<td>
									Situação:
									<br>
									<?php
									$esdid = $_REQUEST['esdid'];
									$sql = "SELECT
											 esdid AS codigo,
											 esddsc AS descricao
											FROM
											 workflow.estadodocumento et
											 INNER JOIN workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND
											 	sisid = ".$_SESSION['sisid']."
											 WHERE et.tpdid = ".TPDID_MAIS_EDUCACAO."
											ORDER BY
											 esdordem;";
									
									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );
									?>
									<script type="text/javascript">
										var esdid 	= 	document.getElementsByName("esdid");
										var option	=	document.createElement('option');
										
										option.text = "Não Iniciado";
										option.value = "naoiniciado";
										
										try {
										  esdid[0].add(option,null); // standards compliant
										} catch(ex) {
										  esdid[0].add(option); // IE only
										}
										
										if("<?=$esdid?>" == "naoiniciado") {
											esdid[0].options[5].selected = true;
										}
									</script>
								</td>
								<!--<td>
									Situação:
									<br>
									<?php
									/*$esdid = $_REQUEST['esdid'];
									$sql = "SELECT 
											mesid as codigo,
											mesdescricao as descricao
											 FROM pdeescola.mesituacao order by mesdescricao";
									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );*/
									?>
								</td>
								--><!-- td valign="bottom">
									Preenchimento:
									<br/>
									<?php /*$preenchimento =  $_REQUEST['preenchimento1'] ;
									campo_texto( 'preenchimento1', 'N', 'S', '', 10, 10, '', '' );*/ ?>
									Até:
									<?php /*$preenchimento =  $_REQUEST['preenchimento2'];
									 campo_texto( 'preenchimento2', 'N', 'S', '', 10, 10, '', '' );*/ ?>
								</td -->
								<td valign="bottom">
									Estado
									<br/>
									<?php
									if($_REQUEST['estuf'])
										$estuf = $_REQUEST['estuf'];
									elseif( $_SESSION['maiseducacao']['filtro']['estuf'] )
										$estuf = $_SESSION['maiseducacao']['filtro']['estuf'];
									
									$sql = "select
											 e.estuf as codigo, e.estdescricao as descricao 
											from
											 territorios.estado e 
											order by
											 e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								<td valign="bottom" id="municipio" style="visibility:<?= ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf']) ? 'visible' : 'hidden'  ?>;">
									Município
									<br/>
									<?
									if ($_REQUEST['estuf'] || $_SESSION['maiseducacao']['filtro']['estuf'])
									{
										if($_REQUEST['muncod'])
											$muncod = $_REQUEST['muncod'];
										elseif( $_SESSION['maiseducacao']['filtro']['muncod'] )
											$muncod = $_SESSION['maiseducacao']['filtro']['muncod'];
										
										$sql = "select
												 muncod as codigo, 
												 mundescricao as descricao 
												from
												 territorios.municipio
												where
												 estuf = '".$estuf."' 
												order by
												 mundescricao asc";
										$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '' );
									}
									?>	
								</td>								
							</tr>
							<tr>
								<td valign="bottom">
									Modalidade de Ensino
									<br/>
									<?
										$modalidade = $_REQUEST['modalidade'];
										$sql = "SELECT
													'F' AS codigo,
													'Ensino Fundamental' AS descricao									
												UNION ALL
												SELECT 
													'M' AS codigo,
													'Ensino Médio' AS descricao";
										$db->monta_combo( "modalidade", $sql, 'S', 'Selecione...', '', '' );
									?>	
								</td>	
								<td valign="bottom">
									Classificação
									<br/>
									<?
										$classificacao = $_REQUEST['classificacao'];
										$sql = "SELECT
													'U' AS codigo,
													'Urbana' AS descricao									
												UNION ALL
												SELECT 
													'R' AS codigo,
													'Rural' AS descricao
												UNION ALL
												SELECT 
													'A' AS codigo,
													'Aberta' AS descricao
												UNION ALL
												SELECT 
													'J' AS codigo,
													'Jovens de 15 a 17 anos' AS descricao";	
										
										$db->monta_combo( "classificacao", $sql, 'S', 'Selecione...', '', '' );
									?>	
								</td>							
							</tr>
							<tr>
								<td valign="middle" id="usuativo">									
									Usuário Ativo:
									<ul style="margin: 0pt; padding: 0pt;">
										<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="1" name="usuativo" <?php echo ($_REQUEST['usuativo'] == 1) ? 'checked' : '' ?>/> <label>Sim</label></li>
										<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="0" name="usuativo" <?php echo ( $_REQUEST['usuativo'] == 0 && isset($_REQUEST['usuativo']) ) ? 'checked' : '' ?>/> <label>Não</label></li>
									</ul>									
								</td>								
							</tr>	
							<tr>
								<td valign="middle" id="aderiupst">
									Aderiu PST:
									<ul style="margin: 0pt; padding: 0pt;">
									<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="S" name="aderiupst" <?php echo ($_REQUEST['aderiupst'] == S) ? 'checked' : '' ?>/> Sim</li>
									<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="N" name="aderiupst" <?php echo ($_REQUEST['aderiupst'] == N && isset($_REQUEST['aderiupst']) ) ? 'checked' : '' ?>/> Não</li>
									<li style="margin: 0pt; width: 120px; list-style-type: none; float: left;"><input type="radio" value="null" name="aderiupst" <?php echo ($_REQUEST['aderiupst'] == null && isset($_REQUEST['aderiupst'])) ? 'checked' : '' ?>/> Não informado</li>									
									</ul>	
								</td>								
							</tr>
							<!--  
							<tr>
								<td valign="middle" id="aderiupst">
									Tipo de Escola:
									<ul style="margin: 0pt; padding: 0pt;">
									<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="proemi" name="tipoescola" <?php echo ($_REQUEST['escola'] == 'proemi') ? 'checked' : '' ?>/> ProEMI</li>
									<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;"><input type="radio" value="pme" name="tipoescola" <?php echo ($_REQUEST['escola'] == 'pme' && isset($_REQUEST['tipoescola']) ) ? 'checked' : '' ?>/> PME</li>
									<li style="margin: 0pt; width: 120px; list-style-type: none; float: left;"><input type="radio" value="ambos" name="tipoescola" <?php echo ($_REQUEST['escola'] == 'ambos' && isset($_REQUEST['tipoescola'])) ? 'checked' : '' ?>/> ProEMI e PME</li>									
									</ul>	
								</td>								
							</tr>
							-->
							<tr>
								<td valign="middle" id="anoanterior">
									Escolas que participaram no ano anterior?
									<input type="checkbox" name="anoanterior" value="1" <?php echo ($_REQUEST['anoanterior'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<?php 
							if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) ) {
							?>
							<tr>
								<td valign="middle">
									Mostrar escolas inativas?
									<input type="checkbox" name="status" value="1" <?php echo ($_REQUEST['status'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<?php } ?>
							<tr>
								<td valign="middle">
									Escolas com Anexo?
									<input type="checkbox" name="escolasAnexo" value="1" <?php echo ($_REQUEST['escolasAnexo'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<tr>
								<td valign="middle">
									Maioria PBF?
									<input type="checkbox" name="escolasPBF" value="1" <?php echo ($_REQUEST['escolasPBF'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<tr>
								<td><br/></td>
							</tr>
						</table>
						<div style="float: left;">
							<input type="hidden" name="requisicao" id="requisicao" value="">
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
							<input type="button" name="" value="Gerar Excel" onclick="return geraExcel();"/>
						</div>
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<? melista(); ?>
<script type="text/javascript"><!--
d = document;

/*
* Faz requisição via ajax
* Filtra o municipio, atravéz do parametro passado 'estuf'
*/
function filtraTipo(estuf) {
	td     = d.getElementById('municipio');
	select = d.getElementsByName('muncod')[0];
	
	/*
	* se estuf vazio
	* esconde o td do municipio e retorna
	*/
	if (!estuf){
		td.style.visibility = 'hidden';
		return;
	}

	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('pdeescola.php?modulo=lista&acao=E', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			
							        	var inner = 'Município<br/>';
										td.innerHTML = inner+res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    }	
    
                   
function validaForm()
{
  /*var p1 = document.formulario.preenchimento1;
  var p2 =  document.formulario.preenchimento2;
  
  if( isNaN( p1.value )  || isNaN( p2.value ) )
  {
  	   alert('Só é permitido números no filtro por preenchimento');
	   p1.value = '';
	   p2.value = '';
	   return false;
  }*/
  window.document.getElementById('requisicao').value = 'pesquisar';
  document.formulario.submit();
  
}

function geraExcel(){
	window.document.getElementById('requisicao').value = 'excel';
  	document.formulario.submit();  
}
 
function abreRelGeralConsolidado() {
	<?if($_SESSION['exercicio'] == '2012'){?>
		window.location = "pdeescola.php?modulo=meprincipal/meAdesao&acao=A";
	<?}else{?>
		window.location = "pdeescola.php?modulo=merelatorio/relatorio_plano_atendimento&acao=A";
	<?}?>
}

function inativarEscola(memid)
{
	if( confirm("Deseja inativar esta escola?") )
	{
		var req = new Ajax.Request('pdeescola.php?modulo=melista&acao=E', {
								        method:     'post',
								        parameters: '&ajaxinativar=1&memid=' + memid,
								        onComplete: function (res)
								        {			
								        	alert(res.responseText);
								        	self.location.href = self.location.href;
								        }
								  });
	}
}

function ativarEscola(memid)
{
	if( confirm("Deseja ativar esta escola?") )
	{
		var req = new Ajax.Request('pdeescola.php?modulo=melista&acao=E', {
	        method:     'post',
	        parameters: '&ajaxativar=1&memid=' + memid,
	        onComplete: function (res)
	        {			
				alert(res.responseText);
				self.location.href = self.location.href;
	        }
	  });
	}
}

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}
</script>