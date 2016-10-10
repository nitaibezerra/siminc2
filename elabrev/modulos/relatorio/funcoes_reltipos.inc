<?

/**
 * Apenas uma funcao de debug para facilitar a visualizacao do resultado de uma query
 */
function mostraResultadoDaQuery( $strSql )
{
	global $db;

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

/**
 * Gera a marca html de checage de um checkbox conforme a entrada
 *
 * @param bool $boolElement
 * @return string
 */
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

/**
 * Geracao do Link do RelTiposAjax Conforme os parametros enviados
 * @param string $strAgrupadoPor
 * @param string $strOrdemGrupo
 * @param array $arrTiposdePropostas
 * @param string $strCodigo
 * @param integer $intContador
 * @param integer $intProgramaId
 * @param integer $intAcaoId
 * @param integer $intUnidadeId
 */
function geraLink( $strAgrupadoPor , $strOrdemGrupo = '' , $arrTiposdePropostas ,  $strCodigo = '' , 
	$intContador , $intProgramaId = 0, $intAcaoId = 0, $intUnidadeId = 0 , $boolIncluiAgrupadoresVazios = false )
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
			'&' . 'strAgrupadoPor'					. '=' . 'PropostasDasAcoes' .
			'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
			'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
			'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
			'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
			'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
			'&' . 'boolIncluiAgrupadoresVazios'		. '=' . (integer) $boolIncluiAgrupadoresVazios .
			'\'' . ',' . '\'' . $intContador . '-' . $strCodigo . '\'';

			break;
		}
		case 'Programas':
		{
			
			$strLink = 
			'\'' .
			'geral/reltipos_ajax.php?' . 
			'&' . 'intProgramaId'					. '=' . $intProgramaId .
			'&' . 'strAgrupadoPor'					. '=' . 'AcoesDosProgramas' .
			'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
			'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
			'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
			'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
			'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
			'&' . 'boolIncluiAgrupadoresVazios'		. '=' . (integer) $boolIncluiAgrupadoresVazios .
			'\'' . ',' . '\'' . $intContador . '-' . $strCodigo . '\'';
			
			//$strLink = $_SESSION['sisdiretorio'] . 'php?modulo=relatorio/relprogramanovo&acao=A&prgid=' . $intProgramaId;
			break;
		}
		case 'Unidades':
		{
			
			$strLink = 
			'\'' .
			'geral/reltipos_ajax.php?' . 
			'&' . 'intUnidadeId'					. '=' . $intUnidadeId .
			'&' . 'strCodigoUnidade'				. '=' . $strCodigo .
			'&' . 'strAgrupadoPor'					. '=' . 'AcoesDasUnidades' .
			'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
			'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
			'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
			'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
			'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
			'&' . 'boolIncluiAgrupadoresVazios'		. '=' . (integer) $boolIncluiAgrupadoresVazios .
			'\'' . ',' . '\'' . $intContador . '-' . $strCodigo . '\'';

			break;
		}
	}

	return $strLink;
}

/**
 * Gera o identificador html da Imagem para que as funcoes que fazem uso do ajax funcionem corretamente
 *
 * @param integer $intLinhaAtual
 * @param string $strCodigo
 * @return string
 */
function geraImagemId( $intLinhaAtual , $strCodigo )
{
	return 'img' . $intLinhaAtual . '-' . $strCodigo;
}

