<?

require_once( "funcoes_reltipos.inc" );

/**
 * Funcao raiz do controle de relatorios por tipo de proposta que recebe os filtros e parametros
 * enviados pela requisição e monta de fato o html desejado
 *
 * @param integer $intPosicaodaColunaOrdenada
 * @param string $strDirecaoDaOrdenacao
 * @param string $strOrdemGrupo
 * @param array $arrTiposdePropostas
 * @param string  $strAgrupadoPor
 * @param integer $intAnoExercicio
 */
function geraRelatorioPorTipodeProposta( $intPosicaodaColunaOrdenada = 1 , $strDirecaoDaOrdenacao = 'ASC' , 
	$strOrdemGrupo , $arrTiposdePropostas , $strAgrupadoPor , $intAnoExercicio)
{
	//			$db->cria_aba($abacod_tela,$url,'');

	$strTitulo					= '';
	$strTituloTotal 			= '';
	$strCampoOrdenarPeloTotal	= '';
	
	geraTitulosPeloAgrupamento( $strAgrupadoPor , $strTitulo , $strTituloTotal, $strCampoOrdenarPeloTotal );
	
	$arrElements =  geraArrayDePropostas( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , $intPosicaodaColunaOrdenada , 
	$strDirecaoDaOrdenacao , $arrTiposdePropostas , ( sizeof( $arrTiposdePropostas ) == 0 ) );
	
	?>
	<style>
	
		TABLE.listagem2
		{
			font-size: 11px;
			padding: 3px;
			border-top: 2px solid #404040;
			border-bottom: 3px solid #dfdfdf;
			border-collapse: collapse;
			width: 90%;
		}
		
		TABLE.listagem2 THEAD
		{
		background-color: #E3E3E3;
		}
				
		TABLE.listagem2 THEAD P
		{
		color: #6A6A6A;
		}
		
		TABLE.listagem2 THEAD P B
		{
		color: #D90000;
		}
			
		TABLE.listagem2 THEAD TD.order
		{
		background-color: #133368;
		border-right: 1px solid #ff9000;
		}
		
		TABLE.listagem2 THEAD TD A, 
		TABLE.listagem2 THEAD TD A:visited
		{
		color: #6A6A6A;
		}

		TABLE.listagem2 THEAD TD A:hover
		{
		color: red;
		}
		
		TABLE.listagem2 THEAD TD.order A
		{
		color: white;
		}
		
		TABLE.listagem2 THEAD TD.order A:visited
		{
		color: #BFBFBF;
		}
		
		TABLE.listagem2 TBODY TD P A
		{
		color: #133368;
		}
		
		TABLE.listagem2 TBODY TD A:visited
		{
		color: black;
		}
		
		TABLE.listagem2 TBODY TD A:hover, 
		TABLE.listagem2 TBODY TD A:visited:hover
		{
		color: #E47100;
		}
	
		TABLE.listagem2 TBODY TD P A.alerta, 
		TABLE.listagem2 TBODY TD P A.alerta:visited
		{
		color: #E47100;
		font-weight: bold;
		}
		
		TABLE.listagem2 TBODY TD P A.alerta:hover, 
		TABLE.listagem2 TBODY TD P A.alerta:visited:hover
		{
		color: black;
		font-weight: bold;
		text-decoration: none;
		}
		
		TABLE.listagem2 TBODY TR.marcado
		{
		background-color: #f5f5f5;
		}
		.propostaDescricao
		{
			width: 730px;
			color:#003c7b;
		}
		.propostaCodigo
		{
			width: 60px;
		}
		.propostaQtdInclusao
		{
			color: rgb( 128, 64, 0 );
			text-align: center;
			width: 15px;
		}
		.propostaQtdAlteracao
		{
			color: blue;
			text-align: center;
			width: 15px;
		}
		.propostaQtdMigracao
		{
			color: black;
			text-align: center;
			width: 15px;
		}
		.propostaQtdFusao
		{
			color: green;
			text-align: center;
			width: 15px;
		}
		.propostaQtdExclusao
		{
			color: red;
			text-align: center;
			width: 15px;
		}
		.propostaQtdTotal
		{
			color: black;
			text-align: center;
			width: 15px;
		}
		

		
		
	</style>	
	<script language="JavaScript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/superTitle.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/superTitle.css">
	<script language="JavaScript">
	
	
	function filtra()
	{
		document.formulario.submit();
	}
	
	function ordena( ordem , direcao)
	{
		document.formulario.ordemlista.value=ordem;
		document.formulario.ordemlistadir.value=direcao;
		document.formulario.submit();
	}
	</script>
	<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" 
	bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
	
		<form name="formulario" method="post">
			<input type="Hidden" name="posicaoDaColunaOrdenada" value="<?= $intPosicaodaColunaOrdenada ?>" />
			<input type="Hidden" name="direcaoDaOrdenacao" value="<?= $strDirecaoDaOrdenacao ?>" />
			<tr>
				<td class="SubTituloDireita">Tipo de proposta:</td>
				<td style="color:#008000;">
					<input type="checkbox" name="arrTiposdePropostas[inclusao]"		value="true"  
					<?= checkIfTrue( $arrTiposdePropostas[ 'inclusao' ] ) ?> />
					Inclusão
					<input type="checkbox" name="arrTiposdePropostas[alteracao]"	value="true" 
					<?= checkIfTrue( $arrTiposdePropostas[ 'alteracao' ] ) ?> />
					Alteração
					<input type="checkbox" name="arrTiposdePropostas[migracao]"		value="true"  
					<?= checkIfTrue( $arrTiposdePropostas[ 'migracao' ] ) ?> />
					Migração
					<input type="checkbox" name="arrTiposdePropostas[exclusao]"		value="true"  
					<?= checkIfTrue( $arrTiposdePropostas[ 'exclusao' ] ) ?> />
					Exclusão
					<input type="checkbox" name="arrTiposdePropostas[fusao]"		value="true"  
					<?= checkIfTrue( $arrTiposdePropostas[ 'fusao' ] ) ?> />
					Fusão
				</td>
				<td class="SubTituloDireita">
					Agrupado por:
				</td>
				<td style="color:#008000;">
				
					<input type="radio" name="strAgrupadoPor" value="Acoes"
					<?= checkIfTrue( $strAgrupadoPor == 'Acoes' ) ?> />
					Ações 
					
					<input type="radio" name="strAgrupadoPor" value="Programas"
					<?= checkIfTrue( $strAgrupadoPor == 'Programas' ); ?> />
					Programas
					
					<input type="radio" name="strAgrupadoPor" value="Unidades"  
					<?= checkIfTrue( $strAgrupadoPor == 'Unidades' ); ?> />
					Unidades
					
				</td>
				<td class="SubTituloDireita" style="text-align:center;">
					<input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"/>
				</td>
			</tr>
		</form>
	</table>
	<? 
	if ( sizeof( $arrElements ) > 0 )
	{
	?>
	<center>
		<div>
			<!-- cabecalho da listagem -->
			<!--table border="0" onclick="alert( this.offsetWidth )" cellspacing="0" cellpadding="2" class="listagem"-->
			<table width="95%" class="listagem" cellspacing="0" cellpadding="0">
				<thead>
					<tr>
						<!-- codigo -->
						<td nowrap class="propostaCodigo title">
							<? geraImagemDaDirecaoDaOrdenacao( 1 , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , 'Código' ); ?>
						</td>
						<!-- titulo -->
						<td class=" propostaDescricao title">
							<? geraImagemDaDirecaoDaOrdenacao( 2 , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , 'Título ' . $strTitulo ); ?>
						</td>
						<!-- total -->
						<td class="propostaQtdInclusao">
							<!-- Inclusoes -->
							<img src="../imagens/barras/BarraInclusoes2.PNG" />
						</td>
						<td class="propostaQtdAlteracao" >
							<!-- Alteracoes -->
							<img src="../imagens/barras/BarraAlteracoes2.PNG" />
						</td>
						<td class="propostaQtdMigracao">
							<!-- Migracoes -->
							<img src="../imagens/barras/BarraMigracoes2.PNG" />
						</td>
						<td class="propostaQtdFusao" >
							<!-- Fusoes -->
							<img src="../imagens/barras/BarraFusoes2.PNG" />
						</td>
						<td class="propostaQtdExclusao" >
							<!-- Exclusoes -->
							<img src="../imagens/barras/BarraExclusoes2.PNG" />
						</td>
						<td class="propostaQtdTotal">
							<!-- contador de tarefas deste elemento neste filtro -->
							<img src="../imagens/barras/BarraQuantidade2.PNG" />
						</td>
					</tr>
				</thead>
			<tbody>
				<?
				
				$intTotalQuantity = 0;
				
				foreach ( $arrElements as $intKey=> $arrElement )
				{
					
					$intTotalQuantity += $arrElement[ 'quantidade' ];
					
					$cor = '';
					
					if( $intKey % 2 == 0) $strStyleMarcado = ''; else $strStyleMarcado='#F7F7F7';
				?>
					<?
					switch( $strAgrupadoPor )
					{
						case 'Acoes':
						{
							?>
							<!-- elemento acao -->
							<tr bgcolor="<?= $strStyleMarcado ?>" onmouseover="this.bgColor='#ffffcc';" 
							 onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td  class="propostaCodigo" >
									<!-- codigo da acao -->
									<a href="elabrev.php?modulo=principal/ppafasequalitativa/acao/cadacao&acao=C&acaid=<?= $arrElement[ 'acao_id' ] ?>&prgid=<?= $arrElement[ 'programa_id' ] ?>" >
										<font title="Código do Programa" ><?= $arrElement[ 'codigo_programa' ] ?></font>.<font title="Código da Ação" >
											<?= $arrElement[ 'codigo_acao' ] ?>
										</font>
									</a>
								</td>
								<td class="propostaDescricao" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome da acao -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								
								
									<td align="center" title="Inclusões" >
											<?= $arrElement[ 'qtd_inclusao' ] ?>
									</th>
									<td align="center" title="Alterações">
											<?= $arrElement[ 'qtd_alteracao' ] ?>
									</td>
									<td align="center" title="Migrações" >
											<?= $arrElement[ 'qtd_migracao' ] ?>
									</td>
									<td align="center" title="Fusões" >
											<?= $arrElement[ 'qtd_fusao' ] ?>
									</td>
									<td align="center" title="Exclusões">
											<?= $arrElement[ 'qtd_exclusao' ] ?>
									</td>
									<td align="center" title="Quantidade" >
											(<?= $arrElement[ 'quantidade' ] ?>)
									</td>								
							</tr>
							<!-- separador -->
							<tr bgcolor="<?= $strStyleMarcado ?>" >
								<td 
								colspan="8" 
								style="padding-left:65px;"
								id="td<?= $arrElement[ 'linkIdConteudo' ] ?>"></td>
							</tr>
							<?
							break;
						}
						case 'Programas':
						{
							?>
							<!-- elemento programa -->
							<tr bgcolor="<?= $strStyleMarcado ?>" onmouseover="this.bgColor='#ffffcc';" 
							onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td  class="propostaCodigo">
									<!-- codigo do programa -->
									<a href="elabrev.php?modulo=principal/ppafasequalitativa/programa/cadprograma&acao=C&prgid=<?= $arrElement[ 'programa_id' ] ?>">
										<font title="Código do Programa" >
											<?= $arrElement[ 'codigo_programa' ] ?>
										</font>
									</a>
								</td>
								<td class="propostaDescricao" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome do programa -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td class="propostaQtdInclusao">
									<!-- Inclusoes -->
									<?=  $arrElement[ 'qtd_inclusao' ]  ?>
								</td>
								<td class="propostaQtdAlteracao" >
									<!-- Alteracoes -->
									<?=  $arrElement[ 'qtd_alteracao' ]  ?>
								</td>
								<td class="propostaQtdMigracao">
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td class="propostaQtdFusao" >
									<!-- Fusoes -->
									<?=  $arrElement[ 'qtd_fusao' ]  ?>
								</td>
								<td class="propostaQtdExclusao" >
									<!-- Exclusoes -->
									<?=  $arrElement[ 'qtd_exclusao' ]  ?>
								</td>
								<td class="propostaQtdTotal">
									<!-- contador de tarefas deste elemento neste filtro -->
									(<?=  $arrElement[ 'quantidade' ]  ?>)
								</td>
							</tr>
							<!-- separador -->
							<tr bgcolor="<?= $strStyleMarcado ?>" >
								<td 
								colspan="8" 
								style="padding-left:65px;"
								id="td<?= $arrElement[ 'linkIdConteudo' ] ?>"></td>
							</tr>
							<?
							break;
						}
						case 'Unidades':
						default:
						{
							?>
							<!-- elemento unidade -->
							<tr bgcolor="<?= $strStyleMarcado ?>" onmouseover="this.bgColor='#ffffcc';" 
							 onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td  class="propostaCodigo">
									<!-- codigo da unidade -->
									<font title="Código da Unidade" ><?= $arrElement[ 'codigo_unidade' ] ?></font>
								</td>
								<td class="propostaDescricao" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome da unidade -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td class="propostaQtdInclusao">
									<!-- Inclusoes -->
									<?=  $arrElement[ 'qtd_inclusao' ]  ?>
								</td>
								<td class="propostaQtdAlteracao" >
									<!-- Alteracoes -->
									<?=  $arrElement[ 'qtd_alteracao' ]  ?>
								</td>
								<td class="propostaQtdMigracao">
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td class="propostaQtdFusao" >
									<!-- Fusoes -->
									<?=  $arrElement[ 'qtd_fusao' ]  ?>
								</td>
								<td class="propostaQtdExclusao" >
									<!-- Exclusoes -->
									<?=  $arrElement[ 'qtd_exclusao' ]  ?>
								</td>
								<td class="propostaQtdTotal">
									<!-- contador de tarefas deste elemento neste filtro -->
									(<?=  $arrElement[ 'quantidade' ]  ?>)
								</td>
							</tr>
							<!-- separador -->
							<tr bgcolor="<?= $strStyleMarcado ?>" >
								<td 
								colspan="8" 
								style="padding-left:65px;"
								id="td<?= $arrElement[ 'linkIdConteudo' ] ?>"></td>
							</tr>
							<?
							break;
						}	
					}
				}
				?>
				<!-- rodape -->
				<? 
				
				if( sizeof( $arrElements ) % 2 == 0) $strStyleMarcado = ''; else $strStyleMarcado='#F7F7F7';
				
				?>
					<tr class="<?= $strStyleMarcado ?>">
						<td></td>
						<?
						switch( $strAgrupadoPor )
						{
							case 'Acoes':
							{
								?>
								<td valign="top" class="title" colspan="7"> 
									<table width="100%">
									  	<tr>
											<td style="border:none;">
							  					<strong>
							  						Total <?= $strTituloTotal ?>: <?= sizeof( $arrElements ) ?>
										  		</strong>
										  	</td>
										  	<td align="right" style="border:none;">
									  			<strong>
									  			Total de Propostas:
									  			<?= $intTotalQuantity ?>
							  					</strong>
								  			</td>
										</tr>
									</table>
								</td>
								<?
								break;
							}
							case 'Programas':
							case 'Unidades':
							default:
							{
								?>
								<td valign="top" class="title" colspan="7"> 
									<table width="100%">
									  	<tr>
											<td style="border:none;">
							  					<strong>
							  						Total <?= $strTituloTotal ?>: <?= sizeof( $arrElements ) ?>
										  		</strong>
										  	</td>
										  	<td align="right" style="border:none;">
									  			<strong>
									  			Total de Localizadores de Gasto:
									  			<?= $intTotalQuantity ?>
							  					</strong>
								  			</td>
										</tr>
									</table>
								</td>
								<?
								break;
							}
						}
						?>	
						<!--
						<td align="right" >
							<strong>
								<?= $intTotalQuantity ?>
							</strong>
						</td>
						-->
					</tr>
			</tbody>
		</table>

	<? 
	} 
	else 
	{ 
	?>
		<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" 
		bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
			<tr>
				<td align="center">
					Não foram encontrados registros
				</td>
			</tr>
		</table>
	<? 
	}
	?>
</center>
	<?
}

