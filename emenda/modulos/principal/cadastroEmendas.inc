<?php

$respAutor = verificaAutor( "cadastroEmendas" );

unset( $_SESSION["emenda"]["emdid"] );

if ( $_REQUEST["emeid"] || $_SESSION["emenda"]["emeid"] ){
	
	$_SESSION["emenda"]["emeid"] = $_REQUEST["emeid"] ? $_REQUEST["emeid"] : $_SESSION["emenda"]["emeid"];  
	$dadosEmenda = pegaDadosEmenda( $_SESSION["emenda"]["emeid"] );
	
}else{
	
	print "<script>"
		. "		alert('Favor selecionar uma emenda!');"
		. "		history.back(-1);"
		. "</script>";

}

validaSessionPTA( $_SESSION["emenda"]["emeid"], false);

$emeid = $_SESSION["emenda"]["emeid"];

if ( $_REQUEST["cadastra"] == "cadastra" ){
	atualizaEmenda( $_REQUEST );
}

require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Dados da Emenda', '');
?>

<script type="text/javascript" src="js/emenda.js"></script>

<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="cadastra" name="cadastra"/>
	<input type="hidden" value="<?=$emeid; ?>" name="emeid"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita" width="200px">Número:</td>
			<td>
				<?php echo $dadosEmenda["numero"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Autor:</td>
			<td>
				<?php echo $dadosEmenda["autor"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Unidade Orçamentária:</td>
			<td>
				<?php echo $dadosEmenda["unidade"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Funcional Programática:</td>
			<td>
				<?php echo $dadosEmenda["funcional"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Subtítulo</td>
			<td>
				<?php echo $dadosEmenda["subtitulo"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Responsável:</td>
			<td>
				<?php					
					$retorno = verificaEmendaExisteVinculoPTA( $emeid ); 
					if( $retorno ){						
						$habilita = 'N';
					} else {
						$habilita = $respAutor["habilitadoEmendas"];
					}
				
					$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"] );
					$filtroResid    = count($residPossiveis) > 0 ? " WHERE resid in (" . implode( ", ", $residPossiveis ) . ")" : "";
					
					$resid = $dadosEmenda["responsavel"];
					
					$sql = "SELECT
								resid as codigo,
								resdsc as descricao
							FROM
								emenda.responsavel
							{$filtroResid}
							ORDER BY
								descricao";
					
					$db->monta_combo("resid", $sql, $habilita, "Selecione...", '', '', '', '', 'N','resid');
					
					$perfil = pegaPerfilArray( $_SESSION["usucpf"] );
					
					if( in_array( SUPER_USUARIO, $perfil ) || in_array( GESTOR_EMENDAS, $perfil ) || in_array( LIBERAR_SENHA, $perfil ) || in_array( ASSESSORIA_PARLAMENTAR, $perfil ) ){
						$disabled = 'onclick="document.getElementById(\'formulario\').submit();" style="cursor: pointer;" ';
					} else {
						$disabled =  ( !possuiPermissao('geral') ? 'disabled="disabled"' :  $respAutor["disabledEmendas"] );
					}
				?> 
			</td>
		</tr>
		<tr bgcolor="#d0d0d0">
			<td></td>
			<td>
				<input type="button" value="Salvar" <?=$disabled; ?> />
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">Detalhes da Emenda</td>
		</tr>
	</table>
</form>
<?php
	listaEmendasDetalhe( $emeid, '' );
?>