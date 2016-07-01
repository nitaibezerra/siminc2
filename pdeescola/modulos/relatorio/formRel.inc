<?php

ini_set("memory_limit","1024M");

if ($_POST || $_GET['entid']){
	include 'resultRel.php';
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo( 'Relatório Geral', 'Relatório Geral' );

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript">
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.f_regiao );	
	selectAllOptions( formulario.f_estuf );	
	selectAllOptions( formulario.f_municipio );	
	selectAllOptions( formulario.f_ideb );
	selectAllOptions( formulario.f_situacao );
	
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="pdeescola.php?modulo=relatorio/formRel&acao=A" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Agrupadores</td>
			<td>
				<?
				$matriz = agrupador();
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
				$campoAgrupador->setDestino( 'agrupador');
				$campoAgrupador->exibir();
				?>
			</td>
		</tr>
		<!-- GRANDES CIDADE ------------------------------------------------ -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Filtro por Tipo
			</td>
			<td>
				<input type="checkbox" name="grandescidades" id="grandescidades" value="1"/>
				<label for="grandescidades">Grandes Cidades</label>
			</td>
		</tr>		
		<TR>
			<td class="SubTituloDireita" valign="top" >
				Tipo
			</td>
			<td>
				&nbsp;&nbsp;
				<label>Estadual<input type="radio" name="tipo" value="1"></label>&nbsp;&nbsp;&nbsp;
				<label>Municipal<input type="radio" name="tipo" value="3"></label>&nbsp;&nbsp;&nbsp;
				<label>Todas<input type="radio" name="tipo" value="1,3"></label>	
			</td>			
		</TR>
		<TR>
			<td class="SubTituloDireita" valign="top">
				Valor
			</td>
			<td>
				<?=campo_texto('valini','N','S','',15,20,'###.###.####.###,##','') ?> <B>até</B> <?=campo_texto('valfim','N','S','',15,20,'###.###.####.###,##','') ?>
			</td>			
		</TR>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ideb' );">
				Classe IDEB
				<input
					type="hidden"
					id="ideb_campo_flag"
					name="ideb_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="ideb_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'ideb' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="ideb_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo = "	select
											tpmid as codigo,
											tpmdsc as descricao
										from territorios.tipomunicipio
										where
											gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
											tpmstatus = 'A'";
						
						combo_popup(
							'f_ideb',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) classe(s) IDEB(s)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr><!--						
		<tr>
			<td class="SubTituloDireita" valign="top">
				Classe IDEB
			</td>
			<td>
			<?
			$sql =	array(
						 array(
								"codigo" 	=> "A",
						 		"descricao"	=> "Prioridades IDEB 2005"					 
						 	  ),
						 array(
								"codigo" 	=> "B",
						 		"descricao"	=> "Prioridades IDEB 2007"					 
						 	  ),			
						 array(
								"codigo" 	=> "C",
						 		"descricao"	=> "Abaixo da Média IDEB 2007"					 
						 	  )
						);
			$db->monta_combo( "epiclasse", $sql, 'S', 'Selecione...', '', '' );
			?>		
			</td>
		</tr>
		--><tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'regiao' );">
				Região
				<input
					type="hidden"
					id="regiao_campo_flag"
					name="regiao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="regiao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'regiao' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="regiao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo = "select
										regcod as codigo,
										regdescricao as descricao
									  from territorios.regiao
									  order by
										regdescricao";
						
						combo_popup(
							'f_regiao',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Região(ões)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true							// habilitar flag contém
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
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="estuf_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT 
										estuf AS codigo,
										estuf || ' - ' || estdescricao AS descricao
									FROM
									    territorios.regiao r   
										INNER JOIN territorios.estado e ON e.regcod = r.regcod	 									
									ORDER BY
										estuf";
						
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
						$sql_combo ="SELECT
										--DISTINCT
										m.muncod AS codigo,
										m.estuf || ' - ' || mundescricao AS descricao
									FROM
										territorios.municipio m
									ORDER BY
										m.estuf";
						
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
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'situacao' );">
				Situação
				<input
					type="hidden"
					id="situacao_campo_flag"
					name="situacao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="situacao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'situacao' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="situacao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php

						$sql_combo = "SELECT
											 ed.esdid AS codigo,
											 ed.esddsc AS descricao
											FROM
											 workflow.estadodocumento ed
											 INNER JOIN workflow.tipodocumento tp ON tp.tpdid = ed.tpdid AND
											 	sisid = ".$_SESSION['sisid']."
											 WHERE ed.tpdid = ".TPDID_PDE_ESCOLA."
											ORDER BY
											 ed.esdordem;";
						
						combo_popup(
							'f_situacao',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) situação(ões)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true
							);
					?>
				</div>
			</td>
		</tr>										
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
			</td>
		</tr>
</table>		
</form>
</body>
</html>
<?php
function agrupador(){
	return array(
				array('codigo' => 'estado',
					  'descricao' => 'Estado'),
				array('codigo' => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' => 'classe',
					  'descricao' => 'Classe IDEB'),	
				array('codigo' => 'nome',
					  'descricao' => 'Escola'),
				array('codigo' => 'situacao',
					  'descricao' => 'Situação')						
			);
}
?>