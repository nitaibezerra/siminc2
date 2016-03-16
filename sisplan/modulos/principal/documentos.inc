<?php
	
	switch( $_REQUEST['evento'] ){
		case 'download':
			$sql = sprintf(
				"select v.verid, v.vernome, v.vertipomime, a.atiid from pde.versaoanexoatividade v inner join pde.anexoatividade a on a.aneid = v.aneid where v.verid = %d",
				$_REQUEST['verid']
			);
			$versao = $db->pegaLinha( $sql );
			$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. $versao['verid'];
			if ( !is_readable( $caminho ) ) {
				redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
			}
			header( 'Content-type: '. $versao['vertipomime'] );
			header( 'Content-Disposition: attachment; filename=' . $versao['vernome'] );
			readfile( $caminho );
			exit();
			
		default:
			break;
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br />';
	monta_titulo( 'Pesquisa', 'Documentos e Anexos.' );
	
	
	
?>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script language="javascript" type="text/javascript">

	function habilitaCbxMunicipio()
	{
		document.formulario.submit();
	}

	function listar_acoes( prgcod )
	{
		var campo_select = document.getElementById( 'acacod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		if( prgcod != '' )
		{
			ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod,document.getElementById('acacod'),true);
		}
	}

	function listar_municipio( cmbmun )
	{
		var campo_select = document.getElementById( 'cmbmun' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		if( cmbmun != '' )
		{
			ajaxGet('../ajax/lista_mun.php?estuf='+cmbmun,document.getElementById('cmbmun'),true);
		}
	}

	function pesquisar_controle () 
	{
		document.formulario.evento.value = 'pesquisar_fonte';
		document.formulario.submit();
	}

	function limpar_controle () 
	{
		location.href = '<?=$_SESSION['sisarquivo'] . ".php?modulo=".$_REQUEST['modulo']."&acao=C"?>';
	}
</script>


<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="0" align="center">
	<tr>
		<td>
			<!-- NOVO CONTROLE -->
				<form action="#" method="post" name="formulario">
					<input type="hidden" name="evento" value=""/>
					<input type="hidden" name="stpid" value="<?=$stpid?>"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Departamento:</td>
							<td>
								<?php
									$cmbdepartamento = empty($_REQUEST['cmbdepartamento'])? 0 : $_REQUEST['cmbdepartamento'];
									$sqlDepartamento = "SELECT depid AS codigo, depdsc AS descricao FROM sisplan.departamento ORDER BY 2"; 
									echo $db->monta_combo('cmbdepartamento', $sqlDepartamento, 'S', 'Todos', '', '' ,'','','', 'cmbdepartamento');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Executora:</td>
							<td>
								<?php
									$cmbunidadeexc = empty($_REQUEST['cmbunidadeexc'])? 0 : $_REQUEST['cmbunidadeexc'];
									$sqlUnidadeExc = "SELECT uexid AS codigo, uexdsc AS descricao FROM planointerno.unidadeexecutora ORDER BY 2"; 
									echo $db->monta_combo('cmbunidadeexc', $sqlUnidadeExc, 'S', 'Todas', '', '' ,'','','', 'cmbunidadeexc');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
							<td>
								<?php
									$prgcod = empty($_REQUEST['prgcod'])? 0 : $_REQUEST['prgcod'];
									$sqlPrg = "SELECT prgcod as codigo, prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa WHERE prgano = '{$_SESSION['exercicio']}' ORDER BY 2"; 
									echo $db->monta_combo('prgcod', $sqlPrg, 'S', 'Todos', 'listar_acoes', '' ,'','','', 'prgcod');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
							<td>
								<?php
									$acacod = empty($_REQUEST['acacod'])? '' : $_REQUEST['acacod'];
									$sqlAcao = "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '{$_SESSION['exercicio']}'";
									$sqlAcao.= empty($_REQUEST['prgcod'])? '' : " AND prgcod = '{$_REQUEST['prgcod']}' ";
									$sqlAcao.= "ORDER BY 2";
									echo $db->monta_combo('acacod', $sqlAcao, 'S', 'Todas', '', '' ,'','600','', 'acacod');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo de Documento:</td>
							<td>
								<?php
									$cmbtipodoc = empty($_REQUEST['cmbtipodoc'])? 0 : $_REQUEST['cmbtipodoc'];
									$sqlTipoDoc = "SELECT taaid AS codigo, taadescricao AS descricao FROM pde.tipoanexoatividade WHERE taalegal = TRUE ORDER BY taadescricao ASC"; 
									echo $db->monta_combo('cmbtipodoc', $sqlTipoDoc, 'S', 'Todos', '', '' ,'','','', 'cmbtipodoc');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">UF:</td>
							<td>
								<?php
									$cmbuf = empty($_REQUEST['cmbuf'])? '' : $_REQUEST['cmbuf'];
									$sqlUf = "SELECT estuf AS codigo,  estuf AS descricao FROM territorios.estado ORDER BY 2"; 
									echo $db->monta_combo('cmbuf', $sqlUf, 'S', 'Todas', 'listar_municipio', '' ,'','','', 'cmbuf');	
								 ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Município:</td>
							<td>
								<?php
									$cmbmun = empty($_REQUEST['cmbmun'])? 0 : $_REQUEST['cmbmun'];
									
									if (empty($_REQUEST['cmbuf']))
									{
										$sqlMun = array(array('codigo' => 0, 'descricao' => 'Escolha primeiro a uf...'));
									}
									else
									{
										$sqlMun = "SELECT muncod AS codigo, estuf || ' - ' || mundescricao AS descricao FROM territorios.municipio ";
										$sqlMun .= empty($_REQUEST['cmbuf'])? '' : " WHERE estuf = '{$_REQUEST['cmbuf']}' ";
										$sqlMun .= " ORDER BY 2 ";
									}

									echo $db->monta_combo('cmbmun', $sqlMun, 'S', 'Todos', '', '' ,'','','', 'cmbmun');	
								 ?>
							</td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td>
								<input type="button" name="botao" value="Pesquisar" onclick="pesquisar_controle();"/>
								<input type="button" name="botao" value="Limpar" onclick="limpar_controle();"/>							
							</td>
						</tr>
					</table>
				</form>
		</td>
	</tr>
</table>

<?php
	unset( $sql );
	
	if ( $_REQUEST['evento'] == 'pesquisar_fonte' )
	{
		$whereAtv = "";
			
		if ( !empty($_REQUEST['cmbdepartamento']) )
			$whereAtv .= " AND atv.depid = '{$_REQUEST['cmbdepartamento']}' ";
			
		if ( !empty($_REQUEST['cmbunidadeexc']) )
			$whereAtv .= " AND atv.uexid = '{$_REQUEST['cmbunidadeexc']}' ";
		
		if ( !empty($_REQUEST['prgcod']) )
			$whereAtv .= " AND aca.prgcod = '{$_REQUEST['prgcod']}' ";

		if ( !empty($_REQUEST['acacod']) )
			$whereAtv .= " AND aca.acacod = '{$_REQUEST['acacod']}' ";
		
		if ( !empty($_REQUEST['cmbtipodoc']) )
			$whereAnx .= " AND anxatv.taaid = '{$_REQUEST['cmbtipodoc']}' ";
		
		if ( !empty($_REQUEST['cmbuf']) )
			$whereAtv .= " AND atv.estuf = '{$_REQUEST['cmbuf']}' ";
		
		if ( !empty($_REQUEST['cmbmun']) )
			$whereAtv .= " AND atv.muncod = '{$_REQUEST['cmbmun']}' ";
		
		$sqlBuscaAtv = "SELECT 	atv.atiid FROM pde.atividade      atv 
										  LEFT JOIN monitora.acao aca  ON atv.acaid = aca.acaid						
								WHERE 1=1 {$whereAtv}
								 ";
		$sqlBuscaFilhosAtv = "SELECT atiid FROM pde.atividade WHERE _atiprojeto IN ({$sqlBuscaAtv})";
		
		$url = "'<a href=\'?modulo={$_REQUEST['modulo']}&acao={$_REQUEST['acao']}&atiid='|| anxatv.atiid ||'";
		$url.= "&verid='|| anxatv.verid  ||'&evento=download\' title=\'Abrir Anexo\'>' || anxatv.anedescricao || '</a>'";						

		$sqlAnexos = "SELECT  distinct(anxatv.oid), 
							  {$url} AS anedescricao, 
							  tpanx.taadescricao,
							  atv.atidescricao,
							  aca.acacod || ' - ' || aca.acadsc AS acadsc,
							  prg.prgcod || ' - ' || prg.prgdsc AS prgdsc
					  FROM       pde.anexoatividade     anxatv
					  INNER JOIN pde.tipoanexoatividade tpanx ON anxatv.taaid = tpanx.taaid
					  INNER JOIN pde.atividade 	  		atv   ON anxatv.atiid = atv.atiid
					  INNER JOIN monitora.acao          aca   ON atv.acaid = aca.acaid
					  INNER JOIN monitora.programa	  	prg   ON prg.prgcod = aca.prgcod
				  	  WHERE anxatv.atiid IN ({$sqlBuscaFilhosAtv}) {$whereAnx};";
	
			
		// exibe o resultado da consulta
		$db->monta_lista($sqlAnexos, array('Código', 'Descrição', 'Tipo', 'Atividade', 'Ação', 'Programa'), 20, 5, 'N', '', '');
	}
	
?>