<?php
header('content-type: text/html; charset=ISO-8859-1');
/*
if ( isset( $_REQUEST['pesquisa'] ) ){
	include "geral_resultado.inc";
	exit();
}
*/

// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = 'fabrica.php?modulo=relatorio/relatorio_geral&acao=A';
	</script>
	<?
	die;
}
// FIM transforma consulta em pública

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			location.href = 'fabrica.php?modulo=relatorio/relatorio_geral&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta


// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}

	switch($_REQUEST['pesquisa']) {
		
		//html
		case '1':
			include "geral_resultado.inc";
			exit;
			
		//xls	
		case '2':
			include "geral_resultado.inc";
			exit;
			
	}
	
}

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	$carregarAux = $_REQUEST['carregar'];
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
	
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;
	unset( $_REQUEST['form'] );
	unset( $_REQUEST['pesquisa'] );
	$titulo = $_REQUEST['titulo'];
	$chkpause = $_REQUEST['chkpause'];
	$demandaematraso = $_REQUEST['demandaematraso'];
	$tiporel = $_REQUEST['tiporel'];
	
	$agrupadorAux = array();
	$colunaAux = array();
	$pesquisacampoAux = array();
	$ordenarcampoAux = array(); 
	
	
	if ( $_REQUEST['agrupador'] ){
		foreach ( $_REQUEST['agrupador'] as $cod ){
			if($cod == 'sistema') $desc = 'Sistema';
			elseif($cod == 'situacaoss') $desc = 'Situação da SS';
			elseif($cod == 'situacaoos') $desc = 'Situação da OS';
			elseif($cod == 'situacaoosc') $desc = 'Situação da OS Contagem';
			elseif($cod == 'requisitante') $desc = 'Requisitante';
			elseif($cod == 'unidade') $desc = 'Unidade do Requisitante';
			elseif($cod == 'tiposervico') $desc = 'Tipo de Serviço';
			elseif($cod == 'tipoordemservico') $desc = 'Tipo da Ordem de Serviço';
			elseif($cod == 'nss') $desc = 'Nº SS';
			elseif($cod == 'nos') $desc = 'Nº OS';
			array_push( $agrupadorAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}


	if ( $_REQUEST['coluna'] ){
		foreach ( $_REQUEST['coluna'] as $cod ){
			if($cod == 'dataaberturass') $desc = 'Data de Abertura da SS';
			elseif($cod == 'expectativaatendimentoss') $desc = 'Expectativa de Atendimento da SS';			
			elseif($cod == 'nss') $desc = 'Nº SS';
			elseif($cod == 'nos') $desc = 'Nº OS';
			elseif($cod == 'previsaoinicioos') $desc = 'Previsão de início da OS';
			elseif($cod == 'previsaotermino') $desc = 'Previsão de término da OS';
			elseif($cod == 'tipoordemservico') $desc = 'Tipo da Ordem de Serviço';
			elseif($cod == 'qtdpfestimada') $desc = 'Qtd. PF Estimada';
			elseif($cod == 'qtdpfdetalhada') $desc = 'Qtd. PF Detalhada';
			elseif($cod == 'artefatos') $desc = 'Artefatos';
			elseif($cod == 'dtss') $desc = 'Data da Situação SS';
			array_push( $colunaAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}
	
	
	if ( $_REQUEST['pesquisacampo'] ){
		foreach ( $_REQUEST['pesquisacampo'] as $cod ){
			if($cod == 'necessidade') $desc = 'Necessidade';
			elseif($cod == 'justificativa') $desc = 'Justificativa';			
			elseif($cod == 'nss') $desc = 'Nº SS';
			elseif($cod == 'nos') $desc = 'Nº OS';
			array_push( $pesquisacampoAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}
											
	if ( $_REQUEST['ordenarcampo'] ){
		foreach ( $_REQUEST['ordenarcampo'] as $cod ){
			if($cod == 'dataaberturass') $desc = 'Data de Abertura da SS';
			elseif($cod == 'expectativaatendimentoss') $desc = 'Expectativa de Atendimento da SS';
			elseif($cod == 'nss') $desc = 'Nº SS';
			elseif($cod == 'nos') $desc = 'Nº OS';
			elseif($cod == 'previsaoinicioos') $desc = 'Previsão de início da OS';
			elseif($cod == 'previsaoterminoos') $desc = 'Previsão de término da OS';
			elseif($cod == 'qtdpfestimada') $desc = 'Qtd. PF Estimada';
			elseif($cod == 'qtdpfdetalhada') $desc = 'Qtd. PF Detalhada';
			array_push( $ordenarcampoAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}	
	
	
}



include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório Geral";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );


//recupera perfil do usuário
$perfil = arrayPerfil();

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">
<!--



/*
	function exibeRelatorioProcesso(){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );
		
		//prepara_formulario();
		//selectAllOptions( formulario.agrupador );
		
		
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			return false;
		}
		
		selectAllOptions( agrupador );
		
		
		selectAllOptions( document.getElementById( 'origemd' ) );
		selectAllOptions( document.getElementById( 'celula' ) );
		selectAllOptions( document.getElementById( 'setor' ) );
		selectAllOptions( document.getElementById( 'solicitante' ) );
		selectAllOptions( document.getElementById( 'tecnico' ) );
		selectAllOptions( document.getElementById( 'situacao' ) );
		
		//valida data
		if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){
			if(!validaData(formulario.dtinicio)){
				alert("Data Início Inválida.");
				formulario.dtinicio.focus();
				return false;
			}		
			if(!validaData(formulario.dtfim)){
				alert("Data Fim Inválida.");
				formulario.dtinicio.focus();
				return false;
			}		
		}
		
		formulario.target = 'resultadoOs';
		var janela = window.open( '', 'resultadoOs', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
		
		formulario.submit();
		janela.focus();
		
	}
*/	
	
	function exibeRelatorioGeral(tipo){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );
		var coluna     = document.getElementById( 'coluna' );
		var pesquisacampo     = document.getElementById( 'pesquisacampo' );
		var ordenarcampo     = document.getElementById( 'ordenarcampo' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';
		
		if(tipo == 'xls') {
			tipo = 'exibir';
			formulario.pesquisa.value='2';
		}

		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		selectAllOptions( coluna );
		selectAllOptions( pesquisacampo );
		selectAllOptions( ordenarcampo );

		
		if(formulario.dtaberturaini.value != '' && formulario.dtaberturafim.value != ''){ 
			if (!validaDataMaior(formulario.dtaberturaini, formulario.dtaberturafim)){
				alert("O Período de Abertura Início da SS não pode ser maior que o Período de Abertura Fim da SS.");
				formulario.dtaberturafim.focus();
				return;
			}
		}

		if(formulario.dtexpectativaini.value != '' && formulario.dtexpectativafim.value != ''){ 
			if (!validaDataMaior(formulario.dtexpectativaini, formulario.dtexpectativafim)){
				alert("O Período de Expectativa Início da SS não pode ser maior que o Período de Expectativa Fim da SS.");
				formulario.dtexpectativafim.focus();
				return;
			}
		}
		
		if(formulario.dtterminoini.value != '' && formulario.dtterminofim.value != ''){ 
			if (!validaDataMaior(formulario.dtterminoini, formulario.dtterminofim)){
				alert("O Período Previsto de Término Início da OS não pode ser maior que o Período Previsto de Término Fim da OS.");
				formulario.dtterminofim.focus();
				return;
			}
		}

		if(formulario.qtdpfini.value != '' && formulario.qtdpffim.value != ''){ 
			if (formulario.qtdpfini.value > formulario.qtdpffim.value){
				alert("A Qtd. de PF Início não pode ser maior que a Qtd. de PF Fim.");
				formulario.qtdpffim.focus();
				return;
			}
		}
		
		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'fabrica.php?modulo=relatorio/relatorio_geral&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}

				if ( !coluna.options.length ){
					alert( 'Favor selecionar ao menos uma coluna!' );
					return false;
				}

				if ( (!pesquisacampo.options.length && formulario.textopesquisa.value!='') || (pesquisacampo.options.length && formulario.textopesquisa.value=='') ){
					alert( 'Favor selecionar ao menos um campo para pesquisa \ne informe o texto para pesquisa!' );
					return false;
				}
				
								
				formulario.action = 'fabrica.php?modulo=relatorio/relatorio_geral&acao=A&tipoRelatorio=xls';
				
			}else if ( tipo == 'salvar' ){
				
				if ( formulario.titulo.value == '' ) {
					alert( 'É necessário informar o título do relatório!' );
					formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
					$nomesExistentes = $db->carregar( $sqlNomesConsulta );
					if ( $nomesExistentes ){
						foreach ( $nomesExistentes as $linhaNome )
						{
							print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
						}
					}
				?>
				var confirma = true;
				var i, j = nomesExistentes.length;
				for ( i = 0; i < j; i++ ){
					if ( nomesExistentes[i] == formulario.titulo.value ){
						confirma = confirm( 'Deseja alterar a consulta já existente?' );
						break;
					}
				}
				if ( !confirma ){
					return;
				}
				
				formulario.target = '_self';
				formulario.action = 'fabrica.php?modulo=relatorio/relatorio_geral&acao=A&salvar=1';
				
		
			}else if( tipo == 'exibir' ){
			 
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}

				if ( !coluna.options.length ){
					alert( 'Favor selecionar ao menos uma coluna!' );
					return false;
				}

				if ( (!pesquisacampo.options.length && formulario.textopesquisa.value!='') || (pesquisacampo.options.length && formulario.textopesquisa.value=='') ){
					alert( 'Favor selecionar ao menos um campo para pesquisa \ne informe o texto para pesquisa!' );
					return false;
				}
				
				
				selectAllOptions( agrupador );
				selectAllOptions( coluna );
				selectAllOptions( pesquisacampo );
				selectAllOptions( ordenarcampo );
				selectAllOptions( document.getElementById( 'tipoos' ) );
				selectAllOptions( document.getElementById( 'tiposervico' ) );
				selectAllOptions( document.getElementById( 'sistema' ) );
				selectAllOptions( document.getElementById( 'requisitante' ) );
				
				formulario.target = 'resultadoGeral';
				var janela = window.open( '', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		}
		
		formulario.submit();
		
	}
	
	
	
	function tornar_publico( prtid ){
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function excluir_relatorio( prtid ){
		if(confirm('Deseja realmente excluir este relatório?')){
			document.formulario.excluir.value = '1';
			document.formulario.prtid.value = prtid;
			document.formulario.target = '_self';
			document.formulario.submit();
		}
	}
	
	function carregar_consulta( prtid ){
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.formulario.prtid.value = prtid;
		exibeRelatorioGeral( 'relatorio' );
	}

	
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}

	/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	function criarrequisicao() {
		return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
	}
	/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	
	/* FUNÇÃO QUE TRATA O RETORNO */
	var pegarretorno = function () {
		try {
				if ( evXmlHttp.readyState == 4 ) {
					if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
						// criando options
						var x = evXmlHttp.responseText.split("&&");
						for(i=1;i<(x.length-1);i++) {
							var dados = x[i].split("##");
							document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
						}
						var dados = x[0].split("##");
						document.getElementById('usrs').options[0] = new Option(dados[1],dados[0]);
						document.getElementById('usrs').value = cpfselecionado;
					}
					if ( evXmlHttp.dispose ) {
						evXmlHttp.dispose();
					}
					evXmlHttp = null;
				}
			}
		catch(e) {}
	};
	/* FIM - FUNÇÃO QUE TRATA O RETORNO */
			
				
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
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
	
	
//-->
</script>




<form action="" method="post" name="formulario" id="filtro"> 

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', $somenteLeitura, '', 65, 100, '', '', 'left', '', 0, 'id="titulo"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Agrupadores:<br><font color="red">(Selecione pelo menos Nº SS ou Nº OS)</font></td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = array();
					$destino = $agrupadorAux;
					
					$origem = array(
						'sistema' => array(
							'codigo'    => 'sistema',
							'descricao' => 'Sistema'
						),
						'situacaoss' => array(
							'codigo'    => 'situacaoss',
							'descricao' => 'Situação da SS'
						),						
						'situacaoos' => array(
							'codigo'    => 'situacaoos',
							'descricao' => 'Situação da OS'
						),						
						'situacaoosc' => array(
							'codigo'    => 'situacaoosc',
							'descricao' => 'Situação da OS Contagem'
						),						
						'requisitante' => array(
							'codigo'    => 'requisitante',
							'descricao' => 'Requisitante'
						),
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidade do Requisitante'
						),
						'tiposervico' => array(
							'codigo'    => 'tiposervico',
							'descricao' => 'Tipo de Serviço'
						),
						'tipoordemservico' => array(
							'codigo'    => 'tipoordemservico',
							'descricao' => 'Tipo da Ordem de Serviço'
						),
						'nss' => array(
							'codigo'    => 'nss',
							'descricao' => 'Nº SS'
						),
						'nos' => array(
							'codigo'    => 'nos',
							'descricao' => 'Nº OS'
						)
						
					);
						
									
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Colunas:</td>
			<td>
				<?php
					unset($agrupador, $destino, $origem);
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = array();
					$destino = $colunaAux;
					
					$origem = array(
									array(
											"codigo" 	=> "dataaberturass",
											"descricao" => "Data de Abertura da SS"
										 ),
								    array(
								    		"codigo"	=> "expectativaatendimentoss",
								    		"descricao" => "Expectativa de Atendimento da SS"
								    	 ),
								    array(
								    		"codigo"	=> "nss",
								    		"descricao" => "Nº SS"
								    	 ),
								    array(
								    		"codigo"	=> "nos",
								    		"descricao" => "Nº OS"
								    	 ),
								    array(
								    		"codigo"	=> "previsaoinicioos",
								    		"descricao" => "Previsão de início da OS"
								    	 ),
								    array(
								    		"codigo"	=> "previsaoterminoos",
								    		"descricao" => "Previsão de término da OS"
								    	 ),
								    array(
											'codigo'    => 'tipoordemservico',
											'descricao' => 'Tipo da Ordem de Serviço'
										),
								    array(
											'codigo'    => 'qtdpfestimada',
											'descricao' => 'Qtd. PF Estimada'
										),
								    array(
											'codigo'    => 'qtdpfdetalhada',
											'descricao' => 'Qtd. PF Detalhada'
										),
								    array(
											'codigo'    => 'artefatos',
											'descricao' => 'Artefatos'
										),
								    array(
											'codigo'    => 'dtss',
											'descricao' => 'Data da Situação SS'
										)	
										
								    	 
									);
						
					
					
					// exibe coluna
					$agrupador->setOrigem( 'naoColuna', null, $origem );
					$agrupador->setDestino( 'coluna', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita" width="15%">
				Ordenar por:
			</td>
			<td>
				<?php
					unset($agrupador, $destino, $origem);
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = array();
					$destino = $ordenarcampoAux;
					
					$origem = array(
								    array(
								    		"codigo"	=> "dataaberturass",
								    		"descricao" => "Data de Abertura da SS"
								    	 ),
									array(
								    		"codigo"	=> "expectativaatendimentoss",
								    		"descricao" => "Expectativa de Atendimento da SS"
								    	 ),								    	 
								    array(
								    		"codigo"	=> "nss",
								    		"descricao" => "Nº SS"
								    	 ),
								    array(
								    		"codigo"	=> "nos",
								    		"descricao" => "Nº OS"
								    	 ),
								    array(
								    		"codigo"	=> "previsaoinicioos",
								    		"descricao" => "Previsão de início da OS"
								    	 ),
								    array(
								    		"codigo"	=> "previsaoterminoos",
								    		"descricao" => "Previsão de término da OS"
								    	 ),
								    array(
											'codigo'    => 'qtdpfestimada',
											'descricao' => 'Qtd. PF Estimada'
										),
								    array(
											'codigo'    => 'qtdpfdetalhada',
											'descricao' => 'Qtd. PF Detalhada'
										)
								    	 
									);
					
					// exibe ordernarcampo
					$agrupador->setOrigem( 'naoOrdenarcampo', null, $origem );
					$agrupador->setDestino( 'ordenarcampo', null, $destino );
					$agrupador->exibir();
				?>
				&nbsp;<select name="tipoordenar">
					<option value='ASC'>CRESCENTE</option>
					<option value='DESC'>DECRESCENTE</option>
				</select>
			</td>
		</tr>		

		</table>
		
		
		<!-- OUTROS FILTROS -->
	
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'outros' );">
					<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
					Relatórios Gerenciais
					<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
				</td>
			</tr>
		</table>
		<div id="outros_div_filtros_off">
			<!--
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
				<tr>
					<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
				</tr>
			</table>
			-->
		</div>
	
		<div id="outros_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
						<?php
						
							//if( $db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) || 
								//possuiPerfil(PERFIL_ADMINISTRADOR) ){
							 	//$bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
							//} 
						
							$sql = sprintf(
								"SELECT Case when prtpublico = true and usucpf = '%s' then 
												'<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
												<img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
												<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
											 ELSE 
											 	'<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ');\">' 
										END as acao, 
										'<a title=\" Carregar consulta no formulário \" href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao 
										FROM public.parametros_tela 
										WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							/*
							$sql = sprintf(
								"SELECT 'abc' as acao, prtdsc FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['mnuid']
							);
							*/
							$cabecalho = array('Ação', 'Nome');
						?>
						<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
						</td>
					</tr>
			</table>
		</div>
		<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

		<!-- FIM OUTROS FILTROS -->
		
		<!-- MINHAS CONSULTAS -->
		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
				</td>
			</tr>
		</table>
		<div id="minhasconsultas_div_filtros_off">
		</div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
						
							$sql = sprintf(
								"SELECT 
									CASE WHEN prtpublico = false THEN 
											 CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN
											 	'<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
											 	 <img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
											 	 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
											 ELSE
											 	'<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">'
											 END 
										 ELSE 
											 CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN
											 	'<img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
											 	 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
											 ELSE
											 	'<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">'
											 END 
									 END as acao, 
									--'<div id=\"nome_' || prtid || '\">' || prtdsc || '</div>' as descricao
									'<a title=\" Carregar consulta no formulário \" href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao 
								 FROM 
								 	public.parametros_tela 
								 WHERE 
								 	mnuid = %d AND usucpf = '%s'",
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							//dbg($sql,1);
							$cabecalho = array('Ação', 'Nome');
						?>
						<td>
							<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
						</td>
					</tr>
			</table>
		</div>
		<!-- FIM MINHAS CONSULTAS -->
		
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
			<?php
			
				// Tipo de Serviço
				$tiposervico = array();
				if ( $_REQUEST['tiposervico'] && $_REQUEST['tiposervico'][0] != '' )
				{
					$sql_carregados = "SELECT
								tpsid AS codigo,
								tpsdsc AS descricao
							FROM 
								fabrica.tiposervico
							WHERE
								tpsstatus = 'A' 
							and tpsid in (".implode(",",$_REQUEST['tiposervico']).")
							ORDER BY
								2 ";
					$tiposervico=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								tpsid AS codigo,
								tpsdsc AS descricao
							FROM 
								fabrica.tiposervico
							WHERE
								tpsstatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Tipo de Serviço:', 'tiposervico',  $stSql, '', 'Selecione o(s) Tipo(s)' );				
			
				
				// Tipo da Ordem de Serviço
				$tipoos = array();
				if ( $_REQUEST['tipoos'] && $_REQUEST['tipoos'][0] != '' )
				{
					$sql_carregados = "SELECT
								tosid AS codigo,
								tosdsc AS descricao
							FROM 
								fabrica.tipoordemservico
							WHERE
								tosstatus = 'A' 
							and tosid in (".implode(",",$_REQUEST['tipoos']).")
							ORDER BY
								2 ";
					$tipoos=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								tosid AS codigo,
								tosdsc AS descricao
							FROM 
								fabrica.tipoordemservico
							WHERE
								tosstatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Tipo da Ordem de Serviço:', 'tipoos',  $stSql, '', 'Selecione o(s) Tipo(s)' );

				// sistemas
				$sistema = array();
				if ( $_REQUEST['sistema'] && $_REQUEST['sistema'][0] != '' )
				{
					$sql_carregados = "SELECT
								sidid AS codigo,
								siddescricao AS descricao
							FROM 
								demandas.sistemadetalhe
							WHERE
								sidstatus = 'A' 
							and sidid in (".implode(",",$_REQUEST['sistema']).")
							ORDER BY
								2 ";
					$sistema=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								sidid AS codigo,
								siddescricao AS descricao
							FROM 
								demandas.sistemadetalhe
							WHERE
								sidstatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Sistemas:', 'sistema',  $stSql, '', 'Selecione o(s) Sistema(s)' );				
				
				
				
				
				// Requisitante
				$requisitante = array();
				if ( $_REQUEST['requisitante'] && $_REQUEST['requisitante'][0] != '' )
				{
					$sql_carregados = "SELECT distinct
								u.usucpf AS codigo,
								u.usunome AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
							Where us.suscod = 'A' and sisid = ".$_SESSION['sisid']."
							and u.usucpf in ('".implode("','",$_REQUEST['requisitante'])."')
							group by u.usucpf, u.usunome
							ORDER BY
								2 ";
					$requisitante=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT distinct
								u.usucpf AS codigo,
								u.usunome AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
							Where us.suscod = 'A' and sisid = ".$_SESSION['sisid']."
							ORDER BY
								2 ";
				mostrarComboPopup( 'Requisitante:', 'requisitante',  $stSql, '', 'Selecione o(s) Requisitante(s)' );
				
				// situação SS
				$requisitante = array();
				if ( $_REQUEST['sitss'] && $_REQUEST['sitss'][0] != '' )
				{
					$sql_carregados = "SELECT esd.esdid as codigo,esd.esddsc as descricao
							FROM workflow.estadodocumento esd
							WHERE tpdid in (select tpdid from workflow.tipodocumento where tpdid = ".TIPO_SITUACAO_SOLICITACAO." and sisid = ".SISID_FABRICA.")
                            AND esd.esdstatus = 'A'
                            and esd.esdid in (".implode(",",$_REQUEST['sitss']).")
							order by esd.esdordem";
					$requisitante=$db->carregar( $sql_carregados );
				}
				
				$stSql = "SELECT esd.esdid as codigo,esd.esddsc as descricao
							FROM workflow.estadodocumento esd
							WHERE tpdid in (select tpdid from workflow.tipodocumento where tpdid = ".TIPO_SITUACAO_SOLICITACAO." and sisid = ".SISID_FABRICA.")
                            AND esd.esdstatus = 'A'
							order by esd.esdordem";
				
				mostrarComboPopup( 'Situação da SS:', 'sitss',  $stSql, '', 'Selecione a(s) Situação(ões)' );
				
				
			?>
		<tr>
			<td class="SubTituloDireita">Período da Situação da SS:</td>
			<td>
				<?= campo_data( 'dtiniss', 'N', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfimss', 'N', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período de Abertura da SS:</td>
			<td>
				<?= campo_data( 'dtaberturaini', 'N', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtaberturafim', 'N', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período da Expectativa de Atendimento da SS:</td>
			<td>
				<?= campo_data( 'dtexpectativaini', 'N', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtexpectativafim', 'N', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período Previsto de Término da OS:</td>
			<td>
				<?= campo_data( 'dtterminoini', 'N', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtterminofim', 'N', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Intervalo Qtd. de P.F. Estimada:</td>
			<td>
				<?= campo_texto( 'qtdpfeini', 'N', 'S', '', 9, 11, '#########', '', 'left', '', 0, 'id="qtdpfini"'); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_texto( 'qtdpfefim', 'N', 'S', '', 9, 11, '#########', '', 'left', '', 0, 'id="qtdpffim"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Intervalo Qtd. de P.F. Detalhada:</td>
			<td>
				<?= campo_texto( 'qtdpfdini', 'N', 'S', '', 9, 11, '#########', '', 'left', '', 0, 'id="qtdpfini"'); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_texto( 'qtdpfdfim', 'N', 'S', '', 9, 11, '#########', '', 'left', '', 0, 'id="qtdpffim"'); ?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita" width="15%">Campos para Pesquisa:</td>
			<td>
				<?php
					unset($agrupador, $destino, $origem);
					
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = array();
					$destino = $pesquisacampoAux;
					
					$origem = array(
						'necessidade' => array(
							'codigo'    => 'necessidade',
							'descricao' => 'Necessidade'
						),
						'justificativa' => array(
							'codigo'    => 'justificativa',
							'descricao' => 'Justificativa'
						),
						'nss' => array(
							'codigo'    => 'nss',
							'descricao' => 'Nº SS'
						),
						'nos' => array(
							'codigo'    => 'nos',
							'descricao' => 'Nº OS'
						)
					);
					
					// exibe 
					$agrupador->setOrigem( 'naoPesquisacampo', null, $origem );
					$agrupador->setDestino( 'pesquisacampo', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Texto para Pesquisa:</td>
			<td>
				<?= campo_texto( 'textopesquisa', 'N', 'S', '', 45, 250, '', ''); ?> 
				<?//= campo_texto( 'assuntodesc', 'N', 'S', '', 45, 250, '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo Relatório:</td>
			<td>
				<input type="radio" name="tiporel" value="1" <?if($tiporel=='1' || !$tiporel) echo "checked"; ?>> 
				Detalhado
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="tiporel" value="2" <?if($tiporel=='2') echo "checked"; ?>> 
				Resumido
			</td>
		</tr>
		
		
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<!--
					<input type="button" value="Visualizar" onclick="exibeRelatorioProcesso();" style="cursor: pointer;"/>
				 
					<input type="button" value="Gerar Arquivo XLS" onclick="" style="cursor: pointer;"/>
				 -->
				
				 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				  &nbsp;&nbsp;&nbsp;
				   
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
				  &nbsp;&nbsp;&nbsp;
				   
				 <input type="button" value="Salvar Consulta" onclick="exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>

<?if($carregarAux){?>
	<script>
		onOffBloco( 'outros' );
		onOffBloco( 'minhasconsultas' );
	</script>
<?}?>