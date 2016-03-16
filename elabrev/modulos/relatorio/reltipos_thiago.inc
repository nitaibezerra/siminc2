<?
error_reporting( E_WARNING );
@session_start();

function mostraResultadoDaQuery( $strSql )
{
	global $db;

	dbg( $strSql );
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	?>
		<table border="1">		
	<?
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );

		if( $intLinhaAtual == 0 )
		{
			$arrKeys = array_keys( $objRegistroAtual );
			?>
				<tr>
			<?
			foreach( $arrKeys as $strKey )
			{
				if ( ! is_int( $strKey ) )
				{
					?>
						<th>
							<?= $strKey ?>
						</th>
					<?
				}
			}
			?>
				</tr>
			<?
		}
		
		?>
			<tr>
		<?
		foreach( $arrKeys as $strKey )
		{
			if ( ! is_int( $strKey ) )
			{
				?>
					<td>
						<?= $objRegistroAtual[ $strKey ] ?>
					</td>
				<?
			}
		}
		?>
			</tr>
		<?
	}
	?>
		</table>
	<?
}

function checkIfTrue( $boolElement )
{
	if( (bool) $boolElement )
	{
		$strReturn = 'checked="checked"';
	}
	else
	{
		$strReturn = '';
	}
	return $strReturn;
}

function geraLink( $strAgrupadoPor , $strOrdemGrupo = '' , $arrTiposdePropostas ,  $intCodigo = 0 , 
	$intContador , $intProgramaId = 0, $intAcaoId = 0 )
{
	$arrAbreviacoesDoAgrupamento = array( 'A' => 'Acoes', 'P' => 'Programas', 'U' => 'Unidades' );
		
	switch( $strAgrupadoPor )
	{
		case 'Acoes':
		{
			//abreconteudo('geral/listaacaoproposta.php?&amp;subnivel=3&amp;acaid=206&amp;prgid=17&amp;inclusao=&amp;exclusao=&amp;alteracao=&amp;fusao=','206')

			$strLink = 
			'\'' .
			'geral/reltipos_ajax.php?' . 
			'&' . 'intAcaoId'						. '=' . $intAcaoId .
			'&' . 'strAgrupadoPor'					. '=' . $strAgrupadoPor .
			'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
			'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
			'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
			'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
			'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
			'\'' . ',' . '\'' . $intContador . '-' . $intCodigo . '\'';

			break;
		}
		case 'Programas':
		{
			
			$strLink = 
			'\'' .
			'geral/reltipos_ajax.php?' . 
			'&' . 'intProgramaId'					. '=' . $intProgramaId .
			'&' . 'strAgrupadoPor'					. '=' . $strAgrupadoPor .
			'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
			'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
			'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
			'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
			'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
			'\'' . ',' . '\'' . $intContador . '-' . $intCodigo . '\'';
			
			//$strLink = $_SESSION['sisdiretorio'] . 'php?modulo=relatorio/relprogramanovo&acao=A&prgid=' . $intProgramaId;
			break;
		}
		case 'Unidades':
		{
			
			$strLink = 
			'\'' .
			'geral/listaacaoproposta.php?' .
			'unicod='		. $intCodigo 	. '&' .
			'tipo='			. converteFormatoDoTipo( $strAgrupadoPor , false ) 	. '&' .
			'ordem='		. $strOrdemGrupo	. '&' .
			'inclusao='		. $arrTiposdePropostas[ 'inclusao' ]	. '&' .
			'exclusao='		. $arrTiposdePropostas[ 'exclusao' ] 	. '&' .
			'alteracao='	. $arrTiposdePropostas[ 'alteracao' ] 	. '&' .
			'migracao='		. $arrTiposdePropostas[ 'migracao' ]	. '&' .
			'fusao='		. $arrTiposdePropostas[ 'fusao' ]		.
			'\'' . ',' . '\'' . $intContador . '-' . $intCodigo . '\'';
			
			break;
		}
	}

	return $strLink;
}

function geraImagemId( $intLinhaAtual , $strCodigo )
{
	return 'img' . $intLinhaAtual . '-' . $strCodigo;
}

function converteFormatoDoTipo( $strTipo , $boolBancoParaTexto = true )
{
	$arrBancoTipo = array
	(
		'I' => 'Inclusão' ,
		'A' => 'Alteração' ,
		'E' => 'Exclusão' ,
		'F' => 'Fusão' ,
		'M' => 'Migração'
	);
	
	if( $boolBancoParaTexto )
	{
		$strReturn = $arrBancoTipo[ $strTipo ];
	}
	else
	{
		$strReturn = array_search( $strTipo , $arrBancoTipo );
	}
	
	return $strReturn;
}

/**
 * No codigo devera aparecer o codigo da unidade e a descricao / nome da unidade.
 * Na expansao irá mostrar todas as acoes ordenadas pelo codigo do programa e depois por
 * acao. Quando clicar na acao expande as propostas.
 *
 * @param string	$strAgrupadoPor
 * @param integer	$intAnoExercicio
 * @param string	$strOrdemGrupo
 * @param integer	$intPosicaodaColunaOrdenada
 * @param string	$strDirecaoDaOrdenacao
 * @param array		$arrTiposdePropostas
 * @return array
 */
