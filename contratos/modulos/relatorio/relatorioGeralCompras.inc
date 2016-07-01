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
		location.href = '?modulo=relatorio/relatorioGeralCompras&acao=A';
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
		location.href = 'evento.php?modulo=relatorio/relatorioGeralCompras&acao=A';
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
			location.href = 'evento.php?modulo=relatorio/relatorioGeralCompras&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta


// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){
	
	if ( $_REQUEST['prtid'] ){
		$tipoRelatorio = $_REQUEST['tipoRelatorio'];
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		$_REQUEST['tipoRelatorio'] = $tipoRelatorio;
		unset( $_REQUEST['salvar'] );
		
	}

	include "os_resultado_compras.inc";
	exit;
	
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
	$titulo = $_REQUEST['titulo'];
	$tiporel = $_REQUEST['tiporel'];
	
	$agrupadorAux = array();
	
	if ( $_REQUEST['agrupador'] ){
		foreach ( $_REQUEST['agrupador'] as $cod ){
			if($cod == 'uasg') $desc = 'UASG';
			elseif($cod == 'item') $desc = 'Item';			
			elseif($cod == 'uf') $desc = 'UF';
			elseif($cod == 'logradouro') $desc = 'Logradouro';
			elseif($cod == 'situacao') $desc = 'Situação';
			elseif($cod == 'nprocesso') $desc = 'Nº Processo';
			array_push( $agrupadorAux, array( 'codigo' => $cod, 'descricao' => $desc ));
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


	

	
	function exibeRelatorioGeral(tipo){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );

		prepara_formulario();
		selectAllOptions( formulario.agrupador );

		formulario.tipoRelatorio.value='html';

		
		if ( tipo == 'relatorio' ){

			formulario.action = 'evento.php?modulo=relatorio/relatorioGeralCompras&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}

				formulario.tipoRelatorio.value='xls';
				formulario.action = 'evento.php?modulo=relatorio/relatorioGeralCompras&acao=A';
				formulario.target = '_self';
				
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
				formulario.action = 'evento.php?modulo=relatorio/relatorioGeralCompras&acao=A&salvar=1';

			}else if( tipo == 'exibir' ){
			 
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}

				formulario.prtid.value='';
				formulario.target = 'resultadoGeral';
				var janela = window.open( '', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		}

		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'nprocesso' ) );
		selectAllOptions( document.getElementById( 'uf' ) );
		selectAllOptions( document.getElementById( 'uasg' ) );
		selectAllOptions( document.getElementById( 'logradouro' ) );
		selectAllOptions( document.getElementById( 'item' ) );
		selectAllOptions( document.getElementById( 'situacao' ) );
		
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
	<input type="hidden" name="tipoRelatorio" value=""/>
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
					
					$origem = array(
						
						'uasg' => array(
							'codigo'    => 'uasg',
							'descricao' => 'UASG'
						),
						'item' => array(
							'codigo'    => 'item',
							'descricao' => 'Item'
						),
						'uf' => array(
							'codigo'    => 'uf',
							'descricao' => 'UF'
						),
						'logradouro' => array(
							'codigo'    => 'logradouro',
							'descricao' => 'Logradouro'
						),
						'situacao' => array(
							'codigo'    => 'situacao',
							'descricao' => 'Situação'
						),
						'nprocesso' => array(
							'codigo'    => 'nprocesso',
							'descricao' => 'Nº Processo'
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
							
							$cabecalho = array('Ação', 'Nome');
						?>
						<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
						</td>
					</tr>
			</table>
		</div>

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

				// Nº PROCESSO
				$nprocesso = array();
				if ( $_REQUEST['nprocesso'] && $_REQUEST['nprocesso'][0] != '' )
				{
					$sql_carregados = "SELECT
								copid AS codigo,
								upper(copnumprocesso)||' - '||copdsc AS descricao
							FROM 
								evento.coprocesso
							WHERE copstatus = 'A' 
							and copid in (".implode(",",$_REQUEST['nprocesso']).")
							ORDER BY
								2 ";
					$nprocesso=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								copid AS codigo,
								upper(copnumprocesso)||' - '||copdsc AS descricao
							FROM 
								evento.coprocesso
							WHERE
								copstatus = 'A'
							ORDER BY
								2 ";
				mostrarComboPopup( 'Nº Processo:', 'nprocesso',  $stSql, '', 'Selecione a(s) UF(s)' );

				// UF
				$uf = array();
				if ( $_REQUEST['uf'] && $_REQUEST['uf'][0] != '' )
				{
					$sql_carregados = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							WHERE uf in ('".implode("','",$_REQUEST['uf'])."')
							ORDER BY
								2 ";
					$uf=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							ORDER BY
								2 ";
				mostrarComboPopup( 'UF:', 'uf',  $stSql, '', 'Selecione a(s) UF(s)' );
				
				// UASG
				$uasg = array();
				if ( $_REQUEST['uasg'] && $_REQUEST['uasg'][0] != '' )
				{
					$sql_carregados = "SELECT
								usgid AS codigo,
								upper(usgcod)||' - '||usgdsc AS descricao
							FROM 
								evento.uasg
							WHERE usgid in (".implode(",",$_REQUEST['uasg']).")
							ORDER BY
								2 ";
					$uasg=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								usgid AS codigo,
								upper(usgcod)||' - '||usgdsc AS descricao
							FROM 
								evento.uasg
							ORDER BY
								2 ";
				mostrarComboPopup( 'UASG:', 'uasg',  $stSql, '', 'Selecione a(s) Unidade(s)' );

				// LOGRADOURO
				$logradouro = array();
				if ( $_REQUEST['logradouro'] && $_REQUEST['logradouro'][0] != '' )
				{
					$sql_carregados = "SELECT
								coeid AS codigo,
								coe.coenddsc || ', ' || coe.coendlog || ', ' || coe.coendnum || ', ' || coe.coendcom || ' ' || coe.coendbai || ', CEP: ' || coe.coendcep || ', ' || mun.mundescricao || '-' || coe.estuf AS descricao
							FROM 
								evento.coenderecoentrega coe
								inner join territorios.municipio mun on coe.muncod = mun.muncod
							WHERE coeid in (".implode(",",$_REQUEST['logradouro']).")
							ORDER BY
								2 ";
					$logradouro=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								coeid AS codigo,
								coe.coenddsc || ', ' || coe.coendlog || ', ' || coe.coendnum || ', ' || coe.coendcom || ' ' || coe.coendbai || ', CEP: ' || coe.coendcep || ', ' || mun.mundescricao || '-' || coe.estuf AS descricao
							FROM 
								evento.coenderecoentrega coe
								inner join territorios.municipio mun on coe.muncod = mun.muncod
							ORDER BY
								2 ";
				mostrarComboPopup( 'Logradouro:', 'logradouro',  $stSql, '', 'Selecione a(s) Logradouro(s)' );

				
				// ITEM
				$item = array();
				if ( $_REQUEST['item'] && $_REQUEST['item'][0] != '' )
				{
					$sql_carregados = "SELECT
								coiid AS codigo,
								coidsc AS descricao
							FROM 
								evento.coitem
							WHERE coistatus = 'A' 
							and coiid in (".implode(",",$_REQUEST['item']).")
							ORDER BY
								2 ";
					$item=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								coiid AS codigo,
								coidsc AS descricao
							FROM 
								evento.coitem
							WHERE coistatus = 'A'
							ORDER BY
								2 ";
				mostrarComboPopup( 'Item:', 'item',  $stSql, '', 'Selecione o(s) Item(ns)' );
				
				// SITUAÇÃO
				$situacao = array();
				if ( $_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '' )
				{
					$sql_carregados = "SELECT
								esdid AS codigo,
								esddsc AS descricao
							FROM 
								evento.coitem
							WHERE coistatus = 'A' 
							and esdid in (".implode(",",$_REQUEST['situacao']).")
							ORDER BY
								e.esdordem ";
					$situacao=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								esdid AS codigo,
								esddsc AS descricao
							FROM 
								workflow.estadodocumento e
							INNER JOIN workflow.tipodocumento t ON t.tpdid = e.tpdid
							WHERE e.esdstatus = 'A' 
							and e.tpdid = 6
							and t.sisid = {$_SESSION['sisid']}
							ORDER BY
								e.esdordem ";
				mostrarComboPopup( 'Situação:', 'situacao',  $stSql, '', 'Selecione o(s) Item(ns)' );
				
			?>

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
				
				 <input type="button" value="Visualizar HTML" onclick="exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				  &nbsp;&nbsp;&nbsp;
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('planilha');" style="cursor: pointer;"/>
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