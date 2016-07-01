<?php

// carrega as funções de integração
require_once( APPRAIZ."monitora/modulos/principal/acao/atividade/acao_atividade_funcoes.inc" );

// carrega as funções do módulo pde
include APPRAIZ . "pde/www/_constantes.php";
include APPRAIZ . "pde/www/_funcoes.php";
include APPRAIZ . "pde/www/_componentes.php";

// obtém dados da atividade vinculada à ação
$atividade = retornaTarefaAcao( $_SESSION['acaid'], $_SESSION['exercicio'] );
$db->commit();
if ( $_REQUEST["atiid"] ) {
	$atividade = atividade_pegar( $_REQUEST["atiid"] );
}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
$_SESSION["projeto"] = $atividade["_atiprojeto"];
define( "PROJETO", $atividade["_atiprojeto"] );
projeto_verifica_selecionado( $atividade["atiid"] );

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido

?>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<?php
			
			$situacao = array(
				STATUS_NAO_INICIADO,
				STATUS_EM_ANDAMENTO,
				STATUS_SUSPENSO,
				STATUS_CANCELADO,
				STATUS_CONCLUIDO
			);
			$diretorio = $_SESSION["sisdiretorio"] . "/atividade";
			echo arvore( $atividade["atiid"], $profundidade, $situacao, null, $diretorio );
			
			?>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr style="background-color: #cccccc">	
		<td>
			<input type="button" name="botao" value="Incluir Plano Interno" onclick="javascript:pesqPlanoInterno();" style="visibility: <?php echo $visibleButton; ?>"/>
			<input type="hidden" name="atiid" value="<?php echo $_GET[atiid]; ?>">
		</td>
	</tr>		
</table>