function geraArrayDePropostasAgrupadasPelasUnidades( $strAgrupadoPor , $intAnoExercicio, $strOrdemGrupo , 
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas )
{
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'UnidadeAcao.unicod' . 									' AS ' . ' unidade_codigo' . 
		' , ' . 	
		'Unidade.unidsc' . 										' AS ' . ' unidade_descricao' . 
		' , ' . 
		'Unidade.uniid' . 										' AS ' . ' unidade_id' . 
		' , ' . 
		' SUM ' . ' ( ' . 'quantidade_de_propostas' . ' ) ' .	' AS ' . 'quantidade_de_propostas' . 
		' , ' .		
		' SUM ' . ' ( ' . 'quantidade_fusao' . ' ) ' . 			' AS ' . 'quantidade_fusao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_migracao' . ' ) ' . 		' AS ' . 'quantidade_migracao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' . 		' AS ' . 'quantidade_alteracao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' . 		' AS ' . 'quantidade_inclusao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' . 		' AS ' . 'quantidade_exclusao' . 
	' FROM ' .
		' ( ' .
		' SELECT ' .
			'acao_codigo' . 
			' , ' . 
			'programa_codigo' . 
			' , ' . 
			'acao_id' . 
			' , ' .
			'programa_id' .
			' , ' .		
			' SUM ' . ' ( ' . 'quantidade_por_tipo' . ' ) ' 	. ' AS ' . 'quantidade_de_propostas' . 
			' , ' .		
			' SUM ' . ' ( ' . 'quantidade_fusao' . ' ) ' 		. ' AS ' . 'quantidade_fusao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_migracao' . ' ) ' 	. ' AS ' . 'quantidade_migracao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' 	. ' AS ' . 'quantidade_alteracao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' 	. ' AS ' . 'quantidade_inclusao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' 	. ' AS ' . 'quantidade_exclusao' . 
		' FROM ' .
			' ( ' .
			' SELECT ' .
				'acao_id' .
				' , ' .
				'Acoes.acacod' . ' AS ' . 'acao_codigo' .
				' ,  '.
				'Acoes.prgcod' . ' AS ' . 'programa_codigo' . 
				' ,  ' .
				'Acoes.prgid' . ' AS ' . 'programa_id' . 
				' ,  ' .
				'tipo' .
				' , ' .
				'COUNT( '. 'acao_id' . ' ) ' . ' AS ' . 'quantidade_por_tipo' .
				' , ' .		
				' SUM ' . ' ( ' . 'quantidade_fusao' . ' ) ' 		. ' AS ' . 'quantidade_fusao' . 
				' , ' .
				' SUM ' . ' ( ' . 'quantidade_migracao' . ' ) ' 	. ' AS ' . 'quantidade_migracao' . 
				' , ' .
				' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' 	. ' AS ' . 'quantidade_alteracao' . 
				' , ' .
				' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' 	. ' AS ' . 'quantidade_inclusao' . 
				' , ' .
				' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' 	. ' AS ' . 'quantidade_exclusao' . 
			 ' FROM ' .
				' ( ' .
	// fusão //
					' SELECT ' .
						'acaid' . ' AS ' . 'acao_id' .
						' , ' .
						'\'F\'' . ' AS ' . 'tipo' .
						' , ' .
				 		'usucpf' . ' AS ' . 'usuario_cpf' .
						' , ' .
				 		'1' . ' AS ' . 'quantidade_fusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_migracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_alteracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_inclusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_exclusao' .
					' FROM ' . 
						'elabrev.proposta_fusao_acao' . 
					' GROUP ' . ' BY ' .
						'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' .
					' UNION ' . ' ALL ' .
	// migração //
					' SELECT ' . 
						'acaid' . ' AS ' . 'acao_id' . 
						' , '.
						'\'M\'' . ' AS ' . 'tipo' .  
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
						' , ' .
				 		'0' . ' AS ' . 'quantidade_fusao' .
				 		' , ' .
				 		'1' . ' AS ' . 'quantidade_migracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_alteracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_inclusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_exclusao' .
					' FROM ' . 
						'elabrev.proposta_migracao_acao' .  
					' GROUP ' . ' BY ' .
						'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' .
					' UNION ' . ' ALL ' .
	// alteração //
					' SELECT ' . 
						'eracod' . ' AS ' . 'acao_id' . 
						' , ' .
				 		'\'A\'' . ' AS ' . 'tipo' .
						' , ' .
				 		'usucpf' . ' AS ' . 'usuario_cpf' . 
						' , ' .
				 		'0' . ' AS ' . 'quantidade_fusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_migracao' .
				 		' , ' .
				 		'1' . ' AS ' . 'quantidade_alteracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_inclusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_exclusao' .
					' FROM ' . 
						'elabrev.elaboracaorevisao' . 
					' WHERE ' .
						'eratabela' . ' = ' . '\'ppaacao_proposta\'' .
					' GROUP ' . ' BY ' .
						'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' .
					' UNION ' . ' ALL ' .
	// inclusão //
					' SELECT ' .
					
						'acaid' . ' AS ' . 'acao_id' . 
						' , '.
						'\'I\'' . ' AS ' . 'tipo' .  
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
						' , ' .
				 		'0' . ' AS ' . 'quantidade_fusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_migracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_alteracao' .
				 		' , ' .
				 		'1' . ' AS ' . 'quantidade_inclusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_exclusao' .
					' FROM ' .
						'elabrev.ppaacao_proposta' . 
					' WHERE ' .
						'acastatus' . ' = ' . '\'' . 'N' . '\'' .
					' GROUP ' . ' BY ' .
						'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' .
					' UNION ' . ' ALL ' .
	// exclusão //
					' SELECT ' .
						'acaid' . ' AS ' . 'acao_id' . 
						' , '.
						'\'E\'' . ' AS ' . 'tipo' .  
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
						' , ' .
				 		'0' . ' AS ' . 'quantidade_fusao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_migracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_alteracao' .
				 		' , ' .
				 		'0' . ' AS ' . 'quantidade_inclusao' .
				 		' , ' .
				 		'1' . ' AS ' . 'quantidade_exclusao' .
					' FROM ' .
						'elabrev.proposta_exclusao_acao' .
				 	' GROUP ' . ' BY ' .
				 		'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' . 
				' ) ' .
			' AS ' .
				'Propostas' .
			' LEFT ' . ' JOIN  ' .
				'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' .
			' ON ' . 
				'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
			' WHERE ' .
				'Acoes.prsano' . ' = ' . $intAnoExercicio .
				$strWhereAppend .
			' GROUP BY '.
				'acao_id' . 
				' , ' .
				'programa_id' .
				' , ' .
				'tipo' . 
				' , ' .
				'acao_codigo' . 
				' , ' .
				'programa_codigo' .
				' , ' .
				'usuario_cpf' .
			' ) ' .
		' AS ' . 'PropostasPorTipo' .
		' GROUP ' . ' BY ' .
			'acao_id' . 
			' , ' .
			'acao_codigo' . 
			' , ' .
			'programa_codigo' . 
			' , ' .
			'programa_id' .
	' ) ' . ' AS ' . 'PropostasPorTipo' . 
	' JOIN '  .
		'elabrev.unidade_acao' . ' AS ' . 'UnidadeAcao' .
	' ON ' .
		'UnidadeAcao.acaid' . ' = ' . 'PropostasPorTipo.acao_id' .
	' JOIN '  .
		'public.unidade' . ' AS ' . 'Unidade' .
	' ON ' .
		'Unidade.unicod' . ' = ' . 'UnidadeAcao.unicod' .
	' GROUP BY ' .
		'unidade_codigo' .
		' , ' .
		'unidade_descricao' .
		' , ' .
		'unidade_id' .
	' ORDER BY ' .
		'UnidadeAcao.unicod' .
	'';	
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$intUnidadeId		= $objRegistroAtual[ 'unidade_id' ];
		$strCodigoUnidade	= $objRegistroAtual[ 'unidade_codigo' ];
		$strDescricao		= $objRegistroAtual[ 'unidade_descricao' ];
		$intQtdPropostas	= $objRegistroAtual[ 'quantidade_de_propostas' ];
		$intQtdFusao		= $objRegistroAtual[ 'quantidade_fusao' ];
		$intQtdMigracao		= $objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= $objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= $objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= $objRegistroAtual[ 'quantidade_exclusao' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoAcao , $intLinhaAtual , $intProgramaId , $intAcaoId );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoUnidade;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoUnidade );
		$arrProposta[ 'codigo_unidade']		= $strCodigoUnidade;
		$arrProposta[ 'descricao' ]			= $strDescricao;
		$arrProposta[ 'quantidade' ]		= $intQtdPropostas;
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		$arrProposta[ 'qtd_migracao' ]		= $intQtdMigracao;
		$arrProposta[ 'qtd_alteracao' ]		= $intQtdAlteracao;
		$arrProposta[ 'qtd_inclusao' ]		= $intQtdInclusao;
		$arrProposta[ 'qtd_exclusao' ]		= $intQtdExclusao;
		
		$arrPropostas[] = $arrProposta;
	}
	return $arrPropostas;
}

