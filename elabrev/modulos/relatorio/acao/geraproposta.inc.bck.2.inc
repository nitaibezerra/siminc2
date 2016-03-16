<?
#
# Sistema Simec
# Setor responsável: SPO-MEC
# Desenvolvedor: Equipe Consultores Simec
# Analista: Gilberto Arruda Cerqueira Xavier
# Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
# Programador: Thiago Mata (e-mail thiago.henrique.mata@gmail.com )
# Módulo:geraproposta.inc
# Finalidade: permitir a construção de relatorio de propostas de alteração em ações
#
# primeira parte do relatório : dados básicos

class PropostasDaAcao
{
	public static function getPropostasDeExclusaoDaAcao( $acacodid , $arrFiltroSPO)
	{
		global $db;
		
		$arrReturn = array();
		
		if ( $acacodid == 0 )
		{
			return $arrReturn;
		}
	
		$sql =	''.
		' SELECT ' .
			'u.usunome' 			. ' AS ' . 'proponente' .
			' , ' .
			'p.peastatus' 			. ' AS ' . 'statusUm' .
			' , ' .
			'p.peastatusspo' 		. ' AS ' . 'statusdois' .
			' , ' .
			'p.peajustificativa' 	. ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' 			. ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' 			. ' AS ' . 'usunomedois' .
		' FROM ' .
			'elabrev.proposta_exclusao_acao p' .
		' INNER JOIN ' .
			'seguranca.usuario u' .
		' ON ' .
			'u.usucpf' . ' = ' . 'p.usucpf' .
		' LEFT JOIN ' . 
			'seguranca.usuario u2' .
		' ON ' .
			'u2.usucpf' . ' = ' . 'p.usucpfdecisor' .
		' LEFT JOIN ' . 
			'seguranca.usuario u3' .
		' ON ' .
			'u3.usucpf' . ' = ' . 'p.usucpfdecisorspo' .
		' WHERE ' .
			'p.acaid' . ' = ' . '\'' . $acacodid . '\'' .
		' AND ' . 
			'p.peastatusspo' . ' IN ' . ' ( \'' . implode( '\',\'' , $arrFiltroSPO ) . '\' ) ' .
		' ORDER BY ' .
			'p.peastatusspo' 	. ' DESC ' .
			' , ' .
			'p.peastatus' 		. ' DESC ' .
			' , ' .
			'u.usunome';

	
		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusum' ];
			$statusDois		= $objRegistroAtual[ 'statusdois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			switch ( $statusDois )
			{
				case 'Z':
				{
					$strStatus = 'MEC';
					$strUsuarioNome = $usunomeum;
					break;		
				}
				case 'S':
				{
					$strStatus = 'SPO';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'D':
				{
					$strStatus = 'Unidade';
					$strUsuarioNome = $usunomeum;
					break;		
				}
				default:
				{
					$strStatus = 'Sem decisão';
					$strUsuarioNome = '';
					break;
				}
			}
			
			$arrElement = array();
			
			$arrElement[ 'par' ]			= $boolPar;
			$arrElement[ 'proponente']		= $proponente;
			$arrElement[ 'justificativa']	= $justificativa;
			$arrElement[ 'status' ]			= $strStatus;
			$arrElement[ 'decisor'] 		= $strUsuarioNome;
			$arrElement[ 'acao'] 		    = $acacodid;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsexclusao'][] =$arrReturn;
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeExclusaoDaAcao( $intAcaoId = 0  , $prgcod, $acacod, 
		$acadsc , $arrFiltroSPO )
	{
		?>
		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Exclusão da Ação <?= $prgcod ?> -  <?= $acacod ?>.<?= $acadsc ?>
					</b>
				</td>
			</tr>
		</table>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
    				<td>
    					<b> Proponente </b>
    				</td>
    				<td>
    					<b> Justificativa </b>
    				</td>
    				<td>
    					<b> Status <br/> Decisão </b>
		    		</td>
    				<td>
    					<b> Decisor
    				</td>
				</tr>
			</thead>
			<tbody>
		<?
		
		$arrPropostas = self::getPropostasDeExclusaoDaAcao( $intAcaoId , $arrFiltroSPO );
		if( sizeof( $arrPropostas ) > 0 )
		{
			foreach( $arrPropostas as $arrProposta )
			{
				if( $arrProposta[ 'par' ] )
				{
					$strBgColor = '';
				}
				else
				{
					$strBgColor = '#F7F7F7';
				}
				?>
					<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
					onmouseout="this.bgColor='<?= $strBgColor ?>';">
						<td>
							<?= $arrProposta[ 'proponente' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'justificativa' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'status' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'decisor' ] ?>
						</td>
					</tr>
				<?
			}
		}
		else
		{
			?>
				<tr align='center'>
					<td colspan=4>
						Não há propostas de exclusão nesta ação
					</td>
				</tr>			
			<?
		}
		
		?>
			</tbody>
		</table>		
		<?
	}
	
	public static function getPropostasDeMigracao( $acacodid , $arrFiltroSPO )
	{
		global $db;
		
		$arrReturn = array();
		
		if ( $acacodid == 0 )
		{
			return $arrReturn;
		}
		
		$sql =	''.
		' SELECT ' .
			'u.usunome' 			. ' AS ' . 'proponente' .
			' , ' .
			'p.pmastatus' 			. ' AS ' . 'statusUm' .
			' , ' .
			'p.pmastatusspo' 		. ' AS ' . 'statusdois' .
			' , ' .
			'p.pmajustificativa' 	. ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' 			. ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' 			. ' AS ' . 'usunomedois' .
			' , ' .
			'prg.prgcod' . ' || ' . '\'-\'' . ' || ' . 'prg.prgdsc' . ' AS ' . ' destino ' .
		' FROM ' .
			'elabrev.proposta_migracao_acao p' . 
		' INNER JOIN ' .
			'elabrev.ppaprograma_proposta prg' . 
		' ON ' . 
			'prg.prgid' . ' = ' . 'p.prgid' . 
		' INNER JOIN ' . 
			'seguranca.usuario u' .
		' ON ' .
			'u.usucpf' . ' = ' . 'p.usucpf' .
		' LEFT JOIN ' .
			'seguranca.usuario u2' .
		' ON ' .
			'u2.usucpf' . ' = ' . 'p.usucpfdecisor' .
		' LEFT JOIN ' . 
			'seguranca.usuario u3' .
		' ON ' .
			'u3.usucpf' . ' = ' . 'p.usucpfdecisorspo' .
		' WHERE ' .
			'p.acaid' . ' = ' . $acacodid .
		' AND ' . 
			'p.pmastatusspo' . ' IN ' . ' ( \'' . implode( '\',\'' , $arrFiltroSPO ) . '\' ) ' .
		' ORDER BY ' . 
			'p.pmastatusspo' . ' DESC ' .
			' , ' .
			'p.pmastatus' . ' DESC ' . 
			' , ' . 
			'u.usunome';
		
		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusdois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			$destino		= $objRegistroAtual[ 'destino' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusDois )
			{
				case 'Z':
				{
					$strStatus = 'MEC';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'S':
				{
					$strStatus = 'SPO';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'D':
				{
					$strStatus = 'Unidade';
					$strUsuarioNome = $usunomeum;
					break;		
				}
				default:
				{
					$strStatus = 'Sem decisão';
					$strUsuarioNome = '';
					break;
				}
			}
			
			$arrElement = array();
			
			$arrElement[ 'par' ]			= $boolPar;
			$arrElement[ 'proponente']		= $proponente;
			$arrElement[ 'justificativa']	= $justificativa;
			$arrElement[ 'status' ]			= $strStatus;
			$arrElement[ 'decisor'] 		= $strUsuarioNome;
			$arrElement[ 'destino'] 		= $destino;
			$arrElement[ 'acao'] 		    = $acacodid;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsmigracao'][] = $arrReturn;
		return $arrReturn;
	}			
		
	public static function montaHtmlPropostasDeMigracaoDaAcao( $intAcaoId = 0  , $prgcod, $acacod, 
		$acadsc , $arrFiltroSPO )
	{
		?>
		
		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Migração da Ação <?= $prgcod ?> -  <?= $acacod ?>.<?= $acadsc ?>
		</td></tr>
		</table>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
    				<td>
    					<b> Proponente </b>
    				</td>
    				<td>
    					<b> Migra para </b>
    				</td>
    				<td>
    					<b> Justificativa </b>
		    		</td>
    				<td>
    					<b> Status <br/> Decisão </b>
		    		</td>
    				<td>
    					<b> Decisor
    				</td>
				</tr>
			</thead>
			<tbody>
		<?
		
		$arrPropostas = self::getPropostasDeMigracao( $intAcaoId , $arrFiltroSPO );
		if( sizeof( $arrPropostas ) > 0 )
		{
			foreach( $arrPropostas as $arrProposta )
			{
				if( $arrProposta[ 'par' ] )
				{
					$strBgColor = '';
				}
				else
				{
					$strBgColor = '#F7F7F7';
				}
				?>
					<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
					onmouseout="this.bgColor='<?= $strBgColor ?>';">
						<td>
							<?= $arrProposta[ 'proponente' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'destino' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'justificativa' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'status' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'decisor' ] ?>
						</td>
					</tr>
				<?
			}
		}
		else
		{
			?>
				<tr align='center'>
					<td colspan=4>
						Não há propostas de migração nesta ação
					</td>
				</tr>			
			<?
		}
		
		?>
			</tbody>
		</table>		
		<?
	}
	
	public static function getPropostasDeFusao( $acacodid , $arrFiltroSPO )
	{
		global $db;
		
		$arrReturn = array();
		
		if ( $acacodid == 0 )
		{
			return $arrReturn;
		}
				
		$sql =	''.
		' SELECT ' .
			'u.usunome' 			. ' AS ' . 'proponente' .
			' , ' .
			'p.pfastatus' 			. ' AS ' . 'statusUm' .
			' , ' .
			'p.pfastatusspo' 		. ' AS ' . 'statusdois' .
			' , ' .
			'p.pfajustificativa' 	. ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' 			. ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' 			. ' AS ' . 'usunomedois' .
			' , ' .
			'a.prgcod' . ' || ' . '\'-\'' 		. ' || ' . 
			'a.acacod' . ' || ' . '\' . \''  	. ' || ' . 
			'a.acadsc' . ' AS ' . ' destino ' .
		' FROM ' .
			'elabrev.proposta_fusao_acao p' . 
		' INNER JOIN ' .
			'elabrev.ppaacao_proposta a' . 
		' ON ' . 
			'a.acaid' . ' = ' . 'p.acaid_externa' . 
		' INNER JOIN ' . 
			'seguranca.usuario u' .
		' ON ' .
			'u.usucpf' . ' = ' . 'p.usucpf' .
		' LEFT JOIN ' .
			'seguranca.usuario u2' .
		' ON ' .
			'u2.usucpf' . ' = ' . 'p.usucpfdecisor' .
		' LEFT JOIN ' . 
			'seguranca.usuario u3' .
		' ON ' .
			'u3.usucpf' . ' = ' . 'p.usucpfdecisorspo' .
		' WHERE ' .
			'p.acaid' . ' = ' . $acacodid .
		' AND ' . 
			'p.pfastatusspo' . ' IN ' . ' ( \'' . implode( '\',\'' , $arrFiltroSPO ) . '\' ) ' .
		' ORDER BY ' . 
			'p.pfastatusspo' . ' DESC ' .
			' , ' .
			'p.pfastatus' . ' DESC ' . 
			' , ' . 
			'u.usunome';
		
		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusdois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			$destino		= $objRegistroAtual[ 'destino' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusDois )
			{
				case 'Z':
				{
					$strStatus = 'MEC';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'S':
				{
					$strStatus = 'SPO';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'D':
				{
					$strStatus = 'Unidade';
					$strUsuarioNome = $usunomeum;
					break;		
				}
				default:
				{
					$strStatus = 'Sem decisão';
					$strUsuarioNome = '';
					break;
				}
			}
			
			$arrElement = array();
			
			$arrElement[ 'par' ]			= $boolPar;
			$arrElement[ 'proponente']		= $proponente;
			$arrElement[ 'justificativa']	= $justificativa;
			$arrElement[ 'status' ]			= $strStatus;
			$arrElement[ 'decisor'] 		= $strUsuarioNome;
			$arrElement[ 'destino'] 		= $destino;
			$arrElement[ 'acao'] 		    = $acacodid;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsfusao'][] = $arrReturn;
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeFusaoDaAcao( $intAcaoId = 0  , $prgcod , $acacod , 
		$acadsc , $arrFiltroSPO )
	{
		?>
		
		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Fusão da Ação <?= $prgcod ?> -  <?= $acacod ?>.<?= $acadsc ?>
		</td></tr>
		</table>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
    				<td>
    					<b> Proponente </b>
    				</td>
    				<td>
    					<b> Funde com </b>
    				</td>
    				<td>
    					<b> Justificativa </b>
		    		</td>
    				<td>
    					<b> Status <br/> Decisão </b>
		    		</td>
    				<td>
    					<b> Decisor
    				</td>
				</tr>
			</thead>
			<tbody>
		<?
		
		$arrPropostas = self::getPropostasDeFusao( $intAcaoId , $arrFiltroSPO );
		if( sizeof( $arrPropostas ) > 0 )
		{
			foreach( $arrPropostas as $arrProposta )
			{
				if( $arrProposta[ 'par' ] )
				{
					$strBgColor = '';
				}
				else
				{
					$strBgColor = '#F7F7F7';
				}
				?>
					<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
					onmouseout="this.bgColor='<?= $strBgColor ?>';">
						<td>
							<?= $arrProposta[ 'proponente' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'destino' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'justificativa' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'status' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'decisor' ] ?>
						</td>
					</tr>
				<?
			}
		}
		else
		{
			?>
				<tr align='center'>
					<td colspan=4>
						Não há propostas de fusão nesta ação
					</td>
				</tr>			
			<?
		}
		
		?>
			</tbody>
		</table>		
		<?
	}
	
	
	
	public static function getPropostasDeAlteracao( $acacodid  , $arrFiltroSPO )
	{
		global $db;
		$arrReturn = array();

		if ( $acacodid == 0 )
		{
			return $arrReturn;
		}	
	
		$sql =	''.
		' SELECT ' .
			'p.eracampo' . ' AS ' . 'atributo' .
			' , ' .
			'p.eraproposta' . ' AS ' . 'proposta' .
			' , ' .
			'u.usunome' . ' AS ' . 'proponente' .
			' , ' .
			'p.erastatus' . ' AS ' . 'statusUm' .
			' , ' .
			'p.erastatusspo' . ' AS ' . 'statusdois' .
			' , ' .
			'p.erajustificativa' . ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome'  . ' AS ' .  'usunomeum' .
			' , ' .
			'u3.usunome'  . ' AS ' .  'usunomedois' .
		' FROM ' .
			'elabrev.elaboracaorevisao p' . 
		' INNER JOIN ' .
			'seguranca.usuario u' . 
		' ON ' . 
			'u.usucpf' . ' = ' . 'p.usucpf' . 
		' LEFT JOIN ' .
			'seguranca.usuario u2' .
		' ON ' .
			'u2.usucpf' . ' = ' . 'p.usucpfdecisor' .
		' LEFT JOIN ' . 
			'seguranca.usuario u3' .
		' ON ' .
			'u3.usucpf' . ' = ' . 'p.usucpfdecisorspo' .
		' WHERE ' .
			'p.eracampo' . ' <> ' . '\'acaobs\'' .
			' AND '.
			'p.eracod' . ' = ' . $acacodid .
			' AND ' .
			'p.eratabela' . ' = ' . '\'' . 'ppaacao_proposta' . '\'' .
		' AND ' . 
			'p.erastatusspo' . ' IN ' . ' ( \'' . implode( '\',\'' , $arrFiltroSPO ) . '\' ) ' .			
		' ORDER BY ' . 
			'p.eracampo' .
			' , ' .
			'p.erastatusspo' . ' DESC ' . 
			' , ' . 
			'p.erastatus' . ' DESC ' . 
			' , '  .
			'u.usunome'
			;
		$objResultSet = $db->record_set($sql);
		
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );

		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$atributo 		= $objRegistroAtual[ 'atributo' ];
			$proposta		= $objRegistroAtual[ 'proposta' ];
			$proponente		= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusdois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusDois )
			{
				case 'Z':
				{
					$strStatus = 'MEC';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'S':
				{
					$strStatus = 'SPO';
					$strUsuarioNome = $usunomedois;
					break;		
				}
				case 'D':
				{
					$strStatus = 'Unidade';
					$strUsuarioNome = $usunomeum;
					break;		
				}
				default:
				{
					$strStatus = 'Sem decisão';
					$strUsuarioNome = '';
					break;
				}
			}
			
			$arrElement = array();
			
			$arrElement[ 'atributo' ]			= $atributo;
			$arrElement[ 'proposta']			= $proposta;
			$arrElement[ 'proponente']			= $proponente;
			$arrElement[ 'statusUm' ]			= $statusUm;
			$arrElement[ 'statusdois'] 			= $statusDois;
			$arrElement[ 'usunomeum'] 			= $usunomeum;
			$arrElement[ 'usunomedois'] 		= $usunomedois;
			$arrElement[ 'status'] 				= $strStatus;
			$arrElement[ 'decisor'] 			= $strUsuarioNome;
			$arrElement[ 'par'] 				= $boolPar;
			$arrElement[ 'justificativa'] 		= $justificativa;
			$arrElement[ 'acao'] 		        = $acacodid;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsalteracao'][] = $arrReturn;
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeAlteracaoDaAcao( $intAcaoId = 0  , $prgcod, $acacod, 
		$acadsc , $arrFiltroSPO )
	{
		?>

		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Alteração de Atributos na Ação <?= $prgcod ?> -  <?= $acacod ?>.<?= $acadsc ?>
					</b>
				</td>
			</tr>
		</table>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
		    		<td>
		    			<b> Proponente </b> 
		    		</td>
		    		<td>
		    			<b> Atributo </b> 
		   			</td>    
				    <td>
					    <b> Proposta </b> 
				    </td>     
		    		<td>
		    			<b> Justificativa </b> 
		    		</td>
		    		<td>
		    			<b> Status<br>Decisão </b> 
		    		</td>
		    		<td>
		    			<b> Decisor </b> 
		    		</td>
				</tr>
			</thead>
			<tbody>
		<?
				
		$arrPropostas = self::getPropostasDeAlteracao( $intAcaoId , $arrFiltroSPO );
		if( sizeof( $arrPropostas ) > 0 )
		{
			foreach( $arrPropostas as $arrProposta )
			{
				if( $arrProposta[ 'par' ] )
				{
					$strBgColor = '';
				}
				else
				{
					$strBgColor = '#F7F7F7';
				}
				?>
					<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
					onmouseout="this.bgColor='<?= $strBgColor ?>';">
						<td>
							<?= $arrProposta[ 'proponente' ] ?>
						</td>
						<td>
							<?= tradutor_att( $arrProposta[ 'atributo' ] ) ?>
						</td>
						<td>
							<?= $arrProposta[ 'proposta' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'justificativa' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'status' ] ?>
						</td>
						<td>
							<?= $arrProposta[ 'decisor' ] ?>
						</td>
					</tr>
				<?
			}
		}
		else
		{
			?>
				<tr align='center'>
					<td colspan=4>
						Não há propostas de alteração nesta ação
					</td>
				</tr>			
			<?
		}
		
		?>
			</tbody>
		</table>		
		<?
	}

	
	public static function MostraFiltro( )
	{
		?>

		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Critérios de Filtro 
					</b>
				</td>
			</tr>
		</table>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
		    		<td>
		    		<?
				    if( sizeof( $_SESSION['filtrorelat'] ) > 0 )
				    { 
				       foreach ($_SESSION['filtrorelat'] as $filtro)
				       {	
				           print $filtro.'<br>';	
				       }
				    }
				    ?>
		    		</td>		    		
				</tr>
			</thead>
			<tbody>	</table>		
		<?
	}	
	
	public static function montaHtmlDasPropostasDaAcao(	$intAcaoId , $boolExclusao , $boolMigracao , 
		$boolFusao , $boolAlteracao , $arrFiltroSPO )
	{
		global $db;
		$sql=	' SELECT ' . 
					'prgcod' . 
					' , ' . 
					'acacod' . 
					' , ' . 
					'acadsc' . 
				' FROM ' . 
					'elabrev.ppaacao_proposta' . 
				' WHERE ' . 
					'acaid' . ' = ' . $intAcaoId;
		$linha = $db->pegalinha( $sql );

		$intPrgCode 	= $linha[ 'prgcod' ];
		$strAcaoCode 	= $linha[ 'acacod' ];
		$strAcaoDsc 	= $linha[ 'acadsc' ];

		if( !$boolExclusao && !$boolMigracao && !$boolFusao && !$boolAlteracao )
		{
			// nenhum elemento de filtro foi marcado logo deve-se mostrar a todos //	
			$boolExclusao 	= true;
			$boolMigracao 	= true;
			$boolFusao		= true;
			$boolAlteracao	= true;
		}
		
		if ( $boolExclusao )
		{
			PropostasDaAcao::montaHtmlPropostasDeExclusaoDaAcao( $intAcaoId , $intPrgCode, $strAcaoCode, 
			$strAcaoDsc , $arrFiltroSPO);
		}
		if ( $boolMigracao )
		{
			PropostasDaAcao::montaHtmlPropostasDeMigracaoDaAcao( $intAcaoId , $intPrgCode, $strAcaoCode, 
			$strAcaoDsc , $arrFiltroSPO);
		}
		if ( $boolFusao )
		{
			PropostasDaAcao::montaHtmlPropostasDeFusaoDaAcao( $intAcaoId , $intPrgCode, $strAcaoCode, 
			$strAcaoDsc, $arrFiltroSPO );
		}
		if ( $boolAlteracao )
		{
			PropostasDaAcao::montaHtmlPropostasDeAlteracaoDaAcao( $intAcaoId , $intPrgCode, $strAcaoCode, 
			$strAcaoDsc, $arrFiltroSPO );
		}		
	}
}

?>

<div align="center">
	<input align="center" type="button" name="btpdf" value="Gerar PDF" onclick="gerarpdf()">
	<br/>
</div>

<?
unset($_SESSION['filtrorelat']);


if ($_REQUEST['acod'])
{
	$_REQUEST[ 'cbexclusao' ] =1;
    $_REQUEST[ 'cbmigracao' ] = 1;
    $_REQUEST[ 'cbfusao' ] = 1;
    $_REQUEST[ 'cbalteracao' ] =1;
    $_REQUEST['acaid'][] = $_REQUEST['acod'];
    $titulo_modulo='Propostas de Alteração na Ação';
	monta_titulo($titulo_modulo,'');
	?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
	<?
}

$boolExclusao	= $_REQUEST[ 'cbexclusao' ];
$boolMigracao	= $_REQUEST[ 'cbmigracao' ];
$boolFusao		= $_REQUEST[ 'cbfusao' ];
$boolAlteracao	= $_REQUEST[ 'cbalteracao' ];
if ($_REQUEST[ 'cbexclusao' ]) $_SESSION['filtrorelat'][] = 'Exclusão';
if ($_REQUEST[ 'cbalteracao']) $_SESSION['filtrorelat'][] = 'Alteração';
if ($_REQUEST[ 'cbmigracao' ]) $_SESSION['filtrorelat'][] = 'Migração';
if ($_REQUEST[ 'cbfusao' ]) $_SESSION['filtrorelat'][] = 'Fusão';

$arrDecspo		= array();

if ( $_REQUEST[ 'cbmec' ] )
{
	$arrDecspo[] = 'Z';
	$_SESSION['filtrorelat'][] = 'Decisão Coord.UMA';
}
if ( $_REQUEST[ 'cbuma' ] )
{
	$arrDecspo[] = 'S';
	$_SESSION['filtrorelat'][] = 'Decisão SPO';	
}
if ( $_REQUEST['cbunid'] )
{
	$arrDecspo[] = 'D';
	$_SESSION['filtrorelat'][] = 'Decisão Unidade';	
}
if ( $_REQUEST['cbsemdec'])
{
	$arrDecspo[] = 'A';
	$_SESSION['filtrorelat'][] = 'Sem decisão';	
}

// se nenhum estiver marcado vale como se todos estivesse //
if( sizeof( $arrDecspo ) == 0 )
{
	$arrDecspo = array( 'Z' , 'S' , 'D' , 'A' );
}
unset($_SESSION['rsexclusao']);
unset($_SESSION['rsmigracao']);
unset($_SESSION['rsfusao']);
unset($_SESSION['rsalteracao']);
unset($_SESSION['acaoidrelat']);
unset($_SESSION['mostrou']);
    
if ($_REQUEST['acaid'][0]=='') unset($_REQUEST['acaid'][0]);
if ($_REQUEST['prgid'][0]=='') unset($_REQUEST['prgid'][0]);
if ($_REQUEST['unicod'][0]=='') unset($_REQUEST['unicod'][0]);

if ($_REQUEST['acaid'])
{
	foreach ($_REQUEST['acaid'] as $intAcaId)
	{
	   $sql = "select a.acacod||'-'||a.acadsc as filtro from elabrev.ppaacao_proposta a where a.acaid=$intAcaId";
	   $acoes = $db->pegaUm($sql);
	   $_SESSION['filtrorelat'][]=' Ações: '.$acoes;	   
	}
}

if ($_REQUEST['unicod'])
{
	foreach ($_REQUEST['unicod'] as $arrUnicod)
	{
	   $sql = "select ua.acaid, u.unicod||'-'||u.unidsc as filtro from elabrev.unidade_acao ua inner join unidade u on ua.unicod=u.unicod where uacstatus='A' and ua.unicod='$arrUnicod'";
	   $acoes = $db->carregar($sql);
	   foreach ($acoes as $acao) {
	   		$_REQUEST['acaid'][]=$acao['acaid'];
	   		$_SESSION['filtrorelat'][]=' Unidade: '.$acao['filtro'];
       }
	}
}

if ($_REQUEST['prgid'])
{
	foreach ($_REQUEST['prgid'] as $intPrgId)
	{
	   $sql = "select a.acaid, p.prgcod||'-'||p.prgdsc as filtro from elabrev.ppaacao_proposta a inner join elabrev.ppaprograma_proposta p on p.prgid=a.prgid where a.prgid=$intPrgId";
	   $acoes = $db->carregar($sql);
	   foreach ($acoes as $acao) {
	   	  $_REQUEST['acaid'][]=$acao['acaid'];
		  $_SESSION['filtrorelat'][]=' Programa: '.$acao['filtro'];		   	  
	   }
	}
}

$_REQUEST['acaid'] = array_unique( (array) $_REQUEST['acaid']);
$_SESSION['filtrorelat'] = array_unique( (array) $_SESSION['filtrorelat']);

foreach ( $_REQUEST['acaid'] as $intAcaoId )
{
	if ( $_SESSION['filtrorelat'] and ! $_SESSION['mostrou'])
	{
		$_SESSION['mostrou']=1;
		PropostasDaAcao::MostraFiltro( );
	}
	$_SESSION['acaoidrelat'][] = $intAcaoId ;
	PropostasDaAcao::montaHtmlDasPropostasDaAcao
	( 
		$intAcaoId 		, 
		$boolExclusao 	, 
		$boolMigracao 	,  
		$boolFusao 		, 
		$boolAlteracao	,
		$arrDecspo
	);
}
//dbg($_SESSION['rsexclusao']);
?>
	</tbody>
</table>
<script>
function gerarpdf()
{
	//Abre popup em branco
	janela = window.open(
		'about:blank',
		"relatorio",
		"menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'"
		);
	//Abre relatório pdf no popup
	janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_prop_aca&acao=R";
}	
</script>
