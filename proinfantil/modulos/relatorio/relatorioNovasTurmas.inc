<?php

// Verifica a requisição e envia para ser processada
if ($_POST['requisicao'] == 'relatorio' || $_POST['requisicao'] == 'relatorio_xls' )
{
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioNovasTurmasResultado.inc");
	exit;
}

// Monta classe para montar página e parâmetros
class relatorioNovasTurmas
{
	// Codifica em utf8 para retornar no ajax
	public function getUtf8ArrCombo( $arrCombo )
	{
		if(count($arrCombo))
		{
			foreach($arrCombo as $k => $op)
			{
				$arrCombo[$k][descricao] = utf8_encode($arrCombo[$k][descricao]);
			}
		}
		return $arrCombo ;
	}
	
	public function getColunas($post)
	{
		
		// Recupera a tipo
		$tipo 	= $post['tipo'];
		// Recupera as entidades
		$entidades 	= $post['entidades'];
		// Array com as colunas do retorno
		$arrayColunas = array();
		
		
		// Caso tenha sido passada alguma entidade
		if($entidades != '')
		{
			// Cria array com as entidades
			$arrTipoEntidade = explode(",", $entidades);
		}
		else
		{
			// Declara ele como vazio
			$arrTipoEntidade = array();
		}

		switch ($tipo) 
		{
			//  Turma
			case 'TURMA':
		    	$arrayColunas[] = array('codigo'=>'T_NOMTURM' , 'descricao' => 'Nome da turma');
		    	$arrayColunas[] = array('codigo'=>'T_CINEP' , 'descricao' => 'Código INEP');
		    	$arrayColunas[] = array('codigo'=>'T_TREDE' , 'descricao' => 'Tipo de rede');
		    	$arrayColunas[] = array('codigo'=>'T_TATEND' , 'descricao' => 'Tipo atendimento');
		    	$arrayColunas[] = array('codigo'=>'T_CADEDUCACENSO' , 'descricao' => 'Está cadastrado no Educacenso');
		    	$arrayColunas[] = array('codigo'=>'T_DTINITURM' , 'descricao' => 'Data início da Turma');
		    	$arrayColunas[] = array('codigo'=>'T_MESCADTURM' , 'descricao' => 'Mês de cadastro da Turma');
		    	$arrayColunas[] = array('codigo'=>'T_DTENVANALISE' , 'descricao' => 'Data Envio para Análise');
		    	$arrayColunas[] = array('codigo'=>'T_QTDMESESREPASSE' , 'descricao' => 'QTD de meses para repasse dos recursos');
		    	$arrayColunas[] = array('codigo'=>'T_QTDALUCREINT' , 'descricao' => 'QTD alunos em creche tempo integral');
		    	$arrayColunas[] = array('codigo'=>'T_QTDALUCREPAR' , 'descricao' => 'QTD alunos em creche tempo parcial');
		    	$arrayColunas[] = array('codigo'=>'T_QTDALUPREINT' , 'descricao' => 'QTD alunos pré-escola integral');
		    	$arrayColunas[] = array('codigo'=>'T_QTDALUPREPAR' , 'descricao' => 'QTD alunos pré-escola parcial');
		    	$arrayColunas[] = array('codigo'=>'T_QTDPROFCREINT' , 'descricao' => 'QTD de professores creche tempo integral');
		    	$arrayColunas[] = array('codigo'=>'T_QTDPROFCREPAR' , 'descricao' => 'QTD de professores creche tempo parcial');
		    	$arrayColunas[] = array('codigo'=>'T_QTDPROFPREINT' , 'descricao' => 'QTD professores pré-escola integral');
		    	$arrayColunas[] = array('codigo'=>'T_QTDPROFPREPAR' , 'descricao' => 'QTD professores pré-escola parcial');
			break;
		    
			// Municipio
			case 'MUNICIPIO':
		        
		        $arrayColunas[] = array('codigo'=>'MUN_MESCAD' , 'descricao' => 'Meses cadastrado');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMCREPUB' , 'descricao' => 'QTD de turmas da creche pública');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMCRECONV' , 'descricao' => 'QTD de turmas da creche conveniada');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMPREPUB' , 'descricao' => 'QTD de turmas da pré-escola pública');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMPRECONV' , 'descricao' => 'QTD de turmas da pré-escola conveniadas');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMUNIPUB' , 'descricao' => 'QTD de turmas da Unificada pública');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDTURMUNICONV' , 'descricao' => 'QTD de turmas da Unificada conveniadas');
		        $arrayColunas[] = array('codigo'=>'MUN_TOTAMPTURM' , 'descricao' => 'Total ampliado de Turmas');
		        
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATCREPUBPARC' , 'descricao' => 'QTD de matrículas da creche parcial pública ');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATCRECONVPARC' , 'descricao' => 'QTD de matrículas da creche parcial conveniada');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATPREPUBPARC' , 'descricao' => 'QTD de matrículas da pré-escola parcial pública ');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATPRECONVPARC' , 'descricao' => 'QTD de matrículas da pré-escola parcial conveniadas');
		        
		        
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATCREPUBINT' , 'descricao' => 'QTD de matrículas da creche integral pública');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATCRECONVINT' , 'descricao' => 'QTD de matrículas da creche conveniada');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATPREPUBINT' , 'descricao' => 'QTD de matrículas da pré-escola pública ');
		        $arrayColunas[] = array('codigo'=>'MUN_QTDMATPRECONVINT' , 'descricao' => 'QTD de matrículas da pré-escola conveniadas');
		        
		        
		        $arrayColunas[] = array('codigo'=>'MUN_AMPMAT' , 'descricao' => 'Total ampliado de Matrículas');
			break;
		}
		

		if( count($arrayColunas) )
		{
			$arrayColunas = $this->getUtf8ArrCombo($arrayColunas);
		}
		
		die(simec_json_encode($arrayColunas));
	}

	
}
// Instancia o objeto da classe do relatório
$obRDU = new relatorioNovasTurmas();
// Caso a requisição seja pelas colunas chama o método getColunas
if($_REQUEST['requisicao'] == "getColunas")
{
	$obRDU->getColunas($_POST);
}

