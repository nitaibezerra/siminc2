<?php

cte_verificaSessao();

$dimid = $_REQUEST['dimid'];
$prgid = $_REQUEST['prgid'];


$podeVerQuestaoPontual = cte_podeVerQuestaoPontual( $_SESSION["inuid"], $prgid ) || !$dimid;
$podeEditarQuestaoPontual = cte_podeEditarQuestaoPontual( $_SESSION["inuid"], $prgid ) || !$dimid;

if ( !$podeVerQuestaoPontual ) 
{
	header('Location: ?modulo=principal/estrutura_avaliacao&acao=A' );
	exit();
}
$habil = $podeEditarQuestaoPontual ? "S" : "N";

$perfis = cte_arrayPerfil();
if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) 	|| 
								in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) || 
								in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
	//não altera
	$habil = 'N';
} 

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo );


if ( $dimid && $prgid )
{
	$sql = "select d.dimcod, d.dimdsc, p.prgcod, p.prgdsc
			from cte.pergunta p
				inner join cte.dimensao d on d.dimid = p.dimid
			where d.dimid = " . $dimid . " 
			and p.prgid = " . $prgid;
	
	$res = $db-> pegaLinha( $sql );
	extract( (array) $res ); // ...
}
elseif( $prgid ){
	$sql = "select d.dimcod, d.dimdsc, p.prgcod, p.prgdsc
			from cte.pergunta p
				left join cte.dimensao d on d.dimid = p.dimid
			where d.dimid is null
			and p.prgid = " . $prgid;
	
	$res = $db-> pegaLinha( $sql );
	extract( (array) $res );
}
else
{
	exit();
}

$sql = sprintf(
	"select * from cte.resposta r where r.prgid = %d and r.inuid = %d",
	$prgid,
	$_SESSION['inuid']
);
$resposta = $db->pegaLinha( $sql );
if ( $resposta && $_REQUEST['formulario'] )
{
	$resposta = array_intersect_key( $_REQUEST, $resposta );
}
extract( (array) $resposta ); // ...

if ( $_REQUEST['formulario'] && $podeEditarQuestaoPontual )
{
	
	if ( $resposta['rspid'] )
	{
		$sql = sprintf(
			"update cte.resposta set usucpf = '%s', rspdsc = '%s', rspdata = '%s' where rspid = %d",
			$_SESSION['usucpf'],
			$_REQUEST['rspdsc'],
			date( "Y-m-d H:i:s" ),
			$resposta['rspid']
		);
	}
	else
	{
		$sql = sprintf(
			"insert into cte.resposta ( prgid, usucpf, inuid, rspdsc ) values ( %d, '%s', %d, '%s' ) ",
			$_REQUEST['prgid'],
			$_SESSION['usucpf'],
			$_SESSION['inuid'],
			
			$_REQUEST['rspdsc']
		);
	}
	if ( !$db->executar( $sql ) )
	{
		$db->rollback();
		$db->insucesso( "Ocorreu um erro ao cadastrar a pergunta." );
	}
	else
	{
		$db->commit();
		$_REQUEST['acao'] = 'A';
		$db->sucesso( "principal/estrutura_avaliacao" );
	}
}

// calcula programas e dimensões próximos e anteriores
// solução temporária
// @todo inventar outro jeito sem carregar todas as questões do banco
$dimcod;
$prgcod;
$sql = "
	select
		d.dimid,
		p.prgid,
		d.dimcod,
		p.prgcod
	from cte.pergunta p
		inner join cte.dimensao d on d.dimid = p.dimid
		inner join cte.instrumento i on i.itrid = d.itrid
		inner join cte.instrumentounidade iu on iu.itrid = i.itrid
	where
		iu.inuid = " . ( (integer) $_SESSION['inuid'] ) . "
	order by
		cast( d.dimcod as integer ),
		cast( p.prgcod as integer )
