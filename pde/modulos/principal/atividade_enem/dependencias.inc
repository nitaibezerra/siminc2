<?php

include_once APPRAIZ . 'includes/workflow.php';

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_enem/arvore', 'A' );
}

function escreveDependencias($numero, $dados){
	
	global $db;
	
	$numero_original = $numero;
	$condicao = array();
	array_push( $condicao, " a._atinumero = '$numero' " );
	while( ( $posicao = strrpos( $numero, '.' ) ) !== false ) {
		$numero = substr( $numero, 0, $posicao );
		array_push( $condicao, " a._atinumero = '$numero' " );
	}
	if ( count( $condicao ) == 0 ) {
		return array();
	}
	$sql = "SELECT DISTINCT
				a._atinumero as numero,
				a._atiprofundidade as profundidade,
				a._atiirmaos as irmaos,
				a._atifilhos as filhos,
				a._atiordem as ordem,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				uni.unidsc,
				ug.ungdsc,
				a.atitipoenem
			FROM 
				pde.atividade a
			LEFT JOIN pde.enemdependencias edp ON edp.dependenciaatiid = a.atiid
			LEFT JOIN pde.estadoatividade ea ON ea.esaid = a.esaid
			LEFT JOIN pde.usuarioresponsabilidade ur ON ur.atiid = a.atiid AND ur.rpustatus = 'A' AND ur.pflcod = ".PERFIL_GERENTE." 
			LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf AND u.usustatus = 'A'
			LEFT JOIN public.unidade uni ON uni.unicod = u.unicod AND uni.unitpocod = 'U' AND uni.unistatus = 'A'
			LEFT JOIN public.unidadegestora ug ON ug.ungcod = u.ungcod AND ug.ungstatus = 'A'
			WHERE
				a._atiprojeto = ".PROJETO." AND
				a.atiidpai is not null AND
				a.atistatus = 'A' AND
				(edp.dependenciaatiid IS NOT NULL OR a.atiid = ".$dados['atiid'].") AND
				(a.atiidpai = (SELECT atiidpai FROM pde.atividade WHERE atiid = ".$dados['atiid'].") OR
				( ".implode( ' or ', $condicao )." ))
			ORDER BY a._atiordem";
//	ver($sql);
	$lista = $db->carregar( $sql );
	
	$rastro = array();
	foreach ( $lista as $indice => $item )
	{
	    
		// inicializa a variável
		$color = '';
		
		// seta a cor à partir do tipo
		if( $item['atitipoenem'] == 'E'):
			$color = '#000099';
			$tipo = '(E)';
		elseif( $item['atitipoenem'] == 'P'):
			$color = '#0033FF';
			$tipo = '(P)';
		elseif( $item['atitipoenem'] == 'S'):
			$color = '#006600';
			$tipo = '(S)';	
		elseif( $item['atitipoenem'] == 'A'):
			$color = '#009933';
			$tipo = '(A)';
		endif;
		
		
		if ( $dados['atiid'] == $item['atiid'] )
		{
			$htmUlt = sprintf(
			'<p style="margin: 0 0 5px %dpx;font-weight:%s;">%s<a style="color:'.$color.';" href="?modulo=%s&acao=%s&atiid=%d">%s %s %s</a></p>',
			$ant != $item['atitipoenem'] ? $indice * 20 + 15 : $ant2+15,
			$dados['atiid'] == $item['atiid'] ? 'bold' : 'normal',
			$indice != 0 && $ant != $item['atitipoenem'] && !($dados['atiid'] == $item['atiid']) && 1==0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
			$dados['modulo'],
			$item['atitipoenem'],
			$item['atiid'],
			$tipo,
			$item['numero'],
			$item['atidescricao']
			);
			$htm = '';
		}
		else
		{
			$htm = sprintf(
			'<p style="margin: 0 0 5px %dpx;font-weight:%s;">%s<a style="color:'.$color.';" href="?modulo=%s&acao=%s&atiid=%d">%s %s %s</a></p>',
			$ant != $item['atitipoenem'] ? $indice * 20 : $ant2+15,
			$dados['atiid'] == $item['atiid'] ? 'bold' : 'normal',
			$indice != 0 && $ant != $item['atitipoenem'] && 1==0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
			$dados['modulo'],
			$item['atitipoenem'],
			$item['atiid'],
			$tipo,
			$item['numero'],
			$item['atidescricao']
			);
		}
		array_push( $rastro, $htm );
		$ant2 = $ant != $item['atitipoenem'] ? $indice * 20 : $ant2 ;
		$ant = $item['atitipoenem'];
	}
	array_push( $rastro, $htmUlt );
	return sprintf( '<div style="margin: 5px">%s</div>', implode( '', $rastro ) );
}