function geraFiltroPorTiposdePropostas( $arrTiposdePropostas , $strNomeTabela )
{
	$arrTiposBancoProposta = array();
	
	if( @$arrTiposdePropostas[ 'inclusao' ] )
	{
		$arrTiposBancoProposta[] = '\'I\'';
	}
	if( @$arrTiposdePropostas[ 'alteracao' ] )
	{
		$arrTiposBancoProposta[] = '\'A\'';
	}
	if( @$arrTiposdePropostas[ 'exclusao' ] )
	{
		$arrTiposBancoProposta[] = '\'E\'';
	}
	if( @$arrTiposdePropostas[ 'fusao' ] )
	{
		$arrTiposBancoProposta[] = '\'F\'';
	}
	if( @$arrTiposdePropostas[ 'migracao' ] )
	{
		$arrTiposBancoProposta[] = '\'M\'';
	}
	
	if( sizeof( $arrTiposBancoProposta ) > 0 )
	{
		$strWhereAppend = '' .
			$strNomeTabela . '.' . 'tipo' . 
			' IN ' .
			' ( ' . 
				implode( ',' , $arrTiposBancoProposta ) . 
			' ) ';
	}
	else
	{
		$strWhereAppend = '';
	}	
	
	return $strWhereAppend;
}

/**
 * No filtro por programa:
 * no codigo devera aparecer o codigo do programa 
 * ordenados pelo codigo do programa seguido do nome do programa.
 * 
 * Na expansao irá mostrar todas as propostas daquele programa dentro do filtro 
 * crud ordenadas
 * por tipo.
 * 
 * @param string	$strAgrupadoPor
 * @param integer	$intAnoExercicio
 * @param string	$strOrdemGrupo
 * @param integer	$intPosicaodaColunaOrdenada
 * @param string	$strDirecaoDaOrdenacao
 * @param array		$arrTiposdePropostas
 * @return array
 */
