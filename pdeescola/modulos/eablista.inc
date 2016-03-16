<?

/*** Temporário, pois as escolas do EAB ainda estão em 2011... ***/
//$_SESSION["exercicio"] = 2011;

unset($_SESSION['eabid']);
unset($_SESSION['entid']);

?>

<!-- Inclue o JQuery -->
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script src="./js/eabajax.js" type="text/javascript"></script>

<?php

if ( !pdeescola_possui_perfil(PDEESC_PERFIL_SUPER_USUARIO) && pdeescola_possui_perfil(PDEESC_PERFIL_CAD_ESCOLA_ABERTA) ){
	$_SESSION['bo_cadastrador_escola_aberta'] = true;	
	$entidade = pdeescola_pegaescola($_SESSION['usucpf']);
?>
	<script>
		redirecionaEAB('eabajax.php', 'tipo=redirecionaeab&entid='+<?php echo $entidade;?>);
	</script>							  	
<?php 
}


/*unset($_SESSION['pdeid']);
if ( selecionarEntidade($_GET['entid']) ){
	header( "Location: pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A" );
	exit();
}*/

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

echo '<br/>';
echo montarAbasArray(carregaAbasEscolaAberta(), "/pdeescola/pdeescola.php?modulo=eablista&acao=E");

?>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
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
										$tpcid = $_POST['tpcid'];
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
											 WHERE et.tpdid = ".TPDID_ESCOLA_ABERTA."
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
									$estuf = $_REQUEST['estuf'];
									
									$sql = "select
											 e.estuf as codigo,
											 e.estdescricao as descricao 
											from
											 territorios.estado e 
											order by
											 e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								<td valign="bottom" id="municipio" style="visibility:<?= $_REQUEST['estuf'] ? 'visible' : 'hidden'  ?>;">
									Município
									<br/>
									<?
									if ($_REQUEST['estuf']) {
										
										$muncod = $_REQUEST['muncod'];
										
										$sql = "select
												 muncod as codigo, 
												 mundescricao as descricao 
												from
												 territorios.municipio
												where
												 estuf = '".$_REQUEST['estuf']."' 
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
							</tr>
							<tr>
								<td><br/></td>
							</tr>
						</table>
						<div style="float: left;">
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
						</div>
					</div>	
				</form>
			</td>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align:middle; text-align:center;">
				<div style="background-color:#f4f4f4; width:100%; border:1px solid #a0a0a0;">
					<br />
					<b><font color="#000000">Para imprimir o Plano Consolidado clique no botão abaixo:</font></b><br /><br />
					<input type="button" id="btGeralConsolidado" name="btGeralConsolidado" value="Relatório Geral Consolidado" />
					<br /><br /><br />
				</div>
			</td>
		</tr>
	</tbody>
</table>
<? eablista(); ?>

<script type="text/javascript">
<!--

jQuery.noConflict();

/*** Quando o DOM estiver pronto ***/
jQuery(document).ready(function()
{
	/*** Redireciona para a página do relatório ***/
	jQuery("#btGeralConsolidado").click(function()
	{
		window.location = 'pdeescola.php?modulo=eabrelatorio/relatorio_plano_atendimento&acao=A';
	});
}); 

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
	var req = new Ajax.Request('pdeescola.php?modulo=eablista&acao=E', {
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
  document.formulario.submit();
  
}
 
</script>
