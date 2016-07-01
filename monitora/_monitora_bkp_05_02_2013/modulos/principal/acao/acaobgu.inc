<?php

/**

 * @author Gilberto Arruda Cerqueira Xavier
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 * @package Monitoramento 
 * @since 15/02/2007
 */

// captura os dados de entrada
$_REQUEST['descricao'] = trim( $_REQUEST['descricao'] );

if ( $_REQUEST['lista'] ) {
	//
	$sql = sprintf(
		"update monitora.acao set acasnbgu = true where acasnrap = 'f' and acastatus ='A' and prgano = '%s' and ( prgcod || acacod ) in ( '%s' )",
		$_SESSION['exercicio'],
		implode( "','", explode( ',', trim( $_REQUEST['bgusim'], ',' ) ) )
	);
	$sim = (boolean) $db->executar( $sql );
	$sql = sprintf(
		"update monitora.acao set acasnbgu = false where acasnrap = 'f' and acastatus ='A' and prgano = '%s' and ( prgcod || acacod ) in ( '%s' )",
		$_SESSION['exercicio'],
		implode( "','", explode( ',', trim( $_REQUEST['bgunao'], ',' ) ) )
	);
	$nao = (boolean) $db->executar( $sql );
	if ( $sim && $nao ) {
		$db->commit();
		$db->sucesso( $modulo, '&bgu='. $_REQUEST['bgu'] .'&descricao='. $_REQUEST['descricao'] );
	} else {
		$db->insucesso( 'Ocorreu uma falha', '&bgu='. $_REQUEST['bgu'] .'&descricao='. $_REQUEST['descricao'], $modulo );
	}
}

// exibe o cabeçalho da página
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';
monta_titulo( $titulo_modulo, 'Lista de Ações Ativas' );

?>
<form method="post" name="filtro">
	<input type="hidden" name="filtro" value="1"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<td>
				<?php
					$descricao = $_REQUEST['descricao'];
					echo campo_texto( 'descricao', '', '', '', 50, 50, '', '' );
				?>
			</td>
		</tr>
		<tr bgcolor="#F2F2F2">
			<td align='right' class="SubTituloDireita">BGU</td>
			<td>
				<input id="bguqualquer" type="radio" name="bgu" value="" <?= $_REQUEST['bgu'] == '' ? 'checked="checked"' : "" ?>/>
				<label for="bguqualquer">Todos</label>
				<input id="bgusim" type="radio" name="bgu" value="true" <?= $_REQUEST['bgu'] == 'true' ? 'checked="checked"' : "" ?>/>
				<label for="bgusim">Sim</label>
				<input id="bgunao" type="radio" name="bgu" value="false" <?= $_REQUEST['bgu'] == 'false' ? 'checked="checked"' : "" ?>/>
				<label for="bgunao">Não</label>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type='button' class="botao" name='consultar' value='Filtrar' onclick="filtrarAcoes()"/></td>
		</tr>
	</table>
</form>
<br/>
<form method="post" name="formulariolistagem">
	<input type="hidden" name="lista" value="1"/>
	<input type="hidden" name="bgusim" value=""/>
	<input type="hidden" name="bgunao" value=""/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td>
				<input type='button' class="botao" name='a' value='Aplicar' onclick="aplicarAlteracao()"/>
				&nbsp;As ações marcadas serão gravadas como BGU
			</td>
		</tr>
	</table>
