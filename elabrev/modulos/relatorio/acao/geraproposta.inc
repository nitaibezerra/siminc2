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

	
	public static function MostraFiltro( $arrFiltrosModoRelatorio )
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
					    if( sizeof( $arrFiltrosModoRelatorio ) > 0 )
					    { 
					       foreach ($arrFiltrosModoRelatorio as $strFiltro)
					       {
					       		?>
					       			<?= $strFiltro ?> <br/>
					       		<?
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
		$arrLinha = $db->pegalinha( $sql );

		$intPrgCode 	= $arrLinha[ 'prgcod' ];
		$strAcaoCode 	= $arrLinha[ 'acacod' ];
		$strAcaoDsc 	= $arrLinha[ 'acadsc' ];

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

/*
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
*/

function getAcoesDasUnidadesPeloId( $arrUnidadesId )
{
	global $db;
	$arrReturnAcoesId = array();

	if ( sizeof( $arrUnidadesId ) == 0 )
	{
		return $arrReturnAcoesId;
	}	

	$sql =	''.
		' SELECT ' . 
			'UnidadeAcao.acaid' . ' AS ' . 'acao_id' .
		' FROM ' . 
			'elabrev.unidade_acao UnidadeAcao' . 
		' INNER JOIN ' .
			'unidade Unidade' . 
		' ON ' .
			'UnidadeAcao.unicod' . ' = ' . 'Unidade.unicod' .
		' WHERE ' .
			'uacstatus' . ' = ' . '\'A\'' . 
		' AND ' .
			'UnidadeAcao.uniid' . ' IN ' . ' (  ' . '\'' . implode( '\'' . ' , ' . '\'' ,  $arrUnidadesId ) .  '\'' . ' ) ' .
		'';
		
	$objResultSet = $db->record_set($sql);
	
	$intNumerodeLinhas = $db->conta_linhas( $objResultSet );

	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
   		$arrReturnAcoesId[] = $objRegistroAtual[ 'acao_id' ];
  	}
  	
  	return $arrReturnAcoesId;
}

function getAcoesDasUnidadesPeloCodigoUnico( $arrUnidadesCodigoUnico )
{
	global $db;
	$arrReturnAcoesId = array();

	if ( sizeof( $arrUnidadesId ) == 0 )
	{
		return $arrReturnAcoesId;
	}	

	$sql =	''.
		' SELECT ' . 
			'UnidadeAcao.acaid' . ' AS ' . 'acao_id' .
		' FROM ' . 
			'elabrev.unidade_acao UnidadeAcao' . 
		' INNER JOIN ' .
			'unidade Unidade' . 
		' ON ' .
			'UnidadeAcao.unicod' . ' = ' . 'Unidade.unicod' .
		' WHERE ' .
			'uacstatus' . ' = ' . '\'A\'' . 
		' AND ' .
			'UnidadeAcao.unicod' . ' IN ' . ' (  ' . '\'' . implode( '\'' . ' , ' . '\'' ,  $arrUnidadesCodigoUnico ) .  '\'' . ' ) ' .
		'';
		
	$objResultSet = $db->record_set($sql);
	
	$intNumerodeLinhas = $db->conta_linhas( $objResultSet );

	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
   		$arrReturnAcoesId[] = $objRegistroAtual[ 'acao_id' ];
  	}
  	return $arrReturnAcoesId;
}

function getAcoesDosProgramasPeloId( $arrProgramasPeloId )
{
	global $db;
	$arrReturnAcoesId = array();

	if ( sizeof( $arrProgramasPeloId ) == 0 )
	{
		return $arrReturnAcoesId;
	}	

	$strSql =	''.
		' SELECT ' . 
			'AcaoProposta.acaid' . ' AS ' . 'acao_id' .
		' FROM ' . 
			'elabrev.ppaacao_proposta AcaoProposta' . 
		' INNER JOIN ' .
			'elabrev.ppaprograma_proposta ProgramaProposta' . 
		' ON ' .
			'ProgramaProposta.prgid' . ' = ' . 'AcaoProposta.prgid' .
		' WHERE ' .
			'AcaoProposta.prgid' . ' IN ' . ' ( ' . implode( ' , ' ,  $arrProgramasPeloId ) . ' ) ' .
		'';
		
	$objResultSet = $db->record_set( $strSql );
	
	$intNumerodeLinhas = $db->conta_linhas( $objResultSet );

	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	
   		$arrReturnAcoesId[] = $objRegistroAtual[ 'acao_id' ];
  	}
  	return $arrReturnAcoesId;
}

