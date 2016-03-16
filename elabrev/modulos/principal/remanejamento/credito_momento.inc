<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, 'Suplementação e Cancelamento de Créditos Adicionais' );


function formatarDataMomento( $data )
{
	$data = explode( ' ', $data );
	$data = explode( '-', $data[0] );
	return $data[2] . '/' . $data[1] . '/' . $data[0];
}

function desformatarDataMomento( $data )
{
	$data = explode( '/', $data );
	return $data[2] . '-' . $data[1] . '-' . $data[0]; 
}

// remove momento
if ( $_REQUEST['remover_momento'] )
{
	$mcrid = (integer) $_REQUEST['remover_momento'];
	$sql = <<<EOT
		update elabrev.momentocredito
		set mcrstatus = 'I'
		where mcrid = $mcrid
EOT;
	$db->executar( $sql );
	$db->commit();
}

// cria variáveis para edição
// caso $edi_ativa esteja com valor falso a tela a edição não é ativada
$edi_ativa    = false;
$edi_mcrid            = '';
$edi_mcrdsc           = '';
$edi_mcrdatainiciouo  = '';
$edi_mcrdatafimuo     = '';
$edi_mcrdatainiciocgo = '';
$edi_mcrdatafimcgo    = '';
if ( isset( $_REQUEST['edi_mcrid'] ) )
{
	$gravar = false;
	$edi_ativa = true;
	$edi_mcrid = (integer) $_REQUEST['edi_mcrid'];
	
	// captura dados do requests
	if ( isset( $_REQUEST['edi_gravar'] ) )
	{
		$edi_mcrdsc           = (string) $_REQUEST['edi_mcrdsc'];
		$edi_mcrdatainiciouo  = desformatarDataMomento( $_REQUEST['edi_mcrdatainiciouo'] );
		$edi_mcrdatafimuo     = desformatarDataMomento( $_REQUEST['edi_mcrdatafimuo'] );
		$edi_mcrdatainiciocgo = desformatarDataMomento( $_REQUEST['edi_mcrdatainiciocgo'] );
		$edi_mcrdatafimcgo    = desformatarDataMomento( $_REQUEST['edi_mcrdatafimcgo'] );
		
		$gravar = true;
	}
	// captura dados do banco
	else if ( $edi_mcrid )
	{
		$sql = "select * from elabrev.momentocredito where mcrid = " . $edi_mcrid;
		$momento_edicao = $db->recuperar( $sql );
		if ( $momento_edicao )
		{
			$edi_mcrdsc           = (string) $momento_edicao['mcrdsc'];
			$edi_mcrdatainiciouo  = reset( explode( ' ', $momento_edicao['mcrdatainiciouo'] ) );
			$edi_mcrdatafimuo     = reset( explode( ' ', $momento_edicao['mcrdatafimuo'] ) );
			$edi_mcrdatainiciocgo = reset( explode( ' ', $momento_edicao['mcrdatainiciocgo'] ) );
			$edi_mcrdatafimcgo    = reset( explode( ' ', $momento_edicao['mcrdatafimcgo'] ) );
		}
	}
	
	// realiza gravação
	if ( $gravar )
	{
		if ( $edi_mcrid )
		{
			$sql = <<<EOT
				update elabrev.momentocredito
				set
					mcrdsc           = '%s',
					mcrdatainiciouo  = '%s',
					mcrdatafimuo     = '%s',
					mcrdatainiciocgo = '%s',
					mcrdatafimcgo    = '%s'
				where mcrid = %d
EOT;
		}
		else
		{
			$sql = "select nextval( 'elabrev.momentocredito_mcrid_seq' )";
			$edi_mcrid = $db->recuperar( $sql );
			$sql = <<<EOT
				insert into elabrev.momentocredito
				( mcrdsc, mcrdatainiciouo, mcrdatafimuo, mcrdatainiciocgo, mcrdatafimcgo, mcrid )
				values
				( '%s', '%s', '%s', '%s', '%s', %d )
EOT;
		}
		$sql = sprintf(
			$sql,
				addslashes( $edi_mcrdsc ),
				addslashes( $edi_mcrdatainiciouo ),
				addslashes( $edi_mcrdatafimuo ),
				addslashes( $edi_mcrdatainiciocgo ),
				addslashes( $edi_mcrdatafimcgo ),
				$edi_mcrid
		);
		$db->executar( $sql );
		$db->commit();
		$edi_ativa = false;
	}
	
}

// captura todos os momentos ativos
$sql = <<<EOT
	select
		*
	from elabrev.momentocredito
	where
		mcrstatus = 'A'
	order by
		mcrdatainiciocgo desc
EOT;
$momentos = $db->carregar( $sql );
$momentos = $momentos ? $momentos : array();

//$momentos = array();

