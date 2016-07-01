<?php
include_once APPRAIZ."includes/classes/Modelo.class.inc";
include_once APPRAIZ."contratos/classes/FaturaContrato.class.inc";
if($_REQUEST['requisicao'] && $_REQUEST['classe'])
{
	$n = new $_REQUEST['classe']();
	$n->$_REQUEST['requisicao']();
}
unset($_SESSION['administrativo']['contratos']['notafiscal']['ftcid']);

//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
montaCabecalhoContrato($_SESSION['ctrid']);

// Por padrão os campos vem habilitados
$desabilitado   = false;
$somenteLeitura = 'S';

$perfis 			= arrayPerfil();
$arPerfisBloqEdicao = array(PERFIL_EQUIPE_TECNICA_UNIDADE,
							PERFIL_CONSULTA_UNIDADE,
							PERFIL_CONSULTA_GERAL);

$arDif = array_diff($perfis, $arPerfisBloqEdicao);
// Bloqueia os campos caso o usuáro só tenha os perfis restritos
if( count($arDif) == 0 ) {
	$desabilitado 	= true;
	$somenteLeitura = 'N';
}
?>
<style>.link{cursor:pointer}</style>
<script>
	function addNovaNota()
	{
		window.location='contratos.php?modulo=principal/addNotaContratos&acao=A';
	}
	function editarFaturamento(ftcid)
	{
		window.location='contratos.php?modulo=principal/addNotaContratos&acao=A&ftcid='+ftcid;
	}
	function excluirFaturamento(ftcid)
	{
		if(confirm("Deseja realmente excluir este Pagamento?")){
			window.location='contratos.php?modulo=principal/faturamentoContratos&acao=A&classe=FaturaContrato&requisicao=excluirFaturamento&ftcid='+ftcid;
		}
	}

	function wf_exibirHistorico( docid )
	{
		var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}	
</script>
<form name=formulario id=formulario method=post >
	<input type="hidden" name="requisicao" id="requisicao" value=""> 
	<input type="hidden" name="classe" id="classe" value=""> 
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Buscar:</td>
	        <td>
	        		<?php
						$arrAtributos = false;
						$arrAtributos['name'] = "pesquisar";
						$arrAtributos['obrigatorio'] = false;
						$arrAtributos['habilitado'] = true;
						$arrAtributos['size'] = 60;
						$arrAtributos['maxsize'] = 60;
						$arrAtributos['align'] = "left";
						$arrAtributos['value'] = $_POST['pesquisar'];
						echo campo_texto($arrAtributos)
					?>
	        </td>      
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' ></td>
	        <td>
	        	<input type="submit" name="btn_buscar" value="Buscar" >
	        	<?php if( $_POST['pesquisar'] ):?>
	        		<input type="button" name="btn_novo" value="Ver Todas" onclick="window.location=window.location" >
	        	<?php endif;?>
	        	<?php if ( !$desabilitado ){ ?>
	        	<input type="button" name="btn_novo" value="Novo pagamento" onclick="addNovaNota()" >
	        	<?php } ?>
	        </td>
	    </tr> 
	</table>
</form>
<?php
 
$fatura = new FaturaContrato();
$fatura->listaFaturamento($_SESSION['ctrid']);
?>
<?php if($_SESSION['administrativo']['contratos']['notafiscal']['alert']): ?>
<script>
	alert('<?php echo $_SESSION['administrativo']['contratos']['notafiscal']['alert'] ?>');
	<?php unset($_SESSION['administrativo']['contratos']['notafiscal']['alert']) ?>
</script>
<?php endif;?>