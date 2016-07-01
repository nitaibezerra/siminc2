<?php

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
//cte_montaTitulo( $titulo_modulo, '' );


if( $_REQUEST['madbloqueio'] && cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,CTE_PERFIL_ADMINISTRADOR)) ){
	
	$madbloqueio = $_REQUEST['madbloqueio']['0'] == 'Sim' ? 'TRUE' : 'FALSE';
	$madestados  = is_array($_REQUEST['estuf']) ? implode($_REQUEST['estuf'],",") : '';
	if( $_REQUEST['madid'] ){
		$sql = "UPDATE 
					cte.monitoramentoadministracao
				SET 
					madalerta = '{$_REQUEST['madalerta']}', 
					madbloqueio = {$madbloqueio},
					madestados = '{$madestados}'
				WHERE 
					madid = 1";
	}else{
		$sql = "INSERT INTO 
					cte.monitoramentoadministracao(madalerta,madbloqueio,madestados)
				VALUES 
					('{$_REQUEST['madalerta']}',{$madbloqueio},'{$madestados}')";
	}
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Dados salvos com sucesso.');</script>";
}

$dados['madestados'] = array();

$sql = "SELECT 
			madid, 
			madalerta, 
			madbloqueio,
			madestados
  		FROM 
  			cte.monitoramentoadministracao
  		WHERE
  			madid = 1
  		LIMIT 1";

$dados = $db->pegaLinha($sql);

$ufsMarcados = explode(",",$dados['madestados']);
$qtdUfsMarcados = count($ufsMarcados);
if( $qtdUfsMarcados == 27 ){
	$todos = 'checked="checked"';
}

if( $dados['madbloqueio'] == 't' ){
	$bloqueio1 = 'checked="checked"';
	$bloqueio2 = '';
}else{
	$bloqueio1 = '';
	$bloqueio2 = 'checked="checked"';
}
?>
<form id="form" method="POST">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<input type="hidden" name="madid" value="<?=$dados['madid'] ?>">
	<tbody>
		<tr>
			<td class='SubTituloDireita' width="10%">
				Alerta do Sistema:
			</td>
			<td>
				<?php
				echo campo_textarea( 'madalerta', 'N', 'S', '', '68', '6', '1000', '' , 0, '', '', '', $dados['madalerta'] ); 
				
				?>
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
				Bloqueio de sistema:
			</td>
			<td>
				<input type="radio" name="madbloqueio[]" value="Sim" <?=$bloqueio1 ?>> Sim
				<input type="radio" name="madbloqueio[]" value="Não" <?=$bloqueio2 ?>> Não
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
				UFs bloqueadas:
			</td>
			<td>
				<?php 
				
				$sql = "SELECT 
							estuf as codigo,
							estuf as descricao
						FROM 
							territorios.estado";
				
				$db->monta_checkbox("estuf[]", $sql, $ufsMarcados, $separador='  '); 
				
				?>
				<br />
				<input type="checkbox" name="todos" id="todos" value="Todos" onclick="marcaTodos(this);" <?=$todos ?>/> Todos
			</td>
		</tr>
		<tr>
			<td class='SubTituloDireita' width="10%">
			</td>
			<td>
				<input type="button" id="btnGravar" name="btnGravar" value="Gravar" onclick="validaForm()"/>
			</td>
		</tr>
	</tbody>
</table>
<script>
function marcaTodos( todos ){
	var estufs = document.getElementsByName('estuf[]');
	
	for( i=0; i<estufs.length; i++ ){
		if( todos.checked ){
			estufs[i].checked = true;
		}else{
			estufs[i].checked = false;
		}
	}
}

function validaForm(){
	var madalerta = document.getElementById('madalerta');

//	if( madalerta.lenght == 0 ){
//		alert('Campo obrigatório.');
//		madalerta.focus();
//		return false;
//	}
	document.getElementById('form').submit();
}
</script>
</form>