</form>
	<?php
		$filtro = '';
		
		
		if ( strlen( $_REQUEST['descricao'] ) ) {
			$filtro = " and lower( prgcod || acacod || acadsc ) like '%". str_replace( ' ', '%', $_REQUEST['descricao'] ) ."%' ";
		}
		/*
		if ( $_REQUEST['descricao'] ) {
			$palavras = explode( ' ', $_REQUEST['descricao'] );
			$filtros = array();
			foreach ( $palavras as &$palavra ) {
				if ( strlen( $palavra ) < 3 ) {
					continue;
				}
				array_push( $filtros, " lower( prgcod || acacod || acadsc ) like '%". strtolower( $palavra ) ."%' " );
			}
			if ( count( $filtros ) > 0 ) {
				$filtro = ' and ('. implode( ' and ', $filtros ) .')';
			}
		}
		*/
		$sql = sprintf(
			"select distinct %s as bgu, prgcod || ' ' || acacod as codigo, acadsc from monitora.acao where acasnrap = 'f' and acastatus ='A' and prgano = '%s' %s %s order by codigo",
			"case when acasnbgu = true then '<input type=\"checkbox\" name=\"acasnbgu[]\" checked=\"checked\" value=\"' || prgcod || acacod || '\" onchange=\"atualizarListaDeAlteracoes( this );\"/>' else '<input type=\"checkbox\" name=\"acasnbgu[]\" value=\"' || prgcod || acacod || '\" onchange=\"atualizarListaDeAlteracoes( this );\"/>' end",
			$_SESSION['exercicio'],
			$_REQUEST['bgu'] != '' ? 'and acasnbgu = '.$_REQUEST['bgu'] : '',
			$filtro 
		);
		$cabecalho = array( 'BGU', 'Código', 'Descrição' );
		$db->monta_lista( $sql, $cabecalho, 50, 20, '', '' ,'' );
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td>
				<input type='button' class="botao" name='b' value='Aplicar' onclick="aplicarAlteracao()"/>
				&nbsp;As ações marcadas serão gravadas como BGU
			</td>
		</tr>
	</table>
<script language="javascript">

	var dav = navigator.appVersion;
	var IE = document.all ? true : false;
	var IE6 = dav.indexOf("MSIE 6.0") >= 0;
	var IE7 = dav.indexOf("MSIE 7.0") >= 0;

	var listaSim = new Array();
	var listaNao = new Array();
	function atualizarListaDeAlteracoes( campo ) {
		if ( campo.checked ) {
			listaSim[listaSim.length] = campo.value;
			for ( var indice = 0; indice < listaNao.length; indice++ ) {
				if ( listaNao[indice] == campo.value ) {
					listaNao[indice] = null;
				}
			}
		} else {
			listaNao[listaNao.length] = campo.value;
			for ( var indice = 0; indice < listaSim.length; indice++ ) {
				if ( listaSim[indice] == campo.value ) {
					listaSim[indice] = null;
				}
			}
		}
	}

	function aplicarAlteracao() {
		for ( var indice = 0; indice < listaSim.length; indice++ ) {
			if ( listaSim[indice] != null ) {
				document.formulariolistagem.bgusim.value += ',' + listaSim[indice];
			}
		}
		for ( var indice = 0; indice < listaNao.length; indice++ ) {
			if ( listaNao[indice] != null ) {
				document.formulariolistagem.bgunao.value += ',' + listaNao[indice];
			}
		}
		document.formulariolistagem.submit();
	}

	function filtrarAcoes() {
		document.filtro.submit();
	}

	function alterarTodos( valor )
	{
		var indice;
		var campo = null;

		if( !IE )
		{
			for ( indice=0; indice < document.formlista.elements.length; indice++ )
			{
				campo = document.formulariolistagem.elements[indice];
				if ( campo.name == 'acasnbgu[]' )
				{
					campo.checked = valor;
					atualizarListaDeAlteracoes( campo );
				}
			}		
		}
		else
		{
			campos = document.body.getElementsByTagName('input');
			for( var  i = 0 ; i < campos.length ; i++ )
			{
				if( campos[ i ].name == 'acasnbgu[]' ) {
					if( valor )
					{
						campos[ i ].setAttribute( 'checked' , 'checked' );
						campos[ i ].checked = 'checked';
					}
					else
					{
						campos[ i ].setAttribute( 'checked' , '' );
						campos[ i ].checked = undefined;
					}
					atualizarListaDeAlteracoes( campos[ i ] );
				}
			}
		}

	}

	if( !IE )
	{
		for ( indice=0; indice < document.formulariolistagem.elements.length; indice++ )
		{
			campo = document.formulariolistagem.elements[indice];
			if ( campo.name == 'acasnbgu[]' )
			{
				atualizarListaDeAlteracoes( campo );
			}
		}		
	}
	else
	{
		campos = document.body.getElementsByTagName('input');
		for( var  i = 0 ; i < campos.length ; i++ )
		{
			if( campos[ i ].name == 'acasnbgu[]' ) {
				atualizarListaDeAlteracoes( campos[ i ] );
			}
		}
	}

</script>