function montaFiltrosModoRelatorio( $boolExclusao , $boolMigracao, $boolFusao ,$boolAlteracao , $arrDecspo ,
	$arrAcaoId , $arrUnidadesCodigoUnico , $arrProgramaId )
{
	global $db;
	
	if ( $boolExclusao )	$arrFiltrosModoRelatorio[] = 'Exclusão';
	if ( $boolMigracao )	$arrFiltrosModoRelatorio[] = 'Migração';
	if ( $boolFusao )		$arrFiltrosModoRelatorio[] = 'Fusão';
	if ( $boolAlteracao )	$arrFiltrosModoRelatorio[] = 'Alteração';

	$arrDecisoesFiltro = array();
	if ( in_array( 'Z' , $arrDecspo) )
	{
		$arrDecisoesFiltro[] = 'Decisão Coord.UMA';
	}
	if ( in_array( 'S' , $arrDecspo) )
	{
		$arrDecisoesFiltro[] = 'Decisão SPO';	
	}
	if ( in_array( 'D' , $arrDecspo) )
	{
		$arrDecisoesFiltro[] = 'Decisão Unidade';	
	}
	if ( in_array( 'A' , $arrDecspo) )
	{
		$arrDecisoesFiltro[] = 'Sem decisão';	
	}
	if( sizeof( $arrDecisoesFiltro ) > 0 )
	{
	    $strFiltroDecisoes = 'Decisão: ' . montaListagem( $arrDecisoesFiltro ) . '.';
		$arrFiltrosModoRelatorio[] = $strFiltroDecisoes;	
	}

	$arrAcoesFiltros = array();
	if( sizeof( $arrAcaoId ) > 0 )
	{
	   $strSql = ''.
		' SELECT ' . 
			'AcaoProposta.acacod' . ' AS ' . 'acao_codigo_unico' .
			' , ' .
			'AcaoProposta.acadsc' . ' AS ' . 'acao_descricao' .
		' FROM ' . 
			'elabrev.ppaacao_proposta AcaoProposta' . 
		' WHERE ' .
			'AcaoProposta.acaid' . ' IN ' .  ' ( ' . implode( ',' , $arrAcaoId ) . ' ) ' .
		' ORDER ' . ' BY ' .
			'acao_codigo_unico' .
		'';

		$objResultSet = $db->record_set( $strSql );
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistro = $db->carrega_registro( $objResultSet , $intLinhaAtual );
			$arrAcoesFiltros[] = $objRegistro[ 'acao_codigo_unico' ] . '-' . $objRegistro[ 'acao_descricao' ];
		}
	}
	if( sizeof( $arrAcoesFiltros ) > 0 )
	{
	    $strFiltroAcoes = 'Ações: ' . montaListagem( $arrAcoesFiltros ) . '.';
		$arrFiltrosModoRelatorio[] = $strFiltroAcoes;	
	}

	
	$arrUnidadesFiltros = array();
	if( sizeof( $arrUnidadesCodigoUnico ) > 0 )
	{
		
	   $strSql = ''.
		' SELECT ' . 
			'Unidade.unicod' . ' AS ' . 'unidade_codigo_unico' .
			' , ' .
			'Unidade.unidsc' . ' AS ' . 'unidade_descricao' .
		' FROM ' . 
			'public.unidade Unidade' . 
		' WHERE ' .
			'unistatus' . ' = ' . '\'A\'' . 
		' AND ' .
			'Unidade.unicod' . ' IN ' .  ' ( ' . '\'' . implode( '\'' . ' , ' . '\'' ,  $arrUnidadesCodigoUnico ) .  '\'' . ' ) ' .
		' ORDER ' . ' BY ' .
			'unidade_codigo_unico' .
		'';
	   
		$objResultSet = $db->record_set( $strSql );
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistro = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	   		$arrUnidadesFiltros[] = $objRegistro[ 'unidade_codigo_unico' ] . '-' . $objRegistro[ 'unidade_descricao' ];
		}
	}
	if( sizeof( $arrUnidadesFiltros ) > 0 )
	{
	    $strFiltroUnidades = 'Unidades: ' . montaListagem( $arrUnidadesFiltros ) . '.';
		$arrFiltrosModoRelatorio[] = $strFiltroUnidades;	
	}

	$arrProgramasFiltros = array();
	if( sizeof( $arrProgramaId ) > 0 )
	{
/*		
	   $strSql = ''.
		' SELECT ' . 
			'ProgramaProposta.prgcod' . ' AS ' . 'programa_codigo_unico' .
			' , ' .
			'ProgramaProposta.prgdsc' . ' AS ' . 'programa_descricao' .
		' FROM ' . 
			'elabrev.ppaacao_proposta AcaoProposta' . 
		' INNER ' . ' JOIN ' .
			'elabrev.ppaprograma_proposta ProgramaProposta' .
		' ON ' .
			'ProgramaProposta.prgid = AcaoProposta.prgId' .
		' WHERE ' .
			'AcaoProposta.prgid' . ' IN ' .  ' ( ' . implode( ',' , $arrAcaoId ) . ' ) ' .
		' ORDER ' . ' BY ' .
			'programa_codigo_unico' .
		'';
*/	   
	   $strSql = ''.
		' SELECT ' . 
			'ProgramaProposta.prgcod' . ' AS ' . 'programa_codigo_unico' .
			' , ' .
			'ProgramaProposta.prgdsc' . ' AS ' . 'programa_descricao' .
		' FROM ' . 
			'elabrev.ppaprograma_proposta ProgramaProposta' .
		' WHERE ' .
			'ProgramaProposta.prgid' . ' IN ' .  ' ( ' . implode( ',' , $arrProgramaId ) . ' ) ' .
		' ORDER ' . ' BY ' .
			'programa_codigo_unico' .
		'';
		
		$objResultSet = $db->record_set( $strSql );
		
		$intNumerodeLinhas = $db->conta_linhas( $objResultSet );
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistro = $db->carrega_registro( $objResultSet , $intLinhaAtual );
	   		$arrProgramasFiltros[] = $objRegistro[ 'programa_codigo_unico' ] . '-' . $objRegistro[ 'programa_descricao' ];
		}
	}
	if( sizeof( $arrProgramasFiltros ) > 0 )
	{
	    $strFiltroProgramas = 'Programas: ' . montaListagem( $arrProgramasFiltros ) . '.';
		$arrFiltrosModoRelatorio[] = $strFiltroProgramas;	
	}

	return $arrFiltrosModoRelatorio;
}