";
$lista = $db->carregar( $sql );
$lista = is_array( $lista ) ? $lista : array();
$anterior = null;
$proximo = null;
foreach ( $lista as $chave => $item )
{
	if ( $item['dimcod'] == $dimcod && $item['prgcod'] == $prgcod )
	{
		$anterior = $lista[$chave-1];
		$proximo = $lista[$chave+1];
	}
}
if ( $anterior )
{
	$dimAnterior = $anterior['dimid'];
	$prgAnterior = $anterior['prgid'];
}
if ( $proximo )
{
	$dimProximo = $proximo['dimid'];
	$prgProximo = $proximo['prgid'];
}
// fim calcula programas e dimensões próximos e anteriores

?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/TextEditor.js"></script>
<script language="javascript" type="text/javascript">
       TextEditor.init();
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<?php if( $resposta['rspid'] ): ?>
	<input type="hidden" name="rspid" value="<?= $resposta['rspid'] ?>"/>
	<?php endif; ?>
	<input type="hidden" name="prgid" value="<?= $_REQUEST['prgid'] ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<?php if( $dimcod ){ ?>
			<tr>
				<td class="SubTituloDireita">Dimensão:</td>
				<td>
					<?=$dimcod.'.  '.$dimdsc;?>
				</td>
			</tr>
		<?php } ?>
		<tr>
			<td class="SubTituloDireita">Pergunta:</td>
			<td>
				<?=$prgcod.'.  '.$prgdsc;?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Resposta:</td>
			<td>
				<?php
					if( $habil == 'S' )	
						echo '<textarea name="rspdsc" class="cte" rows="20">'.$rspdsc.'</textarea>';
					else 
						echo $rspdsc;
				?>
			</td>
		</tr>
		
		<!-- BOTOES -->
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<?php if( $habil == 'S' ): ?>
				<!-- SALVAR -->
				<div style="float: left;">
					<input type='button' class="botao" name='cadastra' value='Salvar' onclick="salvarCriterio()"/>
				</div>
				<?php endif; ?>
				
				<!-- NAVEGACAO PROXIMO ANTERIOR -->
				<div style="float: right;">
					<?php if( $dimAnterior && $prgAnterior ): ?>
						<input
							type="button"
							class="botao"
							name="anterior"
							value="Anterior"
							onclick="questaoAnterior();"
							title="Questão anterior"
						/>
					<?php endif; ?>
					<?php if( $dimProximo && $prgProximo ): ?>
						<input
							type="button"
							class="botao"
							name="proximo"
							value="Próximo"
							onclick="proximaQuestao();"
							title="Próxima questão"
						/>
					<?php endif; ?>
				</div>
				
			</td>
		</tr>
		
	</table>
</form>

<script language="javascript" type="text/javascript">

function salvarCriterio(){
	if ( validar_formulario_criterio() ) {
		document.formulario.submit();
	}
}

function validar_formulario_criterio(){
	var validacao = true;
	var mensagem = 'Os seguintes campos não foram preenchidos:';
	var resposta = trim( tinyMCE.getContent('rspdsc') );
	if ( resposta == '' ) {
		mensagem += '\nResposta';
		validacao = false;
	}
	if ( !validacao ) {
		alert( mensagem );
	}
	return validacao;
}


var houveModificacao = false;

function marcarAlteracao(){
	houveModificacao = true;
}

function verificarAlteracao(){
	return !houveModificacao || confirm( 'Atenção! Caso confirme a ação as alterações não gravadas serão perdidas.' );
}

function questaoAnterior(){
	if( !verificarAlteracao() ){
		return;
	}
	window.location = "?modulo=principal/questoes/questoespontuais&acao=C&dimid=<?= $dimAnterior ?>&prgid=<?= $prgAnterior ?>";
}

function proximaQuestao(){
	if( !verificarAlteracao() ){
		return;
	}
	window.location = "?modulo=principal/questoes/questoespontuais&acao=C&dimid=<?= $dimProximo ?>&prgid=<?= $prgProximo ?>";
} 

</script>
<script language="javascript" type="text/javascript">
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
	
</script>