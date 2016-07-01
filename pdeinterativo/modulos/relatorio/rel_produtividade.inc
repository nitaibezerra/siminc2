<?php
	$htddataini = "";
	$htddatafim = "";
	
	ini_set("memory_limit","1024M");
	set_time_limit(0);
	
	$docid = pegarDocid($entid);
	if($_REQUEST['formulario'])
		$codigo  = $_REQUEST['codigo'];
	else
		$codigo = $_SESSION['sisid'];
		
	if ($_POST || $_GET['entid']){
		include 'resultRelProdutividade.php';
		exit;
	}
	include APPRAIZ. '/includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	
	monta_titulo( 'Relatório de Produtividade', 'Relatório de Produtividade' );

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="/includes/calendario.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function gerarRelatorio(relatorio){
		var formulario = document.formulario;
		var htddataini = document.formulario.htddataini.value;
		var htddataini = document.formulario.htddataini.value;

		formulario.relatorio.value = relatorio;
		
		selectAllOptions( formulario.f_estuf );	
		selectAllOptions( formulario.f_municipio );	
		selectAllOptions( formulario.f_aedid );
		selectAllOptions( formulario.f_pflcod );
			
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';	
		formulario.submit();
		janela.focus();
	}
	
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="pdeinterativo.php?modulo=relatorio/rel_produtividade&acao=A" method="post" enctype="multipart/form-data">	
	<input type="hidden" name="relatorio" id="relatorio" value="html">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
		        <td align='right' class="SubTituloDireita">Nome:</td>
		        <td>
					<?php echo campo_texto('usunome','N',$habil,'',30,200,'',''); ?>
		        </td>
		    </tr>
			<tr>
		        <td class="SubTituloDireita" valign="top">Período da Situação</td>
		        <td>
		           <?php echo  campo_data( 'htddataini', 'N','S','','','','' ); ?> (dd/mm/aaaa) &nbsp;&nbsp;&nbsp;até &nbsp; 
		           <?php echo  campo_data( 'htddatafim', 'N','S','','','','' ); ?> (dd/mm/aaaa)
		        </td>
	    	</tr>
										
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'aedid' );">
					Situacao
					<input
						type="hidden"
						id="aedid_campo_flag"
						name="aedid_campo_flag"
						value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
					/>
				</td>
				<td>
					<div
						id="aedid_campo_off"
						style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
						onclick="javascript:onOffCampo( 'aedid' );"
					>
						<img src="../imagens/combo-todos.gif" border="0" align="middle">
					</div>
					<div id="aedid_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
						<?php
							/*$sql_combo = "
								select esd.esdid  as codigo,
								       esd.esddsc as descricao
								from workflow.estadodocumento esd
									left join workflow.tipodocumento tid on esd.tpdid = esd.tpdid
								where tid.sisid = ".$_SESSION['sisid']."
								and   tid.tpdid = 43
							";*/
							$sql_combo = "
								SELECT aed.aedid::text as codigo, aeddscrealizada as descricao 
								FROM workflow.acaoestadodoc aed 
								INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem 
								WHERE tpdid=".TPD_WF_FLUXO;
							
							combo_popup(
								'f_aedid',						// nome do campo
								$sql_combo,						// sql de captura dos valores
								'Selecione a(s) Situações',	// label
								'400x400',						// tamanho do popup
								0,								// quantidade máxima de itens
								array(),						// valores fixos
								'',								// mensagem
								'S',							// habilitado
								false,							// habilitar busca por código
								true,							// habilitar flag contém
								10,
								400,
								'',
								'',
								'',
								array(
									  array(
									  		"codigo" 	=> "aed.aeddscrealizada",
									  		"descricao" => "Situação"
									  		)
									  )							
							);
						?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'pflcod' );">
					Perfil
					<input
						type="hidden"
						id="pflcod_campo_flag"
						name="pflcod_campo_flag"
						value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
					/>
				</td>
				<td>
					<div
						id="pflcod_campo_off"
						style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
						onclick="javascript:onOffCampo( 'pflcod' );"
					>
						<img src="../imagens/combo-todos.gif" border="0" align="middle">
					</div>
					<div id="pflcod_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
						<?php
							$sql_combo ="
								SELECT pflcod AS codigo,
								       pfldsc AS descricao
								FROM seguranca.perfil
								WHERE sisid = ".$codigo;
							
							combo_popup(
								'f_pflcod',						// nome do campo
								$sql_combo,						// sql de captura dos valores
								'Selecione o(s) perfil',	    // label
								'400x400',						// tamanho do popup
								0,								// quantidade máxima de itens
								array(),						// valores fixos
								'',								// mensagem
								'S',							// habilitado
								false,							// habilitar busca por código
								true,							// habilitar flag contém
								10,
								400,
								'',
								'',
								'',
								array(
									  array(
									  		"codigo" 	=> "pfldsc",
									  		"descricao" => "Perfil"
									  		)
									  )							
							);
						?>
					</div>
				</td>
			</tr>		
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'estuf' );">
					Estado
					<input
						type="hidden"
						id="estuf_campo_flag"
						name="estuf_campo_flag"
						value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
					/>
				</td>
				<td>
					<div
						id="estuf_campo_off"
						style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
						onclick="javascript:onOffCampo( 'estuf' );"
					>
						<img src="../imagens/combo-todos.gif" border="0" align="middle">
					</div>
					<div id="estuf_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
						<?php
							$sql_combo = "
								SELECT estuf AS codigo,
								       estuf || ' - ' || estdescricao AS descricao
								FROM territorios.regiao r   
									INNER JOIN territorios.estado e ON e.regcod = r.regcod	 									
								ORDER BY estuf
							";
							
							combo_popup(
								'f_estuf',						// nome do campo
								$sql_combo,						// sql de captura dos valores
								'Selecione o(s) Estado(s)',	// label
								'400x400',						// tamanho do popup
								0,								// quantidade máxima de itens
								array(),						// valores fixos
								'',								// mensagem
								'S',							// habilitado
								false,							// habilitar busca por código
								true,							// habilitar flag contém
								10,
								400,
								'',
								'',
								'',
								array(
									  array(
									  		"codigo" 	=> "r.regdescricao",
									  		"descricao" => "Região"
									  		)
									  )							
							);
						?>
					</div>
				</td>
			</tr>	
			<tr>
				<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'municipio' );">
					Município
					<input
						type="hidden"
						id="municipio_campo_flag"
						name="municipio_campo_flag"
						value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
					/>
				</td>
				<td>
					<div
						id="municipio_campo_off"
						style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
						onclick="javascript:onOffCampo( 'municipio' );">
						<img src="../imagens/combo-todos.gif" border="0" align="middle">
					</div>
					<div id="municipio_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
						<?php
							$sql_combo ="
								SELECT m.muncod AS codigo,
								       m.estuf || ' - ' || mundescricao AS descricao
								FROM territorios.municipio m
								ORDER BY m.estuf
							";
							
							combo_popup(
								'f_municipio',						// nome do campo
								$sql_combo,						// sql de captura dos valores
								'Selecione o(s) município(s)',		// label
								'400x400',						// tamanho do popup
								0,								// quantidade máxima de itens
								array(),						// valores fixos
								'',								// mensagem
								'S',							// habilitado
								false,							// habilitar busca por código
								true,
								10,
								400,
								'',
								'',
								'',
								array(
									  array(
									  		"codigo" 	=> "m.estuf",
									  		"descricao" => "Unidade de federação"
									  		)
									  )							
								// habilitar flag contém
							);
						?>
					</div>
				</td>
			</tr>
			<tr>
				<td align="center" colspan="2">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('html');"/>
					<input type="button" name="Gerar Relatório Excel" value="Gerar Relatório Excel" onclick="javascript:gerarRelatorio('xls');"/>
				</td>
			</tr>
	</table>		
</form>
</body>
</html>