function geraArrayDePropostasAgrupadasPelosProgramas( $strAgrupadoPor , $intAnoExercicio, $strOrdemGrupo , 
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas )
{
	
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'programa_codigo' . 
		' , ' . 
		'prgograma_descricao' . 
		' , ' .
		'programa_id' .
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_por_tipo' . ' ) ' . ' AS ' . 'quantidade_de_propostas' . 
		' , ' .		
		' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' . ' AS ' . 'quantidade_alteracao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' . ' AS ' . 'quantidade_inclusao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' . ' AS ' . 'quantidade_exclusao' . 
	' FROM ' .
		' ( ' .
		' SELECT ' .
			'programa_id' .
			' , ' .
			'Programas.prgcod' . ' AS ' . 'programa_codigo' .
			' , ' .
			'Programas.prgdsc' . ' AS ' . 'prgograma_descricao' .
			' ,  ' .
			'tipo' .
			' , ' .
			'COUNT( '. 'programa_id' . ' ) ' . ' AS ' . 'quantidade_por_tipo' .
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' . ' AS ' . 'quantidade_alteracao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' . ' AS ' . 'quantidade_inclusao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' . ' AS ' . 'quantidade_exclusao' . 
			
		 ' FROM ' .
			' ( ' .
			// nao existe tabela de fusão de programas //
			// nao existe tabela de migração de programas //
			// alteração //
			' SELECT ' . 
				'eracod' . ' AS ' . 'programa_id' .
				' , ' . 
				'\'A\'' . ' AS ' . 'tipo' .
				' , ' . 
				'usucpf' . ' AS ' . 'usuario_cpf' .
				' , ' .
		 		'1' . ' AS ' . 'quantidade_alteracao' .
		 		' , ' .
		 		'0' . ' AS ' . 'quantidade_inclusao' .
		 		' , ' .
		 		'0' . ' AS ' . 'quantidade_exclusao' .
			' FROM ' . 
				'elabrev.elaboracaorevisao' . 
			' WHERE ' .
				'eratabela' . ' = ' . '\'ppaprograma_proposta\'' .
			' GROUP ' . ' BY ' .
				'programa_id' .
				' , ' .
				'usuario_cpf' .
				' , ' .
				'tipo' .
			' UNION ' . ' ALL ' .
			// inclusão //
			' SELECT ' .
				'prgid' . ' AS ' . 'programa_id' .
				' , ' . 
				'\'I\'' . ' AS ' . 'tipo' .
				' , ' . 
				'usucpf' . ' AS ' . 'usuario_cpf' .
				' , ' .
		 		'0' . ' AS ' . 'quantidade_alteracao' .
		 		' , ' .
		 		'1' . ' AS ' . 'quantidade_inclusao' .
		 		' , ' .
		 		'0' . ' AS ' . 'quantidade_exclusao' .
			' FROM ' .
				'elabrev.ppaprograma_proposta' . 
			' WHERE ' .
				'prgstatus' . ' = ' . '\'' . 'N' . '\'' .
			' GROUP ' . ' BY ' .
				'programa_id' .
				' , ' .
				'usuario_cpf' .
				' , ' .
				'tipo' .
			' ) ' .
		' AS ' .
			'Propostas' .
		' LEFT ' . ' JOIN  ' .
			'elabrev.ppaprograma_proposta' . ' AS ' . 'Programas' .
		' ON ' . 
			'Programas.prgid' . ' = ' . 'Propostas.programa_id' .
		' WHERE ' .
			'Programas.prsano' . ' = ' . $intAnoExercicio .
			$strWhereAppend .
		' GROUP BY '.
			'programa_id' .
			' , ' .
			'tipo' . 
			' , ' .
			'prgograma_descricao' . 
			' , ' .
			'programa_codigo' .
			' , ' .
			'usuario_cpf' .
		' ) ' .
	' AS ' . 'PropostasPorTipo' .
	' GROUP ' . ' BY ' .
		'programa_id' .
		' , ' .
		'programa_codigo' . 
		' , ' .
		'prgograma_descricao' .
	' ORDER BY ' .
		'programa_codigo ' .
	'';	
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$strCodigoPrograma	= $objRegistroAtual[ 'programa_codigo' ];
		$strDescricao		= $objRegistroAtual[ 'prgograma_descricao' ];
		$intQtdPropostas	= $objRegistroAtual[ 'quantidade_de_propostas' ];
		$intProgramaId		= $objRegistroAtual[ 'programa_id' ];
		$intQtdMigracao		= $objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= $objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= $objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= $objRegistroAtual[ 'quantidade_exclusao' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoPrograma , $intLinhaAtual , $intProgramaId , 0 );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoPrograma;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoPrograma );
		$arrProposta[ 'codigo_programa']	= $strCodigoPrograma;
		$arrProposta[ 'descricao' ]			= $strDescricao;
		$arrProposta[ 'quantidade' ]		= $intQtdPropostas;
		$arrProposta[ 'qtd_fusao' ]			= 0;
		$arrProposta[ 'qtd_migracao' ]		= 0;
		$arrProposta[ 'qtd_alteracao' ]		= $intQtdAlteracao;
		$arrProposta[ 'qtd_inclusao' ]		= $intQtdInclusao;
		$arrProposta[ 'qtd_exclusao' ]		= $intQtdExclusao;
		
		$arrPropostas[] = $arrProposta;
	}
	return $arrPropostas;
}