/**
 * Converte do Formato reduzido no completo e vice-versa conforme os parametros
 * 
 * @param string $strTipo
 * @param bool $boolBancoParaTexto
 */
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
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiUnidadesSemPropostas = false )
{
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}
	
	$strSql = '';
	
	
	if( $boolIncluiUnidadesSemPropostas )
	{
		$strSql = '' .
		' SELECT ' .
			'Unidade.unicod' .	' AS ' . 'unidade_codigo'		. ' , ' . 
			'Unidade.unidsc' .	' AS ' . 'unidade_descricao'	. ' , ' . 
			' SUM( ' . ' coalesce( ' . 'quantidade_de_propostas' . ' , ' . '0' . ' ) ' . ' ) ' .
			' , ' .   
			' SUM( ' . ' coalesce( ' . 'quantidade_fusao' 		. ' , ' . '0' . ' ) ' . ' ) ' .
			' , ' .   
			' SUM( ' . ' coalesce( ' . 'quantidade_migracao' 	. ' , ' . '0' . ' ) ' . ' ) ' .
			' , ' .   
			' SUM( ' . ' coalesce( ' . 'quantidade_alteracao' 	. ' , ' . '0' . ' ) ' . ' ) ' .
			' , ' .   
			' SUM( ' . ' coalesce( ' . 'quantidade_inclusao' 	. ' , ' . '0' . ' ) ' . ' ) ' .
			' , ' .   
			' SUM( ' . ' coalesce( ' . 'quantidade_exclusao' 	. ' , ' . '0' . ' ) ' . ' ) ' .  
		' FROM ' .
			'public.unidade' . ' AS ' . 'Unidade' .
		' LEFT OUTER JOIN ' .
			' ( ' . 
		'';
	}	
	
	$strSql .= '' .
	' SELECT ' .
		'Unidade.unicod' . ' AS ' . 'unidade_codigo'	. 
		' , ' . 
		'Unidade.unidsc' . ' AS ' . 'unidade_descricao' .
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
				'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
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
		'Unidade.unicod' .
		' , ' .
		'Unidade.unidsc' .
	' ORDER BY ' .
		'Unidade.unicod' .
	'';	
	
	if( $boolIncluiUnidadesSemPropostas )
	{
		$strSql .= " ) AS UnidadesComPropostas
			ON
				UnidadesComPropostas.unidade_codigo = Unidade.unicod
			WHERE
				Unidade.orgcod = '". CODIGO_ORGAO_SISTEMA. "'
			GROUP BY
				Unidade.unicod 
				,Unidade.unidsc
			ORDER BY
				Unidade.unicod
			";
	}
	
			
	$_SESSION[ "debugger" ] = $strSql ;
	
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$intUnidadeId		= $objRegistroAtual[ 'unidade_id' ];
		$strCodigoUnidade	= $objRegistroAtual[ 'unidade_codigo' ];
		$strDescricao		= $objRegistroAtual[ 'unidade_descricao' ];
		$intQtdPropostas	= (integer)@$objRegistroAtual[ 'quantidade_de_propostas' ];
		$intQtdFusao		= (integer)@$objRegistroAtual[ 'quantidade_fusao' ];
		$intQtdMigracao		= (integer)@$objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= (integer)@$objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= (integer)@$objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= (integer)@$objRegistroAtual[ 'quantidade_exclusao' ];
		$intQtdFusao		= (integer)@$objRegistroAtual[ 'quantidade_fusao' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoUnidade , $intLinhaAtual , 0 , 0 , $intUnidadeId , $boolIncluiUnidadesSemPropostas );
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
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		
		$arrPropostas[] = $arrProposta;
	}
	return $arrPropostas;
}

