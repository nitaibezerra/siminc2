<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

$db2 = new cls_banco();

// Exclui o grupo
if ( !empty($_REQUEST['guoid']) && $_REQUEST['requisicao'] == 'excluir' ){
	
	// Atualiza as UOs
	$sql_atualiza_uo = "";
	$sql_atualiza_uo = "UPDATE dw.uo SET guoid = null
						WHERE guoid = {$_REQUEST['guoid']}";
	
	$db2->executar( $sql_atualiza_uo );
	
	// Exclui o grupo
	$sql_excluir = ""; 
	$sql_excluir = "DELETE FROM dw.grupouo 
					WHERE guoid = {$_REQUEST['guoid']}";
	
	$db2->executar( $sql_excluir );
	$db2->commit();
	$db2->sucesso('principal/consulta_grupos_unidade', 'A');
}

if ( $_REQUEST['submetido'] == 1 ){
	
		$filtro = array();
		
		if( $_REQUEST["guoid"] ){
			array_push($filtro, " gr.guoid = " . $_REQUEST['guoid'] . " ");
		}
			
		if($_REQUEST["guoabrev"]){
			array_push($filtro, " UPPER(gr.guoabrev) like UPPER('%" . $_REQUEST['guoabrev'] . "%') ");
		}	
		
		if($_REQUEST["guodsc"]){
			array_push($filtro, " UPPER(gr.guodsc) like UPPER('%" . $_REQUEST['guodsc'] . "%') ");
		}
		
		// Verifica se foi escolhido conta contábil
		if($_REQUEST['uo'][0]) {
			if( is_array($_REQUEST['uo']) ) {
				array_push($filtro, " gu. in ('".implode("','", $_REQUEST['uo'])."') ");
			}
		}

}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Módulo Financeiro', 'Gerenciamento Dinâmico de Grupos de Unidades Orçamentárias' );

?>

<script>

function alteraGrupo( cod ){
	location.href = '?modulo=principal/insere_grupos_unidade&acao=A&guoid='+cod;
}

function excluirGrupo( cod ){
 	if( confirm('Deseja realmente excluir este grupo?') ) {
		location.href = '?modulo=principal/consulta_grupos_unidade&acao=A&requisicao=excluir&guoid='+cod;
	}
}

</script>

<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="submetido" id="submetido" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita">Código do Grupo</td>
			<td>
				<?php
					echo campo_texto( 'guoid', 'N', 'S', '', 10, 10, '', '', 'left', '',0, 'id="guoid"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição Abreviada</td>
			<td>
				<?php
					echo campo_texto( 'guoabrev', 'N', 'S', '', 50, 100, '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição</td>
			<td>
				<?php
					echo campo_texto( 'guodsc', 'N', 'S', '', 50, 250, '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Orçamentária</td>
			<td>
				<?php
					$sql_uo = "";
					$sql_uo = "SELECT
									unicod as codigo,
									unidsc as descricao
								FROM
									dw.uo
								ORDER BY
									unidsc";
					combo_popup( 'uo', $sql_uo, 'Selecione a(s) Unidade(s) Orçamentária(s)', '400x400', 0, array(), '', 'S', true, false, 10, 400, null, null, $conexao );
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="submit" value="Consultar" style="cursor: pointer;"/>
				<input type="button" value="Ver Todos" style="cursor: pointer;" onclick="formulario.submit();"/>
				<input type="button" value="Inserir Grupo" style="cursor: pointer;" onclick="location.href='financeiro.php?modulo=principal/insere_grupos_unidade&acao=A';"/>
			</td>
		</tr>
	</table>
</form>
<?php

	$sql_grupos_cadastrados = "";
	$sql_grupos_cadastrados = " SELECT
									gr.guoid, 
									'<center><img style=\"cursor:pointer;\" title=\"Alterar Grupo\" align=\"absmiddle\" border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"alteraGrupo(' || gr.guoid || ')\">
						        	 <img style=\"cursor:pointer;\" title=\"Excluir Grupo\" align=\"absmiddle\" border=\"0\" src=\"../imagens/excluir.gif\" onclick=\"excluirGrupo(' || gr.guoid || ')\">
						        	</center>' AS acao,
						        	gr.guoabrev, 
						        	gr.guodsc,
						        	gu.unidsc 
							 	FROM 
							 		dw.grupouo  gr
							 	INNER JOIN
							 		dw.uo gu ON gr.guoid = gu.guoid
							 	WHERE 
							 		guostatus = 'A' " . (!empty($filtro) ? "AND" . implode(" AND ", $filtro) : '') . "
							 	ORDER BY
							 		gr.guoid";
	
	$dados = "";
	$dados = $db2->carregar( $sql_grupos_cadastrados ); 
	
	if( count($dados) > 1 ) {
		
		foreach( $dados as $val ) {
			if( $val['guoid'] != $guoidAtual ) {
	        	
				$z += isset($z) ? 1 : 0;
	        	
				$dados1[$z] = array("acao" 	   => $val['acao'], 
									"resumido" => $val['guoabrev'], 
									"inicio"   => $val['guodsc'], 
									"uos"   => "<div style='color: rgb(0, 102, 204);'>".$val['it_no_unidade_orcamentaria']."</div>");
					
	        	$guoidAtual = $val['guoid'];
	        	
	        } else {
	        	
	        	$dados1[$z]["uos"] .= "<div style='color: rgb(0, 102, 204);'>".$val['it_no_unidade_orcamentaria']."</div>";
	        	
	        }
		}
	}else {
		$dados1 = array();
	}
	
	$cabecalho = array( "Ação", "Descrição Abreviada", "Descrição", "Unidade" );
	$db2->monta_lista( $dados1, $cabecalho, 10, 20, 'N', '', '' );
	
?>