/**
 * No filtro por acao:
 * no codigo devera aparecer o codigo do programa e da acao 
 * ordenados pelo codigo da acao seguido do nome da acao.
 * 
 * Na expansao irá mostrar todas as propostas daquela acao dentro do filtro 
 * crud ordenadas
 * por tipo.
 * 
 * @param string	$strAgrupadoPor
 * @param integer	$intAnoExercicio
 * @param string	$strOrdemGrupo
 * @param integer	$intPosicaodaColunaOrdenada
 * @param string	$strDirecaoDaOrdenacao
 * @param array		$arrTiposdePropostas
 * @return array
 */
function geraArrayDePropostasAgrupadasPelaAcao( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , 
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas )
{
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'acao_codigo' . 
		' , ' . 
		'programa_codigo' . 
		' , ' . 
		'acao_dsc' . 
		' , ' . 
		'acao_id' . 
		' , ' .
		'programa_id' .
		' , ' .		
		' SUM ' . ' ( ' . 'quantidade_por_tipo' . ' ) ' . ' AS ' . 'quantidade_de_propostas' . 
		' , ' .		
		' SUM ' . ' ( ' . 'quantidade_fusao' . ' ) ' . ' AS ' . 'quantidade_fusao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_migracao' . ' ) ' . ' AS ' . 'quantidade_migracao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' . ' AS ' . 'quantidade_alteracao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' . ' AS ' . 'quantidade_inclusao' . 
		' , ' .
		' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' . ' AS ' . 'quantidade_exclusao' . 
	' FROM ' .
		' ( ' .
		' SELECT ' .
			'acao_id' .
			' , ' .
			'Acoes.acacod' . ' AS ' . 'acao_codigo' .
			' , ' .
			'Acoes.acadsc' . ' AS ' . 'acao_dsc' .
			' ,  '.
			'Acoes.prgcod' . ' AS ' . 'programa_codigo' . 
			' ,  ' .
			'Acoes.prgid' . ' AS ' . 'programa_id' . 
			' ,  ' .
			'tipo' .
			' , ' .
			'COUNT( '. 'acao_id' . ' ) ' . ' AS ' . 'quantidade_por_tipo' .
			' , ' .		
			' SUM ' . ' ( ' . 'quantidade_fusao' . ' ) ' . ' AS ' . 'quantidade_fusao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_migracao' . ' ) ' . ' AS ' . 'quantidade_migracao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_alteracao' . ' ) ' . ' AS ' . 'quantidade_alteracao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_inclusao' . ' ) ' . ' AS ' . 'quantidade_inclusao' . 
			' , ' .
			' SUM ' . ' ( ' . 'quantidade_exclusao' . ' ) ' . ' AS ' . 'quantidade_exclusao' . 
		 ' FROM ' .
			' ( ' .
// fusão //
				' SELECT ' .
					'acaid' . ' AS ' . 'acao_id' .
					' , ' .
					'\'F\'' . ' AS ' . 'tipo' .
					' , ' .
			 		'usucpf' . ' AS ' . 'usuario_cpf' .
					' , ' .
			 		'1' . ' AS ' . 'quantidade_fusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_migracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_alteracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_inclusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_exclusao' .
				' FROM ' . 
					'elabrev.proposta_fusao_acao' . 
				' GROUP ' . ' BY ' .
					'acao_id' .
					' , ' .
					'usuario_cpf' .
					' , ' .
					'tipo' .
				' UNION ' . ' ALL ' .
// migração //
				' SELECT ' . 
					'acaid' . ' AS ' . 'acao_id' . 
					' , '.
					'\'M\'' . ' AS ' . 'tipo' .  
					' , ' .
					'usucpf' . ' AS ' . 'usuario_cpf' .
					' , ' .
			 		'0' . ' AS ' . 'quantidade_fusao' .
			 		' , ' .
			 		'1' . ' AS ' . 'quantidade_migracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_alteracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_inclusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_exclusao' .
				' FROM ' . 
					'elabrev.proposta_migracao_acao' .  
				' GROUP ' . ' BY ' .
					'acao_id' .
					' , ' .
					'usuario_cpf' .
					' , ' .
					'tipo' .
				' UNION ' . ' ALL ' .
// alteração //
				' SELECT ' . 
					'eracod' . ' AS ' . 'acao_id' . 
					' , ' .
			 		'\'A\'' . ' AS ' . 'tipo' .
					' , ' .
			 		'usucpf' . ' AS ' . 'usuario_cpf' . 
					' , ' .
			 		'0' . ' AS ' . 'quantidade_fusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_migracao' .
			 		' , ' .
			 		'1' . ' AS ' . 'quantidade_alteracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_inclusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_exclusao' .
				' FROM ' . 
					'elabrev.elaboracaorevisao' . 
				' WHERE ' .
					'eratabela' . ' = ' . '\'ppaacao_proposta\'' .
				' GROUP ' . ' BY ' .
					'acao_id' .
					' , ' .
					'usuario_cpf' .
					' , ' .
					'tipo' .
				' UNION ' . ' ALL ' .
// inclusão //
				' SELECT ' .
				
					'acaid' . ' AS ' . 'acao_id' . 
					' , '.
					'\'I\'' . ' AS ' . 'tipo' .  
					' , ' .
					'usucpf' . ' AS ' . 'usuario_cpf' .
					' , ' .
			 		'0' . ' AS ' . 'quantidade_fusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_migracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_alteracao' .
			 		' , ' .
			 		'1' . ' AS ' . 'quantidade_inclusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_exclusao' .
				' FROM ' .
					'elabrev.ppaacao_proposta' . 
				' WHERE ' .
					'acastatus' . ' = ' . '\'' . 'N' . '\'' .
				' GROUP ' . ' BY ' .
					'acao_id' .
					' , ' .
					'usuario_cpf' .
					' , ' .
					'tipo' .
				' UNION ' . ' ALL ' .
// exclusão //
				' SELECT ' .
					'acaid' . ' AS ' . 'acao_id' . 
					' , '.
					'\'E\'' . ' AS ' . 'tipo' .  
					' , ' .
					'usucpf' . ' AS ' . 'usuario_cpf' .
					' , ' .
			 		'0' . ' AS ' . 'quantidade_fusao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_migracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_alteracao' .
			 		' , ' .
			 		'0' . ' AS ' . 'quantidade_inclusao' .
			 		' , ' .
			 		'1' . ' AS ' . 'quantidade_exclusao' .
				' FROM ' .
					'elabrev.proposta_exclusao_acao' .
			 	' GROUP ' . ' BY ' .
			 		'acao_id' .
					' , ' .
					'usuario_cpf' .
					' , ' .
					'tipo' . 
			' ) ' .
		' AS ' .
			'Propostas' .
		' LEFT ' . ' JOIN  ' .
			'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' .
		' ON ' . 
			'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
		' WHERE ' .
			'Acoes.prsano' . ' = ' . $intAnoExercicio .
			$strWhereAppend .
		' GROUP BY '.
			'acao_id' . 
			' , ' .
			'programa_id' .
			' , ' .
			'tipo' . 
			' , ' .
			'acao_dsc' . 
			' , ' .
			'acao_codigo' . 
			' , ' .
			'programa_codigo' .
			' , ' .
			'usuario_cpf' .
		' ) ' .
	' AS ' . 'PropostasPorTipo' .
	' GROUP ' . ' BY ' .
		'acao_id' . 
		' , ' .
		'acao_codigo' . 
		' , ' .
		'programa_codigo' . 
		' , ' .
		'acao_dsc' .
		' , ' .
		'programa_id' .
	' ORDER BY ' .
		'programa_codigo ' .
		' , ' .
		'acao_codigo' .
		' , ' .
		'programa_id' .
	'';	
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$strCodigoPrograma	= $objRegistroAtual[ 'programa_codigo' ];
		$strCodigoAcao		= $objRegistroAtual[ 'acao_codigo' ];
		$strDescricao		= $objRegistroAtual[ 'acao_dsc' ];
		$intQtdPropostas	= $objRegistroAtual[ 'quantidade_de_propostas' ];
		$intQtdFusao		= $objRegistroAtual[ 'quantidade_fusao' ];
		$intQtdMigracao		= $objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= $objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= $objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= $objRegistroAtual[ 'quantidade_exclusao' ];
		$intAcaoId			= $objRegistroAtual[ 'acao_id' ];
		$intProgramaId		= $objRegistroAtual[ 'programa_id' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoAcao , $intLinhaAtual , $intProgramaId , $intAcaoId );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoAcao;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoAcao );
		$arrProposta[ 'codigo_programa']	= $strCodigoPrograma;
		$arrProposta[ 'codigo_acao']		= $strCodigoAcao;
		$arrProposta[ 'descricao' ]			= $strDescricao;
		$arrProposta[ 'quantidade' ]		= $intQtdPropostas;
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		$arrProposta[ 'qtd_migracao' ]		= $intQtdMigracao;
		$arrProposta[ 'qtd_alteracao' ]		= $intQtdAlteracao;
		$arrProposta[ 'qtd_inclusao' ]		= $intQtdInclusao;
		$arrProposta[ 'qtd_exclusao' ]		= $intQtdExclusao;
		
		$arrPropostas[] = $arrProposta;
	}
	return $arrPropostas;
}