/**
 * Monta o trecho do SQL dos Filtros por Tipo de Proposta conforme o tipo de proposta
 * 
 * @param array $arrTiposdeProposta
 * @param string $strNomeTabela
 */
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
function geraArrayDePropostasdeAcaoAgrupadasPelosProgramas( $strAgrupadoPor , $intAnoExercicio, $strOrdemGrupo , 
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiProgramasSemProposta = false )
{
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}

	if( $boolIncluiProgramasSemProposta )
	{
		$strSql = 	' SELECT ' .
		'Programas.prgid' . ' AS ' . 'programa_id'  .
		' , ' .
		'Programas.prgcod' . ' AS ' . 'programa_codigo' .
		' , ' .
		'Programas.prgdsc' . ' AS ' . 'prgograma_descricao' .
		' , ' .
		'BuscaRestrita.quantidade_de_propostas' .
		' , ' .
		'BuscaRestrita.quantidade_fusao ' .
		' , ' .
		'BuscaRestrita.quantidade_migracao ' .
		' , ' .
		'BuscaRestrita.quantidade_alteracao ' .
		' , ' .
		'BuscaRestrita.quantidade_inclusao ' .
		' , ' .
		'BuscaRestrita.quantidade_exclusao ' .
		' FROM ' .
	 	'elabrev.ppaprograma' . ' AS ' . 'Programas' . 
		' LEFT OUTER JOIN ( ';
	}
		
	$strSql .= '' .
	' SELECT ' .
	
		'programa_id' .
		' , ' .
		'Programas.prgcod' . ' AS ' . 'programa_codigo' .
		' , ' .
		'Programas.prgdsc' . ' AS ' . 'prgograma_descricao' .
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
				'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
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
	' LEFT OUTER JOIN '  .
		'elabrev.ppaprograma' . ' AS ' . 'Programas' .
	' ON ' .
		'Programas.prgid' . ' = ' . 'PropostasPorTipo.programa_id' .
	' GROUP BY ' .
		'Programas.prgcod' .
		' , ' .
		'Programas.prgdsc' .
		' , ' .
		'PropostasPorTipo.programa_id' .
	' ORDER BY ' .
		'Programas.prgcod' .
	'';	

	if( $boolIncluiProgramasSemProposta )
	{
		$strSql .= ' ) AS BuscaRestrita
			 ON BuscaRestrita.programa_id =  Programas.prgid where Programas.prsano::INTEGER = '. $intAnoExercicio;
	}
 	$_SESSION[ "debugger" ] = $strSql ;
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$strCodigoPrograma	= @$objRegistroAtual[ 'programa_codigo' ];
		$strDescricao		= @$objRegistroAtual[ 'prgograma_descricao' ];
		$intQtdPropostas	= (integer)@$objRegistroAtual[ 'quantidade_de_propostas' ];
		$intProgramaId		= (integer)@$objRegistroAtual[ 'programa_id' ];
		$intQtdMigracao		= (integer)@$objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= (integer)@$objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= (integer)@$objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= (integer)@$objRegistroAtual[ 'quantidade_exclusao' ];
		$intQtdFusao		= (integer)@$objRegistroAtual[ 'quantidade_fusao' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoPrograma , $intLinhaAtual , $intProgramaId , 0 , 0 , $boolIncluiProgramasSemProposta );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoPrograma;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoPrograma );
		$arrProposta[ 'codigo_programa']	= $strCodigoPrograma;
		$arrProposta[ 'descricao' ]			= $strDescricao;
		$arrProposta[ 'quantidade' ]		= $intQtdPropostas;
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		$arrProposta[ 'qtd_migracao' ]		= $intQtdMigracao;
		$arrProposta[ 'qtd_alteracao' ]		= $intQtdAlteracao;
		$arrProposta[ 'qtd_inclusao' ]		= $intQtdInclusao;
		$arrProposta[ 'qtd_exclusao' ]		= $intQtdExclusao;
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		$arrProposta[ 'programa_id' ]		= $intProgramaId;
		
		$arrPropostas[] = $arrProposta;
	}
	return $arrPropostas;
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
function geraArrayDePropostasdeProgramaAgrupadasPelosProgramas( $strAgrupadoPor , $intAnoExercicio, $strOrdemGrupo , 
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
		$intQtdFusao		= $objRegistroAtual[ 'quantidade_fusao' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoPrograma , $intLinhaAtual , $intProgramaId , 0 , 0 , $boolIncluiProgramasSemProposta );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoPrograma;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoPrograma );
		$arrProposta[ 'codigo_programa']	= $strCodigoPrograma;
		$arrProposta[ 'descricao' ]			= $strDescricao;
		$arrProposta[ 'quantidade' ]		= $intQtdPropostas;
		$arrProposta[ 'qtd_migracao' ]		= $intQtdMigracao;
		$arrProposta[ 'qtd_alteracao' ]		= $intQtdAlteracao;
		$arrProposta[ 'qtd_inclusao' ]		= $intQtdInclusao;
		$arrProposta[ 'qtd_exclusao' ]		= $intQtdExclusao;
		$arrProposta[ 'qtd_fusao' ]			= $intQtdFusao;
		
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
function geraArrayDePropostasdeAcaoAgrupadasPelaAcao( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , 
	$intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiAcoesSemProposta = false )
{
	global $db;
	
	$strWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	if( $strWhereAppend != '' )
	{
		$strWhereAppend = ' AND ' . $strWhereAppend;
	}
	
	$strSql = '';
	
	if( $boolIncluiAcoesSemProposta )
	{
		$strSql .= 	' SELECT ' .
			'Acoes.acacod'	.	' AS ' . 'acao_codigo' . 
			' , ' . 
			'Acoes.acaid'	.	' AS ' . 'acao_id'  . 
			' , ' .
			'Acoes.acadsc'	.	' AS ' . 'acao_descricao'  . 
			' , ' .  
			'Acoes.prgcod'	.	' AS ' . 'programa_codigo' .
			' , ' .
			'Acoes.prgid'	.	' AS ' . 'programa_id' . 
			' , ' .
			'quantidade_de_propostas' . 
			' , ' .
			'quantidade_fusao' . 
			' , ' .
			'quantidade_migracao' . 
			' , ' .
			'quantidade_alteracao' . 
			' , ' .
			'quantidade_inclusao' . 
			' , ' .
			'quantidade_exclusao' .
		' FROM ' .
			'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' . 
		' LEFT OUTER JOIN ' .
		' ( ';
	}
		
	$strSql .= '' .
	' SELECT ' .
		'acao_codigo' . 
		' , ' . 
		'programa_codigo' . 
		' , ' . 
		'acao_descricao' . 
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
			'Acoes.acadsc' . ' AS ' . 'acao_descricao' .
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
			'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
			$strWhereAppend .
		' GROUP BY '.
			'acao_id' . 
			' , ' .
			'programa_id' .
			' , ' .
			'tipo' . 
			' , ' .
			'acao_descricao' . 
			' , ' .
			'acao_codigo' . 
			' , ' .
			'programa_codigo' .
			' , ' .
			'usuario_cpf' .
		' ) ' .
	' AS ' . 'PropostasPorTipo' .
	'';
		
	if( $boolIncluiAcoesSemProposta )
	{
		$strSql .= ''.
		' GROUP ' . ' BY ' .
			'acao_id' . 
			' , ' .
			'acao_codigo' . 
			' , ' .
			'programa_codigo' . 
			' , ' .
			'acao_descricao' .
			' , ' .
			'programa_id' .
		'';
		
		$strSql .=
		' ORDER BY ' .
			'programa_codigo ' .
			' , ' .
			'acao_codigo' .
			' , ' .
			'programa_id' .
		'';

		$strSql .=  
		' ) ' . ' As ' . ' AcoesComPropostas ' .
			' ON ' .
		'AcoesComPropostas.acao_id' . ' = ' . 'Acoes.acaid ';
		
		$strSql .=  
		' where Acoes.prsano = ' . "'$intAnoExercicio'";
	}
	else
	{
		
		$strSql .= ''.
		' GROUP ' . ' BY ' .
			'acao_id' . 
			' , ' .
			'acao_codigo' . 
			' , ' .
			'programa_codigo' . 
			' , ' .
			'acao_descricao' .
			' , ' .
			'programa_id' .
		'';
		
		$strSql .=
		' ORDER BY ' .
			'programa_codigo ' .
			' , ' .
			'acao_codigo' .
			' , ' .
			'programa_id' .
		'';
	}
	
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
		$strCodigoPrograma	= $objRegistroAtual[ 'programa_codigo' ];
		$strCodigoAcao		= $objRegistroAtual[ 'acao_codigo' ];
		$strDescricao		= $objRegistroAtual[ 'acao_descricao' ];
		$intQtdPropostas	= (integer)@$objRegistroAtual[ 'quantidade_de_propostas' ];
		$intQtdFusao		= (integer)@$objRegistroAtual[ 'quantidade_fusao' ];
		$intQtdMigracao		= (integer)@$objRegistroAtual[ 'quantidade_migracao' ];
		$intQtdAlteracao	= (integer)@$objRegistroAtual[ 'quantidade_alteracao' ];
		$intQtdInclusao		= (integer)@$objRegistroAtual[ 'quantidade_inclusao' ];
		$intQtdExclusao		= (integer)@$objRegistroAtual[ 'quantidade_exclusao' ];
		$intAcaoId			= $objRegistroAtual[ 'acao_id' ];
		$intProgramaId		= $objRegistroAtual[ 'programa_id' ];
		
		$arrProposta = array();
		
		$arrProposta[ 'linkAbreConteudo' ]	= geraLink( $strAgrupadoPor , $strOrdemGrupo , 
			$arrTiposdePropostas , $strCodigoAcao , $intLinhaAtual , $intProgramaId , $intAcaoId ,  0 , $boolIncluiAcoesSemProposta );
		$arrProposta[ 'linkIdConteudo']		= $intLinhaAtual . '-' . $strCodigoAcao;
			
			
		$arrProposta[ 'imgId' ] 			= geraImagemId( $intLinhaAtual , $strCodigoAcao );
		$arrProposta[ 'codigo_programa']	= $strCodigoPrograma;
		$arrProposta[ 'codigo_acao']		= $strCodigoAcao;
		$arrProposta[ 'acao_id']			= $intAcaoId;
		$arrProposta[ 'programa_id']		= $intProgramaId;
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

/**
 * Funcao que pega o array de propostas conforme os agrupadores e filtros solicitados
 *
 * @param string $strAgrupadoPor
 * @param integer $intAnoExercicio
 * @param string $strOrdemGrupo
 * @param integer $intPosicaodaColunaOrdenada
 * @param string $strDirecaoDaOrdenacao
 * @param array $arrTiposdePropostas
 * @return array
 */
function geraArrayDePropostas( $strAgrupadoPor , $intAnoExercicio , $strOrdemGrupo , $intPosicaodaColunaOrdenada , 
	$strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiAgrupadoresVazios = false  )
{
	$arrPropostas = array();
	
	switch( $strAgrupadoPor )
	{
		case 'Acoes':
		{
			$arrPropostas = geraArrayDePropostasdeAcaoAgrupadasPelaAcao( $strAgrupadoPor , $intAnoExercicio , 
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiAgrupadoresVazios);
			break;
		}
		case 'Programas':
		{
			$arrPropostas = geraArrayDePropostasdeAcaoAgrupadasPelosProgramas( $strAgrupadoPor , $intAnoExercicio ,
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiAgrupadoresVazios );
			break;
		}
		case 'Unidades':
		{
			$arrPropostas = geraArrayDePropostasAgrupadasPelasUnidades( $strAgrupadoPor , $intAnoExercicio ,
			$strOrdemGrupo , $intPosicaodaColunaOrdenada , $strDirecaoDaOrdenacao , $arrTiposdePropostas , $boolIncluiAgrupadoresVazios);
			break;
		}
	}
	return $arrPropostas;
}

/**
 * Monta o titulo da página conforme os filtros
 *
 * @param string $strAgrupadoPor
 * @param string $strTitulo
 * @param string $strTituloTotal
 * @param string $strColunaTotal
 */
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

/**
 * Gera o botao clicavel para mudanca de ordenação da coluna
 * @param integer $intPosicaoDaColuna
 * @param integer $intPosicaodaColunaAtualmenteOrdenada
 * @param string $strDirecaoDaOrdenacao
 * @param string $strTituloNome
 */
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

?>