<?php

// Carrega combo dos municipios
if(isset($_POST['carregar_municipios'])){	
	$sql = "SELECT
			 muncod AS codigo, 
			 mundescricao AS descricao 
			FROM territorios.municipio
			WHERE estuf = '".$_POST['carregar_municipios']."' 
			ORDER BY mundescricao ASC";
	
	$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '' );
	die;	
}

// Inativar escola
if ($_POST['inativar_escola'] || $_POST['ativar_escola']) {
	$status = isset($_POST['ativar_escola']) ? 'A' : 'I';
	$sql = "UPDATE em.emiensinomedioinovador SET emistatus = '{$status}' WHERE emiid = ".$_POST['emiid'];
	$db->executar($sql);
	if($db->commit()){
		die('true');
	}
	die('false');	
}

// Verifica se existe ano de exercicio
if( !$_SESSION["exercicio"] )
{
	echo '<script>
			/*** Exibe o alerta de erro ***/
			alert("Ocorreu um erro interno.\n
			       O sistema irá redirecioná-lo à página inicial do módulo.");
			       
			/*** Redireciona o usuário ***/
			location.href = "em.php?modulo=inicio&acao=C";
		  </script>';
	die;
}

unset($_SESSION['entid']);
unset($_SESSION['emiid']);

if ( selecionarEntidade($_GET['entid']) ){
	header( "Location: em.php?modulo=painel&acao=A" );
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

$abacod_tela = 57522;
$url = 'em.php?modulo=lista&acao=E';
$parametros = null;
$arMnuid = array();

$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);

$estuf  = $_REQUEST['estuf']  ? $_REQUEST['estuf']  : $_SESSION['emi']['filtro']['estuf'];
$muncod = $_REQUEST['muncod'] ? $_REQUEST['muncod'] : $_SESSION['emi']['filtro']['muncod'];

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="post" name="formulario" id="formulario">
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
										elseif( $_SESSION['emi']['filtro']['tpcid'] )
											$tpcid = $_SESSION['emi']['filtro']['tpcid'];
										
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
											 --WHERE et.tpdid = ".TPDID_MAIS_EDUCACAO."
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
								<td valign="bottom">
									Estado									
									<br/>
									<?php
									$ativo = 'S';
									if($_SESSION['estuf']){
										$estuf = $_SESSION['estuf'];
										$ativo = 'N';
									}
									$sql = "SELECT
											 e.estuf as codigo, 
											 e.estdescricao as descricao 
											FROM territorios.estado e 
											ORDER BY e.estdescricao ASC";
									
									$db->monta_combo( "estuf", $sql, $ativo, 'Selecione...', '', '');
									?>
								</td>
								<td valign="bottom" id="td_municipio" style="display:none;">
									Município
									<br/>
									<div id="combo_municipio">
										
									</div>
								</td>								
							</tr>
							<!--  
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
							
							<tr>
								<td valign="middle" id="anoanterior">
									Escolas que participaram no ano anterior?
									<input type="checkbox" name="anoanterior" value="1" <?php echo ($_REQUEST['anoanterior'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							-->
							<?php 
							if( checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR)) ) {
							?>
							<tr>
								<td valign="middle" id="anoanterior">
									Mostrar escolas inativas?
									<input type="checkbox" name="status" value="1" <?php echo ($_REQUEST['status'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<?php } ?>
							<tr>
								<td><br/></td>
							</tr>
						</table>
						<div style="float: left;">
							<input type="button" name="" value="Pesquisar" id="btnPesquisar"/>
						</div>
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
	
$anoAnterior = $_SESSION["exercicio"]-1;	
	
extract($_POST);

$_GET['esdid'] = $_GET['esdid'] == '0' ? 'naoiniciado' : $_GET['esdid'];
$esdid = $esdid ? $esdid : $_GET['esdid'];

if($escola)
	$where[] = "e.entnome iLIKE '%{$escola}%'";

if($entcodent)
	$where[] = "e.entcodent iLIKE '%{$entcodent}%'";	
		
if($estuf || $_SESSION['estuf']){
	$estuf = $estuf ? $estuf : $_SESSION['estuf'];
	$where[] = "m.estuf iLIKE '{$estuf}'";
}	

if($muncod)
	$where[] = "m.muncod = '{$muncod}'";
	
if($esdid)	 	
	$where[] = $esdid != 'naoiniciado' ? "est.esdid = '{$esdid}'" : "emi.docid is null";
	
if($tcpid)
	$where[] = "e.tpcid IN ({$tcpid})";
		
if($usuativo == 1 || $usuativo == 0)
	$whereExterno[] = "ativo = '".($usuativo ? 'Sim' : 'Não')."'";

/*	
if($modalidade)
	$where[] = "emi.emimodalidadeensino = '{$modalidade}'";
*/
		
if($aderiupst)
	$where[] = $aderiupst != 'null' ? "emi.emiadesaopst = '{$aderiupst}'" : "emi.emiadesaopst is null";

if($status)
	$where[] = "emi.emistatus = 'I'";
else
	$where[] = "emi.emistatus = 'A'";
	
if($anoanterior)
	$where[] = "emi.entcodent in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')";

if( $_GET['emianoreferencia'] == $_SESSION["exercicio"] ) {
	$where[] = " emi.entcodent not in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')";
}else if( $_GET['emianoreferencia'] == $anoAnterior ) {
	$where[] = " emi.entcodent in (select mem.entcodent from em.emiensinomedioinovador mem where mem.emianoreferencia = ".$anoAnterior." and mem.emistatus = 'A')";
}
	
$perfil = arrayPerfil();
		
$join = "";
	
	if ( checkPerfil(PERFIL_SEC_ESTADUAL) && checkPerfil(PERFIL_SEC_MUNICIPAL) ){
		$join = " INNER JOIN em.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
					ur.pflcod IN (".implode(',',$perfil).") AND
					ur.usucpf = '".$_SESSION['usucpf']."' AND
					(
					 (ur.muncod = m.muncod AND 
					  e.tpcid = 3) OR
 					 ur.entid  = e.entid OR
 					 (ur.estuf  = m.estuf AND
 					  e.tpcid = 1)
 					)"; 
	} elseif ( checkPerfil(PERFIL_SEC_ESTADUAL) ) {
		$join = " INNER JOIN em.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
					ur.pflcod IN (".implode(',',$perfil).") AND
					ur.usucpf = '".$_SESSION['usucpf']."' AND
					(
 					 ur.entid  = e.entid OR
 					 (ur.estuf  = m.estuf AND
 					  e.tpcid = 1)
 					)";
	} elseif ( checkPerfil(PERFIL_SEC_MUNICIPAL) ) { 
		$join = " INNER JOIN em.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
					ur.pflcod IN (".implode(',',$perfil).") AND
					ur.usucpf = '".$_SESSION['usucpf']."' AND
					(
					 (ur.muncod = m.muncod AND 
					  e.tpcid = 3) OR
 					 ur.entid  = e.entid
 					)";
	}  
     
/**
 * Verifica se o usuário tem perfil de 'Super Usuário' ou 'Administrador'
 * e habilita a opção de alterar o emistatus da escola
 */
if( checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_ANALISTA_SEDUC))){
    	
	$acao = "CASE WHEN emi.emistatus = 'A'
				THEN '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"entid=' || e.entid || '&emiid=' || emi.emiid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"' || emi.emiid || '\"><img class=\"btnInativarEscola\" src=\"/imagens/valida6.gif\" border=0 title=\"Inativar Escola\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"' || e.entid || '\"><img class=\"btnPopupMapa\" src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
				ELSE
    			 	 '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"entid=' || e.entid || '&emiid=' || emi.emiid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"ativarEscola(' || emi.emiid || ')\"><img src=\"/imagens/valida1.gif\" border=0 title=\"Ativar Escola\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"' || e.entid || '\"><img class=\"btnPopupMapa\" src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
				END";
    	
}else{    	
	
	$acao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"entid=' || e.entid || '&emiid=' || emi.emiid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
			  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"' || e.entid || '\" ><img class=\"btnPopupMapa\" src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'";
}
    
$sql = "SELECT * FROM(
					SELECT DISTINCT
						 {$acao} as acao,
						 e.entcodent,
						 e.entnome,
						 CASE WHEN e.tpcid = 1 THEN 'Estadual'
						  	   ELSE 'Municipal'
						 END AS tipo,
						 m.estuf, 
						 m.mundescricao,
						 CASE WHEN est.esdid IS NOT NULL THEN est.esddsc
					  	  	  ELSE 'Não Iniciado' 
					  	 END AS situacao,
						 CASE WHEN ur1.entid is not null THEN 'Sim' 
						 	  ELSE 'Não' 
						 END as ativo,
						 'Médio' as ensino
					FROM entidade.entidade e
					INNER JOIN entidade.endereco endi 		  ON endi.entid = e.entid
					LEFT JOIN territorios.municipio m 		  ON m.muncod = endi.muncod
					LEFT JOIN em.usuarioresponsabilidade ur1  ON ur1.entid = e.entid 
							AND ur1.rpustatus = 'A' 
						 	AND ur1.pflcod = 383						 
					INNER JOIN em.emiensinomedioinovador emi  ON emi.entid = e.entid						 	
					LEFT JOIN workflow.documento d 			  ON d.docid = emi.docid
					LEFT JOIN workflow.estadodocumento est 	  ON est.esdid = d.esdid
					$join
					WHERE emi.emianoreferencia = {$_SESSION["exercicio"]}					
					".($where ? ' AND '.implode(' AND ',$where) : '')."
			) as foo ".($whereExterno ? ' WHERE '.implode(' AND ',$whereExterno) : '');
			
				