function geraArrayDePropostas( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , $intPosicaodaColunaOrdenada , 
	$strDirecaoDaOrdenacao , $arrTiposdePropostas )
{
	$arrPropostas = array();
	
	switch( $strAgrupadoPor )
	{
		case 'Acoes':
		{
			$arrPropostas = geraArrayDePropostasAgrupadasPelaAcao( $strAgrupadoPor , $intAnoExercicio , 
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas );
			break;
		}
		case 'Programas':
		{
			$arrPropostas = geraArrayDePropostasAgrupadasPelosProgramas( $strAgrupadoPor , $intAnoExercicio ,
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas);
			break;
		}
		case 'Unidades':
		{
			$arrPropostas = geraArrayDePropostasAgrupadasPelasUnidades( $strAgrupadoPor , $intAnoExercicio ,
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas);
			break;
		}
	}
	return $arrPropostas;
}

function geraTitulosPeloAgrupamento( $strAgrupadoPor , &$strTitulo , &$strTituloTotal , &$strColunaTotal )
{
	switch( $strAgrupadoPor )
	{
		case 'Acoes':
		{
			$strTitulo 		= 'da Ação';
			$strTituloTotal = 'de Ações';
			$strColunaTotal	= 'Quantidade';	
			break;
		}
		case 'Programas':
		{
			$strTitulo 		= 'de Programas';
			$strTituloTotal = 'do Programa';
			$strColunaTotal	= 'Tipo';
			break;
		}
		case 'Unidades':
		{
			$strTitulo 		= 'de Unidades'; 
			$strTituloTotal = 'da Unidade';
			$strColunaTotal	= 'Tipo';
			break;
		}
	}
}

