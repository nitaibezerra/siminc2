<?
// funcoes novas //

class PropostasDaAcao
{
	public static function getPropostasDeExclusaoDaAcao( $acacodid = 0 )
	{
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
			'p.peastatusspo' 		. ' AS ' . 'statusDois' .
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
		' ORDER BY ' .
			'p.peastatusspo' 	. ' DESC ' .
			' , ' .
			'p.peastatus' 		. ' DESC ' .
			' , ' .
			'u.usunome';

		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas($RS);
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusDois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusdois )
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
			
			$arrReturn[] = $arrElement;
		}
		
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeExclusaoDaAcao( $intAcaoId = 0  , $prgcod, $acaocod, $acaodsc )
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
		
		$arrPropostas = self::getPropostasDeExclusaoDaAcao( $intAcaoId );
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
	
	public static function getPropostasDeMigracao( $acacodid )
	{
		
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
			'p.pmastatusspo' 		. ' AS ' . 'statusDois' .
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
		' ORDER BY ' . 
			'p.pmastatusspo' . ' DESC ' .
			' , ' .
			'p.pmastatus' . ' DESC ' . 
			' , ' . 
			'u.usunome';
		
		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas($RS);
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusDois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			$destino		= $objRegistroAtual[ 'destino' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusdois )
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
			
			$arrReturn[] = $arrElement;
		}
		
		return $arrReturn;
	}			
		
	public static function montaHtmlPropostaDeMigracaoDaAcao( $intAcaoId = 0  , $prgcod, $acaocod, $acaodsc )
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
		
		$arrPropostas = self::getPropostasDeMigracao( $intAcaoId );
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
	
	public function getPropostasDeFusao()
	{
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
			'p.pfastatusspo' 		. ' AS ' . 'statusDois' .
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
		' ORDER BY ' . 
			'p.pfastatusspo' . ' DESC ' .
			' , ' .
			'p.pfastatus' . ' DESC ' . 
			' , ' . 
			'u.usunome';
		
		$objResultSet = $db->record_set($sql);
		$intNumerodeLinhas = $db->conta_linhas($RS);
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$proponente 	= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusDois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			$destino		= $objRegistroAtual[ 'destino' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusdois )
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
			
			$arrReturn[] = $arrElement;
		}
		
		return $arrReturn;
	}
	
	public static function montaHtmlPropostaDeFusaoDaAcao( $intAcaoId = 0  , $prgcod, $acaocod, $acaodsc )
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
		
		$arrPropostas = self::getPropostasDeMigracao( $intAcaoId );
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
	
	
	
	public function getPropostaDeAlteracao()
	{
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
			'p.erastatusspo' . ' AS ' . 'statusDois' .
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
			'p.eracampo' . ' <> ' . 'acaobs' .
			' AND '.
			'p.eracod' . ' = ' . $acacodid .
			' AND ' .
			'p.eratabela' . ' = ' . '\'' . 'ppaacao_proposta' . '\'' .
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
		$intNumerodeLinhas = $db->conta_linhas($RS);
		
		for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
		{
			$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
				
			$atributo 		= $objRegistroAtual[ 'atributo' ];
			$proposta		= $objRegistroAtual[ 'proposta' ];
			$proponente		= $objRegistroAtual[ 'proponente' ];
			$statusUm		= $objRegistroAtual[ 'statusUm' ];
			$statusDois		= $objRegistroAtual[ 'statusDois' ];
			$justificativa	= $objRegistroAtual[ 'justificativa' ];
			$usunomeum		= $objRegistroAtual[ 'usunomeum' ];
			$usunomedois	= $objRegistroAtual[ 'usunomedois' ];
			
			$boolPar = ( ( $intLinhaAtual % 2 ) == 0 );
			
			switch ( $statusdois )
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
			$arrElement[ 'statusDois'] 			= $statusDois;
			$arrElement[ 'usunomeum'] 			= $usunomeum;
			$arrElement[ 'usunomedois'] 		= $usunomedois;
			$arrElement[ 'status'] 				= $strStatus;
			$arrElement[ 'decisor'] 			= $strUsuarioNome;
			$arrElement[ 'par'] 				= $boolPar;
			$arrElement[ 'justificativa'] 		= $justificativa;
			
			$arrReturn[] = $arrElement;
		}
		
		return $arrReturn;
	}
	
	public static function montaHtmlPropostaDeAlteracaoDaAcao( $intAcaoId = 0  , $prgcod, $acaocod, $acaodsc )
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
		
		$arrPropostas = self::getPropostasDeMigracao( $intAcaoId );
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
}

// fim das funcoes novas //
?>

<?
/*
Sistema Simec
Setor responsável: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
Módulo:geraproposta.inc
Finalidade: permitir a construção de relatorio de propostas de alteração em ações
*/
// primeira parte do relatório : dados básicos
$_SESSION['rsdados'][] = $acacodid;

