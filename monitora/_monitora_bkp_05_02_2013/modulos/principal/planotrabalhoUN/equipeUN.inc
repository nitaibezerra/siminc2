<?php

// carrega as funções de integração
include "planotrabalhoUN/unidade_atividade_funcoes.php";
// carrega as funções do módulo pde
include "planotrabalhoUN/_constantes.php";
include "planotrabalhoUN/_funcoes.php";
include "planotrabalhoUN/_componentes.php";

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	header( "Location: monitora.php?modulo=principal/planotrabalhoUN/subatividadesUN&acao=A" );
	exit();
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid'] 
);

switch( $_REQUEST['evento'] ){

	case 'atribuir':
		$_REQUEST['apoio'] = array_diff( $_REQUEST['apoio'], $_REQUEST['gerente'] ); # impede que os usuários recebam atribuição dupla de responsabilidade
		atividade_atribuir_responsavel( $atividade['atiid'], PERFIL_GERENTE, $_REQUEST['gerente'] );
		atividade_atribuir_responsavel( $atividade['atiid'], PERFIL_EQUIPE_APOIO_GERENTE, $_REQUEST['apoio'] );
		$db->commit();
		//header( "Location: monitora.php?modulo=principal/planotrabalho/plano&acao=A" );
		print "<script>alert('Operação Realizada com Sucesso!');</script>";
		break;
	default:
		break;

}

// ----- VERIFICA PERMISSÃO
/*
$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário
$permissao_atribuir_gerente = $permissao_formulario;
$permissao_atribuir_apoio   = $permissao_formulario;
*/
$permissao = true;
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário
$permissao_atribuir_gerente = $permissao_formulario;
$permissao_atribuir_apoio   = $permissao_formulario;

$boPerfilSomenteLeitura = boPerfilSomenteLeitura();
$permissao_formulario = (!$boPerfilSomenteLeitura) ? 'S' : 'N'; # S habilita e N desabilita o formulário

$permissao = (!$boPerfilSomenteLeitura) ? true : false;

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );

