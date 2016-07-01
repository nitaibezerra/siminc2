<?

require_once( "relatorio/funcoes_reltipos.inc" );

require_once( "relatorio/reltipos.inc" );

//exit();

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
function geraRelatorioPorProgramaDeElaboracaoERevisao( $intPosicaodaColunaOrdenada = 1 , $strDirecaoDaOrdenacao = 'ASC' , 
	$strOrdemGrupo , $arrTiposdePropostas , $strAgrupadoPor , $intAnoExercicio)
{
	//			$db->cria_aba($abacod_tela,$url,'');

	$strTitulo					= '';
	$strTituloTotal 			= '';
	$strCampoOrdenarPeloTotal	= '';
	
	geraTitulosPeloAgrupamento( $strAgrupadoPor , $strTitulo , $strTituloTotal, $strCampoOrdenarPeloTotal );
	
	$arrElements =  geraArrayDePropostas( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , $intPosicaodaColunaOrdenada , 
	$strDirecaoDaOrdenacao , $arrTiposdePropostas , true );
	
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
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" 
	bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
	
		<form name="formulario" method="post">
			<input type="Hidden" name="posicaoDaColunaOrdenada" value="<?= $intPosicaodaColunaOrdenada ?>" />
			<input type="Hidden" name="direcaoDaOrdenacao" value="<?= $strDirecaoDaOrdenacao ?>" />
		</form>
	</table>
	<? 
	if ( sizeof( $arrElements ) > 0 )
	{
	?>
			<!-- cabecalho da listagem -->
			<center>
			<table border="0" cellspacing="0" cellpadding="2" class="listagem" width="95%">
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
								<td  class="propostaCodigo">
									<!-- codigo da acao -->
									<font title="Código do Programa" ><?= $arrElement[ 'codigo_programa' ] ?></font>.<font title="Código da Ação" ><?= $arrElement[ 'codigo_acao' ] ?>
								</td>
								<td class="propostaDescricao" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome da acao -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td class="propostaQtdInclusao" title="Inclusões">
									<!-- Inclusoes -->
									<?=  $arrElement[ 'qtd_inclusao' ]  ?>
								</td>
								<td class="propostaQtdAlteracao"  title="Alterações">
									<!-- Alteracoes -->
									<?=  $arrElement[ 'qtd_alteracao' ]  ?>
								</td>
								<td class="propostaQtdMigracao" title="Migrações">
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td class="propostaQtdFusao" title="Fusões">
									<!-- Fusoes -->
									<?=  $arrElement[ 'qtd_fusao' ]  ?>
								</td>
								<td class="propostaQtdExclusao" title="Exclusões">
									<!-- Exclusoes -->
									<?=  $arrElement[ 'qtd_exclusao' ]  ?>
								</td>
								<td class="propostaQtdTotal" title="Quantidade">
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
						case 'Programas':
						{
							?>
							<!-- elemento programa -->
							<tr bgcolor="<?= $strStyleMarcado ?>" onmouseover="this.bgColor='#ffffcc';" 
							onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td  class="propostaCodigo">
									<!-- codigo do programa -->
									<font title="Código do Programa" >
										<?= $arrElement[ 'codigo_programa' ] ?>
									</font>
								</td>
								<td class="propostaDescricao" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome do programa -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td class="propostaQtdInclusao" title="Inclusões">
									<!-- Inclusoes -->
									<?=  $arrElement[ 'qtd_inclusao' ]  ?>
								</td>
								<td class="propostaQtdAlteracao" title="Alterações">
									<!-- Alteracoes -->
									<?=  $arrElement[ 'qtd_alteracao' ]  ?>
								</td>
								<td class="propostaQtdMigracao" title="Migrações">
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td class="propostaQtdFusao" title="Fusões">
									<!-- Fusoes -->
									<?=  $arrElement[ 'qtd_fusao' ]  ?>
								</td>
								<td class="propostaQtdExclusao" title="Exclusões">
									<!-- Exclusoes -->
									<?=  $arrElement[ 'qtd_exclusao' ]  ?>
								</td>
								<td class="propostaQtdTotal" title="Quantidade">
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
								<td class="propostaQtdInclusao" title="Inclusões">
									<!-- Inclusoes -->
									<?=  $arrElement[ 'qtd_inclusao' ]  ?>
								</td>
								<td class="propostaQtdAlteracao" title="Alterações">
									<!-- Alteracoes -->
									<?=  $arrElement[ 'qtd_alteracao' ]  ?>
								</td>
								<td class="propostaQtdMigracao" title="Migrações">
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td class="propostaQtdFusao" title="Fusões">
									<!-- Fusoes -->
									<?=  $arrElement[ 'qtd_fusao' ]  ?>
								</td>
								<td class="propostaQtdExclusao" title="Exclusões">
									<!-- Exclusoes -->
									<?=  $arrElement[ 'qtd_exclusao' ]  ?>
								</td>
								<td class="propostaQtdTotal" title="Quantidade">
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
		<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" 
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
	</div>
	<?
}

/**
// chamada padrão ao cabecalho do sistema //
include  APPRAIZ . "includes/cabecalho.inc";
print "<br/>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( $titulo_modulo , $titulo);

$intPosicaodaColunaOrdenada	= (int)		@$_REQUEST[ 'posicaoDaColunaOrdenada' ];
$strDirecaoDaOrdenacao		= (string)	@$_REQUEST[ 'direcaoDaOrdenacao' ];
$strOrdemGrupo				= (string)	@$_REQUEST[ 'ordemgrupo' ];
$arrTiposdePropostas 		= (array)	@$_REQUEST[ 'arrTiposdePropostas' ];
$strAgrupadoPor				= 'Programas';

if( $intPosicaodaColunaOrdenada == null ) 	$intPosicaodaColunaOrdenada = 1;
if( $strDirecaoDaOrdenacao == null ) 		$strDirecaoDaOrdenacao = 'ASC';
if( $strOrdemGrupo == null ) 				$strOrdemGrupo = 'A';

$intAnoExercicio = $_SESSION[ 'exercicio' ];

geraRelatorioPorProgramaDeElaboracaoERevisao( $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $strOrdemGrupo , $arrTiposdePropostas , $strAgrupadoPor , $intAnoExercicio );

*/
?>