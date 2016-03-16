<?
#
# Sistema Simec
# Setor responsável: SPO-MEC
# Desenvolvedor: Equipe Consultores Simec
# Analista: Gilberto Arruda Cerqueira Xavier
# Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
# Programador: Thiago Mata (e-mail thiago.henrique.mata@gmail.com )
# Módulo:gerapropostaprg.inc
# Finalidade: permitir a construção de relatorio de propostas de alteração em programas
#


class Propostas
{
	public static function getPropostasDeExclusao( $prgcodid , $arrFiltroSPO)
	{
		global $db;
		
		$arrReturn = array();
		
		if ( $prgcodid == 0 )
		{
			return $arrReturn;
		}
	
		$sql =	''.
		' SELECT  ' .
			'u.usunome' 			. ' AS ' . 'proponente' .
			' , ' .
			'p.pepstatus' 			. ' AS ' . 'statusUm' .
			' , ' .
			'p.pepstatusspo' 		. ' AS ' . 'statusdois' .
			' , ' .
			'p.pepjustificativa' 	. ' AS ' . 'justificativa' .
			' , ' .
			'u2.usunome' 			. ' AS ' . 'usunomeum' .
			' , ' .
			'u3.usunome' 			. ' AS ' . 'usunomedois' .
		' FROM ' .
			'elabrev.proposta_exclusao_programa p' .
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
			'p.prgid' . ' = ' . '\'' . $prgcodid . '\'' .
		' AND ' . 
			'p.pepstatusspo' . ' IN ' . ' ( \'' . implode( '\',\'' , $arrFiltroSPO ) . '\' ) ' .
		' ORDER BY ' .
			'p.pepstatusspo' 	. ' DESC ' .
			' , ' .
			'p.pepstatus' 		. ' DESC ' .
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
			$arrElement[ 'prog'] 		    = $prgcodid;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsexclusao'][] =$arrReturn;
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeExclusao( $intProgId = 0  , $prgcod, $prgdsc , $arrFiltroSPO )
	{
		?>
		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Exclusão do Programa <?= $prgcod ?> -  <?= $prgdsc ?>
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
		
		$arrPropostas = self::getPropostasDeExclusao( $intProgId , $arrFiltroSPO );
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
						Não há propostas de exclusão neste programa
					</td>
				</tr>			
			<?
		}
		
		?>
			</tbody>
		</table>		
		<?
	}

	public static function getPropostasDeAlteracao( $intProgId  , $arrFiltroSPO )
	{
		global $db;
		$arrReturn = array();

		if ( $intProgId == 0 )
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
			'p.eracod' . ' = ' . $intProgId .
			' AND ' .
			'p.eratabela' . ' = ' . '\'' . 'ppaprograma_proposta' . '\'' .
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
			$arrElement[ 'prog'] 		        = $intProgId;
			
			$arrReturn[] = $arrElement;
		}
		$_SESSION['rsalteracao'][] = $arrReturn;
		return $arrReturn;
	}
	
	public static function montaHtmlPropostasDeAlteracao( $intProgId = 0  , $prgcod, $prgdsc , $arrFiltroSPO )	
	{
		?>

		<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
					<b>
						Propostas de Alteração de Atributos no Programa <?= $prgcod ?> - <?= $prgdsc ?>
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
				
		$arrPropostas = self::getPropostasDeAlteracao( $intProgId, $arrFiltroSPO );
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
						Não há propostas de alteração neste programa
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
		
	public static function montaHtmlDasPropostas( $intProgId , $boolExclusao , $boolAlteracao , $arrFiltroSPO )
	{
		global $db;
		$sql=	' SELECT ' . 
					'prgcod' . 
					' , ' . 
					'prgdsc' . 
				' FROM ' . 
					'elabrev.ppaprograma_proposta' . 
				' WHERE ' . 
					'prgid' . ' = ' . $intProgId;
		$linha = $db->pegalinha( $sql );

		$intPrgCode 	= $linha[ 'prgcod' ];
		$strPrgDsc 	= $linha[ 'prgdsc' ];

		if( !$boolExclusao && !$boolAlteracao )
		{
			// nenhum elemento de filtro foi marcado logo deve-se mostrar a todos //	
			$boolExclusao 	= true;
			$boolAlteracao	= true;
		}
		
		if ( $boolExclusao )
		{
			Propostas::montaHtmlPropostasDeExclusao( $intProgId , $intPrgCode, $strPrgDsc , $arrFiltroSPO);
		}

		if ( $boolAlteracao )
		{
			Propostas::montaHtmlPropostasDeAlteracao( $intProgId , $intPrgCode, $strPrgDsc, $arrFiltroSPO );
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
if ($_REQUEST['pcod'])
{
	$_REQUEST[ 'cbexclusao' ] =1;
    $_REQUEST[ 'cbalteracao' ] =1;
    $_REQUEST['prgid'][] = $_REQUEST['pcod'];
    $titulo_modulo='Propostas de Alteração em Programa';
	monta_titulo($titulo_modulo,'');
	?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
	<?
}

$boolExclusao	= $_REQUEST[ 'cbexclusao' ];
$boolAlteracao	= $_REQUEST[ 'cbalteracao' ];
if ($_REQUEST[ 'cbexclusao' ]) $_SESSION['filtrorelat'][] = 'Exclusão';
if ($_REQUEST[ 'cbalteracao']) $_SESSION['filtrorelat'][] = 'Alteração';


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
unset($_SESSION['rsalteracao']);
unset($_SESSION['prgidrelat']);
unset($_SESSION['mostrou']);

if ($_REQUEST['prgid'][0]=='') unset($_REQUEST['prgid'][0]);


if ($_REQUEST['prgid'])
{
	foreach ($_REQUEST['prgid'] as $intPrgId)
	{
	   $sql = "select p.prgcod||'-'||p.prgdsc as filtro from elabrev.ppaprograma_proposta p where p.prgid=$intPrgId";
	   $programas = $db->pegaUm($sql);
	   $_SESSION['filtrorelat'][]=' Programa: '.$programas;	   
	}
}
$_REQUEST['prgid'] = array_unique($_REQUEST['prgid']);


foreach ( $_REQUEST['prgid'] as $intProgId )
{
	if ( $_SESSION['filtrorelat'] and ! $_SESSION['mostrou'])
	{
		$_SESSION['mostrou']=1;
		Propostas::MostraFiltro( );
	}
	$_SESSION['prgidrelat'][] = $intProgId ;
	Propostas::montaHtmlDasPropostas
	( 
		$intProgId 		, 
		$boolExclusao 	, 
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
	janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_prop_prg&acao=R";
}	
</script>
