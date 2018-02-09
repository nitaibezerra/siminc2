<?php

unset($_SESSION['emenda']);

switch ( $_REQUEST["requisicao"] ){
	
	case 'filtrapesquisa':
		$filtros = filtroPtaAnalise( $_REQUEST );
	break;
	case 'exportar':		
		ob_clean();
		$filtros = filtroPtaAnalise( $_REQUEST );
		$arSql = listaPtaAnalise( $filtros, true );
		header('content-type: text/html; charset=ISO-8859-1');
		//$arSql = $_SESSION['emenda']['arDadosExcelAnalise'];
		$arSql = $arSql ? $arSql : array();
		if( empty($arSql) ){
			echo "<script>
					alert('Não foram encontrados registros para exportar.');
					window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
				</script>";
			exit();
		}
		if( $_SESSION['exercicio'] == '2010' ){
			$cabecalho = array("Nº PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Status", "Unidade", "Iniciativa", "Limite Autorizado");
		} else {
			$cabecalho = array("Nº PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Status", "Unidade", "Iniciativa");
		}
		$db->sql_to_excel($arSql, 'relAnalisePTA', $cabecalho);
		$exporta = "filtrapesquisa";
	break;
	
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

// Cria o título da tela
monta_titulo( 'Analisar Plano de Trabalho', $subtitulo_modulo );
?>

<script type="text/javascript">
var em_elaboracao	     		 			= <?php echo EM_ELABORACAO;?>;
var em_analise_dados     		 			= <?php echo EM_ANALISE_DE_DADOS;?>;
var em_analise_de_merito 		 			= <?php echo EM_ANALISE_DE_MERITO;?>;
var em_analise_tecnica   		 			= <?php echo EM_ANALISE_TECNICA;?>;
var em_empenho   		 		 			= <?php echo EM_EMPENHO;?>;
var em_analise_do_fnde   		 			= <?php echo EM_ANALISE_DO_FNDE;?>;
var em_geracao_minuta_convenio   			= <?php echo EM_GERACAO_DA_MINUTA_DE_CONVENIO;?>;
var em_analise_profe	   				 	= <?php echo EM_ANALISE_PROFE;?>;
var em_criacao_minuta_convenio   			= <?php echo EM_CRIACAO_DO_COVENIO;?>;
var em_assinatura_concedente     			= <?php echo EM_ASSINATURA_CONCEDENTE;?>;
var em_assinatura_convenente	 			= <?php echo EM_ASSINATURA_CONVENENTE;?>;
var em_publicacao_no_dou   		 			= <?php echo EM_PUBLICACAO_NO_DOU;?>;
var em_pre_convenio			   	 			= <?php echo EM_PRE_CONVENIO;?>;
var em_analise_juridica		   	 			= <?php echo EM_ANALISE_JURIDICA;?>;
var recurso_liberado		   				= <?php echo RECURSO_LIBERADO;?>;
var em_solicitacao_reformulacao	 			= <?php echo EM_SOLICITACAO_REFORMULACAO;?>;
var em_reformulacao			   	 			= <?php echo EM_REFORMULACAO_PROCESSO;?>;
var em_analise_tec_reformulacao	 			= <?php echo EM_ANALISE_TECNICA_REFORMULACAO;?>;
var em_empenho_reformulacao		 			= <?php echo EM_EMPENHO_REFORMULACAO;?>;
var em_geracao_ta			   	 			= <?php echo EM_GERACAO_TERMO_ADITIVO;?>;
var em_republicacao			   	 			= <?php echo EM_REPUBLICACAO;?>;
var em_ana_ref_processo		   	 			= <?php echo EM_ANALISE_PROCESSO_REF;?>;
var em_diligencia			   	 			= <?php echo EM_DILIGENCIA;?>;
var em_aceitacao_reformulacao    			= <?php echo EM_ACEITACAO_REFORMULACAO;?>;
var em_identificacao_processo_reformulacao	= <?php echo EM_IDENTIFICACAO_PROCESSO_REFORMULACAO;?>;
var em_reformulacao_processo    			= <?php echo EM_REFORMULACAO_PROCESSO;?>;
var em_processo_reformulado    				= <?php echo EM_PROCESSO_REFORMULADO;?>;
var em_assinatura_reformulacao    			= <?php echo EM_ASSINATURA_REFORMULACAO;?>;
var em_liberacao_recurso_reformulacao    	= <?php echo EM_LIBERACAO_RECURSO_REFORMULACAO;?>;
var em_publicacao_reformulacao    			= <?php echo EM_PUBLICACAO_REFORMULACAO;?>;
var em_liberacao_recurso    				= <?php echo EM_LIBERACAO_RECURSO;?>;
var em_analise_juridica_reformulacao		= <?php echo EM_ANALISE_JURIDICA_REFORMULACAO;?>;
var em_convenio_cancelado					= <?php echo CONVENIO_CANCELADO;?>;
var em_convenio_vigencia_expirada			= <?php echo CONVENIO_EM_VIGENCIA_EXPIRADA;?>;
var em_pta_indeferido						= <?php echo EM_PTA_INDEFERIDO;?>;
var em_prestacao_conta						= <?php echo EM_PRESTACAO_CONTAS;?>;
var em_orcamento_impositivo					= <?php echo EM_ORCAMENTO_IMPOSITIVO;?>;

var em_elaboracao_impositivo				= <?php echo EM_ELABORACAO_IMPOSITIVO;?>;
var em_identificacao_processo_impositivo	= <?php echo EM_IDENTIFICACAO_PROCESSO_IMPOSITIVO;?>;
var vinculacao_unidades_gestoras_impositivo	= <?php echo VINCULACAO_UNIDADES_GESTORAS_IMPOSITIVO;?>;
var em_analise_de_merito_impositivo			= <?php echo EM_ANALISE_DE_MERITO_IMPOSITIVO;?>;
var em_analise_tecnica_impositivo			= <?php echo EM_ANALISE_TECNICA_IMPOSITIVO;?>;
var em_diligencia_impositivo				= <?php echo EM_DILIGENCIA_IMPOSITIVO;?>;
var em_empenho_impositivo					= <?php echo EM_EMPENHO_IMPOSITIVO;?>;
var em_proposta_siconv						= <?php echo EM_PROPOSTA_SICONV;?>;

</script>

<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="filtrapesquisa">
	
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width="190px" class="subtitulodireita">Número Emenda:</td>
			<td width="290px" >
				<?php
					$emecod = $_REQUEST['emecod'];
					echo campo_texto( 'emecod', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '','', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
			<td class="subtitulodireita">Autor:</td>
			<td colspan="3">
				<?php
					$autid = $_REQUEST['autid'];
					
					$sql ="SELECT 
							  	autid as codigo,
							  	autnome as descricao
							FROM 
							  	emenda.autor
							WHERE 
							  	autstatus = 'A'
							ORDER BY
								autnome";
					$db->monta_combo( "autid", $sql, 'S', 'Selecione...', "", '', '', '265','','autid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Responsável:</td>
			<td>
				<?php
					$resid = $_REQUEST['resid'];
					echo comboResponsavel($_SESSION['usucpf'], $resid, 'verificaResponsavel(this)');
				?>
			</td>
			<td class="subtitulodireita">Unidades:</td>
			<td>
				<?php
					$uniid = $_REQUEST['uniid'];
					$sql = "SELECT
								uniid AS codigo,
								unisigla AS descricao
							FROM
								emenda.unidades
							WHERE unistatus = 'A'
							order by descricao";
					$db->monta_combo( "uniid", $sql, 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
		<tr>
		<tr>	
			<td class="subtitulodireita">Número do PTA:</td>
			<td>
				<?php
					$ptrid = $_REQUEST['ptrid']; 
					echo campo_texto( 'ptrid', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '', '', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
			<td width="190px" class="subtitulodireita">Situação:</td>
			<td>
				<?php
					$esdid = $_REQUEST['esdid'];
					
					$sql = "(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 8 
							      AND esdstatus = 'A'
							      AND esdid in (52,53,54,55,56,57,58,60,67,68,69,119,70,120,167,196,197,199)
							  ORDER BY 
							      esdordem) 
							UNION ALL
							  SELECT  0 as codigo, 
							      '------------ Fluxo de Emendas Impositivo ------------' as descricao
							UNION ALL
							(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 163 
							      AND esdstatus = 'A'
							  ORDER BY 
							      esdordem)       
							UNION ALL
							  SELECT  0 as codigo, 
							      '------------ SITUAÇÕES DA REFORMULAÇÃO ------------' as descricao
							UNION ALL
							(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 8 
							      AND esdstatus = 'A'
							      AND esdid NOT in (52,53,54,55,56,57,58,60,67,68,69,119,70,120,167,196,197,199)
							  ORDER BY 
							      esdordem)";
					$db->monta_combo("esdid", $sql, 'S', 'Selecione...', "", '', '', '300','','esdid' );
					
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Número do Convênio FNDE:</td>
			<td>
				<?php
					$ptrnumconvenio = $_REQUEST['ptrnumconvenio']; 
					echo campo_texto( 'ptrnumconvenio', 'N', 'S', '', 27, 50, '', '', '', '', 0 );
				?>
			</td>
			<td class="subtitulodireita">Número do Processo:</td>
			<td>
				<?php
					$ptrnumprocessoempenho = $_REQUEST['ptrnumprocessoempenho']; 
					echo campo_texto( 'ptrnumprocessoempenho', 'N', 'S', '', 52, 50, '#####.######/####-##', '', '', '', 0, '', '', '', "this.value=mascaraglobal('#####.######/####-##',this.value);" );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
				<?php
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>
			<td class="subtitulodireita" width="190px">Nome do Órgão ou Entidade:</td>
			<td>
				<?php
					$enbnome = $_REQUEST['enbnome']; 
					echo campo_texto( 'enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">UF:</td>
			<td>
				<?php
					$estuf = $_REQUEST['estuf'];
					$sql = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM
								territorios.estado
							order by descricao";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
			<td class="subtitulodireita">Município:</td>
			<td>
				<?php
					$mundescricao = $_REQUEST['mundescricao']; 
					echo campo_texto( 'mundescricao', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Iniciativa:</td>
			<td colspan="3">
				<?php
					$iniid = $_REQUEST['iniid'];
					$sql = "SELECT DISTINCT
							    ini.iniid as codigo,
							    ini.ininome as descricao
							FROM 
							    emenda.ptiniciativa pti
							    inner join emenda.iniciativa ini
							        on ini.iniid = pti.iniid
							        --and ini.inistatus = 'A'
							    inner join emenda.planotrabalho ptr
							    	on ptr.ptrid = pti.ptrid
							    	and ptr.ptrstatus = 'A'
							    	 and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
							ORDER BY
							    descricao";
					
					$arIniciativa = $db->carregar( $sql );
					$arIniciativa = ($arIniciativa ? $arIniciativa : array() ); 
					$arDados = array();
					foreach ($arIniciativa as $chave => $v) {
						$sql = "SELECT								
									res.resdsc
								FROM 
								  emenda.responsavel res
								  inner join emenda.iniciativaresponsavel inr
								  	on res.resid = inr.resid
								    and inr.irestatus = 'A'
								WHERE
									inr.iniid = ".$v['codigo']."
								ORDER BY
									res.resdsc";
						
						$resdsc = $db->carregarColuna( $sql );
							
						array_push($arDados, array("codigo" => $v['codigo'],
												 "descricao" => $v['codigo'] .' - '. $v['descricao']."(".implode( ', ', $resdsc ).")"
											));
					}
					$db->monta_combo( "iniid", $arDados, 'S', 'Selecione...', '', '', '', '400', '', '', '', '', 'Lista de iniciativa(s) vinculada(s) ao plano de trabalho' );
				?>
			</td>
		</tr>
		<?php 
				$sql = "SELECT DISTINCT
						    e.espid as codigo,
						    e.espid|| ' - ' ||e.espnome as descricao
						FROM
						    emenda.especificacao e
						    inner join emenda.iniciativaespecificacao ie on ie.espid = e.espid
						    inner join emenda.ptiniciativaespecificacao ptie on ptie.iceid = ie.iceid
						    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid
						    inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid
						    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
						WHERE 
						    --e.espstatus = 'A' and
						    ptie.ptestatus <> 'P'
						    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
						ORDER BY
							e.espid";

				if ( !empty($_REQUEST['espid'][0]) && is_array($_REQUEST['espid']) ){
					$sqlCarregados = "SELECT DISTINCT
										    e.espid as codigo,
										    e.espid|| ' - ' ||e.espnome as descricao
										FROM
										    emenda.especificacao e
										    inner join emenda.iniciativaespecificacao ie on ie.espid = e.espid
										    inner join emenda.ptiniciativaespecificacao ptie on ptie.iceid = ie.iceid
										    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid
										    inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid
										    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
										WHERE 
										    --e.espstatus = 'A' and
										    ptie.ptestatus <> 'P'
										    and e.espid IN (" . ( implode("," , $_REQUEST['espid']) ) . ")
										    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
										ORDER BY
											e.espid";
					$espid = $db->carregar( $sqlCarregados );
				}
				
				mostrarComboPopup( 'Especificações', 'espid',  $sql, $stSqlCarregados, 'Selecione a(s) Especificações(s)' );
		?>
		<tr>
			<td class="subtitulodireita">Limite Autorizado:</td>
			<td colspan="1">
				<input type="radio" value="S" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="liberadocasacivil" name="liberadocasacivil" <? if($_REQUEST["liberadocasacivil"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita"><b>Empenhado:</b></td>
			<td>
				<input type="radio" value="S" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="empenhado" name="empenhado"  <? if($_REQUEST["empenhado"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td colspan="3">
				<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaPTAAnalise();" style="cursor: pointer;">&nbsp;
				<input type="button" value="Exportar para Excel" name="btnExportar" id="btnExportar" onclick="exportarAnaliseExcel();" style="cursor: pointer;">&nbsp;
				<input type="button" value="Enviar E-mail" name="btnEnviarEmail" id="btnEnviarEmail" onclick="enviarEmailAnaliseEntidade();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
</form>
<script language="javascript" type="text/javascript" src="js/listaPtaAnalise.js"></script>
<?php listaPtaAnalise( $filtros, false ); ?>
