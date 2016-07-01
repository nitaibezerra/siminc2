<?php 

// Salva relatório
if ( $_REQUEST['salvar'] == 1 ){
	
	if ($db->pegaUm( "select prtid from public.parametros_tela where prtdsc = '{$_REQUEST['titulo']}'" )){
			
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
		
	}else{
		
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
	$db->sucesso('relatorio/relGeral');
}

// Transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('relatorio/relGeral');
}

// Remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
	
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('relatorio/relGeral');
}

// Exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){
	
	if ( $_REQUEST['prtid'] ){
		
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		
		$itens = $db->pegaUm( $sql );
		
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		
		$_REQUEST = $dados;
		
		unset( $_REQUEST['salvar'] );
	}	
	
	include "relPlanoTrabalhoResult.inc";
	die;	
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Relatório de Plano de Trabalho";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
<!--

	function exibeRelatorio( tipo ){
		
		var formulario = document.formulario;
		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='1';
	
		prepara_formulario();
	
		if ( tipo == 'salvar' ){

			selectAllOptions( agrupador );	
			
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
			formulario.action = 'maismedicos.php?modulo=relatorio/relPlanoTrabalho&acao=A&salvar=1';
			formulario.submit();
				
		}else if ( tipo == 'relatorio' ){
			
			formulario.action = 'maismedicos.php?modulo=relatorio/relPlanoTrabalho&acao=A';
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
			
			formulario.target = 'resultadoGeral';
			var janela = window.open( '?modulo=relatorio/relPlanoTrabalho&acao=A', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			
			formulario.submit();
			janela.focus();		
		}		
	}

	function gerarRelatorioXLS(){
		document.getElementById('req').value = 'geraxls';
		exibeRelatorio();
	}

	function exibeRelatorioNormal()
	{
		document.getElementById('req').value = '';
		exibeRelatorio();
	}

	function tornar_publico( prtid ){
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
		exibeRelatorio( 'relatorio' );
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

		<!-- AGRUPADORES -->
	
		<tr>
			<td width="195" class="SubTituloDireita">Agrupadores:</td>
			<td>
				<?php 
				
				// Início dos agrupadores
				$agrupador = new Agrupador('filtro','');
				
				// Dados padrão de origem
				$origem = array(
					'ptrinstituicao' => array(
						'codigo'    => 'ptrinstituicao',
						'descricao' => 'Instituição'
					),
					'ptrano' => array(
						'codigo'    => 'ptrano',
						'descricao' => 'Ano'
					),
					'ptrmes' => array(
						'codigo'    => 'ptrmes',
						'descricao' => 'Mês'
					)
				);
				
				$destino = array(
							'ptrinstituicao' => array(
								'codigo'    => 'ptrinstituicao',
								'descricao' => 'Instituição'
								)
							);
											
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
				
				?>
			</td>
		</tr>
	
		<!-- FIM AGRUPADORES -->
		
	</table>
	
	
	
	<!-- OUTROS FILTROS -->
	<!--  
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
									WHERE prtpublico = TRUE",
							$_SESSION['usucpf'],
							$_SESSION['usucpf']
						);
						
						$cabecalho = array('Ação', 'Nome');
						
					?>
					<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
					</td>
				</tr>
		</table>
	</div>
	-->
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
							 	usucpf = '%s'",
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
	
	
	<!-- FILTROS -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	
		<tr>
			<td class="subtitulodireita">Situação</td>
			<td>
				<?php
				$sql = "select distinct
							situacao as codigo,
							situacao as descricao
						from maismedicos.ws_planotrabalho ptr
						join maismedicos.ws_planotrabalho_itens pti on ptr.ptrid = pti.ptrid";
				 
				$db->monta_combo('situacao', $sql, 'S', 'Selecione...', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ano</td>
			<td>
				<?php
				$ano=2014;
				for($x=0;$x<=10;$x++){
					$arAno[$x]['codigo'] = $ano+$x;
					$arAno[$x]['descricao'] = $ano+$x;
				}
				$db->monta_combo('ptrano', $arAno, 'S', 'Selecione...', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Mês</td>
			<td>
				<?php
				 
				$mes=1;
				for($x=0;$x<=11;$x++){
					$arMes[$x]['codigo'] = $mes+$x;
					$arMes[$x]['descricao'] = mes_extenso($mes+$x);
				}
				$db->monta_combo('ptrmes', $arMes, 'S', 'Selecione...', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">UF</td>
			<td>
				<?php
				$sql = "select 
							regcod as codigo,
							descricaouf as descricao
						from uf order by uf";
				$db->monta_combo('uf', $sql, 'S', 'Selecione...', '', '', '');
				?>
			</td>
		</tr>
		
		<?php
		
		mostrarComboPopupMunicipios();
		
		$arrVisivel = array("descricao");
		$arrOrdem = array("descricao");
		
		// Diretorias
		$stSql = "select 
					ptrinstituicao as codigo,
					ptrinstituicao as descricao
				from maismedicos.ws_planotrabalho  
				order by ptrinstituicao";

		$stSqlCarregados = "";
		mostrarComboPopup( 'Instituições:', 'ptrinstituicao',  $stSql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		?>		
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioNormal();" style="cursor: pointer;"/>
				<input type="button" value="VisualizarXLS" onclick="gerarRelatorioXLS();">
				<input type="button" value="Salvar Consulta" onclick="exibeRelatorio('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	
	<!-- FIM FILTROS -->
		
</form>