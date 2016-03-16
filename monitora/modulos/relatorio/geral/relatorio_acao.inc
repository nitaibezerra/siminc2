<?php
	@session_start();
	set_time_limit( 0 );
	
	define("PRIMEIRO", 262);
	define("SEGUNDO", 263);
	
	function geraListaDeUnidadesComPreenchimento( $intAnoExercicio, $arrMesesSelecionados = array() , 
		$arrSituacoes , $arrCodigoUnidade , $intPagina , $intTamanhoPagina = 20 )
	{
		global $db;
		
		if( $arrCodigoUnidade != null )
		{
			$arrWhere[] = "a.unicod = '$arrCodigoUnidade'";
		}
		
		if( $arrSituacoes != null ){
			$arrWhere[] = $arrSituacoes;
		}
		
		$ref = '';
		if( $_POST['esdid'] == 'nulo' ){
			$arrWhere[] = " esd.esdid IS NULL";
		}else{
			if( $_POST['esdid'] ){
				$trav = ' AND 1=0';
			}else{
				$trav = '';
			}
			if($_POST['periodo'] == "Segundo"){
				$arrWhere[] = "r.refcod = ".REFERENCIA_SEGUNDO_2012;
			}else{
				$arrWhere[] = "r.refcod = ".REFERENCIA_PRIMEIRO_2012;
			}
		}
		
		--$intPagina;
		
		$strSql = "SELECT
						a.unicod as unidade_id,
						unidade.unidsc as unidade_descricao,
						count(distinct a.acaid) as qtde,
						esd.esdid as situacao_cod,
						esd.esddsc as situacao
					FROM 
						monitora.acao a
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					LEFT  JOIN monitora.avaliacaoparecer avp ON avp.acaid = a.acaid
					LEFT  JOIN monitora.referencia r ON r.refano_ref = a.prgano and r.refcod = avp.refcod and r.refano_ref = '{$_SESSION['exercicio']}'
					LEFT  JOIN workflow.documento wok ON wok.docid = avp.docid
					LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = wok.esdid
					WHERE 
						esd.esdid IN ( 
									SELECT esdid 
									FROM workflow.estadodocumento 
									WHERE 
										esdstatus = 'A' 
										and tpdid = 94 
									ORDER BY esdordem  ) and (a.prodsc is not null or a.prodsc <> '') 
						".($arrWhere ? " AND ".implode(" AND ",$arrWhere) : "")."
					GROUP BY unidade_id,unidade_descricao,situacao_cod,situacao
					UNION ALL
					SELECT
						a.unicod as unidade_id,
						unidade.unidsc as unidade_descricao,
						count(distinct a.acaid) as qtde,
						11 situacao_cod,
						'' as situacao
					FROM 
						monitora.acao a
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					WHERE 
						a.prgano = '{$_SESSION['exercicio']}' and (a.prodsc is not null or a.prodsc <> '') 
						$trav
						AND a.acaid NOT IN (SELECT acaid 
											FROM  monitora.avaliacaoparecer avp
											INNER JOIN monitora.referencia r ON r.refano_ref = '{$_SESSION['exercicio']}' AND r.refcod = avp.refcod )
					GROUP BY unidade_id,unidade_descricao,situacao_cod,situacao
					ORDER BY unidade_id,unidade_descricao";
// 		ver($strSql,d);
		/*
		$strSql = "SELECT
						a.unicod as unidade_id,
						unidade.unidsc as unidade_descricao,
						count(distinct a.acaid) as qtde,
						esd.esdid,
						esd.esddsc as situacao
					FROM monitora.acao a
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					LEFT  JOIN monitora.avaliacaoparecer avp ON avp.acaid = a.acaid
					LEFT  JOIN monitora.referencia r ON r.refano_ref = a.prgano and r.refcod = avp.refcod $ref
					LEFT  JOIN workflow.documento wok ON wok.docid = avp.docid
					LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = wok.esdid
					WHERE 1=1
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
					GROUP BY a.unicod,unidade.unidsc,esd.esdid,esd.esddsc
					ORDER BY a.unicod,unidade.unidsc";
// 		ver($strSql);
		/*
		$strSql = sprintf
		(			
			"	
			SELECT
				unidade.unicod							AS unidade_id ,
				unidade.unidsc							AS unidade_descricao ,
				referencia.refmes_ref					AS mes,
				preencha.acatotal						AS total_de_acoes ,
				preencha.total_em_cadastramento			AS em_cadastramento,
				preencha.total_em_validacao				AS em_validacao,
				preencha.total_finalizada				AS finalizada
			FROM
				monitora.preencheacamensalsituacao	AS preencha
			INNER JOIN
				monitora.referencia			AS referencia 
			ON
				referencia.refcod = preencha.refcod 
			AND
				referencia.refano_ref = '%s'
			AND
				referencia.refmes_ref BETWEEN '%s' AND '%s'
			INNER JOIN
				public.unidade unidade
			USING( unicod )
			WHERE
				preencha.refano_ref = '%s'
				and unicod = '26101'
				%s
				%s
			"
			,
			$intAnoExercicio, 
			$intMesInicial, 
			$intMesFinal, 
			$intAnoExercicio,
			$strSqlWhereUnicod,
			$strSqlSituacoes
			);*/
		//dbg($strSql);
		$arrListaUnidades = $db->carregar( $strSql );
		$arrListaUnidades = $arrListaUnidades ? $arrListaUnidades : Array();
		$arrResultado = array();
		foreach( $arrListaUnidades as $arrUnidade )
		{
			$intUnidadeId	= $arrUnidade[ 'unidade_id' ];
			$strDescricao	= $arrUnidade[ 'unidade_descricao' ];
			$intQuantidade	= $arrUnidade[ 'qtde' ];
			$situacao		= $arrUnidade[ 'situacao' ];
			$arrResultado[ $intUnidadeId ][ 'nome' ] = $strDescricao;
			$arrResultado[ $intUnidadeId ][ 'esdid' ] = $_POST['esdid'];
			$arrResultado[ $intUnidadeId ][ 'unidade_descricao' ] = $arrUnidade[ 'unidade_descricao' ];
			$arrResultado[ $intUnidadeId ][ 'situacao' ][ $situacao ] = $intQuantidade;
			$arrResultado[ $intUnidadeId ][ 'quantidade' ] = array_sum($arrResultado[ $intUnidadeId ][ 'situacao' ]);
		}
		return $arrResultado;
	}
	
	function geraListaDePreenchimento( $intAnoExercicio,  $arrMesesSelecionados = array() , $arrSituacoes , 
		$arrCodigoUnidade )
	{
		global $db;
		
		if( $arrCodigoUnidade !== null )
		{
			$arrWhere[] = sprintf( "a.unicod in ('%s') ", implode( "','" , (array) $arrCodigoUnidade ) );
			$arrWhere2[] = sprintf( "a.unicod in ('%s') ", implode( "','" , (array) $arrCodigoUnidade ) );
		}
		
		if( $arrSituacoes != null ){
			$arrWhere[] = $arrSituacoes;
		}
		
		$ref = '';
		if( $_REQUEST['esdid'] == 'nulo' ){
			$arrWhere[] = " esd.esdid IS NULL";
		}else{
			if( $_REQUEST['esdid'] ){
				$trav = ' AND 1=0';
			}else{
				$trav = '';
			}
			if($_REQUEST['periodo'] == "Segundo"){
				$arrWhere[] = "r.refcod = ".REFERENCIA_SEGUNDO_2012;
			}else{
				$arrWhere[] = "r.refcod = ".REFERENCIA_PRIMEIRO_2012;
			}
		}
		
		$strSql = "	SELECT DISTINCT
						a.prgcod || '.' || a.acacod  || '.' || '.' || a.loccod AS codigo ,
						a.unicod AS unidade_id , 
						a.acadsc as acao_descricao,
						esd.esddsc
					FROM
						monitora.acao a
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					INNER JOIN monitora.avaliacaoparecer avp ON avp.acaid = a.acaid
					INNER JOIN monitora.referencia r ON r.refano_ref = a.prgano and r.refcod = avp.refcod and r.refano_ref = '{$_SESSION['exercicio']}'
					INNER JOIN workflow.documento wok ON wok.docid = avp.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = wok.esdid
					WHERE
						esd.esdid IN (
									SELECT esdid
									FROM workflow.estadodocumento
									WHERE
										esdstatus = 'A'
										and tpdid = 94
									ORDER BY esdordem  )
						".($arrWhere ? " AND ".implode(" AND ",$arrWhere) : "")." and (a.prodsc is not null or a.prodsc <> '') 
					UNION ALL
					SELECT DISTINCT
						a.prgcod || '.' || a.acacod  || '.' || '.' || a.loccod AS codigo ,
						a.unicod AS unidade_id , 
						a.acadsc as acao_descricao,
						'' as esddsc
					FROM
						monitora.acao a
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					WHERE
						a.prgano = '{$_SESSION['exercicio']}' and (a.prodsc is not null or a.prodsc <> '') 
						$trav
						AND a.acaid NOT IN (SELECT acaid
											FROM  monitora.avaliacaoparecer avp
											INNER JOIN monitora.referencia r ON r.refano_ref = '{$_SESSION['exercicio']}' AND r.refcod = avp.refcod )
						".($arrWhere2 ? " AND ".implode(" AND ",$arrWhere2) : "")."
					";
// 		ver($strSql,d);
		/*
		$strSql = " SELECT DISTINCT
						a.prgcod || '.' || a.acacod  || '.' || '.' || a.loccod AS codigo ,
						a.unicod AS unidade_id , 
						a.acadsc as acao_descricao,
						esd.esddsc
					FROM monitora.acao a
					INNER JOIN monitora.avaliacaoparecer avp ON avp.acaid = a.acaid
					INNER JOIN  monitora.referencia r ON r.refcod = avp.refcod
					INNER JOIN public.unidade unidade on unidade.unicod = a.unicod
					INNER JOIN workflow.documento wok ON wok.docid = avp.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = wok.esdid
					WHERE 1=1
					and r.refano_ref = '{$_SESSION['exercicio']}'
					".($arrWhere ? "and ".implode(" and ",$arrWhere) : "")."";
		/*
		$strSql = sprintf(
			"
			SELECT distinct
				acao.prgcod || '.' || acao.acacod  || '.' 
				|| '.' || acao.loccod			AS funprog ,
				acao.unicod										AS unidade_id , 
				acao.acadsc										AS acao_descricao ,
				preencha_acao.situacao							AS situacao ,
				referencia.refmes_ref							AS mes
			FROM 
				monitora.preencheacamensal_aca preencha_acao
			INNER JOIN
				monitora.referencia AS referencia 
			ON
				referencia.refcod = preencha_acao.refcod 
			AND
				referencia.refano_ref = '%s'
			AND
				referencia.refmes_ref BETWEEN '%s' AND '%s'
			INNER JOIN
				monitora.acao acao
			INNER JOIN
				monitora.avaliacaoparecer ap ON ap.acaid = acao.acaid
			LEFT JOIN
				workflow.documento doc ON doc.docid = ap.docid
			ON
				preencha_acao.acaid = acao.acaid
				%s
			WHERE 
				preencha_acao.refano_ref = '%s'
				%s
			" ,
			$intAnoExercicio ,
			$intMesInicial, 
			$intMesFinal, 
			$strSqlWhereUnicod,
			$intAnoExercicio,
			$strSqlSituacoes
			);*/
		//dbg($strSql);
		$arrResultadoQuery = $db->carregar( $strSql );
		$arrResultadoQuery = $arrResultadoQuery ? $arrResultadoQuery : array();
		
		$arrResultado = array();
		foreach( $arrResultadoQuery as $n => $arrUnidade )
		{
			$strFuncProg	= $arrUnidade[ 'codigo' ];
			$strDescricao	= $arrUnidade[ 'acao_descricao' ];
			$arrResultado[$n][ 'codigo' ]		= $strFuncProg;
			$arrResultado[$n][ 'nome' ]		= $strDescricao;
			$arrResultado[$n][ 'situacao' ]	= $arrUnidade[ 'esddsc' ];;
		}
		return $arrResultado;
	}
	
	function geraArrayMesesReferencia( $intAnoExercicio , $intMesSelecionado )
	{
		global $db;
		$strSql = sprintf
		(
			" SELECT
				refmes_ref, 
				refdsc 
			FROM
				monitora.referencia 
			WHERE
				refsngrupo = 'f' 
			AND
				refano_ref = '%s' 
			ORDER BY
				refmes_ref
			",
			$intAnoExercicio
		);
		
		$arrListaMeses = $db->carregar( $strSql );
		$arrOptions = array();
		
		foreach( $arrListaMeses as $arrMes )
		{
			$arrOption = array();
			if ( $arrMes[ 'refmes_ref' ] == $intMesSelecionado )
			{
				$arrOption[ 'selected' ] = 'selected = "selected"';
			}
			else
			{
				$arrOption[ 'selected' ] = '';
			}
			
			$arrOption[ 'value' ] = 'value = "' . $arrMes[ 'refmes_ref' ] . '"';
			$arrOption[ 'innerHTML' ] = $arrMes[ 'refdsc' ];
			$arrOptions[] = $arrOption;
		}
		return $arrOptions;
	}
	
	function geraArrayPeriodo( $intAnoExercicio , $intMesSelecionado )
	{
		global $db;
		$strSql = sprintf
		(
			" SELECT 
						refcod AS codigo,
						refperiodo AS descricao 
					FROM 
						monitora.referencia 
					WHERE 
						refdata_limite_avaliacao_aca IS NOT NULL AND 
						refsnmonitoramento = 't' AND 
						refano_ref= '%s' 
						AND refperiodo IS NOT NULL
					ORDER BY refdsc
					",
			$intAnoExercicio
		);
		$arrListaMeses = $db->carregar( $strSql );
		$arrOptions = array();

		if( $intMesSelecionado == PRIMEIRO ){
			$arrNomesMeses = 'Primeiro';
		}else{
			$arrNomesMeses = 'Segundo';
		}
	
	
		foreach( $arrListaMeses as $arrMes )
		{
			$arrOption = array();
			if ( $arrMes[ 'descricao' ] == "Primeiro" )
			{
				$arrOption[ 'selected' ] = 'selected = "selected"';
			}
			else
			{
				$arrOption[ 'selected' ] = '';
			}
			
			$arrOption[ 'value' ] = 'value = "' . $arrMes[ 'codigo' ] . '"';
			$arrOption[ 'innerHTML' ] = $arrMes[ 'descricao' ];
			$arrOptions[] = $arrOption;
		}
		return $arrOptions;
	}
	
	function geraLinkBarraProgesso( $arrValores,$arrCores )
	{
		foreach($arrValores as $nome => $valor){
			$arrSituacao[] = $nome;
			$arrvalor[] = $valor;
		}
		$strSidDiretorio = $_SESSION[ 'sid_diretorio' ];
		$url = "./geral/infoMonitoraPreenchimento.php?requisicaoAjax=SuperTitleBarraProgresso&situacao=".implode(",",$arrSituacao)."&valor=".implode(",",$arrvalor)."&cor=".implode(",",$arrCores);
		return $url;
	}
	
	function geraBarraProgresso($arrValores,$esdid = null)
	{
		$total = array_sum($arrValores);
		if( !$esdid )
		{
			$arrCores = array("green","yellow","red","white");
			$strAjaxLink = geraLinkBarraProgesso( $arrValores,$arrCores );
			
			?>
				<td class="barraTd" align="center" nowrap="nowrap" >
				<?php //echo "verde:".$intVerde." - amarelo:".$intAmarelo." - branco:".$intBranco." - total:".$intTotal." - mes:".$strMes." - arrSituacao".implode(",",$arrSituacoes)." - FullSize".$intFullSize ?>
					<div onmousemove="SuperTitleAjax( '<?= $strAjaxLink ?>', this );" onmouseout="SuperTitleOff( this );" style="border:black 1px solid;height:10px" >
						<?php $n=0; foreach($arrValores as $valor): ?>
							<?php $tamanho_cor = round( ($valor/$total)*100 , 2 ); ?>
							<div style="height:10px;width:<?=$tamanho_cor?>px;float:left;margin:0px;background-color:<?php echo $arrCores[$n] ?>" ></div>
						<?php $n++;endforeach; ?>
					</div>
				</td>
			<?php
		}
		else
		{
			?>
				<td class="barraTd">
					<span>
						<font title="<?= key($arrValores) ?>" ><?= $arrValores[key($arrValores)] ?></font>
					</span>
				</td>
			<?php
		}
	}
	
	function geraHtmlDoAbreConteudo( $intAnoExercicio , $arrElementos , $arrMesesSelecionados , $arrNomesMeses )
	{
		
		?>
				<table style="width: 100%;border-spacing: 0px; border-collapse: collapse; float: right;" cellspacing="0" cellpadding="0"  >
					<colgroup span="2" />
					<colgroup span="<?= sizeof( $arrMesesSelecionados ) ?>" /> 
					<thead>
						<tr>
							<td>
								Código
							</td>
							<td >
								Nome
							</td>
							<td class="barraTd" align="center"><?php echo $_GET['periodo'] ? $_GET['periodo'] : "Primeiro" ?> Período</td>
						</tr>
					</thead>
					<tbody>
						<?php $booCor = true; ?> 
						<?foreach( $arrElementos as $arrElemento ): ?>
							<?php $strCor = $booCor ? '#FAFAFA' : ''; ?>
							<tr onmouseover="this.bgColor='#ffffdd';" onmouseout="this.bgColor='<?= $strCor ?>';" bgcolor="<?= $strCor ?>">
								<td>
									<?= $arrElemento[ 'codigo' ] ?>
								</td>
								<td>
									<?= $arrElemento[ 'nome' ] ?>
								</td>
								<td colspan="2" >
									<?= $arrElemento[ 'situacao' ] ?>
								</td>
							</tr>
							<?php $booCor = !$booCor; ?>
						<?php endforeach ?>
					</tbody>
				</table>
		<?php
	}
	
	function geraHtmlDoRelatorioDePreenchimento( $intAnoExercicio, $arrUnidadesAnalise , $arrSituacoes , $arrMesOptions  , $arrMesesSelecionados , $arrNomesMeses , $mixCodigoUnidade)
	{
		global $db;
		
		?>
			<style>
			<!--
			.debugger
			{
				border-color: red;
				border-style: solid;
				border-width: 1px;
				
			}
			.barraTd
			{
				text-align: center; 
				font-size: 10px; 
				font-family: Verdana;
				width: 50px;
			}
			.imgVerde
			{
				border-style: solid none solid solid; 
				border-color: rgb(136, 136, 136) -moz-use-text-color rgb(136, 136, 136) rgb(136, 136, 136); 
				border-width: 1px 0pt 1px 1px; 
				height: 10px; 
			}
			.imgAmarelo
			{
				border-style: solid none; 
				border-color: rgb(136, 136, 136) -moz-use-text-color; 
				border-width: 1px 0pt; 
				height: 10px; 
			}
			.imgVermelho
			{
				border-style: solid none; 
				border-color: rgb(136, 136, 136) -moz-use-text-color; 
				border-width: 1px 0pt; 
				height: 10px; 
			}
			.imgBranco
			{
				border-style: solid solid solid none; 
				border-color: rgb(136, 136, 136) rgb(136, 136, 136) rgb(136, 136, 136) 
				-moz-use-text-color; 
				border-width: 1px 1px 1px 0pt; 
				height: 10px; 
			}
			.siglaTd
			{
				color: rgb( 10, 10, 10);
			}
			-->
			</style>
			<br/>
			<?php
				$db->cria_aba( $abacod_tela, $url, '' );
				monta_titulo( 'Relatório de Ações', 'Acompanhamento de Ações por período' );
			?>
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>
			
			<form method="post" name="pformlista">
				<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
					<tr>
						<td class="SubTituloDireita">
							Período:
						</td>
						<td style="color:#000080;">
							<?php
							$arrPeriodo = array(
												0=>array("codigo"=>"Primeiro","descricao"=>"Primeiro Período"),
												1=>array("codigo"=>"Segundo","descricao"=>"Segundo Período")
												);
							$periodo = $_POST['periodo'];
							$db->monta_combo("periodo",$arrPeriodo,"S","","","","","","","","",$periodo);
							?>
						</td>
						<td class="SubTituloDireita">
							Situação:
						</td>
						<td style="color:#000080;">
							<input type="radio" name="esdid" <?php echo !$_POST['esdid'] ? "checked='checked'" : "" ?> value="" />
							Todos
							<input type="radio" name="esdid" <?php echo $_POST['esdid'] == 'nulo' ? "checked='checked'" : "" ?> value="nulo" />
							Não Inicíados
							<?php 
								$sql = "select * from workflow.estadodocumento where esdstatus = 'A' and tpdid = ".FLUXO_AVALIACAO_TPDID." order by esdordem";
								$arrSituacao = $db->carregar($sql);
								$arrSituacao = $arrSituacao ? $arrSituacao : array();
							 ?>
							 <?php foreach($arrSituacao as $situacao): ?>
								<input type="radio" name="esdid" <?php echo $_POST['esdid'] == $situacao['esdid'] ? "checked='checked'" : "" ?> value="<?php echo $situacao['esdid'] ?>" />
								<?php echo $situacao['esddsc'] ?>
							<?php endforeach; ?>
						</td>
						<td class="SubTituloDireita" style="text-align:center;">
							<input type="Button" style="width:80px;" name="pfiltrar" value="Filtrar" onclick="enviarformfiltro()">
						</td>
					</tr>
				</table>
			<table class="listagem" style="width: 95%; color: rgb(51, 51, 51);" align="center" border="0" cellpadding="2" cellspacing="0">
				<tbody>
					<tr bgcolor="#e7e7e7">
						<td align="center" width="">&nbsp;</td>
						<td><b>Unidades</b></td>
						<td><b>Qtd. Ações</b></td>
						<td style="white-space:nowrap;" ><b><?php echo $_POST['periodo'] ? $_POST['periodo'] : "Primeiro" ?> Período</b></td>
						<td style="width: 1px;"></td>
					</tr>
					<?php $intTotalQuantidade = 0; ?>
					<?php $booCor = true; ?> 
					<?php foreach( $arrUnidadesAnalise as $intUnidadeId => $arrUnidadeAnalise ): ?>
						<?php $strUnidadeComplementoId = $intUnidadeId . 'UnidadeComplemento' ?>
						<?php $strCor = $booCor ? '#F7F7F7' : ''; ?>
						<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?= $strCor ?>';" bgcolor="<?= $strCor ?>">
							<td style="padding-top: 3px">
								<img src="../imagens/mais.gif" style="padding-right: 5px;"
								id="img<?= $strUnidadeComplementoId ?>" name="+" 
								onclick="abreconteudo( 'monitora.php?modulo=relatorio/geral/relatorio_acao&acao=C&ajax=1&periodo=<?php echo $_POST['periodo'] ? $_POST['periodo'] : "Primeiro" ?>&codigounidade=<?= $intUnidadeId ?>&esdid=<?=$_POST['esdid'] ?>','<?= $strUnidadeComplementoId ?>')"
								align="absmiddle" border="0" height="9" vspace="3" width="9"/>
							</td>
							<td class="UnidadeNome">
								<font color="#003366">
									<a href="monitora.php?modulo=principal/programa/cadprograma&amp;acao=C&amp;prgid=225">
										<?= $intUnidadeId?> - <?= $arrUnidadeAnalise[ 'nome' ]; ?>
									</a>
								</font>
							</td>
							
							<td class="UnidadeTotal" align="center">
								<b>
									<?= $arrUnidadeAnalise[ 'quantidade' ]; ?>
								</b>
							</td>
								<?php geraBarraProgresso($arrUnidadeAnalise['situacao'],$_POST['esdid']);?>
							<td></td>
						</tr>
						<tr>
							<td style="padding-right: 4px; display:none;" 
							id="td<?= $strUnidadeComplementoId ?>" colspan="<?= 4 + sizeof( $arrMesesSelecionados ) ?>" ></td>
						</tr>
						<?php $intTotalQuantidade += $arrUnidadeAnalise[ 'quantidade' ]; ?>
					<?php $booCor = !$booCor; ?>
					<?php endforeach ?>
					<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';" bgcolor="" >
						<td style="padding-top: 3px" colspan="2" align="right">
							Total
						</td>
						<td class="UnidadeTotal" align="center">
							<b>
								<?= $intTotalQuantidade; ?>
							</b>
						</td>
						<td></td>
						<td></td>
					</tr>
					<tr>
						<td style="padding-right: 4px; display:none;" 
						id="td<?= $strUnidadeComplementoId ?>" colspan="<?= 4 + sizeof( $arrMesesSelecionados ) ?>" ></td>
					</tr>
				</tbody>
			</table>
				
			</form>
			<script language="Javascript">
			function enviarformfiltro() {
				var pfrm = document.pformlista;
				pfrm.submit();
			}
			</script>
		<?
	}

	$intAnoExercicio = $_SESSION['exercicio'];
	
	if( $_REQUEST['esdid'] )
	{
		if( $_REQUEST['esdid'] == 'nulo' ){
			//$arrSituacoes	= "esd.esdid IS NULL ";
		}else{
			$arrSituacoes	= "esd.esdid = {$_REQUEST['esdid']} ";
		}
	}
	else
	{
		//$arrSituacoes	= "esd.esdid in ( select esdid from workflow.estadodocumento where esdstatus = 'A' and tpdid = ".FLUXO_AVALIACAO_TPDID." order by esdordem  )";
	}
	$mixCodigoUnidade	= $_REQUEST[ 'codigounidade' ];
	$intPagina			= $_REQUEST['pagina'];
	$boolAjax			= ( $_REQUEST['ajax'] == 1 );
	
	if( $intPagina == null )
	{
		$intPagina = 1;
	}
	
	$arrMesesSelecionados = array("Primeiro","Segundo");

	if( $boolAjax )
	{
		$arrElementos = geraListaDePreenchimento( $intAnoExercicio , $arrMesesSelecionados , $arrSituacoes , $mixCodigoUnidade );
		geraHtmlDoAbreConteudo( $intAnoExercicio , $arrElementos , $arrMesesSelecionados , $arrNomesMeses );
	}
	else
	{
		include APPRAIZ . "includes/cabecalho.inc";
		$arrMesOptions	= geraArrayPeriodo(
							$intAnoExercicio ,
							$intMesSelecionado
		);
		$arrUnidadesAnalise	= geraListaDeUnidadesComPreenchimento(
								$intAnoExercicio,
								$arrMesesSelecionados,
								$arrSituacoes,
								$mixCodigoUnidade,
								$intPagina
		);
		geraHtmlDoRelatorioDePreenchimento(
			$intAnoExercicio,
			$arrUnidadesAnalise,
			$arrSituacoes,
			$arrMesOptions,
			$arrMesesSelecionados,
			$arrNomesMeses,
			$mixCodigoUnidade
		);	
	}
?>