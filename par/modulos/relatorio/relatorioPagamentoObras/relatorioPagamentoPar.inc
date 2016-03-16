<?php

if ($_POST['ajaxUf']){
	
	if($_POST['ajaxUf'] != 'XX'){
		echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
		$stAnd = "AND regcod in ('".str_replace(",","','",$_POST['ajaxUf'])."')";
	}
	
	$sql_combo = "SELECT
					muncod AS codigo,
					regcod ||' - '|| mundsc AS descricao
				FROM
					public.municipio
				Where munstatus = 'A'
				$stAnd
				ORDER BY 2 ASC;";
	combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);

	exit;
}
// Verifica a requisição e envia para ser processada
if ($_POST['requisicao'] == 'relatorio' || $_POST['requisicao'] == 'relatorio_xls' )
{
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioPagamentoParResultado.inc");
	exit;
}

// Includes
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta o título da página
monta_titulo( 'Relatório Gerencial Pagamentos Obras', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">

// Função responsável por fazer a requisição para o formulário 
function gerarRelatorio(req)
{
	// Recupera obj do formulário 
	var formulario = document.formulario;
	selectAllOptions( formulario.uf );
	selectAllOptions( formulario.muncod );
	
	// Verifica a solicitação e seleciona os combos	
	document.getElementById('req').value = req;

	if( req == 1 )
	{
		document.getElementById('requisicao').value = 'relatorio';
	}
	else
	{
		document.getElementById('requisicao').value = 'relatorio_xls';
	}
	
    // Executa a solicitação			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	// Submete formulário 
	formulario.submit();
	janela.focus();
}


// Função que filtra os municípios 
function filtraMunicipio(){

		
		var uf = document.getElementById('uf');
		var listauf = "";

		for(i=0;i<uf.length;i++){
			listauf = listauf + "," + uf[i].value;
		}

		if(listauf) listauf = listauf.substr(1);
		
		if(!listauf) listauf = 'XX';

		var req = new Ajax.Request('par.php?modulo=relatorio/relatorioProgramas&acao=A', {
				        method:     'post',
				        parameters: '&ajaxUf='+listauf,
				        onComplete: function (res)
				        {
							td_municipio.innerHTML = res.responseText;
				        }
					});

		//filta o campo escola
		var td_escolas   = document.getElementById('td_escolas');

		
}

function appendOptionLast(num)
{
  var elOptNew = document.createElement('option');
  elOptNew.text = 'Append' + num;
  elOptNew.value = 'append' + num;
  var elSel = document.getElementById('selectX');

  try {
    elSel.add(elOptNew, null); // standards compliant; doesn't work in IE
  }
  catch(ex) {
    elSel.add(elOptNew); // IE only
  }
}

</script>
</head>
<body>
	

	<!-- MUNICIPAL -->
	
	
		<form name="formulario" id="formulario" action="" method="post">
		<input type="hidden" name="req" id="req" value="" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />
		<div id="campos_pesquisa" >
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">	
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Programa:</td>
					<td>
						<select style="width: auto" class="CampoEstilo" name="tipo_obra">
							<option value="">Selecione...</option>
							<option value="PAC">PAC</option>
							<option value="PAR">PAR</option>
						</select>
					</td>
				</tr>
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">ID da Obra:</td>
					<td>
						<?= campo_texto( 'preid', 'N', 'S', '', 11 , 11, '##########', '', 'left', '', 0, 'id="preid"'); ?>
					</td>
				</tr>
				<tr>
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Período da Data Solicitação:</td>
					<td>
						 <?= campo_data( 'pagdatapagamento_de', 'N', 'S', '', 'S','','','' ); ?> &nbsp;  Até: &nbsp;<?= campo_data( 'pagdatapagamento_ate', 'N', 'S', '', 'S','','','' ); ?>
						  
					</td>
				</tr>
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Nº empenho:</td>
					<td>
						<?= campo_texto( 'empnumero', 'N', 'S', '', 20, 20, '', '', 'left', '', 0, 'id="empnumero"'); ?>
					</td>
				</tr>
				
				
				<tr>
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Nº processo:</td>
					<td>
						<?= campo_texto( 'nprocesso', 'N', 'S', '', 25, 25, "#####.######/####-##", '', 'left', '', 0, 'id="titulo"'); ?>
					</td>
				</tr>
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Situação Solicitação:</td>
					<td>
						<?php
							$estuf = $_POST['situacao'];
							$sql = "SELECT DISTINCT pagsituacaopagamento as codigo, pagsituacaopagamento as descricao  
									FROM par.pagamento 
									WHERE pagsituacaopagamento <> '' AND pagsituacaopagamento <>  'CANCELADO' AND pagstatus = 'A' 
									ORDER BY pagsituacaopagamento";
							
							$db->monta_combo( "situacao", $sql, 'S', 'Selecione a situação', '', '' );
						?>
					</td>
				</tr>
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Usuário criação:</td>
					<td>
						<?= campo_texto( 'usunome', 'N', 'S', '', 50, 100, '', '', 'left', '', 0, 'id="usunome"'); ?>
					</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita" valign="top">Estado:</td>	
					<td >
						<?php
						
							$sql_combo = "SELECT
										regcod AS codigo,
										descricaouf AS descricao
									FROM
										public.uf
									WHERE idpais = 1
									ORDER BY 2 ASC;";
							combo_popup( 'uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', true);
						
				    	?>	
					</td>
				</tr>
				<tr id="tr_municipio" >
					<td class="SubTituloDireita" valign="top">Município:</td>	
					<td id="td_municipio">
						<?php 
						
							$sql_combo = "SELECT
											muncod AS codigo,
											regcod ||' - '|| mundsc AS descricao
										FROM
											public.municipio
										Where munstatus = 'A'
										and regcod = 'XX'
										ORDER BY 2 ASC;";
							combo_popup( 'muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);			
						
						?>
					</td>
				</tr>
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Obras MI:</td>
					<td>
						<select style="width: auto" class="CampoEstilo" name="tipo_obra_mi">
							<option value="">Todas</option>
							<option value="MI">Somente MI</option>
							<option value="NMI">Somente não MI</option>
						</select>
					</td>
				</tr>
				
				<tr>
					<td colspan="2" align="center">
						<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
						<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
					
					</td>
				
				</tr>
			</table>
		</div>
		</form>
	
</body>
</html>