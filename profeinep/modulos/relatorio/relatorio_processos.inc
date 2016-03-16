<?php
 

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		/*
		?>
		<script language="text/javascript">
			if ( !confirm( 'Deseja alterar a consulta já existente?' ) )
			{
				history.back();
			}
		</script>
		<?
		*/
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=relatorio/relatorio_processos&acao=A';
	</script>
	<?
	die;
}

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
		location.href = '?modulo=relatorio/relatorio_processos&acao=A';
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
			location.href = '?modulo=relatorio/relatorio_processos&acao=A';
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
			include "processos_resultado.inc";
			exit;
		case '2':
			include "processos_resultado.inc";
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
	
	$agrupador2 = array();
	
	if ( $_REQUEST['agrupador'] ){
		
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
			array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
		}
		
	}
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Processos";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script type="text/javascript"><!--

	
function tornar_publico( prtid ){
//	location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
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
	exibeRelatorioProcesso( 'relatorio' );
}

	function exibeRelatorioProcesso( tipo ){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );


		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		

		if ( tipo == 'salvar' ){

			
			selectAllOptions( agrupador );		

			selectAllOptions( document.getElementById( 'coordenacao' ) );
			selectAllOptions( document.getElementById( 'advogado' ) );
			selectAllOptions( document.getElementById( 'tema' ) );
			selectAllOptions( document.getElementById( 'procedencia' ) );
			selectAllOptions( document.getElementById( 'tipoprocesso' ) );
			selectAllOptions( document.getElementById( 'prioridade' ) );
			selectAllOptions( document.getElementById( 'situacao' ) );

			
			if ( formulario.titulo.value == '' ) {
				alert( 'É necessário informar a descrição do relatório!' );
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
			formulario.action = 'profeinep.php?modulo=relatorio/relatorio_processos&acao=A&salvar=1';
			formulario.submit();
				
		}else if ( tipo == 'relatorio' ){
			
			formulario.action = 'profeinep.php?modulo=relatorio/relatorio_processos&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			formulario.submit();

		} else {
		
			var formulario = document.formulario;
			var agrupador  = document.getElementById( 'agrupador' );
			
			
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
				return false;
			}
			
			selectAllOptions( agrupador );
			
			
			selectAllOptions( document.getElementById( 'coordenacao' ) );
			selectAllOptions( document.getElementById( 'advogado' ) );
			selectAllOptions( document.getElementById( 'tema' ) );
			selectAllOptions( document.getElementById( 'procedencia' ) );
			selectAllOptions( document.getElementById( 'tipoprocesso' ) );
			selectAllOptions( document.getElementById( 'prioridade' ) );
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
			
			formulario.target = 'resultadoProcessos';
			var janela = window.open( '?modulo=relatorio/relatorio_processos&acao=A', 'resultadoProcessos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			
			formulario.submit();
			janela.focus();
		
		}
		
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

//
--></script>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript">

$(document).ready(function()
{
	$('#td_procedencia').click(function(){ exibeComboProcedencia(); });
	$('#procedencia_todos').click(function(){ exibeComboProcedencia(); });

	$('#procedencia').dblclick(function()
	{
		var janela = window.open('profeinep.php?modulo=relatorio/listaProcedencia&acao=A', 'relatorio', 'width=400,height=450,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		janela.focus();
	});
});

function exibeComboProcedencia()
{
	if( $('#procedencia_todos').css('display') != 'none' )
	{
		$('#procedencia_todos').hide();
		$('#procedencia_combo').show();
	}
	else
	{
		$('#procedencia_todos').show();
		$('#procedencia_combo').hide();
	}
}

function gerarRelatorioXLS(){
	document.getElementById('req').value = 'geraxls';
	exibeRelatorioProcesso();
}

</script>

<form action="" method="post" name="formulario" id="filtro">
	
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', $somenteLeitura, '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupadores:</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					
					// Dados padrão de destino (nulo)
					if(!$agrupador2){
						$destino = array('nuprocesso' => array(
											'codigo'    => 'nuprocesso',
											'descricao' => 'Nº do Processo'
										)
									);
					}
					else{
						$destino = $agrupador2;
					}
					
					// Dados padrão de origem
					$origem = array(
						'coordenacao' => array(
							'codigo'    => 'coordenacao',
							'descricao' => 'Unidade'
						),
						'advogado' => array(
							'codigo'    => 'advogado',
							'descricao' => 'Advogado'
						),
						'tema' => array(
							'codigo'    => 'tema',
							'descricao' => 'Tema'
						),
						'procedencia' => array(
							'codigo'    => 'procedencia',
							'descricao' => 'Procedência'
						),
						'tipoprocesso' => array(
							'codigo'    => 'tipoprocesso',
							'descricao' => 'Tipo de Processo'
						),	
						'prioridade' => array(
							'codigo'    => 'prioridade',
							'descricao' => 'Prioridade'
						),
						'nuprocesso' => array(
							'codigo'    => 'nuprocesso',
							'descricao' => 'Nº do Processo'
						),
						'cadastrador' => array(
							'codigo'    => 'cadastrador',
							'descricao' => 'Cadastrador'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
				?>
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
				$arrVisivel = array("descricao");
				$arrOrdem = array("descricao");
				
				// Tipos de Processos
				$stSql = " SELECT
								tprid AS codigo,
								tprdsc AS descricao
							FROM 
								profeinep.tipoprocesso
							WHERE tprstatus = 'A'
							%s
							ORDER BY
								2 ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo de Processo:', 'tipoprocesso',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Processo(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);

 				?>
				
				<tr>
					<td class="SubTituloDireita" width="195" valign="top" id="td_procedencia">
						Procedência:
					</td>
					<td>
						<div id="procedencia_todos" style="display:inline;">
							<img border="0" align="middle" src="../imagens/combo-todos.gif" />
						</div>
						<div id="procedencia_combo" style="display:none;">
							<select id="procedencia" name="procedencia[]" class="campoEstilo" style="width:400px;" size="10" multiple="multiple">
								<option value="">Duplo clique para selecionar da lista</option>
							</select>
						</div>
					</td>
				</tr>
				
				<?php
				// Tema
				$stSql = " SELECT
								tasid AS codigo,
								tasdsc AS descricao
							FROM 
								profeinep.tipoassunto
							ORDER BY
								2 ";
				mostrarComboPopup( 'Tema:', 'tema',  $stSql, $stSqlCarregados, 'Selecione o(s) Tema(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
				?>
				<tr>
					<td class="SubTituloDireita">Urgente:</td>
					<td>
					<?php 
					// Prioridade
					$stSql = " SELECT
									'1' as codigo,
									'Sim' as descricao
								UNION ALL
								SELECT
									'2' as codigo,
									'Não' as descricao";
					$db->monta_radio('tipid',$stSql,'S',0)
//					mostrarComboPopup( 'Prioridade:', 'prioridade',  $stSql, $stSqlCarregados, 'Selecione a(s) Prioridade(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
					?>
					</td>
				</tr>
				<?php 
				
				
				// Advogado
				$advogado = array();
				if ( $_REQUEST['advogado'] && $_REQUEST['advogado'][0] != '' )
				{
					$sql_carregados = "SELECT
								a.advid AS codigo,
								e.entnome AS descricao
							FROM 
								profeinep.advogados a
								INNER JOIN entidade.entidade e ON e.entid=a.entid and a.advid in (".implode(",",$_REQUEST['advogado']).")
							where
								advstatus='A'
								and entstatus='A'
							ORDER BY
								2 ";
					$advogado=$db->carregar( $sql_carregados );
				}				
				$stSql = " SELECT
								a.advid AS codigo,
								e.entnome AS descricao
							FROM 
								profeinep.advogados a
							    INNER JOIN entidade.entidade e ON e.entid=a.entid
							where
								advstatus='A'
								and entstatus='A'
							ORDER BY
								2 ";
				mostrarComboPopup( 'Advogado:', 'advogado',  $stSql, '', 'Selecione o(s) Advogado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
				

				// Coordenação
				$stSql = " SELECT
								coonid AS codigo,
								coodsc AS descricao
							FROM 
								profeinep.coordenacao
							ORDER BY
								2 ";
				mostrarComboPopup( 'Unidade:', 'coordenacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
				
				
				
				// Situação
				$stSql = " SELECT
								esdid AS codigo,
								esddsc AS descricao 
							FROM 
								workflow.estadodocumento
							WHERE 
								tpdid = " . TIPODOC . " AND
								esdstatus = 'A'
							ORDER BY
								2 ";
				mostrarComboPopup( 'Situação:', 'situacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);				
			?>
		<tr>
			<td class="SubTituloDireita">Período da Entrada do Processo:</td>
			<td>
				<?= campo_data( 'dtinicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expandir Relatório:</td>
			<?
			$arExp = array(
							array("codigo" => 'true',
								  "descricao" => 'Sim'),
							array("codigo" => '',
								  "descricao" => 'Não')								
						  );
			?>
			<td><?=$db->monta_combo("expandir", $arExp, 'S','','', '', '',240,'N','expandir', '', '', 'Expandir'); ?></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioProcesso();" style="cursor: pointer;"/>
				<input type="button" value="VisualizarXLS" onclick="gerarRelatorioXLS();">
				<input type="button" value="Salvar Consulta" onclick="exibeRelatorioProcesso('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