function geraImagemDaDirecaoDaOrdenacao( $intPosicaoDaColuna , $intPosicaodaColunaAtualmenteOrdenada , 
	$strDirecaoDaOrdenacao , $strTituloNome )
{
	switch( $strTituloNome )
	{
		case 'Tipo':
		case 'Quantidade':
		{
			$strTitle = 'Ordenar por Total';
			break;
		}
		default:
		{
			$strTitle = 'Ordenar por ' . $strTituloNome;
			break;
		}
	}
	
	if( $intPosicaoDaColuna == $intPosicaodaColunaAtualmenteOrdenada )
	{
		$strDirecaoDaOrdenacaoInvertida = ( $strDirecaoDaOrdenacao == 'ASC' ) ? 'DESC' : 'ASC';
		?>
		
		<img  width="11" height="13" align="middle"
		src="../imagens/seta_ordem<?= $strDirecaoDaOrdenacao ?>.gif" />
		
		<label 
		onclick="ordena(<?= $intPosicaoDaColuna?> , '<?= $strDirecaoDaOrdenacaoInvertida ?>' )" 
		title="<?= $strTitle ?>" >
			<strong>
				<?= $strTituloNome ?>
			</strong>
		</label>
		
		<?
	}
	else
	{
		?>
		
		<label 
		onclick="ordena(<?= $intPosicaoDaColuna?> , 'ASC' )"  title="<?= $strTitle ?>" >
			<strong>
				<?= $strTituloNome ?>
			</strong>
		</label>
		
		<?
	}
}

