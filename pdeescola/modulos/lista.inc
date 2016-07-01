<?php
 
if ($_GET['entid']) 
{
	if ( selecionarEntidade($_GET['entid']) ){
		header( "Location: pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A" );
		exit();
	}
	else
	{
	echo "<script>alert('Escola não encontrada na lista de Escolas do PDE Escola!');</script>";
	}
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

print '<br/>';


$perfil = arrayPerfil();
if( !in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil ) || !in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL,  $perfil ) ) {
				
	echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=lista&acao=E");		
}

?>
<script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>
<script type="text/javascript" src="/includes/prototype.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link> 

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
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
								<td>
									Classe IDEB:
									<br>
									<?php
										$epiclasse = $_POST['epiclasse'];
										$sql = sprintf("SELECT
															'' AS codigo,
															'Todas' AS descricao
														UNION ALL
														SELECT
															'A' AS codigo,
															'Classe A' AS descricao
														UNION ALL
														SELECT
															'B' AS codigo,
															'Classe B' AS descricao
															UNION ALL
														SELECT
															'C' AS codigo,
															'Classe C' AS descricao");
										$db->monta_combo( "epiclasse", $sql, 'S', 'Selecione...', '', '' );
									?>				
								</td>
								<td>
									PAF:
									<br>
									<?php
										$pafpago = $_POST['pafpago'];
										$sql = sprintf("SELECT
															'Pago' AS codigo,
															'Pago' AS descricao
														UNION ALL
														SELECT
															'--' AS codigo,
															'Não Pago' AS descricao");
										$db->monta_combo( "pafpago", $sql, 'S', 'Selecione...', '', '' );
									?>				
								</td>
								<td>
									Resultados:
									<br>
									<?php
										$eavvalor = $_POST['eavvalor'];
										$sql = sprintf("SELECT
															'' AS codigo,
															'Todas' AS descricao
														UNION ALL
														SELECT
															'A' AS codigo,
															'Muito Crítico' AS descricao
														UNION ALL
														SELECT
															'B' AS codigo,
															'Crítico' AS descricao
															UNION ALL
														SELECT
															'C' AS codigo,
															'Regular' AS descricao
															UNION ALL
														SELECT
															'D' AS codigo,
															'Bom' AS descricao
															UNION ALL
														SELECT
															'E' AS codigo,
															'Ótimo' AS descricao");
										$db->monta_combo( "eavvalor", $sql, 'S', 'Selecione...', '', '' );
									?>				
								</td>
							</tr>
							<tr>
								<td>
									Situação:
									<br>
									<?php
									$esdid = $_REQUEST['esdid'];
									$sql = "(SELECT
											 	'0' AS codigo,
											 	'Não Iniciado' AS descricao
											 )
											UNION
											(SELECT
											 	esdid AS codigo,
											 	esddsc AS descricao
											 FROM
											 	workflow.estadodocumento et
											 INNER JOIN 
											 	workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND sisid = ".$_SESSION['sisid']."
											 WHERE 
											 	et.tpdid = ".TPDID_PDE_ESCOLA." AND esdstatus = 'A'
											ORDER BY esdordem)
											UNION
										    (SELECT 
												esdid as codigo, 
												esddsc as descricao 
											FROM(
												 SELECT 
													CASE
													WHEN (pdepafretorno IS NOT NULL) THEN 'Escolas Pagas'
													ELSE est.esddsc
													END AS esddsc,
													CASE
												 WHEN est.esdid < 0 THEN est.esdid
													ELSE 999999
													END AS esdid, 
													count(*) as count 
											    FROM
													entidade.entidade e
											    INNER JOIN 
													entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
											    LEFT OUTER JOIN 
													pdeescola.pdeescola pde ON pde.entid = e.entid 
											    LEFT OUTER JOIN 
													workflow.documento d ON d.docid = pde.docid
											    LEFT OUTER JOIN 
													workflow.estadodocumento est ON est.esdid = d.esdid 
											    WHERE 
													pdepafretorno IS NOT NULL
											    GROUP BY 
													pde.pdepafretorno, 
													est.esddsc, 
													est.esdid
											 	 ) as tbl1
											GROUP BY esddsc, esdid);";

									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );
									?>
								</td>
								<td valign="bottom">
									Preenchimento:
									<br/>
									<?php $preenchimento =  $_REQUEST['preenchimento1'] ; ?>
									<?= campo_texto( 'preenchimento1', 'N', 'S', '', 10, 10, '', '' ); ?>
									Até:
									<?php $preenchimento =  $_REQUEST['preenchimento2'] ; ?>
									<?= campo_texto( 'preenchimento2', 'N', 'S', '', 10, 10, '', '' ); ?>
								</td>
								<td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_REQUEST['estuf'];
									$sql = "select
											 e.estuf as codigo, e.estdescricao as descricao 
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
								<td colspan="1">
									<input type="checkbox" id="parcela" name="parcela" align="absmiddle">Parcela complementar
								</td>
								<td>
									Período de Tramitação
									<br />
									<?=campo_data2('datatramite_inicio', 'N', 'S', 'Período de Tramitação', '##/##/####')?>
									&nbsp;&nbsp;até&nbsp;&nbsp;
									<?=campo_data2('datatramite_fim', 'N', 'S', 'Período de Tramitação', '##/##/####')?>
								</td>
							</tr>	
						</table>
						<div align="" style="float: left;">
						<br>
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
						</div>	
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<? lista(); ?>
<script type="text/javascript">
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
  var p1 = document.formulario.preenchimento1;
  var p2 =  document.formulario.preenchimento2;

  var data_inicio 	= document.getElementsByName('datatramite_inicio')[0];
  var data_fim 		= document.getElementsByName('datatramite_fim')[0];
  
  if( isNaN( p1.value )  || isNaN( p2.value ) )
  {
  	   alert('Só é permitido números no filtro por preenchimento');
	   p1.value = '';
	   p2.value = '';
	   return false;
  }

  if(data_inicio.value != "" && data_fim.value == "")
  {
	alert("A data final do 'Período de Tramitação' deve ser informada");
	data_fim.focus();
	return;
  }
  else if(data_inicio.value == "" && data_fim.value != "")
  {
	alert("A data inicial do 'Período de Tramitação' deve ser informada");
	data_inicio.focus();
	return;
  }
  else if(data_inicio.value != "" && data_fim.value != "")
  {
	var inicio  = data_inicio.value.split('\/');
	inicio		= inicio[2] + inicio[1] + inicio[0];

	var fim  = data_fim.value.split('\/');
	fim		 = fim[2] + fim[1] + fim[0];

	if(Number(inicio) > Number(fim))
	{
		alert("A data inicial do 'Período de Tramitação' não pode maior que a data final");
		data_inicio.focus();
		return;
	}
  }
  
  document.formulario.submit(); 
}
</script>