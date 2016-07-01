<?php
	@session_start();
	set_time_limit( 0 );
	
	function gerArrMesesSelecionados( $intAnoExercicio , &$intMesSelecionado )
	{
		setlocale(LC_TIME, 'pt_BR' );
		
		$intMesLimite = ( $intAnoExercicio == date( 'Y' ) ? date( 'n' ) : 12 );
		
		$intMesSelecionado = (integer) ( $intMesSelecionado === null  ? $intMesLimite : $intMesSelecionado );
		
		$arrMeses = array();
		for( $intMes = $intMesSelecionado; $intMes > 0 && count( $arrMeses ) < 6; $intMes-- )
		{
			array_push( $arrMeses, sprintf( "%02d", $intMes ) );
		}
		$arrReturn = array_reverse( $arrMeses );
		return $arrReturn;
	}
	
	function geraListaDeUnidadesComPreenchimento( $intAnoExercicio, $arrMesesSelecionados = array() , 
		$arrSituacoes , $arrCodigoUnidade , $intPagina , $intTamanhoPagina = 20 )
	{
		global $db;
		
		if( sizeof( $arrMesesSelecionados ) > 0 )
		{
			$intMesInicial	= $arrMesesSelecionados[ 0 ];
			$intMesFinal	= $arrMesesSelecionados[ sizeof( $arrMesesSelecionados ) - 1 ];	
		}
		else
		{
			$intMesInicial	= 1;
			$intMesFinal	= 12; 
		}
		
		if( $arrCodigoUnidade != null )
		{
			$strSqlWhereUnicod = sprintf( " AND unidade.unicod in ('%s') ", implode( "','" , (array) $arrCodigoUnidade ) );
		}
		
		$strSqlSituacoes = '';
		if( sizeof( $arrSituacoes ) > 0 )
		{
			if ( !in_array( 'todos' , $arrSituacoes ) )
			{
				if( in_array( 'preenchido_liberado' , $arrSituacoes ) )
				{
					$strSqlSituacoes .= " AND preencha.total_preenchido_liberado > 0 ";
				}
				if( in_array( 'preenchido_nao_liberado' , $arrSituacoes ) )
				{
					$strSqlSituacoes .= " AND preencha.total_preenchido_nao_liberado > 0 ";
				}
				if( in_array( 'nao_preenchido' , $arrSituacoes ) )
				{
					$strSqlSituacoes .= " AND preencha.total_nao_preenchido > 0 ";
				}
			}
		}
		
		--$intPagina;
		
		$strSql = sprintf
		(			
			"	
			SELECT
				unidade.unicod							AS unidade_id ,
				unidade.unidsc							AS unidade_descricao ,
				referencia.refmes_ref					AS mes,
				preencha.acatotal						AS total_de_acoes ,
				preencha.total_preenchido_liberado		AS total_preenchido_liberado ,
				preencha.total_preenchido_nao_liberado	AS total_preenchido_nao_liberado ,
				preencha.total_nao_preenchido			AS total_nao_preenchido
			FROM
				monitora.preencheacamensal	AS preencha
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
			);

		$arrListaUnidades = (array) $db->carregar( $strSql );
		$arrResultado = array();
		foreach( $arrListaUnidades as $arrUnidade )
		{
			$intUnidadeId					= $arrUnidade[ 'unidade_id' ];
			$intMes							= $arrUnidade[ 'mes' ];
			$intQuantidade					= $arrUnidade[ 'total_de_acoes' ];
			$intTotalPreenchidoLiberado		= $arrUnidade[ 'total_preenchido_liberado'] ;  
			$intTotalPreenchidoNaoLiberado	= $arrUnidade[ 'total_preenchido_nao_liberado'] ;  
			$intTotalNaoPreenchido			= $arrUnidade[ 'total_nao_preenchido'] ;
			$strDescricao					= $arrUnidade[ 'unidade_descricao' ];
			
			$arrResultado[ $intUnidadeId ][ 'nome' ] = $strDescricao;
			if( $intQuantidade > 0 )
			{
				$arrResultado[ $intUnidadeId ][ 'quantidade' ] = $intQuantidade;
			}
			$arrResultado[ $intUnidadeId ][ $intMes ][ 'total_preenchido_liberado' ]	= $intTotalPreenchidoLiberado;
			$arrResultado[ $intUnidadeId ][ $intMes ][ 'total_preenchido_nao_liberado' ]= $intTotalPreenchidoNaoLiberado;
			$arrResultado[ $intUnidadeId ][ $intMes ][ 'total_nao_preenchido' ]			= $intTotalNaoPreenchido;
			$arrResultado[ $intUnidadeId ][ $intMes ][ 'total_de_acoes' ]				= $intQuantidade;
		}
		return $arrResultado;
	}
	
	function geraListaDePreenchimento( $intAnoExercicio,  $arrMesesSelecionados = array() , $arrSituacoes , 
		$arrCodigoUnidade )
	{
		global $db;

		if( sizeof( $arrMesesSelecionados ) > 0 )
		{
			$intMesInicial	= $arrMesesSelecionados[ 0 ];
			$intMesFinal	= $arrMesesSelecionados[ sizeof( $arrMesesSelecionados ) - 1 ];	
		}
		else
		{
			$intMesInicial	= 1;
			$intMesFinal	= 12; 
		}
		
		if( $arrCodigoUnidade !== null )
		{
			$strSqlWhereUnicod = sprintf( " AND acao.unicod in ('%s') ", implode( "','" , (array) $arrCodigoUnidade ) );
		}
		
		$strSqlSituacoes = '';
		/**
		 * 2 = liberada,
		 * 1 = nao liberada
		 * 0 = nao preenchida
		 */
		if( sizeof( $arrSituacoes ) > 0 )
		{
			$arrSqlSituacoes = array();
			if ( !in_array( 'todos' , $arrSituacoes ) )
			{
				if( in_array( 'preenchido_liberado' , $arrSituacoes ) )
				{
					$arrSqlSituacoes[] = 2;
				}
				if( in_array( 'preenchido_nao_liberado' , $arrSituacoes ) )
				{
					$arrSqlSituacoes[] = 1;
								}
				if( in_array( 'nao_preenchido' , $arrSituacoes ) )
				{
					$arrSqlSituacoes[] = 0;
				}
			}
			if ( count( $arrSqlSituacoes ) > 0 )
			{
				$strSqlSituacoes = sprintf( " AND situacao IN( '%s' )" , implode( "','" , $arrSqlSituacoes ) ); 
			}
		}
		
		$strSql = sprintf(
			"
			SELECT 
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
			);
		$arrResultadoQuery = $db->carregar( $strSql );
		$arrResultadoQuery = $arrResultadoQuery ? $arrResultadoQuery : array();
		
		$arrResultado = array();
		foreach( $arrResultadoQuery as $arrUnidade )
		{
			$strFuncProg	= $arrUnidade[ 'funprog' ];
			$intMes			= $arrUnidade[ 'mes' ];
			$strSituacao	= $arrUnidade[ 'situacao' ];
			$strDescricao	= $arrUnidade[ 'acao_descricao' ];
			$arrResultado[ $strFuncProg ][ 'nome' ]		= $strDescricao;
			$arrResultado[ $strFuncProg ][ $intMes ]	= $strSituacao;
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
	
	function geraLinkBarraProgesso( $intVerde , $intAmarelo , 
		$intVermelho, $intBranco, $intTotal, $strMes )
	{
		$strSidDiretorio = $_SESSION[ 'sid_diretorio' ];
		$strUrl = 
		 './geral/infoMonitoraPreenchimento.php' .
		'?verde='		. $intVerde .
		'&amarelo='		. $intAmarelo .
		'&vermelho='	. $intVermelho .
		'&branco=' 		. $intBranco .
		'&vltotal='		. $intTotal .
		'&mes='			. $strMes;
		return $strUrl;
	}
	
	function geraBarraProgresso( $intVerde , $intAmarelo , 
		$intBranco, $intTotal = 100 , $strMes , $arrSituacoes , $intFullSize = 40 )
	{
		if( $intTotal == null )
		{
			?>
				<td class="barraTd" align="center" nowrap="nowrap" >
					-
				</td>
			<?
		}
		else
		if( in_array( 'todos' , $arrSituacoes ) )
		{
			$strAjaxLink = geraLinkBarraProgesso( $intVerde , $intAmarelo, 0 , $intBranco , $intTotal ,$strMes );
			
			$intVermelho = $intTotal - $intVerde - $intAmarelo - $intBranco;
			//$intBranco = $intTotal - $intVerde - $intAmarelo - $intVermelho;
			$intVerdeWidth		= round( $intFullSize * ( $intVerde		/ $intTotal ) , 2 );
			$intAmareloWidth	= round( $intFullSize * ( $intAmarelo	/ $intTotal ) , 2 );
			$intVermelhoWidth	= round( $intFullSize * ( $intVermelho	/ $intTotal ) , 2 );
			$intBrancoWidth		= round( $intFullSize * ( $intBranco	/ $intTotal ) , 2 );
			?>
				<td class="barraTd" align="center" nowrap="nowrap" >
					<span onmousemove="SuperTitleAjax( '<?= $strAjaxLink ?>', this );" onmouseout="SuperTitleOff( this );" >
						<img src="../imagens/cor1.gif" style="height:10px;width:<?= $intVerdeWidth ?>px" class="imgVerde"	><img 
						src="../imagens/cor2.gif" style="height:10px;width:<?= $intAmareloWidth ?>px" class="imgAmarelo"		><img
						src="../imagens/cor3.gif" style="height:10px;width:<?= $intVermelhoWidth ?>px" class="imgVermelho"		><img
						src="../imagens/cor0.gif" style="height:10px;width:<?= $intBrancoWidth ?>px" class="imgBranco">
					</span>
				</td>
			<?php
		}
		else
		{
			if( in_array( 'preenchido_liberado' , $arrSituacoes ) )
			{
				$intValue = $intVerde;
				$strColor = 'green';
				$strTitle = 'Total Preenchido e Liberado';
			}
			if( in_array( 'preenchido_nao_liberado' , $arrSituacoes ) )
			{
				$intValue = $intAmarelo;
				$strColor = 'rgb( 100, 100 , 0 )';
				$strTitle = 'Total Preenchido e Não Liberado';
			}
			if( in_array( 'nao_preenchido' , $arrSituacoes ) )
			{
				$intValue = $intBranco;
				$strColor = 'black';
				$strTitle = 'Total Não Preenchido';
			}
			if ( $intValue == 0 )
			{
				$intValue = '-';
			}
			?>
				<td style="color: <?= $strColor ?>; width: 50px" class="barraTd">
					<span>
						<font title="<?= $strTitle ?>" ><?= $intValue ?></font>
					</span>
				</td>
			<?php
		}
	}
	
	function geraHtmlDoAbreConteudo( $intAnoExercicio , $arrElementos , $arrMesesSelecionados , $arrNomesMeses )
	{
//		$arrSituacoes = array( 2 => 'Preenchido e Liberado' , 1 => 'Preenchido e Não Liberado' , 0 => 'Não Preenchido' );
		$arrSituacoes = array( 2 => 'PL' , 1 => 'PNL' , 0 => 'NP' );
		
		?>
				<table style="width: 100%;border-spacing: 0px; border-collapse: collapse; float: right;" cellspacing="0" cellpadding="0"  >
					<colgroup span="2" />
					<colgroup span="<?= sizeof( $arrMesesSelecionados ) ?>" style="width:<?= sizeof( $arrMesesSelecionados ) * 50 ?>px" /> 
					<thead>
						<tr>
							<td width="80px">
								Código
							</td>
							<td >
								Nome
							</td>
							<?php foreach( $arrMesesSelecionados as $intMesSelecionado ): ?>
								<td class="barraTd" align="center">
									<?= strtoupper( substr( $arrNomesMeses[ $intMesSelecionado ] , 0 , 3 ) ) ?>/<?= $intAnoExercicio ?>
								</td>
							<?php endforeach ?>
						</tr>
					</thead>
					<tbody>
						<?php $booCor = true; ?> 
						<?foreach( $arrElementos as $strFuncProg => $arrElemento ): ?>
							<?php $strCor = $booCor ? '#FAFAFA' : ''; ?>
							<tr onmouseover="this.bgColor='#ffffdd';" onmouseout="this.bgColor='<?= $strCor ?>';" bgcolor="<?= $strCor ?>">
								<td>
									<?= $strFuncProg ?>
								</td>
								<td>
									<?= $arrElemento[ 'nome' ] ?>
								</td>
								<?php foreach( $arrMesesSelecionados as $intMesSelecionado ): ?>
									<td class="barraTd siglaTd">
										<?php $strSituacao = $arrSituacoes[ $arrElemento[ $intMesSelecionado ] ]; ?>
										<?= $strSituacao ? $strSituacao : '-'; ?>
									</td>
								<?php endforeach ?>
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
				monta_titulo( 'Relatório de Preenchimento', 'Acompanhamento de Preenchimento Mensal de Ações' );
			?>
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>
			
			<form method="post" name="pformlista">
				<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
					<tr>
						<td class="SubTituloDireita">Mês:</td>
						<td style="color:#000080;">
							<select name="mes">
								<?php foreach( $arrMesOptions as $arrMesOption ): ?>
								
									<option	<?= $arrMesOption[ 'selected' ] ?> <?= $arrMesOption[ 'value' ] ?> >
									
										<?= $arrMesOption[ 'innerHTML' ] ?>
										
									</option>
									
								<?php endforeach; ?>
							</select>
						</td>
						<td class="SubTituloDireita">
							Unidade:
						</td>
						<td style="color:#000080;">
							<input type="text" name="codigounidade" maxlength="8" size="5" value="<?= $mixCodigoUnidade ?>" />
						</td>
						<td class="SubTituloDireita">
							Situação:
						</td>
						<td style="color:#000080;">
							<input type="radio" name="psituacao" value="todos" 
								<?=( in_array( 'todos' , $arrSituacoes ) ? 'checked="checked"' : '' )?>
							/>
								Todos
							<input type="radio" name="psituacao" value="nao_preenchido" 
								<?=( in_array( 'nao_preenchido' , $arrSituacoes ) ? 'checked="checked"' : '' )?>
							/>
								Não Preenchido
							<input type="radio" name="psituacao" value="preenchido_liberado" 
								<?=( in_array( 'preenchido_liberado' , $arrSituacoes ) ? 'checked="checked"' : '' )?>
							/>
								Preenchido e Liberado
							<input type="radio" name="psituacao" value="preenchido_nao_liberado" 
								<?=( in_array( 'preenchido_nao_liberado' , $arrSituacoes ) ? 'checked="checked"' : '' )?>
							/>
								Preenchido e Não Liberado
						</td>
						<td class="SubTituloDireita" style="text-align:center;">
							<input type="Button" style="width:80px;" name="pfiltrar" value="Filtrar" onclick="enviarformfiltro()">
						</td>
					</tr>
				</table>
			<table class="listagem" style="width: 95%; color: rgb(51, 51, 51);" align="center" border="0" cellpadding="2" cellspacing="0">
				<tbody>
					<tr bgcolor="#e7e7e7">
						<td align="center" width="">
							&nbsp;
						</td>
						<td style="">
							<b>
								Unidades
							</b>
						</td>
						<td style="">
							<b>
								Qtd. Ações
							</b>
						</td>
						<?php foreach( $arrMesesSelecionados as $intMesSelecionado ): ?>
							<td style="width: 51px; color: rgb(105, 105, 105); font-size: 10px; font-family: Verdana;" align="center">
								<?= strtoupper( substr( $arrNomesMeses[ $intMesSelecionado ] , 0 , 3 ) ) ?>/<?= $intAnoExercicio ?>
							</td>
						<?php endforeach ?>
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
								onclick="abreconteudo( 'monitora.php?modulo=relatorio/geral/preenchimento&acao=C&ajax=1&mes=<?= $intMesSelecionado ?>&ano=<?= $intAnoExercicio ?>&codigounidade=<?= $intUnidadeId ?>&psituacao=<?= implode( ',' , (array) $arrSituacoes ) ?>'  ,'<?= $strUnidadeComplementoId ?>')"
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
							<?php foreach( $arrMesesSelecionados as $intMesSelecionado ): ?>
								<?php geraBarraProgresso
									( 
										$arrUnidadeAnalise[ $intMesSelecionado ][ 'total_preenchido_liberado' ] , 
										$arrUnidadeAnalise[ $intMesSelecionado ][ 'total_preenchido_nao_liberado' ] ,
										$arrUnidadeAnalise[ $intMesSelecionado ][ 'total_nao_preenchido' ] , 
										$arrUnidadeAnalise[ $intMesSelecionado ][ 'total_de_acoes' ] ,
										$arrNomesMeses[ $intMesSelecionado ] ,
										$arrSituacoes 
									)
								?>
							<?php endforeach ?>
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
	
	$intAnoExercicio	= @$_REQUEST['ano'];
	if( $intAnoExercicio == null )
	$intAnoExercicio	= $_SESSION['exercicio'];
	
	if( @$_REQUEST['psituacao'] != 'todos' && @$_REQUEST['psituacao'] != '' )
	{
		$arrSituacoes	= explode( ',', @$_REQUEST['psituacao'] );
	}
	else
	{
		$arrSituacoes 	= array( 'todos' );
	}
	$mixCodigoUnidade	= @$_REQUEST[ 'codigounidade' ];
	$intMesSelecionado	= @$_REQUEST['mes'];
	$intPagina			= @$_REQUEST['pagina'];
	$boolAjax			= ( @$_REQUEST['ajax'] == 1 );
	
	if( $intPagina == null )
	{
		$intPagina = 1;
	}
	
	$arrMesesSelecionados	= gerArrMesesSelecionados( $intAnoExercicio , $intMesSelecionado );
	$arrNomesMeses = array(
		'01' => 'Janeiro',		'02' => 'Fevereiro',
		'03' => 'Março',		'04' => 'Abril',
		'05' => 'Maio',			'06' => 'Junho',
		'07' => 'Julho',		'08' => 'Agosto',
		'09' => 'Setembro',		'10' => 'Outubro',
		'11' => 'Novembro',		'12' => 'Dezembro'
	);
			
	if( $boolAjax )
	{
		$arrElementos = geraListaDePreenchimento( $intAnoExercicio , $arrMesesSelecionados , $arrSituacoes , $mixCodigoUnidade );
		geraHtmlDoAbreConteudo( $intAnoExercicio , $arrElementos , $arrMesesSelecionados , $arrNomesMeses );
	}
	else
	{
		include APPRAIZ . "includes/cabecalho.inc";
		$arrMesOptions	= geraArrayMesesReferencia(
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