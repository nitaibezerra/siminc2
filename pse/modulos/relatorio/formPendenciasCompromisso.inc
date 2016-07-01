<?php

if ( $_REQUEST['arqid'] ){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("contratualizacao", NULL ,"pse");
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die("<script>self.close();</script>");	
}


if ( $_REQUEST['termo'] ){
	$_SESSION['pse']['muncod'] = $_REQUEST['muncod'];
	$_SESSION['conid'] = $_REQUEST['conid'];
	
	?>
		<script>
			location.href='pse.php?modulo=principal/cadastroTermoPactuacao&acao=A&termo=ok';
		</script>
	<?
	die();
}

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultPendenciasCompromisso.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Pendências/Termo de Compromisso', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(){
	if(document.formulario.tiporelatorio.value == 0){
		alert('Por favor selecione o tipo de relatório.');
		return false;
	}
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.ano );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
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

function mostraCampos(id){
	var tr_tipoanexo = document.getElementById('tr_tipoanexo');
	var tr_tiposecretario = document.getElementById('tr_tiposecretario');

	if(id == 2){
		tr_tipoanexo.style.display = '';
		tr_tiposecretario.style.display = '';
	}
	else{
		tr_tipoanexo.style.display = 'none';
		tr_tiposecretario.style.display = 'none';
	}
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
		<?php
			
				// Filtros do relatório
				
				$stSql = "SELECT DISTINCT
							      porano as codigo,
							      (SELECT CASE porano
								       WHEN 2008 THEN 'Portaria 2.931 de 4 de dezembro de 2008'
								       WHEN 2009 THEN 'Portaria 1.537 de 15 de junho de 2010'
							       ELSE 'Portaria 1.910 de 9 de agosto de 2011'
							      END) as descricao
						 FROM
							      pse.portariapse";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Portaria', 'ano',  $stSql, $stSqlCarregados, 'Selecione o Ano' ); 
				
				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
				
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 

				
				?>
				<tr>
					<td class="SubTituloDireita" width="195" valign="top">
						Tipo de Relatório
					</td>
					<td>
						<select name="tiporelatorio" id="tiporelatorio" onchange="mostraCampos(this.value);">
							<option value="0">Selecione a opção de filtro...</option>
							<option value="1">Não acessou o PSE</option>
							<option value="2">Pendências no Termo de Compromisso</option>
							<option value="3">Cadastro do Termo de Compromisso completo</option>
						</select>
					</td>
				</tr>
				<tr id="tr_tipoanexo" style="display: none;">
					<td class="SubTituloDireita" width="195" valign="top">
						Anexo
					</td>
					<td>
						<select name="tipoanexo" id="tipoanexo">
							<option value="">Com Anexo e Sem Anexo</option>
							<option value="2">Somente Com Anexo</option>
							<option value="3">Somente Sem Anexo</option>
						</select>
					</td>
				</tr>
				<tr id="tr_tiposecretario" style="display: none;">
					<td class="SubTituloDireita" width="195" valign="top">
						Mostrar Dados do Secretário / Representante de Educação / Saúde 
					</td>
					<td>
						<select name="tiposecretario" id="tiposecretario">
							<option value="1">SIM</option>
							<option value="2">NÃO</option>
						</select>
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
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),	
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' 	  => 'portar',
					  'descricao' => 'Portaria')
				
				);
}
?>