<?php
header('content-type: text/html; charset=ISO-8859-1');
/*
if ( isset( $_REQUEST['pesquisa'] ) ){
	include "os_resultado.inc";
	exit();
}
*/
/*
echo "<pre>";
print_r($_REQUEST);
exit;*/
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
		location.href = 'demandas.php?modulo=relatorio/relatorio_os&acao=A';
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
			location.href = 'demandas.php?modulo=relatorio/relatorio_os&acao=A';
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
		case '1':
			include "os_resultado.inc";
			exit;
		case '2':
			include "osxls_resultado.inc";
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
	$anosanteriores = $_REQUEST['anosanteriores'];
	$tiporel = $_REQUEST['tiporel'];
	
	$agrupadorAux = array();
	$colunaAux = array();
	$pesquisacampoAux = array();
	$ordenarcampoAux = array(); 
	
	if ( $_REQUEST['agrupador'] ){
		foreach ( $_REQUEST['agrupador'] as $cod ){
			if($cod == 'origemdemanda') $desc = 'Origem da Demanda';
			elseif($cod == 'celula') $desc = 'Célula';			
			elseif($cod == 'sistema') $desc = 'Sistema';
			elseif($cod == 'setor') $desc = 'Setor';
			elseif($cod == 'solicitante') $desc = 'Solicitante';
			elseif($cod == 'tecnico') $desc = 'Analista / Técnico';
			elseif($cod == 'gestor') $desc = 'Gestor';
			elseif($cod == 'tipodemanda') $desc = 'Tipo da Demanda';
			elseif($cod == 'prioridadeg') $desc = 'Prioridade';
			elseif($cod == 'situacao') $desc = 'Situação';
			elseif($cod == 'avaliacaog') $desc = 'Avaliação';
			elseif($cod == 'classifsistema') $desc = 'Classificação Sistema';
			elseif($cod == 'mesano') $desc = 'Mês / Ano';
			elseif($cod == 'classifdemanda') $desc = 'Classificação da Demanda';
			array_push( $agrupadorAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}

	
	
	if ( $_REQUEST['coluna'] ){
		foreach ( $_REQUEST['coluna'] as $cod ){
			if($cod == 'solicitante') $desc = 'Solicitante';
			elseif($cod == 'origemdemanda') $desc = 'Origem da Demanda';			
			elseif($cod == 'tipodemanda') $desc = 'Tipo da Demanda';
			elseif($cod == 'assunto') $desc = 'Assunto';
			elseif($cod == 'descricao') $desc = 'Descrição';
			elseif($cod == 'dataabertura') $desc = 'Data de Abertura';
			elseif($cod == 'datasituacao') $desc = 'Data da Situação';
			elseif($cod == 'prazoatendimento') $desc = 'Prazo Previsto Calculado';
			elseif($cod == 'prazocatalogo') $desc = 'Prazo Previsto Catálogo';
			elseif($cod == 'tempodecorrido') $desc = 'Duração do Atendimento';
			elseif($cod == 'duracaoatendminutos') $desc = 'Duração do Atendimento Minutos';
			elseif($cod == 'tempopausa') $desc = 'Tempo da Pausa';
			elseif($cod == 'dataconclusao') $desc = 'Data de Conclusão';
			elseif($cod == 'datainicio') $desc = 'Previsão de Início';
			elseif($cod == 'datafim') $desc = 'Previsão de Término';
			elseif($cod == 'tecnico') $desc = 'Analista / Técnico';
			elseif($cod == 'gestor') $desc = 'Gestor';
			elseif($cod == 'tecnicoclassif') $desc = 'Técnico(a) Classificador(a)';
			elseif($cod == 'tecnicoclassifsi') $desc = 'Técnico(a) Classificador(a) SI';
			elseif($cod == 'servicoexec') $desc = 'Serviço Executado';
			elseif($cod == 'avaliacao') $desc = 'Avaliação';
			elseif($cod == 'situacao') $desc = 'Situação';
			elseif($cod == 'observacao') $desc = 'Observação';
			elseif($cod == 'pontuacao') $desc = 'Pontuação';
			elseif($cod == 'qtdservico') $desc = 'Quantidade de Serviço';
			elseif($cod == 'totalpontuacao') $desc = 'Total de Pontuação';
			elseif($cod == 'tempoclassif') $desc = 'Tempo da Classificação';
			elseif($cod == 'tempoclassifcatalogo') $desc = 'Tempo da Classificação Catálogo SI';
			elseif($cod == 'dataclassif') $desc = 'Data da Classificação';
			elseif($cod == 'tempoclassifsi') $desc = 'Tempo da Classificação SI';
			elseif($cod == 'dataclassifsi') $desc = 'Data da Classificação SI';
			elseif($cod == 'prioridade') $desc = 'Prioridade';
			elseif($cod == 'totaldemandas') $desc = 'Total de Demandas';
			elseif($cod == 'valordemanda') $desc = 'Valor da Demanda (R$)';
			elseif($cod == 'tempoadicional') $desc = 'Tempo Adicional Atendimento';
			elseif($cod == 'justtempoadicional') $desc = 'Justificativa do Tempo Adicional';
			elseif($cod == 'justprioridade') $desc = 'Justificativa da Prioridade';
			elseif($cod == 'justurgente') $desc = 'Justificativa de Demanda Urgente';
			elseif($cod == 'nseriepatr') $desc = 'Nº Série / Nº Patrimônio';
			elseif($cod == 'classifdemanda') $desc = 'Classificação das Demandas';
			elseif($cod == 'nss') $desc = 'Solicitação de Serviço (SS)';
			elseif($cod == 'histtramite') $desc = 'Histórico de Tramites';
			array_push( $colunaAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}
	
	
	if ( $_REQUEST['pesquisacampo'] ){
		foreach ( $_REQUEST['pesquisacampo'] as $cod ){
			if($cod == 'assunto') $desc = 'Assunto';
			elseif($cod == 'descricao') $desc = 'Descrição';			
			elseif($cod == 'servicoexecutado') $desc = 'Serviço Executado pelo Técnico';
			elseif($cod == 'solicitante') $desc = 'Solicitante';
			elseif($cod == 'tecnico') $desc = 'Técnico / Analista';
			elseif($cod == 'gestor') $desc = 'Gestor';
			elseif($cod == 'tempoadicional') $desc = 'Justificativa Tempo Adicional';
			array_push( $pesquisacampoAux, array( 'codigo' => $cod, 'descricao' => $desc ));
		}
	}
	
	if ( $_REQUEST['ordenarcampo'] ){
		foreach ( $_REQUEST['ordenarcampo'] as $cod ){
			if($cod == 'codigo') $desc = 'Código';
			elseif($cod == 'dataabertura') $desc = 'Data de Abertura';
			elseif($cod == 'datasituacao') $desc = 'Data da Situação';
			elseif($cod == 'dataconclusao') $desc = 'Data de Conclusão';
			elseif($cod == 'pontuacao') $desc = 'Pontuação';
			elseif($cod == 'qtdservico') $desc = 'Quantidade de Serviço';
			elseif($cod == 'totalpontuacao') $desc = 'Total de Pontuação';
			elseif($cod == 'totaldemandas') $desc = 'Total de Demandas';
			elseif($cod == 'valordemanda') $desc = 'Valor da Demanda (R$)';
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


	function exibeRelatorioGeralXLS(){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );
		var coluna     = document.getElementById( 'coluna' );
		var pesquisacampo     = document.getElementById( 'pesquisacampo' );
		var ordenarcampo     = document.getElementById( 'ordenarcampo' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='2';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		 
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

		var dtFrom,dtTo,dt1,dt2,diff,days = null;

		if(formulario.dtinisit.value != '' && formulario.dtfimsit.value != ''){
			if (!validaDataMaior(formulario.dtinisit, formulario.dtfimsit)){
				alert("O Período da Situação Início não pode ser maior que o Período da Situação Fim.");
				formulario.dtfimsit.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtinisit.value.split("/");
			dtTo = formulario.dtfimsit.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período da Situação não pode ser maior que 12 meses.");
				formulario.dtfimsit.focus();
				return;
			}
		}

		if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){
			if (!validaDataMaior(formulario.dtinicio, formulario.dtfim)){
				alert("O Período de Abertura Início não pode ser maior que o Período de Abertura Fim.");
				formulario.dtfim.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtinicio.value.split("/");
			dtTo = formulario.dtfim.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período de Abertura não pode ser maior que 12 meses.");
				formulario.dtfim.focus();
				return;
			}
		}

		if(formulario.dtiniaf.value != '' && formulario.dtfimaf.value != ''){ 
			if (!validaDataMaior(formulario.dtiniaf, formulario.dtfimaf)){
				alert("O Período do Atendimento Finalizado Início não pode ser maior que o Período do Atendimento Finalizado Fim.");
				formulario.dtfimaf.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtiniaf.value.split("/");
			dtTo = formulario.dtfimaf.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período do Atendimento Finalizado não pode ser maior que 12 meses.");
				formulario.dtfimaf.focus();
				return;
			}
		}
		
		
		
		selectAllOptions( agrupador );
		selectAllOptions( coluna );
		selectAllOptions( pesquisacampo );
		selectAllOptions( ordenarcampo );
		selectAllOptions( document.getElementById( 'origemd' ) );
		selectAllOptions( document.getElementById( 'celula' ) );
		selectAllOptions( document.getElementById( 'sidid' ) );
		selectAllOptions( document.getElementById( 'setor' ) );
		selectAllOptions( document.getElementById( 'solicitante' ) );
		selectAllOptions( document.getElementById( 'tecnico' ) );
		selectAllOptions( document.getElementById( 'gestor' ) );
		selectAllOptions( document.getElementById( 'tecnicoclassif' ) );
		selectAllOptions( document.getElementById( 'situacao' ) );
		selectAllOptions( document.getElementById( 'avaliacao' ) );
		selectAllOptions( document.getElementById( 'classifsistema' ) );
		selectAllOptions( document.getElementById( 'classifdemanda' ) );
		
		formulario.submit();
		
	}


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

		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		selectAllOptions( coluna );
		selectAllOptions( pesquisacampo );
		selectAllOptions( ordenarcampo );

		var dtFrom,dtTo,dt1,dt2,diff,days = null;
		
		if(formulario.dtinisit.value != '' && formulario.dtfimsit.value != ''){ 
			if (!validaDataMaior(formulario.dtinisit, formulario.dtfimsit)){
				alert("O Período da Situação Início não pode ser maior que o Período da Situação Fim.");
				formulario.dtfimsit.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtinisit.value.split("/");
			dtTo = formulario.dtfimsit.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período da Situação não pode ser maior que 12 meses.");
				formulario.dtfimsit.focus();
				return;
			}
		}

		if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){ 
			if (!validaDataMaior(formulario.dtinicio, formulario.dtfim)){
				alert("O Período de Abertura Início não pode ser maior que o Período de Abertura Fim.");
				formulario.dtfim.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtinicio.value.split("/");
			dtTo = formulario.dtfim.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período de Abertura não pode ser maior que 12 meses.");
				formulario.dtfim.focus();
				return;
			}
		}

		if(formulario.dtiniaf.value != '' && formulario.dtfimaf.value != ''){ 
			if (!validaDataMaior(formulario.dtiniaf, formulario.dtfimaf)){
				alert("O Período do Atendimento Finalizado Início não pode ser maior que o Período do Atendimento Finalizado Fim.");
				formulario.dtfimaf.focus();
				return;
			}

			//verifica limite de 12 meses
			dtFrom = formulario.dtiniaf.value.split("/");
			dtTo = formulario.dtfimaf.value.split("/");
			dt1 = new Date(dtFrom[2],dtFrom[1],dtFrom[0]);
			dt2 = new Date(dtTo[2],dtTo[1],dtTo[0]);
			diff = dt2.getTime() - dt1.getTime();
			days = diff/(1000 * 60 * 60 * 24);
			if (days > 365){
				alert("O intervalo do Período do Atendimento Finalizado não pode ser maior que 12 meses.");
				formulario.dtfimaf.focus();
				return;
			}
		}
		

		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'demandas.php?modulo=relatorio/relatorio_os&acao=A';
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
				
								
				formulario.action = 'demandas.php?modulo=relatorio/relatorio_os&acao=A&tipoRelatorio=xls';
				
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
				formulario.action = 'demandas.php?modulo=relatorio/relatorio_os&acao=A&salvar=1';
				
		
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
				selectAllOptions( document.getElementById( 'origemd' ) );
				selectAllOptions( document.getElementById( 'celula' ) );
				selectAllOptions( document.getElementById( 'sidid' ) );
				selectAllOptions( document.getElementById( 'setor' ) );
				selectAllOptions( document.getElementById( 'solicitante' ) );
				selectAllOptions( document.getElementById( 'tecnico' ) );
				selectAllOptions( document.getElementById( 'gestor' ) );
				selectAllOptions( document.getElementById( 'tecnicoclassif' ) );
				selectAllOptions( document.getElementById( 'situacao' ) );
				selectAllOptions( document.getElementById( 'avaliacao' ) );
				selectAllOptions( document.getElementById( 'classifsistema' ) );
				selectAllOptions( document.getElementById( 'classifdemanda' ) );
				
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
			<td class="SubTituloDireita" width="15%">Agrupadores:</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = array();
					$destino = $agrupadorAux;
					
					// Dados padrão de origem
					if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || 
						 in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || 
						 in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || 
						 in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) || 
						 in_array(DEMANDA_PERFIL_DBA, $perfil) || 
						 in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || 
						 in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) ||
						 in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) ||
						 in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil) || 
						 in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || 
						 in_array(DEMANDA_PERFIL_ANALISTA_TECNICO, $perfil) ||
						 in_array(DEMANDA_PERFIL_EMPRESA_TYPE, $perfil) ){
						 	
						$origem = array(
							/*
							'nudemanda' => array(
								'codigo'    => 'nudemanda',
								'descricao' => 'Nº da demanda'
							),*/
							'origem' => array(
								'codigo'    => 'origemdemanda',
								'descricao' => 'Origem da demanda'
							),
							'celula' => array(
								'codigo'    => 'celula',
								'descricao' => 'Célula'
							),
							'sistema' => array(
								'codigo'    => 'sistema',
								'descricao' => 'Sistema'
							),
							'setor' => array(
								'codigo'    => 'setor',
								'descricao' => 'Setor'
							),
							'solicitante' => array(
								'codigo'    => 'solicitante',
								'descricao' => 'Solicitante'
							),
							'tecnico' => array(
								'codigo'    => 'tecnico',
								'descricao' => 'Analista / Técnico'
							),
							'gestor' => array(
								'codigo'    => 'gestor',
								'descricao' => 'Gestor'
							),
							'tipodemanda' => array(
								'codigo'    => 'tipodemanda',
								'descricao' => 'Tipo da Demanda'
							),
							'prioridadeg' => array(
								'codigo'    => 'prioridadeg',
								'descricao' => 'Prioridade'
							),
							'situacao' => array(
								'codigo'    => 'situacao',
								'descricao' => 'Situação'
							),
							'avaliacaog' => array(
								'codigo'    => 'avaliacaog',
								'descricao' => 'Avaliação'
							),
							'classifsistema' => array(
								'codigo'    => 'classifsistema',
								'descricao' => 'Classificação Sistema'
							),
							'mesano' => array(
								'codigo'    => 'mesano',
								'descricao' => 'Mês / Ano'
							),
							'classifdemanda' => array(
								'codigo'    => 'classifdemanda',
								'descricao' => 'Classificação da Demanda'
							)
							
						);
					}
					else{
						$origem = array(
							/*
							'nudemanda' => array(
								'codigo'    => 'nudemanda',
								'descricao' => 'Nº da demanda'
							),*/
							'origem' => array(
								'codigo'    => 'origemdemanda',
								'descricao' => 'Origem da demanda'
							),
							'celula' => array(
								'codigo'    => 'celula',
								'descricao' => 'Célula'
							),
							'sistema' => array(
								'codigo'    => 'sistema',
								'descricao' => 'Sistema'
							),
							'setor' => array(
								'codigo'    => 'setor',
								'descricao' => 'Setor'
							),
							'solicitante' => array(
								'codigo'    => 'solicitante',
								'descricao' => 'Solicitante'
							),
							'tecnico' => array(
								'codigo'    => 'tecnico',
								'descricao' => 'Analista / Técnico'
							),
							'gestor' => array(
								'codigo'    => 'gestor',
								'descricao' => 'Gestor'
							),
							'tipodemanda' => array(
								'codigo'    => 'tipodemanda',
								'descricao' => 'Tipo da Demanda'
							),
							'prioridadeg' => array(
								'codigo'    => 'prioridadeg',
								'descricao' => 'Prioridade'
							),
							'situacao' => array(
								'codigo'    => 'situacao',
								'descricao' => 'Situação'
							),
							'classifsistema' => array(
								'codigo'    => 'classifsistema',
								'descricao' => 'Classificação Sistema'
							),
							'mesano' => array(
								'codigo'    => 'mesano',
								'descricao' => 'Mês / Ano'
							),
							'classifdemanda' => array(
								'codigo'    => 'classifdemanda',
								'descricao' => 'Classificação da Demanda'
							)
							
						);
						
					}	
									
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
					
					// Dados padrão de origem
					if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || 
						 in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) || 
						 in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || 
						 in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) || 
						 in_array(DEMANDA_PERFIL_DBA, $perfil) || 
						 in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || 
						 in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) ||
						 in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) ||
						 in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil) || 
						 in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || 
						 in_array(DEMANDA_PERFIL_ANALISTA_TECNICO, $perfil) || 
						 in_array(DEMANDA_PERFIL_EMPRESA_TYPE, $perfil) ){

						$origem = array(
										array(
												"codigo" 	=> "solicitante",
												"descricao" => "Solicitante"
											 ),
									    array(
									    		"codigo"	=> "origemdemanda",
									    		"descricao" => "Origem da Demanda"
									    	 ),
									    array(
									    		"codigo"	=> "tipodemanda",
									    		"descricao" => "Tipo da Demanda"
									    	 ),
									    array(
									    		"codigo"	=> "assunto",
									    		"descricao" => "Assunto"
									    	 ),
									    array(
									    		"codigo"	=> "descricao",
									    		"descricao" => "Descrição"
									    	 ),
									    array(
									    		"codigo"	=> "dataabertura",
									    		"descricao" => "Data de Abertura"
									    	 ),
									    array(
									    		"codigo"	=> "datasituacao",
									    		"descricao" => "Data da Situação"
									    	 ),
									    array(
									    		"codigo"	=> "prazoatendimento",
									    		"descricao" => "Prazo Previsto Calculado"
									    	 ),
									    array(
									    		"codigo"	=> "prazocatalogo",
									    		"descricao" => "Prazo Previsto Catálogo"
									    	 ),
									    array(
									    		"codigo"	=> "tempodecorrido",
									    		"descricao" => "Duração do Atendimento"
									    	 ),
									    array(
									    		"codigo"	=> "duracaoatendminutos",
									    		"descricao" => "Duração do Atendimento minutos"
									    	 ),
									    array(
									    		"codigo"	=> "tempopausa",
									    		"descricao" => "Tempo da Pausa"
									    	 ),
									    array(
									    		"codigo"	=> "dataconclusao",
									    		"descricao" => "Data de Conclusão"
									    	 ),
									    array(
									    		"codigo"	=> "datainicio",
									    		"descricao" => "Previsão de Início"
									    	 ),
									    array(
									    		"codigo"	=> "datafim",
									    		"descricao" => "Previsão de Término"
									    	 ),
									    array(
									    		"codigo"	=> "tecnico",
									    		"descricao" => "Analista / Técnico"
									    	 ),
									    array(
									    		"codigo"	=> "tecnicoclassif",
									    		"descricao" => "Técnico(a) Classificador(a)"
									    	 ),
									    array(
									    		"codigo"	=> "tecnicoclassifsi",
									    		"descricao" => "Técnico(a) Classificador(a) SI"
									    	 ),
									   	array(
									    		"codigo"	=> "gestor",
									    		"descricao" => "Gestor"
									    	 ),
									    array(
									    		"codigo"	=> "servicoexec",
									    		"descricao" => "Serviço Executado"
									    	 ),
									    array(
									    		"codigo"	=> "avaliacao",
									    		"descricao" => "Avaliação"
									    	 ),
									    array(
									    		"codigo"	=> "situacao",
									    		"descricao" => "Situação"
									    	 ),
									    array(
									    		"codigo"	=> "pontuacao",
									    		"descricao" => "Pontuação"
									    	 ),
									    array(
									    		"codigo"	=> "tempoclassif",
									    		"descricao" => "Tempo da Classificação"
									    	 ),
										array(
									    		"codigo"	=> "tempoclassifcatalogo",
									    		"descricao" => "Tempo da Classificação Catálogo SI"
									    	 ),									    	 
									    array(
									    		"codigo"	=> "dataclassif",
									    		"descricao" => "Data da Classificação"
									    	 ),
									    array(
									    		"codigo"	=> "tempoclassifsi",
									    		"descricao" => "Tempo da Classificação SI"
									    	 ),
									    array(
									    		"codigo"	=> "dataclassifsi",
									    		"descricao" => "Data da Classificação SI"
									    	 ),
									    array(
									    		"codigo"	=> "tempoadicional",
									    		"descricao" => "Tempo Adicional Atendimento"
									    	 ),
									    array(
									    		"codigo"	=> "valordemanda",
									    		"descricao" => "Valor da Demanda (R$)"
									    	 ),
									    array(
									    		"codigo"	=> "qtdservico",
									    		"descricao" => "Quantidade de Serviço"
									    	 ),
									    array(
									    		"codigo"	=> "totalpontuacao",
									    		"descricao" => "Total de Pontuação"
									    	 ),
									    array(
									    		"codigo"	=> "prioridade",
									    		"descricao" => "Prioridade"
									    	 ),
									    array(
									    		"codigo"	=> "observacao",
									    		"descricao" => "Observação" 
									    	 ),
									    array(
									    		"codigo"	=> "totaldemandas",
									    		"descricao" => "Total de Demandas"
									    	 ),
										array(
									    		"codigo"	=> "classifdemanda",
									    		"descricao" => "Classificação das Demandas"
									    	 ),									    	 
									    array(
									    		"codigo"	=> "justtempoadicional",
									    		"descricao" => "Justificativa do Tempo Adicional"
									    	 ),
									    array(
									    		"codigo"	=> "justprioridade",
									    		"descricao" => "Justificativa da Prioridade"
									    	 ),
									    array(
									    		"codigo"	=> "justurgente",
									    		"descricao" => "Justificativa da Demanda Urgente"
									    	 ),
									    array(
									    		"codigo"	=> "nseriepatr",
									    		"descricao" => "Nº Série / Nº Patrimônio"
									    	 ),								    	 
									    array(
									    		"codigo"	=> "qtdhoras",
									    		"descricao" => "Qtd. Horas Atendimento"
									    	 ),								    	 
									    array(
									    		"codigo"	=> "nss",
									    		"descricao" => "Solicitação de Serviço (SS)"
									    	 ),								    	 
									    array(
									    		"codigo"	=> "histtramite",
									    		"descricao" => "Histórico de Tramites"
									    	 )		
									    	 									    	 
										);
					}
					else{
						$origem = array(
										array(
												"codigo" 	=> "solicitante",
												"descricao" => "Solicitante"
											 ),
									    array(
									    		"codigo"	=> "origemdemanda",
									    		"descricao" => "Origem da Demanda"
									    	 ),
									    array(
									    		"codigo"	=> "tipodemanda",
									    		"descricao" => "Tipo da Demanda"
									    	 ),
									    array(
									    		"codigo"	=> "assunto",
									    		"descricao" => "Assunto"
									    	 ),
									    array(
									    		"codigo"	=> "descricao",
									    		"descricao" => "Descrição"
									    	 ),
									    array(
									    		"codigo"	=> "dataabertura",
									    		"descricao" => "Data de Abertura"
									    	 ),
									    array(
									    		"codigo"	=> "datasituacao",
									    		"descricao" => "Data da Situação"
									    	 ),
									    array(
									    		"codigo"	=> "prazoatendimento",
									    		"descricao" => "Prazo Previsto Calculado"
									    	 ),
									    array(
									    		"codigo"	=> "prazocatalogo",
									    		"descricao" => "Prazo Previsto Catálogo"
									    	 ),
									    array(
									    		"codigo"	=> "tempodecorrido",
									    		"descricao" => "Duração do Atendimento"
									    	 ),
									    array(
									    		"codigo"	=> "duracaoatendminutos",
									    		"descricao" => "Duração do Atendimento minutos"
									    	 ),
									    array(
									    		"codigo"	=> "tempopausa",
									    		"descricao" => "Tempo da Pausa"
									    	 ),
									    array(
									    		"codigo"	=> "dataconclusao",
									    		"descricao" => "Data de Conclusão"
									    	 ),
									    array(
									    		"codigo"	=> "datainicio",
									    		"descricao" => "Previsão de Início"
									    	 ),
									    array(
									    		"codigo"	=> "datafim",
									    		"descricao" => "Previsão de Término"
									    	 ),
									    array(
									    		"codigo"	=> "tecnico",
									    		"descricao" => "Analista / Técnico"
									    	 ),
									    array(
									    		"codigo"	=> "tecnicoclassif",
									    		"descricao" => "Técnico(a) Classificador(a)"
									    	 ),
									    array(
									    		"codigo"	=> "gestor",
									    		"descricao" => "Gestor"
									    	 ),
									    array(
									    		"codigo"	=> "servicoexec",
									    		"descricao" => "Serviço Executado"
									    	 ),
									    array(
									    		"codigo"	=> "situacao",
									    		"descricao" => "Situação"
									    	 ),
									    array(
									    		"codigo"	=> "tempoclassif",
									    		"descricao" => "Tempo da Classificação"
									    	 ),
									    array(
									    		"codigo"	=> "tempoadicional",
									    		"descricao" => "Tempo Adicional Atendimento"
									    	 ),
									    array(
									    		"codigo"	=> "qtdservico",
									    		"descricao" => "Quantidade de Serviço"
									    	 ),
									    array(
									    		"codigo"	=> "prioridade",
									    		"descricao" => "Prioridade"
									    	 ),
									    array(
									    		"codigo"	=> "observacao",
									    		"descricao" => "Observação" 
									    	 ),
									    array(
									    		"codigo"	=> "totaldemandas",
									    		"descricao" => "Total de Demandas"
									    	 ),
										array(
									    		"codigo"	=> "classifdemanda",
									    		"descricao" => "Classificação das Demandas"
									    	 ),									    	 
									    array(
									    		"codigo"	=> "justtempoadicional",
									    		"descricao" => "Justificativa do Tempo Adicional"
									    	 ),
									    array(
									    		"codigo"	=> "qtdhoras",
									    		"descricao" => "Qtd. Horas Atendimento"
									    	 ),								    	 
									    array(
									    		"codigo"	=> "histtramite",
									    		"descricao" => "Histórico de Tramites"
									    	 )	
																					    	 								    	 
										);
						
					}
					
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
											"codigo" 	=> "codigo",
											"descricao" => "Código"
										 ),
								    array(
								    		"codigo"	=> "dataabertura",
								    		"descricao" => "Data de Abertura"
								    	 ),
								    array(
								    		"codigo"	=> "datasituacao",
								    		"descricao" => "Data da Situação"
								    	 ),
								    array(
								    		"codigo"	=> "dataconclusao",
								    		"descricao" => "Data de Conclusão"
								    	 ),
								    array(
								    		"codigo"	=> "pontuacao",
								    		"descricao" => "Pontuação"
								    	 ),
								    array(
								    		"codigo"	=> "qtdservico",
								    		"descricao" => "Quantidade de Serviço"
								    	 ),
								    array(
								    		"codigo"	=> "datafimordem",
								    		"descricao" => "Previsão de Término"
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
			
			
				if(!$db->testa_superuser()){					 
			
					$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade 
										where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and ordid is not null
										union
										SELECT c.ordid from demandas.usuarioresponsabilidade ur
										inner join demandas.celula c on c.celid = ur.celid
										where rpustatus='A' and ur.celid is not null and usucpf = '".$_SESSION['usucpf']."'");
					if($ordidx){
						$whereOrdid = " AND ordid IN ('".implode("','",$ordidx)."')";
					}
					
					$celidx = $db->carregarColuna(" SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus = 'A' and usucpf = '".$_SESSION['usucpf']."' and not celid is null");
					if($celidx){
						$whereCelid = " AND ur.celid IN ('".implode("','",$celidx)."')";
					}
					
				}
								
				

				// Origem
				$origemd = array();
				if ( $_REQUEST['origemd'] && $_REQUEST['origemd'][0] != '' )
				{
					$sql_carregados = "SELECT
								ordid AS codigo,
								orddescricao AS descricao
							FROM 
								demandas.origemdemanda
							WHERE ordstatus = 'A'
							and ordid in (".implode(",",$_REQUEST['origemd']).")
							ORDER BY
								2 ";
					$origemd=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								ordid AS codigo,
								orddescricao AS descricao
							FROM 
								demandas.origemdemanda
							WHERE ordstatus = 'A'
							$whereOrdid
							ORDER BY
								2 ";
				mostrarComboPopup( 'Origem:', 'origemd',  $stSql, '', 'Selecione a(s) Origem(ns)' );


				if( $db->testa_superuser() || in_array(1,$ordidx) ){
					// Célula
					$celula = array();
					if ( $_REQUEST['celula'] && $_REQUEST['celula'][0] != '' )
					{
						$sql_carregados = "SELECT
								 c.celid AS codigo,
								 c.celnome || ' - Gerente: ' || 
								 CASE 
								 	 WHEN u.usunome != '' THEN u.usunome
									 ELSE ' - '
								 END as DESCRICAO
								FROM
								 demandas.celula c
					 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
					 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
								WHERE
								 c.celstatus = 'A'
								and c.celid in (".implode(",",$_REQUEST['celula']).")
								ORDER BY
									c.celnome ";
						$celula=$db->carregar( $sql_carregados );
					}
					$stSql = " SELECT
								 c.celid AS codigo,
								 c.celnome || ' - Gerente: ' || 
								 CASE 
								 	 WHEN u.usunome != '' THEN u.usunome
									 ELSE ' - '
								 END as DESCRICAO
								FROM
								 demandas.celula c
					 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
					 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
								WHERE
								 c.celstatus = 'A'
								 $whereCelid
								ORDER BY
								 c.celnome; ";
					mostrarComboPopup( 'Célula:', 'celula',  $stSql, '', 'Selecione a(s) Célula(s)' );
					
					
					// Sistema
					$sidid = array();
					if ( $_REQUEST['sidid'] && $_REQUEST['sidid'][0] != '' )
					{
						$sql_carregados = "select  
												s.sidid  AS codigo, 
												upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
											from 
												demandas.sistemadetalhe s
											left join demandas.sistemacelula c on s.sidid = c.sidid  
											where  s.sidstatus = 'A'
											and s.sidid in (".implode(",",$_REQUEST['sidid']).")
											order by s.sidabrev ";
						
						$sidid=$db->carregar( $sql_carregados );
					}
					$stSql = " select  
									s.sidid  AS codigo, 
									upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
								from 
									demandas.sistemadetalhe s
								left join demandas.sistemacelula ur on s.sidid = ur.sidid  
								where  s.sidstatus = 'A'
								$whereCelid
								order by s.sidabrev ";
					mostrarComboPopup( 'Sistema:', 'sidid',  $stSql, '', 'Selecione o(s) Sistema(s)' );
				}					
					
				
				
				
				// Setor
				$setor = array();
				if ( $_REQUEST['setor'] && $_REQUEST['setor'][0] != '' )
				{
					$sql_carregados = "SELECT
								unaid AS codigo,
								upper(unasigla)||' - '||unadescricao AS descricao
							FROM 
								demandas.unidadeatendimento
							WHERE
								unastatus = 'A' 
							and unaid in (".implode(",",$_REQUEST['setor']).")
							ORDER BY
								2 ";
					$setor=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								unaid AS codigo,
								upper(unasigla)||' - '||unadescricao AS descricao
							FROM 
								demandas.unidadeatendimento
							WHERE
								unastatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Setor:', 'setor',  $stSql, '', 'Selecione o(s) Setor(es)' );

				
				// Classificação da demanda
				$classifdemanda = array();
				if ( $_REQUEST['classifdemanda'] && $_REQUEST['classifdemanda'][0] != '' )
				{
					for($i=0; $i<count($_REQUEST['classifdemanda']); $i++){
						//if($_REQUEST['classifdemanda'][$i] == 'N') $sql_carregados2 .= "select 'N' as codigo, 'Nenhuma' as descricao";
						if($_REQUEST['classifdemanda'][$i] == 'I') $sql_carregados2 .= "select 'I' as codigo, 'Incidente' as descricao";
						if($_REQUEST['classifdemanda'][$i] == 'P') $sql_carregados2 .= "select 'P' as codigo, 'Resolução de problema' as descricao";
						if($_REQUEST['classifdemanda'][$i] == 'M') $sql_carregados2 .= "select 'M' as codigo, 'Requisição de mudança' as descricao";
						if($_REQUEST['classifdemanda'][$i] == 'S') $sql_carregados2 .= "select 'S' as codigo, 'Solicitação de Serviço' as descricao";
						if($i != count($_REQUEST['classifdemanda'])-1) $sql_carregados2 .= " union all ";
						 
					}
					$classifdemanda=$db->carregar( $sql_carregados2 );
				}
				$stSql = " --select 'N' as codigo, 'Nenhuma' as descricao
  						   --union all
						   select 'I' as codigo, 'Incidente' as descricao
						   union all
						   select 'P' as codigo, 'Resolução de problema' as descricao
						   union all
						   select 'M' as codigo, 'Requisição de mudança' as descricao
						   union all
						   select 'S' as codigo, 'Solicitação de Serviço' as descricao
						   ";
				mostrarComboPopup( 'Classificação da Demanda:', 'classifdemanda',  $stSql, '', 'Selecione o(s) Item(ns)' );
				
				
				// Solicitante
				$solicitante = array();
				if ( $_REQUEST['solicitante'] && $_REQUEST['solicitante'][0] != '' )
				{
					$sql_carregados = "SELECT distinct
								u.usucpf AS codigo,
								u.usunome AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							Where ur.rpustatus = 'A'
							and u.usucpf in ('".implode("','",$_REQUEST['solicitante'])."')
							group by u.usucpf, u.usunome
							ORDER BY
								2 ";
					$solicitante=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT distinct
								u.usucpf AS codigo,
								u.usunome AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							Where ur.rpustatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Solicitante:', 'solicitante',  $stSql, '', 'Selecione o(s) Solicitante(s)' );
				
				
				// Analista / Técnico
				$tecnico = array();
				if ( $_REQUEST['tecnico'] && $_REQUEST['tecnico'][0] != '' )
				{
					$sql_carregados = "SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc ||
								(CASE WHEN us.suscod = 'A' THEN
										' (ATIVO)'
									 ELSE
									 	' (INATIVO)'
								END) AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where --ur.rpustatus = 'A' and 
							ur.pflcod in (233,235,237,238,248)
							and u.usucpf in ('".implode("','",$_REQUEST['tecnico'])."')
							group by u.usucpf, u.usunome, p.pfldsc, us.suscod
							ORDER BY
								p.pfldsc, u.usunome ";
					//dbg($sql_carregados,1);
					$tecnico=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc ||
								(CASE WHEN us.suscod = 'A' THEN
										' (ATIVO)' 
									 ELSE
									 	' (INATIVO)' 
								END) AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where --ur.rpustatus = 'A' and 
							ur.pflcod in (233,235,237,238,248)
							$whereCelid
							group by u.usucpf, u.usunome, p.pfldsc, us.suscod
							ORDER BY
								p.pfldsc, u.usunome ";
				mostrarComboPopup( 'Analista / Técnico:', 'tecnico',  $stSql, '', 'Selecione o(s) Analista(s) / Técnico(s)' );

				
				// Tecnico classificador
				$tecnicoclassif = array();
				if ( $_REQUEST['tecnicoclassif'] && $_REQUEST['tecnicoclassif'][0] != '' )
				{
					$sql_carregados = "SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where ur.rpustatus = 'A' and ur.pflcod in (248)
							and u.usucpf in ('".implode("','",$_REQUEST['tecnicoclassif'])."')
							group by u.usucpf, u.usunome, p.pfldsc
							ORDER BY
								p.pfldsc, u.usunome ";
					//dbg($sql_carregados);
					$tecnicoclassif=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where ur.rpustatus = 'A' and ur.pflcod in (248)
							$wheretecnicoclassif
							ORDER BY
								p.pfldsc, u.usunome ";
				mostrarComboPopup( 'Técnico(a) Classificador(a):', 'tecnicoclassif',  $stSql, '', 'Selecione o(s) Técnico(s)' );

				// Gestor
				$gestor = array();
				if ( $_REQUEST['gestor'] && $_REQUEST['gestor'][0] != '' )
				{
					$sql_carregados = "SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where ur.rpustatus = 'A' and ur.pflcod in (232,286)
							and u.usucpf in ('".implode("','",$_REQUEST['gestor'])."')
							group by u.usucpf, u.usunome, p.pfldsc
							ORDER BY
								p.pfldsc, u.usunome ";
					//dbg($sql_carregados);
					$gestor=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								u.usucpf AS codigo,
								u.usunome || ' - ' || p.pfldsc AS descricao
							FROM 
								seguranca.usuario u
							INNER JOIN demandas.usuarioresponsabilidade ur ON ur.usucpf = u.usucpf
							INNER JOIN seguranca.perfil p ON p.pflcod = ur.pflcod
							Where ur.rpustatus = 'A' and ur.pflcod in (232,286)
							$wheregestor
							ORDER BY
								p.pfldsc, u.usunome ";
				mostrarComboPopup( 'Gestor:', 'gestor',  $stSql, '', 'Selecione o(s) Gestor(es)' );
				
				
				// Prioridade
				$prioridade = array();
				if ( $_REQUEST['prioridade'] && $_REQUEST['prioridade'][0] != '' )
				{
					$sql_carregados = "SELECT
								priid AS codigo,
								pridsc AS descricao
							FROM 
								demandas.prioridade
							WHERE
								pristatus = 'A' 
							and priid in (".implode(",",$_REQUEST['prioridade']).")
							ORDER BY
								2 ";
					$prioridade=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								priid AS codigo,
								pridsc AS descricao
							FROM 
								demandas.prioridade
							WHERE
								pristatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Prioridade:', 'prioridade',  $stSql, '', 'Selecione a(s) Prioridade(s)' );
				
				
				// Avaliação
				$avaliacao = array();
				if ( $_REQUEST['avaliacao'] && $_REQUEST['avaliacao'][0] != '' )
				{
					for($i=0; $i<count($_REQUEST['avaliacao']); $i++){
						if($_REQUEST['avaliacao'][$i] == '1') $sql_carregados2 .= "select '1' as codigo, 'Ruim' as descricao";
						if($_REQUEST['avaliacao'][$i] == '2') $sql_carregados2 .= "select '2' as codigo, 'Regular' as descricao";
						if($_REQUEST['avaliacao'][$i] == '3') $sql_carregados2 .= "select '3' as codigo, 'Bom' as descricao";
						if($_REQUEST['avaliacao'][$i] == '4') $sql_carregados2 .= "select '4' as codigo, 'Ótimo' as descricao";
						if($i != count($_REQUEST['avaliacao'])-1) $sql_carregados2 .= " union all ";
						 
					}
					$avaliacao=$db->carregar( $sql_carregados2 );
				}
				$stSql = " select '1' as codigo, 'Ruim' as descricao
  						   union all
						   select '2' as codigo, 'Regular' as descricao
						   union all
						   select '3' as codigo, 'Bom' as descricao
						   union all
						   select '4' as codigo, 'Ótimo' as descricao
						   ";
				mostrarComboPopup( 'Avaliação:', 'avaliacao',  $stSql, '', 'Selecione a(s) Avaliação(ões)' );				
				
				
				// Situação
				$situacao = array();
				if ( $_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '' )
				{
					for($i=0; $i<count($_REQUEST['situacao']); $i++){
						if($_REQUEST['situacao'][$i] == '91,107') $sql_carregados3 .= "select '91,107' as codigo, 'Em Análise' as descricao";
						if($_REQUEST['situacao'][$i] == '92,108') $sql_carregados3 .= "select '92,108' as codigo, 'Em Atendimento' as descricao";
						if($_REQUEST['situacao'][$i] == '100,110') $sql_carregados3 .= "select '100,110' as codigo, 'Cancelada' as descricao";
						if($_REQUEST['situacao'][$i] == '93,111') $sql_carregados3 .= "select '93,111' as codigo, 'Aguardando validação' as descricao";
						if($_REQUEST['situacao'][$i] == '135,136') $sql_carregados3 .= "select '135,136' as codigo, 'Invalidada' as descricao";
						if($_REQUEST['situacao'][$i] == '772,890' || $_REQUEST['situacao'][$i] == '772') $sql_carregados3 .= "select '772,890' as codigo, 'Auditada' as descricao";
						if($_REQUEST['situacao'][$i] == '95,109,170' || $_REQUEST['situacao'][$i] == '95,109') $sql_carregados3 .= "select '95,109,170' as codigo, 'Validada / Validada Sem Pausa / Finalizada' as descricao";
						if($i != count($_REQUEST['situacao'])-1) $sql_carregados3 .= " union all ";
					}
					
					if($sql_carregados3) $situacao = $db->carregar( $sql_carregados3 );					
				}
				$stSql = " select '91,107' as codigo, 'Em Análise' as descricao
  						   union all
						   select '92,108' as codigo, 'Em Atendimento' as descricao
						   union all
						   select '100,110' as codigo, 'Cancelada' as descricao
						   union all
						   select '93,111' as codigo, 'Aguardando validação' as descricao
						   union all
						   select '135,136' as codigo, 'Invalidada' as descricao
						   union all
						   select '772,890' as codigo, 'Auditada' as descricao
						   union all
						   select '95,109,170' as codigo, 'Validada / Validada Fora do Prazo / Finalizada' as descricao ";
				mostrarComboPopup( 'Situação da Demanda:', 'situacao',  $stSql, '', 'Selecione a(s) Situação(ões)' );				
				
				
			?>
		<tr>
			<td class="SubTituloDireita">Situação de Pausa:</td>
			<td>
				<input type="checkbox" name="chkpause" value="S" <?if($chkpause) echo "checked"; ?>> Somente as demandas com pausa.
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Período da Situação da Demanda:</td>
			<td>
				<?= campo_data( 'dtinisit', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfimsit', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período de Abertura da Demanda:</td>
			<td>
				<?= campo_data( 'dtinicio', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período do Atendimento Finalizado:<br>(Independente da situação)</td>
			<td>
				<?= campo_data( 'dtiniaf', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfimaf', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período de Registro de Horas:</td>
			<td>
				<?= campo_data( 'dtinirh', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfimrh', 'S', 'S', '', '' ); ?>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita">Demandas em Atraso:</td>
			<td>
				<input type="checkbox" name="demandaematraso" value="1" <?if($demandaematraso) echo "checked"; ?>> 
				Somente as demandas em atraso.
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
					
					// Dados padrão de origem
					$origem = array(
						'assunto' => array(
							'codigo'    => 'assunto',
							'descricao' => 'Assunto'
						),
						'descricao' => array(
							'codigo'    => 'descricao',
							'descricao' => 'Descrição'
						),
						'servicoexecutado' => array(
							'codigo'    => 'servicoexecutado',
							'descricao' => 'Serviço Executado pelo Técnico'
						),
						'solicitante' => array(
							'codigo'    => 'solicitante',
							'descricao' => 'Solicitante'
						),
						'tecnico' => array(
							'codigo'    => 'tecnico',
							'descricao' => 'Técnico / Analista'
						),
						'gestor' => array(
							'codigo'    => 'gestor',
							'descricao' => 'Gestor'
						),
						'tempoadicional' => array(
							'codigo'    => 'tempoadicional',
							'descricao' => 'Justificativa Tempo Adicional'
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
			<td class="SubTituloDireita">Demandas de Anos Anteriores:</td>
			<td>
				<input type="checkbox" name="anosanteriores" value="1" <?if($anosanteriores) echo "checked"; ?>> 
				Exibir Demandas de Anos Anteriores a 2014 e 2015.
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
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeralXLS();" style="cursor: pointer;"/>
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