if ($exclusao)
{
/**
	$_SESSION['prnexc']=1;
	?>
<br/>
<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<b>
				Propostas de Exclusão da Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?>
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
	
	$proponente		= 0;
	$peastatus		= 0;
	$peastatusspo	= 0;
	$usunome		= 0;
	$usunomeum		= 0;
	$usunomedois	= 0;
	$nlinhas		= 0;

	if ( $acacodid )
	{
		$sql =	''.
		' SELECT ' .
			'u.usunome' . ' AS ' . 'proponente' .
			' , ' .
			'p.peastatus' . ' AS ' . 'statusUm' .
			' , ' .
			'p.peastatusspo' . ' AS ' . 'statusDois' .
			' , ' .
			'p.peajustificativa' . ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' . ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' . ' AS ' . 'usunomedois' .
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
		' ORDER BY ' .
			'p.peastatusspo' . ' DESC ' .
			' , ' .
			'p.peastatus' . ' DESC ' .
			' , ' .
			'u.usunome';

		$RS = $db->record_set($sql);
		$nlinhas = $db->conta_linhas($RS);
	}

	if ($nlinhas >= 0)
	{
		for ($ie=0;$ie<=$nlinhas;$ie++)
		{
			$resexc = $db->carrega_registro($RS,$ie);
			if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;

			if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';

			print "
				<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" 
				onmouseout=\"this.bgColor='$marcado';\">
					<td>
						$proponente
					</td>
					<td>
						$justificativa
					</td>
			";

			if ($statusdois=='Z')
			{
				print '<td> MEC </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusdois=='S')
			{
				print '<td> SPO </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusum=='D')
			{
				print '<td> Unidade </td>';
				if ($usunomeum) print $usunomeum;
			}
			else
			{
				print '<<td> Sem decisão</td>';
			}
			print "<td></td></tr>";
		}
	}
	else
	{
		print  "
			<tr align='center'>
				<td colspan=4>
					Não há propostas de exclusão nesta ação
				</td>
			</tr>
				";
	}
**/
}

if ($migracao)
{
/**
	$_SESSION['prnmig']=1;
	?>
	</tbody>
</table>
<br/>
<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" 
cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<b>
				Propostas de Migração da Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?>
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
    			<b> Migra para </b>
    		</td>    
    		<td>
    			<b> Justificativa </b>
    		</td>
    		<td>
    			<b> Status <br/> Decisão </b>
    		</td>
    		<td>
    			<b> Decisor </b>
    		</td>
		</tr>
	</thead>
	<tbody>
	<?
	
	$proponente		= 0;
	$pmastatus		= 0;
	$pmastatusspo	= 0;
	$usunome		= 0;
	$usunomeum		= 0;
	$usunomedois	= 0;
	$nlinhas		= 0;
	
	if ($acacodid)
	{
		
		$sql =	''.
		' SELECT ' .
			'u.usunome' . ' AS ' . 'proponente' .
			' , ' .
			'p.pmastatus' . ' AS ' . 'statusUm' .
			' , ' .
			'p.pmastatusspo' . ' AS ' . 'statusDois' .
			' , ' .
			'p.pmajustificativa' . ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' . ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' . ' AS ' . 'usunomedois' .
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
		' ORDER BY ' . 
			'p.pmastatusspo' . ' DESC ' .
			' , ' .
			'p.pmastatus' . ' DESC ' . 
			' , ' . 
			'u.usunome';
		
		$RS = $db->record_set($sql);
		$nlinhas = $db->conta_linhas($RS);
	}
	if ($nlinhas >= 0)
	{
		for ( $ie = 0 ; $ie <= $nlinhas ; $ie++ )
		{
			$resexc = $db->carrega_registro($RS,$ie);
			if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
			
			if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			
			print "
				<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" 
				onmouseout=\"this.bgColor='$marcado';\">
					<td>
						$proponente
					</td>
					<td>
						$destino
					</td>
					<td>
						$justificativa
					</td>
				";
			
			if ( $statusdois == 'Z' )
			{
				print '<td> MEC </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ( $statusdois == 'S' )
			{
				print '<td> SPO </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ( $statusum == 'D' )
			{
				print '<td> Unidade </td>';
				if ($usunomeum) print $usunomeum;
			}
			else
			{
				print '<td> Sem decisão </td>';
			}
			print "
				<td></td>
			</tr>
			";

		}
	}
	else
	{
		print  "
			<tr align='center'>
				<td colspan=4>
					Não há propostas de migração nesta ação
				</td>
			</tr>
			";
	}
**/
}


