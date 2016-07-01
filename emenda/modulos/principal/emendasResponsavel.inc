<?php

if( !$_SESSION["emenda"]["resid"] ){
	echo "<script>
			alert('Faltam dados na sessão sobre este responsavel.');
			history.back(-1);
		  </script>";
	die;	
}


if ( $_REQUEST["subacao"] == "listasubfuncao" ){
	
	if ( $_REQUEST["funcod"] ){
		
		$sfucod = $_REQUEST["sfucod"];
		
		$sql = "SELECT DISTINCT 
					sfu.sfucod as codigo, 
					sfu.sfucod || ' - ' || sfu.sfudsc as descricao
				FROM 
					emenda.emenda e
				INNER JOIN 
					emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
				INNER JOIN 
					public.ppasubfuncao sfu ON sfu.funcod = sfu.funcod
				WHERE 
					e.resid = {$_SESSION["emenda"]["resid"]} AND 
					f.prgano = '{$_SESSION["exercicio"]}' AND 
					sfu.funcod = '{$_REQUEST["funcod"]}'
				ORDER BY
					descricao";
		
		$db->monta_combo("sfucod", $sql, "S", "Todas", '', '', '', '250', 'N', 'sfucod');
	}else{
		print 'Selecione uma função...';
	}
	die;
	
}

if ( $_REQUEST["pesquisa"] == "pesquisa" ){
	$filtros = geraFiltroResponsavel( $_REQUEST );
}

if ( $_REQUEST["requisicao"] == "excluir" ){
	excluirVinculoResponsavel( $_REQUEST["emeid"] );
}

$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"] );
	
if($residPossiveis) {
	if ( count($residPossiveis) > 1 ){
		foreach( $residPossiveis as $chave=>$valor ){
			if ( $valor == $_SESSION["emenda"]["resid"]){
				$possivelEditar = true;
				break;
			}else{
				$possivelEditar = false;
			}
		}
	}else{
		$possivelEditar = ($residPossiveis[0] == $_SESSION["emenda"]["resid"]) ? true : false;
	}
} else {
	$possivelEditar = true;
}

require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, '');

// monta o cabeçalho com os dados do responsável
cabecalhoResponsavel( $_SESSION["emenda"]["resid"] );

?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>

<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="pesquisa" name="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="180px">Exercício:</td>
			<td>
				<?php print $_SESSION["exercicio"]; ?>
				<input type="hidden" name="exercicio" id="exercicio" value="<?php print $_SESSION["exercicio"]; ?>"/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Autor da Emenda</td>
			<td>
				<?php
					
					$sql = "SELECT DISTINCT 
								a.autid as codigo,
								a.autnome as descricao
							FROM 
								emenda.autor a
							INNER JOIN 
								emenda.emenda e ON e.autid = a.autid
							INNER JOIN 
								emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							WHERE 
								e.resid = {$_SESSION["emenda"]["resid"]} AND 
								f.prgano = '{$_SESSION["exercicio"]}'
							ORDER BY
								descricao";
					
					$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '250', 'N','autid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ação Orçamentária</td>
			<td>
				<?php
					
					$acacod = $_REQUEST["acacod"];
				
					$sql = "SELECT DISTINCT 
								f.acacod as codigo, 
								f.acacod || ' - ' || f.acadsc as descricao
							FROM
								emenda.emenda e
							INNER JOIN 
								emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							WHERE 
								e.resid = {$_SESSION["emenda"]["resid"]} AND 
								f.prgano = '{$_SESSION["exercicio"]}'
							ORDER BY
								descricao";
					
					$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '250', 'N','acacod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Função</td>
			<td>
				<?php
					
					$funcod = $_REQUEST["funcod"];
				
					$sql = "SELECT DISTINCT 
								fu.funcod as codigo, 
								fu.funcod || ' - ' || fu.fundsc as descricao
							FROM 
								emenda.emenda e
							INNER JOIN 
								emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							INNER JOIN 
								public.ppafuncao fu ON fu.funcod = f.funcod
							WHERE 
								e.resid = {$_SESSION["emenda"]["resid"]} AND 
								f.prgano = '{$_SESSION["exercicio"]}'
							ORDER BY
								descricao";
				
					$db->monta_combo("funcod", $sql, "S", "Todas", "listaSubfuncao(this.value, 'normal', '');", '', '', '250', 'N','funcod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Subfunção</td>
			<td id="subfuncao" style="color: #C0C0C0;"> 
				Selecione uma função...
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Pesquisar"/>
			</td>
		</tr>
		<?php if ( $possivelEditar && possuiPermissao() ): ?>
		<tr>
			<td colspan="2" align="left">
				<a href="#" onclick="inserirEmendaResponsavel();">
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" align="absmiddle" border="0" title="Inserir Contatos"> 
					Vincular Emendas ao Responsável
				</a>
			</td>
		</tr>
		<?php endif; ?>
		<tr>
			<td colspan="2" class="subtitulocentro">Emendas do Responsável</td>
		</tr>
	</table>
</form>
<?php

	listaEmendasResponsavel( $filtros, 'resp', $possivelEditar );

	if ( $funcod && $_REQUEST["sfucod"] ): ?>
		<script>
			listaSubfuncao(<?php echo $funcod; ?>, 'popup', <?php echo $_REQUEST["sfucod"]; ?>);
		</script>
<?php endif; ?>