// chamada padrão ao cabecalho do sistema //
include  APPRAIZ . "includes/cabecalho.inc";

// chamada padrão ao cabecalho do sistema //
print "<br/>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( $titulo_modulo , $titulo);

$intPosicaodaColunaOrdenada	= (int)		@$_REQUEST[ 'posicaoDaColunaOrdenada' ];
$strDirecaoDaOrdenacao		= (string)	@$_REQUEST[ 'direcaoDaOrdenacao' ];
$strOrdemGrupo				= (string)	@$_REQUEST[ 'ordemgrupo' ];
$arrTiposdePropostas 		= (array)	@$_REQUEST[ 'arrTiposdePropostas' ];
$strAgrupadoPor				= (string)	@$_REQUEST[ 'strAgrupadoPor' ];

if( $intPosicaodaColunaOrdenada == null ) 	$intPosicaodaColunaOrdenada = 1;
if( $strDirecaoDaOrdenacao == null ) 		$strDirecaoDaOrdenacao = 'ASC';
if( $strAgrupadoPor == null ) 				$strAgrupadoPor = 'Acoes';
if( $strOrdemGrupo == null ) 				$strOrdemGrupo = 'A';

$intAnoExercicio = $_SESSION[ 'exercicio' ];

geraRelatorioPorTipodeProposta( $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $strOrdemGrupo , $arrTiposdePropostas , $strAgrupadoPor , $intAnoExercicio );
?>