if ($fusao)
{
/**
	$_SESSION['prnfus']=1;
	?>
	</tbody>
</table>  

<br/>

<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" 
	cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<b> Propostas de Fusão na Ação <? print $prgcod.' - '.$acacod.' . '.$acadsc?> </b>
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
    			<b> Funde com </b> 
    		</td>
    		<td> 
    			<b> Justificativa </b> 
    		</td>
    		<td> 
    			<b> Status <br/> Decisão </b> 
    		</td>
    		<td> 
    			<b> Decisor </b> 
    		</td>
		</tr>
	</thead>
	<tbody>
	<?
	
	$proponente		= 0;
	$pfastatus		= 0;
	$pfastatusspo	= 0;
	$usunome		= 0;
	$usunomeum		= 0;
	$usunomedois	= 0;
	$nlinhas		= 0;
	
	if ( $acacodid )
	{
		$sql= "select u.usunome as proponente, p.pfastatus as statusUm, p.pfastatusspo as statusDois, p.pfajustificativa as justificativa, u2.usunome as usunomeum, u3.usunome as usunomedois, a.prgcod||'-'||a.acacod||' . '||a.acadsc as destino from elabrev.proposta_fusao_acao p inner join elabrev.ppaacao_proposta a on a.acaid=p.acaid_externa inner join seguranca.usuario u on u.usucpf=p.usucpf left join seguranca.usuario u2 on u2.usucpf=p.usucpfdecisor left join seguranca.usuario u3 on u3.usucpf=p.usucpfdecisorspo where p.acaid=$acacodid order by p.pfastatusspo desc, p.pfastatus desc, u.usunome";
		$RS = $db->record_set($sql);
		$nlinhas = $db->conta_linhas($RS);
	}
	
	if ($nlinhas >= 0)
	{
		for ($ie=0;$ie<=$nlinhas;$ie++)
		{
			$resexc = $db->carrega_registro($RS,$ie);
			if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
			if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			
			print "
				<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" 
				onmouseout=\"this.bgColor='$marcado';\">
					<td> 
						$proponente 
					</td>
					<td> 
						$destino 
					</td>
					<td> 
						$justificativa 
					</td>";
			
			if ($statusdois=='Z')
			{
				print '<td> MEC </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusdois=='S')
			{
				print '<td> SPO </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusum=='D')
			{
				print '<td> Unidade </td>';
				if ($usunomeum) print $usunomeum;
			}
			else
			{
				print '<td> Sem decisão</td>';
			}
			print "
				<td></td>
			</tr>";
		}
	}
	else
	{
		print  "
			<tr align='center'>
				<td colspan=4>
					Não há propostas de migração nesta ação
				</td>
			</tr>
				";
	}
**/
}
if ($alteracao)
{
	$_SESSION['prnalt']=1;
?>
	</tbody>
</table>

<br/>

<table  width="95%" class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<b>
				Propostas de Alteração de Atributos na Ação:  <? print $prgcod.' - '.$acacod.' . '.$acadsc?>
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
	 
	$proponente		= 0;
	$erastatus		= 0;
	$erastatusspo	= 0;
	$usunome		= 0;
	$usunomeum		= 0;
	$usunomedois	= 0;
	$nlinhas		= 0;
	
	if ( $acacodid )
	{
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
			'p.erastatusspo' . ' AS ' . 'statusDois' .
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
			'p.eracampo' . ' <> ' . 'acaobs' .
			' AND '.
			'p.eracod' . ' = ' . $acacodid .
			' AND ' .
			'p.eratabela' . ' = ' . '\'' . 'ppaacao_proposta' . '\'' .
		' ORDER BY ' . 
			'p.eracampo' .
			' , ' .
			'p.erastatusspo' . ' DESC ' . 
			' , ' . 
			'p.erastatus' . ' DESC ' . 
			' , '  .
			'u.usunome'
			;
			
		$RS = $db->record_set($sql);
		$nlinhas = $db->conta_linhas($RS);
	}

	if ($nlinhas >= 0)
	{
		for ($ie=0;$ie<=$nlinhas;$ie++)
		{
			$resexc = $db->carrega_registro($RS,$ie);
			if(is_array($resexc)) foreach($resexc as $k=>$v) ${$k}=$v;
			if (fmod($ie,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print "
			
			<tr bgcolor='$marcado' onmouseover=\"this.bgColor='#ffffcc';\" 
			onmouseout=\"this.bgColor='$marcado';\">
				<td>
					$proponente
				</td>
				<td>" .
					tradutor_att($atributo) .
				"</td>
				<td>
					$proposta
				</td>
				<td>
					$justificativa
				</td>
					";
			if ($statusdois=='Z')
			{
				print '<td> MEC </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusdois=='S')
			{
				print '<td> SPO </td>';
				if ($usunomedois) print $usunomedois;
			}
			else if ($statusum=='D')
			{
				print '<td> Unidade </td>';
				if ($usunomeum) print $usunomeum;
			}
			else
			{
				print '<td> Sem decisão </td>';
			}
			print "<td></td>
			</tr>";
		}
	}
	else
	{
		print  "
			<tr align='center'>
				<td colspan=4>
					Não há propostas de migração nesta ação
				</td>
			</tr>
				";
	}
}
	?>
	</tbody>
</table>