function montaListagem( $arrElementos , $strSeparator =  ', ' , $strLastSeparator = ' e ' )
{
	switch( sizeof( $arrElementos ) )
	{
		case 0:
		{
			$strResult = '';
			break;
		}
		case 1:
		{
			$strResult = $arrElementos[ 0 ];
			break;
		}
		default:
		{
			$objLast = array_pop( $arrElementos );
			$strResult = implode( $strSeparator , $arrElementos ) . $strLastSeparator . $objLast ;
			break;	
		}
	}

	return $strResult;
}

$boolExclusao	= $_REQUEST[ 'cbexclusao' ];
$boolMigracao	= $_REQUEST[ 'cbmigracao' ];
$boolFusao		= $_REQUEST[ 'cbfusao' ];
$boolAlteracao	= $_REQUEST[ 'cbalteracao' ];

$arrDecspo		= array();
if ( $_REQUEST[ 'cbmec' ] )
{
	$arrDecspo[] = 'Z';
}
if ( $_REQUEST[ 'cbuma' ] )
{
	$arrDecspo[] = 'S';
}
if ( $_REQUEST['cbunid'] )
{
	$arrDecspo[] = 'D';
}
if ( $_REQUEST['cbsemdec'])
{
	$arrDecspo[] = 'A';
}
// se nenhum estiver marcado vale como se todos estivesse //
if( sizeof( $arrDecspo ) == 0 )
{
	$arrDecspo = array( 'Z' , 'S' , 'D' , 'A' );
}

$arrUnidadesId = array();


$arrAcaoId 				= (array) $_REQUEST['acaid'];
//$arrUnidadesId 			= $_REQUEST['uniid'];
$arrUnidadesCodigoUnico = (array) $_REQUEST['unicod'];
$arrProgramaId			= (array) $_REQUEST['prgid'];

$intPos = array_search( '' , $arrAcaoId );
if( $intPos !== false )
{
		unset( $arrAcaoId[ $intPos ] );
};
$intPos = array_search( '' , $arrProgramaId );
if( $intPos !== false )
{
	unset( $arrProgramaId[ $intPos ] );
};

$intPos = array_search( '' , $arrUnidadesCodigoUnico );
if( $intPos !== false )
{
	unset( $arrUnidadesCodigoUnico[ $intPos ] );
};




if ($arrUnidadesId) $arrAcaoId = array_merge( $arrAcaoId , getAcoesDasUnidadesPeloId( $arrUnidadesId ) );
if ($arrUnidadesCodigoUnico) $arrAcaoId = array_merge( $arrAcaoId , getAcoesDasUnidadesPeloCodigoUnico( $arrUnidadesCodigoUnico ) );
if ($arrProgramaId) $arrAcaoId = array_merge( $arrAcaoId , getAcoesDosProgramasPeloId( $arrProgramaId ) );

$arrFiltrosRelatorio = montaFiltrosModoRelatorio( $boolExclusao , $boolMigracao , $boolFusao , $boolAlteracao , $arrDecspo , $arrAcaoId , $arrUnidadesCodigoUnico , $arrProgramaId );

$_SESSION['filtrorelat'] = $arrFiltrosRelatorio;

foreach ( $arrAcaoId as $intAcaoId )
{
	if ( sizeof( $arrFiltrosRelatorio ) > 0 )
	{
		PropostasDaAcao::MostraFiltro( $arrFiltrosRelatorio );
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
