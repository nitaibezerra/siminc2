<?php

	/* Componente Historico de consulta.
	 * Salva os POST da Consulta efetuado
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if($_REQUEST['requisicao'] == 'salvarConsulta'){
		$existe_rel = 0;
	
		$sql = "select prtid from public.parametros_tela where prtdsc = '".$_POST['titulo_consulta']."' and usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";
	
		$existe_rel = $db->pegaUm( $sql );
	
		if ($existe_rel > 0){
			$sql = sprintf(
					"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d, prtobservacao = '%s' WHERE prtid = %d",
					$_POST['titulo_consulta'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_POST['obs_consulta'],
					$existe_rel
			);
			$db->executar( $sql );
			$db->commit();
		}else{
			$sql = sprintf(
					"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid, prtobservacao) VALUES ( '%s', '%s', %s, '%s', %d, '%s' )",
					$_POST['titulo_consulta'],
					addslashes( addslashes( serialize( $_REQUEST ) ) ),
					'FALSE',
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_POST['obs_consulta']
			);
			$db->executar( $sql );
			$db->commit();
		}
		$db->sucesso('relatorio/relatorio_vistoria');
		die;
	}
	
	/* Componente Historico de consulta.
	 * Transforma consulta em pública.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
		$sql = sprintf(
				"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
				$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		$db->sucesso('relatorio/relatorio_vistoria');
		die;
	}
	
	/* Componente Historico de consulta.
	 * Exclui consulta.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
		$sql = sprintf(
				"DELETE from public.parametros_tela WHERE prtid = %d",
				$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		$db->sucesso('relatorio/relatorio_vistoria');
		die;
	}
	
	/* Componente Historico de consulta.
	 * Carrega a consulta salva no historico, consulta dados em banco.
	 * Código implementado em 17/08/2012.
	 * Luciano F. Ribeiro.
	 */
	if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
	
		$sql   = sprintf("select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid']);
	
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes($itens) ) );
	
		extract($dados);
		$_REQUEST = $dados;
		$_POST = $dados;
	}
	//ver($_REQUEST, d);
	if ($_POST['tipo_relatorio']){
		ini_set("memory_limit","256M");
		include("resultado_relatorio_vistoria.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	include APPRAIZ . 'includes/Agrupador.php';

	echo "<br>";
	$db->cria_aba($abacod_tela, $url, '');
	$titulo_modulo = "RELATÓRIO DE VISTORIA (PLANAJEDO X EXECUTADO)";
	monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript">
	//jQuery.noConflict();

	function gerarRelatorio(tipo){
		var formulario = document.filtro;
		var tipo_relatorio = tipo;

		if (formulario.elements['colunasDest'][0] == null){
			alert('Selecione pelo menos uma Coluna!');
			return false;
		}	
		selectAllOptions( colunasDest );
		selectAllOptions( formulario.estuf );
		selectAllOptions( formulario.municipio );

		if(tipo_relatorio == 'visual'){
			document.getElementById('tipo_relatorio').value = 'visual';
		}else{
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		var janela = window.open( '', 'relatorio', 'width=960,height=600,status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		
		janela.focus();
	}	
	
	function tornar_publico( prtid ){
		document.filtro.publico.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function excluir_relatorio( prtid ){
		document.filtro.excluir.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_consulta( prtid ){
		document.filtro.carregar.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.getElementById('carregar').value = '1';
		document.getElementById('prtid').value = prtid;
		document.getElementById('filtro').action = 'obras.php?modulo=relatorio/relatorio_vistoria&acao=A';
		document.getElementById('filtro').target = '_blank';
		document.getElementById('filtro').submit();
	}
	
	function salvarConsultaEfetuada(){
		var formulario = document.filtro;
		
		if ( document.getElementById('titulo_consulta').value == '' ) {
			alert( 'É necessário informar a descrição do relatório!' );
			document.getElementById('titulo_consulta').focus();
			return;
		}

		jQuery("#div_selecao").css("visibility", "visible");

		var nomesExistentes = new Array();
		<?php
			$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela Where usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";
			$nomesExistentes = $db->carregar( $sqlNomesConsulta );
			if($nomesExistentes){
				foreach($nomesExistentes as $linhaNome){
					print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
				}
			}
		?>
		var confirma = true;
		var i, j = nomesExistentes.length;
		for(i = 0; i < j; i++){
			if(nomesExistentes[i] == document.getElementById('titulo_consulta').value){
				confirma = confirm( 'Deseja alterar a consulta já existente?');
				break;
			}
		}
		if(!confirma){
			return;
		}
				
		selectAllOptions( colunasDest );
		selectAllOptions( formulario.estuf );
		selectAllOptions( formulario.municipio );
	}		


	function escolhaRelatorio(visual){

		if(visual == 'html'){
			document.getElementById('tipo_relatorio').value = 'visual';
		}else{
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		jQuery("#div_selecao").css("visibility", "hidden");
    	document.getElementById('filtro').action = 'obras.php?modulo=relatorio/relatorio_vistoria&acao=A&requisicao=salvarConsulta';
		document.getElementById('filtro').target = '_self';
		document.getElementById('filtro').submit();
	}		
	
	/* Alterar visibilidade de um bloco.
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco ){
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/* Alterar visibilidade de um campo. 
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

<style type="text/css">
  	#div_selecao {
  		visibility: hidden;
    	border-style: solid;
    	border-width: 1px;
    	borborder-color: black;
    	background-color: #DCDCDC;
    	width: 23%;
    	height: 14%;
    	position: absolute;
    	top: 58%;
    	left: 35%; 
	}
</style>  			
		
<form action="" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>

	<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<input type="hidden" id="exibir" name="exibir" value="">	
	
	
	<!-- HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco('minhasconsultas');" >  
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />					
				</td>
			</tr>
		</table>
		
		<div name="minhasconsultas_div_filtros_off" id="minhasconsultas_div_filtros_off"></div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
							$sql = sprintf("
									Select Case 
											When prtpublico = false 
												Then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
												Else '<img border=\"0\" src=\"../imagens/grupo_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
											End as acao, 
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela 
									Where mnuid = %d AND usucpf = '%s'",
									$_SESSION['mnuid'],
									$_SESSION['usucpf']
							);
							$cabecalho = array('Ação', 'Nome', 'Observações');	
						?>
					<td>
						<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '95%', null ); ?>
					</td>
				</tr>
			</table>
		</div>		
		<!-- FIM - HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->
		
		
		<!-- CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco('outros');">
					<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
					Relatórios Gerenciais
					<input type="hidden" id="outros_flag" name="outros_flag" value="0" />	
				</td>
			</tr>
		</table>

		<div id="outros_div_filtros_off"></div>
		<div id="outros_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<td>
					<?php
						$sql = sprintf("
									Select	Case
											  When prtpublico = true and usucpf = '%s' 
												Then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
												Else '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'  
											End as acao, 
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela 
									Where mnuid = %d and prtpublico = TRUE
								",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']);
						
						$cabecalho = array('Ação', 'Nome', 'Observações');
						$db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null );
					?>
					</td>
				</tr>
			</table>
		</div>	
		<!-- FIM DA CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="SubTituloDireita">Definir título da consulta</td>
				<td>
					<?= campo_texto( 'titulo_consulta', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo_consulta"'); ?>
				</td>
			</tr>
		<tr>
			<td class="SubTituloDireita">Colunas</td>
			<td>
				<?php
					#Montar e exibe colunas
					$arrayColunas = mostra_colunas();
					$agrupador = new Agrupador('filtro');
					$agrupador->setOrigem( 'colunasOrig', null, $arrayColunas );
					$agrupador->setDestino( 'colunasDest', null );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Ensino:</td>
			<td>
				<input type="radio" id="orgid" name="orgid" value="S" /> Educação Superior
				<input type="radio" id="orgid" name="orgid" value="P"/> Educação Profissional
				<input type="radio" id="orgid" name="orgid" value="B"/> Educação Básica
				<input type="radio" id="orgid" name="orgid" value="T" checked="checked"/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período de Inserção da Obra:</td>
			<td>
				<?php 
				$_REQUEST['htddata_inicio'] = $_REQUEST['htddata_inicio'] ? $_REQUEST['htddata_inicio'] : '01/'.(date('m')-1).'/'.date('Y');
				$_REQUEST['htddata_fim']    = $_REQUEST['htddata_fim'] ? $_REQUEST['htddata_fim'] : '01/'.date('m').'/'.date('Y');
				$htddata_inicio = explode('/', $_REQUEST['htddata_inicio']);
				$htddata_inicio = $htddata_inicio[1].'/'.$htddata_inicio[0].'/'.$htddata_inicio[2];
				$htddata_fim = explode('/', $_REQUEST['htddata_fim']);
				$htddata_fim = $htddata_fim[1].'/'.$htddata_fim[0].'/'.$htddata_fim[2];
				?>
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>	
		<?php
			#UF
			$stSql = "
				SELECT	estuf AS codigo,
						estdescricao AS descricao
				FROM territorios.estado
				ORDER BY estdescricao 
			";
			$stSqlCarregados = "";			
			mostrarComboPopup( 'UF', 'estuf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)');
			
			#Município				
			mostrarComboPopupMunicipios('municipio', 'Municípios', 'S', 'true', 'true', '400px', 'true');
		?>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Situação do Parecer:</td>
			<td>
				<?php					
					$arSituacaoParecer = array(
									array('codigo' => 'A', 'descricao' => 'Aprovada'),
									array('codigo' => 'N', 'descricao' => 'Não Aprovada')
								);
				
					$db->monta_combo( "situacao_parecer", $arSituacaoParecer, "S", "Todas", "", "", "", "", "N", "situacao_parecer" );					
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="gerarRelatorio('visual');"/>
				<input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="gerarRelatorio('xls');"/>
				
				<input type="button" value="Salvar Consulta" onclick="salvarConsultaEfetuada();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	<div id="div_selecao" name="div_selecao">
		<br><b style="margin-left: 6%;">Selecione o tipo de relatório que deseja que seja gerado</b><br>
		<input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_html" onclick="document.getElementById('exibir').value='relatorio_html';escolhaRelatorio('html');">Relatório em HTML <br/>
		<input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_xls" onclick="document.getElementById('exibir').value='relatorio_xls';escolhaRelatorio('xls');">Relatório em XLS
	</div>
</form>

<?php 
	function mostra_colunas(){
		$coluna = array(
				/*array(
						'codigo'    => 'id_obra',
						'descricao' => '01. ID Obras'
				),*/
				array(
						'codigo'    => 'uf_obras',
						'descricao' => '01. UF'
				),
				array(
						'codigo'    => 'nome_obra',
						'descricao' => '02. Nome da Obra'
				),
				array(
						'codigo'    => 'data_insercao_obra',
						'descricao' => '03. Data da Inserção da Obra'
				),
				array(
						'codigo'    => 'data_emissao_os',
						'descricao' => '04. Data da emissão da OS'
				),
				array(
						'codigo'    => 'data_ultima_paracer',
						'descricao' => '05. Data do Parecer'
				),
				array(
						'codigo'    => 'situacao_parecer',
						'descricao' => '06. Situação do Parecer'
				),
				array(
						'codigo'    => 'supervisor_mec',
						'descricao' => '07. Supervisor do MEC'
				),
				array(
						'codigo'    => 'data_tramitacao',
						'descricao' => '08. Data da Tramitação'
				),
				array(
						'codigo'    => 'res_tramitacao',
						'descricao' => '09. Resp. pela tramitação'
				)
		);
		return $coluna;
	}
?>