//dbg(simec_htmlentities($sql),1);

$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Usuário Ativo", "Ensino");
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');


?>
<script type="text/javascript">
<!--

$(function(){

	$('#btnPesquisar').click(function(){
		$('#formulario').submit();
	});
	
	// Popula municipios
	$('select[name=estuf]').change(function(){
	
		$('#td_municipio').show();
		$('#combo_municipio').html('Carregando...');
		
		$.ajax({
			url: 'em.php?modulo=lista&acao=E',
			type: 'post',
			data: 'carregar_municipios='+this.value,
			success: function(res){		
				$('#combo_municipio').html(res);				
			}
		});	
	});
	
	$('.btnInativarEscola').click(function(){
		if( confirm("Deseja inativar esta escola?") ){
			emiid = $(this).parent().attr('id');			
			$.ajax({
				url: 'em.php?modulo=lista&acao=E',
				type: 'post',
				data: 'inativar_escola=true&emiid='+emiid,
				success: function(res){
					if(res=='true'){
						alert('Escola inativada com sucesso.');
						document.location = document.location.href;
					}else{
						alert('Não foi possível inativar a escola!');
					}
				}
			});
		}
	});
	
	$('.btnPopupMapa').click(function(){
		entid = $(this).parent().attr('id');
		window.open('em.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	});
	
	$('.btnDadosEscola').click(function(){	
		params = $(this).parent().attr('id');		
		document.location = 'em.php?modulo=principal/prcEscola&acao=A&'+params;
	});
	
});

function ativarEscola(emiid)
{
	if( confirm("Deseja ativar esta escola?") )
	{
		$.ajax({
			url: 'em.php?modulo=lista&acao=E',
			type: 'post',
			data: 'ativar_escola=true&emiid='+emiid,
			success: function(e){
				if(e=='true'){
					document.location = 'em.php?modulo=lista&acao=E';
				}
			}
		});
	}
}
</script>