echo montarAbasArray(carregardadosplanotrabalhoUN_sub(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/equipeUN&acao=A&atiid=".$_REQUEST['atiid']);

montar_titulo_projeto( $atividade['atidescricao'] );

/**
 * Captura todos os dados dos usuários com um perfil de uma atividade.
 * 
 * @return string[]
 */
function atividade_pegar_responsaveis( $atividade, $perfil ){
	global $db;
	$sql = <<<EOT
		select
			u.*
		from pde.usuarioresponsabilidade ur
			inner join seguranca.usuario u on
				u.usucpf = ur.usucpf
		where
			ur.rpustatus = 'A' and
			u.usustatus = 'A' and
			ur.atiid = %d and
			ur.pflcod = %d
EOT;
	$sql = sprintf( $sql, $atividade, $perfil );
	$usuarios = $db->carregar( $sql );
	return $usuarios ? $usuarios : array();
}



// captura dados de responsáveis por atividades superiores
$responsaveis_gerente_superiores = array();
$responsaveis_apoio_superiores = array();
$rastro = atividade_pegar_rastro( $atividade['numero'] );
array_pop( $rastro );
foreach ( $rastro as $item_rastro )
{
	$responsaveis_gerente = atividade_pegar_responsaveis( $item_rastro['atiid'], PERFIL_GERENTE );
	$responsaveis_apoio = atividade_pegar_responsaveis( $item_rastro['atiid'], PERFIL_EQUIPE_APOIO_GERENTE );
	foreach ( $responsaveis_gerente as $responsavel_gerente )
	{
		$novo_item = array(
			'numero_atividade' => $item_rastro['numero'],
			'nome_atividade'   => $item_rastro['atidescricao'],
			'nome_usuario'     => $responsavel_gerente['usunome'],
			'cpf_usuario'      => $responsavel_gerente['usucpf'],
			'atiid'            => $item_rastro['atiid']
		);
		array_push( $responsaveis_gerente_superiores, $novo_item );
	}
	foreach ( $responsaveis_apoio as $responsavel_apoio )
	{
		$novo_item = array(
			'numero_atividade' => $item_rastro['numero'],
			'nome_atividade'   => $item_rastro['atidescricao'],
			'nome_usuario'     => $responsavel_apoio['usunome'],
			'cpf_usuario'      => $responsavel_apoio['usucpf'],
			'atiid'            => $item_rastro['atiid']
		);
		array_push( $responsaveis_apoio_superiores, $novo_item );
	}
}

extract( $atividade ); # mantém o formulário preenchido

?>
<script language="javascript" type="text/javascript">
	function enviar(){
		selectAllOptions( formulario.gerente );
		selectAllOptions( formulario.apoio );
		document.formulario.submit();
	}
	
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
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
<tr>
<td>
<?= montar_resumo_atividade( $atividade ) ?>
</td>
</tr>
	<tr>
		<td>
			<?//= montar_resumo_atividade( $atividade ) ?>
			<form method="post" name="formulario">
				<input type="hidden" name="evento" value="atribuir"/>
				<input type="hidden" name="atiid" value="<?= $atiid ?>"/>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr style="background-color: #cccccc">
						<td align='left' colspan="2"><b>Responsável</b></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							Responsável:
						</td>
						<td>
							<?php
								$sql_gerente = "
									select
										u.usucpf as codigo,
										u.usucpf || ' - ' || u.usunome as descricao
									from seguranca.usuario u
										inner join pde.usuarioresponsabilidade ur on
											ur.usucpf = u.usucpf
										inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
									where
										ur.rpustatus = 'A' and
										ur.pflcod IN ('" . PERFIL_EQCOOACAO . "','".PERFIL_COORDACAO."') and
										atiid = " . $atividade['atiid'] . "
									order by u.usucpf
								";
								$gerente = $db->carregar( $sql_gerente );
								$gerente = $gerente ? $gerente : array();
								
								//$unicod = $db->pegaUm("select entunicod from entidade.entidade where entid='".$_SESSION['monitora_var']['entid']."'");
								$unicod = $_SESSION['monitora_var']['unicod'];
								$sql_combo = "
									select
										u.usucpf as codigo,
										u.usucpf || ' - ' || u.usunome as descricao
									from seguranca.usuario u
									inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
									left join monitora.usuarioresponsabilidade ur on
											ur.usucpf = u.usucpf  
									where
										( u.suscod = 'A' or u.suscod = 'P' ) and
										us.suscod = 'A' and
										us.sisid = ". $_SESSION['sisid'] ." and 
										ur.pflcod IN ('" . PERFIL_EQCOOACAO . "','".PERFIL_COORDACAO."') and 
										ur.unicod = '".$unicod."'
									group by u.usucpf, u.usunome"; 
								combo_popup(
									'gerente',
									$sql_combo,
									'Selecione o Gerente/Responsável pela Atividade',
									'400x400',
									1,
									array(),
									'',
									$permissao_atribuir_gerente,
									true,
									false,
									3
								);
							?>
						</td>
					</tr>
					<?php if ( count( $responsaveis_gerente_superiores ) ): ?>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
							Responsáveis por Atividades Superiores:
						</td>
						<td>
							<?php foreach ( $responsaveis_gerente_superiores as $responsavel ) : ?>
								<?= formatar_cpf( $responsavel['cpf_usuario'] ) ?>
								<?= $responsavel['nome_usuario'] ?>
								<b>:</b>
								<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $responsavel['atiid'] ?>">
									<?= $responsavel['numero_atividade'] ?>
									<?= $responsavel['nome_atividade'] ?>
								</a>
								<br/>
							<?php endforeach; ?>
						</td>
					</tr>
					<? endif; ?>
					<tr style="background-color: #cccccc">
						<td align='left' colspan="2"><b>Equipe de Apoio</b></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe de Apoio:</td>
						<td>
							<?php
								$sql_apoio = "
									select
										u.usucpf as codigo,
										u.usucpf || ' - ' || u.usunome as descricao
									from seguranca.usuario u
										inner join pde.usuarioresponsabilidade ur on
											ur.usucpf = u.usucpf
										inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
									where
										ur.rpustatus = 'A' and
										u.unicod = '{$unicod}' and
										ur.pflcod = '" . PERFIL_EQUIPE_APOIO_GERENTE . "' and
										atiid = " . $atividade['atiid'] . "
									order by u.usucpf
								";
								$apoio = $db->carregar( $sql_apoio );
								$apoio = $apoio ? $apoio : array();
								$sql_combo = "
									select
										u.usucpf as codigo,
										u.usucpf || ' - ' || u.usunome as descricao
									from seguranca.usuario u
									inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
									inner join monitora.usuarioresponsabilidade ur on ur.usucpf = u.usucpf  
									where
										( u.suscod = 'A' or u.suscod = 'P' ) and
										us.suscod = 'A' and
										u.unicod = '{$unicod}' and
										us.sisid = ". $_SESSION['sisid'] ."
										and us.pflcod IN ( ".PERFIL_GESTORUNIDPLANEJAM.", 
														   ".PERFIL_EQUIPAPOIOGESTORUP.", 
														   ".PERFIL_MONITORA_GESTORUNIDORCAMENTO.", 
														   ".PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO." )
										and ur.rpustatus = 'A'
									group by u.usucpf, u.usunome";
								combo_popup(
									'apoio',
									$sql_combo,
									'Selecione o(s) Identificador(es) de Uso',
									'400x400',
									0,
									array(),
									'',
									$permissao_atribuir_apoio,
									true
								);
							?>
						</td>
					</tr>
					<?php if ( count( $responsaveis_apoio_superiores ) ): ?>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe de Apoio de Atividades Superiores:</td>
						<td>
							<?php foreach ( $responsaveis_apoio_superiores as $responsavel ) : ?>
								<?= formatar_cpf( $responsavel['cpf_usuario'] ) ?>
								<?= $responsavel['nome_usuario'] ?>
								<b>:</b>
								<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $responsavel['atiid'] ?>">
									<?= $responsavel['numero_atividade'] ?>
									<?= $responsavel['nome_atividade'] ?>
								</a>
								<br/>
							<?php endforeach; ?>
						</td>
					</tr>
					<? endif; ?>
					<?php if( $permissao ): ?>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="enviar();"/></td>
						</tr>
					<?php endif; ?>
				</table>
			</form>
		</td>
	</tr>
</table>