// Includes
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$anoInvalido = false;
// Monta o título da página
if($_SESSION['exercicio'] <= 2012 )
{
	$titulo2 = 'O relatório só está disponível para o ano de exercício de 2013 ou superior.';
	$anoInvalido = true;
}
else
{
	$titulo2 = '&nbsp;';
}
monta_titulo( 'Relatório Novas Turmas', $titulo2 );

if($anoInvalido)
{
	die();
}
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
function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
// Define a tipo escolhida (estadual ou municipal) 
function defineTipo(tipo)
{
	// Esconde campos da pesquisa 
	$('campos_pesquisa').hide();
	// Turma 
	if( tipo == 1 )
	{
		var param =	 
			'&tipo=' + 'TURMA';
				
 
	} // Município 
	else
	{
		var param =	 
			'&tipo=' + 'MUNICIPIO';
			
		
	}	
	// Verifica a tipo atual 
	tipo = $$('input:checked[type=radio][name=tipo]')[0].value;
	// Pega as colunas de Origem 
	var colunaOrigem = $('colunaOrigem');
    // Recupera objeto do formulário 
	form = $('formulario');
	
	
	// Faz requisição ajax que irá popular as colnas de acordo com o tipo de entidade selecionado 
		var req = new Ajax.Request('proinfantil.php?modulo=relatorio/relatorioNovasTurmas&acao=A&requisicao=getColunas', {
    	method:     'post',
        parameters: param,
        onComplete: function (res)
        {
			
        	var json = res.responseText.evalJSON();

			// Retira as opções que j existiam
			for (var i=(colunaOrigem.options.length-1); i>=0; i--) {
				
				colunaOrigem.options[i] = null;
				
			}
			// Retira as opções que j existiam
			for (var i=(coluna.options.length-1); i>=0; i--) {
				
				coluna.options[i] = null;
				
			}
			if( json.length ) 
			{
				
				$('campos_pesquisa').show();
				if( tipo == 1 )
				{
						
					$('tr_situ_turma').show();
					
				}
				else
				{
					
					
					$('tr_situ_turma').hide();
				}
			}
	        // Adiciona as escolhidas
	        for (var i=0;i<json.length;i++)
	        { 
				var codigo = json[i].codigo;
	        	var descricao = json[i].descricao;
	        	colunaOrigem.options[colunaOrigem.options.length] = new Option( descricao, codigo , false, false );
	        }
	        
	    	//divRel.innerHTML = res.responseText;
	        
        }
	});
}

// Função responsável por fazer a requisição para o formulário 
function gerarRelatorio(req){

	// Recupera obj do formulário 
	var formulario = document.formulario;
	
	// Verifica a solicitação e seleciona os combos	
	document.getElementById('req').value = req;
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estuf );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.coluna );

	// O campo coluna é obrigatório 
	if (formulario.coluna.value == ''){
		alert('Selecione no mínimo uma coluna');
		return false;
	}
	// Caso seja um relatório html ou excel 
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



jQuery.noConflict
jQuery( document ).ready(function() {

	jQuery.noConflict
	jQuery('#colunaOrigem').css('width', '320px')
	jQuery('#coluna').css('width', '320px')
});
</script>
</head>
<body>
	

		<form name="formulario" id="formulario" action="" method="post">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		
		<tr>
			<td style="width: 20%;" class="SubTituloDireita" valign="top">Tipo:</td>	
			<td>
				<input type="radio" value="1" name="tipo" id="tipo" onclick="defineTipo(this.value);" >Turma
				&nbsp;&nbsp;  
				<input type="radio" value="2" name="tipo" id="tipo" onclick="defineTipo(this.value);" >Município
			</td>
		</tr>
		</table>
	
		<input type="hidden" name="req" id="req" value="" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />


		<div id="campos_pesquisa" style="display: none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">	
				<tr> 
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Colunas</td>
					<td>
					
						<?
						$matriz2 = array();
			                        
						$campoAgrupador2 = new Agrupador( 'formulario' );
						$campoAgrupador2->setOrigem( 'colunaOrigem', null, $matriz2 );
						$campoAgrupador2->setDestino( 'coluna', null, null);
						$campoAgrupador2->exibir();
						?>
					</td>
				</tr>
	<?php 
					$arrVisivel = array("descricao");
					$arrOrdem = array("descricao");
							
					$stSql = "SELECT 	estuf as codigo, 
										estdescricao as descricao 
							  FROM		territorios.estado												
							  ORDER BY	estuf";
			
					$stSqlCarregados = "";
					echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script>";
					mostrarComboPopup('UF:', 'estuf',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
					mostrarComboPopupMunicipios( 'muncod', 'Municípios','S', true,true, '400px', true);	?>

				
				
				<tr id="tr_situ_turma" style="display: none;" >
					<td class="SubTituloDireita" valign="top">Situação:</td>	
					<td id="td_municipio">
					<?php 
							$sql = "SELECT 		esdid as codigo,
												esddsc as descricao
									FROM		workflow.estadodocumento
									WHERE		tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS;
							
							$db->monta_combo('situacao_turma', $sql, 'S', 'Selecione..', '', '', '', '');
					?> 
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