function geraRelatorioPorTipodeProposta( $intPosicaodaColunaOrdenada = 1 , $strDirecaoDaOrdenacao = 'ASC' , 
	$strOrdemGrupo , $arrTiposdePropostas , $strAgrupadoPor , $intAnoExercicio)
{
	//			$db->cria_aba($abacod_tela,$url,'');

	$strTitulo					= '';
	$strTituloTotal 			= '';
	$strCampoOrdenarPeloTotal	= '';
	
	geraTitulosPeloAgrupamento( $strAgrupadoPor , $strTitulo , $strTituloTotal, $strCampoOrdenarPeloTotal );
	
//	dbg( $intAnoExercicio , 1 );
	
	$arrElements =  geraArrayDePropostas( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , $intPosicaodaColunaOrdenada , 
	$strDirecaoDaOrdenacao , $arrTiposdePropostas );
	
	?>
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" 
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
		<!-- cabecalho da listagem -->
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<!-- codigo -->
					<td style="width:65px;" nowrap class="title">
						<? geraImagemDaDirecaoDaOrdenacao( 1 , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , 'Código' ); ?>
					</td>
					<!-- titulo -->
					<td class="title">
						<? geraImagemDaDirecaoDaOrdenacao( 2 , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , 'Título ' . $strTitulo ); ?>
					</td>
					<!-- total -->
					<? // geraImagemDaDirecaoDaOrdenacao( 3 , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $strCampoOrdenarPeloTotal ); ?>
					<td class="title" align="center" style="width: 15px" >
						<img src="../imagens/barras/BarraInclusoes2.PNG" />
					</td>
					<td class="title" align="center" style="width: 15px" >
						<img src="../imagens/barras/BarraAlteracoes2.PNG" />
					</td>
					<td class="title" align="center" style="width: 15px" >
						<img src="../imagens/barras/BarraMigracoes2.PNG" />
					</td>
					<td class="title" align="center" style="width: 15px" >
						<img src="../imagens/barras/BarraFusoes2.PNG" />
					</td>
					<td class="title" align="center" style="width: 15px" >
						<img src="../imagens/barras/BarraExclusoes2.PNG" />
					</td>
					<td class="title" align="center" style="width: 15px" >
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
							style="width:65px;" onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td>
									<!-- codigo da acao -->
									<font title="Código do Programa" ><?= $arrElement[ 'codigo_programa' ] ?></font>.<font title="Código da Ação" ><?= $arrElement[ 'codigo_acao' ] ?>
								</td>
								<td style="color:#003c7b" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome da acao -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td style="width: 15px" >
									<!-- Inclusoes -->
									<font color="maroon">
										<?=  $arrElement[ 'qtd_inclusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Alteracoes -->
									<font color="blue">
										<?=  $arrElement[ 'qtd_alteracao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td style="width: 15px" >
									<!-- Fusoes -->
									<font color="green">
										<?=  $arrElement[ 'qtd_fusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Exclusoes -->
									<font color="red">
										<?=  $arrElement[ 'qtd_exclusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" align=right>
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
								<td>
									<!-- codigo do programa -->
									<font title="Código do Programa" >
										<?= $arrElement[ 'codigo_programa' ] ?>
									</font>
								</td>
								<td style="color:#003c7b" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome do programa -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td>
									<!-- Inclusoes -->
									<font color="maroon">
										<?=  $arrElement[ 'qtd_inclusao' ]  ?>
									</font>
								</td>
								<td>
									<!-- Alteracoes -->
									<font color="blue">
										<?=  $arrElement[ 'qtd_alteracao' ]  ?>
									</font>
								</td>
								<td>
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td>
									<!-- Fusoes -->
									<font color="green">
										<?=  $arrElement[ 'qtd_fusao' ]  ?>
									</font>
								</td>
								<td>
									<!-- Exclusoes -->
									<font color="red">
										<?=  $arrElement[ 'qtd_exclusao' ]  ?>
									</font>
								</td>
								<td align=right>
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
							style="width:65px;" onmouseout="this.bgColor='<?= $strStyleMarcado ?>';" >
								<td>
									<!-- codigo da unidade -->
									<font title="Código da Unidade" ><?= $arrElement[ 'codigo_unidade' ] ?></font>
								</td>
								<td style="color:#003c7b" onclick="abreconteudo(<?= $arrElement[ 'linkAbreConteudo' ] ?>)" > 
									<!-- expansor -->
									<img src="../imagens/mais.gif" name="+" border="0" id="<?= $arrElement[ 'imgId' ] ?>" />
									<!-- nome da unidade -->
									<?= $arrElement[ 'descricao' ] ?>
								</td>
								<td style="width: 15px" >
									<!-- Inclusoes -->
									<font color="maroon">
										<?=  $arrElement[ 'qtd_inclusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Alteracoes -->
									<font color="blue">
										<?=  $arrElement[ 'qtd_alteracao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Migracoes -->
									<?=  $arrElement[ 'qtd_migracao' ]  ?>
								</td>
								<td style="width: 15px" >
									<!-- Fusoes -->
									<font color="green">
										<?=  $arrElement[ 'qtd_fusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" >
									<!-- Exclusoes -->
									<font color="red">
										<?=  $arrElement[ 'qtd_exclusao' ]  ?>
									</font>
								</td>
								<td style="width: 15px" align=right>
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
				
//				dbg( $strAgrupadoPor , 1 );
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
								<td valign="top" class="title" colspan="3"> 
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
		<script>
		/*
		abreconteudo = function abreconteudo(url, idobj)
		{
			var jsNode = document.getElementById('td'+idobj);
			
			if (document.getElementById('img'+idobj).name=='+')
		 	{
				if (document.getElementById('td'+idobj).innerHTML=='')
				{
					alert('escreveu');
					document.getElementById('td'+idobj).style.visibility = "visible";
					document.getElementById('td'+idobj).style.display = "";
					document.getElementById('td'+idobj).innerHTML='<div align=center><img src="../imagens/wait.gif" border="0" align="middle"> <font color=blue>Carregando Dados...</font></div>'
		
					jsNode._xmlHttp = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject("Microsoft.XmlHttp");
					
					document.body.innerHTML += url;
					
					jsNode._xmlHttp.open("GET", url, true);
					jsNode._xmlHttp.onreadystatechange = new Function("func_onload(\"" + idobj + "\")");
		
					// call in new thread to allow ui to update
					window.setTimeout("func_ontimeout(\"" + idobj + "\")", 300);
		
					document.getElementById('img'+idobj).name='-';
					document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
		 		}
			 	else
			 	{
					alert('apagou');
					document.getElementById('img'+idobj).name='-';
					document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
					document.getElementById('td'+idobj).style.visibility = "visible";
					document.getElementById('td'+idobj).style.display = "";
				}
			}
			else
			{
				document.getElementById('img'+idobj).name='+';
				document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('menos.gif', 'mais.gif');
				document.getElementById('td'+idobj).style.visibility = "hidden";
				document.getElementById('td'+idobj).style.display = "none";
			}
		};
		*/		
	</script>
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
	<!-- 
				</td>
			</tr>
		</table>
	</center>
	 -->
	</div>
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
	<?
}

// chamada padrão ao cabecalho do sistema //
include  APPRAIZ . "includes/cabecalho.inc";

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