function gravar($dados){
	
	global $db;
	
	$sql = "UPDATE pde.enemdependencias SET
				edpstatus = 'I'
			WHERE
				atiid = ".$dados['atiid'];
	
	$db->executar($sql);
	
	$sql = '';
	foreach($dados['atiid_'] as $atiid){
		
		$sql .= 'INSERT INTO pde.enemdependencias (atiid, dependenciaatiid)
				 VALUES ('.$dados['atiid'].','.$atiid.');';
	}
	
	if($sql != ''){
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/atividade_enem/dependencias', '&atiid='.$dados['atiid']);
	}
}

if( $_REQUEST['req'] ) {
	$_REQUEST['req']($_REQUEST);
}

$sql = "select
			to_char(atidatainicio, 'YYYYMMDD') as data_inicio,
			to_char((atidatainicio + atiduracao::integer), 'YYYYMMDD') as data_fim
		from
			pde.atividade
		where
			atiid = {$atividade['atiid']}
		and
			atistatus = 'A'";
$dataAtividade = $db->pegaLinha($sql);

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

if( temPerfilSomenteConsulta() )
{
	$permissao = false;
	$permissao_formulario = 'N';
}

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido
?>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function($)
{
		jQuery('#btSalvar').click(function(){
			selectAllOptions( document.getElementById('atiid_') );
			jQuery('#req').val('gravar');
			jQuery('#formDependencia').submit();
		});
	
});
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<?php if( $permissao ): ?>
				<form method="post" name="formDependencia" id="formDependencia" enctype="multipart/form-data">
					<input type="hidden" id="req" name="req" value=""/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
					<?php 
						$numero_original = $atividade;
						$condicao = array();
						array_push( $condicao, " a._atinumero = '$numero' " );
						while( ( $posicao = strrpos( $numero, '.' ) ) !== false ) {
							$numero = substr( $numero, 0, $posicao );
							array_push( $condicao, " a._atinumero = '$numero' " );
						}
						if ( count( $condicao ) == 0 ) {
							return array();
						}
						$sql = "SELECT DISTINCT
									codigo,
									descricao
								FROM
								(
								(SELECT
									a._atinumero||' - '||a.atidescricao  as descricao,
									a.atiid as codigo
								FROM pde.atividade a
								LEFT JOIN pde.estadoatividade ea ON ea.esaid = a.esaid
								LEFT JOIN pde.usuarioresponsabilidade ur ON ur.atiid = a.atiid AND ur.rpustatus = 'A' AND ur.pflcod = ".PERFIL_GERENTE." 
								LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf AND u.usustatus = 'A'
								LEFT JOIN public.unidade uni ON uni.unicod = u.unicod AND uni.unitpocod = 'U' AND uni.unistatus = 'A'
								LEFT JOIN public.unidadegestora ug ON ug.ungcod = u.ungcod AND ug.ungstatus = 'A'
								WHERE
									a._atiprojeto = ".PROJETO." AND
									a.atiidpai is not null AND
									a.atistatus = 'A' AND
									( ".implode( ' or ', $condicao )." )
								ORDER BY a._atiordem)
								UNION ALL
								(SELECT
									a._atinumero||' - '||a.atidescricao  as descricao,
									a.atiid as codigo
								FROM
									pde.atividade a
								WHERE
									a._atiprojeto = ".PROJETO." AND
									a.atistatus = 'A' AND
									atiidpai = (SELECT atiidpai FROM pde.atividade WHERE atiid = ".$_REQUEST['atiid'].") ) ) as foo
								WHERE
									codigo NOT IN (".$_REQUEST['atiid'].")";
						$stSqlCarregados = "SELECT
												a._atinumero||' - '||a.atidescricao  as descricao,
												a.atiid as codigo
											FROM
												pde.atividade a
											INNER JOIN pde.enemdependencias edp ON edp.dependenciaatiid = a.atiid
											WHERE
												edp.edpstatus = 'A' AND
												edp.atiid = ".$_REQUEST['atiid'];
						mostrarComboPopup( 'Dependências', 'atiid_',  $sql, $stSqlCarregados, 'Selecione a(s) Etapa(s)', Array(), '', '', '', '', '', '', false );
					?>
						<tr bgcolor="#cccccc">
							<td colspan="2" align="center">
								<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Cadastrar Dependências">
							</td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
			<?= escreveDependencias( $atividade['numero'], $_REQUEST ) ?>
		</td>
	</tr>
</table>