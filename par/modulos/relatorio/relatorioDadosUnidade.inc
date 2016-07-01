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
	include("relatorioDadosUnidadeResultado.inc");
	exit;
}

// Monta classe para montar página e parâmetros
class relatorioDadosUnidade
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
		
		// Recupera a esfera
		$esfera 	= $post['esfera'];
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
		
		// Caso seja estadual
		if( $esfera == 'estadual' )
		{	
			// Caso o array não seja vazio
			if(count($arrTipoEntidade))
			{
				foreach( $arrTipoEntidade as $k => $ent )
				{
					switch ($ent) 
					{
						//  Secretaria Estadual de Educação
						case 'SEE':
					    	$arrayColunas[] = array('codigo'=>'CNPJ_SEE' , 'descricao' => 'CNPJ Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'NOME_SEE' , 'descricao' => 'Nome Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'RSOC_SEE' , 'descricao' => 'Razão Social Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_SEE' , 'descricao' => 'Email Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'SIGLA_SEE' , 'descricao' => 'Sigla Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'TEL_SEE' , 'descricao' => 'Telefone Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'CEP_SEE' , 'descricao' => 'CEP Secretaria Estadual de Educação');
					    	$arrayColunas[] = array('codigo'=>'END_SEE' , 'descricao' => 'Endereco Secretaria Estadual de Educação');
						break;
					    
						// Secretário(a) Estadual de Educação
						case 'SECEE':
					        $arrayColunas[] = array('codigo'=>'CPF_SECEE' , 'descricao' => 'CPF Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'NOME_SECEE' , 'descricao' => 'Nome Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'EMAIL_SECEE' , 'descricao' => 'Email Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'RG_SECEE' , 'descricao' => 'Registro Geral (RG) Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'OE_SECEE' , 'descricao' => 'Orgão Expedidor Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'SEX_SECEE' , 'descricao' => 'Sexo Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'DTN_SECEE' , 'descricao' => 'Data de Nascimento Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'TEL_SECEE' , 'descricao' => 'Telefone Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'CEP_SECEE' , 'descricao' => 'CEP Secretário(a)');
					        $arrayColunas[] = array('codigo'=>'END_SECEE' , 'descricao' => 'Endereco Secretário(a)');
						break;
					    
						// Equipe local Estadual
					    case 'ELE':
					    	$arrayColunas[] = array('codigo'=>'CPF_ELE' , 'descricao' => 'CPF Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'NOM_ELE' , 'descricao' => 'Nome Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'FUNC_ELE' , 'descricao' => 'Função/Cargo Equipe Local');  
					    	$arrayColunas[] = array('codigo'=>'SEG_ELE' , 'descricao' => 'Segmento Equipe Local');  
					    	$arrayColunas[] = array('codigo'=>'EMAIL_ELE' , 'descricao' => 'E-mail Equipe Local');  
					    	 
					    break;
					    // Comitê Local Estadual 
					    case 'CLE':
					        $arrayColunas[] = array('codigo'=>'CPF_CLE' , 'descricao' => 'CPF Comitê Local');  
					    	$arrayColunas[] = array('codigo'=>'NOME_CLE' , 'descricao' => 'Nome Comitê Local');  
					    	$arrayColunas[] = array('codigo'=>'FUNC_CLE' , 'descricao' => 'Função/Cargo Comitê Local');  
					    	$arrayColunas[] = array('codigo'=>'SEG_CLE' , 'descricao' => 'Segmento Comitê Local');  
					    	$arrayColunas[] = array('codigo'=>'EMAIL_CLE' , 'descricao' => 'E-mail Comitê Local'); 
					    break;
					}
				}
			}
			else 
			{
				
			}
			
		}
		else if( $esfera == 'municipal' )
		{
			// Caso o array não seja vazio
			if(count($arrTipoEntidade))
			{
				foreach( $arrTipoEntidade as $k => $ent )
				{
					switch ($ent) 
					{
						// Prefeitura
						case 'PRF':
							
					    	$arrayColunas[] = array('codigo'=>'CNPJ_PRF' , 'descricao' => ' CNPJ Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'IE_PRF' , 'descricao' => ' Inscrição Estadual Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'NOME_PRF' , 'descricao' => ' Nome Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'RSOC_PRF' , 'descricao' => ' Razão Social Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_PRF' , 'descricao' => ' Email Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'SIG_PRF' , 'descricao' => ' Sigla Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'TEL_PRF' , 'descricao' => ' Telefone Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'FAX_PRF' , 'descricao' => ' Fax Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'CEP_PRF' , 'descricao' => ' CEP Prefeitura');
					    	$arrayColunas[] = array('codigo'=>'END_PRF' , 'descricao' => ' Endereco Prefeitura');
						break;
					    
						//  Prefeito(a) 
						case 'PRFTO':
					        $arrayColunas[] = array('codigo'=>'CPF_PRFTO' , 'descricao' => 'CPF Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'NOME_PRFTO' , 'descricao' => 'Nome Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'EMAIL_PRFTO' , 'descricao' => 'Email Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'RG_PRFTO' , 'descricao' => 'Registro Geral (RG) Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'OE_PRFTO' , 'descricao' => 'Orgão Expedidor Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'SEX_PRFTO' , 'descricao' => 'Sexo Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'DTN_PRFTO' , 'descricao' => 'Data de Nascimento Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'TEL_PRFTO' , 'descricao' => 'Telefone comercial Prefeito(a)');
					        $arrayColunas[] = array('codigo'=>'FAX_PRFTO' , 'descricao' => 'Fax Prefeito(a)'); 
						break;
					    
						// Secretaria Municipal de Educação
					    case 'SME':
					    	
					    	$arrayColunas[] = array('codigo'=>'NOME_SME' , 'descricao' => ' Nome Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_SME' , 'descricao' => ' Email Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'SIG_SME' , 'descricao' => ' Sigla Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'TEL_SME' , 'descricao' => ' Telefone comercial Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'FAX_SME' , 'descricao' => ' Fax Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'CEP_SME' , 'descricao' => ' CEP Secretaria Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'END_SME' , 'descricao' => ' Endereço Secretaria Municipal de Educação');
					    break;
					    //  Dirigente Municipal de Educação 
					    case 'DME':
					    	
					    	$arrayColunas[] = array('codigo'=>'CPF_DME' , 'descricao' => ' CPF Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'NOME_DME' , 'descricao' => ' Nome Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_DME' , 'descricao' => ' Email Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'RG_DME' , 'descricao' => ' Registro Geral (RG) Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'OE_DME' , 'descricao' => ' Orgão Expedidor (RG) Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'SEX_DME' , 'descricao' => ' Sexo Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'DTN_DME' , 'descricao' => ' Data de Nascimento Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'TEL_DME' , 'descricao' => ' Telefone Dirigente Municipal de Educação');
					    	$arrayColunas[] = array('codigo'=>'FAX_DME' , 'descricao' => ' Fax Dirigente Municipal de Educação');
					    break;
					    //  Equipe Local Municipal 
					    case 'ELM':
					    	
					    	$arrayColunas[] = array('codigo'=>'CPF_ELM' , 'descricao' => 'CPF Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'NOME_ELM' , 'descricao' => 'Nome Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'FUNC_ELM' , 'descricao' => 'Função/Cargo Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'SEG_ELM' , 'descricao' => 'Segmento Equipe Local');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_ELM' , 'descricao' => 'E-mail Equipe Local');
					    break;
					    // Comitê Local MUNICIPAL  
					    case 'CLM':  	
						''; array('CPF', 'Nome', 'Função/Cargo', 'Segmento', 'E-mail');
					    	
					    	$arrayColunas[] = array('codigo'=>'CPF_CLM' , 'descricao' => 'CPF Comitê Local');
					    	$arrayColunas[] = array('codigo'=>'NOME_CLM' , 'descricao' => 'Nome Comitê Local');
					    	$arrayColunas[] = array('codigo'=>'FUNC_CLM' , 'descricao' => 'Função/Cargo Comitê Local');
					    	$arrayColunas[] = array('codigo'=>'SEG_CLM' , 'descricao' => 'Segmento Comitê Local');
					    	$arrayColunas[] = array('codigo'=>'EMAIL_CLM' , 'descricao' => 'E-mail Comitê Local');
					    break;
					  
					}
				}
			}
			else
			{
				
			}
		}
		
		if( count($arrayColunas) )
		{
			$arrayColunas = $this->getUtf8ArrCombo($arrayColunas);
		}
		
		die(simec_json_encode($arrayColunas));
	}

	
}
// Instancia o objeto da classe do relatório
$obRDU = new relatorioDadosUnidade();
// Caso a requisição seja pelas colunas chama o método getColunas
if($_REQUEST['requisicao'] == "getColunas")
{
	$obRDU->getColunas($_POST);
}

// Includes
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// Monta o título da página
monta_titulo( 'Relatório Dados da Unidade', '&nbsp;' );
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
// Trata quando a entidade é mudada 
function changeTipoEntidade()
{
	// Verifica a esfera atual 
	esfera = $$('input:checked[type=radio][name=esfera]')[0].value;
	// Pega as colunas de Origem 
	var colunaOrigem = $('colunaOrigem');
    // Recupera objeto do formulário 
	form = $('formulario');
	
	// Esfera ESTADUAL 
	if( esfera == 1 )
	{
		// Lista as entidades selecionadas nos checkboxes 
		listaEntidades = form['tipo_entidade_estado[]'];
		// Cria variável 
		entidadesSelecionadas = '';
        // A princípio declara como nenhum marcado, abaixo verifica-se a existencia 
		existeMarcado = false;
		// Lista todos os checks 
		for ( var i=0;i < listaEntidades.length; i++)
		{ 
			// Caso esteja marcado, desabilita as demasi opções que não podem ser combinadas no relatório 
			if( listaEntidades[i].checked )
			{
				// Monta str que irá virar o array de retorno e seta que existe pelo menos uma marcada 
				entidadesSelecionadas += listaEntidades[i].value + ',';
				existeMarcado = true;	
				// Caso secretaria secretario
				if( (listaEntidades[i].value == 'SEE') || (listaEntidades[i].value == 'SECEE'))
				{
					for ( var ee=0;ee < listaEntidades.length; ee++)
					{
						if((listaEntidades[ee].value != 'SEE') && (listaEntidades[ee].value != 'SECEE'))
						{
							listaEntidades[ee].checked = false;
							listaEntidades[ee].disabled = true;
						}
							
					}
				}
				// Caso equipe local estadual
				if( (listaEntidades[i].value == 'ELE'))
				{
					for ( var ele=0;ele < listaEntidades.length; ele++)
					{
						if((listaEntidades[ele].value != 'ELE'))
						{
							listaEntidades[ele].checked = false;
							listaEntidades[ele].disabled = true;
						}
							
					}
				}
				// Caso comite local estadual
				if( (listaEntidades[i].value == 'CLE'))
				{
					for ( var cle=0;cle < listaEntidades.length; cle++)
					{
						if((listaEntidades[cle].value != 'CLE'))
						{
							listaEntidades[cle].checked = false;
							listaEntidades[cle].disabled = true;
						}
							
					}
				}
					
			}
		}

		// Se nenhum tiver sido marcado habilita todos 
		if( ! existeMarcado )
		{
			for ( var em=0;em < listaEntidades.length; em++)
			{
				listaEntidades[em].checked = false;
				listaEntidades[em].disabled = false;	
			}
		}
		// Trata os parametros da requisicao 
		var param 	= 	'&entidades=' + entidadesSelecionadas + 
				 		'&esfera=' + 'estadual';
		
	}
	else
	{
		// Lista as entidades selecionadas nos checkboxes 
		listaEntidades = form['tipo_entidade_municipio[]'];
		// Cria variável 
		entidadesSelecionadas = '';
		// A princípio declara como nenhum marcado, abaixo verifica-se a existencia 
		existeMarcado = false;
		// Lista todos os checks 		
		for ( var i=0;i < listaEntidades.length; i++)
		{
			// Caso esteja marcado, desabilita as demasi opções que não podem ser combinadas no relatório 
			if( listaEntidades[i].checked )
			{
				// Monta str que irá virar o array de retorno e seta que existe pelo menos uma marcada 
				entidadesSelecionadas += listaEntidades[i].value + ',';
				existeMarcado = true;	
				// Caso prefeito prefeitura
				if( (listaEntidades[i].value == 'PRFTO') || (listaEntidades[i].value == 'PRF'))
				{
					for ( var pr=0;pr < listaEntidades.length; pr++)
					{
						if((listaEntidades[pr].value != 'PRFTO') && (listaEntidades[pr].value != 'PRF'))
						{
							listaEntidades[pr].checked = false;
							listaEntidades[pr].disabled = true;
						}
							
					}
				}
				// Caso Secretaria secretário 
				if( (listaEntidades[i].value == 'SME') || (listaEntidades[i].value == 'DME'))
				{
					for ( var me=0;me < listaEntidades.length; me++)
					{
						if((listaEntidades[me].value != 'SME') && (listaEntidades[me].value != 'DME'))
						{
							listaEntidades[me].checked = false;
							listaEntidades[me].disabled = true;
						}
							
					}
				}
				// Caso equipe local municipal 
				if( (listaEntidades[i].value == 'ELM'))
				{
					for ( var me=0;me < listaEntidades.length; me++)
					{
						if((listaEntidades[me].value != 'ELM'))
						{
							listaEntidades[me].checked = false;
							listaEntidades[me].disabled = true;
						}
							
					}
				}
				// Caso comite local municipal 
				if( (listaEntidades[i].value == 'CLM'))
				{
					for ( var clm=0;clm < listaEntidades.length; clm++)
					{
						if((listaEntidades[clm].value != 'CLM'))
						{
							listaEntidades[clm].checked = false;
							listaEntidades[clm].disabled = true;
						}
							
					}
				}
					
			}
		}
		// Se nenhum tiver sido marcado habilita todos 
		if( ! existeMarcado )
		{
			for ( var em=0;em < listaEntidades.length; em++)
			{
				listaEntidades[em].checked = false;
				listaEntidades[em].disabled = false;	
			}
		}
		// Trata os parametros da requisicao 
		var param =		'&entidades=' + entidadesSelecionadas + 
		 				'&esfera=' + 'municipal';


	}

	// Faz requisição ajax que irá popular as colnas de acordo com o tipo de entidade selecionado 
	var req = new Ajax.Request('par.php?modulo=relatorio/relatorioDadosUnidade&acao=A&requisicao=getColunas', {
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

// Define a esfera escolhida (estadual ou municipal) 
function defineEsfera(esfera)
{
	// Esconde campos da pesquisa 
	$('campos_pesquisa').hide();
	// Caso Estado 
	if( esfera == 1 )
	{
		$('esfera_municipal').hide();
		$('esfera_estadual').show();
		$('tr_municipio').hide();
 
	} // Caso município
	else
	{
		$('esfera_municipal').show();
		$('esfera_estadual').hide();
		$('tr_municipio').show();
		
	}	
	// Atualiza os tipos de entidade 
	changeTipoEntidade();
}

// Função responsável por fazer a requisição para o formulário 
function gerarRelatorio(req){

	// Recupera obj do formulário 
	var formulario = document.formulario;
	
	// Verifica a solicitação e seleciona os combos	
	document.getElementById('req').value = req;
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.tipo_entidade_municipio );
	selectAllOptions( formulario.uf );
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

// Função que filtra os municípios 
function filtraMunicipio(){

		//mostraMunicipio();
		
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

// Mostra os municípios 
function mostraMunicipio(){

	//document.getElementById('agrupador').options.length=0;
	
	var tr_municipio   = document.getElementById('tr_municipio');
	var td_municipio   = document.getElementById('td_municipio');

	var agrupador = document.getElementById('agrupadorOrigem');
	var total = agrupador.length;

	var agrupador = document.getElementById('agrupador');
	var total = agrupador.length;

	for(i=0; i<total; i++){
		if(agrupador[i]){
			if(agrupador[i].value == 'coordenador') {
				document.getElementById('agrupador').remove(i);
			}
		}
	}
		
	var item = document.createElement('option');
	
	if(document.formulario.esfera[1].checked == true || document.formulario.esfera[2].checked == true){
		tr_municipio.style.display = '';
		td_municipio.style.display = '';

	}	
	else{
		td_municipio.style.display = 'none';
		tr_municipio.style.display = 'none';
		
		 item.text = 'Nome do Coordenador';
		 item.value = 'coordenador';
		 
		  try {
			document.getElementById('agrupadorOrigem').add(item, null); // standards compliant; doesn't work in IE
		  }
		  catch(ex) {
			  document.getElementById('agrupadorOrigem').add(item); // IE only
		  }
		
	}
	
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
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		
		<tr>
			<td style="width: 20%;" class="SubTituloDireita" valign="top">Esfera:</td>	
			<td>
				<input type="radio" value="1" name="esfera" id="esfera" onclick="defineEsfera(this.value);" >Estadual
				&nbsp;&nbsp;  
				<input type="radio" value="2" name="esfera" id="esfera" onclick="defineEsfera(this.value);" >Municipal
			</td>
		</tr>
		</table>
	
		<input type="hidden" name="req" id="req" value="" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />
		
		<div id="esfera_municipal" style="display: none;">
			<table id="" class="tabela"  align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			
				<tr>
					<td style="width: 20%;" class="SubTituloDireita" valign="top">Tipos de entidade</td>
					<td>
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="PRF" onchange="changeTipoEntidade();">Prefeitura<br>
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="PRFTO" onchange="changeTipoEntidade();">Prefeito(a)<br>
						
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="SME" onchange="changeTipoEntidade();"> Secretaria Municipal de Educação<br> 
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="DME" onchange="changeTipoEntidade();"> Dirigente Municipal de Educação <br> 
						
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="ELM" onchange="changeTipoEntidade();"> Equipe Local Municipal<br> 
						
						<input type="checkbox" name="tipo_entidade_municipio[]" id="tipo_entidade_municipio[]" value="CLM" onchange="changeTipoEntidade();"> Comitê Local Municipal<br> 
					</td>
				</tr>
				
				
				
			</table>
	</div>
	
	<!-- ESTADUAL -->
	<div id="esfera_estadual" style="display: none;">
		<input type="hidden" name="req" id="req" value="" />
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td style="width: 20%;" class="SubTituloDireita" valign="top">Tipos de entidade</td>
				<td>
					
					<br>
					<input type="checkbox" name="tipo_entidade_estado[]" id="tipo_entidade_estado[]" value="SEE" onchange="changeTipoEntidade();">Secretaria Estadual de Educação  <br>
					<input type="checkbox" name="tipo_entidade_estado[]" id="tipo_entidade_estado[]" value="SECEE" onchange="changeTipoEntidade();">Secretário(a) Estadual de Educação  <br>
					<input type="checkbox" name="tipo_entidade_estado[]" id="tipo_entidade_estado[]" value="ELE" onchange="changeTipoEntidade();">Equipe local<br>
					<input type="checkbox" name="tipo_entidade_estado[]" id="tipo_entidade_estado[]" value="CLE" onchange="changeTipoEntidade();"> Comitê Local<br>
				</td>
			</tr>
		</table>
		</div>

		
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
				<tr id="tr_municipio" style="display:none;">
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
					<td colspan="2" align="center">
						<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
						<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
					
					</td>
				
				</tr>
			</table>
		</div>
		</form>
	

	
	
</body>
<?

// CPF	Nome	Email	Telefone	Formação	Vínculo	Função atual	Órgão	Cargo Efetivo	Exercício	Fase
function colunasMunicipais(){
    return array(
     
    );
}

function colunasEstaduais(){
    return array(
        array('codigo' => 'UF', 'descricao' => 'UF'),
        array('codigo' => 'CNPJ', 'descricao' => 'CNPJ'),
        array('codigo' => 'CPF', 'descricao' => 'CPF'),
        array('codigo' => 'COD_IBGE', 'descricao' => 'Código IBGE'),
        array('codigo' => 'NOME_MUN', 'descricao' => 'Nome do Município'),
        array('codigo' => 'ESFERA', 'descricao' => 'Esfera'),
        array('codigo' => 'ENDERECO', 'descricao' => 'Endereço'),
        array('codigo' => 'CEP_SECRETARIA', 'descricao' => 'CEP da Secretaria'),
        array('codigo' => 'CEP_PREFEITURA', 'descricao' => 'CEP da Prefeitura'),
        array('codigo' => 'NOME_SECRETARIO', 'descricao' => 'Nome do Secretário'),
        array('codigo' => 'NOME_PREFEITO', 'descricao' => 'Nome do Prefeito'),
        array('codigo' => 'SECRETARIA_DE_EDUCACAO', 'descricao' => 'Secretaria de Educação'),
        array('codigo' => 'TEL_PREFEITURA', 'descricao' => 'Telefone da Prefeitura'),
        array('codigo' => 'TEL_SECRETARIA', 'descricao' => 'Telefone da Secretaria')
    );
}



/*
 * 
 * Códigos que irei usar
 * combo_popup_adicionar_item( 'colunaOrigem','4', 'das', true) <<<<<<< adiciona opção na coluna
 * 
 * 
 * */

?>

</html>