?>
<?php if ( $edi_ativa ) : ?>
	<script type="text/javascript" src="../includes/calendario.js"></script>
	<script type="text/javascript">
		
		function gravarEdicao()
		{
			var formulario = document.formulario;
			if ( !formulario.edi_mcrdsc.value )
			{
				alert( 'Preencha o campo nome!' );
				formulario.edi_mcrdsc.focus();
				return;
			}
			if ( !formulario.edi_mcrdatainiciocgo.value )
			{
				alert( 'Preencha o campo de data de início da CGO!' );
				formulario.edi_mcrdatainiciocgo.focus();
				return;
			}
			if ( !formulario.edi_mcrdatafimcgo.value )
			{
				alert( 'Preencha o campo de data de término da CGO!' );
				formulario.edi_mcrdatafimcgo.focus();
				return;
			}
			if ( !formulario.edi_mcrdatainiciouo.value )
			{
				alert( 'Preencha o campo de data de início da UO!' );
				formulario.edi_mcrdatainiciocgo.focus();
				return;
			}
			if ( !formulario.edi_mcrdatafimuo.value )
			{
				alert( 'Preencha o campo de data de término da UO!' );
				formulario.edi_mcrdatafimuo.focus();
				return;
			}
			formulario.submit();
		}
		
		function cancelarEdicao()
		{
			window.location.href = '?modulo=principal/remanejamento/credito_momento&acao=A';
		}
		
	</script>
	<form name="formulario" action="" method="post">
		<input type="hidden" name="edi_gravar" value="1"/>
		<input type="hidden" name="edi_mcrid" value="<?php echo $edi_mcrid; ?>"/>
		<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: none;">
			<tr bgcolor="#bfbfbf">
				<td colspan="2" align="center">
					<?php echo !$edi_mcrid ? 'Cadastrar Momento de Crédito Adicional' : 'Editar Momento de Crédito Adicional' ; ?>
				</td>
			</tr>
			<tr>
				<td width="20%" align="right" class="SubTituloDireita">
					Nome
				</td>
				<td width="80%">
					<input type="text" name="edi_mcrdsc" value="<?php echo $edi_mcrdsc; ?>" class="normal"/>
					<img border='0' src="../imagens/obrig.gif" title="Campo obrigatório"/>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">
					Período CGO
				</td>
				<td>
					<?php
						echo campo_data( 'edi_mcrdatainiciocgo', 'S', 'S', '', 'S' );
					?>
					&nbsp;até&nbsp;
					<?php
						echo campo_data( 'edi_mcrdatafimcgo', 'S', 'S', '', 'S' );
					?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">
					Período UO
				</td>
				<td>
					<?php
						echo campo_data( 'edi_mcrdatainiciouo', 'S', 'S', '', 'S' );
					?>
					&nbsp;até&nbsp;
					<?php
						echo campo_data( 'edi_mcrdatafimuo', 'S', 'S', '', 'S' );
					?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">&nbsp;</td>
				<td >
					<input type="button" name=gravar value="Gravar" onclick="gravarEdicao();"/>
					&nbsp;&nbsp;
					<input type="button" name="cancelar" value="Cancelar" onclick="cancelarEdicao();"/>
				</td>
			</tr>
		</table>
	</form>
<?php endif; ?>
<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center" >
	<?php if ( count( $momentos ) ) : ?>
		<tr bgcolor="#bfbfbf">
			<td rowspan="2" align="center" width="90">Ações</td>
			<td rowspan="2" align="center">Momento</td>
			<td colspan="2" align="center">CGO</td>
			<td colspan="2" align="center">Unidades Orçamentárias</td>
		</tr>
		<tr bgcolor="#bfbfbf">
			<td align="center" width="90">Início</td>
			<td align="center" width="90">Término</td>
			<td align="center" width="90">Início</td>
			<td align="center" width="90">Término</td>
		</tr>
		<?php $cor = '#ffffff'; ?>
		<?php foreach( $momentos as $momento ) : ?>
			<?php $cor = $cor == '#ffffff' ? '#f0f0f0' : '#ffffff'; ?>
			<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
				<td align="center">
					<a
						href="?modulo=principal/remanejamento/credito_momento&acao=A&edi_mcrid=<?php echo $momento['mcrid']; ?>"
					><img align="absmiddle" border="0" src="../imagens/alterar.gif"/></a>
					&nbsp;&nbsp;
					<a
						href="?modulo=principal/remanejamento/credito_momento&acao=A&remover_momento=<?php echo $momento['mcrid']; ?>"
						onclick="return confirm( 'Deseja realmente remover o momento \'<?php echo $momento['mcrdsc']; ?>\' ?' );"
					><img align="absmiddle" border="0" src="../imagens/excluir.gif"/></a>
				</td>
				<td align="left">
					<?php echo $momento['mcrdsc']; ?>
				</td>
				<td align="center">
					<?php echo formatarDataMomento( $momento['mcrdatainiciocgo'] ); ?>
				</td>
				<td align="center">
					<?php echo formatarDataMomento( $momento['mcrdatafimcgo'] ); ?>
				</td>
				<td align="center">
					<?php echo formatarDataMomento( $momento['mcrdatainiciouo'] ); ?>
				</td>
				<td align="center">
					<?php echo formatarDataMomento( $momento['mcrdatafimuo'] ); ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td align="center">
				Nenhum momento foi encontrado
			</td>
		</tr>
	<?php endif; ?>
</table>
<table width="90%" class="tabela" cellspacing="1"  cellpadding="3" bgcolor="#f5f5f5" align="center" style="border-top: none;">
	<tr>
		<td align="center" valign="center">
			<a href="?modulo=principal/remanejamento/credito_momento&acao=A&edi_mcrid=">
				<img align="absmiddle" border="0" src="../imagens/gif_inclui.gif"/>
				Criar novo momento
			</a>
		</td>
	</tr>
</table>